.text
.globl avx_vmovdqu_st_256
.type avx_vmovdqu_st_256, @function
avx_vmovdqu_st_256:
    # rdi = src, rsi = dst
    vmovdqu (%rdi), %ymm0
    vmovdqu %ymm0, (%rsi)
    vzeroupper
    ret
