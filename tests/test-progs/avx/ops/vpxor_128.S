.text
.globl avx_vpxor_128
.type avx_vpxor_128, @function
avx_vpxor_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpxor %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
