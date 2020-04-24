/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_exc_context.s,v $
 * Revision 1.1.4.2  1996/03/09  23:26:12  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:41:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:14:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:02  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:35:41  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:37:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:26:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  22:13:37  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *  OSF DCE Version 1.0 
 */

/* 
 *	File: dce_exp_context.s
 *
 *	setjmp and longjmp primitives required by the kernel RPC
 *	for implementing the DCE Exceptions (CMA Exceptions) package.
 *
 */

#include <mips/cpu.h>
#include <mips/asm.h>
#include <mips/reg.h>
#include <mips/regdef.h>
#include <mips/pcb.h>
#include <mach/mips/vm_param.h>
#include <mips/pmap.h>
#include <mips/thread.h>


	BSS(current_kstack,4)		# for exception()
	BSS(current_pcb,4)		# spares a wired tlb entry

	.set	noreorder		# unless overridden

/*
 * dce_exc_setjmp(jmp_buf)
 *
 * Purpose
 *	Save current context for non-local goto's
 *
 * Description
 *	Saves all registers that are callee-saved in the
 *	given longjmp buffer.  Same as user level _setjmp
 * 	Return 0
 */
LEAF(dce_exc_setjmp)
	sw	ra,KJB_PC*4(a0)
	sw	sp,KJB_SP*4(a0)
	sw	fp,KJB_FP*4(a0)
	sw	s0,KJB_S0*4(a0)
	sw	s1,KJB_S1*4(a0)
	sw	s2,KJB_S2*4(a0)
	sw	s3,KJB_S3*4(a0)
	sw	s4,KJB_S4*4(a0)
	sw	s5,KJB_S5*4(a0)
	sw	s6,KJB_S6*4(a0)
	mfc0	v0,C0_SR
	sw	s7,KJB_S7*4(a0)
	sw	v0,KJB_SR*4(a0)
	move	v0,zero
	j	ra
	END(dce_exc_setjmp)


/*
 * dce_exc_longjmp(jmp_buf)
 *
 * Purpose
 *	Perform a non-local goto
 *
 * Description
 *	Restores all registers that are callee-saved from the
 *	given longjmp buffer.  Same as user level _longjmp
 * 	Return a non-zero value.
 */
LEAF(dce_exc_longjmp)
	lw	ra,KJB_PC*4(a0)
	lw	sp,KJB_SP*4(a0)
	lw	fp,KJB_FP*4(a0)
	lw	s0,KJB_S0*4(a0)
	lw	s1,KJB_S1*4(a0)
	lw	s2,KJB_S2*4(a0)
	lw	s3,KJB_S3*4(a0)
	lw	s4,KJB_S4*4(a0)
	lw	s5,KJB_S5*4(a0)
	lw	s6,KJB_S6*4(a0)
	lw	s7,KJB_S7*4(a0)
	lw	v0,KJB_SR*4(a0)
	nop
	mtc0	v0,C0_SR
	li	v0,1		/* return non-zero */
	j	ra
	END(dce_exc_longjmp)




