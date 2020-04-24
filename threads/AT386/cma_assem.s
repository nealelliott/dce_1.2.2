/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_assem.s,v $
 * Revision 1.1.10.2  1996/03/09  23:30:26  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:45:44  marty]
 *
 * Revision 1.1.10.1  1995/12/07  21:42:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:09  root]
 * 
 * Revision 1.1.8.1  1994/06/09  13:33:24  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:24:43  devsrc]
 * 
 * Revision 1.1.6.2  1993/05/24  20:49:13  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:16:33  cjd]
 * 
 * Revision 1.1.4.2  1993/05/12  13:57:39  jd
 * 	Initial 486 port.
 * 	[1993/05/12  13:56:16  jd]
 * 
 * $EndLog$
 */



/*
 * A thread context consists of the saved register set necessary for
 * rescheduling the thread on another processor.
 *
 * There are two kinds of context switches, synchronous, and
 * asynchronous. The first is a result of a direct call to
 * cma__transfer_thread_ctx. The second is due to an asynchronous
 * event such as expiration of time slice.
 *
 * In the synchronous case only general registers that are valid 
 * across a subroutine call are saved. These are ebx, esi, edi. 
 * They are pushed onto the threads stack. The two registers
 * needed to reconstruct the threads stack context during the 
 * restore can not be kept on the stack. They are saved in the
 * TCB's static context structure. 
 *
 * In the asynchronous case the signal handler will have saved 
 * the volatile registers in the TCB'a asynchronous context
 * structure. It will also have replaced the ip that was
 * saved in the signal's sigcontext structure with the first
 * instruction in cma__force_dispatch. After the signal handler
 * returns through sigreturn execution will resume at 
 * cma__force_dispatch. All context for the running thread  
 * at the time of the signal that was not expicitly stored
 * in the asynchrous context structure by the signal handler is
 * now valid for the thread that was interrupted. Taking care
 * not to destroy any of the threads registers, volatile
 * as well as non volatile, a complete sigcontext is stored
 * on the threads stack. The thread then yields the processor
 * through a call to cma__yield_processor. The thread will resume
 * execution on return from this procedure. At that time a call
 * to sigretrun will properly restore the threads context at the
 * time of the original signal from the sigcontext structure on
 * it's stack.
 */	

#include <machine/asm.h>
#include <syscall.h>
#include <dce/cma_host.h>


	.globl	EXT(cma__bugcheck)
	.globl	EXT(cma__get_async_info)
	.globl	EXT(cma__start_thread)
	.globl	EXT(cma__thread_base)
	.globl	EXT(cma__undefer)
	.globl	EXT(cma__yield_processor)

	.data
#ifdef _CMA_TRACE_KERNEL_
filename: .ascii "cma__assem.s\000"
#endif
create_thread_bug:	.ascii	"create_thread:1\000"
async_alert_bug:	.ascii	"do_async_alert:1\000"
transfer_main_abort_bug:	.ascii	"transfer_main_abort:1\000"
	.text	

#if !_CMA_THREAD_IS_VP_
/*
 * cma__create_thread
 *
 * FUNCTIONAL DESCRIPTION
 *
 * This procedure initializes a threads stack and context and prepares
 * it for scheduling.
 *
 * FORMAL PARAMETERS
 */

#define  STATIC_CONTEXT  	4 
#define  STACK_BASE  		8
#define  CONTEXT_BUFFER 	12

/*
 * IMPLICIT INPUTS
 *
 * None
 *
 *
 * IMPLICIT OUTPUTS
 *
 * Nothing returns to the caller. The child's stack will be initialized
 * to cause it to begin execution at the instruction imediately following
 * the call to cma__execute_thread in this procedure. At that time the 
 * child's stack will already be set up for the call to cma__thread_base
 * to get it started at it's start routine entry point.  
 *
 * FUNCTION VALUE
 *
 * None
 * 
 * SIDE EFFECTS
 *
 * None
 *
 *
 */

ENTRY(cma__create_thread)

