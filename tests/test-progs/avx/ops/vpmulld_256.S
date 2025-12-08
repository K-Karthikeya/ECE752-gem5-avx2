.text
.globl avx_vpmulld_256
.type avx_vpmulld_256, @function
avx_vpmulld_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %ymm0
    vmovdqu (%rsi), %ymm1
    vpmulld %ymm1, %ymm0, %ymm2
    vmovdqu %ymm2, (%rdx)
    vzeroupper
    ret
