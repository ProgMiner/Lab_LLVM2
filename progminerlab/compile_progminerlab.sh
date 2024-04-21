#!/usr/bin/env sh


LLVM_PATH=../build/bin

set -e
set -x

"$LLVM_PATH/clang" -O2 -c -target progminerlab src/main.c -o main.o
"$LLVM_PATH/llvm-readobj" main.o
