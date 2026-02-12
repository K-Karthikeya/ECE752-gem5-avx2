.text
.globl avx_vbroadcastss_256
.type avx_vbroadcastss_256, @function
avx_vbroadcastss_256:
    # rdi = src (pointer to single float), rsi = result
    vbroadcastss (%rdi), %ymm0
    vmovups %ymm0, (%rsi)
    vzeroupper
    ret
