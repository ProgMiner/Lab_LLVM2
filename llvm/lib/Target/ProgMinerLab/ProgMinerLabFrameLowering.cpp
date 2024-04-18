#include "ProgMinerLabFrameLowering.h"

#include <algorithm>

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/Function.h"

#include "MCTargetDesc/ProgMinerLabMCTargetDesc.h"
#include "ProgMinerLabMachineFunctionInfo.h"
#include "ProgMinerLabSubtarget.h"


// #define DEBUG_TYPE "ProgMinerLab-frame-lowering"


using namespace llvm;


void ProgMinerLabFrameLowering::determineCalleeSaves(
    MachineFunction & MF,
    BitVector & SavedRegs,
    RegScavenger * RS
) const {
    TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);

    // unconditionally spill FP if the function uses a frame pointer
    if (hasFP(MF)) {
        // SavedRegs.set(ProgMinerLab::R0);
        SavedRegs.set(ProgMinerLab::FP);
    }
}

void ProgMinerLabFrameLowering::adjustStackToMatchRecords(
    MachineBasicBlock & MBB,
    MachineBasicBlock::iterator MBBI,
    bool Allocate
) const {
    llvm_unreachable("adjustStackToMatchRecords"); // TODO
}

void ProgMinerLabFrameLowering::emitPrologue(MachineFunction & MF, MachineBasicBlock & MBB) const {
    return; // TODO: seems ok
}

void ProgMinerLabFrameLowering::emitEpilogue(MachineFunction & MF, MachineBasicBlock & MBB) const {
    return; // TODO: seems ok
}

bool ProgMinerLabFrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock & MBB,
    MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI,
    const TargetRegisterInfo * TRI
) const {
    if (CSI.empty()) {
        return true;
    }

    // TODO: seems ok
    MachineFunction * const MF = MBB.getParent();
    const TargetInstrInfo & TII = *MF->getSubtarget().getInstrInfo();

    for (auto & CS : CSI) {
        // insert the spill to the stack frame

        Register Reg = CS.getReg();
        const TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
        TII.storeRegToStackSlot(MBB, MI, Reg, !MBB.isLiveIn(Reg), CS.getFrameIdx(), RC, TRI, 0);
    }

    return true;
}

bool ProgMinerLabFrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock & MBB,
    MachineBasicBlock::iterator MI,
    MutableArrayRef<CalleeSavedInfo> CSI,
    const TargetRegisterInfo * TRI
) const {
    if (CSI.empty()) {
        return true;
    }

    // TODO: seems ok
    MachineFunction * const MF = MBB.getParent();
    const TargetInstrInfo & TII = *MF->getSubtarget().getInstrInfo();

    // insert in reverse order
    // loadRegFromStackSlot can insert multiple instructions
    for (auto & CS : reverse(CSI)) {
        Register Reg = CS.getReg();

        const TargetRegisterClass * RC = TRI->getMinimalPhysRegClass(Reg);
        TII.loadRegFromStackSlot(MBB, MI, Reg, CS.getFrameIdx(), RC, TRI, 0);

        assert(MI != MBB.begin() && "loadRegFromStackSlot didn't insert any code!");
    }

    return true;
}

bool ProgMinerLabFrameLowering::hasFP(const MachineFunction & MF) const {
    const TargetRegisterInfo * const RegInfo = MF.getSubtarget().getRegisterInfo();

    const MachineFrameInfo & MFI = MF.getFrameInfo();
    return MF.getTarget().Options.DisableFramePointerElim(MF) // -fomit-frame-pointer
        || RegInfo->hasStackRealignment(MF) || MFI.hasVarSizedObjects()
        || MFI.isFrameAddressTaken();
}

StackOffset ProgMinerLabFrameLowering::getFrameIndexReference(
    const MachineFunction & MF,
    int FI,
    Register & FrameReg
) const {
    // TODO: rewrite!

    const MachineFrameInfo & MFI = MF.getFrameInfo();
    const TargetRegisterInfo * RI = MF.getSubtarget().getRegisterInfo();
    const auto * UFI = MF.getInfo<ProgMinerLabFunctionInfo>();

    // callee-saved registers should be referenced relative to the stack pointer (positive offset),
    // otherwise use the frame pointer (negative offset)
    const auto & CSI = MFI.getCalleeSavedInfo();

    int MinCSFI = 0;
    int MaxCSFI = -1;
    int Offset = MFI.getObjectOffset(FI) - getOffsetOfLocalArea() + MFI.getOffsetAdjustment();

    if (CSI.size()) {
        MinCSFI = CSI[0].getFrameIdx();
        MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
    }

    if (FI >= MinCSFI && FI <= MaxCSFI) {
        FrameReg = ProgMinerLab::SP;
        Offset += MFI.getStackSize();
    } else if (RI->hasStackRealignment(MF) && !MFI.isFixedObjectIndex(FI)) {
        llvm_unreachable("getFrameIndexReference: realigned stack"); // TODO: realigned stack
    } else {
        // TODO: what's going on here
        FrameReg = RI->getFrameRegister(MF);

        if (hasFP(MF)) {
            Offset += UFI->getVarArgsSaveSize();
        } else {
            Offset += MFI.getStackSize();
        }
    }

    return StackOffset::getFixed(Offset);
}

// not preserve stack space within prologue for outgoing variables when the function contains
// variable size objects or there are vector objects accessed by the frame pointer
// let eliminateCallFramePseudoInstr preserve stack space for it
bool ProgMinerLabFrameLowering::hasReservedCallFrame(const MachineFunction & MF) const {
    return !MF.getFrameInfo().hasVarSizedObjects();
}
