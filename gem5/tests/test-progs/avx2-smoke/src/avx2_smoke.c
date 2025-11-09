#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Helper to compare float arrays with exact match (bitwise) to validate VXORPS
static int cmp_u32(const uint32_t *a, const uint32_t *b, int n)
{
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) return i + 1; // return 1-based index on mismatch
    }
    return 0;
}

static int cmp_f32_tol(const float *a, const float *b, int n, float tol)
{
    for (int i = 0; i < n; ++i) {
        float diff = a[i] - b[i];
        if (diff < 0) diff = -diff;
        if (diff > tol) return i + 1; // 1-based index on mismatch
    }
    return 0;
}

int main()
{
    // Use aligned and unaligned buffers to exercise VMOVAPS and VMOVUPS
    alignas(32) float A_aligned[8] = {1.0f, 2.0f, 3.5f, -4.0f, 0.5f, -0.5f, 10.0f, -8.0f};
    alignas(32) float B_aligned[8] = {5.0f, -2.0f, 1.5f,  4.0f, 2.0f,  3.0f, -1.0f,  8.0f};
    alignas(32) float C_add[8];
    alignas(32) float D_mul[8];
    alignas(32) float E_xor[8];

    // Unaligned views: offset by 4 bytes
    uint8_t *A_ua_bytes = (uint8_t *)A_aligned;
    uint8_t *B_ua_bytes = (uint8_t *)B_aligned;
    float *A_unaligned = (float *)(A_ua_bytes + 4);
    float *B_unaligned = (float *)(B_ua_bytes + 4);

    // Load (both unaligned and aligned)
    __m256 va_u = _mm256_loadu_ps(A_unaligned); // VMOVUPS load
    __m256 vb_u = _mm256_loadu_ps(B_unaligned);

    __m256 va_a = _mm256_load_ps(A_aligned);    // VMOVAPS load
    __m256 vb_a = _mm256_load_ps(B_aligned);

    // Do arithmetic: VADDPS, VMULPS, VXORPS
    __m256 vadd = _mm256_add_ps(va_a, vb_a);
    __m256 vmul = _mm256_mul_ps(va_u, vb_u);
    __m256 vxor = _mm256_xor_ps(va_a, vb_u);

    // Store (aligned and unaligned): VMOVAPS, VMOVUPS
    _mm256_store_ps(C_add, vadd);                // aligned store
    _mm256_storeu_ps(D_mul, vmul);               // unaligned store
    _mm256_store_ps(E_xor, vxor);                // aligned store

    // Scalar reference results
    float C_ref[8];
    float D_ref[8];
    uint32_t E_ref_u32[8];

    // Compute refs using the same data views we used for loads
    for (int i = 0; i < 8; ++i) {
        // For vmul we used unaligned loads, so reference should read from A_unaligned/B_unaligned
        C_ref[i] = A_aligned[i] + B_aligned[i];
        D_ref[i] = A_unaligned[i] * B_unaligned[i];
        // bitwise XOR on float payloads
        union { float f; uint32_t u; } ua, ub;
        ua.f = A_aligned[i];
        ub.f = B_unaligned[i];
        E_ref_u32[i] = ua.u ^ ub.u;
    }

    // Validate
    int err = 0;
    err = cmp_f32_tol(C_add, C_ref, 8, 1e-6f);
    if (err) { printf("VADDPS mismatch at lane %d\n", err); return 1; }
    err = cmp_f32_tol(D_mul, D_ref, 8, 1e-6f);
    if (err) { printf("VMULPS mismatch at lane %d\n", err); return 2; }

    // For XOR, compare bitwise
    uint32_t E_got_u32[8];
    memcpy(E_got_u32, E_xor, sizeof(E_got_u32));
    err = cmp_u32(E_got_u32, E_ref_u32, 8);
    if (err) { printf("VXORPS mismatch at lane %d\n", err); return 3; }

    // Exercise VZEROUPPER explicitly
    _mm256_zeroupper();

    // If we got here, all checks passed
    printf("AVX2 smoke test passed.\n");
    return 0;
}
