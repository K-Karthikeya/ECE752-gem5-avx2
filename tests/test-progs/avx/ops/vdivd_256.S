.globl avx_vdivd_256
.type avx_vdivd_256, @function
avx_vdivd_256:
    # Input: %rdi = ptr to a (4 doubles), %rsi = ptr to b (4 doubles), %rdx = ptr to output
    vmovapd (%rdi), %ymm0
    vdivpd (%rsi), %ymm0, %ymm1
    vmovapd %ymm1, (%rdx)
    vzeroupper
    ret
