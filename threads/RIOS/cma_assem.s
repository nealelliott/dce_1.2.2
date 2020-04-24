# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# HISTORY
# $Log: cma_assem.s,v $
# Revision 1.1.13.3  1996/03/09  23:30:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:01  marty]
#
# Revision 1.1.13.2  1995/12/07  21:50:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/DCE1_1_WP/1  1995/04/26  16:55 UTC  jrr
# 	Changes to allow valid AIX core dumps.
# 	[1995/12/07  21:00:40  root]
# 
# Revision 1.1.9.1  1994/06/09  13:35:32  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:26:05  devsrc]
# 
# Revision 1.1.7.1  1993/10/20  21:20:07  rhw
# 	Fix for THR OT 7645 - cma__execute_thread does not initialize the stack
# 	correctly.
# 	[1993/10/20  21:05:57  rhw]
# 
# Revision 1.1.3.2  1992/12/15  22:23:27  alan
# 	Insert copyright notices
# 	[1992/12/07  16:02:56  alan]
# 
# Revision 1.1  1992/01/19  18:35:36  devrcs
# 	Initial revision
# 
# $EndLog$
#       
#
# 
#       DEC CMA threads code 
# 
#       Code for Risc System /6000 for the non portable routines for the
#	A thread context consists of the saved register set necessary for
#	rescheduling the thread on another processor.
#
#	The context consists of the general CPU registers $1-$31 (the value 
#	of $0 never changes and thus need not be saved), the integer divide
#	registers HI & LO, the floating point coprocessor registers $f0-$f31,
#	and the floating point coprocessor control/status register FCR31.
#
#	The context is pushed onto the stack (fetching FCR31 has the desirable
#	side effect of synchronizing with the floating point coprocessor and
#	thereby preventing an exception resulting from interrupting an
#	concurrent floating point instruction execution), and the sp is
#	stored in the static context block in the TCB.
#
#	The process is slightly more complicated in the case of an
#	asynchronous context switch, such as a quantum end.  The interrupt
#	routine saves the return PC and the floating point coprocessor status
#	in the TCB, replaces these values with the address of
#	cma__force_dispatch and a known state status word.  When the interrupt
#	routine returns, execution resumes in cma__force_dispatch instead of
#	at the point where the interruption occurred.  There, the volatile
#	registers are saved in a context area on the stack.  The saved values
#	of the PC and processor status at the point of interruption are
#	retrieved from the TCB by a call to a C subroutine and stored in the
#	context area on the stack. Then the usual dispatching routines are
#	called, resulting in a second, full context being saved on the stack.
#	When the thread context is restored as a result of being rescheduled,
#	it returns to cma__force_dispatch where the volatile registers, PC,
#	and PS are restored allowing the thread to resume execution at the
#	point where the interruption occurred.
#

	.set	frame_size,608	# size of sigcontext structure
	.set	off_iar,40	# offset of iar in sigcontext
	.set	off_msr,44	# offset of msr in sigcontext
	.set	off_cr,48	# offset of ccr in sigcontext
	.set	off_lr,52	# offset of lr in sigcontext
	.set    off_ctr,56	# offset of ctr in sigcontext
	.set	off_xer,60	# offset of xer in sigcontext
	.set	off_mq,64	# offset of mq in sigcontext
	.set	off_tid,68	# offset of tid in sigcontext
	.set    off_fpscr,72	# offset of fpscr in sigcontext
	.set	off_gpr0,224	# offset of r0 in sigcontext
#	.set    off_gpr13,276	# offset of r13 in sigcontext
	.set	off_fpr0,352	# offset of fpr0 in sigcontext
	.set	off_fpr14,464	# offset of fpr14 in sigcontext

