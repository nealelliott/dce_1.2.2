/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: cma_condition.h $Revision: 1.1.9.1 $ $Date: 1996/05/10 13:34:26 $
 */
/*
 * HISTORY
 * $Log: cma_condition.h,v $
 * Revision 1.1.9.1  1996/05/10  13:34:26  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:39 UTC  bissen
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  19:58 UTC  jss  /main/HPDCE02/2]
 *
 * 	Changes for single threaded client.  Added code to handle single threaded
 * 	delays.
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:41 UTC  jss  /main/HPDCE02/1]
 *
 * 	CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:28  devsrc]
 * 	 *
 *
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:07  jd]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  14:32  truitt  /main/HPDCE01/1]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:12:19  truitt]
 * 	 *
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:02  root  1.1.6.3]
 *
 * 	C++ support:
 * 	add 'extern "C" { ... }'
 * 	[1993/08/03  18:11:14  hopkins]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  17:53:23  root]
 *
 * 	Added revision control strings
 * 	[1993/06/04  18:32:17  hopkins]
 *
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:34  alan]
 *
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:01  bolinger]
 *
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:52:37  bolinger]
 *
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  22:33:20  mgm  1.1.1.11]
 *
 * Revision 1.1.5.2  1994/06/09  13:37:28  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:28  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:15:37  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:07  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:24  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:34  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:35:20  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:01  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:52:52  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:52:37  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:54  devrcs
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
 *	Header file for mutex operations
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	6 September 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	14 September 1989
 *		Add semaphore to structure for low-level waiting.
 *	002	Dave Butenhof	04 October 1989
 *		Implement internal interfaces to wait, signal, broadcast.
 *	003	Dave Butenhof	1 November 1989
 *		Put all conditions on a queue.
 *	004	Dave Butenhof	16 November 1989
 *		Change time parameter types from "cma__t_ticks" (obsolete) to
 *		cma_t_date_time.
 *	005	Dave Butenhof	1 December 1989
 *		Abbreviate cma_t_condition to cma_t_cond for POSIX
 *		compatibility.
 *	006	Dave Butenhof & Webb Scales	4 December 1989
 *		Include cma_semaphore_defs.h instead of cma_semaphore.h
 *	007	Dave Butenhof	9 April 1990
 *		Remove definition of known condition queue header; it's now
 *		in cma_debugger.
 *	008	Dave Butenhof	23 May 1990
 *		Make delay mutex & cv extern for pthread delay
 *	030	Dave Butenhof	08 August 1990
 *		Post pending wake on semaphore if no waiters, with single
 *		interlocked instruction.
 *	031	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	032	Dave Butenhof	26 November 1990
 *		Fix a bug in signal; it set cv->event even though a signal
 *		may leave other waiters. Signal must declare that there may
 *		still be waiters.
 *	033	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	034	Dave Butenhof	4 February 1991
 *		Change prototypes for cma__int_wait and cma__int_timed_wait.
 *	035	Dave Butenhof	02 April 1991
 *		Remove pending wakeup when there are definitely no waiters.
 *	036	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 *	037	Paul Curtin	10 May 1991
 *		Added a number of new macros
 *	038	Dave Butenhof	17 May 1991
 *		Fix a bug isolated and diagnosed by Harold Seigel (thanks,
 *		Harold!)  T'was exposed by the removal of pending wake on
 *		condition variables... if a low priority thread waits on a
 *		condition while a high priority thread is waiting on the
 *		associated mutex, releasing the mutex will start the high
 *		priority thread.  If it then signals, there's no trace that
 *		the low priority thread may be waiting, since it hadn't
 *		established its intent to wait, and the signal is ignored.
 *		Now it will clear "event" before releasing the mutex; the
 *		signal will now issue a wake on the semaphore... since the
 *		semaphore queue is empty, cma__sem_wake_one will record a
 *		pending wakeup and the waiter's sem_wait operation will
 *		return immediately.
 *	040	Paul Curtin	24 May 1991
 *		Added a prototype for cma__reinit_cv.
 *	041	Dave Butenhof	22 November 1991
 *		Remove typedef into a separate cma_cond_defs.h file, to
 *		break a circularity from cma_defer.h.
 *	042	Dave Butenhof	10 February 1992
 *		Remove int_cond_create macro; function now differs between
 *		interfaces.
 */

