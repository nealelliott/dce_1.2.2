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
 * $Log: crvb_sig_001.c,v $
 * Revision 1.1.17.2  1996/02/17  23:30:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:05  marty]
 *
 * Revision 1.1.17.1  1995/12/11  22:59:33  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/12/06  17:37 UTC  psn
 * 	Fix the test failure so that SIGSYS that is terminating signal is not sent.
 * 	[1995/12/11  22:35:24  root]
 * 
 * Revision 1.1.14.1  1994/04/01  20:09:49  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:34  jd]
 * 
 * Revision 1.1.12.1  1993/10/12  19:35:30  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:04  rhw]
 * 
 * Revision 1.1.10.2  1993/06/14  20:15:33  sommerfeld
 * 	Don't use sys_siglist on HP-UX
 * 	HP-UX doesn't support SIGXCPU, SIGXFSZ
 * 	[1993/06/10  23:21:41  sommerfeld]
 * 
 * Revision 1.1.6.4  1993/03/17  19:17:52  jd
 * 	Fixed a merge problem and check SIGPIPE for raising an exception.
 * 	[1993/03/17  14:58:50  jd]
 * 
 * Revision 1.1.6.3  1992/12/30  16:31:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:01  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  20:38:08  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:23  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/11  17:16:26  bolinger
 * 	Fix OT defect 2895: add test for whether CMA was built with
 * 	_CMA_SYNC_KILL_ enabled.  If so, this test is partially
 * 	dummied out.
 * 	[1992/05/11  13:48:30  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:41  devrcs
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
 *	CRVB_SIG_001
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	This program tests the builtin signal handling in DECthreads.
 *
 *	The main thread forks a sub-thread which sends a signal to the
 *	process.  There are four types of signals: asynchronous (with
 *	respect to the executing thread) or synchronous, and terminating
 *	and nonterminating.
 *
 *	Nonterminating signals are "noted" by CMA but no explicit 
 *	action is taken.  Synchronous-terminating signals are mapped to
 *	exceptions and raised in the current thread, since, in general,
 *	it was the current thread which caused the signal.  Asynchronous
 *	signals are caught and cause the signal to be sent to the main
 * 	thread, which causes the process to terminate.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	22 March 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	002	Webb Scales	30 October 1990
 *		CMA no longer handles TSTP, by default.
 *	003	Dave Butenhof	27 December 1990
 *		Fix exception names.
 *	004	Dave Butenhof	22 January 1991
 *		Fix exception names again.
 *	005	Dave Butenhof	27 February 1991
 *		Modify it to handle different AIX signal numbers without
 *		blowing up.
 *	006	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 *	007	Dave Butenhof	19 September 1991
 *		Integrate Apollo CMA5 reverse drop: like AIX, SIGLOST and
 *		SIGIOT are synonyms for SIGABRT, so don't test SIGLOST
 *		separately. (A quick check shows that this also applies to
 *		all OSF/1-based systems).
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <signal.h>


int	pid;
#if _CMA_UNIX_TYPE == _CMA__SVR4
extern char *_sys_siglist[];
#else
# ifndef __hpux
extern char *sys_siglist[];
# endif
#endif


#ifndef SIGCLD
# define cond_sigcld
#else
# define cond_sigcld	(sig == SIGCLD) ||
#endif

#if (_CMA_UNIX_TYPE != _CMA__SVR4)
# define is_async_nonterm(sig)	(	\
	(sig == SIGCONT)  || \
	(sig == SIGCHLD)  || \
	cond_sigcld \
	(sig == SIGWINCH) || \
	(sig == SIGURG)   || \
	(sig == SIGIO))
#else
# define is_async_nonterm(sig)	(	\
	(sig == SIGCONT)  || \
	(sig == SIGCHLD)  || \
	(sig == SIGPWR)  || \
	cond_sigcld \
	(sig == SIGWINCH) || \
	(sig == SIGURG))
