//===----------------------------------------------------------------------===//
//  ProgMinerLab instruction operands
//===----------------------------------------------------------------------===//

// immediate operands with a shared generic render method
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
def imm8 : Immediate<i32, [{ return isInt<8>(Imm); }], "Imm8AsmOperand"> {

    let EncoderMethod = "getImm8OpValue";
    let DecoderMethod = "decodeImm8Operand";
}

// imm32 predicate - 32-bit immediate
def Imm32AsmOperand : ImmAsmOperand<"Imm32">;
def imm32 : Immediate<i32, [{ return isInt<32>(Imm); }], "Imm32AsmOperand"> {

    let EncoderMethod = "getImm32OpValue";
    let DecoderMethod = "decodeImm32Operand";
}

// jump target, 8-bit offset from instruction pointer
def JumpTargetAsmOperand : AsmOperandClass {

    let Name = "JumpTarget";
    let ParserMethod = "parseJumpTarget";
    let RenderMethod = "addImmOperands";
    let PredicateMethod = "isJumpTarget";
}

let PrintMethod = "printJumpTargetAsmOperand",
    ParserMatchClass = JumpTargetAsmOperand,
    EncoderMethod = "getJumpTargetOpValue",
    DecoderMethod = "decodeJumpTargetOperand",
    OperandType = "OPERAND_PCREL"
in {

    def jumpTarget : Operand<OtherVT>;

    def callTarget : Operand<i32>;
}

// memory address for load and store
def memAddr : ComplexPattern<iPTR, 1, "SelectMemAddr">;
