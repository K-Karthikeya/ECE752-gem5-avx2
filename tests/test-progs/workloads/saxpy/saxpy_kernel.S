/**
 * SAXPY AVX-256 Kernel
 *
 * Implements Y = a*X + Y using 256-bit AVX with FMA
 * Processes 8 single-precision floats per iteration
 *
 * Matches exact pattern from working avx/ops/ldstfp_256.S test
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

    # Load 8 floats from x - simple direct addressing
    vmovups ymm0, YMMWORD PTR [r8]

    # Load 8 floats from y - simple direct addressing
    vmovups ymm1, YMMWORD PTR [r9]

    # Compute y[i] = a * x[i] + y[i] using FMA
    # VFMADD231PS: ymm1 = (ymm3 * ymm0) + ymm1 = (a * x) + y
    vfmadd231ps ymm1, ymm3, ymm0

    # Store result back to y - simple direct addressing
    vmovups YMMWORD PTR [r9], ymm1

    # Advance pointers by 32 bytes (8 floats * 4 bytes)
    add r8, 32
    add r9, 32
    jmp .L_loop

.L_done:
    # Clean up AVX state before returning
    vzeroupper
    pop rbp
    ret

.size saxpy_avx256, .-saxpy_avx256

    # Declare non-executable stack note to silence linker warning
    .section .note.GNU-stack,"",@progbits
