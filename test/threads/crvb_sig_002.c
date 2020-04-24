/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: crvb_sig_002.c,v $
 * Revision 1.1.98.2  1996/02/17  23:30:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:06  marty]
 *
 * Revision 1.1.98.1  1995/12/11  22:59:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:25  root]
 * 
 * Revision 1.1.95.1  1994/04/01  20:09:51  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:39  jd]
 * 
 * Revision 1.1.93.1  1993/10/12  19:35:32  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:05  rhw]
 * 
 * Revision 1.1.8.2  1993/06/14  20:16:04  sommerfeld
 * 	HP-UX doesn't have SIGXCPU, SIGXFSZ, sys_siglist[]
 * 	[1993/06/10  23:22:00  sommerfeld]
 * 
 * Revision 1.1.4.3  1992/12/30  16:31:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:07  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/29  20:38:14  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:29  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:22:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CRVB_SIG_002
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	This program tests the CMA implementation of the POSIX
 *	'sigwait' routine.
 *	
 *	The main thread creates several subordinate threads which 
 *	wait for specific subsets of the signals.  It then sends
 *	various signals to the process, and the threads verify that
 *	the correct actions result.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	23 March 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	002	Webb Scales	30 October 1990
 *		CMA no longer handles TSTP, by default.
 *		You can only sigwait for asynchronous, terminating signals.
 *	003	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 *	004	Dave Butenhof	24 September 1991
 *		Remove #include of cma_errno.h; cma.h pulls it in
 *		automatically on uniprocessor platforms, and it won't work
 *		on kernel-thread platforms.
 *	005	Dave Butenhof	14 October 1991
 *		Fix list of uncaught signals for OSF-based system (SIGINFO).
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <signal.h>
#include <errno.h>

#if _CMA_OSIMPL_ == _CMA__OS_OSF
# define platform_not_caught(sig)	(sig == SIGINFO)
#elif _CMA_VENDOR_ == _CMA__HP
# define platform_not_caught(sig)	(sig == _SIGRESERVE)
#elif _CMA_VENDOR_ == _CMA__IBM
# define platform_not_caught(sig) \
	((sig == 26) || (sig >= 37 && sig <= 59) \
	|| (sig == SIGGRANT) || (sig == SIGRETRACT) || (sig == SIGSOUND) \
	|| (sig == SIGSAK) || (sig == SIGMIGRATE) || (sig == SIGPRE) \
	|| (sig == SIGMSG) || (sig == SIGPWR) || (sig == SIGDANGER))
#else
# define platform_not_caught(sig)	0
#endif

#define is_not_caught(sig)	\
	((sig == SIGKILL) || (sig == SIGSTOP) || (sig == SIGTSTP) \
	|| (sig == SIGTRAP) || platform_not_caught(sig))

#ifdef __hpux
#define is_sync_term(sig)	(	\
	(sig == SIGILL)  || \
	(sig == SIGIOT)  || \
	(sig == SIGABRT) || \
	(sig == SIGEMT)  || \
	(sig == SIGFPE)  || \
	(sig == SIGBUS)  || \
	(sig == SIGSEGV) || \
	(sig == SIGSYS)  || \
	(sig == SIGPIPE))
#else
#define is_sync_term(sig)	(	\
	(sig == SIGILL)  || \
	(sig == SIGIOT)  || \
	(sig == SIGABRT) || \
	(sig == SIGEMT)  || \
	(sig == SIGFPE)  || \
	(sig == SIGBUS)  || \
	(sig == SIGSEGV) || \
	(sig == SIGSYS)  || \
	(sig == SIGPIPE) || \
	(sig == SIGXCPU) || \
	(sig == SIGXFSZ))
#endif

#define is_time_slicer(sig)	(sig == SIGVTALRM)


#if _CMA_UNIX_TYPE == _CMA__SVR4
extern char *_sys_siglist[];
#else
# ifndef __hpux
extern char *sys_siglist[];
# endif
#endif


static cma_t_address
thread	(cma_t_address sig_num)
    {
    sigset_t	    set;
    cma_t_integer   sig, sw_sig = -2;


    sig = (cma_t_integer)sig_num;

    if (sigemptyset (&set) == -1)  cts_failed ("sigemptyset error");
    if (sigaddset (&set, sig) == -1)  cts_failed ("sigaddset error");
    if (sigprocmask (SIG_BLOCK, &set, (sigset_t *)cma_c_null_ptr) == -1)  
	cts_failed ("sigprocmask (block) error");

    TRY {
        sw_sig = sigwait (&set);

	if (sw_sig == sig)
	    printf ("  S: OK, #%d\t", sw_sig);
	else if (sw_sig == -1)
		if (is_not_caught(sig) || is_sync_term(sig))
		    printf ("  S: OK, err\t");
		else
		    cts_failed ("\nUnexpected error returned: %d", errno);
	    else {
		printf ("  S: Awaited #%d, Returned #%d\n", sig, sw_sig);
		cts_failed ("Wrong signal number returned from sigwait.");
		}
        }
    CATCH_ALL {
        cts_failed ("\nUnexpected exception in sub-thread.");
        }
    ENDTRY

    if (sigprocmask (SIG_UNBLOCK, &set, (sigset_t *)cma_c_null_ptr) == -1)  
	cts_failed ("sigprocmask (unblock) error.");

    return (cma_t_address)sw_sig;
    }


