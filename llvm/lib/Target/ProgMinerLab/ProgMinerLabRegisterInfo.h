#pragma once


#define GET_REGINFO_HEADER
#include "ProgMinerLabGenRegisterInfo.inc"


namespace llvm {

struct ProgMinerLabRegisterInfo : public ProgMinerLabGenRegisterInfo {

public:

    ProgMinerLabRegisterInfo();

    const MCPhysReg * getCalleeSavedRegs(const MachineFunction * MF) const override;

    const uint32_t * getCallPreservedMask(
        const MachineFunction & MF,
        CallingConv::ID CC
    ) const override;

    BitVector getReservedRegs(const MachineFunction & MF) const override;

    bool requiresRegisterScavenging(const MachineFunction & MF) const override;

    bool eliminateFrameIndex(
        MachineBasicBlock::iterator II,
        int SPAdj,
        unsigned FIOperandNum,
        RegScavenger * RS = nullptr
    ) const override;

    // debug information queries
    Register getFrameRegister(const MachineFunction & MF) const override;
};

} // namespace llvm
