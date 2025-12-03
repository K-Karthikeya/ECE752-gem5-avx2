.intel_syntax noprefix
.globl avx_vaddf_128
avx_vaddf_128:
    ; a in rdi, b in rsi, out in rdx
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vaddps xmm2, xmm0, xmm1
    vmovups XMMWORD PTR [rdx], xmm2
    ret
