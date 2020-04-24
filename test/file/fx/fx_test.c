/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fx_test.c,v $
 * Revision 1.1.11.1  1996/10/17  18:23:27  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:51:54  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:42:42  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:22  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:13  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:39  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  20:51:47  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  13:44:50  shl]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * fx_test.c -- 
 *    The main file for the DFS file exporter test program.
 */

#include <stdio.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>

PRIVATE char *inputFileName = (char *) 0;
PRIVATE char *portPrefix = (char *) 0;

PRIVATE boolean_t changeInteractiveMode = FALSE;
PRIVATE boolean_t changeVerboseMode     = FALSE;
PRIVATE boolean_t changeInterfaceMode   = FALSE;
PRIVATE boolean_t changeDisplayLines    = FALSE;

/*
 *  PrintUsage()
 *     Function for printing information about test invocation.
 *
 *  WARNING:
 *     Cannot use the ITL print routines, because ITL has not been
 *     initialized yet.
 */
PRIVATE void PrintUsage(programName)
  IN char *programName;   /* name of the test program */
{
    fprintf(stderr, "Usage: %s\n", programName);
    fprintf(stderr, "\t[-b] -> batch mode (no prompts)\n");
    fprintf(stderr, "\t[-p <port_name>] -> name for ITL server port\n");
    fprintf(stderr, "\t[-v] -> verbose mode\n");
    fprintf(stderr, "\t[-D] -> display mode\n");
    fprintf(stderr, "\t[-E] -> interface mode\n");
    fprintf(stderr, "\t[-I <filename>] -> use file for input\n");
}


/*
 * ProcessCmdLine(argc,argv) 
 *     Processes the command line input.
 *     All program options must be unique by first letter.
 *
 *  WARNING:
 *     Cannot use the ITL print routines, because ITL has not been
 *     initialized yet.
 */
PRIVATE boolean_t ProcessCmdLine(argc,argv)
  IN int argc;
  IN char **argv;
{
    boolean_t retVal = FALSE;
    char *testProgName = argv[0];

    while (--argc > 0) {
	if ((*++argv)[0]  == '-') {
	    switch ((*argv)[1]) {
              case 'b': /* set batch mode (non-interactive input) */
		changeInteractiveMode = TRUE;
		break;
	      case 'p': /* define a socket for the ITL server thread */
		portPrefix = *++argv;
                --argc;
		break;
  	      case 'v': /* set verbose mode */
		changeVerboseMode = TRUE;
		break;
	      case 'I': /* set the input file  */
		inputFileName = *++argv;
                --argc;
		break;
	      case 'D':  /*  Display command lines as they are read  */
		changeDisplayLines = TRUE;
		break;
	      case 'E': /*  Interface mode */
		changeInterfaceMode = TRUE;
		break;
	      default: /* illegal switch */
		fprintf(stderr, "Illegal switch on command line.\n\n");
		PrintUsage(testProgName);
		retVal = TRUE;
	    }
	} else {  /* unexpected command line arg */
	    fprintf(stderr, "unexpected arg: %s\n",argv);
	    PrintUsage(testProgName);
	    retVal = TRUE;
	}
    }
    return(retVal);
}


/*
 * Main - 
 */
void main(argc, argv)
  IN int argc;
  IN char **argv;
{
    int exitValue;

    /*
     *  Parse the command line.
     */
    if (ProcessCmdLine(argc, argv))
	exit(1);

    /*
     *  Initialize the ITL library.  This MUST be done before anything 
     *  else, so that any invocation of ITL routines, e.g. for printing,
     *  will work correctly.
     */
    if (itl_InitItl(FALSE, TRUE, portPrefix)) {
	fprintf(stderr, "ITL Initialization failure.\n");
	exit(1);
    }

    if (changeInteractiveMode)
	itl_SetInteractive(FALSE);
    if (changeVerboseMode)
	setVerbose(TRUE);
    if (changeDisplayLines)
	setDisplayLines(TRUE);
    if (changeInterfaceMode)
	itlIO_SetInterfaceMode(TRUE);

    /*
     *  Initialize the ITL file exporter command set.
     */
    itlFX_AddFXCommands();

    if(inputFileName) 
	exitValue = (itl_RunFromFile(inputFileName,"Do=> ") == FALSE) ? 0 : -1;
    else
	exitValue = (itl_LocalCommandLoop("Do=> ") != NULL) ? 0 : -1;

    if (exitValue)
	fprintf(stderr, "Test Program terminating with an error\n");

    /*  
     *  Free all the memory allocated for variables
     *  and clean up the ITL library.
     */
    itl_CleanupItl();

    exit(exitValue);
}


