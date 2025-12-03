// C harness for AVX double-precision subset assembly test
// Builds alongside avx_subset.S
// Verifies outputs produced by avx_subset_test.
// Layout:
//   out[0..3]   = a + b
//   out[4..7]   = a - b
//   out[8..11]  = a * b
//   out[12..15] = a / b
//   tmp[0..1]   = movddup xmm from a (both lanes == a[0])
//   tmp[2..5]   = movddup ymm from b (lanes [0,1]==b[0], [2,3]==b[2])

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

extern void avx_subset_test(double *a, double *b, double *out, double *tmp, void *unused);

static void init(double *a, double *b) {
    for (int i = 0; i < 4; ++i) {
        a[i] = (double)(i + 1);          // 1..4
        b[i] = 2.0 * (double)(i + 1);    // 2,4,6,8
    }
}

int main(void) {
    // Use aligned_alloc for 32-byte alignment for vmovapd 256b loads/stores.
    double *a   = (double*)aligned_alloc(32, 32);
    double *b   = (double*)aligned_alloc(32, 32);
    // out needs space for 16 doubles (4 groups * 4 lanes)
    double *out = (double*)aligned_alloc(32, 32 * 4);
    // tmp needs space for 6 doubles (2 for xmm, 4 for ymm)
    double *tmp = (double*)aligned_alloc(32, 32);
    if (!a || !b || !out || !tmp) {
        fprintf(stderr, "Allocation failed\n");
        return 2;
    }

    init(a,b);

    // Poison outputs
    for (int i=0;i<16;i++) out[i] = -12345.0;
    for (int i=0;i<6;i++) tmp[i] = -12345.0;

    avx_subset_test(a,b,out,tmp,NULL);

    int errors = 0;
    // Group 0: add
    for (int i=0;i<4;i++) {
        double expect = a[i] + b[i];
        if (out[i] != expect) {
            fprintf(stderr, "ADD[%d]: got=%.8f expect=%.8f\n", i, out[i], expect);
            errors++;
        }
    }
    // Group 1: sub
    for (int i=0;i<4;i++) {
        double expect = a[i] - b[i];
        if (out[4+i] != expect) {
            fprintf(stderr, "SUB[%d]: got=%.8f expect=%.8f\n", i, out[4+i], expect);
            errors++;
        }
    }
    // Group 2: mul
    for (int i=0;i<4;i++) {
        double expect = a[i] * b[i];
        if (out[8+i] != expect) {
            fprintf(stderr, "MUL[%d]: got=%.8f expect=%.8f\n", i, out[8+i], expect);
            errors++;
        }
    }
    // Group 3: div
    for (int i=0;i<4;i++) {
        double expect = a[i] / b[i];
        if (out[12+i] != expect) {
            fprintf(stderr, "DIV[%d]: got=%.8f expect=%.8f\n", i, out[12+i], expect);
            errors++;
        }
    }
    // movddup xmm from a: both lanes should be a[0]
    if (tmp[0] != a[0] || tmp[1] != a[0]) {
        fprintf(stderr, "MOVDDUP XMM: got={%.8f, %.8f} expect={%.8f, %.8f}\n",
                tmp[0], tmp[1], a[0], a[0]);
        errors++;
    }
    // movddup ymm from b: lanes [0,1] == b[0], [2,3] == b[2]
    if (tmp[2] != b[0] || tmp[3] != b[0] || tmp[4] != b[2] || tmp[5] != b[2]) {
        fprintf(stderr, "MOVDDUP YMM: got={%.8f, %.8f, %.8f, %.8f} expect={%.8f, %.8f, %.8f, %.8f}\n",
                tmp[2], tmp[3], tmp[4], tmp[5], b[0], b[0], b[2], b[2]);
        errors++;
    }

    if (errors) {
        printf("AVX double subset test FAIL (%d errors)\n", errors);
    } else {
        printf("AVX double subset test PASS\n");
    }

    free(a); free(b); free(out); free(tmp);
    return errors ? 1 : 0;
}
