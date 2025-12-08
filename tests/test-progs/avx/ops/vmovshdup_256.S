.text
.globl avx_vmovshdup_256
.type avx_vmovshdup_256, @function
avx_vmovshdup_256:
    # rdi = src, rsi = result
    vmovshdup (%rdi), %ymm0
    vmovups %ymm0, (%rsi)
    vzeroupper
    ret
