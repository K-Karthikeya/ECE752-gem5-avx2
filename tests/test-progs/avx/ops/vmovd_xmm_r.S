.text
.globl avx_vmovd_xmm_r
.type avx_vmovd_xmm_r, @function
avx_vmovd_xmm_r:
    # rdi = src (GPR value), rsi = result pointer
    vmovd %edi, %xmm0
    vmovdqu %xmm0, (%rsi)
    ret
