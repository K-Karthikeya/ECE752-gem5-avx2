.intel_syntax noprefix
.globl avx_ldstfp_256
avx_ldstfp_256:
    ; mem in rdi, out in rsi
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups YMMWORD PTR [rdi], ymm0
    vmovups ymm1, YMMWORD PTR [rdi]
    vmovups YMMWORD PTR [rsi], ymm1
    ret
