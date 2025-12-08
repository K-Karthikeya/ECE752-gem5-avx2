.intel_syntax noprefix
.globl avx_vfmadd231f_128
avx_vfmadd231f_128:
    # a in rdi, b in rsi, c in rdx, out in rcx
    # VFMADD231PS: dest = (vvvv * modrm) + dest_input
    # We want: result = (a * b) + c
    # So: xmm0 = c (dest), xmm1 = a (vvvv), xmm2 = b (modrm)
    vmovups xmm0, XMMWORD PTR [rdx]    # xmm0 = c (accumulator)
    vmovups xmm1, XMMWORD PTR [rdi]    # xmm1 = a (vvvv field)
    vmovups xmm2, XMMWORD PTR [rsi]    # xmm2 = b (modrm field)
    # Computes: xmm0 = (xmm1 * xmm2) + xmm0 = (a * b) + c
    vfmadd231ps xmm0, xmm1, xmm2
    vmovups XMMWORD PTR [rcx], xmm0
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
