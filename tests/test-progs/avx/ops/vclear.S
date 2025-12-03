.intel_syntax noprefix
.globl avx_vclear_upper
avx_vclear_upper:
    vzeroupper
    ret
