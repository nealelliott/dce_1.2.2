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
 * $Log: cuvb_wrp_001.c,v $
 * Revision 1.2.54.1  1996/06/04  22:14:31  arvind
 * 	Add cma.h for wrappers
 * 	[1996/03/08  22:18 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.2.52.2  1996/02/17  23:31:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:40  marty]
 * 
 * Revision 1.2.52.1  1995/12/11  23:01:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:57  root]
 * 
 * Revision 1.2.49.1  1994/04/01  20:11:00  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:18  jd]
 * 
 * Revision 1.2.47.1  1993/10/12  19:36:40  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:35  rhw]
 * 
 * Revision 1.2.3.3  1992/12/30  16:51:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:56  zeliff]
 * 
 * Revision 1.2.3.2  1992/09/29  20:39:38  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:33:34  sekhar]
 * 
 * Revision 1.2  1992/01/19  22:14:43  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
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
 *	CUVB_WRP_001
 *
 *  TEST TOPICS:
 *
 *	WRP  -  U*IX wrappers
 *
 *  ABSTRACT:
 *
 *		Make sure that DECthreads are still present in forked
 *		process, BUT the only survivor of the fork() should 
 *		the forking thread.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	13 February 1991
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 *	002	Paul Curtin	05 June 1991
 *		Total revamp of test - Make sure that DECthreads
 *		are still present, BUT the only survivor of the
 *		fork() should the forking thread.
 *	003	Paul Curtin	18 June 1991
 *		Add RIOS specific include.
 */

/*
 *  INCLUDE FILES
 */

/*
 * Include cma.h first, so wrappers are defined
 * before any other headers are included.  cma.h
 * is also included later on; this include makes
 * that one a no-op.
 */
#if (_CMA_VENDOR_ == _CMA__HP)
#include <cma.h>
#endif

#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <dce/cma.h>
#include <dce/cts_test.h>
#if _CMA_PLATFORM_ == _CMA__IBMR2_UNIX
# include <sys/m_wait.h>
#endif
#ifdef __OSF__
#include <errno.h>
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

int
main	(int argc, char *argv[], char *envp[])
    {
    cma_t_integer   index;
#if defined(SNI_SVR4_POSIX)
	int wait_result;
#else
    union wait      wait_result;
#endif	/* defined(SNI_SVR4_POSIX) */

    cts_test ("CUVB_WRP_001", "Test fork(2) wrapper");
    cma_init ();

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
	    cts_result ();
	    }
	}

    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_001.C */
/*  *5    18-JUN-1991 17:20:00 CURTIN "Adding rios specific include" */
/*  *4     5-JUN-1991 19:09:47 CURTIN "Total remake" */
/*  *3    29-MAY-1991 18:03:15 BUTENHOF "Fix compilation problems" */
/*  *2     1-MAR-1991 16:43:09 BUTENHOF "Fix header" */
/*  *1    14-FEB-1991 00:09:05 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_WRP_001.C */
