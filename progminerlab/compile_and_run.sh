#!/usr/bin/env sh


LLVM_PATH=../build/bin
RUNNER_PATH=./build

set -e
set -x

"$LLVM_PATH/clang" -O2 -g -c -S -emit-llvm -target progminerlab \
    -Wall -Werror -pedantic-errors src/main.c -o main.ll
"$LLVM_PATH/opt" -O2 -S main.ll -o main.opt.ll
"$LLVM_PATH/llc" -O2 -march progminerlab main.opt.ll -o main.s
"$LLVM_PATH/llc" -O2 -march progminerlab --filetype=obj main.opt.ll -o main.o
"$LLVM_PATH/llvm-readobj" main.o

"$RUNNER_PATH/runner" main.o