##page;
# cma__force_dispatch 2
#
# FUNCTIONAL DESCRIPTION:
# 
#	This routine is responsible for initiating an asynchronous context
#	switch.
#
#	First, we save all registers which might be altered by subroutine 
#	calls in a context area on the stack.  Then, we call a routine
#	(written in C) to retrieve from the TCB the values which the PC and PS
#	(floating point coprocessor status register) had at the point of
#	interruption.  These values are written into the appropriate places in
#	the context area on the stack.  Then, we yield the processor to
#	another thread.  This results in a full context save before the new
#	thread is scheduled.  When we are once again scheduled, a full context
#	restore is performed in the process.  When control returns to this
#	routine, we restore the volatile registers which we saved, and return.
#
#	This routine is never actually called, rather it is "jumped" into by
#	placing the entry point PC, "cma__call_forced_dispatch", into the
#	resume PC in an AST argument list.
#
#	NOTE:  Therefore, this routine runs in the same frame as the routine
#		which was interrupted by the signal.
#
#	 	 
# FORMAL PARAMETERS:
# 
#	 None:  this routine is never actually called
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
#	 None:  this routine does not return, execution simply resumes at the
#		point where it was interrupted by the timeslice.
# 
# SIDE EFFECTS:
# 
#	 None
#
#	 NOTE:  This routine is not actually called (it is jumped into)
#

	S_PROLOG(cma__force_dispatch)

	ai	r1,r1,-frame_size	# get a sigcontext frame
	mr      r3,r1
        stu	r1,-56(r1)		# get a stack frame to make a call
	bl      ENTRY(cma__get_ibm_async_info)
	.extern	ENTRY(cma__get_ibm_async_info)


	bl      ENTRY(cma__yield_processor)
	.extern ENTRY(cma__yield_processor)

	ai	r1,r1,56                # get rid of stack frame
	mr	r3,r1			# set up call for sigreturn
	stu	r1,-56(r1)              # need a stack frame for the call

        LTOC(r8, cma__g_kernel_critical, data)
	cal	r9,0(r0)
	st      r9,0(r8)
	bl	ENTRY(sigreturn)
        .extern ENTRY(sigreturn)	

	S_EPILOG

	FCNDES(cma__force_dispatch)

##page;
#  cma__get_toc
#
#  returns the toc of the process for the cma__force_dispatch to work right
#
#  only necessary for AIX V3
#

	S_PROLOG(cma__get_toc)
	mr	r3,r2		# copy toc to return
	S_EPILOG

##page;
#
#  FUNCTIONAL DESCRIPTION
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
#	NOTE:	Therefore, this routine runs in the same frame as the routine
#		which was interrupted by the signal, but not at signal level.
#
#  FORMAL PARAMETERS:
#
#	None:	this routine is never actually called
#
#
	S_PROLOG(cma__do_async_alert)
	stu	r1,-56(r1)		#get a stack frame to make a call
	bl	ENTRY(cma__undefer)
	.extern ENTRY(cma__undefer)
	cal	r4,0(r0)
	LTOC(r5, cma__g_kernel_critical, data)
	st	r4,0(r5)
	bl	ENTRY(cma__async_delivery)
	.extern ENTRY(cma__async_delivery)
	bl	ENTRY(cma__bugcheck)
	.extern ENTRY(cma__bugcheck)
	S_EPILOG	

	FCNDES(cma__do_async_alert)

##page;
#
#  FUNCTIONAL DESCRIPTION
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
#	NOTE:	Therefore, this routine runs in the same frame as the routine
#		which was interrupted by the signal, but not at signal level.
#
#  FORMAL PARAMETERS:
#
#	None:	this routine is never actually called
#
#
	S_PROLOG(cma__transfer_main_abort)
	LTOC(r8, cma__g_def_ctx_buf, data)
	l	r1, 0(r8)		#switch to main thread stack
	stu	r1,-56(r1)		#get a stack frame to make a call
	LTOC(r8, cma__g_abort_signal, data)
	l	r3, 0(r8)		#argument is signal number
	bl	ENTRY(cma__abort_process)
	.extern ENTRY(cma__abort_process)
	bl	ENTRY(cma__bugcheck)
	.extern ENTRY(cma__bugcheck)
	S_EPILOG	

	FCNDES(cma__transfer_main_abort)

##page;
# cma__create_thread
#
# FUNCTIONAL DESCRIPTION:
# 
#	 This routine initializes a thread (ie, its stack and context) and
#	 prepares it for the scheduler
#
# FORMAL PARAMETERS:
# 
#	 r3	- the address of the child context buffer
#	 r4	- the address of the base of the child thread stack
#	 r5	- the address of the child TCB
#	 r6	- the address of the function descriptor for the start routine entry point
#	(r7)    - the value of parameter to be passed to the routine
# 
# IMPLICIT INPUTS:
# 
#	 None
# 
# IMPLICIT OUTPUTS:
# 
#	 The child thread stack is initialized with a simulated call frame for
#	 the call to cma__execute_thread and a context block which will cause
#	 the new thread to begin execution at the start of that routine.  We
#	 don't bother to initialize any of the other registers.
#
# FUNCTION VALUE:
# 
#	 None 
# 
# SIDE EFFECTS:
# 
#	 None
#
#	 NOTE:  This routine assumes the stack is already longword aligned
#
#	 NOTE:  This routine assumes that any interruptions which occur while
#		we are one the child's stack instead of this thread's own stack
#		won't be concerned about it.
#
#	 NOTE: This routine currently assumes that this thread will not be
#		preempted during the the call to cma__start_thread.  This will
#		be a feature which is added later with priorities.  At that
#		time we should be sure that the kernel is entered/exited
#		correctly.
#
#	 NOTE: We don't copy the register-base parameters on to the stack
#		because we don't use them after we call our subroutines.
#

	
	PROLOG(cma__create_thread, 32, 32, 0)
	
	LTOC(r8, cma__g_kernel_critical, data)

