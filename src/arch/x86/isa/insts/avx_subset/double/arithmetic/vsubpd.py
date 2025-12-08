microcode = '''
def macroop VSUBPD_128_XMM_XMM {
    vsubf dest=xmm0, src1=xmm0v, src2=xmmrm, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VSUBPD_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vsubf dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VSUBPD_128_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vsubf dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VSUBPD_256_XMM_XMM {
    vsubf dest=xmm0, src1=xmm0v, src2=xmmrm, size=8, VL=32
};
def macroop VSUBPD_256_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vsubf dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=32
};
def macroop VSUBPD_256_XMM_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vsubf dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=32
};
'''