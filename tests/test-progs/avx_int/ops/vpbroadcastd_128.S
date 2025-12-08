.text
.globl avx_vpbroadcastd_128
.type avx_vpbroadcastd_128, @function
avx_vpbroadcastd_128:
    # rdi = src (pointer to 4 bytes), rsi = result
    # Load 16 bytes (128-bit) from memory, which includes our 32-bit value
    vmovdqu (%rdi), %xmm0
    # Broadcast the lower 32 bits to all 4 dwords
    vpbroadcastd %xmm0, %xmm1
    vmovdqu %xmm1, (%rsi)
    ret
