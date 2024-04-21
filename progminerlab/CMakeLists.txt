cmake_minimum_required(VERSION 3.16)
project(progminerlab LANGUAGES C CXX)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ELFIO
    GIT_REPOSITORY https://github.com/serge1/ELFIO.git
    GIT_TAG Release_3.12)

FetchContent_MakeAvailable(ELFIO)

# find_package(LLVM REQUIRED CONFIG
#     PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../../llvm/cmake/modules/LLVM-Config.cmake"
#     NO_DEFAULT_PATH)

# add_definitions(${LLVM_DEFINITIONS})

add_compile_options(-Wall -Werror -pedantic-errors)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# pure implementation

add_executable(orig src/main.c src/sim.cpp src/sim.h src/sim.hpp)

target_link_libraries(orig PRIVATE sfml-graphics)

target_compile_features(orig PRIVATE c_std_11)
target_compile_features(orig PRIVATE cxx_std_17)

# disassembler

add_executable(disasm src/disasm.cpp src/loader.h)

target_link_libraries(disasm PRIVATE elfio)

target_compile_features(disasm PRIVATE cxx_std_17)

# ELF-runner

add_executable(runner src/runner.cpp src/loader.h src/sim.hpp)

target_link_libraries(runner PRIVATE sfml-graphics)
target_link_libraries(runner PRIVATE elfio)

target_compile_features(runner PRIVATE cxx_std_17)

# TODO
# through LLVM

# add_executable(driver src/driver.cpp src/sim.cpp src/sim.h src/sim.hpp)

# target_include_directories(driver PRIVATE ${LLVM_INCLUDE_DIRS})

# target_link_libraries(driver PRIVATE sfml-graphics)
# target_link_libraries(driver PRIVATE LLVM)

# target_compile_features(driver PRIVATE cxx_std_17)
