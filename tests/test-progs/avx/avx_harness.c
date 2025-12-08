#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Prototypes for assembly test routines - Floating Point
void avx_vaddf_128(const float *a, const float *b, float *out);
void avx_vaddf_256(const float *a, const float *b, float *out);
void avx_vsubf_128(const float *a, const float *b, float *out);
void avx_vsubf_256(const float *a, const float *b, float *out);
void avx_vmulf_128(const float *a, const float *b, float *out);
void avx_vmulf_256(const float *a, const float *b, float *out);
void avx_vdivf_128(const float *a, const float *b, float *out);
void avx_vdivf_256(const float *a, const float *b, float *out);
void avx_vminps_128(const float *a, const float *b, float *out);
void avx_vminps_256(const float *a, const float *b, float *out);
void avx_vmaxps_128(const float *a, const float *b, float *out);
void avx_vmaxps_256(const float *a, const float *b, float *out);
void avx_vxorf_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vxorf_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vandf_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vandf_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vfmadd231f_128(const float *a, const float *b, const float *c, float *out);
void avx_vfmadd231f_256(const float *a, const float *b, const float *c, float *out);
void avx_vcmpf_eq_128(const float *a, const float *b, uint32_t *mask_out);
void avx_vcmpf_eq_256(const float *a, const float *b, uint32_t *mask_out);
void avx_vmovsldup_128(const float *src, float *out);
void avx_vmovsldup_256(const float *src, float *out);
void avx_vmovshdup_128(const float *src, float *out);
void avx_vmovshdup_256(const float *src, float *out);
void avx_vbroadcastss_128(const float *src, float *out);
void avx_vbroadcastss_256(const float *src, float *out);
void avx_vmovddup_128(const double *src, double *out);
void avx_vmovddup_256(const double *src, double *out);
void avx_vclear_upper(void);
void avx_ldstfp_128(float *mem, float *out);
void avx_ldstfp_256(float *mem, float *out);

// Prototypes for double-precision AVX instructions
void avx_vaddd_128(const double *a, const double *b, double *out);
void avx_vaddd_256(const double *a, const double *b, double *out);
void avx_vsubd_128(const double *a, const double *b, double *out);
void avx_vsubd_256(const double *a, const double *b, double *out);
void avx_vmuld_128(const double *a, const double *b, double *out);
void avx_vmuld_256(const double *a, const double *b, double *out);
void avx_vdivd_128(const double *a, const double *b, double *out);
void avx_vdivd_256(const double *a, const double *b, double *out);
void avx_vbroadcastsd_128(const double *src, double *out);
void avx_vbroadcastsd_256(const double *src, double *out);

