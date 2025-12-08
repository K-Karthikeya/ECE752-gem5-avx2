.text
.globl avx_vpaddd_128
.type avx_vpaddd_128, @function
avx_vpaddd_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu xmm0, [rdi]
    vmovdqu xmm1, [rsi]
    vpaddd xmm2, xmm0, xmm1
    vmovdqu [rdx], xmm2
    ret
