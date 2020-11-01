#include "SsLoopVec.h"

#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"


using namespace std;
using namespace llvm;

#define DEBUG_TYPE "SsLoopVec"

//-----------------------------------------------------------------------------
// SsLoopVec implementation
//-----------------------------------------------------------------------------

bool SsLoopVec::DoImpl(llvm::Function &F) {
  return true;
}


PreservedAnalyses SsLoopVec::run(llvm::Function &F,
                                          llvm::FunctionAnalysisManager &FAM) {
  bool Changed = DoImpl(F);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}

bool LegacySsLoopVec::runOnFunction(llvm::Function &F) {
  bool Changed = Impl.DoImpl(F);

  return Changed;
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getSsLoopVecPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SsLoopVec", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "SsLoopVec") {
                    FPM.addPass(SsLoopVec());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getSsLoopVecPluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
char LegacySsLoopVec::ID = 0;

// Register the pass - required for (among others) opt
static RegisterPass<LegacySsLoopVec>
    X(/*PassArg=*/"legacy-SsLoopVec",
      /*Name=*/"LegacySsLoopVec",
      /*CFGOnly=*/false,
      /*is_analysis=*/false);
