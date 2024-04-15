#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCInst.h"

#include "MCTargetDesc/ProgMinerLabInstPrinter.h"
#include "TargetInfo/ProgMinerLabTargetInfo.h"
#include "ProgMinerLabTargetMachine.h"
#include "ProgMinerLabSubtarget.h"


using namespace llvm;


// #define DEBUG_TYPE "asm-printer"


namespace {

class ProgMinerLabAsmPrinter : public AsmPrinter {

    const MCSubtargetInfo * STI;

public:

    explicit ProgMinerLabAsmPrinter(TargetMachine & TM, std::unique_ptr<MCStreamer> Streamer)
        : AsmPrinter(TM, std::move(Streamer)), STI(TM.getMCSubtargetInfo()) {}

    StringRef getPassName() const override {
        return "ProgMinerLab Assembly Printer";
    }

    bool emitPseudoExpansionLowering(MCStreamer & OutStreamer, const MachineInstr * MI);

    void emitInstruction(const MachineInstr * MI) override {
        // do any auto-generated pseudo lowerings

        if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
            return;
        }

        llvm_unreachable("emitInstruction"); // TODO

/*
        MCInst TmpInst;
        if (!lowerProgMinerLabMachineInstrToMCInst(MI, TmpInst, *this)) {
            EmitToStreamer(*OutStreamer, TmpInst);
        }
*/
    }

    // Used in pseudo lowerings
    bool lowerOperand(const MachineOperand & MO, MCOperand & MCOp) const {
        llvm_unreachable("lowerOperand"); // TODO

        // return lowerProgMinerLabMachineOperandToMCOperand(MO, MCOp, *this);
    }
};

} // anonymous namespace


// simple pseudo-instructions have their lowering
// (with expansion to real instructions) auto-generated
#include "ProgMinerLabGenMCPseudoLowering.inc"


// force static initialization
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeProgMinerLabAsmPrinter() {
    RegisterAsmPrinter<ProgMinerLabAsmPrinter> X(getTheProgMinerLabTarget());
}
