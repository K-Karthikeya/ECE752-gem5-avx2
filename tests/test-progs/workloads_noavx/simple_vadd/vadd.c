#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <gem5/m5ops.h>

// External assembly function (SSE version)
extern void vadd_sse(float* z, const float* x, const float* y, size_t n);

// Reference C implementation
void vadd_reference(float* z, const float* x, const float* y, size_t n) {
    for (size_t i = 0; i < n; i++) {
        z[i] = x[i] + y[i];
    }
}

int main(int argc, char* argv[]) {
    size_t n = 64;  // Default size

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    // Ensure n is multiple of 4 for SSE (processes 4 floats at a time)
    n = (n + 3) & ~3;

    printf("Simple Vector Addition Benchmark (SSE)\n");
    printf("======================================\n");
    printf("Array size: %zu elements (%.2f KB per array)\n",
           n, (n * sizeof(float)) / 1024.0);
    printf("Operation: Z = X + Y\n");
    printf("SIMD width: 128-bit (4 floats)\n\n");

    // Allocate aligned arrays (16-byte alignment for SSE)
    float* x = (float*)aligned_alloc(16, n * sizeof(float));
    float* y = (float*)aligned_alloc(16, n * sizeof(float));
    float* z_ref = (float*)aligned_alloc(16, n * sizeof(float));
    float* z_sse = (float*)aligned_alloc(16, n * sizeof(float));

    if (!x || !y || !z_ref || !z_sse) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize input arrays with simple values
    for (size_t i = 0; i < n; i++) {
        x[i] = (float)(i + 1);
        y[i] = (float)(i * 2);
    }

    // Run reference version
    printf("Running C reference version...\n");
    vadd_reference(z_ref, x, y, n);

    // Run SSE version
    printf("Running SSE-128 version...\n");
    m5_dump_reset_stats(0, 0);
    vadd_sse(z_sse, x, y, n);
    m5_dump_reset_stats(0, 0);

    // Verify results
    printf("\nVerifying results...\n");
    int mismatches = 0;
    const float epsilon = 1e-5f;

    for (size_t i = 0; i < n; i++) {
        float diff = fabsf(z_ref[i] - z_sse[i]);
        if (diff > epsilon) {
            if (mismatches < 5) {
                printf("  Mismatch at index %zu: reference=%.6f sse=%.6f diff=%.6f\n",
                       i, z_ref[i], z_sse[i], diff);
            }
            mismatches++;
        }
    }

    if (mismatches == 0) {
        printf("PASSED: All results match!\n");

        // Print sample results
        printf("\nSample results:\n");
        for (size_t i = 0; i < 8 && i < n; i++) {
            printf("  z[%zu] = %.2f + %.2f = %.2f\n",
                   i, x[i], y[i], z_sse[i]);
        }
    } else {
        printf("FAILED: %d mismatches found\n", mismatches);
    }

    // Cleanup
    free(x);
    free(y);
    free(z_ref);
    free(z_sse);

    return (mismatches == 0) ? 0 : 1;
}
