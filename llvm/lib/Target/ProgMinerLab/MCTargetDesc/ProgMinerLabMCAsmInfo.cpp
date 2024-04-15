#include "ProgMinerLabMCAsmInfo.h"


using namespace llvm;


ProgMinerLabELFMCAsmInfo::ProgMinerLabELFMCAsmInfo(const Triple & TT) {
    SupportsDebugInformation = false;
    Data32bitsDirective = "\t.word\t";
    Data64bitsDirective = nullptr;
    ZeroDirective = "\t.space\t";
    CommentString = ";";

    UsesELFSectionDirectiveForBSS = false;
    AllowAtInName = true;
    HiddenVisibilityAttr = MCSA_Invalid;
    HiddenDeclarationVisibilityAttr = MCSA_Invalid;
    ProtectedVisibilityAttr = MCSA_Invalid;

    ExceptionsType = ExceptionHandling::None;
}
