.globl avx_vbroadcastsd_128
.type avx_vbroadcastsd_128, @function
avx_vbroadcastsd_128:
    # Input: %rdi = ptr to source double, %rsi = ptr to output (2 doubles)
    vbroadcastsd (%rdi), %xmm0
    vmovapd %xmm0, (%rsi)
    ret
