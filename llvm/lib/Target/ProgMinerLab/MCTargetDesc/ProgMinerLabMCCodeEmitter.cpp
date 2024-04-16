#include "ProgMinerLabMCTargetDesc.h"

#include <cassert>
#include <cstdint>

#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Endian.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/TargetParser/SubtargetFeature.h"


using namespace llvm;


#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");


namespace {

class ProgMinerLabMCCodeEmitter : public MCCodeEmitter {

    const MCInstrInfo & InstrInfo;
    MCContext & Ctx;

public:

    ProgMinerLabMCCodeEmitter(const MCInstrInfo & InstrInfo, MCContext & ctx)
        : InstrInfo(InstrInfo), Ctx(ctx) {}

    ProgMinerLabMCCodeEmitter(const ProgMinerLabMCCodeEmitter &) = delete;
    ProgMinerLabMCCodeEmitter &operator=(const ProgMinerLabMCCodeEmitter &) = delete;

    ~ProgMinerLabMCCodeEmitter() override = default;

    void encodeInstruction(
        const MCInst & MI,
        SmallVectorImpl<char> & CB,
        SmallVectorImpl<MCFixup> & Fixups,
        const MCSubtargetInfo & STI
    ) const override {
        const unsigned size = InstrInfo.get(MI.getOpcode()).getSize();
        const uint64_t instr = getBinaryCodeForInstr(MI, Fixups, STI);
        emitInstr(CB, size, instr);

        // keep track of the # of mi's emitted
        ++MCNumEmitted;
    }

    // getBinaryCodeForInstr - TableGen'erated function
    // for getting the binary encoding for an instruction
    uint64_t getBinaryCodeForInstr(
        const MCInst & MI,
        SmallVectorImpl<MCFixup> & Fixups,
        const MCSubtargetInfo & STI
    ) const;

    // getMachineOpValue - return binary encoding of operand
    // if the machine operand requires relocation, record the relocation and return zero
    unsigned getMachineOpValue(
        const MCInst & MI,
        const MCOperand & MO,
        SmallVectorImpl<MCFixup> & Fixups,
        const MCSubtargetInfo & STI
    ) const {
        if (MO.isReg()) {
            return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
        }

        if (MO.isImm()) {
            return MO.getImm();
        }

        assert(MO.isExpr());
        const MCExpr * Expr = MO.getExpr();

        int64_t Res;
        if (Expr->evaluateAsAbsolute(Res)) {
            return Res;
        }

        llvm_unreachable("unhandled expression!");
    }

    unsigned getImm8OpValue(
        const MCInst & MI,
        unsigned OpNo,
        SmallVectorImpl<MCFixup> & Fixups,
        const MCSubtargetInfo & STI
    ) const {
        const MCOperand & MO = MI.getOperand(OpNo);

        if (MO.isImm()) {
            return MO.getImm();
        }

        assert(MO.isExpr() && "not expression nor an immediate");

        const MCExpr * Expr = MO.getExpr();

        // constant value, no fixup is needed
        if (const MCConstantExpr * CE = dyn_cast<MCConstantExpr>(Expr)) {
            return CE->getValue();
        }

        return 0;
    }

    unsigned getImm32OpValue(
        const MCInst & MI,
        unsigned OpNo,
        SmallVectorImpl<MCFixup> & Fixups,
        const MCSubtargetInfo & STI
    ) const {
        const MCOperand & MO = MI.getOperand(OpNo);

        if (MO.isImm()) {
            return MO.getImm();
        }

        assert(MO.isExpr() && "not expression nor an immediate");

        const MCExpr * Expr = MO.getExpr();

        // constant value, no fixup is needed
        if (const MCConstantExpr * CE = dyn_cast<MCConstantExpr>(Expr)) {
            return CE->getValue();
        }

        return 0;
    }

private:

    static void emitInstr(SmallVectorImpl<char> & CB, unsigned size, uint64_t instr) {
        instr = support::endian::byte_swap(instr, endianness::little);

        const char * const buf = reinterpret_cast<const char *>(&instr);
        CB.append(buf, buf + size); // will work only with little endian encoding
    }
};

} // anonymous namespace

MCCodeEmitter * llvm::createProgMinerLabMCCodeEmitter(const MCInstrInfo & MCII, MCContext & Ctx) {
  return new ProgMinerLabMCCodeEmitter(MCII, Ctx);
}

#include "ProgMinerLabGenMCCodeEmitter.inc"
