.text
.globl avx_vpbroadcastd_128
.type avx_vpbroadcastd_128, @function
avx_vpbroadcastd_128:
    # rdi = src, rsi = result
    vmovd (%rdi), %xmm0
    vpbroadcastd %xmm0, %xmm1
    vmovdqu %xmm1, (%rsi)
    ret
