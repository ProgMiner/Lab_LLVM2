include "llvm/Target/Target.td"

include "ProgMinerLabRegisterInfo.td"
include "ProgMinerLabInstrInfo.td"


class Proc<string Name, list<SubtargetFeature> Features>
    : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

def ProgMinerLabInstrInfo : InstrInfo;

def ProgMinerLab : Target {

    let InstructionSet = ProgMinerLabInstrInfo;
}
