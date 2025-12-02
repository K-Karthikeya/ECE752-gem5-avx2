.intel_syntax noprefix
# FMA-only AVX test: VFMADD231PS YMM
# void avx_fma_test(float *a, float *b, float *c, float *out)
# SysV AMD64: rdi=a, rsi=b, rdx=c, rcx=out
    .text
    .globl avx_fma_test
    .type avx_fma_test,@function
avx_fma_test:
    push rbp
    mov rbp, rsp
    # Load inputs
    vmovaps [rdi], ymm0   # a -> dest (accumulator)
    vmovaps [rsi], ymm1   # b -> multiplier src2
    vmovaps [rdx], ymm2   # c -> addend src1 (vvvv)
    # VFMADD231PS dest = (dest * src2) + src1
    # Operand order (Intel syntax): dest, src1 (addend), src2 (multiplier)
    vfmadd231ps ymm0, ymm2, ymm1
    vmovaps ymm0, [rcx]   # store result
    vzeroupper
    pop rbp
    ret

    .section .note.GNU-stack,"",@progbits
