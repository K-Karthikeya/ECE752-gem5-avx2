.text
.globl avx_vmovd_r_xmm
.type avx_vmovd_r_xmm, @function
avx_vmovd_r_xmm:
    # rdi = src XMM pointer, return value in eax
    vmovdqu (%rdi), %xmm0
    vmovd %xmm0, %eax
    ret
