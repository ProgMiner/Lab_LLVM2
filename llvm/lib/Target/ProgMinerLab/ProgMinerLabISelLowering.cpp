#include "ProgMinerLabISelLowering.h"

#include "ProgMinerLabRegisterInfo.h"
#include "ProgMinerLabSubtarget.h"

using namespace llvm;


ProgMinerLabTargetLowering::ProgMinerLabTargetLowering(
    const TargetMachine & TM,
    const ProgMinerLabSubtarget & STI
): TargetLowering(TM), STI(STI) {
    addRegisterClass(MVT::i32, &ProgMinerLab::GPRRegClass);

    computeRegisterProperties(STI.getRegisterInfo());

/*
    // TODO
    setStackPointerRegisterToSaveRestore(ProgMinerLab::R1);

    // setSchedulingPreference(Sched::Source);

    for (unsigned Opc = 0; Opc < ISD::BUILTIN_OP_END; ++Opc) {
      setOperationAction(Opc, MVT::i32, Expand);
    }

    setOperationAction(ISD::ADD, MVT::i32, Legal);
    setOperationAction(ISD::MUL, MVT::i32, Legal);
    // ...
    setOperationAction(ISD::LOAD, MVT::i32, Legal);
    setOperationAction(ISD::STORE, MVT::i32, Legal);

    setOperationAction(ISD::Constant, MVT::i32, Legal);
    setOperationAction(ISD::UNDEF, MVT::i32, Legal);

    setOperationAction(ISD::BR_CC, MVT::i32, Custom);

    setOperationAction(ISD::FRAMEADDR, MVT::i32, Legal);
    // setOperationAction(ISD::FrameIndex, MVT::i32, Custom);
    // setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
*/
}

const char * ProgMinerLabTargetLowering::getTargetNodeName(unsigned Opcode) const {
    switch (Opcode) {
    case ProgMinerLabISD::NOP: return "ProgMinerLabISD::NOP";
    /*
    case ProgMinerLabISD::CALL: return "ProgMinerLabISD::CALL";
    case ProgMinerLabISD::RET: return "ProgMinerLabISD::RET";
    */
    default: return nullptr;
    }
}

SDValue ProgMinerLabTargetLowering::LowerFormalArguments(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> & Ins,
    const SDLoc & DL,
    SelectionDAG & DAG,
    SmallVectorImpl<SDValue> & InVals
) const {
    return Chain; // TODO
}

SDValue ProgMinerLabTargetLowering::LowerReturn(
    SDValue Chain,
    CallingConv::ID CallConv,
    bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> & Outs,
    const SmallVectorImpl<SDValue> & OutVals,
    const SDLoc & DL,
    SelectionDAG & DAG
) const {
    return Chain; // TODO
}

//===----------------------------------------------------------------------===//
//  Addressing mode description hooks
//===----------------------------------------------------------------------===//

// TODO: verify
bool ProgMinerLabTargetLowering::isLegalAddressingMode(
    const DataLayout & DL,
    const AddrMode & AM,
    Type * Ty,
    unsigned AS,
    Instruction * I
) const {
    llvm_unreachable("isLegalAddressingMode"); // TODO

/*
    // no global is ever allowed as a base
    if (AM.BaseGV) {
        return false;
    }

    if (!isInt<16>(AM.BaseOffs)) {
        return false;
    }

    switch (AM.Scale) {
    case 0: // "r+i" or just "i", depending on HasBaseReg.
        break;

    case 1:
        if (!AM.HasBaseReg) { // allow "r+i".
            break;
        }

        return false; // disallow "r+r" or "r+r+i".
    default:
        return false;
    }

    return true;
*/
}
