microcode = '''
def macroop VMOVAPS_XMM_XMM {
    movfp128 reg1=xmm0, reg2=xmm1
    vclear dest=xmm2, destVL=16
};
def macroop VMOVAPS_XMM_M {
    ldfp128 xmm0, seg, sib, disp, dataSize=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVAPS_M_XMM {
    stfp128 xmm0, seg, sib, disp, dataSize=16
};
def macroop VMOVAPS_YMM_YMM {
    movfp256 reg1=xmm0, reg2=xmm1
};
def macroop VMOVAPS_YMM_M {
    ldfp256 xmm0, seg, sib, disp, dataSize=32
};
def macroop VMOVAPS_M_YMM {
    stfp256 xmm0, seg, sib, disp, dataSize=32
};
'''