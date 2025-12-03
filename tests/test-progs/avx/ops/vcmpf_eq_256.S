.intel_syntax noprefix
.globl avx_vcmpf_eq_256
avx_vcmpf_eq_256:
    # a in rdi, b in rsi, mask_out in rdx
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups ymm1, YMMWORD PTR [rsi]
    vcmpps ymm2, ymm0, ymm1, 0x00   # equal
    vmovups YMMWORD PTR [rdx], ymm2 # store mask lanes (all-ones or zeros)
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
