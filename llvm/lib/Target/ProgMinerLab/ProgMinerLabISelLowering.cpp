#include "ProgMinerLabISelLowering.h"

#include "llvm/CodeGen/CallingConvLower.h"

#include "MCTargetDesc/ProgMinerLabMCTargetDesc.h"
#include "ProgMinerLabRegisterInfo.h"
#include "ProgMinerLabSubtarget.h"

using namespace llvm;


ProgMinerLabTargetLowering::ProgMinerLabTargetLowering(
    const TargetMachine & TM,
    const ProgMinerLabSubtarget & STI
): TargetLowering(TM), STI(STI) {
    addRegisterClass(MVT::i32, &ProgMinerLab::GPRRegClass);

    computeRegisterProperties(STI.getRegisterInfo());

    setStackPointerRegisterToSaveRestore(ProgMinerLab::SP);

    setSchedulingPreference(Sched::Source);

/*
    // TODO

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
    case ProgMinerLabISD::RET: return "ProgMinerLabISD::RET";
    /*
    case ProgMinerLabISD::CALL: return "ProgMinerLabISD::CALL";
    */
    default: return nullptr;
    }
}

//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//

#include "ProgMinerLabGenCallingConv.inc"

//===----------------------------------------------------------------------===//
//  Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

// TODO: rewrite
static SDValue convertValVTToLocVT(
    SelectionDAG & DAG,
    SDValue Val,
    const CCValAssign & VA,
    const SDLoc & DL,
    const ProgMinerLabSubtarget & Subtarget
) {
    EVT LocVT = VA.getLocVT();

    if (VA.getValVT() == MVT::f32) {
        llvm_unreachable("");
    }

    switch (VA.getLocInfo()) {
    case CCValAssign::Full:
        break;

    case CCValAssign::BCvt:
        llvm_unreachable("");
        Val = DAG.getNode(ISD::BITCAST, DL, LocVT, Val);
        break;

    default:
        llvm_unreachable("Unexpected CCValAssign::LocInfo");
    }

    return Val;
}

// convert Val to a ValVT
// should not be called for CCValAssign::Indirect values
// TODO: rewrite
static SDValue convertLocVTToValVT(
    SelectionDAG & DAG,
    SDValue Val,
    const CCValAssign & VA,
    const SDLoc & DL,
    const ProgMinerLabSubtarget & Subtarget
) {
    if (VA.getValVT() == MVT::f32) {
        llvm_unreachable("");
    }

    switch (VA.getLocInfo()) {
    case CCValAssign::Full:
        break;

    case CCValAssign::BCvt:
        llvm_unreachable("");
        Val = DAG.getNode(ISD::BITCAST, DL, VA.getValVT(), Val);
        break;

    default:
        llvm_unreachable("Unexpected CCValAssign::LocInfo");
    }

    return Val;
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

//===----------------------------------------------------------------------===//
//  Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

bool ProgMinerLabTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv,
    MachineFunction & MF,
    bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> & Outs,
    LLVMContext & Context
) const {
    SmallVector<CCValAssign, 16> RVLocs;

    CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, Context);

    if (!CCInfo.CheckReturn(Outs, RetCC_ProgMinerLab)) {
        return false;
    }

    if (CCInfo.getStackSize() != 0 && IsVarArg) {
        llvm_unreachable("stack size != 0 and is vararg"); // TODO: what for
    }

    return true;
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
    const MachineFunction & MF = DAG.getMachineFunction();
    const ProgMinerLabSubtarget & STI = MF.getSubtarget<ProgMinerLabSubtarget>();

    // stores the assignment of the return value to a location
    SmallVector<CCValAssign, 16> RVLocs;

    // info about the registers and stack slot
    CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs, *DAG.getContext());

    CCInfo.AnalyzeReturn(Outs, RetCC_ProgMinerLab);

    SDValue Glue;
    SmallVector<SDValue, 4> RetOps(1, Chain);

    // copy the result values into the output registers
    for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
        CCValAssign &VA = RVLocs[i];
        SDValue Val = OutVals[i];

        // TODO: what for?
        assert(VA.isRegLoc() && "Can only return in registers!");

        Val = convertValVTToLocVT(DAG, Val, VA, DL, STI);
        Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Glue);

        // guarantee that all emitted copies are stuck together
        Glue = Chain.getValue(1);
        RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
    }

    RetOps[0] = Chain; // update chain

    // add the glue node if we have it
    if (Glue.getNode()) {
        RetOps.push_back(Glue);
    }

    return DAG.getNode(ProgMinerLabISD::RET, DL, MVT::Other, RetOps);
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
