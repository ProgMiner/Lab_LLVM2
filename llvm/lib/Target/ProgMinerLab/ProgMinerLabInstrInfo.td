//===----------------------------------------------------------------------===//
//  ProgMinerLab profiles and nodes
//===----------------------------------------------------------------------===//

def ProgMinerLab_ret : SDNode<"ProgMinerLabISD::RET", SDTNone,
    [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
//  ProgMinerLab instructions
//===----------------------------------------------------------------------===//

include "ProgMinerLabInstrFormats.td"
include "ProgMinerLabOperands.td"

// Constant loading

let isMoveImm = true in {

    def CONSTs : SmallConstRegInst<0x2, (outs GPR:$reg), (ins imm8:$imm), "CONSTs $reg, $imm",
        [(set GPR:$reg, imm8:$imm)]>;

    def CONSTl : LargeConstRegInst<0x3, (outs GPR:$reg), (ins imm32:$imm), "CONSTl $reg, $imm",
        [(set GPR:$reg, imm32:$imm)]>;
}

// Arithmetic operations

let Constraints = "$res = $reg1" in {

    def ADD : BinaryInst<0x10, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "ADD $reg1, $reg2", []>;
    def SUB : BinaryInst<0x11, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "SUB $reg1, $reg2", []>;
    def MUL : BinaryInst<0x12, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "MUL $reg1, $reg2", []>;
    def DIV : BinaryInst<0x13, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "DIV $reg1, $reg2", []>;
    def REM : BinaryInst<0x14, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "REM $reg1, $reg2", []>;
    def AND : BinaryInst<0x15, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "AND $reg1, $reg2", []>;
    def OR : BinaryInst<0x16, (outs GPR:$res), (ins GPR:$reg1, GPR:$reg2), "OR $reg1, $reg2", []>;
}

// Memory operations

let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
def LOAD : BinaryInst<0x1C, (outs GPR:$reg1), (ins GPR:$reg2), "LOAD $reg1, $reg2", []>;
// def : Pat<(i32 (load BaseAddr:$r2)), (LDI BaseAddr:$r2, 0)>;
// def : Pat<(i32 (load (add BaseAddr:$r2, simm16:$r3_imm))),
//           (LDI BaseAddr:$r2, simm16:$r3_imm)>;
// def : Pat<(i32 (load (IsOrAdd AddrFI:$r2, simm16:$r3_imm))),
//           (LDI AddrFI:$r2, simm16:$r3_imm)>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
def STORE : BinaryInst<0x1D, (outs), (ins GPR:$reg1, GPR:$reg2), "STORE $reg1, $reg2", []>;
// def : Pat<(store (i32 GPR:$r1), BaseAddr:$r2),
//         (STI_ GPR:$r1, BaseAddr:$r2, 0)>;
// def : Pat<(store (i32 GPR:$r1), (add BaseAddr:$r2, simm16:$r3_imm)),
//         (STI_ GPR:$r1, BaseAddr:$r2, simm16:$r3_imm)>;
// def : Pat<(store (i32 GPR:$r1), (IsOrAdd AddrFI:$r2, simm16:$r3_imm)),
//         (STI_ GPR:$r1, AddrFI:$r2, simm16:$r3_imm)>;

// Control flow

let isBarrier = 1, isReturn = 1, isTerminator = 1, Uses = [SP] in
def RET : SimpleInst<0x01, (outs), (ins), "RET", [(ProgMinerLab_ret)]>;

// Misc

def NOP : SimpleInst<0x00, (outs), (ins), "NOP", []>;

def PIXEL_OFF : BinaryInst<0x1E, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_OFF $reg1, $reg2",
    [(int_progminerlab_pixeloff GPR:$reg1, GPR:$reg2)]>;

def PIXEL_ON : BinaryInst<0x1F, (outs), (ins GPR:$reg1, GPR:$reg2), "PIXEL_ON $reg1, $reg2",
    [(int_progminerlab_pixelon GPR:$reg1, GPR:$reg2)]>;

def FLUSH : SimpleInst<0x02, (outs), (ins), "FLUSH", [(int_progminerlab_flush)]>;

def RAND : UnaryInst<0x6, (outs GPR:$reg), (ins), "RAND $reg",
    [(set GPR:$reg, (int_progminerlab_rand))]>;
