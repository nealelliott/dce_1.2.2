/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crvb_exc_001.c,v $
 * Revision 1.1.51.2  1996/02/17  23:30:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:55  marty]
 *
 * Revision 1.1.51.1  1995/12/11  22:59:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:13  root]
 * 
 * Revision 1.1.49.1  1993/10/12  19:35:09  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:29:54  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:30:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:06:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:20:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA services
 *
 *  FILENAME: 
 * 
 * 	CRVB_EXC_001.C
 *
 *  TEST TOPICS:
 *
 *	EXC  -	Exceptions
 * 
 *  ABSTRACT:
 *
 *	Test of the CMA exception package.
 *
 *  AUTHORS:
 *
 *	Robert Conti
 *
 *  CREATION DATE:
 *
 *	2 August 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	002	R. Conti	5 December 1989	
 *		This version is consistent with the CMA_EXCEPTION.*
 *		files at CMS generation number 4 -- the "maximally
 * 		improved SRC" packages.	
 *	003	R. Conti	5 December 1989	
 *		This version is consistent with the version that
 *		implements C++ like handlers.
 *	004	R. Conti	5 December 1989	
 *		This version adds the {}'s of Bjarne's proposal.
 *	005	R. Conti	5 December 1989	
 *		Add calls to show how exceptions are equivalenced to statuses.
 *	006	R. Conti	6 December 1989	
 * 		Add test for uninitialized exception before unhandled one.
 *	007	Dave Butenhof	8 December 1989
 *		Add cma_init call.
 *	008	Bob Conti	13 December 1989
 *		Add #ifdefs to make this truly portable and add test of FINALLY.
 *	009	Bob Conti	18 December 1989
 *		Add tests of FINALLY combined with CATCHes.  While we
 *		document that such usage is unsupported we include the
 *		usage in this test so that we will know if we inadvertantly
 *		change its behavior.
 *	010	Bob Conti	25 December 1989
 *		Add test of raising exception in FINALLY or in CATCH.
 *	011	Paul Curtin	4 Septemnber 1990
 *		Adjust cts_test statement to reflect CMA/Pthread naming.
 *	012	Dave Butenhof	22 January 1991
 *		Fix exception names.
 *	013	Dave Butenhof	01 April 1991
 *		Fix use of exception package
 */

#include <dce/cma.h>
#include <dce/cts_test.h>


