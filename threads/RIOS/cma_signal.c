/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA/exc_handl
 */
/*
 * HISTORY
 * $Log: cma_signal.c,v $
 * Revision 1.1.9.1  1996/05/10  13:31:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/08  21:52 UTC  bissen
 * 	Port single-threaded
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:40 UTC  psn
 * 	Changes to allow valid AIX core dumps.
 * 	[1995/04/26  16:55 UTC  jrr  /main/DCE1_1_WP/1]
 *
 * Revision 1.1.2.3  1994/09/15  16:29:19  bmw
 * 	CR 12206: AIX core file changes to dump the heap with a core file
 * 	[1994/09/15  16:28:53  bmw]
 * 
 * Revision 1.1.2.2  1994/06/09  13:35:38  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:09  devsrc]
 * 
 * 	Bring forward IBM integration changes to BL10 sources.
 * 	[1992/08/28  14:12:45  bolinger]
 * 
 * 	Bring forward fix from IBM integration of BL10 sources.
 * 	[1992/08/26  14:08:53  bolinger]
 * 
 * 	Merge up to BL10+ internal release from DEC.
 * 	[1992/08/11  21:05:16  bolinger]
 * 
 * 	Bring forward 1.0.1 revision 1.1.3.3.  Also bring forward 1.1.3.5
 * 	(exc_sigs_are_xlated()), but dummy it out.
 * 	[1992/08/10  16:10:55  bolinger]
 * 
 * 	Fix OT defect 3941:  In _CMA_SYNC_KILL_ code, check for app-installed
 * 	signal handler before installing our own.  If app has one, leave it
 * 	installed.
 * 	[1992/06/23  13:28:47  bolinger]
 * 
 * 	More work for defect 2277.  Incorporate Don Bolinger's changes
 * 	for exc_xlated_sigs.
 * 	[1992/05/14  15:02:52  keane]
 * 
 * 	Move sigset_t initialization to init routine, where it belongs.
 * 	[1992/05/12  13:06:10  bolinger]
 * 
 * 	Fix OT defect 3400: Initialize a new sigset_t correctly.
 * 	[1992/05/12  12:51:30  bolinger]
 * 
 * 	Fix for OT defect 2895:  since _CMA_SYNC_KILL_ is hanging around
 * 	for the 1.0.1 release, add new function, callable from functional
 * 	tests, to query whether the sync kill code is enabled.  (Several
 * 	of the tests are partially or wholly dummied out if it is.)  This
 * 	function, exc_sigs_are_xlated(sigset_t *set), is here only
 * 	temporarily.  If it sticks around (as part of the resolution
 * 	of the dreaded bug 2302), it will move the exc_handling.{c,h}.
 * 
 * 	Fix for OT defect 3372:  defer taking over SIGCHLD until an
 * 	app sigwait()'s on it.  This avoids the receipt of SIGCHLD
 * 	signals (which pose a problem when they interrupt syscalls),
 * 	unless a thread has really requested they be delivered.
 * 	[1992/05/11  13:10:30  bolinger]
 * 
 * 	For OT defect 2302, add a workaround pending a real fix: enable
 * 	existing _CMA_SYNC_KILL_ code whenever NDEBUG is not asserted.  Also
 * 	move SIGSYS out of the control of this code.  This code will cause most
 * 	synchronous terminating signals to be handled transparently (SIGPIPE
 * 	being the other exception), effectively causing a core dump at the
 * 	point of signal generation.
 * 	[1992/04/20  18:13:01  bolinger]
 * 
 * 	     Check to see if SIGLOST is defined before using it (APOLLO68K).
 * 	[1992/04/13  18:30:13  mishkin]
 * 
 * 	Fix OT defect 2581:  make cma_sigaction() check presence
 * 	of its handler arguments before trying to use them.  Fix
 * 	OT defect 2613: fix cma__sig_deliver() to return a value
 * 	(indicating signal handled) for a signal whose handling
 * 	is marked SIG_IGN (ignore).
 * 	[1992/04/08  17:52:38  bolinger]
 * 
 * Revision 1.1.2.1  1994/04/01  20:14:14  jd
 * 	Moved from src/threads to here
 * 	[1994/04/01  17:52:50  jd]
 * 
 * Revision 1.1.17.5  1993/10/21  20:24:42  ohara
 * 	make last submission posix - need for svr4
 * 	[1993/10/21  20:23:51  ohara]
 * 
 * Revision 1.1.17.4  1993/10/20  21:30:29  jd
 * 	(5151) Drop from HP.
 * 	[1993/10/20  21:02:57  jd]
 * 
 * Revision 1.1.17.3  1993/10/15  21:19:09  ohara
 * 	reference port 8596
 * 	[1993/10/15  16:32:06  ohara]
 * 
 * Revision 1.1.17.2  1993/10/06  14:01:23  ohara
 * 	fix a merge/edit problem 9051
 * 	[1993/10/06  14:00:40  ohara]
 * 
 * Revision 1.1.17.1  1993/10/05  21:38:45  ohara
 * 	SNI SVR4 ref port 8596
 * 	[1993/10/05  21:36:46  ohara]
 * 
 * 	SNI SVR4 ref port 8596
 * 	[1993/09/15  22:24:08  ohara]
 * 
 * Revision 1.1.15.3  1993/07/13  21:06:10  jd
 * 	RIOS doesn't accept conditionalized arguments within protype lists.
 * 	Conditionalize entire prototype declaration rather than the arguments.
 * 	[1993/07/13  20:27:20  jd]
 * 
 * Revision 1.1.15.2  1993/06/24  21:03:09  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:44:41  hinman]
 * 
 * Revision 1.1.7.10  1993/03/17  19:17:41  jd
 * 	Have SIGPIPE raise an exception.
 * 	[1993/03/17  12:48:59  jd]
 * 
 * Revision 1.1.7.9  1993/02/24  20:39:16  steveg
 * 	Remove USL Copyright.
 * 	[1993/02/24  20:37:12  steveg]
 * 
 * Revision 1.1.7.8  1993/02/02  22:04:59  sommerfeld
 * 	[OT7097] don't call cma__remap_stack_holes if _CMA_PROTECT_MEMORY_
 * 	[OT7098] fix confusion between sa_flags and sa_mask
 * 	is not set.
 * 	[1993/02/02  19:30:56  sommerfeld]
 * 
 * Revision 1.1.7.7  1993/02/01  22:18:49  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:27:08  hinman]
 * 
 * Revision 1.1.7.5  1993/01/11  22:54:37  jd
 * 	Put cma__remap_stack_holes back in sync_kill.
 * 	[1993/01/11  22:53:25  jd]
 * 	Revision 1.1.12.2  1993/01/11  16:21:01  hinman
 * 	[hinman] - Check in merged SNI version
 * 	Revision 1.1.7.3  1992/09/29  20:15:25  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:50:57  sekhar]
 * 
 * Revision 1.1.7.2  1992/09/03  14:39:56  bolinger
 * 	Replace current source file with equivalent from DEC BL10+
 * 	source base, as originally integrated in the
 * 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
 * 	[1992/09/02  14:05:59  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/28  20:45:41  bolinger
 * 	Move latest 1.0.1-based revision into 1.0.2.
 * 	[1992/08/28  20:42:13  bolinger]
 * 
 * Revision 1.1.2.2  1992/05/08  23:51:13  bolinger
 * 	BL10 CMA sources from DEC.
 * 	[1992/05/08  23:50:54  bolinger]
 * 
 * Revision 1.1  1992/01/19  14:40:35  devrcs
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
 *	This module contains routines concerned with handling Unix signals.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	16 March 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	29 March 1990
 *		Skip timeslice signal during initialization.
 *	002	Webb Scales	29 March 1990
 *		Replace 'sigset_t' by 'int'.
 *	003	Webb Scales	9 April 1990
 *		Return to 'sigset_t' and incorporate Apollo changes.
 *		Conditionalize code for VMS.
 *	004	Webb Scales	16 April 1990
 *		Fix typo/think-o off-by-one loop index problem.
 *		Remove redundant cma_host.h include.
 *	005	Webb Scales	26 April 1990
 *		Change sigsetmask to sigprocmask.
 *	006	Webb Scales	27 April 1990
 *		Integrate Apollo changes for BL2 (change sigvec to sigaction).
 *	007	Dave Butenhof	28 May 1990
 *		Include new cma_px.h instead of cma_crtlx.h for sigset_t.
 *	008	Webb Scales	30 May 1990
 *		Reorder module initialization so that the sigwait mutex and cv
 *		are created before the signal handlers are set up.
 *	009	Webb Scales	4 June 1990
 *		Establish timeslicer signal handler with other signal handlers.
 *		Add more exceptions (specific instruction and arithmetics).
 *		Allow 'sigwait'ing on SIGQUIT.
 *	010	Webb Scales	5 June 1990
 *		Bring sigwait up to POSIX spec.  Make sigwait code compileable
 *		on VMS.
 *	011	Dave Butenhof	14 June 1990
 *		Replace cma__test_and_set/cma__unset by cma__kernel_set and
 *		cma__kernel_unset.
 *	012	Dave Butenhof	03 August 1990
 *		Include cma_defer.h (new home of deferral code).
 *	013	Paul Curtin	06 August 1990
 *		Replace abort w/ cma__abort
 *		Replace printf w/ cma__put* functions
 *		Replace cma__unblock_all_waiting w/ cma__sem_wake_all
 *	014	Webb Scales	16 August 1990
 *		Integrate Sun port
 *	015	Dave Butenhof	27 August 1990
 *		Change CMA interfaces to pass handles & structures by reference.
 *	016	Webb Scales	29 August 1990
 *		Replaced instances of RAISE with calls to cma_error.
 *	017	Webb Scales	 8 October 1990
 *		Fixed sigwait to use errno instead of exceptions.
 *		Changed SIGTSTP handling to be like SIGQUIT.
 *	018	Dave Butenhof	25 October 1990
 *		Add name to mutexes created at init time.
 *	019	Webb Scales	29 October 1990
 *		sigwait now waits on thread-specific CV.
 *	020	Paul Curtin	11 December 1990
 *		Implemented sigaction for synchronous signals
 *	021	Dave Butenhof	27 December 1990
 *		Rename exceptions.
 *	022	Dave Butenhof	22 January 1991
 *		Fix exception names
 *	023	Dave Butenhof	1 February 1991
 *		Correct order of args to cma__int_wait to conform to external
 *		interface (cv, mutex).
 *	024	Dave Butenhof	20 February 1991
 *		Try to remove an annoying warning message by adding a
 *		spurious cast in an assignment of identically typed pointers.
 *	025	Dave Butenhof	26 February 1991
 *		Make changes for IBM RS/6000 AIX. SIGIOT and SIGLOST are
 *		synonyms,
 *	026	Webb Scales	12 March 1991
 *		Added an alert-test to sigwait to make it a full alert point.
 *	027	Webb Scales	12 March 1991
 *		Merged Apollo changes to CD4
 *	028	Dave Butenhof	05 April 1991
 *		Add OSF/1 signals to get through cma__init_signal.
 *	029	Paul Curtin	08 April 1991
 *		Added code for quick kill.
 *	030	Paul Curtin	06 May 1991
 *		Replaced call to cma_init() with cma__int_init().
 *	031	Dave Butenhof	08 May 1991
 *		Disable the force_dispatch based "quick kill" on kernel
 *		thread implementations, since it won't work... use
 *		cma__vp_interrupt() instead.
 *	032	Paul Curtin	13 May 1991
 *		Replaced a number of external interace calls w/internal
 *		calls/macros.
 *	033	Dave Butenhof	30 May 1991
 *		Add an fflush(stdout) call to cma__abort_process, since
 *		sometimes it doesn't call cma__abort().
 *	034	Dave Butenhof	30 May 1991
 *		Fix wblk structure to use internal CV pointer, since it's
 *		signalled using internal interface (fix to edit 032).
 *	035	Paul Curtin	3  June 1991
 *		Added a new reinit routine.
 *	036	Dave Butenhof	04 June 1991
 *		Augment #034's fix of #032 by also changing the other half of
 *		the CV functions to internal versions (cond_signal_int).
 *	037	Dave Butenhof	05 June 1991
 *		Integrate IBM reverse drop.
 *	038	Paul Curtin	06 June 1991
 *		Re replaced index in for loop, instead of a 1.
 *	039	Webb Scales and Dave Butenhof	    10 June 1991
 *		- Conditionalize inclusion of I/O stuff.
 *		- POSIX-ize signal stuff for OSF/Ultrix
 *	040	Webb Scales	11 July 1991
 *		Fixed a bug which eats all but the first signal if multiple
 *		signals are already pending when sigwait() is called.
 *	041	Paul Curtin	20 August 1991
 *		Conditionalized out the inclusion stdio.h on VMS
 *	042	Paul Curtin	03 September 1991
 *		Insure process signal mask restore on sync term, qar 134.
 *	043	Dave Butenhof	18 September 1991
 *		Merge in IBM, HPUX, and Apollo reverse drops for CMA5.
 *	044	Dave Butenhof	02 October 1991
 *		Integrate changes provided by Alan Peckham to unprotect guard
 *		pages on all stacks before aborting the process; this allows
 *		the UNIX core dump code (which makes the absurd assumption
 *		that the known universe stops just because a page is
 *		protected) to work on threaded processes. Also, integrate code
 *		for an option to allow setting synch. term. signals to abort
 *		the process with a core dump (this may later be enabled
 *		through a cma_debug() command).
 *	045	Dave Butenhof	04 October 1991
 *		Generalize a few conditionals (like checking for whether
 *		signal name is defined rather than specific platforms).
 *		Suggested by Ken Ouellette. There are probably other places
 *		where this could be done, but I don't have access to signal.h
 *		for all the platforms, and don't care to guess whether the
 *		signals are truly not defined, or whether (for whatever
 *		reason) those who ported DECthreads to the platform chose not
 *		to handle them.
 *	046	Webb Scales	18 November 1991
 *		- Corrected routine header for the default signal handler
 *		- Fixed routine returns for the default signal handler
 *		- Removed all references to cma___c_sigacted
 *	047	Dave Butenhof	26 November 1991
 *		Include unistd.h to make ANSI C checking happy.
 *	048	Dave Butenhof	27 November 1991
 *		Conditionalize more code against VMS, since Alpha DEC C is
 *		more picky than VAX C.
 *	049	Dave Butenhof	19 March 1992
 *		Fix some holes in per-thread sigaction, reported by Dale
 *		Tonogai (HP).
 *	050	Webb Scales	6 April 1992
 *		- Modified sigwait to only install signal handlers when they
 *		  are needed.  
 *		- Removed the latent code for termination on synchronous
 *		  terminating signals, as unhandled exceptions will now 
 *		  terminate the process at the point where they are raised.
 *		- Reworked the abort-process routine to be much simpler.
 *		- Simplified unhandled async. term. signal handling.  
 *		  DEC Non-kernel implementations will call abort-process 
 *		  without bothering to switch contexts.
 *		- Reworked the module to allow it to be build on VMS for
 *		  use with C RTL support.
 *	051	Webb Scales	18 May 1992
 *		Cleanse the wblk links before inserting it into the queue.
 *	052	Brian Keane 	21 May 1992
 *		Add missing typecast on queue operation.
 */

