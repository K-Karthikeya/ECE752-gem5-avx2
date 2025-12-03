# AVX double-precision subset explicit assembly test
# Exercises: VADDPD, VSUBPD, VMULPD, VDIVPD, VMOVDDUP, VZEROUPPER
# Writes results to buffers for C harness verification.
#
# Linux System V AMD64 calling convention assumed.
# Function prototype:
#   void avx_subset_test(double *a, double *b, double *out, double *tmp, void *unused);
# Semantics:
#   - out[0..3]   = VADDPD(a,b)
#   - out[4..7]   = VSUBPD(a,b)
#   - out[8..11]  = VMULPD(a,b)
#   - out[12..15] = VDIVPD(a,b)
#   - tmp[0..1]   = VMOVDDUP_XMM from a[0..1] (duplicates a[0] into both lanes)
#   - tmp[2..5]   = VMOVDDUP_YMM from b[0..3] (duplicates b[0] to [0,1] and b[2] to [2,3])

    .text
    .globl avx_subset_test
    .type avx_subset_test,@function

avx_subset_test:
    # Load a and b as 256b (4 doubles each)
    vmovapd   (%rdi), %ymm0          # ymm0 = a[0..3]
    vmovapd   (%rsi), %ymm1          # ymm1 = b[0..3]

    # out[0..3] = a + b
    vaddpd    %ymm1, %ymm0, %ymm2
    vmovapd   %ymm2, (%rdx)

    # out[4..7] = a - b
    vsubpd    %ymm1, %ymm0, %ymm3
    vmovapd   %ymm3, 32(%rdx)

    # out[8..11] = a * b
    vmulpd    %ymm1, %ymm0, %ymm4
    vmovapd   %ymm4, 64(%rdx)

    # out[12..15] = a / b
    vdivpd    %ymm1, %ymm0, %ymm5
    vmovapd   %ymm5, 96(%rdx)

    # tmp[0..1] = movddup XMM from a (duplicate a[0] across both lanes)
    vmovapd   (%rdi), %xmm6          # load 128 bits (2 doubles) from a
    vmovddup  %xmm6, %xmm7           # duplicate low 64-bit within 128b
    vmovapd   %xmm7, (%rcx)

    # tmp[2..5] = movddup YMM from b (duplicate low of each 128b lane)
    vmovapd   (%rsi), %ymm8          # load b[0..3]
    vmovddup  %ymm8, %ymm9
    vmovapd   %ymm9, 16(%rcx)

    vzeroupper
    ret

    .size avx_subset_test, .-avx_subset_test

    # Mark that this object does not require an executable stack
    .section .note.GNU-stack,"",@progbits
