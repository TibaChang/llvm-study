#ifndef LLVM_TUTOR_INSTRUMENT_BASIC_H
#define LLVM_TUTOR_INSTRUMENT_BASIC_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

//------------------------------------------------------------------------------
// New PM interface
//------------------------------------------------------------------------------
struct SsInline : public llvm::PassInfoMixin<SsInline> {
  llvm::PreservedAnalyses run(llvm::Module &M,
                              llvm::ModuleAnalysisManager &);
  bool runOnModule(llvm::Module &M);
};

//------------------------------------------------------------------------------
// Legacy PM interface
//------------------------------------------------------------------------------
struct LegacySsInline : public llvm::ModulePass {
  static char ID;
  LegacySsInline() : ModulePass(ID) {}
  bool runOnModule(llvm::Module &M) override;

  SsInline Impl;
};

#endif
