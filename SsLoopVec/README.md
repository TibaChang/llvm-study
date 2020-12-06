SsLoopVec - Super simple Loop Vectorize Pass
==============================================
This Pass implements a simple pass to do LoopVec in **foo** function.


Build
---------------
```bash
cd SsLoopVec
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../
make
```

Generate Test file
--------------------
```bash
cd /path/to/SsLoopVec/test-inputs
clang -S -emit-llvm input.c -o input.ll
```

Generate comparable results from original LoopVec to observe its IR
-----------------------------------------------------------------------
```bash
cd /path/to/SsLoopVec/test-inputs
clang -O2 -Rpass-analysis=loop-vectorize -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -emit-llvm -S input.c -o input-vec.ll
llvm-dis input-vec.bc -o input-vec.ll
lli ./input-vec.ll
```

Run on Test file
--------------
```bash
rel_path=~/workspace/ssOptimizer/SsLoopVec
opt -load $rel_path/build/libSsLoopVec.so -legacy-SsLoopVec $rel_path/test-inputs/input-O0.ll -o out.bc -debug
llvm-dis out.bc -o out.ll
lli ./out.bc
```

More Readings
-------------------
[LLVM LoopInfo](https://llvm.org/doxygen/classllvm_1_1LoopInfo.html)
[LLVM VPlan](https://llvm.org/docs/Proposals/VectorizationPlan.html)

Reference for more details
------------------------------
This project is based on [llvm-tutor](https://github.com/banach-space/llvm-tutor/blob/master/README.md)
