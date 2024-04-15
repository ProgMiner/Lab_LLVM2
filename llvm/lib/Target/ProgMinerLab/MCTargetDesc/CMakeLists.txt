
add_llvm_component_library(LLVMProgMinerLabDesc
    ProgMinerLabTargetStreamer.cpp
    ProgMinerLabMCTargetDesc.cpp
    ProgMinerLabInstPrinter.cpp
    ProgMinerLabMCAsmInfo.cpp

    LINK_COMPONENTS
    MC
    Support
    ProgMinerLabInfo

    ADD_TO_COMPONENT
    ProgMinerLab
)
