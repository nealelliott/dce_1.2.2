/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_semaphore.h $Revision: 1.1.8.1 $ $Date: 1996/05/10 13:40:55 $
 */
/*
 * HISTORY
 * $Log: cma_semaphore.h,v $
 * Revision 1.1.8.1  1996/05/10  13:40:55  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:44 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:43 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  17:57  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/21  14:05:46  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:03  root  1.1.8.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:13:29  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:39:36  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:34:32  hopkins]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:55:30  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:28:04  osfrcs  1.1.1.8]
 *
 * 	Modify cma__sem_iwake_one and cma__sem_iwake_all
 * 	to check value of cma__g_in_dispatch before
 * 	calling cma__try_run, since cma__dispatch can't
 * 	be called recursively.  Also, change semantics
 * 	of iwake_all so that _all_ threads are readied before
 * 	attempting to run _any_ of them.
 * 	[1993/01/06  17:46:52  hopkins]
 *
 * Revision 1.1.4.2  1994/06/09  13:38:39  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:27  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:08  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:34  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:30:03  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:40:58  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:39:46  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:53  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:22:16  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:04:11  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:46:31  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:46:11  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:39:22  devrcs
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
 *	Header file for semaphore operations
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
 *	001	Hans Oser			13 October 1989
 *		Changements due to program review
 *	002	Dave Butenhof & Webb Scales	19 October 1989
 *		Change count and defer_count fields to be short, since
 *		they're operated on by _ADAWI, which only takes shorts
 *		anyway.
 *	003	All				3 November 1989
 *		Add cma__validate_semaphore macro.
 *	004	Webb Scales			3 November 1989
 *		Put local declaration of "tcb" inside the cma__incr_sem
 *		macro.
 *	005	Dave Butenhof			17 November 1989
 *		Moved timed semaphore wait from cma_timer to here.
 *	006	Dave Butenhof			22 November 1989
 *		Add TCB to interface.
 *	007	Dave Butenhof	4 December 1989
 *		Remove type definitions to cma_semaphore_defs.h
 *	008	Webb Scales	20 June 1990
 *		Added preempt argument to call to ready in incr sem macro
 *	009	Dave Butenhof	03 August 1990
 *		Simplify from counting semaphores to "blocking objects with
 *		state".  It's almost a binary semaphore, since it has state
 *		to support pending-wakeup, however rather than just P and V
 *		operations, it supports "wait", "wake one", and "wake all"
 *		operations.
 *	010	Dave Butenhof	08 August 1990
 *		Access pending state of semaphore with interlocked
 *		instruction, and add macro to simply post pending wakeup.
 *	011	Webb Scales	10 October 1990
 *		Modified wake all to make it more "atomic"
 *	012	Dave Butenhof	09 April 1991
 *		Use new constants to init atomic bits
 *	013	Dave Butenhof	31 May 1991
 *		Use new argument to cma__block() to support one-to-one VP
 *		synchronous timers.
 *	014	Dave Butenhof	11 June 1991
 *		Add formatting function for semaphore trace array.
 *	015	Webb Scales	11 amy 1992
 *		Reinit elements which are treated like queue heads before ready
 *	016	Webb Scales	18 May 1992
 *		Rework queue manipulation in "wake-all".
 */

#ifndef CMA_SEMAPHORE
#define CMA_SEMAPHORE

/*
 *  INCLUDE FILES
 */
#include <cma.h>
#include <cma_defs.h>
#include <cma_semaphore_defs.h>
#include <cma_queue.h>
#include <cma_errors.h>
#include <cma_tcb_defs.h>
#include <cma_dispatch.h>
#include <cma_defer.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CONSTANTS AND MACROS
 */

#define cma__sem_if_waiters(semaphore) \
    (!cma__queue_empty (&(semaphore)->queue))

#define cma__sem_init(semaphore) \
    { \
    cma__queue_init (&(semaphore)->queue); \
    cma__tac_set (&(semaphore)->nopending); \
    }

#ifdef _CMA_TRACE_SEM_
# define cma__sem_iwake_one(semaphore) \
    cma__sem_iwkone_record (semaphore, __LINE__, __FILE__)
