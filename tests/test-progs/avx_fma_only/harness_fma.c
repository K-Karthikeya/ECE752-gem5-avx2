// Harness for FMA-only test: VFMADD231PS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

extern void avx_fma_test(float *a, float *b, float *c, float *out);

static uint32_t bits(float f){ union { float f; uint32_t u; } v; v.f=f; return v.u; }

int main(void){
    const int N = 8; // 256-bit YMM holds 8 floats
    float *a   = (float*)aligned_alloc(32, N*sizeof(float));
    float *b   = (float*)aligned_alloc(32, N*sizeof(float));
    float *c   = (float*)aligned_alloc(32, N*sizeof(float));
    float *out = (float*)aligned_alloc(32, N*sizeof(float));
    if(!a||!b||!c||!out){ fprintf(stderr,"alloc fail\n"); return 2; }
    for(int i=0;i<N;i++){ a[i]=(float)(i+1); b[i]=2.0f*(float)(i+1); c[i]=3.0f*(float)(i+1); out[i]=-1.0f; }

    avx_fma_test(a,b,c,out);

    int errs=0; for(int i=0;i<N;i++){ float exp = c[i]*b[i]+a[i]; if(fabsf(out[i]-exp)>1e-5f){
            fprintf(stderr,"FMA mismatch lane %d: got=%f (0x%08" PRIx32 ") exp=%f (0x%08" PRIx32 ")\n", i,out[i],bits(out[i]),exp,bits(exp)); errs++; }
    }

    if(errs){ printf("FMA ONLY TEST FAIL (%d errors)\n", errs); } else { printf("FMA ONLY TEST PASS\n"); }
    free(a); free(b); free(c); free(out); return errs?1:0; }
