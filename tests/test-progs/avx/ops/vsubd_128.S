.globl avx_vsubd_128
.type avx_vsubd_128, @function
avx_vsubd_128:
    # Input: %rdi = ptr to a (2 doubles), %rsi = ptr to b (2 doubles), %rdx = ptr to output
    vmovapd (%rdi), %xmm0
    vsubpd (%rsi), %xmm0, %xmm1
    vmovapd %xmm1, (%rdx)
    ret
