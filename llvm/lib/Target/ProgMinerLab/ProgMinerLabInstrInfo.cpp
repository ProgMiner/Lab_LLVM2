#include "ProgMinerLabInstrInfo.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/MC/TargetRegistry.h"

#include "MCTargetDesc/ProgMinerLabMCTargetDesc.h"


using namespace llvm;


#define GET_INSTRINFO_CTOR_DTOR
#include "ProgMinerLabGenInstrInfo.inc"


ProgMinerLabInstrInfo::ProgMinerLabInstrInfo() : ProgMinerLabGenInstrInfo() {}

void ProgMinerLabInstrInfo::storeRegToStackSlot(
    MachineBasicBlock & MBB,
    MachineBasicBlock::iterator I,
    Register SrcReg,
    bool IsKill,
    int FI,
    const TargetRegisterClass * RC,
    const TargetRegisterInfo * TRI,
    Register VReg
) const {
    DebugLoc DL;

    if (I != MBB.end()) {
        DL = I->getDebugLoc();
    }

    // MachineFunction * MF = MBB.getParent();
    // MachineFrameInfo & MFI = MF->getFrameInfo();

    // MachineMemOperand * MMO = MF->getMachineMemOperand(
    //     MachinePointerInfo::getFixedStack(*MF, FI),
    //     MachineMemOperand::MOStore,
    //     MFI.getObjectSize(FI),
    //     MFI.getObjectAlign(FI)
    // );

    BuildMI(MBB, I, DL, get(ProgMinerLab::STORE)).addFrameIndex(FI)
        .addReg(SrcReg, getKillRegState(IsKill)) /* .addMemOperand(MMO) */;
}

void ProgMinerLabInstrInfo::loadRegFromStackSlot(
    MachineBasicBlock & MBB,
    MachineBasicBlock::iterator I,
    Register DstReg,
    int FI,
    const TargetRegisterClass * RC,
    const TargetRegisterInfo * TRI,
    Register VReg
) const {
    DebugLoc DL;

    if (I != MBB.end()) {
        DL = I->getDebugLoc();
    }

    // MachineFunction * MF = MBB.getParent();
    // MachineFrameInfo & MFI = MF->getFrameInfo();

    // MachineMemOperand * MMO = MF->getMachineMemOperand(
    //     MachinePointerInfo::getFixedStack(*MF, FI),
    //     MachineMemOperand::MOLoad,
    //     MFI.getObjectSize(FI),
    //     MFI.getObjectAlign(FI)
    // );

    BuildMI(MBB, I, DL, get(ProgMinerLab::LOAD), DstReg)
        .addFrameIndex(FI) /* .addMemOperand(MMO) */;
}
