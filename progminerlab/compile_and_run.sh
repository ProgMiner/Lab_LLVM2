#!/usr/bin/env sh


LLVM_PATH=../build/bin
RUNNER_PATH=./build

BUILD_DIR=./compile_and_run
TARGET_NAME=progminerlab

set -e

filename="${1:-src/main.c}"
mkdir -p "$BUILD_DIR/$filename"
rmdir "$BUILD_DIR/$filename"

set -x

# compile C to LLVM IR
"$LLVM_PATH/clang" -O2 -g -c -S -emit-llvm -target "$TARGET_NAME" \
    -Wall -Werror -pedantic-errors "$filename" -o "$BUILD_DIR/$filename.ll"

# optimize LLVM IR using opt
"$LLVM_PATH/opt" -O2 -S "$BUILD_DIR/$filename.ll" -o "$BUILD_DIR/$filename.opt.ll"

# print target assembly
"$LLVM_PATH/llc" -O2 -march "$TARGET_NAME" "$BUILD_DIR/$filename.opt.ll" -o "$BUILD_DIR/$filename.s"

# assembly object file
"$LLVM_PATH/llc" -O2 -march "$TARGET_NAME" --filetype=obj "$BUILD_DIR/$filename.opt.ll" \
    -o "$BUILD_DIR/$filename.o"

# print ELF headers and run
"$LLVM_PATH/llvm-readobj" "$BUILD_DIR/$filename.o"
"$RUNNER_PATH/runner" "$BUILD_DIR/$filename.o"
