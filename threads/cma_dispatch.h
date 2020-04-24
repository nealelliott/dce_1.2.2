/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_dispatch.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:36:24 $
 */
/*
 * HISTORY
 * $Log: cma_dispatch.h,v $
 * Revision 1.1.8.1  1996/05/10  13:36:24  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:41 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:35  truitt  /main/HPDCE01/1]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:40:44  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:37:18  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:11:05  prepetit]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  12:31:30  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:26  root  1.1.9.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:12:09  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:10:33  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:33:12  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:40  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:26:49  osfrcs  1.1.1.8]
 *
 * 	Add declaration for cma__g_in_dispatch.
 * 	Add prototype for cma__would_run.
 * 	[1993/01/06  17:46:27  hopkins]
 *
 * Revision 1.1.4.3  1994/06/09  13:37:56  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:53  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/25  19:57:47  ohara
 * 	code cleanup
 * 	[1994/04/20  20:41:31  ohara]
 * 
 * Revision 1.1.4.1  1994/04/01  20:16:14  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:22  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:28:00  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:38:19  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:14:58  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  21:09:45  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:37:03  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:03:06  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:16:47  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:16:30  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(ULTRIX/OSF)	%G%
 */
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 * This software is furnished under a licence and may be used and
 * copied only in accordance of the terms of such license and
 * with the inclusion of the above copyright notice. This
 * software or any other copies thereof may be provided or
 * otherwise made available to any other person. No title to and
 * ownership of the software is herby transferred.
 *
 * The information in this software is subject to change without
 * notice and should not be construed as a commitment by DIGITAL
 * Equipment Corporation.
 *
 * DIGITAL assumes no responability for the use or reliability
 * of its software on equipment which is not supplied by DIGITAL.
 */

/*
 * FACILITY:
 *
 *	CMA services
 *
 * ABSTRACT:
 *
 *	This module defines the dispaching of the threads.
 *
 * AUTHORS:
 *
 *	Hans Oser
 *
 * CREATION DATE:
 *
 *	23 August 1989
 *
 * MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof & Webb Scales	27 September 1989
 *		Fix spelling error (deferral instead of deferal)
 *	002	Hans Oser			03 October 1989
 *		Function cma__get_asynch_info added, cma__dispatch
 *		has now a parameter specifing the mode.
 *	003	Hans Oser and Webb Scales	03 October 1989
 *		Add ctx_buffer parameter to cma__get_async_info
 *	004	Hans Oser			10 October 1989
 *		Move Timer support to cma_timer; make
 *		cma__get_current_tcb available to others.
 *	005	Hans Oser			13 October 1989
 *		changements due to program review.
 *	006	Webb Scales			17 October 1989
 *		Corrected case of #include filenames
 *	007	Dave Butenhof & Webb Scales	19 October 1989
 *		Remove extern defs for test_and_set and unset (which have
 *		moved to cma_host_*.h).
 *	008	Webb Scales			20 October 1989
 *		Placed definition (ie, type and initializer) of 
 *		cma__g_kernel_critical in cma_dispatch.c and placed 
 *		the declaration (ie, "extern") in this file.
 *	009	Dave Butenhof			Halloween 1989
 *		Define queue of known threads.
 *	010	Dave Butenhof			All Saints Day 1989
 *		Convert cma__enter_kernel to macro.
 *	011	Dave Butenhof/Webb Scales/Bob Conti	3 November 1989
 *		Change exit_kernel macro to call undefer before releasing
 *		lock.
 *	012	All				3 November 1989
 *		Redesign deferral.
 *	013	Webb Scales			3 November 1989
 *		Add assertion to exit_kernel
 *	014	Bob Conti			5 November 1989
 *		Delete time argument from cma__assign
 *		Add flag defers_processed to avoid useless calls to undefer.
 *		Change cma__undefer to return an integer so it can be
 *		used in conditionals in a macro where a value is expected.
 *	015	Webb Scales & Bob Conti		6 November 1989
 *		Made cma__ready into a macro and put the quatum increment into
 *		it.
 *	016	Webb Scales	7 November 1989
 *		Changed name of cma__AST_force_dispatch to 
 *		cma__cause_force_dispatch.
 *	017	Webb Scales	18 November 1989
 *		Changed interface to cma__get_async_info
 *	018	Webb Scales	19 November 1989
 *		Corrected proto for cma__cause_force_dispatch
 *	019	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	020	Webb Scales	10 December 1989
 *		Add deferal action for Unix non-blocking I/O.
 *	021	Dave Butenhof	2 March 1990
 *		Integrate Kevin Ackley's changes for Apollo M68000 port.
 *	022	Dave Butenhof	9 April 1990
 *		Remove definition of known thread queue header; it's now
 *		in cma_debugger.
 *	023	Webb Scales	15 June 1990
 *		Added priority scheduling:
 *		- Changed cma__ready from a macro to a routine.
 *		- Made the ready list an array of queue headers.
 *		- Added global "next ready queue" pointer.
 *		- Added defer's for tick updates.
 *	024	Webb Scales	25 July 1990
 *		Moved defer functions to their own module.
 *	025	Dave Butenhof	14 August 1990
 *		Generalize cma__cause_force_dispatch so it can be used for
 *		asynchronous alerts as well as timeslicing.
 *	026	Webb Scales	15 August 1990
 *		Cleaned up some, accomodating HPPA & Sun-68K platforms
 *	027	Dave Butenhof	04 April 1991
 *		Add convenient macros for kernel threads.
 *	028	Paul Curtin	31 May 1991
 *		Added a prototype for a reinit routine
 *	029	Dave Butenhof	31 May 1991
 *		Add timeout argument to cma__block and cma__dispatch (only
 *		used for _CMA_THREAD_IS_VP_).
 *	030	Dave Butenhof	07 October 1991
 *		Add cma__g_threadcnt.
 *	031	Paul Curtin	18 November 1991
 *		Added the use of an Alpha switch.
 *	032	Webb Scales	13 March 1992
 *		Moved VP definitions to cma_vp_defs.h
 */


