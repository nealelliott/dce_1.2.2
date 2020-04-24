/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_defer.c,v $
 * Revision 1.1.15.1  1996/05/10  13:35:32  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:40 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:42 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/24  15:25  truitt  /main/HPDCE01/1]
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:36:43  ajayb]
 * 	 *
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:36:42  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/10/18  14:38:07  prepetit]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:10:38  prepetit]
 *
 * 	merge kk and dce1_0_3
 * 	[1993/09/20  19:44:22  truitt]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:43:19  root  1.1.9.3]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  18:04:04  root]
 *
 * 	Added return 0 to quell compiler warning for function returning an int,
 * 	"cma__undefer()", for CR 8417.
 * 	[1993/08/20  15:38:58  rhw]
 *
 * 	Gamera merge
 * 	[1993/03/26  23:54:07  hopkins]
 *
 * 	Loading drop DCE1_0_2B18
 * 	[1993/03/11  22:26:41  osfrcs  1.1.1.8]
 *
 * 	Add new routines from dale tonogai
 * 	for the signal2 queue routines (called
 * 	only by cma__io_available).
 * 	Make cma__undefer process the signal2
 * 	queue.
 * 	[1992/12/07  21:59:27  hopkins]
 *
 * Revision 1.1.11.2  1994/06/09  13:37:48  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:44  devsrc]
 * 
 * Revision 1.1.11.1  1994/04/01  20:16:05  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:17:05  jd]
 * 
 * Revision 1.1.9.1  1993/10/20  21:30:27  jd
 * 	(5151) Drop from HP.
 * 	[1993/10/20  21:01:25  jd]
 * 
 * Revision 1.1.6.2  1993/08/20  20:47:36  rhw
 * 	Added return 0 to quell compiler warning for function returning and int,/
 * 	"cma__undefer()", for CR 8417.
 * 	[1993/08/20  15:38:58  rhw]
 * 
 * Revision 1.1.3.4  1992/12/15  22:27:39  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:37:49  alan]
 * 
 * Revision 1.1.3.3  1992/10/06  00:45:58  bolinger
 * 	For OT defect 5389:  add assert_warn calls before three calls
 * 	to cma__error() that are particularly likely to occur with the
 * 	CMA kernel locked.  This doesn't fix anything, of course, but
 * 	it may shed light on the point from which cma__error() is
 * 	called with the kernel locked (which it considers an error).
 * 	[1992/10/06  00:45:29  bolinger]
 * 
 * Revision 1.1.3.2  1992/09/03  14:36:27  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:02:43  bolinger]
 * 
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  20:53:25  bolinger]
 * 
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:12:10  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1990, 1992 by
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
 *	These routines allow actions which cannot be performed immediately
 *	to be deferred until a time when they can be performed immediately.
 *
 *  AUTHOR:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	25 July 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	03 August 1990
 *		Update semaphore deferral to new style, and fix signal
 *		deferral accordingly.
 *	002	Dave Butenhof	27 August 1990
 *		Change name of Webb's "interrupt level signal" to
 *		cma_cond_signal
 *		cma_cond_signal_interrupt (since he put the shorter form into
 *		the transfer vector, and has used it in cmalib).
 *	003	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	004	Webb Scales	 2 October 1990
 *		Removed trailing characters from endifs
 *	005	Webb Scales	 4 December 1990
 *		Added new parameter to io_available.
 *	006	Dave Butenhof	4 February 1991
 *		Rearrange signal_int deferral a little, and add internal
 *		interface.
 *	007	Dave Butenhof	6 February 1991
 *		On VAX/VMS, cma_cond_signal_int and cma__int_signal_int will
 *		now use the $wake kernel service to interrupt the null
 *		thread's hibernate loop, which will cause it to yield.  This
 *		attempts to fix the latency in processing deferred signals
 *		which can occur if no other threads are doing things that
 *		cause CMA to exit from the kernel (and process deferrals).
 *	008	Dave Butenhof	09 April 1991
 *		Use new type for "atomic bit" operation target
 *	009	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	010	Dave Butenhof	02 May 1991
 *		Cut down (dramatically) on the number of cma__alloc_mem()
 *		calls during initialization by allocating the entire set of
 *		defer queue entries for the "augment_queue" functions with a
 *		single call and then chopping it up.
 *	011	Dave Butenhof	14 May 1991
 *		Convert to dynamic init of atomic bit types.
 *	012	Dave Butenhof	30 May 1991
 *		Conditionalize out io_avail defer type for one-to-one kernel
 *		thread implementations (where i/o is already thread
 *		synchronous).
 *	013	Paul Curtin	 4 June 1991
 *		Added a fork() reinit routine.
 *	014	Webb Scales and Dave Butenhof	    10 June 1991
 *		Conditionalize inclusion of I/O stuff.
 *	015	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	016	Dave Butenhof	10 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	017	Webb Scales	10 June 1992
 *		Triple the size of the CV-signal-from-interrupt queue.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_defer.h>
