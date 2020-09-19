#ifndef LLVM_TUTOR_INSTRUMENT_BASIC_H
#define LLVM_TUTOR_INSTRUMENT_BASIC_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"

//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------
struct SsSCCP : public llvm::PassInfoMixin<SsSCCP> {
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  bool runOnFunction(llvm::Function &F);
  bool DoSCCP(llvm::Function&, llvm::DominatorTree*);
};

//------------------------------------------------------------------------------
// Legacy PM interface
//------------------------------------------------------------------------------
struct LegacySsSCCP : public llvm::FunctionPass {
  static char ID;
  LegacySsSCCP() : FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &F) override;
  SsSCCP Impl;
};

#endif
