microcode = '''
def macroop VMOVUPS_XMM_M {
    ldfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVUPS_XMM_P {
    rdip t7
    ldfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
    vclear dest=xmm2, destVL=16
};
def macroop VMOVUPS_M_XMM_M {
    stfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_M_XMM_P {
    rdip t7
    stfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_YMM_M {
    # Original 4x64-bit loads (commented out to test ldfp256)
    # ldfp xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    # ldfp xmm1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    # ldfp xmm2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    # ldfp xmm3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    # Single 256-bit load into the YMM destination starting at xmm0
    # dataSize here is element size (8B); microop fetches 4 lanes internally.
    ldfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_YMM_P {
    rdip t7
    ldfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_M_YMM_M {
    # Original 4x64-bit stores (commented out to test stfp256)
    # stfp xmm0, seg, sib, "DISPLACEMENT", dataSize=8
    # stfp xmm1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    # stfp xmm2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    # stfp xmm3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    # Single 256-bit store starting at xmm0
    # dataSize is element size (8B); microop stores 4 lanes internally.
    stfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVUPS_M_YMM_P {
    rdip t7
    stfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
'''