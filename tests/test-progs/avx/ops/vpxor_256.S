.text
.globl avx_vpxor_256
.type avx_vpxor_256, @function
avx_vpxor_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %ymm0
    vmovdqu (%rsi), %ymm1
    vpxor %ymm1, %ymm0, %ymm2
    vmovdqu %ymm2, (%rdx)
    vzeroupper
    ret
