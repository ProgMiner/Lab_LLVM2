#include "TargetInfo/ProgMinerLabTargetInfo.h"

#include "llvm/MC/TargetRegistry.h"


using namespace llvm;


Target & llvm::getTheProgMinerLabTarget() {
    static Target TheProgMinerLabTarget;
    return TheProgMinerLabTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeProgMinerLabTargetInfo() {
    RegisterTarget<Triple::progminer_lab> X(
        getTheProgMinerLabTarget(),
        "progminer_lab",
        "ProgMinerLab 32",
        "PROGMINER_LAB"
    );
}
