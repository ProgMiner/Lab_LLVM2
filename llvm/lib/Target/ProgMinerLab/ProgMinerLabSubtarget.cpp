#include "ProgMinerLabSubtarget.h"

#include "llvm/Target/TargetMachine.h"


using namespace llvm;


#define DEBUG_TYPE "progminer_lab-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "ProgMinerLabGenSubtargetInfo.inc"


ProgMinerLabSubtarget::ProgMinerLabSubtarget(
    const StringRef & CPU,
    const StringRef & TuneCPU,
    const StringRef & FS,
    const TargetMachine & TM
): ProgMinerLabGenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS) {}
