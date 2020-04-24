/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cma_dispatch.c,v $
 * Revision 1.1.25.1  1996/05/10  13:36:10  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:40 UTC  bissen
 * 	Merge up KI context change notification from HPDCE01 branch
 * 	[1995/07/24  20:32 UTC  gaz  /main/HPDCE02/4]
 *
 * 	Bring forward KI_INFORM_CONTEXT change from Rodan
 * 	[1995/07/24  20:14 UTC  gaz  /main/HPDCE02/gaz_mothra10/1]
 *
 * 	Merge callback fixes from branch jss_mothra_fix2.
 * 	[1995/03/14  22:33 UTC  jss  /main/HPDCE02/3]
 *
 * 	Add the end callback to cma__terminate thread.  This is a better place to
 * 	call it because we are in the CMA kernel and about to switch out for the
 * 	last time. Also conditionalize out callback so that it doesn't get called
 * 	after an end callback for that thread.
 * 	[1995/03/13  19:18 UTC  jss  /main/HPDCE02/jss_mothra_fix2/1]
 *
 * 	Merge singlestack code.
 * 	[1994/10/27  21:05 UTC  chris_p  /main/HPDCE02/2]
 *
 * 	Change symbol names with "carevue" to "singlestack"
 * 	[1994/10/17  20:28 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
 *
 * 	Project carevue.
 * 	Threads now use only one stack.
 * 	Stacks are copied at context switch time.
 * 	[1994/09/06  22:52 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	Ajay's performance enhancements
 * 	[1994/05/03  18:20 UTC  jss  /main/HPDCE01/3]
 *
 * 	1. Call cma__queue_dequeue_macro from cma__dispatch().
 * 	2. Call cma__transfer_regs() and cma__restore_regs() directly from cma__dispatch()
 * 	[1994/04/29  19:08 UTC  ajayb  /main/HPDCE01/ajayb_threads_perf/1]
 *
 * 	merge from jss_dce_threads to HPDCE01
 * 	[1994/02/28  20:00 UTC  jss  /main/HPDCE01/2]
 *
 * 	Fixed the out callback logic so that the out callback is
 * 	no called if the old_tcb is equal to the current tcb.
 * 	[1994/02/28  19:41 UTC  jss  /main/HPDCE01/jss_dce_threads/2]
 *
 * 	fixed placement of out callback
 * 	[1994/02/09  22:19  jss  /main/HPDCE01/jss_dce_threads/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:31  truitt  /main/HPDCE01/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:51  devsrc]
 * 	 *
 *
 * 	svr4 code cleanup changes
 * 	[1994/04/20  20:28:13  ohara]
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:16  jd]
 *
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:27:49  hinman]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:12  cjd]
 *
 * 	Initial 486 port.
 * 	[1993/05/11  21:43:05  jd]
 *
 * 	CHFts08929: In the function cma__preempt_victim(), we
 * 	must ensure the thread trying to run always compares to
 * 	the thread that is currently running.  Because of some
 * 	corruption in the cma__g_run_vps queue, we were not
 * 	always comparing the trying to run thread with the
 * 	actually running thread.  By calling cma__get_self_tcb()
 * 	in the function, you are always ensured of comparing
 * 	the correct threads.
 * 	[1993/12/15  15:50:22  truitt]
 * 	 *
 *
 * 	Add 'in' and 'out' support for the C++ callbacks.
 * 	[1993/11/09  18:53:02  truitt]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:39:10  ajayb]
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:37:04  ajayb]
 *
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:27:49  hinman]
 *
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:12  cjd]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:10:55  prepetit]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:10:09  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:24  root  1.1.12.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:08:47  root]
 *
 * 	Initial 486 port.
 * 	[1993/05/11  21:43:05  jd]
 *
 * 	Gamera Merge II
 * 	[1993/04/25  01:23:38  hopkins]
 *
 * 	Loading drop DCE1_0_2B23
 * 	[1993/04/21  20:43:50  root  1.1.6.4]
 *
 * 	Have cma__yield_processor call cma__io_available to make any threads with
 * 	io pending ready before calling the dispatcher.
 * 	[1993/04/07  20:09:22  jd]
 *
 * 	fixed cma__would_run() to be modeled
 * 	after 1.0.2 version of cma__try_run().
 * 	[1993/03/30  22:50:04  hopkins]
 *
 * 	Have cma__yield_processor call cma__io_available to make any threads with
 * 	io pending ready before calling the dispatcher.
 * 	[1993/04/07  20:09:22  jd]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:28  hopkins]
 *
 * 	Loading O
 *
 * Revision 1.1.19.3  1994/06/09  13:37:53  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:51  devsrc]
 * 
 * Revision 1.1.19.2  1994/04/25  19:57:44  ohara
 * 	svr4 code cleanup changes
 * 	[1994/04/20  20:28:13  ohara]
 * 
 * Revision 1.1.19.1  1994/04/01  20:16:10  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:16  jd]
 * 
 * Revision 1.1.17.3  1993/06/24  20:59:34  hinman
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:27:49  hinman]
 * 
 * Revision 1.1.17.2  1993/05/24  20:49:56  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:17:12  cjd]
 * 
 * Revision 1.1.15.2  1993/05/11  22:02:18  jd
 * 	Initial 486 port.
 * 	[1993/05/11  21:43:05  jd]
 * 
 * Revision 1.1.4.8  1993/04/07  20:15:24  jd
 * 	Have cma__yield_processor call cma__io_available to make any threads with
 * 	io pending ready before calling the dispatcher.
 * 	[1993/04/07  20:09:22  jd]
 * 
 * Revision 1.1.4.7  1993/02/24  20:38:46  steveg
 * 	Remove USL Copyright; put placeholder after log comment
 * 	[1993/02/24  20:36:07  steveg]
 * 
 * Revision 1.1.4.6  1993/02/01  22:17:34  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:25:37  hinman]
 * 
 * Revision 1.1.4.5  1993/01/26  13:26:15  jd
 * 	Used _CMA_PLATFORM_ rather than _CMA_VENDOR_ for pmax and rios.
 * 	Left the svr4 check as is since I can't check the change.
 * 	[1993/01/18  17:29:55  jd]
 * 	Revision 1.1.8.2  1993/01/11  16:19:16  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.4.4  1992/12/15  22:27:54  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:11  alan]
 * 
 * Revision 1.1.4.3  1992/09/29  20:14:54  devsrc
 * 	[OT 5373]    SNI/SVR4merge.
 * 	[1992/09/17  20:42:00  sekhar]
 * 
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:41:55  bolinger]
 * 
 * Revision 1.1.4.2  1992/09/03  14:36:54  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:59  bolinger]
 * 
 * 	Officially dummy out cma__cause_force_dispatch() for ref platforms --
 * 	it's no longer needed.
 * 	[1992/08/28  18:38:26  bolinger]
 * 
 * 	Fix the last change made:  add mutex unlock to *other* "branch"
 * 	of #ifdef.
 * 	[1992/08/14  20:01:18  bolinger]
 * 
 * 	Merge up to BL10+ internal release from DEC.
 * 
 * 	Fix bug in dispell_zombies routine that caused mutex to be
 * 	unlocked twice in succesion.
 * 	[1992/08/11  20:54:59  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:15:53  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:14:15  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:41:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1992 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	These routines provide the basic cma dispatcher
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *	Webb Scales
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	1 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof & Webb Scales	27 September 1989
 *		Fix cma__init_dispatch to initialize dispatcher queues.
 *	002	Dave Butenhof & Webb Scales	27 September 1989
 *		Fix cma__unset to use interlocked instructions.
 *	003	Hans Oser			28 September 1989
 *		Fix wrong test for empty queues.
 *	004	Hans Oser			29 September 1989
 *		Fix time slice bug, add null-thread initialisation
 *		and function cma__get_async_info.
 *	005	Hans Oser and Webb Scales	03 October 1989
 *		Add ctx_buffer parameter to cma__get_async_info
 *	006	Hans Oser			06 October 1989
 *		Timer support transferred to module cma_timer
 *	007	Hans Oser			13 October 1989
 *		Changements due to program review
 *	008	Hans Oser			17 October 1989
 *		To correct loss of thread in cma__terminate_thread
 *	009	Dave Butenhof			19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	010	Dave Butenhof			19 October 1989
 *		Replace #pragmas with use of new cma_host.h functions.
 *	011	Webb Scales			20 October 1989
 *		Changed "cma_assem_vms.h" to "cma_assem.h".  Added typecasts.
 *	012	Webb Scales			20 October 1989
 *		Placed definition (ie, type and initializer) of 
 *		cma__g_kernel_critical in this file and placed 
 *		the declaration (ie, "extern") in cma_dispatch.h
 *	013	Webb Scales			23 October 1989
 *		Changed definiton of cma__force_dispatch to a routine, changed
 *		"..._PSL" to "..._ps"
 *	014	Dave Butenhof			Halloween 1989
 *		Implement queue of all threads (chained at create_thread
 *		time) so debug code can find them all.
 *	015	Dave Butenhof			All Saints Day 1989
 *		Remove cma__enter_kernel (it's now a macro).
 *	016	Dave Butenhof			1 November 1989
 *		Move known TCB init to cma__init_static routine for
 *		consistency with other known foo inits.
 *	017	Dave Butenhof			2 November 1989
 *		Fix init to avoid doubly-inserting default TCB on known
 *		threads queue.
 *	018	Dave Butenhof and Bob Conti	3 November 1989
 *		Fix undefer logic to avoid deadlocking on entry from
 *		cma__assign; also change name of "dispatch_defer" flag to
 *		"do_redispatch" for clarity.
 *	019	Webb Scales			3 November 1989
 *		Revamp cma__defer and cma__undefer.
 *	020	Webb Scales			3 November 1989
 *		Fix cma___t_defer_q_entry declaration, and cma__t_semaphore
 *		typecasts in cma__incr_sem arguments.
 *	021	Webb Scales			3 November 1989
 *		Make call to "cma_error" into "cma__error"
 *	022	Bob Conti			5 November 1989
 *		Add undefer calls to *all* callers of cma__assign to 
 *		compensate for our having moved undefer out of cma__assign.
 *		Remove the now-superfluous include of cma_host.
 *		Replace inefficient per-dispatch time-stamping by 
 *		incrementing the current thread's runtime in cma__periodic.
 *		Delete middle (time) parameter from cma__assign.
 *		Add the flag defers_processed that prevents useless calls to 
 *		undefer.
 *	023	Dave Butenhof			6 November 1989
 *		Fix some comments (especially function descriptions).
 *	024	Webb Scales & Bob Conti		6 November 1989
 *		Made cma__ready into a macro and put the quatum increment into
 *		it.  Remove parameter from cma__dispatch.  Removed kernel lock 
 *		form cma__AST_forced_dispatch.  Renamed cma__dispatch to 
 *		cma__yield_processor.  Renamed cma__assign to cma__dispatch.
 *	025	Webb Scales	7 November 1989
 *		Changed name of cma__AST_force_dispatch to 
 *		cma__cause_force_dispatch.  Added include of cma_vm.h
 *	026	Dave Butenhof	17 November 1989
 *		Changes to comply with new timer module.
 *	027	Webb Scales	18 November 1989
 *		Changed interface to cma__get_async_info to single parameter.
 *		Made parameters to cma__cause_force_dispatch system specific.
 *	028	Webb Scales	19 November 1989
 *		Add include for signal header file on ultrix.
 *	029	Dave Butenhof	21 November 1989
 *		Fix debugging hooks so "threads" command can work.
 *	030	Dave Butenhof	1 December 1989
 *		Initialize scheduling policy on null thread.
 *	031	Webb Scales	10 December 1989
 *		Add deferal action for Unix non-blocking I/O.
 *	032	Dave Butenhof	11 December 1989
 *		Fix deferral... since defer advances before inserting,
 *		undefer needs to advance before removing.
 *	033	Webb Scales	11 December 1989
 *		Re-alphabetized the routines.
 *		Moved the call to cma__undefer to inside cma__dispatch.
 *		Fixed undefer to advance before removing.
 *	034	Webb Scales	12 December 1989
 *		Corrected cma__augment_defer_queue, so that it now works.
 *	035	Webb Scales & Dave Butenhof	14 December 1989
 *		undefer unblock_all should generate pending spurious wakeup.
 *	036	Dave Butenhof	26 January 1990
 *		Change cma__get_self_tcb_kernel to cma__get_self_tcb (it no
 *		longer locks the kernel, so distinction is irrelevant).
 *	037	Dave Butenhof	13 February 1990
 *		Context switch errno global cell.
 *	038	Dave Butenhof	26 February 1990
 *		Change use of priority symbol to new standard.
 *	039	Dave Butenhof	2 March 1990
 *		Integrate Kevin Ackley's changes for Apollo M68000 port.
 *	040	Dave Butenhof	21 March 1990
 *		Make null thread do pause in loop.
 *	041	Webb Scales & Dave Butenhof	29 March 1990
 *		Make null thread $hiber on VAX/VMS and call 'select' on
 * 		U*ix.
 *	042	Dave Butenhof	6 April 1990
 *		Null thread must RERAISE debug exception.
 *	043	Dave Butenhof	9 April 1990
 *		Back off 042---TRY/ENDTRY catching doesn't work for
 *		continuable exceptions, and correct fix is now in exception
 *		code (SS$_DEBUG isn't passed to C exception package).
 *	044	Dave Butenhof	9 April 1990
 *		Use new "known_object" structure for known condition queue
 *		(includes mutex).
 *	045	Dave Butenhof & Webb Scales	9 April 1990
 *		Implement zombies and anti-zombie spells.  This allows a
 *		terminating thread to survive until context switched out,
 *		while ensuring that it's eventually dispelled by the
 *		appropriate incantation.
 *	046	Webb Scales	30 May 1990
 *		Add reference count to tcb for joiners to prevent zombies from
 *		being dispelled prematurely.
 *	047	Webb Scales	15 June 1990
 *		Added priority scheduling:
 *		- Made the ready list an array of queue headers.
 *		- Added global "next ready queue" pointer.
 *		- Changed cma__ready from a macro to a routine.
 *		- Modified dispatch routine.
 *		- Added a bunch of local macros.
 *		- Added defer's for tick updates
 *	048	Webb Scales	26 June 1990
 *		Fixed a bug which occurs when a thread dispatches to itself.
 *	049	Webb Scales	25 July 1990
 *		Moved defer functions to their own module.
 *	050	Dave Butenhof	14 August 1990
 *		Generalize cma__cause_force_dispatch so it can be used for
 *		asynchronous alerts as well as timeslicing.
 *	051	Webb Scales	15 August 1990
 *		Incorporated Apollo changes, adding HPPA and SUN 68K platforms.
 *	052	Bob Conti	9 September 1990
 *		Add code to report key state-transition events to the debugger.
 *	053	Dave Butenhof	28 November 1990
 *		Integrate IBM RIOS changes provided by Dave Mehaffy.
 *	054	Dave Butenhof	5 February 1991
 *		Rename "errno" field.
 *	055	Dave Butenhof	6 February 1991
 *		Modify null thread loop to attempt to yield the processor
 *		before returning to $hiber (on VAX/VMS). This allows the
 *		cma_cond_signal_int function to get better performance (when
 *		other threads may be idle) by performing a $wake.
 *	056	Webb Scales	18 March 1991
 *		Changed priority-adjusting scheduling to do priority calculation
 *		only when there is new "data".
 *	057	Dave Butenhof	25 March 1991
 *		Change exception interface names
 *	058	Dave Butenhof	05 April 1991
 *		Support parallel processing via Mach threads...
 *		_CMA_KTHREADS_ defines type of kernel threads (currently,
 *		_CMA__MACH and _CMA__NONE are supported). _CMA_MULTIPLEX_
 *		defines whether user->kernel thread mapping is one-on-one or
 *		multiplexed (currently only one-on-one is supported). The
 *		_CMA_MULTIPLEX_ symbol must be 0 if _CMA_UNIPROCESSOR_ is
 *		true (so it's really "multiprocessor-multiplexed"). The
 * 		symbol _CMA_THREAD_IS_VP_ is set when kernel thread mapping
 *		is one-to-one, for convenience.
 *	059	Paul Curtin	15 April 1991
 *		Added include of thread.h
 *	060	Dave Butenhof	13 May 1991
 *		Use cma__trace_kernel() where kernel is locked, to avoid a
 *		deadlock.
 *	061	Dave Butenhof	23 May 1991
 *		Under OSF/1 systems, the default thread/VP needs to set up
 *		default handling for the per-thread signals by calling
 *		cma__sig_thread_init().
 *	062	Paul Curtin	31 May 1991
 *		Added a reinit routine.
 *	063	Dave Butenhof	31 May 1991
 *		Change locking in cma__terminate_thread.
 *	064	Dave Butenhof	06 June 1991
 *		Integrate IBM reverse-drop changes (RS/6000 path of
 *		cma__cause_force_dispatch()).
 *	065	Webb Scales and Dave Butenhof	    10 June 1991
 *		Conditionalize inclusion of I/O stuff.
 *	066	Dave Butenhof	05 September 1991
 *		Add call to new function cma__vp_yield() to implement
 *		cma__yield_processor() on VP-based DECthreads.
 *	067	Dave Butenhof	18 September 1991
 *		Merge Apollo CMA5 reverse drop. They reimplement the standard
 *		DECthreads logic to switch to main thread and deliver a
 *		terminating signal; instead of using a signal exit trampoline
 *		function, force the next context switch to the main thread,
 *		and when the main thread is scheduled check for a pending
 *		termination signal (this isn't an optimal general solution,
 *		since it adds a test on the "fast path", so I'll leave it in
 *		Apollo condition code).
 *	068	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	069	Paul Curtin	18 November 1991
 *		Alpha work: added include for stdlib and an Alpha switch.
 *	070	Dave Butenhof	27 November 1991
 *		Modify errno context switch code for DEC C.
 *	017	Dave Butenhof	05 December 1991
 *		For a little recreational diversion from Alpha, implement
 *		priority-ordering of realtime threads on a block queue (i.e.,
 *		for mutex lock and condition wait).
 *	018	Dave Butenhof	09 January 1992
 *		Support Alpha hardware in cma__cause_force_dispatch.
 *	019	Dave Butenhof	21 January 1992
 *		Add support for DEC OSF/1 shared library version by saving
 *		some extra registers picie needs in cause_force_dispatch.
 *	020	Dave Butenhof	24 January 1992
 *		Fix a type mismatch (MIPS C) on 019.
 *	021	Webb Scales	28 January 1992
 *		Reworked end-of-quantum preemption on VMS.
 *	022	Dave Butenhof	30 January 1992
 *		Remove cma__trace_kernel calls.
 *	023	Webb Scales	30 January 1992
 *		- Implement our own private $CLRAST.
 *		- Move the undefer which is done during dispatching so that
 *		  it is done before the out-bound thread is queued anywhere.
 *		- Repaired the damage to cause-force-dispatch on VAX/VMS so
 *		  that async alerts will work...until we do them for Alpha.
 *	024	Webb Scales and Dave Butenhof	6 February 1992
 *		Removed unneeded initialization of a/synch contexts
 *	025	Dave Butenhof	10 February 1992
 *		Call cma__vp_dump() before bugcheck if cma__vp_delete()
 *		returns. Also, adjust to new VM protocol.
 *	026	Webb Scales	18 February 1992
 *		- Make code for picie conditionally compiled for picie only.
 *		- Change the exit-kernel to unset-kernel in the Apollo-specific
 *		    abort-process code.
 *		- Removed explicit calls to undefer.
 *	027	Webb Scales	25 February 1992
 *		Reorganized the errno-switching code.
 *	028	Dave Butenhof	10 March 1992
 *		Change timeb field references to timeval, since
 *		cma_t_date_time has been changed to avoid requiring libbsd.a
 *		on AIX and OSF/1.
 *	029	Dave Butenhof	12 March 1992
 *		Use correct symbol in reinit code rather than literal "2".
 *	030	Webb Scales	13 March 1992
 *		Parameterize scheduling policies.
 *	031	Dave Butenhof	19 March 1992
 *		Add call to cma__undefer() before all direct calls to
 *		cma__yield_processor() -- since cma__ready() no longer
 *		undefers.
 *	032	Webb Scales	31 March 1992
 *		Rework asynch context switch for U*ix.
 *	033	Dave Butenhof	15 April 1992
 *		Add check for resheduled terminated thread, and bugcheck.
 *	034	Dave Butenhof	20 April 1992
 *		Modify 033 -- bugcheck only if state is terminated; the
 *		terminated flag is unreliable (it's set without kernel
 *		interlock, and is only used as a predicate for join).
 *	035	Webb Scales	18 May 1992
 *		Initialize temp queue headers in sort-ready-queues.
 *	036	Webb Scales	19 May 1992
 *		Cleanse the null thread's queue links during re-init after fork.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_queue.h>
#include <cma_stack.h>
#include <cma_tcb.h>
#include <cma_assem.h>
#include <cma_dispatch.h>
#include <cma_timer.h>
#include <cma_vm.h>
#include <cma_int_errno.h>
#if _CMA_OS_ == _CMA__UNIX
# include <signal.h>
# if !_CMA_THREAD_SYNC_IO_
#  include <cma_thread_io.h>
# endif
#endif
#include <cma_deb_core.h>
#include <cma_deb_event.h>
#include <cma_sched.h>
#include <cma_vp.h>
#include <cma_defer.h>
#include <cma_debug_client.h>
#include <cma_thread.h>
#include <cma_semaphore.h>
#include <cma_defer.h>
#include <stdlib.h>
#ifndef _HP_INFORM_CONTEXT_
# if __hpux1000p
# include <sys/unistd.h>
# endif  /* __hpux1000p */
#endif  /* _HP_INFORM_CONTEXT_ */

#ifdef _HP_LIBC_R
extern int h_errno;
#endif

/*
 * GLOBAL DATA
 */
#ifndef _HP_CMA_PERF_
int cma__ck_io_at_yield = 0;
#endif /* _HP_CMA_PERF_ */

#ifdef HP_CMA_SINGLESTACK_OPT
extern int cma_singlestack_opt;
#endif

/*
 * Threads put on hold by debugger
 */
cma__t_queue	cma__g_hold_list;		

/*
 * Array of ready queues
 */
cma__t_queue	cma__g_ready_list[cma__c_prio_n_tot];

/*
 * Next ready queue to check
 */
cma_t_integer	cma__g_next_ready_queue = 0;

#ifdef __hpux
/*
 * flag indicating if we're currently in
 * cma__dispatch, so that dispatch is not
 * called recursively.
 */
cma_t_integer	cma__g_in_dispatch = 0;

/*
 * callback structure
 */
extern _ctxcb_spec_t	_cb;

#endif

/*
 * FIX-ME:  These are only here temporarily, until I decide on a permanent home
 *		for them.
 */
cma_t_integer	cma__g_prio_bg_min,
  		cma__g_prio_bg_max,
		cma__g_prio_fg_min,
  		cma__g_prio_fg_max,
  		cma__g_prio_m_0,
  		cma__g_prio_m_1,
  		cma__g_prio_m_2,
  		cma__g_prio_m_3,
  		cma__g_prio_b_0,
  		cma__g_prio_b_1,
  		cma__g_prio_b_2,
  		cma__g_prio_b_3,
  		cma__g_prio_p_1,
  		cma__g_prio_p_2,
  		cma__g_prio_p_3;
cma__t_queue	cma__g_run_vps;		/* Currently running processors */
cma__t_queue	cma__g_susp_vps;	/* Currently suspended processors */
cma_t_integer	cma__g_vp_count;	/* Current number of processors */
cma_t_integer	cma__g_vp_running;	/* Number of running processors */
cma_t_integer	cma__g_threadcnt;	/* Number of threads */

/*
 * LOCAL DATA
 */

#define cma___c_init_cpu_time	    cma__scale_up (10)

static cma__t_int_tcb	*cma___g_null_tcb;	/* Null thread */
static cma__t_queue	cma___g_zombies;
static cma__t_semaphore	cma___g_lastthread;	/* Wait for last thread */

/*
 * LOCAL MACROS
 */

/*
 * Number of null threads (one-to-one mapped kernel thread implementations
 * generally don't have any; n to m mapped kernel thread implementations
 * might need more than one).
 */
#if _CMA_THREAD_IS_VP_
# define cma___c_null_thd_cnt	0
#else
# define cma___c_null_thd_cnt	1
#endif

/*
 * Policies which cause preemption if a lower priority thread is running
 */
#define cma___preempt_policy(tcb)   \
	(cma__sched_is_rt (tcb) || (tcb)->sched.policy == cma_c_sched_ada_low)

/*
 * LOCAL FUNCTIONS
 */

cma_t_address cma___null_thread   (void);

void cma___sort_ready_list  (void);

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine blocks the current thread. If the thread has a realtime
 *	priority (RR or FIFO policy), it is placed at the front of the queue
 *	in priority order. If the thread is non-realtime, it is simply dumped
 *	at the end of the queue.
 *
 *  FORMAL PARAMETERS:
 *
 *	blocked_list    - Address of the queue, where the thread will be
 *			  blocked.
 *	milliseconds	- Number of milliseconds to block before timeout
 *			  (used for timed semaphore wait in VP
 *			  implementation)
 *
 *  IMPLICIT INPUTS:
 *
 *	the current thread tcb
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	cma_c_true if block completed; cma_c_false if it timed out.
 *
 *  SIDE EFFECTS:
 *
 *	The first thread from the ready list will be taken and become
 *	the running thread.
 */
extern cma_t_boolean
cma__block(
	cma__t_queue	*blocked_list,
	cma_t_integer	milliseconds)
    {
    cma__t_int_tcb	*cur_tcb;	/* Current thread's TCB  */
    cma_t_boolean	status;


    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__block with kernel unlocked");

    cur_tcb = cma__get_self_tcb ();	/* Get running thread */

    cur_tcb->state = cma__c_state_blocked;/* current thread's state is blocked*/

    if (cur_tcb->timer.quanta_remaining > 0)
        cur_tcb->timer.quanta_remaining = 0;

    /*
     * If the thread has a realtime policy, sort it into the beginning of the
     * block queue. Otherwise, just insert it at the end.
     *
     * FIX-ME: This doesn't currently sort ADA_LOW threads, which probably
     * violates their definition as "low priority realtime". I won't put
     * them at the front, because I don't want them to have priority over
     * throughput threads. I won't sort them at the end (which would be the
     * "right" solution) because that would mean locating the right place to
     * put throughput and background threads in the middle, and I don't want
     * them to pay the overhead (this could be solved by expanding the block
     * queue to have a start-of-ada_low pointer, but I don't think it's worth
     * the effort right now).
     */
    if (cur_tcb->sched.class == cma__c_class_rt) {
	cma__t_queue	*ptr = cma__queue_next (blocked_list);
	int		priority = cur_tcb->sched.priority;

	while (ptr != blocked_list) {
	    cma__t_int_tcb	*tcb = (cma__t_int_tcb *)ptr;
	    cma__t_sched_class	sclass = tcb->sched.class;
	    int			spriority = tcb->sched.priority;

	    /*
	     * If the next blocked thread's policy has a greater value than
	     * our's (it's "less realtime"), or if it has the same policy
	     * value but a lower priority, then break out of the loop and
	     * insert the new thread BEFORE the one that matched the test.
	     */
	    if ((sclass != cma__c_class_rt) || (spriority < priority))
		break;

	    ptr = cma__queue_next (ptr);
	    }

	/*
	 * Now just insert it: if we didn't find what we were looking for,
	 * "ptr" is back at the queue head, so we'll put it on the end of the
	 * list... which is exactly what we want.
	 */
	cma__queue_insert (&cur_tcb->header.queue, ptr);
	}
    else
	cma__queue_insert (&cur_tcb->header.queue, blocked_list);

#ifdef _CMA_DEBEVT_
    if (cma__g_debug_state.events_enabled)
	cma__debevt_report (cma__c_debevt_blocking);
#endif

    /*
     * Select next ready thread
     */
    status = cma__dispatch (cur_tcb, cma_c_true, milliseconds);

#if _CMA_THREAD_IS_VP_
    /*
     * Remove entry only if it timed out.
     */
    if (!status)
	(void)cma__queue_remove (&cur_tcb->header.queue);
#endif

    return status;
    }

#if _CMA_PLATFORM_ == _CMA__MIPS_UNIX || _CMA_PLATFORM_ == _CMA__IBMR2_UNIX || _CMA_PLATFORM_ == _CMA__SINIX_MX300I || _CMA_PLATFORM_ == _CMA__DCOSX_MIPS || _CMA_PLATFORM_ == _CMA__I386_UNIX
/*
 * On the reference ports, the entire force_dispatch mechanism
 * has been replaced with the simple direct use of sigprocmask()
 * on asymchronous context switches.
 *
 * Any call to cma__cause_force_dispatch() is an error on these
 * platforms.
 */
void
cma__cause_force_dispatch
	(
	cma__t_int_tcb	    *cur_tcb,
	cma_t_address	    target_pc,
	cma__t_sigcontext   *scp)
{
	cma__bugcheck("cma__cause_force_dispatch() called -- shouldn't happen");
}

#else	/* _CMA_VENDOR_ == _CMA__IBM || _CMA_VENDOR_ == _CMA__OSF || _CMA_VENDOR == _CMA__SVR4 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine is called by a signal or AST service routine to initiate
 *	an asynchronous context switch.
 *
 *	First, any context which is not readily available outside of the
 *	signal or AST environment is saved.  Then the return PC and processor
 *	status are set so that, when the signal or AST routine returns,
 *	execution will resume in the asynchronous context switch code, instead
 *	of at the point of interruption, with the processor in a known state.
 *
 *  FORMAL PARAMETERS:
 *
 *	cur_tcb	    The address of the current thread's TCB (read only)
# if (_CMA_OS_ == _CMA__UNIX) && (_CMA_HARDWARE_ != _CMA__VAX)
 *	scp	    The address of the signal context block.  Several fields
 *			in this block are copied into the TCB, and the sc_pc
 *			field is set to the address of cma__force_dispatch.
# endif
# if _CMA_HARDWARE_ == _CMA__VAX
 *	saved_pc    The address of the instruction at the point of 
 *			interruption.  The value of this parameter is 
 *			stored in the TCB and replaced by the address of 
 *			cma__force_dispatch.
 *
 *	saved_ps    The value of the processor status register at the point of 
 *			interruption.  The value of this parameter is stored 
 *			in the TCB and replaced by a known value.
# endif
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
void
cma__cause_force_dispatch (
# if (_CMA_OS_ == _CMA__UNIX) && (_CMA_HARDWARE_ != _CMA__VAX) && (_CMA_HARDWARE_ != _CMA__ALPHA)
	cma__t_int_tcb	    *cur_tcb,
	cma_t_address	    target_pc,
	cma__t_sigcontext   *scp)
# endif
# if (_CMA_HARDWARE_ == _CMA__VAX) || (_CMA_HARDWARE_ == _CMA__ALPHA)
	cma__t_int_tcb	    *cur_tcb,
	cma_t_address	    target_pc,
	cma_t_address	    *saved_pc,
	cma_t_address	    *saved_ps)
# endif
    {
    cma__assert_fail(
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__cause_force_dispatch with kernel unlocked");

# if _CMA_HARDWARE_ == _CMA__MIPS
    /*
     * Save the information which we have now but will need later, when it is
     * hard to get to.
     */
    cur_tcb->async_ctx.sig_mask     = (long int)scp->sc_mask;	/* Save signal mask */
    cur_tcb->async_ctx.restart_pc   = (long int)scp->sc_pc;	/* Save PC at signal */
    cur_tcb->async_ctx.used_fpc     = (long int)scp->sc_ownedfp;	/* Used the FPC */
    cur_tcb->async_ctx.fpc_csr      = (long int)scp->sc_fpc_csr;	/* The FPC C/S reg */
    cur_tcb->async_ctx.fpc_eir      = (long int)scp->sc_fpc_eir;	/* The FPC EI reg */
    cur_tcb->async_ctx.cp0_cause    = (long int)scp->sc_cause;	/* The CP0 cause reg */
    cur_tcb->async_ctx.cp0_badvaddr = (long int)scp->sc_badvaddr;	/* The CP0 bad VA */
    cur_tcb->async_ctx.cpu_badpaddr = (long int)scp->sc_badpaddr;	/* The CPU bd bad PA */

#  ifdef  _CMA_SHLIB_
    /*
     * Save some context which picie modifies before we have a chance to
     * stash it in cma__force_dispatch. Also set up the $r25 register for the
     * call into cma__force_dispatch since picie assumes it has been entered
     * via a "jal ra, r25".
     */
    cur_tcb->async_ctx.t9           = (long int)scp->sc_regs[25];
    cur_tcb->async_ctx.gp           = (long int)scp->sc_regs[28];
    scp->sc_regs[25]                = (long int)target_pc;
#  endif

    /*
     * Cause execution to resume at cma__forced_dispatch instead of at the
     * point of interruption.
     */
    *((cma_t_address *)(&scp->sc_pc)) = target_pc;
# endif

#if _CMA_HARDWARE_ == _CMA__I386

    /*
     * Save the interrupted thread's asynchronous context.
     */

    cur_tcb->async_ctx.a_sigmask        = scp->sc_mask; /* signal mask */
    cur_tcb->async_ctx.a_edx            = scp->sc_edx;  /* saved volatile */
    cur_tcb->async_ctx.a_ecx            = scp->sc_ecx;  /* registers */
    cur_tcb->async_ctx.a_eax            = scp->sc_eax;
    cur_tcb->async_ctx.a_eip            = scp->sc_eip;  /* saved ip */
    cur_tcb->async_ctx.a_efl            = scp->sc_efl;  /* flags word */

    /*
     * Cause execution to resume at cma__forced_dispatch instead of at the
     * point of interruption.
     */

    *((cma_t_address *)(&scp->sc_eip)) = target_pc;
#endif /* _CMA_HARDWARE_ == _CMA_I386 */

#if _CMA_HARDWARE_ == _CMA__CPLMIPS
#endif
# if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
	{
	int toc = cma__get_toc ();

	bcopy(scp, &cur_tcb->async_ctx.ibmr2_scp, sizeof(struct sigcontext));
	scp->sc_jmpbuf.jmp_context.iar = *(long *)target_pc;
	toc = cma__get_toc();
	scp->sc_jmpbuf.jmp_context.gpr[1] =
	    &cur_tcb->ctx_stack[cma__c_ibmr2_ctx_stack_top];
	scp->sc_jmpbuf.jmp_context.gpr[2] = toc;
	}
# endif
# if (_CMA_HARDWARE_ == _CMA__VAX) || (_CMA_HARDWARE_ == _CMA__ALPHA)
#  if _CMA_OS_ != _CMA__VMS
    /*
     * Save the interrupted's thread PC and PS in its tcb.
     */
    cur_tcb->async_ctx.pc  = (long int)*saved_pc;
    cur_tcb->async_ctx.psl = (long int)*saved_ps;
#  endif

    /*
     * Cause execution to resume at cma__forced_dispatch instead of at the
     * point of interruption (after placing the processor in a known state).
     */
    *saved_pc = target_pc;
    *saved_ps = (cma_t_address)cma__c_default_ps;
# endif
# if _CMA_HARDWARE_ == _CMA__M68K
    cur_tcb->async_ctx.restart_pc   = scp->sc_pc;	/* Save PC at signal */
    cur_tcb->async_ctx.restart_ccr  = (short)scp->sc_ps;	/* Save CCR at signal */

    /*
     * Cause execution to resume at cma__forced_dispatch instead of at the
     * point of interruption.
     */
    *((cma_t_address *)(&scp->sc_pc)) = target_pc;
# endif
    }
#endif  /* _CMA_VENDOR_ == _CMA__IBM || _CMA_VENDOR_ == _CMA__OSF  || _CMA_VENDOR_ ==_CMA__SNI || _CMA_VENDOR_ == _CMA__PTC*/

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine resumes the next ready thread
 *
 *  FORMAL PARAMETERS:
 *
 *	old_tcb		The current running TCB (which is being replaced)
 *
 *	save_context	cma_c_true causes current context to be saved.
 *			cma_c_false is used if the current context has already
 *			been saved (a timesliced thread, possibly) or if the
 *			context need not be saved (a terminating thread).
 *	milliseconds	Number of milliseconds to block before timeout
 *			(used for timed semaphore wait in VP implementation)
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	cma_c_true if returns normally; cma_c_false if suspend timed out.
 *
 *  SIDE EFFECTS:
 *
 *	The first thread from the highest priority ready list will become
 *	the running thread.
 */
extern cma_t_boolean
cma__dispatch (
	cma__t_int_tcb	*old_tcb,
	cma_t_boolean	save_context,
	cma_t_integer	milliseconds)
    {
#ifndef HP_CMA_QDEQUEUE_OPT
    cma__t_queue   *first;
#endif


    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__dispatch with kernel unlocked");

    /*
     * Catch up on deferred actions (ready additional threads, update clock)
     */
#ifdef __hpux
    /*
     * Make sure we don't get called recursively
     * via cma__undefer ...
     */
    cma__g_in_dispatch = 1;
    if (!cma__kernel_set (&cma__g_defers_processed)) cma__undefer ();
    cma__g_in_dispatch = 0;
#else
    if (!cma__kernel_set (&cma__g_defers_processed)) cma__undefer ();
#endif

    cma__sched_update_time (
	    old_tcb->sched.cpu_time, 
	    old_tcb->sched.cpu_ticks + 1);


#if !_CMA_THREAD_IS_VP_
    while (cma__queue_empty (&cma__g_ready_list[cma__g_next_ready_queue])) {

	if (cma__g_next_ready_queue > 0)
	    cma__g_next_ready_queue--;
	else {
# if _CMA_MULTIPLEX_
	    cma__queue_remove (&old_tcb->sched.processor->queue);
	    cma__queue_insert (
		    &old_tcb->sched.processor->queue, 
		    &cma__g_susp_vps);

	    cma__exit_kernel ();

	    if (cma__vp_suspend (old_tcb->sched.processor->vp_id, 0) != cma__c_vp_normal)
		cma__bugcheck ("dispatch: VP suspend failed");

	    cma__enter_kernel ();
# else
	    cma__bugcheck ("dispatch: no available VP (uniprocessor)");	/* Shouldn't get here in uniprocessor */
# endif
	    }

	}

#if _CMA_VENDOR_ == _CMA__APOLLO
    /*
     * If there's an abort signal pending, we're going to switch to the
     * default thread no matter what.  Otherwise, pick the next ready
     * thread.
     */
    if (cma__g_abort_signal != 0) {
        cma__g_current_thread = &cma__g_def_tcb;
        }
    else 
#endif
#ifndef HP_CMA_QDEQUEUE_OPT
        {
        first = (cma__t_queue *) (&cma__g_ready_list[cma__g_next_ready_queue])->flink;
        cma__g_current_thread = (cma__t_int_tcb *)
         (cma__queue_dequeue_fast (&cma__g_ready_list[cma__g_next_ready_queue], first));
        }
#else
	cma__g_current_thread = (cma__t_int_tcb *)cma__queue_dequeue (
		&cma__g_ready_list[cma__g_next_ready_queue]);
#endif

    if (cma__g_current_thread->state == cma__c_state_terminated)
	cma__bugcheck ("dispatch: about to dispatch to terminated thread");

    cma__g_current_thread->state = cma__c_state_running;
    cma__g_current_thread->sched.cpu_ticks = 0;

    /*
     * If there were no other ready threads of higher priority, then we are
     * rescheduling the thread which just ran, and there is no more work to 
     * do.  Otherwise, save and restore thread states, etc.
     */
    if (cma__g_current_thread != old_tcb) {

#ifdef __hpux
	/*
	 * Execute out callback function if it exists. Don't execute
	 * this callback if we are switching out for the last time.
	 * (i.e. the thread has terminated and the end callback was run)
	 */
        if (_cb.out && save_context) {
	    cma__t_int_tcb  *tmp_tcb = cma__g_current_thread;
	    cma__g_current_thread = old_tcb;
	    (*_cb.out)();
	    cma__g_current_thread = tmp_tcb;
	}
#endif

	/*
	 * Move the per-processor data to the new tcb.
	 */
	cma__g_current_thread->sched.processor = old_tcb->sched.processor;
	old_tcb->sched.processor = (cma__t_vp *)cma_c_null_ptr;

/* 
 * FIX-ME: At some point we will compile this file with and 
 * without the _CMA_DEBEVT_ flag defined, so that there can be
 * absolutely no overhead for reporting debugging events when the debugger 
 * until the debugger is first invoked by the user.  This will require
 * more work.  For now, the run-time check for any-debug-events is an
 * 80% solution.
 */
# ifdef _CMA_DEBEVT_
	/*
	 * Remember information in the new thread about what thread got 
	 * preempted from the processor. The preemption event is reported
	 * except when the prior thread voluntarily blocked or terminated.
	 */
	if (cma__g_debug_state.events_enabled
		&& (old_tcb->state != cma__c_state_blocked)
		&& (old_tcb->state != cma__c_state_terminated)) {
	    cma__g_current_thread->debug.did_preempt = cma_c_true;
	    cma__g_current_thread->debug.preempted_tcb = old_tcb;
	    }
# endif

	/*
	 * Save the current value of the global errno cell into the outgoing 
	 * TCB, and set it from the incoming TCB.  This is a uniprocessor 
	 * "compromise" to full support for per-thread errno (which requires 
	 * all libraries and client code to use a new errno macro that 
	 * references the TCB cell directly).
	 */
#if _CMA_ERRNO_TYPE_ != _CMA__INTERN_FUNC
# if (_CMA_UNIX_TYPE ==  _CMA__SVR4) || (_CMA_VENDOR_ == _CMA__HP)
        old_tcb->thd_errno = errno;
        errno = cma__g_current_thread->thd_errno;
# else
	old_tcb->thd_errno = cma__get_errno();
	cma__set_errno (cma__g_current_thread->thd_errno);
# endif /* _CMA_UNIX_TYPE == _CMA__SVR4) */
# if _CMA_OS_ == _CMA__VMS
	old_tcb->thd_vmserrno = vaxc$errno;
	vaxc$errno = cma__g_current_thread->thd_vmserrno;
# endif
#endif

#ifdef _HP_LIBC_R
	old_tcb->thd_h_errno = h_errno;
	h_errno = cma__g_current_thread->thd_h_errno;
	old_tcb->thd_rpcerr = rpc_createerr;
	rpc_createerr = cma__g_current_thread->thd_rpcerr;
#endif

	/*
	 * If this dispatch is an asynchronous context switch (the result of an 
	 * end-of-quantum event) then we are currently running at AST level.
	 * If we are switching to a thread which was switched out asynchronously
	 * then everything is fine: restore it and let it return from the AST.
	 * But if we are switching to a thread which was switched sychronously
	 * then we need to get rid of the AST.
	 */
	if (old_tcb->async_ctx.valid)
	    if (!cma__g_current_thread->async_ctx.valid) {
		/* 
		 * Async-to-Sync:
		 *
		 * "Dismiss" the interrupt context.  (Allow other interrupts
		 * to be delivered.)
		 */
		cma__clear_interrupt (old_tcb->async_ctx.interrupt_ctx);
		}
# if _CMA_OS_ == _CMA__UNIX
	    else {
		/*
		 * Async-to-Async:
		 *
		 * The thread that we are switching to was also interrupted.
		 * As part of restoring it, the system will reset the process
		 * signal mask, so provide it with an appropriate value.
		 */
		*(sigset_t *)cma__g_current_thread->async_ctx.interrupt_ctx = 
		    *(sigset_t *)old_tcb->async_ctx.interrupt_ctx;
		}
	else
	    if (cma__g_current_thread->async_ctx.valid) {
		/* 
		 * Sync-to-Async:
		 *
		 * The thread that we are switching to was also interrupted.
		 * As part of restoring it, the system will reset the process
		 * signal mask, so provide it with an appropriate value.
		 */
# if (_CMA_UNIX_TYPE ==  _CMA__SVR4)
		  if (__nr_sigprocmask (
# else
		if (sigprocmask (
# endif
			SIG_SETMASK, 
			cma_c_null_ptr, 
			cma__g_current_thread->async_ctx.interrupt_ctx) == -1)
		    cma__bugcheck("cma_dispatch: Sync-to-Async");
		}

	    /*
	     * Note that no action is required for the Sync-to-Sync case.
	     */
# endif

	/*
	 * Switch to new thread context
	 */
	if (save_context)
#ifdef HP_CMA_SINGLESTACK_OPT
		if (cma_singlestack_opt)
	    	cma_singlestack_transfer_regs(
		      old_tcb, 
		      cma__g_current_thread);
		else
#endif
#if !defined(HP_CMA_DISPATCH_OPT) && defined(__hpux) 
            cma__transfer_regs (
#else
	    cma__transfer_thread_ctx (
#endif
		    &old_tcb->static_ctx,
		    &cma__g_current_thread->static_ctx);
	else
#ifdef HP_CMA_SINGLESTACK_OPT
		if (cma_singlestack_opt)
	    	cma_singlestack_restore_regs(
		    	cma__g_current_thread);
		else
#endif
#if !defined(HP_CMA_DISPATCH_OPT) && defined(__hpux) 
            cma__restore_regs (
#else
	    cma__restore_thread_ctx (
#endif
		    &cma__g_current_thread->static_ctx);

#ifndef _HP_INFORM_CONTEXT_
# if __hpux1000p
	(void) set_userthreadid(cma__g_current_thread->header.sequence);
# endif  /* __hpux1000p */
#endif  /* _HP_INFORM_CONTEXT_ */

#ifdef __hpux
	if (_cb.in)
	    (*_cb.in)();
#endif

# ifdef _CMA_DEBEVT_
	
	/* 
	 * We are about to run.  Notify the debugger of the interesting 
	 * dispatch events.
	 * 
	 * We must report the debugging events here because they must
	 * be reported in the about-to-run thread after it is on its
	 * own stack.
	 * 
	 * NOTES: 
	 * 1) A  "minor" discrepancy in new state is unavioidable due to the 
	 *    fact that we must borrow a few of the thread's registers to
	 *    implement events! The important thing to get right is the stack.
	 * 2) Assumptions: the new thread called cma__dispatch previously to 
	 *    save its context; declaring the event will not destroy any
	 *    aspect of the thread's state unless the user explicitly 
	 *    does it (declaring an event must be "modular").
	 * 3) For now, as a uniform rule, events are all reported while in 
	 *    the kernel.  
	 *    This limits what the user can do when an event is reported, but 
	 *    declaring events outside the kernel is difficult to 
	 *    implement. *WARNING* don't try to leave the kernel and 
	 *    then reenter it to report events; this can break our callers
	 *    who expect that the kernel lock will be held while in 
	 *    the context of the thread that called cma__dispatch.
	 */

	/*
	 * If the debugger created an inconsistency in the dispatch
	 * database, then yield the processor, which knows how to 
	 * clean things back up.  Note this enforces the invariant that
	 * we never leave dispatch with an inconsistency.
	 */
	while (cma__g_debug_state.is_inconsistency) {
	    cma__undefer ();
	    cma__yield_processor ();
	    }


	if (cma__g_debug_state.events_enabled) {
	    cma__t_int_tcb	*cur_tcb;

	    cur_tcb = cma__get_self_tcb ();

	    /* 
	     * If we were requested to run by debugger, checkin.
	     */
	    if (cur_tcb->debug.notify_debugger) {
		cur_tcb->debug.notify_debugger	= cma_c_false;
		cma__debevt_notify ();
		};		

	    /* 
	     * Report the preempting event
	     */
	    if (cur_tcb->debug.did_preempt) {
		cur_tcb->debug.did_preempt = cma_c_false;
		cma__debevt_report (cma__c_debevt_preempting);
		}

	    /* 
	     * Report the running event
	     */
	    cma__debevt_report (cma__c_debevt_running);

	    }
# endif
	}
#else					/* else _CMA_THREAD_IS_VP_ */
    /*
     * FIX-ME: Currently, all of the DEBEVT code is inside the multiplexing
     * conditionals; OK since we don't have debug event support except on
     * VAX/VMS yet anyway, but we need to think through the debugging
     * mechanisms for kernel thread systems!
     */

    /*
     * If we're mapping threads one-to-one, then we need to suspend the VP
     * until we're unblocked.
     */
    cma_t_boolean	status;
    cma__count_vp_blocked ();

    if (save_context) {
	cma__exit_kernel ();		/* Unlock kernel before we sleep! */
	status = cma__vp_suspend (old_tcb->sched.processor->vp_id, milliseconds);

	if (status == cma__c_vp_normal || status == cma__c_vp_err_timeout) {
	    cma__enter_kernel ();
	    return (status == cma__c_vp_normal);
	    }
	else
	    cma__bugcheck ("dispatch: failed to suspend VP");

	}
    else {
	cma__g_vp_count--;
	cma__exit_kernel ();		/* Unlock kernel before we die! */
	cma__trace ((
		cma__c_trc_vp | cma__c_trc_obj,
		"(dispatch) thread %08x (vp %d) about to delete itself",
		old_tcb,
		old_tcb->sched.processor->vp_id->vp));
	cma__vp_delete (old_tcb->sched.processor->vp_id);
	cma__vp_dump ();
	cma__bugcheck ("dispatch: delete failed to terminate execution");
	}

#endif

#if _CMA_VENDOR_ == _CMA__APOLLO
    /*
     * If there's an abort signal pending and we're the default thread, 
     * abort the process now!
     */
    if (cma__g_abort_signal != 0 && cma__g_current_thread == &cma__g_def_tcb) {
	cma__unset_kernel ();
        cma__abort_process (cma__g_abort_signal);
        }
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	See if there are any terminated threads (zombies) which haven't yet
 *	been laid to rest.  If there are, free them.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_zombies queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
extern void
cma__dispell_zombies(void)
    {
    cma__t_int_tcb	*next;
    cma__t_queue	tq;


    cma__queue_init (&tq);

    /*
     * Dispell any TCBs which are on the zombie queue.
     *
     * When a thread terminates, it must free its TCB before actually
     * completing termination.  This is done by placing it on the zombie
     * queue after it has locked the kernel for its final context switch.
     * When the context switch is finished and the kernel is unlocked, all
     * TCBs on the zombie queue are ready to be freed.
     *
     * The queue is protected by the kernel lock, but the TCB can't actually
     * be freed under the kernel lock since the mechanisms rely on mutexes.
     * This constraint results in the somewhat odd construct of exiting and
     * then re-entering the kernel on each iteration of the loop.
     */
    cma__enter_kernel ();

    while (next = (cma__t_int_tcb *)cma__queue_dequeue (&cma___g_zombies)) {
	cma_t_boolean	freed_tcb = cma_c_false;
	cma_t_integer	joiners;


	cma__exit_kernel ();
	cma__int_lock (next->mutex);
	joiners = next->joiners;

	/*
	 * The check for "joiners == 0" is for the odd situation where a
	 * thread has been detached (otherwise it wouldn't be on the zombie
	 * queue), but some thread has done a join using a duplicate handle
	 * (or one thread detached it while another was already joining with
	 * it and hasn't yet continued: perhaps a low priority thread).  The
	 * thread will stay on the zombie queue until the join has completed.
	 */
	if (joiners == 0) {
#if _CMA_THREAD_IS_VP_
	    cma__t_vp		*vpstr = next->sched.processor;
	    cma__t_vp_state	state;


		cma__int_unlock (next->mutex);
	    if (cma__vp_get_state (vpstr->vp_id, &state) == cma__c_vp_normal) {

		if (state.run_state == cma__c_vp_st_stop) {
		    vpstr->vp_id = (cma__t_vpid)cma_c_null_ptr;
#else
		{
		    {
			cma__int_unlock (next->mutex);
#endif
		    cma__trace ((
			    cma__c_trc_obj | cma__c_trc_disp,
			    "(dispell_zombies) thread %08x freeing tcb %08x",
			    cma__get_self_tcb (),
			    next));
		    cma__free_tcb (next);
		    freed_tcb = cma_c_true;
		    }

		}

	    }
	else
	    cma__int_unlock (next->mutex);

	cma__enter_kernel ();

	/*
	 * If we couldn't actually free the TCB yet, then put it on a
	 * temporary queue (which will later be moved to the real zombie
	 * queue).
	 */
	if (!freed_tcb) {
	    cma__queue_insert (&next->header.queue, &tq);
	    }

	}

    /*
     * Now, if there are any TCBs that we couldn't free, they're on the local
     * temporary queue.  Before releasing the kernel lock, we move those
     * entries back to the real zombie queue.
     */
    while (!cma__queue_empty (&tq)) {
	cma__t_queue	*first;


	first = cma__queue_dequeue (&tq);
	cma__queue_insert (first, &cma___g_zombies);
	}

    cma__exit_kernel ();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Return address of asynchronous context block in the current tcb
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	current tcb
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	The address of the asynchronous context block in the current tcb
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
cma__t_async_ctx *
cma__get_async_info(void)
    {
    cma__t_int_tcb *cur_tcb;


    cur_tcb = cma__get_self_tcb ();

    return &cur_tcb->async_ctx;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	initialize the dispatcher 
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__ready_list	    - CMA ready list
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__ready_list	    - CMA ready list
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 * This routine will initialize the default thread and dispatch database.
 * 
 */
void
cma__init_dispatch(void)
    {
    static cma__t_string	null_thread = "null thread";
    cma_t_integer		i;
    cma__t_vp			*def_vp;


    cma__queue_init (&cma___g_zombies);
    cma__sem_init (&cma___g_lastthread);

    cma__trace ((
	    cma__c_trc_init | cma__c_trc_disp,
	    "(init_dispatch) creating default VP"));
    def_vp = cma__alloc_object (cma__t_vp);

    if ((cma_t_address)def_vp == cma_c_null_ptr)
	cma__bugcheck ("init: can't allocate vp struc");

    cma__queue_init (&def_vp->interrupts);
    def_vp->current_thread = &cma__g_def_tcb;
    def_vp->vp_id = cma__vp_get_id ();
    cma__g_vp_count = 1;
    cma__g_vp_running = 1;
    cma__g_current_thread->sched.processor = def_vp;
    cma__g_current_thread->state = cma__c_state_running;

#if (_CMA_OSIMPL_ == _CMA__OS_OSF) && !_CMA_UNIPROCESSOR_
    /*
     * If this is OSF/1 (which supports some per-thread signals), initialize
     * the standard handlers for those signals within the default thread/vp.
     */
    cma__sig_thread_init ();
#endif

#if !_CMA_THREAD_IS_VP_
    cma__trace ((
	    cma__c_trc_init | cma__c_trc_disp,
	    "(init_dispatch) initializing VP queues"));
    cma__queue_init (&cma__g_run_vps);
    cma__queue_init (&cma__g_susp_vps);

    cma__queue_insert (&def_vp->queue, &cma__g_run_vps);

    cma__trace ((
	    cma__c_trc_init | cma__c_trc_disp,
	    "(init_dispatch) initializing ready lists"));

    for (i = 0; i < cma__c_prio_n_tot; i++)
	cma__queue_init (&cma__g_ready_list[i]);

    cma__queue_init (&(cma__g_hold_list));		


    /*
     * Force cma__ready to perform a priority queue calculation 
     */    
    cma__g_current_thread->timer.quanta_remaining = cma__c_quanta;

    cma__g_current_thread->sched.adj_time = -(cma__c_prio_interval + 1);

    cma__g_current_thread->sched.tot_time = 
	cma__g_current_thread->sched.priority *	cma___c_init_cpu_time;
    cma__g_current_thread->sched.time_stamp = 0;
    cma__g_current_thread->sched.cpu_time = cma___c_init_cpu_time;

    cma__g_prio_bg_min = cma__scale_up (cma_c_prio_back_min);	/* Temporary choice of inits */
    cma__g_prio_bg_max = cma__scale_up (cma_c_prio_back_max);
    cma__g_prio_fg_min = cma__scale_up (cma_c_prio_through_min);
    cma__g_prio_fg_max = cma__scale_up (cma_c_prio_through_max);

    cma__g_prio_m_0 = 4 * cma__scale_up (cma__scale_up (cma__c_prio_n_0))
	    / (cma__g_prio_fg_max - cma__g_prio_fg_min);
    cma__g_prio_m_1 = 4 * cma__scale_up (cma__scale_up (cma__c_prio_n_1))
	    / (cma__g_prio_fg_max - cma__g_prio_fg_min);
    cma__g_prio_m_2 = 4 * cma__scale_up (cma__scale_up (cma__c_prio_n_2))
	    / (cma__g_prio_fg_max - cma__g_prio_fg_min);
    cma__g_prio_m_3 = 4 * cma__scale_up (cma__scale_up (cma__c_prio_n_3))
	    / (cma__g_prio_fg_max - cma__g_prio_fg_min);

    cma__g_prio_b_0 = cma__c_prio_n_0 
	    * (0 * cma__g_prio_fg_min + 4 * cma__g_prio_fg_max)
	    / (cma__g_prio_fg_min - cma__g_prio_fg_max) + cma__c_prio_o_0;
    cma__g_prio_b_1 = cma__c_prio_n_1 
	    * (1 * cma__g_prio_fg_min + 3 * cma__g_prio_fg_max)
	    / (cma__g_prio_fg_min - cma__g_prio_fg_max) + cma__c_prio_o_1;
    cma__g_prio_b_2 = cma__c_prio_n_2 
	    * (2 * cma__g_prio_fg_min + 2 * cma__g_prio_fg_max)
	    / (cma__g_prio_fg_min - cma__g_prio_fg_max) + cma__c_prio_o_2;
    cma__g_prio_b_3 = cma__c_prio_n_3 
	    * (3 * cma__g_prio_fg_min + 1 * cma__g_prio_fg_max)
	    / (cma__g_prio_fg_min - cma__g_prio_fg_max) + cma__c_prio_o_3;

    cma__g_prio_p_1 = cma__g_prio_fg_min
	    + 1 * (cma__g_prio_fg_max - cma__g_prio_fg_min)/4;
    cma__g_prio_p_2 = cma__g_prio_fg_min
	    + 2 * (cma__g_prio_fg_max - cma__g_prio_fg_min)/4;
    cma__g_prio_p_3 = cma__g_prio_fg_min
	    + 3 * (cma__g_prio_fg_max - cma__g_prio_fg_min)/4;

    /*
     * Create null thread
     */
    cma__trace ((
	    cma__c_trc_init | cma__c_trc_disp,
	    "(init_dispatch) creating null thread tcb"));
    cma___g_null_tcb		= cma__get_tcb (&cma__g_def_attr);

    /*
     * Change from normal TCB defaults to those of a null thread
     */
    cma___g_null_tcb->kind		= cma__c_thkind_null; 
    cma___g_null_tcb->sched.priority	= cma_c_prio_idle_min;
    cma___g_null_tcb->sched.policy	= cma_c_sched_idle;
    cma___g_null_tcb->sched.rtb		= cma_c_false;	/* Not meaningful */
    cma___g_null_tcb->sched.spp		= cma_c_false;	/* Not meaningful */
    cma___g_null_tcb->sched.fixed_prio	= cma_c_false;	/* Not meaningful */
    cma___g_null_tcb->sched.class	= cma__c_class_idle;
    cma__obj_set_name (cma___g_null_tcb, null_thread);
    cma__obj_set_name (cma___g_null_tcb->mutex, null_thread);
    cma__obj_set_name (cma___g_null_tcb->term_cv, null_thread);

    cma__trace ((
	    cma__c_trc_init | cma__c_trc_disp,
	    "(init_dispatch) null thread tcb is %08x; creating thread...",
	    cma___g_null_tcb));

    if (!cma__int_make_thread (
	    cma___g_null_tcb,
	    (cma_t_thread *)cma_c_null_ptr,
	    (cma_t_start_routine)cma___null_thread,
	    cma_c_null_ptr))
	cma__bugcheck ("init: can't make null thread");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Search for a preemption victim.
 *
 *  FORMAL PARAMETERS:
 *
 *	ptcb	    - Address of the tcb requesting the preempt
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_run_vps	    - Preemptable processors
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Address of victim's VP data structure or null for no victim.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
cma__t_vp *
cma__preempt_victim(
	cma__t_int_tcb	*ptcb)
    {
    cma__t_queue	*vp;
    cma__t_int_tcb	*ttcb, 
			*vtcb = (cma__t_int_tcb *)cma_c_null_ptr;
    cma_t_boolean	victim;


#if !_CMA_THREAD_IS_VP_

/*
 * CHFts08929: because the cma__g_run_vps queue is not
 * being properly maintained at context-switch time, and
 * because hpux does not use the multi-processor
 * capability this queue supposedly provides, we can
 * ignore altogether which thread is sitting on top of
 * the cma__g_run_vps queue.  Instead, when checking for
 * a pre-emption, we can simply set ttcb to the currently
 * running thread.  This way, we are always guaranteed
 * to be checking the trying-to-run thread against the
 * correct (possible) victim.
 */
#if !__hpux
    vp = cma__queue_next (&cma__g_run_vps);
    while (vp != &cma__g_run_vps) {
	ttcb = ((cma__t_vp *)vp)->current_thread;
#else
	ttcb = cma__get_self_tcb();
#endif

	switch (ptcb->sched.class) {
	    case cma__c_class_rt:
		{
		victim = (ttcb->sched.class == cma__c_class_rt
			? (cma__sched_prio_rt(ptcb) > cma__sched_prio_rt(ttcb))
			: cma_c_true);
		break;
		}
	    case cma__c_class_fore:
		{
		switch (ttcb->sched.class) {
		    case cma__c_class_rt:
			{
			victim = cma_c_false;
			break;
			}
		    case cma__c_class_fore:
			{
	        	victim = (cma__sched_prio_fore(ptcb)
				> cma__sched_prio_fore(ttcb));
			break;
			}
		    case cma__c_class_back:
		    case cma__c_class_idle:
			{
			victim = cma_c_true;
			break;
			}
		    default:
			{
			cma__bugcheck ("preempt_victim: bad preempt policy");
			break;
			}
		    }

		break;
	        }
	    case cma__c_class_back:
		{
		switch (ttcb->sched.class) {
		    case cma__c_class_rt:
		    case cma__c_class_fore:
			{
			victim = cma_c_false;
			break;
			}
		    case cma__c_class_back:
			{
	        	victim = (cma__sched_priority(ptcb)
				> cma__sched_priority(ttcb));
			break;
			}
		    case cma__c_class_idle:
			{
			victim = cma_c_true;
			break;
			}
		    default:
			{
			cma__bugcheck ("preempt_victim: bad target policy");
			break;
			}
		    }

		break;
	        }
	    /*
	     * Idle threads should never be the cause of a preemption.
	     */
	    default:
		{
		cma__bugcheck ("preempt_victim: idle thread caused preemption");
		break;
		}
	    }

	if (victim)
	    if (!vtcb)
	        vtcb = ttcb;
	    else
		switch (ttcb->sched.class) {
		    case cma__c_class_rt:
			{
			if (vtcb->sched.class == cma__c_class_rt)
			    if (cma__sched_prio_rt(ttcb) 
				    < cma__sched_prio_rt(vtcb))
			        vtcb = ttcb;

			break;
			}
		    case cma__c_class_fore:
			{
			switch (vtcb->sched.class) {
			    case cma__c_class_rt:
				{
				vtcb = ttcb;
				break;
				}
			    case cma__c_class_fore:
				{
				if (cma__sched_prio_fore(ttcb)
					< cma__sched_prio_fore(vtcb))
				    vtcb = ttcb;
				break;
				}
			    case cma__c_class_back:
			    case cma__c_class_idle:
				{
				break;
				}
			    default:
				{
				cma__bugcheck ("preempt_victim:4");
				break;
				}
			    }

			break;
	        	}
		    case cma__c_class_back:
			{
			switch (vtcb->sched.class) {
			    case cma__c_class_rt:
			    case cma__c_class_fore:
				{
				vtcb = ttcb;
				break;
				}
			    case cma__c_class_back:
				{
		        	if (cma__sched_priority(ttcb)
					< cma__sched_priority(vtcb))
				    vtcb = ttcb;

				break;
				}
			    case cma__c_class_idle:
				{
				break;
				}
			    default:
				{
				cma__bugcheck ("preempt_victim:5");
				break;
				}
			    }

			break;
		        }
		    case cma__c_class_idle:
			{
			vtcb = ttcb;
			break;
		        }
		    default:
			{
			cma__bugcheck ("preempt_victim:6");
			break;
			}
		    }

#if !__hpux
	vp = cma__queue_next (vp);
	}
#endif

    return (vtcb ? vtcb->sched.processor : (cma__t_vp *)cma_c_null_ptr);
#else
    return (cma__t_vp *)cma_c_null_ptr;
#endif
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Place a thread in the appropriate ready-queue
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb	    		    - Address of the tcb
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 * 	This routine will add the thread tcb at the end of the appropriate
 * 	cma__g_ready_list[i].
 */
void
cma__ready(
	cma__t_int_tcb	*tcb,
	cma_t_boolean	preempt)
    {
    cma_t_integer   cur_ticks;		/* Current absolute time in ticks */
    cma_t_integer   p_i,		/* Current priority (scaled) */
		    q_i;		/* Queue to be readied on */


#if !_CMA_THREAD_IS_VP_
    /*
     * State transition:  from "running" to "ready"
     */
    tcb->state = cma__c_state_ready;

    /*
     * If this thread was preempted, it will finish its quanta later.  
     * Otherise, it has finished its quanta, so give it a new one for next 
     * time.
     */
    if (!preempt)
        tcb->timer.quanta_remaining += cma__c_quanta;

    /*
     * Ensure that a thread never "accumulates" quanta.
     */
    if (tcb->timer.quanta_remaining > cma__c_quanta)
        tcb->timer.quanta_remaining = cma__c_quanta;

    cur_ticks = cma__get_time_ticks ();

    cma__sched_update_time (
	    tcb->sched.tot_time, 
	    cur_ticks - tcb->sched.time_stamp);
    tcb->sched.time_stamp = cur_ticks;

    /*
     * If the thread has been put on_hold, "ready" the thread to the Hold List.
     */
    if (tcb->debug.on_hold) {
	cma__queue_insert (
	    &(tcb->header.queue), 
	    &cma__g_hold_list);
	/*
	 * NOTE:  We are returning here, thus, all code that applies to both
	 * threads on_hold and not on_hold *must* be performed ABOVE.
	 */
	return;
	}

    /*
     * Check to see if we should recalculate the proper queue.
     */
    if (cur_ticks - tcb->sched.adj_time > cma__c_prio_interval) {
	/*
	 * Update the "last priority adjustment" timestamp.
	 */
	tcb->sched.adj_time = cur_ticks;

	/*
	 * Find the appropriate queue, based on policy and priority.
	 */
	switch (tcb->sched.class) {
	    case cma__c_class_fore:
		/*
		 * We currently assume that all foreground class threads
		 * have variable priorities.
		 */
		{
		p_i = cma__sched_priority (tcb);

		if (p_i < cma__g_prio_p_2)
		    if (p_i < cma__g_prio_p_1) {
			if (p_i < cma__g_prio_fg_min) 
			    p_i = cma__g_prio_fg_min;
			q_i = cma__scale_dn (cma__scale_dn (cma__g_prio_m_0 
				* p_i)) + cma__g_prio_b_0;
			}
		    else
			q_i = cma__scale_dn (cma__scale_dn (cma__g_prio_m_1 
				* p_i)) + cma__g_prio_b_1;
		else
		    if (p_i < cma__g_prio_p_3)
			q_i = cma__scale_dn (cma__scale_dn (cma__g_prio_m_2
				* p_i)) + cma__g_prio_b_2;
		    else {
			if (p_i > cma__g_prio_fg_max) 
			    p_i = cma__g_prio_fg_max;
			q_i = cma__scale_dn (cma__scale_dn (cma__g_prio_m_3
				* p_i)) + cma__g_prio_b_3;
			}

		break;
		}
	    case cma__c_class_back:
		{
		if (tcb->sched.fixed_prio)
		    q_i = tcb->sched.priority + cma__c_prio_o_al;
		else {
		    p_i = cma__sched_priority (tcb);

		    if (p_i < cma__g_prio_bg_min)  p_i = cma__g_prio_bg_min;
		    if (p_i > cma__g_prio_bg_max)  p_i = cma__g_prio_bg_max;

		    q_i = (cma__c_prio_n_bg * (p_i - cma__g_prio_bg_min))
			    / (cma__g_prio_bg_max - cma__g_prio_bg_min)
			    + cma__c_prio_o_bg;
		    }
		break;
		}
	    case cma__c_class_rt:
		{
		/*
		 * If we implement more than one realtime queue, choose which 
		 * queue here and set q_i appropriately.
		 */
		q_i = cma__c_prio_o_rt;

		break;
		}
	    case cma__c_class_idle:
		{
		q_i = cma__c_prio_o_id;
		break;
		}
	    default:
		{
		cma__bugcheck ("ready:7");
		break;
		}
	    }  /* switch */

	/* 
	 * "Remember" which queue we're going on.
	 */
	tcb->sched.q_num = q_i;
	} /* if */
    else
	/*
	 * No need to update the queue (not enough time has passed to make it
	 * worth the expense of the calculation), just use the same queue as 
	 * last time.
	 */
	q_i = tcb->sched.q_num;

    if (q_i > cma__g_next_ready_queue)	cma__g_next_ready_queue = q_i;

    if (tcb->sched.class != cma__c_class_rt) {
	/*
	 * FIX-ME: The preempted thread should be the next to run...for example
	 *	    if it is a throughput thread, it should be put in the top
	 *	    throughput queue without regard to its calculated priority.
	 */
	if (preempt)
	    cma__queue_insert_after (
		    &(tcb->header.queue), 
		    &cma__g_ready_list[q_i]);
	else
	    cma__queue_insert (
		    &(tcb->header.queue), 
		    &cma__g_ready_list[q_i]);
	}
    else {
	/*
	 * The real-time queue is kept in sorted order.
	 */
	cma__t_queue    *tmp_q;
	cma__t_int_tcb  *tmp_tcb;


	tmp_q = cma__queue_next (&cma__g_ready_list[q_i]);
	tmp_tcb = (cma__t_int_tcb *)tmp_q;

	while ((tmp_q != &cma__g_ready_list[q_i])
		&& (tcb->sched.priority <= tmp_tcb->sched.priority)) {

	    /*
	     * If this is a preempt, quit searching when we get to the
	     * first of equal priority, and then insert before it.  
	     * Otherwise, the loop will exit when we hit one of lower
	     * priority, and we'll insert before it, instead.
	     */
	    if (preempt && tcb->sched.priority == tmp_tcb->sched.priority)
		break;

	    tmp_q = cma__queue_next (tmp_q);
	    tmp_tcb = (cma__t_int_tcb *)tmp_q;
	    }

	cma__queue_insert (&(tcb->header.queue), tmp_q);
	}
#else
    /*
     * If this is a one-to-one kernel thread mapping, then all we need to do
     * here is start the thread's VP running. However, the null thread is
     * never allowed to run except when all normal threads have been blocked,
     * so don't start it.
     */
    if (tcb->kind != cma__c_thkind_null) {
	tcb->state = cma__c_state_running;

	if (cma__vp_resume ((tcb)->sched.processor->vp_id) != cma__c_vp_normal)
	    cma__bugcheck ("ready:8");

	cma__count_vp_running ();
	}
    else
	tcb->state = cma__c_state_ready;	/* Null thread is "ready" */

#endif					/* !_CMA_THREAD_IS_VP_ */
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Performs pre- or post- `fork() reinitialization' work.
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__ready_list	    - CMA ready list
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__ready_list	    - CMA ready list
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 * This routine will initialize the default thread and dispatch database.
 * 
 */
extern void
cma__reinit_dispatch(
	cma_t_integer	flag)		/* Indicate pre (1) or post (0) fork */
    {
    cma_t_integer   i;

    if (flag == cma__c_reinit_postfork_clear) {	/* Post-fork child handling */
	cma__queue_init (&cma___g_zombies);
	cma__g_vp_count = 1;
	cma__g_vp_running = 1;
	cma__g_threadcnt = 1;		/* Default thread */
	cma__sem_init (&cma___g_lastthread);

#if !_CMA_THREAD_IS_VP_
# if 0
	cma__queue_init (&cma__g_run_vps);
	cma__queue_init (&cma__g_susp_vps);
	cma__queue_insert (&def_vp->queue, &cma__g_run_vps);
# endif

	for (i = 0; i < cma__c_prio_n_tot; i++) {
	    cma__queue_init (&cma__g_ready_list[i]);
	    }

	cma__queue_init (&cma__g_hold_list);		
	cma__g_threadcnt++;		/* Count the null thread */
	cma__queue_zero (&(cma___g_null_tcb->header.queue));
	cma__ready (cma___g_null_tcb, cma_c_false);
#endif
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Start the thread 
 *
 *  FORMAL PARAMETERS:
 *
 *	thread_tcb		    - Address of the tcb
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 * 	This routine will add the thread tcb at the end of the
 * 	cma__g_ready_list.
 */
void
cma__start_thread(
	cma__t_int_tcb	*thread_tcb)
    {
    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__start_thread with kernel unlocked");

    /*
     * Force cma__ready to perform a priority queue calculation 
     */    
    thread_tcb->sched.adj_time = -(cma__c_prio_interval + 1);

    thread_tcb->sched.tot_time = 
	    thread_tcb->sched.priority * cma___c_init_cpu_time;
    thread_tcb->sched.time_stamp = cma__get_time_ticks ();
    thread_tcb->sched.cpu_time = cma___c_init_cpu_time;
#if !_CMA_THREAD_IS_VP_
    thread_tcb->sched.processor = (cma__t_vp *)cma_c_null_ptr;
#endif
    thread_tcb->timer.quanta_remaining = 0;	/* cma__ready increments */

    cma__ready (thread_tcb, cma_c_false);
    cma__try_run (thread_tcb);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Terminate a thread 
 *
 *  FORMAL PARAMETERS:
 *
 *	thread_tcb	    - Address of the tcb
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none	(this routine never returns.)
 *
 *  SIDE EFFECTS:
 *
 *	This routine never returns, the thread running it is simply never 
 *	resecheduled.
 */
void
cma__terminate_thread(
	cma__t_int_tcb	*thread_tcb)
    {
    /*
     * Dispell any current undead before we add this thread to the queue.
     * This prevents too many from accumulating, while ensuring that this
     * particular zombie will survive until after context switch.
     */
    cma__dispell_zombies ();

    cma__trace ((
	    cma__c_trc_obj | cma__c_trc_disp,
	    "(terminate_thread) thread %08x terminating, %sdetached",
	    thread_tcb,
	    (thread_tcb->detached ? "" : "not ")));

    cma__enter_kernel ();

    if (thread_tcb->detached) {
	/*
	 * Remove from known threads list
	 */
	(void) cma__queue_remove (&thread_tcb->threads);	
	cma__queue_insert (&thread_tcb->header.queue, &cma___g_zombies);
	}
    else
	/*
	 * The removal from known threads list is deferred to detach
	 */
	thread_tcb->state = cma__c_state_terminated;

    cma__g_threadcnt--;			/* One less thread running */

    /*
     * If this is the default (initial) thread, then don't completely exit
     * until all other threads have exited; then exit the process.
     */
    if (thread_tcb->kind == cma__c_thkind_initial) {

	while (cma__g_threadcnt > cma___c_null_thd_cnt) {
	    TRY {
		cma__sem_wait (&cma___g_lastthread, thread_tcb);
		}
	    CATCH_ALL {		
		}
	    ENDTRY
	    }

	cma__unset_kernel ();		/* Unlock the kernel */

	/*
	 * P1003.4a/D4 doesn't specify the process exit status when all
	 * threads have exited. P1003.4a/D5 specifies that the status is
	 * "unspecified". A value of "0" seems reasonable.
	 */
#if _CMA_OS_ != _CMA__VMS
	exit (0);
#else
	sys$exit (0);
#endif
	}
    else {

	if (cma__g_threadcnt <= cma___c_null_thd_cnt)
	    cma__sem_iwake_one (&cma___g_lastthread);

	}

#ifdef __hpux
    if (_cb.end)
	(*_cb.end)();
#endif

    /*
     * Select next ready thread
     */
    cma__dispatch (
	    thread_tcb,
	    cma_c_false,
	    0);

    cma__bugcheck ("terminate_thread: continued after termination");
    }

#if !_CMA_THREAD_IS_VP_
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Try to find a processor for just readied thread
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb	- Address of the tcb
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 * 	This routine will add the thread tcb at the end of the
 * 	cma__g_ready_list.
 */
void
cma__try_run (
	cma__t_int_tcb	*tcb)
    {
    cma__t_vp	    *vp;		/* Pointer to processor */

    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__try_run with kernel unlocked");

    /*
     * Since we readied a thread, let's see if we can wake up
     * some suspended vp.
     */
# if _CMA_MULTIPLEX_
    if (!cma__queue_empty (&cma__g_susp_vps)) {
	vp = (cma__t_vp *)cma__queue_dequeue (&cma__g_susp_vps);
	cma__queue_insert (&vp->queue, &cma__g_run_vps);

	if (cma__vp_resume (vp->vp_id) != cma__c_vp_normal)
		cma__bugcheck ("try_run:1");

	cma__count_vp_running ();
	}
    else if (cma__vp_create (&vp->vp_id) == cma__c_vp_normal) {
	cma__t_vp_state		state;

	cma__g_vp_count++;
	}
    else
# endif					/* _CMA_MULTIPLEX_ */
    if (tcb->sched.spp)			/* "Strict priority preemption" */
	if (vp = cma__preempt_victim (tcb)) {
	    cma__t_int_tcb *cur_tcb;


	    cur_tcb = cma__get_self_tcb ();

	    if (vp == cur_tcb->sched.processor) {
		cma__undefer ();
		cma__yield_processor ();
		}
	    else
# if _CMA_MULTIPLEX_
		/*
		 * FIX-ME: This won't compile, since the
		 * cma__cause_force_dispatch function needs additional
		 * arguments.  Will need to be redesigned when multiprocessor
		 * multiplexing is implemented.
		 */
		cma__vp_interrupt (vp, cma__cause_force_dispatch, tcb);
# else
		cma__bugcheck ("try_run: spurious vp");
# endif

	    }

    }
#endif					/* _CMA_THREAD_IS_VP_ */

#ifdef __hpux
#if !_CMA_THREAD_IS_VP_
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Determine if given thread would preempt this one.
 *
 *	Copied most of code from cma__try_run, removed
 *	_CMA_MULTIPLEX_ code.  Simply return true/false
 *	rather than call cma__ready and cma__yield.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb	- Address of the tcb
 *
 *  IMPLICIT INPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma__g_ready_list[]	    - CMA ready lists
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 * 	none
 */
cma_t_boolean cma__would_run (
	cma__t_int_tcb	*tcb)
    {
    cma__t_vp	    *vp;		/* Pointer to processor */

    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__would_run with kernel unlocked");

# if _CMA_MULTIPLEX_
    cma__bugcheck( "would_run: not implemented for multiplex" );
# endif					/* _CMA_MULTIPLEX_ */

    if (tcb->sched.spp)			/* "Strict priority preemption" */
	if (vp = cma__preempt_victim (tcb)) {
	    cma__t_int_tcb *cur_tcb;


	    cur_tcb = cma__get_self_tcb ();

	    if (vp == cur_tcb->sched.processor) {
		return cma_c_true;
		}
	    else
		cma__bugcheck ("would_run: spurious vp");

	    }

    return cma_c_false;
    }
#endif					/* _CMA_THREAD_IS_VP_ */
#endif	/* __hpux */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine performs a context switch
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	the current thread tcb, the cma__g_ready_list
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	The current thread's tcb will be appended to the cma__g_ready_list,
 *	the first tcb will be taken from the ready list and its thread
 *	will become the running thread.  
 */
void
cma__yield_processor(void)
    {
    cma__t_int_tcb	*cur_tcb;	/* Current thread's TCB  */


    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__yield_processor with kernel unlocked");

#if !_CMA_THREAD_IS_VP_
    cur_tcb = cma__get_self_tcb ();

#ifndef _HP_CMA_PERF_
    if (cma__ck_io_at_yield)
#endif /* _HP_CMA_PERF_ */
      {
      static struct timeval polling_timeout = {0, 0};
      /*
       * The first two parameters are arbitrary; poll (null timeout).
       */
      (void)cma__io_available (
              cma__c_io_read,
              0,
              &polling_timeout);
      }

    /* 
     * Insert the current thread on the rear of the appropriate ready queue.
     */
    cma__ready (cur_tcb, cma_c_false);

    /*
     * If the debugger changed anything (e.g. priority or hold for one
     * or more threads), sort the ready lists and hold queue to make the 
     * dispatcher database consistent.
     */
    if (cma__g_debug_state.is_inconsistency) {
	cma__g_debug_state.is_inconsistency = cma_c_false;
 	cma___sort_ready_list (); 
	}


    /*
     * Select next ready thread
     */
    cma__dispatch (
	    cur_tcb,
	    cma_c_true,			/* Save current context */
	    0);
#ifndef _HP_CMA_PERF_
    cma__ck_io_at_yield = 1;
#endif /* _HP_CMA_PERF_ */     
#else
    cma__vp_yield ();
#endif					/* _CMA_THREAD_IS_VP_ */
    }

#ifndef _HP_CMA_PERF_
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine performs a context switch
 *	It is called from cma___null_thread() and
 *	from rpc__dg_select_dispatch().
 *	This routine does not call cma__io_available()
 *	as cma___null_thread() has just done so. This
 *	is the only difference from cma__yield_processor().
 *	This is an attempt to save unnecessary calls to
 *	select().
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	the current thread tcb, the cma__g_ready_list
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	The current thread's tcb will be appended to the cma__g_ready_list,
 *	the first tcb will be taken from the ready list and its thread
 *	will become the running thread.  
 */
void
cma__yield_processor_noselect(void)
    {
    cma__t_int_tcb	*cur_tcb;	/* Current thread's TCB  */


    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__yield_processor with kernel unlocked");

#if !_CMA_THREAD_IS_VP_
    cur_tcb = cma__get_self_tcb ();

/*
    {
    static struct timeval polling_timeout = {0, 0};
*/
    /*
     * The first two parameters are arbitrary; poll (null timeout).
     */
/*
    (void)cma__io_available (
            cma__c_io_read,
            0,
            &polling_timeout);
    }
*/

    /* 
     * Insert the current thread on the rear of the appropriate ready queue.
     */
    cma__ready (cur_tcb, cma_c_false);

    /*
     * If the debugger changed anything (e.g. priority or hold for one
     * or more threads), sort the ready lists and hold queue to make the 
     * dispatcher database consistent.
     */
    if (cma__g_debug_state.is_inconsistency) {
	cma__g_debug_state.is_inconsistency = cma_c_false;
 	cma___sort_ready_list (); 
	}


    /*
     * Select next ready thread
     */
    cma__dispatch (
	    cur_tcb,
	    cma_c_true,			/* Save current context */
	    0);
#else
    cma__vp_yield ();
#endif					/* _CMA_THREAD_IS_VP_ */
    }
#endif /* _HP_CMA_PERF_ */     
	
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Null thread function (declared to match cma_t_start_routine)
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
#if !_CMA_THREAD_IS_VP_
cma_t_address
cma___null_thread(void)
    {
    while (cma_c_true) {
	TRY {
# if _CMA_OS_ == _CMA__VMS
	    while (cma_c_true) {
		extern sys$hiber (void);
		cma__trace ((cma__c_trc_null, "(null_thread) hibernating"));
		sys$hiber ();
		cma__trace ((cma__c_trc_null, "(null_thread) yielding"));
		cma_yield ();		/* See if other threads need to run */
		}
# endif
# if _CMA_OS_ == _CMA__UNIX
	    cma__enter_kernel ();

	    while (cma_c_true) {
		cma_t_date_time		delta;
		struct timeval		wait_time, *wait_ptr;


		/*
		 * Check the timer queue. Ready any threads whose entries have
		 * expired.  Determine if the queue is now empty, and get the
		 * delta time to the next expiration, if it is not.
		 *
		 * If the timer queue is now empty, set the wait pointer to 
		 * null indicating an infinite timeout.
		 */
		if (!cma__check_timer_queue (&delta)) {
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
		    /*
		     * select() on the RS/6000 returns EINVAL when the
		     * timeout is NULL and no file descriptors are selected,
		     * as often happens with sigwait(). So we set up a wait
		     * time of approx. 68 years to simulate the infinite wait
		     * implied by wait_ptr = NULL
		     */
		    wait_time.tv_sec = 0x7FFFFFFF;
		    wait_time.tv_usec = 0;
		    wait_ptr = &wait_time;
#else		    
		    wait_ptr = (struct timeval *)cma_c_null_ptr;
#endif
		    }
		else {
		    wait_time.tv_sec = delta.tv_sec;
		    wait_time.tv_usec = delta.tv_usec;
		    wait_ptr = &wait_time;
		    }

		/* 
		 * Block at a select(2) call until a timer queue entry expires.
		 * (The first two parameters are arbitrary.)
		 */
		(void)cma__io_available (cma__c_io_read, 0, wait_ptr);
#ifndef _HP_CMA_PERF_
                cma__ck_io_at_yield = 0;
#endif /* _HP_CMA_PERF_ */

		/*
		 * Since there is now a file ready for I/O, or a timer queue
		 * entry ready to expire, yield the processor to let things
		 * happen.
		 */
		cma__undefer ();
#ifndef _HP_CMA_PERF_
		cma__yield_processor_noselect ();
#else
		cma__yield_processor ();
#endif /* _HP_CMA_PERF_ */
	        }

#  if 0
	    /*
	     * The following statement ought to be here, but needn't be, as
	     * it's never executed due to the infinite loop. Its ghost is
	     * left here to avoid confusing anyone...
	     */
	    cma__exit_kernel ();
#  endif
# endif
	    }
	CATCH_ALL {			/* Catch and drop all exceptions */
# if _CMA_OS_ == _CMA__UNIX
	    cma__exit_kernel ();
# endif
# ifndef NDEBUG
	    (void) cma__assert_warn (0, "Exception in null thread.");
	    exc_report (THIS_CATCH);
# endif
	    }
	ENDTRY
	}
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___sort_ready_list is used to remove all inconsistencies
 *	in the dispatcher database.  Inconsistencies can arrive from
 *	two sources: the debugger has changed a thread's priority or
 *	even policy, or the scheduler has modified priorities without
 *	moving the thread.
 *
 *	While scanning the queues, this function also looks for a "next thread"
 *	that the debugger may have requested.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 * 
 */
void
cma___sort_ready_list(void)
    {
    cma_t_integer	i;
    cma__t_queue	temp_ready_list[cma__c_prio_n_tot];
    cma__t_queue	temp_hold_list;
    cma__t_queue	temp_next_thread;
    cma__t_int_tcb	*tcb;

    cma__assert_fail(
	    cma__tac_isset (&cma__g_kernel_critical),
	    "In cma__sort_ready_list with kernel unlocked");

    /*
     * Move the ready and hold lists to temp-ready, temp-hold
     */
    for (i = 0; i < cma__c_prio_n_tot; i++) {
	cma__queue_zero (&temp_ready_list[i]);
	cma__queue_insert_after (&temp_ready_list[i],&cma__g_ready_list[i]);
	(void) cma__queue_remove (&cma__g_ready_list[i]);
	cma__queue_init (&cma__g_ready_list[i]);
	}

    cma__queue_zero (&temp_hold_list);
    cma__queue_insert_after (&temp_hold_list,&cma__g_hold_list);
    cma__queue_remove (&cma__g_hold_list);
    cma__queue_init (&cma__g_hold_list);

    /*
     * Init the queue to hold the next_thread
     */
    cma__queue_init (&temp_next_thread);

    /*
     * Scan the temp hold list: ready thread unless debugger wants it run
     */
    while (!cma__queue_empty (&temp_hold_list)) {
	tcb = (cma__t_int_tcb *)cma__queue_dequeue (&temp_hold_list);

	if (tcb == cma__g_debug_state.next_to_run) {
	    cma__g_debug_state.next_to_run = (cma__t_int_tcb *)cma_c_null_ptr;
	    cma__queue_insert(&(tcb->header.queue), &temp_next_thread);
	    }
	else
	    cma__ready (tcb, cma_c_false);

	}

    /*
     * Scan the ready hold list: ready thread unless debugger wants it run
     */
    for (i = 0; i < cma__c_prio_n_tot; i++) {
	while (!cma__queue_empty (&temp_ready_list[i])) {
	    tcb = (cma__t_int_tcb *)cma__queue_dequeue (&temp_ready_list[i]);
	    if (tcb == cma__g_debug_state.next_to_run) {
		cma__g_debug_state.next_to_run =
		    (cma__t_int_tcb *)cma_c_null_ptr;
		cma__queue_insert(&(tcb->header.queue), &temp_next_thread);
		}
	    else
		cma__ready (tcb, cma_c_false);
	    }
	}
	
    /*
     * Finally, insert the debugger-requested thread at the very top of ready.
     */
    if (!cma__queue_empty (&temp_next_thread)) {
	tcb = (cma__t_int_tcb *)cma__queue_dequeue (&temp_next_thread);
	cma__g_next_ready_queue	= cma__c_prio_o_rt;
	cma__g_debug_state.events_enabled   = cma_c_true;
	tcb->debug.notify_debugger = cma_c_true;
	cma__queue_insert(
	    &(tcb->header.queue),
	    &cma__g_ready_list[cma__g_next_ready_queue]);
	}

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DISPATCH.C */
/*   52    4-AUG-1992 11:04:41 BUTENHOF "Clear vpid when VP is deleted" */
/*  *51   19-MAY-1992 14:20:46 SCALES "Cleanse null threads' queue pointers during re-init" */
/*  *50   18-MAY-1992 16:48:39 SCALES "Cleanse queue links in sort-ready-queues" */
/*  *49   20-APR-1992 10:07:29 BUTENHOF "Modify bugcheck for terminated" */
/*  *48   15-APR-1992 14:43:20 BUTENHOF "Bugcheck on dispatch to terminated thread" */
/*  *47    3-APR-1992 18:33:51 SCALES "Rework async context switch for U*ix" */
/*  *46   31-MAR-1992 13:30:32 BUTENHOF "Alpha OSF" */
/*  *45   19-MAR-1992 13:17:07 BUTENHOF "Add undefer() calls before yield_processor()" */
/*  *44   18-MAR-1992 19:01:04 SCALES "Parameterize scheduling policies" */
/*  *43   13-MAR-1992 11:09:02 BUTENHOF "Clean up reinit" */
/*  *42   10-MAR-1992 16:25:20 BUTENHOF "Change timeb to timeval" */
/*  *41   26-FEB-1992 19:14:56 SCALES "errno reorganization" */
/*   37A1 26-FEB-1992 13:43:50 KEANE "Fixed shared lib prob in BL9" */
/*  *40   19-FEB-1992 13:48:53 SCALES "Perform undeferal on enter-kernel" */
/*  *39   18-FEB-1992 15:28:41 BUTENHOF "Call vp_dump in case of terminate bugcheck" */
/*  *38   10-FEB-1992 11:29:25 SCALES "Remove unneeded initializations" */
/*  *37   30-JAN-1992 22:12:24 SCALES "Implement our own $CLRAST" */
/*  *36   30-JAN-1992 11:55:51 BUTENHOF "Get rid of trace_kernel" */
/*  *35   29-JAN-1992 23:47:24 SCALES "Rework async context switch on VMS" */
/*  *34   24-JAN-1992 10:02:01 BUTENHOF "Fix type mismatch" */
/*  *33   23-JAN-1992 14:23:23 BUTENHOF "Integrate DEC OSF/1 shared library support" */
/*   22A1 20-JAN-1992 17:15:19 SCALES "Integrate vp_yield for Tin" */
/*  *32    9-JAN-1992 14:25:49 BUTENHOF "Support Alpha in cause_force_dispatch" */
/*  *31    6-DEC-1991 07:19:26 BUTENHOF "Sort realtime threads on blocked queues" */
/*  *30   27-NOV-1991 09:24:41 BUTENHOF "Change errno access" */
/*  *29   26-NOV-1991 16:15:10 BUTENHOF "Get rid of exit() call on VMS" */
/*  *28   22-NOV-1991 11:55:45 BUTENHOF "Add casts" */
/*  *27   18-NOV-1991 11:04:00 CURTIN "Added include for stdlib and an Alpha switch" */
/*  *26   14-OCT-1991 13:38:27 BUTENHOF "Refine/fix use of config symbols" */
/*  *25   24-SEP-1991 16:26:58 BUTENHOF "Apollo-specific quick kill" */
/*  *24   17-SEP-1991 13:23:23 BUTENHOF "Implement ""yield"" for Mach kernel threads" */
/*  *23    2-JUL-1991 16:52:36 BUTENHOF "Cache VPs with thread" */
/*  *22   21-JUN-1991 11:59:18 BUTENHOF "Fix some type mismatch errors" */
/*  *21   11-JUN-1991 17:16:45 BUTENHOF "Clean out some old cma__traces" */
/*  *20   10-JUN-1991 18:21:29 SCALES "Add sccs headers for Ultrix" */
/*  *19   10-JUN-1991 17:54:04 SCALES "Conditionalize inclusion of I/O stuff" */
/*  *18    5-JUN-1991 18:37:42 BUTENHOF "Add cast to free_mem() call" */
/*  *17    5-JUN-1991 17:31:24 BUTENHOF "Clean up some disabled code" */
/*  *16    3-JUN-1991 17:12:57 BUTENHOF "More debugging for an error condition..." */
/*  *15    2-JUN-1991 19:36:24 BUTENHOF "Remove null thread and timer code for VPs" */
/*  *14   31-MAY-1991 14:12:26 CURTIN "Added a new reinit routine" */
/*  *13   29-MAY-1991 17:14:28 BUTENHOF "Minimize null thread kernel locking (MP)" */
/*  *12   14-MAY-1991 13:43:17 BUTENHOF "Continue OSF/1 debugging" */
/*  *11    3-MAY-1991 11:25:47 BUTENHOF "Continue OSF/1 debugging" */
/*  *10   24-APR-1991 11:11:08 BUTENHOF "Continue OSF/1 debugging" */
/*  *9    15-APR-1991 15:54:24 CURTIN "added an include" */
/*  *8    12-APR-1991 23:35:30 BUTENHOF "Implement VP layer to support Mach threads" */
/*  *7     1-APR-1991 18:08:19 BUTENHOF "Integrate exception changes" */
/*  *6    28-MAR-1991 17:22:21 SCALES "Improve variable priority dispatch" */
/*  *5    14-MAR-1991 13:45:36 SCALES "Convert to stream format for ULTRIX build" */
/*  *4    12-MAR-1991 19:22:50 SCALES "Merge Apollo changes to CD4" */
/*  *3     6-FEB-1991 18:57:07 BUTENHOF "Improve response of signal_int operation" */
/*  *2     6-FEB-1991 01:32:51 BUTENHOF "Change errno field name" */
/*  *1    12-DEC-1990 21:44:53 BUTENHOF "Thread dispatcher" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DISPATCH.C */
