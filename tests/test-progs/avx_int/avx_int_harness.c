#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Forward declarations for assembly functions
extern void avx_vpaddd_128(int32_t *a, int32_t *b, int32_t *result);
extern void avx_vpaddd_256(int32_t *a, int32_t *b, int32_t *result);
extern void avx_vpandd_128(int32_t *a, int32_t *b, int32_t *result);
extern void avx_vpandd_256(int32_t *a, int32_t *b, int32_t *result);
extern void avx_vpminsd_128(int32_t *a, int32_t *b, int32_t *result);
extern void avx_vpminsd_256(int32_t *a, int32_t *b, int32_t *result);
extern void avx_vpminsq_128(int64_t *a, int64_t *b, int64_t *result);
extern void avx_vpminsq_256(int64_t *a, int64_t *b, int64_t *result);
extern void avx_vpbroadcastd_128(int32_t *src, int32_t *result);
extern void avx_vpbroadcastd_256(int32_t *src, int32_t *result);
extern void avx_vmovdqu_load_128(int32_t *src, int32_t *result);
extern void avx_vmovdqu_load_256(int32_t *src, int32_t *result);
extern void avx_vmovdqu_store_128(int32_t *src, int32_t *result);
extern void avx_vmovdqu_store_256(int32_t *src, int32_t *result);

#define ALIGN32 __attribute__((aligned(32)))

int test_count = 0;
int pass_count = 0;

