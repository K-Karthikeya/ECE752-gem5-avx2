.intel_syntax noprefix
.globl avx_vmulf_128
avx_vmulf_128:
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vmulps xmm2, xmm0, xmm1
    vmovups XMMWORD PTR [rdx], xmm2
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
