


class Reg<bits<4> HW, string name, int size> {

    bits<4> HWEncoding = HW;

    string Name = name;
    string purpose = "VEC";

    int Size = size;
}

class DReg<bits<4> HW, string name> : Reg<HW, name, 64>;
class QReg<bits<4> HW, string name> : Reg<HW, name, 128>;

multiclass DQRegs<bits<4> HW, int n> {

    def D#n : DReg<HW, "d"#n>;
    def Q#n : DReg<HW, "q"#n>;
}

let purpose = "GPR" in
def D1 : DReg<1, "d1">;

def Q1 : QReg<1, "q1">;

foreach i = 2-4 in {

    defm : DQRegs<i, i>;
}
