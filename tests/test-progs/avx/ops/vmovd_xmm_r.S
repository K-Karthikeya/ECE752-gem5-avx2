.text
.globl avx_vmovd_xmm_r
.type avx_vmovd_xmm_r, @function
avx_vmovd_xmm_r:
    # edi = src (32-bit GPR value), rsi = result pointer
    # Note: first arg is already in %edi (lower 32 bits of %rdi)
    vmovd %edi, %xmm0
    vmovdqu %xmm0, (%rsi)
    ret
