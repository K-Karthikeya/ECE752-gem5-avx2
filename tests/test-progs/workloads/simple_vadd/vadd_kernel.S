.intel_syntax noprefix
.text
.globl vadd_avx256
.type vadd_avx256, @function

# void vadd_avx256(float* z, const float* x, const float* y, size_t n)
# Arguments (x86-64 System V ABI):
#   rdi = z (output array)
#   rsi = x (input array 1)
#   rdx = y (input array 2)
#   rcx = n (number of elements)

vadd_avx256:
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
    
    # Load 8 floats from x into ymm0
    vmovups ymm0, YMMWORD PTR [r8]
    
    # Load 8 floats from y into ymm1
    vmovups ymm1, YMMWORD PTR [r9]
    
    # Add: ymm2 = ymm0 + ymm1
    vaddps ymm2, ymm0, ymm1
    
    # Store result to z
    vmovups YMMWORD PTR [r10], ymm2
    
    # Advance pointers by 32 bytes (8 floats)
    add r8, 32
    add r9, 32
    add r10, 32
    
    jmp .L_loop

.L_done:
    # Clean up upper halves of YMM registers
    vzeroupper
    
    pop rbp
    ret

.size vadd_avx256, .-vadd_avx256
