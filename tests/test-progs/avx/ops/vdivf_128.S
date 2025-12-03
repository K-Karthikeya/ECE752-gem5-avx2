.intel_syntax noprefix
.globl avx_vdivf_128
avx_vdivf_128:
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vdivps xmm2, xmm0, xmm1
    vmovups XMMWORD PTR [rdx], xmm2
    ret
