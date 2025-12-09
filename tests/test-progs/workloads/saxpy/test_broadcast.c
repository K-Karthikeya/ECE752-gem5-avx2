#include <stdio.h>
#include <immintrin.h>

int main() {
    float scalar = 2.5f;
    __m256 result = _mm256_broadcast_ss(&scalar);
    
    float output[8];
    _mm256_storeu_ps(output, result);
    
    printf("Broadcast test:\n");
    printf("Input scalar: %f\n", scalar);
    printf("Output vector: ");
    for (int i = 0; i < 8; i++) {
        printf("%f ", output[i]);
    }
    printf("\n");
    
    // Check if all values are 2.5
    int success = 1;
    for (int i = 0; i < 8; i++) {
        if (output[i] != 2.5f) {
            success = 0;
            break;
        }
    }
    
    if (success) {
        printf("PASS: All lanes contain 2.5\n");
    } else {
        printf("FAIL: Broadcast did not work correctly\n");
    }
    
    return success ? 0 : 1;
}
