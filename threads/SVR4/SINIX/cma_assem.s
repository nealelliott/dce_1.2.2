/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_assem.s,v $
 * Revision 1.1.8.2  1996/03/09  23:30:50  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:46:06  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:53:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:02:12  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:14  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:37  devsrc]
 * 
 * Revision 1.1.3.4  1993/02/01  21:46:53  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:24:49  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:18:20  hinman]
 * 
 * Revision 9.6.1.4  92/10/01  10:09:14  blurie
 * 	No additional messages.
 * 
 * Revision 9.6.4.2  92/09/30  14:55:04  blurie
 * 	Remove assembler code for /usr/ces assembler.  We are not
 * 	supporting it anymore.
 * 
 * 	Fix up cma__create_thread so that the frame pointer in the
 * 	base of the child thread's stack is zero.  This is necessary
 * 	to conform to the SVR4 i386 ABI.
 * 
 * Revision 9.6.1.3  92/09/10  16:29:33  hinman
 * 	[hinman] - Fold forward from raj_om
 * 
 * Revision 9.6.2.2  92/09/10  15:52:40  raj
 * 	[raj] merging SVR4 changes into 1.0.2
 * 
 * Revision 9.6.1.2  92/08/05  12:48:33  hinman
 * 	[hinman] - Fold backward from DCE1TOP
 * 
 * Revision 2.1.13.3  92/07/31  11:34:42  blurie
 * 	Don't push three parameters before call to create_thread because
 * 	only the tcb is needed now.
 * 	[92/07/30  17:09:41  blurie]
 * 
 * Revision 9.1.1.1  92/07/19  12:54:41  hinman
 * 	Create maintenance branch
 * 
 * Revision 9.1  92/07/19  12:54:38  hinman
 * 	Create split point
 * 
 * Revision 2.1.13.2  92/01/17  16:16:05  blurie
 * 	No additional changes
 * 	[92/01/16  15:21:19  blurie]
 * 
 * Revision 2.1.11.2  91/11/27  10:36:34  hinman
 * 	[hinman for blurie] - Fold forward without merging
 * 	[91/11/27  10:29:20  hinman]
 * 
 * Revision 2.1.9.2  91/11/18  15:47:28  hinman
 * 	[hinman for blurie] - Fold forward without merge
 * 	[91/11/18  15:33:17  hinman]
 * 
 * Revision 2.1.7.2  91/11/13  15:59:24  blurie
 * 	Added code for cma__transfer_main_abort routine.
 * 	[91/11/13  14:09:44  blurie]
 * 
 * Revision 2.1.5.3  91/10/15  16:54:14  blurie
 * 	In assembly code for AT&T assembler, in routine cma__create_thread,
 * 	changed lines:
 * 
 * 		cmpl $1, %eax
 * 		jnz	 retry
 * 
 * 	to:
 * 		cmpl $0, %eax
 * 		jne	retry
 * 
 * 	because the jnz did not seem to be performing properly.
 * 	[91/10/14  14:32:40  blurie]
 * 
 * 	Added AT&T assembly code to module which is conditionally compiled
 * 	if the compiler is a Siemens compiler.  Included <dce/cma_config.h>
 * 	to get definition for compiler.
 * 	[91/09/30  13:16:02  blurie]
 * 
 * 	Changed merged from dce47.
 * 	[91/08/14  17:12:17  blurie]
 * 
 * Revision 2.1.5.2  91/08/14  17:28:17  blurie
 * 	Changed merged from dce47.
 * 	[91/08/14  17:12:17  blurie]
 * 
 * Revision 2.1.3.2  91/06/21  10:41:06  blurie
 * 	Ben's initial threads changes
 * 	[91/06/21  07:36:31  blurie]
 * 
 * Revision 2.1.1.4  91/05/06  15:36:54  hinman
 * 	[hinman] - Checkin version from freestanding snap3 tree
 * 	[91/05/06  09:33:54  hinman]
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
 * needed to reconstruct the threads stack context (ESP, EBP) during the 
 * restore cannot be kept on the stack. They are saved in the
 * TCB's static context structure. 
 *
 */	
/*
*	Because of differences between the AT&T assembly language and
*	the Intel assembly language, this module contains code for both
*	types.  If the compiler being used is a Siemens compiler, use
*	the Intel assembly language, otherwise use the AT&T assembly
*	language.
*/

