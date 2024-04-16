#include "ProgMinerLabMCTargetDesc.h"

#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCValue.h"
#include "llvm/MC/MCExpr.h"


using namespace llvm;


namespace {

class ProgMinerLabAsmBackend : public MCAsmBackend {

protected:

    const Target & TheTarget;

public:

    ProgMinerLabAsmBackend(const Target & T)
        : MCAsmBackend(endianness::little), TheTarget(T) {}

    unsigned getNumFixupKinds() const override {
        return 0;
    }

    // fixupNeedsRelaxation - target specific predicate for whether a given
    // fixup requires the associated instruction to be relaxed
    bool fixupNeedsRelaxation(
        const MCFixup & Fixup,
        uint64_t Value,
        const MCRelaxableFragment * DF,
        const MCAsmLayout & Layout
    ) const override {
        llvm_unreachable("fixupNeedsRelaxation");
    }

    bool writeNopData(
        raw_ostream & OS,
        uint64_t Count,
        const MCSubtargetInfo * STI
    ) const override {
        for (uint64_t i = 0; i != Count; ++i) {
            support::endian::write<uint8_t>(OS, 0x00, Endian);
        }

        return true;
    }
};

class ELFProgMinerLabAsmBackend : public ProgMinerLabAsmBackend {

    Triple::OSType OSType;

public:

    ELFProgMinerLabAsmBackend(const Target & T, Triple::OSType OSType)
        : ProgMinerLabAsmBackend(T), OSType(OSType) {}

    void applyFixup(
        const MCAssembler & Asm,
        const MCFixup & Fixup,
        const MCValue & Target,
        MutableArrayRef<char> Data,
        uint64_t Value,
        bool IsResolved,
        const MCSubtargetInfo * STI
    ) const override {
        llvm_unreachable("applyFixup");
        return;
    }

    std::unique_ptr<MCObjectTargetWriter> createObjectTargetWriter() const override {
        return createProgMinerLabELFObjectWriter(false, MCELFObjectTargetWriter::getOSABI(OSType));
    }
};

} // anonymous namespace

MCAsmBackend * llvm::createProgMinerLabAsmBackend(
    const Target & T,
    const MCSubtargetInfo & STI,
    const MCRegisterInfo & MRI,
    const MCTargetOptions & Options
) {
    return new ELFProgMinerLabAsmBackend(T, STI.getTargetTriple().getOS());
}
