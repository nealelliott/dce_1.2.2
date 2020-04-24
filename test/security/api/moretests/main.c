/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: main.c,v $
 * Revision 1.2.7.2  1996/02/17  23:25:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:42  marty]
 *
 * Revision 1.2.7.1  1995/12/11  21:48:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:29  root]
 * 
 * Revision 1.2.5.1  1993/10/13  20:35:20  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/13  20:34:52  mccann]
 * 
 * Revision 1.2.3.4  1992/12/31  17:29:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:55:22  htf]
 * 
 * Revision 1.2.3.3  1992/11/23  14:50:49  cuti
 * 	Modify dce_error_inq_text auguments. Also merge from rpc's main.c
 * 	[1992/11/23  14:49:41  cuti]
 * 
 * Revision 1.1.5.2  1992/09/04  16:07:53  bolinger
 * 	Fix OT defect 5339:  track EXCEPTION union member name changes
 * 	introduced by new threads source base.  (These changes do not
 * 	alter code generated to access members involved.)
 * 	[1992/09/04  14:58:59  bolinger]
 * 
 * Revision 1.1.3.5  1992/07/16  21:25:32  dceguest
 * 	Changed TSHMessage() to TSHMessage(); to prevent compile problem.
 * 	[1992/07/16  21:24:50  dceguest]
 * 
 * Revision 1.1.3.4  1992/07/16  01:18:08  smith
 * 	fixed compile problem with ifdef.
 * 	[1992/07/16  00:50:28  smith]
 * 
 * Revision 1.1.3.3  1992/07/15  02:11:55  smith
 * 	changed getopt for proper type casting.
 * 
 * 	changed TSHlog to correct the number of args.
 * 
 * 	changed TSHLog to %ld vs. %lx to read in decimal instead of hex.
 * 
 * 	changed "string" arg in TSHLog() and dce_error_inq_text()
 * 	        to a character array to prevent failure.
 * 	[1992/07/15  01:42:51  smith]
 * 
 * Revision 1.1  1992/01/19  03:35:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  (c) Copyright 1990, 1991 OPEN SOFTWARE FOUNDATION, INC.
**  ALL RIGHTS RESERVED
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass. &
**      Locus Computing Corporation, Burlington, Mass.
**
**
**  $Log: main.c,v $
 * Revision 1.2.7.2  1996/02/17  23:25:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:42  marty]
 *
 * Revision 1.2.8.2  1996/02/17  22:43:42  marty
 * 	Update OSF copyright year
 *
 * Revision 1.2.7.1  1995/12/11  21:48:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:29  root]
 *
 * Revision 1.2.6.2  1995/12/11  20:52:29  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.2.5.1  1993/10/13  20:35:20  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/13  20:34:52  mccann]
 *
 * Revision 1.2.4.2  1993/10/13  20:34:52  mccann
 * 	CR8651 64bit porting changes
 *
 * Revision 1.2.3.4  1992/12/31  17:29:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:55:22  htf]
 *
 * Revision 1.2.4.2  1992/12/31  16:55:22  htf
 * 	Embedding copyright notice
 *
 * Revision 1.2.3.3  1992/11/23  14:50:49  cuti
 * 	Modify dce_error_inq_text auguments. Also merge from rpc's main.c
 * 	[1992/11/23  14:49:41  cuti]
 *
 * Revision 1.2.4.2  1992/11/23  14:49:41  cuti
 * 	Modify dce_error_inq_text auguments. Also merge from rpc's main.c
 *
 * Revision 1.1.5.2  1992/09/04  16:07:53  bolinger
 * 	Fix OT defect 5339:  track EXCEPTION union member name changes
 * 	introduced by new threads source base.  (These changes do not
 * 	alter code generated to access members involved.)
 * 	[1992/09/04  14:58:59  bolinger]
 *
 * Revision 1.1.4.2  1992/09/04  14:58:59  bolinger
 * 	Fix OT defect 5339:  track EXCEPTION union member name changes
 * 	introduced by new threads source base.  (These changes do not
 * 	alter code generated to access members involved.)
 *
 * Revision 1.1.3.5  1992/07/16  21:25:32  dceguest
 * 	Changed TSHMessage() to TSHMessage(); to prevent compile problem.
 * 	[1992/07/16  21:24:50  dceguest]
 *
 * Revision 1.1.5.2  1992/07/16  21:24:50  dceguest
 * 	Changed TSHMessage() to TSHMessage(); to prevent compile problem.
 *
 * Revision 1.1.3.4  1992/07/16  01:18:08  smith
 * 	fixed compile problem with ifdef.
 * 	[1992/07/16  00:50:28  smith]
 *
 * Revision 1.1  1992/01/19  03:35:19  devrcs
 * Initial revision
 * Revision 1.1  91/08/22  11:05:53  finger
 * Initial check-in
 * 
