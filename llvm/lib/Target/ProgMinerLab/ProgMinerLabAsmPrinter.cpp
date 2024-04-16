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

// TODO: verify
static MCOperand lowerSymbolOperand(
    const MachineOperand & MO,
    MCSymbol * Sym,
    const AsmPrinter & AP
) {
    MCContext & Ctx = AP.OutContext;

    const MCExpr * ME = MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);

    if (!MO.isJTI() && !MO.isMBB() && MO.getOffset()) {
        ME = MCBinaryExpr::createAdd(ME, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
    }

    return MCOperand::createExpr(ME);
}

static bool lowerProgMinerLabMachineOperandToMCOperand(
    const MachineOperand & MO,
    MCOperand & MCOp,
    const AsmPrinter & AP
) {
    switch (MO.getType()) {
    case MachineOperand::MO_Register:
        // ignore all implicit register operands
        if (MO.isImplicit()) {
            return false;
        }

        MCOp = MCOperand::createReg(MO.getReg());
        break;

    case MachineOperand::MO_RegisterMask:
        // regmasks are like implicit defs
        return false;

    case MachineOperand::MO_Immediate:
        MCOp = MCOperand::createImm(MO.getImm());
        break;

    case MachineOperand::MO_MachineBasicBlock:
        MCOp = lowerSymbolOperand(MO, MO.getMBB()->getSymbol(), AP);
        break;

    case MachineOperand::MO_GlobalAddress:
        MCOp = lowerSymbolOperand(MO, AP.getSymbolPreferLocal(*MO.getGlobal()), AP);
        break;

    case MachineOperand::MO_BlockAddress:
        MCOp = lowerSymbolOperand(MO, AP.GetBlockAddressSymbol(MO.getBlockAddress()), AP);
        break;

    case MachineOperand::MO_ExternalSymbol:
        MCOp = lowerSymbolOperand(MO, AP.GetExternalSymbolSymbol(MO.getSymbolName()), AP);
        break;

    case MachineOperand::MO_ConstantPoolIndex:
        MCOp = lowerSymbolOperand(MO, AP.GetCPISymbol(MO.getIndex()), AP);
        break;

    case MachineOperand::MO_JumpTableIndex:
        MCOp = lowerSymbolOperand(MO, AP.GetJTISymbol(MO.getIndex()), AP);
        break;

    default:
        report_fatal_error("lowerProgMinerLabMachineInstrToMCInst: unknown operand type");
    }

    return true;
}

static bool lowerProgMinerLabMachineInstrToMCInst(
    const MachineInstr * MI,
    MCInst & OutMI,
    AsmPrinter & AP
) {
    OutMI.setOpcode(MI->getOpcode());

    for (const MachineOperand & MO : MI->operands()) {
        MCOperand MCOp;

        if (lowerProgMinerLabMachineOperandToMCOperand(MO, MCOp, AP)) {
            OutMI.addOperand(MCOp);
        }
    }

    return false;
}

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

        MCInst TmpInst;
        if (!lowerProgMinerLabMachineInstrToMCInst(MI, TmpInst, *this)) {
            EmitToStreamer(*OutStreamer, TmpInst);
        }
    }

    // Used in pseudo lowerings
    bool lowerOperand(const MachineOperand & MO, MCOperand & MCOp) const {
        return lowerProgMinerLabMachineOperandToMCOperand(MO, MCOp, *this);
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