#include <cma_handle.h>
#include <cma_tcb_defs.h>
#include <cma_timer.h>
#include <cma_semaphore.h>
#if (_CMA_OS_ == _CMA__UNIX) && !_CMA_THREAD_SYNC_IO_
# include <cma_thread_io.h>
#endif
#include <cma_vm.h>
#include <cma_condition.h>


/*
 * GLOBAL DATA
 */

/*
 * Clear when defers are pending
 */
cma__t_atomic_bit cma__g_defers_processed;

#ifndef _HP_CMA_PERF_
extern int      cma__ck_io_at_yield;
#endif /* _HP_CMA_PERF_ */

/*
 * LOCAL DATA
 */

#define cma___c_defer_queue_size    300

typedef struct CMA___T_DEFER_Q_ENTRY {
    struct CMA___T_DEFER_Q_ENTRY *next;
    cma_t_integer   type;
    cma_t_address   value1;
    cma_t_address   value2;
    } cma___t_defer_q_entry;

static cma___t_defer_q_entry
    *cma___g_defer_q_insert = (cma___t_defer_q_entry *)cma_c_null_ptr,
    *cma___g_defer_q_remove = (cma___t_defer_q_entry *)cma_c_null_ptr;


#define cma___c_signal_queue_size    1000

typedef struct CMA___T_SIGNAL_Q_ENTRY {
    struct CMA___T_SIGNAL_Q_ENTRY *next;
    cma__t_int_cv	*cv;
    cma_t_integer	seq;
    } cma___t_signal_q_entry;

static cma___t_signal_q_entry
    *cma___g_signal_q_insert = (cma___t_signal_q_entry *)cma_c_null_ptr,
    *cma___g_signal_q_remove = (cma___t_signal_q_entry *)cma_c_null_ptr;

/*
 * Separate queue needed for cma__io_available which uses the signal
 * deferral queue but may be called at interrupt and non-interrupt
 * time.
 */
static cma___t_signal_q_entry
    *cma___g_signal2_q_insert = (cma___t_signal_q_entry *)cma_c_null_ptr,
    *cma___g_signal2_q_remove = (cma___t_signal_q_entry *)cma_c_null_ptr;

/*
 * LOCAL MACROS
 */


/*
 * LOCAL FUNCTIONS
 */

void cma___augment_defer_queue  (cma_t_natural);

void cma___augment_signal_queue  (cma_t_natural);

void cma___augment_signal2_queue  (cma_t_natural);


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition variable from an interrupt routine
 *
 *	ASSUMPTIONS:
 *	    1) This routine is only called by an AST or Signal.
 *	    2) Only one thread of execution is active at a time.
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Condition variable object to signal
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_signal_q_insert  -  points to "beginning" of signal queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	An entry in the signal queue
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Note:  This routine does not actually signal the indicated condition 
 *		variable.  Instead it causes it to be signalled at the next 
 *		time a thread leaves the CMA kernel.
 */
