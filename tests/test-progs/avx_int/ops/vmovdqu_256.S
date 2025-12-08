.text
.globl avx_vmovdqu_load_256
.type avx_vmovdqu_load_256, @function
avx_vmovdqu_load_256:
    # rdi = src, rsi = result
    vmovdqu ymm0, [rdi]
    vmovdqu [rsi], ymm0
    vzeroupper
    ret

.globl avx_vmovdqu_store_256
.type avx_vmovdqu_store_256, @function
avx_vmovdqu_store_256:
    # rdi = src, rsi = result
    vmovdqu ymm0, [rdi]
    vmovdqu [rsi], ymm0
    vzeroupper
    ret
