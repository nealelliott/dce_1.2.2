/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_main.c,v $
 * Revision 1.1.414.1  1996/10/17  18:27:31  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:48  damon]
 *
 * Revision 1.1.409.1  1994/02/04  20:45:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:35  devsrc]
 * 
 * Revision 1.1.407.1  1993/12/07  17:44:45  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:28:07  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:12  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:23  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:16:47  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	  Selected comments:
 * 	    Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	    Initial checkin.
 * 	Transarc delta: fred-fix-itl-initialization 1.1
 * 	  Selected comments:
 * 	    Fix initialization code in file_test.c.  Also fixed the itl_main.c code
 * 	    to reflect how other ITL based programs should initialize the ITL.
 * 	    Changed the makefile to export two test scripts, that are there for
 * 	    validating changes to ITL.
 * 	    Changed how initialization is done, and fixed usage functions, as well
 * 	    as the function to process the command line.
 * 	[1992/09/23  21:25:54  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_main.c,v 1.1.414.1 1996/10/17 18:27:31 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    PRIVATE  HandleTempCommand
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1990, 1991
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $TALog: itl_main.c,v $
 * Revision 1.6  1994/11/01  22:29:50  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.5 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.5  1993/01/14  17:38:26  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.4 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.2  1992/08/12  14:57:59  fred
 * Fix initialization code in file_test.c.  Also fixed the itl_main.c code
 * to reflect how other ITL based programs should initialize the ITL.
 * Changed the makefile to export two test scripts, that are there for
 * validating changes to ITL.
 *
 * Changed how initialization is done, and fixed usage functions, as well
 * as the function to process the command line.
 * [from revision 1.1 by delta fred-fix-itl-initialization, revision 1.1]
 *
 * Revision 1.1  1992/08/10  20:11:14  fred
 * Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * Initial checkin.
 *
 * Initial checkin.
 * [added by delta fred-checkin-itl, revision 1.1]
 *
 * Revision 1.14  1992/07/21  17:22:26  oz
 * Removed (unused) declaration of qsort.
 * [from revision 1.13 by delta oz-3749-remove-qsort-decl-in-itl_main.c, revision 1.1]
 *
 * Revision 1.13  1992/03/26  12:53:43  jaffe
 * Prologue Changes for 1.0.1
 *
 *
 * [from revision 1.12 by delta jaffe-ttools-prologue, revision 1.1]
 *
 * Revision 1.12  1992/02/05  20:49:08  oz
 * 	Move include of tc.h to the front
 *
 * Revision 1.11  92/02/04  22:17:28  oz
 * 	 - When using tran-C the server thread must be started
 * 	   after itlTC_ module is initialized.
 * 	[92/02/04  14:05:35  oz]
 * 
 * Revision 1.10  92/02/02  09:22:58  oz
 * 	 - added initTC()
 * 	[92/01/31  18:13:39  oz]
 * 
 * 	 - changed the default set of commands included
 * 	[92/01/31  15:38:54  oz]
 * 
 * Revision 1.9  91/10/23  16:47:05  oz
 * 	- Added a temporary command to test itl_GetSegment
 * 	  Left it there for mechanism for testing new temporary commands.
 * 
 * Revision 1.8  91/10/18  11:09:43  oz
 * 	Set it so that it always initializes TRAN and adds
 * 	the tran commands.
 * 
 * Revision 1.7  91/10/17  15:52:57  mss
 * 	(a) reenabled other modules
 * 	(b) added tran-c
 * 
 * Revision 1.6  91/10/03  12:49:45  oz
 * 	Print the RCSid in main in verbose mode.
 * 	itl_LocalCommandLoop now returns a string.
 * 
 * Revision 1.5  91/09/30  16:19:11  oz
 * 	Added a command line flag: -p for server port prefix.
 * 	When not specified the server thread is not started.
 * 
 * Revision 1.4  91/09/27  14:13:10  oz
 * 	Set it to start the Server thread by default - prefix == ItlTest_
 * 
 * Revision 1.3  91/09/21  19:47:48  oz
 * 	Removed inclusion of any libraries such as threadTid or tran
 * 	itl now tests only the raw interface: commands, IO, exp, etc.
 * 
 * Revision 1.2  91/09/16  11:58:40  mss
 * 	Renamed
 * 	 printMsg => itl_PrintMsg
 * 	 errorMsg => itl_PrintErrMsg
 * 	 currentErr => itl_CurrentErr
 * 	Commented out server intialization
 * 
 * Revision 1.1  91/08/22  17:21:54  mss
 * 	Modified REC tester to make separate library
 * 
 */

/*
 * Test Program:  itl_main.c
 *
 * Description:
 *     The code for a sample main program
 *
 */


static char RCSid[] = "$Id: itl_main.c,v 1.1.414.1 1996/10/17 18:27:31 damon Exp $";

#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>


PRIVATE char *portPrefix = "ITLtester";


/*
 * Macros for printing information about test invocation.
 */
#define PrintShortUsage \
  itl_PrintMsg((Cur_Out, "Usage: %s [-bdvDE] [-I <input file>]\n", \
	    itlName))

#define PrintLongUsage					        \
    MACRO_BEGIN							\
        PrintShortUsage;					\
        itl_PrintMsg((Cur_Out, "    -v = verbose\n"));		     \
        itl_PrintMsg((Cur_Out, "    -b = batch mode (no prompts)\n"));   \
        itl_PrintMsg((Cur_Out, "    -d = debug mode (More Output)\n"));  \
        itl_PrintMsg((Cur_Out, "    -D = display lines mode\n"));        \
        itl_PrintMsg((Cur_Out, "    -E = interface mode\n"));            \
        itl_PrintMsg((Cur_Out, "    -I <input file>\n"));                \
    MACRO_END



/*
 * Function: ProcessCmdLine(argc,argv) 
 *
 *
 * Description:
 *     All program options must be unique by first letter.
 */

char *ProcessCmdLine(argc,argv)
    int argc;
    char **argv;
{
#define MIN_NUM_ARGS 5

    char *inputFileName = NULL;

    while (--argc > 0) {
	if ((*++argv)[0]  == '-') {
	    switch ((*argv)[1]) {

              case 'b':  /* set batch mode (non-interactive input) */
		itl_SetInteractive(FALSE);
		break;

  	      case 'd': /* set debug mode */
		setDebug(DebugLevel | ITL_DEBUG_TRACE);
		break;

  	      case 'v': /* set verbose mode */
		setVerbose(TRUE);
		break;

	      case 'D':   /*  Display command lines as they are read  */
		setDisplayLines(TRUE);
		break;
        
	      case 'E':  /* Interface mode */
		itlIO_SetInterfaceMode(TRUE);
		break;

	      case 'I':  /* set the input file  */
		inputFileName = *++argv;
                --argc;
		break;
		
	      default: /* illegal switch */
		itl_PrintErrMsg((itl_CurrentErr(), "illegal switch on command line"));
		PrintShortUsage;
	    }
	} 
	else {  /* unexpected command line arg */
	    itl_PrintMsg((Cur_Out, "unexpected arg: %s\n",argv));
	    PrintShortUsage;
	    exit(1);
	}
    }

    return(inputFileName);
}



PRIVATE boolean_t HandleTempCommand(param)
  IN int param;
{
    char *seg, *doneStr;
    int len;

    itl_Prompt((Cur_Out, "Terminating string: "));
    doneStr = itl_GetString(NULL, NULL);
    
    itl_Prompt((Cur_Out, "Type the segment, end with %s: ",
		doneStr));
    seg = itl_GetSegment(doneStr, "Seg=> ", FALSE, &len);
    itl_Message("The segment is of length %d: %s\n", len, seg);
    ITL_FREE(doneStr);
    ITL_FREE(seg);
}

#define ITL_TEMP_COMMAND                                   \
    {"temporary", HandleTempCommand, 0, FALSE,             \
	          COMMAND_MODE_ALL,                        \
		  "set a script variable",                 \
                  "var",                                   \
                  ""}


PRIVATE itl_commandTable_t mainCommands[] = {
    ITL_TEMP_COMMAND
};

/*
 * Main - 
 */
void main(argc, argv)
    int argc;
    char **argv;
{
    char *fileName;

    itlName = argv[0];

    (void) itl_InitItl(FALSE, TRUE, portPrefix);


    itl_fTrace(Cur_Out,
	       ITL_VERBOSE_TRACE,
	       "Starting %s: %s\n",
	       itlName,
	       RCSid);

    fileName = ProcessCmdLine(argc, argv);

/*
 *  If you were writing an ITL test program and wanted to initialize
 *  a components command set you would issue something like the following.
 *
 *    itl<component>_Add<component>Commands();
 *
 *  So if you were using the Encina Volume Service you would issue,
 *
 *    itlVol_AddVolCommands()
 *
 *  and to obtain the command set for the basic file system functions in 
 *  the OS, you would have.
 *
 *    itlFile_AddFileCommands();
 */

    itl_AddCmdTable(mainCommands, 
		    sizeof(mainCommands)/sizeof(itl_commandTable_t));

    if (fileName) {
	if (itl_RunFromFile(fileName, fileName)) {
	    itl_fMessage(Cur_Err,
			 "Test Program terminating with an error\n");
	    exit(1);
	}
    } else {
	/* 
	 *  Call localCommandLoop rather than CommandLoop:
	 *  This allocates a history buffer and frees it before
	 *  returning, which makes it easier to use a plumber to
	 *  identify memory leaks.
	 */
	if (!itl_LocalCommandLoop("Do=> ")) {
	    itl_fMessage(Cur_Err,
			 "Test Program terminating with an error\n");
	    exit(1);
	}
    }

    /*  Free all the memory allocated for variables
     *  and clean up the rt_exp module:
     */
    itl_CleanupItl();

    exit(0);
}