/*
 *  INCLUDE FILES
 */


#include <cma.h>
#include <cma_defs.h>
#include <cma_alert.h>
#include <cma_errors.h>
#include <cma_queue.h>
#include <cma_handle.h>
#include <cma_init.h>
#include <cma_condition.h>
#include <cma_mutex.h>
#include <cma_tcb_defs.h>
#include <cma_tcb.h>
#include <cma_stack.h>
#include <cma_defer.h>
#include <cma_timer.h>
#include <cma_sigwait.h>
#include <cma_util.h>
#include <cma_int_errno.h>
#include <cma_assem.h>
#include <cma_vp.h>
#include <cma_signal.h>
# include <cma_thread_io.h>
# include <unistd.h>
#include <signal.h>
# include <stdio.h>

# include <exc_handling.h>

/*
 * Global data
 */

cma_t_integer	    cma__g_abort_signal;	/* Signal sent to abort proc */

extern EXCEPTION	io_avail_exc;
extern cma_t_integer	io_avail_null_thread;


typedef struct sigaction sigaction_t;

/*
 * LOCAL TYPEDEFS
 */
typedef	    void (*cma___t_sig_hand_ptr) (int);

typedef void (*cma___t_sighandler)(int, int, struct sigcontext *);

typedef struct CMA___T_SIGWAIT_BLOCK {
    cma__t_queue	queue;	/* Must be the first field */
    sigset_t		*set;	/* Signals which thread is awaiting */
    cma_t_integer	signal;	/* Signal that the thread received */
    cma__t_int_cv	*cv;	/* Condition variable thread blocks on */
    cma__t_int_tcb	*tcb;	/* Just for debugging */
    } cma___t_sigwait_block;
		    

