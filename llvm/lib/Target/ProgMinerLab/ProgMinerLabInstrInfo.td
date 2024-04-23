//===----------------------------------------------------------------------===//
//  ProgMinerLab profiles and nodes
//===----------------------------------------------------------------------===//

def SDT_ProgMinerLabCall : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def SDT_ProgMinerLabCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_ProgMinerLabCallSeqEnd : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;

def progminerlab_call : SDNode<"ProgMinerLabISD::CALL", SDT_ProgMinerLabCall,
    [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic]>;

def progminerlab_ret : SDNode<"ProgMinerLabISD::RET", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// these are target-independent nodes, but have target-specific formats
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_ProgMinerLabCallSeqStart,
    [SDNPHasChain, SDNPOutGlue]>;

def callseq_end : SDNode<"ISD::CALLSEQ_END", SDT_ProgMinerLabCallSeqEnd,
    [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
//  ProgMinerLab instructions
//===----------------------------------------------------------------------===//

include "ProgMinerLabInstrFormats.td"
include "ProgMinerLabOperands.td"

// Constant loading

let isMoveImm = 1 in {

    def CONSTs : SmallConstRegInst<0x3, (outs GPR:$reg), (ins imm8:$imm), "CONSTs $reg, $imm",
        [(set GPR:$reg, imm8:$imm)]>;

    def CONSTl : LargeConstRegInst<0x4, (outs GPR:$reg), (ins imm32:$imm), "CONSTl $reg, $imm",
        [(set GPR:$reg, imm32:$imm)]>;
}

// Arithmetic operations

let isMoveReg = 1 in
def MOV : BinaryInst<0x10, (outs GPR:$reg1), (ins GPR:$reg2), "MOV $reg1, $reg2", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, Constraints = "$res = $reg1" in
class Arith<bits<8> op> : BinaryInst<op, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2),
    NAME#" $reg1, $reg2", []>;

let isAdd = 1 in
def ADD : Arith<0x11>;
def SUB : Arith<0x12>;
def MUL : Arith<0x13>;
def DIV : Arith<0x14>;
def REM : Arith<0x15>;
def AND : Arith<0x16>;
def OR  : Arith<0x17>;
def XOR : Arith<0x18>;
def SHL : Arith<0x19>;
def SHR : Arith<0x1A>;
def ASR : Arith<0x1B>;

class Compare<bits<8> op> : Arith<op> {

    let isCompare = 1;
}

// second digit corresponds to CondCode numeration
def CMPeq : Compare<0x21>;
def CMPgt : Compare<0x22>;
def CMPge : Compare<0x23>;
def CMPlt : Compare<0x24>;
def CMPle : Compare<0x25>;
def CMPne : Compare<0x26>;

def : Pat<(add GPR:$reg1, GPR:$reg2), (ADD GPR:$reg1, GPR:$reg2)>;
def : Pat<(sub GPR:$reg1, GPR:$reg2), (SUB GPR:$reg1, GPR:$reg2)>;
def : Pat<(mul GPR:$reg1, GPR:$reg2), (MUL GPR:$reg1, GPR:$reg2)>;
def : Pat<(sdiv GPR:$reg1, GPR:$reg2), (DIV GPR:$reg1, GPR:$reg2)>;
def : Pat<(srem GPR:$reg1, GPR:$reg2), (REM GPR:$reg1, GPR:$reg2)>;
def : Pat<(and GPR:$reg1, GPR:$reg2), (AND GPR:$reg1, GPR:$reg2)>;
def : Pat<(or GPR:$reg1, GPR:$reg2), (OR GPR:$reg1, GPR:$reg2)>;
def : Pat<(xor GPR:$reg1, GPR:$reg2), (XOR GPR:$reg1, GPR:$reg2)>;
def : Pat<(shl GPR:$reg1, GPR:$reg2), (SHL GPR:$reg1, GPR:$reg2)>;
def : Pat<(srl GPR:$reg1, GPR:$reg2), (SHR GPR:$reg1, GPR:$reg2)>;
def : Pat<(sra GPR:$reg1, GPR:$reg2), (ASR GPR:$reg1, GPR:$reg2)>;

class PatCC<CondCode CC, ProgMinerLabInst CMP> : Pat<(setcc GPR:$reg1, GPR:$reg2, CC),
    (CMP GPR:$reg1, GPR:$reg2)>;

def : PatCC<SETEQ, CMPeq>;
def : PatCC<SETGT, CMPgt>;
def : PatCC<SETGE, CMPge>;
def : PatCC<SETLT, CMPlt>;
def : PatCC<SETLE, CMPle>;
def : PatCC<SETNE, CMPne>;