L1:	l	r9, 0(r8)	# prevent reschedule by setting lock
	cmpi	cr0, r9, 0
	bne	L1

	cal	r9,1(r0)	
	st	r9,0(r8)	# set the lock

	ai	r4,r4,-16	# make some room on the child's stack
	st	r5,0(r4)	# copy TCB to child stack
	st	r6,4(r4)	# copy start routine address to child stack
	st	r7,8(r4)        # copy argument value to child stack

	ai	r4,r4,-frame_size	#  frame for cma__restore_thread_ctx
	st	r4,0(r3)	# save child stack pointer in TCB
	stm     r0,off_gpr0(r4) # fake some registers
	LTOC(r6,cma__execute_thread, entry)
	st	r6,off_iar(r4)	# store place to execute in context
	mr	r3,r5		#set up parameter for cma__start_thread
	bl	ENTRY(cma__start_thread)
	.extern	ENTRY(cma__start_thread)

	bl	ENTRY(cma__undefer)
	.extern	ENTRY(cma__undefer)

	LTOC(r7,cma__g_kernel_critical,data)
	cal	r6,0(r0)
	st	r6,0(r7)	# unlock the lock

	EPILOG(0,0,0)		# cleanup stack frame and return

##page;
# cma__do_call_on_stack
#
# FUNCTIONAL DESCRIPTION:
# 
#	 This routine calls the specified routine on the specified stack,
#	 passing the specified parameter.
#	 
# FORMAL PARAMETERS:
# 
#	 r3	-  of base of target stack
#	 r4	-  address of a routine entry point
#	 r5	-  of parameter to be passed to the routine
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

	PROLOG(cma__do_call_on_stack, 31, 32, 0)

	mr	r31,r1		# save a copy of r1
	mr	r1,r3		# get new stack pointer
	mr	r3,r5		# set up argument for function call
	l	r6,0(r4)        # get address of function
	mtlr	r6		# set lr to where we want to go
	brl			# call the routine

	mr	r1,r31		# restore our original r1

	EPILOG(1,0,0)

##page;
# cma__execute_thread
#
# FUNCTIONAL DESCRIPTION:
#
# 	This is the routine which occupies the frame at the base of the stack.
#	It calls the user's "start routine", it holds the catch-all handler
#	(set up in cma__create_thread), and it calls the thread cleanup
#	routines after the "start routine" returns.
#
#	In the event of an exception, the catch-all handler may unwind back to
#	this routine, write the reason for termination in -4(fp) [the exit
#	status], and resume this routine to run down the thread.
#
# FORMAL PARAMETERS:
# 
#	r3, TCB(r1)	   - the address of the thread tcb
#	r4, START_RTN(r1)  - the address of the start routine entry point
#	r5, START_ARG(r1)  - the value of parameter to be passed to the routine
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
#	 None (This function never returns)
# 
# SIDE EFFECTS:
#
#	 Note:  This routine is never explicitly called:  A call frame for it
#		and a thread context buffer pointing to it are built by
#		cma__create_thread.  When the thread is scheduled, it begins
#		executing at the first instruction.  The parameters are
#		"passed" to this routine in the stack frame, placed there by
#		cma__create_thread, not in the argument registers.
#
#	 NOTE:  This routine DOES NOT RETURN: the call to cma__thread_base
#		results in thread termination (eventually).
#

	S_PROLOG(cma__execute_thread)

	l	r3,0(r1)
	l	r4,4(r1)
	l	r5,8(r1)
	stu	r1,-56(r1)
	cal	r6,0(0)		# initialize the top of stack
	st	r6,0(r1)	# to NULL for traceback
	b	ENTRY(cma__thread_base)
	.extern	ENTRY(cma__thread_base)

	S_EPILOG


