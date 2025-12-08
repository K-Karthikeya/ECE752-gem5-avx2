.text
.globl avx_vpbroadcastd_256
.type avx_vpbroadcastd_256, @function
avx_vpbroadcastd_256:
    # rdi = src, rsi = result
    vmovd (%rdi), %xmm0
    vpbroadcastd %xmm0, %ymm1
    vmovdqu %ymm1, (%rsi)
    vzeroupper
    ret
