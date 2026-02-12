.text
.globl avx_vpbroadcastd_128
.type avx_vpbroadcastd_128, @function
avx_vpbroadcastd_128:
    # rdi = src (pointer to 4 bytes), rsi = result
    # Broadcast 32-bit value directly from memory to all lanes
    vpbroadcastd (%rdi), %xmm0
    vmovdqu %xmm0, (%rsi)
    ret
