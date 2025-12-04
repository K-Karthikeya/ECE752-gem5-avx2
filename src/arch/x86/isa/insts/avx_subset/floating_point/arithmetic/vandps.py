microcode = '''
def macroop VANDPS_XMM_XMM {
    vandf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VANDPS_XMM_XMM_XMM_XMM {
    vandf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VANDPS_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VANDPS_XMM_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VANDPS_XMM_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VANDPS_XMM_XMM_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VANDPS_XMM_XMM_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

def macroop VANDPS_YMM_YMM {
    vandf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VANDPS_YMM_YMM_XMM_XMM {
    vandf dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VANDPS_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VANDPS_YMM_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VANDPS_YMM_YMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VANDPS_YMM_YMM_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VANDPS_YMM_YMM_XMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''
