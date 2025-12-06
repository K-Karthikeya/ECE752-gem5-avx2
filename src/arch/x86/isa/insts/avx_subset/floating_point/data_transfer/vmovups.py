microcode = '''
def macroop VMOVUPS_128_M {
    ldfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVUPS_128_P {
    rdip t7
    ldfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVUPS_M_128_M {
    stfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_M_128_P {
    rdip t7
    stfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_256_M {
    # Single 256-bit load into the YMM destination starting at xmm0
    # dataSize here is element size (8B); microop fetches 4 lanes internally.
    ldfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_256_P {
    rdip t7
    ldfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_M_256_M {
    # Single 256-bit store starting at xmm0
    # dataSize is element size (8B); microop stores 4 lanes internally.
    stfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_M_256_P {
    rdip t7
    stfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
'''