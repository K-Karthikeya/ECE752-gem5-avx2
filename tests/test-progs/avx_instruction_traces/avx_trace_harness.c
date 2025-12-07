//===========================================================================
// AVX Instruction Trace Test Harness
// Minimal version - no float constants to avoid x87 instructions
//===========================================================================

// External assembly function
extern void avx_trace_test(float *a, float *b, float *out);

int main() {
    // Use integer representation of floats to avoid x87 FPU instructions
    // IEEE 754: 1.0f = 0x3f800000, 2.0f = 0x40000000, etc.
    unsigned int a_int[8] __attribute__((aligned(32))) = {
        0x3f800000, 0x40000000, 0x40400000, 0x40800000,  // 1.0, 2.0, 3.0, 4.0
        0x40a00000, 0x40c00000, 0x40e00000, 0x41000000   // 5.0, 6.0, 7.0, 8.0
    };
    unsigned int b_int[8] __attribute__((aligned(32))) = {
        0x41000000, 0x40e00000, 0x40c00000, 0x40a00000,  // 8.0, 7.0, 6.0, 5.0
        0x40800000, 0x40400000, 0x40000000, 0x3f800000   // 4.0, 3.0, 2.0, 1.0
    };
    unsigned int out_int[104] __attribute__((aligned(32)));
    
    // Zero output (avoiding memset which might emit unsupported instructions)
    for (int i = 0; i < 104; i++) {
        out_int[i] = 0;
    }

    // Cast to float* for the assembly function
    float *a = (float *)a_int;
    float *b = (float *)b_int;
    float *out = (float *)out_int;

    // Run the assembly test (this is what we're tracing)
    avx_trace_test(a, b, out);

    // Return 0 if test completed
    return 0;
}
