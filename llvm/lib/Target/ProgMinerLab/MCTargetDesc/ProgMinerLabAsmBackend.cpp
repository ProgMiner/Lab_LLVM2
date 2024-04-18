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

#include "ProgMinerLabFixupKinds.h"


using namespace llvm;


namespace {

class ProgMinerLabAsmBackend : public MCAsmBackend {

protected:

    const Target & TheTarget;

public:

    ProgMinerLabAsmBackend(const Target & T)
        : MCAsmBackend(endianness::little), TheTarget(T) {}

    unsigned getNumFixupKinds() const override {
        return ProgMinerLab::NumTargetFixupKinds;
    }

    const MCFixupKindInfo & getFixupKindInfo(MCFixupKind Kind) const override {
        const static MCFixupKindInfo InfosLE[ProgMinerLab::NumTargetFixupKinds] = {
            // name, offset, bits, flags

            { "fixup_ProgMinerLab_IP8_1", 8, 8, MCFixupKindInfo::FKF_IsPCRel },
        };

        // fixup kinds from .reloc directive are like R_SPARC_NONE
        // they do not require any extra processing
        if (Kind >= FirstLiteralRelocationKind) {
            return MCAsmBackend::getFixupKindInfo(FK_NONE);
        }

        if (Kind < FirstTargetFixupKind) {
            return MCAsmBackend::getFixupKindInfo(Kind);
        }

        assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() && "unexpected kind!");
        return InfosLE[Kind - FirstTargetFixupKind];
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
        OS.write_zeros(Count);
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
        unsigned Offset = Fixup.getOffset();
        unsigned NumBytes = 0;

        switch (Fixup.getKind()) {
        case ProgMinerLab::fixup_ProgMinerLab_IP8_1:
            NumBytes = 1;
            Offset += 1;
            break;

        default:
            return;
        }

        // for each byte of the fragment that the fixup touches,
        // mask in the bits from the fixup value
        //
        // the Value has been "split up" into the appropriate bitfields above
        for (unsigned i = 0; i != NumBytes; ++i) {
            Data[Offset + i] |= uint8_t((Value >> (i * 8)) & 0xFF);
        }

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
