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

// Control flow

let isBarrier = 1, isReturn = 1, isTerminator = 1, Uses = [SP] in
def RET : SimpleInst<0x01, (outs), (ins), "RET", [(ProgMinerLab_ret)]>;

// Constant loading

let isMoveImm = true in {

    def CONSTs : SmallConstRegInst<0x2, (outs GPR:$reg), (ins imm8:$imm), "CONSTs $reg, $imm",
        [(set GPR:$reg, imm8:$imm)]>;

    def CONSTl : LargeConstRegInst<0x3, (outs GPR:$reg), (ins imm32:$imm), "CONSTl $reg, $imm",
        [(set GPR:$reg, imm32:$imm)]>;
}

// Misc

def NOP : SimpleInst<0x00, (outs), (ins), "NOP", []>;

def FLUSH : SimpleInst<0x02, (outs), (ins), "FLUSH", []>;

def PIXEL_OFF : BinaryInst<0x1E, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_OFF $reg1, $reg2", []>;

def PIXEL_ON : BinaryInst<0x1F, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_ON $reg1, $reg2", []>;
