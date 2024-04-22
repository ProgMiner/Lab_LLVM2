#include <random>

#include "loader.h"
#include "sim.h"


// entry point symbol name
#define ENTRY_POINT "main"

enum {

    SP = 14,
};

struct run_info : code_info {

    std::size_t entry_point;

    const uint8_t * resolve_offset(std::size_t pos, const uint8_t * it) const;
};

const uint8_t * run_info::resolve_offset(std::size_t pos, const uint8_t * it) const {
    pos += read_number<int32_t>(it);

    return std::next(code_begin, pos);
}

struct run_context {

    std::size_t ip;
    std::array<int32_t, 16> regs;

    std::unordered_map<std::size_t, std::array<uint8_t, 4096>> mem;

    uint8_t & get_mem(std::size_t addr) {
        return mem[addr >> 12][addr & 0xFFF];
    }

    uint32_t load(uint32_t addr) {
        addr += 3;

        uint32_t value = 0;
        for (std::size_t i = 0; i < 4; ++i) {
            value <<= 8;

            value |= get_mem(addr - i);
        }

        return value;
    }

    void store(uint32_t addr, uint32_t value) {
        for (std::size_t i = 0; i < 4; ++i) {
            get_mem(addr + i) = value & 0xFF;
            value >>= 8;
        }
    }

    void push(uint32_t value) {
        store(regs[SP] -= 4, value);
    }

    uint32_t pop() {
        const uint32_t value = load(regs[SP]);

        regs[SP] += 4;
        return value;
    }

    template<typename F>
    void binop(const uint8_t * it, F && f);
};

static std::size_t get_entry_point(const ELFIO::elfio & reader) {
    for (auto [name, pos] : read_symtab(reader)) {
        if (name == ENTRY_POINT) {
            return pos;
        }
    }

    throw std::invalid_argument { "unable to find entry point " ENTRY_POINT };
}

static run_info load_file(const ELFIO::elfio & reader) {
    check_file(reader);

    run_info result;
    find_code_section(reader, result);
    result.entry_point = get_entry_point(reader);

    return result;
}

static std::pair<uint8_t, uint8_t> get_reg2(const uint8_t * it) {
    const uint8_t regs = *it;

    return { regs >> 4, regs & 0xF };
}

template<typename F>
void run_context::binop(const uint8_t * it, F && f) {
    auto [r1, r2] = get_reg2(it);

    regs[r1] = f(regs[r1], regs[r2]);
}

static inline void dump_regs(std::ostream & os, const run_context & ctx) {
    for (std::size_t i = 0; i < 16; ++i) {
        os << "regs[" << i << "] = " << ctx.regs[i] << std::endl;
    }
}

static int run(const run_info & info) {
    run_context ctx { info.entry_point };

    ctx.push(UINT32_MAX);

    SimContext sim;

    for (
        const uint8_t * it = std::next(info.code_begin, info.entry_point);
        info.code_begin <= it && it < info.code_end;
    ) {
        const uint8_t * next_it = info.next_instr(it);

        const std::size_t pos = std::distance(info.code_begin, it);

        const uint8_t * tmp = it;
        const auto opcode = info.read_number<uint8_t>(tmp);

        // print_pos(std::cout, pos);
        // std::cout << std::endl;

        // dump_regs(std::cout, ctx);

        switch (opcode >> 4) {
        case 0x0:
            switch (opcode & 0xF) {
            case 0x0:
                break;

            case 0x1:
                next_it = info.resolve_offset(pos, tmp);
                break;

            case 0x2:
                sim.flush();
                break;

            case 0x3:
                ctx.push(std::distance(info.code_begin, next_it));
                next_it = info.resolve_offset(pos, tmp);
                break;

            case 0x4:
                next_it = std::next(info.code_begin, ctx.pop());
                break;

            default:
                goto unknown;
            }

            break;

        case 0x1:
            switch (opcode & 0xF) {
            case 0x0: {
                ctx.binop(tmp, [](int32_t, int32_t b) {
                    return b;
                });

                break;
            }

            case 0x1:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a + b;
                });

                break;

            case 0x2:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a - b;
                });

                break;

            case 0x3:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a * b;
                });

                break;

            case 0x4:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a / b;
                });

                break;

            case 0x5:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a % b;
                });

                break;

            case 0x6:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
                });

                break;

            case 0x7:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
                });

                break;

            case 0x8:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b);
                });

                break;

            case 0x9:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return static_cast<uint32_t>(a) << static_cast<uint32_t>(b);
                });

                break;

            case 0xA:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return static_cast<uint32_t>(a) >> static_cast<uint32_t>(b);
                });

                break;

            case 0xB:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return static_cast<int64_t>(a) >> static_cast<uint32_t>(b);
                });

                break;

            case 0xC: {
                auto [r1, r2] = get_reg2(tmp);

                ctx.regs[r1] = ctx.load(ctx.regs[r2]);
                break;
            }

            case 0xD: {
                auto [r1, r2] = get_reg2(tmp);

                ctx.store(ctx.regs[r1], ctx.regs[r2]);
                break;
            }

            case 0xE: {
                auto [r1, r2] = get_reg2(tmp);

                sim.set_pixel(ctx.regs[r1], ctx.regs[r2], false);
                break;
            }

            case 0xF: {
                auto [r1, r2] = get_reg2(tmp);

                sim.set_pixel(ctx.regs[r1], ctx.regs[r2], true);
                break;
            }
            }

            break;

        case 0x2:
            switch (opcode & 0xF) {
            case 0x1:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a == b;
                });

                break;

            case 0x2:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a > b;
                });

                break;

            case 0x3:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a >= b;
                });

                break;

            case 0x4:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a < b;
                });

                break;

            case 0x5:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a <= b;
                });

                break;

            case 0x6:
                ctx.binop(tmp, [](int32_t a, int32_t b) {
                    return a != b;
                });

                break;

            default:
                goto unknown;
            }

            break;

        case 0x3:
            ctx.regs[opcode & 0xF] = info.read_number<int8_t>(tmp);
            break;

        case 0x4:
            ctx.regs[opcode & 0xF] = info.read_number<int32_t>(tmp);
            break;

        case 0x5:
            if (!ctx.regs[opcode & 0xF]) {
                next_it = info.resolve_offset(pos, tmp);
            }

            break;

        case 0x6:
            ctx.regs[opcode & 0xF] = sim_rand();
            break;

unknown:
        default: {
            std::ostringstream os;
            os << "unknown instruction at ";
            print_pos(os, pos);

            throw std::invalid_argument { os.str() };
        }
        }

        it = next_it;
    }

    // simple heuristic to differentiate between occasional OOB jump from normal quit
    if (ctx.regs[SP] != 0) {
        throw std::invalid_argument { "code jumped out of bounds" };
    }

    return ctx.regs[0]; // return value in R0
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

    run_info info;

    try {
        info = load_file(reader);
    } catch (const std::exception & e) {
        std::cerr << "Bad ELF file: " << e.what() << std::endl;
        return 2;
    }

    int result;

    try {
        result = run(info);
    } catch (const std::exception & e) {
        std::cerr << "Bad binary code: " << e.what() << std::endl;
        return 3;
    }

    std::cerr << "Exit code: " << result << std::endl;
}

int32_t sim_rand() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int32_t> distrib(0);

    return distrib(gen);
}
