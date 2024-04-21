#pragma once

#include <elfio/elfio.hpp>


enum {

    EM_PROGMINERLAB = 259,
};

struct code_info {

    const uint8_t * code_begin;
    const uint8_t * code_end;

    void check_avail(const uint8_t * it) const {
        if (it >= code_end) {
            throw std::invalid_argument { "out of code section bounds" };
        }
    }

    template<typename T, std::enable_if_t<std::is_integral_v<T>> * = nullptr>
    T read_number(const uint8_t * & it) const {
        const std::size_t n = sizeof(T);

        check_avail(it + (n - 1));

        const T result = *reinterpret_cast<const T *>(it);
        it += n;

        return result;
    }

    const uint8_t * next_instr(const uint8_t * it) const {
        const auto opcode = read_number<uint8_t>(it);

        switch (opcode) {

        // simple
        case 0x00:
        case 0x02:
        case 0x04:
            break;

        // unary
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6C:
        case 0x6D:
        case 0x6E:
        case 0x6F:
            break;

        // binary
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
            read_number<uint8_t>(it);
            break;

        // small constant
            break;

        // large constant
        case 0x01:
        case 0x03:
            read_number<uint32_t>(it);
            break;

        // small constant + reg
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
            read_number<uint8_t>(it);
            break;

        // large constant + reg
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
            read_number<uint32_t>(it);
            break;

        // unknown
        default:
            break;
        }

        return it;
    }
};

inline void check_file(const ELFIO::elfio & reader) {
    if (reader.get_class() != ELFIO::ELFCLASS32) {
        throw std::invalid_argument { "unsupported ELF class" };
    }

    if (reader.get_encoding() != ELFIO::ELFDATA2LSB) {
        throw std::invalid_argument { "unsupported byte ordering" };
    }

    if (reader.get_machine() != EM_PROGMINERLAB) {
        throw std::invalid_argument { "unsupported target machine" };
    }
}

inline void find_code_section(const ELFIO::elfio & reader, code_info & info) {
    const uint8_t * code_begin = nullptr;
    const uint8_t * code_end = nullptr;

    for (std::size_t i = 0; i < reader.sections.size(); ++i) {
        ELFIO::section * psec = reader.sections[i];

        if (psec->get_name() == ".text") {
            if (code_begin) {
                throw std::invalid_argument { "more than one .text section in file" };
            }

            code_begin = reinterpret_cast<const uint8_t *>(reader.sections[i]->get_data());
            code_end = std::next(code_begin, psec->get_size());
        }
    }

    if (!code_begin) {
        throw std::invalid_argument { "no .text section in file" };
    }

    info.code_begin = code_begin;
    info.code_end = code_end;
}

class read_symtab {

    const ELFIO::elfio & reader;

public:

    class iterator;

    read_symtab(const ELFIO::elfio & reader) noexcept: reader(reader) {}

    iterator begin() const noexcept {
        return { reader, 0, 0 };
    }

    iterator end() const noexcept {
        return { reader, reader.sections.size(), 0 };
    }

    class iterator {

        const ELFIO::elfio & reader;

        std::size_t i, j;

        iterator(const ELFIO::elfio & reader, std::size_t i, std::size_t j) noexcept
            : reader(reader), i(i), j(j)
        {
            to_symtab();
        }

        friend class read_symtab;

    public:

        std::pair<std::string, std::size_t> operator*() const noexcept {
            const ELFIO::symbol_section_accessor symbols(reader, reader.sections[i]);

            std::string name;
            ELFIO::Elf64_Addr value;
            ELFIO::Elf_Xword size;
            unsigned char bind;
            unsigned char type;
            ELFIO::Elf_Half section_index;
            unsigned char other;

            symbols.get_symbol(j, name, value, size, bind, type, section_index, other);

            return { name, value };
        }

        iterator & operator++() noexcept {
            ++j;

            const ELFIO::symbol_section_accessor symbols(reader, reader.sections[i]);

            if (j == symbols.get_symbols_num()) {
                j = 0;

                ++i;
                to_symtab();
            }

            return *this;
        }

        bool operator!=(const iterator & that) noexcept {
            return i != that.i || j != that.j;
        }

    private:

        void to_symtab() {
            while (
                i < reader.sections.size()
                && reader.sections[i]->get_type() != ELFIO::SHT_SYMTAB
            ) {
                ++i;
            }
        }
    };
};
