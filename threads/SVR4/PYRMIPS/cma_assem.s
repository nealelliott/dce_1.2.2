/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: cma_assem.s,v $
 * Revision 1.1.8.2  1996/03/09  23:30:48  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:46:03  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:52:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:01:54  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:02  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:30  devsrc]
 * 
 * Revision 1.1.3.3  1992/12/15  22:23:46  alan
 * 	Insert copyright notices
 * 	[1992/12/07  18:57:50  alan]
 * 
 * Revision 1.1.3.2  1992/09/29  21:25:25  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:37:29  sekhar]
 * 
 * $EndLog$
 */

/*
 *  Copyright (c) 1989, 1991
 *  by Digital Equipment Corporation, Maynard, Mass.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by Digital Equipment
 *  Corporation.
 *
 *  Digital assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by Digital.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	These are non-portable subroutines for thread service routines.  They
 *	cannot be written in C because they manipulate specific registers, etc.
 *
 *
 *  AUTHORS:
 *
 *	Bill Rees and David Bein
 *
 *  CREATION DATE:
 *
 *	30 July 1992
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *	A thread context consists of the saved register set necessary for
 *	rescheduling the thread on another processor.
 *
 *	The context consists of the general CPU registers $1-$31 (the value 
 *	of $0 never changes and thus need not be saved), the integer divide
 *	registers HI & LO, the floating point coprocessor registers $f0-$f31,
 *	and the floating point coprocessor control/status register FCR31.
 *
 *	The context is pushed onto the stack (fetching FCR31 has the desirable
 *	side effect of synchronizing with the floating point coprocessor and
 *	thereby preventing an exception resulting from interrupting an
 *	concurrent floating point instruction execution), and the sp is
 *	stored in the static context block in the TCB.
 *
 *	The process is slightly more complicated in the case of an
 *	asynchronous context switch, such as a quantum end.  The interrupt
 *	routine saves the return PC and the floating point coprocessor status
 *	in the TCB, replaces these values with the address of
 *	cma__force_dispatch and a known state status word.  When the interrupt
 *	routine returns, execution resumes in cma__force_dispatch instead of
 *	at the point where the interruption occurred.  There, the volatile
 *	registers are saved in a context area on the stack.  The saved values
 *	of the PC and processor status at the point of interruption are
 *	retrieved from the TCB by a call to a C subroutine and stored in the
 *	context area on the stack. Then the usual dispatching routines are
 *	called, resulting in a second, full context being saved on the stack.
 *	When the thread context is restored as a result of being rescheduled,
 *	it returns to cma__force_dispatch where the volatile registers, PC,
 *	and PS are restored allowing the thread to resume execution at the
 *	point where the interruption occurred.
 */

#ifdef __LANGUAGE_C__
# undef __LANGUAGE_C__
#endif

#ifndef __LANGUAGE_ASSEMBLY__
# define __LANGUAGE_ASSEMBLY__
#endif
#include <sys/regdef.h>
#include <sys/asm.h>
#include <sys/fpu.h>
#include <sys/softfp.h>
#include <sys/syscall.h>
#include <sys/inst.h>
#include <setjmp.h>
#include "dce_reg.h"
#include <cma_config.h>
#include <cma_host.h>
#include <cma_defs.h>

#define ROUTINE(name)							\
NESTED(name, FSIZE, ra)		 ;/* FSIZE is #def'd for in routine */ \
	subu	sp, FSIZE		 ;/* Create frame for routine */	\
	sw	ra, FSIZE-FOFFS(sp)	 /* Fetch the return address */

#undef RETURN
#define RETURN								\
	lw	ra, FSIZE-FOFFS(sp)	 ;/* Fetch the return address */  \
	addu	sp, FSIZE		 ;/* Pop the frame */		\
	j	ra			 /* And return */

#undef	CALL
#define	CALL	jal

#define RROUTINE(name)							\
NOGP_NESTED(name, FSIZE, ra)		 ;/* FSIZE is #def'd for in routine */ \
	subu	sp, FSIZE		 ;/* Create frame for routine */	\
	sw	ra, FSIZE-FOFFS(sp)	 /* Fetch the return address */


/*
 * Convenient stack offsets for routines that make calls when loading or
 * storing argument values.
 */