#include <dce/cma_config.h>
#include <dce/cma_host.h>
#include <cma_host.h>

.file "cma_assem"
.text

/*
*	Functions import into this module
*/

/*
*	Functions exported from this module
*/

.globl cma__transfer_thread_ctx
.globl cma__restore_thread_ctx
.globl cma__test_and_set
.globl cma__unset
.globl cma__do_call_on_stack
.globl cma__fetch_sp
.globl cma__create_thread
.globl cma__do_async_alert
.globl cma__execute_thread
.globl cma__transfer_main_abort

/*
 * cma__create_thread
 *
 * FUNCTIONAL DESCRIPTION
 *
 * This procedure initializes a new thread's stack and context and prepares
 * it for scheduling.
 *
 * FORMAL PARAMETERS
 */

#define  STATIC_CONTEXT_OFFSET 	8 	/* new threads static context buffer */
#define  STACK_BASE_OFFSET  	12	/* new thread's stack base */
#define  CONTEXT_BUFFER_OFFSET 	16	/* new thread's tcb */
#define  START_ROUTINE_OFFSET 	20	/* new thread's start routine */
#define  START_PARAMETER_OFFSET	24	/* new thread's parameter */

/*
 * IMPLICIT INPUTS
 *
 * None
 *
 * IMPLICIT OUTPUTS
 *
 * The child's stack will be initialized
 * to cause it to begin execution at the instruction imediately following
 * the call to cma__execute_thread in this procedure. At that time, the 
 * child's stack will already be set up for the call to cma__thread_base
 * to get it started at it's start routine entry point.  
 *
 * FUNCTION VALUE
 *
 * None
 * 
 * SIDE EFFECTS
 *
 *	After calling cma__execute_thread(), the parent does not return to this
 *	function.  When the parent exits cma__execute_thread(), it returns 
 *	directly to the caller of this function.  This is because the call
 *	to cma__execute_thread() is made on the child's stack rather than
 *	the parent's stack.
 *
 */

ENTRY(cma__create_thread)

/*
/ We do not want to be rescheduled until we finish this, so indicate that
/ there is someone in the cma kernel.
*/
	movl	$1,%eax

retry:	
	xchg	cma__g_kernel_critical,%eax
	cmpl	$0,%eax
	jne		retry

/*
*	Save the parent's frame pointer on stack.
*/

	pushl %ebp

/*
/ Save the parent's stack in eax and store the child's static context buffer
/ in edx so we'll have access to while running on the child's stack.
*/

	movl	STATIC_CONTEXT_OFFSET(%esp),%edx
	movl	%esp,%eax
	
	
/*
/ Switch to the child's stack.
*/
	movl	STACK_BASE_OFFSET(%esp),%ecx
	movl	%ecx,%esp

/*
*	Set up the bottom frame for the child's stack.  The SVR4 386 ABI
*	says that the last frame pointer on the stack should be zero.
*/
	
	movl $0, %ebp
	pushl %ebp

/*
/ Build the parameter list for cma__execute_thread.
*/
	movl	START_PARAMETER_OFFSET(%eax),%ecx
	pushl	%ecx
	movl	START_ROUTINE_OFFSET(%eax),%ecx
	pushl	%ecx
	movl	CONTEXT_BUFFER_OFFSET(%eax),%ecx
	pushl	%ecx

/*
*	This call is being made on the child's stack.
*/

	call	cma__execute_thread

/*
/ The child gets here through cma__restore_thread_ctx. That procedure,
/ as a security precaution, clears all the volatile registers. 
/ We can therefore determine what context we are in at this point 
/ by checking eax.  The child needs to go through cma__thread_base 
/ to get started.
/ If eax does not contain 0, then an error has occured.
/ 
*/

	cmpl	$0,%eax
	jne		error_check	

/*
/ The parameters for cma__thread_base are already on the stack.
/ Call it to start up the thread. 
*/	
	call	cma__thread_base

error_check:

/*
/ cma_thread_base should never return.  The initial caller should not
/	return here either.
*/

	call	cma__bugcheck


