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
    : ProgMinerLabGenRegisterInfo(ProgMinerLab::NoRegister)
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
    Reserved.set(ProgMinerLab::TMP);

    if (TFI->hasFP(MF)) {
        Reserved.set(ProgMinerLab::FP);
    }

    return Reserved;
}

// TODO: rewrite!
bool ProgMinerLabRegisterInfo::eliminateFrameIndex(
    MachineBasicBlock::iterator II,
    int SPAdj,
    unsigned FIOperandNum,
    RegScavenger * RS
) const {
    assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

    MachineInstr & MI = *II;
    MachineBasicBlock & MBB = *MI.getParent();
    MachineFunction & MF = *MBB.getParent();
    // MachineRegisterInfo & MRI = MF.getRegInfo();
    const TargetInstrInfo & TII = *MF.getSubtarget().getInstrInfo();
    DebugLoc DL = MI.getDebugLoc();

    const int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

    Register FrameReg;
    int32_t Offset = getFrameLowering(MF)
        ->getFrameIndexReference(MF, FrameIndex, FrameReg).getFixed();

    if (isInt<8>(Offset)) {
        BuildMI(MBB, II, DL, TII.get(ProgMinerLab::CONSTs))
            .addReg(ProgMinerLab::TMP, RegState::Define).addImm(Offset);
    } else {
        BuildMI(MBB, II, DL, TII.get(ProgMinerLab::CONSTl))
            .addReg(ProgMinerLab::TMP, RegState::Define).addImm(Offset);
    }

    BuildMI(MBB, II, DL, TII.get(ProgMinerLab::ADD)).addReg(ProgMinerLab::TMP, RegState::Define)
        .addReg(ProgMinerLab::TMP, RegState::Kill).addReg(FrameReg);

    MI.getOperand(FIOperandNum).ChangeToRegister(ProgMinerLab::TMP, false, false, true);
    return false;
}

Register ProgMinerLabRegisterInfo::getFrameRegister(const MachineFunction & MF) const {
    const TargetFrameLowering * TFI = getFrameLowering(MF);

    return TFI->hasFP(MF) ? ProgMinerLab::FP : ProgMinerLab::SP;
}
