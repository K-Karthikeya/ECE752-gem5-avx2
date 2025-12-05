microcode = '''
def macroop VDIVPS_128_XMM_XMM {
    vdivf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VDIVPS_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vdivf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VDIVPS_128_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vdivf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

def macroop VDIVPS_256_YMM_YMM {
    vdivf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VDIVPS_256_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vdivf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VDIVPS_256_YMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vdivf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''
