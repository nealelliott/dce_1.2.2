/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: euvb_wrp_001.c,v $
 * Revision 1.1.59.2  1996/02/17  23:31:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:56  marty]
 *
 * Revision 1.1.59.1  1995/12/11  23:02:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:12  root]
 * 
 * Revision 1.1.57.1  1994/04/01  20:11:25  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:13  jd]
 * 
 * Revision 1.1.55.1  1993/10/12  19:37:05  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:46  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:52:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:07  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/11  17:21:10  bolinger
 * 	Provide OT enhancement 3375:  distinguish between the failure
 * 	of the wait(2) syscall, and the return of unexpected information
 * 	from or about the child waited for.
 * 	[1992/05/11  13:44:02  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
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
 *	EUVB_WRP_001
 *
 *  TEST TOPICS:
 *
 *	WRP  -  U*IX wrappers
 *
 *  ABSTRACT:
 *
 *	Verify the recent addition of special code in the fork()
 *	wrapper to skip the wrapper code if DECthreads hasn't yet
 *	been initialized; it should just do the fork.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	03 October 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <sys/signal.h>
#include <pthread_exc.h>
#include <dce/cts_test.h>
#include <sys/wait.h>
#include <sys/types.h>
/*
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
# include <sys/m_wait.h>
#endif
*/
#include <errno.h>

pthread_t	thread;
pthread_mutex_t	m;
pthread_cond_t	cv;
int		doneflag;

pthread_addr_t
thdfunc	(char	*whoami)
    {
    cts_comment ("%s worker thread lives!", whoami);
    pthread_mutex_lock (&m);
    doneflag = 1;
    pthread_cond_signal (&cv);
    pthread_mutex_unlock (&m);
    return (pthread_addr_t)1;
    }

void
do_thready_stuff	(char	*whoami)
    {
    int			status;
    pthread_addr_t	result;
    struct timespec	delta, timeout;


    pthread_mutex_init (&m, pthread_mutexattr_default);
    pthread_cond_init  (&cv, pthread_condattr_default);
    doneflag = 0;
    pthread_create (
	    &thread,
	    pthread_attr_default,
	    (pthread_startroutine_t)thdfunc,
	    (pthread_addr_t)whoami);
    pthread_mutex_lock (&m);
    delta.tv_nsec = 0;
    delta.tv_sec  = 10;
    pthread_get_expiration_np (&delta, &timeout);

    while (!doneflag) {
	status = pthread_cond_timedwait (&cv, &m, &timeout);

	if (status == -1 && errno == EAGAIN) {
	    cts_failed ("%s thread timed out", whoami);
	    exit (-1);
	    }

	}

    pthread_mutex_unlock (&m);
    pthread_join (thread, &result);

    if (result != (pthread_addr_t)1)
	cts_failed ("%s thread returned failure", whoami);

    }

int
main	(int argc, char *argv[], char *envp[])
    {
	int pid;

    cts_test ("EUVB_WRP_001", "test fork() without DECthreads init.");
    if ((pid = fork()) == 0) {
	/*
	 * This is the child process
	 */
	do_thready_stuff ("child");
	cts_exit ();
	exit (1);
	}
    else {
	int	wait_result, rpid;

	/*
	 * This is the parent process
	 */
	do_thready_stuff ("parent");
	rpid = wait(&wait_result);		/* Wait for child to complete */

	if (rpid == -1) {
		perror("wait failed");
		cts_failed("wait failed");
	}
	else if (rpid != pid)
		cts_failed("got a different child (!?)");
	else if (WEXITSTATUS (wait_result) != 1)
	    cts_failed ("bad exit status (%d) from forked child",
			wait_result);

	}

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_WRP_001.C */
/*  *1    15-OCT-1991 06:49:44 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_WRP_001.C */
