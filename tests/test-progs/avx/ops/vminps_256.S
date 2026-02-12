.text
.globl avx_vminps_256
.type avx_vminps_256, @function
avx_vminps_256:
    # rdi = a, rsi = b, rdx = result
    vmovups (%rdi), %ymm0
    vmovups (%rsi), %ymm1
    vminps %ymm1, %ymm0, %ymm2
    vmovups %ymm2, (%rdx)
    vzeroupper
    ret
