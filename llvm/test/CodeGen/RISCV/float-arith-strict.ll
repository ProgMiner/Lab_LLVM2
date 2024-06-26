; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -mtriple=riscv32 -mattr=+f -verify-machineinstrs < %s \
; RUN:   -disable-strictnode-mutation -target-abi=ilp32f \
; RUN:   | FileCheck -check-prefixes=CHECKIF,RV32IF %s
; RUN: llc -mtriple=riscv64 -mattr=+f -verify-machineinstrs < %s \
; RUN:   -disable-strictnode-mutation -target-abi=lp64f \
; RUN:   | FileCheck -check-prefixes=CHECKIF,RV64IF %s
; RUN: llc -mtriple=riscv32 -verify-machineinstrs < %s \
; RUN:   -disable-strictnode-mutation | FileCheck -check-prefix=RV32I %s
; RUN: llc -mtriple=riscv64 -verify-machineinstrs < %s \
; RUN:   -disable-strictnode-mutation | FileCheck -check-prefix=RV64I %s
; RUN: llc -mtriple=riscv32 -mattr=+zfinx -verify-machineinstrs < %s \
; RUN:   -disable-strictnode-mutation -target-abi=ilp32 \
; RUN:   | FileCheck -check-prefixes=CHECKIZFINX,RV32IZFINX %s
; RUN: llc -mtriple=riscv64 -mattr=+zfinx -verify-machineinstrs < %s \
; RUN:   -disable-strictnode-mutation -target-abi=lp64 \
; RUN:   | FileCheck -check-prefixes=CHECKIZFINX,RV64IZFINX %s

