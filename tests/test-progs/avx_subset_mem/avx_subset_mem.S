.intel_syntax noprefix
# AVX subset memory/reg and XMM-focused test
# Exercises: VMOVUPS (load/store), VADDPS (mem/reg forms), VMULPS (mem/reg), VXORPS, VZEROUPPER
# Also covers overlapping dest=src1 and dest=src2 semantics, and 128-bit (XMM) and 256-bit (YMM) paths.
# Prototype: void avx_subset_mem_test(float *a, float *b, float *outY, float *outX, float *tmp);
#   a: base array (at least 32 bytes)
#   b: second array (at least 32 bytes)
#   outY: output 8 floats for YMM path
#   outX: output 4 floats for XMM path
#   tmp: scratch (at least 32 bytes)

.text
.globl avx_subset_mem_test
.type avx_subset_mem_test,@function

avx_subset_mem_test:
    # rdi=a, rsi=b, rdx=outY, rcx=outX, r8=tmp

    # --- 256-bit YMM section: mixed mem/reg and overlap ---
    # Load a into ymm0 (mem -> reg)
    vmovups   ymm0, [rdi]
    # Add from memory: ymm0 = ymm0 + [b]
    vaddps    ymm0, ymm0, [rsi]
    # Multiply into a different dest from register and memory: ymm1 = ymm0 * [a]
    vmulps    ymm1, ymm0, [rdi]
    # Self XOR zeroing on ymm2 using dest=src1=src2 form with reg
    vxorps    ymm2, ymm2, ymm2
    # Store results
    vmovups   [rdx], ymm1
    vmovaps   [r8], ymm2         # write zeros to tmp (aligned store ok if aligned)

    # --- 128-bit XMM section: ensure L=0 path and overlap with dest=src2 ---
    # Load lower 128 from a and b
    vmovups   xmm3, [rdi]
    vmovaps   xmm4, [rsi]
    # Use dest=src2 form: xmm4 = xmm3 + xmm4 (overlap on src2/dest)
    vaddps    xmm4, xmm3, xmm4
    # Multiply using mem as src2 and dest=src1 overlap: xmm3 = xmm3 * [rsi]
    vmulps    xmm3, xmm3, [rsi]
    # Store both XMM results
    vmovups   [rcx], xmm4

    vzeroupper
    ret

.size avx_subset_mem_test, .-avx_subset_mem_test

# Mark non-exec stack
.section .note.GNU-stack,"",@progbits
