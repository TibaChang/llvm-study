#include "SsInline.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

using namespace llvm;

#define DEBUG_TYPE "Ss-Inline"

Constant *CreateGlobalCounter(Module &M, StringRef GlobalVarName) {
  auto &CTX = M.getContext();

  // This will insert a declaration into M
  Constant *NewGlobalVar =
      M.getOrInsertGlobal(GlobalVarName, IntegerType::getInt32Ty(CTX));

  // This will change the declaration into definition (and initialise to 0)
  GlobalVariable *NewGV = M.getNamedGlobal(GlobalVarName);
  NewGV->setLinkage(GlobalValue::CommonLinkage);
  NewGV->setAlignment(MaybeAlign(4));
  NewGV->setInitializer(llvm::ConstantInt::get(CTX, APInt(32, 0)));

  return NewGlobalVar;
}

//-----------------------------------------------------------------------------
// SsInline implementation
//-----------------------------------------------------------------------------
bool SsInline::runOnModule(Module &M) {
  bool Instrumented = false;

  // Function name <--> IR variable that holds the call counter
  llvm::StringMap<Constant *> CallCounterMap;
  // Function name <--> IR variable that holds the function name
  llvm::StringMap<Constant *> FuncNameMap;

  auto &CTX = M.getContext();

  // STEP 1: For each function in the module, inject a call-counting code
  // --------------------------------------------------------------------
  for (auto &F : M) {
    if (F.isDeclaration())
      continue;

    // Get an IR builder. Sets the insertion point to the top of the function
    IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());

    // Create a global variable to count the calls to this function
    std::string CounterName = "CounterFor_" + std::string(F.getName());
    Constant *Var = CreateGlobalCounter(M, CounterName);
    CallCounterMap[F.getName()] = Var;

    // Create a global variable to hold the name of this function
    auto FuncName = Builder.CreateGlobalStringPtr(F.getName());
    FuncNameMap[F.getName()] = FuncName;

    // Inject instruction to increment the call count each time this function
    // executes
    LoadInst *Load2 = Builder.CreateLoad(Var);
    Value *Inc2 = Builder.CreateAdd(Builder.getInt32(1), Load2);
    Builder.CreateStore(Inc2, Var);

    // The following is visible only if you pass -debug on the command line
    // *and* you have an assert build.
    LLVM_DEBUG(dbgs() << " Instrumented: " << F.getName() << "\n");

    Instrumented = true;
  }

  // Stop here if there are no function definitions in this module
  if (false == Instrumented)
    return Instrumented;

  // STEP 2: Inject the declaration of printf
  // ----------------------------------------
  // Create (or _get_ in cases where it's already available) the following
  // declaration in the IR module:
  //    declare i32 @printf(i8*, ...)
  // It corresponds to the following C declaration:
  //    int printf(char *, ...)
  PointerType *PrintfArgTy = PointerType::getUnqual(Type::getInt8Ty(CTX));
  FunctionType *PrintfTy =
      FunctionType::get(IntegerType::getInt32Ty(CTX), PrintfArgTy,
                        /*IsVarArgs=*/true);

  FunctionCallee Printf = M.getOrInsertFunction("printf", PrintfTy);

  // Set attributes as per inferLibFuncAttributes in BuildLibCalls.cpp
  Function *PrintfF = dyn_cast<Function>(Printf.getCallee());
  PrintfF->setDoesNotThrow();
  PrintfF->addParamAttr(0, Attribute::NoCapture);
  PrintfF->addParamAttr(0, Attribute::ReadOnly);

  // STEP 3: Inject a global variable that will hold the printf format string
  // ------------------------------------------------------------------------
  llvm::Constant *ResultFormatStr =
      llvm::ConstantDataArray::getString(CTX, "%-20s %-10lu\n");

  Constant *ResultFormatStrVar =
      M.getOrInsertGlobal("ResultFormatStrIR", ResultFormatStr->getType());
  dyn_cast<GlobalVariable>(ResultFormatStrVar)->setInitializer(ResultFormatStr);

  std::string out = "";
  out += "=================================================\n";
  out += "LLVM-TUTOR: analysis results\n";
  out += "=================================================\n";
  out += "NAME                 #N DIRECT CALLS\n";
  out += "-------------------------------------------------\n";

  llvm::Constant *ResultHeaderStr =
      llvm::ConstantDataArray::getString(CTX, out.c_str());

  Constant *ResultHeaderStrVar =
      M.getOrInsertGlobal("ResultHeaderStrIR", ResultHeaderStr->getType());
  dyn_cast<GlobalVariable>(ResultHeaderStrVar)->setInitializer(ResultHeaderStr);

  // STEP 4: Define a printf wrapper that will print the results
  // -----------------------------------------------------------
  // Define `printf_wrapper` that will print the results stored in FuncNameMap
  // and CallCounterMap.  It is equivalent to the following C++ function:
  // ```
  //    void printf_wrapper() {
  //      for (auto &item : Functions)
  //        printf("llvm-tutor): Function %s was called %d times. \n",
  //        item.name, item.count);
  //    }
  // ```
  // (item.name comes from FuncNameMap, item.count comes from
  // CallCounterMap)
  FunctionType *PrintfWrapperTy =
      FunctionType::get(llvm::Type::getVoidTy(CTX), {},
                        /*IsVarArgs=*/false);
  Function *PrintfWrapperF = dyn_cast<Function>(
      M.getOrInsertFunction("printf_wrapper", PrintfWrapperTy).getCallee());

  // Create the entry basic block for printf_wrapper ...
  llvm::BasicBlock *RetBlock =
      llvm::BasicBlock::Create(CTX, "enter", PrintfWrapperF);
  IRBuilder<> Builder(RetBlock);

  // ... and start inserting calls to printf
  // (printf requires i8*, so cast the input strings accordingly)
  llvm::Value *ResultHeaderStrPtr =
      Builder.CreatePointerCast(ResultHeaderStrVar, PrintfArgTy);
  llvm::Value *ResultFormatStrPtr =
      Builder.CreatePointerCast(ResultFormatStrVar, PrintfArgTy);

  Builder.CreateCall(Printf, {ResultHeaderStrPtr});

  LoadInst *LoadCounter;
  for (auto &item : CallCounterMap) {
    LoadCounter = Builder.CreateLoad(item.second);
    Builder.CreateCall(
        Printf, {ResultFormatStrPtr, FuncNameMap[item.first()], LoadCounter});
  }

  // Finally, insert return instruction
  Builder.CreateRetVoid();

  // STEP 5: Call `printf_wrapper` at the very end of this module
  // ------------------------------------------------------------
  appendToGlobalDtors(M, PrintfWrapperF, /*Priority=*/0);

  return true;
}

PreservedAnalyses SsInline::run(llvm::Module &M,
                                          llvm::ModuleAnalysisManager &) {
  bool Changed = runOnModule(M);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}

bool LegacySsInline::runOnModule(llvm::Module &M) {
  bool Changed = Impl.runOnModule(M);

  return Changed;
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getSsInlinePluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "SsInline", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "SsInline") {
                    MPM.addPass(SsInline());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getSsInlinePluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
char LegacySsInline::ID = 0;

// Register the pass - required for (among others) opt
static RegisterPass<LegacySsInline>
    X(/*PassArg=*/"legacy-SsInline",
      /*Name=*/"LegacySsInline",
      /*CFGOnly=*/false,
      /*is_analysis=*/false);
