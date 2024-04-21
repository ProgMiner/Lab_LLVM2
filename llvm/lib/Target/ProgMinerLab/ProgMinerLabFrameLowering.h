#pragma once

#include "llvm/CodeGen/TargetFrameLowering.h"


namespace llvm {

class ProgMinerLabSubtarget;

class ProgMinerLabFrameLowering : public TargetFrameLowering {

    const ProgMinerLabSubtarget & STI;

public:

    explicit ProgMinerLabFrameLowering(const ProgMinerLabSubtarget & STI)
        : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(4), 0)
        , STI(STI)
        {}

    // emitProlog/emitEpilog - these methods insert prolog and epilog code into the function
    void emitPrologue(MachineFunction & MF, MachineBasicBlock & MBB) const override;
    void emitEpilogue(MachineFunction & MF, MachineBasicBlock & MBB) const override;

    void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

    bool spillCalleeSavedRegisters(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator MI,
        ArrayRef<CalleeSavedInfo> CSI,
        const TargetRegisterInfo * TRI
    ) const override;

    bool restoreCalleeSavedRegisters(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator MI,
        MutableArrayRef<CalleeSavedInfo> CSI,
        const TargetRegisterInfo * TRI
    ) const override;

    void processFunctionBeforeFrameFinalized(
        MachineFunction & MF,
        RegScavenger * RS
    ) const override;

    // hasFP - return true if the specified function should have a dedicated
    // frame pointer register. For most targets this is true only if the function
    // has variable sized allocas or if frame pointer elimination is disabled
    bool hasFP(const MachineFunction & MF) const override;

    MachineBasicBlock::iterator eliminateCallFramePseudoInstr(
        MachineFunction & MF,
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator I
    ) const override;

    StackOffset getFrameIndexReference(
        const MachineFunction & MF,
        int FI,
        Register & FrameReg
    ) const override;

    bool hasReservedCallFrame(const MachineFunction & MF) const override;

private:

    void adjustStackToMatchRecords(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator MI,
        bool Allocate
    ) const;

    void adjustReg(
        MachineBasicBlock & MBB,
        MachineBasicBlock::iterator MBBI,
        const DebugLoc & DL,
        Register DestReg,
        Register SrcReg,
        int64_t Val,
        MachineInstr::MIFlag Flag
    ) const;
};

} // namespace llvm
