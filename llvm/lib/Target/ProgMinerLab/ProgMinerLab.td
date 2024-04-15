include "llvm/Target/Target.td"

include "ProgMinerLabRegisterInfo.td"
include "ProgMinerLabInstrInfo.td"


def ProgMinerLabInstrInfo : InstrInfo;

def ProgMinerLab : Target {

    let InstructionSet = ProgMinerLabInstrInfo;
}
