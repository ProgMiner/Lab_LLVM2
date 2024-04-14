#include "ProgMinerLabMCTargetDesc.h"

#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "TargetInfo/ProgMinerLabTargetInfo.h"


using namespace llvm;


#define GET_REGINFO_MC_DESC
#include "ProgMinerLabGenRegisterInfo.inc"


static MCRegisterInfo * createProgMinerLabMCRegisterInfo(const Triple & TT) {
    MCRegisterInfo * X = new MCRegisterInfo();

    InitProgMinerLabMCRegisterInfo(X, ProgMinerLab::I1);
    return X;
}

// We need to define this function for linking succeed
extern "C" void LLVMInitializeProgMinerLabTargetMC() {
    Target & TheProgMinerLabTarget = getTheProgMinerLabTarget();

    TargetRegistry::RegisterMCRegInfo(TheProgMinerLabTarget, createProgMinerLabMCRegisterInfo);
}
