/**
 * Matrix Multiplication Benchmark
 * 
 * Implements C = A * B for square matrices using AVX-256 vectorization.
 * 
 * This demonstrates:
 * - 8-wide SIMD vectorization in inner loop
 * - FMA benefits for multiply-accumulate
 * - Row-major memory access patterns
 * 
 * Verification is done against a C reference implementation (naive triple-nested loop).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gem5/m5ops.h>

// Assembly kernel (AVX-256 only)
extern void matmul_avx256(float *C, const float *A, const float *B, size_t N);

/**
 * Read timestamp counter
 */
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

/**
 * Initialize matrix with test data
 */
void init_matrix(float *mat, size_t N) {
    for (size_t i = 0; i < N * N; i++) {
        mat[i] = (float)(i % 10) / 10.0f;  // Values from 0.0 to 0.9
    }
}

/**
 * C reference implementation for verification
 * Simple triple-nested loop
 */
void matmul_reference(float *C, const float *A, const float *B, size_t N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            float sum = 0.0f;
            for (size_t k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

/**
 * Verify results match within tolerance
 */
int verify_matrices(const float *C_ref, const float *C_avx, size_t N) {
    const float epsilon = 1e-3f;  // Larger tolerance due to accumulated FP error
    int errors = 0;
    
    for (size_t i = 0; i < N * N; i++) {
        float diff = C_ref[i] - C_avx[i];
        if (diff < 0) diff = -diff;
        
        if (diff > epsilon) {
            if (errors < 10) {
                printf("Mismatch at index %zu: scalar=%f avx=%f diff=%f\n",
                       i, C_ref[i], C_avx[i], diff);
            }
            errors++;
        }
    }
    
    return errors;
}

int main(int argc, char **argv) {
    // Matrix size: default 128x128 (small enough for gem5 but meaningful)
    size_t N = 128;
    if (argc > 1) {
        N = (size_t)atoi(argv[1]);
        // Round up to multiple of 8 for AVX
        N = (N + 7) & ~7;
    }
    
    printf("Matrix Multiplication Benchmark\n");
    printf("================================\n");
    printf("Matrix size: %zux%zu (%.2f MB per matrix)\n",
           N, N, (N * N * sizeof(float)) / (1024.0 * 1024.0));
    printf("Operation: C = A * B\n\n");
    
    // Allocate aligned memory
    float *A = (float*)aligned_alloc(32, N * N * sizeof(float));
    float *B = (float*)aligned_alloc(32, N * N * sizeof(float));
    float *C_ref = (float*)aligned_alloc(32, N * N * sizeof(float));
    float *C_avx = (float*)aligned_alloc(32, N * N * sizeof(float));
    
    if (!A || !B || !C_ref || !C_avx) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Initialize matrices
    init_matrix(A, N);
    init_matrix(B, N);
    memset(C_ref, 0, N * N * sizeof(float));
    memset(C_avx, 0, N * N * sizeof(float));
    
    // Run C reference version for verification
    printf("Running C reference version...\n");
    matmul_reference(C_ref, A, B, N);
    
    // Run AVX version with timing
    printf("Running AVX-256 version...\n");
    uint64_t start = rdtsc();
    m5_dump_reset_stats(0, 0);
    matmul_avx256(C_avx, A, B, N);
    m5_dump_reset_stats(0, 0);
    uint64_t avx_cycles = rdtsc() - start;
    
    // Verify
    printf("\nVerifying results...\n");
    int errors = verify_matrices(C_ref, C_avx, N);
    if (errors > 0) {
        printf("FAILED: %d mismatches found\n", errors);
    } else {
        printf("PASSED: Results match\n");
    }
    
    // Performance metrics
    double flops = 2.0 * N * N * N;  // N^3 multiplies + N^3 adds
    printf("\nPerformance Results:\n");
    printf("--------------------\n");
    printf("AVX-256 cycles: %llu\n", (unsigned long long)avx_cycles);
    printf("Total FLOPs:    %.0f\n", flops);
    printf("Cycles/FLOP:    %.2f\n", avx_cycles / flops);
    
    // Cleanup
    free(A);
    free(B);
    free(C_ref);
    free(C_avx);
    
    return (errors > 0) ? 1 : 0;
}
