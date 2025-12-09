	.file	"test_fma.c"
	.section .rdata,"dr"
LC13:
	.ascii "FMA Intrinsic Test:\0"
LC14:
	.ascii "a = %f\12\0"
LC15:
	.ascii "PASS\0"
LC16:
	.ascii "FAIL\0"
	.align 4
LC17:
	.ascii "[%d] x=%f y=%f result=%f expected=%f %s\12\0"
	.text
	.globl	_test_fma_intrinsic
	.def	_test_fma_intrinsic;	.scl	2;	.type	32;	.endef
_test_fma_intrinsic:
LFB3401:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-32, %esp
	subl	$496, %esp
	flds	LC0
	fstps	488(%esp)
	flds	LC1
	fstps	132(%esp)
	flds	LC2
	fstps	136(%esp)
	flds	LC3
	fstps	140(%esp)
	flds	LC4
	fstps	144(%esp)
	flds	LC5
	fstps	148(%esp)
	flds	LC6
	fstps	152(%esp)
	flds	LC7
	fstps	156(%esp)
	flds	LC8
	fstps	160(%esp)
	flds	LC2
	fstps	100(%esp)
	flds	LC4
	fstps	104(%esp)
	flds	LC6
	fstps	108(%esp)
	flds	LC8
	fstps	112(%esp)
	fld1
	fstps	116(%esp)
	flds	LC10
	fstps	120(%esp)
	flds	LC11
	fstps	124(%esp)
	flds	LC12
	fstps	128(%esp)
	flds	488(%esp)
	fstps	164(%esp)
	vbroadcastss	164(%esp), %ymm0
	vmovaps	%ymm0, 432(%esp)
	leal	132(%esp), %eax
	movl	%eax, 168(%esp)
	movl	168(%esp), %eax
	vmovups	(%eax), %ymm0
	vmovaps	%ymm0, 400(%esp)
	leal	100(%esp), %eax
	movl	%eax, 172(%esp)
	movl	172(%esp), %eax
	vmovups	(%eax), %ymm0
	vmovaps	%ymm0, 368(%esp)
	vmovaps	432(%esp), %ymm0
	vmovaps	%ymm0, 240(%esp)
	vmovaps	400(%esp), %ymm0
	vmovaps	%ymm0, 208(%esp)
	vmovaps	368(%esp), %ymm0
	vmovaps	%ymm0, 176(%esp)
	vmovaps	208(%esp), %ymm1
	vmovaps	176(%esp), %ymm0
	vfmadd231ps	240(%esp), %ymm1, %ymm0
	vmovaps	%ymm0, 336(%esp)
	leal	68(%esp), %eax
	movl	%eax, 328(%esp)
	vmovaps	336(%esp), %ymm0
	vmovaps	%ymm0, 272(%esp)
	movl	328(%esp), %eax
	vmovaps	272(%esp), %ymm0
	vmovups	%ymm0, (%eax)
	movl	$LC13, (%esp)
	call	_puts
	flds	488(%esp)
	fstpl	4(%esp)
	movl	$LC14, (%esp)
	call	_printf
	movl	$0, 492(%esp)
	jmp	L6
