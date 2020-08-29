SsInline - Super simple Inline Pass
=======================================
This Pass implements a simple pass to inline functions called in **main** function.


Build
---------------
```bash
cd SsInline
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../
make
```

Generate Test file
--------------------
```bash
cd /path/to/SsInline/test-inputs
# change input.c in different directories to compare the differences.

# choose one of the following commands
## 1.
clang -S -emit-llvm input.c
## 2.
clang -O0 -emit-llvm -S input.c -o input.ll
## 3.
opt -always-inline input_for_hello.ll -o input_for_hello.bc 
llvm-dis input_for_hello.bc -o input_for_hello.ll.out
```


Run
--------------
```bash
export rel_path=~/path/to/ssOptimizer/SsInline
opt -load $rel_path/build/libSsInline.so -legacy-SsInline $rel_path/test-inputs/no-inline/input.ll -o out.bc -debug
llvm-dis out.bc -o out.ll
lli ./out.bc
```