**
**
**  @(#)main.c	1.29
**  TSH Library 
**	Extracted	1/11/91 18:18:10
**	from delta on	12/14/90 16:46:43
*/
#ifndef LINT
static char main_sccs_id[] = "@(#)main.c	1.29";
#endif

/*
 *  Module:
 *
 *	main()/main.c
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <dce/cmalib_crtlx.h>
#include <tsh_main.h>
#include <tsh_common.h>
#include <dce/exc_handling.h>
#include <dce/cma_errno.h> 
#include <dce/dce_error.h>

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

/*  Here are the functions that reside in this module.  
 *  They offer certain services
 *  to generic test programs as part of the TSH.
 */
ndr_long_int  TSHOptions();
#ifdef __STDC__
	void TSHLog(CtxBlock *ctx, ...);
#else
	void TSHLog();
#endif

#ifdef __STDC__
	void TSHMessage(CtxBlock *ctx, int flag, ...);
#else
	void TSHMessage();
#endif

ndr_long_int  TSHStatus();


/*  These are internal function used to print various things.
 */
void          PrintInfo();
unsigned32    PrintStatus();
char          *ErrorText();


/*  This is the interface to the generic test programs that 
 *  implement the TSH test
 *  files.  This routine should make calls to the functions in this module.
 */
ndr_long_int  TestFunction();

#ifdef BS
ndr_char *dce_error_inq_text();
#endif


#ifdef DEBUG
/*  And a self test function...
 */
void TSHDebug();
#endif


main(argc,argv)
ndr_long_int  argc;
char          **argv;
{
CtxBlock      Ctx, *ctx;
ndr_long_int  ArgsUsed;
ndr_long_int  TestStatus = 0l;
error_status_t  st;

#ifdef DEBUG
PRINT_ARGS(argc, argv);
#endif


/*  Point the ctx pointer to the actual block.
 */
ctx = &Ctx;


/*  Get the command line sitches and build the 
 *  context block with information about
 *  how to run this instance of the test.
 */
ArgsUsed = TSHOptions(ctx, argc, argv);
  
/*    
if (ctx->OptionsMask & MODEBUG) 
	rpc__dbg_set_switches("0-4.3", &st);
*/

#ifdef DEBUG
/*  TSHDebug has a bunch of debug printfs.  they test 
 *  the functions in this module.
 */
TSHDebug(ctx, argc, argv, ArgsUsed);
#endif


/*  Set up the exception handling for this test run.
 */
TRY


/*  Make a call to the test function and return its status to the TSH.
 */
TestStatus = TestFunction(ctx, argc - ArgsUsed, &argv[ArgsUsed]);


/*  Process any exceptions raised during the call to TestFunction()
 */
CATCH_ALL
if (THIS_CATCH->kind == exc_kind_status_c)
    {
    TSHMessage (ctx, MOFAILURE, "caught a status exception: %x\n",
        THIS_CATCH->status.status);
    if(!(TestStatus == errno))
        TestStatus = TSH_EXCEPTION_CAUGHT;
    }
else
    {
    TSHMessage (ctx, MOFAILURE, "caught an address exception: %x\n",
        THIS_CATCH->address.address);
    TestStatus = TSH_EXCEPTION_CAUGHT;
    }

/*  complete the exception handle block.
 */
ENDTRY


/*  Print the status of the test, success or failure, along with the return
 *  from the test.
 */
return( PrintStatus(ctx, TestStatus) );
}



/*
 *  Module:
 *
 *	TSHOptions()/main.c
 * 
 *  Description:
 *
 *	The function TSHOptions() strips off the common TSH options.
 *	They are '-d mask' for specifying what the test output should focus
 *	on, '-s' for sending data to the screen (via stderr) as well as
 *	stdout which in all likelyhood has been redirected to a log file, and
 *	'-S' which adds verbosity to the stderr output.
 *
 *  Input:
 *
 *	ctx  - Context block for global type variables set by these options,
 *	argc - Argument count from command line,
 *	argv - Arguments from command line.
 *
 *  Output:
 *
 *	optind - # of arguments used from the command line.  This is 
 *	  important since the actual arguments used by the routines that 
 *	  call this function typically start from the middle of the args
 *	  list.  This value is used for the starting argument for the
 *	  args list once all of the common options are picked off.
 *
 *  Example:
 *
 *	If the command line coming into a typical TSH test program is:
 *
 *	rpc_object_set_type -d 100 -s "a string" rpc_s_ok O_UUID T_UUID
 *
 *	Argc should have 8 with valuue for argv[0] = object_set_type, 
 *	argv[1] = -d, ... and argv[7] = T_UUID.  The arguments that are 
 * 	used by the test itself start at argv[5] (rpc_s_ok).  This 
 *	function uses the common options to set some flags, and then
 *	return the position of the first argument it knew nothing of.
 *	In this example, optind would return 5 so that argv[5] would be
 *	the first non-common option.
 *
 */ 
ndr_long_int TSHOptions(ctx, argc, argv)
CtxBlock     *ctx;              /* common args fill in a context block. */
ndr_long_int argc;              /* argc for command line from main() */
char_p_t     **argv;            /* argv for command line from main() */
{
int          c;			/* Char for picking up options from getopt */
extern char  *optarg;		/* option argument if it exists */
extern int   optind;		/* argv index of next operand */


#ifdef DEBUG
printf("-------------------------------------------\nTSHOptions:\n\nInput args are\n");
PRINT_ARGS(argc, argv);
#endif


/*  Set the entries in the context block to their default states.
 */
ctx->ProgramName    = argv[0];
ctx->OptionsMask    = DEFAULTMASK;
ctx->stderrSet      = FALSE;
ctx->PerformanceSet = FALSE;
ctx->ErrorSet       = FALSE;


/*  Pick off the arguments we look for.  
 */
while((c = (int) getopt(argc, (char **)argv, "d:sp")) != EOF)
    {
    switch(c)
        {
        case 'd': ctx->OptionsMask = atoi(optarg);
		  break;

	case 's': ctx->stderrSet  = TRUE;
		  break;

	case 'p': ctx->PerformanceSet = TRUE;
		  break;

	default:  break;
        }
    }


/*  Now set the two ctx block fields that hold the Info block stuff.  Also 
 *  bump the optind variable.
 */
ctx->InfoUsed       = FALSE;
ctx->InfoString     = argv[optind++];


return(optind);
}



/*
 *  Module:
 *
 *	TSHLog()/main.c
 *
 *  Description:
 *
 *  	Prints the message past to it by invoking vfprintf() with the 
 *	arguments.  For this reason, the arguments should be in printf
 *	form to begin with.  The context block is also passed in to
 *	describe how the arguments will be displayed.  
 *
 *	This function prints out the given info in one of two ways: to stdout
 *	(which may be redirected into a log file) or to stderr (which always
 *  	points to the screen) as well as stdout.
 *
 *  Input:
 *
 *	ctx - context block fo this test program
 *	va_list - variable argument list used to hold data in printf format.
 *
 *  Output:
 *
 *	none
 * 
 */

#ifdef __STDC__

void TSHLog(CtxBlock *ctx, ...)

#else

void TSHLog(ctx, va_alist)
CtxBlock *ctx;
va_dcl

#endif
{
va_list	args;
char *	fmt;


/*  Start the variable argument list processing.  The first arg should be the
 *  format, since these arguments follow the printf format.
 */
#ifdef __STDC__
va_start(args, ctx);
#else
va_start(args);
#endif

fmt = va_arg(args,char *);


/*  If the stderrSet paramater is TRUE, then we will send data out to stderr,
 *  but in all cases, print to stdout.
 */
if(ctx->stderrSet)
    vfprintf(stderr,fmt,args);

vfprintf(stdout,fmt,args);


/*  End of varaible argument list processing.
 */
va_end(args);
}



/*
 *  Module:
 *
 *	TSHMessage()/main.c
 *
 *  Description:
 *
 *  	Very similar to TSHLog, but its only prints out stuff if the  
 *	context block has the message type activated.  If this is an info
 *	message and the info bit is set in the context block options field
 *	then the info message will be printed.
 *
 *	This function uses TSHLog to actually print the message out.
 *
 *	NOTE:  since the string length is not readily available, a fixed 
 *	size buffer is used for the message string.  This adds a limitation 
 *	to the string length of the user message.
 *
 *  Input:
 *
 *	ctx - context block fo this test program
 *	flag - message request level, e.g. INFO, WARNING, SUCCESS, etc.
 *	va_list - variable argument list used to hold data in printf format.
 *
 *  Output:
 *
 *	none
 * 
 */
#ifdef __STDC__

void TSHMessage(CtxBlock *ctx, int flag, ...)

#else

void TSHMessage(ctx, flag, va_alist)
CtxBlock *ctx;
int flag;
va_dcl

#endif
{
va_list	args;
char *	fmt;
char message[256];
char new_fmt[256];


/*  Start the variable argument list processing.  The first arg should be the
 *  format, since these arguments follow the printf format.
 */
#ifdef __STDC__
va_start(args, flag);
#else
va_start(args);
#endif

fmt = va_arg(args,char *);


/*  These are a series of ifs, instead of a case because, the 
 *  flag may be an orred
 *  combination of two or more of these attributes.  The convention 
 *  we opted for is
 *  to print a message for each of the orred flags.
 */
if (ctx->OptionsMask & flag)
    {
    if(!ctx->InfoUsed)
        PrintInfo(ctx);

    /*  If the flag is a MOFAILURE, print it as a failure message.
     */
    if (flag & MOFAILURE)
        {
        sprintf(new_fmt, "Failure Message:  %s", fmt);
        vsprintf(message, new_fmt, args);

        TSHLog(ctx, message);
        }

    /*  If the flag is a MOSUCCESS, print it as a success message.
     */
    if (flag & MOSUCCESS)
        {
        sprintf(new_fmt, "Success Message:  %s", fmt);
        vsprintf(message, new_fmt, args);

        TSHLog(ctx, message);
        }

    /*  If the flag is a MOWARNING, print it as a warning message.
     */
    if (flag & MOWARNING)
        {
        sprintf(new_fmt, "Warning Message:  %s", fmt);
        vsprintf(message, new_fmt, args);

        TSHLog(ctx, message);
        }
    
    /*  If the flag is a MOTRACE, print it as a trace message.
     */
    if (flag & MOTRACE)
        {
        sprintf(new_fmt, "Trace Message:    %s", fmt);
        vsprintf(message, new_fmt, args);

        TSHLog(ctx, message);
        }

    /*  If the flag is a MOINFO, print it as a info message.
     */
    if (flag & MOINFO)
        {
        sprintf(new_fmt, "Info Message:     %s", fmt);
        vsprintf(message, new_fmt, args);

        TSHLog(ctx, message);
        }
    }


/*  End of varaible argument list processing.
 */
va_end(args);
}



/*
 *  Module:
 *
 *	TSHStatus()/main.c
 *
 */
ndr_long_int TSHStatus(ctx, level, ErrorString, AStatus, EStatus)
CtxBlock *ctx;
int level;
char *ErrorString;
error_status_t AStatus;
error_status_t EStatus;
{
int NoMatch = TRUE;
char *NewMessage;


#ifdef DEBUG 
printf("-------------------------------------------\nTSHStatus:\n\nInput args are\n");
printf("    Program Name = %s, mask = %x, level = %x, AStatus (%x) should = EStatus (%x)\n    .%s.\n",
    ctx->ProgramName, ctx->OptionsMask, level, AStatus, EStatus, ErrorString);
#endif 


if (AStatus == EStatus) 
    {
    NoMatch = FALSE;
    }
else if (AStatus != EStatus) 
    {
    /*  Append a line feed to the incoming error string and print it out
     *  with TSHMessage().  These incoming strings are not formatted.
     */
    NewMessage = (char *)calloc(1, strlen(ErrorString)+2);
    sprintf(NewMessage, "%s\n", ErrorString);

    TSHMessage(ctx, MOFAILURE, NewMessage);
    cfree(NewMessage);


    /*  Give an indication if the test itself failed, or one of the 
     *  the test support functions failed.
     */
    if(level == TEST)  TSHMessage(ctx, MOFAILURE, "failure set by the function under test\n");
    if(level == SETUP) TSHMessage(ctx, MOFAILURE, "failure set by a test setup function\n");
    if(level == CLNUP) TSHMessage(ctx, MOFAILURE, "failure set by a test cleanup function\n");


    /*  Set to TRUE the indicaters that signal error
     */
    ctx->ErrorSet = TRUE;
    NoMatch = TRUE;
    }


return(NoMatch);
}



/*  PrintInfo - print the test header block, and note the fact that it did so
 *  that this info gets printed only once.
 */
void PrintInfo(ctx) 
CtxBlock *ctx;
{ 
TSHLog(ctx, "\n"); 
TSHLog(ctx, "---------------------------------------------------------------\n"); 
TSHLog(ctx, "Routine Tested:   %s\n", ctx->ProgramName); 
TSHLog(ctx, "Tested For:       %s\n", ctx->InfoString); 
TSHLog(ctx, "\n"); 

ctx->InfoUsed = TRUE; 
}



/*  PrintStatus - if status is rpc_s_ok, print the success completion
 *  message, otherwise print the failure completion message.
 */
unsigned32 PrintStatus(ctx, test_status)   
CtxBlock *ctx;
unsigned32 test_status;
{ 
error_status_t status;
dce_error_string_t	errorbuf;

if ((test_status == rpc_s_ok)  && (ctx->ErrorSet == FALSE))
    { 
    if(ctx->OptionsMask & MOSUCCESS) 
        { 
        if(ctx->InfoUsed == FALSE) 
            PrintInfo(ctx) ;

        TSHLog(ctx, "Test Status:      Test SUCCEEDED\n"); 


/*  If STATUS is also set, print out the returned status value as well
 */
        if(ctx->OptionsMask & MOSTATUS)
            {
            TSHLog(ctx, "Status Returned:  %lx\n", test_status); 
            TSHLog(ctx, "Status Text:      %s", ErrorText(test_status, &status));
            if(status == FAILURE)
                { 
                dce_error_inq_text(test_status, errorbuf, (int *)&status);
                TSHLog(ctx, "%s", errorbuf);
                switch(status)
                    {
                    case rpc_s_unknown_status_code:
                        TSHLog(ctx, " or user defined error state");
                        break;
                    case rpc_s_ok: 
                        break;
                    default:
                        TSHStatus(ctx,CLNUP,"dce_error_inq_text() failed", status, 
                            rpc_s_ok);
                    }
                }


            TSHLog(ctx, "\n\n"); 
            }
        }

    return(TSH_SUCCESS);
    } 
else 
    { 
    if(ctx->OptionsMask & MOFAILURE) 
        { 
        if(ctx->InfoUsed == FALSE) 
            PrintInfo(ctx); 

        TSHLog(ctx, "Test Status:      Test FAILED\n"); 


/*  If STATUS is also set, print out the returned status value as well
 */
        if(ctx->OptionsMask & MOSTATUS)
            {
            TSHLog(ctx, "Status Returned:  %ld\n", test_status); 
            TSHLog(ctx, "Status Text:      %s", ErrorText(test_status, &status));
            if(status == FAILURE)
                { 
                dce_error_inq_text(test_status, errorbuf, (int *)&status);
                TSHLog(ctx, "%s", errorbuf);
                switch(status)
                    {
                    case rpc_s_unknown_status_code:
                        TSHLog(ctx, " or user defined error state");
                        break;
                    case rpc_s_ok: 
                        break;
                    default:
                        TSHStatus(ctx,CLNUP,"dce_error_inq_text() failed", status, 
                            rpc_s_ok);
                    }
                }


            TSHLog(ctx, "\n\n"); 
            }
        }
    return(TSH_TEST_FAILED);
    } 
} 



char *ErrorText(OurErrorCode, status)
unsigned32 OurErrorCode;
error_status_t *status;
{
extern int sys_nerr;
extern char  *sys_errlist[];

*(status) = rpc_s_ok;

switch(OurErrorCode)
    {
    case rpc_s_ok:                 return((char *)"successful completion");
    case TSH_TEST_FAILED:          return((char *)"test failed");
    case TSH_DATA_MISSMATCH:       return((char *)"data missmatch error");
    case TSH_UNKNOWN_SUBCASE:      return((char *)"unknown subcase");
    case TSH_UNKNOWN_ARGUMENT:     return((char *)"unknown argument");
    case TSH_INVALID_ARGUMENT:     return((char *)"invalid argument");
    case TSH_BAD_ARG_COUNT:        return((char *)"bad argument count");
    case TSH_EXCEPTION_CAUGHT:     return((char *)"unexpected or wrong exception caught");
    case TSH_INCORRECT_DATA:       return((char *)"incorrect data returned");
    case TSH_EXCEPTION_NOT_RAISED: return((char *)"exception not raised");
    case TSH_UNIMPLEMENTED:        return((char *)"test currently unimplemented");
    default:
	if(OurErrorCode < sys_nerr) 
	    return(sys_errlist[OurErrorCode]);
        *(status) = FAILURE;
        return ((char *)NULL);
    }
}


#ifdef DEBUG
/*  TSHDebug has a bunch of debug printfs.  They test the 
 *  functions in this module.
 */
void TSHDebug(ctx, argc, argv, ArgsUsed)

CtxBlock *ctx;
int argc;
char *argv[];
int ArgsUsed;
{
printf("ArgsUsed = %d <= argc = %d\n", ArgsUsed, argc);
printf("Here are some ctx structure members:\n");
printf("    ProgramName = %s,\n    stderrSet = %d,\n    OptionsMask = %d: ",
    ctx->ProgramName, ctx->stderrSet, ctx->OptionsMask);
printf("F = %d, S = %d, W = %d, T = %d, I = %d\n", (ctx->OptionsMask & MOFAILURE), 
    (ctx->OptionsMask & MOSUCCESS), (ctx->OptionsMask & MOWARNING), 
    (ctx->OptionsMask & MOTRACE),   (ctx->OptionsMask & MOINFO));

TSHLog(ctx, "this is stderrSet: %d\n", ctx->stderrSet);
TSHMessage(ctx, MOFAILURE, "This is a failure message.\n");
TSHMessage(ctx, MOINFO, "this is an INFO text - stderrSet: %d\n", ctx->stderrSet);
TSHMessage(ctx, MOINFO | MOWARNING, 
"* this is an INFO and a WARNING text - stderrSet: %d\n*\n* This is one very large (>80 chars) printf line:\n*        there should be no problemn", ctx->stderrSet);
TSHStatus(ctx, TEST, "this line denotes error", TRUE, FALSE);
TSHMessage(ctx, MOINFO, "So far so good....\n");
}
#endif


#ifdef BS1
void rpc_call_transmit_fault(call_handle, fault_info, status)
rpc_call_handle_t call_handle;
rpc_iovector_p_t fault_info;
unsigned32 *status;
{
rpc_call_fault(call_handle, (rpc_iovector_elt_t *)fault_info, status);
}


ndr_char *dce_error_inq_text(value, status)
unsigned32 value;
error_status_t *status;
{
unsigned_char_p_t string;
rpc_error_inq_text(value, &string, status);
return(string);
}
#endif

#ifdef  BS2
void rpc_mgmt_inq_com_timeout(binding, timeout, status)
handle_t binding;
signed32 *timeout;
error_status_t *status;
{
    *status = TSH_UNIMPLEMENTED;
}
void rpc_mgmt_set_com_timeout(binding, timeout, status)
handle_t binding;
signed32 timeout;
error_status_t *status;
{
    *status = TSH_UNIMPLEMENTED;
}
#endif
