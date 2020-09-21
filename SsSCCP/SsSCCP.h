#ifndef LLVM_TUTOR_INSTRUMENT_BASIC_H
#define LLVM_TUTOR_INSTRUMENT_BASIC_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"

//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------
class SsSCCP : public llvm::PassInfoMixin<SsSCCP> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  bool runOnFunction(llvm::Function &F);
  bool DoSCCP(llvm::Function&, llvm::DominatorTree*);
  class AllocaInfo {
    llvm::Instruction *Inst;
    int ConstantNum;
  };
};

//------------------------------------------------------------------------------
// Legacy PM interface
//------------------------------------------------------------------------------
class LegacySsSCCP : public llvm::FunctionPass {
public:
  static char ID;
  LegacySsSCCP() : FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &F) override;
  SsSCCP Impl;
};

#endif
