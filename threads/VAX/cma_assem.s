 # @OSF_COPYRIGHT@
 # COPYRIGHT NOTICE
 # Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 # ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 # the full copyright text.
 # HISTORY
 # $Log: cma_assem.s,v $
 # Revision 1.1.6.2  1996/03/09  23:30:56  marty
 # 	Update OSF copyright year
 # 	[1996/03/09  22:46:11  marty]
 #
 # Revision 1.1.6.1  1995/12/07  21:58:02  root
 # 	Submit OSF/DCE 1.2.1
 # 	[1995/12/07  21:04:01  root]
 # 
 # Revision 1.1.4.1  1994/06/09  13:36:59  devsrc
 # 	 CR10892 - fix copyright in file
 # 	[1994/06/09  13:27:05  devsrc]
 # 
 # Revision 1.1.2.2  1992/12/15  22:24:30  alan
 # 	Insert copyright notices
 # 	[1992/12/07  16:11:16  alan]
 # 
 # Revision 1.1  1992/01/19  18:36:01  devrcs
 # 	Initial revision
 # 
 # $EndLog$
 #
 #  Copyright (c) 1990, 1991
 #  by DIGITAL Equipment Corporation, Maynard, Mass.
 #
 #  This software is furnished under a license and may be used and  copied
 #  only  in  accordance  with  the  terms  of  such  license and with the
 #  inclusion of the above copyright notice.  This software or  any  other
 #  copies  thereof may not be provided or otherwise made available to any
 #  other person.  No title to and ownership of  the  software  is  hereby
 #  transferred.
 #
 #  The information in this software is subject to change  without  notice
 #  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 #  Corporation.
 #
 #  DIGITAL assumes no responsibility for the use or  reliability  of  its
 #  software on equipment which is not supplied by DIGITAL.
 ##

 ##
 #  FACILITY:
 #
 #	CMA services
 #
 #  ABSTRACT:
 #
 #	These are non-portable subroutines for thread service routines.  They
 #	cannot be written in C because they manipulate specific registers, etc.
 #
 #  AUTHORS:
 #
 #	Webb Scales
 #
 #  CREATION DATE:
 #
 #	5 January 1990
 #
 #  MODIFICATION HISTORY:
 #
 #	001	Dave Butenhof	16 August 1990
 #		Implement "springboard" routine for asynchronous alert
 #		delivery.
 #	002	Webb Scales	 2 October 1990
 #		Removed copyright character from file.
 #	003	Paul Curtin	8 April 1991
 #		Added cma__transfer_main_abort, etc.
 #	004	Dave Butenhof	01 May 1991
 #		Add arguments to cma__bugcheck() calls.
 #	005	Dave Butenhof	09 October 1991
 #		Repair a misleading comment.
 ##

 ##
 #	A thread context consists of the saved register set necessary for
 #	rescheduling the thread on another processor.
 #
 #	The thread context is saved in a stack frame created by a "calls"
 #	instruction.  By setting all the register bits in the entry mask, the
 #	"calls" instruction saves r6-r11 automatically, along with the return 
 #	pc, fp, ap, and psw, in short, the whole context (since the context is
 #	saved/restored during a routine call, we do not need to save registers
 #	which are not normally saved across routine calls).  The fp for the
 #	frame containing the context is stored in the static context buffer in
 #	the TCB, along with the sp.  When we restore this context, we simply
 #	restore the fp & sp from the static context buffer and execute a RET
 #	instruction.  This causes the sp, fp, pc, psw, and ap to be correctly
 #	restored from the information in the call frame, along with all the
 #	general registers, and the thread resumes at the instruction after the
 #	call to save the context.
 #
 #	In the case of an asynchronous context switch, the return pc and psl at
 #	the point of interruption are fetched from the AST argument list and
 #	stored in the TCB.  Then the return pc in the argument list is
 #	overwritten with the address of the first instruction in
 #	cma__do_call_dispatch, so that, when the AST completes, execution will
 #	resume in cma__do_call_dispatch instead of at the point of interruption.
 #	Here, the pc & psl for the point of interruption are retrieved from the
 #	TCB (via a C subroutine) and are stored on the stack.  Next r0-r5,
 #	which would be destroyed by subsequent subroutine calls are pushed on
 #	the stack.  Finally, the normal dispatching routines are called to run
 #	the next thread.  When the interrupted thread is rescheduled, control
 #	returns from the dispatching subroutine call.  r0-r5 are restored, and
 #	we execute an REI instruction to resume execution at the point where the
 #	original timeslice AST interrupted.
 #
 #	The static context buffer (in the TCB) only contains the fp and the
 #	sp for the suspended thread.  All other context is saved on the
 #	thread's stack.  
 #
 #
 #	STATIC CONTEXT BUFFER:
 #
 #	+----------------+  <---- Address of base of static context block 
 #	|       fp       | :0	Address of current frame
 #	+----------------+
 #	|       sp       | :4	Current sp
 #	+----------------+
 #
 #
 #	ASYNCHRONOUS CONTEXT BUFFER:
 #
 #	+----------------+  <---- Address of base of asynchronous context block 
 #	|       pc       | :0	pc at point of interruption
 #	+----------------+
 #	|       psl      | :4	psl at point of interruption
 #	+----------------+
 #
 #
 #	STACK DURING SYNCHRONOUS SUSPENSION (Standard VAX stack frame):
 #	
 #	+----------------+  
 #	|     new_ctx    |	address of static context block of new thread
 #	+----------------+
 #	|     old_ctx    |	address of static context block of old thread
 #	+----------------+  
 #	|            | 2 | :ap	argument count in call to context-save
 #	+----------------+  
 #	|       r11      |
 #	+----------------+
 #	|       r10      |	|
 #	+----------------+	|
 #	       ...		| Decreasing Addresses
 #	+----------------+	|
 #	|       r3       |	V
 #	+----------------+
 #	|       r2       |	(r0 & r1 are not saved)
 #	+----------------+
 #	|       pc       |	Return pc 
 #	+----------------+
 #	|       fp       |	fp of calling frame
 #	+----------------+
 #	|       ap       |	ap of calling routine
 #	+----------------+
 #	|spS0|EM|  psw   |	Stack alignment bits, bit indicating CALLS,
 #	+----------------+	   register-save portion of entry mask, psw
 #	|        0	 | :fp	Condition Handler address (none)
 #	+----------------+ :sp	
 #
 #
 #	ADDITIONAL STACK DURING ASYNCHRONOUS SUSPENSION:
 #	
 #	+----------------+  
 #	|       psl      |	psl at point of interruption
 #	+----------------+
 #	|       pc       |	pc at point of interruption
 #	+----------------+
 #	|       r5       |	|
 #	+----------------+	|
 #	|	r4	 |	|
 #	+----------------+	|
 #	|       r3       |	|
 #	+----------------+	|
 #	|	r2	 |	| Decreasing Addresses
 #	+----------------+	|
 #	|       r1       |	|
 #	+----------------+	|
 #	|	r0	 |	|
 #	+----------------+	|
 #		...		V
 #	  call frames for
 #	 dispatch routines
 ##

	.text

 # Routines which are referenced below:
	.globl	_cma__async_delivery
	.globl	_cma__bugcheck
	.globl	_cma__get_async_info
	.globl	_cma__start_thread
	.globl	_cma__terminate_thread
	.globl	_cma__thread_base
	.globl	_cma__undefer
	.globl	_cma__yield_processor
	.globl	_cma__abort_process

 # Global data:
	.globl	_cma__c_default_ps	# Exported
	.globl	_cma__g_kernel_critical	# Imported
	.globl	_cma__g_def_ctx_buf	# Imported
	.globl	_cma__g_abort_signal	# Imported