L10:
	movl	492(%esp), %eax
	flds	132(%esp,%eax,4)
	fmuls	488(%esp)
	movl	492(%esp), %eax
	flds	100(%esp,%eax,4)
	faddp	%st, %st(1)
	fstps	332(%esp)
	movl	492(%esp), %eax
	flds	68(%esp,%eax,4)
	flds	332(%esp)
	fucomip	%st(1), %st
	jp	L12
	flds	332(%esp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jne	L7
	movl	$LC15, %edx
	jmp	L9
L12:
	fstp	%st(0)
L7:
	movl	$LC16, %edx
L9:
	flds	332(%esp)
	movl	492(%esp), %eax
	flds	68(%esp,%eax,4)
	movl	492(%esp), %eax
	flds	100(%esp,%eax,4)
	movl	492(%esp), %eax
	flds	132(%esp,%eax,4)
	fxch	%st(3)
	movl	%edx, 40(%esp)
	fstpl	32(%esp)
	fxch	%st(1)
	fstpl	24(%esp)
	fstpl	16(%esp)
	fstpl	8(%esp)
	movl	492(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC17, (%esp)
	call	_printf
	addl	$1, 492(%esp)
L6:
	cmpl	$7, 492(%esp)
	jle	L10
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE3401:
/APP
	.text
.globl test_fma_asm
.type test_fma_asm, @function
test_fma_asm:
    # Args: xmm0=a, rdi=x, rsi=y, rdx=result
    vbroadcastss %xmm0, %ymm3
    vmovups (%rdi), %ymm0
    vmovups (%rsi), %ymm1
    vfmadd231ps %ymm0, %ymm3, %ymm1
    vmovups %ymm1, (%rdx)
    vzeroupper
    ret

	.section .rdata,"dr"
LC19:
	.ascii "\12FMA Assembly Test:\0"
/NO_APP
	.text
	.globl	_test_fma_assembly
	.def	_test_fma_assembly;	.scl	2;	.type	32;	.endef
_test_fma_assembly:
LFB3402:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$168, %esp
	flds	LC0
	fstps	-16(%ebp)
	flds	LC1
	fstps	-52(%ebp)
	flds	LC2
	fstps	-48(%ebp)
	flds	LC3
	fstps	-44(%ebp)
	flds	LC4
	fstps	-40(%ebp)
	flds	LC5
	fstps	-36(%ebp)
	flds	LC6
	fstps	-32(%ebp)
	flds	LC7
	fstps	-28(%ebp)
	flds	LC8
	fstps	-24(%ebp)
	flds	LC2
	fstps	-84(%ebp)
	flds	LC4
	fstps	-80(%ebp)
	flds	LC6
	fstps	-76(%ebp)
	flds	LC8
	fstps	-72(%ebp)
	fld1
	fstps	-68(%ebp)
	flds	LC10
	fstps	-64(%ebp)
	flds	LC11
	fstps	-60(%ebp)
	flds	LC12
	fstps	-56(%ebp)
	leal	-116(%ebp), %eax
	movl	%eax, 12(%esp)
	leal	-84(%ebp), %eax
	movl	%eax, 8(%esp)
	leal	-52(%ebp), %eax
	movl	%eax, 4(%esp)
	flds	-16(%ebp)
	fstps	(%esp)
	call	_test_fma_asm
	movl	$LC19, (%esp)
	call	_puts
	flds	-16(%ebp)
	fstpl	4(%esp)
	movl	$LC14, (%esp)
	call	_printf
	movl	$0, -12(%ebp)
	jmp	L14
L18:
	movl	-12(%ebp), %eax
	flds	-52(%ebp,%eax,4)
	fmuls	-16(%ebp)
	movl	-12(%ebp), %eax
	flds	-84(%ebp,%eax,4)
	faddp	%st, %st(1)
	fstps	-20(%ebp)
	movl	-12(%ebp), %eax
	flds	-116(%ebp,%eax,4)
	flds	-20(%ebp)
	fucomip	%st(1), %st
	jp	L20
	flds	-20(%ebp)
	fucomip	%st(1), %st
	fstp	%st(0)
	jne	L15
	movl	$LC15, %edx
	jmp	L17
L20:
	fstp	%st(0)
L15:
	movl	$LC16, %edx
L17:
	flds	-20(%ebp)
	movl	-12(%ebp), %eax
	flds	-116(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	flds	-84(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	flds	-52(%ebp,%eax,4)
	fxch	%st(3)
	movl	%edx, 40(%esp)
	fstpl	32(%esp)
	fxch	%st(1)
	fstpl	24(%esp)
	fstpl	16(%esp)
	fstpl	8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC17, (%esp)
	call	_printf
	addl	$1, -12(%ebp)
L14:
	cmpl	$7, -12(%ebp)
	jle	L18
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE3402:
	.def	___main;	.scl	2;	.type	32;	.endef
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB3403:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	call	___main
	call	_test_fma_intrinsic
	call	_test_fma_assembly
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE3403:
	.section .rdata,"dr"
	.align 4
LC0:
	.long	1075838976
	.align 4
LC1:
	.long	1036831949
	.align 4
LC2:
	.long	1045220557
	.align 4
LC3:
	.long	1050253722
	.align 4
LC4:
	.long	1053609165
	.align 4
LC5:
	.long	1056964608
	.align 4
LC6:
	.long	1058642330
	.align 4
LC7:
	.long	1060320051
	.align 4
LC8:
	.long	1061997773
	.align 4
LC10:
	.long	1067030938
	.align 4
LC11:
	.long	1068708659
	.align 4
LC12:
	.long	1070386381
	.ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
	.def	_puts;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_test_fma_asm;	.scl	2;	.type	32;	.endef
