.intel_syntax noprefix
.globl avx_vfmadd231f_256
avx_vfmadd231f_256:
    ; a in rdi, b in rsi, c in rdx, out in rcx
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups ymm1, YMMWORD PTR [rsi]
    vmovups ymm2, YMMWORD PTR [rdx]
    vfmadd231ps ymm0, ymm1, ymm2   ; ymm0 = ymm0 * ymm1 + ymm2
    vmovups YMMWORD PTR [rcx], ymm0
    ret