#define AA0	0
#define AA1	4
#define AA2	4*2
#define AA3	4*3
#define AA4	4*4
#define AA5	4*5
#define AA6	4*6
#define AA7	4*7
#define AA8	4*8

/*
 * Routines which are referenced below:
 */
	.globl	cma__abort_process
	.globl	cma__async_delivery
	.globl	cma__bugcheck
#ifdef _CMA_TRACE_KERNEL_
	.globl	cma__enter_kern_record
	.globl	cma__exit_kern_record
#endif
	.globl	cma__get_async_info
	.globl	cma__get_self_tcb
	.globl	cma__start_thread
	.globl	cma__thread_base
	.globl	cma__undefer
#ifdef _CMA_TRACE_KERNEL_
	.globl	cma__unset_kern_record
#endif
	.globl	cma__yield_processor
#if !_CMA_UNIPROCESSOR_
	.globl	cma__vp_lock
	.globl	cma__vp_unlock
#endif

/*
 * Global data:
 */
	IMPORT(cma__g_abort_signal, 4)
	IMPORT(cma__g_def_ctx_buf, 4)
	IMPORT(cma__g_kernel_critical, 4)

	.rdata

/*
 * This is the default value of the FPA control/status register
 * (Actually, this is my best guess):  No pending or past exceptions,
 * cleared condition bit, all traps enabled, round-to-nearest mode
 */
	EXPORT(cma__c_default_ps)
	.word	0x00000f80

	.data
#ifdef _CMA_TRACE_KERNEL_
	.align	2
filename: .ascii "cma_assem.s\000"
#endif
	.text	
	.align	2

#if !_CMA_THREAD_IS_VP_
/*
 * cma__create_thread
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	 This routine initializes a thread (ie, its stack and context) and
 *	 prepares it for the scheduler
 *
 * FORMAL PARAMETERS:
 * 
 *	 a0	- the address of the child context buffer
 *	 a1	- the address of the base of the child thread stack
 *	 a2	- the address of the child TCB
 *	 a3	- the address of the start routine entry point
 *	 C_ARG(sp)  - the value of parameter to be passed to the routine
 * 
 * IMPLICIT INPUTS:
 * 
 *	 None
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	 The child thread stack is initialized with a simulated call frame for
 *	 the call to cma__execute_thread and a context block which will cause
 *	 the new thread to begin execution at the start of that routine.  We
 *	 don't bother to initialize any of the other registers.
 *
 * FUNCTION VALUE:
 * 
 *	 None 
 * 
 * SIDE EFFECTS:
 * 
 *	 None
 *
 *	 NOTE:  This routine assumes the stack is already longword aligned
 *
 *	 NOTE:  This routine assumes that any interruptions which occur while
 *		we are one the child's stack instead of this thread's own stack
 *		won't be concerned about it.
 *
 *	 NOTE: This routine currently assumes that this thread will not be
 *		preempted during the the call to cma__start_thread.  This will
 *		be a feature which is added later with priorities.  At that
 *		time we should be sure that the kernel is entered/exited
 *		correctly.
 *
 *	 NOTE: We don't copy the register-base parameters on to the stack
 *		because we don't use them after we call our subroutines.
 *
 * Frame contains:
 *	 two saved registers
 *	 no local variables
 *	 one subroutine parameter
 */
#undef	FSIZE
#define	FSIZE 12*4		/* Room for 10 args + 2 saved regs */
#undef	FOFFS
#define	FOFFS 1*4

#define C_ARG   (FSIZE+4*4)
#define C_SP	a1

ROUTINE(cma__create_thread)
	sw	s0, FSIZE-FOFFS-4(sp)
#if 1
	.cprestore FSIZE-FOFFS-8
	/*.cprestore FSIZE-FOFFS+4(sp)     /* restore the global pointer*/
#endif
	.mask	0x90010000, -FOFFS	/* M_RA+M_S0 */
/*
 * Prevent reschedule interruptions
 */
#ifdef _CMA_TRACE_KERNEL_
	sw	a0, FSIZE(sp)
	sw	a1, FSIZE+4(sp)
	sw	a2, FSIZE+8(sp)
	sw	a3, FSIZE+12(sp)
	li	a0, __LINE__
	la	a1, filename
	jal	cma__enter_kern_record
	lw	a0, FSIZE(sp)
	lw	a1, FSIZE+4(sp)
	lw	a2, FSIZE+8(sp)
	lw	a3, FSIZE+12(sp)
