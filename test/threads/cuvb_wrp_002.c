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
 * $Log: cuvb_wrp_002.c,v $
 * Revision 1.1.72.2  1996/02/17  23:31:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:40  marty]
 *
 * Revision 1.1.72.1  1995/12/11  23:01:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:58  root]
 * 
 * Revision 1.1.70.1  1994/04/01  20:11:02  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:22  jd]
 * 
 * Revision 1.1.68.1  1993/10/12  19:36:42  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:36  rhw]
 * 
 * Revision 1.1.5.4  1992/12/30  16:51:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:00  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:39:40  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:56  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/03  14:32:18  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:10  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/12  21:12:31  bolinger
 * 	Remove duplicated #include lines.
 * 
 * Revision 1.1.2.2  1992/06/30  13:49:56  bolinger
 * 	Fix OT defect 4444:  use fflush((FILE *)0) construct only
 * 	if __STDC__ defined.  Otherwise flush stdout and stderr
 * 	by name.
 * 	[1992/06/29  19:25:55  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:36  devrcs
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
 *	CUVB_WRP
 *
 *  TEST TOPICS:
 *
 *	WRP  -  U*IX wrappers
 *
 *  ABSTRACT:
 *
 *	
 *	Test to see that only the forking thread is present
 *	in the forked process, and do this from a thread 
 *	that is not the initial thread in the initial process.
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	05 June 1991
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	18 June 1991
 *		Add RIOS specific include.
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
# include <sys/m_wait.h>
#endif


cma_t_thread	thd;
cma_t_thread	thd2;
cma_t_cond	cv;
cma_t_integer	cv_pred = 1;
cma_t_mutex	mut;



static cma_t_address
fork_reaper    (
    cma_t_address	arg)
    {
    cma_mutex_lock (&mut);
    while (cv_pred)
	cma_cond_wait (&cv, &mut);
    cma_mutex_unlock (&mut);

    /*
     * The only other thread in the forked process has notified
     * us that it is about to exit, wait 10 seconds and then
     * exit() the process.  (result goes to parent process).
     */
    cma_delay (10.0);
    exit(1);
    return (cma_c_null_ptr);
    }



static cma_t_address
worker    (cma_t_address	arg)
    {
    cma_t_integer	pid;

#ifdef __STDC__
    if (fflush((FILE *)0) < 0)
    {
	perror("can't fflush((FILE *)0)");
	fflush(stdout);
	fflush(stderr);
    }
#else
	fflush(stdout);
	fflush(stderr);
#endif
    pid = fork ();			/* fork a child */

    if (pid == -1) {
	cts_failed ("fork(2) failed with %d", errno);
	}

    if (pid == 0) {

	/* Create the reaper thread */
	cma_thread_create (&thd2, &cma_c_null, fork_reaper, cma_c_null_ptr);

	/*
	 * Test that a condition variable and mutex created in
	 * the parent process will work in the forked process.
	 * Also notifies reaper thread that we are about to 
	 * exit.  
	 *
	 * NOTE: this exit should never routine to the join
	 * by the 'initial' thread in the forked process, because
	 * the 'initial' thread should not executing in this process.
	 */

	cma_mutex_lock (&mut);
	cv_pred = 0;
	cma_cond_signal (&cv);
	cma_mutex_unlock (&mut);
	}

    return ((cma_t_address)pid);
    }



int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_exit_status   join_status;
    cma_t_address	result;
#if ((_CMA_UNIX_TYPE == _CMA__SVR4) && (!defined(_CMA_BSD_CMP_)))
    int             wait_result;
#else
    union wait      wait_result;
#endif

    cts_test ("CUVB_WRP_002", "Test cma_fork from non-initial thread");
    cma_init ();

    /*
     * Create a mutex and a condition variable, to be used
     * later by the forking process.  PROOF: CVs & mutexes persistent.
     */

    cma_cond_create (&cv, &cma_c_null);
    cma_mutex_create (&mut, &cma_c_null);

    /*
     * Create worker thread, which will procede to do a fork.
     */

    cma_thread_create (&thd, &cma_c_null, worker, cma_c_null_ptr);

    /*
     * Wait for child thread, but should never execute past 
     * the join, upon fork() the default tcb gets dropped 
     * from `existence'. - - - IN THE FORKED PROCESS.
     */
    
    cma_thread_join (&thd, &join_status, &result);

    if (((cma_t_integer)result) == 0)	/* if in forked process */
        cts_failed ("Parent thread should not be executing at this point!");

    if (((cma_t_integer)result) > 0) {
        while (wait(&wait_result) != ((cma_t_integer)result));
#if ((_CMA_UNIX_TYPE == _CMA__SVR4) && (!defined(_CMA_BSD_CMP_)))
    if (WEXITSTATUS(wait_result) != 1)
#else
    if (wait_result.w_T.w_Retcode != 1)
#endif
		cts_failed ("Received bad exit status from forked child.");
	}

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_002.C */
/*  *3    18-JUN-1991 17:19:53 CURTIN "Adding rios specific include" */
/*  *2    17-JUN-1991 15:02:41 CURTIN "Fixed some typing" */
/*  *1     5-JUN-1991 19:15:25 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_002.C */
