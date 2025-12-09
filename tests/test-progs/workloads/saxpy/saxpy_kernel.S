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
    
    # Load 8 floats from x[i] into ymm0
    vmovups (%rsi,%rax,4), %ymm0
    
    # Load 8 floats from y[i] into ymm1
    vmovups (%rdi,%rax,4), %ymm1
    
    # Compute y[i] = a * x[i] + y[i] using FMA
    # ymm1 = ymm0 * ymm3 + ymm1
    vfmadd231ps %ymm0, %ymm3, %ymm1
    
    # Store result back to y[i]
    vmovups %ymm1, (%rdi,%rax,4)
    
    # Advance to next 8 elements
    add $8, %rax
    jmp .L_loop
    
.L_done:
    # Clean up AVX state before returning
    vzeroupper
    ret

.size saxpy_avx256, .-saxpy_avx256
