.text
.globl avx_vpandd_256
.type avx_vpandd_256, @function
avx_vpandd_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu ymm0, [rdi]
    vmovdqu ymm1, [rsi]
    vpandd ymm2, ymm0, ymm1
    vmovdqu [rdx], ymm2
    vzeroupper
    ret
