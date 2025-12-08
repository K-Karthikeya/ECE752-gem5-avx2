.text
.globl avx_vpminsq_128
.type avx_vpminsq_128, @function
avx_vpminsq_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpminsq %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
