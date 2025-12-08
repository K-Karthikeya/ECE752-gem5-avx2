.text
.globl avx_vpbroadcastd_256
.type avx_vpbroadcastd_256, @function
avx_vpbroadcastd_256:
    # rdi = src (pointer to 4 bytes), rsi = result
    # Load 16 bytes from memory (includes our 32-bit value)
    vmovdqu (%rdi), %xmm0
    # Broadcast the lower 32 bits to all 8 dwords in ymm
    vpbroadcastd %xmm0, %ymm1
    vmovdqu %ymm1, (%rsi)
    vzeroupper
    ret
