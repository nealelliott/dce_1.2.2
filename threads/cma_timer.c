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
 * HISTORY
 * $Log: cma_timer.c,v $
 * Revision 1.1.13.1  1996/05/10  13:43:23  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:46 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:45 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:44  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:11  jd]
 *
 * 	Ajay's performance enhancements
 * 	[1994/05/03  18:22 UTC  jss  /main/HPDCE01/2]
 *
 * 	Optimize calls to gettimeofday() from cma_check_timer_queue()
 * 	[1994/04/29  19:09 UTC  ajayb  /main/HPDCE01/ajayb_threads_perf/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:52  truitt  /main/HPDCE01/1]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:51:38  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:38:35  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/10/18  20:15:13  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:08:17  gaz]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:18  alan]
 *
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:55:55  sekhar]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:20  bolinger]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:44  root  1.1.3.3]
 *
 * Revision 1.1.8.2  1994/06/09  13:38:59  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:44  devsrc]
 * 
 * Revision 1.1.8.1  1994/04/01  20:17:35  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:11  jd]
 * 
 * Revision 1.1.4.4  1992/12/15  22:32:08  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:43:18  alan]
 * 
 * Revision 1.1.4.3  1992/09/29  20:16:11  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:55:55  sekhar]
 * 
 * Revision 1.1.4.2  1992/09/03  14:41:42  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:20  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:35  bolinger
 * 	Correct uses of tv_usec member of struct timeval to reflect
 * 	fact that it's a signed long.
 * 	[1992/08/11  21:18:58  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:08:17  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:08:01  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:41:09  devrcs
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
 *	timer services for the scheduler
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	8 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Hans Oser	10 October 1989
 *		cma__tick_wait, cma__tick_delay, cma__tick_wait_insert added;
 *		All timer support moved from cma_dispatch into this module.
 *	002	Hans Oser	11 October 1989
 *		Timer initialisation completed
 *	003	Hans Oser	13 October 1989
 *		Changements due to program review
 *	004	Dave Butenhof	19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	005	Webb Scales	19 October 1989
 *		Added #include <cma_host.h> for cma__test_and_set/cma__unset
 *	006	Hans Oser	20 October 1989
 *		Additions for ULTRIX nonblocking I/O
 *	007	Webb Scales	20 October 1989
 *		Made it acceptable to MIPS/Ultrix C
 *	008	Hans Oser	26 October 1989
 *		cma__timed_semaphore_decrement corrected.
 *	009	Dave Butenhof	27 October 1989
 *		Make cma__g_tick_time static.
 *	010	Dave Butenhof	All Saints Day 1989
 *		Make use of cma__enter_kernel instead of manually whiling on
 *		test-and-set.
 *	011	Webb Scales	3 November 1989
 *		Temporarily #ifdef'd out semaphore.lock field references
 *	012	Bob Conti	4 November 1989
 *		Removed superfluous include cma_host.
 *	013	Bob Conti	5 November 1989
 *		Renamed time_slice constant to quanta, and
 *		time_slice field to quanta_remaining.
 *		Replaced last_dispatched time-stamp (which caused high
 *		overhead per context-switch) with an incrementing total
 *		runtime (in ticks).  Added constant for one tick.
 *		Rename actual to current everywhere.
 *	014	Bob Conti	6 November 1989
 *		Modify timed_semaphore_decrement to use kernel critical lock
 *		and get its code "mostly right".
 *	015	Webb Scales & Bob Conti		6 November 1989
 *		Moved quantum increment into cma__ready.  Added exit_kernel's
 *		where appropriate.
 *	016	Webb Scales	7 November 1989
 *		Renamed "PC" & "PSL" to "ret_pc" & "ret_psl".
 *		Consolidated calls to cma__AST_force_dispatch and
 *		cma__signal_force_dispatch into a single, os-independent call,
 *		cma__cause_force_dispatch.
 *	017	Dave Butenhof	9 November 1989
 *		Modify to use system time rather than private "ticks", and
 *		rename things for clarity.  Simplify a lot of the algorithms
 *		(especially in the timer AST/signal code).  Add some services
 *		for time conversion (using cma_t_date_time and cma_t_interval
 *		as bases).  (MAJOR REWRITE)
 *	018	Webb Scales	18 November 1989
 *		Changed call to cma__cause_force_dispatch to pass the address
 *		of the signal context block for MIPS/Ultrix.
 *	019	Webb Scales	19 November 1989
 *		Correct a typecast.
 *	020	Webb Scales	8 December 1989
 *		Overhauled non-blocking I/O
 *	021	Webb Scales	10 December 1989
 *		Changed cma__check_io to cma__io_available.
 *	022	Dave Butenhof and Webb Scales	9 January 1990
 *		Fix prototype of cma___periodic. Conditional should be
 *		"unix", not "ultrix".
 *	023	Webb Scales	9 January 1990
 *		Fix cma__add_time & cma__subtract_time to correctly detect and 
 *		handle carries/borrows to/from seconds from/to milliseconds.
 *	024	Dave Butenhof	26 January 1990
 *		Change cma__get_self_tcb_kernel to cma__get_self_tcb (it no
 *		longer locks the kernel, so distinction is irrelevant).
 *	025	Dave Butenhof	15 February 1990
 *		Fix interval_to_time routine; on VMS, it uses RTL routine to
 *		convert to quadword date/time.  A value of 0.0 is an "invalid
 *		time" since no quadword time value for delta-0 can exist.
 *		Bypass call for 0.0, and return quadword of -1 (smallest
 *		delta time).
 *	026	Webb Scales	23 March 1990
 *		Now avoids unnecessary dispatches.
 *	027	Dave Butenhof & Webb Scales	29 March 1990
 *		Add cell for next-timer-queue entry time (for null thread
 *		select).  Make timeslice signal value "global".
 *	028	Dave Butenhof & Webb Scales	6 April 1990
 *		Integrate Apollo changes.
 *	029	Webb Scales	17 April 1990
 *		Catch exceptions raised by get_self_tcb in periodic.
 *	030	Webb Scales	4 June 1990
 *		Establish timeslice signal handler in cma_signal.
 *		Make timeslice routine ("periodic") available to cma_signal.
 *	031	Dave Butenhof	6 June 1990
 *		Finish 030 (make name change of cma___periodic to
 *		cma__periodic consistent).
 *	032	Dave Butenhof	14 June 1990
 *		Use new cma__kernel_[un]set operations to deal with the
 *		kernel lock, instead of cma__test_and_set and cma__unset,
 *		which may now try to lock the kernel (on processors without
 *		interlocked instructions).
 *	033	Webb Scales	15 June 1990
 *		Added priority scheduling
 *		- Added global tick counter (absolute time)
 *		- Tick count is updated by timeslicer or deferred for kernel
 *		- Thread cpu time is now measured in ticks
 *	034	Webb Scales	6 July 1990
 *		Disabled tick-deferral in timeslicer.
 *	035	Dave Butenhof	03 August 1990
 *		Removed semaphore increment from timed condition wait timeout
 *	036	Dave Butenhof	14 August 1990
 *		Implement (primitive) asynchronous alert/cancel by checking
 *		for a pending alert on the current thread at each timer
 *		interrupt, and doing magic to cause the exception if there is
 *		one.
 *	036	Webb Scales	16 August 1990
 *		- Changed #ifdef's to #if's
 *		- Modified Apollo timeslice rate
 *		- Added uniprocessor optimization around get_self in ti*
 *		- Rearranged ifdefs in call to force_dispatch
 *	037	Bob Conti	1 October 1990
 *		Delete cma_c_null_thread
 *	038	Webb Scales & Dave Butenhof	 4 December 1990
 *		Modified check_timer_queue, making it global.
 *		Added a parameter to io_available.
 *	039	Webb Scales	 6 December 1990
 *		Added HP-specific asynch alert and yield actions.
 *	040	Paul Curtin	03 January 1991
 *		Added new feature to cma__check_timer_queue to 
 *		handle case were entry is readied.
 *	041	Dave Butenhof & Webb Scales	3 January 1991
 *		Fix 040: don't write to "next_time" parameter unless it's
 *		there.
 *	042	Dave Butenhof	7 February 1991
 *		Change names of alert state bits.
 *	043	Dave Butenhof	12 March 1991
 *		Add optimization to timer processing: check for (and process)
 *		expired entries at head of queue when inserting new entries.
 *	045	Dave Butenhof	04 April 1991
 *		Change _CMA_UNIPROCESSOR_ to 0/1 instead of ifdef/ifndef
 *	046	Dave Butenhof	24 April 1991
 *		DECthreads' "thread synchronous I/O" wrappers aren't used in
 *		non-multiplexed MP environment (the system is assumed to
 *		support that directly for kernel threads). So don't let the
 *		timer function make a call to io_available.
 *	047	Paul Curtin	31 May 1991
 *		Added a fork() reinitialization routine.
 *	048	Dave Butenhof	31 May 1991
 *		Disable timeslicing and such for _CMA_THREAD_IS_VP_ mode;
 *		just use thread-synchronous kernel timers.
 *	049	Paul Curtin	 5 May 1991
 *		Rearranged flags in reinit routine.
 *	050	Webb Scales and Dave Butenhof	    10 June 1991
 *		Conditionalize inclusion of I/O stuff.
 *	051	Dave Butenhof
 *		Integrate Apollo CMA5 reverse drop: remove special cased
 *		quantum and timer interval for Apollo platforms.
 *	052	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	053	Paul Curtin	18 November 1991
 *		Alpha work: added an include for starlet.h, and a number
 *		of Alpha switches
 *	054	Paul Curtin	10 November 1991
 *		Added an include for cma_util.h
 *	055	Paul Curtin	20 December 1991
 *		Remove starlet.h on VAX
 *	056	Webb Scales	28 January 1992
 *		- Changed exit-kernel to unset-kernel in periodic.
 *		- Reworked the end-quantum preemption on VMS
 *	057	Webb Scales	29 January 1992
 *		Repair damage to async alerts on VAX/VMS.
 *	058	Webb Scales	11 February 1992
 *		Add asynchronous alert code for both Alpha and VAX VMS.
 *	059	Dave Butenhof	10 March 1992
 *		Convert from ftime() function and 'struct timeb' to
 *		gettimeofday() function and 'struct timeval'. The new version
 *		isn't dependent on BSD-isms relegated to libbsd.a on many
 *		systems (such as OSF/1 and AIX).
 *	060	Webb Scales	13 March 1992
 *		Use scheduling policy "run-till block" characteristic instead 
 *		of policy itself to determine whether to do anynch. ctx. switch
 *	061	Dave Butenhof	19 March 1992
 *		Add call to cma__undefer() before all direct calls to
 *		cma__yield_processor() -- since cma__ready() no longer
 *		undefers.
 *	062	Webb Scales	31 March 1992
 *		Rework asynch context switch for U*ix.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_timer.h>
#include <cma_tcb.h>
#include <cma_dispatch.h>
#include <cma_stack.h>
#include <cma_condition.h>
#include <cma_semaphore.h>
#include <cma_assem.h>
#include <cma_util.h>
#include <cma_alert.h>
#include <cma_defer.h>

#if _CMA_OS_ == _CMA__VMS
# include <lib$routines.h>
# include <libdtdef.h>
# if _CMA_HARDWARE_ == _CMA__ALPHA
#  include <starlet.h>
# endif
#endif

#if _CMA_OS_ == _CMA__UNIX
# if !_CMA_THREAD_SYNC_IO_
#  include <cma_thread_io.h>
# endif
# include <sys/time.h>
#endif


/*
 * GLOBAL DATA
 */

cma_t_date_time	cma__g_one_tick;
cma_t_date_time	cma__g_next_tqe;
cma_t_integer	cma__g_time_ticks = 0;

/*
 * LOCAL DATA
 */

static cma_t_date_time	cma___g_last_time;
static cma_t_date_time	cma___g_start_time;
static cma__t_queue	cma___g_timer_queue;
#ifndef HP_CMA_GETTIME_OPT
static cma_t_integer    cma___g_checked_time;
#endif

/*
 * The timer interval is .1 seconds.
 */
#if _CMA_OS_ == _CMA__VMS
static cma_t_date_time	cma___g_vms_time_interval = {-1000*1000, -1};
#endif

#if _CMA_OS_ == _CMA__UNIX
# define cma___c_unix_time_interval  100000
#endif
void cma__zero_time(cma_t_date_time *);

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Add two times.
 *
 *  FORMAL PARAMETERS:
 *
 *	result	Result time
 *
 *	time1	first input time
 *	
 *	time2	second input time
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
extern void
cma__add_time(
	cma_t_date_time	*result,
	cma_t_date_time	*time1,
	cma_t_date_time	*time2)
    {
#if _CMA_OS_ == _CMA__VMS
    lib$add_times (time1, time2, result);
#else
    result->tv_usec = time1->tv_usec + time2->tv_usec;
    result->tv_sec  = time1->tv_sec  + time2->tv_sec;

    if (result->tv_usec >= 1000000) {	/* check for carry */
	result->tv_usec -= 1000000;
	result->tv_sec    += 1;
	}
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Check the timer queue and return time to next entry.
 *
 *  FORMAL PARAMETERS:
 *
 *	next_time : address of a cma_t_date_time to receive the delta time 
 *		    until the next timer queue entry will expire.
 *
 *  IMPLICIT INPUTS:
 *
 *	The timer queue.
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The ready queue(s).
 *
 *  FUNCTION VALUE:
 *
 *	Boolean, false if timer queue is empty at the time of the call;
 *		 true if timer queue is not empty at the time of the call.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#if !_CMA_THREAD_IS_VP_
extern cma_t_boolean
cma__check_timer_queue(
	cma_t_date_time	*next_time)
    {
    cma__t_tcb_time	*tcb_time;
    cma__t_int_tcb	*cur_tcb;
    cma_t_boolean	ret_val;
    cma_t_boolean	readied;


    cma__assert_fail (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "cma___check_timer_queue:  kernel unlocked.");

#ifndef HP_CMA_GETTIME_OPT
    cma___g_checked_time = 0;
#else
    readied = cma_c_false;
    cma__get_time (&cma___g_last_time);
#endif

    if (!cma__queue_empty (&cma___g_timer_queue)) {
#ifndef HP_CMA_GETTIME_OPT
        readied = cma_c_false;
        cma__get_time (&cma___g_last_time);
        cma___g_checked_time = 1;
#endif
	tcb_time = (cma__t_tcb_time *)cma__queue_next(
		&cma___g_timer_queue);

	while (cma__time_lss (&tcb_time->wakeup_time, &cma___g_last_time)
		&& (&tcb_time->queue != &cma___g_timer_queue)) {
	    (void)cma__queue_remove (&tcb_time->queue);
	    cur_tcb = cma__base (tcb_time, timer, cma__t_int_tcb);

	    switch (tcb_time->mode) {

		case cma__c_semaphore_timer : {
		    (void)cma__queue_remove (&cur_tcb->header.queue);
		    cur_tcb->event_status = cma__c_semaphore_timeout;
		    break;
		    }
#if _CMA_OS_ == _CMA__UNIX
		case cma__c_select_timer : {
		    (void)cma__queue_remove (&cur_tcb->header.queue);
		    cur_tcb->event_status = cma__c_select_timeout;
		    break;
		    }
#endif
		default : {		/* we have normal timer entry */
		    }

		}

	    cma__ready (cur_tcb, cma_c_false);
	    readied = cma_c_true;
	    tcb_time = (cma__t_tcb_time *)cma__queue_next (
		    &cma___g_timer_queue);
	    }

	/*
	 * If there are no more timer queue entries, zero the "next tqe" to
	 * reflect this.  If there are more entries, set the "next tqe" as
	 * appropriate.
	 */
	if (cma__queue_empty (&cma___g_timer_queue)) {
	    cma__zero (&cma__g_next_tqe);
	    }
	else {
	    cma__copy_time (
		    &cma__g_next_tqe,
		    &(((cma__t_tcb_time *)cma__queue_next (
			    &cma___g_timer_queue))->wakeup_time));
	    }
	    
	if (next_time != (cma_t_date_time *)cma_c_null_ptr)
	    if (readied) {
		cma__zero (next_time);
		}
	    else
		cma__subtract_time (
			next_time,
			&cma__g_next_tqe,
			&cma___g_last_time);

	ret_val = cma_c_true;
	}
    else
	ret_val = cma_c_false;

    return ret_val;
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Get current time
 *
 *  FORMAL PARAMETERS:
 *
 *      time	current time
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
extern void
cma__get_time(
	cma_t_date_time	*time)		/* Get the current date and time */
    {
#if _CMA_OS_ == _CMA__VMS
    sys$gettim ((long int *)time);
#endif
#if _CMA_OS_ == _CMA__UNIX
# if (_CMA_UNIX_TYPE != _CMA__SVR4)
    struct timezone	tmptz;
# endif


    /*
     * We don't bother with local time, so just put the timezone info in a
     * temporary and forget it.
     */
# if (_CMA_UNIX_TYPE != _CMA__SVR4)
    gettimeofday (time, &tmptz);
# else
    gettimeofday (time);
# endif
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize dispatch timer
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
 *	This routine starts the cma_dispatch timer, used for timeslicing
 * 	and timeout functions.
 */
void
cma__init_timer(void)
    {
#if !_CMA_THREAD_IS_VP_
# if _CMA_OS_ == _CMA__UNIX
    struct itimerval unix_timer, unix_oldtime;
# endif


    cma__get_time (&cma___g_start_time);

    /*
     * Initialize "time of first timer queue entry" to indicate that there
     * are no entries in the timer queue.
     */
    cma__zero (&cma__g_next_tqe);
    cma__queue_init (&cma___g_timer_queue);

# if _CMA_OS_ == _CMA__VMS
    cma__copy_time (&cma__g_one_tick, &cma___g_vms_time_interval);
    sys$setimr (
	    0,
	    &cma___g_vms_time_interval,
	    cma__periodic,
	    0,
	    0);
# endif
# if _CMA_OS_ == _CMA__UNIX
    cma__g_one_tick.tv_sec	= 0;
    cma__g_one_tick.tv_usec	= cma___c_unix_time_interval;

    unix_timer.it_interval.tv_sec  = 0;
    unix_timer.it_interval.tv_usec = cma___c_unix_time_interval;
    unix_timer.it_value.tv_sec  = 0;
    unix_timer.it_value.tv_usec = cma___c_unix_time_interval;
    setitimer (cma__c_timer, &unix_timer, &unix_oldtime);
# endif
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Insert entry in timer queue.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb	TCB to be inserted in queue.
 * 
 *      time	Time at which entry should expire.
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
 *	cma_c_true if inserted; cma_c_false if time is already passed.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#if !_CMA_THREAD_IS_VP_
cma_t_boolean
cma__insert_timer(
	cma__t_int_tcb	*tcb,
	cma_t_date_time	*time)
    {
    cma_t_boolean	value;
    cma__t_tcb_time	*time_list;


    cma__assert_warn (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "Call to cma__insert_timer outside of kernel.");

    /*
     * To possibly reduce the latency in processing timer events, check
     * events against the current time; if there are expired events on the
     * queue, process them.
     */
    (void)cma__check_timer_queue ((cma_t_date_time *)cma_c_null_ptr);
#ifndef HP_CMA_GETTIME_OPT
    /*
     * cma__check_timer_queue() won't have checked the time if the
     * queue was empty, so we need to update cma__g_last_time
     * ourselves ...
     */
    if(!cma___g_checked_time) cma__get_time(&cma___g_last_time);
#endif

    if (cma__time_lss (time, &cma___g_last_time))
	value = cma_c_false;
    else {
	cma__copy_time (&tcb->timer.wakeup_time, time);

	if (cma__queue_empty (&cma___g_timer_queue))
	    time_list = (cma__t_tcb_time *)&cma___g_timer_queue;
	else {				/* queue not empty */
	    time_list = (cma__t_tcb_time *)cma__queue_next (
		    &cma___g_timer_queue);

	    while (cma__time_lss (&time_list->wakeup_time, time)
		    && (&time_list->queue != &cma___g_timer_queue))
		time_list = (cma__t_tcb_time *)cma__queue_next (
			&time_list->queue);

	    }

	cma__queue_insert (&tcb->timer.queue, &time_list->queue);
	cma__copy_time (
		&cma__g_next_tqe,
		&(((cma__t_tcb_time *)cma__queue_next (
			&cma___g_timer_queue))->wakeup_time));
	value = cma_c_true;
	}

    return value;
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Convert cma_t_interval to delta cma_t_date_time.
 *
 *  FORMAL PARAMETERS:
 *
 *	interval	The interval
 * 
 *      time		Address to return delta time.
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
 *	raises exception if interval is invalid (negative).
 */
extern void
cma__interval_to_time(
	cma_t_interval	interval,
	cma_t_date_time	*time)
    {
#if _CMA_OS_ == _CMA__VMS
    cma_t_integer	delta_secs;
#endif

    if (interval < 0.0)
	cma__error (cma_s_badparam);

#if _CMA_OS_ == _CMA__VMS
    delta_secs = LIB$K_DELTA_SECONDS_F;

    if (interval == 0.0) {
	time->low = ~0;
	time->high = ~0;
	}
    else {
	cma_t_integer	status;

	
	status = lib$cvtf_to_internal_time (
		&delta_secs,
		&interval,
		time);

	if ((status & 7) != 1)		/* If return isn't NORMAL status */
	    cma__error (cma_s_badparam);

	}
#else
    time->tv_sec = (long)interval;
    time->tv_usec = (long)((interval - (float)time->tv_sec) * 1000000);
#endif
    }

/*
 *  dce1.1 cma__periodic moved to <platform>/cma_host -jd
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Perform any pre and/or post fork() reinitialization work
 *	for this module.
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
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
cma__reinit_timer(
	cma_t_integer	flag)
    {
#if !_CMA_THREAD_IS_VP_

    if (flag == 2) {	    /* Perform child post fork() work, in kernel */
	/*
	 * Initialize "time of first timer queue entry" to indicate that there
	 * are no entries in the timer queue.
	 */
	cma__zero (&cma__g_next_tqe);
	cma__queue_init (&cma___g_timer_queue);
	}

#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Subtract one time from another.
 *
 *  FORMAL PARAMETERS:
 *
 *      difference  - result
 * 
 *	minuend	    - time from which to subtract
 * 
 *	subtrahend  - time to subtract
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
extern void
cma__subtract_time(
	cma_t_date_time	*difference,
	cma_t_date_time	*minuend,
	cma_t_date_time	*subtrahend)
    {
#if _CMA_OS_ == _CMA__VMS
    lib$sub_times (minuend, subtrahend, difference);
#else
    difference->tv_usec	= minuend->tv_usec - subtrahend->tv_usec;
    difference->tv_sec	= minuend->tv_sec  - subtrahend->tv_sec;

	/*
	 * Note that tv_usec and tv_sec are defined as (signed)
	 * longs -- so it's appropriate to check for < 0 in
	 * checking for a borrow.
	 */
    if (difference->tv_usec < 0) {	/* check for borrow */
	difference->tv_usec += 1000000;
	difference->tv_sec    -= 1;
	}
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Zero time value
 *
 *  FORMAL PARAMETERS:
 *
 *	time	the time to be zeroed
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
cma__zero_time(
	cma_t_date_time	*time)
    {
#if _CMA_OS_ == _CMA__VMS
    time->low  = 0;
    time->high = 0;
#else
    time->tv_usec	= 0;
    time->tv_sec		= 0;
#endif
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TIMER.C */
/*  *27    3-APR-1992 18:34:13 SCALES "Rework async context switch on U*ix" */
/*  *26   19-MAR-1992 13:18:13 BUTENHOF "Add undefer() calls before yield_processor()" */
/*  *25   18-MAR-1992 19:02:06 SCALES "Parameterize scheduling policies" */
/*  *24   10-MAR-1992 16:27:39 BUTENHOF "Change ftime to gettimeofday" */
/*  *23   11-FEB-1992 16:30:16 SCALES "Asynch alerts for VMS (VAX & Alpha)" */
/*  *22   30-JAN-1992 22:13:05 SCALES "Repair async alerts for VAX/VMS" */
/*  *21   29-JAN-1992 23:48:41 SCALES "Rework async context switch on VMS" */
/*  *20   20-DEC-1991 11:30:57 CURTIN "removed starlet.h on VAX/VMS" */
/*  *19   20-NOV-1991 14:01:35 CURTIN "Added an include for cma_util.h" */
/*  *18   18-NOV-1991 11:42:07 CURTIN "Alpha work" */
/*  *17   14-OCT-1991 13:42:06 BUTENHOF "Refine/fix use of config symbols" */
/*  *16   24-SEP-1991 16:29:33 BUTENHOF "Merge CMA5 reverse IBM/HP/Apollo drops" */
/*  *15   11-JUN-1991 17:17:40 BUTENHOF "Remove old cma__traces" */
/*  *14   10-JUN-1991 18:24:50 SCALES "Add sccs headers for Ultrix" */
/*  *13   10-JUN-1991 17:55:44 SCALES "Conditionalize inclusion of I/O stuff" */
/*  *12    5-JUN-1991 16:15:52 CURTIN "fork work" */
/*  *11    2-JUN-1991 19:37:02 BUTENHOF "Stop using timers for _CMA_THREAD_IS_VP_" */
/*  *10   31-MAY-1991 16:44:26 CURTIN "Added a fork() reinitialization routine" */
/*  *9    29-MAY-1991 17:46:05 BUTENHOF "Fiddle with kthread stuff" */
/*  *8    14-MAY-1991 13:43:37 BUTENHOF "Integrate changes lost in disk crash" */
/*  *7     2-MAY-1991 13:59:26 BUTENHOF "Don't let it try thread_io under OSF/1" */
/*  *6    12-APR-1991 23:37:26 BUTENHOF "Change _CMA_UNIPROCESSOR_ to 0/1" */
/*  *5    13-MAR-1991 14:22:52 BUTENHOF "Timer optimization: process expired entries on insertion" */
/*  *4    12-FEB-1991 01:29:41 BUTENHOF "Change to alert control bits" */
/*  *3     4-JAN-1991 01:53:22 BUTENHOF "Fix the replacement bug." */
/*  *2     3-JAN-1991 18:58:57 CURTIN "completed additions" */
/*  *1    12-DEC-1990 21:55:05 BUTENHOF "Timer services" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_TIMER.C */
