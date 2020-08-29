#include "SsInline.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Transforms/Utils/ValueMapper.h"


using namespace llvm;

#define DEBUG_TYPE "Ss-Inline"

Constant *CreateGlobalCounter(Module &M, StringRef GlobalVarName) {
  auto &CTX = M.getContext();

  // This will insert a declaration into M
  Constant *NewGlobalVar =
      M.getOrInsertGlobal(GlobalVarName, IntegerType::getInt32Ty(CTX));

  // This will change the declaration into definition (and initialise to 0)
  GlobalVariable *NewGV = M.getNamedGlobal(GlobalVarName);
  NewGV->setLinkage(GlobalValue::CommonLinkage);
  NewGV->setAlignment(MaybeAlign(4));
  NewGV->setInitializer(llvm::ConstantInt::get(CTX, APInt(32, 0)));

  return NewGlobalVar;
}

//-----------------------------------------------------------------------------
// SsInline implementation
//-----------------------------------------------------------------------------
bool SsInline::runOnModule(Module &M) {
  bool Instrumented = false;

  auto &CTX = M.getContext();

  Function *Caller;
  Function *Callee;
  for (auto &F : M) {
    if (F.isDeclaration())
      continue;

    for (auto &BB : F) {
      if (F.getName() == "main") {
        //FIXME: currently, we only process the caller "main"
        for (auto &Instr : BB) {
          if (CallInst *callInst = dyn_cast<CallInst>(&Instr)) {
            if (Function *calledFunction = callInst->getCalledFunction()) {
                Caller = &F;
                Callee = calledFunction;
                // TODO: process multiple callee
                LLVM_DEBUG(dbgs() << "Caller: " << Caller->getName() << "; Callee: " << Callee->getName() << "\n");
            }
          }
        }
      }
    }
  }

  Value *RetVal;
  CallInst *call;
  ValueToValueMapTy vmap;
  std::vector<Value *>ArgsToReplace;
  std::vector<Value *>UsedVarAsArgs;
  for (auto &BB : *Caller) {
    for (auto &Ins : BB) {
      // As per the comments in CallSite.h (more specifically, comments for
      // the base class CallSiteBase), ImmutableCallSite constructor creates
      // a valid call-site or NULL for something which is NOT a call site.
      auto ICS = ImmutableCallSite(&Ins);
      if (nullptr == ICS.getInstruction()) {
        continue;
      }

      // Check whether the called function is directly invoked
      auto DirectInvoc =
          dyn_cast<Function>(ICS.getCalledValue()->stripPointerCasts());
      if (nullptr == DirectInvoc) {
        continue;
      }
      std::vector<Instruction *> cloneIns;
      // Variables to keep track of the new bindings
      if (Callee->getName() == DirectInvoc->getName()) {
        errs() << "Found call: " << DirectInvoc->getName() << " w/ [" << Ins << "]\n";
        if (call = dyn_cast<CallInst>(&Ins)) {
          // get where the arg comes from for the function, which will be used to replace the arg in function when inline
          for (auto& Arg : DirectInvoc->args()) {
            errs() << "Func call src -> " << Arg << "\n";
            ArgsToReplace.push_back(const_cast<Argument *>(&Arg));
          }
          // 1. clone the instr. in the function
          for (auto &BB : *DirectInvoc) {
            for (auto &Ins : BB) {
              Instruction *cloneIns = Ins.clone();
              errs() << "-----------Clone Inst: " << *cloneIns << "\n";
              int arg_idx = 0;
              for(auto Arg : ArgsToReplace) {
                // 2. replace the parameter w/ arg passed in
                errs() << "Arg: " << *Arg << "\n";
                int op_idx = 0;
                for (User::op_iterator op = cloneIns->op_begin(), e = cloneIns->op_end(); op != e; ++op) {
                  errs() << "Op[" << op_idx << "]: "<< **op << "\n";
                  if (cloneIns->getOperand(op_idx) == Arg) {
                    errs() << "Replace: " << **op << " -> " << *(call->getArgOperand(op_idx)) << "\n";
                    cloneIns->setOperand(op_idx, call->getArgOperand(arg_idx));//FIXME
                  }
                  op_idx++;
                }
                arg_idx++;
              }
              cloneIns->insertBefore(call);
              vmap[&Ins] = cloneIns;
              RemapInstruction(cloneIns, vmap, RF_NoModuleLevelChanges | RF_IgnoreMissingLocals);
              if (Ins.isTerminator()) {
                RetVal = cloneIns->getOperand(0);
                errs() << "Func ret val: " << *RetVal << "\n";
                // remove the ret
                cloneIns->eraseFromParent();
              }
            }
          }
          // 3. replace call related op
          errs() << "Call: " << *call << "\n";
          for (auto user : call->users()) {
            errs() << "Call's user: " << *user << "\n";// replace the op
            int i = -1;
            // try to find which operand is the call
            for (auto operand = user->operands().begin();
                operand != user->operands().end(); ++operand) {
              errs() << "Op of Call's user -> " << **operand << "\n";
              i++;
              if (*operand == call) {
                errs() << "Found the call\n";
                break;
              }
            }
            // replace returned val w/ real value
            user->setOperand(i, RetVal);
          }
        }
      }
    }
  }
  // 4. remove call
  call->eraseFromParent();
  // show results
  errs() << *Caller << "\n";

  return true;
}

PreservedAnalyses SsInline::run(llvm::Module &M,
                                          llvm::ModuleAnalysisManager &) {
  bool Changed = runOnModule(M);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}

bool LegacySsInline::runOnModule(llvm::Module &M) {
  bool Changed = Impl.runOnModule(M);

  return Changed;
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getSsInlinePluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SsInline", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "SsInline") {
                    MPM.addPass(SsInline());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getSsInlinePluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
char LegacySsInline::ID = 0;

// Register the pass - required for (among others) opt
static RegisterPass<LegacySsInline>
    X(/*PassArg=*/"legacy-SsInline",
      /*Name=*/"LegacySsInline",
      /*CFGOnly=*/false,
      /*is_analysis=*/false);
