.text

.global _start

_start:

	# int x = write(stdout, "hello world\n", sizeof("hello world\n"));
	mov     $1, %rax		# write(
	mov     $1, %rdi		# stdout,
	mov     $message, %rsi		# message = "hello world\n",
	mov     $length, %rdx		# 12
						## (sizeof(message) = 12)
	syscall			# );
	
	mov	%rax, %r10		# saves the return value of write system call into r10

	# exit(x);
	mov     $60, %rax		# exit(
	mov     %r10, %rdi		# 12 
						## (exit with the return value from write)
	syscall			# );

	message:
	.ascii  "hello world\n"
    length = . - message
