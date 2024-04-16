
add_llvm_component_library(LLVMProgMinerLabDesc
    ProgMinerLabELFObjectWriter.cpp
    ProgMinerLabTargetStreamer.cpp
    ProgMinerLabMCCodeEmitter.cpp
    ProgMinerLabMCTargetDesc.cpp
    ProgMinerLabInstPrinter.cpp
    ProgMinerLabAsmBackend.cpp
    ProgMinerLabMCAsmInfo.cpp

    LINK_COMPONENTS
    MC
    Support
    ProgMinerLabInfo

    ADD_TO_COMPONENT
    ProgMinerLab
)
