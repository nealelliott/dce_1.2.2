/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: SYS.h,v $
 * Revision 1.1.4.2  1996/02/18  23:12:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:12:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:08  root]
 * 
 * Revision 1.1.2.2  1992/12/30  20:41:58  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:21:38  htf]
 * 
 * Revision 1.1  1992/01/19  15:34:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* SYS.h 4.1 83/05/10 */

#include <syscall.h>

#ifdef PROF
#define	ENTRY(x)	.globl _/**/x; .align 2; _/**/x: .word 0; \
			.data; 1:; .long 0; .text; moval 1b,r0; jsb mcount
#else
#define	ENTRY(x)	.globl _/**/x; .align 2; _/**/x: .word 0
#endif PROF
#define	SYSCALL(x)	err: jmp cerror; ENTRY(x); chmk $SYS_/**/x; jcs err
#define	PSEUDO(x,y)	ENTRY(x); chmk $SYS_/**/y
#define	CALL(x,y)	calls $x, _/**/y

	.globl	cerror
