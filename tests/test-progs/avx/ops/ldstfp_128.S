.intel_syntax noprefix
.globl avx_ldstfp_128
avx_ldstfp_128:
    # mem in rdi, out in rsi
    vmovups xmm0, XMMWORD PTR [rdi]
    # store and reload to exercise store path
    vmovups XMMWORD PTR [rdi], xmm0
    vmovups xmm1, XMMWORD PTR [rdi]
    vmovups XMMWORD PTR [rsi], xmm1
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
