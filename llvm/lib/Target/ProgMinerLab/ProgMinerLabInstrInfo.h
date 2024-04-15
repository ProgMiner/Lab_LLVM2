#pragma once

#include "llvm/CodeGen/TargetInstrInfo.h"

#include "ProgMinerLabRegisterInfo.h"


#define GET_INSTRINFO_HEADER
#include "ProgMinerLabGenInstrInfo.inc"


namespace llvm {

// class ProgMinerLabSubtarget;

class ProgMinerLabInstrInfo : public ProgMinerLabGenInstrInfo {

public:

    ProgMinerLabInstrInfo();
};

} // namespace llvm
