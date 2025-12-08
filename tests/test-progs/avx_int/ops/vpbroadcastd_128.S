.text
.globl avx_vpbroadcastd_128
.type avx_vpbroadcastd_128, @function
avx_vpbroadcastd_128:
    # rdi = src, rsi = result
    vpbroadcastd (%rdi), %xmm0
    vmovdqu %xmm0, (%rsi)
    ret
