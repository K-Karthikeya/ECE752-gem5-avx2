.text
.globl avx_vmaxps_256
.type avx_vmaxps_256, @function
avx_vmaxps_256:
    # rdi = a, rsi = b, rdx = result
    vmovups (%rdi), %ymm0
    vmovups (%rsi), %ymm1
    vmaxps %ymm1, %ymm0, %ymm2
    vmovups %ymm2, (%rdx)
    vzeroupper
    ret