extern void
cma_cond_signal_int(
	cma_t_cond	*condition)	/* Condition to signal */
    {
    cma__t_int_cv	*int_cv;


    /*
     * Validate the condition variable.
     */
    int_cv = cma__validate_cv (condition);

    /*
     * Advance pointer to next entry in the (circular) signal deferral queue.
     * Make sure that the entry is free.
     * Copy the information into the entry.
     */
     
    cma___g_signal_q_insert = cma___g_signal_q_insert->next;

    cma__assert_warn (cma___g_signal_q_insert != cma___g_signal_q_remove,
		"cma_cond_signal_int: signal queue full");
    if (cma___g_signal_q_insert == cma___g_signal_q_remove)
	cma__error (cma_s_signal_q_full);

    cma___g_signal_q_insert->cv = int_cv;
    cma___g_signal_q_insert->seq = int_cv->header.sequence;

    /*
     * Clear the flag that says all defers have been processed
     * so that this one will get noticed.
     */
    cma__unset (&cma__g_defers_processed);

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    /*
     * Try to minimize the latency in processing the deferred signal.  On
     * VAX/VMS, the null thread (if currently running) spins in a tight loop
     * hibernating. When awakened, it'll try to yield (which undefers the
     * signals).
     */
    sys$wake (0, 0);
#endif
    }	

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Defer a kernel action
 *
 *	ASSUMPTIONS:
 *	    1) This routine is only called by an AST or Signal.
 *	    2) Only one thread of execution is active at a time.
 *
 *  FORMAL PARAMETERS:
 *
 *	type    -  type of action being deferred
 *	value1  -  first argument required by the action
 *	value2  -  second argument required by the action
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_defer_q_insert  -  points to "beginning" of defer queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	An entry in the defer queue
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
cma__defer(
	cma_t_integer	type,
	cma_t_address	value1,
	cma_t_address	value2)
    {

    /*
     * Advance pointer to next entry in the (circular) deferral queue.
     * Make sure that the entry is free.
     * Copy the information into the entry.
     */
     
    cma___g_defer_q_insert = cma___g_defer_q_insert->next;

    cma__assert_warn (cma___g_defer_q_insert != cma___g_defer_q_remove,
		"cma__defer: defer queue full");
    if (cma___g_defer_q_insert == cma___g_defer_q_remove)
	cma__error (cma_s_defer_q_full);

    cma___g_defer_q_insert->type = type;
    cma___g_defer_q_insert->value1 = value1;
    cma___g_defer_q_insert->value2 = value2;

    /*
     * Clear the flag that says all defers have been processed
     * so that this one will get noticed.
     */
    cma__unset (&cma__g_defers_processed);
    }	

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	initialize the defer mechanisms
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
 *	none
 * 
 */
void
cma__init_defer(void)
    {
    cma__tac_set (&cma__g_defers_processed);
    cma___augment_defer_queue  (cma___c_defer_queue_size);
    cma___augment_signal_queue (cma___c_signal_queue_size);
    cma___augment_signal2_queue (cma___c_signal_queue_size);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition variable from an interrupt routine (internal)
 *
 *	ASSUMPTIONS:
 *	    1) This routine is only called by an AST or Signal.
 *	    2) Only one thread of execution is active at a time.
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Address of CV object to signal
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_signal_q_insert  -  points to "beginning" of signal queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	An entry in the signal queue
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Note:  This routine does not actually signal the indicated condition 
 *		variable.  Instead it causes it to be signalled at the next 
 *		time a thread leaves the CMA kernel.
 */
extern void
cma__int_signal_int(
	cma__t_int_cv	*int_cv)	/* Condition to signal */
    {
    /*
     * Advance pointer to next entry in the (circular) signal deferral queue.
     * Make sure that the entry is free.
     * Copy the information into the entry.
     */
     
    cma___g_signal_q_insert = cma___g_signal_q_insert->next;

    cma__assert_warn (cma___g_signal_q_insert != cma___g_signal_q_remove,
		"cma__int_signal_int: signal queue full");
    if (cma___g_signal_q_insert == cma___g_signal_q_remove)
	cma__error (cma_s_signal_q_full);

    cma___g_signal_q_insert->cv = int_cv;
    cma___g_signal_q_insert->seq = int_cv->header.sequence;

    /*
     * Clear the flag that says all defers have been processed
     * so that this one will get noticed.
     */
    cma__unset (&cma__g_defers_processed);

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    /*
     * Try to minimize the latency in processing the deferred signal.  On
     * VAX/VMS, the null thread (if currently running) spins in a tight loop
     * hibernating. When awakened, it'll try to yield (which undefers the
     * signals).
     */
    sys$wake (0, 0);
#endif
    }	

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Signal a condition variable from an interrupt routine (internal)
 *
 *	ASSUMPTIONS:
 *	    1) This routine is only called by cma__io_available().
 *
 *  FORMAL PARAMETERS:
 *
 *	condition		Address of CV object to signal
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_signal2_q_insert  -  points to "beginning" of signal queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	An entry in the signal2 queue
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	Note:  This routine does not actually signal the indicated condition 
 *		variable.  Instead it causes it to be signalled at the next 
 *		time a thread leaves the CMA kernel.
 */
