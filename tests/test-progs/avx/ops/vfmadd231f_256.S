.intel_syntax noprefix
.globl avx_vfmadd231f_256
avx_vfmadd231f_256:
    # a in rdi, b in rsi, c in rdx, out in rcx
    vmovups ymm0, YMMWORD PTR [rdi]
    vmovups ymm1, YMMWORD PTR [rsi]
    vmovups ymm2, YMMWORD PTR [rdx]
    # We want out = a*b + c. For 231 form: dest = dest*src3 + src2.
    # So use src3=b (ymm1) and src2=c (ymm2):
    vfmadd231ps ymm0, ymm2, ymm1
    vmovups YMMWORD PTR [rcx], ymm0
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
