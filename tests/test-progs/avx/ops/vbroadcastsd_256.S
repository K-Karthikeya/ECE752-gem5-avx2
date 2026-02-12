.globl avx_vbroadcastsd_256
.type avx_vbroadcastsd_256, @function
avx_vbroadcastsd_256:
    # Input: %rdi = ptr to source double, %rsi = ptr to output (4 doubles)
    vbroadcastsd (%rdi), %ymm0
    vmovapd %ymm0, (%rsi)
    vzeroupper
    ret
