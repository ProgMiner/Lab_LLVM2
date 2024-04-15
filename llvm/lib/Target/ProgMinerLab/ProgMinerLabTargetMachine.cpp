#include "ProgMinerLabTargetMachine.h"

#include <memory>

#include "llvm-c/Target.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Triple.h"

#include "TargetInfo/ProgMinerLabTargetInfo.h"
#include "ProgMinerLabISelDagToDag.h"


using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeProgMinerLabTarget() {

    // register the target
    RegisterTargetMachine<ProgMinerLabTargetMachine> A(getTheProgMinerLabTarget());
}

static std::string computeDataLayout(
    const Triple & TT,
    StringRef CPU,
    const TargetOptions & Options,
    bool IsLittle
) {
    return "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32";
}

static Reloc::Model getEffectiveRelocModel(bool JIT, std::optional<Reloc::Model> RM) {
    if (!RM || JIT) {
        return Reloc::Static;
    }

    return *RM;
}

ProgMinerLabTargetMachine::ProgMinerLabTargetMachine(
    const Target & T,
    const Triple & TT,
    StringRef CPU,
    StringRef FS,
    const TargetOptions & Options,
    std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CM,
    CodeGenOptLevel OL,
    bool JIT,
    bool IsLittle
) : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, IsLittle), TT, CPU, FS, Options,
        getEffectiveRelocModel(JIT, RM), getEffectiveCodeModel(CM, CodeModel::Small), OL),
    TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
    Subtarget(TT, CPU, FS, *this)
{
    initAsmInfo();
}

ProgMinerLabTargetMachine::~ProgMinerLabTargetMachine() = default;

namespace {

/// ProgMinerLab code generator pass configuration options
class ProgMinerLabPassConfig : public TargetPassConfig {

public:

    ProgMinerLabPassConfig(ProgMinerLabTargetMachine & TM, PassManagerBase & PM)
        : TargetPassConfig(TM, PM) {}

    ProgMinerLabTargetMachine & getProgMinerLabTargetMachine() const {
        return getTM<ProgMinerLabTargetMachine>();
    }

    bool addInstSelector() override;
};

} // anonymous namespace

TargetPassConfig * ProgMinerLabTargetMachine::createPassConfig(PassManagerBase & PM) {
    return new ProgMinerLabPassConfig(*this, PM);
}

bool ProgMinerLabPassConfig::addInstSelector() {
    addPass(createProgMinerLabISelDag(getProgMinerLabTargetMachine()));
    return false;
}
