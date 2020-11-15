rel_path=~/workspace/ssOptimizer/SsLoopVec
#opt -load-pass-plugin $rel_path/build/libSsLoopVec.so -passes=SsLoopVec -disable-output $rel_path/test-inputs/no-inline/input_for_hello.ll
#opt -load $rel_path/build/libSsLoopVec.so -legacy-SsLoopVec $rel_path/test-inputs/no-inline/input_for_hello.ll -o out.bc -debug
opt -load $rel_path/build/libSsLoopVec.so -legacy-SsLoopVec $rel_path/test-inputs/input-O0.ll -o out.bc -debug
llvm-dis out.bc -o out.ll
lli ./out.bc
