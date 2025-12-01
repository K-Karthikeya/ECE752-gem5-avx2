microcode = '''
# VCMPPS (EQ predicate only placeholder). Immediate ignored currently.
def macroop VCMPPS_XMM_XMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_M_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VCMPPS_XMM_P_I {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

def macroop VCMPPS_YMM_YMM_I {
    vcmpf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VCMPPS_YMM_M_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VCMPPS_YMM_P_I {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vcmpf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''
