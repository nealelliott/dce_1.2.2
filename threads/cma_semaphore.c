/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_semaphore.c,v $
 * Revision 1.1.8.1  1996/05/10  13:40:43  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:44 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  17:59  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:01:27  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:02  root  1.1.8.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:38:20  root]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:55:24  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:27:59  osfrcs  1.1.1.8]
 *
 * 	Modify cma__sem_iwkone_record and
 * 	cma__sem_iwkall_record to so that they
 * 	don't call cma__try_run if in cma__dispatch.
 * 	Also, change behavior of iwkall_record so that
 * 	all threads are readied before any attempt is
 * 	made to run one, regardless of whether we're
 * 	in dispatch or not, since that's probably a
 * 	better semantic and has to be implemented that
 * 	way at least for the case that we're called via
 * 	cma__dispatch.
 * 	[1993/01/06  17:46:36  hopkins]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:38  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:26  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:06  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:31  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:29:58  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:53  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:39:40  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:48  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:12  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:02:15  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:45:24  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:45:02  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:22  devrcs
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
 *	These routines provide the basic semaphore operations
 *
 *  AUTHORS:
 *
 *	Hans Oser
 *
 *  CREATION DATE:
 *
 *	11 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof & Webb Scales	27 September 1989
 *		Unlock semaphore ahead of blocking.
 *	002	Hans Oser			28 September 1989
 *		Fix empty queue tests, correct usage of builtins.
 *	003	Hans Oser			12 October 1989
 *		Added return values after blocking
 *	004	Hans Oser			13 October 1989
 *		Changements due to program review
 *	005	Dave Butenhof			19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	006	Dave Butenhof & Webb Scales	19 October 1989
 *		Remove inline operators.
 *	007	Webb Scales			19 October 1989
 *		Added a semicolon on cma__add_atomic in cma__increment_semaphore
 *	008	Hans Oser			25 October 1989	
 *		Additions for non-blocking I/O
 *	009	Dave Butenhof			27 October 1989
 *		Modify decrement_semaphore to avoid blocking if an alert is
 *		pending on current TCB.
 *	010	Dave Butenhof			30 October 1989
 *		Back off semaphore modification if decrement_semaphore exits
 *		due to pending alert (condition wait was aborted).
 *	011	Dave Butenhof	All Saints Day 1989
 *		Make use of cma__enter_kernel instead of manually whiling on
 *		test-and-set.
 *	012	Dave Butenhof and Bob Conti	3 November 1989
 *		Fix semaphore deferral problems:
 *		- Undefer on blocking decrement (as well as on increment)
 *		- Fix undefer_sema to only unblock the number of threads
 *		which are waiting, and also close window where additional
 *		defers could go unnoticed during process.
 *	013	All				3 November 1989
 *		Simplify semaphores.
 *	014	Webb Scales			3 November 1989
 *		Removed unneeded local declaration "tcb" from 
 *		cma__increment_semaphore and cma__unblock_all_waiting and put
 *		it inside the cma__incr_sem macro.
 *	015	Webb Scales			3 November 1989
 *		Rearranged cma__decrement_semaphore, since cma__block does an
 *		exit_kernel.
 *	016	Bob Conti			4 November 1989
 *		Removed superfluous include
 *	017	Webb Scales & Bob Conti		6 November 1989
 *		Added exit-kernel's where appropriate.
 *	018	Webb Scales			7 November 1989
 *		Added #include <cma_stack.h> for cma__get_self_tcb_kernel()
 *	019	Dave Butenhof			17 November 1989
 *		- Include cma_condition.h explicitly, since cma_tcb.h no longer
 *		  does it.
 *		- Move timed semaphore decrement operation here from
 *		  cma_timer.c
 *	020	Dave Butenhof			20 November 1989
 *		Timed semaphore wait isn't storing semaphore backpointer into
 *		timer entry... fix it.
 *	021	Dave Butenhof			22 November 1989
 *		Pass TCB into decrement_semaphore, and assume kernel is
 *		already locked (more efficient, more reliable).  This also
 *		allows me to move the condition variable-specific checks for
 *		alert-pending from the decrement_semaphore routines out into
 *		the condition wait routines where they belong.
 *	022	Dave Butenhof & Webb Scales	14 December 1989
 *		Change unblock_all_waiters to generate a pending spurious
 *		wakeup to deal with late waiters.
 *	023	Dave Butenhof	14 June 1990
 *		Replace use of cma__test_and_set on kernel lock with
 *		cma__kernel_set.
 *	024	Dave Butenhof	03 August 1990
 *		Simplify from counting semaphores to "blocking objects with
 *		state".  It's almost a binary semaphore, since it has state
 *		to support pending-wakeup, however rather than just P and V
 *		operations, it supports "wait", "wake one", and "wake all"
 *		operations.
 *	025	Dave Butenhof	08 August 1990
 *		Access pending state of semaphore with interlocked
 *		instruction.
 *	026	Dave Butenhof	31 May 1991
 *		Use new argument to cma__block() to support one-to-one VP
 *		synchronous timers.
 *	027	Dave Butenhof	11 June 1991
 *		Add formatting function for semaphore trace array.
 *	028	Dave Butenhof	25 July 1991
 *		Support debug build without VAXCRTL on VMS by using internal
 *		*printf emulation functions (#defined to real functions on
 *		UNIX platforms).
 *	029	Paul Curtin	20 August 1991
 *		Conditionalized out the inclusion of stdio.h on VMS.
 *	030	Dave Butenhof	17 September 1991
 *		Fix cma__sem_format_array() to close the file.
 *	031	Dave Butenhof	10 October 1991
 *		Change cma__sem_format_array() to write to stdout so it can
 *		be called from cma_debug().
 *	032	Dave Butenhof	10 March 1992
 *		Change timeb field references to timeval, since
 *		cma_t_date_time has been changed to avoid requiring libbsd.a
 *		on AIX and OSF/1.
 *	033	Dave Butenhof	17 March 1992
 *		Add cma__undefer() call prior to cma__yield_processor(),
 *		since low-level dispatch logic no longer undefers.
 *	034	Dave Butenhof	24 March 1992
 *		Make the sem trace variables static.
 *	035	Webb Scales	18 May 1992
 *		Rework queue manipulation in "wake-all".
 */

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_defs.h>
#include <cma_dispatch.h>
#include <cma_semaphore.h>
#include <cma_tcb.h>
#include <cma_defer.h>
#if _CMA_TRACE_SEM_
# include <cma_stack.h>
# if _CMA_OS_ == _CMA__UNIX
#  include <stdio.h>
# endif
#endif

