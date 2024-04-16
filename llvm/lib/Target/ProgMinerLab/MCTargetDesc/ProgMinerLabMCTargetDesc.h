#pragma once

#include <memory>

#include "llvm/Support/Endian.h"


namespace llvm {

class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCCodeEmitter * createProgMinerLabMCCodeEmitter(const MCInstrInfo & MCII, MCContext & Ctx);

MCAsmBackend * createProgMinerLabAsmBackend(
    const Target & T,
    const MCSubtargetInfo & STI,
    const MCRegisterInfo & MRI,
    const MCTargetOptions & Options
);

std::unique_ptr<MCObjectTargetWriter> createProgMinerLabELFObjectWriter(bool Is64Bit, uint8_t OSABI);

} // namespace llvm


// Defines symbolic names for ProgMinerLab registers
// This defines a mapping from register name to register number
#define GET_REGINFO_ENUM
#include "ProgMinerLabGenRegisterInfo.inc"

// Defines symbolic names for the ProgMinerLab instructions
#define GET_INSTRINFO_ENUM
#include "ProgMinerLabGenInstrInfo.inc"
