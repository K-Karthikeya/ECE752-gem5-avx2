# Minimal entry point bypassing libc startup (which uses x87)

.section .data
    .align 32
    a_data:
        .float 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0

    .align 32
    b_data:
        .float 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0

.section .bss
    .align 32
    out_data:
        .skip 416  # 104 * 4 bytes

.section .rodata
    .align 32
    expected:
        # 0..7   VMOVUPS a
        .float 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0
        # 8..15  VMOVAPS b
        .float 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0
        # 16..23 VMINPS reg-reg (min(a,b))
        .float 1.0, 2.0, 3.0, 4.0, 4.0, 3.0, 2.0, 1.0
        # 24..31 VMAXPS reg-reg (max(a,b))
        .float 8.0, 7.0, 6.0, 5.0, 5.0, 6.0, 7.0, 8.0
        # 32..39 VMOVSLDUP reg-reg (dup lows of a)
        .float 1.0, 1.0, 3.0, 3.0, 5.0, 5.0, 7.0, 7.0
        # 40..47 VMOVSHDUP reg-reg (dup highs of b)
        .float 7.0, 7.0, 5.0, 5.0, 3.0, 3.0, 1.0, 1.0
        # 48..55 VBROADCASTSS a[0]
        .float 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0
        # 56..63 VMINPS reg-mem (min(a,b))
        .float 1.0, 2.0, 3.0, 4.0, 4.0, 3.0, 2.0, 1.0
        # 64..71 VMAXPS reg-mem (max(a,b))
        .float 8.0, 7.0, 6.0, 5.0, 5.0, 6.0, 7.0, 8.0
        # 72..79 VMOVSLDUP reg-mem
        .float 1.0, 1.0, 3.0, 3.0, 5.0, 5.0, 7.0, 7.0
        # 80..87 VMOVSHDUP reg-mem
        .float 7.0, 7.0, 5.0, 5.0, 3.0, 3.0, 1.0, 1.0
        # 88..95 VBROADCASTSS a[1]
        .float 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0
        # 96..103 VZEROUPPER (lower 128 = a[0..3], upper = 0)
        .float 1.0, 2.0, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0

.section .text
    .globl _start
    .type _start, @function

_start:
    # Load data pointers using RIP-relative addressing
    lea a_data(%rip), %rdi
    lea b_data(%rip), %rsi
    lea out_data(%rip), %rdx

    # Call avx_trace_test
    call avx_trace_test

    # Verify out_data against expected; if mismatch, exit code = 1
    lea out_data(%rip), %rsi      # rsi = actual
    lea expected(%rip), %rdi      # rdi = expected
    mov $104, %rcx                # count of floats
    xor %eax, %eax                # accumulates mismatches
1:
    mov (%rdi), %r8d
    mov (%rsi), %r9d
    cmp %r8d, %r9d
    jne 2f
    add $4, %rdi
    add $4, %rsi
    loop 1b
    jmp 3f
2:
    mov $1, %eax                  # mark failure
3:

    # Exit with code in eax (0 = pass, 1 = fail)
    mov %eax, %edi          # preserve computed status
    mov $60, %rax           # SYS_exit
    syscall

    .size _start, .-_start

# Disable the default libc startup
    .weak __libc_start_main
    .set __libc_start_main, _start
