microcode = '''
def macroop VADDPS_XMM_XMM {
    # Use base subregister (sub=0) for all operands; vclear clears upper 128.
    vaddf dest=xmm0, src1=xmm0v, src2=xmm0m, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VADDPS_XMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    vaddf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=16
    vclear dest=xmm2, destVL=16
};
def macroop VADDPS_YMM_YMM {
    vaddf dest=xmm0, src1=xmm0v, src2=xmm0m, size=4, VL=32
};
def macroop VADDPS_YMM_M {
    ldfp ufp0, seg, sib, "DISPLACEMENT", dataSize=8
    ldfp ufp1, seg, sib, "DISPLACEMENT + 8", dataSize=8
    ldfp ufp2, seg, sib, "DISPLACEMENT + 16", dataSize=8
    ldfp ufp3, seg, sib, "DISPLACEMENT + 24", dataSize=8
    vaddf dest=xmm0, src1=xmm0v, src2=ufp0, size=4, VL=32
};
'''