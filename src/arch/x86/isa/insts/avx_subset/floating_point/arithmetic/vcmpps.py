microcode = '''
# VCMPPS with imm8 predicate (lower 5 bits used by microop).
# Inst format duplicates operand types in mnemonic, so we need both base and duplicated variants.
def macroop VCMPPS_XMM_XMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_XMM_XMM_XMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_M_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_XMM_M_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_P_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_XMM_P_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16, ext="IMMEDIATE"
    vclear dest=xmm2, destVL=16
};

def macroop VCMPPS_YMM_YMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32, ext="IMMEDIATE"
};
def macroop VCMPPS_YMM_YMM_XMM_XMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32, ext="IMMEDIATE"
};
def macroop VCMPPS_YMM_M_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32, ext="IMMEDIATE"
};
def macroop VCMPPS_YMM_YMM_M_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32, ext="IMMEDIATE"
};
def macroop VCMPPS_YMM_P_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32, ext="IMMEDIATE"
};
def macroop VCMPPS_YMM_YMM_P_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32, ext="IMMEDIATE"
};
'''
