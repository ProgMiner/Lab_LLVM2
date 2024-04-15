#pragma once


namespace llvm {

class FunctionPass;
class ProgMinerLabTargetMachine;

FunctionPass * createProgMinerLabISelDag(ProgMinerLabTargetMachine & TM);

} // namespace llvm
