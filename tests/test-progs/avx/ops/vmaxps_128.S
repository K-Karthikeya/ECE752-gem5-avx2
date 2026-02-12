.text
.globl avx_vmaxps_128
.type avx_vmaxps_128, @function
avx_vmaxps_128:
    # rdi = a, rsi = b, rdx = result
    vmovups (%rdi), %xmm0
    vmovups (%rsi), %xmm1
    vmaxps %xmm1, %xmm0, %xmm2
    vmovups %xmm2, (%rdx)
    ret
