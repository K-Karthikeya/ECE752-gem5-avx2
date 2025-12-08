.text
.globl avx_vpsubd_256
.type avx_vpsubd_256, @function
avx_vpsubd_256:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %ymm0
    vmovdqu (%rsi), %ymm1
    vpsubd %ymm1, %ymm0, %ymm2
    vmovdqu %ymm2, (%rdx)
    vzeroupper
    ret