#else
# ifdef __hpux
  /*
   * If we're already in cma__dispatch (which means we
   * got here via cma__undefer), we don't want to call
   * cma__try_run, because it may try and ready this thread,
   * which is either: a) already on the ready list, or;
   * b) is supposed to be blocked or terminated.  Worse,
   * if cma__try_run runs the new thread, this thread will
   * not be on the ready list when it returns, and once this
   * thread gets context switched out (which it will, since
   * we're returning to cma__dispatch), it may never run again.
   */
# define cma__sem_iwake_one(semaphore) \
    { \
    cma__t_int_tcb	*__itcb__; \
    if (!cma__queue_empty (&(semaphore)->queue)) { \
	__itcb__ = (cma__t_int_tcb *)cma__queue_dequeue ( \
		&(semaphore)->queue); \
	cma__ready (__itcb__, cma_c_false); \
	if(!cma__g_in_dispatch) \
	    cma__try_run (__itcb__); \
	} \
    else \
	cma__kernel_unset (&(semaphore)->nopending); \
    }
# else
# define cma__sem_iwake_one(semaphore) \
    { \
    cma__t_int_tcb	*__itcb__; \
    if (!cma__queue_empty (&(semaphore)->queue)) { \
	__itcb__ = (cma__t_int_tcb *)cma__queue_dequeue ( \
		&(semaphore)->queue); \
	cma__ready (__itcb__, cma_c_false); \
	cma__try_run (__itcb__); \
	} \
    else \
	cma__kernel_unset (&(semaphore)->nopending); \
    }
# endif	/* __hpux */
#endif

/*
 * Awaken all current waiters.
 *
 *     If the queue is non-empty
 *         Get the address of the last TCB in the queue (this will be used as
 *                 the queue head).
 *         Remove the queue head from the queue.
 *         Reinitialize the queue head.
 *
 * This is all safe to do because we hold the kernel.  At this point, we could
 * conceivably release the kernel (except that we need to call cma__ready).
 * Regardless, the semaphore queue is now empty and can accept new waiters, we
 * just need to wake the threads that were waiting.
 *
 *         Using the last TCB as if it were the queue head, wake each thread
 *                 until the queue is "empty".
 *         Finally wake the last thread.
 *    If there were no waiters, set the wake pending state.
 */
#ifdef _CMA_TRACE_SEM_
# define cma__sem_iwake_all(semaphore) \
    cma__sem_iwkall_record (semaphore, __LINE__, __FILE__)
#else
# ifdef __hpux
  /*
   * We don't want to call cma__try_run
   * if we're in cma__dispatch, see more
   * extensive comments at cma__sem_iwake_one.
   *
   * Also, since we have to ready _all_ woken
   * threads before we're finished here, but
   * we can't run any of them if we're in dispatch,
   * the semantic is changed:  the thread that runs
   * when we're done may not be the first preempt-
   * capable thread we woke.  Since this may even
   * be a better semantic, let's make it consistent
   * by doing it this way even when we're _not_
   * called via cma__dispatch/cma__undefer.
   */
# define cma__sem_iwake_all(semaphore) \
    { \
    cma__t_int_tcb	*__itcb__; \
    cma__t_queue	__q__; \
    cma_t_boolean	__woke_preempt_thread__ = cma_c_false; \
    if (!cma__queue_empty (&(semaphore)->queue)) { \
	cma__queue_zero (&__q__); \
	cma__queue_insert_after (&__q__, &(semaphore)->queue); \
	cma__queue_remove (&(semaphore)->queue);\
	cma__queue_init (&(semaphore)->queue);\
	while (!cma__queue_empty (&__q__)) { \
	    __itcb__ = (cma__t_int_tcb *)cma__queue_dequeue (&__q__);\
	    cma__ready (__itcb__, cma_c_false); \
	    if(cma__would_run(__itcb__)) \
		__woke_preempt_thread__ = cma_c_true; \
	    } \
	if(!cma__g_in_dispatch && __woke_preempt_thread__ == cma_c_true) { \
	    cma__yield_processor(); \
	    } \
	} \
    else \
	cma__kernel_unset (&(semaphore)->nopending); \
    }
