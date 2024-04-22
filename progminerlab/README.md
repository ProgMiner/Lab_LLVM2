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

## Original application

Original test application, allows to check that source could be written in target-independent
manner:

```sh
./build/orig
```
