
add_llvm_component_library(LLVMProgMinerLabInfo
    ProgMinerLabTargetInfo.cpp

    LINK_COMPONENTS
    MC
    Support

    ADD_TO_COMPONENT
    ProgMinerLab
)
