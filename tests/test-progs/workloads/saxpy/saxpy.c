/**
 * SAXPY Benchmark: Y = a*X + Y
 *
 * This benchmark implements the classic SAXPY operation (Single-precision A*X Plus Y)
 * using AVX-256 vectorization with FMA instructions.
 *
 * SAXPY is compute-bound and benefits heavily from:
 * - 8-wide SIMD (256-bit AVX)
 * - FMA (Fused Multiply-Add) instructions
 *
 * Verification is done against a C reference implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gem5/m5ops.h>

// Assembly kernel (AVX-256 only)
extern void saxpy_avx256(float *y, const float *x, float a, size_t n);

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
 * Verify AVX results match reference C implementation
 */
int verify_results(const float *y_ref, const float *y_avx, size_t n) {
    const float epsilon = 1e-5f;
    int errors = 0;

    for (size_t i = 0; i < n; i++) {
        float diff = y_ref[i] - y_avx[i];
        if (diff < 0) diff = -diff;

        if (diff > epsilon) {
            if (errors < 10) {  // Only print first 10 errors
                printf("Mismatch at index %zu: reference=%f avx=%f diff=%f\n",
                       i, y_ref[i], y_avx[i], diff);
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
        // Round up to multiple of 8 for AVX alignment
        n = (n + 7) & ~7;
    }

    printf("SAXPY Benchmark\n");
    printf("===============\n");
    printf("Array size: %zu elements (%.2f KB per array)\n",
           n, (n * sizeof(float)) / 1024.0);
    printf("Operation: Y = %.2f * X + Y\n\n", 2.5f);

    // Allocate aligned memory (32-byte alignment for AVX)
    float *x = (float*)aligned_alloc(32, n * sizeof(float));
    float *y_ref = (float*)aligned_alloc(32, n * sizeof(float));
    float *y_avx = (float*)aligned_alloc(32, n * sizeof(float));

    if (!x || !y_ref || !y_avx) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize data
    init_arrays(x, y_ref, n);
    memcpy(y_avx, y_ref, n * sizeof(float));

    const float a = 2.5f;

    // Run C reference version for verification
    printf("Running C reference version...\n");
    saxpy_reference(y_ref, x, a, n);

    // Run AVX version with timing
    printf("Running AVX-256 version...\n");
    uint64_t start = rdtsc();
    m5_dump_reset_stats(0, 0);
    saxpy_avx256(y_avx, x, a, n);
    m5_dump_reset_stats(0, 0);
    uint64_t avx_cycles = rdtsc() - start;

    // Verify correctness
    printf("\nVerifying results...\n");
    int errors = verify_results(y_ref, y_avx, n);
    if (errors > 0) {
        printf("FAILED: %d mismatches found\n", errors);
    } else {
        printf("PASSED: Results match\n");
    }

    // Report performance
    printf("\nPerformance Results:\n");
    printf("--------------------\n");
    printf("AVX-256 cycles: %llu\n", (unsigned long long)avx_cycles);
    printf("Cycles/element: %.2f\n", (double)avx_cycles / n);
    printf("Operations:     %zu FMAs (2 ops each = %zu total ops)\n", n, n * 2);

    // Cleanup
    free(x);
    free(y_ref);
    free(y_avx);

    return (errors > 0) ? 1 : 0;
}
