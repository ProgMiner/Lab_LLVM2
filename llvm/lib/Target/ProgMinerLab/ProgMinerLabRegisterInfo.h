#pragma once


#define GET_REGINFO_HEADER
#include "ProgMinerLabGenRegisterInfo.inc"


namespace llvm {

struct ProgMinerLabRegisterInfo : public ProgMinerLabGenRegisterInfo {

public:

    ProgMinerLabRegisterInfo();
};

} // namespace llvm
