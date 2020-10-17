SsInline - Super simple Inline Pass
=======================================
This Pass implements a simple pass to do SCCP in **foo** function.


Build
---------------
```bash
cd SsSCCP
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../
make
```

Generate Test file
--------------------
```bash
cd /path/to/SsSCCP/test-inputs

# choose one of the following commands
## 1.
clang -S -emit-llvm input.c -o input.ll
## 2.
opt --view-dom input.ll
# The command above will show messages like:
Writing '/tmp/dom-4953a3.dot'...  done.
Trying 'xdg-open' program... Remember to erase graph file: /tmp/dom-4953a3.dot
Writing '/tmp/dom-c5edbc.dot'...  done.
Trying 'xdg-open' program... Remember to erase graph file: /tmp/dom-c5edbc.dot

## 3. Open the target dom tree for your desire
xdot /path/to/$file.dot &

```


Run
--------------
```bash
export rel_path=~/workspace/ssOptimizer/SsSCCP
opt -load $rel_path/build/libSsSCCP.so -legacy-SsSCCP $rel_path/test-inputs/input.ll -o out.bc -debug
llvm-dis out.bc -o out.ll
opt --view-dom out.ll
xdot /path/to/$file.dot &
```

Reference for more details
------------------------------
This project is based on [llvm-tutor](https://github.com/banach-space/llvm-tutor/blob/master/README.md)
