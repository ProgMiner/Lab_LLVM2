//  CPU INSTRUCTION FORMATS:
//  Registers: 16 registers (32 bits) R0-R15
//
//  FF FF FF FF
//        ^^ ^^ - R3/IMM - third register or immediate
//      ^       - R2     - second register
//     ^        - R1     - first register
//  ^^          - opcode - operation code

//  INSTRUCTION SET ARCHITECTURE:
//  [OPC] INSTR ARGS        (OPERATION)
//  0x10  WRITE R1          (WRITE R1)
//  0x11  MOV   R1, R2      (R1 = R2)
//  0x12  MOVli R1, IMM     (R1 = IMM)
//  0x13  MOVhi R1, IMM     (R1 = IMM << 16)
//  0x21  ADD   R1, R2, R3  (R1 = R2 + R3)
//  0x31  ADDi  R1, R2, IMM (R1 = R2 + IMM)
//  0x22  SUB   R1, R2, R3  (R1 = R2 - R3)
//  0x32  SUBi  R1, R2, IMM (R1 = R2 - IMM)
//  0x23  MUL   R1, R2, R3  (R1 = R2 * R3)
//  0x33  MULi  R1, R2, IMM (R1 = R2 * IMM)
//  0x24  DIV   R1, R2, R3  (R1 = R2 / R3)
//  0x34  DIVi  R1, R2, IMM (R1 = R2 / IMM)


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


enum Opcode {

    OPCODE_WRITE = 0x10,
    OPCODE_MOV   = 0x11,
    OPCODE_MOVli = 0x12,
    OPCODE_MOVhi = 0x13,
    OPCODE_ADD   = 0x21,
    OPCODE_ADDi  = 0x31,
    OPCODE_SUB   = 0x22,
    OPCODE_SUBi  = 0x32,
    OPCODE_MUL   = 0x23,
    OPCODE_MULi  = 0x33,
    OPCODE_DIV   = 0x24,
    OPCODE_DIVi  = 0x34,
};

struct InstrOperands {

    bool    reg1:1;     // has reg 1
    bool    reg2:1;     // has reg 2
    uint8_t reg3_imm:2; // has reg 3 (1) or imm (2), 3 is illegal
};

const char * const INSTR_NAME_TABLE[256] = {

    [OPCODE_WRITE] = "WRITE",
    [OPCODE_MOV]   = "MOV",
    [OPCODE_MOVli] = "MOVli",
    [OPCODE_MOVhi] = "MOVhi",
    [OPCODE_ADD]   = "ADD",
    [OPCODE_ADDi]  = "ADDi",
    [OPCODE_SUB]   = "SUB",
    [OPCODE_SUBi]  = "SUBi",
    [OPCODE_MUL]   = "MUL",
    [OPCODE_MULi]  = "MULi",
    [OPCODE_DIV]   = "DIV",
    [OPCODE_DIVi]  = "DIVi",
};

const char * const REG_NAME_TABLE[16] = {

    [ 0] = "R0",
    [ 1] = "R1",
    [ 2] = "R2",
    [ 3] = "R3",
    [ 4] = "R4",
    [ 5] = "R5",
    [ 6] = "R6",
    [ 7] = "R7",
    [ 8] = "R8",
    [ 9] = "R9",
    [10] = "R10",
    [11] = "R11",
    [12] = "R12",
    [13] = "R13",
    [14] = "R14",
    [15] = "R15",
};

const struct InstrOperands INSTR_OPERANDS_TABLE[256] = {

    [OPCODE_WRITE] = { true, false, 0 },
    [OPCODE_MOV]   = { true, true , 0 },
    [OPCODE_MOVli] = { true, false, 2 },
    [OPCODE_MOVhi] = { true, false, 2 },
    [OPCODE_ADD]   = { true, true , 1 },
    [OPCODE_ADDi]  = { true, true , 2 },
    [OPCODE_SUB]   = { true, true , 1 },
    [OPCODE_SUBi]  = { true, true , 2 },
    [OPCODE_MUL]   = { true, true , 1 },
    [OPCODE_MULi]  = { true, true , 2 },
    [OPCODE_DIV]   = { true, true , 1 },
    [OPCODE_DIVi]  = { true, true , 2 },
};


