# ProgMinerLab simulator

## Build

```sh
cmake -B build
cmake --build build
```

## Disassembler

Allows to disassemble compiled ELF files of ProgMinerLab architecture:

```sh
./build/disasm ./main.o
```

## Runner

Allows to run compiled ELF files of ProgMinerLab architecture in virtual environment:

```sh
./build/runner ./main.o
```

## Driver

Allows to automatically compile and run given C file into LLVM IR, next ProgMinerLab assembly file
and ELF object file, next run it with runner:

```sh
./compile_and_run.sh src/main.c
```

If filename isn't given, assumes `src/main.c`.

## Original application

Original test application, allows to check that source could be written in target-independent
manner:

```sh
./build/orig
```
