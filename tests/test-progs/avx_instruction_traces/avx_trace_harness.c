//===========================================================================
// AVX Instruction Trace Test Harness
// Minimal version to avoid unsupported instructions in gem5
//===========================================================================

// External assembly function
extern void avx_trace_test(float *a, float *b, float *out);

// Simple memset replacement
void simple_memset(void *ptr, int value, unsigned long size) {
    unsigned char *p = (unsigned char *)ptr;
    for (unsigned long i = 0; i < size; i++) {
        p[i] = (unsigned char)value;
    }
}

int main() {
    // Aligned input vectors
    float a[8] __attribute__((aligned(32))) = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    float b[8] __attribute__((aligned(32))) = {8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    float out[104] __attribute__((aligned(32)));
    
    simple_memset(out, 0, sizeof(out));

    // Run the assembly test (this is what we're tracing)
    avx_trace_test(a, b, out);

    // Return 0 if test completed (no validation in gem5 to avoid unsupported instructions)
    return 0;
}
