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

    void copyPhysReg(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator I,
        const DebugLoc & DL,
        MCRegister DestReg,
        MCRegister SrcReg,
        bool KillSrc
    ) const override;

    void storeRegToStackSlot(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator MI,
        Register SrcReg,
        bool isKill,
        int FrameIndex,
        const TargetRegisterClass * RC,
        const TargetRegisterInfo * TRI,
        Register VReg
    ) const override;

    void loadRegFromStackSlot(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator MI,
        Register DestReg,
        int FrameIndex,
        const TargetRegisterClass * RC,
        const TargetRegisterInfo * TRI,
        Register VReg
    ) const override;
};

} // namespace llvm
