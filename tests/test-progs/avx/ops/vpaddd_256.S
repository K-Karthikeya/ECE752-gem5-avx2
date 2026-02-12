.text
.globl avx_vpaddd_256
.type avx_vpaddd_256, @function
avx_vpaddd_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %ymm0
    vmovdqu (%rsi), %ymm1
    vpaddd %ymm1, %ymm0, %ymm2
    vmovdqu %ymm2, (%rdx)
    vzeroupper
    ret
