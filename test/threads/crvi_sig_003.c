/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvi_sig_003.c,v $
 * Revision 1.1.58.2  1996/02/17  23:30:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:25  marty]
 *
 * Revision 1.1.58.1  1995/12/11  23:00:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:42  root]
 * 
 * Revision 1.1.56.1  1993/10/12  19:36:12  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:21  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:32:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:22:27  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/05  20:42:14  bolinger
 * 	To permit this test to process signals asynchronously, avoid
 * 	entering CMA kernel while waiting for them:  just spin while
 * 	waiting.
 * 	[1992/03/05  20:41:21  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990, 1991 by
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
 *	CRVI_SIG_003
 *
 *  TEST TOPICS:
 *
 *	SIG  -  U*IX signal handling
 *
 *  ABSTRACT:
 *
 *	This is simply an infinite loop which reports the effects of signals.
 *	The idea is to run this program in one process, and use 'kill' to
 *	send signals to it from another process.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	22 March 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	002	Dave Butenhof	20 February 1991
 *		Remove alert control kludge (which worked around early bug
 *		in alert delivery).
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <dce/cts_test.h>


cma_t_boolean working = cma_c_true;


main ()
    {
    cts_test ("CRVI_SIG_003", "Test U*IX signal handling.");

    printf ("\nProcess pid is %d.\n\n", getpid ());

    cma_init ();

    while (working) {
	    TRY {
			while (cma_c_true)
				;
	        }
	    CATCH (exc_e_illinstr) {
	        printf ("Handling a SIGILL exception.\n");
	        }
	    CATCH (exc_e_SIGTRAP) {
	        printf ("Handling a SIGTRAP exception.\n");
	        }
	    CATCH (exc_e_SIGIOT) {
	        printf ("Handling a SIGIOT exception.\n");
	        }
	    CATCH (exc_e_SIGEMT) {
	        printf ("Handling a SIGEMT exception.\n");
	        }
	    CATCH (exc_e_aritherr) {
	        printf ("Handling a SIGFPE exception.\n");
	        }
	    CATCH (exc_e_illaddr) {
	        printf ("Handling a SIGSEGV or SIGBUS exception.\n");
	        }
	    CATCH (exc_e_SIGSYS) {
	        printf ("Handling a SIGSYS exception.\n");
	        }
	    CATCH (exc_e_SIGPIPE) {
	        printf ("Handling a SIGPIPE exception.\n");
	        }
	    CATCH (exc_e_excpu) {
	        printf ("Handling a SIGXCPU exception.\n");
	        }
	    CATCH (exc_e_exfilsiz) {
	        printf ("Handling a SIGXFSZ exception.\n");
	        }
	    CATCH (cma_e_alerted) {
	        printf ("Handling an alert.\n");
	        }
	    CATCH_ALL {
	        cts_failed ("Unexpected exception.\n");
	        }
	    ENDTRY
	    }

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVI_SIG_003.C */
/*  *9    20-FEB-1991 23:40:52 BUTENHOF "Fix alert control" */
/*  *8    12-FEB-1991 01:31:22 BUTENHOF "Changes to alert control" */
/*  *7    24-JAN-1991 00:15:46 BUTENHOF "Fix exception names" */
/*  *6     4-SEP-1990 16:59:59 SCALES "Change filename references" */
/*  *5     4-SEP-1990 16:01:32 SCALES "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:06:29 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:33:25 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:38:06 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:58:34 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVI_SIG_003.C */
