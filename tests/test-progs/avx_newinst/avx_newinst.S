# AVX new instructions test: VFMADD231PS, VSUBPS, VANDPS, VDIVPS, VCMPPS
# Inputs: a, b, c, output buffers for each instruction
# Linux System V AMD64 calling convention
# void avx_newinst_test(float *a, float *b, float *c, float *out_fma, float *out_sub, float *out_and, float *out_div, float *out_cmp)
    .text
    .globl avx_newinst_test
    .type avx_newinst_test,@function
avx_newinst_test:
    # VFMADD231PS: out_fma[i] = a[i]*b[i] + c[i]
    vmovaps   (%rdi), %ymm0      # a
    vmovaps   (%rsi), %ymm1      # b
    vmovaps   (%rdx), %ymm2      # c
    vfmadd231ps %ymm1, %ymm2, %ymm0  # ymm0 = ymm1*ymm0 + ymm2 = a*b + c
    vmovaps   %ymm0, (%r9)       # out_fma
    # VSUBPS: out_sub[i] = a[i] - b[i]
    vmovaps   (%rdi), %ymm3
    vmovaps   (%rsi), %ymm4
    vsubps    %ymm4, %ymm3, %ymm5
    vmovaps   %ymm5, (%r10)      # out_sub
    # VANDPS: out_and[i] = a[i] & b[i] (bitwise)
    vmovaps   (%rdi), %ymm6
    vmovaps   (%rsi), %ymm7
    vandps    %ymm7, %ymm6, %ymm8
    vmovaps   %ymm8, (%r11)      # out_and
    # VDIVPS: out_div[i] = a[i] / b[i]
    vmovaps   (%rdi), %ymm9
    vmovaps   (%rsi), %ymm10
    vdivps    %ymm10, %ymm9, %ymm11
    vmovaps   %ymm11, (%r12)     # out_div
    # VCMPPS: out_cmp[i] = (a[i] < b[i]) ? 1.0f : 0.0f (predicate 1)
    vmovaps   (%rdi), %ymm13
    vmovaps   (%rsi), %ymm14
    vcmpps    $1, %ymm14, %ymm13, %ymm15  # 1 = LT
    vmovaps   %ymm15, (%r13)     # out_cmp
    vzeroupper
    ret
