.text
.globl avx_vmovdqu_load_128
.type avx_vmovdqu_load_128, @function
avx_vmovdqu_load_128:
    # rdi = src, rsi = result
    vmovdqu xmm0, [rdi]
    vmovdqu [rsi], xmm0
    ret

.globl avx_vmovdqu_store_128
.type avx_vmovdqu_store_128, @function
avx_vmovdqu_store_128:
    # rdi = src, rsi = result
    vmovdqu xmm0, [rdi]
    vmovdqu [rsi], xmm0
    ret