#else
# if _CMA_UNIPROCESSOR_
10:	lw	s0, cma__g_kernel_critical
	bnez	s0, 10b
	li	t0, 1
	sw	t0, cma__g_kernel_critical
# else
	sw	a0, FSIZE(sp)
	sw	a1, FSIZE+4(sp)
	sw	a2, FSIZE+8(sp)
	sw	a3, FSIZE+12(sp)
1:	la	a0, cma__g_kernel_critical
	CALL	cma__tas
	bnez	v0,1b
	lw	a0, FSIZE(sp)
	lw	a1, FSIZE+4(sp)
	lw	a2, FSIZE+8(sp)
	lw	a3, FSIZE+12(sp)
# endif
#endif
/*
 * Initialize the new thread's stack for "call" to cma__execute_thread.
 */
	subu	C_SP, 5*4	/* This is space for the argument list. */

/*
 * Build the parameter list for cma__execute_thread on the stack.
 * Technically the first four argments should be passed in registers,
 * but cma__transfer_thread_ctx doesn't restore volatile registers, like
 * the arguments registers.  Since there is room reserved on the stack
 * for these arguments anyway, put them there, and we'll make sure that
 * cma__execute_thread looks for them there instead of in the registers.
 */

/*
 * Store the TCB address in a0 and the start routine address in a1, and
 * Load the start routine argument and store it in a2
 */
	sw	a2, 0(C_SP)
/*
 * Set up a context area on the stack so that cma__restore_thread_ctx
 * will do the right thing.
 */
	/* subu	C_SP, EF_SIZE	 /* This is space for the context area. */
	subu	C_SP, EF_NREGS*4 /* This is space for the context area. */
	sw	C_SP, 0(a0)	/* Store sp in static context buffer */
/*
 * The rest of the stack frame is built by cma__execute_thread
 */

/*
 * Set up "ra" in the context buffer so that the new thread "resumes"
 * execution at the beginning of cma__execute_thread.
 */
	la	t0, cma__execute_thread /* Starting PC */
	sw	t0, EF_RA*4(C_SP)	/* Store PC in ra in context area */
	sw	t0, EF_T9*4(C_SP)	/* Store PC in ra in context area */

/*
/*
 * The child thread is now ready to run, so inform the scheduler
 * NOTE:  The parent may or may not be preempted by the child here.
 */
	move	a0, a2
	CALL	cma__start_thread

/*
 * We can now be rescheduled
 */

#ifdef _CMA_TRACE_KERNEL_
	li	a0, __LINE__
	la	a1, filename
	jal	cma__exit_kern_record
#else
	CALL	cma__undefer
	sw	zero, cma__g_kernel_critical
#endif
	lw	s0, FSIZE-FOFFS-4(sp)	/* Restore saved register */

	RETURN
END(cma__create_thread)
#endif		/* if !_CMA_THREAD_IS_VP_ */

/*
 * cma__do_call_on_stack
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	 This routine calls the specified routine on the specified stack,
 *	 passing the specified parameter.
 *	 
 * FORMAL PARAMETERS:
 * 
 *	 a0	-  of base of target stack
 *	 a1	-  address of a routine entry point
 *	 a2	-  of parameter to be passed to the routine
 * 
 * IMPLICIT INPUTS:
 * 
 *	 None
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	 None
 *
 * FUNCTION VALUE:
 * 
 *	 Value returned by the supplied routine, if any
 * 
 * SIDE EFFECTS:
 *       currently wreaks havoc on the stack
 * 
 *	 None
 *
 * Frame contains:
 *	 two saved registers
 *	 no local variables
 *	 one subroutine parameter
 */
#undef	FSIZE
#define	FSIZE 4*4
#undef	FOFFS
#define	FOFFS 1*4

RROUTINE(cma__do_call_on_stack)
	sw	s0, FSIZE-FOFFS-4(sp)	/* Protocol requires saving this reg */
	.mask	0x90010000, -FOFFS	/* M_RA+M_S0 */

/*
 * Switch to specified stack
 */
	move	s0, sp		/* Store the current sp */
	subu	a0, a0, 4	/* Compute new stack, with space for arg */
	move	sp, a0		/* Set to new stack */

