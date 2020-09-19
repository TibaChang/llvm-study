#include "SsSCCP.h"

#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Dominators.h"


using namespace llvm;

#define DEBUG_TYPE "SsSCCP"

//-----------------------------------------------------------------------------
// SsSCCP implementation
//-----------------------------------------------------------------------------


bool SsSCCP::DoSCCP(Function &F, DominatorTree *DT) {
  bool Changed = false;

  if (DT == nullptr) {
    errs() << "DT is nullptr\n";
    return false;
  }

  //FIXME
  if (F.getName() != "foo") {
    return false;
  }

  //TODO
  for (auto node = GraphTraits<DominatorTree *>::nodes_begin(DT);
       node != GraphTraits<DominatorTree *>::nodes_end(DT); ++node) {
    BasicBlock *BB = node->getBlock();
    // whatever you want to do with BB
    errs() << "BB-> " << BB->getName() << "\n";
  }


  return Changed;
}

PreservedAnalyses SsSCCP::run(llvm::Function &F,
                                          llvm::FunctionAnalysisManager &FAM) {
  DominatorTree *DT = &FAM.getResult<DominatorTreeAnalysis>(F);
  bool Changed = DoSCCP(F, DT);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}

bool LegacySsSCCP::runOnFunction(llvm::Function &F) {
  DominatorTree DT(F);
  bool Changed = Impl.DoSCCP(F, &DT);

  return Changed;
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getSsSCCPPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SsSCCP", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "SsSCCP") {
                    FPM.addPass(SsSCCP());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getSsSCCPPluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
char LegacySsSCCP::ID = 0;

// Register the pass - required for (among others) opt
static RegisterPass<LegacySsSCCP>
    X(/*PassArg=*/"legacy-SsSCCP",
      /*Name=*/"LegacySsSCCP",
      /*CFGOnly=*/false,
      /*is_analysis=*/false);
