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

class SimpleInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst8<outs, ins, asmstr, pattern> {

    let Inst = op;
}

class ArithInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst16<outs, ins, asmstr, pattern> {

    bits<4> r1 = 0;
    bits<4> r2 = 0;

    let Inst{15-8} = op;
    let Inst{7-4} = r1;
    let Inst{3-0} = r2;
}

class SmallConstInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst16<outs, ins, asmstr, pattern> {

    bits<8> imm = 0;

    let Inst{15-8} = op;
    let Inst{7-0} = imm;
}

class LargeConstInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : ProgMinerLabInst40<outs, ins, asmstr, pattern> {

    bits<32> imm = 0;

    let Inst{39-32} = op;
    let Inst{31-0} = imm;
}

// Pseudo instructions format
class ProgMinerLabPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
   : ProgMinerLabInst<1, outs, ins, asmstr, pattern> {

  let isPseudo = 1;
  let isCodeGenOnly = 1;
}
