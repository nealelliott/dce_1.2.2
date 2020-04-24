/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: evrb_exc_001.c,v $
 * Revision 1.1.6.2  1996/02/17  23:31:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:57  marty]
 *
 * Revision 1.1.6.1  1995/12/11  23:02:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:13  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:11:26  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:18:17  jd]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:10  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:48  devrcs
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
 *	EVRB_EXC_001
 *
 *  TEST TOPICS:
 *
 *	EXC  -  Exceptions
 *
 *  ABSTRACT:
 *
 *	There is a bug on VAX VMS in the 'cma5d' snapshot where a
 *	'resignalled' condition (ss$_debug, continuable conditions,
 *	etc.) could be passed on to the saved DECthreads handler for
 *	an outer TRY block... ad infinitum.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	18 November 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <cts_test.h>
#include <ssdef.h>

extern void VAXC$ESTABLISH ();
extern void lib$signal ();

EXCEPTION	test_this;

extern int handler1
#ifdef _EXC_PROTO_
	(unsigned int sigargs[], unsigned int mechargs[])
#else
	(sigargs, mechargs)
	unsigned int sigargs[];
	unsigned int mechargs[];
#endif
    {

    if (sigargs[1] == SS$_WASSET) {
	cts_comment ("native handler1 reporting for duty!");
	return SS$_CONTINUE;
	}
    else {
	cts_failed ("native handler1 called unexpectedly");
	return SS$_RESIGNAL;
	}

    }

extern int handler2
#ifdef _EXC_PROTO_
	(unsigned int sigargs[], unsigned int mechargs[])
#else
	(sigargs, mechargs)
	unsigned int sigargs[];
	unsigned int mechargs[];
#endif
    {

    if (sigargs[1] == SS$_WASSET)
	cts_comment ("native handler2 reporting for duty!");
    else
	cts_failed ("native handler2 called unexpectedly");

    return SS$_RESIGNAL;
    }

int
sub	(void)
    {
    VAXC$ESTABLISH (handler2);

    TRY {
	TRY {
	    TRY {
		lib$signal (SS$_WASSET, 0);	/* Signal success status */
		RAISE (test_this);
		}
	    CATCH (test_this) {
		cts_comment ("caught test (sub inner)");
		RERAISE;
		}
	    CATCH_ALL {
		cts_failed ("caught exception (sub inner):");
		exc_report (THIS_CATCH);
		}
	    ENDTRY
	    }
	CATCH (test_this) {
	    cts_comment ("caught test (sub middle)");
	    RERAISE;
	    }
	CATCH_ALL {
	    cts_failed ("caught exception (sub middle):");
	    exc_report (THIS_CATCH);
	    }
	ENDTRY
	}
    CATCH (test_this) {
	cts_comment ("caught test (sub outer)");
	RERAISE;
	}
    CATCH_ALL {
	cts_failed ("caught exception (sub outer):");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    }

int
main	(int argc, char *argv[], char *envp[])
    {
    cts_test ("EVRB_EXC_001", "Test nested TRY blocks & resignalled conditions");
    VAXC$ESTABLISH (handler1);
    EXCEPTION_INIT (test_this);

    TRY {
	TRY {
	    TRY {
		sub ();
		}
	    CATCH (test_this) {
		cts_comment ("caught test (main inner)");
		RERAISE;
		}
	    CATCH_ALL {
		cts_failed ("caught exception (main inner):");
		exc_report (THIS_CATCH);
		}
	    ENDTRY
	    }
	CATCH (test_this) {
	    cts_comment ("caught test (main middle)");
	    RERAISE;
	    }
	CATCH_ALL {
	    cts_failed ("caught exception (main middle):");
	    exc_report (THIS_CATCH);
	    }
	ENDTRY
	}
    CATCH (test_this) {
	cts_comment ("caught test (main outer)");
	}
    CATCH_ALL {
	cts_failed ("caught exception (main outer):");
	exc_report (THIS_CATCH);
	}
    ENDTRY

    cts_result ();
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element EVRB_EXC_001.C */
/*  *1    18-NOV-1991 10:25:07 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EVRB_EXC_001.C */