/*
 * Call the specified routine
 */
	move	a0, a2		/* Set up parameter list */
	move    t9, a1          /* move address to t9  */
	CALL	t9		/* Call users routine */

/*
 * Switch back to old stack, pop the frame, and return
 */
	move	sp, s0			/* Restore the sp */
	lw	s0, FSIZE-FOFFS-4(sp)	/* Restore saved register */

	RETURN
END(cma__do_call_on_stack)

/*
 * cma__execute_thread
 *
 * FUNCTIONAL DESCRIPTION:
 *
 * 	This is the routine which occupies the frame at the base of the stack.
 *	It calls the user's "start routine", it holds the catch-all handler
 *	(set up in cma__create_thread), and it calls the thread cleanup
 *	routines after the "start routine" returns.
 *
 *	In the event of an exception, the catch-all handler may unwind back to
 *	this routine, write the reason for termination in -4(fp) [the exit
 *	status], and resume this routine to run down the thread.
 *
 * FORMAL PARAMETERS:
 * 
 *	a0, TCB(sp)	   - the address of the thread tcb
 *	a1, START_RTN(sp)  - the address of the start routine entry point
 *	a2, START_ARG(sp)  - the value of parameter to be passed to the routine
 * 
 * IMPLICIT INPUTS:
 * 
 *	 None
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	 None
 *
 * FUNCTION VALUE:
 * 
 *	 None (This function never returns)
 * 
 * SIDE EFFECTS:
 *
 *	 Note:  This routine is never explicitly called:  A call frame for it
 *		and a thread context buffer pointing to it are built by
 *		cma__create_thread.  When the thread is scheduled, it begins
 *		executing at the first instruction.  The parameters are
 *		"passed" to this routine in the stack frame, placed there by
 *		cma__create_thread, not in the argument registers.
 *
 *	 NOTE:  This routine DOES NOT RETURN: the call to cma__thread_base
 *		results in thread termination (eventually).
 *
 * Frame contains:
 *	 one saved register
 *	 one local variable (EXIT_STAT)
 *	 three subroutine parameters
 */
#undef	FSIZE
#define	FSIZE 12*4	/* Room for 10 args */
#undef	FOFFS
#define	FOFFS 2*4

#define TCB	    (FSIZE+0*4)
#define START_RTN   (FSIZE+1*4)
#define START_ARG   (FSIZE+2*4)
#define ARG_T9      (FSIZE+4*4)
#define ARG_GP      (FSIZE+3*4)

#define EXIT_STAT   (FSIZE-1*4)

ROUTINE(cma__execute_thread)
	.mask	M_RA|M_T9 , -FOFFS		/* Just save ra */

	lw	a0, TCB(sp)		/* Load TCB pointer & routine */
	CALL	cma__thread_base	/* Call thread base routine */
1:      
	j 1b
	/* break	2			/* Do not continue execution! */
END(cma__execute_thread)

#undef	FSIZE
#define	FSIZE 12*4	/* Room for 10 args */
#undef	FOFFS
#define	FOFFS 2*4

#define TCB	    (FSIZE+0*4)
#define START_RTN   (FSIZE+1*4)
#define START_ARG   (FSIZE+2*4)

#define EXIT_STAT   (FSIZE-1*4)




#if !_CMA_THREAD_IS_VP_
/*
 * cma__restore_thread_ctx
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	 This routine restores all context associated with a thread which is
 *	 necessary for scheduling it on the current processor.
 *
 *	 This routine causes execution to resume in the target thread, and
 *	 therefore does not return to the calling routine.
 *	 
 * FORMAL PARAMETERS:
 * 
 *	 a0	-  address of the buffer which holds the static context
 * 
 * IMPLICIT INPUTS:
 * 
 *	 None
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	 The main CPU general registers ($1-$31), the HI and LO registers, 
 *	 the PC (sort of), the CP1 (FPA) registers ($f0-$f31), and the FPA
 *	 control/status register (FCR31).
 *
 * FUNCTION VALUE:
 * 
 *	 None  (This routine does not return to the caller.)
 * 
 * SIDE EFFECTS:
 * 
 *	 NOTE:  This routine DOES NOT RETURN in the typical sense.
 */
NOGP_LEAF(cma__restore_thread_ctx)