main ()
    {
    cma_t_thread	th;
    cma_t_integer	i, sig, pid;
    sigset_t            o_sigmask, c_sigmask;
    cma_t_exit_status	status;
    cma_t_address	result;
    cma_t_boolean       waiting, alerted, except;


    cts_test ("CRVB_SIG_002", "Test sigwait");

    pid = getpid ();

    /* Save sigmask *before* CMA inits */
    if (sigprocmask(SIG_BLOCK, (sigset_t *)cma_c_null_ptr, &o_sigmask) == -1)
	cts_failed ("sigprocmask (fetch) error");

    cma_init ();

    for (i = 1; i <= 2; i++) {
	printf ("\nRun #%d:\n", i);
        printf ("Signal\t\tMeaning\t\t\t    Thread\t  Main\n");
        printf ("======\t==========================\t==============\t========\n");

        for (sig = 1; sig < NSIG; sig++) {
	    int j;

#if _CMA_UNIX_TYPE == _CMA__SVR4
	    printf ("  %2d\t %-24s\t", sig, _sys_siglist[sig]);
#else
# ifndef __hpux
	    printf ("  %2d\t %-24s\t", sig, sys_siglist[sig]);
# else
	    printf ("  %2d\t %-24s\t", sig, "");
# endif
#endif


	    /*
	     * Check the current process signal mask
	     */
	    if (sigprocmask(SIG_BLOCK, (sigset_t *)cma_c_null_ptr, &c_sigmask) == -1)
		cts_failed ("sigprocmask error on signal %d", sig);

	    for (j = 1; j < NSIG; j++)
		if (sigismember (&c_sigmask, j) != sigismember (&o_sigmask, j))
		    cts_failed ("Modified signal currently set.");

	    /*
	     * FIX-ME:  for the present, skip the signal used for timeslicing 
	     */
	    if (is_time_slicer(sig)) {
	        printf ("(Time Slice Signal [skipped].)\n");
		continue;
	        }

	    /*
	     * If it's a signal we can test, create a thread to sigwait on
	     * it, and send the signal.
	     */
	    if (!is_not_caught(sig) && !is_sync_term(sig)) {
		/*
		 * Create a thread to send the signal to and see what
		 * happens.
		 */
		cma_thread_create (
			&th,
			&cma_c_null,
			thread,
			(cma_t_address)sig);

		/*
		 * Allow the thread to call sigwait.
		 */
		cma_yield ();
	    
		/*
		 * Signal the thread
		 */
	        TRY {
		    if (kill (pid, (int)sig) == -1) {
		        cts_failed ("Error in 'kill'.");
			perror ("Error message from 'kill'");
		        }
		    }
		CATCH_ALL {
		    cts_failed ("Unexpected exception during kill.");
		    }
		ENDTRY

		/*
		 * Wait for it to complete.
		 */
		waiting = cma_c_true;
		alerted = cma_c_false;
		except  = cma_c_false;

		while (waiting) {

		    TRY {
			cma_thread_join (
				&th,
				&status,
				&result);

			waiting = cma_c_false;
			}
		    CATCH (cma_e_alerted) {
			alerted = cma_c_true;
			}
		    CATCH_ALL {
			except = cma_c_true;
			}
		    ENDTRY
		    }

		if (alerted)
		    cts_failed ("Unexpected alert in main thread.");
		else if (except)
		    cts_failed ("Unexpected exception in main thread.");
		else if (status != cma_c_term_normal)
		    cts_failed ("Unexpected bad sub-thread terminatation.");
		else
		    printf (" M: OK.\n");

		cma_thread_detach (&th);
		}
	    else {

		if (is_sync_term (sig))
		    printf ("(synch. term.)\n");

		if (is_not_caught (sig))
		    printf ("(not caught by DECthreads)\n");

		}

            }

        }

    printf ("\n");

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_002.C */
/*  *12   14-OCT-1991 13:48:53 BUTENHOF "Fix problems" */
/*  *11   24-SEP-1991 16:37:23 BUTENHOF "Remove cma_errno.h" */
/*  *10   29-MAY-1991 18:02:22 BUTENHOF "Fix compilation errors" */
/*  *9     1-MAR-1991 11:46:22 BUTENHOF "Fix signal tests for RIOS" */
/*  *8    30-OCT-1990 19:57:05 SCALES "Update for TSTP, etc." */
/*  *7     4-SEP-1990 16:59:45 SCALES "Change filename references" */
/*  *6     4-SEP-1990 16:01:21 SCALES "Convert to new CMA interfaces" */
/*  *5     3-JUL-1990 12:04:46 BUTENHOF "Change copyright" */
/*  *4    21-JUN-1990 08:31:06 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *3     7-JUN-1990 11:37:55 SCALES "Updates to CTS" */
/*  *2     6-JUN-1990 19:07:06 SCALES "Must block signals before sigwaiting, now." */
/*  *1    17-MAY-1990 13:58:27 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_002.C */
