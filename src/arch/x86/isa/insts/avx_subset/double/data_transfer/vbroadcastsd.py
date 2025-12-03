microcode = '''
def macroop VBROADCASTSD_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VBROADCASTSD_XMM_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VBROADCASTSD_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=32
};
def macroop VBROADCASTSD_YMM_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=32
};
'''