.text
.globl avx_vpminsq_256
.type avx_vpminsq_256, @function
avx_vpminsq_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu ymm0, [rdi]
    vmovdqu ymm1, [rsi]
    vpminsq ymm2, ymm0, ymm1
    vmovdqu [rdx], ymm2
    vzeroupper
    ret
