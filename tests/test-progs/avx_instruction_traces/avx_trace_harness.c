//===========================================================================
// AVX Instruction Trace Test - just the function definition
// Entry point is in avx_trace_start.S (bypasses libc)
//===========================================================================

extern void avx_trace_test(float *a, float *b, float *out);
