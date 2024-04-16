#pragma once

#include <vector>

#include "llvm/CodeGen/MachineFunction.h"


namespace llvm {

/// this class is derived from MachineFunction private
/// ProgMinerLab target-specific information for each MachineFunction
class ProgMinerLabFunctionInfo : public MachineFunctionInfo {

    virtual void anchor();

    bool ReturnStackOffsetSet = false;
    unsigned ReturnStackOffset = -1U;

    /// FrameIndex for start of varargs area
    int VarArgsFrameIndex = 0;

    /// size of the save area used for varargs
    int VarArgsSaveSize = 0;

    /// size of stack frame to save callee saved registers
    unsigned CalleeSavedStackSize = 0;

public:

    ProgMinerLabFunctionInfo() {}
    explicit ProgMinerLabFunctionInfo(MachineFunction & MF) {}

    ~ProgMinerLabFunctionInfo() {}

    int getVarArgsFrameIndex() const {
        return VarArgsFrameIndex;
    }

    void setVarArgsFrameIndex(int Off) {
        VarArgsFrameIndex = Off;
    }

    int getVarArgsSaveSize() const {
        return VarArgsSaveSize;
    }

    void setVarArgsSaveSize(int Size) {
        VarArgsSaveSize = Size;
    }

    unsigned getCalleeSavedStackSize() const {
        return CalleeSavedStackSize;
    }

    void setCalleeSavedStackSize(unsigned Size) {
        CalleeSavedStackSize = Size;
    }

    unsigned getReturnStackOffset() const {
        assert(ReturnStackOffsetSet && "Return stack offset not set");

        return ReturnStackOffset;
    }

    void setReturnStackOffset(unsigned Off) {
        assert(!ReturnStackOffsetSet && "Return stack offset set twice");

        ReturnStackOffset = Off;
        ReturnStackOffsetSet = true;
    }
};

} // namespace llvm
