#include <sstream>
#include <iomanip>

#include "loader.h"


struct disasm_info : code_info {

    std::unordered_map<std::size_t, std::string> labels;

    void add_labels();

    void print_offset(std::ostream & os, std::size_t pos, const uint8_t * it) const;

    void disasm(std::ostream &) const;
};

void disasm_info::add_labels() {
    for (const uint8_t * it = code_begin; it < code_end; ) {
        const uint8_t * const next_it = next_instr(it);

        const uint8_t * tmp = it;
        const auto opcode = read_number<uint8_t>(tmp);

        if (opcode == 0x01 || opcode == 0x03 || (opcode >> 4) == 0x5) {
            const int32_t off = read_number<int32_t>(tmp);

            const std::size_t pos = std::distance(code_begin, it) + off;
            if (auto it_label = labels.find(pos); it_label == labels.end()) {
                std::ostringstream os;
                os << "L_" << pos;

                labels[pos] = os.str();
            }
        }

        it = next_it;
    }
}

static void print_byte(std::ostream & os, uint8_t b) {
    const char pf = os.fill('0');

    os << std::setw(2) << std::hex << std::uppercase << static_cast<uint32_t>(b)
       << std::dec << std::nouppercase;

    os.fill(pf);
}

static void print_pos(std::ostream & os, std::size_t pos) {
    const char pf = os.fill('0');

    os << std::setw(4) << std::hex << std::uppercase << pos
       << std::dec << std::nouppercase;

    os.fill(pf);
}

void disasm_info::print_offset(std::ostream & os, std::size_t pos, const uint8_t * it) const {
    const auto off = read_number<int32_t>(it);

    pos += off;
    os << off << " -> ";

    if (auto it_label = labels.find(pos); it_label != labels.end()) {
        os << it_label->second;
    } else {
        print_pos(os, pos);
    }
}

const char * reg_to_string(uint8_t reg) {
    static const char * const REGISTERS[] = {
        "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9",
        "R10", "R11", "R12", "TMP", "SP", "FP"
    };

    return REGISTERS[reg & 0xF];
}

void print_reg(std::ostream & os, uint8_t reg) {
    os << reg_to_string(reg);
}

void print_reg2(std::ostream & os, const uint8_t * it) {
    const uint8_t regs = *it;

    print_reg(os, regs >> 4);
    os << ", ";

    print_reg(os, regs & 0xF);
}

void disasm_info::disasm(std::ostream & os) const {
    for (const uint8_t * it = code_begin; it < code_end; ) {
        const uint8_t * const next_it = next_instr(it);

        const std::size_t pos = std::distance(code_begin, it);

        if (auto it_label = labels.find(pos); it_label != labels.end()) {
            os << std::endl;

            os << it_label->second << ':' << std::endl;
        }

        print_pos(os, pos);
        os << " | ";

        for (const uint8_t * it1 = it; it1 < next_it; ++it1) {
            print_byte(os, *it1);
            os << ' ';
        }

        for (std::size_t i = std::distance(it, next_it); i < 5; ++i) {
            os << "   ";
        }

        os << "| ";

        const uint8_t * tmp = it;
        const auto opcode = read_number<uint8_t>(tmp);

        switch (opcode >> 4) {
        case 0x0:
            switch (opcode & 0xF) {
            case 0x0:
                os << "NOP";
                break;

            case 0x1:
                os << "JMP ";
                print_offset(os, pos, tmp);
                break;

            case 0x2:
                os << "FLUSH";
                break;

            case 0x3:
                os << "CALL ";
                print_offset(os, pos, tmp);
                break;

            case 0x4:
                os << "RET";
                break;

            default:
                goto unknown;
            }

            break;

        case 0x1:
            switch (opcode & 0xF) {
            case 0x0:
                os << "MOV ";
                print_reg2(os, tmp);
                break;

            case 0x1:
                os << "ADD ";
                print_reg2(os, tmp);
                break;

            case 0x2:
                os << "SUB ";
                print_reg2(os, tmp);
                break;

            case 0x3:
                os << "MUL ";
                print_reg2(os, tmp);
                break;

            case 0x4:
                os << "DIV ";
                print_reg2(os, tmp);
                break;

            case 0x5:
                os << "REM ";
                print_reg2(os, tmp);
                break;

            case 0x6:
                os << "AND ";
                print_reg2(os, tmp);
                break;

            case 0x7:
                os << "OR ";
                print_reg2(os, tmp);
                break;

            case 0x8:
                os << "XOR ";
                print_reg2(os, tmp);
                break;

            case 0x9:
                os << "SHL ";
                print_reg2(os, tmp);
                break;

            case 0xA:
                os << "SHR ";
                print_reg2(os, tmp);
                break;

            case 0xB:
                os << "ASR ";
                print_reg2(os, tmp);
                break;

            case 0xC:
                os << "LOAD ";
                print_reg2(os, tmp);
                break;

            case 0xD:
                os << "STORE ";
                print_reg2(os, tmp);
                break;

            case 0xE:
                os << "PIXEL_OFF ";
                print_reg2(os, tmp);
                break;

            case 0xF:
                os << "PIXEL_ON ";
                print_reg2(os, tmp);
                break;
            }

            break;

        case 0x2:
            switch (opcode & 0xF) {
            case 0x1:
                os << "CMPeq ";
                print_reg2(os, tmp);
                break;

            case 0x2:
                os << "CMPgt ";
                print_reg2(os, tmp);
                break;

            case 0x3:
                os << "CMPge ";
                print_reg2(os, tmp);
                break;

            case 0x4:
                os << "CMPlt ";
                print_reg2(os, tmp);
                break;

            case 0x5:
                os << "CMPle ";
                print_reg2(os, tmp);
                break;

            case 0x6:
                os << "CMPne ";
                print_reg2(os, tmp);
                break;

            default:
                goto unknown;
            }

            break;

        case 0x3:
            os << "CONSTs ";
            print_reg(os, opcode);
            os << ", " << static_cast<int32_t>(read_number<int8_t>(tmp));
            break;

        case 0x4:
            os << "CONSTl ";
            print_reg(os, opcode);
            os << ", " << read_number<int32_t>(tmp);
            break;

        case 0x5:
            os << "IFZ ";
            print_reg(os, opcode);
            os << ", ";
            print_offset(os, pos, tmp);
            break;

        case 0x6:
            os << "RAND ";
            print_reg(os, opcode);
            break;

unknown:
        default:
            os << "# UNKNOWN";
            break;
        }

        os << std::endl;

        it = next_it;
    }
}

static disasm_info load_file(const ELFIO::elfio & reader) {
    check_file(reader);

    disasm_info result;
    find_code_section(reader, result);

    for (auto [name, pos] : read_symtab(reader)) {
        result.labels[pos] = name;
    }

    return result;
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <ELF file>" << std::endl;
        return 0;
    }

    ELFIO::elfio reader;
    if (!reader.load(argv[1])) {
        std::cerr << "Unable to read file " << argv[1] << " as ELF" << std::endl;
        return 1;
    }

    disasm_info info;

    try {
        info = load_file(reader);
    } catch (const std::exception & e) {
        std::cerr << "Bad ELF file: " << e.what() << std::endl;
        return 2;
    }

    info.add_labels();
    info.disasm(std::cout);
}
