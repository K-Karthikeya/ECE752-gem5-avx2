.text
.globl avx_vpor_128
.type avx_vpor_128, @function
avx_vpor_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpor %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