def : PatCC<SETUEQ, CMPeq>;
def : PatCC<SETUGT, CMPgt>;
def : PatCC<SETUGE, CMPge>;
def : PatCC<SETULT, CMPlt>;
def : PatCC<SETULE, CMPle>;
def : PatCC<SETUNE, CMPne>;

// Memory operations

let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
def LOAD : BinaryInst<0x1C, (outs GPR:$reg1), (ins GPR:$reg2), "LOAD $reg1, $reg2", []>;
def : Pat<(i32 (load memAddr:$reg2)), (LOAD memAddr:$reg2)>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
def STORE : BinaryInst<0x1D, (outs), (ins GPR:$reg1, GPR:$reg2), "STORE $reg1, $reg2", []>;
def : Pat<(store (i32 GPR:$reg1), memAddr:$reg2), (STORE memAddr:$reg2, GPR:$reg1)>;

// Control flow

let hasSideEffects = 0, mayLoad = 0, mayStore = 0,
    isBranch = 1, isBarrier = 1, isTerminator = 1
in {

    def JMP : LargeConstInst<0x01, (outs), (ins jumpTarget:$imm), "JMP $imm", [(br bb:$imm)]>;

    def IFZ : LargeConstRegInst<0x5, (outs), (ins GPR:$reg, jumpTarget:$imm), "IFZ $reg, $imm", []>;
}

def : Pat<(brcond GPR:$reg, bb:$imm), (IFZ (CMPeq GPR:$reg, 0), jumpTarget:$imm)>;

class PatBrCC<CondCode CC, ProgMinerLabInst CMP> : Pat<(brcc CC, GPR:$reg1, GPR:$reg2, bb:$imm),
    (IFZ (CMP GPR:$reg1, GPR:$reg2), jumpTarget:$imm)>;

// inversed comparison due to IFZ
def : PatBrCC<SETEQ, CMPne>;
def : PatBrCC<SETGT, CMPle>;
def : PatBrCC<SETGE, CMPlt>;
def : PatBrCC<SETLT, CMPge>;
def : PatBrCC<SETLE, CMPgt>;
def : PatBrCC<SETNE, CMPeq>;

def : PatBrCC<SETUEQ, CMPne>;
def : PatBrCC<SETUGT, CMPle>;
def : PatBrCC<SETUGE, CMPlt>;
def : PatBrCC<SETULT, CMPge>;
def : PatBrCC<SETULE, CMPgt>;
def : PatBrCC<SETUNE, CMPeq>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1 in {

    let isCall = 1 in
    def CALL : LargeConstInst<0x03, (outs), (ins callTarget:$imm), "CALL $imm",
        [(progminerlab_call tglobaladdr:$imm)]>;

    let isReturn = 1, isTerminator = 1 in
    def RET : SimpleInst<0x04, (outs), (ins), "RET", [(progminerlab_ret)]>;
}

// Misc

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def NOP : SimpleInst<0x00, (outs), (ins), "NOP", []>;

let hasSideEffects = 1, mayLoad = 0, mayStore = 0 in {

    let isAsCheapAsAMove = 1 in
    def PIXEL_OFF : BinaryInst<0x1E, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_OFF $reg1, $reg2",
        [(int_progminerlab_pixeloff GPR:$reg1, GPR:$reg2)]>;

    let isAsCheapAsAMove = 1 in
    def PIXEL_ON : BinaryInst<0x1F, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_ON $reg1, $reg2",
        [(int_progminerlab_pixelon GPR:$reg1, GPR:$reg2)]>;

    let hasDelaySlot = 1 in
    def FLUSH : SimpleInst<0x02, (outs), (ins), "FLUSH", [(int_progminerlab_flush)]>;
}

let hasSideEffects = 1, mayLoad = 0, mayStore = 0, isAsCheapAsAMove = 1 in
def RAND : UnaryInst<0x6, (outs GPR:$reg), (ins), "RAND $reg",
    [(set GPR:$reg, (int_progminerlab_rand))]>;

let hasSideEffects = 1, mayLoad = 0, mayStore = 0 in
def DUMP : SmallConstRegInst<0x7, (outs), (ins imm8:$imm, GPR:$reg), "DUMP $imm, $reg",
    [(int_progminerlab_dump imm8:$imm, GPR:$reg)]>;

// Pseudo instructions

let Uses = [SP], Defs = [SP] in {

    def ADJCALLSTACKDOWN : ProgMinerLabPseudoInst<(outs), (ins i32imm:$amt, i32imm:$amt2),
        "# ADJCALLSTACKDOWN $amt, $amt2", [(callseq_start timm:$amt, timm:$amt2)]>;

    def ADJCALLSTACKUP : ProgMinerLabPseudoInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
        "# ADJCALLSTACKUP $amt1", [(callseq_end timm:$amt1, timm:$amt2)]>;
}
