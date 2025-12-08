.text
.globl avx_vmovdqu_ld_128
.type avx_vmovdqu_ld_128, @function
avx_vmovdqu_ld_128:
    # rdi = src, rsi = dst
    vmovdqu (%rdi), %xmm0
    vmovdqu %xmm0, (%rsi)
    ret
