microcode = """
def macroop VBROADCASTSD_128_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=16
};
def macroop VBROADCASTSD_128_XMM_P {
    rdip t7
    ldfp ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=16
};
def macroop VBROADCASTSD_256_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=32
};
def macroop VBROADCASTSD_256_XMM_P {
    rdip t7
    ldfp ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vbcastf dest=xmm0, src=ufp0, size=8, VL=32
};
"""
