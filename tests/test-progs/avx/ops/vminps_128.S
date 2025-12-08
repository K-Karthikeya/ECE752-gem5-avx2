.text
.globl avx_vminps_128
.type avx_vminps_128, @function
avx_vminps_128:
    # rdi = a, rsi = b, rdx = result
    vmovups (%rdi), %xmm0
    vmovups (%rsi), %xmm1
    vminps %xmm1, %xmm0, %xmm2
    vmovups %xmm2, (%rdx)
    ret
