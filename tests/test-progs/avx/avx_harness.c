#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Prototypes for assembly test routines.
void avx_vaddf_128(const float *a, const float *b, float *out);
void avx_vaddf_256(const float *a, const float *b, float *out);
void avx_vsubf_128(const float *a, const float *b, float *out);
void avx_vsubf_256(const float *a, const float *b, float *out);
void avx_vmulf_128(const float *a, const float *b, float *out);
void avx_vmulf_256(const float *a, const float *b, float *out);
void avx_vdivf_128(const float *a, const float *b, float *out);
void avx_vdivf_256(const float *a, const float *b, float *out);
void avx_vxorf_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vxorf_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vandf_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vandf_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vfmadd231f_128(const float *a, const float *b, const float *c, float *out);
void avx_vfmadd231f_256(const float *a, const float *b, const float *c, float *out);
void avx_vcmpf_eq_128(const float *a, const float *b, uint32_t *mask_out);
void avx_vcmpf_eq_256(const float *a, const float *b, uint32_t *mask_out);
void avx_vclear_upper(void);
void avx_ldstfp_128(float *mem, float *out);
void avx_ldstfp_256(float *mem, float *out);

static int check_vec_f32(const float *got, const float *exp, int lanes)
{
    for (int i = 0; i < lanes; i++) {
        // Allow small epsilon for FP ops
        float diff = got[i] - exp[i];
        if (diff < 0) diff = -diff;
        if (diff > 1e-6f) {
            printf("FAIL lane %d: got=%g exp=%g\n", i, got[i], exp[i]);
            return 0;
        }
    }
    return 1;
}

