.text
.globl avx_vpmulld_128
.type avx_vpmulld_128, @function
avx_vpmulld_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpmulld %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
