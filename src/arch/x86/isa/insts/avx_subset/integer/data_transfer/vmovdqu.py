microcode = '''
# VMOVDQU: unaligned move of packed dword/qword integers
# Same pattern as VMOVUPS (floating-point unaligned)

# 128-bit width (VEX.L=0); load and store forms
def macroop VMOVDQU_128_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    movfp dest=xmm0, src1=ufp0, size=16, VL=16
};
def macroop VMOVDQU_128_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    movfp dest=xmm0, src1=ufp0, size=16, VL=16
};
def macroop VMOVDQU_M_128_M {
    ldfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    stfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_M_128_P {
    rdip t7
    ldfp128 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    stfp128 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
};

# 256-bit width (VEX.L=1); load and store forms
def macroop VMOVDQU_256_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    movfp dest=xmm0, src1=ufp0, size=32, VL=32
};
def macroop VMOVDQU_256_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    movfp dest=xmm0, src1=ufp0, size=32, VL=32
};
def macroop VMOVDQU_M_256_M {
    ldfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    stfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
};
def macroop VMOVDQU_M_256_P {
    rdip t7
    ldfp256 ufp0, seg, riprel, "DISPLACEMENT", dataSize=8
    stfp256 ufp0, seg, sib, "DISPLACEMENT", dataSize=8
};
'''
