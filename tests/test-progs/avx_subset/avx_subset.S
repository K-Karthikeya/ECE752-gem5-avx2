# AVX subset explicit assembly test
# Exercises: VMOVUPS/VMOVAPS (load/store), VADDPS, VMULPS, VXORPS, VMINPS, VMAXPS,
# VMOVSLDUP, VMOVSHDUP, VBROADCASTSS, VZEROUPPER.
#
# Calling convention: System V AMD64.
# Prototype: void avx_subset_test(float *a, float *b, float *out_base);
#   a, b: input vectors (8 floats each, 32-byte aligned)
#   out_base: points to a contiguous result area of 56 floats laid out as:
#     0  .. 7   : product  ( (a+b)*a )
#     8  .. 15  : zeros from vxorps
#     16 .. 23  : vminps(a,b)
#     24 .. 31  : vmaxps(a,b)
#     32 .. 39  : vmovsldup(a)
#     40 .. 47  : vmovshdup(b)
#     48 .. 55  : vbroadcastss(a[0])
# All stores use VMOVUPS to avoid alignment traps.

    .text
    .globl avx_subset_test
    .type avx_subset_test,@function

avx_subset_test:
    # Load inputs
    vmovups   (%rdi), %ymm0          # ymm0 = a[0..7]
    vmovaps   (%rsi), %ymm1          # ymm1 = b[0..7] (aligned load)

    # (a + b)
    vaddps    %ymm1, %ymm0, %ymm2    # ymm2 = a + b

    # (a + b) * a
    vmulps    %ymm0, %ymm2, %ymm3    # ymm3 = (a+b)*a
    vmovups   %ymm3, (%rdx)          # store product at out_base[0]

    # Zero vector via xor
    vxorps    %ymm3, %ymm3, %ymm4    # ymm4 = 0
    vmovups   %ymm4, 32(%rdx)        # out_base[8]

    # Min/Max
    vminps    %ymm1, %ymm0, %ymm5    # min(a,b)
    vmovups   %ymm5, 64(%rdx)        # out_base[16]

    vmaxps    %ymm1, %ymm0, %ymm6    # max(a,b)
    vmovups   %ymm6, 96(%rdx)        # out_base[24]

    # Duplicate low/high lanes
    vmovsldup %ymm0, %ymm7           # dup low elements of a
    vmovups   %ymm7, 128(%rdx)       # out_base[32]

    vmovshdup %ymm1, %ymm7           # dup high elements of b
    vmovups   %ymm7, 160(%rdx)       # out_base[40]

    # Broadcast a[0] to all lanes
    vbroadcastss (%rdi), %ymm7
    vmovups   %ymm7, 192(%rdx)       # out_base[48]

    vzeroupper
    ret

    .size avx_subset_test, .-avx_subset_test

    # Mark that this object does not require an executable stack
    .section .note.GNU-stack,"",@progbits
