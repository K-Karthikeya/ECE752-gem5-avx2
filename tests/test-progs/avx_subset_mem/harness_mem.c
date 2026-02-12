// Harness for avx_subset_mem_test
// Verifies mixed memory/reg operand handling, overlapping dest semantics, and 128-bit vs 256-bit paths.
// Expected YMM outY[i] = (a[i] + b[i]) * a[i]
// Expected tmp (first 8 floats) = 0.0f
// Expected XMM outX[i] = a[i] + b[i] (since vaddps xmm4 = xmm3 + xmm4)
// And implicit multiply result in xmm3 discarded (not stored) but exercised.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

extern void avx_subset_mem_test(float *a, float *b, float *outY, float *outX, float *tmp);

static uint32_t fbits(float f){ union { float f; uint32_t u; } v; v.f=f; return v.u; }

int main(void) {
    float *a   = (float*)aligned_alloc(32, 32); // 8 floats
    float *b   = (float*)aligned_alloc(32, 32);
    float *outY= (float*)aligned_alloc(32, 32);
    float *outX= (float*)aligned_alloc(32, 16); // 4 floats
    float *tmp = (float*)aligned_alloc(32, 32);
    if (!a||!b||!outY||!outX||!tmp){ fprintf(stderr,"alloc fail\n"); return 2; }

    for(int i=0;i<8;i++){ a[i]=(float)(i+1); b[i]=2.0f*(float)(i+1); }
    for(int i=0;i<8;i++){ outY[i]=-1.0f; tmp[i]=-1.0f; }
    for(int i=0;i<4;i++){ outX[i]=-1.0f; }

    avx_subset_mem_test(a,b,outY,outX,tmp);

    int errors=0;
    // Check YMM results
    for(int i=0;i<8;i++){
        float expectY=(a[i]+b[i])*a[i];
        if(outY[i]!=expectY){
            fprintf(stderr,"YMM mismatch[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n", i,outY[i],fbits(outY[i]),expectY,fbits(expectY));
            errors++;
        }
        if(tmp[i]!=0.0f){
            fprintf(stderr,"TMP mismatch[%d]: got=%f (0x%08" PRIx32 ") expect=0.0 (0x00000000)\n", i,tmp[i],fbits(tmp[i]));
            errors++;
        }
    }
    // Check XMM add results
    for(int i=0;i<4;i++){
        float expectX = a[i]+b[i];
        if(outX[i]!=expectX){
            fprintf(stderr,"XMM mismatch[%d]: got=%f (0x%08" PRIx32 ") expect=%f (0x%08" PRIx32 ")\n", i,outX[i],fbits(outX[i]),expectX,fbits(expectX));
            errors++;
        }
    }

    if(errors){ printf("AVX subset mem test FAIL (%d errors)\n", errors); }
    else { printf("AVX subset mem test PASS\n"); }

    free(a); free(b); free(outY); free(outX); free(tmp);
    return errors?1:0;
}