static int check_vec_u32(const uint32_t *got, const uint32_t *exp, int lanes)
{
    for (int i = 0; i < lanes; i++) {
        if (got[i] != exp[i]) {
            printf("FAIL lane %d: got=%#x exp=%#x\n", i, got[i], exp[i]);
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    int pass = 1;

    // 128-bit floats (4 lanes)
    {
        float a[4] = {1.0f, -2.5f, 3.0f, 0.0f};
        float b[4] = {2.0f, 4.5f, -1.0f, 7.0f};
        float out[4];
        float exp_add[4];
        for (int i = 0; i < 4; i++) exp_add[i] = a[i] + b[i];
        avx_vaddf_128(a, b, out);
        if (!check_vec_f32(out, exp_add, 4)) pass = 0; else printf("PASS vaddf_128\n");

        float exp_sub[4];
        for (int i = 0; i < 4; i++) exp_sub[i] = a[i] - b[i];
        avx_vsubf_128(a, b, out);
        if (!check_vec_f32(out, exp_sub, 4)) pass = 0; else printf("PASS vsubf_128\n");

        float exp_mul[4];
        for (int i = 0; i < 4; i++) exp_mul[i] = a[i] * b[i];
        avx_vmulf_128(a, b, out);
        if (!check_vec_f32(out, exp_mul, 4)) pass = 0; else printf("PASS vmulf_128\n");

        float bb[4] = {2.0f, 4.5f, 1.0f, 7.0f}; // avoid zero divisors
        float exp_div[4];
        for (int i = 0; i < 4; i++) exp_div[i] = a[i] / bb[i];
        avx_vdivf_128(a, bb, out);
        if (!check_vec_f32(out, exp_div, 4)) pass = 0; else printf("PASS vdivf_128\n");

        uint32_t au[4] = {0xFFFF0000u, 0x0u, 0xAAAAAAAAu, 0x12345678u};
        uint32_t bu[4] = {0x0000FFFFu, 0xFFFFFFFFu, 0x55555555u, 0x87654321u};
        uint32_t out_u[4];
        uint32_t exp_xor[4];
        for (int i = 0; i < 4; i++) exp_xor[i] = au[i] ^ bu[i];
        avx_vxorf_128(au, bu, out_u);
        if (!check_vec_u32(out_u, exp_xor, 4)) pass = 0; else printf("PASS vxorf_128\n");

        uint32_t exp_and[4];
        for (int i = 0; i < 4; i++) exp_and[i] = au[i] & bu[i];
        avx_vandf_128(au, bu, out_u);
        if (!check_vec_u32(out_u, exp_and, 4)) pass = 0; else printf("PASS vandf_128\n");

        float c[4] = {0.5f, -1.0f, 2.0f, 3.0f};
        float exp_fma[4];
        for (int i = 0; i < 4; i++) exp_fma[i] = a[i] * b[i] + c[i];
        avx_vfmadd231f_128(a, b, c, out);
        if (!check_vec_f32(out, exp_fma, 4)) pass = 0; else printf("PASS vfmadd231f_128\n");

        uint32_t mask[4];
        float ae[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        float beq[4] = {1.0f, -3.0f, 3.0f, 5.0f};
        avx_vcmpf_eq_128(ae, beq, mask);
        uint32_t exp_mask[4] = {0xFFFFFFFFu, 0x0u, 0xFFFFFFFFu, 0x0u};
        if (!check_vec_u32(mask, exp_mask, 4)) pass = 0; else printf("PASS vcmpf_eq_128\n");
    }

    // 256-bit floats (8 lanes)
    {
        float a[8] = {1.0f, -2.5f, 3.0f, 0.0f, 5.5f, -7.0f, 8.0f, 1.25f};
        float b[8] = {2.0f, 4.5f, -1.0f, 7.0f, -3.5f, 0.5f, 2.25f, -1.25f};
        float out[8];
        float exp_add[8];
        for (int i = 0; i < 8; i++) exp_add[i] = a[i] + b[i];
        avx_vaddf_256(a, b, out);
        if (!check_vec_f32(out, exp_add, 8)) pass = 0; else printf("PASS vaddf_256\n");

        float exp_sub[8];
        for (int i = 0; i < 8; i++) exp_sub[i] = a[i] - b[i];
        avx_vsubf_256(a, b, out);
        if (!check_vec_f32(out, exp_sub, 8)) pass = 0; else printf("PASS vsubf_256\n");

        float exp_mul[8];
        for (int i = 0; i < 8; i++) exp_mul[i] = a[i] * b[i];
        avx_vmulf_256(a, b, out);
        if (!check_vec_f32(out, exp_mul, 8)) pass = 0; else printf("PASS vmulf_256\n");

        float bb[8] = {2.0f, 4.5f, 1.0f, 7.0f, 2.0f, 1.0f, 0.75f, 0.5f};
        float exp_div[8];
        for (int i = 0; i < 8; i++) exp_div[i] = a[i] / bb[i];
        avx_vdivf_256(a, bb, out);
        if (!check_vec_f32(out, exp_div, 8)) pass = 0; else printf("PASS vdivf_256\n");

        uint32_t au[8] = {0xFFFF0000u,0x0u,0xAAAAAAAAu,0x12345678u,0xDEADBEEFu,0xCAFEBABEu,0x13579BDFu,0x2468ACE0u};
        uint32_t bu[8] = {0x0000FFFFu,0xFFFFFFFFu,0x55555555u,0x87654321u,0xFEEDFACEu,0x0u,0x02468ACEu,0x13579BDFu};
        uint32_t out_u[8];
        uint32_t exp_xor[8];
        for (int i = 0; i < 8; i++) exp_xor[i] = au[i] ^ bu[i];
        avx_vxorf_256(au, bu, out_u);
        if (!check_vec_u32(out_u, exp_xor, 8)) pass = 0; else printf("PASS vxorf_256\n");

        uint32_t exp_and[8];
        for (int i = 0; i < 8; i++) exp_and[i] = au[i] & bu[i];
        avx_vandf_256(au, bu, out_u);
        if (!check_vec_u32(out_u, exp_and, 8)) pass = 0; else printf("PASS vandf_256\n");

        float c[8] = {0.5f,-1.0f,2.0f,3.0f,0.0f,1.0f,-2.0f,4.0f};
        float exp_fma[8];
        for (int i = 0; i < 8; i++) exp_fma[i] = a[i] * b[i] + c[i];
        avx_vfmadd231f_256(a, b, c, out);
        if (!check_vec_f32(out, exp_fma, 8)) pass = 0; else printf("PASS vfmadd231f_256\n");

        uint32_t mask[8];
        float ae[8] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f};
        float beq[8] = {1.0f,-3.0f,3.0f,5.0f,5.0f,0.0f,7.1f,8.0f};
        avx_vcmpf_eq_256(ae, beq, mask);
        uint32_t exp_mask[8] = {0xFFFFFFFFu,0x0u,0xFFFFFFFFu,0x0u,0xFFFFFFFFu,0x0u,0x0u,0xFFFFFFFFu};
        if (!check_vec_u32(mask, exp_mask, 8)) pass = 0; else printf("PASS vcmpf_eq_256\n");
    }

    // vclear upper (vzeroupper)
    avx_vclear_upper();
    printf("PASS vclear_upper\n");

    // ldst for 128/256 to test load/store microops sequences
    {
        float mem128[4] = {10.f, 20.f, 30.f, 40.f};
        float out128[4] = {0};
        avx_ldstfp_128(mem128, out128);
        if (!check_vec_f32(out128, mem128, 4)) pass = 0; else printf("PASS ldstfp_128\n");

        float mem256[8] = {1.f,2.f,3.f,4.f,5.f,6.f,7.f,8.f};
        float out256[8] = {0};
        avx_ldstfp_256(mem256, out256);
        if (!check_vec_f32(out256, mem256, 8)) pass = 0; else printf("PASS ldstfp_256\n");
    }

    if (!pass) {
        printf("\nOverall: FAIL\n");
        return 1;
    }
    printf("\nOverall: PASS\n");
    return 0;
}
