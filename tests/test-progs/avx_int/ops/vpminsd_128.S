.text
.globl avx_vpminsd_128
.type avx_vpminsd_128, @function
avx_vpminsd_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu xmm0, [rdi]
    vmovdqu xmm1, [rsi]
    vpminsd xmm2, xmm0, xmm1
    vmovdqu [rdx], xmm2
    ret
