microcode = '''
# Broadcast low 32-bit float to all lanes
def macroop VBROADCASTSS_128_XMM_XMM {
    vbroadcastf dest=xmm0, src1=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VBROADCASTSS_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VBROADCASTSS_128_XMM_P {
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

def macroop VBROADCASTSS_256_YMM_YMM {
    vbroadcastf dest=xmm0, src1=xmmrm, size=4, VL=32
};
def macroop VBROADCASTSS_256_YMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=32
};
def macroop VBROADCASTSS_256_YMM_P {
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vbroadcastf dest=xmm0, src1=ufp0, size=4, VL=32
};
'''

