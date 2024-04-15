#include "ProgMinerLabSubtarget.h"

#include "llvm/Target/TargetMachine.h"


using namespace llvm;


#define DEBUG_TYPE "progminer_lab-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "ProgMinerLabGenSubtargetInfo.inc"


void ProgMinerLabSubtarget::anchor() {}

ProgMinerLabSubtarget::ProgMinerLabSubtarget(
    const Triple & TT,
    const StringRef & CPU,
    const StringRef & FS,
    const TargetMachine & TM
): ProgMinerLabGenSubtargetInfo(TT, CPU, /* TuneCPU = */ CPU, FS), InstrInfo(),
    FrameLowering(*this), TLInfo(TM, *this) {}