/*
 * cma__execute_thread
 *
 * FUNCTIONAL DESCRIPTION
 *
 * This procedure gets called only from cma_thread_create(). 
 * It is entered on the stack 
 * of the child that cma_thread_create() is creating. It sets 
 * the child up for restoration by cma__restore_thread_ctx(). It 
 * then switches back to the parent's stack and hands the child's
 * thread to the scheduler through a call to cma__start_thread().
 * The parent returns directly to the caller of the cma_create_thread()
 * routine.  
 * The child will 'return' to cma_create_thread() when it
 * get's scheduled but it will return via cma__restore_thread_ctx()
 * which clears all the volatile registers including eax. 
 *
 * FORMAL PARAMETERS
 */
 
#undef   START_ROUTINE_OFFSET
#undef	 START_PARAMETER_OFFSET

/* 
 *
 * IMPLICIT INPUTS
 *
 * edx	-	Contains pointer to child's static context block.
 * eax	-	Contains parent's stack pointer.
 *
 * IMPLICIT OUTPUTS
 *
 * The child's stack is set up for restoration by 
 * cma__restore_thread_ctx().
 * 
 * FUNTION VALUE
 * 
 * returns 1.
 *
 * SIDE EFFECTS
 *
 * The child does not return directly from this procedure. It 
 * returns instead via cma__restore_thread_ctx when it get's 
 * scheduled to run.  
 * The parent is set up to return directly to the caller of 
 * cma_create_thread.
 *
 */

ENTRY(cma__execute_thread)
	
/*
/ Save the non volatile registers on the stack.
*/

	pushl	%ebx
	pushl	%esi	
	pushl  	%edi	
	
/*
/ Save the stack pointer and base pointer away in the thread's
/ static context buffer.
*/
	movl	%esp,S_SP(%edx)
	movl	%ebp,S_BP(%edx)

/*
/ The new thread's stack is now correct for being executed by
/ cma__restore_thread(). Switch back to the current thread's stack
/ and pass the new thread context off to the scheduler.
*/

	movl	%eax,%esp

	movl	CONTEXT_BUFFER_OFFSET(%esp),%eax

/*
*	Restore the current thread's frame pointer.
*/

	popl %ebp

	pushl	%eax
	call	cma__start_thread	
	popl	%eax


/* 
/ The new thread is all set and we are back on our own stack so let's
/ let ourselves be rescheduled.
*/

	call	cma__undefer
	movl	$0,cma__g_kernel_critical

/*
/ Return to the caller of cma_create_thread.
*/

	ret

/*
 * cma__transfer_thread_ctx
 * 
 * FUNCTION DESCRIPTION
 * 
 * This procedure stores the context of the thread calling this
 * routine and then performs a call 
 * to cma_restore_thread_ctx() to cause another thread to execute. It's
 * only entered as a subroutine call, therefore only need concern itself
 * with saving the synchronous context.
 *
 * FORMAL PARAMETERS
 */

#define	  CURRENT_SCTX 	16    /* Current threads static context buffer */
#define   NEXT_SCTX		20   /* Static context buffer of next thread to run */

/*
 * IMPLICIT INPUTS
 *
 * The non volatile cpu register to be saved on the current thread's
 * stack.
 *
 * IMPLICIT OUTPUTS
 *
 * The non volatile cpu register to be saved on the current thread's
 * stack.
 *
 * FUNCTION VALUE
 * 
 * None
 *
 * SIDE EFFECTS
 *
 * This routine never returns. cma_restore_thread_ctx will cause execution
 * to resume in the new thread's context.
 */

ENTRY(cma__transfer_thread_ctx)
	
/*
/
/ Save the non volatile registers.
*/
	pushl	%ebx
	pushl	%esi
	pushl	%edi

/*
/ Save the stack context in the current threads static context buffer.
*/
	movl	CURRENT_SCTX(%esp),%eax
	movl	%esp,S_SP(%eax)
	movl	%ebp,S_BP(%eax)

/*
/ Go restore the context of the next thread to run.
*/

	movl	NEXT_SCTX(%esp),%eax
	pushl	%eax
	call	cma__restore_thread_ctx

	ret

/*
 * cma_restore_thead_ctx
 *
 * FUNCTIONAL DESCRIPTION
 *
 * This procedure restores the context of the next thread to run. The
 * thead will resume execution at the IP at the time of its last call
 * to cma__transfer_thread_ctx(). cma__transfer_thread_ctx() only saved
 * the non volatile registers, therefore this procedure only restores those
 * registers.
 *
 * FORMAL PARAMETERS
 */

#undef  NEXT_SCTX
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
 * to the IP at the time of the last call to cma__transfer_thread.
 */