##page;
# cma__restore_thread_ctx
#
# FUNCTIONAL DESCRIPTION:
# 
#	 This routine restores all context associated with a thread which is
#	 necessary for scheduling it on the current processor.
#
#	 This routine causes execution to resume in the target thread, and
#	 therefore does not return to the calling routine.
#	 
# FORMAL PARAMETERS:
# 
#	 r3	-  address of the buffer which holds the static context
# 
# IMPLICIT INPUTS:
# 
#	 None
# 
# IMPLICIT OUTPUTS:
# 
#	 The main CPU general registers ($1-$31), the HI and LO registers, 
#	 the PC (sort of), the CP1 (FPA) registers ($f0-$f31), and the FPA
#	 control/status register (FCR31).
#
# FUNCTION VALUE:
# 
#	 None  (This routine does not return to the caller.)
# 
# SIDE EFFECTS:
# 
#	 NOTE:  This routine DOES NOT RETURN in the typical sense.
#
#

	S_PROLOG(cma__restore_thread_ctx)

	l	r1,0(r3)		# load the new stack pointer

	l	r0, off_mq(r1)		# load the MQ register
	mtmq	r0
	l	r0, off_xer(r1)		# load the XER register
	mtxer	r0
	l	r0, off_ctr(r1)		# load the CTR register
	mtctr	r0

	lfd	f14, off_fpr14 (r1)	# load the floating point registers
	lfd	f15, off_fpr14 +  1 * 8 (r1)

	lfd	f0, off_fpscr - 4 (r1)	#  fetch the FPSCR, and a word of
					#  garbage into f0

	lfd	f16, off_fpr14 +  2 * 8 (r1)
	lfd	f17, off_fpr14 +  3 * 8 (r1)
	lfd	f18, off_fpr14 +  4 * 8 (r1)
	lfd	f19, off_fpr14 +  5 * 8 (r1)
	lfd	f20, off_fpr14 +  6 * 8 (r1)

	mtfsf	0xff, f0		#  load the FPSCR, from f0

	lfd	f21, off_fpr14 +  7 * 8 (r1)
	lfd	f22, off_fpr14 +  8 * 8 (r1)
	lfd	f23, off_fpr14 +  9 * 8 (r1)
	lfd	f24, off_fpr14 + 10 * 8 (r1)
	lfd	f25, off_fpr14 + 11 * 8 (r1)
	lfd	f26, off_fpr14 + 12 * 8 (r1)
	lfd	f27, off_fpr14 + 13 * 8 (r1)
	lfd	f28, off_fpr14 + 14 * 8 (r1)
	lfd	f29, off_fpr14 + 15 * 8 (r1)
	lfd	f30, off_fpr14 + 16 * 8 (r1)
	lfd	f31, off_fpr14 + 17 * 8 (r1)

	l	r0,off_cr(r1)		# grab the condition register
	mtcrf	0xff,r0			# and load it

	lm	r0,off_gpr0(r1) 	# load the registers
	l	r0,off_iar(r1)		# get new pc to execute
	ai	r1,r1,frame_size	# throw away frame
	mtlr	r0
	brl				# jump to new thread

	S_EPILOG