_cma__c_default_ps:
	.long 0x03C00000

.globl	_cma__create_thread
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine initializes a thread (ie, its stack and context) and
 #	 prepares it for the scheduler.
 #
 #	 This routine also contains the code which executes in the frame at the
 #	 base of a thread's stack and calls the user's "start routine".
 #
 # FORMAL PARAMETERS:
 # 
 #	 4(ap)  - the address of the child context buffer
 #	 8(ap)  - the address of the base of the child thread stack
 #	 12(ap) - the address of the child TCB
 #	 16(ap) - the address of the start routine entry mask
 #	 20(ap) - the value of parameter to be passed to the routine
 # 
 # IMPLICIT INPUTS:
 # 
 #	 None
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 The child thread stack is initialized with two call frames:  one for
 #	 the "call" to "cma___execute_thread" and one for a "call" to
 #	 cma__transfer_thread_ctx which contains the saved context which will
 #	 be restored when the child thread is first scheduled.
 #
 # FUNCTION VALUE:
 # 
 #	 None 
 # 
 # SIDE EFFECTS:
 # 
 #	 None
 #
 #	 NOTE:  This routine assumes that any interruptions which occur while
 #		we are on the child's stack instead of this thread's own stack
 #		won't be concerned about what stack they are on.
 #
