//===========================================================================
// AVX Instruction Trace Test Harness
// Tests all implemented AVX instructions and validates results
//===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// External assembly function
extern void avx_trace_test(float *a, float *b, float *out);

// Tolerance for floating-point comparison
#define EPSILON 0.0001f

// Helper to compare floats
int float_equal(float a, float b) {
    return fabsf(a - b) < EPSILON;
}

// Print a vector
void print_vector(const char *label, float *vec, int count) {
    printf("%s: ", label);
    for (int i = 0; i < count; i++) {
        printf("%.2f ", vec[i]);
    }
    printf("\n");
}

int main() {
    // Aligned input vectors
    float a[8] __attribute__((aligned(32))) = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    float b[8] __attribute__((aligned(32))) = {8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    float out[104] __attribute__((aligned(32)));
    
    memset(out, 0, sizeof(out));

    printf("=== AVX Instruction Trace Test ===\n\n");
    printf("Input vectors:\n");
    print_vector("a", a, 8);
    print_vector("b", b, 8);
    printf("\n");

    // Run the assembly test
    avx_trace_test(a, b, out);

    printf("=== Test Results ===\n\n");

    // Test 1: VMOVUPS
    printf("1. VMOVUPS (load a):\n");
    print_vector("   Result", &out[0], 8);
    int vmovups_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[i], a[i])) vmovups_ok = 0;
    }
    printf("   Status: %s\n\n", vmovups_ok ? "PASS" : "FAIL");

    // Test 2: VMOVAPS
    printf("2. VMOVAPS (load b):\n");
    print_vector("   Result", &out[8], 8);
    int vmovaps_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[8+i], b[i])) vmovaps_ok = 0;
    }
    printf("   Status: %s\n\n", vmovaps_ok ? "PASS" : "FAIL");

    // Test 3: VMINPS
    printf("3. VMINPS(a,b) reg-reg:\n");
    print_vector("   Result", &out[16], 8);
    int vminps_ok = 1;
    for (int i = 0; i < 8; i++) {
        float expected = fminf(a[i], b[i]);
        if (!float_equal(out[16+i], expected)) vminps_ok = 0;
    }
    printf("   Status: %s\n\n", vminps_ok ? "PASS" : "FAIL");

    // Test 4: VMAXPS
    printf("4. VMAXPS(a,b) reg-reg:\n");
    print_vector("   Result", &out[24], 8);
    int vmaxps_ok = 1;
    for (int i = 0; i < 8; i++) {
        float expected = fmaxf(a[i], b[i]);
        if (!float_equal(out[24+i], expected)) vmaxps_ok = 0;
    }
    printf("   Status: %s\n\n", vmaxps_ok ? "PASS" : "FAIL");

    // Test 5: VMOVSLDUP
    printf("5. VMOVSLDUP(a) reg-reg:\n");
    print_vector("   Result", &out[32], 8);
    float sldup_expected[8] = {a[0], a[0], a[2], a[2], a[4], a[4], a[6], a[6]};
    int vmovsldup_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[32+i], sldup_expected[i])) vmovsldup_ok = 0;
    }
    printf("   Status: %s\n\n", vmovsldup_ok ? "PASS" : "FAIL");

    // Test 6: VMOVSHDUP
    printf("6. VMOVSHDUP(b) reg-reg:\n");
    print_vector("   Result", &out[40], 8);
    float shdup_expected[8] = {b[1], b[1], b[3], b[3], b[5], b[5], b[7], b[7]};
    int vmovshdup_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[40+i], shdup_expected[i])) vmovshdup_ok = 0;
    }
    printf("   Status: %s\n\n", vmovshdup_ok ? "PASS" : "FAIL");

    // Test 7: VBROADCASTSS
    printf("7. VBROADCASTSS(a[0]) reg-mem:\n");
    print_vector("   Result", &out[48], 8);
    int vbroadcast_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[48+i], a[0])) vbroadcast_ok = 0;
    }
    printf("   Status: %s\n\n", vbroadcast_ok ? "PASS" : "FAIL");

    // Test 8: VMINPS reg-mem
    printf("8. VMINPS(a,b) reg-mem:\n");
    print_vector("   Result", &out[56], 8);
    int vminps_mem_ok = 1;
    for (int i = 0; i < 8; i++) {
        float expected = fminf(a[i], b[i]);
        if (!float_equal(out[56+i], expected)) vminps_mem_ok = 0;
    }
    printf("   Status: %s\n\n", vminps_mem_ok ? "PASS" : "FAIL");

    // Test 9: VMAXPS reg-mem
    printf("9. VMAXPS(a,b) reg-mem:\n");
    print_vector("   Result", &out[64], 8);
    int vmaxps_mem_ok = 1;
    for (int i = 0; i < 8; i++) {
        float expected = fmaxf(a[i], b[i]);
        if (!float_equal(out[64+i], expected)) vmaxps_mem_ok = 0;
    }
    printf("   Status: %s\n\n", vmaxps_mem_ok ? "PASS" : "FAIL");

    // Test 10: VMOVSLDUP reg-mem
    printf("10. VMOVSLDUP(a) reg-mem:\n");
    print_vector("    Result", &out[72], 8);
    int vmovsldup_mem_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[72+i], sldup_expected[i])) vmovsldup_mem_ok = 0;
    }
    printf("    Status: %s\n\n", vmovsldup_mem_ok ? "PASS" : "FAIL");

    // Test 11: VMOVSHDUP reg-mem
    printf("11. VMOVSHDUP(b) reg-mem:\n");
    print_vector("    Result", &out[80], 8);
    int vmovshdup_mem_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[80+i], shdup_expected[i])) vmovshdup_mem_ok = 0;
    }
    printf("    Status: %s\n\n", vmovshdup_mem_ok ? "PASS" : "FAIL");

    // Test 12: VBROADCASTSS a[1]
    printf("12. VBROADCASTSS(a[1]) reg-mem:\n");
    print_vector("    Result", &out[88], 8);
    int vbroadcast2_ok = 1;
    for (int i = 0; i < 8; i++) {
        if (!float_equal(out[88+i], a[1])) vbroadcast2_ok = 0;
    }
    printf("    Status: %s\n\n", vbroadcast2_ok ? "PASS" : "FAIL");

    // Test 13: VZEROUPPER
    printf("13. VZEROUPPER (upper lanes should be zero):\n");
    print_vector("    Result", &out[96], 8);
    printf("    Note: First 4 floats should match a[0..3], last 4 should be 0.0\n");
    int vzeroupper_ok = 1;
    for (int i = 0; i < 4; i++) {
        if (!float_equal(out[96+i], a[i])) vzeroupper_ok = 0;
    }
    for (int i = 4; i < 8; i++) {
        if (!float_equal(out[96+i], 0.0f)) vzeroupper_ok = 0;
    }
    printf("    Status: %s\n\n", vzeroupper_ok ? "PASS" : "FAIL");

    // Summary
    int total = 13;
    int passed = vmovups_ok + vmovaps_ok + vminps_ok + vmaxps_ok + 
                 vmovsldup_ok + vmovshdup_ok + vbroadcast_ok +
                 vminps_mem_ok + vmaxps_mem_ok + vmovsldup_mem_ok +
                 vmovshdup_mem_ok + vbroadcast2_ok + vzeroupper_ok;
    
    printf("=== Summary ===\n");
    printf("Passed: %d/%d tests\n", passed, total);
    
    return (passed == total) ? 0 : 1;
}
