#include "SsInline.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

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

  // Function name <--> IR variable that holds the call counter
  llvm::StringMap<Constant *> CallCounterMap;
  // Function name <--> IR variable that holds the function name
  llvm::StringMap<Constant *> FuncNameMap;

  auto &CTX = M.getContext();

  Function *Caller;
  Function *Callee;
  for (auto &F : M) {
    if (F.isDeclaration())
      continue;

    for (auto &BB : F) {
      for (auto &Instr : BB) {
        if (CallInst *callInst = dyn_cast<CallInst>(&Instr)) {
          if (Function *calledFunction = callInst->getCalledFunction()) {
            if (calledFunction->getName().startswith("foo")) {
              Caller = &F;
              Callee = calledFunction;
              LLVM_DEBUG(dbgs() << "Caller: " << Caller->getName() << "; Callee: " << Callee->getName() << "\n");
              //TODO: a function can be callee for multiple caller
            }
          }
        }
      }
    }
  }
  LLVM_DEBUG(dbgs() << "Tmp Caller: " << Caller->getName() << "; Tmp Callee: " << Callee->getName() << "\n");

  //print callee  FIXME: remove me
  for (auto &BB : *Callee) {
    for (auto Inst = BB.begin(), IE = BB.end(); Inst != IE; ++Inst) {
      Instruction* ii = &*Inst;
      LLVM_DEBUG(dbgs() << *ii << "\n");
    }
  }

  // TODO: find the position to call callee(foo)
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
      if (Callee->getName() == DirectInvoc->getName()) {
        errs() << "Found target: " << DirectInvoc->getName() << "\n";
        //TODO: remove the call related instr.
        errs() << *DirectInvoc << "---\n";
      }
    }
  }

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