ct1:	.asciz	"create_thread:1"

_cma__create_thread:
	.word 0				# Entry mask: no saved registers

	# I am about to do something during which I don't want to be rescheduled
1:	bbssi	$0,_cma__g_kernel_critical,1b

	# "Remember" our current frame pointer and where the child static
	# context buffer is
	movl	4(ap),r0		# Instead of passing it on the stack
	movl	fp,r1			# Save fp so we can switch back

	# Switch to child stack
	movl	8(ap),sp		# Switch to child stack
	bicl2	$3,sp			# Align the stack before we start

	# Build the parameter list for "cma___execute_thread"
	pushl	20(ap)			# Push on start routine parameter
	pushl	16(ap)			# Push on start routine address
	pushl	12(ap)			# Push on childs TCB address

	#
	# Call "cma___execute_thread" on behalf of the child thread.
	#
	# NOTE: The call to "3f" never returns to here.  In the parent thread,
	#	control (eventually) returns directly to the caller of
	#	cma__create_thread.  In the child thread, the thread is
	#	terminated before it can return to here, and it is never
	#	rescheduled.
	#

	calls	$3,3f			# Call "cma___execute_thread"

	# This point should never be reached during execution, as the call to
	# "cma___execute_thread" never returns.
	pushab	ct1
	calls	$1,_cma__bugcheck	# Handle the error
	halt				#  and quit!

	#
	# "cma___execute_thread"
	# 
	# FORMAL PARAMETERS:
	# 
	#	r0	  - the address of the child thread static context block
	#	r1	  - the saved value of the parent thread frame pointer
	#	4(ap)	  - the address of the child thread tcb
	#	8(ap)	  - the address of the start routine entry mask
	#	12(ap)	  - the value of parameter to be passed to the routine
	# 
	#

et1:	.asciz	"execute_thread:1"

3:	.word 0				# Entry mask for "cma___execute_thread"

	# Break the back-pointers to the caller
	clrw	4(fp)	# Zero the psw
	clrw	6(fp)	# Zero the saved register mask
	clrl	8(fp)	# No saved ap
	clrl	12(fp)	# Clear the saved fp (no previous frame)
	clrl	16(fp)	# No saved pc

	#
	# Now build another frame on the stack, as though we had just called
	# cma__transfer_thread_ctx.  This frame holds the context required to
	# schedule the thread.
	#
	# NOTE: In the parent thread, the call to 7 returns directly to the
	#	caller of cma__create_thread.  The child thread, when it is
	#	first scheduled, will resume execution by returning from this
	#	call.
	#

	pushl	4(ap)			# We'll need this
	calls	$1,7f			# Create frame for cma__transfer_thread_ctx

	#
	# The child thread begins execution here, when it is scheduled for the
	# first time.
	#
	
	pushl	12(ap)			# Push thread argument
	pushl	8(ap)			# Push thread routine
	pushl	4(ap)			# Push thread's TCB address
	calls	$3,_cma__thread_base	# Start up the thread

	# This point should never be reached during execution, as the call to
	# cma__thread_base never returns.
	pushab	et1
	calls	$1,_cma__bugcheck	# Handle the error
	halt				#  and quit!

	#
	# "cma___save_thread_ctx"
	# 
	# FORMAL PARAMETERS:
	#
	#	r0	- the address of the child thread static context block
	#	r1	- the saved value of the parent thread frame pointer
	#	4(ap)   - the address of the child thread tcb
	#

	# Save all the registers (r6-r11)
