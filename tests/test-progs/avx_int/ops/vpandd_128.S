.text
.globl avx_vpandd_128
.type avx_vpandd_128, @function
avx_vpandd_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu xmm0, [rdi]
    vmovdqu xmm1, [rsi]
    vpandd xmm2, xmm0, xmm1
    vmovdqu [rdx], xmm2
    ret
