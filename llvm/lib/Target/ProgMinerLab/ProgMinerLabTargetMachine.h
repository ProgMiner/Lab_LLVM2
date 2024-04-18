#pragma once

#include "llvm/Target/TargetMachine.h"

#include "ProgMinerLabInstrInfo.h"
#include "ProgMinerLabSubtarget.h"
#include "ProgMinerLabMachineFunctionInfo.h"


namespace llvm {

class ProgMinerLabTargetMachine : public LLVMTargetMachine {

    std::unique_ptr<TargetLoweringObjectFile> TLOF;
    ProgMinerLabSubtarget Subtarget;

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

     ~ProgMinerLabTargetMachine() override;

    const ProgMinerLabSubtarget * getSubtargetImpl(const Function &) const override {
      return &Subtarget;
    }

    // pass pipeline configuration
    TargetPassConfig * createPassConfig(PassManagerBase & PM) override;

    TargetLoweringObjectFile * getObjFileLowering() const override {
        return TLOF.get();
    }

    ProgMinerLabFunctionInfo * createMachineFunctionInfo(
        BumpPtrAllocator & Allocator,
        const Function & F,
        const TargetSubtargetInfo * STI
    ) const override;
};

} // end namespace llvm