7:	.word	0x0fc0

	# Save the fp & sp in the static context block
	movq	fp,(r0)			# Save the current fp & sp

	# Switch back to parent thread's stack
	movl	r1,fp			# Restore parent's fp
	movl	fp,sp			# Restore parent's sp

	# The child thread is all ready to run, so inform the scheduler
	# Note:  The parent may or may not be preempted by the child here
	pushl	4(ap)			# The child TCB
	calls	$1,_cma__start_thread	# Brace for colision, Capt'n

	# The new thread is completely initialized and I am back on my own
	# stack, so now I am free to be rescheduled
	calls	$0,_cma__undefer
	bbcci	$0,_cma__g_kernel_critical,9f
9:	ret

.globl	_cma__do_call_on_stack
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine calls the specified routine on the specified stack,
 #	 passing the specified parameter.
 #	 
 # FORMAL PARAMETERS:
 # 
 #	 4(ap)  -  address of base of target stack
 #	 8(ap)  -  address of a routine entry mask
 #	 12(ap) -  value of parameter to be passed to the routine
 # 
 # IMPLICIT INPUTS:
 # 
 #	 None
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 None
 #
 # FUNCTION VALUE:
 # 
 #	 Value returned by the supplied routine, if any
 # 
 # SIDE EFFECTS:
 # 
 #	 None
 #

_cma__do_call_on_stack:
	.word 0				# Entry mask: no registers saved

	movl	4(ap),sp		# Switch to new stack

	pushl	12(ap)			# Put the routine param on the new stack
	calls	$1,*8(ap)		# Call the user's routine

	ret				# The routine value is already in r0


.globl	_cma__fetch_sp
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 Return the value of the stack pointer.
 #
 # FORMAL PARAMETERS:
 # 
 #	 none
 # 
 # IMPLICIT INPUTS:
 # 
 #	 none
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 none
 #
 # FUNCTION VALUE:
 # 
 #	 The SP
 # 
 # SIDE EFFECTS:
 # 
 #	 none
 #
 #
_cma__fetch_sp:
	.word	0
	moval	24(fp), r0
	ret

.globl	_cma__restore_thread_ctx
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine restores all context associated with a thread necessary
 #	 for scheduling it on the current processor.
 #
 #	 This routine causes execution to resume in the target thread.
 #	 
 # FORMAL PARAMETERS:
 # 
 #	 4(ap)  -  address of the buffer which holds the static context
 # 
 # IMPLICIT INPUTS:
 # 
 #	 The contents of the new stack and frame
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 The sixteen VAX registers (r0-r11, ap, fp, sp, & pc), and the psl.
 #
 # FUNCTION VALUE:
 # 
 #	 None  (This routine does not return to the caller.)
 # 
 # SIDE EFFECTS:
 # 
 #	 NOTE:  This routine DOES NOT RETURN in the typical sense.  When it
 #		returns, it returns to a different thread.  This routine does
 #		not return to its caller.
 #
 #

_cma__restore_thread_ctx:
	.word 0x0FC0			# Entry mask:  Save registers 6-11

	#
	# The thread context is saved on the stack in the call frame in the
	# call to cma__transfer_thread_ctx (bits are set for all registers
	# which must be preserved across a call).
	#
	# Thus restoring a thread's context comes down to restoring its frame
	# pointer and returning.
	#

	# Restore the newly-scheduled thread's context
	movq	*4(ap),fp		# Restore the new thread's fp & sp

	# Returning loads all of the general registers, the sp, psw, pc, & ap,
	# based on the information in the fp and the stack frame.
	ret


.globl	_cma__test_and_set
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine executes an interlocked test-and-set instruction.
 #
 # FORMAL PARAMETERS:
 # 
 #	 4(ap)	- Address of longword (bit 0 is used)
 # 
 # IMPLICIT INPUTS:
 # 
 #	 none
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 none
 #
 # FUNCTION VALUE:
 # 
 #	 The previous value of the bit.
 # 
 # SIDE EFFECTS:
 # 
 #	 none
 #
 #
_cma__test_and_set:
	.word	0
	movl	$1, r0
	bbssi	$0, *4(ap), 1f
	clrl	r0
1:	ret


