/**
 * SAXPY AVX-256 Kernel - DEBUG VERSION
 */

.text
.globl saxpy_avx256
.type saxpy_avx256, @function

saxpy_avx256:
    # Arguments:
    #   rdi = float *y
    #   rsi = float *x  
    #   xmm0 = float a
    #   rcx = size_t n
    
    # Save registers we'll clobber for debugging
    push %rbx
    
    # Broadcast scalar 'a' to all lanes of ymm3
    vbroadcastss %xmm0, %ymm3
    
    # Store ymm3 to memory to check broadcast worked
    sub $32, %rsp
    vmovups %ymm3, (%rsp)
    
    # Initialize loop counter
    xor %rax, %rax
    
.L_loop:
    cmp %rcx, %rax
    jge .L_done
    
    # Load X and Y
    vmovups (%rsi,%rax,4), %ymm0
    vmovups (%rdi,%rax,4), %ymm1
    
    # FMA: ymm1 = ymm1 + (ymm3 * ymm0)
    vfmadd231ps %ymm0, %ymm3, %ymm1
    
    # Store result
    vmovups %ymm1, (%rdi,%rax,4)
    
    add $8, %rax
    jmp .L_loop
    
.L_done:
    add $32, %rsp
    pop %rbx
    vzeroupper
    ret
