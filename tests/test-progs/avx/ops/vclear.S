.intel_syntax noprefix
.globl avx_vclear_upper
avx_vclear_upper:
    vzeroupper
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
