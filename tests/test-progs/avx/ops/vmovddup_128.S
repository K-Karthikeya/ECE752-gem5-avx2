.text
.globl avx_vmovddup_128
.type avx_vmovddup_128, @function
avx_vmovddup_128:
    # rdi = src, rsi = result
    vmovddup (%rdi), %xmm0
    vmovups %xmm0, (%rsi)
    ret