#ifndef CMA_CONDITION
#define CMA_CONDITION

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_cond_defs.h>
#include <cma_attr.h>
#include <cma_semaphore.h>
#include <cma_timer.h>
#include <cma_mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void  cma__singlethreaded_delay (cma_t_date_time *);

/*
 * CONSTANTS AND MACROS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Broadcast a condition variable (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Pointer to condition variable
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
#define cma__int_broadcast(cv) { \
    if (!cma__test_and_set (&((cma__t_int_cv *)cv)->event)) \
	cma__int_cv_broadcast ((cma__t_int_cv *)cv); \
    }


/*
 * These two macros implement the semaphore wait (and surrounding mechanisms
 * specific to condition variables) for both timed and untimed waits.  They
 * begin by attempting to deliver any pending alert, so that (if possible)
 * the cma_s_alerted exception is raised before anything else has been done.
 * However, it's also possible that an alert could sneak in between the check
 * and the wait; we can't allow the wait to block if an alert has become
 * pending.  Therefore, AFTER locking the kernel (which will prevent the
 * delivery of an alert), we'll check for an alert: the thread only waits if
 * there is none.  Finally, in case we skipped the wait (or the wait was
 * broken through by an alert arriving), attempt to deliver an alert again.
 *
 * To "inline" condition operations, we use an "event" bit, maintained by
 * interlocked instructions.  The bit is clear when one or more threads MIGHT
 * be waiting on the condition (a test-and-set is used to determine whether
 * a wakeup should be attempted on signal or broadcast).  Note that the event
 * bit is cleared before waiting on the semaphore, and also after the wait
 * returns; this ensures that threads can't be forgotten because of any race
 * between the signaller setting the bit and some other waiter clearing it.
 */
