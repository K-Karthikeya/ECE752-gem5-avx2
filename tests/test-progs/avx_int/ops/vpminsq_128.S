.text
.globl avx_vpminsq_128
.type avx_vpminsq_128, @function
avx_vpminsq_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu xmm0, [rdi]
    vmovdqu xmm1, [rsi]
    vpminsq xmm2, xmm0, xmm1
    vmovdqu [rdx], xmm2
    ret
