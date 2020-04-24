/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: cma_ux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:33:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:48  marty]
 *
 * Revision 1.1.4.1  1995/12/07  21:57:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:50  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:53:18  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:58  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/14  21:32:04  ohara
 * 	moved again
 * 	[1994/04/14  21:00:43  ohara]
 * 
 * Revision 1.1.2.1  1994/04/01  20:15:10  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  18:32:10  jd]
 * 
 * Revision 1.1.8.2  1993/06/24  21:22:16  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:48:22  hinman]
 * 
 * Revision 1.1.4.5  1993/02/01  22:32:17  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:33:59  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:23:45  hinman]
 * 
 * Revision 9.5.1.7  92/12/17  14:12:27  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.3.2  92/11/06  11:42:59  raj
 * 	[raj] Changes for new naming scheme with reentrant libraries
 * 
 * Revision 9.5.1.6  92/09/30  12:42:55  root
 * 	Acceptance of OSF rev 1.1.4.3
 * 
 * Revision 9.12.1.2  92/09/30  10:38:34  hinman
 * 	Auto checkin of OSF rev 1.1.4.3
 * 
 * Revision 1.1.4.3  1992/09/29  20:16:22  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:57:03  sekhar]
 * 
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:43:14  bolinger]
 * 
 * Revision 1.1.4.2  1992/09/03  14:42:08  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:07:41  bolinger]
 * 
 * 	Bring forward IBM integration changes to BL10 sources.
 * 	[1992/08/28  14:19:14  bolinger]
 * 
 * 	Bring forward 1.0.1 revisions 1.1.2.2, 1.1.2.3 (fix for OT defect
 * 	2891), revision 1.1.2.4 (fix for 2319).
 * 	[1992/08/10  16:39:29  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/09  00:11:03  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/09  00:10:46  bolinger]
 * 
 * Revision 1.1.2.4  1992/05/05  17:25:49  keane
 * 	Address defect 2319.  If an atfork handler is null, CMA will not try to call
 * 	it from cma___do_fork().
 * 	[1992/04/24  15:23:18  keane]
 * 
 * Revision 1.1.2.3  1992/04/23  21:21:54  bolinger
 * 	Complete the fix for OT defect 2891 (see last revision) to disable
 * 	virtual interval timer in child process resulting from a fork().  The
 * 	timer must be disabled in the parent, then re-enabled after the fork(),
 * 	to close a window that otherwise exists in the child between its
 * 	resumption of execution and a putative call to setitimer() to disable
 * 	the timer.
 * 	[1992/04/23  21:21:16  bolinger]
 * 
 * Revision 1.1.2.2  1992/04/20  22:08:58  bolinger
 * 	Fix OT defect 2891 (and probably 2593):  To get around probable bug
 * 	(definite deficiency, at least) in OSF/1 1.0.x reference kernel, make
 * 	cma_fork() explicitly disable ITIMER_VIRTUAL interval timer in the
 * 	newly-forked child process.  Since this is known not to be a problem in
 * 	the other reference kernel, and may not be in third-party kernels, put
 * 	the added call to setitimer() under #ifdef _CMA_VENDOR_ == _CMA__OSF.
 * 	Other kernels that don't disable this itimer will need to be added to
 * 	the #ifdef.
 * 	[1992/04/20  22:08:02  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1991, 1992 by
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
 *  Digital assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	DECthreads services
 *
 *  ABSTRACT:
 *
 *	Wrappers for UNIX process control functions.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	12 February 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	13 February 1991
 *		Fix initial bugs
 *	002	Dave Butenhof	26 February 1991
 *		Bugcheck on errors in child code after fork.
 *	003	Dave Butenhof	01 May 1991
 *		Add arguments to cma__bugcheck() calls.
 *	004	Paul Curtin	05 June 1991
 *		Total rewrite of fork: it now clears all existence
 *		of threads other than the forking thread in the
 *		forked process. ('cept the null thread of course)
 *	005	Paul Curtin	06 June	1991
 *		Added Al Simon's atfork work.
 *	006	Webb Scales	10 June 1991
 *		Removed cma_errno include.
 *	007	Dave Butenhof	11 June 1991
 *		Conditionalize thread I/O reinit (not used on multiprocessor
 *		implementations).  Also include cma_init.h to get
 *		cma__int_init() macro.
 *	008	Dave Butenhof	10 September 1991
 *		Modify cma_fork() wrapper to skip all the atfork() and reinit
 *		stuff and just call "raw" fork() if DECthreads hasn't been
 *		initialized yet (there's no need for the other stuff).
 *	009	Dave Butenhof	19 September 1991
 *		Integrate HPUX CMA5 reverse drop: include HP-specific header
 *		if using HP reentrant libc.
 *	010	Dave Butenhof	04 October 1991
 *		Clean up use of _CMA_UNIPROCESSOR_
 *	011	Dave Butenhof	07 January 1992
 *		Build in modification of OSF/1's malloc.c from libpthreads.a;
 *		it has pre/post fork functions that need to be called.
 *	012	Dave Butenhof	11 February 1992
 *		A law of nature has just been changed: cma__alloc_mem now
 *		returns cma_c_null_ptr on an allocation failure, rather than
 *		raising an exception. This allows greater efficiency, since
 *		TRY is generally expensive. Anyway, apply the process of
 *		evolution: adapt or die.
 *	013	Webb Scales	18 February 1992
 *		Since defered actions are now performed on both enter- and
 *		exit-kernel, replace these macros where undeferrals are 
 *		unwanted.
 *	014	Dave Butenhof	12 March 1992
 *		Add tracing.
 */


/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_attr.h>
#include <cma_condition.h>
#include <cma_context.h>
#include <cma_deb_core.h>
#include <cma_defs.h>
#include <cma_dispatch.h>
#include <cma_stack.h>
#include <cma_signal.h>
#include <cma_defer.h>
#include <cma_once.h>
#include <cma_tcb.h>
#include <cma_timer.h>
#include <cma_kernel.h>
#include <cma_mutex.h>
#include <cma_queue.h>
#include <cma_vm.h>
#include <cma_init.h>
#include <cma_vp.h>
# if !_CMA_THREAD_SYNC_IO_
#  include <cma_thread_io.h>
# endif
# include <sys/time.h>
# include <signal.h>
# include <cma_ux.h>
#include <stdio.h>

# include <cma_ux_redefs.h>

/*
 * GLOBAL DATA
 */

/*
 * LOCAL DATA
 */

#define CMA___C_PRE_FORK 0		/* cma_atfork definitions */
#define CMA___C_PARENT_POST_FORK 1
#define CMA___C_CHILD_POST_FORK 2
#define CMA___C_MAX_FORK_RTNS 3                           

static cma__t_queue	cma___g_fork_handler_queue;
static cma__t_int_mutex	*cma___g_fork_queue_mutex;

typedef struct CMA___T_FORK_RTNS {
    cma__t_queue queue;                 /* Front and back links           */
    cma_t_address user_state;           /* User state parameter           */
    cma_t_fork_rtn routines[CMA___C_MAX_FORK_RTNS];
                                        /* Array of fork routine pointers */
                                        /* Pre-fork, Parent post-fork     */
                                        /* Child post_fork                */
    } cma___t_fork_rtns;

/*
 * LOCAL MACROS
 */

/*
 * LOCAL FUNCTIONS
 */

static void
cma___do_fork (cma_t_natural);
                                     

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Perform UNIX fork(2) function, and shut down CMA in the child process
 *	(turn off timeslicing, and lock kernel to prevent erroneous use).
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
 *	The pid of the new process, or 0 in the child
 *
 *  SIDE EFFECTS:
 *
 *	Shuts down all but the forking thread in the child process.
 */
extern pid_t
cma_fork(void)
    {
    int	pid;
	struct itimerval	unix_timer, unix_oldtime;

    /*
     * We're putting a lot of complication around the fork() routine to
     * handle thread system rundown and reinitialization. None of that is
     * necessary if DECthreads hasn't been initialized yet. So first, test
     * whether initialization has been done... and if not, just call fork()
     * directly and return.
     */
    if (!cma__tac_isset (&cma__g_init_started))
	return __nr_fork ();

    cma__trace ((
	    cma__c_trc_wrp,
	    "(fork) beginning prefork actions"));

    /*
     * Call do_fork specifying that we're just prior to fork.
     * This for atfork() functionality.
     */
    cma___do_fork (CMA___C_PRE_FORK);

    /*
     * Call reinit routines passing 0 to indicate pre-fork reinitialize.  For
     * the most part these routines simply perform a lock on internal locks.
     */

    cma__reinit_debug (cma__c_reinit_prefork_lock);
    cma__reinit_vp (cma__c_reinit_prefork_lock);
    cma__reinit_attr (cma__c_reinit_prefork_lock);
    cma__reinit_mutex (cma__c_reinit_prefork_lock);
    cma__reinit_signal (cma__c_reinit_prefork_lock);
    cma__reinit_memory (cma__c_reinit_prefork_lock);
    cma__reinit_timer (cma__c_reinit_prefork_lock);
#if !_CMA_THREAD_SYNC_IO_
    cma__reinit_thread_io (cma__c_reinit_prefork_lock);
#endif
    cma__reinit_tcb (cma__c_reinit_prefork_lock);
    cma__reinit_once (cma__c_reinit_prefork_lock);
    cma__reinit_dispatch (cma__c_reinit_prefork_lock);
    cma__reinit_stack (cma__c_reinit_prefork_lock);
    cma__reinit_defer (cma__c_reinit_prefork_lock);
    cma__reinit_context (cma__c_reinit_prefork_lock);
    cma__reinit_cv (cma__c_reinit_prefork_lock);
    cma__int_lock (cma__g_global_lock);
    cma__set_kernel ();		/* No interference! */
	/*
	 * As part of workaround to ensure that VIRTUAL itimer is
	 * disabled in child, disable here (for duration of fork()).
	 * This closes a window in the child between the fork() and
	 * the execution of a hypothetical setitimer() call below
	 * in which an alarm could be delivered.
	 */
	cma__memset((char *)&unix_timer, 0, sizeof(unix_timer));
	setitimer(ITIMER_VIRTUAL, &unix_timer, &unix_oldtime);

    pid = __nr_fork();

    if (pid != 0) {				/* parent process */
	setitimer(ITIMER_VIRTUAL, &unix_oldtime, (struct itimerval *)0);

	/*
	 * Exit kernel and unlock all locks in the parent process.
	 */
	cma__unset_kernel ();
	cma__int_unlock (cma__g_global_lock);
	cma__reinit_cv (cma__c_reinit_postfork_unlock);
	cma__reinit_context (cma__c_reinit_postfork_unlock);
	cma__reinit_defer (cma__c_reinit_postfork_unlock);
	cma__reinit_stack (cma__c_reinit_postfork_unlock);
	cma__reinit_dispatch (cma__c_reinit_postfork_unlock);
	cma__reinit_once (cma__c_reinit_postfork_unlock);
	cma__reinit_tcb (cma__c_reinit_postfork_unlock);
#if !_CMA_THREAD_SYNC_IO_
	cma__reinit_thread_io (cma__c_reinit_postfork_unlock);
#endif
	cma__reinit_timer (cma__c_reinit_postfork_unlock);
	cma__reinit_memory (cma__c_reinit_postfork_unlock);
	cma__reinit_signal (cma__c_reinit_postfork_unlock);
	cma__reinit_mutex (cma__c_reinit_postfork_unlock);
	cma__reinit_attr (cma__c_reinit_postfork_unlock);
	cma__reinit_vp (cma__c_reinit_postfork_unlock);
	cma__reinit_debug (cma__c_reinit_postfork_unlock);

         /*
          * Call do fork specifying that we're in parent after fork.
          * This for atfork() functionality.
          */
	cma___do_fork (CMA___C_PARENT_POST_FORK);
	cma__trace ((
		cma__c_trc_wrp,
		"(fork) parent completed postfork actions"));
	}
    else {					/* child process */
	/*
	 * Reinitialize required data structures, and remove all other
	 * threads from the known threads list in forked process.
	 */

	/*
	 * FIX-ME:
	 *
	 * This is inefficient. In each case, we need to clear state and
	 * unlock the mutexes. The two are separated because the parent
	 * post-fork processing also unlocks, but doesn't reinit. Still,
	 * there's no need for two separate sets of calls: the reinit
	 * functions should be implemented as a switch() with the clear case
	 * preceeding and falling through into the unlock case.
	 */
	cma__unset_kernel ();	
	cma__int_unlock (cma__g_global_lock);
	cma__reinit_cv (cma__c_reinit_postfork_clear);
	cma__reinit_cv (cma__c_reinit_postfork_unlock);
	cma__reinit_context (cma__c_reinit_postfork_clear);
	cma__reinit_context (cma__c_reinit_postfork_unlock);
	cma__reinit_defer (cma__c_reinit_postfork_clear);
	cma__reinit_defer (cma__c_reinit_postfork_unlock);
	cma__reinit_stack (cma__c_reinit_postfork_clear);
	cma__reinit_stack (cma__c_reinit_postfork_unlock);
	cma__reinit_dispatch (cma__c_reinit_postfork_clear);
	cma__reinit_dispatch (cma__c_reinit_postfork_unlock);
	cma__reinit_once (cma__c_reinit_postfork_clear);
	cma__reinit_once (cma__c_reinit_postfork_unlock);
	cma__reinit_tcb (cma__c_reinit_postfork_clear);
	cma__reinit_tcb (cma__c_reinit_postfork_unlock);
#if !_CMA_THREAD_SYNC_IO_
	cma__reinit_thread_io (cma__c_reinit_postfork_clear);
	cma__reinit_thread_io (cma__c_reinit_postfork_unlock);
#endif
	cma__reinit_timer (cma__c_reinit_postfork_clear);
	cma__reinit_timer (cma__c_reinit_postfork_unlock);
	cma__reinit_memory (cma__c_reinit_postfork_clear);
	cma__reinit_memory (cma__c_reinit_postfork_unlock);
	cma__reinit_signal (cma__c_reinit_postfork_clear);
	cma__reinit_signal (cma__c_reinit_postfork_unlock);
	cma__reinit_mutex (cma__c_reinit_postfork_clear);
	cma__reinit_mutex (cma__c_reinit_postfork_unlock);
	cma__reinit_attr (cma__c_reinit_postfork_clear);
	cma__reinit_attr (cma__c_reinit_postfork_unlock);
	cma__reinit_vp (cma__c_reinit_postfork_clear);
	cma__reinit_vp (cma__c_reinit_postfork_unlock);
	cma__reinit_debug (cma__c_reinit_postfork_clear);
	cma__reinit_debug (cma__c_reinit_postfork_unlock);

	/*
	 * Call do fork specifying that we're in child just after fork. This
	 * for atfork() functionality.
	 */
	cma___do_fork (CMA___C_CHILD_POST_FORK);
	cma__trace ((
		cma__c_trc_wrp,
		"(fork) child completed postfork actions"));
	}

    return pid;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Register caller's fork handler routines.
 *
 *  FORMAL PARAMETERS:
 *
 *      user_state - the pointer to user state--passed to each routine.
 *	pre_fork - the routine to be called before performing the fork.
 *      parent_fork - the routine to be called after, in the parent.
 *      child_fork - the routine to be called after, in the child.
 *
 *  IMPLICIT INPUTS:
 *
 *	cma___g_fork_handler_queue, cma___g_fork_queue_mutex
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
 *	allocates memory.
 */
extern void
cma_atfork       (
	cma_t_address user_state,
	cma_t_fork_rtn pre_fork,
        cma_t_fork_rtn parent_fork,
        cma_t_fork_rtn child_fork)
    {
    cma___t_fork_rtns * entry;

    /*
     * Initialize CMA if it has not already been done.
     */
    cma__int_init();

    /*
     * Allocate a new fork routines structure to hold them.
     */
    entry = cma__alloc_object (cma___t_fork_rtns);

    /*
     * FIX-ME:
     *
     * It doesn't seem right for "atfork" to return exceptions. I'm not going
     * to change the prototype to return a value right now, but we ought to
     * consider that later.
     */
    if ((cma_t_address)entry == cma_c_null_ptr)
	cma__error (exc_s_insfmem);

    /*
     * Populate the new entry.
     */
    entry->user_state = user_state;
    entry->routines [CMA___C_PRE_FORK] = pre_fork;
    entry->routines [CMA___C_PARENT_POST_FORK] = parent_fork;
    entry->routines [CMA___C_CHILD_POST_FORK] = child_fork;

    /*
     * Take the lock to assure consistency and visibility.
     */
    cma__int_lock (cma___g_fork_queue_mutex);

    /*
     * Insert the entry at the head of the queue so we get LIFO order.
     */
    cma__queue_insert_after ((cma__t_queue *)entry, &cma___g_fork_handler_queue);

    /*
     * Release the lock.
     */
    cma__int_unlock (cma___g_fork_queue_mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Call the registered fork handlers for the appropriate fork case.
 *
 *  FORMAL PARAMETERS:
 *
 *	which_case - whether this is pre-fork, post-fork parent, or 
 *                   post-fork child.
 *
 *  IMPLICIT INPUTS:
 *
 *	the fork handler queue and fork handler mutex.
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
static void
cma___do_fork(
	cma_t_natural which_case)
    {
    cma___t_fork_rtns * entry;

    /*
     * Take the lock on the queue to assure data visibility and 
     * consistency.
     */
    cma__int_lock (cma___g_fork_queue_mutex);

    /*
     * If the queue is empty we have nothing to do.
     */
    if (!cma__queue_empty (&cma___g_fork_handler_queue)) {
	entry = (cma___t_fork_rtns *) cma__queue_next (&cma___g_fork_handler_queue);

	do {
	    if ((cma_t_address)entry->routines[which_case] != cma_c_null_ptr)
	        (*(entry->routines[which_case]))(entry->user_state);
	    entry = (cma___t_fork_rtns *) cma__queue_next (
			(cma__t_queue *) entry);
	    } while (entry != (cma___t_fork_rtns *) &cma___g_fork_handler_queue);
	}

    /*
     * Release the lock.
     */
    cma__int_unlock (cma___g_fork_queue_mutex);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Init the fork handlers mechanism.
 *
 *  FORMAL PARAMETERS:
 *
 *	none
 *
 *  IMPLICIT INPUTS:
 *
 *	the fork handler queue and fork handler mutex.
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
cma__init_atfork       (void)
    {
    cma___g_fork_queue_mutex = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_fork_queue_mutex, "atfork queue");
    cma__queue_init (&cma___g_fork_handler_queue);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      Disable CMA timer before calling the non reentrant libc version
 *      of execve. 
 *
 *  FORMAL PARAMETERS:
 *
 *  IMPLICIT INPUTS:
 *
 *  IMPLICIT OUTPUTS:
 *
 *  FUNCTION VALUE:
 *
 */
extern int
cma_execve       (const char *path, char *const *argv, char *const *cnup )
    {
    struct itimerval old, new;
    int rv;

# if !_CMA_REENTRANT_CLIB_
    cma__int_init ();
    cma_lock_global ();
# endif

    timerclear(&new.it_interval);
    timerclear(&new.it_value);
    setitimer(ITIMER_VIRTUAL, &new, &old);

    rv = __nr_execve( path, argv, cnup);
    setitimer(ITIMER_VIRTUAL, &old, (struct itimerval *)0);

# if !_CMA_REENTRANT_CLIB_
    cma_unlock_global ();
# endif

    return rv;
    }


/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_UX.C */
/*   14A1 17-APR-1992 08:34:06 KEANE "Apply undeferral fix to improved BL9" */
/*  *17   13-MAR-1992 14:10:46 BUTENHOF "Add tracing" */
/*  *16   19-FEB-1992 13:50:37 SCALES "Perform undeferal on enter-kernel" */
/*  *15   18-FEB-1992 15:31:27 BUTENHOF "Adapt to new alloc_mem protocol" */
/*   10A1 20-JAN-1992 17:15:27 SCALES "Integrate changes for Tin malloc()" */
/*  *14    7-JAN-1992 17:27:35 BUTENHOF "Call malloc pre/post fork functions on OSF/1 kthreads" */
/*  *13   14-OCT-1991 13:42:19 BUTENHOF "Refine/fix use of config symbols" */
/*  *12   24-SEP-1991 16:29:52 BUTENHOF "Call raw fork() in cma_fork if CMA not inited" */
/*  *11   17-SEP-1991 13:24:58 BUTENHOF "Call raw fork() in cma_fork if CMA not inited" */
/*  *10   12-JUN-1991 12:21:55 BUTENHOF "Catch another call to reinit_thread_io" */
/*  *9    11-JUN-1991 16:32:46 BUTENHOF "Fix reinit code" */
/*  *8    10-JUN-1991 18:25:06 SCALES "Add sccs headers for Ultrix" */
/*  *7    10-JUN-1991 17:56:03 SCALES "Remove include of cma_errno" */
/*  *6     6-JUN-1991 11:22:14 CURTIN "Added Al Simon's atfork work" */
/*  *5     5-JUN-1991 16:16:08 CURTIN "fork work" */
/*  *4     2-MAY-1991 13:59:54 BUTENHOF "Add argument to cma__bugcheck() calls" */
/*  *3    27-FEB-1991 15:48:33 BUTENHOF "Bugcheck on errors in child reset code" */
/*  *2    14-FEB-1991 23:51:14 BUTENHOF "Fix it up" */
/*  *1    13-FEB-1991 00:04:34 BUTENHOF "UNIX process control wrappers" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_UX.C */