extern void
cma__int_signal2_int(
	cma__t_int_cv	*int_cv)	/* Condition to signal */
    {
    /*
     * Advance pointer to next entry in the (circular) signal deferral queue.
     * Make sure that the entry is free.
     * Copy the information into the entry.
     */
     
    cma___g_signal2_q_insert = cma___g_signal2_q_insert->next;

    cma__assert_warn (cma___g_signal2_q_insert != cma___g_signal2_q_remove,
		"cma__int_signal2_int: signal2 queue full");
    if (cma___g_signal2_q_insert == cma___g_signal2_q_remove)
	cma__error (cma_s_signal_q_full);

    cma___g_signal2_q_insert->cv = int_cv;
    cma___g_signal2_q_insert->seq = int_cv->header.sequence;

    /*
     * Clear the flag that says all defers have been processed
     * so that this one will get noticed.
     */
    cma__unset (&cma__g_defers_processed);

#if _CMA_PLATFORM_ == _CMA__VAX_VMS
    /*
     * Try to minimize the latency in processing the deferred signal.  On
     * VAX/VMS, the null thread (if currently running) spins in a tight loop
     * hibernating. When awakened, it'll try to yield (which undefers the
     * signals).
     */
    sys$wake (0, 0);
#endif
    }	

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Perform fork() reinitialization work.
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
 * 
 */
