.globl avx_vmuld_256
.type avx_vmuld_256, @function
avx_vmuld_256:
    # Input: %rdi = ptr to a (4 doubles), %rsi = ptr to b (4 doubles), %rdx = ptr to output
    vmovapd (%rdi), %ymm0
    vmulpd (%rsi), %ymm0, %ymm1
    vmovapd %ymm1, (%rdx)
    vzeroupper
    ret
