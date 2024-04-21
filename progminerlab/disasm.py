#!/usr/bin/env python3

import sys


def reg(x):
    return reg.REGISTERS[x]


reg.REGISTERS = {
    0x0: 'R0',
    0x1: 'R1',
    0x2: 'R2',
    0x3: 'R3',
    0x4: 'R4',
    0x5: 'R5',
    0x6: 'R6',
    0x7: 'R7',
    0x8: 'R8',
    0x9: 'R9',
    0xA: 'R10',
    0xB: 'R11',
    0xC: 'R12',
    0xD: 'TMP',
    0xE: 'SP',
    0xF: 'FP',
}


def disasm(code, labels=dict(), pos=0):
    '''
    code - file-like object of bytes to disassemble
    labels - dict from offset in code to label
    '''

    def read_small_const(f=lambda x: f'{x}'):
        nonlocal instr

        x = code.read(1)

        instr += x
        x = int.from_bytes(x, signed=True)

        return f(x)

    def read_large_const(f=lambda x: f'{x}'):
        nonlocal instr

        x = code.read(4)

        instr += x
        x = int.from_bytes(x, 'little', signed=True)

        return f(x)

    def read_offset():
        return read_small_const(lambda x: f'{x} -> 0x{pos + x:04X}')

    def read_regs(f=lambda r1, r2: f'{r1}, {r2}'):
        nonlocal instr

        regs = code.read(1)

        instr += regs

        regs = regs[0]
        reg1, reg2 = regs >> 4, regs & 0xF

        return f(reg(reg1), reg(reg2))

    while opcode := code.read(1):
        opcode = opcode[0]

        instr = [opcode]
        mnemo = None

        match [opcode >> 4, opcode & 0xF]:
            case [0x0, 0x0]:
                mnemo = 'NOP'

            case [0x0, 0x1]:
                mnemo = f'JMPs {read_offset()}'

            case [0x0, 0x2]:
                mnemo = 'FLUSH'

            case [0x0, 0x3]:
                mnemo = f'CALL {read_offset()}'

            case [0x0, 0x4]:
                mnemo = 'RET'

            case [0x1, 0x0]:
                mnemo = f'MOV {read_regs()}'

            case [0x1, 0x1]:
                mnemo = f'ADD {read_regs()}'

            case [0x1, 0x2]:
                mnemo = f'SUB {read_regs()}'

            case [0x1, 0x3]:
                mnemo = f'MUL {read_regs()}'

            case [0x1, 0x4]:
                mnemo = f'DIV {read_regs()}'

            case [0x1, 0x5]:
                mnemo = f'REM {read_regs()}'

            case [0x1, 0x6]:
                mnemo = f'AND {read_regs()}'

            case [0x1, 0x7]:
                mnemo = f'OR {read_regs()}'

            case [0x1, 0x8]:
                mnemo = f'XOR {read_regs()}'

            case [0x1, 0x9]:
                mnemo = f'SHL {read_regs()}'

            case [0x1, 0xA]:
                mnemo = f'SHR {read_regs()}'

            case [0x1, 0xB]:
                mnemo = f'ASR {read_regs()}'

            case [0x1, 0xC]:
                mnemo = f'LOAD {read_regs()}'

            case [0x1, 0xD]:
                mnemo = f'STORE {read_regs()}'

            case [0x1, 0xE]:
                mnemo = f'PIXEL_OFF {read_regs()}'

            case [0x1, 0xF]:
                mnemo = f'PIXEL_ON {read_regs()}'

            case [0x2, 0x1]:
                mnemo = f'CMPeq {read_regs()}'

            case [0x2, 0x2]:
                mnemo = f'CMPgt {read_regs()}'

            case [0x2, 0x3]:
                mnemo = f'CMPge {read_regs()}'

            case [0x2, 0x4]:
                mnemo = f'CMPlt {read_regs()}'

            case [0x2, 0x5]:
                mnemo = f'CMPle {read_regs()}'

            case [0x2, 0x6]:
                mnemo = f'CMPne {read_regs()}'

            case [0x3, r]:
                mnemo = f'CONSTs {reg(r)}, {read_small_const()}'

            case [0x4, r]:
                mnemo = f'CONSTl {reg(r)}, {read_large_const()}'

            case [0x5, r]:
                mnemo = f'IFZs {reg(r)}, {read_offset()}'

            case [0x6, r]:
                mnemo = f'RAND {reg(r)}'

            case _:
                mnemo = f'# UNKNOWN {hex(opcode)}'

        instrStr = ' '.join([f'{x:02X}' for x in instr])
        print(f'{pos:04X} | {instrStr:<14} | {mnemo}')

        pos += len(instr)


if __name__ == '__main__':
    if len(sys.argv) > 1:
        # TODO support ELF parsing with labels

        print(f'Usage: {sys.argv[0]}')
        exit(0)

    disasm(sys.stdin.buffer)
