
add_llvm_component_group(ProgMinerLab)

set(LLVM_TARGET_DEFINITIONS ProgMinerLab.td)

tablegen(LLVM ProgMinerLabGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM ProgMinerLabGenMCCodeEmitter.inc -gen-emitter)

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
    Analysis
    AsmPrinter
    CodeGen
    Core
    GlobalISel
    MC
    ProgMinerLabInfo
    SelectionDAG
    Support
    Target

    ADD_TO_COMPONENT
    ProgMinerLab
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
