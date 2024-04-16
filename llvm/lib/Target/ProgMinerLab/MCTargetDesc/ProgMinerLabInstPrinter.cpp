#include "ProgMinerLabInstPrinter.h"

#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"


using namespace llvm;


// #define DEBUG_TYPE "asm-printer"

#include "ProgMinerLabGenAsmWriter.inc"


void ProgMinerLabInstPrinter::printRegName(raw_ostream & O, MCRegister Reg) const {
    O << getRegisterName(Reg);
}

void ProgMinerLabInstPrinter::printInst(
    const MCInst * MI,
    uint64_t Address,
    StringRef Annot,
    const MCSubtargetInfo & STI,
    raw_ostream & O
) {
    printInstruction(MI, Address, O);
    printAnnotation(O, Annot);
}

void ProgMinerLabInstPrinter::printOperand(const MCInst * MI, int OpNo, raw_ostream & O) {
    const MCOperand & MO = MI->getOperand(OpNo);

    if (MO.isReg()) {
        printRegName(O, MO.getReg());
        return;
    }

    if (MO.isImm()) {
        O << MO.getImm();
        return;
    }

    assert(MO.isExpr() && "Unknown operand kind in printOperand");
    MO.getExpr()->print(O, &MAI);
}

void ProgMinerLabInstPrinter::printImm8AsmOperand(const MCInst * MI, int OpNum, raw_ostream & O) {
    if (MI->getOperand(OpNum).isImm()) {
        const int64_t Value = MI->getOperand(OpNum).getImm();

        assert(isInt<8>(Value) && "Invalid argument, value must be in range [-128, 127]");
        O << Value;
    } else {
        printOperand(MI, OpNum, O);
    }
}

void ProgMinerLabInstPrinter::printImm32AsmOperand(const MCInst * MI, int OpNum, raw_ostream & O) {
    if (MI->getOperand(OpNum).isImm()) {
        const int64_t Value = MI->getOperand(OpNum).getImm();

        assert(isInt<32>(Value) && "Invalid argument, value must be 32-bit");
        O << Value;
    } else {
        printOperand(MI, OpNum, O);
    }
}
