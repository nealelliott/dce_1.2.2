/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fset_test.c,v $
 * Revision 1.1.108.1  1996/10/17  18:20:37  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:56  damon]
 *
 * Revision 1.1.103.1  1994/02/04  20:40:52  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:37  devsrc]
 * 
 * Revision 1.1.101.1  1993/12/07  17:42:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:02:59  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:27:18  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:01:38  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:23:15  jaffe
 * 	Transarc delta: jdp-add-itl-stuff 1.8
 * 	  Selected comments:
 * 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
 * 	    libitlFset.a
 * 	    Update error message in itlu_CheckReturnCode() so that the offending command
 * 	    is printed.
 * 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
 * 	    Fixed type in previous revision.
 * 	    Removed processing for '-d' flag'
 * 	    Changed interface and implementation of result checking routines.
 * 	    Provide routine for converting a value back to a name, assuming that
 * 	    the value--name mapping was constructed using itlu_ProcessValueMap().
 * 	    Checkpointing in order to import another delta.
 * 	    Fix misc. compilation problems and close inorder to open another delta.
 * 	    Make this stuff compile on the pmax.
 * 	Transarc delta: jdp-itl-add-fset-driver 1.2
 * 	  Selected comments:
 * 	    Add the code for fset_test, the fileset (ftutil) test driver.
 * 	    ITL fileset test driver.
 * 	    This delta adds "fset" to the list of build directories in the test tree
 * 	    and adds all of the test scripts.
 * 	[1992/10/27  18:35:49  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_aggr.h>
#include <utils/itl/itl_bomb.h>
#include <utils/itl/itl_fset.h>

PRIVATE boolean_t interactiveMode = TRUE;
PRIVATE boolean_t explicitMode    = FALSE;
PRIVATE char *inputFileName       = NULL;

PRIVATE char *portPrefix = NULL;


PRIVATE void PrintUsage(programName)
    IN char *programName;		/* name of the test program */
{
    itl_Message("Usage: %s\n", programName);
    itl_Message("\t[-b] -> batch mode (no prompts)\n");
    itl_Message("\t[-v] -> verbose mode\n");
    itl_Message("\t[-D] -> display mode\n");
    itl_Message("\t[-E] -> explicit mode\n");
    itl_Message("\t[-I <file name>] -> use file for input\n");
}


PRIVATE boolean_t ProcessCmdLine(argc,argv)
  IN int argc;
  IN char **argv;
{
    boolean_t retVal = FALSE;
    char *testProgName = argv[0];

    while (--argc > 0) {
	if ((*++argv)[0]  == '-') {
	    switch ((*argv)[1]) {

  	      case 'v': /* set verbose mode */
		setVerbose(TRUE);
		break;
	      case 'I': /* set the input file  */
		inputFileName = *++argv;
                --argc;
		break;
	      case 'D':  /* Display command lines as they are read  */
		setDisplayLines(TRUE);
		break;
              case 'b': /* Set batch mode (non-interactive input) */
		interactiveMode = FALSE;
		break;
	      case 'E': /* Explicit mode */
		explicitMode = TRUE;
		break;
	      default: /* illegal switch */
		itl_Error("Illegal switch on command line.\n\n");
		PrintUsage(testProgName);
		retVal = TRUE;
	    }
	} else {  /* unexpected command line arg */
	    itl_Message("unexpected arg: %s\n",argv);
	    PrintUsage(testProgName);
	    retVal = TRUE;
	}
    }
    return(retVal);
}


main(argc, argv)
    IN int argc;
    IN char **argv;
{
    long	code;
    char        *errorMessage;

    /*  Initialize the ITL library. */
    if (itl_InitItl(explicitMode, interactiveMode, portPrefix)) {
	itl_Error("ITL Initialization failure.\n");
	exit(1);
    }

    /*  Parse the command line. */
    if (ProcessCmdLine(argc, argv))
	exit(1);

    /* Initialize the utility package. */
    if (code = itlu_Init()) {
	itlu_ErrorMessage(code, &errorMessage);
	itl_Error("Failed to initialize the utility package: %s\n",
		       errorMessage);
	itlu_ErrorMessageFree(errorMessage);   /* just to be neat */
	exit(1);
    }

    /* Initialize the bomb point package. */
    if (code = itlBomb_Init()) {
	itlu_ErrorMessage(code, &errorMessage);
	itl_Error("Failed to initialize the bomb point package: %s\n",
		       errorMessage);
	itlu_ErrorMessageFree(errorMessage);   /* just to be neat */
	exit(1);
    }

    /* Initialize the aggregate package. */
    if (code = itlAggr_Init()) {
	itlu_ErrorMessage(code, &errorMessage);
	itl_Error("Failed to initialize the aggregate package: %s\n",
		       errorMessage);
	itlu_ErrorMessageFree(errorMessage);   /* just to be neat */
	exit(1);
    }

    /* Initialize the fileset package. */
    if (code = itlFset_Init()) {
	itlu_ErrorMessage(code, &errorMessage);
	itl_Error("Failed to initialize the fileset package: %s\n",
		       errorMessage);
	itlu_ErrorMessageFree(errorMessage);   /* just to be neat */
	exit(1);
    }

    /* Initialize the file (os_calls) command set. */
    itlFile_AddFileCommands();

    if(inputFileName) {
       if (itl_RunFromFile(inputFileName, "Do=> ")) {
	    itl_Error("Test program terminating with an error\n");
	    exit(1);
	}
    }
    else {
	if (itl_LocalCommandLoop("Do=> ") == NULL) {
	    itl_Error("Test loop terminating with an error\n");
	    exit(1);
	}
    }

    itl_CleanupItl();

    exit(0);
}


