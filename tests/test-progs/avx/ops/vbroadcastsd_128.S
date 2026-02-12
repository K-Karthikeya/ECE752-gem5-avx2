.globl avx_vbroadcastsd_128
.type avx_vbroadcastsd_128, @function
avx_vbroadcastsd_128:
    # Input: %rdi = ptr to source double, %rsi = ptr to output (2 doubles)
    # Note: vbroadcastsd xmm, m64 is not valid. Use vmovddup instead.
    vmovddup (%rdi), %xmm0
    vmovapd %xmm0, (%rsi)
    ret
