.intel_syntax noprefix
.text
.globl vadd_sse
.type vadd_sse, @function

# void vadd_sse(float* z, const float* x, const float* y, size_t n)
# Arguments (x86-64 System V ABI):
#   rdi = z (output array)
#   rsi = x (input array 1)
#   rdx = y (input array 2)
#   rcx = n (number of elements)

vadd_sse:
    push rbp
    mov rbp, rsp

    # Set up pointers
    mov r8, rsi          # r8 = x pointer
    mov r9, rdx          # r9 = y pointer
    mov r10, rdi         # r10 = z pointer

    # Calculate end pointer: z + n*4
    lea r11, [rdi + rcx*4]

.L_loop:
    # Check if done
    cmp r10, r11
    jge .L_done

    # Load 4 floats from x into xmm0 (128-bit SSE)
    movups xmm0, XMMWORD PTR [r8]

    # Load 4 floats from y into xmm1
    movups xmm1, XMMWORD PTR [r9]

    # Add: xmm2 = xmm0 + xmm1
    movaps xmm2, xmm0
    addps xmm2, xmm1

    # Store result to z
    movups XMMWORD PTR [r10], xmm2

    # Advance pointers by 16 bytes (4 floats)
    add r8, 16
    add r9, 16
    add r10, 16

    jmp .L_loop

.L_done:
    pop rbp
    ret

.size vadd_sse, .-vadd_sse