/*
 * LOCAL DATA
 */

static cma__t_int_mutex *cma___g_sigwait_mutex;
static cma__t_queue	cma___g_sigwait_queue;
static cma_t_integer	cma___g_sigwaiters[NSIG];
static sigset_t		cma___g_sig_wait_mask;
#ifdef	AIX_CORE_FILE
static sigset_t		cma___g_sig_core_mask;
#endif	/* AIX_CORE_FILE */
static sigset_t		cma___g_sigact_block_mask;
static sigset_t		cma___g_sig_block_mask;
static char		ibmr2_vtalrm_stack_area[8192];
static caddr_t		ibmr2_vtalrm_stack = 
			    ((caddr_t) &ibmr2_vtalrm_stack_area[8191]);

/*
 * LOCAL MACROS
 */

/*
 * No special actions are required for non-terminating signals.
 */
#define	cma___sig_async_nonterm	(cma___t_sig_hand_ptr)cma__sig_deliver
#define	cma___sig_sync_nonterm	(cma___t_sig_hand_ptr)cma__sig_deliver


/*
 * LOCAL FUNCTIONS
 */


static void cma___sig_async_term	 (int,int,struct sigcontext *);


static void cma___sig_sync_term  (int,int,struct sigcontext *);


void cma___sig_sync_kill  (int,int,struct sigcontext *);

static void cma__preinit_sig_handler  (int,int,struct sigcontext *);


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_sigaction:  POSIX sigaction function.
 *
 *	This routine allows any thread to establish a handler for
 *	(almost) any synchronous signal.
 *
 *
 *	Note:  as per POSIX, only one thread is awakened per signal delivery.
 *
 *  FORMAL PARAMETERS:
 *
 *	sig -- signal for which the user wishes to establish a handler in
 *		a particular thread.
 *
 *	act -- a sigaction structure containing
 *		    sa_handler	- routine to handle signal
 *		    sa_mask	_ signals to block out while in sa_handler
 *		    sa_flags	- any flags currently set
 *
 *	oact -- old values of act from most recent call to sigaction for the
 *		corresponding thread and signal if any.
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
 *      A 0 return value indicated that the call succeeded.  A -1 return
 *      value indicates an error occurred and errno is set to indicated
 *      the reason.
 *
 *  SIDE EFFECTS:
 *
 *	Establishes handlers for specific signal, removes any previous
 *	handler for this signal.  The user must take care to reestablish
 *	the previous handler if they so wish for it to function at another
 *	time.
 */