#define cma__timed_wait(cv,mutex,timeout,return) \
    { \
    cma__t_int_tcb	*tcb; \
    if (!cma__is_multithreaded()) { \
       cma__singlethreaded_delay(timeout); \
       (return) = cma_s_timed_out; \
    } \
    else { \
       tcb = cma__get_self_tcb (); \
       cma__attempt_delivery (tcb); \
       cma__int_unlock ((mutex)); \
       cma__unset (&(cv)->event); \
       cma__enter_kernel (); \
       if ((!tcb->alert.pending) || (!tcb->alert.g_enable)) { \
	   tcb->wait_cv = (cv); \
	   if (cma__sem_timed_wait ( \
		   &(cv)->semaphore, \
		   tcb, \
		   (timeout))) \
	       (return) = cma_s_normal; \
	   else \
	       (return) = cma_s_timed_out; \
	   tcb->wait_cv = (cma__t_int_cv *)cma_c_null_ptr; \
	   } \
       cma__exit_kernel (); \
       cma__unset (&(cv)->event); \
       cma__int_lock ((mutex)); \
       cma__attempt_delivery (tcb); \
       } \
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *  cma__wait
 *
 *  FORMAL PARAMETERS:
 *
 *	cma__t_int_cv	    *condition
 *	cma__t_int_mutex    *mutex
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
#define cma__wait(cv,mutex) \
    { \
    cma__t_int_tcb	*tcb; \
    tcb = cma__get_self_tcb (); \
    cma__attempt_delivery (tcb); \
    cma__unset (&(cv)->event); \
    cma__int_unlock ((mutex)); \
    cma__enter_kernel (); \
    if ((!tcb->alert.pending) || (!tcb->alert.g_enable)) { \
	tcb->wait_cv = (cv); \
	cma__sem_wait (&(cv)->semaphore, tcb); \
	tcb->wait_cv = (cma__t_int_cv *)cma_c_null_ptr; \
	} \
    cma__exit_kernel (); \
    cma__unset (&(cv)->event); \
    cma__int_lock ((mutex)); \
    cma__attempt_delivery (tcb); \
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition variable (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Pointer to condition variable
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
#define cma__int_signal(cv) { \
    if (!cma__test_and_set (&((cma__t_int_cv *)cv)->event)) { \
	cma__unset (&((cma__t_int_cv *)cv)->event); \
	cma__int_cv_signal ((cma__t_int_cv *)cv); \
	} \
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_cond_delete - performs work for cma_cond_delete
 *
 *  FORMAL PARAMETERS:
 *
 *	cma_t_cond	*_condition_	    - Condition variable to free
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	New CV
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_cond_delete(_condition_) { \
    cma__t_int_cv       *_int_cv_; \
    (_int_cv_) = cma__validate_null_cv (_condition_); \
    if ((_int_cv_) == (cma__t_int_cv *)cma_c_null_ptr) \
        return; \
    if (cma__cv_waiting ((_condition_))) \
        cma__error (cma_s_in_use); \
    cma__free_cv (_int_cv_); \
    cma__clear_handle (_condition_); \
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__int_cond_wait - Performs work for cma_cond_wait
 *
 *  FORMAL PARAMETERS:
 *
 *	cma_t_cond	*_condition_	- Condition variable to wait on
 *	cma_t_mutex	*_mutex_	_ Mutex to unlock during wait
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	New CV
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#ifdef NDEBUG
# define cma__int_cond_wait(_cond_,_mu_) { \
    cma__t_int_mutex    *_int_mu_; \
    cma__t_int_cv       *_int_cv_; \
    (_int_mu_) = (cma__t_int_mutex *)((cma__t_int_handle *)(_mu_))->pointer; \
    (_int_cv_) = (cma__t_int_cv *)((cma__t_int_handle *)(_cond_))->pointer; \
    cma__wait ((_int_cv_), (_int_mu_)); \
    }
#else
# define cma__int_cond_wait(_condition_,_mutex_) { \
    cma__t_int_mutex    *_int_mutex_; \
    cma__t_int_cv       *_int_cv_; \
    (_int_mutex_) = cma__validate_mutex (_mutex_); \
    (_int_cv_) = cma__validate_cv (_condition_); \
    cma__wait ((_int_cv_), (_int_mutex_)); \
    }
#endif

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Find out whether a condition variable has waiters (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	condition	Pointer to condition object to test
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
 *	cma_c_true if condition has waiters, cma_c_false otherwise.
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
#define cma__int_cv_waiting(cv) \
    cma__sem_if_waiters (&(cv)->semaphore)

/*
 * TYPEDEFS
 */

/*
 *  GLOBAL DATA
 */

extern cma__t_int_mutex	*cma__g_delay_mutex;
extern cma__t_int_cv	*cma__g_delay_cv;

/*
 * INTERNAL INTERFACES
 */

extern cma_t_boolean cma__cv_waiting  (cma_t_cond *);

extern void cma__destroy_cv  (cma__t_int_cv *_cv);

extern void cma__free_cv  (cma__t_int_cv *);	

extern void cma__free_cv_nolock  (cma__t_int_cv *);

extern cma__t_int_cv *cma__get_cv  (cma__t_int_attr *);	

extern void cma__init_cv  (void);

extern void cma__int_cv_broadcast  (cma__t_int_cv *);

extern void cma__int_cv_signal  (cma__t_int_cv *);

extern cma_t_status cma__int_timed_wait  (cma__t_int_cv *,cma__t_int_mutex *,cma_t_date_time *);

extern void cma__int_wait  (cma__t_int_cv *,cma__t_int_mutex *);

extern void cma__reinit_cv  (cma_t_integer);

#ifdef __cplusplus
}
#endif

#endif
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CONDITION.H */
/*  *13   18-FEB-1992 15:27:44 BUTENHOF "Remove macros for create object" */
/*  *12   22-NOV-1991 11:55:02 BUTENHOF "Split into cma_condition.h and cma_cond_defs.h" */
/*  *11   10-JUN-1991 19:50:55 SCALES "Convert to stream format for ULTRIX build" */
/*  *10   10-JUN-1991 19:20:08 BUTENHOF "Fix the sccs headers" */
/*  *9    10-JUN-1991 18:17:46 SCALES "Add sccs headers for Ultrix" */
/*  *8    24-MAY-1991 16:43:50 CURTIN "Added a new reinit routine" */
/*  *7    17-MAY-1991 15:10:10 BUTENHOF "Unset ""event"" in cond wait before releasing mutex" */
/*  *6    10-MAY-1991 11:56:35 CURTIN "Put to use a number of new macros" */
/*  *5    12-APR-1991 23:34:56 BUTENHOF "Change type of internal locks" */
/*  *4     3-APR-1991 10:51:46 BUTENHOF "Remove pending wakeup" */
/*  *3     5-FEB-1991 00:59:33 BUTENHOF "Change prototypes for int CV ops" */
/*  *2    14-DEC-1990 00:55:15 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:42:47 BUTENHOF "Condition variables" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CONDITION.H */
