.intel_syntax noprefix
.globl avx_vfmadd231f_256
avx_vfmadd231f_256:
    # a in rdi, b in rsi, c in rdx, out in rcx
    # VFMADD231PS: dest = (vvvv * modrm) + dest_input
    # We want: result = (a * b) + c
    # So: ymm0 = c (dest), ymm1 = a (vvvv), ymm2 = b (modrm)
    vmovups ymm0, YMMWORD PTR [rdx]    # ymm0 = c (accumulator)
    vmovups ymm1, YMMWORD PTR [rdi]    # ymm1 = a (vvvv field)
    vmovups ymm2, YMMWORD PTR [rsi]    # ymm2 = b (modrm field)
    # Computes: ymm0 = (ymm1 * ymm2) + ymm0 = (a * b) + c
    vfmadd231ps ymm0, ymm1, ymm2
    vmovups YMMWORD PTR [rcx], ymm0
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
