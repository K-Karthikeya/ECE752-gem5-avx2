.text
.globl avx_vbroadcastss_128
.type avx_vbroadcastss_128, @function
avx_vbroadcastss_128:
    # rdi = src (pointer to single float), rsi = result
    vbroadcastss (%rdi), %xmm0
    vmovups %xmm0, (%rsi)
    ret
