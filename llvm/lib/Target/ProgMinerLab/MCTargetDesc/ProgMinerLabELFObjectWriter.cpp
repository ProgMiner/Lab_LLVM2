#include "ProgMinerLabMCTargetDesc.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/MC/MCExpr.h"


using namespace llvm;


namespace {

class ProgMinerLabELFObjectWriter : public MCELFObjectTargetWriter {

public:

    ProgMinerLabELFObjectWriter(bool Is64Bit, uint8_t OSABI)
        : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_PROGMINERLAB,
            /* HasRelocationAddend = */ true) {}

    ~ProgMinerLabELFObjectWriter() override = default;

protected:

    unsigned getRelocType(
        MCContext & Ctx,
        const MCValue &Target,
        const MCFixup & Fixup,
        bool IsPCRel
    ) const override {
        MCFixupKind Kind = Fixup.getKind();

        if (Kind >= FirstLiteralRelocationKind) {
            return Kind - FirstLiteralRelocationKind;
        }

        llvm_unreachable("Unimplemented fixup -> relocation");
    }

    bool needsRelocateWithSymbol(
        const MCValue & Val,
        const MCSymbol & Sym,
        unsigned Type
    ) const override {
        return false;
    }
};

} // anonymous namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createProgMinerLabELFObjectWriter(bool Is64Bit, uint8_t OSABI) {
    return std::make_unique<ProgMinerLabELFObjectWriter>(Is64Bit, OSABI);
}