/
/ We do not want to be rescheduled untill we finish this.
/

	movl	$1,%eax
1:	xchg	%eax,EXT(cma__g_kernel_critical)
	test	$1,%eax
	jnz	1b

/
/ Save the parent's stack and store the child's static context buffer
/ in edx so we'll have access to while running on the child's stack.
/

	movl	STATIC_CONTEXT(%esp),%edx
	movl	%esp,%eax
	
	
/
/ Switch to the childs stack.
/

	movl	STACK_BASE(%esp),%ecx
	movl	%ecx,%esp

/
/ Build the parameter list for cma__execute_thread.
/

	movl	CONTEXT_BUFFER(%eax),%ecx
	pushl	%ecx

	call	EXT(cma__execute_thread)

/
/ Both the parent and the child get to this point. The parent
/ returns from cma__excute_thread after placing a 1 in eax. The 
/ child gets here through cma__restore_thread_ctx. That procedure,
/ as a security precaution, clears all the volatile registers. 
/ We can therefore determine what context we are in at this point 
/ by checking eax. The parent wants to return to the caller and 
/ the child needs to go through cma__thread_base to get started.
/

	cmpl	$0,%eax
	jne	1f	

/
/ The parameters for cma__thread_base are already on the stack.
/ Call it to start up the thread. 
/	

	call	EXT(cma__thread_base)

/
/ cma_thread_base should never return.
/

	movl	%eax, create_thread_bug
	pushl	%eax
	call	EXT(cma__bugcheck)
	addl	$4,%esp

1:
/
/ Return to the parent.
/

	ret
#endif		/* if !_CMA_THREAD_IS_VP_ */


/*
 * cma__do_call_on_stack
 *
 * FUNCTIONAL DESCRITION
 *
 *	 This routine calls the specified routine on the specified stack,
 *	 passing the specified parameter.
 * 
 * FORMAL PARAMETERS
 */
#undef 	 STACK_BASE
#undef   START_ROUTINE
#undef   START_PARAMETER

#define  STACK_BASE    		4   	
#define  START_ROUTINE 		8	
#define  START_PARAMETER	12	

/*
 * IMPLICIT INPUTS
 *
 * None
 *
 * IMPLICIT OUTPUTS
 *
 * None
 *
 * FUNCTION VALUE 
 *
 *	 Value returned by the supplied routine, if any
 *
 * SIDE EFFECTS
 *
 * None
 */

ENTRY(cma__do_call_on_stack)
	
/
/ We need to store the current stack pointer in a non-volatile register.
/ Save the register. 
/

	pushl	%ebx
	movl	%esp,%ebx

/
/ Get the parameter and start routine off the current stack.
/

	movl	START_ROUTINE+4(%esp),%eax
	movl	START_PARAMETER+4(%esp),%edx

/ 
/ Go to the new stack.
/
	
	movl	STACK_BASE+4(%ebx),%esp

/
/ Put the parameter on the new stack.
/

	pushl	%edx

/
/ Do the call
/	

	call	*%eax

/
/ Remove the argument from the new stack so its the way we found it
/

	addl	$4,%esp
/
/ eax now contains the routines return value. Restore the original stack
/ and return.
/

	movl	%ebx,%esp
	popl	%ebx
	ret

/*
 * cma__execute_thread
 *
 * FUNCTIONAL DESCRIPTION
 *
 * This procedure gets called only from cma_thread_create. It
 * therefore need not be exported. It is entered on the stack 
 * of the child that cma_thread_create is creating. It sets 
 * the child up for restoration by cma__restore_thead_ctx. It 
 * then switches back to the parents stack and hands the childs
 * thread to the schedular through a call to cma__start_thread.
 * The parent then returns a 1 to cma_thread_create through eax.
 * The child will also return to cma_create_thread when it
 * gets scheduled but it will return via cma__restore_thread_ctx
 * which clears all the volatile registers including eax. 
 *
 * FORMAL PARAMETERS
 */
 
#undef   CONTEXT_BUFFER
#define  CONTEXT_BUFFER 	4

