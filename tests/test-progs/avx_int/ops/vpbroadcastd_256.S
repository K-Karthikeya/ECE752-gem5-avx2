.text
.globl avx_vpbroadcastd_256
.type avx_vpbroadcastd_256, @function
avx_vpbroadcastd_256:
    # rdi = src (pointer to 4 bytes), rsi = result
    # Broadcast 32-bit value directly from memory to all lanes
    vpbroadcastd (%rdi), %ymm0
    vmovdqu %ymm0, (%rsi)
    vzeroupper
    ret
