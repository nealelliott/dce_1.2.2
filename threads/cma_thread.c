/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_thread.c,v $
 * Revision 1.1.14.1  1996/05/10  13:42:59  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/30  21:20 UTC  bissen
 * 	Merge fix to cma_thread_set_sched
 *
 * 	HP revision /main/DCE_1.2/bissen_122_work/1  1996/04/30  21:17 UTC  bissen
 * 	Fix cma_thread_set_sched()
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/08  21:46 UTC  bissen
 * 	Merge in single-threaded client changes for scheduling and priorities.
 * 	[1995/03/24  18:55 UTC  jss  /main/HPDCE02/5]
 *
 * 	Single threaded client changes.  Modify cma_thread_set_sched() and
 * 	cma_thread_set_prio() so that they do not yield the processor in single-
 * 	threaded mode.
 * 	[1995/03/17  16:57 UTC  jss  /main/HPDCE02/jss_mothra_fix3/1]
 *
 * 	Merge callback fixes from branch jss_mothra_fix2.
 * 	[1995/03/14  22:33 UTC  jss  /main/HPDCE02/4]
 *
 * 	Fix the callback mechanism so that the end callback is not called until
 * 	the thread is in CMA kernel and just about ready to switch out for the
 * 	last time.
 * 	[1995/03/13  19:18 UTC  jss  /main/HPDCE02/jss_mothra_fix2/1]
 *
 * 	Merge from branch jss_mothra_fix.
 * 	[1995/01/20  23:38 UTC  jss  /main/HPDCE02/3]
 *
 * 	CHFts14092.  Fix exc_raise() so that a SIGPIPE signal that is not handled
 * 	does not abort the process with a core file and does not print out the
 * 	exception report.
 * 	[1995/01/20  23:17 UTC  jss  /main/HPDCE02/jss_mothra_fix/1]
 *
 * 	Merge from branch jss_single_thread to HPDCE02.
 * 	[1995/01/03  15:42 UTC  jss  /main/HPDCE02/2]
 *
 * 	Changes for single threaded client.  Added CMA initialization routines
 * 	to cma_thread_create().
 * 	[1995/01/03  15:40 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 *
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1994/07/06  16:44 UTC  jss  /main/HPDCE02/1]
 *
 * 	merge kk and hpdce01
 * 	[1994/01/25  14:27  truitt  /main/HPDCE01/1]
 *
 * 	Add 'start' and 'end' support for C++ callbacks.
 * 	[1993/11/09  18:56:53  truitt]
 * 	 *
 *
 * 	Change polarity of ifdef for including cma performance fixes
 * 	[1993/10/29  18:48:43  ajayb]
 *
 * 	Add proper ifdefs to include DEPT changes.
 * 	[1993/10/19  19:38:12  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/10/18  20:15:03  ajayb]
 *
 * 	Merging DEPT changes
 * 	[1993/09/30  15:08:02  gaz]
 *
 * 	Loading drop DCE1_0_3b03
 * 	[1993/09/15  17:44:36  root  1.1.3.3]
 *
 * Revision 1.1.10.2  1994/06/09  13:38:57  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:41  devsrc]
 * 
 * Revision 1.1.10.1  1994/04/01  20:17:30  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:19:05  jd]
 * 
 * Revision 1.1.6.4  1993/01/26  13:26:33  jd
 * 	Don't allow a thread to join itself.
 * 	[1993/01/21  21:49:13  jd]
 * 
 * Revision 1.1.6.3  1992/12/15  22:31:49  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:42:53  alan]
 * 
 * Revision 1.1.6.2  1992/09/03  14:41:18  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:03  bolinger]
 * 
 * Revision 1.1.2.3  1992/08/14  20:23:14  bolinger
 * 	Merge up to BL10+ internal release from DEC.  (In this case,
 * 	this is a no-op, since DEC had already given us the only
 * 	bug fix in this file that BL10+ contained.)
 * 	[1992/08/11  21:28:21  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.1.3.3 (fix for OT defect 4149).
 * 	[1992/08/10  16:33:39  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:05:30  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:05:14  bolinger]
 * 
 * Revision 1.1.3.4  1992/06/26  20:09:38  bolinger
 * 	Trivial fixes to comments.
 * 	[1992/06/26  20:09:16  bolinger]
 * 
 * Revision 1.1.3.3  1992/06/26  20:06:29  bolinger
 * 	Fix OT defect 4149:  eliminate race condition between cma_thread_detach()
 * 	and cma__terminate_thread(), in which a TCB could be recycled before
 * 	its previous occupant was really dead.
 * 	[1992/06/22  19:21:19  bolinger]
 * 
 * Revision 1.1.3.2  1992/04/29  14:17:20  keane
 * 	Remove unused arguments to cma__create_thread
 * 	[1992/04/29  13:50:37  keane]
 * 
 * Revision 1.1  1992/01/19  14:40:27  devrcs
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
 *	These routines provide the basic thread services.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	24 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *  	001	Webb Scales	26 September 1989
 *		Added the thread info routines
 *	002	Webb Scales and Dave Butenhof	27 September 1989
 *		Fix pointer booboo
 *	003	Dave Butenhof	29 September 1989
 *		cma_fork needs to set priority on new thread.
 *	004	Bob Conti	6 October 1989
 *		Add the call to cma__error to make exit work.
 *		Add assertion for cma_assem_vms.
 *	005	Dave Butenhof	16 October 1989
 *		Convert TCB mutex operations to internal form.
 *	006	Dave Butenhof	19 October 1989
 *		Use new type-specific handle validation macros.
 *	007	Dave Butenhof	19 October 1989
 *		Modify use of queue operations to use appropriate casts
 *		rather than depending on ANSI C "void *" (sigh).
 *	008	Webb Scales	20 October 1989
 *		Changed "cma_assem_vms.h" to "cma_assem.h"
 *	009	Dave Butenhof	Halloween 1989
 *		cma_detach needs to remove thread from "known threads" queue
 *		before freeing TCB.
 *	010	Webb Scales	3 November 1989
 *		cma_yield needs to enter the kernel before calling cma__dispatch
 *	011	Webb Scales & Bob Conti	    6 November 1989
 *		Renamed cma__dispatch to cma__yield_processor.  Added an 
 *		exit-kernel call to cma_yeild.
 *	012	Dave Butenhof	1 December 1989
 *		Modify external entries to track POSIX changes to names and
 *		argument ordering.
 *	013	Dave Butenhof	8 December 1989
 *		Signal cma_s_unimp when client attempts to change priority or
 *		scheduling policy.
 *	014	Dave Butenhof	12 December 1989
 *		Add cma__start_thread routine (called by cma__execute_thread
 *		from assembly code); this is responsible for setting up
 *		portable exception handlers, calling the client start
 *		routine, and cleaning up the thread.
 *	015	Dave Butenhof	14 December 1989
 *		- Now that we have exception handling, protect the condition
 *		  wait in cma_thread_join with a TRY/ENDTRY to catch
 *		  cma_e_alerted and unlock the TCB mutex.
 *		- Modify cma__thread_base to report an error on CATCH_ALL,
 *		  using new routine cma__report_error.
 *	016	Dave Butenhof & Bob Conti	15 December 1989
 *		Change cma__report_error to cma_exc_report.
 *	017	Dave Butenhof	15 February 1990
 *		Correct oversight; add priority argument to
 *		cma_thread_set_sched to conform to POSIX pthreads.
 *	018	Dave Butenhof	26 February 1990
 *		Fix set_sched and set_priority to deal with new symbols
 *		correctly.
 *	019	Dave Butenhof & Webb Scales	9 April 1990
 *		When creating a new thread, check for zombie threads.
 *	020	Webb Scales	30 May 1990
 *		Increment a reference count while joining with a thread.
 *		Also changed TRY to catch all exceptions during a join.
 *	021	Webb Scales	15 June 1990
 *		Added priority sceduling
 *		- Rearranged part of the tcb.
 *		- "Enabled" set_sched and added new policies.
 *		- "Enabled" set_prio and added new policies.
 *		- mutexed tcb reads for getting priority and scheduling policy.
 *	022	Paul Curtin	06 August 1990
 *		Replace abort w/ cma__abort.
 *	023	Webb Scales	16 August 1990
 *		Various clean-up stuff.
 *	024	Dave Butenhof	27 August 1990
 *		Change interfaces to pass handles & structures by reference.
 *	025	Bob Conti	10 October 1990
 *		Fix bug in set_priority: unlock before raising error.
 *	026	Dave Butenhof	14 December 1990
 *		Change cma_attributes.h to cma_attr.h (shorten length)
 *	027	Dave Butenhof	1 February 1991
 *		Correct order of args to cma__int_wait to conform to external
 *		interface (cv, mutex).
 *	028	Dave Butenhof	12 March 1991
 *		P1003.4a allows the "status" pointer passed in to
 *		pthread_join to be null, which is a nice convenience if you
 *		don't care about the return status. Make cma_thread_join
 *		equivalent by allowing either pointer to be null.
 *	029	Dave Butenhof	25 March 1991
 *		Change from cma_exception to exc_handling
 *	030	Dave Butenhof	05 April 1991
 *		Support Mach kernel threads (with one-to-one mapping)
 *	031	Paul Curtin	08 April 1991
 *		Adjust call to abort process to pass signal value.
 *	032	Paul Curtin	23 April 1991
 *		Added kernel lock around known thread list access.
 *	033	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	034	Webb Scales and Dave Butenhof	02 May 1991
 *		Fix set_priority and set_scheduler to make the dispatcher
 *		recalculate the thread's priority queue, and yield to allow
 *		appropriate threads a chance to take over.
 *	035	Dave Butenhof	23 May 1991
 *		Under OSF/1 systems, cma__thread_base should set up handling
 *		for the per-thread signals (by calling
 *		cma__sig_thread_init()).
 *	036	Dave Butenhof	29 May 1991
 *		Raise cma_e_unimp on attempts to modify thread
 *		priority or policy.
 *	037	Dave Butenhof	01 July 1991
 *		Improve VP caching for Mach threads by just suspending it
 *		inside cma__thread_base and letting it loop.
 *	038	Dave Butenhof	12 September 1991
 *		Integrate Nat Mishkin's performance improvements to make more
 *		intelligent use of the file descriptor numbers; ULTRIX V4.2
 *		allows 4096 files, but most systems are generated to support
 *		lower numbers, and DECthreads is currently carrying around
 *		all that extra baggage. With this change, it'll only check
 *		and use the ones actually supported on the system. Also,
 *		shrink the TCB by dynamically allocating the file descriptor
 *		mask at thread creation time.
 *	039	Dave Butenhof	06 December 1991
 *		For convenience (I've got no intention of documenting this
 *		for now, but it seems like a nice one-plus), save the status
 *		code for a status exception that terminates a thread.
 *	040	Dave Butenhof	10 February 1992
 *		Add cma__trace call on VP creation.
 *	041	Dave Butenhof	11 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	042	Dave Butenhof	13 March 1992
 *		Since cma__thread_base() doesn't use start routine and arg,
 *		there's no need to pass them into the assembly code: change
 *		call to cma__create_thread() appropriately.
 *	043	Webb Scales	13 March 1992
 *		Parameterized scheduling policies.  Added enter-kernel around
 *		setting policy and priority.
 *	044	Webb Scales and Dave Butenhof	23 March 1992
 *		Fix detach to handle multiple detaches.
 *	045	Dave Butenhof	13 April 1992
 *		Remove most CATCH clauses and CATCH_ALL from cma__thread_base
 *		function so that the new "exception interest" model wi work.
 *		That is, exceptions not explicitly handled will result in a
 *		process dump (on VMS, image exit).
 *	045A1	Webb Scales	 8 June 1992
 *		Fix a window in detach where a thread object could be 
 *		prematurely reclaimed.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_attr.h>
#include <cma_context.h>
#include <cma_kernel.h>
#include <cma_dispatch.h>
#include <cma_handle.h>
#include <cma_stack.h>
#include <cma_tcb.h>
#include <cma_thread.h>
#include <cma_assem.h>
#include <cma_mutex.h>
#include <cma_condition.h>
#include <cma_util.h>
#include <cma_deb_core.h>
#include <cma_deb_event.h>
#include <cma_debug_client.h>
#include <cma_vp.h>
#include <cma_vm.h>
#include <cma_sched.h>

#if _CMA_PER_THD_SYNC_SIGS_
# include <cma_signal.h>
#endif

#if _CMA_OS_ != _CMA__VMS
# include <signal.h>
#endif

/*
 * GLOBAL DATA
 */

#ifdef __hpux
/*
 * callback structure
 */
extern _ctxcb_spec_t	_cb;

#endif

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 */


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine grants permission to delete a thread object
 *
 *  FORMAL PARAMETERS:
 *
 *	thread  -  Address of the handle of the thread to detach
 *
 *  IMPLICIT INPUTS:
 *
 *	The thread TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The thread TCB
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	The thread may be freed.
 */
extern void
cma_thread_detach(
	cma_t_thread	*thread)	/* Handle of thread to detach*/
    {
    cma__t_int_tcb  *tcb;
	cma_t_boolean	do_free;

	do_free = cma_c_false;

    /* 
     * Validate the handle, insure that it's a thread, and get the TCB address
     */
    tcb = cma__validate_null_tcb (thread);

    /*
     * If thread is a null handle, do nothing and return
     */
    if (tcb == (cma__t_int_tcb *) cma_c_null_ptr) return;

    /*
     * Lock the tcb mutex to insure visibility.  If the thread has terminated,
     * then free the tcb now.  If the thread has not yet terminated, then mark
     * the tcb to-be-freed when the thread does terminate
     */
    cma__int_lock (tcb->mutex);		/* Lock for visibility */

	if (!tcb->detached) {
		tcb->detached = cma_c_true;	/* Mark it for freeing */
		if (tcb->terminated) {		/* If the thread has terminated...*/
			cma__assert_not_kernel ();	/* Shouldn't already be in kernel */
			cma__enter_kernel ();
			if (tcb->state == cma__c_state_terminated) {	/* ...and if really dead */
				do_free = cma_c_true;	/* ...then free the TCB (& stack) */
				(void)cma__queue_remove (&tcb->threads);	/* Remove fr. known threads queue */
			}
			cma__exit_kernel ();
		}
	}

	cma__int_unlock (tcb->mutex);	/* Visibility */

	if (do_free)
		cma__free_tcb (tcb);	/* ...then free the TCB (& stack) */

    /*
     * This handle is no longer valid, clear it
     */
    cma__clear_handle (thread);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine causes the thread to terminate prematurely, with an error
 *
 *  FORMAL PARAMETERS:
 *
 *	None
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_exit_error(void)
    {
    cma__t_int_tcb	*self;


    /* 
     * Terminate the thread by raising a special exception.
     */
    self = cma__get_self_tcb ();
    self->exit_status = cma_c_term_error;
    cma__error (cma_s_exit_thread);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine causes the thread to terminate prematurely, with success
 *	status and a return value.
 *
 *  FORMAL PARAMETERS:
 *
 *	result	The thread's result value
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_exit_normal(
	cma_t_address	result)
    {
    cma__t_int_tcb	*self;


    /* 
     * Terminate the thread by raising a special exception.
     */
    self = cma__get_self_tcb ();
    self->exit_status = cma_c_term_normal;
    self->return_value = result;
    cma__error (cma_s_exit_thread);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine creates a new thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	new_thread	- Address of handle to point to new thread (output)
 *	attr		- Address of handle for the attributes object to use
 *			  in creating the thread (and stack)
 *	start_routine	- Address of function which the new thread will execute
 *	arg		- Argument to be passed in the call to the function
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	A new thread is born and possibly scheduled (ie, it possibly preempts
 *	the current (calling) thread).
 */
extern void
cma_thread_create(
	cma_t_thread		*new_thread, /* Handle of new thread */
	cma_t_attr		*attr,	/* Att's of thread & stack */
	cma_t_start_routine	start_routine, /* Function to be executed */
	cma_t_address		arg)	/* Argument to start routine */
    {
    cma__t_int_attr	*new_att;	/* Internal form of attrib. obj */
    cma__t_int_tcb	*new_tcb;	/* New thread's TCB */

    /*
     * We don't initialize all of CMA until necessary.  cma__int_init2() is
     * not called until we need to be multi-threaded.
     */

    cma__int_init ();
    cma__int_init2 ();

    /*    
     * Get internal pointer to att object (and validate it while we're at it)
     */
    new_att = cma__validate_default_attr (attr);
		
    /* 
     * Get a TCB (and stack) for the new thread
     */
    cma__dispell_zombies ();		/* see if we can free a tcb/stack */
    new_tcb = cma__get_tcb (new_att);

    if ((cma_t_address)new_tcb == cma_c_null_ptr)
	cma__error (exc_s_insfmem);

    if (!cma__int_make_thread (new_tcb, new_thread, start_routine, arg)) {
	cma__free_tcb (new_tcb);
	cma__error (exc_s_insfmem);
	}

    /*
     * Point user's handle at the new thread object
     */
    cma__object_to_handle ((cma__t_object *)new_tcb, new_thread);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine allows the user to retrive the priority of a given thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread	  - The handle for the thread object whose priority is sought
 *	priority  - The address of a varible to recieve the thread's priority
 *
 *  IMPLICIT INPUTS:
 *
 *	The indicated thread's TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_get_priority(
	cma_t_thread	*thread,
	cma_t_priority	*priority)
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__t_int_tcb  *tcb;

    /*
     * Validate the handle, and retrieve the address of the TCB from it.
     */
    tcb = cma__validate_tcb (thread);

    /*
     * Fetch the priority from the thread's TCB and store it at the address
     * supplied by the user.
     */
    cma__int_lock (tcb->mutex);		    /* Lock for visibility */
    *priority = tcb->sched.priority;
    cma__int_unlock (tcb->mutex);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine allows the user to retrive the scheduling policy of a
 *	given thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		The handle for the thread object whose scheduling
 *			policy is sought.
 *	policy		The address of a varible to recieve the thread's
 *			scheduling policy./
 *
 *  IMPLICIT INPUTS:
 *
 *	The indicated thread's TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_get_sched(
	cma_t_thread		*thread,
	cma_t_sched_policy	*policy)
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__t_int_tcb  *tcb;

    /*
     * Validate the handle, and retrieve the address of the TCB from it.
     */
    tcb = cma__validate_tcb (thread);

    /*
     * Fetch the priority from the thread's TCB and store it at the address
     * supplied by the user.
     */
    cma__int_lock (tcb->mutex);		    /* Lock for visibility */
    *policy = tcb->sched.policy;
    cma__int_unlock (tcb->mutex);
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine allows the user to get a handle for the current thread.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread	- The address of storage to receive a handle for this thread
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_get_self(
	cma_t_thread	*thread)
    {
    cma__t_int_tcb *tcb;


    /*
     * Get a pointer to the current thread's TCB
     */
    tcb = cma__get_self_tcb ();

    /*
     * Point user's handle at the current thread object
     */
    cma__object_to_handle ((cma__t_object *)tcb, thread);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine causes the calling thread to block until a specific thread
 *	 terminates.  
 *
 *  FORMAL PARAMETERS:
 *
 *	thread	    - Address of the handle of the thread to await
 *	exit_status - Address of variable to receive a value indicating whether
 *			termination was normal, due an error, or due to an alert
 *	result	    - Address of variable to receive the value returned from the
 *			thread's start function, if any
 *
 *  IMPLICIT INPUTS:
 *
 *	The target thread's TCB
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_join(
	cma_t_thread		*thread,	/* Thread to await	    */
	cma_t_exit_status	*exit_status, /* Indicates successful term*/
	cma_t_address		*result) /* Value thread's function  */
    {
    cma__t_int_tcb  *tcb;


    /*
     * Validate the handle, insure that it's a thread, and get the TCB address
     */
    tcb = cma__validate_tcb (thread);

    /*
     * Avoid deadlock.
     */
    if(tcb == cma__get_self_tcb())
        cma__error(cma_s_in_use);

    /*
     * Wait for the thread to finish, looping to catch spurious wake-ups.  Then
     * copy the exit_status and return value into the parameters.
     *
     * Increment a reference count to prevent the thread's zombie from being 
     * freed prematurely.
     */

    cma__int_lock (tcb->mutex);

    tcb->joiners++;
    
    TRY {
	while (!tcb->terminated) {
	    cma__trace ((
		    cma__c_trc_tmp,
		    "(thread_join) thread %08x waiting on %08x",
		    cma__get_self_tcb (),
		    tcb));
	    cma__int_wait (tcb->term_cv, tcb->mutex);
	    }
	}
    CATCH_ALL {
	cma__trace ((
		cma__c_trc_tmp,
		"(thread_join) exception %08x in thread %08x",
		(THIS_CATCH)->status.status,
		cma__get_self_tcb ()));
	tcb->joiners--;
	cma__int_unlock (tcb->mutex);
	RERAISE;
	}
    ENDTRY

    cma__trace ((
	    cma__c_trc_tmp,
	    "(thread_join) thread %08x status %08x, result %08x",
	    tcb,
	    tcb->exit_status,
	    tcb->return_value));

    if (exit_status != (cma_t_exit_status *)cma_c_null_ptr)
	*exit_status = tcb->exit_status;

    if (result != (cma_t_address *)cma_c_null_ptr)
	*result = tcb->return_value;

    tcb->joiners--;

    cma__int_unlock (tcb->mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine allows the user to set the priority of a given thread to a
 *	particular value.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread	  - The handle for the thread object whose priority is to change
 *	priority  - The value of the new priority
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The indicated thread's TCB
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_set_priority(
	cma_t_thread	*thread,
	cma_t_priority	priority)
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__t_int_tcb  *tcb;

    /*
     * Validate the handle, and retrieve the address of the TCB from it.
     */
    tcb = cma__validate_tcb (thread);

    /*
     * Lock for read as well as write.
     */
    cma__int_lock (tcb->mutex);

    /*
     * Make sure that the priority value is in range, and raise an error if
     * not.
     */

    switch (tcb->sched.policy) {
	case cma_c_sched_fifo : {

	    if (priority < cma_c_prio_fifo_min 
		    || priority > cma_c_prio_fifo_max) {
		cma__int_unlock (tcb->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_rr : {

	    if (priority < cma_c_prio_rr_min
		    || priority > cma_c_prio_rr_max) {
		cma__int_unlock (tcb->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_throughput : {

	    if (priority < cma_c_prio_through_min
		    || priority > cma_c_prio_through_max) {
		cma__int_unlock (tcb->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_background : {

	    if (priority < cma_c_prio_back_min
		    || priority > cma_c_prio_back_max) {
		cma__int_unlock (tcb->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_ada_low : {

	    if (priority < cma_c_prio_ada_low_min
		    || priority > cma_c_prio_ada_low_max) {
		cma__int_unlock (tcb->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	case cma_c_sched_idle : {

	    if (priority < cma_c_prio_idle_min
		    || priority > cma_c_prio_idle_max) {
		cma__int_unlock (tcb->mutex);
		cma__error (cma_s_badparam);
		}

	    break;
	    }
	default : {
	    cma__bugcheck ("thread_set_priority: bad policy in tcb");
	    break;
	    }
	}

    /*
     * Store the priority value supplied by the user in the thread's TCB.
     * Enter kernel, first, to avoid conflicts with the scheduler.
     */
    cma__enter_kernel ();
    tcb->sched.priority = priority;
    cma__exit_kernel ();
    cma__int_unlock (tcb->mutex);

    /*
     * Force cma__ready to perform a priority queue calculation 
     */    
    cma__enter_kernel ();
    tcb->sched.adj_time = -(cma__c_prio_interval + 1);
    if (cma__is_multithreaded())
        cma__yield_processor ();		/* Should we give up CPU? */
    cma__exit_kernel ();
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine allows the user to set the scheduling policy of a given
 *	thread to a particular value.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread		The handle for the thread object whose priority is to
 *			change.
 *	
 *	policy		The value of the new scheduling policy
 *
 *	priority	New priority for thread (interpreted under new
 *			scheduling policy).
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The indicated thread's TCB
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_thread_set_sched(
	cma_t_thread		*thread,
	cma_t_sched_policy	policy,
	cma_t_priority		priority)
    {
#if (_CMA_KTHREADS_ == _CMA__MACH)
    /*
     * FIX-ME:
     * 
     * Currently, Mach threads don't support POSIX.4 policy, so we can't
     * support this function.  Remove this restriction when possible!
     */
    cma__error (cma_s_unimp);
#else
    cma__t_int_tcb  *tcb;

    /*
     * Validate the handle, and retrieve the address of the TCB from it.
     */
    tcb = cma__validate_tcb (thread);

    /*
     * Make sure that the policy value is in range, and raise an error if
     * not.
     */

    switch (policy) {
	case cma_c_sched_fifo : {

	    if (priority < cma_c_prio_fifo_min
		    || priority > cma_c_prio_fifo_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_rr : {

	    if (priority < cma_c_prio_rr_min
		    || priority > cma_c_prio_rr_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_throughput : {

	    if (priority < cma_c_prio_through_min
		    || priority > cma_c_prio_through_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_background : {

	    if (priority < cma_c_prio_back_min
		    || priority > cma_c_prio_back_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_ada_low : {

	    if (priority < cma_c_prio_ada_low_min
		    || priority > cma_c_prio_ada_low_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	case cma_c_sched_idle : {

	    if (priority < cma_c_prio_idle_min
		    || priority > cma_c_prio_idle_max)
		cma__error (cma_s_badparam);

	    break;
	    }
	default : {
	    cma__error (cma_s_badparam);
	    break;
	    }
	}

    /*
     * Store the priority value supplied by the user in the thread's TCB.
     * Enter the kernel to insure that all of these writes take place 
     * "atomically".
     */
    cma__int_lock (tcb->mutex);
    cma__enter_kernel ();
    tcb->sched.policy = policy;
    tcb->sched.priority = priority;
    cma__sched_parameterize (tcb, policy);
    cma__exit_kernel ();
    cma__int_unlock (tcb->mutex);

    /*
     * Force cma__ready to perform a priority queue calculation 
     */    
    cma__enter_kernel ();
    tcb->sched.adj_time = -(cma__c_prio_interval + 1);
    if (cma__is_multithreaded())
        cma__yield_processor ();		/* Should we give up CPU? */
    cma__exit_kernel ();
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine allows a thread to notify the scheduler of its willingness
 *	to release its processor to other threads of the same (or higher)
 *	priority.
 *
 *  FORMAL PARAMETERS:
 *
 *	None
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	None
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma_yield(void)
    {
    cma__enter_kernel ();
    cma__yield_processor ();
    cma__exit_kernel ();
    }

#ifndef _HP_CMA_PERF_
extern void
cma_yield_noselect(void)
    {
    cma__enter_kernel ();
    cma__yield_processor_noselect ();
    cma__exit_kernel ();
    }

#endif /* _HP_CMA_PERF_ */
/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine cleans up after a thread has terminated.
 *
 *  FORMAL PARAMETERS:
 *
 *	thread	    - Address of the dead thread TCB
 *
 *  IMPLICIT INPUTS:
 *
 *	None
 *
 *  IMPLICIT OUTPUTS:
 *
 *	The target thread's TCB
 *
 *  FUNCTION VALUE:
 *
 *	None
 *
 *  SIDE EFFECTS:
 *
 *	None
 */
extern void
cma__cleanup_thread(
	cma__t_int_tcb		*tcb,	/* Thread to clean up */
	cma_t_address		result,	/* Value thread's function */
	cma_t_exit_status	exit_status) /* Indicates successful term*/
    {
    cma__int_lock (tcb->mutex);			/* Writer's lock */
    cma__run_down_context (tcb);		/* Run PTC destructors */
    tcb->exit_status = exit_status;		/* Record for posterity */
    tcb->return_value = result;			/* Record for posterity */
    tcb->terminated = cma_c_true;		/* Mark thread terminated */
    cma__int_unlock (tcb->mutex);		/* Visibility */
    cma__trace ((
	    cma__c_trc_obj,
	    "(cleanup_thread) %08x broadcasting to joiners",
	    tcb));
    cma__int_broadcast (tcb->term_cv);		/* Kick the joiners */
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine sets up a thread to execute
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		Address of the thread's TCB
 *	handle		Address of thread handle (cma_c_null_ptr for internal
 *			threads)
 *	start_routine	Client's thread routine
 *	start_arg	Client's thread argument
 *	start_it	cma_c_true if thread should be started; else it's
 *			left unscheduled (VP suspended if _CMA_THREAD_IS_VP_)
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
extern cma_t_boolean
cma__int_make_thread(
	cma__t_int_tcb		*tcb,
	cma_t_thread		*handle,
	cma_t_start_routine	start_routine,
	cma_t_address		start_arg)
    {
    cma__t_int_stack	*new_stack;	/* New thread's stack */
#if _CMA_THREAD_IS_VP_
    cma__t_vp		*vp;		/* VP structure for new VP */
    cma__t_vpid		vpid;		/* ID of VP (if nonmultiplexed mP) */
    cma__t_vp_state	state;		/* State array for new VP */
#endif


    /*
     * Get address of the new child thread's stack object
     */
    new_stack = (cma__t_int_stack *)cma__queue_next (&tcb->stack);
    tcb->start_code =	start_routine;
    tcb->start_arg =	start_arg;

    /*
     * Remember some stuff for debugging displays
     */
    tcb->debug.start_pc = (cma_t_address)start_routine;
    tcb->debug.object_addr = (cma_t_address)handle;

    cma__trace ((
	    cma__c_trc_obj,
	    "(int_make_thread) thread %08x (%d), pc %08x (arg %08x), sp %08x, handle %08x",
	    tcb,
	    tcb->header.sequence,
	    start_routine,
	    start_arg,
	    new_stack->stack_base,
	    handle));

    /*
     * Create a runnable thread:  Initialize the stack and context, and
     * notify the scheduler
     */
#if _CMA_THREAD_IS_VP_
    /*
     * If we have kernel threads, and we're enforcing one-to-one mapping,
     * then create a new VP for the thread.
     */
    if ((cma_t_address)tcb->sched.processor == cma_c_null_ptr) {
	vp = cma__alloc_object (cma__t_vp);

	if ((cma_t_address)vp == cma_c_null_ptr)
	    return cma_c_false;

	tcb->sched.processor = vp;
	}
    else
	vp = tcb->sched.processor;

    cma__enter_kernel ();

    if ((cma_t_address)tcb->sched.processor->vp_id == cma_c_null_ptr) {

	if (cma__vp_create (&vpid) != cma__c_vp_normal) {
	    cma__exit_kernel ();
	    return cma_c_false;
	    }

	vp->vp_id = vpid;
	cma__g_vp_count++;		/* Count the new VP */
	}
    else
	vpid = vp->vp_id;


    state.stack = (int)new_stack->stack_base;
    state.tcb = (int)tcb;
    state.start_routine = (int)start_routine;
    state.start_arg = (int)start_arg;
    cma__vp_set_start (vpid, &state);
    cma__queue_init (&vp->interrupts);
    vp->current_thread = tcb;
    cma__start_thread (tcb);
    cma__exit_kernel ();
    cma__trace ((
	    cma__c_trc_obj | cma__c_trc_vp,
	    "(int_make_thread) thread %08x (%d); vp %d allocated",
	    tcb,
	    tcb->header.sequence,
	    vpid->vp));
#else
    cma__create_thread (
	    &tcb->static_ctx,		/* Address of base of child context */
	    new_stack->stack_base,	/* Address of base of child stack */
	    tcb);
#endif

    return cma_c_true;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	This routine begins thread execution, and handles errors.
 *
 *  FORMAL PARAMETERS:
 *
 *	tcb		Address of the thread's TCB
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
cma__thread_base(
	cma__t_int_tcb		*tcb)
    {
    cma_t_start_routine	routine	= tcb->start_code;
    cma_t_address	argument= tcb->start_arg;
    cma_t_address	value;
    cma_t_exit_status	status;
#if 0
    char	output[128];
#endif


    /*
     * A new thread is started with the kernel locked.  The first thing we
     * need to do is unlock the kernel to allow other business to proceed:
     */
#if !_CMA_THREAD_IS_VP_
    cma__exit_kernel ();
#endif

    cma__trace ((
	    cma__c_trc_obj,
	    "(thread_base) beginning thread %08x (%d) at pc %08x (arg %08x), sp %08x",
	    tcb,
	    tcb->header.sequence,
	    routine,
	    argument,
	    cma__fetch_sp ()));

#if _CMA_PER_THD_SYNC_SIGS_
    cma__sig_thread_init ();
#endif

    status = cma_c_term_normal;		/* Assume it'll be normal */

    TRY {
	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_activating);
	tcb->debug.substate = cma__c_substa_normal;

#ifdef __hpux
	if (_cb.start)
	    (*_cb.start)();
#endif

	value = (routine) (argument);

	cma__trace ((
		cma__c_trc_obj,
		"(thread_base) thread %08x completed start routine with %08x",
		tcb,
		value));

	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_terminating);

	}
    CATCH (cma_e_alerted) {
	/*
	 * If the thread was terminated by an alert, set the return status
	 * appropriately.
	 */
	cma__trace ((
		cma__c_trc_obj,
		"(thread_base) thread %08x was alerted",
		tcb));

	tcb->debug.substate = cma__c_substa_term_alt;
	status = cma_c_term_alert;

	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_term_alert);

	}
    CATCH (cma_e_exit_thread) {
	/*
	 * If the thread was terminated by a voluntary exit, set the
	 * appropriate return status and value.
	 */
	cma__trace ((
		cma__c_trc_obj,
		"(thread_base) thread %08x exited with %08x",
		tcb,
		tcb->return_value));

	tcb->debug.substate = cma__c_substa_term_exc;
	status = tcb->exit_status;
	value = tcb->return_value;

	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_term_exc);

	}
#if 0
    CATCH (cma_e_assertion) {
	/*
	 * If the thread was terminated with an assertion failure, delete the
	 * process.
	 */
	tcb->debug.substate = cma__c_substa_term_exc;

	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_term_exc);

	output[0] = '\000';
	cma__putstring	(output, "%Thread ");
	cma__putint	(output, cma__get_self_tcb ()->header.sequence);
	cma__putstring	(output, " terminating with assertion error.");
	cma__puteol	(output);
# if _CMA_OS_ == _CMA__VMS
	cma__abort ();
# else
	cma__abort_process (SIGIOT);
# endif
	}
    CATCH (cma_e_bugcheck) {
	/*
	 * If the thread was terminated with a bugcheck, delete the process.
	 */
	tcb->debug.substate = cma__c_substa_term_exc;

	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_term_exc);

	output[0] = '\000';
	cma__putstring	(output, "%Thread ");
	cma__putint	(output, cma__get_self_tcb ()->header.sequence);
	cma__putstring	(output, " terminating with bugcheck.");
	cma__puteol	(output);
# if _CMA_OS_ == _CMA__VMS
	cma__abort ();
# else
	cma__abort_process (SIGIOT);
# endif
	}
#ifdef __hpux
    CATCH (cma_e_SIGPIPE) {
	/*
	 * CHFts14092: If a SIGPIPE signal has been raised as an exception
         * but not handled by this thread, abort the process using the
         * SIG_DFL action for SIGPIPE.
	 */
	cma__abort_process (SIGPIPE);
	}
#endif
    CATCH_ALL {
	/*
	 * If the thread was terminated by any other exception, set the
	 * return status to indicate a generic error exit.
	 */
	tcb->debug.substate = cma__c_substa_term_exc;
	output[0] = '\000';
	cma__putstring	(output, "%Thread ");
	cma__putint	(output, cma__get_self_tcb ()->header.sequence);
	cma__putstring	(output, " terminating with exception:");
	cma__puteol	(output);
	exc_report (THIS_CATCH);	/* Generate an error report */
	status = cma_c_term_error;

	if (THIS_CATCH->kind == exc_kind_status_c)
	    value = (cma_t_address)THIS_CATCH->status.status;
	else
	    value = (cma_t_address)THIS_CATCH->address.address;

	if (cma__g_debug_state.events_enabled)
	    cma__debevt_report (cma__c_debevt_term_exc);

	}
#endif
    ENDTRY

    cma__trace ((
	    cma__c_trc_obj,
	    "(thread_base) thread %08x (%d) cleaning up",
	    tcb, tcb->header.sequence));

    TRY {
	cma__cleanup_thread (tcb, value, status);
	cma__trace ((
		cma__c_trc_obj,
		"(thread_base) thread %08x (%d) terminating",
		tcb, tcb->header.sequence));
	cma__terminate_thread (tcb);
	}
    CATCH_ALL {
	exc_report (THIS_CATCH);	/* Generate an error report */
	cma__bugcheck ("thread_base: exception during thread cleanup");
	}
    ENDTRY

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD.C */
/*   31    5-AUG-1992 21:52:14 KEANE "Make yield on mach platforms avoid entering kernel" */
/*   30    4-AUG-1992 13:57:24 BUTENHOF "Set saved SP to initial" */
/*   29    4-AUG-1992 11:04:55 BUTENHOF "Fix int_make_thread" */
/*   28   26-JUN-1992 14:00:53 CURTIN "Merge in 25A1 changes" */
/*  *25A1 10-JUN-1992 22:49:05 SCALES "Apply fixes for BL10+" */
/*   27    9-JUN-1992 15:01:17 KEANE "Make casts to (int) casts to (cma_t_integer)" */
/*   26    9-JUN-1992 10:46:48 CURTIN "Add Alpha VMS cactus link in cma__thread_base" */
/*  *25   14-APR-1992 13:14:59 BUTENHOF "Remove exception handlers from thread_base" */
/*  *24   25-MAR-1992 09:34:38 SCALES "Fix to handle multiple detaches" */
/*  *23   18-MAR-1992 19:01:55 SCALES "Parameterize scheduling policies" */
/*  *22   13-MAR-1992 14:10:17 BUTENHOF "Remove excess thread_base arguments" */
/*  *21   18-FEB-1992 15:30:45 BUTENHOF "Adapt to new alloc_mem protocol" */
/*  *20   10-FEB-1992 08:51:41 BUTENHOF "Add thread creation trace" */
/*  *19   13-DEC-1991 09:53:56 BUTENHOF "one-plus exception termination: return status as value" */
/*  *18   14-OCT-1991 13:41:34 BUTENHOF "Add thread count, clean up uniproc/OSF conds" */
/*  *17   24-SEP-1991 16:29:10 BUTENHOF "Complete Tin optimizations" */
/*  *16   17-SEP-1991 13:24:36 BUTENHOF "Complete Tin optimizations" */
/*  *15    2-JUL-1991 16:52:59 BUTENHOF "Cache VPs with thread" */
/*  *14   10-JUN-1991 18:24:31 SCALES "Add sccs headers for Ultrix" */
/*  *13    2-JUN-1991 19:36:55 BUTENHOF "Add trace info to cleanup_thead" */
/*  *12   29-MAY-1991 17:45:57 BUTENHOF "Support OSF/1 per-thread signals" */
/*  *11   14-MAY-1991 13:44:15 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *10    3-MAY-1991 11:26:03 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *9    23-APR-1991 11:30:20 CURTIN "added kernel lock around known thread list access" */
/*  *8    12-APR-1991 23:37:06 BUTENHOF "OSF/1 Mach thread support" */
/*  *7     8-APR-1991 20:31:39 CURTIN "added some process abort changes" */
/*  *6     1-APR-1991 18:09:39 BUTENHOF "QAR 93, exception text" */
/*  *5    13-MAR-1991 14:22:43 BUTENHOF "Allow null pointers in thread_join" */
/*  *4     5-FEB-1991 00:59:57 BUTENHOF "Improve inlining of internal cv operations" */
/*  *3    24-JAN-1991 00:35:19 BUTENHOF "Remove assertion for cma_s_bugcheck" */
/*  *2    14-DEC-1990 00:56:02 BUTENHOF "Change module names" */
/*  *1    12-DEC-1990 21:54:25 BUTENHOF "Thread management" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_THREAD.C */