#ifndef CMA_DISPATCH
#define CMA_DISPATCH

/*
 * INCLUDE FILES
 */

#include <cma_tcb_defs.h>
#include <cma.h>
#include <cma_kernel.h>
#include <cma_timer.h>
#include <cma_vp_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

/*
 * TYPEDEFS
 */

/*
 * GLOBAL DATA
 */

extern cma__t_queue	cma__g_ready_list[cma__c_prio_n_tot];
extern cma_t_integer	cma__g_next_ready_queue; /* Next queue to check */
extern cma_t_integer	cma__g_threadcnt;	/* Number of threads */

#ifdef __hpux
extern cma_t_integer	cma__g_in_dispatch;	/* Don't let cma__dispatch
						   get called recursively */
#endif

/*
 * INTERNAL INTERFACES
 */

/*
 * The current thread is blocked on this list and the topmost thread of
 * the ready list is resumed.
 */
extern cma_t_boolean cma__block  (cma__t_queue *,cma_t_integer);

/*
 * start a forced dispatch from a signal or AST routine
 */
extern void
cma__cause_force_dispatch
#if (_CMA_OS_ == _CMA__UNIX) && (_CMA_HARDWARE_ != _CMA__VAX) && (_CMA_HARDWARE_ != _CMA__ALPHA)
	(cma__t_int_tcb *,cma_t_address,cma__t_sigcontext *);
#endif
#if (_CMA_HARDWARE_ == _CMA__VAX) || (_CMA_HARDWARE_ == _CMA__ALPHA)
	(cma__t_int_tcb *,cma_t_address ,cma_t_address *,cma_t_address *);
#endif

/*
 * The next ready thread will be made running
 *
 * (save_context is set to cma_c_false for unscheduling dead threads.)
 */
extern cma_t_boolean cma__dispatch  (cma__t_int_tcb	*,cma_t_boolean ,cma_t_integer );

/*
 * Dispell zombie threads (threads which have terminated but have not yet
 * been freed).
 */
extern void cma__dispell_zombies  (void);

/*
 * Get information saved during a timeslice AST or signal
 */
extern cma__t_async_ctx * cma__get_async_info  (void);

/*
 * Initialize the dispatcher: the initial executing process becomes a thread.
 */
extern void cma__init_dispatch  (void);

/*
 * Search for a victim for preemption
 */
extern cma__t_vp * cma__preempt_victim  (cma__t_int_tcb *);

/*
 * Ready the thread: added it to the tail of the appropriate ready list.
 */
extern void cma__ready  (cma__t_int_tcb *,cma_t_boolean);

/*
 * Perform pre and/or post fork() reinitialization work.
 */
extern void cma__reinit_dispatch  (cma_t_integer);

/*
 * Start the thread: the thread is added at the tail of the ready list.
 * The tcb is initialized.
 */
extern void cma__start_thread  (cma__t_int_tcb *);

/*
 * Set thread to idle status (= non dispatchable)
 */
extern void cma__terminate_thread  (cma__t_int_tcb *);

/*
 * Try to run a just-readied thread
 */
extern void cma__try_run  (cma__t_int_tcb *);

#ifdef __hpux
/*
 * Determine if a thread would
 * preempt the current thread.
 */
extern cma_t_boolean cma__would_run (cma__t_int_tcb *);
#endif

/*
 * The current thread is put at the tail of the ready list and the topmost
 * thread of the ready list is resumed. 
 */ 
extern void cma__yield_processor(void);

#ifndef _HP_CMA_PERF_
extern void cma__yield_processor_noselect (void);
#endif /* _HP_CMA_PERF_ */

#ifdef __cplusplus
}
#endif

#endif

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DISPATCH.H */
/*  *13   31-MAR-1992 13:30:55 BUTENHOF "Alpha OSF" */
/*  *12   18-MAR-1992 19:01:22 SCALES "Move VP stuff to cma_vp_defs.h" */
/*  *11   18-NOV-1991 11:00:19 CURTIN "Added the use of an Alpha switch" */
/*  *10   14-OCT-1991 13:38:39 BUTENHOF "Add thread count, clean up uniproc/OSF conds" */
/*  *9    10-JUN-1991 19:52:54 SCALES "Convert to stream format for ULTRIX build" */
/*  *8    10-JUN-1991 19:20:38 BUTENHOF "Fix the sccs headers" */
/*  *7    10-JUN-1991 18:21:36 SCALES "Add sccs headers for Ultrix" */
/*  *6     2-JUN-1991 19:36:34 BUTENHOF "Add arguments" */
/*  *5    31-MAY-1991 14:12:12 CURTIN "Added a proto for a new reinit routine" */
/*  *4    14-MAY-1991 13:38:53 BUTENHOF "Fix try_run declaration" */
/*  *3     3-MAY-1991 11:25:57 BUTENHOF "Fix try_run declaration" */
/*  *2    12-APR-1991 23:35:39 BUTENHOF "Implement VP layer to support Mach threads" */
/*  *1    12-DEC-1990 21:44:57 BUTENHOF "Thread dispatcher" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DISPATCH.H */
