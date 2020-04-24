/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: euvb_exc_nbi_001.c,v $
 * Revision 1.1.37.2  1996/02/17  23:31:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:55  marty]
 *
 * Revision 1.1.37.1  1995/12/11  23:02:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:11  root]
 * 
 * Revision 1.1.35.1  1994/04/01  20:11:21  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:04  jd]
 * 
 * Revision 1.1.33.1  1993/10/12  19:37:01  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:45  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:39  devrcs
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
 *	EUVB_EXC_NBI_001
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	For applications that wish to detect writes to a pipe without
 *	a reader by handling the exc_SIGPIPE_e exception, test that
 *	the thread-synchronous I/O package correctly propagates the
 *	SIGPIPE signal and retains data integrity (e.g., release
 *	mutexes).
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	06 November 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>

int tube[2];

int
main	(int argc, char *argv[], char *envp[])
    {
    char	outmsg[10];
    int		res, i;


    cts_test ("EUVB_EXC_NBI_001", "test SIGPIPE handling on write/send");

    if (pipe (tube) == -1)
	cts_failed ("system call failure: pipe(tube)");

    if (close (tube[0]) == -1)
	cts_failed ("system call failure: close(tube[0])");

    /*
     * Try to write to the broken pipe. This ought to cause the SIGPIPE
     * signal to be sent to the process. DECthreads will convert it into the
     * exc_SIGPIPE_e exception. We attempt the write repeatedly to ensure
     * that terminating the write with an exception doesn't cause the thread
     * synchronous I/O package to be left in an inconsistent state (e.g.,
     * with a mutex locked). Presumably, a locked mutex would cause the next
     * write to hang. (NOTE: we really need a watchdog timer here!)
     */
    for (i = 0; i < 5; i++) {
	cts_comment ("attempt %d:", i);
	TRY {
	    res = write (tube[1], outmsg, 10);
	    cts_failed ("  write to broken pipe didn't raise SIGPIPE");

	    if (res != 0)
		cts_comment ("   ret %d, errno %d", res, errno);

	    }
	CATCH (exc_SIGPIPE_e) {
	    cts_comment ("  SIGPIPE received");
	    }
	CATCH_ALL {
	    cts_failed ("  unexpected exception raised by write to broken pipe");
	    }
	ENDTRY
	}

    if (close (tube[1]) == -1)
	cts_failed ("system call failure: close(tube[1])");

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_EXC_NBI_001.C */
/*  *1     6-NOV-1991 09:07:22 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EUVB_EXC_NBI_001.C */