.globl	_cma__transfer_thread_ctx
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine stores the context of one thread and restores the
 #	 context of another, thus performing an actual context switch.
 #
 #	 The context is saved, automatically, in the frame of this routine by
 #	 the call.  All that's left to do is store the address of this frame
 #	 (ie, the current fp) in the static context buffer.
 #
 #	 The newly restored thread resumes processing with the instruction
 #	 following it's last call to cma__transfer_thread_ctx or the
 #	 instruction it was at when it was interrupted.  That is, this
 #	 routine returns directly to that instruction.
 #
 # FORMAL PARAMETERS:
 # 
 #	 4(ap)  -  address of current thread's context buffer
 #	 8(ap)  -  address of new thread's context buffer
 # 
 # IMPLICIT INPUTS:
 # 
 #	 The current fp.
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 The sixteen VAX registers (r0-r11, ap, fp, sp, & pc), and the psl.
 #
 # FUNCTION VALUE:
 # 
 #	 None
 # 
 # SIDE EFFECTS:
 # 
 #	 "During" the call to this routine, another thread runs.
 #
 #
_cma__transfer_thread_ctx:
	.word 0x0FC0		# Entry mask:  Save registers 6-11

	#
	# The thread context is saved on the stack in the call frame for this
	# routine (bits are set for all registers which must be preserved).
	#
	# Thus saving and restoring a thread's context comes down to saving and
	# restoring its frame pointer.
	#

	# Save the current thread's context
	movq	fp,*4(ap)		# Save the current fp & sp

	# Restore the newly-scheduled thread's context
	movq	*8(ap),fp		# Restore the new thread's fp & sp

	# Returning loads all of the general registers, the sp, psw, pc, & ap,
	# based on the information in the fp and the stack frame.
	ret


.globl	_cma__unset
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine atomically clears bit 0 of the specified address.
 #
 # FORMAL PARAMETERS:
 # 
 #	 4(ap)	- Address of longword (bit 0 is used)
 # 
 # IMPLICIT INPUTS:
 # 
 #	 none
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 none
 #
 # FUNCTION VALUE:
 # 
 #	 none
 # 
 # SIDE EFFECTS:
 # 
 #	 none
 #
 #
_cma__unset:
	.word	0
	bbcci	$0, *4(ap), 1f
1:	ret

.globl	_cma__do_async_alert
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	This routine is responsible for initiating asynchronous alert
 #	delivery.  It calls a routine (written in C) to actually raise the
 #	alert exception, to ensure that the raise is always consistent with
 #	the CMA exception package implementation on the platform (the extra
 #	call is probably not a serious performance issue in the context of an
 #	alert which will usually terminate the thread anyway).
 #
 #	This routine is never actually CALLed, rather it is "jumped" into by
 #	placing the entry point pc, "cma__do_async_alert", into the
 #	resume pc in a signal context block.
 #
 #	NOTE:  Therefore, this routine runs in the same frame as the routine
 #	       which was interrupted by the signal, but not at signal level.
 #
 # FORMAL PARAMETERS:
 # 
 #	None:  this routine is never actually called
 # 
 # IMPLICIT INPUTS:
 # 
 #	None
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	None
 #
 # FUNCTION VALUE:
 # 
 #	None
 # 
 # SIDE EFFECTS:
 # 
 #	None
 #
aa1:	.asciz	"do_async_alert:1"

_cma__do_async_alert:
	calls	$0,_cma__undefer
	bbcci	$0,_cma__g_kernel_critical,9f
9:
	calls	$0,_cma__async_delivery	# Deliver the alert

 #
 # This point should never be reached, since the CMA exception package doesn't
 # support continuable exceptions.
 #
	pushab	aa1
	calls	$1,_cma__bugcheck	# Handle the error
	halt				#  and quit


.globl	_cma__transfer_main_abort
#
# FUNCTIONAL DESCRIPTION:
# 
#	 This routine restores the main threads context, and calls
#	 cma__abort_process.
#
#	 
# FORMAL PARAMETERS:
# 
#	 None
# 
# IMPLICIT INPUTS:
# 
#	 The main threads static context buffer, and the signal prompting
#	 this abort 
# 
# IMPLICIT OUTPUTS:
# 
#	 None
#
# FUNCTION VALUE:
# 
#	 None  (This routine does not return to the caller.)
# 
# SIDE EFFECTS:
# 
#	 NOTE:  This routine DOES NOT RETURN it calls cma__abort_process
#		in the main threads context which causes process termination.
#
_cma__transfer_main_abort:

	# Switch to the initial thread's stack
	movq	*_cma__g_def_ctx_buf,fp     # Restore the main thread's FP & SP

	# Call cma__abort_process with the specific signal as the argument
	pushl	_cma__g_abort_signal
	calls	$1,_cma__abort_process

	#
	# This point should never be reached
	#

	halt


