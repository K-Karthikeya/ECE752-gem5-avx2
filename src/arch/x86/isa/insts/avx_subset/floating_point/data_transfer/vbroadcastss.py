microcode = """
# Broadcast low 32-bit float to all lanes
def macroop VBROADCASTSS_128_XMM_XMM {
    vbroadcastf dest=xmm0, src1=xmmrm, size=4, VL=16
};
def macroop VBROADCASTSS_128_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=4
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=16
};
def macroop VBROADCASTSS_128_XMM_P {
    rdip t7
    ldfp ufp0, seg, riprel, "DISPLACEMENT", dataSize=4
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=16
};

def macroop VBROADCASTSS_256_XMM_XMM {
    vbroadcastf dest=xmm0, src1=xmmrm, size=4, VL=32
};
def macroop VBROADCASTSS_256_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=4
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=32
};
def macroop VBROADCASTSS_256_XMM_P {
    rdip t7
    ldfp ufp0, seg, riprel, "DISPLACEMENT", dataSize=4
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=32
};
"""
