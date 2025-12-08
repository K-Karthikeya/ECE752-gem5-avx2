.text
.globl avx_vmovdqu_ld_256
.type avx_vmovdqu_ld_256, @function
avx_vmovdqu_ld_256:
    # rdi = src, rsi = dst
    vmovdqu (%rdi), %ymm0
    vmovdqu %ymm0, (%rsi)
    vzeroupper
    ret
