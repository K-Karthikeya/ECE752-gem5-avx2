.text
.globl avx_vmovsldup_128
.type avx_vmovsldup_128, @function
avx_vmovsldup_128:
    # rdi = src, rsi = result
    vmovsldup (%rdi), %xmm0
    vmovups %xmm0, (%rsi)
    ret