ENTRY(cma__restore_thread_ctx)


/*
/ Restore the thread's stack context.
*/
	movl	NEXT_SCTX(%esp),%eax
	movl	S_SP(%eax),%esp
	movl	S_BP(%eax),%ebp

/*
/ Now we are running on the next thread's stack and it's volatile registers
/ were saved there. Restore them.
*/	
	popl	%edi
	popl	%esi
	popl	%ebx

/*
/ Security considerations dictate that no thread will be given
/ information on another thread's context without the latter's
/ explicite permission. As innocent as the contents of the volatile
/ registers may seem, it is information on the last thread's context.
/ Zero these registers.
*/
	movl	$0,%eax
	movl	$0,%edx
	movl	$0,%ecx

/*
/ This threads non volatile context is now exactly as it was on it's last
/ entry to cma__transfer_thread_ctx. We need only do a return at this
/ point.
*/
	ret

/*
 * cma__test_and_set	
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
 * Bit zero of the argument is set to 1.
 *
 * FUNCTION VALUE
 *
 * Previous value in the address pointed to by the argument.
 *
 * SIDE EFFECTS
 *
 * None
 */

ENTRY(cma__test_and_set)

	movl	$1,%eax
	movl 	SET_ADDRESS(%esp),%ecx
	xchgl	(%ecx),%eax
	and		$1,%eax
	ret

/*
 * cma__unset	
 *
 * FUNCTIONAL DESCRIPTION
 * 
 * This procedure implements an atomic clear of bit 0
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
 * Bit 0 of the argument is set to 0.
 *
 * FUNCTION VALE
 *
 * None 
 * 
 * SIDE EFFECTS
 *
 * None
 */

ENTRY(cma__unset)

	movl	$0,%eax
	movl	SET_ADDRESS(%esp),%ecx
	xchg	(%ecx),%eax
	ret

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

#define  STACK_BASE    		8   	
#define  START_ROUTINE 		12	
#define  START_PARAMETER	16	

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
	
/*
/ We need to store the current stack pointer in a non-volatile register.
/ Save the register. 
*/

	pushl	%ebx
	movl	%esp,%ebx

/*
/ Get the parameter and start routine off the current stack.
*/

	movl	START_ROUTINE(%esp),%eax
	movl	START_PARAMETER(%esp),%edx

/* 
/ Go to the new stack.
*/
	
	movl	STACK_BASE(%esp),%esp

/*
/ Put the parameter on the new stack.
*/

	pushl	%edx

/*
/ Do the call
*/	

	call	*%eax

/*
/ Remove the argument from the new stack so it's the way we found it.
*/

	addl	$4,%esp
/*
/ eax now contains the routine's return value. Restore the original stack
/ and return.  This assumes that ebx was not trashed on the call.
*/

	movl	%ebx,%esp
	popl	%ebx
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
 *	resume pc in a sigcontext block.
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
	call	cma__undefer
	movl	$0,cma__g_kernel_critical

	call	cma__async_delivery

/*
/ This point should never be reached, since the CMA exception package doesn't
/ support continuable exceptions.
*/

	pushl	$0
	call	cma__bugcheck
	ret

/*
* FUNCTIONAL DESCRIPTION:
* 
*	 This routine restores the main threads context, and calls
*	 cma__abort_process.
*
*	 
* FORMAL PARAMETERS:
* 
*	 None
* 
* IMPLICIT INPUTS:
* 
*	 The main threads static context buffer, and the signal prompting
*	 this abort 
* 
* IMPLICIT OUTPUTS:
* 
*	 None
*
* FUNCTION VALUE:
* 
*	 None  (This routine does not return to the caller.)
* 
* SIDE EFFECTS:
* 
*	 NOTE:  This routine DOES NOT RETURN it calls cma__abort_process
*		in the main threads context which causes process termination.
*
*/
ENTRY(cma__transfer_main_abort)

	/*
	* Switch to the initial thread's stack
	* Restore main thread's stack pointer and base pointer.
	*/
	movl    cma__g_def_ctx_buf,%eax
	movl	S_SP(%eax),%esp
	movl 	S_BP(%eax),%ebp

	/*
	* Call cma__abort_process with the specific signal as the argument
	*/
	pushl	cma__g_abort_signal
	call	cma__abort_process

	/*
	* This point should never be reached
	*/

	call	cma__bugcheck
