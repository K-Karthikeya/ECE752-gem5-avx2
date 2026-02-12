/**
 * SAXPY SSE Kernel
 * 
 * Implements Y = a*X + Y using 128-bit SSE (no FMA available)
 * Processes 4 single-precision floats per iteration
 */

.intel_syntax noprefix
.text
.globl saxpy_sse
.type saxpy_sse, @function

saxpy_sse:
    # Arguments (SysV AMD64 calling convention):
    #   rdi = float *y (output array, also input)
    #   rsi = float *x (input array)
    #   xmm0 = float a (scalar multiplier)
    #   rdx = size_t n (number of elements)
    
    push rbp
    mov rbp, rsp
    
    # Broadcast scalar 'a' to all 4 lanes of xmm3
    shufps xmm0, xmm0, 0    # xmm3 = [a, a, a, a]
    movaps xmm3, xmm0
    
    # Use pointers instead of index for simpler addressing
    # r8 = current x pointer, r9 = current y pointer
    mov r8, rsi
    mov r9, rdi
    
    # Calculate end pointer for y
    lea r10, [rdi + rdx*4]
    
.L_loop:
    # Check if we've processed all elements (r9 >= r10 means done)
    cmp r9, r10
    jge .L_done
    
    # Load 4 floats from x
    movups xmm0, XMMWORD PTR [r8]
    
    # Load 4 floats from y
    movups xmm1, XMMWORD PTR [r9]
    
    # Compute y[i] = a * x[i] + y[i] using separate multiply and add
    # (No FMA in SSE)
    movaps xmm2, xmm0
    mulps xmm2, xmm3        # xmm2 = a * x
    addps xmm2, xmm1        # xmm2 = (a * x) + y
    
    # Store result back to y
    movups XMMWORD PTR [r9], xmm2
    
    # Advance pointers by 16 bytes (4 floats * 4 bytes)
    add r8, 16
    add r9, 16
    jmp .L_loop
    
.L_done:
    pop rbp
    ret

.size saxpy_sse, .-saxpy_sse

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
