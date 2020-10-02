#include "SsSCCP.h"

#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/DenseMap.h"


using namespace std;
using namespace llvm;

#define DEBUG_TYPE "SsSCCP"

//-----------------------------------------------------------------------------
// SsSCCP implementation
//-----------------------------------------------------------------------------

/// This code only looks at accesses to allocas.
static bool isInterestingInstruction(const Instruction *I) {
  return (isa<LoadInst>(I) && isa<AllocaInst>(I->getOperand(0))) ||
         (isa<StoreInst>(I) && isa<AllocaInst>(I->getOperand(1)));
}

static void SCCP_GatherAlloca(BasicBlock &BB, DenseMap<Instruction *, SsSCCP::AllocaInfo> *AllocaMap) {
  uint64_t Num;
  for (auto &Ins : BB) {
    if (isa<AllocaInst>(&Ins)) {
      errs() << Ins << "\n";
      for (User *user : Ins.users()) {
        auto *UserI = dyn_cast<Instruction>(user);
        //errs() << *UserI << "\n";
        if (UserI && isa<StoreInst>(UserI) && isa<ConstantInt>(UserI->getOperand(0))) {
          ConstantInt *Val = cast<ConstantInt>(UserI->getOperand(0));
          Num = Val->getLimitedValue(~0U);
          SsSCCP::AllocaInfo alloc(&BB, Num);
          (*AllocaMap)[&Ins] = alloc;
        }
        //TODO: more than one store to the alloca
      }
    }
  }
#if 0
  for (auto &ele: *AllocaMap) {
    errs() << *(ele.first) << ": " << (ele.second).Num << " in " << (ele.second).BB->getName() << "\n";
  }
#endif
}

static void SCCP_SearchDFS(DominatorTree *DT, BasicBlock *BB, DenseMap<Instruction *, SsSCCP::AllocaInfo> *AllocaMap, uint32_t CurrentDomLevel) {
  SmallVector<BasicBlock *, 24> DominatedBBs;
  DT->getDescendants(BB, DominatedBBs);
  uint32_t CurrentLevel;
  Instruction *pInst;
  int i;
  for (auto *dBB : DominatedBBs) {
    CurrentLevel = DT->getNode(dBB)->getLevel();
    if (CurrentDomLevel == CurrentLevel) {
      //errs() << "    child BB=[" << dBB->getName() << "] at lvl=[" << CurrentLevel << "]\n";
      // if found store to the alloca in map, treat it as changed.
      for (auto &Inst : *dBB) {
        if (isa<StoreInst>(Inst)) {
          //errs() << "Found store: " << Inst << "\n";
          pInst = cast<Instruction>((Inst.getOperand(1)));
          if ((*AllocaMap).find(pInst) != (*AllocaMap).end()) {
#if 0
            for (i = 0; i < CurrentDomLevel; i++) {
              errs() << "___ ";
            }
#endif
            errs() << "<Erase> " << *pInst << "; for " << Inst << "\n";
            (*AllocaMap).erase(pInst);
          }
        }
      }
      SCCP_SearchDFS(DT, dBB, AllocaMap, CurrentDomLevel + 1);
    }
  }
  if (DominatedBBs.size() == 1) {
#if 0
    for (i = 0; i < CurrentDomLevel; i++) {
      errs() << "___ ";
    }
    errs() << "END at BB[" << BB->getName() << "].\n";
#endif
  }
}

static void SCCP_ReplaceAllocaWithConstantDFS(DominatorTree *DT, BasicBlock *BB, DenseMap<Instruction *, SsSCCP::AllocaInfo> *AllocaMap, uint32_t CurrentDomLevel, DenseMap<Instruction *, SsSCCP::AllocaInfo> *ReplaceMap) {
  //TODO: replace all AllocaMap w/ the int in Instr.
  SmallVector<BasicBlock *, 24> DominatedBBs;
  DT->getDescendants(BB, DominatedBBs);
  uint32_t CurrentLevel;
  Instruction *opInst;
  bool bNextInst;
  for (auto *dBB : DominatedBBs) {
    CurrentLevel = DT->getNode(dBB)->getLevel();
    if (CurrentDomLevel == CurrentLevel) {
      for (auto &Inst : *dBB) {
        //errs() << "BB: " << dBB->getName() << "-> " << Inst << "\n";
        bNextInst = false;
        for (auto operand = Inst.operands().begin();
            operand != Inst.operands().end(); ++operand) {
          opInst = dyn_cast<Instruction>(*operand);
          if ((opInst != nullptr) && ((*AllocaMap).find(opInst) != (*AllocaMap).end())) {
            // add the "load" into ReplaceMap
            (*ReplaceMap)[&Inst] = (*AllocaMap)[opInst];
            //LLVM_DEBUG(dbgs() << "Op of instr[ " << Inst << "] -> " << **operand << "; Num=[" << (*ReplaceMap)[&Inst].Num << "]\n");
            bNextInst = true;
          }
        }
        if (bNextInst) {
          continue;
        }
        // Replace the op in ReplaceMap
        int i = 0;
        for (auto operand = Inst.operands().begin();
            operand != Inst.operands().end(); ++operand) {
          // 1. iterate the op of the inst
          opInst = dyn_cast<Instruction>(*operand);
          if ((opInst != nullptr) && ((*ReplaceMap).find(opInst) != (*ReplaceMap).end())) {
            // 2. if the op is in ReplaceMap, replace it.
            auto TargetVal = ConstantInt::get((*operand)->getType(), (*ReplaceMap)[opInst].Num);
            errs() << "Try to replace the user of Inst: [" << Inst << "] w/ op[" << *TargetVal << "]\n"; 
            Inst.setOperand(i, TargetVal);
            errs() << "After replace-> [" << Inst << "]\n"; 
          }
          i++;
        }
      }
      SCCP_ReplaceAllocaWithConstantDFS(DT, dBB, AllocaMap, CurrentDomLevel + 1, ReplaceMap);
    }
  }
}


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

  // Goal: Proves values to be constant, and replaces them with constants
  DenseMap<Instruction *, AllocaInfo> AllocaMap;
  SCCP_GatherAlloca(F.getEntryBlock(), &AllocaMap);
  SmallVector<BasicBlock *, 24> DominatedBBs;
  DT->getDescendants(DT->getRoot(), DominatedBBs);
  for (auto *BB : DominatedBBs) {
    if (DT->getNode(BB)->getLevel() == 1) {
      errs() << "Begin DFS: " << BB->getName() << "\n";
      DenseMap<Instruction *, AllocaInfo> LocalAllocaMap = AllocaMap;
      SCCP_SearchDFS(DT, BB, &LocalAllocaMap, 1); // start at level 1
      DenseMap<Instruction *, SsSCCP::AllocaInfo> ReplaceMap;
      SCCP_ReplaceAllocaWithConstantDFS(DT, BB, &LocalAllocaMap, 1, &ReplaceMap);
      //TODO: remove the inst in ReplaceMap
      for (auto &ele : ReplaceMap) {
        (*(ele.first)).eraseFromParent();
      }
    }
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
