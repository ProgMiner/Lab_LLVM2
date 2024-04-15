
// ProgMinerLab register file declaration
class ProgMinerLabReg<bits<4> Enc, string name> : Register<name> {

    let Namespace = "ProgMinerLab";

    let HWEncoding{3-0} = Enc;
}

// Registers
foreach i = 0-15 in {

    def R#i : ProgMinerLabReg<i, "r"#i>, DwarfRegNum<[i]>;
}

// Classes
def GPR : RegisterClass<"ProgMinerLab", [i32], 32,
    (add R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15)>;
