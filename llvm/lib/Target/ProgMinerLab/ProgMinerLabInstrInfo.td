//===----------------------------------------------------------------------===//
//  ProgMinerLab instructions
//===----------------------------------------------------------------------===//

include "ProgMinerLabInstrFormats.td"
include "ProgMinerLabOperands.td"

// Misc instructions

def NOP : SimpleInst<0x00, (outs), (ins), "NOP", []>;
