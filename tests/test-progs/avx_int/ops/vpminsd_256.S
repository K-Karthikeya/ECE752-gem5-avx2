.text
.globl avx_vpminsd_256
.type avx_vpminsd_256, @function
avx_vpminsd_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu ymm0, [rdi]
    vmovdqu ymm1, [rsi]
    vpminsd ymm2, ymm0, ymm1
    vmovdqu [rdx], ymm2
    vzeroupper
    ret