/* 
 *
 * IMPLICIT INPUTS
 *
 * %edx - static context buffer
 * %eax - new stack pointer
 *
 * IMPLICIT OUTPUTS
 *
 * The childs stack is set up for restoration by 
 * cma__restore_thread_ctx.
 * 
 * FUNTION VALUE
 * 
 * returns 1.
 *
 * SIDE EFFECTS
 *
 * The child does not return directly from this procedure. It 
 * returns instead via cma__restore_thread_ctx when it gets 
 * scheduled to run.
 *
 */

ENTRY(cma__execute_thread)

/
/ Need to save the return address to cma__create_thread from the childs
/ stack so we can put it on the parents stack.
/

	movl	(%esp),%ecx
	
/
/ Save the non volatile registers on the stack.
/

	pushl	%ebx
	pushl	%esi	
	pushl   %edi	
	
/
/ Save the stack pointer and base pointer away in the threads
/ static context buffer.
/

	movl	%esp,(%edx)
	movl	%esp,4(%edx)

/
/ The new threads stack is now correct for being executed by
/ cma__restore_thread. Switch back to the current threads stack
/ and pass the new thread context off to the scheduler.
/

	movl	%eax,%esp

/
/ Now push the parents return address so it can get back to 
/ cma__create_thread.
/

	pushl	%ecx

/
/ Give the child to the schedular.
/

	movl	16(%esp),%eax		
	pushl	%eax
	call	EXT(cma__start_thread)	
	addl	$4,%esp

/ 
/ The new thread is all set and we are back on our own stack so lets
/ let ourselves be rescheduled.
/

	call	EXT(cma__undefer)
	movl	$0,EXT(cma__g_kernel_critical)

/
/ A 1 in eax tells cma_create_thread that we are returning from this
/ procedure in the current threads context and we should return to
/ the caller.
/

	movl	$1,%eax
	ret

#if !_CMA_THREAD_IS_VP_

/*
 * cma__restore_thead_ctx
 *
 * FUNCTIONAL DESCRIPTION
 *
 * This procedure restores the context of the next thread to run. The
 * thead will resume execution at the ip at the time of its last call
 * to cma__transfer_thread_ctx. cma__transfer_thread_ctx only saved
 * the volatile registers, therefore this procedure only restores those
 * registers.
 *
 * FORMAL PARAMETERS
 */

#define NEXT_SCTX  4   /* Static context buffer of the next thread to run */

/*
 * IMPLICIT INPUTS
 * 
 * None
 *
 * IMPLICIT OUTPUTS
 *
 * The non volatile registers of the next thread to run.
 *
 * FUNCTION VALUES
 *
 * None
 *
 * SIDE EFFECTS
 * 
 * This procedure does not return it the normal sense. It return instead
 * to the ip at the time of the last call to cma__transfer_thread.
 */

ENTRY(cma__restore_thread_ctx)


/
/ Restore the threads stack context.
/

	movl	NEXT_SCTX(%esp),%eax
	movl	(%eax),%esp
	movl	4(%eax),%ebp

/
/ Now we are running on the next threads stack and its volatile registers
/ were saved there. Restore them.
/	

	popl	%edi
	popl	%esi
	popl	%ebx

/
/ Security considerations dictate that no thread will be given
/ information on another threads context without the latters
/ explicite permission. As innocent as the contents of the volatile
/ registers may seem, it is information on the last threads context.
/ Zero these registers.
/

	movl	$0,%eax
	movl	$0,%edx
	movl	$0,%ecx	

/
/ This threads non volatile context is now exactly as it was on its last
/ entry to cma__transfer_thread_ctx. We need only do a return at this
/ point.
/

	ret

