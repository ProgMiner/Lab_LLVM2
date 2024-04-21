#include "loader.h"


// entry point symbol name
#define ENTRY_POINT "main"

struct run_info : code_info {

    std::size_t entry_point;
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

    (void) info;

    // Elf_Xword code_start = 0;
}
