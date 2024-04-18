//===----------------------------------------------------------------------===//
// Calling conventions for ProgMinerLab architecture
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// ProgMinerLab return value calling convention
//===----------------------------------------------------------------------===//

def RetCC_ProgMinerLab : CallingConv<[

    // promote i8/i16 return values to i32
    CCIfType<[i8, i16], CCPromoteToType<i32>>,

    // i32 return values
    CCIfType<[i32], CCAssignToReg<[R0]>>,

    // integer values get stored in stack slots that are 4 bytes in size and 4-byte aligned
    // CCIfType<[i64], CCAssignToStack<8, 4>>,
    CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

//===----------------------------------------------------------------------===//
// ProgMinerLab argument calling conventions
//===----------------------------------------------------------------------===//

def CC_ProgMinerLab : CallingConv<[

    // promote i8/i16 arguments to i32
    CCIfType<[i8, i16], CCPromoteToType<i32>>,

    // the first 4 integer arguments are passed in integer registers
    CCIfType<[i32], CCAssignToReg<[R1, R2, R3, R4]>>,

    // integer values get stored in stack slots that are 4 bytes in size and 4-byte aligned
    // CCIfType<[i64], CCAssignToStack<8, 4>>,
    CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

def CSR_ProgMinerLab : CalleeSavedRegs<(add R8, R9, R10, R11, R12, SP, FP)>;
