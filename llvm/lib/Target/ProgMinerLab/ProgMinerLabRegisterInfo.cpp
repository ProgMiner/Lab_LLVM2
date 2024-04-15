#include "ProgMinerLabRegisterInfo.h"

#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/IR/Function.h"

#include "ProgMinerLab.h"
#include "ProgMinerLabFrameLowering.h"
#include "ProgMinerLabInstrInfo.h"
#include "ProgMinerLabSubtarget.h"


using namespace llvm;


#define GET_REGINFO_TARGET_DESC
#include "ProgMinerLabGenRegisterInfo.inc"


ProgMinerLabRegisterInfo::ProgMinerLabRegisterInfo()
    : ProgMinerLabGenRegisterInfo(ProgMinerLab::R0)
    {}

const MCPhysReg * ProgMinerLabRegisterInfo::getCalleeSavedRegs(const MachineFunction * MF) const {
    llvm_unreachable("getCalleeSavedRegs"); // TODO
    // return CSR_ProgMinerLab_SaveList;
}

const uint32_t * ProgMinerLabRegisterInfo::getCallPreservedMask(
    const MachineFunction & MF,
    CallingConv::ID CC
) const {
    llvm_unreachable("getCallPreservedMask"); // TODO
    // return CSR_ProgMinerLab_RegMask;
}

// TODO: check cconv
BitVector ProgMinerLabRegisterInfo::getReservedRegs(const MachineFunction & MF) const {
    llvm_unreachable("getReservedRegs"); // TODO

/*
    ProgMinerLabFrameLowering const * TFI = getFrameLowering(MF);

    BitVector Reserved(getNumRegs());
    Reserved.set(ProgMinerLab::R1); // R1 is stack pointer

    if (TFI->hasFP(MF)) {
        Reserved.set(ProgMinerLab::R2); // R2 is frame pointer
    }

    return Reserved;
*/
}

bool ProgMinerLabRegisterInfo::requiresRegisterScavenging(const MachineFunction & MF) const {
    llvm_unreachable("requiresRegisterScavenging"); // TODO: what for?
    return false;
}

// TODO: rewrite!
bool ProgMinerLabRegisterInfo::eliminateFrameIndex(
    MachineBasicBlock::iterator II,
    int SPAdj,
    unsigned FIOperandNum,
    RegScavenger * RS
) const {
    llvm_unreachable("eliminateFrameIndex"); // TODO

/*
    assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

    MachineInstr & MI = *II;
    MachineFunction & MF = *MI.getParent()->getParent();
    DebugLoc DL = MI.getDebugLoc();

    const int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

    Register FrameReg;
    int Offset = getFrameLowering(MF)
        ->getFrameIndexReference(MF, FrameIndex, FrameReg)
        .getFixed();

    Offset += MI.getOperand(FIOperandNum + 1).getImm();

    if (!isInt<16>(Offset)) {
        llvm_unreachable("Offset must be 16-bit integer");
    }

    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
    MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
    return false;
*/
}

Register ProgMinerLabRegisterInfo::getFrameRegister(const MachineFunction & MF) const {
    llvm_unreachable("getFrameRegister"); // TODO

/*
    const TargetFrameLowering * TFI = getFrameLowering(MF);

    // R2 is frame pointer, R1 is stack pointer
    return TFI->hasFP(MF) ? ProgMinerLab::R2 : ProgMinerLab::R1;
*/
}

#if 0
// TODO
bool ProgMinerLabRegisterInfo::needsFrameMoves(const MachineFunction & MF) {
    return MF.needsFrameMoves();
}
#endif

#if 0
// TODO
bool ProgMinerLabRegisterInfo::useFPForScavengingIndex(const MachineFunction & MF) const {
    llvm_unreachable("");
}
#endif
