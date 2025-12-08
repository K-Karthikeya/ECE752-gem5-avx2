.text
.globl avx_vpminsd_128
.type avx_vpminsd_128, @function
avx_vpminsd_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpminsd %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
