#include <stdio.h>
#include <immintrin.h>

// Test FMA instruction directly
void test_fma_intrinsic() {
    float a = 2.5f;
    float x[8] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
    float y[8] = {0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.2f, 1.4f, 1.6f};
    float result[8];
    
    __m256 va = _mm256_set1_ps(a);
    __m256 vx = _mm256_loadu_ps(x);
    __m256 vy = _mm256_loadu_ps(y);
    
    // FMA: result = y + (a * x)
    __m256 vresult = _mm256_fmadd_ps(va, vx, vy);
    _mm256_storeu_ps(result, vresult);
    
    printf("FMA Intrinsic Test:\n");
    printf("a = %f\n", a);
    for (int i = 0; i < 8; i++) {
        float expected = a * x[i] + y[i];
        printf("[%d] x=%f y=%f result=%f expected=%f %s\n",
               i, x[i], y[i], result[i], expected,
               (result[i] == expected) ? "PASS" : "FAIL");
    }
}

// Test with inline assembly
extern void test_fma_asm(float a, const float *x, const float *y, float *result);

__asm__(
".text\n"
".globl test_fma_asm\n"
".type test_fma_asm, @function\n"
"test_fma_asm:\n"
"    # Args: xmm0=a, rdi=x, rsi=y, rdx=result\n"
"    vbroadcastss %xmm0, %ymm3\n"
"    vmovups (%rdi), %ymm0\n"
"    vmovups (%rsi), %ymm1\n"
"    vfmadd231ps %ymm0, %ymm3, %ymm1\n"
"    vmovups %ymm1, (%rdx)\n"
"    vzeroupper\n"
"    ret\n"
);

void test_fma_assembly() {
    float a = 2.5f;
    float x[8] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f};
    float y[8] = {0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.2f, 1.4f, 1.6f};
    float result[8];
    
    test_fma_asm(a, x, y, result);
    
    printf("\nFMA Assembly Test:\n");
    printf("a = %f\n", a);
    for (int i = 0; i < 8; i++) {
        float expected = a * x[i] + y[i];
        printf("[%d] x=%f y=%f result=%f expected=%f %s\n",
               i, x[i], y[i], result[i], expected,
               (result[i] == expected) ? "PASS" : "FAIL");
    }
}

int main() {
    test_fma_intrinsic();
    test_fma_assembly();
    return 0;
}