main ()
    {
    EXCEPTION e1, e2, uninitialized;	/* Declare three exceptions */
#ifdef VMS
    globalvalue ss$_accvio;		/* Import a VMS condition code */
#endif

    cts_test ("CRVB_EXC_001", "Test portable exception package");

    cma_init ();

    /*
     * Initialize two of the three exceptions.
     */
    EXCEPTION_INIT(e1);
    EXCEPTION_INIT(e2);

#ifdef VMS
    /*
     * Make exception e2 equivalent to a system status code
     * Catch clauses naming this exception will match the 
     * the corresponding system status when raised as an exception.
     */
    exc_set_status (&e2, ss$_accvio);
#endif

    /*
     * Now try out various test cases.
     */

    printf ("** Test that only one exception part triggers [4]\n");
    TRY {
	printf ("1 - about to raise the exception\n");
	RAISE (e1);
    }
    CATCH (e2) {
	cts_failed ("****Error - in wrong exception handler");
    }
    CATCH (e1) {
	printf ("2 - handling the exception\n");
    }
    CATCH (e2) {
	cts_failed ("****Error - in wrong exception handler");
    }
    FINALLY {
	/* Test (unsupported) behavior of FINALLY combined with CATCH */
	printf ("3 - in FINALLY after CATCH caught exception\n");
    }
    ENDTRY
    printf ("4 - Normal fall through ENDTRY\n");

    printf ("** Test that a raise works in CATCH [4]\n");
    TRY {
	TRY {
	    printf ("1 - in inner try\n");
	    RAISE(e1);
        }
        CATCH (e1) {
	    printf ("2 - about to raise in CATCH\n");
 	    RAISE (e2);
        }
	CATCH (e2) {
	    printf ("****Error - in wrong handler\n");
	}
        ENDTRY
    }
    CATCH (e2) {
	printf ("3 - absorbing exception\n");
    }
    ENDTRY
    printf ("4 - Normal fall through ENDTRY\n");

    printf ("** Test that a raise works in FINALLY [4]\n");
    TRY {
	TRY {
	    printf ("1 - in inner try\n");
        }
        FINALLY {
	    printf ("2 - about to raise in FINALLY \n");
 	    RAISE (e2);
        }
        ENDTRY
    }
    CATCH (e2) {
	printf ("3 - absorbing exception\n");
    }
    ENDTRY
    printf ("4 - Normal fall through ENDTRY\n");


    printf ("** Test that catchall handler can be found [4]\n");
    TRY {
	printf ("1 - about to raise the exception\n");
	RAISE (e1);
    }
    CATCH (e2) {
	cts_failed ("****Error - in wrong exception handler");
    }
    CATCH_ALL {
	printf ("2 - in catchall handler\n");
    }
    FINALLY {
	/* Test (unsupported) behavior of FINALLY combined with CATCH */
	printf ("3 - in FINALLY after catchall handler\n");
    }
    ENDTRY
    printf ("4 - Normal fall through ENDTRY\n");


    printf ("** Test that normal fall through works [3]\n");
    TRY {
	TRY {
	    printf ("1 - in TRY \n");
	}
	ENDTRY
	printf ("2 - After the ENDTRY\n");
    }
    ENDTRY
    printf ("3 - Normal fall through ENDTRY\n");


    printf ("** Test that nested TRYs work with exception [4]\n");
    TRY {
	TRY {
	    printf ("1 - about to raise the exception\n");
	    RAISE (e1);
	}
	FINALLY {
	    printf ("2 - in FINALLY with exception\n");
	}
	ENDTRY
	cts_failed ("**** Error, fell through the ENDTRY");
    }	
    CATCH_ALL {
	printf ("3 - in catchall handler\n");
    }
    ENDTRY
    printf ("4 - Normal fall through ENDTRY\n");


    printf ("** Test that TRYs can nest in TRYs [7]\n");
    TRY {
        TRY {
	    printf ("1 - about to fall through TRY\n");
	}
	FINALLY {
	    printf ("2 - in FINALLY with normal fallthrough\n");
	}
        ENDTRY
	printf ("3 - After the ENDTRY\n");

        TRY {
	    printf ("4 - about to raise the exception\n");
	    RAISE (e1);
	}
	FINALLY {
	    printf ("5 - in FINALLY with normal fallthrough\n");
	}
        ENDTRY
	cts_failed ("****Error, fell through an ENDTRY with exception");
    }
    CATCH_ALL {
	printf ("6 - in CATCHALL handler\n");
    }
    ENDTRY
    printf ("7 - Normal fall through ENDTRY\n");


    printf ("** Test that TRYs can nest in CATCH [6]\n");
    TRY {
        TRY {
	    printf ("1 - about to raise the exception\n");
	    RAISE (e1);
	}
        CATCH (e1) {

	    printf ("2 - in exception handler\n");
	    TRY {
	        printf ("3 - about to fall through TRY\n");
	    }
	    ENDTRY

	    TRY {
		printf ("4 - about to raise same exception\n");
		RAISE (e1);
	    }
	    ENDTRY
	}
	FINALLY {
	    /* Test (unsupported) behavior of FINALLY combined with CATCH */
	    cts_failed (
		"**** Error: FINALLY entered after exception raised in CATCH");
	}
        ENDTRY
    }
    CATCH_ALL {
	printf ("5 - in CATCHALL handler\n");
    }
    ENDTRY
    printf ("6 - Normal fall through ENDTRY\n");

    /*
     * Note: like Bjarne's THIS CATCH construct, a RERAISE
     * is only valid if lexically nested within a CATCH clause.
     */
    printf ("** Test that RERAISE works [6]\n");
    TRY {
        TRY {
	    printf ("1 - about to raise the exception\n");
	    RAISE (e1);
	}
        CATCH (e1) {

	    printf ("2 - in exception handler\n");
	    TRY {
	        printf ("3 - about to fall through TRY\n");
	    }
	    ENDTRY

	    TRY {
		printf ("4 - about to raise same exception\n");
		RERAISE;  /* Reraise e1 */
	    }
	    ENDTRY
	}
        ENDTRY

    }
    CATCH (e1) {
	printf ("5 - in outer handler\n");
    }
    ENDTRY
    printf ("6 - Normal fall through ENDTRY\n");


    /*
     * Note: initialized exceptions contain a sentinel that we use
     * to check if an exception was declared but not initialized.
     */
    printf ("** Test what happens with uninitialized exception [3]\n");
    TRY {
        TRY {
	    printf ("1 - about to raise the exception\n");
	    RAISE (uninitialized);
	    cts_failed ("****Error - did not raise");
	}
        ENDTRY
    }
    CATCH (exc_e_uninitexc) {
	printf ("2 - caught special implementation defined exception\n");
    }
    ENDTRY
    printf ("3 - Normal fall through ENDTRY\n");


    printf ("** Test what happens with unhandled exception [1]\n");
    TRY {
        TRY {
	    cts_comment ("1 - about to raise the exception, will look like accvio \n");
	    RAISE (e2);
	    cts_failed ("****Error - did not raise");
	}
        CATCH (e1) {
	    cts_failed ("****Error - in wrong exception handler");
	}
        ENDTRY
    }
    ENDTRY

    cts_failed ("****Error - fell through outer ENDTRY");

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_EXC_001.C */
/*  *8     1-APR-1991 17:17:44 BUTENHOF "Fix use of exception package" */
/*  *7    24-JAN-1991 00:15:31 BUTENHOF "Fix exception names" */
/*  *6     5-SEP-1990 10:52:12 CURTIN "Convert to new CMA interfaces" */
/*  *5    18-JUL-1990 23:03:53 SCALES "Reduce use of COMMNT" */
/*  *4     3-JUL-1990 12:03:50 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:28:50 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2     7-JUN-1990 11:36:17 SCALES "Updates to CTS" */
/*  *1    17-MAY-1990 13:57:58 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CRVB_EXC_001.C */
