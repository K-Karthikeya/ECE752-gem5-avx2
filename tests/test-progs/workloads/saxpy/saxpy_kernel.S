/**
 * SAXPY AVX-256 Kernel
 * 
 * Implements Y = a*X + Y using 256-bit AVX with FMA
 * Processes 8 single-precision floats per iteration
 * 
 * Uses Intel syntax to match tested AVX implementation
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
    
    # Broadcast scalar 'a' to all lanes of ymm3
    vbroadcastss ymm3, xmm0
    
    # Initialize loop counter: i = 0
    xor rax, rax
    
.L_loop:
    # Check if we've processed all elements (rax >= rdx means done)
    cmp rax, rdx
    jge .L_done
    
    # Calculate byte offset: rax * 4 (since each float is 4 bytes)
    # Load 8 floats from x[i] into ymm0
    lea r8, [rsi + rax*4]
    vmovups ymm0, YMMWORD PTR [r8]
    
    # Load 8 floats from y[i] into ymm1
    lea r9, [rdi + rax*4]
    vmovups ymm1, YMMWORD PTR [r9]
    
    # Compute y[i] = a * x[i] + y[i] using FMA
    # VFMADD231PS: ymm1 = (ymm3 * ymm0) + ymm1 = (a * x) + y
    vfmadd231ps ymm1, ymm3, ymm0
    
    # Store result back to y[i]
    vmovups YMMWORD PTR [r9], ymm1
    
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
