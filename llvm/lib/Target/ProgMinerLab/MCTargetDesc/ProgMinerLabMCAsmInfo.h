#pragma once

#include "llvm/MC/MCAsmInfoELF.h"


namespace llvm {

class Triple;

class ProgMinerLabELFMCAsmInfo : public MCAsmInfoELF {

public:

    explicit ProgMinerLabELFMCAsmInfo(const Triple & TheTriple);
};

} // namespace llvm
