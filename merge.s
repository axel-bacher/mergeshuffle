// This code works in the same way as the one in the file merge_vec.c. It runs
// slightly faster than the one produced by the compiler.
.text
.global	merge
	.align	16
perm1:	.int	303239696
	.int	1803315264
	.int	3166732288
	.int	3221225472
perm2:	.int	0
	.int	1077952576
	.int	2483065856
	.int	3918790656
mask1:	.int	858993459
	.int	252645135
	.int	16711935
	.int	65535
mask2:	.int	1073741823
	.int	54476799
	.int	197439
	.int	3
merge:	movq	%rdi, %r8
	leaq	(%rdi,%rsi,4), %rsi
	leaq	(%rdi,%rdx,4), %r9
	movaps	perm1, %xmm4
	movaps	perm2, %xmm5
	movaps	mask1, %xmm6
	movaps	mask2, %xmm7
	jmp	new
loop:	vmovups	(%rdi), %xmm0
	vmovups	(%rsi), %xmm1
	vpslld	%xmm2, %xmm4, %xmm3
	vpermilps	%xmm3, %xmm0, %xmm0
	vpslld	%xmm2, %xmm5, %xmm3
	vpermilps	%xmm3, %xmm1, %xmm1
	vpsrld	%xmm2, %xmm6, %xmm3
	vmaskmovps	%xmm3, %xmm1, (%rdi)
	vpsrld	%xmm2, %xmm7, %xmm3
	vmaskmovps	%xmm3, %xmm0, (%rsi)
	movq	%r10, %rdi
	movq	%r11, %rsi
	shrq	$4, %rax
	subl	$4, %ecx
	jnz	skip
new:	rdrand	%rax
	jnc	new
	movl	$64, %ecx
skip:	movl	%eax, %edx
	andl	$15, %edx
	addl	%edx, %edx
	vmovd	%edx, %xmm2
	popcntl	%edx, %edx
	leaq	16(%rdi), %r10
	leaq	(%rsi,%rdx,4), %r11
	cmpq	%rsi, %r10
	ja	skip2
	cmpq	%r9, %r11
	jna	loop
	jmp	skip2
swap:	cmpq	%r9, %rsi
	jnb	out2
	movl	(%rdi), %r10d
	movl	(%rsi), %r11d
	movl	%r11d, (%rdi)
	movl	%r10d, (%rsi)
	addq	$4, %rsi
test:	addq	$4, %rdi
	shrq	%rax
	decl	%ecx
	jnz	skip2
new2:	rdrand	%rax
	jnc	new2
	movl	$64, %ecx
skip2:	testb	$1, %al
	jnz	swap
	cmpq	%rsi, %rdi
	jb	test
out2:	movq	%r9, %rax
	subq	%r8, %rax
	shrq	$2, %rax
	bsrl	%eax, %ecx
	xorl	%eax, %eax
	btsl	%ecx, %eax
	leal	-1(%rax,%rax), %ecx
	jmp	start3
new3:	rdrand	%eax
	jnc	new3
	andl	%ecx, %eax
	leaq	(%r8,%rax,4), %rsi
	cmpq	%rdi, %rsi
	ja	new3
	movl	(%rdi), %eax
	movl	(%rsi), %edx
	movl	%edx, (%rdi)
	movl	%eax, (%rsi)
	addq	$4, %rdi
start3:	cmpq	%r9, %rdi
	jb	new3
	ret
