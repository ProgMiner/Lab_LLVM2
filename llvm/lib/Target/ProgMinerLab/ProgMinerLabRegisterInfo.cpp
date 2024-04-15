#include "ProgMinerLabRegisterInfo.h"

#include "llvm/CodeGen/TargetInstrInfo.h"

#include "ProgMinerLab.h"
#include "ProgMinerLabFrameLowering.h"


using namespace llvm;


#define GET_REGINFO_TARGET_DESC
#include "ProgMinerLabGenRegisterInfo.inc"


ProgMinerLabRegisterInfo::ProgMinerLabRegisterInfo()
    : ProgMinerLabGenRegisterInfo(ProgMinerLab::R0)
    {}
