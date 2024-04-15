#include "ProgMinerLabMCTargetDesc.h"

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "TargetInfo/ProgMinerLabTargetInfo.h"


using namespace llvm;


#define GET_REGINFO_MC_DESC
#include "ProgMinerLabGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "ProgMinerLabGenInstrInfo.inc"


static MCRegisterInfo * createProgMinerLabMCRegisterInfo(const Triple & TT) {
    MCRegisterInfo * X = new MCRegisterInfo();

    InitProgMinerLabMCRegisterInfo(X, ProgMinerLab::R0);
    return X;
}

static MCInstrInfo *createProgMinerLabMCInstrInfo() {
    MCInstrInfo * X = new MCInstrInfo();

    InitProgMinerLabMCInstrInfo(X);
    return X;
}

// We need to define this function for linking succeed
extern "C" void LLVMInitializeProgMinerLabTargetMC() {
    Target & TheProgMinerLabTarget = getTheProgMinerLabTarget();

    // Register the MC register info
    TargetRegistry::RegisterMCRegInfo(TheProgMinerLabTarget, createProgMinerLabMCRegisterInfo);

    // Register the MC instruction info
    TargetRegistry::RegisterMCInstrInfo(TheProgMinerLabTarget, createProgMinerLabMCInstrInfo);
}