void print_i32_array(const char *label, int32_t *arr, int count) {
    printf("%s: ", label);
    for (int i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_i64_array(const char *label, int64_t *arr, int count) {
    printf("%s: ", label);
    for (int i = 0; i < count; i++) {
        printf("%lld ", (long long)arr[i]);
    }
    printf("\n");
}

int compare_i32(int32_t *expected, int32_t *actual, int count, const char *test_name) {
    test_count++;
    for (int i = 0; i < count; i++) {
        if (expected[i] != actual[i]) {
            printf("FAIL: %s - Mismatch at index %d: expected %d, got %d\n", 
                   test_name, i, expected[i], actual[i]);
            print_i32_array("  Expected", expected, count);
            print_i32_array("  Actual", actual, count);
            return 0;
        }
    }
    printf("PASS: %s\n", test_name);
    pass_count++;
    return 1;
}

int compare_i64(int64_t *expected, int64_t *actual, int count, const char *test_name) {
    test_count++;
    for (int i = 0; i < count; i++) {
        if (expected[i] != actual[i]) {
            printf("FAIL: %s - Mismatch at index %lld: expected %lld, got %lld\n", 
                   test_name, (long long)i, (long long)expected[i], (long long)actual[i]);
            print_i64_array("  Expected", expected, count);
            print_i64_array("  Actual", actual, count);
            return 0;
        }
    }
    printf("PASS: %s\n", test_name);
    pass_count++;
    return 1;
}

void test_vpaddd_128() {
    ALIGN32 int32_t a[4] = {1, 2, 3, 4};
    ALIGN32 int32_t b[4] = {10, 20, 30, 40};
    ALIGN32 int32_t result[4] = {0};
    ALIGN32 int32_t expected[4] = {11, 22, 33, 44};
    
    avx_vpaddd_128(a, b, result);
    compare_i32(expected, result, 4, "VPADDD 128-bit");
}

void test_vpaddd_256() {
    ALIGN32 int32_t a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    ALIGN32 int32_t b[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    ALIGN32 int32_t result[8] = {0};
    ALIGN32 int32_t expected[8] = {11, 22, 33, 44, 55, 66, 77, 88};
    
    avx_vpaddd_256(a, b, result);
    compare_i32(expected, result, 8, "VPADDD 256-bit");
}

void test_vpandd_128() {
    ALIGN32 int32_t a[4] = {0xFF, 0xF0, 0x0F, 0xAA};
    ALIGN32 int32_t b[4] = {0x0F, 0xFF, 0xFF, 0x55};
    ALIGN32 int32_t result[4] = {0};
    ALIGN32 int32_t expected[4] = {0x0F, 0xF0, 0x0F, 0x00};
    
    avx_vpandd_128(a, b, result);
    compare_i32(expected, result, 4, "VPANDD 128-bit");
}

void test_vpandd_256() {
    ALIGN32 int32_t a[8] = {0xFF, 0xF0, 0x0F, 0xAA, 0xFF00, 0x00FF, 0xFFFF, 0x0000};
    ALIGN32 int32_t b[8] = {0x0F, 0xFF, 0xFF, 0x55, 0x00FF, 0xFF00, 0x0F0F, 0xFFFF};
    ALIGN32 int32_t result[8] = {0};
    ALIGN32 int32_t expected[8] = {0x0F, 0xF0, 0x0F, 0x00, 0x0000, 0x0000, 0x0F0F, 0x0000};
    
    avx_vpandd_256(a, b, result);
    compare_i32(expected, result, 8, "VPANDD 256-bit");
}

void test_vpminsd_128() {
    ALIGN32 int32_t a[4] = {10, -5, 100, -200};
    ALIGN32 int32_t b[4] = {20, -10, 50, -100};
    ALIGN32 int32_t result[4] = {0};
    ALIGN32 int32_t expected[4] = {10, -10, 50, -200};
    
    avx_vpminsd_128(a, b, result);
    compare_i32(expected, result, 4, "VPMINSD 128-bit");
}

void test_vpminsd_256() {
    ALIGN32 int32_t a[8] = {10, -5, 100, -200, 1000, -1000, 5, -5};
    ALIGN32 int32_t b[8] = {20, -10, 50, -100, 500, -500, 10, -10};
    ALIGN32 int32_t result[8] = {0};
    ALIGN32 int32_t expected[8] = {10, -10, 50, -200, 500, -1000, 5, -10};
    
    avx_vpminsd_256(a, b, result);
    compare_i32(expected, result, 8, "VPMINSD 256-bit");
}

void test_vpminsq_128() {
    ALIGN32 int64_t a[2] = {1000000000000LL, -2000000000000LL};
    ALIGN32 int64_t b[2] = {2000000000000LL, -1000000000000LL};
    ALIGN32 int64_t result[2] = {0};
    ALIGN32 int64_t expected[2] = {1000000000000LL, -2000000000000LL};
    
    avx_vpminsq_128(a, b, result);
    compare_i64(expected, result, 2, "VPMINSQ 128-bit");
}

void test_vpminsq_256() {
    ALIGN32 int64_t a[4] = {1000000000000LL, -2000000000000LL, 5000000000000LL, -500000000000LL};
    ALIGN32 int64_t b[4] = {2000000000000LL, -1000000000000LL, 3000000000000LL, -1000000000000LL};
    ALIGN32 int64_t result[4] = {0};
    ALIGN32 int64_t expected[4] = {1000000000000LL, -2000000000000LL, 3000000000000LL, -1000000000000LL};
    
    avx_vpminsq_256(a, b, result);
    compare_i64(expected, result, 4, "VPMINSQ 256-bit");
}

void test_vpbroadcastd_128() {
    ALIGN32 int32_t src[4] = {42, 0, 0, 0};
    ALIGN32 int32_t result[4] = {0};
    ALIGN32 int32_t expected[4] = {42, 42, 42, 42};
    
    avx_vpbroadcastd_128(src, result);
    compare_i32(expected, result, 4, "VPBROADCASTD 128-bit");
}

void test_vpbroadcastd_256() {
    ALIGN32 int32_t src[8] = {99, 0, 0, 0, 0, 0, 0, 0};
    ALIGN32 int32_t result[8] = {0};
    ALIGN32 int32_t expected[8] = {99, 99, 99, 99, 99, 99, 99, 99};
    
    avx_vpbroadcastd_256(src, result);
    compare_i32(expected, result, 8, "VPBROADCASTD 256-bit");
}

void test_vmovdqu_128() {
    ALIGN32 int32_t src[4] = {11, 22, 33, 44};
    ALIGN32 int32_t result[4] = {0};
    
    avx_vmovdqu_load_128(src, result);
    compare_i32(src, result, 4, "VMOVDQU load 128-bit");
    
    ALIGN32 int32_t store_result[4] = {0};
    avx_vmovdqu_store_128(result, store_result);
    compare_i32(src, store_result, 4, "VMOVDQU store 128-bit");
}

void test_vmovdqu_256() {
    ALIGN32 int32_t src[8] = {11, 22, 33, 44, 55, 66, 77, 88};
    ALIGN32 int32_t result[8] = {0};
    
    avx_vmovdqu_load_256(src, result);
    compare_i32(src, result, 8, "VMOVDQU load 256-bit");
    
    ALIGN32 int32_t store_result[8] = {0};
    avx_vmovdqu_store_256(result, store_result);
    compare_i32(src, store_result, 8, "VMOVDQU store 256-bit");
}

int main() {
    printf("=== AVX Integer Instruction Tests ===\n\n");
    
    test_vpaddd_128();
    test_vpaddd_256();
    test_vpandd_128();
    test_vpandd_256();
    test_vpminsd_128();
    test_vpminsd_256();
    test_vpminsq_128();
    test_vpminsq_256();
    test_vpbroadcastd_128();
    test_vpbroadcastd_256();
    test_vmovdqu_128();
    test_vmovdqu_256();
    
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", test_count);
    printf("Passed: %d\n", pass_count);
    printf("Failed: %d\n", test_count - pass_count);
    
    return (test_count == pass_count) ? 0 : 1;
}
