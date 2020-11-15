# gen optimized llvm-ir
clang -O2 -Rpass-analysis=loop-vectorize -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -emit-llvm -S input.c -o input-vec.ll

# gen non-opt llvm-ir
clang -O0 -emit-llvm -S input.c -o input-O0.ll
