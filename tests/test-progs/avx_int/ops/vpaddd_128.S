.text
.globl avx_vpaddd_128
.type avx_vpaddd_128, @function
avx_vpaddd_128:
    # rdi = a, rsi = b, rdx = result
    vmovdqu (%rdi), %xmm0
    vmovdqu (%rsi), %xmm1
    vpaddd %xmm1, %xmm0, %xmm2
    vmovdqu %xmm2, (%rdx)
    ret
