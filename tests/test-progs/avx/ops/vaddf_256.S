.intel_syntax noprefix
.globl avx_vaddf_256
avx_vaddf_256:
    ; a in rdi, b in rsi, out in rdx
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups ymm1, YMMWORD PTR [rsi]
    vaddps ymm2, ymm0, ymm1
    vmovups YMMWORD PTR [rdx], ymm2
    ret
