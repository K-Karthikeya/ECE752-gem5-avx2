microcode = """
# Duplicate low 32-bit float in each 64-bit lane
# 128-bit (VEX.L=0)
def macroop VMOVSLDUP_128_XMM_XMM {
    vmovsldup dest=xmm0, src1=xmmrm, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVSLDUP_128_XMM_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vmovsldup dest=xmm0, src1=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VMOVSLDUP_128_XMM_P {
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vmovsldup dest=xmm0, src1=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};

# 256-bit (VEX.L=1)
def macroop VMOVSLDUP_256_XMM_XMM {
    vmovsldup dest=xmm0, src1=xmmrm, size=4, VL=32
};
def macroop VMOVSLDUP_256_XMM_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    vmovsldup dest=xmm0, src1=ufp0, size=4, VL=32
};
def macroop VMOVSLDUP_256_XMM_P {
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    vmovsldup dest=xmm0, src1=ufp0, size=4, VL=32
};
"""
