/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: runtest.h,v $
 * Revision 1.1.66.1  1996/10/02  17:18:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:40  damon]
 *
 * Revision 1.1.60.2  1994/06/09  13:58:45  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:33:59  annie]
 * 
 * Revision 1.1.60.1  1994/02/04  20:11:52  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:30  devsrc]
 * 
 * Revision 1.1.58.1  1993/12/07  17:17:32  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:45:52  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/18  22:07:14  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:47:26  cjd]
 * 
 * Revision 1.1.4.2  1992/08/31  18:51:30  jaffe
 * 	Fixed missing Header
 * 	[1992/08/30  13:24:04  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  21:57:11  jdp
 * 	Transarc delta: ota-freed-frags-2464 1.5
 * 	  Files modified:
 * 	    anode: anode.p.h, block_alloc.c, mkdir.test
 * 	    anode: new-block-security.hm.int.test
 * 	    anode: new-block-security.rcvr.test, new-block-security.test
 * 	    anode: runtest.c, runtest.h, test_anode.c
 * 	  Selected comments:
 * 	    Closing only so I can fix another bug.
 * 	    So far only new tests and supporting code are done.  The seems to behave
 * 	    as expected since NBS and FB changes aren't in yet.
 * 	    Doesn't compile now.
 * 	    Define constants for max script, op and file name length.
 * 	    More work on freed block part of new block security.
 * 	    Now compiles but new code disabled.
 * 	    Close to edit another delta.
 * 	    Freed Block fragment stuff now seems to work.  all_tests succeeds.  This
 * 	    could be incorporated in to a release if anyone cared.
 * 	    Due to a change in episode configuration 2.15,16,or 17, the interface to
 * 	    buffer_EquivTran changed.  It no longer returns a code.
 * 	[1992/05/13  20:31:07  jdp]
 * 
 * Revision 1.1.2.2  1992/05/06  14:12:12  jdp
 * 	Initial revision.
 * 	[1992/05/05  23:42:23  jdp]
 * 
 * $EndLog$
 */
/* Copyright (C) 1991, 1990 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/runtest.h,v 1.1.66.1 1996/10/02 17:18:50 damon Exp $ */

#ifndef TRANSARC_EPI_RUNTEST_H
#define TRANSARC_EPI_RUNTEST_H

#include <dcedfs/episode/epi_thread.h>

extern int afsdb_episode_runtest;

#define EPIR_MAX_TESTNAME 64
#define EPIR_MAX_TESTFILENAME 96
#define EPIR_MAX_OPNAME 32

struct epir_operation {
    char *name;
    long (*proc)();
};

struct epir_operationTable {
    int nOperations;
    int sorted;				/* set if list is sorted by name */
    long (*testProlog)();
    long (*testEpilog)();
    long (*opProlog)();
    long (*opEpilog)();
    struct epir_operation *table;
};

struct epir_operationData {
    char *testFile;			/* file name being executed */
    struct epir_operationTable *ops;	/* parameters to RunTest */
    opaque rock;
    long flags;
    FILE *testStream;			/* stream to read test from */
    char testName[EPIR_MAX_TESTNAME];	/* from first line of script */
    long version;			/* test version number */
    int lineNumber;			/* line number in test file */
    char op[EPIR_MAX_OPNAME];		/* name of opcode */
    char line[256];			/* rest of line w/ cmd's parameters */
    char whereami[256];			/* location string for com_err */
    long errorCode;			/* code expected by this cmd */
    long lastCode;			/* code returned by last cmd */
    pthread_t pid;			/* LWP process ptr */
    int procId;				/* serial number of process; 0 ->
                                         * initial (main) process */
    int endProcessing;                  /* end command encountered */
};

#define EPIR_NEW_PROCESS 1
#define EPIR_NEW_TEST 2

extern int afsdb_episode_runtest;

EXPORT long epir_RunTest _TAKES((
  IN char *testFile,			/* file name of test script */
  IN struct epir_operationTable *ops,	/* array of defined operations */
  IN opaque rock,
  IN long flags));

EXPORT long epir_WaitAll _TAKES(( void ));

EXPORT long epir_StartTest _TAKES((
  IN int   argc,
  IN char *argv[],
  IN struct epir_operationTable *ops,	/* array of defined operations */
  IN opaque rock));

#endif
