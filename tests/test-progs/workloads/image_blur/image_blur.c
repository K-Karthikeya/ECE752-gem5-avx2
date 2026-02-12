/**
 * Image Blur Benchmark
 *
 * Implements 3x3 Gaussian blur on a grayscale image using AVX-256 vectorization
 * that processes 8 pixels at once.
 *
 * This demonstrates:
 * - SIMD multiply-accumulate (VMULPS, VADDPS)
 * - Memory access patterns typical of image processing
 * - Practical use case for AVX vectorization
 *
 * Gaussian kernel (normalized):
 *   1/16 * [ 1  2  1 ]
 *          [ 2  4  2 ]
 *          [ 1  2  1 ]
 *
 * Verification is done against a C reference implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

// Assembly kernel (AVX-256 only)
extern void blur_avx256(float *out, const float *in, size_t width, size_t height);

/**
 * Read timestamp counter
 */
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

/**
 * Initialize image with test pattern
 */
void init_image(float *img, size_t width, size_t height) {
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            // Create a checkerboard + gradient pattern
            float val = ((x / 8 + y / 8) % 2) * 128.0f + (x + y) * 0.5f;
            img[y * width + x] = val;
        }
    }
}

/**
 * C reference implementation: 3x3 Gaussian blur
 */
void blur_reference(float *out, const float *in, size_t width, size_t height) {
    // Gaussian kernel weights (normalized)
    const float kernel[3][3] = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            float sum = 0.0f;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    // Handle borders with mirroring
                    size_t sy = y;
                    size_t sx = x;

                    if (y == 0 && ky < 0) sy = 0;
                    else if (y == height-1 && ky > 0) sy = height-1;
                    else sy = y + ky;

                    if (x == 0 && kx < 0) sx = 0;
                    else if (x == width-1 && kx > 0) sx = width-1;
                    else sx = x + kx;

                    sum += in[sy * width + sx] * kernel[ky+1][kx+1];
                }
            }

            out[y * width + x] = sum;
        }
    }
}

/**
 * Verify results match within tolerance
 */
int verify_images(const float *img_ref, const float *img_avx,
                  size_t width, size_t height) {
    const float epsilon = 1e-2f;  // Tolerance for FP differences
    int errors = 0;

    for (size_t i = 0; i < width * height; i++) {
        float diff = img_ref[i] - img_avx[i];
        if (diff < 0) diff = -diff;

        if (diff > epsilon) {
            if (errors < 10) {
                printf("Mismatch at pixel %zu: reference=%f avx=%f diff=%f\n",
                       i, img_ref[i], img_avx[i], diff);
            }
            errors++;
        }
    }

    return errors;
}

/**
 * Simple image checksum for validation
 */
float compute_checksum(const float *img, size_t width, size_t height) {
    double sum = 0.0;
    for (size_t i = 0; i < width * height; i++) {
        sum += img[i];
    }
    return (float)sum;
}

int main(int argc, char **argv) {
    // Image size: default 256x256
    size_t width = 256;
    size_t height = 256;

    if (argc > 2) {
        width = (size_t)atoi(argv[1]);
        height = (size_t)atoi(argv[2]);
        // Round width to multiple of 8 for AVX
        width = (width + 7) & ~7;
    }

    printf("Image Blur Benchmark (3x3 Gaussian)\n");
    printf("====================================\n");
    printf("Image size: %zux%zu pixels (%.2f KB)\n",
           width, height, (width * height * sizeof(float)) / 1024.0);
    printf("Operation: 3x3 convolution with Gaussian kernel\n\n");

    // Allocate aligned memory
    float *img_in = (float*)aligned_alloc(32, width * height * sizeof(float));
    float *img_scalar = (float*)aligned_alloc(32, width * height * sizeof(float));
    float *img_avx = (float*)aligned_alloc(32, width * height * sizeof(float));

    if (!img_in || !img_scalar || !img_avx) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize input image
    init_image(img_in, width, height);

    // Run C reference version for verification
    printf("Running C reference version...\n");
    blur_reference(img_scalar, img_in, width, height);
    float checksum_ref = compute_checksum(img_scalar, width, height);

    // Run AVX version with timing
    printf("Running AVX-256 version...\n");
    uint64_t start = rdtsc();
    blur_avx256(img_avx, img_in, width, height);
    uint64_t avx_cycles = rdtsc() - start;
    float checksum_avx = compute_checksum(img_avx, width, height);

    // Verify
    printf("\nVerifying results...\n");
    printf("Checksum (reference): %f\n", checksum_ref);
    printf("Checksum (AVX):       %f\n", checksum_avx);

    int errors = verify_images(img_scalar, img_avx, width, height);
    if (errors > 0) {
        printf("FAILED: %d pixel mismatches found\n", errors);
    } else {
        printf("PASSED: Images match\n");
    }

    // Performance metrics
    size_t total_pixels = width * height;
    size_t ops_per_pixel = 9 * 2;  // 9 multiplies + 9 adds per pixel

    printf("\nPerformance Results:\n");
    printf("--------------------\n");
    printf("AVX-256 cycles: %llu\n", (unsigned long long)avx_cycles);
    printf("Cycles/pixel:   %.2f\n", (double)avx_cycles / total_pixels);
    printf("Total ops:      %zu (18 ops/pixel * %zu pixels)\n",
           ops_per_pixel * total_pixels, total_pixels);

    // Cleanup
    free(img_in);
    free(img_scalar);
    free(img_avx);

    return (errors > 0) ? 1 : 0;
}
