microcode = '''
# VCMPPS with imm8 predicate (lower 5 bits used by microop).
# Inst format generates multiple mnemonic variants; we define only the fully-qualified ones.

# XMM reg-reg forms
def macroop VCMPPS_128_XMM_XMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};

# XMM memory forms
def macroop VCMPPS_128_XMM_M_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_128_XMM_P_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};

# YMM reg-reg forms (parser appends _XMM_XMM)
def macroop VCMPPS_256_XMM_XMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32, ext="IMMEDIATE"
};

# YMM memory forms
def macroop VCMPPS_256_XMM_M_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32, ext="IMMEDIATE"
};
def macroop VCMPPS_256_XMM_P_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32, ext="IMMEDIATE"
};
'''
