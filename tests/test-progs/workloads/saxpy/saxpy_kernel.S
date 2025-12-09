/**
 * SAXPY AVX-256 Kernel
 * 
 * Implements Y = a*X + Y using 256-bit AVX with FMA
 * Processes 8 single-precision floats per iteration
 * 
 * Uses ONLY tested instructions: VMOVUPS_256, VBROADCASTSS_256, VFMADD231PS_256
 */

.intel_syntax noprefix
.text
.globl saxpy_avx256
.type saxpy_avx256, @function

saxpy_avx256:
    # Arguments (SysV AMD64 calling convention):
    #   rdi = float *y (output array, also input)
    #   rsi = float *x (input array)
    #   xmm0 = float a (scalar multiplier)
    #   rdx = size_t n (number of elements)
    
    push rbp
    mov rbp, rsp
    
    # Broadcast scalar 'a' to all 8 lanes of ymm3
    vbroadcastss ymm3, xmm0
    
    # Initialize loop counter: i = 0
    xor rax, rax
    
.L_loop:
    # Check if we've processed all elements (rax >= rdx means done)
    cmp rax, rdx
    jge .L_done
    
    # Load 8 floats from x[i] into ymm0 (32-byte load)
    vmovups ymm0, YMMWORD PTR [rsi + rax*4]
    
    # Load 8 floats from y[i] into ymm1 (32-byte load)
    vmovups ymm1, YMMWORD PTR [rdi + rax*4]
    
    # Compute y[i] = a * x[i] + y[i] using FMA
    # VFMADD231PS: ymm1 = (ymm3 * ymm0) + ymm1 = (a * x) + y
    vfmadd231ps ymm1, ymm3, ymm0
    
    # Store result back to y[i] (32-byte store)
    vmovups YMMWORD PTR [rdi + rax*4], ymm1
    
    # Advance to next 8 elements
    add rax, 8
    jmp .L_loop
    
.L_done:
    # Clean up AVX state before returning
    vzeroupper
    pop rbp
    ret

.size saxpy_avx256, .-saxpy_avx256

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
