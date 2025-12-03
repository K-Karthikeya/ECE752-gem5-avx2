.intel_syntax noprefix
.globl avx_vcmpf_eq_128
avx_vcmpf_eq_128:
    # a in rdi, b in rsi, mask_out in rdx
    vmovups xmm0, XMMWORD PTR [rdi]
    vmovups xmm1, XMMWORD PTR [rsi]
    vcmpps xmm2, xmm0, xmm1, 0x00   ; equal
    vmovups XMMWORD PTR [rdx], xmm2 ; store mask lanes (all-ones or zeros)
    ret