/*
 * Now, transfer to the new thread (ie, switch to its stack).
 */
	lw	sp, 0(a0)		/* Load the new sp from the TCB */

/*
 * Restore the values in all the registers that we are required to
 * preserve across subroutine calls
 */
	lw	s0,  EF_S0*4(sp)
	lw	s1,  EF_S1*4(sp)
	lw	s2,  EF_S2*4(sp)
	lw	s3,  EF_S3*4(sp)
	lw	s4,  EF_S4*4(sp)
	lw	s5,  EF_S5*4(sp)
	lw	s6,  EF_S6*4(sp)
	lw	s7,  EF_S7*4(sp)
#ifndef fp
	lw	s8,  EF_S8*4(sp)
#else
	lw	fp,  EF_FP*4(sp)
#endif
#if 0   /* for the time being....@!*/
/*
 * We shouldn't have to synchronize with the FPC.  That should be done
 * either by the save-context routine or by the terminate thread
 * routine.
 */
	l.d	$f20, _JB_F20*4(sp)
	l.d	$f22, _JB_F22*4(sp)
	l.d	$f24, _JB_F24*4(sp)
	l.d	$f26, _JB_F26*4(sp)
	l.d	$f28, _JB_F28*4(sp)
	l.d	$f30, _JB_F30*4(sp)
#endif /* for the if 0  */
/*
 * Load the return address so we know where to go back to
 * and load the t9 and gp registers.
 */
	lw	ra,  EF_RA*4(sp)
        lw      t9,  EF_T9*4(sp)

/*
 * Remove context area from the stack
 */
	move	a0, sp
	addu	a0, EF_NREGS*4
	move	sp, a0

/*
 * Now simply return.  There is no frame to pop, the return address is
 * already in "ra"
 */
	j	ra

END(cma__restore_thread_ctx)

/*
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine restores the main thread context, and calls
 *	cma_abort_process.
 *
 * FORMAL PARAMETERS:
 * 
 *	None:  this routine is never actually called
 * 
 * IMPLICIT INPUTS:
 * 
 *	None
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	None
 *
 * FUNCTION VALUE:
 * 
 *	None
 * 
 * SIDE EFFECTS:
 * 
 *	None
 */


/*
 * cma__transfer_thread_ctx 2
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	 This routine stores the context of one thread and restores the context
 *	 of another, thus performing an actual context switch.  This routine is
 *	 used only for synchronous context switches (but the context which it
 *	 restores may have been synchronously or asynchronously saved).
 *
 *	 The newly restored thread resumes processing with the instruction
 *	 following it's last call to "cma__save_thread_ctx" (really
 * 	 cma__create_thread) or cma__transfer_thread_ctx, ie, this routine
 *	 returns directly to that instruction.
 *
 * FORMAL PARAMETERS:
 * 
 *	 a0  -  address of current thread's static context buffer
 *	 a1  -  address of new thread's static context buffer
 * 
 * IMPLICIT INPUTS:
 * 
 *	 The main CPU general registers ($1-ra), the HI and LO registers, 
 *	 the PC (sort of), the CP1 (FPA) registers ($f0-$f31), and the FPA
 *	 control/status register (FCR31).
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	 The main CPU general registers ($1-ra), the HI and LO registers, 
 *	 the PC (sort of), the CP1 (FPA) registers ($f0-$f31), and the FPA
 *	 control/status register (FCR31).
 *
 * FUNCTION VALUE:
 * 
 *	 None
 * 
 * SIDE EFFECTS:
 * 
 *	 NOTE:  When this routine returns, it is in a different thread...
 */
NOGP_LEAF(cma__transfer_thread_ctx)

/*
 * Make room on the stack and create a couple of scratch registers
 */
	subu	sp, EF_NREGS*4	/* Make room on the stack for the context */

/*
 * Store the values in all the registers that we are required to
 * preserve across subroutine calls
 */
	sw	s0,  EF_S0*4(sp)
	sw	s1,  EF_S1*4(sp)
	sw	s2,  EF_S2*4(sp)
	sw	s3,  EF_S3*4(sp)
	sw	s4,  EF_S4*4(sp)
	sw	s5,  EF_S5*4(sp)
	sw	s6,  EF_S6*4(sp)
	sw	s7,  EF_S7*4(sp)
#ifndef fp
	sw	s8,  EF_S8*4(sp)
