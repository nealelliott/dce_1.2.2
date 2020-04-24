/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27signal.c,v $
 * Revision 1.1.4.2  1996/02/18  18:23:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:16:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:37  root]
 * 
 * Revision 1.1.2.5  1994/09/06  12:26:35  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:25:18  keutel]
 * 
 * Revision 1.1.2.4  1994/06/21  14:48:33  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:03:49  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:18:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:19  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:20  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:48:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  14:17:21  keutel]
 * 
 * 	creation
 * 	[1994/03/21  14:47:14  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27signal.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:23:54 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d25_sigactionmap()                                             */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function may be considered as a convenience function which      */
/*       allows to use asynchronous signals (e.g. SIGALRM or SIGUSR1) in     */
/*       a multithreaded environment in nearly the same way as in a non-     */
/*       threaded environment. Because the system call sigaction(), applied  */
/*       to an asynchronous signal, fails in a threaded environment, the     */
/*       function creates a new thread which from now associated to the      */
/*       signal specified in the calling parameter 'signo'.                  */
/*       This thread waits for signals of that type by calling sigwait().    */
/*       Everytime a signal occurs, the thread performs the action specified */
/*       by the user (e.g. ignoring the signal or calling the user's signal  */
/*       routine).                                                           */
/*                                                                           */
/*       NOTE: This function was primary implemented for use in a GDS-       */
/*             environment (e.g. by the stubs, dsa, etc.). For use in a      */
/*             more general multithreaded environment extensions are         */
/*             required to synchronize access to the static signal infor-    */
/*             mation table and to handle additional asynchronous signals.   */
/*                                                                           */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       signo       = signal no.         (see system call sigaction())      */
/*       act         = signal information (            "              )      */
/*       oact        = signal information (            "              )      */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*       oact        = previous signal information (see system call          */
/*                                                              sigaction()) */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*                     see system call sigaction()                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 16.3.94     */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <signal.h>
#include <dce/d27util.h>

/* ------------------ DECLARATION OF LOCAL DATA ---------------------------- */
 
static struct {				/* signal information table */
    boolean	     d27_siginit ;
    struct sigaction d27_siginfo ;
} d27_sigtable[NSIG] ;

/* ------------------ DECLARATION OF LOCAL FUNCTIONS ----------------------- */
 
static pthread_startroutine_t d27_async_sig_hdl (pthread_addr_t arg);

/* ------------- convenience sigaction()-routine --------------------------- */

int d27_sigactionmap (int signo, const struct sigaction *act,
		      struct sigaction *oact) {

    pthread_startroutine_t d27_async_sig_hdl (pthread_addr_t) ;
    pthread_t		   d27_sighandle ;
    pthread_attr_t 	   d27_attr ;
    static pid_t	   d27_mypid ;
    pid_t		   d27_tmppid ;
    signed32		   i ;

    /* verify for synchronous, asynchronous signal type */
    if (signo != SIGHUP  && signo != SIGINT  && signo != SIGQUIT &&
	signo != SIGALRM && signo != SIGTERM && signo != SIGUSR1 &&
#if !defined SIGPOLL
	signo != SIGUSR2)
#else
	signo != SIGUSR2 && signo != SIGPOLL)
#endif
	/* synchronous signals are still handled by sigaction() */
	return (sigaction (signo, act, oact)) ;

    if ((d27_tmppid = getpid ()) != d27_mypid) {
	/* if d27_sigaction is called the first time in a process */
	/* the signal information table has to be (re)initialized */
	for (i = 0; i < NSIG; i++) {
	    d27_sigtable[i].d27_siginit = FALSE ;
	    d27_sigtable[i].d27_siginfo.sa_handler = SIG_DFL ;
	    d27_sigtable[i].d27_siginfo.sa_flags = 0 ;
	    sigemptyset (&d27_sigtable[i].d27_siginfo.sa_mask) ;
	}
	d27_mypid = d27_tmppid ;
    }

    if (oact != (struct sigaction *) NULL) {
	/* return signal information previously associated with signo */
	*oact = d27_sigtable[signo-1].d27_siginfo ;
    }

    if (act != (struct sigaction *) NULL) {
	/* save new signal information associated with signo */
	d27_sigtable[signo-1].d27_siginfo = *act ;
    }

    if (!d27_sigtable[signo-1].d27_siginit) {
	/* create thread dealing with an asynchronous signal */

	/* (initialize thread specific attributes object first) */
	/* create thread attributes object */	
	if (pthread_attr_create (&d27_attr) == -1) {
	    return (-1) ;
	}
	/* change inherit scheduling attribute */
	if (pthread_attr_setinheritsched (&d27_attr,
						 PTHREAD_DEFAULT_SCHED) == -1) {
	    return (-1) ;
	}
	/* change scheduling policy attribute */
	if (pthread_attr_setsched (&d27_attr, SCHED_RR) == -1) {
	    return (-1) ;
	}
	/* change scheduling priority attribute */
	if (pthread_attr_setprio (&d27_attr, PRI_RR_MAX) == -1) {
	    return (-1) ;
	}
	/* create thread */
	if (pthread_create (&d27_sighandle, d27_attr, (pthread_startroutine_t)
			       d27_async_sig_hdl, (pthread_addr_t) signo) < 0) {
	    return (-1) ;
	}
	/* delete thread attributes object */
	if (pthread_attr_delete (&d27_attr) == -1) {
	    return (-1) ;
	}
	d27_sigtable[signo-1].d27_siginit = TRUE ;
    }
    return (0) ;
}

/* ----------- asynchronous signal handler thread --------------------------- */

static pthread_startroutine_t d27_async_sig_hdl (pthread_addr_t arg) {
    int		d27_signo = (int) arg ;
    sigset_t	d27_sigmask ;

    /* initialize signal mask */
    sigemptyset (&d27_sigmask) ;

    /* add signal to signal mask */
    sigaddset (&d27_sigmask, d27_signo) ;

    for (;;) {
	/* wait for asynchronous signal */
	if ((d27_signo = sigwait (&d27_sigmask)) == -1) {
	    perror ("sigwait failed") ;
	    continue ;
	}
	/* asynchronous signal received, check type of signal handling */
	if (d27_sigtable[d27_signo-1].d27_siginfo.sa_handler == SIG_IGN)
	    continue ;
	if (d27_sigtable[d27_signo-1].d27_siginfo.sa_handler == SIG_DFL)
	    exit (0) ;

	/* call user's signal handler routine */
	d27_sigtable[d27_signo-1].d27_siginfo.sa_handler (d27_signo) ;
    }
}
