
add_llvm_component_group(ProgMinerLab)

add_llvm_target(ProgMinerLabCodeGen
    ProgMinerLabTargetMachine.cpp

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
