/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_condition.c,v $
 * Revision 1.1.9.1  1996/05/10  13:34:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:39 UTC  bissen
 * 	Add cma__singlethreaded_delay
 * 	[1996/05/09  21:04:20  arvind]
 *
 * Revision 1.1.5.2  1994/06/09  13:37:27  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:26  devsrc]
 * 
 * Revision 1.1.5.1  1994/04/01  20:15:35  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:04  jd]
 * 
 * Revision 1.1.2.3  1992/12/15  22:26:20  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:28  alan]
 * 
 * Revision 1.1.2.2  1992/09/03  14:35:10  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:01:56  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:21:01  bolinger
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:52:18  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  22:52:10  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  22:51:52  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:38:46  devrcs
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
 *	Operations on condition variable object
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
 *	001	Dave Butenhof	18 September 1989
 *		Implement condition variables on top of dispatcher's
 *		semaphores.
 *	002	Dave Butenhof	4 October 1989
 *		Implement internal interfaces to wait, signal, broadcast.
 *	003	Dave Butenhof	9 October 1989
 *		Use cma__error to raise exceptions where necessary.
 *	004	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 *	005	Dave Butenhof	18 October 1989
 *		cma__queue_insert is now a macro, which expands to a block;
 *		this module includes a call which is the sole statement on
 *		the "then" of an if statement, and the trailing ";" (after a
 *		"}") breaks the "else" clause.  Fix the reference in such a
 *		way that some future conversion back to a routine call won't
 *		break it again.
 *	006	Dave Butenhof	19 October 1989
 *		Use new type-specific handle validation macros.
 *	007	Dave Butenhof	19 October 1989
 *		Substitute "cma_t_address" for explicit "void *" to make
 *		porting easier.
 *	008	Dave Butenhof	19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *      009     Webb Scales     19 October 1989
 *              Add type-casts where MIPS pcc requires them.
 *	010	Dave Butenhof	24 October 1989
 *		Enhance free & destroy to handle attr. obj. delete-pending
 *		and refcnt.
 *	011	Dave Butenhof	24 October 1989
 *		Implement the timer-oriented services (cma_get_expiration,
 *		cma_expired, and cma_delay).
 *	012	Dave Butenhof	27 October 1989
 *		Hook up waits to handle alerts...
 *	013	Dave Butenhof	All Saints Day 1989
 *		Include stack header file to get cma__get_self_tcb.
 *	014	Dave Butenhof	1 November 1989
 *		Put all conditions on a queue.
 *	015	Dave Butenhof	16 November 1989
 *		- Change time parameter types from "cma__t_ticks" (obsolete) to
 *		  cma_t_date_time.
 *		- Finish timed service implementation.
 *	016	Dave Butenhof	22 November 1989
 *		decrement_semaphore now requires caller to have kernel
 *		locked, and to pass TCB.  This allows moving the
 *		condition-specific checks for alert-pending from semaphore to
 *		here, where they belong!
 *	017	Dave Butenhof	30 November 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	018	Dave Butenhof	5 December 1989
 *		Reverse arguments to cma_time_get_expiration to match name's
 *		claim of "time" as primary "object".
 *	019	Dave Butenhof	11 December 1989
 *		The "disable" bit should be ignored for wait, since it
 *		controls *asynchronous* alerts only!
 *	020	Dave Butenhof & Webb Scales	14 December 1989
 *		cma_cond_signal should increment if semaphore is <= 0 to
 *		generate a "pending spurious wakeup" to catch late waiters.
 *	021	Dave Butenhof	12 February 1990
 *		Simplify cma_delay somewhat by using a local static mutex
 *		and CV, instead of dynamically creating and then deleting a
 *		scratch mutex and condition.
 *	022	Dave Butenhof	9 April 1990
 *		Use new "known_object" structure for known condition queue
 *		(includes mutex).
 *	023	Dave Butenhof	10 April 1990
 *		Catch memory errors over object allocation, and set names in
 *		internal objects.
 *	024	Dave Butenhof	23 May 1990
 *		Make delay mutex & cv extern for pthread delay
 *	025	Webb Scales	30 May 1990
 *		Put FINALLY clause in for cma_delay.
 *	026	Dave Butenhof	5 June 1990
 *		Support cache high water mark.
 *	027	Dave Butenhof	7 June 1990
 *		Erase destroyed objects if not NDEBUG, to catch references to
 *		dead objects (can be used in conjunction with cma_maxcond=0,
 *		which disables caching).
 *	028	Dave Butenhof	18 June 1990
 *		Use macros to clear object name (only defined for debug
 *		build).
 *	029	Dave Butenhof	03 August 1990
 *		Use new semaphore primitives (simplified and inlined).
 *	030	Dave Butenhof	08 August 1990
 *		Post pending wake on semaphore if no waiters, with single
 *		interlocked instruction.
 *	031	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	032	Webb Scales	30 October 1990
 *		Removed uses of external handle fields.
 *	033	Webb Scales	30 October 1990
 *		Added casts to change #32.
 *	034	Dave Butenhof	26 November 1990
 *		Fix a bug in signal; it set cv->event even though a signal
 *		may leave other waiters. Signal must declare that there may
 *		still be waiters.
 *	035	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	036	Dave Butenhof	1 February 1991
 *		Improve inlining of internal cv operations; and change args
 *		in int_wait to conform to external interface for consistency.
 *	037	Dave Butenhof	7 February 1991
 *		Changes to alert control bit names.
 *	038	Dave Butenhof	02 April 1991
 *		Remove pending wakeup when there are definitely no waiters.
 *	039	Dave Butenhof	09 April 1991
 *		Use new constants to init atomic bits
 *	040	Paul Curtin	10 May 1991
 *		Put a number of new macros to use.
 *	041	Paul Curtin	24 May 1991
 *		Added a cma__reinit_cv routine.
 *	042	Paul Curtin	31 May 1991
 *		Added code to clear cv waiter queues in reinit routine.
 *	043	Dave Butenhof	10 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	044	Dave Butenhof	05 June 1992
 *		Fix delete_pending attr handling in free code -- it needs to
 *		free the attr. obj. instead of destroying it (which also
 *		fixes a race in attr. handling).
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_alert.h>
#include <cma_attr.h>
#include <cma_mutex.h>
#include <cma_condition.h>
#include <cma_handle.h>
#include <cma_vm.h>
#include <cma_sequence.h>
#include <cma_semaphore.h>
#include <cma_stack.h>
#include <cma_errors.h>
#include <cma_timer.h>
#include <cma_dispatch.h>
#include <cma_deb_core.h>
#include <cma_init.h>
#include <cma_int_errno.h>

