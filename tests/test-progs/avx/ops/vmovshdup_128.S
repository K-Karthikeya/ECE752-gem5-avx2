.text
.globl avx_vmovshdup_128
.type avx_vmovshdup_128, @function
avx_vmovshdup_128:
    # rdi = src, rsi = result
    vmovshdup (%rdi), %xmm0
    vmovups %xmm0, (%rsi)
    ret
