.text
.globl avx_vmovsldup_256
.type avx_vmovsldup_256, @function
avx_vmovsldup_256:
    # rdi = src, rsi = result
    vmovsldup (%rdi), %ymm0
    vmovups %ymm0, (%rsi)
    vzeroupper
    ret
