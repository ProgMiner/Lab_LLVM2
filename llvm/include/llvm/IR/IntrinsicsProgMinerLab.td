
// all intrinsics will start with "llvm.progminerlab."
let TargetPrefix = "progminerlab" in {

def int_progminerlab_pixeloff: ClangBuiltin<"sim_pixel_off">,
    Intrinsic<[], [llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_pixelon: ClangBuiltin<"sim_pixel_on">,
    Intrinsic<[], [llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_flush: ClangBuiltin<"sim_flush">,
    Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_rand: ClangBuiltin<"sim_rand">,
    Intrinsic<[llvm_i32_ty], [], [IntrNoMem, IntrHasSideEffects]>;

def int_progminerlab_dump: ClangBuiltin<"dump">,
    Intrinsic<[], [llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

}
