.intel_syntax noprefix
.globl avx_vdivf_256
avx_vdivf_256:
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups ymm1, YMMWORD PTR [rsi]
    vdivps ymm2, ymm0, ymm1
    vmovups YMMWORD PTR [rdx], ymm2
    ret
