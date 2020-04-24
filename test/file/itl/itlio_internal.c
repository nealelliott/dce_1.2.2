/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itlio_internal.c,v 1.1.417.1 1996/10/17 18:28:03 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    SHARED   itlIO_IncBraceLevel, itlIO_TraceCurrentLocation, 
 *             itlIO_AdvanceLine, itlIO_GotoHistLoc, 
 *             itlIO_GetUninterpretedLine, itlIO_SetIncludePath, 
 *             itlIO_InternalInit
 *    
 *    EXPORT   itlIO_DumpIOState, itlIO_IsInterface, itlIO_IsInteractive, 
 *             itl_SetInteractive, itlIO_DefaultScope, itlIO_PrintPrompt, 
 *             itlIO_DonePrompting, itlIO_SkipWhites, 
 *             itlIO_MoveToEndOfBuffer, itlIO_GetLine, itlIO_PrintHist, 
 *             itl_RunFromString, itl_RunFromFile, itl_RunFromInput, 
 *             itlIO_GotoBraceLevel, itl_RestoreThreadState, 
 *             itl_FreeThreadState
 *    
 *    PRIVATE  InitializeITLThread, StartITLThread, CleanupITLThread, 
 *             OpenFileOnDir, OpenCurrentFile, GetNewLine
 *    
 *    extern   itlIO_AdvanceToNonWhite
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itlio_internal.c,v $
 * Revision 1.1.417.1  1996/10/17  18:28:03  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:05  damon]
 *
 * Revision 1.1.412.1  1994/02/04  20:46:00  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:50  devsrc]
 * 
 * Revision 1.1.410.1  1993/12/07  17:45:04  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:32:10  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/11  18:31:52  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:12:02  htf]
 * 
 * Revision 1.1.2.3  1992/10/28  17:58:58  jaffe
 * 	Transarc delta: fred-itl-dir-stat-mem-cmds 1.2
 * 	  Selected comments:
 * 	    Adding support for directory functions, stat function, and memory mapped
 * 	    files.
 * 	    Not done yet, just upgrading.
 * 	    Imported fix from Ofer.
 * 	    This complets this delta.
 * 	Transarc delta: fred-itl-ofer-fixes-9-22-92 1.2
 * 	  Selected comments:
 * 	    Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	    related.  Others concerned the printing of error messages.  In most cases
 * 	    messages are no longer printed by support functions.  The determination of
 * 	    whether a message should be printed is now left to functions that call these
 * 	    support functions.
 * 	    Removed excessive calls to strlen().
 * 	    Removed a number of functions and collapsed them into a single recursive
 * 	    function, GetTerm().
 * 	    Fix (picky) compiler warnings
 * 	[1992/10/28  12:51:52  jaffe]
 * 
 * Revision 1.5  1992/10/02  20:32:18  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.2 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.4  1992/09/23  15:53:50  fred
 * 	Incorporate fixes made to the TP version of ITL.  Some of them are performance
 * 	related.  Others concerned the printing of error messages.  In most cases
 * 	messages are no longer printed by support functions.  The determination of
 * 	whether a message should be printed is now left to functions that call these
 * 	support functions.
 * 
 * 	Removed excessive calls to strlen().
 * 	Removed a number of functions and collapsed them into a single recursive
 * 	function, GetTerm().
 * 	[from revision 1.3 by delta fred-itl-ofer-fixes-9-22-92, revision 1.1]
 * 
 * Revision 1.3  1992/08/28  12:35:10  fred
 * 	Adding support for directory functions, stat function, and memory mapped
 * 	files.
 * 
 * 	Not done yet, just upgrading.
 * 
 * 	Imported fix from Ofer.
 * 	[from revision 1.2 by delta fred-itl-dir-stat-mem-cmds, revision 1.1]
 * 
 * Revision 1.2  1992/08/20  19:37:50  fred
 * 	Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	succeeds that was expected to fail.
 * 	Added the verifyFileError command to toggle file system function verification.
 * 	(The code was already there I just was not making it available.)
 * 
 * 	Ofer bug fixes.
 * 	[from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:38  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * 	@TRANSARC_COPYRIGHT@
 * 
 * 	$TALog: itlio_internal.c,v $
 * 	Revision 1.15  1994/11/01  22:30:25  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.13 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.13  1993/06/03  18:32:49  shl
 * 	[merge of changes from 1.10 to 1.12 into 1.11]
 * 
 * Revision 1.12  1993/06/03  12:50:16  fred
 * 	To port all ITL based test programs to Solaris.  This includes the following
 * 	test programs,
 * 
 * 	fset_test, fx_test, bakserver_test file_test
 * 
 * 	as well as all the code under the src/test/file/itl directory.
 * 
 * 	The changes, to the .c files, if not explicitly listed, would normally be
 * 	one or more of the following.
 * 
 * 	include stdlib.h to define the free() function
 * 	include sys/types.h
 * 	work around the fact that the sys/mman.h file on Solaris defines
 * 	  the constants PRIVATE and SHARED.
 * 	remove extra ';', usually at the end of macro declarations
 * 
 * 	I believe all bsd'isms are now removed from ITL.
 * 
 * 	SUNOS5 porting changes.
 * 	[from r1.10 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.11  1993/05/21  19:20:37  fred
 * 	To import ITL fixes from the Encina group that either fix problems we
 * 	have, or would impact the changes I made to the shellExec command.
 * 
 * 	Note:
 * 	    By accident I made changes in this delta that I should have made in
 * 	    fred-db3575-itl-spoke-thread-safeness-fixes.  So this delta and that
 * 	    one rely on one another.
 * 
 * 	Encina ITL fixes.
 * 	[from r1.10 by delta fred-db3579-encina-itl-fixes-for-shell, r1.1]
 * 
 * Revision 1.10  1993/03/08  13:52:07  fred
 * 	To import fixes to ITL from Ofer.  Also, fixed some outstanding minor
 * 	bugs we found -
 * 	 - The spoke (distributed ITL) had a bug in it's sync point code.  It was
 * 	   not removing a variable from the hash table, which caused confusion when
 * 	   a sync point name was re-used in a tight loop.  Also, it was not
 * 	   setting the timeout field correctly during sync point creation.
 * 
 * 	 - Remove the dependency of the ITL utilities library on the com_err
 * 	   library.  I wrote our own error handling function that deals with both
 * 	   dce and errno correctly.  Besides, the error_message() function in the
 * 	   com_err library is not thread-safe.
 * 
 * 	Incorporation of Encina ITL fixes.
 * 	[from r1.9 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.9  1993/03/01  14:48:27  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.8 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 6.2  1993/01/28  22:16:35  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Cast key destructor argument properly.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * Revision 1.8  1992/05/21  13:06:20  oz
 * 	When the name of an include file was a full path (either relative or
 * 	absolute) and the file did not exist ITL did not report an error.
 * 	This required a 1-line change: add a print message (itl_Error)
 * 	in the branch that detects the file does not exist.
 * 
 * 	Added an itl_Error call in OpenCurrentFile when
 * 	OpenFileName returns NULL
 * 	[from revision 1.7 by delta oz-2802-ITL-Report-an-error-when-file-not-found, revision 1.1]
 * 
 * Revision 1.7  1992/04/21  14:51:19  oz
 * 	Include types.h before stat.h in itlio_internal.c
 * 
 * 	[from revision 1.5 by delta oz-2441-include-types.h-before-stat.h, revision 1.1]
 * 
 * Revision 1.5  1992/04/06  21:50:33  oz
 * 	Additional minor fixes.
 * 
 * 	gotoBraceLevel does nothing if IO-Error is TRUE
 * 	[from revision 1.4 by delta oz-2278:Errors-in-optional-clause, revision 1.2]
 * 
 * Revision 1.4  1992/04/02  14:49:15  oz
 * 	  Continued developlment on the access to recursive access.
 * 	  fetch&Op works properly now.
 * 	  test&Set still doesn't
 * 
 * 	  GetLine deals with field specifications.
 * 	[from revision 1.3 by delta oz-2228:Add-access-to-recursive-structures, revision 1.2]
 * 
 * Revision 1.3  1992/03/27  21:16:03  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	Was previously part of itl_io.c
 * 	[added by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.2  92/03/04  15:14:47  oz
 * 	- eofP optional for itl_GotoBraceLevel
 * 	[92/02/24  12:53:47  oz]
 * 
 * 	- Moved itlIO_ParsingError and SetErrorStatus back to itl_io.c
 * 	[92/02/24  08:32:37  oz]
 * 
 * 	Initial Checkin
 * 	[92/02/23  16:01:23  oz]
 * 
 * 	$EndTALog$
 * 
 * $EndLog$
 */

/*
 * itlio_internal.c -- 
 *    The internal portion of the I/O and parsing routines
 *    for the Interactive Test Library (ITL).
 */
/*
 *  When running with DCE, we use the exception handler to make sure
 *  our state is always consistent - ITL can be used in conjunction with
 *  TRAN-C which uses the DCE exception handler.
 *  We ALWAYS use the following syntax:
 *	TRY {
 *	  .
 *	  .
 *     } FINALLY {
 *        .
 *        .
 *     } ENDTRY;
 */

#ifndef ITL_WITHOUT_DCE
#include <dce/exc_handling.h>
#else
#define TRY
#define ENDTRY 
#define FINALLY
#endif /* ITL_WITHOUT_DCE */

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <assert.h>

#include <pthread.h>

#include <utils/itl/itl_private.h>
#include <utils/itl/itlio_internal.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_threadStatic.h>

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: itlio_internal.c,v 1.1.417.1 1996/10/17 18:28:03 damon Exp $";
#endif

PRIVATE char *InitialIOString = NULL;
SHARED char *itlIO_includePathVarName = "itlIO_ThreadIncludePath";

/*
 *  Declare keys for thread data area(s) used by this module.
 */
PRIVATE pthread_key_t ioDataKey;
PRIVATE pthread_key_t historyDataKey;

PRIVATE void InitIOData(infoP)
  IN itlIO_info_t *infoP;
{
    infoP->io_out = stdout;	             /* output file */
    infoP->io_error = stderr;	             /* error output file */
    infoP->fileName = "";                    /* fileName  */
    infoP->parentFile = "";	             /* parent File */
    infoP->fileActive = TRUE;                /* file Active (or string)    */
    infoP->inputString = &InitialIOString;   /*  Input from a string */
    infoP->interactive = TRUE;               /*  Interactive mode?  */
    infoP->nestingLevel = 0;	             /*  Nesting level  */
    infoP->braceLevel = 0;	             /*  Brace Nesting level */
    infoP->defaultScope = var_global;        /*  Default Scope  */
    infoP->error = FALSE;	             /*  Parsing Error	*/
    infoP->interface = FALSE;                /* interface mode */
    infoP->includeNesting = 0;               /* include nesting level	*/
    infoP->earlyQuit = FALSE;                /* Was an early quit detectred  */
    infoP->openPermitted = TRUE;	     /* allowed to open file if needed */
    infoP->defaultInput = stdin;             /* The default input file */
}

/*
 *  itlIO_Data
 *	The IO information of this thread.
 */
SHARED itlIO_info_t *itlIO_Data()
{
    itlIO_info_t *currentIOP;

    ITL_THREAD_STATIC_INIT(ioDataKey, currentIOP, itlIO_info_t, InitIOData);

    if (itl_traceLevel & ITL_THREAD_SLOT_TRACE) {
	pthread_t pthreadId = pthread_self();

	/*
	 *  We cannot use itl_TracePrintf here because we do not
	 *  want it to call Cur_Out!!
	 */
	fprintf(currentIOP->io_out,
		"[%d] (-< Current_IO_Data on slot %d, data %x\n",
		pthread_getunique_np(&pthreadId),
		(int) ioDataKey,
		currentIOP);
	fflush(currentIOP->io_out);
    }

    return(currentIOP);
}


/*
 *  Dump the IO state for the thread
 */
EXPORT void itlIO_DumpIOState(outFile)
  IN FILE *outFile;
{
    itlIO_info_t *currentInfo = Current_IO;

    if (currentInfo->fileActive) {
	itl_Message("Running from a file: %s, parent file is %s.",
		    currentInfo->fileName,
		    currentInfo->parentFile ? currentInfo->parentFile :
		    "[NULL]");
    }
    else {
	itl_Message("Running from a string at address %x\n\tThe remaining length is %d\n",
		    *(currentInfo->inputString),
		    strlen(*(currentInfo->inputString)));
    }
    itl_Message("\tModes: Interactive - %s, interface - %s\n\tNesting %d, Brace Nesting: %d\n\tCurrent IO: Out %x, Err %x; IO Error: %d\n",
		  currentInfo->interactive ? "on" : "off",
		  currentInfo->interface ? "on" : "off",
		  currentInfo->nestingLevel,
		  currentInfo->braceLevel,
		  currentInfo->io_out,
		  currentInfo->io_error,
		  currentInfo->error);
}


EXPORT boolean_t itlIO_IsInterface()
{
    return(Current_IO->interface);
}

EXPORT boolean_t itlIO_SetInterfaceMode(flag)
  IN boolean_t flag;
{
    Current_IO->interface = flag;
    /*
     *  Set the command mode which controls the available commands
     */
    if (flag) {
	CommandMode |= COMMAND_MODE_INTERFACE;
    } else {
	CommandMode &= ~COMMAND_MODE_INTERFACE;
    }
    itl_AdviseUser("Explicit mode is %s\n",
		   Current_IO->interface ? "on" : "off");
    return(FALSE);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *	   Routines for accessing and setting the
 *	   thread Specific IO data.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */

/*
 *    itlIO_IsInteractive()
 *  Are we in interactive mode?
 *  This effects prompting and other output.
 */
EXPORT boolean_t itlIO_IsInteractive()
{
  return(Current_IO->interactive);
}

/*
 *    itlIO_EarlyQuit()
 *  Was an early quit detected?
 */
EXPORT boolean_t itlIO_EarlyQuit()
{
  return(Current_IO->earlyQuit);
}

/*
 *    itlIO_SetEarlyQuit()
 *  Set the flag indicating whether a quit was detected or not.
 */
EXPORT void itlIO_SetEarlyQuit(flag)
  IN	boolean_t flag;
{
  Current_IO->earlyQuit = flag;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *	itlIO_NestingLevel()
 *	Return the current nesting level for this thread.
 *	The nesting level is set automatically when the thread
 *	start a loop or a coLoop.
 */
EXPORT unsigned int itlIO_NestingLevel()
{
    return(Current_IO->nestingLevel);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *	itlIO_IncNestingLevel()
 *	Increment the nesting level by a specified ammount and return
 *	the new value.
 */
EXPORT unsigned int itlIO_IncNestingLevel(num)
  IN int num;
{
    return(Current_IO->nestingLevel += num);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *	itlIO_BraceLevel()
 *	Return the current brace nesting level for this thread.
 *	When transactions are started with tran-C the code to
 *	be executed in the transaction is enclosed in Braces.
 *	When the transaction is aborted, it may need to jump to
 *	the corresponding brace.
 */
EXPORT unsigned int itlIO_BraceLevel()
{
    return(Current_IO->braceLevel);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_IncBraceLevel
 *	Increment the current brace level
 *	Return the new brace level.
 */
SHARED int itlIO_IncBraceLevel(num)
  IN int num;
{
    return (Current_IO->braceLevel += num);
}

/*  
 *   currentOutput:  Return a file handle for the current output
 *                   file.  The default is stdout.
 */
EXPORT FILE *itl_CurrentOutput()
{
    return(Current_IO->io_out);
}

/*  
 *  currentErr:  Return a file handle for the current error output
 *               file.  The default is stderr.
 */
EXPORT FILE *itl_CurrentErr()
{
    return(Current_IO->io_error);
}

/*
 *  itlIO_GetFilename():
 *     Return the current input file name.
 */
EXPORT char *itlIO_GetFilename()
{
    return(Current_IO->fileName);
}

/*
 *  Set (or reset) interactive mode.
 */
EXPORT boolean_t itl_SetInteractive(flag)
IN boolean_t flag;
{
    Current_IO->interactive = flag;
    return(FALSE);
}

/*
 *  itlIO_defaultScope():
 *     Return the default scope for variables
 */
EXPORT scope_t itlIO_DefaultScope()
{
    return(Current_IO->defaultScope);
}

/*
 *  itlIO_SetDefaultScope()
 *     set the default scope for variables.
 *     By default, the default scope is set to LOCAL in all threads
 *     but the main thread.
 */
EXPORT void itlIO_SetDefaultScope(scope)
  IN scope_t scope;
{
    Current_IO->defaultScope = scope;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  The History Buffer
 *
 *  All commands are first cached into a history ring and then executed.
 *  The location into the history buffer are stored in a thread safe
 *  structure of type history_t.
 *
 *  For each history buffer there may be more than one thread that is
 *  allowed to write to that buffer.  Each history buffer has a latch and
 *  a prompt counter.  A thread must hold the lock before it is
 *  permitted to write to the history buffer.  When a thread needs a line
 *  it follows the following algorithm:
 *    1- See if the line is already there, if so use it.
 *    2- Attempt to get the lock
 *      A: Got the lock
 *         1- See if the line is already there.  
 *	      If so, drop the lock and use the line.
 *	   2- If the line is still not there, read it from the input
 *	      and write it to the buffer.
 *	B: Did not get the lock:
 *	   1- sleep for a second or so and try again (go to step 1)
 *
 *  In order to prevent too much prompting, each history buffer keeps
 *  a count of the last line for which the user was prompted for.
 *  When a thread needs a line, if it is already there, it uses it.
 *  If it is not there it checks to see if the line it is looking for
 *  has already been prompted for, if so, it does not prompt but simply
 *  tries to get the line.  If the line has not been prompted for, the 
 *  thread tries to get the lock, following the steps as above.
 *  If the thread gets the lock it checks whether the line was prompted
 *  for.  If so it does not prompt.
 *  If the line was not prompted for it prompts and updates the counter.
 *  If the thread cannot get the lock it sleeps a bit and tries again.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 *  			The history buffer
 *
 */
/*
 *  itlIO_fileHandle_t:
 *	A linked list of file handles associated with this history buffer
 *	Each time an include is nested, if the thread needs
 *	to read from the file it reads from the current open
 *	file handle, if any, or opens a new one and pushes it
 *	on to the stack.
 *	Each file has a nesting level associated with it.
 *	This is used to determine if the file needs to be opened or not.
 *
 *	Each history buffer has an array to a stack of fileHandles (of
 *	this type).  The extra level of indirection is needed in orer
 *	for all threads that are using the history buffer to see the same
 *	stack (they each have a copy of the history buffer structure).
 */
typedef struct itlIO_fileHandle_struct {
  FILE		*handle;		/*  The open file descriptor  */
  char		*fileName;		/*  The name of the open file */
  int		 lineCount;		/*  Number of lines read      */
  int		 includeNesting;	/*  The nesting level of this file  */
  struct itlIO_fileHandle_struct *prevP;/*  Previous file structure   */
} itlIO_fileHandle_t;
/*
 *  histLine_t
 *	One entry in the history buffer.
 */
typedef struct {
   char *line;		/*  The text of the line   */
   int	 len;		/*  The length of the line */
   int	 nesting;	/*  The include nesting (for the return statement) */
   int	 offset;	/*  Offset to the first word that was interpreted
			 *  in this line.  This is to distinguish between
			 *  text that was inserted into the history buffer
			 *  while skipping looking for a '}' or EOF
			 */
   int	 fileIndex;	/*  The line number in the current include file */
}  itlIO_histLine_t;

typedef struct {
    int index;			/*  The line in the history buffer	*/
    int offset;			/*  The offset into the line.		*/
    boolean_t printed;		/*  Was the line displayed already?	*/
    itlIO_histLine_t *active;	/*  The active history buffer 		*/
    itlIO_fileHandle_t **fileHandles;  /* A stack of open fileHandles	*/
    pthread_mutex_t *updateLock;/*  The lock needed to write to the buffer */
    pthread_mutex_t *promptLock;/*  The lock needed for prompting	*/
    int *lastPromptP;		/*  The last line the user was prompted for */
    int maxLen;			/*  The size allocated for the buffer	*/
    int *histLenP;		/*  The current number of lines		*/
    char **lastPromptStrP;	/*  The last prompt for this history buffer */
    int	 fileIndexStart;	/*  The index in the history buffer when
				 *  the file started
				 */
} itlIO_history_t;


SHARED int histLen   = 0;
SHARED itlIO_histLine_t commandHistory[MAX_HISTORY_LENGTH];
PRIVATE pthread_mutex_t historyUpdateLock;
PRIVATE pthread_mutex_t promptCountUpdateLock;
PRIVATE char *lastPromptStr;
PRIVATE int global_lastPrompt = -1;
PRIVATE itlIO_fileHandle_t *globalFileHandleP = NULL;
SHARED int itl_coLoopStackSize = 100*1024;


PRIVATE void InitHistoryData(historyP)
  IN itlIO_history_t *historyP;
{
    historyP->index = 0;       /* index   */
    historyP->offset = 0;       /* offset  */
    historyP->printed = FALSE;   /* printed? */
    historyP->active = commandHistory;
    historyP->fileHandles = &globalFileHandleP; /* fileHandles: default null */
    historyP->updateLock = &historyUpdateLock;
    historyP->promptLock = &promptCountUpdateLock;
    historyP->lastPromptP = &global_lastPrompt;
    historyP->maxLen = MAX_HISTORY_LENGTH;
    historyP->histLenP = &histLen;
    historyP->lastPromptStrP = &lastPromptStr;
    historyP->fileIndexStart = 0;
}


PRIVATE itlIO_history_t *Current_Hist_Data()
{
    itlIO_history_t *currentHist;

    ITL_THREAD_STATIC_INIT(historyDataKey, currentHist, itlIO_history_t, 
			   InitHistoryData);

    itl_fTrace(Cur_Out, 
	       ITL_THREAD_SLOT_TRACE,
	       "Current_Hist on slot %d, data %x\n",
	       (int) historyDataKey,
	       currentHist);

    return(currentHist);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *	 Macros and routines for accessing
 *	 thread specific history data.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */

#define HistIndex(_hist)	   ((_hist)->index)
#define HistOffset(_hist)          ((_hist)->offset)
#define HistActive(_hist)          ((_hist)->active)
#define HistLock(_hist)	           ((_hist)->updateLock)
#define HistPromptLock(_hist)	   ((_hist)->promptLock)
#define HistLastPrompt(_hist)    (*((_hist)->lastPromptP))
#define HistLastPromptStr(_hist) (*((_hist)->lastPromptStrP))
#define HistMaxLen(_hist)	   ((_hist)->maxLen)
#define HistHistLen(_hist)       (*((_hist)->histLenP))
#define LinePrinted(_hist)         ((_hist)->printed)
#define HistIndexStart(_hist)	   ((_hist)->fileIndexStart)

#define Current_Hist    Current_Hist_Data()

#define Hist_Index		HistIndex(Current_Hist)
#define Hist_Offset		HistOffset(Current_Hist)
#define Hist_Active		HistActive(Current_Hist)
#define Hist_Lock		HistLock(Current_Hist)
#define Hist_PromptLock		HistPromptLock(Current_Hist)
#define Hist_LastPrompt		HistLastPrompt(Current_Hist)
#define Hist_LastPromptStr	HistLastPromptStr(Current_Hist)
#define Hist_MaxLen		HistMaxLen(Current_Hist)
#define Hist_HistLen		HistHistLen(Current_Hist)
#define Line_Printed		LinePrinted(Current_Hist)
#define Hist_IndexStart		HistIndexStart(Current_Hist)

/*
 *  itlIO_HistoryLoc()
 *    returns a character pointer pointing to the current location in
 *    the history list.
 */
SHARED char *itlIO_HistoryLoc()
{
    itlIO_history_t *CH = Current_Hist;
    if (HistIndex(CH) >= HistHistLen(CH))
	return((char *)NULL);
    else
	return(&(HistActive(CH)[HistIndex(CH)].line[HistOffset(CH)]));
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_PrintPrompt
 *	return TRUE iff the current thread should prompt.
 *	When this function return TRUE, the prompt latch is
 *	kept locked and should be unlocked after the caller
 *	finishes prompting.
 */
EXPORT boolean_t itlIO_PrintPrompt()
{
    itlIO_history_t *CH = Current_Hist;
    struct timespec sleepTime;

    /*
     *  We could not get the lock, a sleep time of 100ms
     */
    sleepTime.tv_sec = 0;
    sleepTime.tv_nsec = 100000;

    if (!itlIO_IsInteractive() || !itlIO_SkipWhites()) {
	return(FALSE);
    }
    while (HistLastPrompt(CH) < HistIndex(CH)) {
	/*
	 *  There is nothing useful in the buffer yet.  Try to prompt.
	 */
	if (pthread_mutex_trylock(HistPromptLock(CH))) {
	    /*
	     *  We succeeded in getting the prompt lock
	     */
	    if (HistLastPrompt(CH) < HistIndex(CH)) {
		HistLastPrompt(CH) = HistIndex(CH);
		itl_Trace(ITL_IO_TRACE,
			  "Set the prompt index to %d.\n",
			  HistLastPrompt(CH));
		/*
		 *  The latch is intentionally left locked and will
		 *  be unlocked by the caller.
		 */
		return(TRUE);
	    }
	    else {
		pthread_mutex_unlock(HistPromptLock(CH));
		return(FALSE);
	    }
	}
	else {
	    itl_Trace(ITL_IO_TRACE, "Sleeping waiting for prompt lock.\n");

	    pthread_delay_np(&sleepTime);
	}
    }
    return(FALSE);
}

/*
 *  itlIO_DonePrompting
 *	When the user is is allowed to prompt, the call to
 *	itlIO_PrintPrompt leaves the promptLock locked.
 *	This releases that lock.
 */
EXPORT void itlIO_DonePrompting()
{
    pthread_mutex_unlock(Current_Hist->promptLock);
}

EXPORT void itlIO_Cleanup()
{
  int i = 0;
  while (commandHistory[i].line) {
    ITL_UTIL_FREE(commandHistory[i].line);
    i++;
  }
}

EXPORT char *itlIO_LastPromptStr()
{
    return(Hist_LastPromptStr);
}

/*
 *  itlIO_LastPromptStrP
 *	Return the pointer to the last prompt string (char **)
 */
EXPORT char **itlIO_LastPromptStrP()
{
    return(Current_Hist->lastPromptStrP);
}

SHARED void itlIO_TraceCurrentLocation()
{
  if (Hist_Active[Hist_Index].fileIndex != -1) {
    itl_fMessage(Cur_Err, "Line %d of file %s, Contents:\n    %s\n",
		 Hist_Active[Hist_Index].fileIndex,
		 Current_IO->fileName,
		 Hist_Active[Hist_Index].line);
  } else {
    itl_fMessage(Cur_Err, "In a procedure in file %s, Contents:\n    %s\n",
		 Current_IO->fileName,
		 Hist_Active[Hist_Index].line);
  }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 *
 *  The ITL_IO module has information associated with each thread.
 *  It registers itself as a client to make sure all this information
 *  is up to date.  It provides the following three upcalls.
 *
 *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 */
typedef struct {
    itlIO_history_t *parentHist;
    itlIO_info_t *parentIO;
    varList_t parentVars;
} ioThreadCreate_t;

PRIVATE void InitializeITLThread(ptrP)
  IN pointer_t *ptrP;
{
    ioThreadCreate_t *threadInfoP;
    itlIO_history_t *histP;
    itlIO_info_t *ioP;

    ITL_NEW(histP, itlIO_history_t);
    *histP = *Current_Hist;

    ITL_NEW(ioP, itlIO_info_t);
    *ioP = *Current_IO;

    ITL_NEW(threadInfoP, ioThreadCreate_t);
    threadInfoP->parentIO = ioP;
    threadInfoP->parentHist = histP;
    threadInfoP->parentVars = itlExp_CollectAllVars(TRUE, TRUE);
    *ptrP = (pointer_t)threadInfoP;
}

PRIVATE void StartITLThread(ptrP)
  IN pointer_t *ptrP;
{
    ioThreadCreate_t *threadInfoP = (ioThreadCreate_t *)*ptrP;
    pthread_t pthreadId = pthread_self();

    *Current_Hist = *(threadInfoP->parentHist);
    *Current_IO = *(threadInfoP->parentIO);
    itlExp_SetParentVarList(&threadInfoP->parentVars);
    itl_SetIntResult(&threadID, (int) pthread_getunique_np(&pthreadId));
    Current_IO->defaultScope = var_local;
}

PRIVATE void CleanupITLThread(ptrP)
  IN pointer_t *ptrP;
{
    ioThreadCreate_t *threadInfoP = (ioThreadCreate_t *)*ptrP;

    ITL_FREE(threadInfoP->parentHist);
    ITL_FREE(threadInfoP->parentIO);
    itlExp_FreeVarList(&threadInfoP->parentVars);
    ITL_FREE(threadInfoP);
}

/******************************************************************
 ******************************************************************
 ********
 ********    Internal FUNCTIONS for performing the RAW I/O.
 ********
 ******************************************************************
 ******************************************************************
 */


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *		Routines for opening UNIX files
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
/*******************************************************************
 *
 *  OpenFileName -
 *     Given a file name, open it for reading and return the file handle.
 *     The file is assumed to be in the current dirctory.
 *     If the first character is the special character ~, expand it.
 *
 *     The function returns a file descriptor if it successfully opened
 *     the file and NULL otherwise.
 *
 *******************************************************************
 */
PRIVATE FILE *OpenFileName(fileName)
  IN char *fileName;
{
    char *home, *realName;
    FILE *retVal;
    struct stat statBuffer;
    char *getenv();

    /*
     *  Check for the ~ and process the file name appropriately.
     */
    if (*fileName == '~') {
	if (fileName[1] == '/') {  /* use current users home */
	    if ((home = getenv("HOME")) != NULL) {
		/*
		 *  No need to add 1 to the calculation to make room for a
		 *  end of string character since we include the
		 *  ~ in the filename, but it is removed when actually doing
		 *  the copy.
		 */
		ITL_UTIL_ALLOC(realName, char *, 
				(strlen(home) + strlen(fileName)));
		sprintf(realName, "%s%s", home, &fileName[1]);
		itl_Trace(ITL_IO_TRACE, "Using file %s\n", realName);
	    } else {
		itl_Trace(ITL_IO_TRACE,
			  "HOME variable not defined, ~ is invalid.\n");
		realName = fileName;
	    }
	} else {   /* use another users home directory */
	    struct passwd *pwEntry;
	    char *fileNameEnd = strchr(fileName, '/');
	    int nameEnd;

	    if (fileNameEnd) {
		void setpwent();
		setpwent();
		nameEnd = strchr(fileName, '/') - fileName;
		fileName[nameEnd] = '\0';
		if ((pwEntry = getpwnam(&fileName[1])) != NULL) {
		    /*
		     *  Allocate space for the real name:  The remainder
		     *  of the name, + the path returned from getpwnam +
		     *  a / and the terminating NULL.
		     */
		    ITL_UTIL_ALLOC(realName, char *, 
				    (strlen(pwEntry->pw_dir) +
				     strlen(&fileName[nameEnd+1]) + 2));
		    sprintf(realName, "%s/%s",
			    pwEntry->pw_dir,
			    &fileName[nameEnd+1]);
		} else {
		    itl_Trace(ITL_IO_TRACE,
			      "Invalid user name, %s.\n", &fileName[1]);
		    realName = fileName;
		}
		fileName[nameEnd] = '/';
	    } else {
		realName = fileName;
	    }
	}
    } else {
	realName = fileName;
    }
    if ((stat(realName, &statBuffer) != -1) &&
	(statBuffer.st_mode & S_IFREG)) {
	retVal = fopen(realName, "r");
    } else {
	itl_Trace(ITL_IO_TRACE,
		  "file %s is not a regular file\n", fileName);
	retVal = NULL;
    }
    /*
     *  Update the currentFileName variable if we succeeded in the open.
     */
    if (retVal) 
	itl_SetStrResult(&currentFileName, realName);

    if (realName != fileName)
	ITL_FREE(realName);

    return(retVal);
}

/************************************************************************
 *  OpenFileStructure -
 *     Given a file name open the file and update the fileHandle structure.
 *	
 ************************************************************************
 */
PRIVATE boolean_t OpenFileStructure _TAKES((
  IN itlIO_history_t *histStructP,
  IN char *fileName
));
PRIVATE boolean_t OpenFileStructure(histStructP, fileName)
  IN itlIO_history_t *histStructP;
  IN char *fileName;
{
  itlIO_fileHandle_t *newHandleP;
  ITL_NEW(newHandleP, itlIO_fileHandle_t);

  if (strlen(fileName) == 0) {
    /*
     *  Use the default file (stdin)
     */
    newHandleP->handle = Current_IO->defaultInput;
  } else {
    newHandleP->handle = OpenFileName(fileName);
  }

  if (newHandleP->handle) {
    newHandleP->prevP = *(histStructP->fileHandles);
    newHandleP->includeNesting = Current_IO->includeNesting;
    newHandleP->lineCount = 0;
    ITL_ALLOC(newHandleP->fileName, char *, strlen(fileName) + 1);
    ITL_MEMCOPY(fileName, newHandleP->fileName, strlen(fileName) + 1);
    *(histStructP->fileHandles) = newHandleP;

    itl_Trace(ITL_FILE_TRACE,
	      "OpenFileStructure for file %s, level %d str: %x, prev %x\n",
	      fileName, newHandleP->includeNesting,
	      newHandleP, newHandleP->prevP);
  } else {
    itl_Trace(ITL_FILE_TRACE,
	      "OpenFileStructure for file %s, level %d Failed.\n",
	      fileName, newHandleP->includeNesting);
    ITL_FREE(newHandleP);
    return(FALSE);
  }
  return(TRUE);
}

/************************************************************************
 *  OpenFileOnDir -
 *     Given a file name and a directory path, try to open the
 *     file in that directory for reading.
 *     If the file exists, and the open succeeds the routine frees
 *     the current file name (Current_IO->fileName) and sets the full
 *     path of the file it found as the new current fileName.
 *     It then returns FALSE.
 *     If the open does not succeed the current file name is not
 *     changed and the routine returns TRUE.
 ************************************************************************
 */
PRIVATE boolean_t OpenFileOnDir(fileName, dir, dirLen)
  IN char *fileName;
  IN char *dir;
  IN int dirLen;
{
    char *altFile;
    ITL_UTIL_ALLOC(altFile, char *,
		       dirLen + strlen(fileName) + 2);
    ITL_UTIL_MEMCOPY(dir, altFile, dirLen);
    altFile[dirLen] = '\0';
    (void) strcat(altFile, "/");
    (void) strcat(altFile, fileName);
    itl_Trace(ITL_VERBOSE_TRACE,
	      "Trying to open the file %s\n", altFile);
    if (OpenFileStructure(Current_Hist, altFile)) {
	/*
	 *  Store the REAL name of the file.
	 */
	ITL_FREE(Current_IO->fileName);
	Current_IO->fileName = altFile;
	return(FALSE);
    }
    else {
	ITL_FREE(altFile);
	return(TRUE);
    }
}
/*
 *	CurrentIncludePath
 *
 *	Return the current include path for the thread, if any.
 */
PRIVATE char *CurrentIncludePath _TAKES(());
PRIVATE char *CurrentIncludePath()
{
    itl_var_t *varP = itl_LocateVariable(itlIO_includePathVarName);
    if (varP && VAR_TYPE(varP) == itl_stringType) {
	return(VAR_STR_VALUE(varP));
    } else {
	return(NULL);
    }
}

/**************************************************************************
 *  OpenCurrentFile()
 *	Open the current file (Current_IO->fileName) for reading.
 *      Path Search:
 *	  1- First check if it is an absolute name (i.e., starts with
 *	     either / or ~).
 *	  2- Check the file in the directory where the parent file
 *	     (if any) is in.
 *	  3- If the user specified a path, check all the files on the path.
 *	  4- If the user did not specify a path, check the current dir.
 **************************************************************************
 */
PRIVATE boolean_t OpenCurrentFile()
{
    char *fileName = Current_IO->fileName;
    char *defDir, *parentFile, *filePart;
    itl_Trace(ITL_VERBOSE_TRACE,
	      "Opening the file %s\n",
	      fileName);
    /*
     *	Check whether the file is an absolute file name.
     */
    if ((fileName[0] == '~') || (fileName[0] == '/') ||
	(strlen(fileName) == 0)) {
	if (OpenFileStructure(Current_Hist, fileName)) {
	    return(FALSE);
	} else {
	    itl_Error("Unable to open the file %s\n", fileName);
	    return(TRUE);
	}
    }
    /*
     *  Check the directory the parent file, if any, is in.
     */
    if ((parentFile = Current_IO->parentFile) &&
	(filePart = strrchr(parentFile, '/'))) {
	/*  There is a parent file and its directory is not NULL
	 */
	if (!OpenFileOnDir(fileName,
			   parentFile,
			   (filePart - parentFile))) {
	    return(FALSE);
	}
    }
    /*
     *  The first choice did not pan out, try the default
     *  path specified by the user, if any.
     */
    if (defDir = CurrentIncludePath()) {
	char *endDir;
	/*
	 *  The path is a list of directories seperated by colons.
	 */
	do {
	    endDir = strchr(defDir, ':');
	    if (!OpenFileOnDir(fileName,
			       defDir,
			       endDir ? endDir - defDir : strlen(defDir))) {
		return(FALSE);
	    }
	    defDir = endDir + 1;
	} while (endDir);
    } else {
	/*
	 *  The user did not specify a path, check the current dir.
	 */
	if (OpenFileStructure(Current_Hist, fileName)) {
	    return(FALSE);
	}
    }
    /*
     *  If we are here - We still could not find the file.
     *  Give up.
     */
    itl_Error("Unable to open the file %s\n", fileName);
    return(TRUE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *		Getting Data into the history buffer
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */

/****************************************************************************
 * Function: GetInputString
 *   Get one line of input from the current input device (may be a string).
 *
 *   If the EOF or the end of string is reached without reading any
 *   characters, the function returns the string "quit".
 *   The return string is allocated dynamically.
 *
 *   When the user wants to read from a file the test program writes the
 *   name of the file in the IO record (Current_IO->fileName) and notes that
 *   the file has not been opened (Current_IO->fileOpened = FALSE).
 *   The file is opened only when necessary.
 * 
 *   When a file is included within a loop, all the same routines
 *   are called, but since the file is cached in the history buffer after
 *   the first iteration of the loop, it is opened only once.
 *
 *  Locking:
 *	The caller is assumed to hold the History Lock upon entry to
 *	this routine.  This lock is held throughout and kept held on return.
 ****************************************************************************
 */
PRIVATE char *GetInputString(eofP)
  OUT boolean_t *eofP;
{
    char *string;
    int strAllocated;
    int ind = 0;
    char c;
    itlIO_info_t *ioP = Current_IO;

    *eofP = FALSE;	  /*  Default  */
    if (ioP->fileActive) {
        itlIO_fileHandle_t *curHandleP = *(Current_Hist->fileHandles);
	/*  Reading from a file (as opposed to a string)
	 *  Check if the file is open or should be opened.
	 */
	if (!curHandleP ||
	    ((curHandleP->includeNesting != ioP->includeNesting))) {

	    assert(!curHandleP ||
		  (curHandleP->includeNesting == (ioP->includeNesting - 1)) ||
		  (ioP->openPermitted &&
		   (curHandleP->includeNesting < ioP->includeNesting)));
	    /*
	     *  We need to open the file
	     */
	    if (! ioP->openPermitted) {
		/*
		 *  We are not permitted to open the file.
		 *  This means we are attempting to read passed the EOF.
		 */
		ITL_ALLOC(string, char *, 5);
		ITL_UTIL_MEMCOPY("quit", string, 5);
		*eofP = TRUE;
		return(string);
	    } else if (OpenCurrentFile()) {
		*eofP = TRUE;
		return(NULL);
	    } else {
		/*
		 *  The current handle changed, cache the new one
		 */
		itl_Trace(ITL_IO_TRACE,
			  "Opened the file %s\n",
			  ioP->fileName);
		curHandleP = *(Current_Hist->fileHandles);
	    }
	}
	strAllocated = 0;
	string = NULL;
	do {
	    /*
	     *  We are going to read from the input file using READ
	     *  rather then getc:
	     *    1- When we read using FILE * instead of a file
	     *	     descriptor a read blocks writes to the same
	     *	     file descriptor, which is annoying when we
	     *	     are trying to read from a socket.
	     *	  2- We read only one character at a time anyway.
	     * XXX
	     *    For now, we convert a FILE * to fileD using fileno,
	     *	  In the future maybe we should open the file using open
	     *    instead of fopen and keep fileD instead of a FILE *.
	     */
	    int fileD = fileno(curHandleP->handle);

	    if (ind >= strAllocated) {
		strAllocated += 32;
		ITL_UTIL_REALLOC(string, char *, strAllocated);
	    }
	    if (read(fileD, &c, 1) < 1) {
		string[ind] = '\0';
		if(ind==0) {
		    itlIO_fileHandle_t *prevHandleP;
		    *eofP = TRUE;
		    ITL_UTIL_REALLOC(string, char *, 5);
		    ITL_UTIL_MEMCOPY("quit", &string[ind], 5);

		    /*
		     * We read passed the EOF, pop the file off the stack
		     */
		    itl_Trace(ITL_FILE_TRACE,
			      "Closing file %s, level %d (current %d) struct %x.\n",
			      curHandleP->fileName,
			      ioP->includeNesting,
			      curHandleP->includeNesting,
			      curHandleP);
	      
		    prevHandleP = curHandleP->prevP;
		    fclose(curHandleP->handle);
		    ITL_FREE(curHandleP->fileName);
		    ITL_FREE(curHandleP);

		    *(Current_Hist->fileHandles) = prevHandleP;
		    curHandleP = *(Current_Hist->fileHandles);
		}		
		break;
	    } else {
		string[ind] = c;
		if (string[ind] == EOLN) {
		    string[ind] = '\0';
		    curHandleP->lineCount ++;
		    break;
		}
		ind++;
	    }
	} while (TRUE);
    } else {  /*  fileActive == FALSE */
	int EOLN_Loc;

	assert(*(ioP->inputString) != (char *) NULL);
	if ((*(ioP->inputString))[0] == '\0') {
	    ITL_UTIL_ALLOC(string, char *, 5);
	    ITL_UTIL_MEMCOPY("quit", string, 5);
	    *eofP = TRUE;
	} else {
	    EOLN_Loc = strcspn(*(ioP->inputString), "\n");
	    ITL_UTIL_ALLOC(string, char *, EOLN_Loc+1);
	    ITL_UTIL_MEMCOPY((pointer_t)(*(ioP->inputString)),
			     (pointer_t)string,
			     EOLN_Loc);
	    string[EOLN_Loc] = '\0';
	    *(ioP->inputString) += EOLN_Loc;
	    if ((*(ioP->inputString))[0] == EOLN)
		(*(ioP->inputString))++;
	}
    }
    return(string);
}


/*
 *  GetNewLine(index)
 *   Get a new line from input and insert it into the end of
 *   the current history buffer.
 *
 *  Only one thread reads and updates any history buffer.
 *  The program maintains the invariant that the entry following
 *  the current entry is always set to NULL so that it is safe
 *  for threads not doing IO to wait till the entry is not NULL.
 *  Upon initialization and when starting a local history
 *  buffer the first entry is reset to NULL.
 *  BEFORE writing a new entry, the process in charge of IO
 *  for the history buffer resets the next entry in the 
 *  history buffer.
 *
 */
PRIVATE boolean_t GetNewLine(anIndex, display)
  IN int anIndex;
  IN boolean_t display;
{
    boolean_t eof;
    char *newHistLine;
    itl_Trace(ITL_IO_TRACE, "Looking for line %d\n", anIndex);

    assert(Hist_HistLen < Hist_MaxLen);

    while (!pthread_mutex_trylock(Hist_Lock)) {
	struct timespec sleepTime;
	/*
	 *  We could not get the lock, sleep and try again later.
	 */
	sleepTime.tv_sec = 1;
	sleepTime.tv_nsec = 0;
	itl_Trace(ITL_IO_TRACE, "Sleeping waiting for line %d\n", anIndex);

	pthread_delay_np(&sleepTime);

	if (Hist_Active[anIndex].line) {
	    /*
	     *  While we were asleep someone else read the line for us
	     */
	    itl_Trace(ITL_IO_TRACE, "IO_GetNewLine found:<%s>\n",
		      Hist_Active[anIndex].line);

	    if (display && isDisplayLines()) {
		itl_Message("+(%d): %s\n",
			    Hist_Active[anIndex].nesting,
			    Hist_Active[anIndex].line);
		Line_Printed = TRUE;
	    }
	    return(FALSE);
	}
    }
    
    /*
     *  Check, maybe someone else read the line for us.
     */
    if (Hist_Active[anIndex].line) {
	pthread_mutex_unlock(Hist_Lock);
	if (display && isDisplayLines()) {
	    itl_Message("+(%d): %s\n",
			Hist_Active[anIndex].nesting,
			Hist_Active[anIndex].line);
	    Line_Printed = TRUE;
	}
	return(FALSE);
    }

    /*
     *  Now that we have the lock we can read from the input
     */
    
    newHistLine = GetInputString(&eof);
    if (!newHistLine) {
	pthread_mutex_unlock(Hist_Lock);
	return(TRUE);
    }
    itl_Trace(ITL_IO_TRACE,
	      "GetInputString returned:<%s> for line %d\n",
	      newHistLine, Hist_HistLen);
    
    assert(Hist_HistLen == Hist_Index);
    Hist_Active[Hist_HistLen+1].line = NULL;	/*  Initialize the next line */

    if (display && isDisplayLines()) {
	itl_Message("*(%d): %s\n",
		    Current_IO->includeNesting,
		    newHistLine);
	Line_Printed = TRUE;
    }
    /*
     *  Check for the special HIST_CHARACTER which is used to edit
     *  the hostory buffer (similar to !! or !2 or !-2 in csh
     *  We check if the next character is a * for backwards
     *  compatability: !* Was traditionally the print commment line
     *  prefix.
     */
    if ((*newHistLine == HIST_CHARACTER) &&
	(newHistLine[1] != '*')) {
	/*  If the first letter in the list is the history character,
	 *  it is a special line asking to repeat a previous line,
	 *  like ! in csh.
	 */
	int prevIndex;
	if (newHistLine[1] == HIST_CHARACTER) {
	    prevIndex = -1;
	} else {
	    prevIndex = atoi(&newHistLine[1]);
	}
	if (prevIndex < 0) {
	    /*  Negative, move back from last line
	     */
	    prevIndex = Hist_HistLen + prevIndex;
	} 
	if ((prevIndex >= 0) || (prevIndex < Hist_HistLen)) {
	    /*  It is a valid index, copy the appropriate line
	     *  If the index is not valid the line is kept as is
	     */
	    /*  First, free the line containing the @ info  */
	    ITL_FREE(newHistLine);
	    ITL_UTIL_ALLOC(newHistLine, char *,
			   (Hist_Active[prevIndex].len + 1));
	    ITL_UTIL_MEMCOPY(Hist_Active[prevIndex].line, newHistLine,
			     (Hist_Active[prevIndex].len + 1));
	    if (itlIO_IsInteractive()) { 
		itl_Message("%d: %s\n", Hist_HistLen, newHistLine);
	    }
	    if (display && isDisplayLines()) {
		itl_Message("-(%d): %s\n",
			    Current_IO->includeNesting,
			    newHistLine);
		Line_Printed = TRUE;
	    }
	}
    }
    Hist_Active[Hist_HistLen].len = strlen(newHistLine);
    Hist_Active[Hist_HistLen].nesting = Current_IO->includeNesting;
    assert(Current_Hist->fileHandles);
    if (*(Current_Hist->fileHandles)) {
      Hist_Active[Hist_HistLen].fileIndex =
	  (*(Current_Hist->fileHandles))->lineCount;
    } else {
      Hist_Active[Hist_HistLen].fileIndex = -1;
    }
    Hist_Active[Hist_HistLen].line = newHistLine;
    Hist_HistLen++;

    pthread_mutex_unlock(Hist_Lock);
    return(eof);
}

/************************************************************************
 ************************************************************************
 ********      
 ********      
 ********      Routines for moving the cursor in the history buffer.
 ********      
 ********      
 ************************************************************************
 ************************************************************************
 */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_SkipWhites()
 *    Advance the index into the history buffer skipping over white spaces.
 *    return FALSE if a non white space was found and TRUE if it got
 *    to the end of the buffer.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT boolean_t itlIO_SkipWhites()
{
    boolean_t Found = FALSE;
    itlIO_history_t *CH = Current_Hist;

    do {
	char *curLine;

	assert(HistIndex(CH) <= HistHistLen(CH));
	itl_Trace(ITL_IO_DEBUG,
		  "- SkipWhites: index %d offset %d len %d\n",
		  HistIndex(CH), HistOffset(CH), HistHistLen(CH));
	if (HistIndex(CH) == HistHistLen(CH))
	    return(TRUE);

	curLine = HistActive(CH)[HistIndex(CH)].line;

	if ((HistOffset(CH) == 0) && isDisplayLines() && !LinePrinted(CH)) {
	    itl_Message("+(%d): %s\n",
			HistActive(CH)[HistIndex(CH)].nesting,
			curLine + HistOffset(CH));
	    LinePrinted(CH) = TRUE;
	}
	HistOffset(CH) += strspn(curLine + HistOffset(CH), WhiteSpaces);

	if (curLine[HistOffset(CH)] == '\0') {
	    /*
	     *  We reached the end of the line, move to the next line.
	     */
	    HistIndex(CH) = HistIndex(CH) + 1;
	    HistOffset(CH) = 0;
	    LinePrinted(CH) = FALSE;
	}
	else {
	    Found = TRUE;
	}
    } while (!Found);
    assert(HistActive(CH)[HistIndex(CH)].line[HistOffset(CH)] != ' ');
    return(FALSE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_AdvanceToNonWhite():
 *     Advance the pointer into the history buffer skipping white
 *     spaces and reading from input if necessary
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
boolean_t itlIO_AdvanceToNonWhite()
{
    if (!itlIO_SkipWhites()) {
	Current_IO->openPermitted = FALSE;
	return(FALSE);
    }
    do {
	/*
	 *  Get a new line.
	 *  We do not care if EOF was reached, if it was, we are interested
	 *  In the 'quit' command read by GetNewLine by default
	 */
	(void) GetNewLine(Hist_Index, TRUE);
	if (itlIO_ParsingError())
	    return(TRUE);
        if (!itlIO_SkipWhites()) {
	    Current_IO->openPermitted = FALSE;
	    return(FALSE);
	}
	/*
	 *  The user typed in an empty line, provide some default prompt
	 */
	itl_PromptUser("%s", itlIO_LastPromptStr());
    } while (TRUE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_MoveToEndOfBuffer
 *     move to the end of the history buffer.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT void itlIO_MoveToEndOfBuffer()
{
    Hist_Index = Hist_HistLen;
    Hist_Offset = 0;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_AdvanceIndex(int):
 *     Advance the history index a specified amount in the current line.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT void itlIO_AdvanceIndex(len)
IN int len;
{
  Hist_Offset += len;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_AdvanceLine:
 *    Advance to the next line (skip over the rest of this one)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
SHARED void itlIO_AdvanceLine()
{
    Hist_Offset += strlen(itlIO_HistoryLoc());
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_CurrentHistLoc:
 *    Return the current location in the history buffer (index, offset)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
SHARED void itlIO_CurrentHistLoc(indexP, offsetP)
  OUT int *indexP;
  OUT int *offsetP;
{
    *indexP = Hist_Index;
    *offsetP = Hist_Offset;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_GotoHistLoc:
 *    Goto a specified location in the history buffer (index, offset)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
SHARED void itlIO_GotoHistLoc(index, offset)
  IN int index;
  IN int offset;
{
    Hist_Index = index;
    Hist_Offset = offset;
}

/*
 *  itlIO_GotoIncludeNesting
 *	Skip the rest of the file and/or history buffer till
 *	arriving at the desired nesting level
 */
PRIVATE void itlIO_GotoIncludeNesting _TAKES((IN  int level));
PRIVATE void itlIO_GotoIncludeNesting(level)
  IN  int level;
{
  itlIO_history_t *CH = Current_Hist;
  itlIO_histLine_t *active = HistActive(CH);

  assert(Current_IO->includeNesting > level);

  while (1) {
    if (!active[HistIndex(CH)].line) {
      /*
       *  We are at the end of the history buffer.  We have to keep
       *  reading till the end of this file.
       */
      boolean_t eof = GetNewLine(HistIndex(CH), FALSE);
      if (eof) {
	/*
	 *  We reached EOF, decrement the include nesting and skip over
	 *  the last line of the fiel (the function GetNewLine inserted
	 *  a "quit" line).
	 */
	Current_IO->includeNesting --;
	if (!itlIO_ParsingError()) {
	    /*
	     * We really reached EOF (as opposed to an error)
	     * skip over the last line of the file (the function
	     * GetNewLine inserted a "quit" line).
	     */
	    HistIndex(CH) ++;
	    HistOffset(CH) = 0;
	}
	if (Current_IO->includeNesting <= level) {
	  break;
	} else {
	  continue;
	}
      }
    }
    /*
     *  We keep our include nesting in pace with the include nesting
     *  of the lines we are skipping in case we reach the end of file.
     *  In that case, we must finish reading that file and we must
     *  make sure that the nesting level of lines we are reading
     *  correspond to the nesting level of the file
     */
    Current_IO->includeNesting = active[HistIndex(CH)].nesting;
    if (Current_IO->includeNesting <= level) {
      break;
    } else {
      HistIndex(CH) ++;
      HistOffset(CH) = 0;
    }
  }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlIO_GetUninterpretedLine(outStr, doneStr, outLen):
 *    Get a line of input without interpreting variables.
 *    If doneStr is not NULL and the line contains the string doneStr
 *    then the function returns TRUE.  Otherwise it returns FALSE.
 *    The line does not include the string doneStr or a newLine.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
SHARED boolean_t itlIO_GetUninterpretedLine(outStr, doneStr, outLen)
  OUT char *outStr;
  IN char *doneStr;
  OUT int *outLen;
{
  int curLen = 0;
  char curChar;

  if (itlIO_HistoryLoc() == NULL) {
      assert(Hist_Offset == 0);
      if (GetNewLine(Hist_Index, TRUE)) {
	  if (outLen)
	      *outLen = 0;
	 return(TRUE);
      }
  }
  curChar = *itlIO_HistoryLoc();

  while ((curChar != EOLN) && (curChar != '\0')) {
      if (doneStr && *doneStr &&
	     (!strncmp(itlIO_HistoryLoc(), doneStr, strlen(doneStr)))) {
	/*
	 * Reached the end, skip over the done string
	 */
	itlIO_AdvanceIndex(strlen(doneStr));
	*outStr++ = EOLN;
	curLen++;
	*outStr = '\0';
	if (outLen)
	    *outLen = curLen;
	return(TRUE);
    }
    else {
	*outStr++ = curChar;
	curLen++;
	itlIO_AdvanceIndex(1);
    }
    curChar = *itlIO_HistoryLoc();
  }
  /*
   *  Do the appropriate things at the end of the line:
   *  Move the history pointer to the next line.
   */
  *outStr = '\0';
  Hist_Index++;
  Hist_Offset = 0;
  Line_Printed = FALSE;

  if (outLen)
      *outLen = curLen;
  return(FALSE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlIO_GetLine(outStr, doneStr, outLen):
 *    Get a line of input after interpreting variables.
 *    If doneStr is not NULL and the line contains the string doneStr
 *    then the function returns TRUE.  Otherwise it returns FALSE.
 *    The line does not include the string doneStr or a newLine.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT boolean_t itlIO_GetLine(outStr, doneStr, outLen)
OUT char *outStr;
IN char *doneStr;
OUT int *outLen;
{
  char *var_Name;
  itl_var_t *temp_Var;
  int curLen = 0;
  char curChar;

  if (itlIO_HistoryLoc() == NULL) {
      assert(Hist_Offset == 0);
      if (GetNewLine(Hist_Index, TRUE)) {
	if (outLen)
	    *outLen = 0;
	 return(TRUE);
    }
  }
  curChar = *itlIO_HistoryLoc();

  while ((curChar != EOLN) && (curChar != '\0')) {
    if (curChar == '$') {
	boolean_t err;
	/*
	 *  Skip over the $ sign, if the next character is a '('
	 *  then we are evaluating an expression, otherwise a variable.
	 */
	itlIO_AdvanceIndex(1);
	if (*itlIO_HistoryLoc() == '(') {
	    int len;
	    /*
	     *  We found a paren:  Leave it there, it will be parsed
	     *  by the call to itl_GetString which will also convert
	     *  the expression to a string for us.
	     */
	    char *tempStr = itl_GetString(&err, &len);
	    ITL_UTIL_MEMCOPY(tempStr, outStr, len);
	    outStr += len;
	    curLen += len;
	    ITL_FREE(tempStr);
	}
	else {
	    var_Name = itl_GetVariableName(&err, NULL);
	    temp_Var = itl_LocateVariable(var_Name);
	    if (!temp_Var) {
		/*
		 *  The variable was not found, its default value is 0.
		 *  We don't die since it's only an echo statement.
		 *  This is a warning and not an error.
		 *  ITL Does not handle errors properly at this level
		 *  since this routine is called directly from the input
		 *  reader.
		 */
		itl_Message("WARNING: Accessing a non-existent variable %s in an echo.\n",
			    var_Name);
		*outStr++ = '0';
		curLen++;
	    }   else {
		char *dotLoc = strchr(var_Name, '.');
		int len;
		char *tempStr;
		boolean_t valAlloc = FALSE;
		var_val_t *valP = VAR_VALUE_REC(temp_Var);

		if (dotLoc) {
		    valP = itlExp_GetNestedField(valP, dotLoc + 1);
		    if (!valP) {
			*outStr = '\0';
			if (outLen)
			    *outLen = 0;
			return(TRUE);
		    }
		    valAlloc = TRUE;
		}
		tempStr = itl_VarValueToString(valP, &len);
		ITL_UTIL_MEMCOPY(tempStr, outStr, len);
		outStr += len;
		curLen += len;
		if (valAlloc)
		    itl_FreeVarVal(valP);
		ITL_FREE(tempStr);
	    }
	    ITL_FREE(var_Name);
	}
    }
    else if (curChar == '@') {
	/*
	 *  Skip over the @ sign and replace it by a $ sign in the output
	 */
	itlIO_AdvanceIndex(1);
	*outStr++ = '$';
	curLen++;
    }
    else if (doneStr && *doneStr &&
	     (!strncmp(itlIO_HistoryLoc(), doneStr, strlen(doneStr)))) {
	/*
	 * Reached the end, skip over the done string
	 */
	itlIO_AdvanceIndex(strlen(doneStr));
	*outStr++ = EOLN;
	curLen++;
	*outStr = '\0';
	if (outLen)
	    *outLen = curLen;
	return(TRUE);
    }
    else {
	*outStr++ = curChar;
	curLen++;
	itlIO_AdvanceIndex(1);
    }
    curChar = *itlIO_HistoryLoc();
  }
  /*
   *  Do the appropriate things at the end of the line:
   *  Move the history pointer to the next line.
   */
  *outStr = '\0';
  Hist_Index++;
  Hist_Offset = 0;
  Line_Printed = FALSE;

  if (outLen)
      *outLen = curLen;
  return(FALSE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_PrintHist()
 *    print the max most recent commands in the history buffer
 *    If the buffer contains fewer than the desired number of commands
 *    it prints out the entire buffer.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itlIO_PrintHist(max)
int max;
{
    int i;
    int start = Hist_HistLen - max;

    if (start < 0) 
	start = 0;
    for (i = 0; (i < max) && (i < Hist_HistLen); i++) {
	itl_Message("%4d (%d): %s\n",
		    start + i,
		    Hist_Active[start + i].nesting,
		    Hist_Active[start + i].line);
    }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_LocalCommandLoop -
 *  Calls command loop directing it to use a local HistoryList instead
 *  of the global list.  Before returning, the routine deallocates
 *  any memory it allocated for the history buffer.  The history pointers
 *  of the caller are not changed.
 *
 *  This should be used when executing undo descriptions.
 *
 *  This routine starts a new history buffer.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT char *itl_LocalCommandLoop(promptStr)
char *promptStr;
{
  /*  initialize a whole new history array after storing away all
   *  the currently used values.
   */

  int i;
  char *retVal;
  itlIO_histLine_t *localHistory;
  pthread_mutex_t localHistoryLock;
  pthread_mutex_t localPromptLock;
  int localLastPrompt;
  char *localLastPromptStr;
  int *localHistLenP;
  itlIO_fileHandle_t *localFileHandleP = NULL;
  itlIO_history_t entryHist;
  entryHist = *Current_Hist;

  itl_Trace(ITL_IO_TRACE,
	    "LocalCommmand: Prev: Hist:0x%x, LenP:0x%x Index: %d, offset:%d\n",
	    Hist_Active,
	    Current_Hist->histLenP,
	    Hist_Index, Hist_Offset);
	       

  ITL_ALLOC(localHistLenP, int *, sizeof(int));
  *localHistLenP = 0;
  Current_Hist->histLenP = localHistLenP;
  ITL_ALLOC(localHistory, itlIO_histLine_t *,
	    MAX_LOCAL_HISTORY_LENGTH * sizeof(itlIO_histLine_t));
  Hist_MaxLen = MAX_LOCAL_HISTORY_LENGTH;
  Hist_Active = localHistory;
  pthread_mutex_init(&localHistoryLock, pthread_mutexattr_default);
  Hist_Lock = &localHistoryLock;
  pthread_mutex_init(&localPromptLock, pthread_mutexattr_default);
  Hist_PromptLock = &localPromptLock;
  Current_Hist->lastPromptP = &localLastPrompt;
  /*
   *  Initialize the stack of file handles to the empty stack
   */
  Current_Hist->fileHandles = &localFileHandleP;

  ITL_ALLOC(localLastPromptStr, char *, 5);
  sprintf(localLastPromptStr, " =: ");
  Current_Hist->lastPromptStrP = &localLastPromptStr;
  Hist_LastPrompt = -1;

  Hist_HistLen = 0;
  Hist_Index = 0;
  Hist_Offset = 0;

  localHistory[0].line = (char *)NULL;
  localHistory[0].len = 0;

  itl_Trace(ITL_IO_TRACE,
	    "LocalCommmand: New: Hist:0x%x, LenP:0x%x Index: %d, offset:%d\n",
	    Hist_Active,
	    Current_Hist->histLenP,
	    Hist_Index, Hist_Offset);

  TRY {
      retVal = itl_CommandLoop(promptStr);
  } FINALLY {
     /*
      *  Cleanup the history buffer.
      */
     for(i = 0; (i < MAX_LOCAL_HISTORY_LENGTH) && localHistory[i].line; i++) {
         itl_Trace(ITL_IO_TRACE,
   		"Local line[%d] = %s -- Freeing\n",
   		i, localHistory[i].line);
         ITL_FREE(localHistory[i].line);
     }
     ITL_FREE(localHistory);
     ITL_FREE(localHistLenP);
     ITL_FREE(localLastPromptStr);
     *Current_Hist = entryHist;
     pthread_mutex_destroy(&localHistoryLock);
     pthread_mutex_destroy(&localPromptLock);
  } ENDTRY;

  itl_Trace(ITL_IO_TRACE,
	    "LocalComm: Restored: Hist:%p, LenP:%p Index: %d, offset:%d\n",
	    Hist_Active,
	    Current_Hist->histLenP,
	    Hist_Index, Hist_Offset);

  return(retVal);
}
     

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunFromString
 *    itl_RunFromString is called with a procedure: a script in a string.
 *    The string is executed by a recursive call to the command loop.
 *    However, the command executed are not entered into the current
 *    history buffer, rather, this command calls LocalCommandLoop which
 *    starts its own history buffer which is also deallocated before the
 *    routine returns.
 *
 *    This routine returns a boolean, which is the boolean returned
 *    from itl_LocalCommandLoop.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT boolean_t itl_RunFromString(str_p)
IN char *str_p;
{
    boolean_t retVal;
    char *endWord;
    char *localIOString = str_p;
    itlIO_info_t entryIO;
    entryIO = *Current_IO;
  
    Current_IO->inputString = &localIOString;
    Current_IO->fileActive = FALSE;
    itl_SetInteractive(FALSE);
    itl_Trace(ITL_IO_TRACE,
	      "Running from a string:\n%s\n------------------------\n",
	      str_p);

    TRY {
	endWord = itl_LocalCommandLoop("");
    } FINALLY {
	*Current_IO = entryIO;
    } ENDTRY;
    /*
     *  Here we should verify that we ended with the right word.
     */
    retVal = !endWord;

    itl_Trace(ITL_VERBOSE_TRACE, "<== finished processing the string\n");
    return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunFromFile() -- 
 *   Run reading input from the given file.
 *  When the test program is instructed to run from a file
 *  it starts a recursive CommandLoop call.  Before starting
 *  the recursive command loop it stores relevant information
 *  and restores it after it finishes processing the file.
 *   
 *  The file is not opened at this time.  Rather, the file
 *  name is stored and the file is opened only when necessary.
 *  When a file is included inside a loop the include command
 *  is handled the same way in each iteration.  But, in the
 *  first iteration the history buffer is empty and the file
 *  is opened and read from.  In later iteration the file is
 *  cahced into the history buffer and therefore will not be
 *  opened or read.
 *
 *  If the file was opened, the flag Current_IO->fileOpened
 *  will be true and the file will be closed before returning
 *  from this routine.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT boolean_t itl_RunFromFile(fileName, PromptStr)
IN char *fileName;
IN char *PromptStr;
{
    boolean_t retVal;
    char *endWord;
    itlIO_history_t *CH = Current_Hist;
    int prevFileIndex;
    itlIO_info_t entryIO, *threadIO = Current_IO;
    entryIO = *threadIO;
  
    threadIO->fileActive = TRUE;
    threadIO->parentFile = threadIO->fileName;
    /*
     *  Copy the name locally, so that we can change it later to the
     *  actual file name if the file is not found in the current
     *  directory
     */
    ITL_UTIL_ALLOC(threadIO->fileName, char *, strlen(fileName)+1);
    ITL_UTIL_MEMCOPY(fileName, threadIO->fileName, strlen(fileName)+1);

    threadIO->openPermitted = TRUE;

    prevFileIndex = HistIndexStart(CH);
    HistIndexStart(CH) = HistIndex(CH) + 1;

    itl_SetInteractive(FALSE);
    threadIO->includeNesting ++;    
    
    itl_Trace(ITL_IO_TRACE,
	      "Running from the file %s\n",
	      threadIO->fileName);
   TRY {
      itlIO_histLine_t *active = HistActive(CH);
      int curIndex = HistIndex(CH);
      /*
       *  Check to see if the file is in the history buffer or not.
       *  1- If it is in the history buffer, run with the current
       *     history buffer.
       *  2- If it is not in the history buffer, include the file
       *     in its own history buffer.
       *  Note: if we are at the end of the history buffer,
       *     the following will read from the file and fill the buffer.
       */
      if (itlIO_AdvanceToNonWhite()) {
	endWord = NULL;
	goto tryEnd;
      }

      if ((HistIndex(CH) == curIndex) &&
	  (active[HistIndex(CH)].line[HistOffset(CH)] == '*')) {
	/*
	 *  We are still on the same line as the include command
	 *  and the next character is the comment character.
	 *  Skip over it.  We treat this special case otherwise
	 *  if the script contains a comment after the include command,
	 *  as in:   include foo.itl   *** This will initialize BAR
	 *  will suffer performance penalties by forcing the file
	 *  to be read again each time
	 */
	itl_CommentLine();
	if (itlIO_AdvanceToNonWhite()) {
	  endWord = NULL;
	  goto tryEnd;
	} 
      } 
      assert(active[HistIndex(CH)].line);
      if (active[HistIndex(CH)].nesting == threadIO->includeNesting) {
	endWord = itl_CommandLoop(PromptStr);
      } else {
	assert(active[HistIndex(CH)].nesting == (threadIO->includeNesting-1));
	/*
	 *  We already read from the history buffer - but we read the
	 *  wrong data.  Nevertheless, it reset our open-permitted
	 *  flag.  Before running the file in its own buffer, reset the
	 *  open permitted flag
	 */
	threadIO->openPermitted = TRUE;
	HistIndexStart(CH) = 0;
	endWord = itl_LocalCommandLoop(PromptStr);
      }
    tryEnd: ;
   } FINALLY {
      /*
       *  Free the local copy of the name
       */
      ITL_FREE(threadIO->fileName);
      /*  If the quit was detected in the middle of the file
       *  we must skip to the end of the file
       */
      itlIO_GotoIncludeNesting(threadIO->includeNesting - 1);

      HistIndexStart(CH) = prevFileIndex;

      /*
       *  The following automatically resets the earlyQuit flag
       */
      *threadIO = entryIO;
   } ENDTRY;
    
    /*
     *  Here we should verify that we ended with the right word.
     */
    retVal = !endWord;

    itl_SetStrResult(&currentFileName, entryIO.fileName);
    return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunFromInput():
 *     Similar to itl_RunFromFile but starts running from a
 *     standard input instead.
 *     This may be called from include files to read input
 *     interactively.  When running multithreaded this allows
 *     one thread to be interactive while another reads from
 *     a file.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT boolean_t itl_RunFromInput(promptStr)
IN char *promptStr;
{
    boolean_t retVal;
    char *endWord;
    itlIO_fileHandle_t *newHandleP;
    itlIO_info_t entryIO, *threadIO = Current_IO;
    entryIO = *threadIO;
  
    threadIO->fileActive = TRUE;
    threadIO->fileName = "";
    (*(threadIO->inputString)) = NULL;
    itl_SetInteractive(TRUE);
    
    /*
     *  simulate opening the file
     */
    ITL_NEW(newHandleP, itlIO_fileHandle_t);

    newHandleP->handle = stdin;
    ITL_ALLOC(newHandleP->fileName, char *, 6);
    ITL_MEMCOPY("stdin", newHandleP->fileName, 6);
    newHandleP->lineCount = 0;
    newHandleP->prevP = *(Current_Hist->fileHandles);
    newHandleP->includeNesting = Current_IO->includeNesting;
    *(Current_Hist->fileHandles) = newHandleP;
    
    TRY {
       endWord = itl_CommandLoop(promptStr);
   } FINALLY {
       *threadIO = entryIO;
   } ENDTRY;
    /*
     *  Here we should verify that we ended with the right word.
     */
    retVal = !endWord;
    
    return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_GotoBraceLevel
 *	Scan the rest of the buffer counting braces until the brace
 *	level gets down to the desired value.
 *	If the brace level is already at the desired level (or lower)
 *	this function returns without doing a thing and returns TRUE;
 *	If the parse error flag is SET, the function does nothing and
 *	returns TRUE.
 *	Otherwise, the function returns FALSE;
 */
EXPORT boolean_t itlIO_GotoBraceLevel(level, force, eofP)
  IN unsigned int level;
  IN boolean_t force;
  OUT boolean_t *eofP;
{
    itlIO_history_t *CH = Current_Hist;
    itlIO_info_t *curIO = Current_IO;
    int includeNesting = curIO->includeNesting;
    boolean_t eof;

    if (!eofP)
	eofP = &eof;
    *eofP = FALSE;

    if (itlIO_ParsingError()) {
	return(TRUE);
    }

    if (curIO->braceLevel < level) {
	return(TRUE);
    } else if ((curIO->braceLevel == level) && !force) {
	return(FALSE);
    }

    while(TRUE) {
	if (!HistActive(CH)[HistIndex(CH)].line) {
	    /*
	     *  The history buffer is empty, we need a new line
	     */
	    itl_PromptUser("looking for %d %c%s => ",
			   (curIO->braceLevel - level == 0 ? 1 :
			    curIO->braceLevel - level),
			   curIO->braceLevel - level == 0 ? '{' : '}',
			   curIO->braceLevel - level <= 1 ? "" : "'s");
	    if (*eofP = GetNewLine(HistIndex(CH), FALSE)) {
	      if (curIO->includeNesting > includeNesting) {
		/*
		 *  We are reading from a nested include file
		 */
		curIO->includeNesting --;
		continue;
	      } else {
		curIO->braceLevel = level;
		return(TRUE);
	      }
	    }
	} else {
	    char curChar;
	    boolean_t inString = FALSE;
	    /*
	     *  We have a line, Scan it.
	     */
	    while (curChar = HistActive(CH)[HistIndex(CH)].line[HistOffset(CH)]){
		/*
		 *  Skip over the character we just read and examine it.
		 */
		HistOffset(CH)++;
		switch(curChar) {
		  case '{':
		    if (!inString) {
			curIO->braceLevel++;
		    }
		    break;
		  case '}':
		    if (!inString) {
			if ((--(curIO->braceLevel)) <= level) {
			    return(curIO->braceLevel != level);
			}
		    }
		    break;
		  case '"':
		    inString = !inString;
		    break;
		  default:
		    break;
		}
	    }
	    /*
	     *  We did not find the desired level on this line.
	     *  Move to the next line.  While skipping the history
	     *  buffer we may need to read new lines from nested
	     *  included files.  In such case we need to read till
	     *  the EOF of the included file.  We must make sure that
	     *  all the lines we are skipping have the correct nesting
	     *  level, so we adjust our IO nesting level accordingly.
	     */
	    HistOffset(CH) = 0;
	    HistIndex(CH)++;
	    if (HistActive(CH)[HistIndex(CH)].line) {
		curIO->includeNesting = HistActive(CH)[HistIndex(CH)].nesting;
	    }
	}
    }
}
/*
 *  itlIO_SetIncludePath -
 *     set the search path for include files.
 */
SHARED void itlIO_SetIncludePath(dir)
  IN char *dir;
{
    /*
     *  Make sure the directory path does not have a / at the end,
     *  if it does, remove the /, we will add it when we append the file
     *  name.
     */
    if (dir[strlen(dir)-1] == '/') {
	dir[strlen(dir)-1] = '\0';
    }
    itl_SetStrVariable(itlIO_includePathVarName, dir, var_local);
}

/*
 * itlIO_InternalInit() -- 
 *    Initializes the internal IO module.  Needed for pthread 
 *    implementation so that thread data areas could be safely
 *    generated.
 *
 * ASSUMPTIONS:
 *    This routine is only called once, and non-concurrently via the
 *    itl_InitItl() function.
 *
 * RETURN CODES:
 *    FALSE - success
 *    TRUE  - error
 */
SHARED boolean_t itlIO_InternalInit()
{
    boolean_t returnVal = FALSE;

    pthread_mutex_init(&historyUpdateLock, pthread_mutexattr_default);
    pthread_mutex_init(&promptCountUpdateLock, pthread_mutexattr_default);

    ITL_ALLOC(lastPromptStr, char *, 5);
    sprintf(lastPromptStr, " =: ");
    global_lastPrompt = -1;

    /*
     *  Make sure the state of new threads is correct by registering
     *  with itl_AddThreadState
     */
    (void) itl_AddThreadState(InitializeITLThread,
			      StartITLThread,
			      CleanupITLThread);

    commandHistory[0].line = NULL;
    commandHistory[0].len = 0;

    if (pthread_keycreate(&ioDataKey, (pthread_destructor_t) free) != 0) 
    {
	/*
	 *  Use fprintf cause we are initializing, and ITL routines may
	 *  not be ready.
	 */
	fprintf(stderr, "Key creation failed in itl_InitInternalIO errno = %d\n", 
		  errno);
	returnVal = TRUE;
    }
    if (pthread_keycreate(&historyDataKey, (pthread_destructor_t) free) != 0) 
    {
	/*
	 *  Use fprintf cause we are initializing, and ITL routines may
	 *  not be ready.
	 */
	fprintf(stderr, "Key creation failed in itl_InitInternalIO errno = %d\n", 
		  errno);
	returnVal = TRUE;
    }
    return(returnVal);
}

/*
 *		The following were added temporarily
 *		for temporary tran-C support and should
 *		eventually be removed since we have REAL
 *		tran-C support.
 */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_GetThreadState-
 *	Return an opaque copy of the thread data to the client.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_GetThreadState(ptrP)
  OUT pointer_t *ptrP;
{
    char *buffer;
    int totalSize = (sizeof(itlIO_info_t) + sizeof(itlIO_history_t) + sizeof(int));
    int offset;

    ITL_ALLOC(buffer, char *, totalSize);

    ITL_MEMCOPY((pointer_t)&totalSize,
		(pointer_t)buffer,
		sizeof(int));
    offset = sizeof(int);

    ITL_MEMCOPY((pointer_t)Current_IO,
		(pointer_t)(buffer + offset),
		sizeof(itlIO_info_t));
    offset += sizeof(itlIO_info_t);

    ITL_MEMCOPY((pointer_t)Current_Hist,
		(pointer_t)(buffer + offset),
		sizeof(itlIO_history_t));
    offset += sizeof(itlIO_history_t);


    *ptrP = (pointer_t) buffer;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_RestoreThreadState
 *	Restore the state of the thread from a previous
 *	state gotten through a call to itl_GetThreadState.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_RestoreThreadState(ptrP)
  IN pointer_t *ptrP;
{
    char *buffer = (char *)*ptrP;
    int totalSize = sizeof(itlIO_info_t) + sizeof(itlIO_history_t) + sizeof(int);
    int actualSize, offset;

    ITL_MEMCOPY((pointer_t)buffer, (pointer_t)&actualSize, sizeof(int));
    assert(actualSize == totalSize);

    offset = sizeof(int);

    ITL_MEMCOPY((pointer_t)(buffer + offset),
		(pointer_t)Current_IO,
		sizeof(itlIO_info_t));
    offset += sizeof(itlIO_info_t);

    ITL_MEMCOPY((pointer_t)(buffer + offset),
		(pointer_t)Current_Hist,
		sizeof(itlIO_history_t));
    offset += sizeof(itlIO_history_t);
    assert(offset == actualSize);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_FreeThreadState
 *	Free the state of the thread received from a previous
 *	state gotten through a call to itl_GetThreadState.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_FreeThreadState(ptrP)
  IN pointer_t *ptrP;
{
    ITL_FREE(*ptrP);
}
