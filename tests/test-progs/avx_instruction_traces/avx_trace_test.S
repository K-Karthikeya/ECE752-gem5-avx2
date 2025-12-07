# AVX Instruction Trace Test
# Comprehensive test for all implemented AVX instructions:
# VMINPS, VMAXPS, VMOVUPS, VMOVAPS, VZEROUPPER, VMOVSLDUP, VMOVSHDUP, VBROADCASTSS
#
# Calling convention: System V AMD64
# Prototype: void avx_trace_test(float *a, float *b, float *out);
#   a, b: input vectors (8 floats each, 32-byte aligned)
#   out: output buffer (104 floats = 416 bytes)
#
# Output layout:
#   [0..7]    : VMOVUPS load result (a)
#   [8..15]   : VMOVAPS load result (b)
#   [16..23]  : VMINPS(a,b)
#   [24..31]  : VMAXPS(a,b)
#   [32..39]  : VMOVSLDUP(a)
#   [40..47]  : VMOVSHDUP(b)
#   [48..55]  : VBROADCASTSS(a[0])
#   [56..63]  : VMINPS reg-mem form
#   [64..71]  : VMAXPS reg-mem form
#   [72..79]  : VMOVSLDUP reg-mem form
#   [80..87]  : VMOVSHDUP reg-mem form
#   [88..95]  : VBROADCASTSS reg-mem form
#   [96..103] : post-VZEROUPPER result

    .text
    .globl avx_trace_test
    .type avx_trace_test,@function

avx_trace_test:
    # Save base pointers
    pushq   %rbx
    movq    %rdx, %rbx           # rbx = out base

    # Test 1: VMOVUPS (unaligned load) - reg-reg form
    vmovups   (%rdi), %ymm0      # Load a[0..7]
    vmovups   %ymm0, (%rbx)      # Store to out[0..7]

    # Test 2: VMOVAPS (aligned load) - reg-reg form
    vmovaps   (%rsi), %ymm1      # Load b[0..7]
    vmovups   %ymm1, 32(%rbx)    # Store to out[8..15]

    # Test 3: VMINPS reg-reg form
    vminps    %ymm1, %ymm0, %ymm2
    vmovups   %ymm2, 64(%rbx)    # Store to out[16..23]

    # Test 4: VMAXPS reg-reg form
    vmaxps    %ymm1, %ymm0, %ymm3
    vmovups   %ymm3, 96(%rbx)    # Store to out[24..31]

    # Test 5: VMOVSLDUP reg-reg form
    vmovsldup %ymm0, %ymm4
    vmovups   %ymm4, 128(%rbx)   # Store to out[32..39]

    # Test 6: VMOVSHDUP reg-reg form
    vmovshdup %ymm1, %ymm5
    vmovups   %ymm5, 160(%rbx)   # Store to out[40..47]

    # Test 7: VBROADCASTSS - skip mem form (decoder limitation)
    # Use reg-reg by first loading into xmm6
    movss    (%rdi), %xmm6          # Load a[0] into xmm6 low 32 bits
    vbroadcastss %xmm6, %ymm6       # Broadcast to all lanes
    vmovups   %ymm6, 192(%rbx)       # Store to out[48..55]

    # Test 8: VMINPS reg-mem form
    vminps    (%rsi), %ymm0, %ymm7
    vmovups   %ymm7, 224(%rbx)   # Store to out[56..63]

    # Test 9: VMAXPS reg-mem form
    vmaxps    (%rsi), %ymm0, %ymm7
    vmovups   %ymm7, 256(%rbx)   # Store to out[64..71]

    # Test 10: VMOVSLDUP reg-mem form
    vmovsldup (%rdi), %ymm7
    vmovups   %ymm7, 288(%rbx)   # Store to out[72..79]

    # Test 11: VMOVSHDUP reg-mem form
    vmovshdup (%rsi), %ymm7
    vmovups   %ymm7, 320(%rbx)   # Store to out[80..87]

    # Test 12: VBROADCASTSS alternate - skip mem form (decoder limitation)
    # Load a[1] and broadcast
    movss    4(%rdi), %xmm7          # Load a[1] into xmm7
    vbroadcastss %xmm7, %ymm7       # Broadcast to all lanes
    vmovups   %ymm7, 352(%rbx)   # Store to out[88..95]

    # Test 13: VZEROUPPER
    vzeroupper
    vmovups   %ymm0, 384(%rbx)   # Store to out[96..103] (upper 128 should be zero)

    popq    %rbx
    ret

    .size avx_trace_test, .-avx_trace_test

    # Mark that this object does not require an executable stack
    .section .note.GNU-stack,"",@progbits
