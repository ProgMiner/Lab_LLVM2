//===----------------------------------------------------------------------===//
//  ProgMinerLab profiles and nodes
//===----------------------------------------------------------------------===//

def progminerlab_ret : SDNode<"ProgMinerLabISD::RET", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

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

def MOV : BinaryInst<0x10, (outs GPR:$reg1), (ins GPR:$reg2), "MOV $reg1, $reg2", []>;

let Constraints = "$res = $reg1" in {

    def ADD : BinaryInst<0x11, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "ADD $reg1, $reg2", []>;
    def SUB : BinaryInst<0x12, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "SUB $reg1, $reg2", []>;
    def MUL : BinaryInst<0x13, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "MUL $reg1, $reg2", []>;
    def DIV : BinaryInst<0x14, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "DIV $reg1, $reg2", []>;
    def REM : BinaryInst<0x15, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "REM $reg1, $reg2", []>;
    def AND : BinaryInst<0x16, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "AND $reg1, $reg2", []>;
    def OR : BinaryInst<0x17, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "OR $reg1, $reg2", []>;
    def XOR : BinaryInst<0x18, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "XOR $reg1, $reg2", []>;

    // second digit corresponds to CondCode numeration
    def CMPeq : BinaryInst<0x21, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "CMPeq $reg1, $reg2", []>;
    def CMPgt : BinaryInst<0x22, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "CMPgt $reg1, $reg2", []>;
    def CMPge : BinaryInst<0x23, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "CMPge $reg1, $reg2", []>;
    def CMPlt : BinaryInst<0x24, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "CMPlt $reg1, $reg2", []>;
    def CMPle : BinaryInst<0x25, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "CMPle $reg1, $reg2", []>;
    def CMPne : BinaryInst<0x26, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "CMPne $reg1, $reg2", []>;
}

// Memory operations

let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
def LOAD : BinaryInst<0x1C, (outs GPR:$reg1), (ins GPR:$reg2), "LOAD $reg1, $reg2", []>;
def : Pat<(i32 (load memAddr:$reg2)), (LOAD memAddr:$reg2)>;
// def : Pat<(i32 (load (add BaseAddr:$r2, simm16:$r3_imm))),
//           (LDI BaseAddr:$r2, simm16:$r3_imm)>;
// def : Pat<(i32 (load (IsOrAdd AddrFI:$r2, simm16:$r3_imm))),
//           (LDI AddrFI:$r2, simm16:$r3_imm)>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
def STORE : BinaryInst<0x1D, (outs), (ins GPR:$reg1, GPR:$reg2), "STORE $reg1, $reg2", []>;
def : Pat<(store (i32 GPR:$reg1), memAddr:$reg2), (STORE memAddr:$reg2, GPR:$reg1)>;
// def : Pat<(store (i32 GPR:$reg1), (add BaseAddr:$reg2, simm16:$r3_imm)),
//         (STI_ GPR:$r1, BaseAddr:$r2, simm16:$r3_imm)>;
// def : Pat<(store (i32 GPR:$r1), (IsOrAdd AddrFI:$r2, simm16:$r3_imm)),
//         (STI_ GPR:$r1, AddrFI:$r2, simm16:$r3_imm)>;

// Control flow

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBranch = 1, isTerminator = 1 in {

    let isBarrier = 1 in
    def JMPs : SmallConstInst<0x01, (outs), (ins jumpTarget:$imm), "JMPs $imm", [(br bb:$imm)]>;

    // def JMPl : LargeConstInst<0x02, (outs), (ins imm32:$imm), "JMPl $imm", [(br bb:$imm)]>;

    def IFZs : SmallConstRegInst<0x5, (outs), (ins GPR:$reg, jumpTarget:$imm), "IFZs $reg, $imm",
        [(brcond GPR:$reg, bb:$imm)]>;

    // def IFZl : SmallConstRegInst<0x4, (outs), (ins GPR:$reg, jumpTarget:$imm), "IFZl $reg, $imm",
    //     [(brcond GPR:$reg, bb:$imm)]>;
}

class PatBrCC<CondCode CC, ProgMinerLabInst CMP> : Pat<(brcc CC, GPR:$reg1, GPR:$reg2, bb:$imm),
    (IFZs (CMP GPR:$reg1, GPR:$reg2), jumpTarget:$imm)>;

// inversed comparison due to IFZ
def : PatBrCC<SETEQ, CMPne>;
def : PatBrCC<SETGT, CMPle>;
def : PatBrCC<SETGE, CMPlt>;
def : PatBrCC<SETLT, CMPge>;
def : PatBrCC<SETLE, CMPgt>;
def : PatBrCC<SETNE, CMPeq>;

let isBarrier = 1, isReturn = 1, isTerminator = 1, Uses = [SP] in
def RET : SimpleInst<0x03, (outs), (ins), "RET", [(progminerlab_ret)]>;

// Misc

def NOP : SimpleInst<0x00, (outs), (ins), "NOP", []>;

def PIXEL_OFF : BinaryInst<0x1E, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_OFF $reg1, $reg2",
    [(int_progminerlab_pixeloff GPR:$reg1, GPR:$reg2)]>;

def PIXEL_ON : BinaryInst<0x1F, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_ON $reg1, $reg2",
    [(int_progminerlab_pixelon GPR:$reg1, GPR:$reg2)]>;

def FLUSH : SimpleInst<0x02, (outs), (ins), "FLUSH", [(int_progminerlab_flush)]>;

def RAND : UnaryInst<0x6, (outs GPR:$reg), (ins), "RAND $reg",
    [(set GPR:$reg, (int_progminerlab_rand))]>;
