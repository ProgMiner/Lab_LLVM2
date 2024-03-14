
add_llvm_component_library(LLVMProgMinerLabDesc
    ProgMinerLabMCTargetDesc.cpp

    LINK_COMPONENTS
    MC
    Support
    ProgMinerLabInfo

    ADD_TO_COMPONENT
    ProgMinerLab
)