/*
 * LOCAL DATA
 */

#ifdef _CMA_TRACE_SEM_
typedef enum CMA___T_SEM_TYPE {
    cma__c_sem_empty = 0,
    cma__c_sem_wait,
    cma__c_sem_timedwait,
    cma__c_sem_wakeone,
    cma__c_sem_wakeall,
    cma__c_sem_resume
    } cma___t_sem_type;

typedef struct CMA___T_SEM_TRACE {
    cma_t_integer	line;		/* Line, file, thd of operation */
    char		*file;
    cma__t_int_tcb	*thread;
    cma_t_integer	thdseq;
    cma__t_int_tcb	*target;	/* Target thread for wake */
    cma_t_integer	tarseq;
    cma__t_semaphore	*sem;
    cma___t_sem_type	operation;	/* Type of operation */
    cma_t_boolean	pending;
    } cma___t_sem_trace;

static cma___t_sem_trace cma___g_sem_trace[_CMA_TRACE_SEM_];
static cma_t_integer	cma___g_sem_idx;
static char 		*cma___g_operations[6] = {
    "empty",
    "wait",
    "timedwait",
    "wakeone",
    "wakeall",
    "resume"
    };
#endif

/*
 * LOCAL FUNCTIONS
 */
	
#ifdef _CMA_TRACE_SEM_

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine formats the entries in cma___g_sem_trace to assist in
 *	debugging.
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
 */
