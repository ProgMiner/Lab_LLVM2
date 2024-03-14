#pragma once

#include <optional>

#include "llvm/Target/TargetMachine.h"


namespace llvm {

extern Target TheProgMinerLabTarget;

class ProgMinerLabTargetMachine : public LLVMTargetMachine {

    std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:

    ProgMinerLabTargetMachine(
        const Target & T,
        const Triple & TT,
        StringRef CPU,
        StringRef FS,
        const TargetOptions & Options,
        std::optional<Reloc::Model> RM,
        std::optional<CodeModel::Model> CM,
        CodeGenOptLevel OL,
        bool JIT,
        bool isLittle = true
    );

    TargetPassConfig * createPassConfig(PassManagerBase & PM) override;

    TargetLoweringObjectFile * getObjFileLowering() const override {
        return TLOF.get();
    }
};

} // end namespace llvm
