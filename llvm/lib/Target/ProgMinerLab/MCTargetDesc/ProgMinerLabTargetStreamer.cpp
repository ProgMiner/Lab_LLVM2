#include "ProgMinerLabTargetStreamer.h"


using namespace llvm;


ProgMinerLabTargetStreamer::ProgMinerLabTargetStreamer(MCStreamer & S) : MCTargetStreamer(S) {}
ProgMinerLabTargetStreamer::~ProgMinerLabTargetStreamer() = default;
