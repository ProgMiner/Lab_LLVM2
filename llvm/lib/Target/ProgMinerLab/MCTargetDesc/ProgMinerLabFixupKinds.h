#pragma once

#include "llvm/MC/MCFixup.h"

namespace llvm {

namespace ProgMinerLab {

    // although most of the current fixup types reflect a unique relocation one can have
    // multiple fixup types for a given relocation and thus need to be uniquely named
    //
    // this table *must* be in the same order of
    // MCFixupKindInfo Infos[ProgMinerLab::NumTargetFixupKinds] in ProgMinerLabAsmBackend.cpp
    enum Fixups {

        fixup_ProgMinerLab_IP8_1 = FirstTargetFixupKind,

        LastTargetFixupKind,
        NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
    };

} // namespace ProgMinerLab

} // namespace llvm