extern void
cma__sem_format_array(void)
    {
    cma_t_integer	i, empty = -1;
    char		buffer[256];


    buffer[0] = '\0';
    cma__putstring (buffer, "Dump of semaphore trace array:");
    cma__puteol (buffer);
    cma__puteol (buffer);
    cma__putformat (buffer, "\tCurrent active index is %d", cma___g_sem_idx);
    cma__puteol (buffer);

    for (i = 0; i < _CMA_TRACE_SEM_; i++) {

	if (cma___g_sem_trace[i].operation == cma__c_sem_empty) {
	    /*
	     * If this entry is empty, then just skip it. If it's the first
	     * in a series of empty entries, record the index.
	     */
	    if (empty == -1) empty = i;
	    }
	else {
	    /*
	     * If we've found a non-empty entry, then display it. First, if
	     * we just completed a series of empty entries, report the
	     * range.
	     */
	    if (empty != -1) {
		cma__putformat (
			buffer,
			"[[Entries %d to %d are empty]]",
			empty,
			i-1);
		cma__puteol (buffer);
		empty = -1;
		}

	    cma__putformat (
		    buffer,
		    "[%03d] %s(%08x)[%spend] %08x (%d); thd %08x (%d) from \"%s\":%d,",
		    i,
		    cma___g_operations[cma___g_sem_trace[i].operation],
		    cma___g_sem_trace[i].sem,
		    (cma___g_sem_trace[i].pending ? "" : "no"),
		    cma___g_sem_trace[i].target,
		    cma___g_sem_trace[i].tarseq,
		    cma___g_sem_trace[i].thread,
		    cma___g_sem_trace[i].thdseq,
		    cma___g_sem_trace[i].file,
		    cma___g_sem_trace[i].line);
	    cma__puteol (buffer);
	    }

	}

    if (empty != -1) {
	cma__putformat (
		buffer,
		"[[Entries %d to %d are empty]]\n",
		empty,
		_CMA_TRACE_SEM_-1);
	cma__puteol (buffer);
	empty = -1;
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Function equivalents of the production-version semaphore macros
 *	defined in cma_semaphore.h.  These versions maintain a history of
 *	semaphore operations in the array cma___g_sem_trace to aid in
 *	debugging.
 *
 *		cma__sem_iwake_all(semaphore,line,file)
 *		cma__sem_iwake_one(semaphore,line,file)
 *		cma__sem_wait(semaphore,tcb,line,file)
 *
 *  FORMAL PARAMETERS:
 *
 *	semaphore	pointer to semaphore
 *
 *	tcb		pointer to TCB
 *
 *	line		line number where operation was invoked
 *
 *	file		name of file where operation was invoked
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
cma__sem_iwkall_record(
	cma__t_semaphore	*semaphore,
	cma_t_integer		line,
	char			*file)
    {
    cma__t_int_tcb	*tcb;
    cma__t_queue	semq;
#ifdef __hpux
    cma_t_boolean	woke_preempt_thread = cma_c_false;
#endif


    if (!cma__queue_empty (&(semaphore)->queue)) {
	cma__queue_zero (&semq);
	cma__queue_insert_after (&semq, &(semaphore)->queue);
	cma__queue_remove (&(semaphore)->queue);
	cma__queue_init (&(semaphore)->queue);

	while (!cma__queue_empty (&semq)) {
	    tcb = (cma__t_int_tcb *)cma__queue_dequeue (&semq);
	    cma___g_sem_trace[cma___g_sem_idx].line = line;
	    cma___g_sem_trace[cma___g_sem_idx].file = file;
	    cma___g_sem_trace[cma___g_sem_idx].thread = cma__get_self_tcb ();
	    cma___g_sem_trace[cma___g_sem_idx].thdseq =
		cma___g_sem_trace[cma___g_sem_idx].thread->header.sequence;
	    cma___g_sem_trace[cma___g_sem_idx].target = tcb;
	    cma___g_sem_trace[cma___g_sem_idx].tarseq = tcb->header.sequence;
	    cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
	    cma___g_sem_trace[cma___g_sem_idx].operation = cma__c_sem_wakeall;
	    cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_false;

	    if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
		cma___g_sem_idx = 0;
	    else
		cma___g_sem_idx++;

	    cma__ready (tcb, cma_c_false);
#ifdef __hpux
	    /*
	     * We don't want to call cma__dispatch recursively
	     * (see more extensive comments at cma__sem_iwake_one
	     * in cma_semaphore.h), so we can't call try_run if
	     * we're already in cma_dispatch.  Since this may change
	     * which thread runs next for the case that we're in
	     * dispatch, we may as well make it work the same way
	     * when we're _not_ in dispatch.  The highest priority
	     * thread woken will be the next to run, not the first
	     * preempt-capable thread.  (Actually, none of the woken
	     * threads may run, if there are other threads out there
	     * with higher priorities.)
	     */
	    if(cma__would_run(tcb))
		woke_preempt_thread = cma_c_true;
#else
	    cma__try_run (tcb);
#endif
	    }
#ifdef __hpux
	/*
	 * See comments above.
	 */
	if( !cma__g_in_dispatch && woke_preempt_thread == cma_c_true ) {
	    cma__yield_processor();
	    }
#else
	cma__try_run ((cma__t_int_tcb *)semq);
#endif
	}
    else {
	cma___g_sem_trace[cma___g_sem_idx].line = line;
	cma___g_sem_trace[cma___g_sem_idx].file = file;
	cma___g_sem_trace[cma___g_sem_idx].thread = cma__get_self_tcb ();
	cma___g_sem_trace[cma___g_sem_idx].thdseq =
	    cma___g_sem_trace[cma___g_sem_idx].thread->header.sequence;
	cma___g_sem_trace[cma___g_sem_idx].target = (cma__t_int_tcb *)0;
	cma___g_sem_trace[cma___g_sem_idx].tarseq = 0;
	cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
	cma___g_sem_trace[cma___g_sem_idx].operation = cma__c_sem_wakeall;
	cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_true;

	if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	    cma___g_sem_idx = 0;
	else
	    cma___g_sem_idx++;

	cma__kernel_unset (&(semaphore)->nopending);
	}

    }

extern void
cma__sem_iwkone_record(
	cma__t_semaphore	*semaphore,
	cma_t_integer		line,
	char			*file)
    {
    cma__t_int_tcb	*tcb;


    if (!cma__queue_empty (&(semaphore)->queue)) {
	tcb = (cma__t_int_tcb *)cma__queue_dequeue (
		&(semaphore)->queue);
	cma___g_sem_trace[cma___g_sem_idx].line = line;
	cma___g_sem_trace[cma___g_sem_idx].file = file;
	cma___g_sem_trace[cma___g_sem_idx].thread = cma__get_self_tcb ();
	cma___g_sem_trace[cma___g_sem_idx].thdseq =
	    cma___g_sem_trace[cma___g_sem_idx].thread->header.sequence;
	cma___g_sem_trace[cma___g_sem_idx].target = tcb;
	cma___g_sem_trace[cma___g_sem_idx].tarseq = tcb->header.sequence;
	cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
	cma___g_sem_trace[cma___g_sem_idx].operation = cma__c_sem_wakeone;
	cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_false;

	if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	    cma___g_sem_idx = 0;
	else
	    cma___g_sem_idx++;

	cma__ready (tcb, cma_c_false);
#ifdef __hpux
	/*
	 * We don't want to call cma__dispatch recursively,
	 * see more extensive comments at cma__sem_iwake_one
	 * in cma_semaphore.h.
	 */
	if(!cma__g_in_dispatch)
	    cma__try_run (tcb);
#else
	cma__try_run (tcb);
#endif
	}
    else {
	cma___g_sem_trace[cma___g_sem_idx].line = line;
	cma___g_sem_trace[cma___g_sem_idx].file = file;
	cma___g_sem_trace[cma___g_sem_idx].thread = cma__get_self_tcb ();
	cma___g_sem_trace[cma___g_sem_idx].thdseq =
	    cma___g_sem_trace[cma___g_sem_idx].thread->header.sequence;
	cma___g_sem_trace[cma___g_sem_idx].target = (cma__t_int_tcb *)0;
	cma___g_sem_trace[cma___g_sem_idx].tarseq = 0;
	cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
	cma___g_sem_trace[cma___g_sem_idx].operation = cma__c_sem_wakeone;
	cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_true;

	if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	    cma___g_sem_idx = 0;
	else
	    cma___g_sem_idx++;

	cma__kernel_unset (&(semaphore)->nopending);
	}

    }

extern void
cma__sem_wait_record(
	cma__t_semaphore	*semaphore,
	cma__t_int_tcb		*tcb,
	cma_t_integer		line,
	char			*file)
    {
    cma__assert_warn (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "cma__sem_wait called without kernel locked.");
    tcb->event_status = cma__c_semaphore_event;
    cma___g_sem_trace[cma___g_sem_idx].line = line;
    cma___g_sem_trace[cma___g_sem_idx].file = file;
    cma___g_sem_trace[cma___g_sem_idx].thread = tcb;
    cma___g_sem_trace[cma___g_sem_idx].thdseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].target = tcb;
    cma___g_sem_trace[cma___g_sem_idx].tarseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
    cma___g_sem_trace[cma___g_sem_idx].operation = cma__c_sem_wait;
    cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_false;

    if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	cma___g_sem_idx = 0;
    else
	cma___g_sem_idx++;

    if (cma__kernel_set (&(semaphore)->nopending)) {
	cma__block (&(semaphore)->queue, 0);
	cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_false;
	}
    else
	cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_true;

    cma___g_sem_trace[cma___g_sem_idx].line = line;
    cma___g_sem_trace[cma___g_sem_idx].file = file;
    cma___g_sem_trace[cma___g_sem_idx].thread = tcb;
    cma___g_sem_trace[cma___g_sem_idx].thdseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].target = tcb;
    cma___g_sem_trace[cma___g_sem_idx].tarseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
    cma___g_sem_trace[cma___g_sem_idx].operation = cma__c_sem_resume;

    if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	cma___g_sem_idx = 0;
    else
	cma___g_sem_idx++;

    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a semaphore with timeout
 *
 *  FORMAL PARAMETERS:
 *
 *	semaphore	pointer to semaphore
 *
 *	tcb		pointer to TCB
 * 
 *	timeout		cma_t_date_time for timeout.
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
 *	cma_c_true if decrement completed: cma_c_false if timed out.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
cma_t_boolean
cma__sem_timed_wait(
	cma__t_semaphore	*semaphore,
	cma__t_int_tcb		*tcb,
	cma_t_date_time		*timeout)
    {
    cma_t_boolean	value = cma_c_true;
#if _CMA_THREAD_IS_VP_
    cma_t_date_time	delta, current;
    cma_t_integer	delta_ms;
#endif


    cma__assert_warn (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "cma__sem_timed_wait called without kernel locked.");

    tcb->event_status = cma__c_semaphore_event;
#ifdef _CMA_TRACE_SEM_
    cma___g_sem_trace[cma___g_sem_idx].line = __LINE__;
    cma___g_sem_trace[cma___g_sem_idx].file = __FILE__;
    cma___g_sem_trace[cma___g_sem_idx].thread = tcb;
    cma___g_sem_trace[cma___g_sem_idx].thdseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].target = tcb;
    cma___g_sem_trace[cma___g_sem_idx].tarseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
    cma___g_sem_trace[cma___g_sem_idx].operation  = cma__c_sem_timedwait;
    cma___g_sem_trace[cma___g_sem_idx].pending  = cma_c_false;

    if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	cma___g_sem_idx = 0;
    else
	cma___g_sem_idx++;
#endif

    if (cma__kernel_set (&semaphore->nopending)) {
	/*
	 * Wait for timeout or a wakeup, whichever comes first.
	 */
	tcb->timer.mode = cma__c_semaphore_timer;
	tcb->timer.semaphore = semaphore;

	/*
	 * Attempt to insert a timer entry.  If it succeeds (the specified
	 * time hasn't passed), then block the thread until either a wakeup
	 * occurs or the timer expires.  If the insertion fails (the timeout
	 * has already expired), mark the wait status as "timeout" for the
	 * common code below.
	 */
#if _CMA_THREAD_IS_VP_
	cma__get_time (&current);
	cma__subtract_time (&delta, timeout, &current);
# if _CMA_OS_ == _CMA__VMS
	/*
	 * FIX-ME:
	 *
	 * Need to implement this before supporting VMS kernel threads
	 */
	this doesn't matter since vms doesn't have kernel threads!!!
# else
	delta_ms = ((delta.tv_sec * 1000) + ((delta.tv_usec + 999) / 1000));
# endif

	if (delta_ms <= 0)
	    value = cma_c_false;	/* Instant timeout */
	else
	    value = cma__block (&semaphore->queue, delta_ms);

#else
	if (cma__insert_timer (tcb, timeout))
	    cma__block (&semaphore->queue, 0);
	else {
	    cma__undefer ();
	    cma__yield_processor ();
	    tcb->event_status = cma__c_semaphore_timeout;
	    }

	/*
	 * If the timer expired, then return value 'false'; otherwise, leave
	 * the default value 'true', and remove the TCB from the timer queue.
	 */
	if (tcb->event_status == cma__c_semaphore_timeout)
	    value = cma_c_false;
	else
	    (void)cma__queue_remove (
		    &tcb->timer.queue);
#endif

#ifndef _CMA_TRACE_SEM_
	}
#else
	cma___g_sem_trace[cma___g_sem_idx].pending  = cma_c_false;
	}
    else
	cma___g_sem_trace[cma___g_sem_idx].pending = cma_c_true;

    cma___g_sem_trace[cma___g_sem_idx].line = __LINE__;
    cma___g_sem_trace[cma___g_sem_idx].file = __FILE__;
    cma___g_sem_trace[cma___g_sem_idx].thread = tcb;
    cma___g_sem_trace[cma___g_sem_idx].thdseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].target = tcb;
    cma___g_sem_trace[cma___g_sem_idx].tarseq = tcb->header.sequence;
    cma___g_sem_trace[cma___g_sem_idx].sem = semaphore;
    cma___g_sem_trace[cma___g_sem_idx].operation  = cma__c_sem_resume;

    if (cma___g_sem_idx >= _CMA_TRACE_SEM_-1)
	cma___g_sem_idx = 0;
    else
	cma___g_sem_idx++;
