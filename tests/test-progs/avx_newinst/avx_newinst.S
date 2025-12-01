# AVX new instructions test: VFMADD231PS, VSUBPS, VANDPS, VDIVPS, VCMPPS
# Inputs: a, b, c, output buffers for each instruction
# Linux System V AMD64 calling convention
# void avx_newinst_test(float *a, float *b, float *c, float *out_fma, float *out_sub, float *out_and, float *out_div, float *out_cmp)
    .text
    .globl avx_newinst_test
    .type avx_newinst_test,@function
avx_newinst_test:
    # SysV AMD64 arg regs: rdi=a, rsi=b, rdx=c, rcx=out_fma, r8=out_sub, r9=out_and
    # Establish a stable frame and read 7th/8th args (out_div, out_cmp) from stack.
    pushq   %rbp
    movq    %rsp, %rbp
    movq    16(%rbp), %r10      # r10 = out_div (7th arg)
    movq    24(%rbp), %r11      # r11 = out_cmp (8th arg)
    # VFMADD231PS: out_fma[i] = (a[i] * b[i]) + c[i]
    vmovaps   (%rdi), %ymm0      # a
    vmovaps   (%rsi), %ymm1      # b
    vmovaps   (%rdx), %ymm2      # c
    # Semantics (231): dest = (dest * src2) + src1
    # GAS AT&T encoding maps first src to r/m; to force register form, put src2 first.
    # We want (a * b) + c → src2=b (ymm1), src1=c (ymm2)
    vfmadd231ps %ymm1, %ymm2, %ymm0  # ymm0 = (a * b) + c
    vmovaps   %ymm0, (%rcx)      # out_fma
    # VSUBPS: out_sub[i] = a[i] - b[i]
    vmovaps   (%rdi), %ymm3
    vmovaps   (%rsi), %ymm4
    vsubps    %ymm4, %ymm3, %ymm5
    vmovaps   %ymm5, (%r8)       # out_sub
    # VANDPS: out_and[i] = a[i] & b[i] (bitwise)
    vmovaps   (%rdi), %ymm6
    vmovaps   (%rsi), %ymm7
    vandps    %ymm7, %ymm6, %ymm8
    vmovaps   %ymm8, (%r9)       # out_and
    # VDIVPS: out_div[i] = a[i] / b[i]
    vmovaps   (%rdi), %ymm9
    vmovaps   (%rsi), %ymm10
    vdivps    %ymm10, %ymm9, %ymm11
    vmovaps   %ymm11, (%r10)     # out_div
    # VCMPPS: out_cmp[i] = (a[i] < b[i]) ? 1.0f : 0.0f (predicate 1)
    vmovaps   (%rdi), %ymm13
    vmovaps   (%rsi), %ymm14
    vcmpps    $1, %ymm14, %ymm13, %ymm15  # 1 = LT
    vmovaps   %ymm15, (%r11)     # out_cmp
    vzeroupper
    popq    %rbp
    ret

    .section .note.GNU-stack,"",@progbits
