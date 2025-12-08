microcode = '''
# VMOVDQU: unaligned move of packed dword/qword integers
# Pattern: load directly into xmm0, store directly from xmm0

# 128-bit width (VEX.L=0); load and store forms
def macroop VMOVDQU_128_M {
    ldfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_128_P {
    rdip t7
    ldfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_M_128_M {
    stfp128 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_M_128_P {
    rdip t7
    stfp128 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};

# 256-bit width (VEX.L=1); load and store forms
def macroop VMOVDQU_256_M {
    ldfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_256_P {
    rdip t7
    ldfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_M_256_M {
    stfp256 xmm0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_M_256_P {
    rdip t7
    stfp256 xmm0, seg, riprel, "DISPLACEMENT", dataSize=8
};
'''