#endif

#if (_CMA_PLATFORM_ == _CMA__IBMR2_UNIX) || (_CMA_VENDOR == _CMA__APOLLO) || (_CMA_OSIMPL_ == _CMA__OS_OSF) || (_CMA_UNIX_TYPE == _CMA__SVR4)
/*
 * On some platforms, SIGLOST and SIGIOT are synonyms for SIGABRT; having the
 * same signal listed in different signal classes would result in an error
 * from the testing code; so remove the SIGLOST signal from the list.
 */
# define cond_siglost
#else
# define cond_siglost	(sig == SIGLOST) ||
#endif

#if (_CMA_UNIX_TYPE != _CMA__SVR4)
# define is_async_term(sig)	(	\
       (sig == SIGHUP)    || \
       (sig == SIGINT)    || \
       (sig == SIGALRM)   || \
       (sig == SIGTERM)   || \
       (sig == SIGVTALRM) || \
       (sig == SIGPROF)   || \
       cond_siglost	     \
       (sig == SIGUSR1)   || \
       (sig == SIGUSR2))
#else
# define is_async_term(sig)	(	\
       (sig == SIGHUP)    || \
       (sig == SIGINT)    || \
       (sig == SIGALRM)   || \
       (sig == SIGTERM)   || \
       (sig == SIGVTALRM) || \
       (sig == SIGPROF)   || \
       cond_siglost	     \
       (sig == SIGUSR1)   || \
	   (sig == SIGPOLL) || \
       (sig == SIGUSR2)) 
#endif


/*
 * The following are the signals the behavior of which 
 * changes according to whether the sync kill code is enabled.
 */
#define is_sync_term_kill(sig)	(	\
	(sig == SIGILL)  || \
	(sig == SIGIOT)  || \
	(sig == SIGEMT)  || \
	(sig == SIGFPE)  || \
	(sig == SIGBUS)  || \
	(sig == SIGSYS)  || \
	(sig == SIGSEGV))

#ifdef __hpux
#define is_sync_term(sig)	(	\
	(sig == SIGABRT) || \
        (sig == SIGPIPE))
#else
#define is_sync_term(sig)	(	\
	(sig == SIGABRT) || \
        (sig == SIGPIPE) || \
	(sig == SIGXCPU) || \
	(sig == SIGXFSZ))
#endif

#define is_sync_nonterm(sig)	((sig == SIGTTIN) || (sig == SIGTTOU))

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

#define is_not_caught(sig)	(	\
	(sig == SIGKILL) || \
	(sig == SIGSTOP) || \
	(sig == SIGTSTP) || \
	(sig == SIGQUIT) || \
	(sig == SIGTRAP) || platform_not_caught(sig))

#define is_time_slicer(sig)	(sig == SIGVTALRM)


static cma_t_address
thread	(cma_t_address sig)
    {
    cma_t_boolean   err = cma_c_false;


    TRY {
        if (kill (pid, (int)sig) == -1) {
	    cts_failed ("Error in 'kill' in sub-thread");
	    perror ("Error message from 'kill'");
	    }

	printf (" S: --\t");
        }
    CATCH (exc_e_illinstr) {
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
	}
    CATCH (exc_e_resaddr) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_privinst) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_resoper) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
#if _CMA_UNIX_TYPE == _CMA__SVR4
    CATCH (exc_e_illopc) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_illopn) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_illadr) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_illtrp) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_prvopc) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_prvreg) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_coproc) {			/* Special case of SIGILL */
        if ((int)sig != SIGILL) err = cma_c_true;
        else printf (" S: OK\t");
        }
# endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */
    CATCH (exc_e_SIGIOT) {
        if ((int)sig != SIGIOT) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_SIGEMT) {
        if ((int)sig != SIGEMT) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_aritherr) {
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_intovf) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_intdiv) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_fltovf) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_fltdiv) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_fltund) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_decovf) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_subrng) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
#if _CMA_UNIX_TYPE == _CMA__SVR4
    CATCH (exc_e_fltinv) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_fltres) {			/* Special case of SIGFPE */
        if ((int)sig != SIGFPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
#endif # endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */
    CATCH (exc_e_illaddr) {
        if ((int)sig != SIGBUS && (int)sig != SIGSEGV) err = cma_c_true;
        else printf (" S: OK\t");
        }
#if _CMA_UNIX_TYPE == _CMA__SVR4
    CATCH (exc_e_maperr) {			/* Special case of SIGSEGV */
        if ((int)sig != SIGSEGV) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_accerr) {			/* Special case of SIGSEGV */
        if ((int)sig != SIGSEGV) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_adraln) {			/* Special case of SIGBUS */
        if ((int)sig != SIGBUS) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_adrerr) {			/* Special case of SIGBUS */
        if ((int)sig != SIGBUS) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_objerr) {			/* Special case of SIGBUS */
        if ((int)sig != SIGBUS) err = cma_c_true;
        else printf (" S: OK\t");
        }
#endif # endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */
    CATCH (exc_e_SIGSYS) {
        if ((int)sig != SIGSYS) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_SIGPIPE) {
        if ((int)sig != SIGPIPE) err = cma_c_true;
        else printf (" S: OK\t");
        }
#ifndef __hpux
    CATCH (exc_e_excpu) {
        if ((int)sig != SIGXCPU) err = cma_c_true;
        else printf (" S: OK\t");
        }
    CATCH (exc_e_exfilsiz) {
        if ((int)sig != SIGXFSZ) err = cma_c_true;
        else printf (" S: OK\t");
        }
#endif
    CATCH (cma_e_alerted) {
        err = cma_c_true;
        cts_failed ("Unexpected alert in sub-thread.");
        }
    CATCH_ALL {
        err = cma_c_true;
        }
    ENDTRY

    if (err) {
        printf (" S: BAD.\n");
        cts_failed ("Unexpected exception in sub-thread.");
        }
    }


main ()
    {
    cma_t_thread	th;
    cma_t_integer	i, sig;
#if defined(SNI_SVR4_POSIX)
	sigset_t            o_sigmask, c_sigmask;
	cma_t_integer		sigloop_index;
#else
	cma_t_integer       o_sigmask, c_sigmask;
#endif
    cma_t_exit_status	status;
    cma_t_address	result;
    cma_t_boolean       waiting, alerted, except;
	sigset_t		set;


    cts_test ("CRVB_SIG_001", "Test U*IX signal handling.");

    pid = getpid ();
#if defined(SNI_SVR4_POSIX)
    if ( sigprocmask( (int)NULL ,(sigset_t *)NULL, &o_sigmask ) != 0 )
        {
        cma__bugcheck();
        }
#else
    o_sigmask = sigsetmask (0);
    sigsetmask (o_sigmask);
#endif

    cma_init ();

    for (i = 1; i <= 2; i++) {
	printf ("\nRun #%d:\n", i);
        printf ("Signal\t\tMeaning\t\t\t   Class\tThread\t  Main\n");
        printf ("======\t==========================\t==============\t======\t========\n");

        for (sig = 1; sig < NSIG; sig++) {
#if _CMA_UNIX_TYPE == _CMA__SVR4
        printf ("  %2d\t %-24s\t", sig, _sys_siglist[sig]);
#else
# ifndef __hpux
        printf ("  %2d\t %-24s\t", sig, sys_siglist[sig]);
# else
        printf ("  %2d\t %-24s\t", sig, "" );
# endif
#endif


#if defined(SNI_SVR4_POSIX)
        if ( sigprocmask( SIG_SETMASK ,&o_sigmask, &c_sigmask ) == -1 )
			{
			perror("sigprocmask");
			cts_failed ("sigprocmask");
            }
		for (sigloop_index = 1; sigloop_index < NSIG; sigloop_index++)
			{
			if ( !((sigismember(&o_sigmask, sigloop_index) && 
						sigismember(&c_sigmask, sigloop_index)) ||
				   (!sigismember(&o_sigmask, sigloop_index) && 
						!sigismember(&c_sigmask, sigloop_index))))
				{
				printf("old mask: %x\tnew mask: %x\n\n",o_sigmask, c_sigmask);
				cts_failed ("Modified signal currently set.");
				}
			}
#else
	c_sigmask = sigsetmask (o_sigmask);
	if (c_sigmask != o_sigmask)
		cts_failed ("Modified signal currently set.");

#endif

	    /*
	     * Self-check the signal classification macros, and print the 
	     * signal classification.
	     */
	    if ( (is_async_nonterm(sig) && is_async_term(sig))   ||
	         (is_async_nonterm(sig) && is_sync_term(sig))    ||
	         (is_async_nonterm(sig) && is_sync_nonterm(sig)) ||
	         (is_async_nonterm(sig) && is_not_caught(sig))   ||
	         (is_async_term(sig)    && is_sync_term(sig))    ||
	         (is_async_term(sig)    && is_sync_nonterm(sig)) ||
	         (is_async_term(sig)    && is_not_caught(sig))   ||
	         (is_sync_term(sig)     && is_sync_nonterm(sig)) ||
	         (is_sync_term(sig)     && is_not_caught(sig))   ||
	         (is_sync_nonterm(sig)  && is_not_caught(sig)) )
	        cts_failed ("Signal class macro definition inclusion.");

		sigemptyset(&set);
		sigaddset(&set, sig);
	    if (is_time_slicer(sig))
	        printf ("(Time Slice Signal [skipped])\n");
	    else if (is_not_caught(sig))
	        printf ("(Not Caught by CMA [skipped])\n");
	    else if (is_async_nonterm(sig))
	        printf (" Async/Nterm\t");
	    else if (is_async_term(sig))
	        printf (" Async/Term: not tested\n");
		else if (is_sync_term_kill(sig) ) {
			printf (" Sync/Term not handled [skipped]\n");
			continue;
		}
	    else if (is_sync_nonterm(sig))
	        printf (" Sync/Nterm\t");
	    else if (is_sync_term(sig))
	        printf (" Sync/Term\t");
	    else {			/* this isn't one we know */
			printf ("(Unknown [skipped])\n");
	        continue;
		}
	        
	    
	    /* 
	     * Skip the ones that we don't (can't, won't) catch,
	     * and skip the signal used for timeslicing.
	     */
	    if (is_not_caught(sig) || is_time_slicer(sig) || is_async_term(sig))
		continue;

	    /*
	     * Create a thread to send the signal and see what happens.
	     */
	    cma_thread_create (
		    &th,
		    &cma_c_null,
		    thread,
		    (cma_t_address)sig);

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
        }

    printf ("\n");

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_001.C */
/*  *13   14-OCT-1991 13:48:47 BUTENHOF "Fix problems" */
/*  *12   24-SEP-1991 16:37:17 BUTENHOF "Integrate Apollo CMA5 reverse drop" */
/*  *11   29-MAY-1991 18:02:18 BUTENHOF "Fix compilation errors" */
/*  *10   27-FEB-1991 19:22:17 BUTENHOF "Fix signal tests for RIOS" */
/*  *9    24-JAN-1991 00:15:39 BUTENHOF "Fix exception names" */
/*  *8    28-DEC-1990 00:44:09 BUTENHOF "Fix a few exception names" */
/*  *7    30-OCT-1990 19:56:55 SCALES "Update for TSTP, etc." */
/*  *6     4-SEP-1990 16:59:26 SCALES "Change filename references" */
/*  *5     4-SEP-1990 16:01:09 SCALES "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:04:40 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:30:50 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:37:39 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:23 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_001.C */

