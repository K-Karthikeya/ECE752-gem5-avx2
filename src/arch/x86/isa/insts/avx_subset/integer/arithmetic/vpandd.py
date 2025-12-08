microcode = '''
# VPANDD: packed 32-bit bitwise AND
# dest = src1 & src2 (per 32-bit lane)

# 128-bit width (VEX.L=0); reg-reg and reg/mem forms
def macroop VPANDD_128_XMM_XMM {
    vandi dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
};
def macroop VPANDD_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandi dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
};
def macroop VPANDD_128_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vandi dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
};

# 256-bit width (VEX.L=1); reg-reg and reg/mem forms
def macroop VPANDD_256_XMM_XMM {
    vandi dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VPANDD_256_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vandi dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VPANDD_256_XMM_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vandi dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''