/*
 * cma__transfer_main_abort
 *
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
ENTRY(cma__transfer_main_abort)
	movl	%eax, EXT(cma__g_def_ctx_buf)
	movl	%esp,(%eax)
	movl	%ebp,4(%eax)	
	movl	%eax, EXT(cma__g_abort_signal)
	pushl	%eax
	call	EXT(cma__abort_process)
	addl	$4,%esp

	movl	%eax, transfer_main_abort_bug
	pushl	%eax
	call	EXT(cma__bugcheck)	
	addl	$4,%esp

	ret

/*
 * cma__transfer_thread_ctx
 * 
 * FUNCTION DESCRIPTION
 * 
 * This procedure stores the context of a thread then performs a call 
 * to cma_restore_thread_ctx to cause another thread to execute. Its
 * only entered as a subroutine call, therefore only need concern itself
 * with saving the synchronous context.
 *
 * FORMAL PARAMETERS
 */

#undef	NEXT_SCTX
#define	CURRENT_SCTX 	4    /* Current threads static context buffer */
#define NEXT_SCTX	8    /* Static context buffer of next thread to run */

/*
 * IMPLICIT INPUTS
 *
 * The non volatile cpu register to be saved on the current threads
 * stack.
 *
 * IMPLICIT OUTPUTS
 *
 * The non volatile cpu register to be saved on the current threads
 * stack.
 *
 * FUNCTION VALUE
 * 
 * None
 *
 * SIDE EFFECTS
 *
 * This routine never returns. cma_restore_thread_ctx will cause execution
 * to resume in the new threads context.
 */

ENTRY(cma__transfer_thread_ctx)
	
/
/
/ Save the non volatile registers.
/

	pushl	%ebx
	pushl	%esi
	pushl	%edi
/
/ Save the stack context in the current threads static context buffer.
/

	movl	CURRENT_SCTX+12(%esp),%eax
	movl	%esp,(%eax)
	movl	%ebp,4(%eax)	

/
/ Go restore the context of the next thread to run.
/

	movl	NEXT_SCTX+12(%esp),%eax
	pushl	%eax
	call	EXT(cma__restore_thread_ctx)
	addl	$4,%esp

	ret

/*
 * FUNCTIONAL DESCRIPTION
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
 * FORMAL PARAMETERS
 * 
 *	None:  this routine is never actually called
 * 
 * IMPLICIT INPUTS
 * 
 *	None
 * 
 * IMPLICIT OUTPUTS
 * 
 *	None
 *
 * FUNCTION VALUE
 * 
 *	None
 * 
 * SIDE EFFECTS
 * 
 *	None
 */

ENTRY(cma__do_async_alert)
	call	EXT(cma__undefer)
	movl	$0, EXT(cma__g_kernel_critical)

	call	EXT(cma__async_delivery)
/ This point should never be reached, since the CMA exception package doesnt
/ support continuable exceptions.
/

	movl	%eax, async_alert_bug
	pushl	%eax
	call	EXT(cma__bugcheck)	
	addl	$4,%esp
	ret
#endif		/* if !_CMA_THREAD_IS_VP_ */

/*
 * cma__do_interrupt
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine is responsible for delivering an async call from the
 *	virtual processor interface cma__vp_interrupt. Its not actually
 *	called, although the stack has been set up so that it behaves as a
 *	call. For convenience, the registers are set up so that the address
 *	to be called is in s0 (callee-saved register), and the argument to
 *	that routine is in a0 (the first argument register), so this code
 *	doesnt need to move anything around before the call.
 *
 * FORMAL PARAMETERS:
 * 
 *	none
 * 
 * IMPLICIT INPUTS:
 * 
 *	%eax	interrupt argument
 *	%ebx	interrupt handler address
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
ENTRY(cma__do_interrupt)
	pushl	%eax
	call	*%ebx
	addl	$4,%esp
	popl	%ebx		/* Restore involuntary callers %ebx */
	popl	%eax		/* Restore old %eax, too */
	ret			/* Return to involuntary caller */

