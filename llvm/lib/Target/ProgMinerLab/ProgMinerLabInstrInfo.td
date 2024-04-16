//===----------------------------------------------------------------------===//
// Sim profiles and nodes
//===----------------------------------------------------------------------===//

def ProgMinerLab_ret : SDNode<"ProgMinerLabISD::RET", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
//  ProgMinerLab instructions
//===----------------------------------------------------------------------===//

include "ProgMinerLabInstrFormats.td"
include "ProgMinerLabOperands.td"

// Misc instructions

def NOP : SimpleInst<0x00, (outs), (ins), "NOP", []>;

let isBarrier = 1, isReturn = 1, isTerminator = 1, Uses = [SP] in
def RET : SimpleInst<0x01, (outs), (ins), "RET", [(ProgMinerLab_ret)]>;

// Constant loading

let isMoveImm = true in {

    def CONSTs : SmallConstRegInst<0x2, (outs GPR:$reg), (ins imm8:$imm), "CONSTs $reg, $imm",
        [(set GPR:$reg, imm8:$imm)]>;

    def CONSTl : LargeConstRegInst<0x3, (outs GPR:$reg), (ins imm32:$imm), "CONSTl $reg, $imm",
        [(set GPR:$reg, imm32:$imm)]>;
}
