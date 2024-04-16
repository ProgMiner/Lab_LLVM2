//===----------------------------------------------------------------------===//
//  ProgMinerLab instructions format
//
//  Simple instructions:
//
//  FF
//  ^^ - opcode - operation code
//
//  Arithmetic instructions:
//
//  FF FF
//      ^ - r2     - second reg
//     ^  - r1     - first reg
//  ^^    - opcode - operation code
//
//  Small constant instructions:
//
//  FF FF
//     ^^ - imm8   - constant operand
//  ^^    - opcode - operation code
//
//  Large constant instructions:
//
//  FF FF FF FF FF
//     ^^ ^^ ^^ ^^ - imm32  - constant operand
//  ^^             - opcode - operation code
//
//===----------------------------------------------------------------------===//

// Generic instruction format
class ProgMinerLabInst<int size, dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {

    // Inst and Size: for tablegen(... -gen-emitter) and
    // tablegen(... -gen-disassembler) in CMakeLists.txt
    let Namespace = "ProgMinerLab";
    let DecoderNamespace = "ProgMinerLab";

    let Size = size;

    let OutOperandList = outs;
    let InOperandList  = ins;

    let AsmString = asmstr;
    let Pattern   = pattern;
}

// Base classes for different sizes formats

class ProgMinerLabInst8<dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst<1, outs, ins, asmstr, pattern> {

    field bits<8> Inst;
}

class ProgMinerLabInst16<dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst<2, outs, ins, asmstr, pattern> {

    field bits<16> Inst;
}

class ProgMinerLabInst40<dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst<5, outs, ins, asmstr, pattern> {

    field bits<40> Inst;
}

// Instruction formats

// simple instruction - 8-bit opcode without operands
class SimpleInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst8<outs, ins, asmstr, pattern> {

    let Inst = op;
}

// unary instruction - 4-bit opcode + register operand
class UnaryInst<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst8<outs, ins, asmstr, pattern> {

    bits<4> reg;

    let Inst{7-4} = reg;
    let Inst{3-0} = op;
}

// binary instruction - 8-bit opcode + 2 register operands
class BinaryInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst16<outs, ins, asmstr, pattern> {

    bits<4> reg1;
    bits<4> reg2;

    let Inst{15-12} = reg2;
    let Inst{11-8} = reg1;
    let Inst{7-0} = op;
}

// small constant instruction - 8-bit opcode + 8-bit constant
class SmallConstInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst16<outs, ins, asmstr, pattern> {

    bits<8> imm;

    let Inst{15-8} = imm;
    let Inst{7-0} = op;
}

// large constant instruction - 8-bit opcode + 32-bit constant
class LargeConstInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst40<outs, ins, asmstr, pattern> {

    bits<32> imm;

    let Inst{39-8} = imm;
    let Inst{7-0} = op;
}

// small constant register instruction - 4-bit opcode + register + 8-bit constant
class SmallConstRegInst<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst16<outs, ins, asmstr, pattern> {

    bits<8> imm;
    bits<4> reg;

    let Inst{15-8} = imm;
    let Inst{7-4} = reg;
    let Inst{3-0} = op;
}

// large constant register instruction - 4-bit opcode + register + 32-bit constant
class LargeConstRegInst<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst40<outs, ins, asmstr, pattern> {

    bits<32> imm;
    bits<4> reg;

    let Inst{39-8} = imm;
    let Inst{7-4} = reg;
    let Inst{3-0} = op;
}

// pseudo instruction
class ProgMinerLabPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
   : ProgMinerLabInst<1, outs, ins, asmstr, pattern> {

  let isPseudo = 1;
  let isCodeGenOnly = 1;
}
