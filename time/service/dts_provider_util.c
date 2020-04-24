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
 * $Log: dts_provider_util.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:13:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:35  root]
 * 
 * Revision 1.1.2.10  1994/08/23  20:32:23  cbrooks
 * 	Code Cleanup - ANSI switches
 * 	[1994/08/23  15:26:23  cbrooks]
 * 
 * Revision 1.1.2.9  1994/08/16  18:13:00  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:36  cbrooks]
 * 
 * Revision 1.1.2.8  1994/08/02  19:05:27  cbrooks
 * 	CR11514 - add <stdio.h> to includes files
 * 	[1994/08/02  14:55:18  cbrooks]
 * 
 * Revision 1.1.2.8  1994/08/02  19:05:27  cbrooks
 * 	CR11514 - add <stdio.h> to includes files
 * 	[1994/08/02  14:55:18  cbrooks]
 * 
 * Revision 1.1.2.7  1994/07/22  17:10:58  cbrooks
 * 	CR11391 - unknown identifier
 * 	[1994/07/22  17:08:13  cbrooks]
 * 
 * Revision 1.1.2.6  1994/07/21  13:27:41  cbrooks
 * 	CR11374 - use <pthread.h>, not <dce/pthread.h>
 * 	[1994/07/21  13:23:26  cbrooks]
 * 
 * Revision 1.1.2.5  1994/07/20  20:24:42  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:26:01  cbrooks]
 * 
 * Revision 1.1.2.4  1994/06/29  21:00:09  ohara
 * 	Merged with changes from 1.1.2.3
 * 	[1994/06/29  20:59:51  ohara]
 * 
 * 	add fcntl.h and sys/types.h
 * 	[1994/06/07  22:52:37  ohara]
 * 
 * Revision 1.1.2.3  1994/06/09  19:28:00  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:27:25  annie]
 * 
 * Revision 1.1.2.2  1994/05/19  14:48:35  tom
 * 	Define a reasonable path_max in case limits.h doesn't.
 * 	[1994/05/19  14:48:18  tom]
 * 
 * Revision 1.1.2.1  1994/05/18  19:49:16  tom
 * 	Use provider specific define so this can be built stand-alone.
 * 	[1994/05/16  16:06:00  tom]
 * 
 * 	Change to the var/adm/time directory, not root.
 * 	[1994/05/13  17:54:35  tom]
 * 
 * 	Initial checkin.
 * 
 * $EndLog$
 */

/*
 * Routine to detach a provider from its tty
 */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <dce/dce.h>
#include <dce/rpc.h>
#include <dce/dtsprovider.h>	/* Time provider interface definition */
#include <dts_provider_util.h>
#include <sys/signal.h>

#ifndef PATH_MAX
#define PATH_MAX 	1024	/* some systems require pathconf(PATH_MAX) */
#endif				/* but we don't need to be that exact here */


extern rpc_binding_vector_p_t bind_vector ;

#define PrintOSError() \
        (void) fprintf(stderr, \
                       "\nFatal error at line %d in file %s\n\t%s\n", \
                       __LINE__,                        \
                       __FILE__,                        \
		       strerror( errno ));

int
daemon(int do_chdir, int do_close)
{
	int cpid;
	char homedir[PATH_MAX+1];

	if ((cpid = fork()) == -1)
		return (-1);
	if (cpid)
		exit(0);
	(void) setsid();
	if (do_chdir) {
		strcpy(homedir, dcelocal_path);
		strcat(homedir, K_PROVIDER_HOME_DIR);
		(void) chdir(homedir);
	}
	if (do_close) {
		int devnull = open("/dev/null", O_RDWR, 0);

		if (devnull != -1) {
			(void) dup2(devnull, 0);
			(void) dup2(devnull, 1);
			(void) dup2(devnull, 2);
			if (devnull > 2)
				(void) close(devnull);
		}
	}
	return (0);
}



/*
 * Internal Prototypes
 */

static int catch[] = {
    SIGHUP, SIGINT, SIGQUIT, SIGTERM, 0
};


#ifdef IGNORE_SIGNALS 
static int ignore[] = {
#ifdef SIGXCPU
    SIGXCPU, SIGXFSZ,
#endif
#ifdef SIGLOST
    SIGLOST,
#endif
    SIGIO, SIGPIPE, SIGALRM, SIGPROF, SIGUSR1, SIGUSR2, 0
};
#endif


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
void
signal_catcher (
    pthread_addr_t arg_p
)
{
    sigset_t    signals;
    int     sig, *sig_p;
    struct sigaction action;
    error_status_t status = rpc_s_ok ;
    
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
     * define only if we wish to ignore signals, as opposed 
     * to letting them default 
     */

#ifdef IGNORE_SIGNALS
    /*
     * Ignore extraneous signals
     */
    sig_p = ignore;
    while (sig = *sig_p++)
	(void) sigaction( sig, &action, (struct sigaction *)0) ;
#endif 
    /*
     * block these while waiting
     */
    if (sigprocmask(SIG_BLOCK, &signals, NULL) < 0)
    {
	PrintOSError();
	exit(1);
    }
    for ( ; ; ) {
        sig = sigwait(&signals);
        switch (sig) {

        default:	/* generate a terminate event on all signals */
	    rpc_mgmt_stop_server_listening( (rpc_binding_handle_t)NULL, 
					   &status);
            break;

	case -1:	/* error from sigwait */
	    PrintOSError();
	    exit(1);
        }
    }
    return ;			/* which will cancel this thread */
}


