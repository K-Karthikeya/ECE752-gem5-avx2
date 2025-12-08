.text
.globl avx_vpbroadcastd_256
.type avx_vpbroadcastd_256, @function
avx_vpbroadcastd_256:
    # rdi = src, rsi = result
    vpbroadcastd (%rdi), %ymm0
    vmovdqu %ymm0, (%rsi)
    vzeroupper
    ret
