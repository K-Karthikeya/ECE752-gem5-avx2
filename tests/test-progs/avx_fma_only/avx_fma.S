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
    # Load inputs (Intel syntax: dest, src). Use loads here.
    vmovaps ymm0, [rdi]   # a -> dest (accumulator)
    vmovaps ymm1, [rsi]   # b -> multiplier src2
    vmovaps ymm2, [rdx]   # c -> addend src1 (vvvv)
    # Harness expects: out[i] = a[i] * b[i] + c[i]
    # Use VFMADD213PS which computes: dest = (dest * src1) + src2
    # Operand order (Intel syntax): dest, src1 (multiplier=b), src2 (addend=c)
    vfmadd213ps ymm0, ymm1, ymm2
    vmovaps [rcx], ymm0   # store result
    vzeroupper
    pop rbp
    ret

    .section .note.GNU-stack,"",@progbits
