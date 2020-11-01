#ifndef LLVM_TUTOR_INSTRUMENT_BASIC_H
#define LLVM_TUTOR_INSTRUMENT_BASIC_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------
class SsLoopVec : public llvm::PassInfoMixin<SsLoopVec> {
public:
  bool DoImpl(llvm::Function &F);
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &);
  bool runOnFunction(llvm::Function &F);
};

//------------------------------------------------------------------------------
// Legacy PM interface
//------------------------------------------------------------------------------
class LegacySsLoopVec : public llvm::FunctionPass {
public:
  static char ID;
  LegacySsLoopVec() : FunctionPass(ID) {}
  bool runOnFunction(llvm::Function &F) override;
  SsLoopVec Impl;
};

#endif
