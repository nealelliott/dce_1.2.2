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
 * @(#)Module: cma_timer.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:43:35 $
 */
/*
 * HISTORY
 * $Log: cma_timer.h,v $
 * Revision 1.1.8.1  1996/05/10  13:43:35  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:46 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:45 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:45  devsrc]
 * 	 *
 *
 * 	code cleanup
 * 	[1994/04/20  20:42:16  ohara]
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:15  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:53  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  17:48:35  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:45  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:14:44  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  19:07:45  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:35:48  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:25  alan]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:56:27  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:25  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:08:47  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:36:07  mgm  1.1.1.11]
 *
 * Revision 1.1.4.3  1994/06/09  13:39:01  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:45  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/25  19:57:51  ohara
 * 	code cleanup
 * 	[1994/04/20  20:42:16  ohara]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:38  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:15  jd]
 * 
 * Revision 1.1.2.4  1992/12/15  22:32:13  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:25  alan]
 * 
 * Revision 1.1.2.3  1992/09/29  20:16:16  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:56:27  sekhar]
 * 
 * Revision 1.1.2.2  1992/09/03  14:41:47  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:25  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:09:00  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:08:47  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	%W%	(DEC OSF/1)	%G%
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
 *	Header file for timer functions
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	5 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Hans Oser		    10 October 1989
 *		cma__wait, cma__delay, cma__timed_wait added;
 *		All timer support moved from cma_dispatch into
 *		this module.
 *	002	Hans Oser		    13 October 1989
 *		Changements due to program review
 *	003	Hans Oser		    20 October 1989
 *		Changements for ULTRIX non_blocking I/O
 *	004	Hans Oser		    27 October 1989
 *		cma__g_timer_lock made global
 *	005	Bob Conti		5 November 1989
 *		Renamed time_slice constant to quanta, and
 *		time_slice field to quanta_remaining.
 *		Rename "actual" to "current" everywhere.
 *	006	Dave Butenhof		8 November 1989
 *		Major changes to use system time and simplify algorithms.
 *	007	Dave Butenhof	4 December 1989
 *		Include cma_tcb_defs.h instead of cma_tcb.h
 *	008	Dave Butenhof	5 March 1990
 *		Integrate Kevin Ackley's changes for Apollo M68000 port.
 *	009	Dave Butenhof & Webb Scales	29 March 1990
 *		Add cell for next-timer-queue entry time (for null thread
 *		select).  Make timeslice signal value "global".
 *	010	Webb Scales	4 June 1990
 *		Make timeslice routine available to other modules (cma_signal).
 *	011	Webb Scales	15 June 1990
 *		Added priority scheduling:  added a global ticks counter for
 *		absolute time, and a macro for fetching the value.
 *	012	Webb Scales	16 August 1990
 *		Added include of signal.h for timeslicer declaration on Unix
 *	013	Dave Butenhof & Webb Scales	 4 December 1990
 *		Added proto for cma__check_timer_queue
 *	014	Paul Curtin	31 May 1991
 *		Added a proto for fork() reinitialization routine.
 *	015	Dave Butenhof	18 September 1991
 *		Integrate Apollo CMA5 reverse drop: remove Apollo special
 *		cases for quantum and timer interval.
 *	016	Dave Butenhof	22 November 1991
 *		Remove include of string.h, which isn't needed.
 *	017	Webb Scales	28 January 1992
 *		Update the prototype for cma-periodic on VMS.
 *	018	Webb Scales	30 January 1992
 *		Un-do changes to the cma-periodic prototype.
 *	019	Webb Scales	11 February 1992
 *		Re-do the prototype for cma-periodic on VMS.
 *	020	Dave Butenhof	10 March 1992
 *		Change timeb field references to timeval, since
 *		cma_t_date_time has been changed to avoid requiring libbsd.a
 *		on AIX and OSF/1.
 */

#ifndef CMA_TIMER
#define CMA_TIMER

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#if _CMA_OS_ == _CMA__UNIX
# include <signal.h>
#endif
#include <cma_tcb_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__c_semaphore_timer	1
#define cma__c_normal_timer	2
#define cma__c_select_timer     3

#if _CMA_OS_ == _CMA__UNIX
#define cma__c_timer		    ITIMER_VIRTUAL
#define cma__c_timer_signal	    SIGVTALRM
#endif

#define cma__c_quanta		2	/* default quanta for time slicing */

#define cma__get_time_ticks()	cma__g_time_ticks

/*
 * Macro to copy one system time value to another.
 */
#if _CMA_OS_ == _CMA__VMS
#define cma__copy_time(output,input)					\
    ((output)->high = (input)->high, (output)->low = (input)->low)

#define cma__time_lss(t1,t2)						\
    (((t1)->high < (t2)->high) || (((t1)->high == (t2)->high)		\
	&& ((t1)->low < (t2)->low)))
#else
#define cma__copy_time(output,input)					\
    ((output)->tv_sec = (input)->tv_sec, (output)->tv_usec = (input)->tv_usec)

#define cma__time_lss(t1,t2)						\
    (((t1)->tv_sec < (t2)->tv_sec) || (((t1)->tv_sec == (t2)->tv_sec)	\
	&& ((t1)->tv_usec < (t2)->tv_usec)))
#endif

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

extern cma_t_date_time	cma__g_one_tick;
extern cma_t_date_time	cma__g_next_tqe;
extern cma_t_integer	cma__g_time_ticks;


/*
 * EXTERNAL INTERFACES
 */

/*
 * Add times
 */
extern void cma__add_time  (cma_t_date_time	*,cma_t_date_time *,cma_t_date_time *);

#if !_CMA_THREAD_IS_VP_
/*
 * ready threads on timer queue
 */
extern cma_t_boolean cma__check_timer_queue  (cma_t_date_time *);
#endif

/*
 * Get the current time
 */
extern void cma__get_time  (cma_t_date_time *);

/*
 * Start the timer: this is used for thread timeslicing, timed semaphore
 * waits, and non-blocking I/O.
 */
extern void cma__init_timer  (void);

#if !_CMA_THREAD_IS_VP_
/*
 * Insert an entry (in order) on the timer queue.  If the specified time
 * preceeds the current system time, then the entry is not inserted, and the
 * value cma_c_false is returned.
 */
extern cma_t_boolean cma__insert_timer  (cma__t_int_tcb *,cma_t_date_time *);
#endif

/*
 * Convert a floating point interval (cma_t_interval) to a system time
 * (cma_t_date_time).
 */
extern void cma__interval_to_time  (cma_t_interval,cma_t_date_time *);

#if !_CMA_THREAD_IS_VP_
/*
 * Timeslice routine
 */
extern void
cma__periodic
# if _CMA_OS_ == _CMA__VMS      
(void);
# endif
# if _CMA_OS_ == _CMA__UNIX
(int,cma__t_siginfo,cma__t_sigcontext *);
# endif
#endif

/*
 * Perform pre/post fork() reinitialization work
 */
extern void cma__reinit_timer  (cma_t_integer);

/*
 * Subtract times.
 */
extern void cma__subtract_time  (cma_t_date_time *,cma_t_date_time *,cma_t_date_time *);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TIMER.H */
/*  *13   10-MAR-1992 16:27:49 BUTENHOF "Change timeb to timeval" */
/*  *12   11-FEB-1992 16:30:31 SCALES "Change periodic's proto" */
/*  *11   30-JAN-1992 22:13:29 SCALES "Repair async alerts for VAX/VMS" */
/*  *10   29-JAN-1992 23:49:11 SCALES "Rework end-quantum preemption for VMS" */
/*  *9    22-NOV-1991 13:32:55 BUTENHOF "Remove include of string.h" */
/*  *8    24-SEP-1991 16:29:43 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *7    10-JUN-1991 19:57:37 SCALES "Convert to stream format for ULTRIX build" */
/*  *6    10-JUN-1991 19:22:14 BUTENHOF "Fix the sccs headers" */
/*  *5    10-JUN-1991 18:24:55 SCALES "Add sccs headers for Ultrix" */
/*  *4     2-JUN-1991 19:37:08 BUTENHOF "Stop using timers for _CMA_THREAD_IS_VP_" */
/*  *3    31-MAY-1991 16:44:43 CURTIN "Added a fork() reinitialization routine proto" */
/*  *2     2-MAY-1991 13:59:34 BUTENHOF "Export timeslice interval" */
/*  *1    12-DEC-1990 21:55:08 BUTENHOF "Timer services" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TIMER.H */
