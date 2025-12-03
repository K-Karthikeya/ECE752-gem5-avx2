.intel_syntax noprefix
.globl avx_vfmadd231f_128
avx_vfmadd231f_128:
    # a in rdi, b in rsi, c in rdx, out in rcx
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vmovups xmm2, XMMWORD PTR [rdx]
    vfmadd231ps xmm0, xmm1, xmm2   # xmm0 = xmm0 * xmm1 + xmm2
    vmovups XMMWORD PTR [rcx], xmm0
    ret
