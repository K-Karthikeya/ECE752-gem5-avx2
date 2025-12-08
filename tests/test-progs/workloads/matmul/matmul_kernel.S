/**
 * Matrix Multiplication Kernels
 * 
 * AVX version uses:
 * - 8-wide vectorization in the innermost loop
 * - FMA for multiply-accumulate
 * - Blocked access pattern for better cache utilization
 */

.text

/**
 * AVX-256 Matrix Multiplication: C = A * B
 * 
 * Uses row-major layout: C[i][j] = sum(A[i][k] * B[k][j])
 */
.globl matmul_avx256
.type matmul_avx256, @function

matmul_avx256:
    # Arguments:
    #   rdi = float *C (output NxN matrix)
    #   rsi = float *A (input NxN matrix)
    #   rdx = float *B (input NxN matrix)
    #   rcx = size_t N (matrix dimension)
    
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15
    
    mov %rcx, %r15          # r15 = N (save for later)
    xor %r12, %r12          # r12 = i (row of A and C)
    
.L_i_loop:
    cmp %r15, %r12
    jge .L_i_done
    
    xor %r13, %r13          # r13 = j (column of B and C, step by 8)
    
.L_j_loop:
    cmp %r15, %r13
    jge .L_j_done
    
    # Initialize accumulator: ymm0 = 0 (will hold C[i][j:j+7])
    vxorps %ymm0, %ymm0, %ymm0
    
    xor %r14, %r14          # r14 = k (inner product dimension)
    
.L_k_loop:
    cmp %r15, %r14
    jge .L_k_done
    
    # Broadcast A[i][k] to all lanes of ymm1
    # Address: A + (i * N + k) * 4
    mov %r12, %rax
    imul %r15, %rax         # rax = i * N
    add %r14, %rax          # rax = i * N + k
    vbroadcastss (%rsi,%rax,4), %ymm1
    
    # Load B[k][j:j+7] into ymm2
    # Address: B + (k * N + j) * 4
    mov %r14, %rax
    imul %r15, %rax         # rax = k * N
    add %r13, %rax          # rax = k * N + j
    vmovups (%rdx,%rax,4), %ymm2
    
    # Accumulate: ymm0 += ymm1 * ymm2 (FMA)
    vfmadd231ps %ymm2, %ymm1, %ymm0
    
    inc %r14
    jmp .L_k_loop
    
.L_k_done:
    # Store C[i][j:j+7] = ymm0
    # Address: C + (i * N + j) * 4
    mov %r12, %rax
    imul %r15, %rax
    add %r13, %rax
    vmovups %ymm0, (%rdi,%rax,4)
    
    add $8, %r13
    jmp .L_j_loop
    
.L_j_done:
    inc %r12
    jmp .L_i_loop
    
.L_i_done:
    vzeroupper
    
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    ret

.size matmul_avx256, .-matmul_avx256
