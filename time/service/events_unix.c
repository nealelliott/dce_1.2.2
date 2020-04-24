/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: events_unix.c,v $
 * Revision 1.1.63.2  1996/02/17  23:35:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:59  marty]
 *
 * Revision 1.1.63.1  1995/12/08  18:14:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  17:31 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDC01 into HPDCE02
 * 	[1995/12/08  17:26:04  root]
 * 
 * Revision 1.1.7.3  1993/09/17  19:36:04  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/17  19:35:14  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:50  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  20:24:43  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:24:14  root]
 * 
 * Revision 1.1.5.2  1993/03/23  22:01:43  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/23  22:00:50  truitt]
 * 
 * Revision 1.1.1.7  1993/03/11  22:35:23  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.61.4  1994/05/16  18:51:56  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:32  rhw]
 * 
 * Revision 1.1.61.3  1994/04/21  22:06:12  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:00  cbrooks]
 * 
 * Revision 1.1.61.2  1994/01/21  21:52:10  rsalz
 * 	NULL checkin
 * 	[1994/01/21  21:51:50  rsalz]
 * 
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:31  rsalz]
 * 
 * Revision 1.1.61.1  1994/01/21  21:08:40  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:31  rsalz]
 * 
 * Revision 1.1.5.4  1993/01/12  17:07:49  tom
 * 	Bug 6617 - Create a thread to handle synchronous signals correctly.
 * 	[1993/01/12  17:06:42  tom]
 * 
 * Revision 1.1.5.3  1992/12/30  16:39:27  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:23:03  htf]
 * 
 * Revision 1.1.5.2  1992/09/29  20:44:20  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:07:38  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/24  16:55:54  grober
 * 	HP/UX Porting Changes
 * 	[1992/06/22  16:00:21  grober]
 * 
 * Revision 1.1.3.3  1992/06/18  21:07:00  smythe
 * 	promoting previous changes to source tree
 * 	[1992/06/18  21:06:36  smythe]
 * 
 * Revision 1.1.3.2  1992/06/05  18:52:33  hopkins
 * 	Disable non-existent signals on HPUX
 * 	[1992/06/05  15:50:41  hopkins]
 * 
 * Revision 1.1  1992/01/19  15:34:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module EVENTS_ULTRIX.C
 *	Version 1.0
 */
/*
 * Copyright (c) 1989, 1990, 1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    This module contains U*ix-specific event handling routines.
 *
 * ENVIRONMENT:	Ultrix 4.0 and above with pthreads
 *
 * AUTHORS:
 *              Margaret Olson
 *		Brian Bailey (threaded modifications)
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	August 1989
 *		January 1991
 *
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */


#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>

#include <sys/signal.h>

/*
 * Internal Prototypes
 */

static int catch[] = {
    SIGHUP, SIGINT, SIGQUIT, SIGTERM, 0
};
static int ignore[] = {
#ifdef SIGXCPU
    SIGXCPU, SIGXFSZ,
#endif
#ifdef SIGLOST
    SIGLOST,
#endif
    SIGIO, SIGPIPE, SIGALRM, SIGPROF, SIGUSR1, SIGUSR2, 0
};

/* signal_catcher ()
 *
 * Functional Description:
 *	separate thread waits for signals.
 *	It also does other signal-related processing, like ignoring
 *      specified signals -- this is not strictly necessary, but seems
 * 	reasonable.
 *
 *	NOTE:  Don't catch or ignore VTALRM, because the threads package uses
 *	this to do timeslicing and thread scheduling.
 * Inputs:
 *	none
 *
 * Outputs:
 *	none
 *
 * Return Value:
 *	none
 *
 */
static int
signal_catcher (
    char    *arg_p
)
{
    sigset_t    signals;
    int     sig, *sig_p;
    struct sigaction action;
    
    /* begin */

    action.sa_flags = 0 ;
    sigemptyset(&action.sa_mask) ;
    action.sa_handler = SIG_IGN ;

    sigemptyset(&signals);

    /*
     * These are the asynchronous signals we want to catch
     */
    sig_p = catch;
    while (sig = *sig_p++)
        sigaddset(&signals, sig);

    /*
     * Ignore extraneous signals
     */
    sig_p = ignore;
    while (sig = *sig_p++)
	(void) sigaction( sig, &action, (struct sigaction *)0) ;
    /*
     * block these while waiting
     */
    if (sigprocmask(SIG_BLOCK, &signals, NULL) < 0)
	BugCheckOSstatus(errno);
    for ( ; ; ) {
        sig = sigwait(&signals);
        switch (sig) {

        default:	/* generate a terminate event on all signals */
    	    GenEvent(ETterminate);
            break;

	case -1:	/* error from sigwait */
	    BugCheckOSstatus(errno);
	    exit(1);
        }
    }
}

/*
 *++
 *  InitSysdepEvent()
 *
 *  Functional Description:
 *	Perform System-specific event initialization.
 * 	This routine is called by the portable routine InitEvent at startup.
 *
 *	Due to threads not supporting asynchronous signals, we set up
 *	a thread which waits for synchronous signals and then generates
 *	a terminate event.
 *
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *      none
 *
 *  Outputs:
 *      none
 *
 *  Implicit Outputs:
 *      none
 *
 *  Value Returned:
 *      StatusBlk
 *
 *  Side Effects:
 *
 *--
 */
StatusBlk InitSysdepEvent(void)
{
    StatusBlk statusBlk;
    pthread_t signal_thread;    /* nobody ever looks at this */

    if (pthread_create(&signal_thread,
                   pthread_attr_default,
                   (pthread_startroutine_t)signal_catcher,
                   (pthread_addr_t)0) < 0)
	statusBlk.status = DTSS_ERROR;
    else
	statusBlk.status = DTSS_SUCCESS;
    return(statusBlk);
} /* end of routine InitSysdepEvent */
