.intel_syntax noprefix
.globl avx_vxorf_256
avx_vxorf_256:
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups ymm1, YMMWORD PTR [rsi]
    vxorps ymm2, ymm0, ymm1
    vmovups YMMWORD PTR [rdx], ymm2
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
