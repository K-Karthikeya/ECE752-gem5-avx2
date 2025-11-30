#include <immintrin.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Simple, focused unit-style checks for individual AVX/AVX2 ops.
// We'll test a minimal subset used by our decoder: VADDPS, VMULPS,
// VXORPS, VMOVUPS/VMOVAPS (load+store), and VZEROUPPER.

static int cmp_u32(const uint32_t *a, const uint32_t *b, int n)
{
    for (int i = 0; i < n; ++i) if (a[i] != b[i]) return i + 1;
    return 0;
}

static int cmp_f32_tol(const float *a, const float *b, int n, float tol)
{
    for (int i = 0; i < n; ++i) {
        float d = a[i] - b[i];
        if (d < 0) d = -d;
        if (d > tol) return i + 1;
    }
    return 0;
}

static int test_vaddps()
{
    alignas(32) float a[8] = {1,2,3,4,5,6,7,8};
    alignas(32) float b[8] = {8,7,6,5,4,3,2,1};
    alignas(32) float got[8];
    float ref[8];
    for (int i = 0; i < 8; ++i) ref[i] = a[i] + b[i];

    __m256 va = _mm256_load_ps(a);
    __m256 vb = _mm256_load_ps(b);
    __m256 vc = _mm256_add_ps(va, vb); // VADDPS
    _mm256_store_ps(got, vc);

    int e = cmp_f32_tol(got, ref, 8, 1e-6f);
    if (e) { printf("vaddps fail lane %d\n", e); return 1; }
    return 0;
}

static int test_vmulps()
{
    alignas(32) float a[8] = {1,2,3,4,5,6,7,8};
    alignas(32) float b[8] = {0.5f,2,3,0.25f,1,0.5f,-1,2};
    alignas(32) float got[8];
    float ref[8];
    for (int i = 0; i < 8; ++i) ref[i] = a[i] * b[i];

    __m256 va = _mm256_load_ps(a);
    __m256 vb = _mm256_load_ps(b);
    __m256 vc = _mm256_mul_ps(va, vb); // VMULPS
    _mm256_store_ps(got, vc);

    int e = cmp_f32_tol(got, ref, 8, 1e-6f);
    if (e) { printf("vmulps fail lane %d\n", e); return 2; }
    return 0;
}

static int test_vxorps()
{
    alignas(32) float a[8] = {1,2,3,4,5,6,7,8};
    alignas(32) float b[8] = {8,7,6,5,4,3,2,1};
    alignas(32) float got[8];
    uint32_t got_u32[8], ref_u32[8];

    __m256 va = _mm256_load_ps(a);
    __m256 vb = _mm256_load_ps(b);
    __m256 vc = _mm256_xor_ps(va, vb); // VXORPS (bitwise XOR on payload)
    _mm256_store_ps(got, vc);
    memcpy(got_u32, got, sizeof(got_u32));

    for (int i = 0; i < 8; ++i) {
        union { float f; uint32_t u; } ua, ub;
        ua.f = a[i]; ub.f = b[i];
        ref_u32[i] = ua.u ^ ub.u;
    }

    int e = cmp_u32(got_u32, ref_u32, 8);
    if (e) { printf("vxorps fail lane %d\n", e); return 3; }
    return 0;
}

static int test_vmovs()
{
    // Unaligned load and store (VMOVUPS), and aligned (VMOVAPS)
    alignas(32) uint8_t raw[64];
    for (int i = 0; i < 64; ++i) raw[i] = (uint8_t)(i * 3 + 1);

    float *a = (float *)raw;           // aligned at 32
    float *u = (float *)(raw + 4);     // intentionally unaligned

    alignas(32) float out_a[8];
    alignas(32) float out_u[8];

    __m256 va = _mm256_load_ps(a);     // VMOVAPS load
    __m256 vu = _mm256_loadu_ps(u);    // VMOVUPS load (unaligned)

    _mm256_store_ps(out_a, va);        // aligned store
    _mm256_storeu_ps(out_u, vu);       // unaligned store

    // Validate bitwise
    if (memcmp(out_a, a, 32) != 0) { printf("vmovaps mismatch\n"); return 4; }
    if (memcmp(out_u, u, 32) != 0) { printf("vmovups mismatch\n"); return 5; }
    return 0;
}

static int test_vzeroupper()
{
    // There isn't a direct way to observe upper halves other than ABI
    // behavior; we ensure the intrinsic executes without crashing.
    _mm256_zeroupper();
    return 0;
}

int main()
{
    int rc = 0;
    if ((rc = test_vaddps())) return rc;
    if ((rc = test_vmulps())) return rc;
    if ((rc = test_vxorps())) return rc;
    if ((rc = test_vmovs())) return rc;
    if ((rc = test_vzeroupper())) return rc;
    printf("avx-mini passed.\n");
    return 0;
}
