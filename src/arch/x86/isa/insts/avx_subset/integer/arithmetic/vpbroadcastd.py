microcode = """
# VPBROADCASTD: broadcast 32-bit integer to all lanes
# Source is a 32-bit value (from reg or memory), replicated to all lanes

# 128-bit width (VEX.L=0); reg-reg form (VPBROADCASTD reg, reg)
def macroop VPBROADCASTD_128_XMM_XMM {
    vbroadcasti dest=xmm0, src1=xmmrm, size=4, VL=16
};

# 128-bit width; reg-mem SIB form (VPBROADCASTD reg, [mem])
def macroop VPBROADCASTD_128_XMM_M {
    ld t1, seg, sib, "DISPLACEMENT", dataSize=4
    mov2fp ufp0, t1, 0, size=4
    vbroadcasti dest=xmm0, src1=ufp0, size=4, VL=16
};

# 128-bit width; reg-mem RIP-relative form
def macroop VPBROADCASTD_128_XMM_P {
    rdip t7
    ld t1, seg, riprel, "DISPLACEMENT", dataSize=4
    mov2fp ufp0, t1, 0, size=4
    vbroadcasti dest=xmm0, src1=ufp0, size=4, VL=16
};

# 256-bit width (VEX.L=1); reg-reg form
def macroop VPBROADCASTD_256_XMM_XMM {
    vbroadcasti dest=xmm0, src1=xmmrm, size=4, VL=32
};

# 256-bit width; reg-mem SIB form
def macroop VPBROADCASTD_256_XMM_M {
    ld t1, seg, sib, "DISPLACEMENT", dataSize=4
    mov2fp ufp0, t1, 0, size=4
    vbroadcasti dest=xmm0, src1=ufp0, size=4, VL=32
};

# 256-bit width; reg-mem RIP-relative form
def macroop VPBROADCASTD_256_XMM_P {
    rdip t7
    ld t1, seg, riprel, "DISPLACEMENT", dataSize=4
    mov2fp ufp0, t1, 0, size=4
    vbroadcasti dest=xmm0, src1=ufp0, size=4, VL=32
};
"""
