
// all intrinsics will start with "llvm.progminerlab."
let TargetPrefix = "progminerlab" in {

def int_progminerlab_pixeloff: ClangBuiltin<"simPixelOff">,
    Intrinsic<[], [llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_pixelon: ClangBuiltin<"simPixelOn">,
    Intrinsic<[], [llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_flush: ClangBuiltin<"simFlush">,
    Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_rand: ClangBuiltin<"simRand">,
    Intrinsic<[llvm_i32_ty], [], [IntrHasSideEffects]>;

}
