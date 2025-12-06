microcode = '''
# Packed single-precision min
def macroop VMINPS_XMM_XMM {
    vminf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMINPS_XMM_XMM_XMM_XMM {
    vminf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMINPS_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMINPS_XMM_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMINPS_XMM_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMINPS_XMM_XMM_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMINPS_XMM_XMM_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

def macroop VMINPS_YMM_YMM {
    vminf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VMINPS_YMM_YMM_XMM_XMM {
    vminf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VMINPS_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VMINPS_YMM_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VMINPS_YMM_YMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VMINPS_YMM_YMM_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VMINPS_YMM_YMM_XMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vminf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''