#else
	sw	fp,  EF_FP*4(sp)
#endif

/*
 * I assume we don't need to synchronize with the FPC since this is a
 * normal subroutine call
 */
#if 0   /* for the time being ...@!*/
	s.d	$f20, _JB_F20*4(sp)
	s.d	$f22, _JB_F22*4(sp)
	s.d	$f24, _JB_F24*4(sp)
	s.d	$f26, _JB_F26*4(sp)
	s.d	$f28, _JB_F28*4(sp)
	s.d	$f30, _JB_F30*4(sp)
#endif
/*
 * Save the restart PC  and the t9 register...
 */
	sw	ra,  EF_RA*4(sp)
	sw      t9,  EF_T9*4(sp)

/*
 * Store the sp in the TCB 
 */
	sw	sp, 0(a0)

/*
 * And go restore the new thread's context
 */
	move	a0, a1		/* Context to restore */
	j	cma__restore_thread_ctx

/*
 * And, cma_restore_thread_ctx takes care of the rest  (There is no frame
 * to pop, and the new return address is stored in the context buffer.)
 */
END(cma__transfer_thread_ctx)

/*
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine is responsible for initiating asynchronous alert
 *	delivery.  It calls a routine (written in C) to actually raise the
 *	alert exception, to ensure that the raise is always consistent with
 *	the CMA exception package implementation on the platform (the extra
 *	call is probably not a serious performance issue in the context of an
 *	alert which will usually terminate the thread anyway).
 *
 *	This routine is never actually CALLed, rather it is "jumped" into by
 *	placing the entry point pc, "cma__do_async_alert", into the
 *	resume pc in a signal context block.
 *
 *	NOTE:  Therefore, this routine runs in the same frame as the routine
 *	       which was interrupted by the signal, but not at signal level.
 *
 * FORMAL PARAMETERS:
 * 
 *	None:  this routine is never actually called
 * 
 * IMPLICIT INPUTS:
 * 
 *	None
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	None
 *
 * FUNCTION VALUE:
 * 
 *	None
 * 
 * SIDE EFFECTS:
 * 
 *	None
 */

#endif		/* if !_CMA_THREAD_IS_VP_ */

/* Functional Description:
** a0 - argument to a2 (cma__abort_process)
** a1 - stack to switch to (main context stack pointer)
** a2 - function to jump to (cma__abort_process).
*/
NOGP_LEAF(cma__pyrmips_abort)
   move    sp,     a1      /* main thread stack pointer */
   j       a2
   .set noreorder
   nop
   .set reorder
   break   2
END(cma__pyrmips_abort)

#if 0  /* this is for mach:  see cma_vp.c */
/*
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine is responsible for delivering an async call from the
 *	virtual processor interface cma__vp_interrupt. It's not actually
 *	called, although the stack has been set up so that it behaves as a
 *	call. For convenience, the registers are set up so that the address
 *	to be called is in s0 (callee-saved register), and the argument to
 *	that routine is in a0 (the first argument register), so this code
 *	doesn't need to move anything around before the call.
 *
 * FORMAL PARAMETERS:
 * 
 *	none
 * 
 * IMPLICIT INPUTS:
 * 
 *	a0	interrupt argument
 *	s0	interrupt handler address
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	None
 *
 * FUNCTION VALUE:
 * 
 *	None
 * 
 * SIDE EFFECTS:
 * 
 *	None
 */
	.ent	cma__do_interrupt
EXPORT(cma__do_interrupt)
	CALL	s0			/* Deliver the alert */
	lw	ra, 0(sp)		/* Restore the return address */
	lw	s0, 4(sp)		/* Restore involuntary caller's s0 */
	lw	a0, 8(sp)		/* Restore old a0, too */
	addu	sp, 12			/* Restore old sp */
	j	ra			/* Return to involuntary caller */
END(cma__do_interrupt)	
#endif  /* endif for mach segment */
/*
 * cma__fetch_sp
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine returns the current value of sp (the stack pointer).
 *
 * FORMAL PARAMETERS:
 * 
 *	none
 * 
 * IMPLICIT INPUTS:
 * 
 *	none
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	none
 *
 * FUNCTION VALUE:
 * 
 *	the current value of sp
 * 
 * SIDE EFFECTS:
 * 
 *	none
 */
