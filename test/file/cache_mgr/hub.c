/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * hub.c -- 
 *    Main program, "driver", for cache consistency tests.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hub.c,v $
 * Revision 1.1.8.1  1996/10/17  18:17:52  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:49  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:16  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:56:50  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:54  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:50:51  jaffe]
 * 
 * $EndLog$
 */

#include "hub.h"

/* Main program and utilties for the driver end of the multimachine tester.
 * Run as
 * hub machine-1 ip-port-1 machine-2 ip-port-2 ... <flags>
 * runs the tests on all specified machines.  Each client on the slave
 * machine runs the "wheel" program, which prints out its ip port
 * when it starts.
 *
 * Also provides subroutines for making test writing easy.  Note that most
 * tests will scale with the # of machines participating.  For example,
 * the mode checking tests will make a change at one site, and verify that
 * all of the others can see that change.  It might even try some concurrency,
 * such as start some readers, make a change, and verify that all readers
 * read one or the other value, and then do a few final reads that make
 * sure that the later readers read the new value.
 */

static DoTests(asetp, acx)
  struct wlr_clientSet *asetp;
  struct wlr_context *acx;
{
    long code, rcode;

    rcode = 0;	/* overall return code */

    code = wlr_RenameChecks(asetp, acx);
    printf("Rename checks return code %d\n", code);
    if (!rcode) rcode = code;

    code = wlr_ModeChecks(asetp, acx);
    printf("Mode checks return code %d\n", code);
    if (!rcode) rcode = code;

    code = wlr_DirScanChecks(asetp, acx);
    printf("DirScan checks return code %d\n", code);
    if (!rcode) rcode = code;

    code = wlr_DataChecks(asetp, acx);
    printf("Data checks return code %d\n", code);
    if (!rcode) rcode = code;

    printf("Overall tests returning %d ", rcode);
    if (rcode) printf("(failure)\n");
    else printf("(success)\n");

    return rcode;
}


main(argc, argv)
  int argc;
  char **argv;
{
    struct wlr_context cx;
    long code;
    struct wlr_clientSet *csetp;

    umask(0);
    ParseCommands(argc, argv, &cx, &csetp);	/* creates client structures */
    code = DoTests(csetp, &cx);			/* run tests on all clients */
    wlr_CleanUpDir(wlr_ClientSetFirst(csetp), ".");  /* clean up our mess */
    exit(code);
}
