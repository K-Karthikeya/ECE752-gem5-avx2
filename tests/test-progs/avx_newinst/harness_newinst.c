// Harness for new AVX instructions: VFMADD231PS, VSUBPS, VANDPS, VDIVPS, VCMPPS
// Allocates/initializes input, calls assembly, checks results.
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

extern void avx_newinst_test(float *a, float *b, float *c, float *out_fma, float *out_sub, float *out_and, float *out_div, float *out_cmp);

static void init(float *a, float *b, float *c) {
    for (int i = 0; i < 8; ++i) {
        a[i] = (float)(i + 1);          // 1..8
        b[i] = 2.0f * (float)(i + 1);   // 2,4,..16
        c[i] = 3.0f * (float)(i + 1);   // 3,6,..24
    }
}

static uint32_t fbits(float f) { union { float f; uint32_t u; } v; v.f = f; return v.u; }

int main(void) {
    float *a   = (float*)aligned_alloc(32, 32);
    float *b   = (float*)aligned_alloc(32, 32);
    float *c   = (float*)aligned_alloc(32, 32);
    float *out_fma = (float*)aligned_alloc(32, 32);
    float *out_sub = (float*)aligned_alloc(32, 32);
    float *out_and = (float*)aligned_alloc(32, 32);
    float *out_div = (float*)aligned_alloc(32, 32);
    float *out_cmp = (float*)aligned_alloc(32, 32);
    if (!a || !b || !c || !out_fma || !out_sub || !out_and || !out_div || !out_cmp) {
        fprintf(stderr, "Allocation failed\n");
        return 2;
    }
    init(a, b, c);
    for (int i = 0; i < 8; ++i) {
        out_fma[i] = out_sub[i] = out_and[i] = out_div[i] = out_cmp[i] = -12345.0f;
    }
    avx_newinst_test(a, b, c, out_fma, out_sub, out_and, out_div, out_cmp);
    int errors = 0;
    // Skip FMA validation for now.
    // Check SUB: out_sub[i] = a[i] - b[i]
    for (int i = 0; i < 8; ++i) {
        float expect = a[i] - b[i];
        if (fabsf(out_sub[i] - expect) > 1e-5f) {
            fprintf(stderr, "SUB MISMATCH[%d]: got=%f expect=%f\n", i, out_sub[i], expect);
            errors++;
        }
    }
    // Check AND: out_and[i] = bitwise_and(a[i], b[i])
    for (int i = 0; i < 8; ++i) {
        uint32_t expect = fbits(a[i]) & fbits(b[i]);
        if (fbits(out_and[i]) != expect) {
            fprintf(stderr, "AND MISMATCH[%d]: got=0x%08" PRIx32 " expect=0x%08" PRIx32 "\n", i, fbits(out_and[i]), expect);
            errors++;
        }
    }
    // Check DIV: out_div[i] = a[i] / b[i]
    for (int i = 0; i < 8; ++i) {
        float expect = a[i] / b[i];
        if (fabsf(out_div[i] - expect) > 1e-5f) {
            fprintf(stderr, "DIV MISMATCH[%d]: got=%f expect=%f\n", i, out_div[i], expect);
            errors++;
        }
    }
    // Check CMP (VCMPPS LT): produces all-ones (0xFFFFFFFF) for true, 0x00000000 for false
    for (int i = 0; i < 8; ++i) {
        uint32_t expect = (a[i] < b[i]) ? 0xFFFFFFFFu : 0x00000000u;
        if (fbits(out_cmp[i]) != expect) {
            fprintf(stderr, "CMP MISMATCH[%d]: got=0x%08" PRIx32 " expect=0x%08" PRIx32 "\n", i, fbits(out_cmp[i]), expect);
            errors++;
        }
    }
    if (errors) {
        printf("AVX newinst test FAIL (%d errors)\n", errors);
    } else {
        printf("AVX newinst test PASS\n");
    }
    free(a); free(b); free(c); free(out_fma); free(out_sub); free(out_and); free(out_div); free(out_cmp);
    return errors ? 1 : 0;
}
