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
    BasicBlock *LBody, *LInc, *LCond, *LEnd;
    LEnd = L.getExitBlock();
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
    // Assume the unroll factor is 1
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

    Type *ScalarTy, *VecTy, *PtrTy;
    GetElementPtrInst *Gep;
    LoadInst *LoadCast;
    for (Instruction &Inst : *LBody) {
      if (Inst.getOpcode() == Instruction::GetElementPtr) {
        IRBuilder<> Builder(&Inst);
        Gep = cast<GetElementPtrInst>(&Inst);
        unsigned int Addr = Gep->getType()->getPointerAddressSpace();
        ScalarTy = Gep->getType()->getPointerElementType();
        VecTy = VectorType::get(ScalarTy, InterleaveFactor);
        PtrTy = VecTy->getPointerTo(Addr);
        Value *GepUser;
        int op_cnt = 0;
        for (auto user : Gep->users()) {
          errs() << "gep user->" << *user << "\n";
          GepUser = user;
          for (auto operand = user->operands().begin();
            operand != user->operands().end(); ++operand) {
            if (*operand == Gep) {
              errs() << "Found gep\n";
              // create bitcast
              BitCastInst *Bitcast = new BitCastInst(Gep, PtrTy);
              Bitcast->insertAfter(Gep);
              if (Instruction *OldLoad = dyn_cast<LoadInst>(user)) {
                // change the load from Gep to bitcast
                LoadCast = new LoadInst(Bitcast);
                LoadCast->insertAfter(Bitcast);
                break;
              }
            }
            op_cnt++;
          }
        }
        break;
      }
    }
    // create alloca for b[4]
    IRBuilder<> AllocaBuilder(F.getEntryBlock().getFirstNonPHI());
    AllocaInst *AllocaTmp;
    AllocaTmp = AllocaBuilder.CreateAlloca(VecTy);
    StoreInst *StoreTmp = new StoreInst((Value*)(ConstantInt::get(VecTy, 0)), AllocaTmp);
    StoreTmp->insertAfter(AllocaTmp);
    errs() << "New F Entry-> " << (F.getEntryBlock()) << "\n";
    // Load vec AllocaTmp as LoadAllocaTmp
    Instruction *LoadAllocaTmp = new LoadInst(AllocaTmp);
    LoadAllocaTmp->insertAfter(LoadCast);
    // add LoadCast & LoadAllocaTmp as VecTmpSum
    Instruction *VecTmpSum = BinaryOperator::CreateAdd(LoadAllocaTmp, LoadCast);
    VecTmpSum->insertAfter(LoadAllocaTmp);
    // store VecTmpSum back to AllocaTmp
    StoreInst *StoreTmpBack = new StoreInst((Value*)VecTmpSum, AllocaTmp);
    StoreTmpBack->insertAfter(VecTmpSum);
    // cleanup
    bool bStartToRemove = false;
    SmallVector<Instruction *, 10> InstToRemoveVec;
    for (Instruction &Inst : *LBody) {
      if (&Inst == LBody->getTerminator()) {
        break;
      }
      if (bStartToRemove) {
        InstToRemoveVec.push_back(&Inst);
      }
      if (&Inst == StoreTmpBack) {
        bStartToRemove = true;
      }
    }
    for (auto it = (InstToRemoveVec.end() - 1) ; it >= InstToRemoveVec.begin(); --it) {
      (*it)->eraseFromParent();
    }
#if 0
    // store the tmps in loop.body
    Instruction *prevLastInst;
    for (Instruction &Inst : *LBody) {
      if (Inst.getOpcode() == Instruction::Br) {
        break;
      }
      prevLastInst = &Inst;
    }
    LoadInst *LoadFinal;
    for (Instruction &Inst : *LEnd) {
      LoadFinal = dyn_cast<LoadInst>(&Inst);
      if (LoadFinal) {
        break;
      }
    }
    StoreInst *StoreFinalBack = new StoreInst((Value*)FinalSum, LoadFinal->getOperand(0));
    StoreFinalBack->insertAfter(prevLastInst);
#endif
    errs() << "New Body-> " << *LBody << "\n";
    LoadInst *LoadFinal;
    for (Instruction &Inst : *LEnd) {
      LoadFinal = dyn_cast<LoadInst>(&Inst);
      if (LoadFinal) {
        break;
      }
    }
    // sum the results
    IRBuilder<> ExtractBuilder(LEnd->getFirstNonPHI());
    Instruction *LoadVecTmp = ExtractBuilder.CreateLoad(VecTy, AllocaTmp);
    Value *ExtractTmps[4];
    Instruction *ExtractSums[2];
    for (uint64_t i = 0; i < InterleaveFactor; i++) {
      ExtractTmps[i] = ExtractBuilder.CreateExtractElement(LoadVecTmp, i);
      if ((i%2) == 1) {
        Instruction *SumTmp = BinaryOperator::CreateAdd(ExtractTmps[i-1], ExtractTmps[i]);
        SumTmp->insertAfter((Instruction *)ExtractTmps[i]);
        ExtractSums[i/2] = SumTmp;
      }
    }
    Instruction *FinalSum = BinaryOperator::CreateAdd(ExtractSums[0], ExtractSums[1]);
    FinalSum->insertAfter(ExtractSums[1]);
    // store the the orig
    StoreInst *StoreFinalBack = new StoreInst((Value*)FinalSum, LoadFinal->getOperand(0));
    StoreFinalBack->insertAfter(FinalSum);
    errs() << "New End-> " << *LEnd << "\n";
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
