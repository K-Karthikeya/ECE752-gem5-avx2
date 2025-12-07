//===========================================================================
// AVX Instruction Trace Test Harness
// Pure assembly to avoid any C library initialization
//===========================================================================

extern void avx_trace_test(float *a, float *b, float *out);

// Data in .data section (initialized by loader, no x87 needed)
__attribute__((section(".data"))) 
unsigned int a_int[8] __attribute__((aligned(32))) = {
    0x3f800000, 0x40000000, 0x40400000, 0x40800000,  // 1.0, 2.0, 3.0, 4.0
    0x40a00000, 0x40c00000, 0x40e00000, 0x41000000   // 5.0, 6.0, 7.0, 8.0
};

__attribute__((section(".data")))
unsigned int b_int[8] __attribute__((aligned(32))) = {
    0x41000000, 0x40e00000, 0x40c00000, 0x40a00000,  // 8.0, 7.0, 6.0, 5.0
    0x40800000, 0x40400000, 0x40000000, 0x3f800000   // 4.0, 3.0, 2.0, 1.0
};

__attribute__((section(".bss")))
unsigned int out_int[104] __attribute__((aligned(32)));

int main(void) {
    // Use inline asm to avoid any C compiler-generated code
    float *a;
    float *b;
    float *out;
    
    asm volatile (
        "lea a_int(%%rip), %0\n\t"
        "lea b_int(%%rip), %1\n\t"
        "lea out_int(%%rip), %2\n\t"
        : "=r" (a), "=r" (b), "=r" (out)
        :
        : "memory"
    );

    // Call the test
    avx_trace_test(a, b, out);

    return 0;
}
