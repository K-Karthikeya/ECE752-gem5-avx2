.globl avx_vdivd_128
.type avx_vdivd_128, @function
avx_vdivd_128:
    # Input: %rdi = ptr to a (2 doubles), %rsi = ptr to b (2 doubles), %rdx = ptr to output
    vmovapd (%rdi), %xmm0
    vdivpd (%rsi), %xmm0, %xmm1
    vmovapd %xmm1, (%rdx)
    ret