#endif

    return value;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEMAPHORE.C */
/*  *15   18-MAY-1992 16:49:17 SCALES "Rework queue manipulation slightly for wake-all" */
/*  *14   24-MAR-1992 14:58:26 BUTENHOF "Fix typo" */
/*  *13   24-MAR-1992 14:47:34 BUTENHOF "Make sem trace variables static" */
/*  *12   17-MAR-1992 09:56:09 BUTENHOF "Undefer before yielding" */
/*  *11   10-MAR-1992 16:27:09 BUTENHOF "Change timeb to timeval" */
/*  *10   14-OCT-1991 13:39:52 BUTENHOF "Change trace dumps to go to stdout" */
/*  *9    17-SEP-1991 13:21:22 BUTENHOF "Fix format debug array functions" */
/*  *8    21-AUG-1991 16:44:02 CURTIN "Removed VMS include of stdio.h" */
/*  *7    25-JUL-1991 13:53:46 BUTENHOF "Use cma__int_*printf functions" */
/*  *6    11-JUN-1991 17:17:24 BUTENHOF "Add & use functions to dump kernel/sem trace arrays" */
/*  *5    10-JUN-1991 18:23:11 SCALES "Add sccs headers for Ultrix" */
/*  *4     2-JUN-1991 19:36:39 BUTENHOF "Use timed IPC for delay" */
/*  *3    29-MAY-1991 17:45:36 BUTENHOF "Try some semaphore logging..." */
/*  *2    12-APR-1991 23:36:35 BUTENHOF "Init atomic bits using new constants" */
/*  *1    12-DEC-1990 21:52:17 BUTENHOF "Binary semaphores" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEMAPHORE.C */
