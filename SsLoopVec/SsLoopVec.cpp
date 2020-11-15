#include "SsLoopVec.h"

#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"


using namespace std;
using namespace llvm;

#define DEBUG_TYPE "SsLoopVec"

//-----------------------------------------------------------------------------
// SsLoopVec implementation
//-----------------------------------------------------------------------------

bool SsLoopVec::DoImpl(llvm::Function &F, llvm::LoopInfo &LI) {
  //FIXME: currently only run on a specific function for development.
  if (F.getName() != "foo") {
    return false;
  }

  SmallVector<Loop *, 16> PreorderLoops = LI.getLoopsInPreorder();
  do {
    Loop &L = *PreorderLoops.pop_back_val();
    BasicBlock *Header = L.getHeader();
    BasicBlock *LBody, *LInc;
    //errs() << "Header-> \n" << *Header << "\n";
    // get the term. of header (2 candicates)
    Instruction *HeaderTerm = Header->getTerminator();
    for (auto operand = HeaderTerm->operands().begin();
                    operand != HeaderTerm->operands().end(); ++operand) {
      // if it is not loop.end
      if ((L.getExitBlock() != *operand) && isa<BasicBlock>(operand)) {
        // it is loop body
        LBody = dyn_cast<BasicBlock>(*operand);
      }
    }
    Instruction *BodyTerm = LBody->getTerminator();
    LInc = dyn_cast<BasicBlock>(BodyTerm->getOperand(0));
    if (!LBody || !LInc) {
      return false;
    } else {
        errs() << "Body-> " << *LBody << "\n";
        errs() << "Inc-> " << *LInc << "\n";
    }
    // TODO: vectorize body in LBody
    // TODO: modify the step in LInc

  } while (!PreorderLoops.empty());
  return true;
}


PreservedAnalyses SsLoopVec::run(llvm::Function &F,
                                          llvm::FunctionAnalysisManager &FAM) {
  LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
  bool Changed = DoImpl(F, LI);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}

bool LegacySsLoopVec::runOnFunction(llvm::Function &F) {
  DominatorTree DT(F);
  LoopInfo LI(DT);
  bool Changed = Impl.DoImpl(F, LI);

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
