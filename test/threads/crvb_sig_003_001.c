/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: crvb_sig_003_001.c,v $
 * Revision 1.1.69.2  1996/02/17  23:30:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:08  marty]
 *
 * Revision 1.1.69.1  1995/12/11  22:59:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:27  root]
 * 
 * Revision 1.1.67.1  1994/04/01  20:09:53  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:14:44  jd]
 * 
 * Revision 1.1.65.1  1993/10/12  19:35:36  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:07  rhw]
 * 
 * Revision 1.1.5.4  1992/12/30  16:31:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:18  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:38:25  devsrc
 * 	[OT 5373] SNI/SVR4 Merge UPDATE: Fixed function prototype headers to eliminate
 * 	          compiler errors on AIX3.2
 * 	[1992/09/24  20:03:57  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:40  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/03  14:31:27  bolinger
 * 	Move current 1.0.1-backed file (modified to work with new
 * 	threads source base) from private branch into 1.0.2 tree.
 * 	(See OT defect 5300.)
 * 	[1992/09/02  13:44:26  bolinger]
 * 
 * Revision 1.1.3.3  1992/08/30  19:09:50  bolinger
 * 	Force a bus error on the RIOS by changing how the fault
 * 	is provoked.
 * 
 * Revision 1.1.3.2  1992/08/12  21:03:30  bolinger
 * 	Need to handle SIGBUS as well as SIGSEGV to catch bad memory
 * 	reference reliably.
 * 
 * Revision 1.1.2.2  1992/06/12  19:27:55  bolinger
 * 	Fix OT defect 4198:  dummy out test when _CMA_SYNC_KILL_ is enabled in
 * 	the CMA we're using.
 * 	[1992/06/10  22:09:31  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:19:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990 by
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
 *	CRVB_SIG_003_001
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	This program tests the CMA implementation of the POSIX
 *	'sigaction' routine.
 *	
 *
 *  AUTHORS:
 *
 *      Paul Curtin
 *
 *  CREATION DATE:
 *
 *	3 December 1990
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <dce/cma_ux.h>
#include <signal.h>
#include <dce/cma_px.h>
#include <dce/cma_errno.h>
#   if _CMA_UNIX_TYPE == _CMA__SVR4
#include <dce/cma_defs.h>
#endif

cma_t_integer  segv_flag = 1;

void
segv_handler
#   if _CMA_UNIX_TYPE == _CMA__SVR4
	(
	int					sig,
	cma__t_siginfo		siginfo,
	cma__t_sigcontext	*scp)
#   else
        (
	int			sig,
	int			siginfo,
	struct sigcontext	*scp)
#   endif
    {
    
    cts_result();
    segv_flag = 0;
    exit(0);
    }

static cma_t_address
segv_thread	(cma_t_address arg)
    {
    struct sigaction vec;
    struct sigaction ovec;
    sigset_t empty_mask;
    int flags;
    int sig;
    int number = 9;
    int i;
    int arr[2];

    vec.sa_handler = segv_handler;
	sigemptyset(&vec.sa_mask);
    vec.sa_flags = 0;
    sigaction (SIGSEGV, &vec, &ovec);
	sigaction (SIGBUS, &vec, &ovec);

    for (i = 0; i < 1000000000; ++i) {
      *(char *)i = 10;
    }

    return;
    }


main ()
    {
    cma_t_thread       thread1;
    cma_t_integer      argument;
    cma_t_exit_status  status;
    cma_t_address      result;
	sigset_t		set;
    
    cts_test ("CRVB_SIG_003_001", "Test sigaction");

    cma_init ();

	/*
	 * 1.0.1 _CMA_SYNC_KILL_ hack
	 */
    sigemptyset(&set);
    sigaddset(&set, SIGSEGV);

    cma_thread_create (
	    &thread1,
	    &cma_c_null,
	    segv_thread,
	    (cma_t_address)&argument);

    cma_thread_join (&thread1, &status, &result);

    if (segv_flag) cts_failed ("Handler to catch SIGSEGV did not execute.");

    cts_result ();

    }


/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_003_001.C */
/*  *1    17-DEC-1990 17:02:07 CURTIN "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_SIG_003_001.C */
