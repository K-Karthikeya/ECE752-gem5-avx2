.text
.globl avx_vmovdqu_load_128
.type avx_vmovdqu_load_128, @function
avx_vmovdqu_load_128:
    # rdi = src, rsi = result
    vmovdqu (%rdi), %xmm0
    vmovdqu %xmm0, (%rsi)
    ret

.globl avx_vmovdqu_store_128
.type avx_vmovdqu_store_128, @function
avx_vmovdqu_store_128:
    # rdi = src, rsi = result
    vmovdqu (%rdi), %xmm0
    vmovdqu %xmm0, (%rsi)
    ret
