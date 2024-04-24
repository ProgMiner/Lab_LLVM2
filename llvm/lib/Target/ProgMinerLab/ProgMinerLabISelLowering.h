#pragma once

#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"


namespace llvm {

class ProgMinerLabSubtarget;
class ProgMinerLabTargetMachine;

namespace ProgMinerLabISD {

enum NodeType : unsigned {

    // start the numbering where the builtin ops and target ops leave off
    FIRST_NUMBER = ISD::BUILTIN_OP_END,

    CALL,
    RET,
};

} // namespace ProgMinerLabISD

class ProgMinerLabTargetLowering : public TargetLowering {

    const ProgMinerLabSubtarget & STI;

public:

    explicit ProgMinerLabTargetLowering(
        const TargetMachine & TM,
        const ProgMinerLabSubtarget & STI
    );

    // provide custom lowering hooks for some operations
    SDValue LowerOperation(SDValue Op, SelectionDAG & DAG) const override;

    // this method returns the name of a target specific DAG node
    const char * getTargetNodeName(unsigned Opcode) const override;

    // return true if the addressing mode represented by AM is legal for this target,
    // for a load/store of the specified type
    bool isLegalAddressingMode(
        const DataLayout & DL,
        const AddrMode & AM,
        Type * Ty,
        unsigned AS,
        Instruction * I = nullptr
    ) const override;

    ProgMinerLabSubtarget const & getSubtarget() const {
        return STI;
    }

private:

    SDValue LowerFormalArguments(
        SDValue Chain,
        CallingConv::ID CallConv,
        bool IsVarArg,
        const SmallVectorImpl<ISD::InputArg> & Ins,
        const SDLoc & DL,
        SelectionDAG & DAG,
        SmallVectorImpl<SDValue> & InVals
    ) const override;

    bool CanLowerReturn(
        CallingConv::ID CallConv,
        MachineFunction & MF,
        bool IsVarArg,
        const SmallVectorImpl<ISD::OutputArg> & ArgsFlags,
        LLVMContext & Context
    ) const override;

    SDValue LowerReturn(
        SDValue Chain,
        CallingConv::ID CallConv,
        bool IsVarArg,
        const SmallVectorImpl<ISD::OutputArg> & Outs,
        const SmallVectorImpl<SDValue> & OutVals,
        const SDLoc & DL,
        SelectionDAG & DAG
    ) const override;

    SDValue LowerCall(
        TargetLowering::CallLoweringInfo & CLI,
        SmallVectorImpl<SDValue> & InVals
    ) const override;

    bool mayBeEmittedAsTailCall(const CallInst * CI) const override {
        return false;
    }

    SDValue lowerBRCOND(SDValue Op, SelectionDAG & DAG) const;
    SDValue lowerSELECT(SDValue Op, SelectionDAG & DAG) const;

/*
    void ReplaceNodeResults(
        SDNode * N,
        SmallVectorImpl<SDValue> & Results,
        SelectionDAG & DAG
    ) const override;

    SDValue PerformDAGCombine(SDNode * N, DAGCombinerInfo & DCI) const override;
*/
};

} // namespace llvm
