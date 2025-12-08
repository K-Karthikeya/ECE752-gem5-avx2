.text
.globl avx_vmovddup_256
.type avx_vmovddup_256, @function
avx_vmovddup_256:
    # rdi = src, rsi = result
    vmovddup (%rdi), %ymm0
    vmovups %ymm0, (%rsi)
    vzeroupper
    ret
