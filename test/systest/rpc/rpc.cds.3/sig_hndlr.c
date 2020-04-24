/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sig_hndlr.c,v $
 * Revision 1.1.62.2  1996/02/17  23:27:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:05  marty]
 *
 * Revision 1.1.62.1  1995/12/11  22:43:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:20:14  root]
 * 
 * Revision 1.1.59.2  1994/06/10  20:50:48  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:51  devsrc]
 * 
 * Revision 1.1.59.1  1993/12/22  20:51:28  mhickey
 * 	no_changes
 * 	[1993/12/22  20:50:44  mhickey]
 * 
 * 	Made the arguement to the cleanup routine point to real storage.
 * 	[1993/12/20  19:47:22  mhickey]
 * 
 * 	A number of fixes to support the fix for defect 7743
 * 	in the server code.
 * 	- Fixed comments to reflect new operation
 * 	- Added include of rpc.h for rpc types and prototypes
 * 	- Changed kill_func to cleanup_func
 * 	- Now uses argv_p as a pointer to "client" or "server" - used to decide how to
 * 	  shut down main thread along with checks on the value.
 * 	- Changed the startup error action to be thread exit instead of kill the main.
 * 	- Changed the default action for a server to be:
 * 	    - try to wake the main thread via server stop listening
 * 	    - if that fails, call the cleanup routine directly.
 * 	  and for a client
 * 	    - call the cleanup routine directly.
 * 	[1993/12/11  05:08:43  mhickey]
 * 
 * Revision 1.1.4.1  1993/10/20  19:36:54  ohara
 * 	svr4 ref port
 * 	[1993/10/20  19:35:57  ohara]
 * 
 * Revision 1.1.2.2  1993/03/12  04:28:52  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:54  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* checkin log from before rename - preserved for auditing
 *---*
 * Revision 1.1.1.2  1993/01/29  20:35:20  mhickey
 * 	Initial check in for the rpc.cds.3 system test.
 *
 * Revision 1.1.2.2  1993/01/22  19:59:21  mhickey
 * 	Initial checkin of code I "stole" from dts to handle
 * 	signals in a threaded app.
 * 	[1993/01/19  20:41:54  mhickey]
 *---*
 *---*/

/*---*
 *---* sig_hndlr.c - block and catch signals to handle keyboard
 *---*               interrupts cleanly.
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE1.0.2 DCE System Tests
 *---*
 *---*    The DCE 1.0.2 system tests correspond to the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---*    plan.
 *---* 
 *---* FUNCTION
 *---*    Blocks on receipt of a SIGINT, SIGHUP, SIGQUIT, etc, and when 
 *---*    received, generates a report and possibly terminates.
 *---*
 *---* CALLED BY
 *---*    Program's main thread.
 *---* 
 *---* CALLS INTO 
 *---*    'C' library, a cleanup function and a report generation function
 *---*
 *---* PREREQUISITES
 *---*    Assumes the external definition of a pointer to a void function
 *---*    called "rep_gen_func_p" that takes an integer as an arguement.
 *---*    rep_gen_func_p should point to some sort of report generator
 *---*    for the invoking function.  Assumes the external definition of 
 *---*    a pointer to a void function (cleanup_func_p) that takes a 
 *---*    pthread_id as an arguement.  This function should cleanup any
 *---*    namespace objects created by a server, kill threads, etc.
 *---*    Also assumes that the variable debug is declared externally.
 *---*    The passed char pointer argv_p must point to "client" or "server"
 *---*    If argv_p points to "client", then the receipt of an signal 
 *---*    except SIGHUP or SIGQUIT will result in the cleanup function being
 *---*    called, and a call to exit().  If argv_p points to "server", then
 *---*    upon receipt of a signal other than SIGHUP or SIGQUIT, a call
 *---*    is made to rpc_mgmt_stop_server_listening() to attempt to let
 *---*    the main thread do it's own cleanup.  If this call fails (ie: bad
 *---*    return value in stat parameter), then the cleanup function is called
 *---*    followed by a call to exit.
 *---*/

/*---*
 *---* sig_catcher() - runs in a seperate thread and catches asynch signals
 *---*                 extracted from Tom Jordahl's event_unix code for dts
 *---*/
#include <errno.h>
#include <stdio.h>
#include <sys/signal.h>
#include <pthread.h>
#include <dce/rpc.h>


/*---*
 *---* extern declarations
 *---*/
extern int debug;

extern void (*cleanup_func_p)();
extern void (*rep_gen_func_p)();

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
 *	arg_p = points to "server" or "client" - if "server" then the terminate
 *                                               action is to stop listening;
 *                                               if "client" then execute
 *                                               the cleanup routine
 *                                               directly.
 *
 * Outputs:
 *	none
 *
 * Return Value:
 *	none
 *
 */

int
signal_catcher (arg_p)
    char    *arg_p;
{
    sigset_t    signals;
    int     sig, *sig_p;
    unsigned32  stat;
    int pid=getpid();
    pthread_t thd_id = pthread_self();

    /*---* check arg_p *---*/
    if (!arg_p || (strcmp(arg_p, "server") && strcmp(arg_p,"client")))
    {
       printf("sig_catcher(%d): missing or bad arg...exitting.\n");
       pthread_exit((pthread_addr_t)1);
    }
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
        /*---* 
         *---* we use the older signal function here because sigaction is 
         *---* not correctly wrapped by pthreads at this time.
         *---*/
        signal(sig, SIG_IGN);

    /*
     * block these while waiting
     */
    if (sigprocmask(SIG_BLOCK, &signals, NULL) < 0)
    {
	perror("Trying to block signals...");
        pthread_exit((pthread_addr_t)1);
    }

    while (1) {
        sig = sigwait(&signals);
        switch (sig) {

	case -1:	/* error from sigwait */
	    perror("Waiting for signals");
            (*cleanup_func_p)(&thd_id);
            break;

        case SIGQUIT: /*---* we will use this to generate a report *---*/
            if (debug)
               printf("sig_catcher(%d): got signal %d...\n", pid, sig);
    	    (*rep_gen_func_p)(0);
            break;

        case SIGHUP: /*---* we will use this to toggle debug mode *---*/
            printf("sig_catcher(%d): Toggling debug to %s...\n", pid, 
                                                         debug? "off": "on");
            fflush(stdout);
            debug = debug? 0: 1;
            break;

        default:     /* generate a terminate event on all except SIGQUIT */
            if (debug)
            {
               printf("sig_catcher(%d): got signal %d...\n", pid, sig);
               printf("sig_catcher(%d): Stopping the server listen...\n", pid);
	    }
            if (!strcmp(arg_p, "server"))
	    {
               rpc_mgmt_stop_server_listening((rpc_binding_handle_t)NULL, &stat);
               if (stat != rpc_s_ok)
	       {
                  printf("sig_catcher(%d): couldn't wake main thread, calling cleanup routine\n");
                  (*rep_gen_func_p)(sig);
                  (*cleanup_func_p)(&thd_id);               
               }
            }
            else
            {
               if (debug)
                  printf("sig_catcher(%d): Cancelling threads...\n", pid);
               (*rep_gen_func_p)(sig);
               (*cleanup_func_p)(&thd_id);               

               if (debug)
                  printf("sig_catcher(%d): Exiting.\n",pid);\
               exit(1);
	    }
            break;

        }
    }
    /*
     * We never exit while loop, but if we did...
     */
    sigprocmask(SIG_UNBLOCK, &signals, NULL);
}