NOGP_LEAF(cma__fetch_sp)
	move	v0, sp
	j	ra
END(cma__fetch_sp)

/*
 * cma__fetch_gp
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine returns the current value of gp (the global pointer).
 *
 * FORMAL PARAMETERS:
 * 
 *	none
 * 
 * IMPLICIT INPUTS:
 * 
 *	none
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	none
 *
 * FUNCTION VALUE:
 * 
 *	the current value of sp
 * 
 * SIDE EFFECTS:
 * 
 *	none
 */
NOGP_LEAF(cma__fetch_gp)
	move	v0, gp
	j	ra
END(cma__fetch_gp)

/*
 * cma__do_break
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine causes a breakpoint. It's used to implement "set visible"
 *	on a VP implementation (not used for uniprocessors). cma__vp_interrupt
 *	is used to cause the target thread to execute this function.
 *
 * FORMAL PARAMETERS:
 * 
 *	none
 * 
 * IMPLICIT INPUTS:
 * 
 *	none
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	none
 *
 * FUNCTION VALUE:
 * 
 *	none
 * 
 * SIDE EFFECTS:
 * 
 *	none
 */
NOGP_LEAF(cma__do_break)
	break	2
	j	ra
END(cma__do_break)

#if 0  /*  #if _CMA_THREAD_IS_VP_   */

	.set	noreorder

/*
 * cma__tas
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine performs an atomic test-and-set operation on the data
 *	at a specified address.  The value is set (to its own address), and
 *	the previous value is returned.
 *
 * FORMAL PARAMETERS:
 * 
 *	a0	Address of an int on which to perform test-and-set function
 * 
 * IMPLICIT INPUTS:
 * 
 *	none
 * 
 * IMPLICIT OUTPUTS:
 * 
 *	none
 *
 * FUNCTION VALUE:
 * 
 *	v0 == 0 if word was previously clear, non-zero if previously set.
 * 
 * SIDE EFFECTS:
 * 
 *	0(a0) is always set to the (initial) value of a0 (its own address)
 *	when this returns.
 */
LEAF(cma__tas)
	lw	v0, 0(a0)
	nop
	bnez	v0, 10f
	nop
	.word	tas_op		/* Returns previous 0(a0) in a0 */
10:	j	ra
	move	v0, a0
END(cma__tas)

	.set	reorder

#endif

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S */
/*  *17    5-NOV-1991 14:58:09 BUTENHOF "[-.mips]" */
/*  *16   29-OCT-1991 12:11:04 BUTENHOF "Remove spurious declaration" */
/*  *15   14-OCT-1991 13:34:06 BUTENHOF "Fix TIN compilation" */
/*   12A1  3-SEP-1991 16:57:00 BUTENHOF "Fix BL7 bug" */
/*  *14    3-SEP-1991 10:28:48 BUTENHOF "More changes for OSF shareable library" */
/*  *13   22-AUG-1991 14:10:26 BUTENHOF "Fix do_call_on_stack code for Tin ld" */
/*  *12   11-JUN-1991 16:33:22 BUTENHOF "Make appropriate trace kernel calls" */
/*  *11   10-JUN-1991 18:08:54 SCALES "Add sccs headers for Ultrix" */
/*  *10   16-MAY-1991 14:09:21 BUTENHOF "Restore s0 in cma__create_thread()" */
/*  *9    15-MAY-1991 13:22:07 BUTENHOF "Change TAS" */
/*  *8    14-MAY-1991 16:59:48 BUTENHOF "Remove traces" */
/*  *7    14-MAY-1991 13:57:29 BUTENHOF "Change test-and-set" */
/*  *6     3-MAY-1991 11:26:54 BUTENHOF "Change test-and-set" */
/*  *5    12-APR-1991 23:33:47 BUTENHOF "OSF/1 Mach thread support" */
/*  *4     8-APR-1991 20:33:20 CURTIN "added cma__transfer_main_abort" */
/*  *3     3-APR-1991 15:59:32 BUTENHOF "Modify to support generic OSF/1 build" */
/*  *2    12-DEC-1990 20:33:29 BUTENHOF "Fix assem include, and clean up CMS history" */
/*  *1    12-DEC-1990 18:57:46 BUTENHOF "MIPS specific assem file" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S */
