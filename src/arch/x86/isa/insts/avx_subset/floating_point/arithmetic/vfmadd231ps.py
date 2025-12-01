microcode = '''
# FMA 231 form: dest = (src2 * src1) + dest (accumulator). Implemented via fused microop vfmadd231f.
def macroop VFMADD231PS_XMM_XMM {
    vfmadd231f dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_XMM_XMM_XMM_XMM {
    vfmadd231f dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_XMM_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_XMM_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_XMM_XMM_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_XMM_XMM_XMM_P {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

def macroop VFMADD231PS_YMM_YMM {
    limm t0, 0
    limm t1, 0
    limm t2, 0
    # Debug: mark register variant selection
    vclear dest=xmm2, destVL=0
    vfmadd231f dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VFMADD231PS_YMM_YMM_XMM_XMM {
    vfmadd231f dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VFMADD231PS_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    # Debug: mark memory variant selection
    vclear dest=xmm2, destVL=0
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VFMADD231PS_YMM_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VFMADD231PS_YMM_YMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VFMADD231PS_YMM_YMM_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VFMADD231PS_YMM_YMM_XMM_P {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''
