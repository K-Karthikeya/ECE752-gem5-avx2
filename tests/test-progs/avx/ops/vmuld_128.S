.globl avx_vmuld_128
.type avx_vmuld_128, @function
avx_vmuld_128:
    # Input: %rdi = ptr to a (2 doubles), %rsi = ptr to b (2 doubles), %rdx = ptr to output
    vmovapd (%rdi), %xmm0
    vmulpd (%rsi), %xmm0, %xmm1
    vmovapd %xmm1, (%rdx)
    ret
