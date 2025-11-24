microcode = '''
def macroop VXORPS_XMM_XMM {
    vxorf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_XMM_XMM_XMM_XMM {
    vxorf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_XMM_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_XMM_XMM_P {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_XMM_XMM_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_XMM_XMM_XMM_P {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VXORPS_YMM_YMM {
    vxorf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VXORPS_YMM_YMM_XMM_XMM {
    vxorf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VXORPS_YMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp ufp2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp ufp3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VXORPS_YMM_YMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp ufp2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp ufp3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VXORPS_YMM_YMM_P {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp ufp2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp ufp3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VXORPS_YMM_YMM_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp ufp2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp ufp3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VXORPS_YMM_YMM_XMM_P {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp ufp2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp ufp3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    vxorf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''