.globl	_cma___do_call_dispatch
.globl	_cma__force_dispatch
 #
 # FUNCTIONAL DESCRIPTION:
 # 
 #	 This routine is responsible for initiating an asynchronous context
 #	 switch.  It calls a routine (writen in C) to retrieve the address of
 #	 the current thread's context buffer and the restart pc & psl.  It then
 #	 saves the psl, pc, and r0-r5.  It calls cma__dispatch to run a new
 #	 thread.  When the call to cma__dispatch returns (ie when this thread
 #	 has been rescheduled), r0-r5 are restored and this routine returns
 #	 directly to the instruction which was interrupted by the AST.
 #
 #	 This routine is never actually CALLed, rather it is "jumped" into by
 #	 placing the entry point pc, "cma__force_dispatch", into the
 #	 resume pc in an AST argument list.
 #
 #	 NOTE:  Therefore, this routine runs in the same frame as the routine
 #		which was interrupted by the AST, but not at AST level.
 #
 # FORMAL PARAMETERS:
 # 
 #	 None:  this routine is never actually called
 # 
 # IMPLICIT INPUTS:
 # 
 #	 The current values of r0-r5.
 # 
 # IMPLICIT OUTPUTS:
 # 
 #	 The current values of r0-r5.
 #
 # FUNCTION VALUE:
 # 
 #	 None  (This routine returns by an REI to the point of interruption.)
 # 
 # SIDE EFFECTS:
 # 
 #	 Another thread is run "during" the call to cma__dispatch.
 #

_cma___do_call_dispatch:
	.word 0				# Entry mask

_cma__force_dispatch:			# Entry point AST routine to returns to

	# Make room on the stack for the pc and psl
	subl2	$8,sp

	# Save the registers which, by convention, are not preserved across
	# function calls
	movq	r4,-(sp)		# Save r4 & r5
	movq	r2,-(sp)		# Save r2 & r3
	movq	r0,-(sp)		# Save r0 & r1

	# Call cma__get_async_info to get the address of async context block	
	calls	$0,_cma__get_async_info	# r0 gets the address

	# Move the pc and psl on to the stack above r0 & r1
	movq	(r0),24(sp)

	# Run a different thread for a while
	calls	$0,_cma__yield_processor	# Perform the dispatch

	calls	$0,_cma__undefer
	bbcci	$0,_cma__g_kernel_critical,9f
9:
	# Restore the rest of our context (ie, prepare to run again)
	# Don't make any CALLs after restoring r0-r5, they might get trashed
	movq	(sp)+,r0		# Restore r0 & r1
	movq	(sp)+,r2		# Restore r2 & r3
	movq	(sp)+,r4		# Restore r4 & r5

	# And run this thread again
	rei				# Restore pc & psl (from the stack)

 #  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM_VAX_ULTRIX.S 
 #  *9     9-OCT-1990 17:24:48 SCALES "Remove copyright character" 
 #  *8    16-AUG-1990 12:55:44 BUTENHOF "Implement asynchronous alerts" 
 #  *7     3-JUL-1990 07:30:19 BUTENHOF "Change copyright header" 
 #  *6    18-MAY-1990 07:09:16 BUTENHOF "Optimize mutex and cv" 
 #  *5     9-FEB-1990 11:06:50 BUTENHOF "Convert to stream format" 
 #  *4     1-FEB-1990 06:53:44 BUTENHOF "Remove useless routine" 
 #  *3    24-JAN-1990 09:46:59 BUTENHOF "Fix up the file to build through pcc" 
 #  *2    23-JAN-1990 14:04:49 BUTENHOF "Make function prototypes conditional" 
 #  *1     9-JAN-1990 13:33:19 BUTENHOF "VAX ULTRIX assembly code file" 
 #  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM_VAX_ULTRIX.S 
#  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S
#  *4     9-OCT-1991 14:31:51 BUTENHOF "Fix a misleading comment"
#  *3     2-MAY-1991 14:01:06 BUTENHOF "Add arg to bugchecks"
#  *2     8-APR-1991 20:33:54 CURTIN "added cma__transfer_main_abort"
#  *1    12-DEC-1990 19:02:08 BUTENHOF "VAX specific assem file"
#  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S
