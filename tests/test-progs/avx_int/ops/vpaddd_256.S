.text
.globl avx_vpaddd_256
.type avx_vpaddd_256, @function
avx_vpaddd_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu ymm0, [rdi]
    vmovdqu ymm1, [rsi]
    vpaddd ymm2, ymm0, ymm1
    vmovdqu [rdx], ymm2
    vzeroupper
    ret
