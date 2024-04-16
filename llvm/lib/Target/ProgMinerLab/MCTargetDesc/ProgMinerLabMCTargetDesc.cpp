#include "ProgMinerLabMCTargetDesc.h"

#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "TargetInfo/ProgMinerLabTargetInfo.h"
#include "ProgMinerLabTargetStreamer.h"
#include "ProgMinerLabInstPrinter.h"
#include "ProgMinerLabMCAsmInfo.h"


using namespace llvm;


#define GET_REGINFO_MC_DESC
#include "ProgMinerLabGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "ProgMinerLabGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "ProgMinerLabGenSubtargetInfo.inc"


static MCRegisterInfo * createProgMinerLabMCRegisterInfo(const Triple & TT) {
    MCRegisterInfo * const X = new MCRegisterInfo();

    InitProgMinerLabMCRegisterInfo(X, ProgMinerLab::NoRegister);
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

    const unsigned SP = MRI.getDwarfRegNum(ProgMinerLab::SP, true);
    MAI->addInitialFrameState(MCCFIInstruction::cfiDefCfa(nullptr, SP, 0));

    return MAI;
}

static MCInstPrinter * createProgMinerLabMCInstPrinter(
    const Triple & T,
    unsigned SyntaxVariant,
    const MCAsmInfo & MAI,
    const MCInstrInfo & MII,
    const MCRegisterInfo & MRI
) {
    return new ProgMinerLabInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer * createTargetAsmStreamer(
    MCStreamer & S,
    formatted_raw_ostream & OS,
    MCInstPrinter * InstPrint,
    bool isVerboseAsm
) {
    return new ProgMinerLabTargetStreamer(S);
}

// We need to define this function for linking succeed
extern "C" void LLVMInitializeProgMinerLabTargetMC() {
    Target & TheProgMinerLabTarget = getTheProgMinerLabTarget();

    RegisterMCAsmInfoFn X(TheProgMinerLabTarget, createProgMinerLabMCAsmInfo);

    // register the MC register info
    TargetRegistry::RegisterMCRegInfo(TheProgMinerLabTarget, createProgMinerLabMCRegisterInfo);

    // register the MC instruction info
    TargetRegistry::RegisterMCInstrInfo(TheProgMinerLabTarget, createProgMinerLabMCInstrInfo);

    // register the MC subtarget info
    TargetRegistry::RegisterMCSubtargetInfo(TheProgMinerLabTarget, createProgMinerLabMCSubtargetInfo);

    // register the MCInstPrinter
    TargetRegistry::RegisterMCInstPrinter(TheProgMinerLabTarget, createProgMinerLabMCInstPrinter);
    TargetRegistry::RegisterAsmTargetStreamer(TheProgMinerLabTarget, createTargetAsmStreamer);

    // register the MC Code Emitter
    TargetRegistry::RegisterMCCodeEmitter(TheProgMinerLabTarget, createProgMinerLabMCCodeEmitter);

    // register the asm backend
    TargetRegistry::RegisterMCAsmBackend(TheProgMinerLabTarget, createProgMinerLabAsmBackend);
}
