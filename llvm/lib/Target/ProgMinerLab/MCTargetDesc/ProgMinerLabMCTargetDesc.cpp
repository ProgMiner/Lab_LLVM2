#include "ProgMinerLabMCTargetDesc.h"

#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "ProgMinerLabMCAsmInfo.h"
#include "TargetInfo/ProgMinerLabTargetInfo.h"


using namespace llvm;


#define GET_REGINFO_MC_DESC
#include "ProgMinerLabGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "ProgMinerLabGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "ProgMinerLabGenSubtargetInfo.inc"


static MCRegisterInfo * createProgMinerLabMCRegisterInfo(const Triple & TT) {
    MCRegisterInfo * const X = new MCRegisterInfo();

    InitProgMinerLabMCRegisterInfo(X, ProgMinerLab::R0);
    return X;
}

static MCInstrInfo * createProgMinerLabMCInstrInfo() {
    MCInstrInfo * const X = new MCInstrInfo();

    InitProgMinerLabMCInstrInfo(X);
    return X;
}

static MCSubtargetInfo * createProgMinerLabMCSubtargetInfo(
    const Triple & TT,
    StringRef CPU,
    StringRef FS
) {
    return createProgMinerLabMCSubtargetInfoImpl(TT, CPU, /* TuneCPU */ CPU, FS);
}

static MCAsmInfo * createProgMinerLabMCAsmInfo(
    const MCRegisterInfo & MRI,
    const Triple & TT,
    const MCTargetOptions & Options
) {
    MCAsmInfo * const MAI = new ProgMinerLabELFMCAsmInfo(TT);

    // stack pointer is R1
    const unsigned SP = MRI.getDwarfRegNum(ProgMinerLab::R1, true);

    MAI->addInitialFrameState(MCCFIInstruction::cfiDefCfa(nullptr, SP, 0));
    return MAI;
}

// We need to define this function for linking succeed
extern "C" void LLVMInitializeProgMinerLabTargetMC() {
    Target & TheProgMinerLabTarget = getTheProgMinerLabTarget();

    RegisterMCAsmInfoFn X(TheProgMinerLabTarget, createProgMinerLabMCAsmInfo);

    // Register the MC register info
    TargetRegistry::RegisterMCRegInfo(TheProgMinerLabTarget, createProgMinerLabMCRegisterInfo);

    // Register the MC instruction info
    TargetRegistry::RegisterMCInstrInfo(TheProgMinerLabTarget, createProgMinerLabMCInstrInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(TheProgMinerLabTarget, createProgMinerLabMCSubtargetInfo);
}
