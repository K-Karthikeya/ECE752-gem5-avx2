.text
.globl avx_vpsubd_128
.type avx_vpsubd_128, @function
avx_vpsubd_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpsubd %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