# else
# define cma__sem_iwake_all(semaphore) \
    { \
    cma__t_int_tcb	*__itcb__; \
    cma__t_queue	__q__; \
    if (!cma__queue_empty (&(semaphore)->queue)) { \
	cma__queue_zero (&__q__); \
	cma__queue_insert_after (&__q__, &(semaphore)->queue); \
	cma__queue_remove (&(semaphore)->queue);\
	cma__queue_init (&(semaphore)->queue);\
	while (!cma__queue_empty (&__q__)) { \
	    __itcb__ = (cma__t_int_tcb *)cma__queue_dequeue (&__q__);\
	    cma__ready (__itcb__, cma_c_false); \
	    cma__try_run (__itcb__); \
	    } \
	} \
    else \
	cma__kernel_unset (&(semaphore)->nopending); \
    }
# endif	/* __hpux */
#endif

/*
 * NOTE: only use this when kernel is not locked (it uses "unset" rather than
 * "kernel_unset").
 */
#define cma__sem_post_pending(semaphore) \
    cma__unset (&(semaphore)->nopending);

#ifdef _CMA_TRACE_SEM_
# define cma__sem_wait(semaphore,tcb) \
    cma__sem_wait_record (semaphore, tcb, __LINE__, __FILE__)
#else
# define cma__sem_wait(semaphore,tcb) \
    { \
    cma__assert_warn ( \
	    cma__tac_isset (&cma__g_kernel_critical), \
	    "cma__sem_wait called without kernel locked."); \
    tcb->event_status = cma__c_semaphore_event; \
    if (cma__kernel_set (&(semaphore)->nopending)) \
	cma__block (&(semaphore)->queue, 0); \
    }
#endif

#define cma__sem_wake_all(semaphore) \
    { \
    if (!cma__tryenter_kernel ()) { \
	cma__sem_iwake_all (semaphore); \
	cma__exit_kernel (); \
	} \
    else \
	cma__defer ( \
		cma__c_defer_wake_all, \
		(cma_t_address)(semaphore), \
		cma_c_null_ptr); \
    }

#define cma__sem_wake_one(semaphore) \
    { \
    if (!cma__tryenter_kernel ()) { \
	cma__sem_iwake_one (semaphore); \
	cma__exit_kernel (); \
	} \
    else \
	cma__defer ( \
		cma__c_defer_wake_one, \
		(cma_t_address)(semaphore), \
		cma_c_null_ptr); \
    }

/*
 *  TYPE DEFINITIONS
 */

/*
 *  GLOBAL DATA
 */

/*
 * EXTERNAL INTERFACES
 */

/*
 * Decrement a semaphore with timeout.
 */
#ifdef _CMA_TRACE_SEM_
extern void cma__sem_format_array  (void);

extern void cma__sem_iwkall_record  (cma__t_semaphore *,cma_t_integer,char *);

extern void cma__sem_iwkone_record  (cma__t_semaphore *,cma_t_integer,char *);

extern void cma__sem_wait_record  
       (cma__t_semaphore *,cma__t_int_tcb *,cma_t_integer,char *);
#endif

extern cma_t_boolean cma__sem_timed_wait  
          (cma__t_semaphore *,cma__t_int_tcb *,cma_t_date_time *);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEMAPHORE.H */
/*  *11   18-MAY-1992 16:49:32 SCALES "Rework queue manipulation slightly for wake-all" */
/*  *10   15-MAY-1992 15:04:06 SCALES "Add queue consistency checks" */
/*  *9    11-JUN-1991 17:17:32 BUTENHOF "Add & use functions to dump kernel/sem trace arrays" */
/*  *8    10-JUN-1991 19:55:37 SCALES "Convert to stream format for ULTRIX build" */
/*  *7    10-JUN-1991 19:21:27 BUTENHOF "Fix the sccs headers" */
/*  *6    10-JUN-1991 18:23:15 SCALES "Add sccs headers for Ultrix" */
/*  *5     2-JUN-1991 19:36:44 BUTENHOF "Use VP-synch timers" */
/*  *4    29-MAY-1991 17:45:43 BUTENHOF "Try some semaphore logging..." */
/*  *3    14-MAY-1991 13:43:33 BUTENHOF "Integrate changes lost in disk crash" */
/*  *2    12-APR-1991 23:36:39 BUTENHOF "Init atomic bit" */
/*  *1    12-DEC-1990 21:52:18 BUTENHOF "Binary semaphores" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEMAPHORE.H */
