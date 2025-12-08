microcode = '''
# VPMINSD: packed 32-bit signed integer minimum
# dest = min(src1, src2) per 32-bit lane

# 128-bit width (VEX.L=0); reg-reg and reg/mem forms
def macroop VPMINSD_128_XMM_XMM {
    vmini dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=16
};
def macroop VPMINSD_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
};
def macroop VPMINSD_128_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
};

# 256-bit width (VEX.L=1); reg-reg and reg/mem forms
def macroop VPMINSD_256_XMM_XMM {
    vmini dest=xmm0, src1=xmm0v, src2=xmmrm, size=4, VL=32
};
def macroop VPMINSD_256_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
def macroop VPMINSD_256_XMM_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};

# VPMINSQ: packed 64-bit signed integer minimum
# dest = min(src1, src2) per 64-bit lane

# 128-bit width (VEX.L=0); reg-reg and reg/mem forms
def macroop VPMINSQ_128_XMM_XMM {
    vmini dest=xmm0, src1=xmm0v, src2=xmmrm, size=8, VL=16
};
def macroop VPMINSQ_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=16
};
def macroop VPMINSQ_128_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=16
};

# 256-bit width (VEX.L=1); reg-reg and reg/mem forms
def macroop VPMINSQ_256_XMM_XMM {
    vmini dest=xmm0, src1=xmm0v, src2=xmmrm, size=8, VL=32
};
def macroop VPMINSQ_256_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=32
};
def macroop VPMINSQ_256_XMM_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vmini dest=xmm0, src1=xmm0v, src2=ufp0, size=8, VL=32
};
'''