/* 
 * cma__force_dispatch
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine is responsible for initiating an asynchronous context
 *	switch.
 *
 *	First, we save all registers which might be altered by subroutine 
 *	calls in a context area on the stack.  Then, we call a routine
 *	(written in C) to retrieve from the TCB the ip at the time of
 *	interruption. This is written into the appropriate place in
 *	the context area on the stack.  Then, we yield the processor to
 *	another thread.  This results in a full context save before the new
 *	thread is scheduled.  When we are once again scheduled, a full context
 *	restore is performed in the process.  When control returns to this
 *	routine, we restore the volatile registers which we saved, and return.
 *
 *	This routine is never actually called, rather it is "jumped" into by
 *	placing the entry point  "cma__call_forced_dispatch", into the
 *	resume ip in the signal handlers sigcontext block.
 *
 *	NOTE:  Therefore, this routine runs in the same frame as the routine
 *		which was interrupted by the signal.
 *
 *	 	 
 * FORMAL PARAMETERS:
 * 
 *	 None:  this routine is never actually called
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
 *	 None:  this routine does not return, execution simply resumes at the
 *		point where it was interrupted by the timeslice.
 * 
 * SIDE EFFECTS:
 * 
 *	 None
 *
 *	 NOTE:  This routine is not actually called (it is jumped into)
 */

ENTRY(cma__force_dispatch)
/
/ The stack pointer is that of the thread that was interrupted by the 
/ signal. Build a full sigcontext frame on it. We can use eax, edx and
/ ecx since thier values are also in the asynchronous context structure 
/ for this thead.
/
 
/
/ Get the pointer to the asynchronous context structure for this thread.
/ The call may destroy the volatile registers but we will still have them
/ in the asynchronous structure itself.
/

	call	EXT(cma__get_async_info)	
	movl	%eax,%ecx
	
	movl	%esp,%eax
	pushl	%ss	
	pushl	%eax
	movl	A_EFLAGS (%ecx),%eax
	pushl	%eax
	pushl	%cs
	movl	A_EIP(%ecx),%eax
	pushl	%eax
/
/ Sigreturn isnt going to care about trapno or errno.
/

	pushl	$0
	pushl	$0
	movl	%esp,%edx
	movl	A_EAX(%ecx),%eax
	pushl 	%eax
	movl	A_ECX(%ecx),%eax
	pushl	%eax
	movl	A_EDX(%ecx),%eax
	pushl	%eax
	pushl	%ebx
	pushl	%edx
	pushl	%ebp
	pushl	%esi
	pushl	%edi
	pushl	%ds
	pushl	%es
	pushl	%fs
	pushl	%gs
/
/ We are not on the signal stack.
/

	pushl	$0
	movl	A_SIGMASK(%ecx),%eax
	pushl	%eax

	call	EXT(cma__yield_processor)
/
/ Let someone else run.
/

	movl	$0, EXT(cma__g_kernel_critical)
	pushl	%esp
	call	EXT(sigreturn)
	addl	$4,%esp
	ret

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

ENTRY(cma__fetch_sp)
	movl	%esp,%eax
	ret

#if _CMA_THREAD_IS_VP_
/*
 * cma__do_break
 *
 * FUNCTIONAL DESCRIPTION:
 * 
 *	This routine causes a breakpoint. Its used to implement "set visible"
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
ENTRY(cma__do_break)
	bpt
	ret

/*
 * cma__tas
 *
 * FUNCTIONAL DESCRIPTION
 * 
 * This procedure implements an atomic test and set of bit 0
 * in the long word pointed to by the agrument.
 *
 * FORMAL PARAMETERS
 */

#define  SET_ADDRESS  4

/*
 * IMPLICIT INPUTS
 * 
 * None
 *
 * IMPLICIT OUTPUTS
 *
 * None
 *
 * FUNCTION VALE
 *
 * Previous value in the address pointed to by the argument.
 *
 * 
 * SIDE EFFECTS
 *
 * None
 */

ENTRY(cma__tas)

	movl	$1,%eax
	movl	4(%esp),%edx
	cmpl	$0,%edx
	je	1f
	xchg	%eax,(%edx)
	andl	$1,%eax
1:
	ret
#endif		/* if !_CMA_THREAD_IS_VP_ */