void
cma__reinit_defer(
	cma_t_integer	    flag)
    {

    if (flag == cma__c_reinit_postfork_clear) {
        cma__tac_set (&cma__g_defers_processed);
	cma___g_defer_q_remove = cma___g_defer_q_insert;
	cma___g_signal_q_remove = cma___g_signal_q_insert;
	cma___g_signal2_q_remove = cma___g_signal2_q_insert;
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Perform deferred kernel actions
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	The deferred action queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma___g_defer_q_remove is advanced to the "end" of the deferral queue
 *
 *  FUNCTION VALUE:
 *
 *	none (Returning cma_t_integer is only so this can be called in a macro)
 *
 *  SIDE EFFECTS:
 *
 *	This routine performs the deferred kernel actions (principally,
 *	unblocking threads).
 */
cma_t_integer
cma__undefer(void)
   {
   cma__assert_warn (
	    cma__tac_isset (&cma__g_kernel_critical),
	    "Undefer with kernel critical unlocked"
	    );

    /* 
     * Set the flag that is cleared if there are any pending defers
     * We do this because some callers might not use the optimization
     * of setting this flag before they call us.
     */
    (void) cma__kernel_set (&cma__g_defers_processed);

    while (cma___g_defer_q_remove != cma___g_defer_q_insert) {
	cma___g_defer_q_remove = cma___g_defer_q_remove->next;

	switch (cma___g_defer_q_remove->type) {
	    case cma__c_defer_wake_one:
		{
		cma__sem_iwake_one (
			(cma__t_semaphore *)(cma___g_defer_q_remove->value1));
		break;
		}
	    case cma__c_defer_wake_all:
		{
		cma__sem_iwake_all (
			(cma__t_semaphore *)(cma___g_defer_q_remove->value1));
		break;
		}
#if (_CMA_OS_ == _CMA__UNIX) && !_CMA_THREAD_IS_VP_
	    case cma__c_defer_io_avail:
		{
		static struct timeval polling_timeout = {0, 0};


		/*
		 * Since we are not interested in the result of this call, we 
		 * arbitrairly select read and file #0.  Poll (null timeout).
		 */
		cma__io_available (
			cma__c_io_read, 
			0, 
			&polling_timeout);
#ifndef _HP_CMA_PERF_
                cma__ck_io_at_yield = 0;
#endif /* _HP_CMA_PERF_ */
		break;
		}
#endif
	    default:
		{
		cma__bugcheck ("undefer:1");
		break;
		}

	    }

	}

    while (cma___g_signal_q_remove != cma___g_signal_q_insert) {
	cma__t_int_cv	*int_cv;

	cma___g_signal_q_remove = cma___g_signal_q_remove->next;

	int_cv = cma___g_signal_q_remove->cv;

	/*
	 * It's possible that this defer entry has been on the queue for some
	 * time.  It's conceivable that the condition variable has actually
	 * been deleted (and possibly reused) within that time.  Therefore,
	 * we have to check the validity.  We know that the pointer is valid,
	 * however, so validity checking can be simplified to ensuring that
	 * the pointer points to something which is a condition variable with
	 * the proper sequence number (which was stored in the defer entry).
	 *
	 * If the CV isn't valid, then we simply ignore the defer entry.
	 * It's illegal to delete a condition variable if any threads are
	 * waiting on it, and no threads can issue a wait on a deleted
	 * condition variable: therefore, even if it were possible to signal
	 * that deleted condition variable, the signal could never possibly
	 * have any effect on a thread (exactly as if the client signalled a
	 * condition variable on which no thread ever waited).  Since the
	 * signal can have no effect, failing to perform the signal could
	 * actually be classed as "an optimization".
	 */
	if (int_cv->header.sequence == cma___g_signal_q_remove->seq
		&& int_cv->header.type == cma__c_obj_cv)
	    cma__sem_iwake_one (&(int_cv->semaphore));

	}

    while (cma___g_signal2_q_remove != cma___g_signal2_q_insert) {
	cma__t_int_cv	*int_cv;

	cma___g_signal2_q_remove = cma___g_signal2_q_remove->next;

	int_cv = cma___g_signal2_q_remove->cv;

	if (int_cv->header.sequence == cma___g_signal2_q_remove->seq
		&& int_cv->header.type == cma__c_obj_cv)
	    cma__sem_iwake_one (&(int_cv->semaphore));

	}

    return 0;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	Augment defer queue.  This routine initializes or adds fresh
 *	new queue entries to the defer queue.
 *
 *  FORMAL PARAMETERS:
 *
 *	number  -  number of entries to add to the defer queue
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_defer_q_insert  -  used to find "start" of queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma___g_defer_q_insert  -  Initialized if null
 *	cma___g_defer_q_remove  -  Initialized if null
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
cma___augment_defer_queue(
	cma_t_natural	number)
    {
    cma_t_integer		i;
    cma_t_natural		count, size;
    cma___t_defer_q_entry	*dq_start, *dq_ptr;

   
    /*
     * Allocate a block large enough for the appropriate number of entries.
     * Or, if that fails, allocate as many as we can.
     */

    for (count = number; count > 0; count--) {
	size = (cma_t_integer)(sizeof (cma___t_defer_q_entry) * count);
	dq_ptr = (cma___t_defer_q_entry *)cma__alloc_mem (size);

	if ((cma_t_address)dq_ptr != cma_c_null_ptr)
	    break;

	}

    /*
     * Insert new entries after the node pointed to by the insertion
     * pointer.  Link them into the circular list.
     */
    dq_start = cma___g_defer_q_insert;
    
    for (i = 0; i < count; i++) {

	if (dq_start == (cma___t_defer_q_entry *)cma_c_null_ptr)
	    dq_start = dq_ptr;
	else {
	    dq_ptr->next = dq_start->next;
	    }

	dq_start->next = dq_ptr;
	dq_ptr++;			/* Advance to next block */
	}
	
    /*
     * If the defer queue is empty, then initialize both of its pointers.
     */
    if (cma___g_defer_q_insert == (cma___t_defer_q_entry *)cma_c_null_ptr) {
	cma___g_defer_q_insert = dq_start;
	cma___g_defer_q_remove = dq_start;
	}

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	Augment deferred signal queue.  This routine initializes or adds fresh
 *	new queue entries to the deferred signal queue.
 *
 *  FORMAL PARAMETERS:
 *
 *	number  -  number of entries to add to the deferred signal queue
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_signal_q_insert  -  used to find "start" of queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma___g_signal_q_insert  -  Initialized if null
 *	cma___g_signal_q_remove  -  Initialized if null
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
cma___augment_signal_queue(
	cma_t_natural	number)
    {
    cma_t_integer		i;
    cma_t_natural		size, count;
    cma___t_signal_q_entry	*dq_start, *dq_ptr;

   
    /*
     * Allocate a block large enough for the appropriate number of entries.
     * Or, if that fails, allocate as many as we can.
     */

    for (count = number; count > 0; count--) {
	size = (cma_t_integer)(sizeof (cma___t_signal_q_entry) * count);
	dq_ptr = (cma___t_signal_q_entry *)cma__alloc_mem (size);

	if ((cma_t_address)dq_ptr != cma_c_null_ptr)
	    break;

	}

    /*
     * Insert new entries after the node pointed to by the insertion
     * pointer.  Link them into the circular list.
     */
    dq_start = cma___g_signal_q_insert;
    
    for (i = 0; i < count; i++) {

	if (dq_start == (cma___t_signal_q_entry *)cma_c_null_ptr)
	    dq_start = dq_ptr;
	else {
	    dq_ptr->next = dq_start->next;
	    }

	dq_start->next = dq_ptr;
	dq_ptr++;			/* Advance to next block */
	}
	
    /*
     * If the signal queue is empty, then initialize both of its pointers.
     */
    if (cma___g_signal_q_insert == (cma___t_signal_q_entry *)cma_c_null_ptr) {
	cma___g_signal_q_insert = dq_start;
	cma___g_signal_q_remove = dq_start;
	}
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 * 	Augment deferred signal2 queue.  This routine initializes or adds fresh
 *	new queue entries to the deferred signal2 queue.
 *
 *  FORMAL PARAMETERS:
 *
 *	number  -  number of entries to add to the deferred signal queue
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_signal2_q_insert  -  used to find "start" of queue
 *
 *  IMPLICIT OUTPUTS:
 *
 *	cma___g_signal2_q_insert  -  Initialized if null
 *	cma___g_signal2_q_remove  -  Initialized if null
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
cma___augment_signal2_queue(
	cma_t_natural	number)
    {
    cma_t_integer		i;
    cma_t_natural		count, size;
    cma___t_signal_q_entry	*dq_start, *dq_ptr;

   
    /*
     * Allocate a block large enough for the appropriate number of entries.
     * Or, if that fails, allocate as many as we can.
     */
    for (count = number; count > 0; count--) {
	size = (cma_t_integer)(sizeof (cma___t_signal_q_entry) * count);
	dq_ptr = (cma___t_signal_q_entry *)cma__alloc_mem (size);

	if ((cma_t_address)dq_ptr != cma_c_null_ptr)
	    break;

	}

    /*
     * Insert new entries after the node pointed to by the insertion
     * pointer.  Link them into the circular list.
     */
    dq_start = cma___g_signal2_q_insert;
    
    for (i = 0; i < count; i++) {

	if (dq_start == (cma___t_signal_q_entry *)cma_c_null_ptr)
	    dq_start = dq_ptr;
	else {
	    dq_ptr->next = dq_start->next;
	    }

	dq_start->next = dq_ptr;
	dq_ptr++;			/* Advance to next block */
	}
	
    /*
     * If the signal2 queue is empty, then initialize both of its pointers.
     */
    if (cma___g_signal2_q_insert == (cma___t_signal_q_entry *)cma_c_null_ptr) {
	cma___g_signal2_q_insert = dq_start;
	cma___g_signal2_q_remove = dq_start;
	}
    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEFER.C */
/*  *15   10-JUN-1992 22:44:41 SCALES "Increase the size of the signal deferral queue" */
/*  *14   18-FEB-1992 15:28:33 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *13   14-OCT-1991 13:38:14 BUTENHOF "Refine/fix use of config symbols" */
/*  *12   10-JUN-1991 18:20:26 SCALES "Add sccs headers for Ultrix" */
/*  *11   10-JUN-1991 17:53:49 SCALES "Conditionalize inclusion of I/O stuff" */
/*  *10    5-JUN-1991 17:31:18 BUTENHOF "Add cast to alloc_mem()" */
/*  *9     5-JUN-1991 16:12:31 CURTIN "fork work" */
/*  *8    31-MAY-1991 13:59:31 BUTENHOF "Remove io_avail defer for k-thread impls" */
/*  *7    14-MAY-1991 16:18:17 BUTENHOF "Remove use of ""cma__c_tac_static_clear""" */
/*  *6     2-MAY-1991 16:46:01 BUTENHOF "Fix bug in previous change" */
/*  *5     2-MAY-1991 13:57:55 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *4    12-APR-1991 23:35:18 BUTENHOF "Change type of internal locks" */
/*  *3     6-FEB-1991 18:57:00 BUTENHOF "Improve response of signal_int operation" */
/*  *2     5-FEB-1991 00:59:37 BUTENHOF "Add internal interface to signal_int" */
/*  *1    12-DEC-1990 21:44:19 BUTENHOF "Defer events while kernel locked" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_DEFER.C */