##page;
# cma__transfer_thread_ctx 2
#
# FUNCTIONAL DESCRIPTION:
# 
#	 This routine stores the context of one thread and restores the context
#	 of another, thus performing an actual context switch.  This routine is
#	 used only for synchronous context switches (but the context which it
#	 restores may have been synchronously or asynchronously saved).
#
#	 The newly restored thread resumes processing with the instruction
#	 following it's last call to cma__save_thread_ctx or
#	 cma__transfer_thread_ctx, ie, this routine returns directly to that
#	 instruction.
#
# FORMAL PARAMETERS:
# 
#	 r3  -  address of current thread's static context buffer
#	 r4  -  address of new thread's static context buffer
# 
# IMPLICIT INPUTS:
# 
#	 The main CPU general registers ($1-ra), the HI and LO registers, 
#	 the PC (sort of), the CP1 (FPA) registers ($f0-$f31), and the FPA
#	 control/status register (FCR31).
# 
# IMPLICIT OUTPUTS:
# 
#	 The main CPU general registers ($1-ra), the HI and LO registers, 
#	 the PC (sort of), the CP1 (FPA) registers ($f0-$f31), and the FPA
#	 control/status register (FCR31).
#
# FUNCTION VALUE:
# 
#	 None
# 
# SIDE EFFECTS:
# 
#	 NOTE:  When this routine returns, it is in a different thread...
#
#


	S_PROLOG(cma__transfer_thread_ctx)

	ai	r1,r1,-frame_size	# get a sigcontext frame
	stm	r0,off_gpr0(r1)	        # save registers

	mfcr	r0			# grab the condition register
	st	r0,off_cr(r1)		# and save it

	# save the floating point registers and the FPSCR.  The code
	# for the FPSCR save was adapted from disown_fp.  I assume it
	# is interleaved for timing purposes.

	stfd	f14, off_fpr14 (r1)	# save the floating point registers
	stfd	f15, off_fpr14 +  1 * 8 (r1)
	
	mffs	f0			#  get FPSCR into low end of f0

	stfd	f16, off_fpr14 +  2 * 8 (r1)
	stfd	f17, off_fpr14 +  3 * 8 (r1)
	stfd	f18, off_fpr14 +  4 * 8 (r1)
	stfd	f19, off_fpr14 +  5 * 8 (r1)
	stfd	f20, off_fpr14 +  6 * 8 (r1)

	l	r0, off_fpscr - 4 (r1)	#  pick up word preceeding fpscr
					#  area in sigcontext structure

	stfd	f21, off_fpr14 +  7 * 8 (r1)
	stfd	f22, off_fpr14 +  8 * 8 (r1)
	stfd	f23, off_fpr14 +  9 * 8 (r1)
	stfd	f24, off_fpr14 + 10 * 8 (r1)
	stfd	f25, off_fpr14 + 11 * 8 (r1)

	stfd	f0, off_fpscr - 4 (r1)	#  store FPSCR plus hiword of junk
	st	r0, off_fpscr - 4 (r1)	#  restore stuff stepped on by hiword
					#  junk

	stfd	f26, off_fpr14 + 12 * 8 (r1)
	stfd	f27, off_fpr14 + 13 * 8 (r1)
	stfd	f28, off_fpr14 + 14 * 8 (r1)
	stfd	f29, off_fpr14 + 15 * 8 (r1)
	stfd	f30, off_fpr14 + 16 * 8 (r1)
	stfd	f31, off_fpr14 + 17 * 8 (r1)

	mfmq	r0			# save the MQ register
	st	r0, off_mq (r1)
	mfxer	r0			# save the XER register
	st	r0, off_xer (r1)
	mfctr	r0			# save the CTR register
	st	r0, off_ctr (r1)

	mflr	r0			# get the return address
	st	r0,off_iar(r1)		# save the restart pc
	st	r1,0(r3)		# save the sp in the TCB
	mr	r3,r4			# get new context to transfer 

	b	ENTRY(cma__restore_thread_ctx)
	.extern	ENTRY(cma__restore_thread_ctx)

	S_EPILOG

##page;
# cma__fetch_sp
#
# FUNCTIONAL DESCRIPTION:
# 
#	This routine returns the current value of sp (the stack pointer).
#
# FORMAL PARAMETERS:
# 
#	none
# 
# IMPLICIT INPUTS:
# 
#	none
# 
# IMPLICIT OUTPUTS:
# 
#	none
#
# FUNCTION VALUE:
# 
#	the current value of sp
# 
# SIDE EFFECTS:
# 
#	none

	S_PROLOG(cma__fetch_sp)
	
	mr	r3,r1		# get sp to return to caller

	S_EPILOG		# return

	.toc
	TOCE(cma__g_kernel_critical,data)
	TOCE(cma__g_def_ctx_buf,data)
	TOCE(cma__g_abort_signal,data)
	TOCE(cma__execute_thread, entry)
        TOCE(cma__force_dispatch, entry)
	TOCE(cma__transfer_main_abort,entry)
	TOCE(cma__do_async_alert, entry)
 #  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S
 #  *4    12-JUN-1991 19:02:00 BUTENHOF "OK... deactivate transfer_main_abort"
 #  *3    12-JUN-1991 18:56:01 BUTENHOF "Add transfer_main_abort entry"
 #  *2    31-JAN-1991 12:59:25 SCALES "Convert to stream format for ULTRIX build"
 #  *1    15-JAN-1991 22:28:14 BUTENHOF "RIOS-specific assembler source"
 #  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S


