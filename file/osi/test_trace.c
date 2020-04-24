/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_trace.c,v $
 * Revision 1.1.626.1  1996/10/02  18:12:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:21  damon]
 *
 * Revision 1.1.621.3  1994/07/13  22:18:57  devsrc
 * 	removed extra <<< theat was there
 * 	[1994/06/29  12:28:48  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  17:42:45  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:00:35  mbs]
 * 
 * Revision 1.1.621.2  1994/06/09  14:17:28  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:29:37  annie]
 * 
 * Revision 1.1.621.1  1994/02/04  20:27:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:38  devsrc]
 * 
 * Revision 1.1.619.1  1993/12/07  17:31:38  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:15:26  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/21  14:53:35  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:57:27  cjd]
 * 
 * Revision 1.1.5.4  1992/12/09  20:34:28  jaffe
 * 	Transarc delta: ota-ot6058-rcsid-parse-too-fragile 1.1
 * 	  Selected comments:
 * 	    The recent change in CML organization confused the heck out of the RCSID
 * 	    parsing code.  I've changed it to be much more robust to future
 * 	    confusion.  Also the built in package names were wrong so updated them.
 * 	    I tried the run the test_trace program to see if I had broken anything
 * 	    but that routine was badly written.  I augmented it with a perl script
 * 	    which verifies that the test_trace program's output is acceptable.
 * 	    Move assert tests to end so line numbers would be monotonic.
 * 	    Flush stdout and stderr so output is serialized deterministically.
 * 	    Fix pmax ASSERT_CODE.
 * 	[1992/12/04  17:13:21  jaffe]
 * 
 * Revision 1.1.5.3  1992/11/24  18:24:23  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:16:30  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  20:49:53  jaffe
 * 	Sync with Transarc, Replace missing RCS id
 * 	[1992/08/31  15:34:03  jaffe]
 * 
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    remove rt_r4 ifdefs.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:43:37  jaffe]
 * 
 * Revision 1.1.3.3  1992/05/22  14:18:28  treff
 * 	5/21/92 DCT @ OSF
 * 	#ifndef AFS_DEBUG around #define AFS_DEBUG
 * 	[1992/05/21  23:06:48  treff]
 * 
 * Revision 1.1.3.2  1992/05/05  04:30:23  mason
 * 	delta jdp-ot2556-osi-put-tracing-back 1.4
 * 	[1992/04/27  12:13:29  mason]
 * 
 * Revision 1.1  1992/01/19  02:42:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* test_trace.c --
 */

/* Copyright (C) 1991, 1990 Transarc Corporation - All rights reserved */
#ifndef AFS_DEBUG
#define AFS_DEBUG
#endif 

#define AFSL_DEBUG_LEVEL 3		/* set medium high for testing */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <osi.h>

FILEID ("$Header: /u0/rcs_trees/dce/rcs/file/osi/test_trace.c,v 1.1.626.1 1996/10/02 18:12:05 damon Exp $")

#define DEBUG_THIS_FILE 5

static int Predicate()
{
    printf ("NOT EXPECTING Predicate TO BE CALLED\n");
    return 5;
}

