/**
 * SAXPY AVX-256 Kernel
 * 
 * Implements Y = a*X + Y using 256-bit AVX with FMA
 * Processes 8 single-precision floats per iteration
 */

.text
.globl saxpy_avx256
.type saxpy_avx256, @function

saxpy_avx256:
    # Arguments:
    #   rdi = float *y (output array, also input)
    #   rsi = float *x (input array)
    #   xmm0 = float a (scalar multiplier)
    #   rdx = size_t n (number of elements)
    
    # Broadcast scalar 'a' to all lanes of ymm3
    vbroadcastss %xmm0, %ymm3
    
    # Initialize loop counter: i = 0
    xor %rax, %rax
    
.L_loop:
    # Check if we've processed all elements
    cmp %rdx, %rax
    jge .L_done
    
    # Load 8 floats from x[i] into ymm0 (split into two 128-bit loads for gem5)
    vmovups (%rsi,%rax,4), %xmm0
    vmovups 16(%rsi,%rax,4), %xmm1
    vinsertf128 $1, %xmm1, %ymm0, %ymm0
    
    # Load 8 floats from y[i] into ymm1 (split into two 128-bit loads for gem5)
    vmovups (%rdi,%rax,4), %xmm1
    vmovups 16(%rdi,%rax,4), %xmm2
    vinsertf128 $1, %xmm2, %ymm1, %ymm1
    
    # Compute y[i] = a * x[i] + y[i] using FMA
    # ymm1 = ymm0 * ymm3 + ymm1
    vfmadd231ps %ymm0, %ymm3, %ymm1
    
    # Store result back to y[i] (split into two 128-bit stores for gem5)
    vextractf128 $0, %ymm1, %xmm0
    vextractf128 $1, %ymm1, %xmm2
    vmovups %xmm0, (%rdi,%rax,4)
    vmovups %xmm2, 16(%rdi,%rax,4)
    
    # Advance to next 8 elements
    add $8, %rax
    jmp .L_loop
    
.L_done:
    # Clean up AVX state before returning
    vzeroupper
    ret

.size saxpy_avx256, .-saxpy_avx256
