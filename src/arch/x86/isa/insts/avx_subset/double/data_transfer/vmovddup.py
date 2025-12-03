microcode = '''
def macroop VMOVDDUP_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vdupf dest=xmm0, src=ufp0, lane=0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVDDUP_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vdupf dest=xmm0, src=ufp0, lane=0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVDDUP_XMM_XMM {
    vdupf dest=xmm0, src=xmmrm, lane=0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVDDUP_XMM_XMM_XMM_XMM {
    vdupf dest=xmm0, src=xmmrm, lane=0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVDDUP_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vdupf_half dest=xmm0, src=ufp0, size=8, VL=32
};
def macroop VMOVDDUP_YMM_YMM {
    vdupf_half dest=xmm0, src=xmmrm, size=8, VL=32
};
def macroop VMOVDDUP_YMM_YMM_XMM_XMM {
    vdupf_half dest=xmm0, src=xmmrm, size=8, VL=32
};

'''