main(argc, argv)
  IN int argc;
  IN char *argv[];
{
    extern struct afsl_tracePackage afsl_tr_osi;
    int test;
    int i = 5;

#define MAX_ASSERT_TESTS 10
    if (argc > 1) goto test_asserts;

    /* Do trace tests */

    afsl_Trace (AFSL_TR_SLEEP, ("trace_test: EXPECTING NO OUTPUT 1"));

    afsl_TraceEnabled (1,1, AFSL_TR_ALWAYS,
		       ("trace_test:1: always : 0x%x %d 0%o %c%s%c",
			1234, 1234, 1234, '"', "string", '"'));

    afsl_tr_global.enabledTypes = -1;
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test:2: global enabledTypes is -1"));

    afsl_tr_global.enabledTypes = 4;
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test:3: global enabledTypes is 4"));

    afsl_tr_global.enabledTypes = 8;
    afsl_TraceCond (AFSL_TR_SLEEP, i==Predicate(),
		    ("trace_test: EXPECTING NO OUTPUT 2"));
    afsl_Trace (AFSL_TR_UNUSUAL,
		("trace_test:4: unusual: one=%d, str=%s", 1, "string"));

    afsl_tr_global.enabledTypes = 0;
    afsl_tr_osi.enabledTypes = 4;
    afsl_TraceCond (AFSL_TR_SLEEP, 10.0/3.0 > 3.0,
		    ("trace_test:5: osi enabledTypes is 4 (%i/%i=%g>%i.0)",
		     10, 3, 10.0/3.0, 3));
    afsl_TraceCond (AFSL_TR_SLEEP, i==4/*false*/,
		    ("trace_test: EXPECTING NO OUTPUT 3"));
    afsl_Trace (AFSL_TR_UNUSUAL, ("trace_test: EXPECTING NO OUTPUT 4"));

    afsl_SetFileNumber();

    afsl_tr_global.enabledTypes = 4;
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test: EXPECTING NO OUTPUT 5"));

    afsl_tr_global.enabledFiles = -1;
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test: EXPECTING NO OUTPUT 6"));

    afsl_tr_osi.enabledFiles = (1<<(5));
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test: EXPECTING NO OUTPUT 7"));

    afsl_tr_osi.enabledFiles = (1<<(5-1));
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test:6: enabledFiles is 1 << 5-1"));

    {	/* reset this initialization to testing add package */
	static struct afsl_tracePackage myPackage;
	afsl_trace.control = 0;
	afsl_AddPackage ("osi", &myPackage);
    }

    afsl_tr_osi.enabledFiles = -1;
    afsl_Trace (AFSL_TR_SLEEP, ("trace_test:7: enabledFiles is -1"));

    fflush(stdout); fflush(stderr);

    /* Now do assert tests by recursively calling ourselves */

    for (i=1; i<=MAX_ASSERT_TESTS; i++) {
	char buf[128];
	int code;

	fflush(stdout); fflush(stderr);

	sprintf (buf, "%s %d", argv[0], i);
	code = system (buf);
	if (code) {
	    if (errno) perror ("test_trace: assert");
#ifdef mips
#define ASSERT_CODE 0x8600
#else
#ifdef RIOS
#define ASSERT_CODE 0xffff8600
#else
#define ASSERT_CODE 0
#endif
#endif

	    if (code != ASSERT_CODE) {
		printf ("errno=%d, status = %x (code=%d, %ssignal=%d)\n",
			errno, code, (code&0xff),
			((code&0x8000) ? "coredump, ":""), (code>>8)&0x7f);
		exit (1);
	    }
	}
    }
    system ("rm -f core");
    exit (0);

test_asserts:
    /* we are being called recursively via system(3) */

    test = atoi (argv[1]);
    switch (test) {
      case 1:
	printf ("shouldn't assert at %d\n",__LINE__+1); fflush(stdout);
	assert (i == 5);
	break;
      case 2:
	printf ("assert at %d\n",__LINE__+1); fflush(stdout);
	assert (i == 4);
	break;
      case 3:
	printf ("shouldn't assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_Assert (i==5);
	break;
      case 4:
	printf ("assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_Assert (i==4);
	break;
      case 5:
	printf ("shouldn't assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_PAssert (i==5, ("i(%d) not 5", i));
	break;
      case 6:
	printf ("assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_PAssert (i==4, ("i(%d) not %d", i, 4));
	break;
      case 7:
	printf ("assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_AssertTruth (i==4);
	break;
      case 8:
	printf ("assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_PAssertTruth (i==4, ("i(%d) not 4", i));
	break;
      case 9:
	printf ("shouldn't assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_AssertCond (i==4);
	break;
      case 10:
	printf ("shouldn't assert at %d\n",__LINE__+1); fflush(stdout);
	afsl_PAssertCond (i==4, ("i(%d) not 4", i));
	break;
      default:
	exit (0xb6);
    }
    exit (0);
}
