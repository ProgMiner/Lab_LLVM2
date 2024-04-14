
// ProgMinerLab register file declaration
class ProgMinerLabReg<bits<16> Enc, string n> : Register<n> {

    let Namespace = "ProgMinerLab";

    let HWEncoding = Enc;
}

// Integer registers
foreach i = 1-8 in {

    def I#i : ProgMinerLabReg<!sub(i, 1), "i"#i>;
}

// Memory registers
foreach i = 1-8 in {

    def M#i : ProgMinerLabReg<!sub(i, 1), "m"#i>;
}

// Classes

// Integer GPRs
def IGPR : RegisterClass<"ProgMinerLab", [i32], 32,
    (add I1, I2, I3, I4, I5, I6, I7, I8)>;

// Memory GPRs
def MGPR : RegisterClass<"ProgMinerLab", [i32], 32,
    (add M1, M2, M3, M4, M5, M6, M7, M8)>;
