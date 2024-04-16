//===----------------------------------------------------------------------===//
//  ProgMinerLab instruction operands
//===----------------------------------------------------------------------===//

// Immediate operands with a shared generic render method.
class ImmAsmOperand<string name> : AsmOperandClass {

    let Name = name;
    let RenderMethod = "addImmOperands";
    let DiagnosticType = !strconcat("Invalid", name);
}

class Immediate<ValueType vt, code pred, string asmop> : Operand<vt>, ImmLeaf<vt, pred> {

    let PrintMethod = "print"#asmop;
    let ParserMatchClass = !cast<AsmOperandClass>(asmop);

    let OperandType = "OPERAND_IMMEDIATE";
}

// imm8 predicate - 8-bit immediate
def Imm8AsmOperand : ImmAsmOperand<"Imm8">;
def imm8 : Immediate<i32, [{ return -128 <= Imm && Imm < 128; }], "Imm8AsmOperand"> {

    let EncoderMethod = "getImm8OpValue";
    let DecoderMethod = "decodeImm8Operand";
}

// imm32 predicate - 32-bit immediate
def Imm32AsmOperand : ImmAsmOperand<"Imm32">;
def imm32 : Immediate<i32, [{ return INT32_MIN <= Imm && Imm <= INT32_MAX; }], "Imm32AsmOperand"> {

    let EncoderMethod = "getImm32OpValue";
    let DecoderMethod = "decodeImm32Operand";
}
