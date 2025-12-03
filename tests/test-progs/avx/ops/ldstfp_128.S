.intel_syntax noprefix
.globl avx_ldstfp_128
avx_ldstfp_128:
    ; mem in rdi, out in rsi (but our prototype mem, out uses rdi, rsi). Harness uses mem, out.
    vmovups xmm0, XMMWORD PTR [rdi]
    ; store and reload to exercise store path
    vmovups XMMWORD PTR [rdi], xmm0
    vmovups xmm1, XMMWORD PTR [rdi]
    vmovups XMMWORD PTR [rsi], xmm1
    ret
