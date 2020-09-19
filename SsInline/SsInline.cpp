#include "SsInline.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Transforms/Utils/ValueMapper.h"


using namespace llvm;

#define DEBUG_TYPE "Ss-Inline"

//-----------------------------------------------------------------------------
// SsInline implementation
//-----------------------------------------------------------------------------

bool SsInline::inlineOnce(Function &Caller) {
  Value *RetVal;
  CallInst *call;
  ValueToValueMapTy vmap;
  std::vector<Value *>ArgsToReplace;
  std::vector<Value *>UsedVarAsArgs;
  for (auto &BB : Caller) {
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
      LLVM_DEBUG(dbgs() << "Found call: " << DirectInvoc->getName() << " w/ [" << Ins << "]\n");
      if (call = dyn_cast<CallInst>(&Ins)) {
        // get where the arg comes from for the function, which will be used to replace the arg in function when inline
        for (auto& Arg : DirectInvoc->args()) {
          LLVM_DEBUG(dbgs() << "Func call src -> " << Arg << "\n");
          ArgsToReplace.push_back(const_cast<Argument *>(&Arg));
        }
        // 1. clone the instr. in the function
        for (auto &BB : *DirectInvoc) {
          for (auto &Ins : BB) {
            Instruction *cloneIns = Ins.clone();
            LLVM_DEBUG(dbgs() << "-----------Clone Inst: " << *cloneIns << "\n");
            int arg_idx = 0;
            for(auto Arg : ArgsToReplace) {
              // 2. replace the parameter w/ arg passed in
              LLVM_DEBUG(dbgs() << "Arg: " << *Arg << "\n");
              int op_idx = 0;
              for (User::op_iterator op = cloneIns->op_begin(), e = cloneIns->op_end(); op != e; ++op) {
                LLVM_DEBUG(dbgs() << "Op[" << op_idx << "]: "<< **op << "\n");
                if (cloneIns->getOperand(op_idx) == Arg) {
                  LLVM_DEBUG(dbgs() << "Replace: " << **op << " -> " << *(call->getArgOperand(op_idx)) << "\n");
                  cloneIns->setOperand(op_idx, call->getArgOperand(arg_idx));
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
              LLVM_DEBUG(dbgs() << "Func ret val: " << *RetVal << "\n");
              // remove the ret
              cloneIns->eraseFromParent();
            }
          }
        }
        // 3. replace call related op
        LLVM_DEBUG(dbgs() << "Call: " << *call << "\n");
        for (auto user : call->users()) {
          LLVM_DEBUG(dbgs() << "Call's user: " << *user << "\n");
          int i = -1;
          // try to find which operand is the call
          for (auto operand = user->operands().begin();
              operand != user->operands().end(); ++operand) {
            LLVM_DEBUG(dbgs() << "Op of Call's user -> " << **operand << "\n");
            i++;
            if (*operand == call) {
              LLVM_DEBUG(dbgs() << "Found the call to remove.\n");
              break;
            }
          }
          // replace returned val w/ real value
          user->setOperand(i, RetVal);
        }
        // 4. remove call
        call->eraseFromParent();
        // 5. show results
        LLVM_DEBUG(dbgs() << Caller << "\n");
        return true;
      }
    }
  }
  return false;
}


bool SsInline::runOnModule(Module &M) {
  bool Changed = false;

  auto &CTX = M.getContext();

  Function *Caller;
  Function *Callee;
  std::vector<Function *> AllCallee;
  for (auto &F : M) {
    if (F.isDeclaration())
      continue;
    if (F.getName() == "main") {
      //FIXME: currently, we only inline to the caller "main"
      for (auto &BB : F) {
        for (auto &Instr : BB) {
          if (CallInst *callInst = dyn_cast<CallInst>(&Instr)) {
            if (Function *calledFunction = callInst->getCalledFunction()) {
                Caller = &F;
            }
          }
        }
      }
    }
  }

  // inline a call once
  while (inlineOnce(*Caller)) {
    Changed = true;
  }

  return Changed;
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
