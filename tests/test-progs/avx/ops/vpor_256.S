.text
.globl avx_vpor_256
.type avx_vpor_256, @function
avx_vpor_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %ymm0
    vmovdqu (%rsi), %ymm1
    vpor %ymm1, %ymm0, %ymm2
    vmovdqu %ymm2, (%rdx)
    vzeroupper
    ret
