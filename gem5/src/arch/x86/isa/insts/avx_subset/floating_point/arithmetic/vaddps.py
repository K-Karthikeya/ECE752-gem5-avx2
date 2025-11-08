microcode = '''
def macroop VADDPS_XMM_XMM {
    vaddf dest=xmm0, src1=xmm1, src2=xmm2, size=4, VL=16
    vclear dest="InstRegIndex(FLOATREG_XMM_IDX(xmm0, 2))", destVL=16
};
def macroop VADDPS_XMM_M {
    ldfp128 ufp1, seg, sib, disp, dataSize=16
    vaddf dest=xmm0, src1=xmm1, src2=ufp1, size=4, VL=16
    vclear dest="InstRegIndex(FLOATREG_XMM_IDX(xmm0, 2))", destVL=16
};
def macroop VADDPS_YMM_YMM {
    vaddf dest=xmm0, src1=xmm1, src2=xmm2, size=4, VL=32
};
def macroop VADDPS_YMM_M {
    ldfp256 ufp1, seg, sib, disp, dataSize=32
    vaddf dest=xmm0, src1=xmm1, src2=ufp1, size=4, VL=32
};
'''