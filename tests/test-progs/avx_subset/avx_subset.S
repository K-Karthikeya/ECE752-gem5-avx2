# AVX subset explicit assembly test
# Exercises: VMOVUPS (load/store), VMOVAPS (aligned load/store), VADDPS, VMULPS, VXORPS, VZEROUPPER
# Uses multiple addressing forms (no displacement, displacement, RIP-relative) to trigger size variations.
# Result verification strategy (described in README to be added):
#   - Stores computed vectors to a result buffer; main harness checks values.
#   - vxorps self-zero test writes all zeros.
#
# Linux System V AMD64 calling convention assumed.
# Function prototype: void avx_subset_test(float *a, float *b, float *out, float *tmp, float *riprel_target);
#   a: base array (at least 32 bytes, maybe aligned or not)
#   b: second array (aligned to 32 for vmovaps)
#   out: output buffer (at least 32 bytes)
#   tmp: scratch (at least 32 bytes)
#   riprel_target: symbol region for RIP-relative loads
#
# Registers on entry:
#   rdi=a, rsi=b, rdx=out, rcx=tmp, r8=riprel_target
# r9, r10, r11 are caller-saved and free to use.
#
# Postconditions:
#   out[0..7] = (a[i] + b[i]) * a[i]  (vmulps(vaddps(a,b), a))
#   tmp[0..7] = 0.0f (vxorps zero result)
#   riprel_target[0..7] unchanged (only read)
#   Upper ymm registers cleared by vzeroupper before return.
#
# To assemble: gcc -c avx_subset.S -o avx_subset.o -mavx
# To link a harness: gcc harness.c avx_subset.o -o avx_subset_test -mavx

    .text
    .globl avx_subset_test
    .type avx_subset_test,@function

avx_subset_test:
    # Preserve caller-saved we will use (none heavily; keep minimal)

    # 1. VMOVUPS load from 'a' (no displacement)
    vmovups   (%rdi), %ymm0          # ymm0 = a[0..7]

    # 2. VMOVAPS aligned load from 'b' (ensure harness aligns 'b')
    vmovaps   (%rsi), %ymm1          # ymm1 = b[0..7]

    # 3. VADDPS reg-reg (ymm0 + ymm1) -> ymm2
    vaddps    %ymm1, %ymm0, %ymm2    # ymm2 = a + b

    # 4. VMULPS (ymm2 * ymm0) -> ymm3
    vmulps    %ymm0, %ymm2, %ymm3    # ymm3 = (a+b)*a

    # 5. VXORPS to zero (ymm4 = ymm3 ^ ymm3) then store zeros to tmp
    vxorps    %ymm3, %ymm3, %ymm4    # ymm4 = all zero

    # 6. VMOVUPS store result vector (ymm3) to out
    vmovups   %ymm3, (%rdx)          # store computed result

    # 7. VMOVAPS store zero vector (ymm4) to tmp (aligned store if tmp aligned)
    vmovaps   %ymm4, (%rcx)

    # 8. RIP-relative load (trigger _P variant) from riprel_target
    # Place label after a fixed NOP sled to ensure RIP distance is small.
    lea       rip_source(%rip), %r9
    vmovups   (%r9), %xmm0           # 128-bit load (tests VEX.L=0 path implicitly)

    # 9. VZEROUPPER to clear upper lanes (simulated state hygiene)
    vzeroupper

    ret

    .size avx_subset_test, .-avx_subset_test

    .p2align 4
rip_source:
    # 32 bytes of data for RIP-relative load test (8 floats)
    .long 0x3f800000  # 1.0f
    .long 0x40000000  # 2.0f
    .long 0x40400000  # 3.0f
    .long 0x40800000  # 4.0f
    .long 0x40a00000  # 5.0f
    .long 0x40c00000  # 6.0f
    .long 0x40e00000  # 7.0f
    .long 0x41000000  # 8.0f

    # Mark that this object does not require an executable stack
    .section .note.GNU-stack,"",@progbits
