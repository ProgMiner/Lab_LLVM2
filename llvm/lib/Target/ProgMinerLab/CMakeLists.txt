
add_llvm_component_group(ProgMinerLab)

set(LLVM_TARGET_DEFINITIONS ProgMinerLab.td)

tablegen(LLVM ProgMinerLabGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM ProgMinerLabGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM ProgMinerLabGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(ProgMinerLabCommonTableGen)

add_llvm_target(ProgMinerLabCodeGen
    ProgMinerLabTargetMachine.cpp
    ProgMinerLabRegisterInfo.cpp
    ProgMinerLabInstrInfo.cpp
    ProgMinerLabSubtarget.cpp

    LINK_COMPONENTS
    CodeGen
    Core
    Support
    Target
    ProgMinerLabInfo

    ADD_TO_COMPONENT
    ProgMinerLab
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
