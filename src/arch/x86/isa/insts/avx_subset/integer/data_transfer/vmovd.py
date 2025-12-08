microcode = '''
# VMOVD: Move 32-bit integer between GPR and XMM register
# Zero-extends when loading into XMM, extracts lower 32 bits when storing

# Load from GPR to XMM (zeros upper bits)
def macroop VMOVD_XMM_R {
    mov2fp xmml, regm, srcSize=4, destSize=8
    lfpimm xmmh, 0
};

# Load from memory to XMM (zeros upper bits)
def macroop VMOVD_XMM_M {
    ldfp xmml, seg, sib, disp, dataSize=4
    lfpimm xmmh, 0
};

def macroop VMOVD_XMM_P {
    rdip t7
    ldfp xmml, seg, riprel, disp, dataSize=4
    lfpimm xmmh, 0
};

# Store from XMM to GPR (extracts lower 32 bits)
def macroop VMOVD_R_XMM {
    mov2int reg, xmmlm, size=4
};

# Store from XMM to memory (extracts lower 32 bits)
def macroop VMOVD_M_XMM {
    stfp xmml, seg, sib, disp, dataSize=4
};

def macroop VMOVD_P_XMM {
    rdip t7
    stfp xmml, seg, riprel, disp, dataSize=4
};
'''