extern int
cma_sigaction(
	int sig,
	struct sigaction *act,
	struct sigaction *oact
	)
    {
    cma__t_int_tcb	*tcb;	    /* get tcb to access sigaction array */

    if (!cma__is_multithreaded()) return (sigaction (sig, act, oact));
    cma__int_init ();

     /*
      * Check to see that the signal requested in this sigaction
      * call is not specifically disallowed by our sigaction mask
      * and that it is currently blocked by the process.
      */

    if (sigismember (&cma___g_sigact_block_mask, sig)) 
      return (cma__set_errno (EINVAL), -1);

    tcb = cma__get_self_tcb ();

    /*
     * Fix-me: cma_sigaction cannot be called while within a sigaction
     * handler, else a deadlock occurs.  
     * Question: will other threads be searching/changing this sigaction 
     * list someday??  If so locking must be installed.  Fix-me, perhaps.
     */

    if (oact) {				/* If caller wants old handler info */

	if (tcb->sigaction_data[sig].sa_handler != 0) {
	    oact->sa_handler = tcb->sigaction_data[sig].sa_handler;
	    oact->sa_mask = tcb->sigaction_data[sig].sa_mask;
	    oact->sa_flags = tcb->sigaction_data[sig].sa_flags;
	    }
	else {				/* Must be default */
	    oact->sa_handler = SIG_DFL;
	    sigemptyset (&oact->sa_mask);
	    oact->sa_flags = 0;
	    }

	}

    if (act) {
	tcb->sigaction_data[sig].sa_handler = act->sa_handler;
	tcb->sigaction_data[sig].sa_mask = act->sa_mask;
	tcb->sigaction_data[sig].sa_flags = act->sa_flags;
	}

    return (0);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma_sigwait:  POSIX sigwait function.
 *
 *	This routine allows any thread to wait for any signal (more or less).
 *
 *	The caller must have blocked (masked out) all the signals specified
 *	to be awaited before calling this routine.  These signals are then
 *	reenabled (unblocked) by this routine until the thread is awakened.
 *	If there are no other threads waiting for a specific signal, then,
 *	when a that signal is delivered and the waiting thread is awakened,
 *	that signal is blocked again before the thread returns.
 *
 *	Note:  as per POSIX, only one thread is awakened per signal delivery.
 *
 *  FORMAL PARAMETERS:
 *
 *	set -- address of a mask containing bits set for each signal the thread
 *		is awaiting.
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
 *	Returns the number of the signal actually caught.
 *
 *  SIDE EFFECTS:
 *
 *	This routine messes around with the signal mask.
 *
 *	NOTE:	A thread may not attempt to catch SIGKILL or SIGSTOP.
 */
extern int
cma_sigwait(
	sigset_t    *set)
    {
    cma_t_integer	    i;
    cma_t_boolean	    sigbitset, one_set = cma_c_false;
    _CMA_VOLATILE_ cma___t_sigwait_block   wblk;
    sigset_t		    sig_mask;
    static sigaction_t	    sig_oact[NSIG];

    cma__int_init ();
    cma__int_init2 ();

    /*
     * For each signal, check to see if it is set in the user's mask.  If it is
     * also set in the "illegal values" mask, or if it is not set in the current
     * mask, then return an error.  Also, if there are no signals set in the 
     * user's mask, return an error.
     */


    for (i = 1; i < NSIG; i++) {
        if ((sigbitset = sigismember (set, i))
		&& (sigismember (&cma___g_sig_wait_mask, i)))
	    return (cma__set_errno (EINVAL), -1);

	cma__assert_fail (
		((sigbitset | 1) == 1),	    /* Check for boolean result */
		"cma_sigwait: sigismember error.\n");

	one_set = one_set || sigbitset;
	}

    if (!one_set)   return (cma__set_errno (EINVAL), -1);

    cma__queue_zero (&wblk.queue);
    wblk.set = (sigset_t *)set;		/* copy the pointer */
    wblk.signal = -1;			/* initialize predicate */
    wblk.tcb = cma__get_self_tcb ();
    wblk.cv  = wblk.tcb->tswait_cv;

    /*
     * FIX-ME:  On VMS this should probably block ASTs
     */
    if (sigprocmask (SIG_BLOCK, &cma___g_sig_block_mask, &sig_mask) == -1)
	cma__bugcheck ("sigwait:2");

    cma__int_lock (cma___g_sigwait_mutex);
    cma__queue_insert ((cma__t_queue *)&wblk.queue, &cma___g_sigwait_queue);

    /*
     * Install the appropriate DECthreads signal handler for each signal that 
     * the thread is waiting for, provided that we don't currently have a 
     * handler already in place (i.e., if there are already waiters for a 
     * given signal, then we've already put a handler in place for it).
     */
    for (i = 1; i < NSIG; i++)
	if (sigismember (set, i)) {
	    if (cma___g_sigwaiters[i] == 0) {
		sigaction_t act;


		act.sa_mask = cma___g_sig_block_mask;
		act.sa_flags = 0;
		act.sa_handler = 
		    (i == SIGURG || i == SIGTSTP || i == SIGCONT || 
		    i == SIGCHLD || i == SIGIO || i == SIGWINCH ?
		    (cma___t_sig_hand_ptr)cma___sig_async_nonterm :
		    (cma___t_sig_hand_ptr)cma___sig_async_term);

		if (sigaction (i, &act, &sig_oact[i]) == -1)  
		    cma__bugcheck ("sigwait:3");

		}
	    /*
	     * Keep a count of how many threads are waiting on each signal 
	     * so we know when to flip bits in the signal mask, install/remove
	     * signal handlers, etc.
	     */
	    cma___g_sigwaiters[i]++;
	    }

    cma__int_unlock (cma___g_sigwait_mutex);

    /*
     * Reenable the previously enabled signals, and then reenable the signals
     * that this thread is waiting for.
     *
     * FIX-ME:  On VMS this should probably unblock ASTs
     */
    if (sigprocmask (SIG_SETMASK, &sig_mask, (sigset_t *)cma_c_null_ptr) == -1)
	cma__bugcheck ("sigwait:5");

    if (sigprocmask (SIG_UNBLOCK, set, (sigset_t *)cma_c_null_ptr) == -1)
	cma__bugcheck ("sigwait:6");

    cma__int_lock (wblk.tcb->tswait_mutex);

    TRY {
	if (wblk.signal == -1)
	    while (wblk.signal == -1)
		cma__int_wait (wblk.tcb->tswait_cv, wblk.tcb->tswait_mutex);
	else
	    cma__attempt_delivery (wblk.tcb);
	}
    FINALLY {
	cma__int_unlock (wblk.tcb->tswait_mutex);

	/*
	 * FIX-ME:  On VMS this should probably block ASTs
	 */
	if (sigprocmask (SIG_BLOCK, &cma___g_sig_block_mask, &sig_mask) == -1)
	    cma__bugcheck ("sigwait:7");

	cma__int_lock (cma___g_sigwait_mutex);
	(void)cma__queue_remove (&wblk.queue);

	/*
	 * Leave those signals enabled which still have threads waiting on 
	 * them, and re-block the others in this thread's sigwait set.
	 */
	for (i = 1; i < NSIG; i++)
	    if (sigismember (set, i)) {
		cma___g_sigwaiters[i]--;
		if (cma___g_sigwaiters[i] == 0) {
		    if (sigaddset (&sig_mask, i) == -1)
			cma__bugcheck ("sigwait:8");

		    if (sigaction (
			    i, 
			    &sig_oact[i], 
			    (struct sigaction *)cma_c_null_ptr
			    ) == -1)  
			cma__bugcheck ("sigwait:9");
		    }
		}

	cma__int_unlock (cma___g_sigwait_mutex);

	/*
	 * FIX-ME:  On VMS this should probably unblock ASTs
	 */
	if (sigprocmask (SIG_SETMASK, &sig_mask, (sigset_t *)cma_c_null_ptr) 
		== -1)
	    cma__bugcheck ("sigwait:11");

	}
    ENDTRY

    return wblk.signal;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__abort_process:  abort process execution.
 *
 *  FORMAL PARAMETERS:
 *
 *	Signal to be aborted with
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
 *	Deinstalls signal handlers and causes a terminating signal.
 */
extern void
cma__abort_process(
	cma_t_integer	abort_signal)
    {
    /*
     * We are about to suddenly terminate the process, so make sure that 
     * any pending output makes it.
     */
    (void)fflush (stdout);

    /*
     * Set the signal action to the default for the signal which is gonna
     * kill us.
     */
    signal (abort_signal, SIG_DFL);
    /*
     * Send the killing signal, "na zdarovia".
     */
    if (kill (getpid(), abort_signal) == -1)	/* Kill the process */
	cma__bugcheck("cma__abort_process:  could not send signal %d", abort_signal);

    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__init_signal:  Initialize CMA signal handling
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
 *	Registers signal handlers for (almost) all signals.
 */
extern void
cma__init_signal
	(void)
    {
    cma_t_integer    i;

    cma___g_sigwait_mutex = cma__get_mutex (&cma__g_def_attr);
    cma__obj_set_name (cma___g_sigwait_mutex, "sigwait");
    cma__queue_init  (&cma___g_sigwait_queue);

    for (i = 1; i < NSIG; i++)
	cma___g_sigwaiters[i] = 0;

    /*
     * Mask for disallowed signals for sigwait.
     * SIGKILL and SIGSTOP cannot be caught, and therefore cannot be awaited.
     * The others are "synchronous terminating" signals, for which we are 
     * currently imposing the restriction that they cannot be awaited.  There 
     * is no need for this restriction, we are simply waiting for their use to
     * be justified.
     */
    if (sigemptyset (&cma___g_sig_wait_mask) == -1)
	cma__bugcheck ("init_signal:7");
    if (sigaddset (&cma___g_sig_wait_mask, SIGKILL) == -1)
	cma__bugcheck ("init_signal:8");
    if (sigaddset (&cma___g_sig_wait_mask, SIGSTOP) == -1)
	cma__bugcheck ("init_signal:9");
    if (sigaddset (&cma___g_sig_wait_mask, SIGILL)  == -1)
	cma__bugcheck ("init_signal:10");
    if (sigaddset (&cma___g_sig_wait_mask, SIGTRAP) == -1)
	cma__bugcheck ("init_signal:11");
    if (sigaddset (&cma___g_sig_wait_mask, SIGIOT)  == -1)
	cma__bugcheck ("init_signal:12");
    if (sigaddset (&cma___g_sig_wait_mask, SIGEMT)  == -1)
	cma__bugcheck ("init_signal:13");
    if (sigaddset (&cma___g_sig_wait_mask, SIGFPE)  == -1)
	cma__bugcheck ("init_signal:14");
    if (sigaddset (&cma___g_sig_wait_mask, SIGBUS)  == -1)
	cma__bugcheck ("init_signal:15");
    if (sigaddset (&cma___g_sig_wait_mask, SIGSEGV) == -1)
	cma__bugcheck ("init_signal:16");
    if (sigaddset (&cma___g_sig_wait_mask, SIGSYS)  == -1)
	cma__bugcheck ("init_signal:17");
    if (sigaddset (&cma___g_sig_wait_mask, SIGPIPE) == -1)
	cma__bugcheck ("init_signal:18");
    if (sigaddset (&cma___g_sig_wait_mask, SIGXCPU) == -1)
	cma__bugcheck ("init_signal:19");
    if (sigaddset (&cma___g_sig_wait_mask, SIGXFSZ) == -1)
	cma__bugcheck ("init_signal:20");

    /*
     * Mask for disallowed signals for sigaction.
     * As above SIGKILL AND SIGSTOP cannot be caught, other blocked signals
     * currently consist of asynchronous signals which this 
     * implementation of sigaction does not allow sigaction to handle.
     */

    if (sigemptyset (&cma___g_sigact_block_mask) == -1)
	cma__bugcheck ("init_signal:22");
    if (sigaddset (&cma___g_sigact_block_mask, SIGKILL) == -1)
	cma__bugcheck ("init_signal:23");
    if (sigaddset (&cma___g_sigact_block_mask, SIGSTOP) == -1)
	cma__bugcheck ("init_signal:24");
    if (sigaddset (&cma___g_sigact_block_mask, SIGHUP) == -1) 
	cma__bugcheck ("init_signal:25");
    if (sigaddset (&cma___g_sigact_block_mask, SIGINT) == -1)
	cma__bugcheck ("init_signal:26");
    if (sigaddset (&cma___g_sigact_block_mask, SIGQUIT) == -1)
	cma__bugcheck ("init_signal:27");
    if (sigaddset (&cma___g_sigact_block_mask, SIGALRM) == -1)
	cma__bugcheck ("init_signal:28");
    if (sigaddset (&cma___g_sigact_block_mask, SIGTERM) == -1)
	cma__bugcheck ("init_signal:29");
    if (sigaddset (&cma___g_sigact_block_mask, SIGURG) == -1)
	cma__bugcheck ("init_signal:30");
    if (sigaddset (&cma___g_sigact_block_mask, SIGSTOP) == -1)
	cma__bugcheck ("init_signal:31");
    if (sigaddset (&cma___g_sigact_block_mask, SIGTSTP) == -1)
	cma__bugcheck ("init_signal:32");
    if (sigaddset (&cma___g_sigact_block_mask, SIGCONT) == -1)
	cma__bugcheck ("init_signal:33");
    if (sigaddset (&cma___g_sigact_block_mask, SIGCHLD) == -1)
	cma__bugcheck ("init_signal:34");
    if (sigaddset (&cma___g_sigact_block_mask, SIGTTIN) == -1)
	cma__bugcheck ("init_signal:35");
    if (sigaddset (&cma___g_sigact_block_mask, SIGTTOU) == -1)
	cma__bugcheck ("init_signal:36");
    if (sigaddset (&cma___g_sigact_block_mask, SIGIO) == -1)
	cma__bugcheck ("init_signal:37");
    if (sigaddset (&cma___g_sigact_block_mask, SIGXCPU) == -1)
	cma__bugcheck ("init_signal:38");
    if (sigaddset (&cma___g_sigact_block_mask, SIGVTALRM) == -1) 
	cma__bugcheck ("init_signal:39");
    if (sigaddset (&cma___g_sigact_block_mask, SIGPROF) == -1)
	cma__bugcheck ("init_signal:40");
    if (sigaddset (&cma___g_sigact_block_mask, SIGWINCH) == -1) 
	cma__bugcheck ("init_signal:41");
    if (sigaddset (&cma___g_sigact_block_mask, SIGUSR1) == -1)
	cma__bugcheck ("init_signal:43");
    if (sigaddset (&cma___g_sigact_block_mask, SIGUSR2) == -1)
	cma__bugcheck ("init_signal:44");
    if (sigaddset (&cma___g_sigact_block_mask, SIGCLD) == -1)
	cma__bugcheck ("init_signal:45");
    /*
     * Block all signals during signal delivery, except...
     *   SIGKILL, SIGSTOP, and SIGCONT may not be blocked.
     *   SIGTRAP is used to handle debugger breakpoints.
     *   SIGQUIT is useful for rescuing yourself!
     */
    if (sigfillset (&cma___g_sig_block_mask) == -1)
	cma__bugcheck ("init_signal:1");
    if (sigdelset (&cma___g_sig_block_mask, SIGKILL) == -1)
	cma__bugcheck ("init_signal:2");
    if (sigdelset (&cma___g_sig_block_mask, SIGSTOP) == -1)
	cma__bugcheck ("init_signal:3");
    if (sigdelset (&cma___g_sig_block_mask, SIGCONT) == -1)
	cma__bugcheck ("init_signal:4");
    if (sigdelset (&cma___g_sig_block_mask, SIGTRAP) == -1)
	cma__bugcheck ("init_signal:5");
# ifndef NDEBUG
    if (sigdelset (&cma___g_sig_block_mask, SIGQUIT) == -1)
	cma__bugcheck ("init_signal:6");
# endif

    {
    sigaction_t act;


    act.sa_mask = cma___g_sig_block_mask;
		act.sa_flags = 0;

#ifdef  AIX_CORE_FILE
    /*
     * the default action of the following syncronous signals is
     * to dump core
     */
    if (sigemptyset (&cma___g_sig_core_mask) == -1)
        cma__bugcheck ("init_signal:60");
    if (sigaddset (&cma___g_sig_core_mask, SIGQUIT) == -1)
        cma__bugcheck ("init_signal:61");
    if (sigaddset (&cma___g_sig_core_mask, SIGILL) == -1)
        cma__bugcheck ("init_signal:62");
    if (sigaddset (&cma___g_sig_core_mask, SIGTRAP) == -1)
        cma__bugcheck ("init_signal:63");
    if (sigaddset (&cma___g_sig_core_mask, SIGIOT) == -1)
        cma__bugcheck ("init_signal:64");
    if (sigaddset (&cma___g_sig_core_mask, SIGEMT) == -1)
        cma__bugcheck ("init_signal:65");
    if (sigaddset (&cma___g_sig_core_mask, SIGFPE) == -1)
        cma__bugcheck ("init_signal:65");
    if (sigaddset (&cma___g_sig_core_mask, SIGBUS) == -1)
        cma__bugcheck ("init_signal:66");
    if (sigaddset (&cma___g_sig_core_mask, SIGSEGV) == -1)
        cma__bugcheck ("init_signal:67");
    if (sigaddset (&cma___g_sig_core_mask, SIGSYS) == -1)
        cma__bugcheck ("init_signal:68");
    if (sigaddset (&cma___g_sig_core_mask, SIGPRE) == -1)
        cma__bugcheck ("init_signal:69");
#endif  /* AIX_CORE_FILE */

    for (i = 1; i < NSIG; i++) {

	/*
	 * Timeslice signal 
	 *
	 * (This is handled outside the switch statement, because C doesn't 
	 *  allow multiple duplicate case expressions.)
	 */
	if (i == cma__c_timer_signal) {
	    {
	    static struct sigstack vtalrm_sigstack;

	    vtalrm_sigstack.ss_sp =
			(caddr_t)((unsigned int)(&ibmr2_vtalrm_stack_area[8184]) & 0xfffffff8);
	    vtalrm_sigstack.ss_onstack  = 0;

	    sigstack(&vtalrm_sigstack, NULL);

	    act.sa_flags |= SA_ONSTACK;
	    }
	    act.sa_handler = cma__periodic;
	    if (sigaction (i, &act, cma_c_null_ptr) == -1)
		cma__bugcheck ("init_signal:46");


	    act.sa_flags = 0;
	    continue;	/* Skip over the switch statement for this iteration */
            }

	switch (i) {

	    /*
	     * Asynchronous Terminating Signals
	     */
            case SIGHUP:
            case SIGINT:
            case SIGALRM:
            case SIGTERM:
            case SIGVTALRM:
            case SIGPROF:
            case SIGUSR1:
            case SIGUSR2:
            case SIGDANGER:
                {
		/*
		 * These handlers are now installed as needed by sigwait().
		 */
		break;
                }

	    /*
	     * Asynchronous Non-terminating Signals
	     */
            case SIGCONT:
            case SIGCHLD:
            case SIGWINCH:
                {
		/*
		 * These handlers are now installed as needed by sigwait().
		 */
		break;
                }

	    /*
	     * Synchronous Terminating Signals
	     */
            case SIGILL:
            case SIGIOT:
            case SIGEMT:
            case SIGFPE:
            case SIGBUS:
            case SIGSEGV:
#ifdef	AIX_CORE_FILE
	    case SIGTRAP:
	    case SIGSYS:
	    /*
	     * add these async signals, they also dump core
	     */
	    case SIGQUIT:
	    case SIGPRE:
		{
		act.sa_flags &= ~(SA_NODUMP);
		act.sa_flags |= SA_FULLDUMP;
		act.sa_handler = (cma___t_sig_hand_ptr)cma___sig_sync_term;
		if (sigaction (i, &act, cma_c_null_ptr) == -1)
			cma__bugcheck ("init_signal:70");
		break;
		}
#else
		/*
		 * If the _CMA_SYNC_KILL_ option is set, then most syncronous
		 * terminating signals will directly abort the process with a
		 * core dump. NOTE: it is NOT intended that DECthreads be
		 * built with this option enabled. Rather, the code is here
		 * as a placeholder so that a cma_debug() command can
		 * eventually be implemented to switch handling of these
		 * signals from normal exception to abort-process behavior.
		 */
		{
		static char		crash_stack_area[8192];
		static struct sigstack	crash_sigstack;

		if (!crash_sigstack.ss_sp) {
		    crash_sigstack.ss_sp =
			&crash_stack_area[sizeof(crash_stack_area)];

		    sigstack(&crash_sigstack, NULL);
		    }

		act.sa_flags |= SV_ONSTACK;
		act.sa_handler = cma___sig_sync_kill;

		if (sigaction (i, &act, cma_c_null_ptr) == -1)
		    cma__bugcheck ("init_signal:48.5");

		act.sa_flags &= ~SV_ONSTACK;
		break;
		}

            case SIGSYS:
#endif	/* AIX_CORE_FILE */

            case SIGPIPE:
            case SIGXCPU:
            case SIGXFSZ:
                {
		act.sa_handler = (cma___t_sig_hand_ptr)cma___sig_sync_term;
		if (sigaction (i,&act,(struct sigaction *)cma_c_null_ptr) == -1)
		    cma__bugcheck ("init_signal:49");
		break;
                }

	    /*
	     * Synchronous Non-Terminating Signals
	     */
            case SIGTTIN:
            case SIGTTOU:
                {
		act.sa_handler = cma___sig_sync_nonterm;
		if (sigaction (i,&act,(struct sigaction *)cma_c_null_ptr) == -1)
		    cma__bugcheck ("init_signal:50");
		break;
                }

	    /*
	     * I/O Signals.  CMA has special handlers for these.
	     */
            case SIGURG:
            case SIGIO:
                {
		/*
		 * These handlers are now installed as needed by sigwait().
		 */
		break;
                }

	    /*
	     * Signals which CMA does not catch.
	     */
            case SIGKILL:	/* Cannot be caught or ignored. */
            case SIGSTOP:	/* Cannot be caught or ignored. */
#ifndef	AIX_CORE_FILE
	    /*
	     * these signals have been added to the signals that dump core
	     */
	    case SIGTRAP:	/* Don't mess with debugger bpts. */
            case SIGQUIT:	/* Preserve current behavior. */
            case SIGPRE:
#endif	/* AIX_CORE_FILE */
            case SIGTSTP:
            case 26:           /* Don't catch reserved signals */
            case 37:
            case 38:
            case 39:
            case 40:
            case 41:
            case 42:
            case 43:
            case 44:
            case 45:
            case 46:
            case 47:
            case 48:
            case 49:
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
            case 56:
            case 57:
            case 58:
            case 59:
	    case SIGGRANT:
            case SIGRETRACT:
            case SIGSOUND:
            case SIGSAK:
            case SIGMIGRATE:
            case SIGMSG:
            case SIGPWR:
                {
		/* 
		 * Do nothing for these signals.
		 */
		break;
                }

	    /*
	     * All Other Signals
	     *
	     * All signals should be accounted for above:  this case should
	     * never occur.
	     */
            default:
		{
                cma__bugcheck ("init_signal:52");
		break;
		}
	    }
	}

	/* change core dump signals to produce a full core file */
#ifdef	AIX_CORE_FILE
	for (i = 1; i < NSIG; i++) {
		switch (sigismember (&cma___g_sig_core_mask, i)) {
			case 1:
				if (sigaction (i, 0, &act) == -1)
					cma__bugcheck ("init_signal:55");
				act.sa_flags |= SA_FULLDUMP;
				if (sigaction (i, &act, 0) == -1)
					cma__bugcheck ("init_signal:56");
			case 0:
				break;
			default:
				cma__bugcheck ("init_signal:57");
		}
	}
#else
	for (i = SIGQUIT; i < SIGPIPE; i++) {
		if (i == SIGKILL)
			continue;
		if (sigaction (i, 0, &act) == -1)
			cma__bugcheck ("init_signal:55");
		act.sa_flags |= SA_FULLDUMP;
		if (sigaction (i, &act, 0) == -1)
			cma__bugcheck ("init_signal:56");
	}
#endif	/* AIX_CORE_FILE */
    }
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__reinit_signal:  Do pre/post re-Initialize 
 *
 *  FORMAL PARAMETERS:
 *
 *	flag
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
cma__reinit_signal
	(
	cma_t_integer	    flag)
    {
    cma_t_integer   i;

    if (flag == cma__c_reinit_prefork_lock) {
	cma__int_lock (cma___g_sigwait_mutex);
	}

    if (flag == cma__c_reinit_postfork_unlock) {
	cma__int_unlock (cma___g_sigwait_mutex);
	}

    if (flag == cma__c_reinit_postfork_clear) {
	cma__queue_init	(&cma___g_sigwait_queue);

	for (i = 1; i < NSIG; i++)
	    cma___g_sigwaiters[i] = 0;

	}
	
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma__sig_deliver:  routine to perform signal "delivery" to sigwait'd
 *	threads and to perform calls to any sigaction handlers in current
 *	thread.
 *
 *	This routine is enabled as a signal handler for most of the signals.
 *
 *  FORMAL PARAMETERS:
 *
 *	sig  -- integer representing the number of the signal
 *	code -- integer representing code for addition hardware information
 *	scp  -- pointer to signal context buffer
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
 *	Boolean value (in case we were called as a function instead of a
 *	signal handler) which indicates that we handled the function (i.e.,
 *	that no additional handling is required).
 *
 *  SIDE EFFECTS:
 *
 *	Threads waiting for this signal are awakened.
 *
 *	NOTE:  This routine assumes that ALL signals are blocked by the system
 *		signal delivery mechanism (ie, which calls this routine).
 *
 *	NOTE:  This routine assumes that it is the only thread running.  We
 *		require mutual exclusion to delete from a queue, but locking a
 *		mutex in a signal handler is problematical.
 */
extern cma_t_boolean
cma__sig_deliver(
	int		    sig,
	int		    code,
	struct sigcontext   *scp)
    {
    cma___t_sigwait_block   *wblk;
    cma__t_int_tcb	    *tcb = cma__get_self_tcb ();
    cma_t_boolean	    resend = cma_c_false;


    if (tcb->sigaction_data[sig].sa_handler == SIG_IGN) {
	return cma_c_true;  /* Ignore this signal */
	}
    else if ((tcb->sigaction_data[sig].sa_handler != 0)
	     && (tcb->sigaction_data[sig].sa_handler != SIG_DFL)) 
	{
	cma___t_sighandler handler = 
	    (cma___t_sighandler)tcb->sigaction_data[sig].sa_handler;
	(*handler)(sig, code, scp);
	return cma_c_true;
	}

/*
 * FIX-ME: make sure sigaction_data is cleared from the tcb upon detach!!
 */
    
#ifdef _CMA_SIGNAL_TRACE_
	{
	char		output[cma__c_buffer_size];


	output[0] = '\0';
	cma__putstring (output, "Signal ");
	cma__putstring (output, " (#");
	cma__putint    (output, sig);
	cma__putstring (output, ") occurred in thread 0x");
	cma__puthex    (output, cma__get_self_tcb());

	if (code != 0) {
	    cma__putstring  (output, " with code 0x");
	    cma__puthex     (output, code);
	    }
	cma__putstring (output, " at PC 0x");
	cma__puthex    (output, scp->sc_pc);
	cma__putstring (output, ".");
	cma__puteol    (output);
	}
#endif

    wblk = (cma___t_sigwait_block *)cma__queue_next (&cma___g_sigwait_queue);
    while (wblk != (cma___t_sigwait_block*)&cma___g_sigwait_queue
	    && !(sigismember(wblk->set, sig) && (wblk->signal == -1))) {

	/*
	 * Check to see if this waiter is looking for this signal but is
	 * already processing another one.
	 */
	if (sigismember(wblk->set, sig) && (wblk->signal != -1))
	    resend = cma_c_true;

	wblk = (cma___t_sigwait_block *)cma__queue_next (&wblk->queue);
	}

    if (wblk != (cma___t_sigwait_block*)&cma___g_sigwait_queue) {
	wblk->signal = sig;
        cma__int_signal_int (wblk->cv);
	if (io_avail_null_thread) {
	    io_avail_null_thread = 0; /* in case another signal comes in */
	    if (sigsetmask(scp->sc_mask) == -1)
		cma__bugcheck("sig_deliver:1");
	    exc_raise(&io_avail_exc);
	}
	else
	    return (cma_c_true);
	}
    else
	if (resend) {
	    /*
	     * We couldn't find a thread to wake up for this signal.  We 
	     * found a thread which could have handled this signal, but
	     * it was "in the process" of handling another signal so it 
	     * couldn't handle this one.  So, since we've now determined
	     * that there are no threads prepared to receive this signal
	     * modify the scp so that this signal will be blocked when 
	     * we return from processing it.  And then send this signal
	     * to us again so it will be pending the next time someone
	     * looks for it.  Oh, and return a "true" from this routine
	     * to say that we took care of this signal, so it doesn't
	     * terminate the process.
	     */
	    SIGADDSET(scp->sc_mask, sig);
	    if (kill (getpid(), sig) == -1) cma__abort();
	    return cma_c_true;
	    }

#ifdef	AIX_CORE_FILE
    if (sigismember (&cma___g_sig_core_mask, sig) &&
   		(tcb->sigaction_data[sig].sa_handler == SIG_DFL))
	cma__core (sig, code, scp);
#else
	else
#endif  /* AIX_CORE_FILE */

	    return cma_c_false;
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___sig_sync_kill:  Signal handler routine for synchronous,
 *	(process) terminating signals.
 *
 *	This routine is enabled as a signal handler.
 *
 *  FORMAL PARAMETERS:
 *
 *	sig  -- integer representing the number of the signal code -- integer
 *	representing code for addition hardware information scp  -- pointer
 *	to signal context buffer
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
 *	An exception is raised in the calling thread. Kill the entire process
 *	by re-issuing the signal. This signal is trapped on a separate stack
 *	in order to avoid bad $sp on stack overflow.
 *
 *	NOTE:  This routine assumes that ALL signals are blocked by the
 *	system signal delivery mechanism (ie, which calls this routine).
 */
void
cma___sig_sync_kill(
	int		    sig,
	int		    code,
	struct sigcontext   *scp)
    {
    sigaction_t act;
    cma__t_int_tcb	*self;

    self = cma__get_self_tcb ();    
    if (cma__sig_deliver (sig, code, scp)) return;

    /*
     * Dump core 
     */
    act.sa_mask = cma___g_sig_block_mask;

    act.sa_flags = SV_ONSTACK | SA_FULLDUMP;
    act.sa_handler = SIG_DFL;

    if (sigaction (sig, &act, cma_c_null_ptr) == -1)
        cma__bugcheck ("sig_sync_kill:1");

    kill (getpid(), sig);

    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___sig_async_term:  Signal handler routine for asynchronous,
 *	(process) terminating signals.
 *
 *	This routine is enabled as a signal handler.
 *
 *  FORMAL PARAMETERS:
 *
 *	sig  -- integer representing the number of the signal
 *	code -- integer representing code for addition hardware information
 *	scp  -- pointer to signal context buffer
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
 *	Any thread which is sigwait'ing for this signal is awakened.
 *	If no threads are waiting, the main thread is forced to exit.
 *
 *	NOTE:  This routine assumes that ALL signals are blocked by the system
 *		signal delivery mechanism (ie, which calls this routine).
 */
static void
cma___sig_async_term(
	int		    sig,
	int		    code,
	struct sigcontext   *scp)
    {
    cma__t_int_tcb	*self;
    /*
     * Inform any threads which are sigwait'ing for this signal.
     *
     * If there is at least one waiting thread, simply return.
     */
    if (cma__sig_deliver (sig, code, scp)) return;
    /*
     * Make an attempt to lock the kernel.
     *
     * FIX-ME: This continues regardless of whether it acquires the kernel or
     * some other thread has it.  In an MP implementation, this is surely
     * wrong unless we stop other threads first!  But we can't use a regular
     * cma__enter_kernel(), since this is a signal handler, and the current
     * thread might already own the kernel (there's no way to tell).
     */
    self = cma__get_self_tcb ();
    (void)cma__tryenter_kernel ();

    if (self == &cma__g_def_tcb)	/* If this is main thread */
	cma__abort_process(sig);
    else {
	/*
	 * Abort the process by resending the signal (after some preparation).
	 */
	cma__abort_process (sig);
	}
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	cma___sig_sync_term:  Signal handler routine for synchronous,
 *	(process) terminating signals.
 *
 *	This routine is enabled as a signal handler.
 *
 *  FORMAL PARAMETERS:
 *
 *	sig  -- integer representing the number of the signal
 *	code -- integer representing code for addition hardware information
 *	scp  -- pointer to signal context buffer
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
 *	True if a thread was waiting for this signal.
 *
 *  SIDE EFFECTS:
 *
 *	An exception is raised in the calling thread.
 *	Any thread which is sigwait'ing for this signal is awakened.
 *
 *	NOTE:  This routine assumes that ALL signals are blocked by the system
 *		signal delivery mechanism (ie, which calls this routine).
 */
static void
cma___sig_sync_term(
	int		    sig,
	int		    code,
	struct sigcontext   *scp)
    {
    cma_t_integer	exception_status;    

    /*
     * Inform any threads which are sigwait'ing for this signal.
     */
    if ((cma__sig_deliver (sig, code, scp))) return;
    switch (sig) {
	case SIGILL:
            {
		exception_status = exc_s_illinstr;
		break;
	    }
        case SIGTRAP:
            {
	    exception_status = exc_s_SIGTRAP;
	    break;
	    }
        case SIGIOT:
            {
	    exception_status = exc_s_SIGIOT;
	    break;
	    }
        case SIGEMT:
            {
	    exception_status = exc_s_SIGEMT;
	    break;
	    }
        case SIGFPE:
            {
	    exception_status = exc_s_aritherr;
	    break;
	    }

        case SIGBUS:
        case SIGSEGV:
            {
	    exception_status = exc_s_illaddr;
	    break;
	    }
        case SIGSYS:
            {
	    exception_status = exc_s_SIGSYS;
	    break;
	    }
        case SIGPIPE:
            {
	    exception_status = exc_s_SIGPIPE;
	    break;
	    }
        case SIGXCPU:
            {
	    exception_status = exc_s_excpu;
	    break;
	    }
        case SIGXFSZ:
            {
	    exception_status = exc_s_exfilsiz;
	    break;
	    }
        default:
	    {
            cma__bugcheck ("sig_sync_term:1");
	    break;
	    }
	}

    /*
     * Because DECthreads uses _setjmp() & _longjmp() for performance,
     * delivering the exception (via _longjmp()) will not restore the
     * previous signal mask; this would leave signals permanently disabled.
     * To work around the problem without requiring the use of the much
     * slower setjmp() on every TRY block, we'll just manually restore the
     * signal mask from the context block before raising the exception.
     */
    if (sigprocmask (
	    SIG_SETMASK,
	        (sigset_t *)&scp->sc_mask,
	    (sigset_t *)cma_c_null_ptr)	== -1)
	cma__bugcheck ("sig_sync_term:2");

    cma__error (exception_status);
    }


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__preinit_signal:  Pre-Initialize CMA signal handling. This
 *      routine sets signal handlers for signals that need to be propogated
 *      as exceptions. It is called in single-threaded client mode only.
 *
 *  FORMAL PARAMETERS:
 *
 *      None
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
 *      Registers signal handlers for signals that are returned as
 *      exceptions.
 */
extern void
cma__preinit_signal(void)
    {
    sigaction_t act;
    /*
     * Block all signals during signal delivery, except...
     *   SIGKILL, SIGSTOP, and SIGCONT may not be blocked.
     *   SIGTRAP is used to handle debugger breakpoints.
     *   SIGQUIT is useful for rescuing yourself!
     */
    if (sigfillset (&cma___g_sig_block_mask) == -1)
        cma__bugcheck ("preinit_signal:1");
    if (sigdelset (&cma___g_sig_block_mask, SIGKILL) == -1)
        cma__bugcheck ("preinit_signal:2");
    if (sigdelset (&cma___g_sig_block_mask, SIGSTOP) == -1)
        cma__bugcheck ("preinit_signal:3");
    if (sigdelset (&cma___g_sig_block_mask, SIGCONT) == -1)
        cma__bugcheck ("preinit_signal:4");
    if (sigdelset (&cma___g_sig_block_mask, SIGTRAP) == -1)
        cma__bugcheck ("preinit_signal:5");
# ifndef NDEBUG
    if (sigdelset (&cma___g_sig_block_mask, SIGQUIT) == -1)
        cma__bugcheck ("preinit_signal:6");
# endif

    act.sa_mask = cma___g_sig_block_mask;
    act.sa_flags = 0;
    act.sa_handler = (cma___t_sig_hand_ptr)cma__preinit_sig_handler;
    if (sigaction (SIGPIPE,&act,(struct sigaction *)cma_c_null_ptr) == -1)
        cma__bugcheck ("preinit_signal:7");
    if (sigaction (SIGSYS,&act,(struct sigaction *)cma_c_null_ptr) == -1)
        cma__bugcheck ("preinit_signal:8");
    if (sigaction (SIGXCPU,&act,(struct sigaction *)cma_c_null_ptr) == -1)
        cma__bugcheck ("preinit_signal:9");
    if (sigaction (SIGXFSZ,&act,(struct sigaction *)cma_c_null_ptr) == -1)
        cma__bugcheck ("preinit_signal:10");
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *      cma__preinit_sig_handler:  Signal handler routine for synchronous,
 *      (process) terminating signals that should be propogated as exceptions.
 *
 *      This routine is enabled as a signal handler.
 *
 *  FORMAL PARAMETERS:
 *
 *      sig  -- integer representing the number of the signal
 *      code -- integer representing code for addition hardware information
 *      scp  -- pointer to signal context buffer
 *
 *  IMPLICIT INPUTS:
 *
 *      none
 *
 *  IMPLICIT OUTPUTS:
 *
 *      none
 *
 *  FUNCTION VALUE:
 *
 *
 *  SIDE EFFECTS:
 *
 *      An exception is raised in the calling thread.
 *
 *      NOTE:  This routine assumes that ALL signals are blocked by the system
 *              signal delivery mechanism (ie, which calls this routine).
 */
static void
cma__preinit_sig_handler (
        int                 sig,
        int                 code,
        struct sigcontext   *scp)
    {
    cma_t_integer       exception_status;

    switch (sig) {
        case SIGPIPE:
            {
            exception_status = exc_s_SIGPIPE;
            break;
            }
        case SIGSYS:
            {
            exception_status = exc_s_SIGSYS;
            break;
            }
        case SIGXCPU:
            {
            exception_status = exc_s_SIGXCPU;
            break;
            }
        case SIGXFSZ:
            {
            exception_status = exc_s_SIGXFSZ;
            break;
            }
        default:
            {
            cma__bugcheck ("preinit_sig_handler:1");
            break;
            }
        }

#if __hpux1010p
    if (sigprocmask (
            SIG_SETMASK,
                &scp->sc_sx.sl_uc.uc_sigmask,
            (sigset_t *)cma_c_null_ptr) == -1)
        cma__bugcheck ("preinit_sig_handler:2");
#else
    if (sigprocmask (
            SIG_SETMASK,
                (sigset_t *)&scp->sc_mask,
            (sigset_t *)cma_c_null_ptr) == -1)
        cma__bugcheck ("preinit_sig_handler:2");
#endif  /* if __hpux1010p */

    cma__error (exception_status);
    }
