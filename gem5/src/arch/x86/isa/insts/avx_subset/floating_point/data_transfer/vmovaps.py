microcode = '''
def macroop VMOVAPS_XMM_M {
    ldfp xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp xmm1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVAPS_XMM_P {
    rdip t7
    ldfp xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    ldfp xmm1, seg, riprel, "DISPLACEMENT + 8", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVAPS_M_XMM_M {
    stfp xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    stfp xmm1, seg, sib, "DISPLACEMENT + 8", dataSize=8
};
def macroop VMOVAPS_M_XMM_P {
    rdip t7
    stfp xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    stfp xmm1, seg, riprel, "DISPLACEMENT + 8", dataSize=8
};
def macroop VMOVAPS_YMM_M {
    ldfp xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp xmm1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp xmm2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp xmm3, seg, sib, "DISPLACEMENT + 24", dataSize=8
};
def macroop VMOVAPS_YMM_P {
    rdip t7
    ldfp xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    ldfp xmm1, seg, riprel, "DISPLACEMENT + 8", dataSize=8
    ldfp xmm2, seg, riprel, "DISPLACEMENT + 16", dataSize=8
    ldfp xmm3, seg, riprel, "DISPLACEMENT + 24", dataSize=8
};
def macroop VMOVAPS_M_YMM_M {
    stfp xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    stfp xmm1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    stfp xmm2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    stfp xmm3, seg, sib, "DISPLACEMENT + 24", dataSize=8
};
def macroop VMOVAPS_M_YMM_P {
    rdip t7
    stfp xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    stfp xmm1, seg, riprel, "DISPLACEMENT + 8", dataSize=8
    stfp xmm2, seg, riprel, "DISPLACEMENT + 16", dataSize=8
    stfp xmm3, seg, riprel, "DISPLACEMENT + 24", dataSize=8
};
'''