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

#include "MCTargetDesc/ProgMinerLabMCTargetDesc.h"
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
    return CSR_ProgMinerLab_SaveList;
}

const uint32_t * ProgMinerLabRegisterInfo::getCallPreservedMask(
    const MachineFunction & MF,
    CallingConv::ID CC
) const {
    return CSR_ProgMinerLab_RegMask;
}

// TODO: check cconv
BitVector ProgMinerLabRegisterInfo::getReservedRegs(const MachineFunction & MF) const {
    ProgMinerLabFrameLowering const * TFI = getFrameLowering(MF);

    BitVector Reserved(getNumRegs());
    Reserved.set(ProgMinerLab::SP);

    if (TFI->hasFP(MF)) {
        Reserved.set(ProgMinerLab::FP);
    }

    return Reserved;
}

bool ProgMinerLabRegisterInfo::requiresRegisterScavenging(const MachineFunction & MF) const {
    return false; // TODO: what for?
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
    const TargetFrameLowering * TFI = getFrameLowering(MF);

    return TFI->hasFP(MF) ? ProgMinerLab::FP : ProgMinerLab::SP;
}
