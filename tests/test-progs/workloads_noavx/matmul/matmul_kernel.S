/**
 * Matrix Multiplication Kernel (SSE version)
 * 
 * SSE version uses:
 * - 4-wide vectorization in the innermost loop
 * - Separate multiply and add (no FMA available in SSE)
 * - Blocked access pattern for better cache utilization
 */

.intel_syntax noprefix
.text

/**
 * SSE Matrix Multiplication: C = A * B
 * 
 * Uses row-major layout: C[i][j] = sum(A[i][k] * B[k][j])
 */
.globl matmul_sse
.type matmul_sse, @function

matmul_sse:
    # Arguments:
    #   rdi = float *C (output NxN matrix)
    #   rsi = float *A (input NxN matrix)
    #   rdx = float *B (input NxN matrix)
    #   rcx = size_t N (matrix dimension)
    
    push rbx
    push r12
    push r13
    push r14
    push r15
    
    mov r15, rcx            # r15 = N (save for later)
    xor r12, r12            # r12 = i (row of A and C)
    
.L_i_loop:
    cmp r12, r15
    jge .L_i_done
    
    xor r13, r13            # r13 = j (column of B and C, step by 4)
    
.L_j_loop:
    cmp r13, r15
    jge .L_j_done
    
    # Initialize accumulator: xmm0 = 0 (will hold C[i][j:j+3])
    xorps xmm0, xmm0
    
    xor r14, r14            # r14 = k (inner product dimension)
    
.L_k_loop:
    cmp r14, r15
    jge .L_k_done
    
    # Broadcast A[i][k] to all lanes of xmm1
    # Address: A + (i * N + k) * 4
    mov rax, r12
    imul rax, r15           # rax = i * N
    add rax, r14            # rax = i * N + k
    movss xmm1, DWORD PTR [rsi + rax*4]
    shufps xmm1, xmm1, 0    # Broadcast to all 4 lanes
    
    # Load B[k][j:j+3] into xmm2
    # Address: B + (k * N + j) * 4
    mov rax, r14
    imul rax, r15           # rax = k * N
    add rax, r13            # rax = k * N + j
    movups xmm2, XMMWORD PTR [rdx + rax*4]
    
    # Accumulate: xmm0 += xmm1 * xmm2 (separate multiply and add)
    movaps xmm3, xmm1
    mulps xmm3, xmm2        # xmm3 = xmm1 * xmm2
    addps xmm0, xmm3        # xmm0 += xmm3
    
    inc r14
    jmp .L_k_loop
    
.L_k_done:
    # Store C[i][j:j+3] = xmm0
    # Address: C + (i * N + j) * 4
    mov rax, r12
    imul rax, r15
    add rax, r13
    movups XMMWORD PTR [rdi + rax*4], xmm0
    
    add r13, 4              # Process 4 columns at a time (SSE width)
    jmp .L_j_loop
    
.L_j_done:
    inc r12
    jmp .L_i_loop
    
.L_i_done:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

.size matmul_sse, .-matmul_sse
