#include <stdio.h>
#include <string.h>

extern void saxpy_test_kernel(float *y, const float *x, float a, size_t n);

__asm__(
".text\n"
".globl saxpy_test_kernel\n"
".type saxpy_test_kernel, @function\n"
"saxpy_test_kernel:\n"
"    # rdi=y, rsi=x, xmm0=a, rcx=n\n"
"    vbroadcastss %xmm0, %ymm3\n"
"    xor %rax, %rax\n"
".L_loop:\n"
"    cmp %rcx, %rax\n"
"    jge .L_done\n"
"    vmovups (%rsi,%rax,4), %ymm0\n"
"    vmovups (%rdi,%rax,4), %ymm1\n"
"    vfmadd231ps %ymm0, %ymm3, %ymm1\n"
"    vmovups %ymm1, (%rdi,%rax,4)\n"
"    add $8, %rax\n"
"    jmp .L_loop\n"
".L_done:\n"
"    vzeroupper\n"
"    ret\n"
);

int main() {
    size_t n = 16;
    float x[16], y_ref[16], y_test[16];
    float a = 2.5f;
    
    // Initialize like SAXPY does
    for (size_t i = 0; i < n; i++) {
        x[i] = (float)(i % 100) / 10.0f;
        y_ref[i] = (float)(i % 50) / 5.0f;
    }
    memcpy(y_test, y_ref, n * sizeof(float));
    
    // Reference calculation
    for (size_t i = 0; i < n; i++) {
        y_ref[i] = a * x[i] + y_ref[i];
    }
    
    // Test kernel
    saxpy_test_kernel(y_test, x, a, n);
    
    printf("SAXPY Loop Test:\n");
    printf("a = %f, n = %zu\n\n", a, n);
    
    int errors = 0;
    for (size_t i = 0; i < n; i++) {
        float diff = y_ref[i] - y_test[i];
        if (diff < 0) diff = -diff;
        if (diff > 1e-5f) {
            printf("[%zu] x=%f y_orig=%f ref=%f test=%f FAIL\n",
                   i, x[i], (float)(i % 50) / 5.0f, y_ref[i], y_test[i]);
            errors++;
        } else {
            printf("[%zu] x=%f y_orig=%f ref=%f test=%f PASS\n",
                   i, x[i], (float)(i % 50) / 5.0f, y_ref[i], y_test[i]);
        }
    }
    
    printf("\n%s: %d/%zu tests passed\n", errors ? "FAIL" : "PASS", (int)(n - errors), n);
    return errors ? 1 : 0;
}