define float @fadd_s(float %a, float %b) nounwind strictfp {
; CHECKIF-LABEL: fadd_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fadd.s fa0, fa0, fa1
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fadd_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fadd_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fadd_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fadd.s a0, a0, a1
; CHECKIZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.fadd.f32(float %a, float %b, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.fadd.f32(float, float, metadata, metadata)

define float @fsub_s(float %a, float %b) nounwind strictfp {
; CHECKIF-LABEL: fsub_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fsub.s fa0, fa0, fa1
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fsub_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call __subsf3
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fsub_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call __subsf3
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fsub_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fsub.s a0, a0, a1
; CHECKIZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.fsub.f32(float %a, float %b, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.fsub.f32(float, float, metadata, metadata)

define float @fmul_s(float %a, float %b) nounwind strictfp {
; CHECKIF-LABEL: fmul_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmul.s fa0, fa0, fa1
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fmul_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call __mulsf3
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fmul_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call __mulsf3
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fmul_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fmul.s a0, a0, a1
; CHECKIZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.fmul.f32(float %a, float %b, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.fmul.f32(float, float, metadata, metadata)

define float @fdiv_s(float %a, float %b) nounwind strictfp {
; CHECKIF-LABEL: fdiv_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fdiv.s fa0, fa0, fa1
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fdiv_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call __divsf3
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fdiv_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call __divsf3
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fdiv_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fdiv.s a0, a0, a1
; CHECKIZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.fdiv.f32(float %a, float %b, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.fdiv.f32(float, float, metadata, metadata)

define float @fsqrt_s(float %a) nounwind strictfp {
; CHECKIF-LABEL: fsqrt_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fsqrt.s fa0, fa0
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fsqrt_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call sqrtf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fsqrt_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call sqrtf
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fsqrt_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fsqrt.s a0, a0
; CHECKIZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.sqrt.f32(float %a, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.sqrt.f32(float, metadata, metadata)

define float @fmin_s(float %a, float %b) nounwind strictfp {
; RV32IF-LABEL: fmin_s:
; RV32IF:       # %bb.0:
; RV32IF-NEXT:    addi sp, sp, -16
; RV32IF-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32IF-NEXT:    call fminf
; RV32IF-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32IF-NEXT:    addi sp, sp, 16
; RV32IF-NEXT:    ret
;
; RV64IF-LABEL: fmin_s:
; RV64IF:       # %bb.0:
; RV64IF-NEXT:    addi sp, sp, -16
; RV64IF-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64IF-NEXT:    call fminf
; RV64IF-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64IF-NEXT:    addi sp, sp, 16
; RV64IF-NEXT:    ret
;
; RV32I-LABEL: fmin_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call fminf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fmin_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call fminf
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; RV32IZFINX-LABEL: fmin_s:
; RV32IZFINX:       # %bb.0:
; RV32IZFINX-NEXT:    addi sp, sp, -16
; RV32IZFINX-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32IZFINX-NEXT:    call fminf
; RV32IZFINX-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32IZFINX-NEXT:    addi sp, sp, 16
; RV32IZFINX-NEXT:    ret
;
; RV64IZFINX-LABEL: fmin_s:
; RV64IZFINX:       # %bb.0:
; RV64IZFINX-NEXT:    addi sp, sp, -16
; RV64IZFINX-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64IZFINX-NEXT:    call fminf
; RV64IZFINX-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64IZFINX-NEXT:    addi sp, sp, 16
; RV64IZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.minnum.f32(float %a, float %b, metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.minnum.f32(float, float, metadata) strictfp

define float @fmax_s(float %a, float %b) nounwind strictfp {
; RV32IF-LABEL: fmax_s:
; RV32IF:       # %bb.0:
; RV32IF-NEXT:    addi sp, sp, -16
; RV32IF-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32IF-NEXT:    call fmaxf
; RV32IF-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32IF-NEXT:    addi sp, sp, 16
; RV32IF-NEXT:    ret
;
; RV64IF-LABEL: fmax_s:
; RV64IF:       # %bb.0:
; RV64IF-NEXT:    addi sp, sp, -16
; RV64IF-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64IF-NEXT:    call fmaxf
; RV64IF-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64IF-NEXT:    addi sp, sp, 16
; RV64IF-NEXT:    ret
;
; RV32I-LABEL: fmax_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call fmaxf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fmax_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call fmaxf
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; RV32IZFINX-LABEL: fmax_s:
; RV32IZFINX:       # %bb.0:
; RV32IZFINX-NEXT:    addi sp, sp, -16
; RV32IZFINX-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32IZFINX-NEXT:    call fmaxf
; RV32IZFINX-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32IZFINX-NEXT:    addi sp, sp, 16
; RV32IZFINX-NEXT:    ret
;
; RV64IZFINX-LABEL: fmax_s:
; RV64IZFINX:       # %bb.0:
; RV64IZFINX-NEXT:    addi sp, sp, -16
; RV64IZFINX-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64IZFINX-NEXT:    call fmaxf
; RV64IZFINX-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64IZFINX-NEXT:    addi sp, sp, 16
; RV64IZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.maxnum.f32(float %a, float %b, metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.maxnum.f32(float, float, metadata) strictfp

define float @fmadd_s(float %a, float %b, float %c) nounwind strictfp {
; CHECKIF-LABEL: fmadd_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmadd.s fa0, fa0, fa1, fa2
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fmadd_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    call fmaf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fmadd_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -16
; RV64I-NEXT:    sd ra, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    call fmaf
; RV64I-NEXT:    ld ra, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 16
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fmadd_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fmadd.s a0, a0, a1, a2
; CHECKIZFINX-NEXT:    ret
  %1 = call float @llvm.experimental.constrained.fma.f32(float %a, float %b, float %c, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
declare float @llvm.experimental.constrained.fma.f32(float, float, float, metadata, metadata) strictfp

define float @fmsub_s(float %a, float %b, float %c) nounwind strictfp {
; CHECKIF-LABEL: fmsub_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmv.w.x fa5, zero
; CHECKIF-NEXT:    fadd.s fa5, fa2, fa5
; CHECKIF-NEXT:    fmsub.s fa0, fa0, fa1, fa5
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fmsub_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s0, 8(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s1, 4(sp) # 4-byte Folded Spill
; RV32I-NEXT:    mv s0, a1
; RV32I-NEXT:    mv s1, a0
; RV32I-NEXT:    mv a0, a2
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    lui a2, 524288
; RV32I-NEXT:    xor a2, a0, a2
; RV32I-NEXT:    mv a0, s1
; RV32I-NEXT:    mv a1, s0
; RV32I-NEXT:    call fmaf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s0, 8(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s1, 4(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fmsub_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -32
; RV64I-NEXT:    sd ra, 24(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s0, 16(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s1, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    mv s0, a1
; RV64I-NEXT:    mv s1, a0
; RV64I-NEXT:    mv a0, a2
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    lui a2, 524288
; RV64I-NEXT:    xor a2, a0, a2
; RV64I-NEXT:    mv a0, s1
; RV64I-NEXT:    mv a1, s0
; RV64I-NEXT:    call fmaf
; RV64I-NEXT:    ld ra, 24(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s0, 16(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s1, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 32
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fmsub_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fadd.s a2, a2, zero
; CHECKIZFINX-NEXT:    fmsub.s a0, a0, a1, a2
; CHECKIZFINX-NEXT:    ret
  %c_ = fadd float 0.0, %c ; avoid negation using xor
  %negc = fneg float %c_
  %1 = call float @llvm.experimental.constrained.fma.f32(float %a, float %b, float %negc, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}

define float @fnmadd_s(float %a, float %b, float %c) nounwind strictfp {
; CHECKIF-LABEL: fnmadd_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmv.w.x fa5, zero
; CHECKIF-NEXT:    fadd.s fa4, fa0, fa5
; CHECKIF-NEXT:    fadd.s fa5, fa2, fa5
; CHECKIF-NEXT:    fnmadd.s fa0, fa4, fa1, fa5
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fnmadd_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s0, 8(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s1, 4(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s2, 0(sp) # 4-byte Folded Spill
; RV32I-NEXT:    mv s0, a2
; RV32I-NEXT:    mv s1, a1
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    mv s2, a0
; RV32I-NEXT:    mv a0, s0
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    lui a2, 524288
; RV32I-NEXT:    xor a1, s2, a2
; RV32I-NEXT:    xor a2, a0, a2
; RV32I-NEXT:    mv a0, a1
; RV32I-NEXT:    mv a1, s1
; RV32I-NEXT:    call fmaf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s0, 8(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s1, 4(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s2, 0(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fnmadd_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -32
; RV64I-NEXT:    sd ra, 24(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s0, 16(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s1, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s2, 0(sp) # 8-byte Folded Spill
; RV64I-NEXT:    mv s0, a2
; RV64I-NEXT:    mv s1, a1
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    mv s2, a0
; RV64I-NEXT:    mv a0, s0
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    lui a2, 524288
; RV64I-NEXT:    xor a1, s2, a2
; RV64I-NEXT:    xor a2, a0, a2
; RV64I-NEXT:    mv a0, a1
; RV64I-NEXT:    mv a1, s1
; RV64I-NEXT:    call fmaf
; RV64I-NEXT:    ld ra, 24(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s0, 16(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s1, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s2, 0(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 32
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fnmadd_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fadd.s a0, a0, zero
; CHECKIZFINX-NEXT:    fadd.s a2, a2, zero
; CHECKIZFINX-NEXT:    fnmadd.s a0, a0, a1, a2
; CHECKIZFINX-NEXT:    ret
  %a_ = fadd float 0.0, %a
  %c_ = fadd float 0.0, %c
  %nega = fneg float %a_
  %negc = fneg float %c_
  %1 = call float @llvm.experimental.constrained.fma.f32(float %nega, float %b, float %negc, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}

define float @fnmadd_s_2(float %a, float %b, float %c) nounwind strictfp {
; CHECKIF-LABEL: fnmadd_s_2:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmv.w.x fa5, zero
; CHECKIF-NEXT:    fadd.s fa4, fa1, fa5
; CHECKIF-NEXT:    fadd.s fa5, fa2, fa5
; CHECKIF-NEXT:    fnmadd.s fa0, fa4, fa0, fa5
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fnmadd_s_2:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s0, 8(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s1, 4(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s2, 0(sp) # 4-byte Folded Spill
; RV32I-NEXT:    mv s0, a2
; RV32I-NEXT:    mv s1, a0
; RV32I-NEXT:    mv a0, a1
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    mv s2, a0
; RV32I-NEXT:    mv a0, s0
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    lui a2, 524288
; RV32I-NEXT:    xor a1, s2, a2
; RV32I-NEXT:    xor a2, a0, a2
; RV32I-NEXT:    mv a0, s1
; RV32I-NEXT:    call fmaf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s0, 8(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s1, 4(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s2, 0(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fnmadd_s_2:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -32
; RV64I-NEXT:    sd ra, 24(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s0, 16(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s1, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s2, 0(sp) # 8-byte Folded Spill
; RV64I-NEXT:    mv s0, a2
; RV64I-NEXT:    mv s1, a0
; RV64I-NEXT:    mv a0, a1
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    mv s2, a0
; RV64I-NEXT:    mv a0, s0
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    lui a2, 524288
; RV64I-NEXT:    xor a1, s2, a2
; RV64I-NEXT:    xor a2, a0, a2
; RV64I-NEXT:    mv a0, s1
; RV64I-NEXT:    call fmaf
; RV64I-NEXT:    ld ra, 24(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s0, 16(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s1, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s2, 0(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 32
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fnmadd_s_2:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fadd.s a1, a1, zero
; CHECKIZFINX-NEXT:    fadd.s a2, a2, zero
; CHECKIZFINX-NEXT:    fnmadd.s a0, a1, a0, a2
; CHECKIZFINX-NEXT:    ret
  %b_ = fadd float 0.0, %b
  %c_ = fadd float 0.0, %c
  %negb = fneg float %b_
  %negc = fneg float %c_
  %1 = call float @llvm.experimental.constrained.fma.f32(float %a, float %negb, float %negc, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}

define float @fnmsub_s(float %a, float %b, float %c) nounwind strictfp {
; CHECKIF-LABEL: fnmsub_s:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmv.w.x fa5, zero
; CHECKIF-NEXT:    fadd.s fa5, fa0, fa5
; CHECKIF-NEXT:    fnmsub.s fa0, fa5, fa1, fa2
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fnmsub_s:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s0, 8(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s1, 4(sp) # 4-byte Folded Spill
; RV32I-NEXT:    mv s0, a2
; RV32I-NEXT:    mv s1, a1
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    lui a1, 524288
; RV32I-NEXT:    xor a0, a0, a1
; RV32I-NEXT:    mv a1, s1
; RV32I-NEXT:    mv a2, s0
; RV32I-NEXT:    call fmaf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s0, 8(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s1, 4(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fnmsub_s:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -32
; RV64I-NEXT:    sd ra, 24(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s0, 16(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s1, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    mv s0, a2
; RV64I-NEXT:    mv s1, a1
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    lui a1, 524288
; RV64I-NEXT:    xor a0, a0, a1
; RV64I-NEXT:    mv a1, s1
; RV64I-NEXT:    mv a2, s0
; RV64I-NEXT:    call fmaf
; RV64I-NEXT:    ld ra, 24(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s0, 16(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s1, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 32
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fnmsub_s:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fadd.s a0, a0, zero
; CHECKIZFINX-NEXT:    fnmsub.s a0, a0, a1, a2
; CHECKIZFINX-NEXT:    ret
  %a_ = fadd float 0.0, %a
  %nega = fneg float %a_
  %1 = call float @llvm.experimental.constrained.fma.f32(float %nega, float %b, float %c, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}

define float @fnmsub_s_2(float %a, float %b, float %c) nounwind strictfp {
; CHECKIF-LABEL: fnmsub_s_2:
; CHECKIF:       # %bb.0:
; CHECKIF-NEXT:    fmv.w.x fa5, zero
; CHECKIF-NEXT:    fadd.s fa5, fa1, fa5
; CHECKIF-NEXT:    fnmsub.s fa0, fa5, fa0, fa2
; CHECKIF-NEXT:    ret
;
; RV32I-LABEL: fnmsub_s_2:
; RV32I:       # %bb.0:
; RV32I-NEXT:    addi sp, sp, -16
; RV32I-NEXT:    sw ra, 12(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s0, 8(sp) # 4-byte Folded Spill
; RV32I-NEXT:    sw s1, 4(sp) # 4-byte Folded Spill
; RV32I-NEXT:    mv s0, a2
; RV32I-NEXT:    mv s1, a0
; RV32I-NEXT:    mv a0, a1
; RV32I-NEXT:    li a1, 0
; RV32I-NEXT:    call __addsf3
; RV32I-NEXT:    lui a1, 524288
; RV32I-NEXT:    xor a1, a0, a1
; RV32I-NEXT:    mv a0, s1
; RV32I-NEXT:    mv a2, s0
; RV32I-NEXT:    call fmaf
; RV32I-NEXT:    lw ra, 12(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s0, 8(sp) # 4-byte Folded Reload
; RV32I-NEXT:    lw s1, 4(sp) # 4-byte Folded Reload
; RV32I-NEXT:    addi sp, sp, 16
; RV32I-NEXT:    ret
;
; RV64I-LABEL: fnmsub_s_2:
; RV64I:       # %bb.0:
; RV64I-NEXT:    addi sp, sp, -32
; RV64I-NEXT:    sd ra, 24(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s0, 16(sp) # 8-byte Folded Spill
; RV64I-NEXT:    sd s1, 8(sp) # 8-byte Folded Spill
; RV64I-NEXT:    mv s0, a2
; RV64I-NEXT:    mv s1, a0
; RV64I-NEXT:    mv a0, a1
; RV64I-NEXT:    li a1, 0
; RV64I-NEXT:    call __addsf3
; RV64I-NEXT:    lui a1, 524288
; RV64I-NEXT:    xor a1, a0, a1
; RV64I-NEXT:    mv a0, s1
; RV64I-NEXT:    mv a2, s0
; RV64I-NEXT:    call fmaf
; RV64I-NEXT:    ld ra, 24(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s0, 16(sp) # 8-byte Folded Reload
; RV64I-NEXT:    ld s1, 8(sp) # 8-byte Folded Reload
; RV64I-NEXT:    addi sp, sp, 32
; RV64I-NEXT:    ret
;
; CHECKIZFINX-LABEL: fnmsub_s_2:
; CHECKIZFINX:       # %bb.0:
; CHECKIZFINX-NEXT:    fadd.s a1, a1, zero
; CHECKIZFINX-NEXT:    fnmsub.s a0, a1, a0, a2
; CHECKIZFINX-NEXT:    ret
  %b_ = fadd float 0.0, %b
  %negb = fneg float %b_
  %1 = call float @llvm.experimental.constrained.fma.f32(float %a, float %negb, float %c, metadata !"round.dynamic", metadata !"fpexcept.strict") strictfp
  ret float %1
}
