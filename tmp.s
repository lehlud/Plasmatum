	.text
	.file	"tmp.ll"
	.globl	__unnamed_1                     # -- Begin function 
	.p2align	4, 0x90
	.type	__unnamed_1,@function
__unnamed_1:                            # @0
	.cfi_startproc
# %bb.0:
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$8, %edi
	callq	malloc@PLT
	movq	$42, (%rax)
	movb	$1, 8(%rsp)
	movq	%rax, 16(%rsp)
	movb	8(%rsp), %al
	movq	16(%rsp), %rdx
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	__unnamed_1, .Lfunc_end0-__unnamed_1
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	movl	%eax, %esi
	movq	%rsi, %rdi
	callq	__unnamed_1@PLT
	movb	$42, %al
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
