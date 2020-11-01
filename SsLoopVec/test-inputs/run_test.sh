clang -O2 -Rpass-analysis=loop-vectorize -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -emit-llvm -S input.c -o input-vec.ll
llvm-dis input-vec.bc -o input-vec.ll
lli ./input-vec.ll
