/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_alert.c,v $
 * Revision 1.1.6.2  1996/02/17  23:33:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:52  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:59:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:04:28  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:37:15  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:17  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:19  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:15:36  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:25:46  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:35:41  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 *	Implement CMA alert services.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	14 September 1989
 *
 *  MODIFICATION HISTORY:
 * 	001	Bob Conti	6 October 1989
 *	    	Add in the code to raise cma_s_alerted. 	
 *	002	Dave Butenhof	12 October 1989
 *		Use internal mutex operations on tcb->mutex.
 *	003	Dave Butenhof	19 October 1989
 *		Use new type-specific handle validation macros.
 *	004	Webb Scales	19 October 1989
 *		Include cma_stack.h for cma__get_self_tcb() and type-cast
 *		cma_c_null_ptr
 *	005	Dave Butenhof	27 October 1989
 *		Make cma___attempt_delivery externally visible.
 *	006	Dave Butenhof	All Saints Day 1989
 *		Make use of cma__enter_kernel instead of manually whiling on
 *		test-and-set.
 *	007	Bob Conti	4 November 1989
 *		Remove superfluous include of cma_host
 *	008	Dave Butenhof	17 November 1989
 *		Include cma_condition.h explicitly, since cma_tcb.h no longer
 *		does it.
 *	009	Dave Butenhof	30 November 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	010	Dave Butenhof	5 December 1989
 *		Tweak some names.
 *	011	Dave Butenhof	11 December 1989
 *		Ignore "disable" bit for attempt_delivery: disable really
 *		controls asynchronous alerts, and we're dealing with the
 *		synchronous variety!
 *	012	Dave Butenhof	12 December 1989
 *		Don't enter kernel for cma_thread_alert.  It increments a
 *		semaphore... which will go into kernel mode.  The fact that
 *		we are already in kernel means the increment is deferred,
 *		which is unnecessary.
 *	013	Dave Butenhof	14 December 1989
 *		Include cma_semaphore.h (nobody noticed the omission before
 *		since it only called a function: now unblock_all_waiting has
 *		been made a macro).
 *	014	Dave Butenhof	01 March 1990
 *		Early versions of the CMA architecture specified that
 *		cma_thread_alert should ignore a null handle.  This is wrong;
 *		the only operations which should do that are those which
 *		clear the handle (to make them idempotent).  The new version
 *		of the spec corrects this problem; so I'm fixing the code,
 *		too.
 *	015	Dave Butenhof	01 May 1990
 *		Fix error in implementation of the architected behavior of
 *		"cma___attempt_delivery".  It should unconditionally
 *		*disable* (asynch) delivery before raising the alert
 *		exception, to restore default operation. Instead, it *defers*
 *		(all) delivery.
 *	016	Dave Butenhof and Webb Scales	03 August 1990
 *		Change semaphore usage.
 *	017	Dave Butenhof	15 August 1990
 *		Add new function cma__asynch_delivery, used by assembler code
 *		to call cma__attempt_delivery with appropriate TCB (since
 *		the assembler code doesn't have access to the macros
 *		specifying how to get the current TCB).
 *	018	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	019	Dave Butenhof	7 February 1991
 *		Implement new rationalized alert control primitives, allowing
 *		scoped "general disable," "general enable," and "asynchronous
 *		disable."  Asynchronous enable must still be done inside an
 *		asynchronous disable scope, since it cannot reliably return a
 *		previous state.
 *	020	Dave Butenhof	20 February 1991
 *		Add explicit casts to pointer assignments.
 *	021	Dave Butenhof	11 April 1991
 *		For non-uniprocessor implementations, use VP interrupt to
 *		deliver async alert.
 *	022	Paul Curtin	 8 May 1991
 *		cma_alert_test now uses the cma__int_alert_test macro.
 *	023	Dave Butenhof	04 October 1991
 *		Correct decision on whether to use cma__vp_interrupt(); it's
 *		not just "!_CMA_UNIPROCESSOR_".
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_alert.h>
#include <cma_condition.h>
#include <cma_dispatch.h>
#include <cma_errors.h>
#include <cma_tcb.h>
#include <cma_handle.h>
#include <cma_mutex.h>
#include <cma_stack.h>
#include <cma_semaphore.h>
#include <cma_vp.h>

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Disable asynchronous alert delivery.
 *
 *  FORMAL PARAMETERS:
 *
 *	prior		Return previous state of deferral
 *
 *  IMPLICIT INPUTS:
 *
 *	TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Thread's alert state
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
cma_alert_disable_asynch(
	cma_t_alert_state	*ep)	/* Previous state */
    {
    cma__t_int_tcb	*tcb;		/* Current thread TCB */
    cma__t_int_alert	*prior = (cma__t_int_alert *)ep;


    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);
    *prior = tcb->alert;		/* Save current alert state */
    tcb->alert.count++;			/* Increment nesting count */
    tcb->alert.a_enable = cma_c_false;
    cma__int_unlock (tcb->mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Disable general alert processing until further notice (allow "atomic"
 * 	operations).
 *
 *  FORMAL PARAMETERS:
 *
 *	prior		Return previous state of general alertability
 *
 *  IMPLICIT INPUTS:
 *
 *	TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Thread's alert state
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
cma_alert_disable_general(
	cma_t_alert_state	*ep)	/* Previous state */
    {
    cma__t_int_tcb	*tcb;		/* Current thread TCB */
    cma__t_int_alert	*prior = (cma__t_int_alert *)ep;


    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);
    *prior = tcb->alert;		/* Save current alert state */
    tcb->alert.count++;			/* Increment nesting count */
    tcb->alert.g_enable = cma_c_false;
    cma__int_unlock (tcb->mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Enable asynchronous alerts.  This should be done within a section of
 *	code bracketed by cma_alert_disable_asynch and cma_alert_restore calls,
 *	since enable cannot return a prior state reliably (it must
 *	enable asynchronous delivery before it returns, so return value
 *	linkages may not be completed).
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Thread's alert state
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
cma_alert_enable_asynch(void)
    {
    cma__t_int_tcb	*tcb;		/* Current thread TCB */


    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);
    tcb->alert.a_enable = cma_c_true;
    cma__int_unlock (tcb->mutex);
    cma__attempt_delivery (tcb);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Enable general alert processing until further notice.
 *
 *  FORMAL PARAMETERS:
 *
 *	prior		Return previous state of general alertability
 *
 *  IMPLICIT INPUTS:
 *
 *	TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Thread's alert state
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
cma_alert_enable_general(
	cma_t_alert_state	*ep)	/* Previous state */
    {
    cma__t_int_tcb	*tcb;		/* Current thread TCB */
    cma__t_int_alert	*prior = (cma__t_int_alert *)ep;


    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);
    *prior = tcb->alert;		/* Save current alert state */
    tcb->alert.count++;			/* Increment nesting count */
    tcb->alert.g_enable = cma_c_true;
    cma__int_unlock (tcb->mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Restore previous state of alert delivery.
 *
 *  FORMAL PARAMETERS:
 *
 *	prior		Previous state of delivery
 *
 *  IMPLICIT INPUTS:
 *
 *	TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Thread's alert state
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
cma_alert_restore(
	cma_t_alert_state	*ep)	/* Previous state */
    {
    cma__t_int_tcb	*tcb;		/* Current thread TCB */
    cma__t_int_alert	*prior = (cma__t_int_alert *)ep;


    tcb = cma__get_self_tcb ();
    cma__int_lock (tcb->mutex);

    /*
     * Compare the nesting counts in the prior state and the current TCB.
     * There are three separate cases:
     *
     * 1) If the counts are equal, then this scope has already been restored;
     *    the call is ignored (this supports idempotency).
     * 2) If the prior count is lower than the TCB's count, then this is an
     *    outer scope; restore it and set the nesting count to the prior
     * 	  value (note that this allows intermediate scopes to be skipped).
     * 3) If the prior count is higher than the TCB's count, this is a
     *    nesting violation; raise the appropriate exception.
     */
    if (tcb->alert.count != prior->count) {

	if (prior->count < tcb->alert.count) {
	    tcb->alert.count = prior->count;
	    tcb->alert.g_enable = prior->g_enable;
	    tcb->alert.a_enable = prior->a_enable;
	    }
	else {
	    cma__int_unlock (tcb->mutex);
	    cma__error (cma_s_alert_nesting);
	    }

	}

    cma__int_unlock (tcb->mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Test whether alert is pending, and raise cma_s_alerted if so.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	TCB
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
cma_alert_test(void)
    {
    cma__int_alert_test ();
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Send an alert to another thread...
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		Thread handle for target thread
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	Target thread's alert state
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	May cause the target thread to become runnable and preempt the
 *	current thread.
 */
extern void
cma_thread_alert(
	cma_t_thread	*thread)	/* Target thread handle */
    {
    cma__t_int_tcb	*tcb;		/* Current thread TCB */


    tcb = cma__validate_tcb (thread);

    cma__int_lock (tcb->mutex);
    tcb->alert.pending = cma_c_true;	/* Mark that alert is pending */

#if _CMA_THREAD_IS_VP_
    if (tcb->alert.g_enable && tcb->alert.a_enable)
	cma__vp_interrupt (
		tcb->sched.processor->vp_id,
		(cma__t_vp_handler)cma__attempt_delivery,
		(cma_t_address)tcb);
#endif

    if (tcb->wait_cv != (cma__t_int_cv *)cma_c_null_ptr) {
	cma__int_unlock (tcb->mutex);	/* For efficiency, unlock before */
	cma__int_broadcast (tcb->wait_cv);	/* .. broadcasting */
	}
    else
	cma__int_unlock (tcb->mutex);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Used by macro code to attempt delivery of an alert to the current
 *	thread (the assem module doesn't have access to cma__get_self_tcb,
 *	which may be a C macro in UNIPROCESSOR builds of CMA).
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
cma__async_delivery(void)
    {
    cma__attempt_delivery (cma__get_self_tcb ());
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Attempt to deliver any pending alerts for the current thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb	Address of the TCB object.
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
cma__attempt_delivery(
	cma__t_int_tcb	*tcb)		/* TCB to check */
    {
    cma__int_lock (tcb->mutex);

    /*
     * Test whether alert is pending and we're allowed to raise it.
     *
     * NOTE: asynchronous alert delivery is disabled during alert delivery
     *
     * NOTE: the mutex is released before raising the alert, since access to
     * the TCB might be required by a handler and we don't want a deadlock...
     * the "else" clause of this "if" releases the mutex if alert delivery is
     * disabled or unnecessary.  Unstructured, but necessary.
     */
    if (tcb->alert.pending
	    && (tcb->alert.g_enable)) {
	tcb->alert.pending = cma_c_false;
	tcb->alert.a_enable = cma_c_false;
	cma__int_unlock (tcb->mutex);
	cma__error (cma_s_alerted);
	}
    else
	cma__int_unlock (tcb->mutex);

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ALERT.C */
/*  *8    14-OCT-1991 13:37:26 BUTENHOF "Refine/fix use of config symbols" */
/*  *7    10-JUN-1991 18:16:27 SCALES "Add sccs headers for Ultrix" */
/*  *6    29-MAY-1991 16:58:49 BUTENHOF "Fix call to cma__vp_interrupt()" */
/*  *5    10-MAY-1991 11:09:48 CURTIN "added a use of the cma__int_alert_test macro" */
/*  *4    12-APR-1991 23:34:42 BUTENHOF "Use VP interrupt for async alert if possible" */
/*  *3    20-FEB-1991 22:07:50 BUTENHOF "Add casts for pointer assignment" */
/*  *2    12-FEB-1991 01:28:28 BUTENHOF "New alert control primitives" */
/*  *1    12-DEC-1990 21:40:32 BUTENHOF "alerts" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_ALERT.C */
