/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: dce_exc_context.s,v $
 * Revision 1.1.8.2  1996/03/09  23:26:03  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:41:43  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:29  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:53:36  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:45  devsrc]
 * 
 * Revision 1.1.4.1  1993/09/01  14:29:35  delgado
 * 	DFS 486 port
 * 	[1993/09/01  14:27:00  delgado]
 * 
 * $EndLog$
 */
 
#include <machine/asm.h>

	.text

#define	O_EDI	0
#define	O_ESI	4
#define	O_EBX	8
#define	O_EBP	12
#define	O_ESP	16
#define O_EIP	20
#define	O_IPL	24

.globl _dce_exc_setjmp
.align ALIGN
_dce_exc_setjmp:
	movl	4(%esp), %edx		/ address of save area
	movl	_curr_ipl, %eax
	movl	%eax, O_IPL(%edx)	/ save ipl level for longjmp
	movl	%edi, O_EDI(%edx)
	movl	%esi, O_ESI(%edx)
	movl	%ebx, O_EBX(%edx)
	movl	%ebp, O_EBP(%edx)
	movl	%esp, O_ESP(%edx)
	movl	(%esp), %ecx		/ %eip (return address)
	movl	%ecx, O_EIP(%edx)
	subl	%eax, %eax		/ retval <- 0
	ret

.globl _dce_exc_longjmp
.align ALIGN
_dce_exc_longjmp:
	call	_splhi
	movl	4(%esp), %edx		/ address of save area
	movl	O_EDI(%edx), %edi
	movl	O_ESI(%edx), %esi
	movl	O_EBX(%edx), %ebx
	movl	O_EBP(%edx), %ebp
	movl	O_ESP(%edx), %esp
	movl	O_EIP(%edx), %ecx	/ %eip (return address)
#if	STACK_LIMIT_CHECK
/ following code loads the limit field of the kernel stack selector
/ descriptor with the current threads kernel stack base. This prevents
/ writing into the last page of the kernel stack (you had better have at
/ least two pages of kernel stack!). when the stack overflows a double
/ fault will happen (actually you will get a stack segment exception, but
/ will then get a double fault because the kernel stack isnt usable).
	movl	_active_threads,%eax
	movl	THREAD_STACK(%eax),%eax	/ get stack base
	shrl	$12,%eax		/ drop page part
	movw	%ax,_gdt+KSSSEL
	shrl	$16,%eax		/ get high order bits
	orb	$0x80+0x40,%al		/ set D + G
	movb	%al,_gdt+6+KSSSEL
	movw	$KSSSEL,%ax
	movw	%ax,%ss			/ access new descriptor
#endif	/* STACK_LIMIT_CHECK */
	pushl	%ecx
	pushl	O_IPL(%edx)
	call	_splx		/ restore ipl level
	popl	%edx
	popl	%ecx
	movl	$1, %eax		/ ret val <- 1
	addl	$4, %esp		/ ret address
	jmp	*%ecx			/ indirect

