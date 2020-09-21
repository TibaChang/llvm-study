rel_path=~/workspace/ssOptimizer/SsSCCP
#opt -load-pass-plugin $rel_path/build/libssInline.so -passes=ssInline -disable-output $rel_path/test-inputs/no-inline/input_for_hello.ll
#opt -load $rel_path/build/libSsInline.so -legacy-SsInline $rel_path/test-inputs/no-inline/input_for_hello.ll -o out.bc -debug
opt -load $rel_path/build/libSsSCCP.so -legacy-SsSCCP $rel_path/test-inputs/input.ll -o out.bc -debug
#llvm-dis out.bc -o out.ll
#lli ./out.bc
