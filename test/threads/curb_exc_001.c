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
 * $Log: curb_exc_001.c,v $
 * Revision 1.1.53.2  1996/02/17  23:30:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:27  marty]
 *
 * Revision 1.1.53.1  1995/12/11  23:00:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:46  root]
 * 
 * Revision 1.1.51.1  1994/04/01  20:10:35  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:20  jd]
 * 
 * Revision 1.1.49.1  1993/10/12  19:36:17  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:24  rhw]
 * 
 * Revision 1.1.3.3  1992/12/30  16:33:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:08:58  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:38:51  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:32:56  sekhar]
 * 
 * Revision 1.1  1992/01/19  03:22:45  devrcs
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
 *	CURB_EXC_001
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	The cma__ts_open and cma__ts_open2 wrapper macros did a
 *	direct return when an error was received from the kernel
 *	I/O function. That prevented ENDTRY from popping the
 *	exception context, causing the next RAISE to deliver the
 *	exception to the open() wrapper frame (even though it has
 *	since returned). This program tests the fix.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	11 November 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#if ((_CMA_UNIX_TYPE == _CMA__SVR4) && (!defined(_CMA_BSD_CMP_)))
#include <sys/types.h>
#include <fcntl.h>
#endif
#include <signal.h>		/* Signal definitions */
#include <sys/file.h>
#include <sys/stat.h>

EXCEPTION appl_e_enoent;	/* No such file or directory */

int
main	(int argc, char *argv[], char *envp[])
    {
    int fd;


    cts_test ("CURB_EXC_001", "test exception bugfix in open() wrappers");
    cma_init ();

    EXCEPTION_INIT (appl_e_enoent);
    exc_set_status (&appl_e_enoent, ENOENT);

    TRY {
	fd = open ("./doesnotexist", O_RDONLY,  S_IRWXU);

	if (fd == -1) {
	    EXCEPTION openexc;


	    EXCEPTION_INIT (openexc);
	    exc_set_status (&openexc, errno);
	    RAISE (openexc);
	    }

	}
    CATCH (appl_e_enoent) {
	cts_comment ("caught ENOENT exception");
	}
    CATCH_ALL {
	cts_failed ("got unexpected exception:");
	exc_report (THIS_CATCH);
	exit (-1);
	}

    ENDTRY

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CURB_EXC_001.C */
/*  *1    11-NOV-1991 13:23:49 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CURB_EXC_001.C */
