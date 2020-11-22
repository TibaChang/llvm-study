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
    BasicBlock *LBody, *LInc, *LCond;
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
    // get Loop.Inc from Loop.Body->Terminator
    Instruction *BodyTerm = LBody->getTerminator();
    LInc = dyn_cast<BasicBlock>(BodyTerm->getOperand(0));
    // get Loop.Inc from Loop.Inc->Terminator
    Instruction *IncTerm = LInc->getTerminator();
    LCond = dyn_cast<BasicBlock>(IncTerm->getOperand(0));
    if (!LBody || !LInc || !LCond) {
      return false;
    } else {
        errs() << "Cond-> " << *LCond << "\n";
        errs() << "Body-> " << *LBody << "\n";
        errs() << "Inc-> " << *LInc << "\n";
    }
    errs() << "\n----------------Start Impl.------------------\n\n";
    const unsigned int InterleaveFactor = 4;
    // use loop.inc to modify the iter.
    ConstantInt *ConstIntVar = NULL;
    Value *opVal = NULL;
    int i = 0;
    for (Instruction &Inst : *LInc) {
      if (Inst.getOpcode() == Instruction::Add) {
        for (auto operand = Inst.operands().begin();
                        operand != Inst.operands().end(); ++operand) {
          if (dyn_cast<ConstantInt>(operand)) {
            ConstIntVar = dyn_cast<ConstantInt>(operand);
            if (i == 1) {
              opVal = dyn_cast<Value>((Inst.getOperand(0)));
            } else {
              opVal = dyn_cast<Value>((Inst.getOperand(1)));
            }
          }
          i++;
        }
        if (ConstIntVar) {
          uint64_t Num = ConstIntVar->getLimitedValue(~0U) * InterleaveFactor;
          Instruction *NewIter = BinaryOperator::CreateAdd(opVal, ConstantInt::get(ConstIntVar->getType(), Num));
          //errs() << "New instr. -> " << *NewIter << "\n";
          ReplaceInstWithInst(&Inst, NewIter);
          break;
        }
      }
    }
    //errs() << "New loop.inc -> " << *LInc << "\n";
    unsigned long long NumOfIter;
    for (Instruction &Inst : *LCond) {
      CmpInst *Cmp = dyn_cast<CmpInst>(&Inst);
      if (Cmp) {
        for (auto operand = Inst.operands().begin();
                        operand != Inst.operands().end(); ++operand) {
          if (dyn_cast<ConstantInt>(operand)) {
            ConstIntVar = dyn_cast<ConstantInt>(operand);
            NumOfIter = ConstIntVar->getLimitedValue(~0U);
            break;
          }
        }
      }
    }
    errs() << "Iter = " << NumOfIter << "\n";
    // calc the size of tmp for $b
    int tmpVecStoreSize = NumOfIter / InterleaveFactor;
    //TODO

    // The bit cast is done as follows
#if 0
    // vectorize loop body w/ bitcast
    for (Instruction &Inst : *LBody) {
      if (Inst.getOpcode() == Instruction::GetElementPtr) {
        // TODO: vectorize body in LBody
        // bitcast?
        IRBuilder<> Builder(&Inst);
        GetElementPtrInst *Gep = cast<GetElementPtrInst>(&Inst);
        unsigned int Addr = Gep->getType()->getPointerAddressSpace();
        Type *ScalarTy = Gep->getType()->getPointerElementType();
        //Type *ScalarTy = (cast<GetElementPtrInst>(prevInst))->getType()->getPointerElementType();
        errs() << "Scalar-> " << *ScalarTy << "\n";
        Type *VecTy = VectorType::get(ScalarTy, InterleaveFactor);
        Type *PtrTy = VecTy->getPointerTo(Addr);
        Value * Bitcast = Builder.CreateBitCast(Gep, PtrTy);
        errs() << "Get type:" << *Bitcast << "\n";
        errs() << "New Body-> " << *LBody << "\n";
        break;
      }
    }
#endif

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
