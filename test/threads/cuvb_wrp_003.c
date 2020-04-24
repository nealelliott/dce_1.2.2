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
 * $Log: cuvb_wrp_003.c,v $
 * Revision 1.1.74.2  1996/02/17  23:31:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:41  marty]
 *
 * Revision 1.1.74.1  1995/12/11  23:01:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:59  root]
 * 
 * Revision 1.1.71.1  1994/04/01  20:11:04  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:27  jd]
 * 
 * Revision 1.1.69.1  1993/10/12  19:36:44  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:36  rhw]
 * 
 * Revision 1.1.5.4  1992/12/30  16:51:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:03  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:39:42  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:34:07  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/03  14:32:24  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:45:14  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/12  21:13:00  bolinger
 * 	Rearrange #include lines to match other tests.
 * 
 * Revision 1.1.2.2  1992/05/11  17:07:28  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:45:29  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:37  devrcs
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
 *	CUVB_WRP_003
 *
 *  TEST TOPICS:
 *
 *	WRP  -  U*IX wrappers
 *
 *  ABSTRACT:
 *
 *	This is the same as cuvb_wrp_001, however we set up
 *	atfork routines and make sure each runs in the 
 *	appropriate place.
 *
 *  AUTHORS:
 *
 *	Paul curtin
 *
 *  CREATION DATE:
 *
 *	06 June 1991
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
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
# include <sys/m_wait.h>
#endif


#define	    NUM_THDS	10

cma_t_thread	thd[NUM_THDS];
cma_t_integer	pid = 1;

static cma_t_address
worker    (cma_t_address	arg)
    {
    /*
     * Upon fork any and all threads running this routine should
     * be removed (only the forking thread, in this case the 
     * initial thread) should remain - as well as the null thread.
     * So if we so the child's pid (0), here it is an error.
     */
    while (1) {
	if (pid == 0)
	    cts_failed ("Worker thread should not live in forked process.");
	}
    }

static void
pre_fork    (cma_t_address	arg)
    {
    /*
     * Add one to the calculation here in pre_fork to 
     * prove we got here - makes calculation = 2.
     */
    (*((cma_t_integer *) arg))++;
    }

static void
parent_fork    (cma_t_address	arg)
    {
    /*
     * Add one to the calculation here in parent_fork to
     * prove we got here, this makes the final calculation
     * for the parent = 3.
     */
    (*((cma_t_integer *) arg))++;
    }

static void
child_fork    (cma_t_address	arg)
    {
    /*
     * Add two to the calculation here in child_fork to 
     * prove we got here, this makes the final calculation
     * for the child = 4.
     */
    (*((cma_t_integer *) arg))++;
    (*((cma_t_integer *) arg))++;
    }


int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer   index;
#if defined(SNI_SVR4_POSIX)
    int             wait_result;
#else
    union wait      wait_result;
#endif
    cma_t_integer   atfork_data = 1;


    cts_test ("CUVB_WRP_003", "Test fork(2) wrapper with atfork() handlers");
    cma_init ();

    /*
     * atfork_data status out being 1, then is incremented to
     * 2 in the pre_fork routine.  In the parent_fork routine
     * if is incremented to 3, and in the child_fork routine
     * 2 is added to it (2) to give 4.  So, once the fork is
     * completed the parent should see 3 and the child 4.
     */
    
    atfork ((cma_t_address)&atfork_data,
	    pre_fork,
	    parent_fork,
	    child_fork);

    /*
     * Create a number or worker threads; which upon fork
     * will be cleaned up in the forked process and should
     * NEVER execute there.
     */

    for (index = 0; index < NUM_THDS; index++) 
        cma_thread_create (&thd[index], &cma_c_null, worker, cma_c_null_ptr);

    pid = fork ();			/* fork a child */

    if (pid == -1) {
	cts_failed ("fork(2) failed with %d", errno);
	exit (-1);
	}
    else {
	if (pid == 0) {			    /* forked process */
	    cma_delay (10.0);
	    if (atfork_data != 4) {
		cts_failed ("Child atfork routine did not work.");
		cts_result ();
		}
	    exit (1);
	    }
	else {				    /* parent process */
	    while (wait(&wait_result) != pid);
#if defined(SNI_SVR4_POSIX)
        if (WEXITSTATUS(wait_result) != 1)
#else
        if (wait_result.w_T.w_Retcode != 1)
#endif /* defined(SNI_SVR4_POSIX) */
		cts_failed ("Received bad exit status from forked child.");
	    if (atfork_data != 3)
		cts_failed ("Parent atfork routine did not work.");
	    cts_result ();
	    }
	}

    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_001.C */
/*  *4     5-JUN-1991 19:09:47 CURTIN "Total remake" */
/*  *3    29-MAY-1991 18:03:15 BUTENHOF "Fix compilation problems" */
/*  *2     1-MAR-1991 16:43:09 BUTENHOF "Fix header" */
/*  *1    14-FEB-1991 00:09:05 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_001.C */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_003.C */
/*  *3    18-JUN-1991 17:19:45 CURTIN "Adding rios specific include" */
/*  *2    17-JUN-1991 15:03:06 CURTIN "Fixed some typing, removed erroneous data" */
/*  *1     6-JUN-1991 17:44:04 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_003.C */
