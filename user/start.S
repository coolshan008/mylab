
.code32
.globl start
.extern	main
start:
	movl	$1,%eax
	movl	$2,%ebx
	movl	$3,%ecx
	movl	$4,%edx
	call	main
