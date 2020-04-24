/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: file_test.c,v $
 * Revision 1.1.408.1  1996/10/17  18:31:00  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:55:31  damon]
 *
 * $EndLog$
 */
/*
 * file_test.c -- 
 *    The main file for the basic file system test program.
 */

#ifdef	SUNOS5
#include <sys/types.h>
#endif
#include <stdio.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_spoke.h>
#include <utils/itl/itl_spokeDFS.h>

/*
 *   The exported variables below are used to start up spokes
 *   automatically when the -A option is used.
 */
PRIVATE char *inputFileName = (char *) 0;
PRIVATE char *portPrefix = (char *) 0;
PRIVATE char *listenName =  (char *) 0;
PRIVATE char *connectName =  (char *) 0;
PRIVATE char *configFile =  (char *) 0;
PRIVATE char *princName =  (char *) 0;
PRIVATE char *passWord =  (char *) 0;
PRIVATE char *logFile =  (char *) 0;
PRIVATE char *traceDumpFile = (char *) 0;

EXPORT char *OutputFileName = (char *) 0;
EXPORT char *PrincipalName = (char *) 0;
EXPORT char *PrincipalPasswd = (char *) 0;

PRIVATE boolean_t changeInteractiveMode = FALSE;
PRIVATE boolean_t changeVerboseMode     = FALSE;
PRIVATE boolean_t changeInterfaceMode   = FALSE;
PRIVATE boolean_t changeDisplayLines    = FALSE;

/*
 *  PrintUsage()
 *     Function for printing information about test invocation.
 *
 *  CAUTION:
 *     Note that we do not print info about the -P option.  It is
 *     an internal option only.
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
    fprintf(stderr, "\t[-c <port_name>] -> name of port for spoke to connect to\n");
    fprintf(stderr, "\t[-l <port_name>] -> name of port for spoke to listen on\n");
    fprintf(stderr, "\t[-p <port_name>] -> name for ITL server port\n");
    fprintf(stderr, "\t[-t <trace_file_name>] -> name for icl trace file.\n");
    fprintf(stderr, "\t[-v] -> verbose mode\n");
    fprintf(stderr, "\t[-C -> <configure-file>] -> for Hub-spoke setup>\n");
    fprintf(stderr, "\t[-D] -> display mode\n");
    fprintf(stderr, "\t[-E] -> interface mode\n");
    fprintf(stderr, "\t[-I <filename>] -> use file for input\n");
    fprintf(stderr, "\t[-O <filename>] -> redirect output to file\n");
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

	      case 'c':  /* socket to connect to if acting as a spoke */
		connectName = *++argv;
		argc--;
		break;

	      case 'l':  /* socket to listen on if acting as a spoke */
		listenName = *++argv;
		argc--;
		break;

	      case 'p': /* define a socket for the ITL server thread */
		portPrefix = *++argv;
                --argc;
		break;

	      case 't': /* define icl trace file, and activate icl dumps */
		traceDumpFile = *++argv;
                --argc;
		break;

  	      case 'v': /* set verbose mode */
		changeVerboseMode = TRUE;
		break;

	      case 'C': /* set the config file for Hub */
		configFile = *++argv;
                --argc;
		break;

	      case 'D':  /*  Display command lines as they are read  */
		changeDisplayLines = TRUE;
		break;

	      case 'E': /*  Interface mode */
		changeInterfaceMode = TRUE;
		break;

	      case 'I': /* set the input file  */
		inputFileName = *++argv;
                --argc;
		break;

	      case 'O': /*  Redirect spoke output */
                logFile = *++argv;
                --argc;
		break;

	      case 'P': /*  principle authentication (for spoke). */
		        /* NOTE: INTERNAL USE ONLY, for auto start of spokes */
                princName = *++argv;
                --argc;
                passWord = *++argv;
                --argc;
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
	exit(-1);

    /*
     *  Initialize the ITL library.  This MUST be done before anything 
     *  else, so that any invocation of ITL routines, e.g. for printing,
     *  will work correctly.
     */
    if (itl_InitItl(FALSE, TRUE, portPrefix)) {
	fprintf(stderr, "ITL Initialization failure.\n");
	exit(-1);
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
     *  Initialize the ITL file command set (before spokes listen).
     */
    itlCM_AddCMCommands();
    itlFile_AddFileCommands();

    if (listenName || connectName)
    {
	itlSpoke_AddCmds();
	itlSpokeDFS_AddCommands();
	if (listenName ) {
	    itlSpoke_Listen(listenName, configFile);
        }
	if (connectName) {
	    itlSpoke_Connect(connectName);
        }
    }

    if (princName && passWord)
       itl_dce_login(princName, passWord);

    if (logFile)
          spoke_RedirectOutput(logFile);
    if ( listenName && configFile ) {
       if ( spoke_AutoSpoke(configFile, listenName) != 0 ) {
          itl_Error("Failed to auto start spokes\n");
          itl_CleanupItl();
          exit(exitValue); 
       }
       spoke_syncHubSpoke();
     }
    
    if (traceDumpFile != NULL)
    {
	itlu_Init();
	if (itlu_TraceDumpOnError(TRUE, traceDumpFile))
	{
	    itl_Error("FAILED to initialize icl trace dump.\n");
	    exit(-1);
	}
    }

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
