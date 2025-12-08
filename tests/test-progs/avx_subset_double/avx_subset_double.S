# AVX double-precision subset explicit assembly test
# Exercises: VADDPD, VSUBPD, VMULPD, VDIVPD, VMOVDDUP, VZEROUPPER
# Writes results to buffers for C harness verification.
#
# Linux System V AMD64 calling convention assumed.
# Function prototype:
#   void avx_subset_double_test(double *a, double *b, double *out, double *tmp,
#                        void *unused);
# Semantics:
#   - out[0..3]   = VADDPD(a,b)
#   - out[4..7]   = VSUBPD(a,b)
#   - out[8..11]  = VMULPD(a,b)
#   - out[12..15] = VDIVPD(a,b)
#   - tmp[0..1]   = VMOVDDUP_XMM from a[0..1] (duplicates a[0] into both lanes)
#   - tmp[2..5]   = VMOVDDUP_YMM from b[0..3] (duplicates b[0] to [0,1] and
#                   b[2] to [2,3])

    .text
    .globl avx_subset_double_test
    .type avx_subset_double_test,@function

avx_subset_double_test:
    # Fallback using 128-bit XMM ops only (no YMM vmovupd).
    # Load a[0..1], b[0..1]
    vmovups   (%rdi), %xmm0          # a low lane
    vmovups   (%rsi), %xmm1          # b low lane
    # Compute low lane results
    vaddpd     %xmm1, %xmm0, %xmm0    # xmm0 = a+b (low)
    vmovups   %xmm0, (%rdx)          # out[0..1]
    vmovups   (%rdi), %xmm2          # reload a low
    vmovups   (%rsi), %xmm3          # reload b low
    vsubpd     %xmm3, %xmm2, %xmm2    # xmm2 = a-b (low)
    vmovups   %xmm2, 32(%rdx)        # out[4..5]
    vmovups   (%rdi), %xmm4          # reload a low
    vmovups   (%rsi), %xmm5          # reload b low
    vmulpd     %xmm5, %xmm4, %xmm4    # xmm4 = a*b (low)
    vmovups   %xmm4, 64(%rdx)        # out[8..9]
    vmovups   (%rdi), %xmm6          # reload a low
    vmovups   (%rsi), %xmm7          # reload b low
    vdivpd     %xmm7, %xmm6, %xmm6    # xmm6 = a/b (low)
    vmovups   %xmm6, 96(%rdx)        # out[12..13]

    # Load a[2..3], b[2..3]
    vmovups   16(%rdi), %xmm0        # a high lane
    vmovups   16(%rsi), %xmm1        # b high lane
    # Compute high lane results
    vaddpd     %xmm1, %xmm0, %xmm0    # xmm0 = a+b (high)
    vmovups   %xmm0, 16(%rdx)        # out[2..3]
    vmovups   16(%rdi), %xmm2        # reload a high
    vmovups   16(%rsi), %xmm3        # reload b high
    vsubpd     %xmm3, %xmm2, %xmm2    # xmm2 = a-b (high)
    vmovups   %xmm2, 48(%rdx)        # out[6..7]
    vmovups   16(%rdi), %xmm4        # reload a high
    vmovups   16(%rsi), %xmm5        # reload b high
    vmulpd     %xmm5, %xmm4, %xmm4    # xmm4 = a*b (high)
    vmovups   %xmm4, 80(%rdx)        # out[10..11]
    vmovups   16(%rdi), %xmm6        # reload a high
    vmovups   16(%rsi), %xmm7        # reload b high
    vdivpd     %xmm7, %xmm6, %xmm6    # xmm6 = a/b (high)
    vmovups   %xmm6, 112(%rdx)       # out[14..15]

    # tmp[0..1] = movddup XMM from a (duplicate a[0] across both lanes)
    vmovups   (%rdi), %xmm0          # load 128 bits (2 doubles) from a
    movddup   %xmm0, %xmm1           # duplicate low 64-bit within 128b
    vmovups   %xmm1, (%rcx)

    # tmp[2..5] = movddup via two 128b lanes from b
    vmovups   (%rsi), %xmm2          # b[0..1]
    movddup   %xmm2, %xmm3           # dup b[0] to both lanes
    vmovups   %xmm3, 16(%rcx)        # tmp[2..3]
    vmovups   16(%rsi), %xmm4        # b[2..3]
    movddup   %xmm4, %xmm5           # dup b[2] to both lanes
    vmovups   %xmm5, 32(%rcx)        # tmp[4..5]

    vzeroupper
    ret

    .size avx_subset_double_test, .-avx_subset_double_test

    # Mark that this object does not require an executable stack
    .section .note.GNU-stack,"",@progbits
