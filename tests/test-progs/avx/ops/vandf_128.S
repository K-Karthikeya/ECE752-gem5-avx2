.intel_syntax noprefix
.globl avx_vandf_128
avx_vandf_128:
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vandps xmm2, xmm0, xmm1
    vmovups XMMWORD PTR [rdx], xmm2
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
