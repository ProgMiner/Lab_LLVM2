
add_llvm_component_group(ProgMinerLab)

set(LLVM_TARGET_DEFINITIONS ProgMinerLab.td)

tablegen(LLVM ProgMinerLabGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM ProgMinerLabGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM ProgMinerLabGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM ProgMinerLabGenCallingConv.inc -gen-callingconv)
tablegen(LLVM ProgMinerLabGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM ProgMinerLabGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM ProgMinerLabGenDAGISel.inc -gen-dag-isel)

add_public_tablegen_target(ProgMinerLabCommonTableGen)

add_llvm_target(ProgMinerLabCodeGen
    ProgMinerLabTargetMachine.cpp
    ProgMinerLabFrameLowering.cpp
    ProgMinerLabRegisterInfo.cpp
    ProgMinerLabISelDagToDag.cpp
    ProgMinerLabISelLowering.cpp
    ProgMinerLabAsmPrinter.cpp
    ProgMinerLabInstrInfo.cpp
    ProgMinerLabSubtarget.cpp

    LINK_COMPONENTS
    AsmPrinter
    CodeGen
    Core
    MC
    SelectionDAG
    Support
    Target
    ProgMinerLabInfo

    ADD_TO_COMPONENT
    ProgMinerLab
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