/*
 * GLOBAL DATA
 */

cma__t_int_mutex	*cma__g_delay_mutex;
cma__t_int_cv		*cma__g_delay_cv;

/*
 * LOCAL MACROS
 */

/*
 * LOCAL DATA
 */

static cma__t_sequence	cma___g_cv_seq;

/*
 * LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Broadcast a condition variable
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Condition variable object to broadcast
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
cma_cond_broadcast(
	cma_t_cond	*condition)	/* Condition to broadcast */
    {
    cma__t_int_cv	*int_cv;


#ifdef NDEBUG
    int_cv = (cma__t_int_cv *)((cma__t_int_handle *)condition)->pointer;
#else
    int_cv = cma__validate_cv (condition);
#endif

    /*
     * Unblock all waiters
     */
    if (!cma__test_and_set (&int_cv->event)) {
	cma__sem_wake_all(&int_cv->semaphore);
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Create a new condition variable object.
 *
 *  FORMAL PARAMETERS:
 *
 *	new_condition	Output handle
 *
 *	att		Attributes object to use in creation
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
cma_cond_create(
	cma_t_cond	*new_condition,	/* New handle to fill in */
	cma_t_attr	*att)		/* Old attr obj to use */
    {
    cma__t_int_cv               *cv; 
    cma__t_int_attr             *int_att; 


    (int_att) = cma__validate_default_attr (att); 
    (cv) = cma__get_cv (int_att); 

    if ((cma_t_address)cv == cma_c_null_ptr)
	cma__error (exc_s_insfmem);
    else
	cma__object_to_handle ((cma__t_object *)cv, new_condition); 

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delay for some amount of time.
 *
 *  FORMAL PARAMETERS:
 *
 *	interval	length of time to wait
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
cma_delay(
	cma_t_interval	interval)
    {
    cma_t_date_time	expiration;


    cma_time_get_expiration (&expiration, interval);
    cma__int_lock (cma__g_delay_mutex);	/* Lock the mutex */

    /*
     * Wait until the specified time.  There will never be a SIGNAL or
     * BROADCAST on this condition variable, so the only other way it can
     * awaken is if it's alerted.  We need to handle a possible alert,
     * release the TCB's mutex, and resignal the alert for the client to
     * handle.
     */
    TRY {
	while (cma__int_timed_wait (
		    cma__g_delay_cv,
		    cma__g_delay_mutex,
		    &expiration) != cma_s_timed_out);
	}
    FINALLY {
	cma__int_unlock (cma__g_delay_mutex);
	}
    ENDTRY

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Delete (free) a condition variable object
 *
 *  FORMAL PARAMETERS:
 *
 *	condition	Condition variable object to free
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
cma_cond_delete(
	cma_t_cond	*condition)	/* Condition variable to free */
    {
    cma__int_cond_delete (condition);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Return a cma_t_date_time for some specified interval in the future (to
 *	be used as an expiration time).
 *
 *  FORMAL PARAMETERS:
 *
 *	interval	The number of seconds (float) in the future.
 *
 *	expiration	Address of cma_t_date_time structure for return.
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
cma_time_get_expiration(
	cma_t_date_time	*expiration,
	cma_t_interval	interval)
    {
    cma_t_date_time	delta, now;


    cma__get_time (&now);
    cma__interval_to_time (interval, &delta);
    cma__add_time (expiration, &now, &delta);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition variable
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Condition variable object to signal
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
cma_cond_signal(
	cma_t_cond	*condition)	/* Condition to signal */
    {
    cma__t_int_cv	*int_cv;


#ifdef NDEBUG
    int_cv = (cma__t_int_cv *)((cma__t_int_handle *)condition)->pointer;
#else
    int_cv = cma__validate_cv (condition);
#endif

    /*
     * Unblock one waiter
     */
    if (!cma__test_and_set (&int_cv->event)) {
	cma__unset (&int_cv->event);	/* May still be other waiters! */
	cma__sem_wake_one (&int_cv->semaphore);
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a condition variable, with a timeout in case of a long wait.
 *
 *  FORMAL PARAMETERS:
 *
 *	condition	Condition variable object to wait on.
 *
 *	mutex		Mutex to release during wait.
 *
 *	expiration	Time at which wait should expire
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
extern cma_t_status
cma_cond_timed_wait(
	cma_t_cond	*condition,	/* Condition variable to wait on */
	cma_t_mutex	*mutex,		/* Mutex to unlock during wait */
	cma_t_date_time	*expiration)	/* Timeout */
    {
    cma__t_int_mutex	*int_mutex;
    cma__t_int_cv	*int_cv;
    cma_t_status	return_value;


    int_mutex = cma__validate_mutex (mutex);
    int_cv = cma__validate_cv (condition);
    cma__timed_wait (int_cv, int_mutex, expiration, return_value);
    return return_value;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a condition variable
 *
 *  FORMAL PARAMETERS:
 *
 *	condition	Condition variable object to wait on.
 *
 *	mutex		Mutex to release during wait.
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
cma_cond_wait(
	cma_t_cond	*condition,	/* Condition variable to wait on */
	cma_t_mutex	*mutex)		/* Mutex to unlock during wait */
    {
    cma__int_cond_wait (condition, mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Find out whether a condition variable has waiters.
 *
 *  FORMAL PARAMETERS:
 *
 *	condition	Condition object to test
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
extern cma_t_boolean
cma__cv_waiting(
	cma_t_cond	*condition)	/* Condition to test */
    {
    cma__t_int_cv	*int_cv;


    int_cv = cma__validate_cv (condition);

    return cma__sem_if_waiters (&int_cv->semaphore);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Deallocate a cv structure---don't try to cache it (this is
 *	used to remove a cv from a cache list!)
 *
 *  FORMAL PARAMETERS:
 *
 *	old_cv	Address of the condition object
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
cma__destroy_cv(
	cma__t_int_cv	*old_cv)	/* The cv obj to delete */
    {
    /*
     * Free all the objects which are contained in the condition obj, and
     * then deallocate the object's memory.  THIS ROUTINE ASSUMES THAT THE
     * PARENT ATTRIBUTES OBJECT IS LOCKED, AND THAT THE CALLER MANAGES THE
     * CACHE LINKS.
     */
    cma__assert_warn (
	    cma__int_mutex_locked (old_cv->attributes->mutex),
	    "cma__destroy_cv called without attributes object locked.");

    /*
     * Also assert that the condition being destroyed doesn't have waiters,
     * just to be safe...
     */
    cma__assert_warn (
	    !cma__int_cv_waiting (old_cv),
	    "cma__destroy_cv called for condition with waiters.");
    cma__free_mem ((cma_t_address)old_cv);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a condition object to the cache list
 *
 *  FORMAL PARAMETERS:
 *
 *	old_cv	Condition object to free
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
cma__free_cv(
	cma__t_int_cv	*old_cv)	/* Cv object to free */
    {
    cma__t_int_attr	*attrib;

    attrib = old_cv->attributes;
    cma__int_lock (attrib->mutex);
    cma__free_cv_nolock (old_cv);

    if ((attrib->refcnt == 0) && (attrib->delete_pending)) {
	cma__int_unlock (attrib->mutex);
	cma__free_attributes (attrib);
	}
    else
	cma__int_unlock (attrib->mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free a condition object to the cache list.  Assumes that the
 *	attributes object is already locked!
 *
 *  FORMAL PARAMETERS:
 *
 *	old_cv	Condition object to free
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
cma__free_cv_nolock(
	cma__t_int_cv	*old_cv)	/* Cv object to free */
    {
    cma__t_int_attr	*attrib;	/* Point to controlling attr obj */


    attrib = old_cv->attributes;

    /*
     * Assert that the attributes object is locked.
     */
    cma__assert_warn (
	    cma__int_mutex_locked (attrib->mutex),
	    "cma__free_cv_nolock called without attributes object locked.");

    /*
     * Assert that the condition being destroyed doesn't have waiters, just
     * to be safe...
     */
    cma__assert_warn (
	    !cma__int_cv_waiting (old_cv),
	    "cma__free_cv called for condition with waiters.");

    /*
     * Add to the cached attributes list
     */
    old_cv->header.sequence = 0;
    attrib->refcnt--;
    cma__int_lock (cma__g_known_cvs.mutex);
    (void)cma__queue_remove (&old_cv->header.queue);
    cma__int_unlock (cma__g_known_cvs.mutex);

    if ((old_cv->header.revision == attrib->cache[cma__c_obj_cv].revision)
	    && (! attrib->delete_pending)) {

	if (attrib->cache[cma__c_obj_cv].count
		< cma__g_env[cma__c_env_maxcond].value) {
	    attrib->cache[cma__c_obj_cv].count += 1;
	    cma__queue_insert (
		    &old_cv->header.queue,
		    &attrib->cache[cma__c_obj_cv].queue);
	    }
	else {
	    cma__destroy_cv (old_cv);

	    while ((attrib->cache[cma__c_obj_cv].count
		    > cma__g_env[cma__c_env_mincond].value)
		    && (! cma__queue_empty (
			    &attrib->cache[cma__c_obj_cv].queue))) {
		cma__t_int_cv	*item;

		item = (cma__t_int_cv *)cma__queue_dequeue (
			&attrib->cache[cma__c_obj_cv].queue);
		attrib->cache[cma__c_obj_cv].count -= 1;
		cma__destroy_cv (item);
		}

	    }

	}
    else
	cma__destroy_cv (old_cv);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Allocate a condition object.
 *
 *  FORMAL PARAMETERS:
 *
 *	attrib		Attributes object to use
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
 *	Address of new condition object
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern cma__t_int_cv *
cma__get_cv(
	cma__t_int_attr	*attrib)	/* Attributes object to use */
    {
    cma_t_boolean	list_empty;	/* true if we need to create struct */
    cma__t_int_cv	*new_cv;	/* Pointer to new cv */


    cma__int_lock (attrib->mutex);

    if (attrib->delete_pending) {
	cma__int_unlock (attrib->mutex);
	return (cma__t_int_cv *)cma_c_null_ptr;
	}

    if (cma__queue_empty (&attrib->cache[cma__c_obj_cv].queue))
	list_empty = cma_c_true;
    else {
	new_cv = (cma__t_int_cv *)cma__queue_dequeue (
		&attrib->cache[cma__c_obj_cv].queue);
	attrib->cache[cma__c_obj_cv].count -= 1;
	list_empty = cma_c_false;
	}

    attrib->refcnt++;

    if (list_empty) {
	/*
	 * If we need to create a fresh cv structure, do so. Initialize
	 * the appropriate fields.
	 */
	new_cv = cma__alloc_object (cma__t_int_cv);

	if ((cma_t_address)new_cv == cma_c_null_ptr) {
	    cma__int_unlock (attrib->mutex);
	    return (cma__t_int_cv *)cma_c_null_ptr;
	    }

	new_cv->header.type = cma__c_obj_cv;
	new_cv->attributes = attrib;
	}

    /*
     * Whether the cv is old or new, we need to ensure certain field
     * initialization. The most important step is initializing the semaphore
     * which is used for waiting.
     */
    new_cv->header.sequence = cma__assign_sequence (&cma___g_cv_seq);
    new_cv->header.revision = attrib->cache[cma__c_obj_cv].revision;
    cma__obj_clear_name (new_cv);
    cma__sem_init (&new_cv->semaphore);
    cma__tac_set (&new_cv->event);	/* no waiters yet */
    cma__int_lock (cma__g_known_cvs.mutex);
    cma__queue_insert (&new_cv->header.queue, &cma__g_known_cvs.queue);
    cma__int_unlock (cma__g_known_cvs.mutex);
    cma__int_unlock (attrib->mutex);

    return new_cv;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize CMA_CONDITION.C local data
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
 *	initialize static data
 */
extern void
cma__init_cv(void)
    {
    cma__init_sequence (&cma___g_cv_seq);
    cma__obj_set_name (cma___g_cv_seq.mutex, "cond sequence object");
    cma__g_known_cvs.mutex	= cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_known_cvs.mutex, "known cond list");
    cma__g_delay_mutex		= cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma__g_delay_mutex, "mutex for delay");
    cma__g_delay_cv		= cma__get_cv (&cma__g_def_attr);
    cma__obj_set_name (cma__g_delay_cv, "cond for delay");
    }
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
extern void
cma__int_cv_broadcast(
	cma__t_int_cv		*condition)
    {
    cma__sem_wake_all (&condition->semaphore);
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
extern void
cma__int_cv_signal(
	cma__t_int_cv		*condition)
    {
    cma__sem_wake_one (&condition->semaphore);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a condition variable with timeout (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Pointer to mutex to release during wait.
 *
 *	condition	Pointer to condition variable object to wait on.
 *
 *	ticks		Pointer to ticks structure for wait time.
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
extern cma_t_status
cma__int_timed_wait(
	cma__t_int_cv		*condition,
	cma__t_int_mutex	*mutex,
	cma_t_date_time		*timeout)
    {
    cma_t_status	return_value;

    cma__timed_wait (condition, mutex, timeout, return_value);
    return return_value;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Wait on a condition variable (internal)
 *
 *  FORMAL PARAMETERS:
 *
 *	mutex		Pointer to mutex to release during wait.
 *
 *	condition	Pointer to condition variable object to wait on.
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
cma__int_wait(
	cma__t_int_cv		*condition,
	cma__t_int_mutex	*mutex)
    {
    cma__wait (condition, mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      Performs pre- or post- `fork() reinitialization' work.
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
 *	initialize static data
 */
extern void
cma__reinit_cv(
	cma_t_integer	flag)
    {
    cma__t_queue        *ptr;
    cma__t_int_cv	*cv;

    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock(cma__g_known_cvs.mutex);
	cma__int_lock(cma__g_delay_mutex);
	}
    else if (flag == cma__c_reinit_postfork_unlock) {
	cma__int_unlock(cma__g_delay_mutex);
	cma__int_unlock(cma__g_known_cvs.mutex);
	}
    else if (flag == cma__c_reinit_postfork_clear) {
	    ptr = &cma__g_known_cvs.queue;
	    while ((ptr = cma__queue_next (ptr)) != &cma__g_known_cvs.queue) {
		cv = (cma__t_int_cv *)ptr;
		cma__queue_init (&cv->semaphore.queue);
		}
	    }                                                     	

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__singlethreaded_delay()
 *
 *      Timed delay routine for single threaded applications.
 *
 *  FORMAL PARAMETERS:
 *
 *      timeout  - absolute time value
 *
 *  IMPLICIT INPUTS:
 *
 *      None
 *
 *  IMPLICIT OUTPUTS:
 *
 *      None
 *
 *  FUNCTION VALUE:
 *
 *      None
 *
 *  SIDE EFFECTS:
 *
 *      None
 */

extern void
cma__singlethreaded_delay (cma_t_date_time  *timeout)
    {
    cma_t_date_time    delta, current;
    cma_t_integer      result;

    while (cma_c_true) {
        cma__get_time (&current);
        if (cma__time_lss (timeout, &current))
            break;
        cma__subtract_time (&delta, timeout, &current);
        if ((result = select (0, 0, 0, 0, &delta)) == 0)
            break;
        else if ((result == -1) && (errno != EINTR))
            cma__bugcheck("cma__singlethreaded_delay: unexpected return value from select");
	}
    }



/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CONDITION.C */
/*  *12    5-JUN-1992 13:34:48 BUTENHOF "Fix delete_pending attr handling" */
/*  *11   18-FEB-1992 15:27:36 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *10   10-JUN-1991 18:17:32 SCALES "Add sccs headers for Ultrix" */
/*  *9     5-JUN-1991 16:11:36 CURTIN "fork work" */
/*  *8    24-MAY-1991 16:43:34 CURTIN "Added a new reinit routine" */
/*  *7    10-MAY-1991 11:56:09 CURTIN "Added a number of new macros" */
/*  *6    12-APR-1991 23:34:50 BUTENHOF "Init atomic bits using new constants" */
/*  *5     3-APR-1991 10:51:36 BUTENHOF "Remove pending wakeup" */
/*  *4    12-FEB-1991 01:28:39 BUTENHOF "Change to alert control bits" */
/*  *3     5-FEB-1991 00:59:27 BUTENHOF "Improve inlining of internal cv operations" */
/*  *2    14-DEC-1990 00:55:10 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:42:44 BUTENHOF "Condition variables" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CONDITION.C */
