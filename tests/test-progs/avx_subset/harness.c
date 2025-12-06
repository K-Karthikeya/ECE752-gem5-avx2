// C harness for explicit AVX subset assembly test
// Builds alongside avx_subset.S
// Verifies outputs produced by avx_subset_test.
// Output layout (contiguous block pointed to by out_base):
//   [0..7]   product   = (a + b) * a
//   [8..15]  zeros     = vxorps(a+b, a+b)
//   [16..23] min_ab    = vminps(a, b)
//   [24..31] max_ab    = vmaxps(a, b)
//   [32..39] dup_lo    = vmovsldup(a)
//   [40..47] dup_hi    = vmovshdup(b)
//   [48..55] bcast     = vbroadcastss(a[0])

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

extern void avx_subset_test(float *a, float *b, float *out_base);

static void init(float *a, float *b) {
    for (int i = 0; i < 8; ++i) {
        a[i] = (float)(i + 1);          // 1..8
        b[i] = 2.0f * (float)(i + 1);   // 2,4,..16
    }
}

static uint32_t fbits(float f) {
    union { float f; uint32_t u; } v; v.f = f; return v.u; }

int main(void) {
    // 32-byte alignment for all buffers
    float *a        = (float*)aligned_alloc(32, 32);
    float *b        = (float*)aligned_alloc(32, 32);
    float *out_base = (float*)aligned_alloc(32, 56 * sizeof(float));
    if (!a || !b || !out_base) {
        fprintf(stderr, "Allocation failed\n");
        return 2;
    }

    init(a,b);

    // Poison outputs
    for (int i=0;i<56;i++){ out_base[i] = -12345.0f; }

    avx_subset_test(a, b, out_base);

    int errors = 0;

    // helpers for indexes
    const int OFF_PROD   = 0;
    const int OFF_ZERO   = 8;
    const int OFF_MIN    = 16;
    const int OFF_MAX    = 24;
    const int OFF_DUPLO  = 32;
    const int OFF_DUPHI  = 40;
    const int OFF_BCAST  = 48;

    // Product check
    for (int i = 0; i < 8; ++i) {
        float add = a[i] + b[i];
        float expect = add * a[i];
        float got = out_base[OFF_PROD + i];
        if (got != expect) {
            fprintf(stderr,
                "Mismatch PROD[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n",
                i, got, fbits(got), expect, fbits(expect));
            errors++;
        }
    }

    // Zero check
    for (int i = 0; i < 8; ++i) {
        float got = out_base[OFF_ZERO + i];
        if (got != 0.0f) {
            fprintf(stderr,
                "Mismatch ZERO[%d]: got=%f (0x%08" PRIx32 ") expect=0.0 (0x00000000)\n",
                i, got, fbits(got));
            errors++;
        }
    }

    // Min/Max
    for (int i = 0; i < 8; ++i) {
        float exp_min = fminf(a[i], b[i]);
        float exp_max = fmaxf(a[i], b[i]);
        float got_min = out_base[OFF_MIN + i];
        float got_max = out_base[OFF_MAX + i];
        if (got_min != exp_min) {
            fprintf(stderr,
                "Mismatch MIN[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n",
                i, got_min, fbits(got_min), exp_min, fbits(exp_min));
            errors++;
        }
        if (got_max != exp_max) {
            fprintf(stderr,
                "Mismatch MAX[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n",
                i, got_max, fbits(got_max), exp_max, fbits(exp_max));
            errors++;
        }
    }

    // vmovsldup: duplicate low element of each 64-bit chunk
    for (int lane = 0; lane < 4; ++lane) {
        int base = lane * 2;
        float exp = a[base];
        if (out_base[OFF_DUPLO + base] != exp || out_base[OFF_DUPLO + base + 1] != exp) {
            fprintf(stderr,
                "Mismatch DUPLO pair lane=%d: got={%f,%f} expect={%f,%f}\n",
                lane,
                out_base[OFF_DUPLO + base], out_base[OFF_DUPLO + base + 1],
                exp, exp);
            errors++;
        }
    }

    // vmovshdup: duplicate high element of each 64-bit chunk (uses b)
    for (int lane = 0; lane < 4; ++lane) {
        int base = lane * 2;
        float exp = b[base + 1];
        if (out_base[OFF_DUPHI + base] != exp || out_base[OFF_DUPHI + base + 1] != exp) {
            fprintf(stderr,
                "Mismatch DUPHI pair lane=%d: got={%f,%f} expect={%f,%f}\n",
                lane,
                out_base[OFF_DUPHI + base], out_base[OFF_DUPHI + base + 1],
                exp, exp);
            errors++;
        }
    }

    // Broadcast
    for (int i = 0; i < 8; ++i) {
        float got = out_base[OFF_BCAST + i];
        if (got != a[0]) {
            fprintf(stderr,
                "Mismatch BCAST[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n",
                i, got, fbits(got), a[0], fbits(a[0]));
            errors++;
        }
    }

    if (errors) {
        printf("AVX subset test FAIL (%d errors)\n", errors);
    } else {
        printf("AVX subset test PASS\n");
    }

    free(a); free(b); free(out_base);
    return errors ? 1 : 0;
}
