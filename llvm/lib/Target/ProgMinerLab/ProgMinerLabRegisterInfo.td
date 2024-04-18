
// ProgMinerLab register file declaration
class ProgMinerLabReg<bits<4> Enc, string name> : Register<name> {

    let Namespace = "ProgMinerLab";

    let HWEncoding{3-0} = Enc;
}

// GPRs
foreach i = 0-12 in {

    def R#i : ProgMinerLabReg<i, "r"#i>, DwarfRegNum<[i]>;
}

// temporary register for relative addressing
def TMP : ProgMinerLabReg<13, "r13">, DwarfRegNum<[13]>;

// stack pointer
let AltNames = ["SP"] in
def SP : ProgMinerLabReg<14, "r14">, DwarfRegNum<[14]>;

// frame pointer
let AltNames = ["FP"] in
def FP : ProgMinerLabReg<15, "r15">, DwarfRegNum<[15]>;

// Classes
def GPR : RegisterClass<"ProgMinerLab", [i32], 32,
    (add R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, TMP, SP, FP)>;
