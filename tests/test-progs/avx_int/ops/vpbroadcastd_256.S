.text
.globl avx_vpbroadcastd_256
.type avx_vpbroadcastd_256, @function
avx_vpbroadcastd_256:
    # rdi = src, rsi = result
    vmovd xmm0, [rdi]
    vpbroadcastd ymm1, xmm0
    vmovdqu [rsi], ymm1
    vzeroupper
    ret