void printInstr(uint32_t instr) {
    const enum Opcode opcode = instr >> 24;

    const char * name = INSTR_NAME_TABLE[opcode];

    if (!name) {
        name = "# UNKNOWN";
    }

    printf("%s", name);

    const struct InstrOperands operands = INSTR_OPERANDS_TABLE[opcode];

    bool need_comma = false;

    if (operands.reg1) {
        if (need_comma) {
            printf(",");
        }

        printf(" %s", REG_NAME_TABLE[(instr >> 20) & 0xF]);
        need_comma = true;
    }

    if (operands.reg2) {
        if (need_comma) {
            printf(",");
        }

        printf(" %s", REG_NAME_TABLE[(instr >> 16) & 0xF]);
        need_comma = true;
    }

    if (operands.reg3_imm == 1) {
        if (need_comma) {
            printf(",");
        }

        printf(" %s", REG_NAME_TABLE[instr & 0xF]);
        need_comma = true;
    } else if (operands.reg3_imm == 2) {
        if (need_comma) {
            printf(",");
        }

        printf(" %d", instr & 0xFFFF);
        need_comma = true;
    }

    return;
}

void printAsm(uint32_t * bin, uint32_t binSize) {
    printf("INSTRUCTIONS:\n");

    for (uint32_t i = 0; i < binSize; ++i) {
        printInstr(bin[i]);
        printf("\n");
    }

    printf("\n");
}

void execute(uint32_t * bin, uint32_t binSize) {
    printf("EXECUTION:\n");

    int32_t regs[16] = {};

    for (uint32_t i = 0; i < binSize; ++i) {
        const uint32_t instr = bin[i];

        const enum Opcode opcode = instr >> 24;

        const uint8_t reg1 = (instr >> 20) & 0xF;
        const uint8_t reg2 = (instr >> 16) & 0xF;
        const uint16_t reg3_imm = instr & 0xFFFF;

        switch (opcode) {
        case OPCODE_WRITE:
            printf("%d\n", (int) regs[reg1]);
            break;

        case OPCODE_MOV:
            regs[reg1] = regs[reg2];
            break;

        case OPCODE_MOVli:
            regs[reg1] |= reg3_imm;
            break;

        case OPCODE_MOVhi:
            regs[reg1] |= (reg3_imm << 16);
            break;

        case OPCODE_ADD:
            regs[reg1] = regs[reg2] + regs[reg3_imm];
            break;

        case OPCODE_ADDi:
            regs[reg1] = regs[reg2] + reg3_imm;
            break;

        case OPCODE_SUB:
            regs[reg1] = regs[reg2] - regs[reg3_imm];
            break;

        case OPCODE_SUBi:
            regs[reg1] = regs[reg2] - reg3_imm;
            break;

        case OPCODE_MUL:
            regs[reg1] = regs[reg2] * regs[reg3_imm];
            break;

        case OPCODE_MULi:
            regs[reg1] = regs[reg2] * reg3_imm;
            break;

        case OPCODE_DIV:
            regs[reg1] = regs[reg2] / regs[reg3_imm];
            break;

        case OPCODE_DIVi:
            regs[reg1] = regs[reg2] / reg3_imm;
            break;

        default:
            printf("UNSUPPORTED INSTRUCTION\n");
            return;
        }
    }

    return;
}

int main() {
    uint32_t BIN[] = {0x13000001, 0x10000000, 0x34100100, 0x10100000,\
                      0x31210010, 0x10200000, 0x32310010, 0x10300000,\
                      0x21320003, 0x10300000, 0x22230002, 0x10200000,\
                      0x22330002, 0x10300000, 0x21120003, 0x10100000,\
                      0x33110010, 0x10100000, 0x24000001, 0x10000000};
    printAsm(BIN, sizeof(BIN)/sizeof(uint32_t));
    execute(BIN, sizeof(BIN)/sizeof(uint32_t));
    return 0;
}

// EXPECTED OUTPUT:
//
// INSTRUCTIONS:
// MOVhi R0, 1
// WRITE R0
// DIVi R1, R0, 256
// WRITE R1
// ADDi R2, R1, 16
// WRITE R2
// SUBi R3, R1, 16
// WRITE R3
// ADD R3, R2, R3
// WRITE R3
// SUB R2, R3, R2
// WRITE R2
// SUB R3, R3, R2
// WRITE R3
// ADD R1, R2, R3
// WRITE R1
// MULi R1, R1, 16
// WRITE R1
// DIV R0, R0, R1
// WRITE R0
//
// EXECUTION:
// 65536
// 256
// 272
// 240
// 512
// 240
// 272
// 512
// 8192
// 8
