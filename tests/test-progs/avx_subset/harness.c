// C harness for explicit AVX subset assembly test
// Builds alongside avx_subset.S
// Verifies outputs produced by avx_subset_test.
// Expected out[i] = (a[i] + b[i]) * a[i]
// Expected tmp[i] = 0.0f
// Also prints hexadecimal bit patterns for debugging discrepancies.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

extern void avx_subset_test(float *a, float *b, float *out, float *tmp, float *riprel_target);

static void init(float *a, float *b) {
    for (int i = 0; i < 8; ++i) {
        a[i] = (float)(i + 1);          // 1..8
        b[i] = 2.0f * (float)(i + 1);   // 2,4,..16
    }
}

static uint32_t fbits(float f) {
    union { float f; uint32_t u; } v; v.f = f; return v.u; }

int main(void) {
    // Use aligned_alloc for 32-byte alignment needed by vmovaps.
    float *a   = (float*)aligned_alloc(32, 32);
    float *b   = (float*)aligned_alloc(32, 32);
    float *out = (float*)aligned_alloc(32, 32);
    float *tmp = (float*)aligned_alloc(32, 32);
    if (!a || !b || !out || !tmp) {
        fprintf(stderr, "Allocation failed\n");
        return 2;
    }

    init(a,b);

    // Poison outputs so we can detect failure to write.
    for (int i=0;i<8;i++){ out[i] = -12345.0f; tmp[i] = -12345.0f; }

    avx_subset_test(a,b,out,tmp,NULL);

    int errors = 0;
    for (int i = 0; i < 8; ++i) {
        float add = a[i] + b[i];
        float expect = add * a[i];
        if (out[i] != expect) {
            fprintf(stderr,
                "Mismatch OUT[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n",
                i, out[i], fbits(out[i]), expect, fbits(expect));
            errors++;
        }
        if (tmp[i] != 0.0f) {
            fprintf(stderr,
                "Mismatch TMP[%d]: got=%f (0x%08" PRIx32 ") expect=0.0 (0x00000000)\n",
                i, tmp[i], fbits(tmp[i]));
            errors++;
        }
    }

    if (errors) {
        printf("AVX subset test FAIL (%d errors)\n", errors);
    } else {
        printf("AVX subset test PASS\n");
    }

    free(a); free(b); free(out); free(tmp);
    return errors ? 1 : 0;
}
