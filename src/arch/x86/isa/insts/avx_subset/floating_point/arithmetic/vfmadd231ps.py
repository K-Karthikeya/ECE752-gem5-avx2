microcode = '''
# FMA 231: dest = (src2 * src1) + dest. Use fused microop vfmadd231f.

# 128-bit width (VEX.L=0); reg-reg and reg/mem forms
def macroop VFMADD231PS_128_XMM_XMM {
    vfmadd231f dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=16
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VFMADD231PS_128_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=16
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

# 256-bit width (VEX.L=1); reg-reg and reg/mem forms
def macroop VFMADD231PS_256_XMM_XMM {
    vfmadd231f dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
    vclear dest=xmm4, destVL=32
};
def macroop VFMADD231PS_256_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=32
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
    vclear dest=xmm4, destVL=32
};
def macroop VFMADD231PS_256_XMM_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=32
    vfmadd231f dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
    vclear dest=xmm4, destVL=32
};
'''
