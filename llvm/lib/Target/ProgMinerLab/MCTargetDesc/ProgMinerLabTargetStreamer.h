#pragma once

#include "llvm/MC/MCStreamer.h"


namespace llvm {

class ProgMinerLabTargetStreamer : public MCTargetStreamer {

public:

    ProgMinerLabTargetStreamer(MCStreamer & S);
    ~ProgMinerLabTargetStreamer() override;
};

} // namespace llvm
