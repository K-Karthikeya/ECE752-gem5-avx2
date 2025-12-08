.text
.globl avx_vpbroadcastd_128
.type avx_vpbroadcastd_128, @function
avx_vpbroadcastd_128:
    # rdi = src, rsi = result
    vmovd xmm0, [rdi]
    vpbroadcastd xmm1, xmm0
    vmovdqu [rsi], xmm1
    ret
