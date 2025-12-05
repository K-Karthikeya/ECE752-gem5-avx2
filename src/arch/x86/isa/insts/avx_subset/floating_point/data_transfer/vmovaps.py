microcode = '''
def macroop VMOVAPS_128_M {
    ldfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVAPS_128_P {
    rdip t7
    ldfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVAPS_M_128_M {
    stfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVAPS_M_128_P {
    rdip t7
    stfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVAPS_256_M {
    ldfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVAPS_256_P {
    rdip t7
    ldfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVAPS_M_256_M {
    stfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVAPS_M_256_P {
    rdip t7
    stfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
'''