/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: icl_signal.c,v $
 * Revision 1.1.42.1  1996/10/02  17:52:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:03  damon]
 *
 * Revision 1.1.37.2  1994/06/09  14:11:43  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:22  annie]
 * 
 * Revision 1.1.37.1  1994/02/04  20:21:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:19  devsrc]
 * 
 * Revision 1.1.35.1  1993/12/07  17:27:04  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:39:42  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/19  16:05:15  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  14:15:33  cjd]
 * 
 * Revision 1.1.2.2  1992/12/09  20:20:36  jaffe
 * 	Transarc delta: comer-ot6129-user-level-icl-dumping 1.5
 * 	  Selected comments:
 * 	    Added code to ICL trace logs at user level.  The guinea pig for this
 * 	    change was dfsbind, also modified as part of this delta.  Most of the
 * 	    actual tracing and some of the ICL work was done at IBM by Jean
 * 	    Pehkonen.
 * 	    Force the program to dump its log: kill -30 <pid>
 * 	    Added signal handlers -- one threaded, one for processes sleeping in
 * 	    the kernel.
 * 	    Added dfsbind to EXPINC_SUBDIRS
 * 	    In files where both osi.h and compat.h are included, the reference
 * 	    to osi.h must be first.  In each of these files, osi.h was implicitly
 * 	    included, so I added an explicit reference before compat.h.
 * 	    Installed message catalog.
 * 	Transarc delta: jaffe-sync-with-dcedfs-changes 1.1
 * 	  Selected comments:
 * 	    Fix occasions of afs/ that were not yet at the OSF
 * 	[1992/12/04  16:46:27  jaffe]
 * 
 * $EndLog$
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>

IMPORT void icl_DumpUser _TAKES(());
/******************************************************
 * Code to handle dumps in user space that are started 
 * by a signal.
 */

/* define signal mask */
static sigset_t    sigmask;

/* threaded routine to handle signal */
static void SigHandle()
{ 
    int signal_num=0;

    sigemptyset(&sigmask);
    if (sigaddset(&sigmask, SIGUSR1) < 0)
    {
	(void) fprintf(stderr, "unable to catch signal, errno = %d\n", errno);
	pthread_exit(0);
    }
    
    while (signal_num = sigwait(&sigmask)) 
    {
	if (signal_num < 0)
	{
	    (void) fprintf(stderr, "icl_signal: sigwait() failed, errno = %d\n", 
			   errno);
	    break;
	}
	if (signal_num == SIGUSR1)
	    icl_DumpUser();
    }
    pthread_exit(0);
    
}

/* Function to set up thread to handle signals */
int icl_StartCatcher(waitInKernel)
int waitInKernel;		/* are we waiting in the kernel? */
{
    static int Initted = 0;
    static pthread_t sig_thread_id;
    int code;

    if (waitInKernel)
    {
#ifdef AFS_SIGACTION_ENV
	struct sigaction vec;
#else
	struct sigvec vec;
#endif

	/* If we're waiting in the kernel, the pthread isn't going to help us */
	bzero((char *)&vec, sizeof(vec));
#ifdef AFS_SIGACTION_ENV
	vec.sa_handler = icl_DumpUser;
	code = sigaction(SIGUSR1, &vec, NULL);
#else /* SIGACTION */
	vec.sv_handler = icl_DumpUser;
	code = sigvec(SIGUSR1, &vec, NULL);
#endif /* SIGACTION */
	return code;
    }

    if (Initted)
	return 0;
    Initted = 1;

    /* set up signal handler to dump the log */
    return(pthread_create(&sig_thread_id, pthread_attr_default,
                      (pthread_startroutine_t)SigHandle,
		       (pthread_addr_t)0));
}
