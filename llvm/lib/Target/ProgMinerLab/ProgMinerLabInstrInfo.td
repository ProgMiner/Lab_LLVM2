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

def CONSTs : SmallConstInst<?, (outs GPR:$r), (ins imm8:$imm), "CONSTs $r, $imm",
    [(set GPR:$r, imm8:$imm)]> {

    bits<4> r = 0;

    let Inst{15-12} = 2;
    let Inst{11-8} = r;
}

def CONSTl : LargeConstInst<?, (outs GPR:$r), (ins imm32:$imm), "CONSTl $r, $imm",
    [(set GPR:$r, imm32:$imm)]> {

    bits<4> r = 0;

    let Inst{39-36} = 3;
    let Inst{35-32} = r;
}