// Prototypes for integer AVX instructions
void avx_vpaddd_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpaddd_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpand_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpand_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpxor_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpxor_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpor_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpor_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpsubd_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpsubd_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpmulld_128(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpmulld_256(const uint32_t *a, const uint32_t *b, uint32_t *out);
void avx_vpminsd_128(const int32_t *a, const int32_t *b, int32_t *out);
void avx_vpminsd_256(const int32_t *a, const int32_t *b, int32_t *out);
void avx_vpbroadcastd_128(const uint32_t *src, uint32_t *out);
void avx_vpbroadcastd_256(const uint32_t *src, uint32_t *out);
void avx_vmovdqu_ld_128(const uint32_t *src, uint32_t *out);
void avx_vmovdqu_st_128(const uint32_t *src, uint32_t *out);
void avx_vmovdqu_ld_256(const uint32_t *src, uint32_t *out);
void avx_vmovdqu_st_256(const uint32_t *src, uint32_t *out);
void avx_vmovd_xmm_r(uint32_t src, uint32_t *out);
uint32_t avx_vmovd_r_xmm(const uint32_t *src);

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

static int check_vec_f64(const double *got, const double *exp, int lanes)
{
    for (int i = 0; i < lanes; i++) {
        // Allow small epsilon for FP ops
        double diff = got[i] - exp[i];
        if (diff < 0) diff = -diff;
        if (diff > 1e-12) {
            printf("FAIL lane %d: got=%g exp=%g\n", i, got[i], exp[i]);
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

        // VMINPS 128-bit
        float am[4] = {1.0f, -2.5f, 3.0f, -5.0f};
        float bm[4] = {2.0f, -1.0f, 2.5f, -6.0f};
        float exp_min[4];
        for (int i = 0; i < 4; i++) exp_min[i] = (am[i] < bm[i]) ? am[i] : bm[i];
        avx_vminps_128(am, bm, out);
        if (!check_vec_f32(out, exp_min, 4)) pass = 0; else printf("PASS vminps_128\n");

        // VMAXPS 128-bit
        float exp_max[4];
        for (int i = 0; i < 4; i++) exp_max[i] = (am[i] > bm[i]) ? am[i] : bm[i];
        avx_vmaxps_128(am, bm, out);
        if (!check_vec_f32(out, exp_max, 4)) pass = 0; else printf("PASS vmaxps_128\n");

        // VMOVSLDUP 128-bit - duplicate low (lanes 0,0,2,2)
        float sldup_src[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        float exp_sldup[4] = {1.0f, 1.0f, 3.0f, 3.0f};
        avx_vmovsldup_128(sldup_src, out);
        if (!check_vec_f32(out, exp_sldup, 4)) pass = 0; else printf("PASS vmovsldup_128\n");

        // VMOVSHDUP 128-bit - duplicate high (lanes 1,1,3,3)
        float exp_shdup[4] = {2.0f, 2.0f, 4.0f, 4.0f};
        avx_vmovshdup_128(sldup_src, out);
        if (!check_vec_f32(out, exp_shdup, 4)) pass = 0; else printf("PASS vmovshdup_128\n");

        // VBROADCASTSS 128-bit
        // float broadcast_src = 3.14159f;
        // float exp_bcast[4] = {broadcast_src, broadcast_src, broadcast_src, broadcast_src};
        // avx_vbroadcastss_128(&broadcast_src, out);
        // if (!check_vec_f32(out, exp_bcast, 4)) pass = 0; else printf("PASS vbroadcastss_128\n");

        // VMOVDDUP 128-bit - duplicate low double
        double ddup_src[2] = {1.5, 2.5};
        double ddup_out[2];
        double exp_ddup[2] = {1.5, 1.5};
        avx_vmovddup_128(ddup_src, ddup_out);
        if (ddup_out[0] != exp_ddup[0] || ddup_out[1] != exp_ddup[1]) {
            printf("FAIL vmovddup_128\n");
            pass = 0;
        } else {
            printf("PASS vmovddup_128\n");
        }

        // VADDPD 128-bit (2 doubles)
        double ad[2] = {1.5, -2.75};
        double bd[2] = {3.25, 1.5};
        double outd[2];
        double exp_addd[2] = {4.75, -1.25};
        avx_vaddd_128(ad, bd, outd);
        if (!check_vec_f64(outd, exp_addd, 2)) pass = 0; else printf("PASS vaddd_128\n");

        // VSUBPD 128-bit
        double exp_subd[2] = {-1.75, -4.25};
        avx_vsubd_128(ad, bd, outd);
        if (!check_vec_f64(outd, exp_subd, 2)) pass = 0; else printf("PASS vsubd_128\n");

        // VMULPD 128-bit
        double exp_muld[2] = {4.875, -4.125};
        avx_vmuld_128(ad, bd, outd);
        if (!check_vec_f64(outd, exp_muld, 2)) pass = 0; else printf("PASS vmuld_128\n");

        // VDIVPD 128-bit
        double exp_divd[2] = {0.461538462, -1.833333333};
        avx_vdivd_128(ad, bd, outd);
        if (!check_vec_f64(outd, exp_divd, 2)) pass = 0; else printf("PASS vdivd_128\n");

        // // VBROADCASTSD 128-bit
        // double bcast_src_d = 2.718281828;
        // double exp_bcastd[2] = {bcast_src_d, bcast_src_d};
        // avx_vbroadcastsd_128(&bcast_src_d, outd);
        // if (!check_vec_f64(outd, exp_bcastd, 2)) pass = 0; else printf("PASS vbroadcastsd_128\n");
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

        // VMINPS 256-bit
        float am[8] = {1.0f, -2.5f, 3.0f, -5.0f, 7.0f, -8.5f, 9.0f, -10.0f};
        float bm[8] = {2.0f, -1.0f, 2.5f, -6.0f, 6.5f, -9.0f, 8.5f, -9.5f};
        float exp_min[8];
        for (int i = 0; i < 8; i++) exp_min[i] = (am[i] < bm[i]) ? am[i] : bm[i];
        avx_vminps_256(am, bm, out);
        if (!check_vec_f32(out, exp_min, 8)) pass = 0; else printf("PASS vminps_256\n");

        // VMAXPS 256-bit
        float exp_max[8];
        for (int i = 0; i < 8; i++) exp_max[i] = (am[i] > bm[i]) ? am[i] : bm[i];
        avx_vmaxps_256(am, bm, out);
        if (!check_vec_f32(out, exp_max, 8)) pass = 0; else printf("PASS vmaxps_256\n");

        // VMOVSLDUP 256-bit - duplicate low (lanes 0,0,2,2,4,4,6,6)
        float sldup_src[8] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
        float exp_sldup[8] = {1.0f, 1.0f, 3.0f, 3.0f, 5.0f, 5.0f, 7.0f, 7.0f};
        avx_vmovsldup_256(sldup_src, out);
        if (!check_vec_f32(out, exp_sldup, 8)) pass = 0; else printf("PASS vmovsldup_256\n");

        // VMOVSHDUP 256-bit - duplicate high (lanes 1,1,3,3,5,5,7,7)
        float exp_shdup[8] = {2.0f, 2.0f, 4.0f, 4.0f, 6.0f, 6.0f, 8.0f, 8.0f};
        avx_vmovshdup_256(sldup_src, out);
        if (!check_vec_f32(out, exp_shdup, 8)) pass = 0; else printf("PASS vmovshdup_256\n");

        // // VBROADCASTSS 256-bit
        // float broadcast_src = 2.71828f;
        // float exp_bcast[8];
        // for (int i = 0; i < 8; i++) exp_bcast[i] = broadcast_src;
        // avx_vbroadcastss_256(&broadcast_src, out);
        // if (!check_vec_f32(out, exp_bcast, 8)) pass = 0; else printf("PASS vbroadcastss_256\n");

        // VMOVDDUP 256-bit - duplicate low double of each 128-bit lane
        double ddup_src[4] = {1.5, 2.5, 3.5, 4.5};
        double ddup_out[4];
        double exp_ddup[4] = {1.5, 1.5, 3.5, 3.5};
        avx_vmovddup_256(ddup_src, ddup_out);
        int ddup_ok = 1;
        for (int i = 0; i < 4; i++) {
            if (ddup_out[i] != exp_ddup[i]) ddup_ok = 0;
        }
        if (!ddup_ok) {
            printf("FAIL vmovddup_256\n");
            pass = 0;
        } else {
            printf("PASS vmovddup_256\n");
        }

        // VADDPD 256-bit (4 doubles)
        double ad256[4] = {1.5, -2.75, 3.0, -5.5};
        double bd256[4] = {3.25, 1.5, -1.0, 2.5};
        double outd256[4];
        double exp_addd256[4] = {4.75, -1.25, 2.0, -3.0};
        avx_vaddd_256(ad256, bd256, outd256);
        if (!check_vec_f64(outd256, exp_addd256, 4)) pass = 0; else printf("PASS vaddd_256\n");

        // VSUBPD 256-bit
        double exp_subd256[4] = {-1.75, -4.25, 4.0, -8.0};
        avx_vsubd_256(ad256, bd256, outd256);
        if (!check_vec_f64(outd256, exp_subd256, 4)) pass = 0; else printf("PASS vsubd_256\n");

        // VMULPD 256-bit
        double exp_muld256[4] = {4.875, -4.125, -3.0, -13.75};
        avx_vmuld_256(ad256, bd256, outd256);
        if (!check_vec_f64(outd256, exp_muld256, 4)) pass = 0; else printf("PASS vmuld_256\n");

        // VDIVPD 256-bit
        double exp_divd256[4] = {0.461538462, -1.833333333, -3.0, -2.2};
        avx_vdivd_256(ad256, bd256, outd256);
        if (!check_vec_f64(outd256, exp_divd256, 4)) pass = 0; else printf("PASS vdivd_256\n");

        // VBROADCASTSD 256-bit
        // double bcast_src_d256 = 1.414213562;
        // double exp_bcastd256[4] = {bcast_src_d256, bcast_src_d256, bcast_src_d256, bcast_src_d256};
        // avx_vbroadcastsd_256(&bcast_src_d256, outd256);
        // if (!check_vec_f64(outd256, exp_bcastd256, 4)) pass = 0; else printf("PASS vbroadcastsd_256\n");
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

    // Integer AVX Tests - 128-bit
    {
        uint32_t a[4] = {10, 20, 30, 40};
        uint32_t b[4] = {5, 15, 25, 35};
        uint32_t out[4];
        
        // VPADDD 128-bit
        uint32_t exp_add[4];
        for (int i = 0; i < 4; i++) exp_add[i] = a[i] + b[i];
        avx_vpaddd_128(a, b, out);
        if (!check_vec_u32(out, exp_add, 4)) pass = 0; else printf("PASS vpaddd_128\n");
        
        // VPAND 128-bit
        uint32_t exp_and[4];
        for (int i = 0; i < 4; i++) exp_and[i] = a[i] & b[i];
        avx_vpand_128(a, b, out);
        if (!check_vec_u32(out, exp_and, 4)) pass = 0; else printf("PASS vpand_128\n");
        
        // VPXOR 128-bit
        uint32_t exp_xor[4];
        for (int i = 0; i < 4; i++) exp_xor[i] = a[i] ^ b[i];
        avx_vpxor_128(a, b, out);
        if (!check_vec_u32(out, exp_xor, 4)) pass = 0; else printf("PASS vpxor_128\n");
        
        // VPOR 128-bit
        uint32_t exp_or[4];
        for (int i = 0; i < 4; i++) exp_or[i] = a[i] | b[i];
        avx_vpor_128(a, b, out);
        if (!check_vec_u32(out, exp_or, 4)) pass = 0; else printf("PASS vpor_128\n");
        
        // VPSUBD 128-bit
        uint32_t exp_sub[4];
        for (int i = 0; i < 4; i++) exp_sub[i] = a[i] - b[i];
        avx_vpsubd_128(a, b, out);
        if (!check_vec_u32(out, exp_sub, 4)) pass = 0; else printf("PASS vpsubd_128\n");
        
        // VPMULLD 128-bit
        uint32_t exp_mul[4];
        for (int i = 0; i < 4; i++) exp_mul[i] = a[i] * b[i];
        avx_vpmulld_128(a, b, out);
        if (!check_vec_u32(out, exp_mul, 4)) pass = 0; else printf("PASS vpmulld_128\n");
        
        // VPMINSD 128-bit
        int32_t sa[4] = {-10, 50, -100, 200};
        int32_t sb[4] = {20, -30, -50, 150};
        int32_t sout[4];
        int32_t exp_min[4];
        for (int i = 0; i < 4; i++) exp_min[i] = (sa[i] < sb[i]) ? sa[i] : sb[i];
        avx_vpminsd_128(sa, sb, sout);
        if (!check_vec_u32((uint32_t*)sout, (uint32_t*)exp_min, 4)) pass = 0; else printf("PASS vpminsd_128\n");
        
        // VPBROADCASTD 128-bit
        uint32_t broadcast_val = 0x12345678;
        uint32_t exp_broadcast[4] = {broadcast_val, broadcast_val, broadcast_val, broadcast_val};
        avx_vpbroadcastd_128(&broadcast_val, out);
        if (!check_vec_u32(out, exp_broadcast, 4)) pass = 0; else printf("PASS vpbroadcastd_128\n");
        
        // VMOVDQU load/store 128-bit
        uint32_t src[4] = {0xDEADBEEF, 0xCAFEBABE, 0x12345678, 0x87654321};
        uint32_t dst[4] = {0};
        avx_vmovdqu_ld_128(src, dst);
        if (!check_vec_u32(dst, src, 4)) pass = 0; else printf("PASS vmovdqu_ld_128\n");
        
        uint32_t dst2[4] = {0};
        avx_vmovdqu_st_128(src, dst2);
        if (!check_vec_u32(dst2, src, 4)) pass = 0; else printf("PASS vmovdqu_st_128\n");
        
        // VMOVD - GPR to XMM
        uint32_t gpr_val = 0xDEADBEEF;
        uint32_t xmm_result[4] = {0};
        avx_vmovd_xmm_r(gpr_val, xmm_result);
        uint32_t expected_movd[4] = {gpr_val, 0, 0, 0};
        if (!check_vec_u32(xmm_result, expected_movd, 4)) pass = 0; else printf("PASS vmovd_xmm_r\n");
        
        // VMOVD - XMM to GPR
        uint32_t xmm_src[4] = {0x12345678, 0xAABBCCDD, 0x11223344, 0x55667788};
        uint32_t gpr_result = avx_vmovd_r_xmm(xmm_src);
        if (gpr_result != xmm_src[0]) {
            printf("FAIL vmovd_r_xmm: got=%#x exp=%#x\n", gpr_result, xmm_src[0]);
            pass = 0;
        } else {
            printf("PASS vmovd_r_xmm\n");
        }
    }

    // Integer AVX Tests - 256-bit
    {
        uint32_t a[8] = {10, 20, 30, 40, 50, 60, 70, 80};
        uint32_t b[8] = {5, 15, 25, 35, 45, 55, 65, 75};
        uint32_t out[8];
        
        // VPADDD 256-bit
        uint32_t exp_add[8];
        for (int i = 0; i < 8; i++) exp_add[i] = a[i] + b[i];
        avx_vpaddd_256(a, b, out);
        if (!check_vec_u32(out, exp_add, 8)) pass = 0; else printf("PASS vpaddd_256\n");
        
        // VPAND 256-bit
        uint32_t exp_and[8];
        for (int i = 0; i < 8; i++) exp_and[i] = a[i] & b[i];
        avx_vpand_256(a, b, out);
        if (!check_vec_u32(out, exp_and, 8)) pass = 0; else printf("PASS vpand_256\n");
        
        // VPXOR 256-bit
        uint32_t exp_xor[8];
        for (int i = 0; i < 8; i++) exp_xor[i] = a[i] ^ b[i];
        avx_vpxor_256(a, b, out);
        if (!check_vec_u32(out, exp_xor, 8)) pass = 0; else printf("PASS vpxor_256\n");
        
        // VPOR 256-bit
        uint32_t exp_or[8];
        for (int i = 0; i < 8; i++) exp_or[i] = a[i] | b[i];
        avx_vpor_256(a, b, out);
        if (!check_vec_u32(out, exp_or, 8)) pass = 0; else printf("PASS vpor_256\n");
        
        // VPSUBD 256-bit
        uint32_t exp_sub[8];
        for (int i = 0; i < 8; i++) exp_sub[i] = a[i] - b[i];
        avx_vpsubd_256(a, b, out);
        if (!check_vec_u32(out, exp_sub, 8)) pass = 0; else printf("PASS vpsubd_256\n");
        
        // VPMULLD 256-bit
        uint32_t exp_mul[8];
        for (int i = 0; i < 8; i++) exp_mul[i] = a[i] * b[i];
        avx_vpmulld_256(a, b, out);
        if (!check_vec_u32(out, exp_mul, 8)) pass = 0; else printf("PASS vpmulld_256\n");
        
        // VPMINSD 256-bit
        int32_t sa[8] = {-10, 50, -100, 200, -300, 400, -500, 600};
        int32_t sb[8] = {20, -30, -50, 150, -200, 300, -400, 500};
        int32_t sout[8];
        int32_t exp_min[8];
        for (int i = 0; i < 8; i++) exp_min[i] = (sa[i] < sb[i]) ? sa[i] : sb[i];
        avx_vpminsd_256(sa, sb, sout);
        if (!check_vec_u32((uint32_t*)sout, (uint32_t*)exp_min, 8)) pass = 0; else printf("PASS vpminsd_256\n");
        
        // VPBROADCASTD 256-bit
        uint32_t broadcast_val = 0xABCDEF01;
        uint32_t exp_broadcast[8];
        for (int i = 0; i < 8; i++) exp_broadcast[i] = broadcast_val;
        avx_vpbroadcastd_256(&broadcast_val, out);
        if (!check_vec_u32(out, exp_broadcast, 8)) pass = 0; else printf("PASS vpbroadcastd_256\n");
        
        // VMOVDQU load/store 256-bit
        uint32_t src[8] = {0xDEADBEEF, 0xCAFEBABE, 0x12345678, 0x87654321, 
                           0xFEEDFACE, 0x13579BDF, 0x2468ACE0, 0xAABBCCDD};
        uint32_t dst[8] = {0};
        avx_vmovdqu_ld_256(src, dst);
        if (!check_vec_u32(dst, src, 8)) pass = 0; else printf("PASS vmovdqu_ld_256\n");
        
        uint32_t dst2[8] = {0};
        avx_vmovdqu_st_256(src, dst2);
        if (!check_vec_u32(dst2, src, 8)) pass = 0; else printf("PASS vmovdqu_st_256\n");
    }

    if (!pass) {
        printf("\nOverall: FAIL\n");
        return 1;
    }
    printf("\nOverall: PASS\n");
    return 0;
}
