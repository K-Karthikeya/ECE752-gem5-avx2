/**
 * SAXPY Benchmark: Y = a*X + Y (SSE version)
 * 
 * This benchmark implements the classic SAXPY operation (Single-precision A*X Plus Y)
 * using SSE-128 vectorization (no AVX, no FMA).
 * 
 * SAXPY is compute-bound and demonstrates:
 * - 4-wide SIMD (128-bit SSE)
 * - Separate multiply + add (no FMA)
 * 
 * Verification is done against a C reference implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Assembly kernel (SSE only)
extern void saxpy_sse(float *y, const float *x, float a, size_t n);

/**
 * Read timestamp counter for accurate cycle measurement
 */
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

/**
 * Initialize arrays with test data
 */
void init_arrays(float *x, float *y, size_t n) {
    for (size_t i = 0; i < n; i++) {
        x[i] = (float)(i % 100) / 10.0f;  // 0.0 to 9.9
        y[i] = (float)(i % 50) / 5.0f;    // 0.0 to 9.8
    }
}

/**
 * C reference implementation for verification
 */
void saxpy_reference(float *y, const float *x, float a, size_t n) {
    for (size_t i = 0; i < n; i++) {
        y[i] = a * x[i] + y[i];
    }
}

/**
 * Verify SSE results match reference C implementation
 */
int verify_results(const float *y_ref, const float *y_sse, size_t n) {
    const float epsilon = 1e-5f;
    int errors = 0;
    
    for (size_t i = 0; i < n; i++) {
        float diff = y_ref[i] - y_sse[i];
        if (diff < 0) diff = -diff;
        
        if (diff > epsilon) {
            if (errors < 10) {  // Only print first 10 errors
                printf("Mismatch at index %zu: reference=%f sse=%f diff=%f\n",
                       i, y_ref[i], y_sse[i], diff);
            }
            errors++;
        }
    }
    
    return errors;
}

int main(int argc, char **argv) {
    // Array size: default 8192 elements (32KB per array)
    size_t n = 8192;
    if (argc > 1) {
        n = (size_t)atoi(argv[1]);
        // Round up to multiple of 4 for SSE alignment
        n = (n + 3) & ~3;
    }
    
    printf("SAXPY Benchmark (SSE)\n");
    printf("=====================\n");
    printf("Array size: %zu elements (%.2f KB per array)\n", 
           n, (n * sizeof(float)) / 1024.0);
    printf("Operation: Y = %.2f * X + Y\n", 2.5f);
    printf("SIMD width: 128-bit (4 floats)\n\n");
    
    // Allocate aligned memory (16-byte alignment for SSE)
    float *x = (float*)aligned_alloc(16, n * sizeof(float));
    float *y_ref = (float*)aligned_alloc(16, n * sizeof(float));
    float *y_sse = (float*)aligned_alloc(16, n * sizeof(float));
    
    if (!x || !y_ref || !y_sse) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Initialize data
    init_arrays(x, y_ref, n);
    memcpy(y_sse, y_ref, n * sizeof(float));
    
    const float a = 2.5f;
    
    // Run C reference version for verification
    printf("Running C reference version...\n");
    saxpy_reference(y_ref, x, a, n);
    
    // Run SSE version with timing
    printf("Running SSE-128 version...\n");
    uint64_t start = rdtsc();
    saxpy_sse(y_sse, x, a, n);
    uint64_t sse_cycles = rdtsc() - start;
    
    // Verify
    printf("\nVerifying results...\n");
    int errors = verify_results(y_ref, y_sse, n);
    if (errors > 0) {
        printf("FAILED: %d mismatches found\n", errors);
    } else {
        printf("PASSED: Results match\n");
    }
    
    // Performance metrics
    double flops = 2.0 * n;  // n multiplies + n adds
    printf("\nPerformance Results:\n");
    printf("--------------------\n");
    printf("SSE-128 cycles: %llu\n", (unsigned long long)sse_cycles);
    printf("Total FLOPs:    %.0f\n", flops);
    printf("Cycles/FLOP:    %.2f\n", sse_cycles / flops);
    
    // Cleanup
    free(x);
    free(y_ref);
    free(y_sse);
    
    return (errors > 0) ? 1 : 0;
}
