.intel_syntax noprefix
.globl avx_vfmadd231f_128
avx_vfmadd231f_128:
    # a in rdi, b in rsi, c in rdx, out in rcx
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vmovups xmm2, XMMWORD PTR [rdx]
    # For 231 form: dest = dest*src3 + src2. Choose src3=b, src2=c.
    vfmadd231ps xmm0, xmm1, xmm2
    vmovups XMMWORD PTR [rcx], xmm0
    ret

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
