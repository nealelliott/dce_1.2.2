/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_io.h,v $
 * Revision 1.1.412.1  1996/10/17  18:27:25  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:45  damon]
 *
 * Revision 1.1.407.1  1994/02/04  20:45:27  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:33  devsrc]
 * 
 * Revision 1.1.405.1  1993/12/07  17:44:42  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:27:24  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:31:05  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:03  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:16:39  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	  Selected comments:
 * 	    Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	    Initial checkin.
 * 	Transarc delta: fred-fix-itl-bugs-8-20-92 1.1
 * 	  Selected comments:
 * 	    Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	    Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	    succeeds that was expected to fail.
 * 	    Added the verifyFileError command to toggle file system function verification.
 * 	    (The code was already there I just was not making it available.)
 * 	    Ofer bug fixes.
 * 	[1992/09/23  21:24:56  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_io.h,v 1.1.412.1 1996/10/17 18:27:25 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    extern   void
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $TALog: itl_io.h,v $
 * Revision 1.8  1994/11/01  22:29:42  cfe
 * Bring over the changes that the OSF made in going from their DCE 1.0.3
 * release to their DCE 1.1 release.
 * [from r1.6 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 *
 * Revision 1.6  1993/03/08  13:51:58  fred
 * To import fixes to ITL from Ofer.  Also, fixed some outstanding minor
 * bugs we found -
 *  - The spoke (distributed ITL) had a bug in it's sync point code.  It was
 *    not removing a variable from the hash table, which caused confusion when
 *    a sync point name was re-used in a tight loop.  Also, it was not
 *    setting the timeout field correctly during sync point creation.
 *
 *  - Remove the dependency of the ITL utilities library on the com_err
 *    library.  I wrote our own error handling function that deals with both
 *    dce and errno correctly.  Besides, the error_message() function in the
 *    com_err library is not thread-safe.
 *
 * Incorporation of Encina ITL fixes.
 * [from r1.5 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 *
 * Revision 1.5  1993/01/14  17:38:23  jaffe
 * revdrop of latest OSF copyright changes to these files
 * [from r1.4 by delta osf-revdrop-01-12-92, r1.1]
 *
 * Revision 1.2  1992/08/20  19:37:45  fred
 * Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * succeeds that was expected to fail.
 * Added the verifyFileError command to toggle file system function verification.
 * (The code was already there I just was not making it available.)
 *
 * Ofer bug fixes.
 * [from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 *
 * Revision 1.1  1992/08/10  20:11:13  fred
 * Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * Initial checkin.
 *
 * Initial checkin.
 * [added by delta fred-checkin-itl, revision 1.1]
 *
 * Revision 1.19  1992/03/27  21:15:52  oz
 * Changes which were made before 1.0.1 and were not supposed to be
 * incorporated in 1.0.1 and therefore were not readied and were
 * not moved to CML with the rest of the file.
 *
 * These changes add tranC support for ITL as well as handling
 * transaction aborts (exceptions) in the lower level io modules.
 *
 * See General Comment Above
 * [from revision 1.18 by delta oz-2172-itl-support-tranc, revision 1.1]
 *
 * Revision 1.16  1992/02/20  16:27:33  oz
 * 	- New variable: itlIO_CreateIndThreadP
 * 	[92/02/19  16:50:35  oz]
 *
 * Revision 1.15  92/02/07  11:23:38  oz
 * 	 - Four functions moved to itl_io:
 * 	   coLoop, loop, exec, and threadExec.
 * 	[92/02/06  13:16:32  oz]
 * 
 * Revision 1.14  92/02/04  22:17:22  oz
 * 	- new variable: itlIO_CreateIndThreadP
 * 	[92/02/04  14:04:57  oz]
 * 
 * Revision 1.13  92/02/02  09:22:48  oz
 * 	- gotoBraceLevel takes an additional parameter: force
 * 	[92/01/31  10:49:19  oz]
 * 
 * Revision 1.12  92/01/28  23:24:13  oz
 * 	 - New command: itlIO_TraceCurrentLocation
 * 
 * Revision 1.11  92/01/23  18:13:06  oz
 * 	 - new function:
 * 		itlIO_GotoBraceLevel
 * 	 - new Command:
 * 		ITL_CLOSE_BRACE_CMD
 * 		ITL_OPEN_BRACE_CMD
 * 	[92/01/23  17:09:39  oz]
 * 
 * 	- New Function:
 * 		itlIO_BraceLevel
 * 	- Corrected prototypes for functions with no parameters.
 * 	[92/01/23  11:37:52  oz]
 * 
 * Revision 1.10  92/01/02  09:58:36  oz
 * 	 - Changed the name of the ifThenElse command to if
 * 	[91/12/26  09:54:30  oz]
 * 
 * Revision 1.9  91/12/24  14:16:25  oz
 * 	 - Added an if-then-else command
 * 	[91/12/24  14:14:53  oz]
 * 
 * Revision 1.8  91/12/12  13:55:48  oz
 * 	 - New script command: dumpIOData
 * 	[91/12/10  20:12:31  oz]
 * 
 * 	 - new functionL itlIO_DumpIOState
 * 	[91/12/10  19:42:16  oz]
 * 
 * Revision 1.7  91/10/21  15:06:50  oz
 * 	New Command: randomSeed
 * 
 * Revision 1.6  91/10/17  16:19:33  fred
 * 	Added include of itl_private.h, because of the use of LATCH_TYPE.
 *
 * Revision 1.5  91/10/17  08:39:25  fred
 * 	Added the itlIO_GetFilename() function.
 * 
 * Revision 1.4  91/10/16  19:05:23  oz
 * 	Moved itlIO_SkipWhites() to itl.h
 * 
 * Revision 1.3  91/10/03  12:55:58  oz
 * 	New Command: itlIO_NestingLevel()
 * 
 * Revision 1.2  91/09/12  17:57:06  oz
 * 	Changes of the REC tester to make a general itl tool.
 * 	Many cosmetic name changes to conform to transarc standard.
 * 	This file should contain all the external declarations exported
 * 	by the routine itl_io.c
 * 	Most of these are external declarations of routines prefixed by itlIO_
 * 
 * Revision 1.1  91/08/22  17:21:33  mss
 * 	Modified REC tester to make separate library
 * 
 * Revision 1.1  91/08/08  18:32:24  oz
 * 	Initial Checkin.
 * 
 */
/*
 * itl_io.h -- 
 *    The export files for the IO parsing routines of the itl utility.
 */ 

#ifndef ITL_IO_H
#define ITL_IO_H

#include <utils/itl/itl_private.h>   /* for LATCH_TYPE */

/*
 *  WhiteSpaces is a string containing the white characters
 *  AlphaNumChars is the set of alpha numeric characters
 */
extern char *WhiteSpaces;
extern char *AlphaNumChars;
extern int  itl_LastSignalReceived;
extern int  itl_coLoopStackSize;

/*
 *	itlIO_CreateIndThreadP
 *  A pointer to a function called to start an independent thread.
 *  Normally, this will point to a local function that will call
 *  pthread_create.  However, when running under tran-C this should
 *  be changed to point to the tran-C function concThread
 */
extern void (*itlIO_CreateIndThreadP) _TAKES((
					      IN void (*fn)(void*),
					      IN void *arg));

/*
 *	itlIO_CreateIndThreadP
 *  A pointer to a function called to start several synchronous
 *  threads.
 *  Normally, this will point to a local function that will call
 *  pthread_create several times and will then join with all the
 *  threads.  However, when running under tran-C this should
 *  be changed to point to the tran-C function cofor
 */
extern void (*itlIO_CreateSeveralThreadsP) _TAKES((
					   IN void (*fn)(pointer_t),
					   IN int numThreads,
					   IN pointer_t *argList));

#define HIST_CHARACTER '!'

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_LastPromptStrP
 *	Return the pointer to the last prompt string (char **)
 */
extern char **itlIO_LastPromptStrP _TAKES((void));


/*
 *  itlIO_DumpIoState:
 *	Dump the IO state for the thread.
 */
extern void itlIO_DumpIOState _TAKES((  IN FILE *outFile )) ;

/*
 *  itlIO_TraceCurrentLocation
 *	Print the current location in the history buffer as
 *	well as the contents of the current line to Cur_Err
 */
extern void itlIO_TraceCurrentLocation _TAKES((void));

/*
 *    itlIO_EarlyQuit()
 *  Was an early quit detected?
 */
extern boolean_t itlIO_EarlyQuit _TAKES((void));

/*
 *    itlIO_SetEarlyQuit()
 *  Set the flag indicating whether a quit was detected or not.
 */
extern void itlIO_SetEarlyQuit _TAKES((IN boolean_t flag));

/*
 *  itlIO_Cleanup
 *	Called before termination to cleanup the itl_IO module
 */
extern void itlIO_Cleanup _TAKES((void));

/*
 *	itlIO_NestingLevel()
 *	Return the current nesting level for this thread.
 *	The nesting level is set automatically when the thread
 *	start a loop or a coLoop.
 */
extern unsigned int itlIO_NestingLevel _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *	itlIO_IncNestingLevel()
 *	Increment the nesting level by a specified ammount and return
 *	the new value.
 */
extern unsigned int itlIO_IncNestingLevel _TAKES((IN int num));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *	itlIO_BraceLevel()
 *	Return the current brace nesting level for this thread.
 *	When transactions are started with tran-C the code to
 *	be executed in the transaction is enclosed in Braces.
 *	When the transaction is aborted, it may need to jump to
 *	the corresponding brace.
 */
extern unsigned int itlIO_BraceLevel _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_IncBraceLevel
 *	Increment the current brace level
 *	Return the new brace level.
 */
extern int itlIO_IncBraceLevel _TAKES((IN int num));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_GotoBraceLevel
 *	Scan the rest of the buffer counting braces until the brace
 *	level gets down to the desired value.
 *	If the brace level is already at the desired level (or lower)
 *	this function returns without doing a thing and returns TRUE;
 *	Otherwise, the function returns FALSE;
 *
 *	If the parameter 'force' is TRUE and the current brace level
 *	is equal to the desired level, the call will first look for
 *	a '{', increase the level, and then look for the matchin end '}'.
 */
extern boolean_t itlIO_GotoBraceLevel _TAKES((
					      IN  unsigned int level,
					      IN  boolean_t force,
					      OUT boolean_t *eofP
					      ));


/*
 *  itlIO_AdvanceToNonWhite():
 *     Advance the pointer into the history buffer skipping white
 *     spaces and reading from input if necessary.
 *     This routine repeatedly calls itlIO_SkipWhites - when skipWhites
 *     return TRUE indicating that the end of the history buffer is reached
 *     this routine reads a new line of input into the history buffer by
 *     calling IO_GetNewLine.
 */
extern boolean_t itlIO_AdvanceToNonWhite _TAKES((void));

/*
 *  itlIO_AdvanceIndex(int):
 *     Advance the history index a specified amount in the current line.
 */
extern void itlIO_AdvanceIndex _TAKES((IN int len));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *  itlIO_AdvanceLine
 *	Advance the history buffer one line
 */
extern void itlIO_AdvanceLine _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_CurrentHistLoc:
 *    Return the current location in the history buffer (index, offset)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
extern void itlIO_CurrentHistLoc _TAKES((OUT int *indexP,
					 OUT int *offsetP));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itlIO_GotoHistLoc:
 *    Goto a specified location in the history buffer (index, offset)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
extern void itlIO_GotoHistLoc _TAKES((IN int index,
				      IN int offset));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_ProcessComments():
 *	
 *    Skip over all white space and comments processing the comments
 *    that need to be processed (e.g., lines that begin
 *    with !* maybe typed out).
 *
 *    This routine is called by itl_GetCommand before it calls
 *    itl_GetWord to get the actual commands.  It is possible, though
 *    may have some undesirable effects to always call this routine before
 *    calling itl_GetWord:  this will enable comments between parameters,
 *    but may restrict the type of allowable words.
 *
 *    It should be safe to do this and to surround by double quotes any
 *    word that begins with a * or a | or a !.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itlIO_ProcessComments _TAKES((
					       IN char *promptStr));

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
extern boolean_t itlIO_GetLine _TAKES((
				       OUT char *outStr,
				       IN char *doneStr,
				       OUT int *outLen));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_PrintHist()
 *    print the max most recent commands in the history buffer
 *    If the buffer contains fewer than the desired number of commands
 *    it prints out the entire buffer.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itlIO_PrintHist _TAKES((IN int max));



/************************************************************************
 ************************************************************************
 ********      
 ********      
 ********      Intermediate level routines for parsing the history buffer.
 ********      
 ********      
 ************************************************************************
 ************************************************************************
 */

/*
 * itlIO_WordFromString()
 *    given a string it returns the first word in the string and the
 *    length of that word.  The delims specifies what a word means.
 *       if flag==TRUE then a word consists of consecutive letters
 *       NOT IN delims.
 *    Otherwise, a word consists of consecutive letters IN delims.
 * 
 *    The word is allocated dynamically and it is up to the caller
 *    to free the space when it is no longer necessary.
 *    This routine returns a null terminated word - However, the length
 *    returned is the actual length of the word and does not include
 *    the terminating NULL character.
 *
 */
extern char *itlIO_WordFromString _TAKES((
					  IN char *str,
					  IN char *delims,
					  IN boolean_t flag,
					  OUT int *lenP));
/*
 *   itl_GetRawWord()  
 *    return one word from the current location in the history buffer.
 *    This is the basic routine for reading from the history buffer.
 *    The word is allocated dynamically and it is up to the caller to
 *    free the space when it is no longer needed.
 *    The word is a null terminated string; lenP returns the length
 *    of the word not including the terminating null.
 */
extern char *itl_GetRawWord _TAKES((
				    OUT boolean_t *eofP,
				    OUT int *lenP
				    ));


/*
 *   itl_GetAlphaWord()  
 *    return one word from the current location in the history buffer.
 *    The word returned comprises solely of alpha numeric characters.
 *    (This may be used for variable names.)
 */
extern char *itl_GetAlphaWord _TAKES((
				      OUT boolean_t *eofP,
				      OUT int *lenP
				      ));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *  itl_GetQuotedString:
 *     Get a quoted string: a string surrounded by '"'s:
 *  It is assumed that the first " has already been read.  It reads
 *  a string and terminates at the first ".
 *  The string is not allowed to contain new lines.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
extern char *itl_GetQuotedString _TAKES((
					 OUT boolean_t *err,
					 INOUT int *outLen
					 ));

/*
 *  itlIO_DefaultScope():
 *     Return the default scope for variables
 */
extern scope_t itlIO_DefaultScope _TAKES((void));

/*
 *  itlIO_SetDefaultScope()
 *     set the default scope for variables.
 *     The default scope is set to LOCAL in all threads but the main thread.
 */
extern void itlIO_SetDefaultScope _TAKES((IN scope_t scope));

/*
 *  itlIO_ParsingError - return whether an IO parsing error has occurred.
 *  When such an error occurs, the routine itlIO_SetErrorStatus(TRUE)
 *  is called.
 */
extern boolean_t itlIO_ParsingError _TAKES((void));

/*
 *  itlIO_GetFilename()
 *    Returns the name of the current input file.
 */
extern char *itlIO_GetFilename _TAKES((
				       void
				       ));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlIO_startLooping:  Loop through a portion of the history buffer
 *    VarName is the name of the loop variable (a script variable)
 *    startVal: initial value of VarName
 *    incVal: the increment each time through the loop
 *    MaxVal: The value should not exceed MaxVal
 *
 *    When the variable multiThreaded is set, all the iterations (except
 *    the first) are executed concurrently, each by one thread.
 *
 *
 *       WARNING:  THE LOOP IS ALWAYS EXECUTED AT LEAST ONCE!
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itlIO_StartLooping _TAKES((
					    IN char *varName,
					    IN int startVal,
					    IN int incVal,
					    IN int maxVal,
					    IN boolean_t multiThreaded));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_HandleLoopCommand
 *	Handle the looping script commands:
 */
extern boolean_t itlIO_HandleLoopCommand _TAKES((IN int multiThreaded));

#define ITL_COLOOP_CMD                           \
{"coLoop", itlIO_HandleLoopCommand, TRUE, FALSE, \
     COMMAND_MODE_ALL,                           \
     "loop executing each iteration in a thread",\
     "misc",                                     \
     ""}

#define ITL_LOOP_CMD                           \
{"loop", itlIO_HandleLoopCommand, FALSE, FALSE,\
     COMMAND_MODE_ALL,                         \
     "start looping.",                         \
     "misc",                                   \
     ""}

#define ITL_EXEC_CMD                    \
{"exec", itlIO_HandleExecProc, 0, FALSE,\
     COMMAND_MODE_ALL,                  \
     "execute a procedure.",            \
     "misc",                            \
     ""}

#define ITL_THREAD_EXEC_CMD                          \
{"threadExec", itlIO_HandleExecProc, 1, FALSE,       \
     COMMAND_MODE_ALL,                               \
     "execute a procedure in an independent thread.",\
     "misc",                                         \
     ""}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_HandleExecProc
 *	Execute a script procedure either in the same thread or
 *	in an indepedent thread.
 */
extern boolean_t itlIO_HandleExecProc _TAKES((IN int param));



/*
 *  itlIO_SetIncludePath -
 *     set the search path for include files.
 */
extern void itlIO_SetIncludePath _TAKES((IN char *dir));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  Handle the 'repeat' - 'while' command
 *
 *  Logic:  Repeat simply stores the current location in the
 *          history buffer and calls command loop recursively.
 *          When the while command is encountered the commandLoop
 *          returns withought reading the condition after the while
 *          loop, thus, when the command Loop returns to this routine,
 *          the condition is read and checked.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_RepeatCommand _TAKES((IN int param));

#define ITL_IO_REPEAT_CMD                                    \
{"repeat",  itl_RepeatCommand, 0, FALSE,                     \
     COMMAND_MODE_ALL,                                       \
     "repeat the section of code to the corresponding while",\
     "misc",                                                 \
     ""}

#define ITL_IO_DO_CMD                                        \
{"do",  itl_RepeatCommand, 0, FALSE,                         \
     COMMAND_MODE_ALL,                                       \
     "repeat the section of code to the corresponding while",\
     "misc",                                                 \
     ""}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  Braces:
 *	The user can use braces to start a recursive invocation
 *	of the ITL test prgram, though there is no real use for
 *	it now.  The open command is provided because it is free.
 *	Some commands, such as if-then-else require open '{'
 *	and expect to be ended by a '}' and that is why the
 *	close brace command is provided.  It simply causes
 *	an invocation of the command loop to end.
 */

extern boolean_t HandleBraceCmd _TAKES((IN int param));
#define ITL_CLOSE_BRACE_CMD                  \
{"}", HandleBraceCmd, FALSE, TRUE,           \
     COMMAND_MODE_ALL,                       \
     "Return from a recursive brace nesting",\
     "misc",                                 \
     ""}

#define ITL_OPEN_BRACE_CMD             \
{"}", HandleBraceCmd, TRUE, TRUE,      \
     COMMAND_MODE_ALL,                 \
     "Start a recursive brace nesting",\
     "misc",                           \
     ""}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  Handle the 'if-then-else' command
 *
 *  Logic:
 *	The syntax:
 *		if cond {
 *		   .
 *		   .
 *		   .
 *		}
 *		
 *	The else part is optional:
 *		else {
 *		   .
 *		   .
 *		   .
 *		}
 *	The curly braces are REQUIRED and must be surrounded by
 *	white spaces.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t HandleIfThenElse _TAKES(( IN int param ));

#define ITL_IF_THEN_ELSE_CMD       \
{"if",  HandleIfThenElse, 0, FALSE,\
     COMMAND_MODE_ALL,             \
     "An if then else construct",  \
     "misc",                       \
     ""}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  Dump the IO state of the test program:
 *	Provide information about the current IO thread info.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t HandleDumpIOState();

#define ITL_IO_DUMP_DATA_CMD                          \
     {"dumpIOData", HandleDumpIOState, FALSE, FALSE,  \
	  COMMAND_MODE_ALL,                           \
	  "print out the current state of the thread",\
	  "misc",                                     \
	  ""}


/*
 *  IO_HandleSetPath
 *  The default command to set the unclude path.
 */
extern boolean_t IO_HandleSetPath _TAKES((IN int param));

#define IO_PATH_ENTRY                           \
     {"path", IO_HandleSetPath, 0, FALSE,       \
	  COMMAND_MODE_ALL,                     \
	  "set a search path for include files",\
	  "misc",                               \
	  ""}


/*
 *  IO_HandleIncludeFile - Param:
 *    perform the "include" command: Prompt  for a file name and direct
 *    the itl program to recieve input from that file.
 *    0: prompt for a file and read from that file
 *    1: read from stdin.
 */
extern boolean_t IO_HandleIncludeFile _TAKES((IN int param));

#define IO_INCLUDE_COMMAND                        \
      {"include",  IO_HandleIncludeFile, 0, FALSE,\
	   COMMAND_MODE_ALL,                      \
	   "read commands from a specified file.",\
	   "misc",                                \
	   ""}

/*
 *  randomSeed command:
 *	Set the seed for the random number generator
 */
extern boolean_t itlIO_RandomSeed _TAKES((IN int param));

#define IO_RANDOM_SEED_COMMAND                             \
      {"randomSeed",  itlIO_RandomSeed, 0, FALSE,          \
	   COMMAND_MODE_ALL,                               \
	   "Set the seed for the random number generator.",\
	   "misc",                                         \
	   ""}


/*
 *  IO_HandleHistoryCommand
 *     Display the last n lines in the history buffer.
 */
extern boolean_t itlIO_HandleHistoryCommand _TAKES((IN int param));

#define IO_HISTORY_COMMAND                            \
     {"history", itlIO_HandleHistoryCommand, 0, FALSE,\
	  COMMAND_MODE_ALL,                           \
	  "type the last n input lines\n\tThe number of lines is taken from the script variable 'history'",\
	  "misc",                                     \
	  ""}


/*
 *  itlIO_MoveToEndOfBuffer
 *     move to the end of the history buffer.
 *     This routine should be called when returning to interactive mode after
 *     an error is encountered in the middle of a file, or a loop.
 */
extern void itlIO_MoveToEndOfBuffer _TAKES((void));

/* global history variables */
#define MAX_HISTORY_LENGTH        22000
#define MAX_LOCAL_HISTORY_LENGTH  4500
extern int histLen;

extern LATCH_TYPE threadBlockMutex;

/*
 *  itlExp_Cleanup: Cleanup the itlExp module before exitting.
 */
extern void itlExp_Cleanup _TAKES((void));

extern boolean_t itlIO_Init _TAKES((void));

#endif  /* ITL_IO_H */
