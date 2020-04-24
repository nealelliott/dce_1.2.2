/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_io.c,v 1.1.111.1 1996/10/17 18:27:20 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    SHARED   HandleDumpIOState, itlIO_TraceCurrentLocation, 
 *             itlIO_ProcessComments, HandleBraceCmd, 
 *             itlIO_HandleLoopCommand, itlIO_HandleExecProc
 *    
 *    EXPORT   itlIO_DumpIOState, itlIO_IsInterface, itl_PrintAdviseP, 
 *             itl_SetInteractive, itlIO_DefaultScope, itl_SetErrorStatus, 
 *             itlIO_SetDefaultScope, itlIO_SetIncludePath, itl_Message, 
 *             itl_fMessage, itl_AdviseUser, itl_fAdviseUser, itl_Trace, 
 *             itl_fTrace, itl_PrintError, itl_fPrintError, itl_PromptUser, 
 *             itl_fPromptUser, itlIO_PrintPrompt, itlIO_DonePrompting, 
 *             itlIO_SkipWhites, itlIO_MoveToEndOfBuffer, itl_GetCharacter, 
 *             itl_EchoLine, itlIO_GetLine, itlIO_PrintHist, 
 *             itl_RunFromString, itl_RunFromFile, itl_RunFromInput, 
 *             itl_AddThreadState, HandleIfThenElse, itl_RepeatCommand, 
 *             itlIO_StartLooping, itlIO_MatchBrace, itlIO_GotoBraceLevel, 
 *             itl_OptionalClause, itl_StartIndependentThread, 
 *             itl_RunStringInThread, itl_StartServerThread, 
 *             itl_GetThreadState, itl_RestoreThreadState, 
 *             itl_FreeThreadState, itl_InitIO, IO_HandleIncludeFile, 
 *             itlIO_RandomSeed, itlIO_HandleHistoryCommand
 *    
 *    PRIVATE  PrintAMessage, PromptUser, OpenFileOnDir, OpenCurrentFile, 
 *             GetNewLine, itlIO_AdvanceLine, itlIO_GetUninterpretedLine, 
 *             PerformThreadInit, PerformThreadStart, PerformThreadCleanup, 
 *             PerformCoIteration, StartCoLooping, StartLooping, 
 *             CreateIndThread, CreateSeveralThreads, IndependentThread, 
 *             StrThread, ServerThread
 *    
 *    extern   void, itlIO_AdvanceToNonWhite
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1991,1992,1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_io.c,v $
 * Revision 1.1.111.1  1996/10/17  18:27:20  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:42  damon]
 *
 * Revision 1.1.106.1  1994/02/04  20:45:23  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:31  devsrc]
 * 
 * Revision 1.1.104.1  1993/12/07  17:44:37  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:27:03  jaffe]
 * 
 * Revision 1.1.2.6  1993/01/11  18:30:57  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:09:52  htf]
 * 
 * Revision 1.1.2.5  1992/12/09  21:32:47  jaffe
 * 	Transarc delta: fred-ot-6193-move-procid-to-base-itl 1.1
 * 	  Selected comments:
 * 	    To make the process id and system page size variables available to all test
 * 	    programs but placing their initialization in the ITL library and not the
 * 	    itlFile library.
 * 	    Also incorporated fixes to allow parameters to be passed to procedures
 * 	    started in detached threads.
 * 	    Fix PromptUser to work with procedures running in detached threads.
 * 	[1992/12/07  13:34:04  jaffe]
 * 
 * Revision 1.1.2.4  1992/11/18  21:59:56  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Changes for procedure parameter passing.
 * 	[1992/11/18  15:06:46  jaffe]
 * 
 * Revision 1.1.2.3  1992/10/28  17:58:04  jaffe
 * 	Transarc delta: fred-itl-ofer-fix-loop-incr 1.1
 * 	  Selected comments:
 * 	    To include Ofer's fix to loop increment values.
 * 	    Check for a 0 increment value in itlIO_StartLooping().
 * 	Transarc delta: fred-itl-ofer-fixes-9-24-92 1.1
 * 	  Selected comments:
 * 	    Include some minor fixes made to the TP ITL.
 * 	    Fixed two memory leaks.
 * 	[1992/10/28  12:33:54  jaffe]
 * 
 * 	@TRANSARC_COPYRIGHT@
 * 
 * 	$TALog: itl_io.c,v $
 * 	Revision 1.28  1994/11/01  22:29:37  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.25 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.25  1993/06/03  19:58:44  shl
 * 	[merge of changes from 1.18 to 1.19 into 1.24]
 * 
 * Revision 1.19  1993/04/21  19:12:45  srinivas
 * 	changes for solaris
 * 
 * 	changes for solaris
 * 	[from r1.18 by delta srinivas-itl-1, r1.1]
 * 
 * Revision 1.24  1993/06/03  18:32:04  shl
 * 	[merge of changes from 1.19 to 1.21 into 1.20]
 * 
 * Revision 1.21  1993/06/03  12:50:11  fred
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
 * 	Use srand48/lrand48 and not srandom/random.
 * 	[from r1.19 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.20  1993/05/21  19:20:33  fred
 * 	To import ITL fixes from the Encina group that either fix problems we
 * 	have, or would impact the changes I made to the shellExec command.
 * 
 * 	Note:
 * 	    By accident I made changes in this delta that I should have made in
 * 	    fred-db3575-itl-spoke-thread-safeness-fixes.  So this delta and that
 * 	    one rely on one another.
 * 
 * 	Encina ITL fixes.
 * 	[from r1.18 by delta fred-db3579-encina-itl-fixes-for-shell, r1.1]
 * 
 * Revision 1.18  1993/03/08  13:51:56  fred
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
 * 	[from r1.17 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.17  1993/03/01  14:48:24  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.16 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 6.2  1993/01/28  22:16:27  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Conditionalize the explicit extern of 'srand()' to avoid conflicts
 * 	with gcc 'stdlib.h' declarations. Cast call of srand() to 'void'.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * Revision 1.16  1993/01/14  20:05:42  jaffe
 * 	[merge of changes from 1.12 to 1.15 into 1.13]
 * 
 * Revision 1.15  1993/01/14  17:38:17  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.12 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * 	Revision 1.11  1992/11/12  16:35:45  mcinerny
 * 	[merge of changes from 1.7 to 1.10 into 1.8]
 * 
 * Revision 1.10  1992/11/10  19:49:11  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.7 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.8  1992/11/03  15:27:35  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Changes for procedure parameter passing.
 * 	[from revision 1.7 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.7  1992/10/14  11:28:56  mcinerny
 * 	[merge of changes from 1.3 to 1.6 into 1.5]
 * 
 * Revision 1.6  1992/10/13  18:19:29  fred
 * 	To include Ofer's fix to loop increment values.
 * 
 * 	Check for a 0 increment value in itlIO_StartLooping().
 * 	[from revision 1.3 by delta fred-itl-ofer-fix-loop-incr, r1.1]
 * 
 * Revision 1.5  1992/10/05  16:14:13  jdp
 * 	[merge of changes from 1.2 to 1.4 into 1.3]
 * 
 * Revision 1.4  1992/10/02  20:32:03  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.2 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.3  1992/09/24  15:23:25  fred
 * 	Include some minor fixes made to the TP ITL.
 * 
 * 	Fixed two memory leaks.
 * 	[from revision 1.2 by delta fred-itl-ofer-fixes-9-24-92, revision 1.1]
 * 
 * Revision 1.2  1992/08/20  19:37:40  fred
 * 	Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	succeeds that was expected to fail.
 * 	Added the verifyFileError command to toggle file system function verification.
 * 	(The code was already there I just was not making it available.)
 * 
 * 	Ofer bug fixes.
 * 	[from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:08  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.62  1992/06/12  16:54:05  oz
 * 	itl_GetQuotedString returned the wrong length.
 * 
 * 	itl_GetQuotedString incorrectly returned the number of characters
 * 	it read (which is wrong when there are quoted characters.
 * 	[from revision 1.60 by delta oz-3204-ITL-correct-returned-len-in-quotesString, revision 1.1]
 * 
 * Revision 1.60  1992/06/06  17:36:47  oz
 * 	Allow the user to insert a '"' or a newline inside a string
 * 
 * 	Changed the routine: itl_GetQuotedString.
 * 	It now scans one letter at a time and deals with it.
 * 	[from revision 1.59 by delta oz-3064-allow-special-chars-in-strings, revision 1.1]
 * 
 * Revision 1.59  1992/05/18  17:36:50  oz
 * 	Corrected a forgotten comment
 * 
 * 	[from revision 1.58 by delta oz-2576:ITL:Do-not-abort-top-level-tran-on-parse-error, revision 1.2]
 * 
 * Revision 1.58  1992/05/12  19:53:17  oz
 * 	When a parse error occurs, ITL aborts the current transaction even
 * 	if the test is in interactive mode and the transaction is a top
 * 	level transaction.  This is not needed.
 * 	This change prevents a tansaction in the top most interactive
 * 	level from being aborted as a result of a parse error.
 * 
 * 	The function itl_OptionalClause neither increments nor
 * 	decrements the itlIO_NestingLevel
 * 	[from revision 1.57 by delta oz-2576:ITL:Do-not-abort-top-level-tran-on-parse-error, revision 1.1]
 * 
 * Revision 1.57  1992/04/29  13:37:16  oz
 * 	itl_SetErrorStatus must be called only after the rest of the module
 * 	was initialized.
 * 	[from revision 1.56 by delta oz-2517-ITL:Initialize-parse-error-var, revision 1.2]
 * 
 * Revision 1.56  1992/04/28  21:17:34  oz
 * 	ITL by default takes the global value of a local variable if the
 * 	thread does not have a local value.  Since the ErrorStatus is not set,
 * 	if any thread sets the flag to TRUE, all the threads will see the
 * 	value (in the global value), which is an error.  By setting it to
 * 	FALSE at initialization the defaul value is FALSE for any new thread,
 * 	as it should be.
 * 
 * 	Call itl_SetErrorStatus(FALSE) in itl_InitIO()
 * 	[from revision 1.54 by delta oz-2517-ITL:Initialize-parse-error-var, revision 1.1]
 * 
 * Revision 1.54  1992/04/06  21:50:28  oz
 * 	Additional minor fixes.
 * 
 * 	optionalClause did not increment/decrement the nesting level correctly.
 * 	optionalClause does nothing if IO-Error is TRUE
 * 	[from revision 1.53 by delta oz-2278:Errors-in-optional-clause, revision 1.2]
 * 
 * Revision 1.53  1992/04/06  20:31:33  oz
 * 	Optional clause now inclrements the nesting level to make the called
 * 	command loop to abort on error.
 * 
 * 	Optional clause now inclrements the nesting level to make the called
 * 	command loop to abort on error.
 * 	[from revision 1.52 by delta oz-2278:Errors-in-optional-clause, revision 1.1]
 * 
 * Revision 1.52  1992/04/02  14:49:09  oz
 * 	  Continued developlment on the access to recursive access.
 * 	  fetch&Op works properly now.
 * 	  test&Set still doesn't
 * 
 * 	  Added missing return statement
 * 	[from revision 1.51 by delta oz-2228:Add-access-to-recursive-structures, revision 1.2]
 * 
 * Revision 1.51  1992/04/01  20:53:10  oz
 * 	ITL allows the user to define her own variable types.  These may be nested
 * 	structures.  This delta allows the user to access data imbedded in such
 * 	structures and to modify single fields in an existing structure.
 * 
 * 	  New function:
 * 	itlIO_VariableBaseName
 * 	[from revision 1.50 by delta oz-2228:Add-access-to-recursive-structures, revision 1.1]
 * 
 * Revision 1.50  1992/03/30  21:09:11  oz
 * 	ITL stores two state information in script variables.
 * 	Make this variables invisible.
 * 
 * 	Renamed the variable '*itl*ThreadName'  to be ' itl ThreadName'
 * 	and '*itl*PortPrefix' to be ' itl PortPrefix'
 * 	[from revision 1.49 by delta oz-2202-Make_Itl_State_Vars_Invisible, revision 1.1]
 * 
 * Revision 1.49  1992/03/27  21:15:46  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	This file was broken into smaller files (modularity)
 * 	[from revision 1.48 by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.45  1992/02/20  16:27:25  oz
 * 	 - Added a pointer: itlIO_CreateSeveralThreadsP
 * 	   which points to a function to be used for coLoop
 * 	 - use a new structure: coloopInfo_t to pass info to coLoop
 * 	   threads.  (Contains out params at the end)
 * 	 - New Function: CreateSeveralThreads
 * 	   It is the default value for CreateSeveralThreadsP
 * 	[92/02/19  16:50:14  oz]
 * 
 * Revision 1.44  92/02/11  14:30:24  fred
 * 	Changed UTIL_ASSERT to ASSERT.
 * 	[92/02/11  14:28:15  fred]
 * 
 * Revision 1.43  92/02/07  13:52:52  oz
 * 	 - Replaced uses of CATCH_ALL  with   FINALLY
 * 
 * Revision 1.42  92/02/07  11:23:28  oz
 * 	 - Include dce excepetion handling or stub it out (tranc support)
 * 	 - DebugLevel  ==>  itl_traceLevel
 * 	 - var_t       ==>  itl_var_t
 * 	 - New functions (move here from itl_command.c)
 * 	       itlIO_HandleLoopCommand
 * 	       itlIO_HandleExecProc
 * 	[92/02/06  13:15:38  oz]
 * 
 * Revision 1.41  92/02/05  17:16:09  mss
 * 	Added RERAISE in runfromstring
 * 
 * Revision 1.40  92/02/04  22:17:02  oz
 * 	 - Added tran-C threadin support:
 * 	   When a thread is created instead of calling bde_ThreadCreate
 * 	   call a function pointed to by itlIO_CreateIndThreadP.
 * 	   Usually it calls the bde function, under tran-C it calls concThread
 * 	 - New function: CreateIndThread
 * 	[92/02/04  14:04:21  oz]
 * 
 * 	 - GetNewLine now displays the lines it is reading
 * 	   when DisplayLine mode is on.
 * 	[92/02/03  17:35:31  oz]
 * 
 * 	- SetStrResult now does not take the length, rather, it computes it.
 * 	[92/02/03  14:06:55  oz]
 * 
 * Revision 1.39  92/02/02  09:22:33  oz
 * 	 - Prompt while going to a brace level
 * 	[92/02/02  09:15:26  oz]
 * 
 * 	- RunFromString:
 * 	  Set the current_IO when in the CATCH_ALL
 * 	[92/01/31  18:49:11  oz]
 * 
 * 	 - Added forgotten RERAISE commands
 * 	[92/01/31  18:25:47  oz]
 * 
 * 	 - Adding support for tran-C:
 * 	   Catch DCE exceptions and make sure to clean up before reraising them.
 * 	   This is done for all routines that may indirectly call catch_abort
 * 	 - The include path is now store in an ITL variable -
 * 	   Previously it was in a thread slot and was not always freed.
 * 	[92/01/31  10:48:05  oz]
 * 
 * Revision 1.38  92/01/28  23:23:27  oz
 * 	 - New command: itlIO_TraceCurrentLocation
 * 
 * Revision 1.37  92/01/24  10:31:38  oz
 * 	 - Corrected prompt for optional clause in if
 * 	[92/01/24  10:28:52  oz]
 * 
 * 	 - New function:
 * 		itl_OptionalClause
 * 	 - Use itl_OptionalClause to implement the if-then-else function.
 * 	[92/01/24  09:42:55  oz]
 * 
 * Revision 1.36  92/01/23  18:12:57  oz
 * 	 - replaced the if construct by the NEW construct using {}
 * 	[92/01/23  17:07:40  oz]
 * 
 * 	- itl_Getcommand does not parse curly braces (as it temporarily did)
 * 	[92/01/23  11:45:09  oz]
 * 
 * 	 - Added support for braces:
 * 	    Keep the brace nesting level in thread IO structure.
 * 	 - Added macros for accessing a specific history structure.
 * 	 - HistoryLoc now reads the thread structure only once (instead of 4)
 * 	 - New functions:
 * 		itlIO_MatchBrace
 * 		itlIO_GotoBraceLevel
 * 	[92/01/23  11:36:33  oz]
 * 
 * Revision 1.35  92/01/20  08:30:35  oz
 * 	 - Renamed itl_PrintAdvise to be itl_PrintAdviseP
 * 	[92/01/19  08:50:19  oz]
 * 
 * 	 - Defined itl_AdviseUser and itl_fAdviseUser
 * 	 - Using
 * 		itl_AdviseUser	instead of itl_Advise
 * 		itl_PromptUser	instead of itl_Prompt
 * 	[92/01/18  18:10:25  oz]
 * 
 * 	 - new global var: itlIO_StartTime
 * 	 - Using:
 * 		itl_Message	instead of itl_PrintMsg
 * 		itl_Error	instead of itl_PrintErrMsg
 * 	 - Removed PrintTraceMessage
 * 	 - Defined one function for all types of printing: PrintAMessage
 * 	 - New Functions:
 * 		itl_Message
 * 		itl_fMessage
 * 		itl_PrintError
 * 		itl_fPrintError
 * 	 - Print the current time at initialization
 * 	 - Every print (except prompt) prints the time elapsed since startTime
 * 	[92/01/18  13:10:09  oz]
 * 
 * 	 - changed ITL_ALLOC to UTIL_ALLOC int PrintTraceMessage
 * 	[92/01/15  15:07:21  oz]
 * 
 * 	 - Changed the three letter prefixes for itl_Trace
 * 	[92/01/15  15:01:57  oz]
 * 
 * 	- New: PrintTraceMessage: similar to PrintMessage but it
 * 	  also prepends a 3 letter mask-dependent code.
 * 	- Converted all tracing to the new itl_Trace function.
 * 	[92/01/15  14:56:55  oz]
 * 
 * 	 - New function: PrintMessage: Not finished.
 * 	[92/01/15  10:50:58  oz]
 * 
 * 	 - Use var-args routine for output
 * 	 - So far: itl_Trace and itl_PromptUser
 * 	 - itl_Prompt now keeps the last prompt string and repeats it
 * 	   after an empty line is entered.
 * 	 - itlIO_PrintPrompt keeps a lock when it returns TRUE that must
 * 	   be unlocked by calling itlIO_DonePrompting.
 * 	 - itlIO_LastPrommptStr - returns the last string prompted
 * 	[92/01/12  09:50:47  oz]
 * 
 * 	 - Adding var-args functions for printing and prompting
 * 	[92/01/11  09:25:01  oz]
 * 
 * Revision 1.34  92/01/16  16:41:21  oz
 * 	 - corrected fprintf
 * 	[92/01/16  10:52:22  oz]
 * 
 * 	 - Traced thread slots used.
 * 	[92/01/15  10:17:58  oz]
 * 
 * Revision 1.33  92/01/10  11:30:00  oz
 * 	 - Corrected declarations to compile with ansi-c on HP
 * 	   (replaced extern by PRIVATE)
 * 
 * Revision 1.32  92/01/09  14:06:22  oz
 * 	 - BUG: getLine and getUninterpretedLine did not update the
 * 	        outLen out variable on the last line.  It was particularly
 * 	        bad when the DONE word was on the first line.
 * 	   FIXED.
 * 
 * Revision 1.31  92/01/08  15:16:33  oz
 * 	 - The parent thread IO info was not passed to children threads
 * 	   FIXED.
 * 	 - New Functions:
 * 	       StrThread - A thread which runs an ITL procedure.
 * 	       itl_RunStringInThread
 * 	[92/01/06  09:16:25  oz]
 * 
 * Revision 1.30  91/12/24  14:16:16  oz
 * 	 - Added an if-then-else command
 * 	[91/12/24  14:14:26  oz]
 * 
 * Revision 1.29  91/12/19  13:58:14  oz
 * 	 - BUG: When a coLoop was started inside a procedure (while running
 * 	        from a string) each thread had its own copy of the string
 * 	        pointer.  Therefore, some lines were executed more than once.
 * 	   FIXED:  Now, each thread_IO Structure (of a thread) contains a
 * 	        pointer to the pointer to the string.  Therefore, each line
 * 	        will be read only once
 * 	 - Minor Bug:  The routine that starts running from a file reset
 * 	        the string pointer which resulted in bad behavior when
 * 	        a file was included inside a string (only after the above
 * 	        bug fix).  This is redundant and WRONG and was eliminated.
 * 	   Fixed.
 * 
 * Revision 1.28  91/12/17  14:09:31  fred
 * 	Changed functions invoked via bde_ThreadCreate() to accept a pointer_t
 * 	and then cast it to  an iteration_t.  This is due to changes in
 * 	bde.h.
 * 
 * Revision 1.27  91/12/17  12:53:29  fred
 * 	Moved function delcarations after definition of iterate_t.
 * 
 * Revision 1.26  91/12/17  12:48:52  fred
 * 	Declared functons for bde_ThreadCreate() calls.
 * 
 * Revision 1.25  91/12/12  13:55:37  oz
 * 	Fixed type in function name
 * 	[91/12/10  20:14:16  oz]
 * 
 * 	 - new command: HandleDumpIOState
 * 	[91/12/10  20:11:50  oz]
 * 
 * 	 - New function: itlIO_DumpIOState
 * 	 - Added a prompt count to each history buffer to avoid over prompting.
 * 	   Advise still does not work correctly.
 * 	 - New functionL itlIO_PrintPrompt
 * 	[91/12/10  19:40:35  oz]
 * 
 * Revision 1.24  91/11/21  15:33:13  oz
 * 	Changed IO reads:
 * 	  All the threads read holding a mutex.
 * 	  There is one mutex per history buffer
 * 	[91/11/21  12:27:30  oz]
 * 
 * 	int_type is now itl_intType
 * 	[91/11/11  16:35:27  oz]
 * 
 * Revision 1.23  91/11/21  10:25:39  oz
 * 	- added varaible: itl_coLoopStackSize
 * 	- Default stack size now 100K
 * 
 * Revision 1.21  91/11/06  16:30:08  oz
 * 	 - removed TraceToSink
 * 
 * Revision 1.20  91/11/04  17:10:43  oz
 * 	  Corrected bug in coLoop: It did not update the string
 * 	  pointer of the parent when a coLoop executed inside a procedure.
 * 
 * Revision 1.19  91/10/30  17:19:11  oz
 * 	 Added itl_SetInteractive(FALSE) in startLooping after the
 * 	 first iteration so that remaining iterations are not interactive.
 * 	 There was a problem: if you ended the loop with partial endLoo
 * 	 then it would advise you after each iteration.
 * 
 * Revision 1.18  91/10/30  16:06:57  mss
 * 	Undid SHARING of private function
 * 
 * Revision 1.17  91/10/23  16:43:44  oz
 * 	- New functions:
 * 		itlIO_GetUninterpretedLine
 * 		itl_GetThreadState
 * 		itl_RestoreThreadState
 * 		itl_FreeThreadState
 * 	- Loop, coLoop and while commands now check the terminating
 * 	  words and complain if it was the wrong one.
 * 
 * Revision 1.16  91/10/22  08:44:06  mss
 * 	Temporarily exposed Current_IO_Data for use by Tran-C
 * 
 * Revision 1.15  91/10/21  15:16:32  oz
 * 	Small fix to GetSegment: Break instead of returning TRUE.
 * 
 * Revision 1.14  91/10/21  15:06:20  oz
 * 	- Corrected get segment: It was broken when interpret was FALSE:
 * 	  it did not read new commands if the history buffer was empty.
 * 	- New Command: itlIO_RandomSeed
 * 
 * Revision 1.13  91/10/21  11:45:56  oz
 * 	Renamed: isInterface	=>  itlIO_IsInterface
 * 		 setInterfaceMode   =>  itlIO_SetInterfaceMode
 * 
 * Revision 1.13  1992/11/29  23:17:06  fred
 * 	To convert ITL to use the random() function instead of rand().
 * 	Also added the "error" command.
 * 
 * 	Changed randomSeed command to call srandom() not srand().
 * 	[from revision 1.12 by delta fred-ot6219-use-random-not-rand, r1.1]
 * 
 * Revision 1.12  1992/11/24  20:28:21  fred
 * 	To make the process id and system page size variables available to all test
 * 	programs but placing their initialization in the ITL library and not the
 * 	itlFile library.
 * 
 * 	Also incorporated fixes to allow parameters to be passed to procedures
 * 	started in detached threads.
 * 
 * 	Fix PromptUser to work with procedures running in detached threads.
 * 	[from revision 1.11 by delta fred-ot-6193-move-procid-to-base-itl, r1.1]
 * 
 * Revision 1.11  1992/11/12  16:35:45  mcinerny
 * 	[merge of changes from 1.7 to 1.10 into 1.8]
 * 
 * Revision 1.10  1992/11/10  19:49:11  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.7 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.8  1992/11/03  15:27:35  fred
 * 	Incorporates Ofer's changes that permit procedures to have parameters
 * 	passed to them, and also includes the vector and valList data types.  The
 * 	vector data type is needed for DFS testing.  It allows us to manipulate
 * 	binary data.
 * 
 * 	Changes for procedure parameter passing.
 * 	[from revision 1.7 by delta fred-itl-ofer-fixes-10-30, r1.1]
 * 
 * Revision 1.7  1992/10/14  11:28:56  mcinerny
 * 	[merge of changes from 1.3 to 1.6 into 1.5]
 * 
 * Revision 1.6  1992/10/13  18:19:29  fred
 * 	To include Ofer's fix to loop increment values.
 * 
 * 	Check for a 0 increment value in itlIO_StartLooping().
 * 	[from revision 1.3 by delta fred-itl-ofer-fix-loop-incr, r1.1]
 * 
 * Revision 1.5  1992/10/05  16:14:13  jdp
 * 	[merge of changes from 1.2 to 1.4 into 1.3]
 * 
 * Revision 1.4  1992/10/02  20:32:03  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.2 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.3  1992/09/24  15:23:25  fred
 * 	Include some minor fixes made to the TP ITL.
 * 
 * 	Fixed two memory leaks.
 * 	[from revision 1.2 by delta fred-itl-ofer-fixes-9-24-92, revision 1.1]
 * 
 * Revision 1.2  1992/08/20  19:37:40  fred
 * 	Manually imported bug fixes from the Encina ITL.  Got them from Ofer.
 * 	Also, I changed the ITL_FILE_CHECK_ERROR macro to fail when something
 * 	succeeds that was expected to fail.
 * 	Added the verifyFileError command to toggle file system function verification.
 * 	(The code was already there I just was not making it available.)
 * 
 * 	Ofer bug fixes.
 * 	[from revision 1.1 by delta fred-fix-itl-bugs-8-20-92, revision 1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:08  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.62  1992/06/12  16:54:05  oz
 * 	itl_GetQuotedString returned the wrong length.
 * 
 * 	itl_GetQuotedString incorrectly returned the number of characters
 * 	it read (which is wrong when there are quoted characters.
 * 	[from revision 1.60 by delta oz-3204-ITL-correct-returned-len-in-quotesString, revision 1.1]
 * 
 * Revision 1.60  1992/06/06  17:36:47  oz
 * 	Allow the user to insert a '"' or a newline inside a string
 * 
 * 	Changed the routine: itl_GetQuotedString.
 * 	It now scans one letter at a time and deals with it.
 * 	[from revision 1.59 by delta oz-3064-allow-special-chars-in-strings, revision 1.1]
 * 
 * Revision 1.59  1992/05/18  17:36:50  oz
 * 	Corrected a forgotten comment
 * 
 * 	[from revision 1.58 by delta oz-2576:ITL:Do-not-abort-top-level-tran-on-parse-error, revision 1.2]
 * 
 * Revision 1.58  1992/05/12  19:53:17  oz
 * 	When a parse error occurs, ITL aborts the current transaction even
 * 	if the test is in interactive mode and the transaction is a top
 * 	level transaction.  This is not needed.
 * 	This change prevents a tansaction in the top most interactive
 * 	level from being aborted as a result of a parse error.
 * 
 * 	The function itl_OptionalClause neither increments nor
 * 	decrements the itlIO_NestingLevel
 * 	[from revision 1.57 by delta oz-2576:ITL:Do-not-abort-top-level-tran-on-parse-error, revision 1.1]
 * 
 * Revision 1.57  1992/04/29  13:37:16  oz
 * 	itl_SetErrorStatus must be called only after the rest of the module
 * 	was initialized.
 * 	[from revision 1.56 by delta oz-2517-ITL:Initialize-parse-error-var, revision 1.2]
 * 
 * Revision 1.56  1992/04/28  21:17:34  oz
 * 	ITL by default takes the global value of a local variable if the
 * 	thread does not have a local value.  Since the ErrorStatus is not set,
 * 	if any thread sets the flag to TRUE, all the threads will see the
 * 	value (in the global value), which is an error.  By setting it to
 * 	FALSE at initialization the defaul value is FALSE for any new thread,
 * 	as it should be.
 * 
 * 	Call itl_SetErrorStatus(FALSE) in itl_InitIO()
 * 	[from revision 1.54 by delta oz-2517-ITL:Initialize-parse-error-var, revision 1.1]
 * 
 * Revision 1.54  1992/04/06  21:50:28  oz
 * 	Additional minor fixes.
 * 
 * 	optionalClause did not increment/decrement the nesting level correctly.
 * 	optionalClause does nothing if IO-Error is TRUE
 * 	[from revision 1.53 by delta oz-2278:Errors-in-optional-clause, revision 1.2]
 * 
 * Revision 1.53  1992/04/06  20:31:33  oz
 * 	Optional clause now inclrements the nesting level to make the called
 * 	command loop to abort on error.
 * 
 * 	Optional clause now inclrements the nesting level to make the called
 * 	command loop to abort on error.
 * 	[from revision 1.52 by delta oz-2278:Errors-in-optional-clause, revision 1.1]
 * 
 * Revision 1.52  1992/04/02  14:49:09  oz
 * 	  Continued developlment on the access to recursive access.
 * 	  fetch&Op works properly now.
 * 	  test&Set still doesn't
 * 
 * 	  Added missing return statement
 * 	[from revision 1.51 by delta oz-2228:Add-access-to-recursive-structures, revision 1.2]
 * 
 * Revision 1.51  1992/04/01  20:53:10  oz
 * 	ITL allows the user to define her own variable types.  These may be nested
 * 	structures.  This delta allows the user to access data imbedded in such
 * 	structures and to modify single fields in an existing structure.
 * 
 * 	  New function:
 * 	itlIO_VariableBaseName
 * 	[from revision 1.50 by delta oz-2228:Add-access-to-recursive-structures, revision 1.1]
 * 
 * Revision 1.50  1992/03/30  21:09:11  oz
 * 	ITL stores two state information in script variables.
 * 	Make this variables invisible.
 * 
 * 	Renamed the variable '*itl*ThreadName'  to be ' itl ThreadName'
 * 	and '*itl*PortPrefix' to be ' itl PortPrefix'
 * 	[from revision 1.49 by delta oz-2202-Make_Itl_State_Vars_Invisible, revision 1.1]
 * 
 * Revision 1.49  1992/03/27  21:15:46  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	This file was broken into smaller files (modularity)
 * 	[from revision 1.48 by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.45  1992/02/20  16:27:25  oz
 * 	 - Added a pointer: itlIO_CreateSeveralThreadsP
 * 	   which points to a function to be used for coLoop
 * 	 - use a new structure: coloopInfo_t to pass info to coLoop
 * 	   threads.  (Contains out params at the end)
 * 	 - New Function: CreateSeveralThreads
 * 	   It is the default value for CreateSeveralThreadsP
 * 	[92/02/19  16:50:14  oz]
 * 
 * Revision 1.44  92/02/11  14:30:24  fred
 * 	Changed UTIL_ASSERT to ASSERT.
 * 	[92/02/11  14:28:15  fred]
 * 
 * Revision 1.43  92/02/07  13:52:52  oz
 * 	 - Replaced uses of CATCH_ALL  with   FINALLY
 * 
 * Revision 1.42  92/02/07  11:23:28  oz
 * 	 - Include dce excepetion handling or stub it out (tranc support)
 * 	 - DebugLevel  ==>  itl_traceLevel
 * 	 - var_t       ==>  itl_var_t
 * 	 - New functions (move here from itl_command.c)
 * 	       itlIO_HandleLoopCommand
 * 	       itlIO_HandleExecProc
 * 	[92/02/06  13:15:38  oz]
 * 
 * Revision 1.41  92/02/05  17:16:09  mss
 * 	Added RERAISE in runfromstring
 * 
 * Revision 1.40  92/02/04  22:17:02  oz
 * 	 - Added tran-C threadin support:
 * 	   When a thread is created instead of calling bde_ThreadCreate
 * 	   call a function pointed to by itlIO_CreateIndThreadP.
 * 	   Usually it calls the bde function, under tran-C it calls concThread
 * 	 - New function: CreateIndThread
 * 	[92/02/04  14:04:21  oz]
 * 
 * 	 - GetNewLine now displays the lines it is reading
 * 	   when DisplayLine mode is on.
 * 	[92/02/03  17:35:31  oz]
 * 
 * 	- SetStrResult now does not take the length, rather, it computes it.
 * 	[92/02/03  14:06:55  oz]
 * 
 * Revision 1.39  92/02/02  09:22:33  oz
 * 	 - Prompt while going to a brace level
 * 	[92/02/02  09:15:26  oz]
 * 
 * 	- RunFromString:
 * 	  Set the current_IO when in the CATCH_ALL
 * 	[92/01/31  18:49:11  oz]
 * 
 * 	 - Added forgotten RERAISE commands
 * 	[92/01/31  18:25:47  oz]
 * 
 * 	 - Adding support for tran-C:
 * 	   Catch DCE exceptions and make sure to clean up before reraising them.
 * 	   This is done for all routines that may indirectly call catch_abort
 * 	 - The include path is now store in an ITL variable -
 * 	   Previously it was in a thread slot and was not always freed.
 * 	[92/01/31  10:48:05  oz]
 * 
 * Revision 1.38  92/01/28  23:23:27  oz
 * 	 - New command: itlIO_TraceCurrentLocation
 * 
 * Revision 1.37  92/01/24  10:31:38  oz
 * 	 - Corrected prompt for optional clause in if
 * 	[92/01/24  10:28:52  oz]
 * 
 * 	 - New function:
 * 		itl_OptionalClause
 * 	 - Use itl_OptionalClause to implement the if-then-else function.
 * 	[92/01/24  09:42:55  oz]
 * 
 * Revision 1.36  92/01/23  18:12:57  oz
 * 	 - replaced the if construct by the NEW construct using {}
 * 	[92/01/23  17:07:40  oz]
 * 
 * 	- itl_Getcommand does not parse curly braces (as it temporarily did)
 * 	[92/01/23  11:45:09  oz]
 * 
 * 	 - Added support for braces:
 * 	    Keep the brace nesting level in thread IO structure.
 * 	 - Added macros for accessing a specific history structure.
 * 	 - HistoryLoc now reads the thread structure only once (instead of 4)
 * 	 - New functions:
 * 		itlIO_MatchBrace
 * 		itlIO_GotoBraceLevel
 * 	[92/01/23  11:36:33  oz]
 * 
 * Revision 1.35  92/01/20  08:30:35  oz
 * 	 - Renamed itl_PrintAdvise to be itl_PrintAdviseP
 * 	[92/01/19  08:50:19  oz]
 * 
 * 	 - Defined itl_AdviseUser and itl_fAdviseUser
 * 	 - Using
 * 		itl_AdviseUser	instead of itl_Advise
 * 		itl_PromptUser	instead of itl_Prompt
 * 	[92/01/18  18:10:25  oz]
 * 
 * 	 - new global var: itlIO_StartTime
 * 	 - Using:
 * 		itl_Message	instead of itl_PrintMsg
 * 		itl_Error	instead of itl_PrintErrMsg
 * 	 - Removed PrintTraceMessage
 * 	 - Defined one function for all types of printing: PrintAMessage
 * 	 - New Functions:
 * 		itl_Message
 * 		itl_fMessage
 * 		itl_PrintError
 * 		itl_fPrintError
 * 	 - Print the current time at initialization
 * 	 - Every print (except prompt) prints the time elapsed since startTime
 * 	[92/01/18  13:10:09  oz]
 * 
 * 	 - changed ITL_ALLOC to UTIL_ALLOC int PrintTraceMessage
 * 	[92/01/15  15:07:21  oz]
 * 
 * 	 - Changed the three letter prefixes for itl_Trace
 * 	[92/01/15  15:01:57  oz]
 * 
 * 	- New: PrintTraceMessage: similar to PrintMessage but it
 * 	  also prepends a 3 letter mask-dependent code.
 * 	- Converted all tracing to the new itl_Trace function.
 * 	[92/01/15  14:56:55  oz]
 * 
 * 	 - New function: PrintMessage: Not finished.
 * 	[92/01/15  10:50:58  oz]
 * 
 * 	 - Use var-args routine for output
 * 	 - So far: itl_Trace and itl_PromptUser
 * 	 - itl_Prompt now keeps the last prompt string and repeats it
 * 	   after an empty line is entered.
 * 	 - itlIO_PrintPrompt keeps a lock when it returns TRUE that must
 * 	   be unlocked by calling itlIO_DonePrompting.
 * 	 - itlIO_LastPrommptStr - returns the last string prompted
 * 	[92/01/12  09:50:47  oz]
 * 
 * 	 - Adding var-args functions for printing and prompting
 * 	[92/01/11  09:25:01  oz]
 * 
 * Revision 1.34  92/01/16  16:41:21  oz
 * 	 - corrected fprintf
 * 	[92/01/16  10:52:22  oz]
 * 
 * 	 - Traced thread slots used.
 * 	[92/01/15  10:17:58  oz]
 * 
 * Revision 1.33  92/01/10  11:30:00  oz
 * 	 - Corrected declarations to compile with ansi-c on HP
 * 	   (replaced extern by PRIVATE)
 * 
 * Revision 1.32  92/01/09  14:06:22  oz
 * 	 - BUG: getLine and getUninterpretedLine did not update the
 * 	        outLen out variable on the last line.  It was particularly
 * 	        bad when the DONE word was on the first line.
 * 	   FIXED.
 * 
 * Revision 1.31  92/01/08  15:16:33  oz
 * 	 - The parent thread IO info was not passed to children threads
 * 	   FIXED.
 * 	 - New Functions:
 * 	       StrThread - A thread which runs an ITL procedure.
 * 	       itl_RunStringInThread
 * 	[92/01/06  09:16:25  oz]
 * 
 * Revision 1.30  91/12/24  14:16:16  oz
 * 	 - Added an if-then-else command
 * 	[91/12/24  14:14:26  oz]
 * 
 * Revision 1.29  91/12/19  13:58:14  oz
 * 	 - BUG: When a coLoop was started inside a procedure (while running
 * 	        from a string) each thread had its own copy of the string
 * 	        pointer.  Therefore, some lines were executed more than once.
 * 	   FIXED:  Now, each thread_IO Structure (of a thread) contains a
 * 	        pointer to the pointer to the string.  Therefore, each line
 * 	        will be read only once
 * 	 - Minor Bug:  The routine that starts running from a file reset
 * 	        the string pointer which resulted in bad behavior when
 * 	        a file was included inside a string (only after the above
 * 	        bug fix).  This is redundant and WRONG and was eliminated.
 * 	   Fixed.
 * 
 * Revision 1.28  91/12/17  14:09:31  fred
 * 	Changed functions invoked via bde_ThreadCreate() to accept a pointer_t
 * 	and then cast it to  an iteration_t.  This is due to changes in
 * 	bde.h.
 * 
 * Revision 1.27  91/12/17  12:53:29  fred
 * 	Moved function delcarations after definition of iterate_t.
 * 
 * Revision 1.26  91/12/17  12:48:52  fred
 * 	Declared functons for bde_ThreadCreate() calls.
 * 
 * Revision 1.25  91/12/12  13:55:37  oz
 * 	Fixed type in function name
 * 	[91/12/10  20:14:16  oz]
 * 
 * 	 - new command: HandleDumpIOState
 * 	[91/12/10  20:11:50  oz]
 * 
 * 	 - New function: itlIO_DumpIOState
 * 	 - Added a prompt count to each history buffer to avoid over prompting.
 * 	   Advise still does not work correctly.
 * 	 - New functionL itlIO_PrintPrompt
 * 	[91/12/10  19:40:35  oz]
 * 
 * Revision 1.24  91/11/21  15:33:13  oz
 * 	Changed IO reads:
 * 	  All the threads read holding a mutex.
 * 	  There is one mutex per history buffer
 * 	[91/11/21  12:27:30  oz]
 * 
 * 	int_type is now itl_intType
 * 	[91/11/11  16:35:27  oz]
 * 
 * Revision 1.23  91/11/21  10:25:39  oz
 * 	- added varaible: itl_coLoopStackSize
 * 	- Default stack size now 100K
 * 
 * Revision 1.21  91/11/06  16:30:08  oz
 * 	 - removed TraceToSink
 * 
 * Revision 1.20  91/11/04  17:10:43  oz
 * 	  Corrected bug in coLoop: It did not update the string
 * 	  pointer of the parent when a coLoop executed inside a procedure.
 * 
 * Revision 1.19  91/10/30  17:19:11  oz
 * 	 Added itl_SetInteractive(FALSE) in startLooping after the
 * 	 first iteration so that remaining iterations are not interactive.
 * 	 There was a problem: if you ended the loop with partial endLoo
 * 	 then it would advise you after each iteration.
 * 
 * Revision 1.18  91/10/30  16:06:57  mss
 * 	Undid SHARING of private function
 * 
 * Revision 1.17  91/10/23  16:43:44  oz
 * 	- New functions:
 * 		itlIO_GetUninterpretedLine
 * 		itl_GetThreadState
 * 		itl_RestoreThreadState
 * 		itl_FreeThreadState
 * 	- Loop, coLoop and while commands now check the terminating
 * 	  words and complain if it was the wrong one.
 * 
 * Revision 1.16  91/10/22  08:44:06  mss
 * 	Temporarily exposed Current_IO_Data for use by Tran-C
 * 
 * Revision 1.15  91/10/21  15:16:32  oz
 * 	Small fix to GetSegment: Break instead of returning TRUE.
 * 
 * Revision 1.14  91/10/21  15:06:20  oz
 * 	- Corrected get segment: It was broken when interpret was FALSE:
 * 	  it did not read new commands if the history buffer was empty.
 * 	- New Command: itlIO_RandomSeed
 * 
 * Revision 1.13  91/10/21  11:45:56  oz
 * 	Renamed: isInterface	=>  itlIO_IsInterface
 * 		 setInterfaceMode   =>  itlIO_SetInterfaceMode
 * 
 * Revision 1.12  91/10/17  12:02:40  fred
 * 	Moved code to update the currentFileName variable from OpenFileOnDir()
 * 	to OpenFileName()
 * 
 * Revision 1.11  91/10/17  08:39:18  fred
 * 	Added the itlIO_GetFilename() function.
 * 
 * Revision 1.10  91/10/16  19:08:22  oz
 * 	- Renamed:
 * 	    setStrResult  ->  itl_SetStrResult
 * 	    setIntResult  ->  itl_SetIntResult
 * 
 * Revision 1.9  91/10/03  12:53:18  oz
 * 	New Routine: itlIO_NestingLevel()
 * 	itl_LocalCommandLoop now retuns a string (see itl_CommandLoop)
 * 
 * Revision 1.8  91/10/02  11:50:09  oz
 * 	Renamed serverRoutine to be itlServer_Start
 * 	Write the RCSid in verbose tracing
 * 
 * Revision 1.7  91/09/30  16:18:28  oz
 * 	Replaced GETC with READ() when reading from a file.
 * 	Solves the `10 thread problem'
 * 
 * Revision 1.6  91/09/29  17:04:17  mss
 * 	Passed address for storage to thread-local state manipulation routines
 * 
 * Revision 1.5  91/09/22  10:12:55  oz
 * 	Added:
 * 	 - itl_EchoLine
 * 	 - itl_CommentLine
 * 	Fixed:
 * 	 - itl_GetLine now interprets $( : Evaluates the expression following.
 * 
 * Revision 1.4  91/09/21  19:44:14  oz
 * 	- During IO, when the first character is HIST_CHARACTER, added
 * 	  a check to see if the second is *, to make sure that
 * 	  !* is interpreted correctly.
 * 
 * Revision 1.3  91/09/16  11:56:49  mss
 * 	Renamed  CommandLoop =>  itl_CommandLoop consistently
 * 
 * Revision 1.2  91/09/12  17:54:07  oz
 * 	Changes to convert the REC tester to a genral ITL Test Tool
 * 	Many cosmetic changes: name changes to conform to transarc standards.
 * 	The basic idea: routines used by everyone, or generally export
 * 	are prefixed with itl_
 * 	Routine of this module that are exported to other ITL modules (and
 * 	more advanced users who really want to use them) are prefixed
 * 	by itlIO_.  Internal (private) routines start with an upper case letter.
 * 	Some substantial changes:
 * 	  - PrintComments now simply calls itlIO_GetLine to get and
 * 	    interpret a line.
 * 	  - itlIO_Getline - returns the length of the string.
 * 	    . Interprets variable names correctly.
 * 	    . Does not skip over text after the done word.
 * 	  - itlIO_GetWord reads a token no interpreting special characters
 * 	  - itl_GetString interprets special char such as ", ( $ etc.
 * 	  - Repeat command: Takes and arbitrary value and converts
 * 	    it to boolean (as opposed to reading integer).
 * 	  - Separated looping from coLooping: they are sufficiently
 * 	    different that it makes more sense to put them each in a routine.
 * 	  - added: itl_StartIndependentThread: (to replace startThreadOnSocket)
 * 
 * Revision 1.1  91/08/22  17:21:09  mss
 * 	Modified REC tester to make separate library
 * 
 * 	$EndTALog$
 * 
 * $EndLog$
 */
/*
 * itl_io.c -- 
 *    The I/O and parsing routines for the Interactive Test Library (ITL).
 */


#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>

#include <pthread.h>

#include <utils/itl/itl_stds.h>

#include <utils/itl/itl_private.h>
#include <utils/itl/itl_io.h>
#include <utils/itl/itlio_internal.h>
#include <utils/itl/itl_thread.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_dceActions.h>

static char RCSid[] = "$";


#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: itl_io.c,v 1.1.111.1 1996/10/17 18:27:20 damon Exp $";
#endif



/*
 *  	Global Variables and Module State
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

/* 
 *    Global  Variables
 */

EXPORT char *WhiteSpaces = WHITE_SPACES;
EXPORT char *AlphaNumChars = ALPHA_NUM_CHARS;

EXPORT int  itl_LastSignalReceived;

PRIVATE void CreateIndThread _TAKES((IN void (*fn)(void*),
				    IN pointer_t argP));
/*
 *	itlIO_CreateIndThreadP
 *  A pointer to a function called to start an independent thread.
 *  Normally, this will point to a local function that will call
 *  pthread_create.  However, when running under tran-C this should
 *  be changed to point to the tran-C function concThread
 */
SHARED void (*itlIO_CreateIndThreadP) _TAKES((
					      IN void (*fn)(void*),
					      IN void *arg))
  = CreateIndThread;

PRIVATE void CreateSeveralThreads _TAKES((IN void (*fn)(pointer_t),
					  IN int numThreads,
					  IN pointer_t *argList));
/*
 *	itlIO_CreateSeveralThreadsP
 *  A pointer to a function called to start several synchronous
 *  threads.
 *  Normally, this will point to a local function that will call
 *  pthread_create several times and will then join with all the
 *  threads.  However, when running under tran-C this should
 *  be changed to point to the tran-C function cofor
 */
SHARED void (*itlIO_CreateSeveralThreadsP) _TAKES((
					   IN void (*fn)(pointer_t),
					   IN int numThreads,
					   IN pointer_t *argList))
      = CreateSeveralThreads;


SHARED boolean_t HandleDumpIOState(param)
  IN int param;
{
    itlIO_DumpIOState(Cur_Out);
    return(FALSE);
}

/*
 *  itl_PrintAdviseP
 *	returns whether an advise should be printed or not.
 */
EXPORT boolean_t itl_PrintAdviseP()
{
    return(itlIO_IsDebug() || (itlIO_IsInteractive() && itlIO_SkipWhites()));
}


/************************************************************************
 ************************************************************************
 ********      
 ********      
 ********      Low level routines for parsing the history buffer.
 ********      
 ********      
 ************************************************************************
 ************************************************************************
 */


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_NextCharacter():
 *    return the next nonWhite character in the history buffer,
 *    reading from the input stream if necessary.
 *    The character is not skipped over.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char itl_NextCharacter()
{
  if (itlIO_AdvanceToNonWhite())
      return(-1);
  else
      return(*itlIO_HistoryLoc());
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_GetCharacter():
 *    Return the next character in the history buffer - skipping over
 *    white spaces and reading from the input if necessary.  The history
 *    index is advanced to the next raw-character after the character
 *    that is returned.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char itl_GetCharacter()
{
  char retVal;
  retVal = itl_NextCharacter();
  itlIO_AdvanceIndex(1);
  return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_MatchCharacter(c)
 *     Check whether the next character in the input stream is the
 *     specified character.  If it is return TRUE and skip over the
 *     character.  Otherwise return FALSE and leave the character there.
 *
 *     This routine skips over white space and reads from the input stream
 *     if necessary.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT boolean_t itl_MatchCharacter(ch)
IN int ch;
{
    if (itl_NextCharacter() == ch) {
	itlIO_AdvanceIndex(1);
	return(TRUE);
    } else {
	return(FALSE);
    }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_EchoLine():
 *    Print the rest of the line interpreting variables.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_EchoLine()
{
    char text[MAX_STRING_LENGTH];
    (void) itlIO_GetLine(text, NULL, NULL);
    itl_Message("%s\n", text);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_CommentLine - treat the rest of the line as a comment
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_CommentLine()
{
    itlIO_AdvanceLine();
}


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

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
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
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char *itlIO_WordFromString(str, delims, flag, lenP)
  IN char *str;
  IN char *delims;
  IN boolean_t flag;
  OUT int *lenP;
{
    char *word;
    
    itl_Trace(ITL_IO_TRACE,
	      "IO_WordFromString: Str: %s\n    delims, %s\n   Mode: %s,  lenP: %p\n",
	      str, delims,
	      flag ? "exclusive" : "inclusive", lenP);
    
    *lenP = flag ? strcspn(str, delims) : strspn(str, delims);
    ITL_UTIL_ALLOC(word, char *, *lenP+1);
    ITL_UTIL_MEMCOPY((pointer_t)str, (pointer_t) word, *lenP);
    word[*lenP] = '\0';
    return(word);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *   itl_GetWord()  
 *    return one word from the current location in the history buffer.
 *    This is the basic routine for reading from the history buffer.
 *    The word is allocated dynamically and it is up to the caller to
 *    free the space when it is no longer needed.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char *itl_GetWord(eofP, lenP)
  OUT boolean_t *eofP;
  OUT int *lenP;
{
    char *word;
    if (*eofP = itlIO_AdvanceToNonWhite())
	return(NULL);
    else {
	word = itlIO_WordFromString(itlIO_HistoryLoc(),
				    WhiteSpaces, TRUE, lenP);
	itlIO_AdvanceIndex(*lenP);
	return(word);
    }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *   itl_PeekWord()  
 *    return one word from the current location in the history buffer.
 *    This routine returns the same word that would return from a call
 *    to itl_GetWord, but does not advance the location in the
 *    history buffer.
 *
 *    The word is allocated dynamically and it is up to the caller to
 *    free the space when it is no longer needed.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char *itl_PeekWord(eofP, lenP)
  OUT boolean_t *eofP;
  OUT int *lenP;
{
    char *retVal;
    int startOffset;
    int startIndex;
    itlIO_CurrentHistLoc(&startOffset, &startIndex);
    retVal = itl_GetWord(eofP, lenP);
    itlIO_GotoHistLoc(startOffset, startIndex);
    return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *   itl_PeekString()  
 *    return one word from the current location in the history buffer.
 *    This routine returns the same word that would return from a call
 *    to itl_GetString, but does not advance the location in the history
 *    buffer.
 *
 *    The word is allocated dynamically and it is up to the caller to
 *    free the space when it is no longer needed.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char *itl_PeekString(eofP, lenP)
  OUT boolean_t *eofP;
  OUT int *lenP;
{
    char *retVal;
    int startOffset;
    int startIndex;
    itlIO_CurrentHistLoc(&startIndex, &startOffset);
    retVal = itl_GetString(eofP, lenP);
    itlIO_GotoHistLoc(startIndex, startOffset);
    return(retVal);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_GetAlphaWord()  
 *    return one word from the current location in the history buffer.
 *    The word returned comprises solely of alpha numeric characters.
 *    (e.g., this is used for variable names.)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT char *itl_GetAlphaWord(eofP, lenP)
OUT boolean_t *eofP;
OUT int *lenP;
{
    if (*eofP = itlIO_AdvanceToNonWhite())
	return(NULL);
    else {
	char *word =
	    itlIO_WordFromString(itlIO_HistoryLoc(), AlphaNumChars, FALSE, lenP);
	itlIO_AdvanceIndex(*lenP);
	return(word);
    }
}

/*
 *  itlIO_VariableBaseName
 *	Return the base name of the variable (without the field modifiers).
 *	The return value is dynamically allocated and copied.
 */
SHARED char *itlIO_VariableBaseName(name)
  IN  char *name;
{
    char *retVal, *dotLocation;
    int len;

    if (dotLocation = strchr(name, '.')) {
	len = dotLocation - name;
    } else {
	len = strlen(name);
    }
    ITL_ALLOC(retVal, char *, len + 1);
    ITL_MEMCOPY(name, retVal, len);
    retVal[len] = '\0';
    return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_GetVariableName()
 *    return a variable name from the current location in the history buffer.
 *    A variable name may be different than a word or alphaWord: in
 *    particular, this routines recognizes square brackets and interprets
 *    the variable name as an array.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT char *itl_GetVariableName(errP, lenP)
  OUT boolean_t *errP;
  INOUT int *lenP;
{
    itl_var_t *tempVar;
    char *name;
    int curLen;

    if (itl_MatchCharacter('{')) {
	name = itl_GetVariableName(errP, lenP);
	if (name && !*errP && (itlIO_HistoryLoc()[0] != '}')) {
	    itl_Error("Missing '}' after variable name %s\n", name);
	    *errP = TRUE;
	    ITL_FREE(name);
	    return(NULL);
	} else {
	    itlIO_AdvanceIndex(1);  /*  Skip over the '}'   */
	    return(name);
	}
    }
    if (itl_MatchCharacter('$')) {
        /* 
	 *  The name starts with a $, interpret that as a variable
	 */
	name = itl_GetVariableName(errP, lenP);
	if (*errP) {
	    return(NULL);
	}
	if ((tempVar = itl_LocateVariable(name)) != (itl_var_t *)NULL) {
	    ITL_FREE(name);
	    name = itl_VarValueToString(VAR_VALUE_REC(tempVar), lenP);
	}
	return(name);
    } 

    name = itl_GetAlphaWord(errP, &curLen);
    if (*errP) {
	return(NULL);
    }

    while ((*itlIO_HistoryLoc() == '[') || (*itlIO_HistoryLoc() == '.')) {
	char nextC = itlIO_HistoryLoc()[1];
	if (itl_MatchCharacter('[')) {
	    var_val_t *index;
	    char *indStr;
	    int indLen;
	    index = GetExpression();
	    if (*errP = !itl_MatchCharacter(']')) {
		itl_Error("Expecting a ] to close array reference\n");
		if (name)
		    ITL_FREE(name);
		return(NULL);
	    }
	    indStr = itl_VarValueToString(index, &indLen);
	    /* add 2 for the braces */
	    ITL_UTIL_REALLOC(name, char *, curLen + indLen + 3);
	    name[curLen++] = '[';
	    ITL_UTIL_MEMCOPY(indStr, &name[curLen], indLen);
	    curLen += indLen;
	    name[curLen++] = ']';
	    name[curLen] = '\0';
	    ITL_FREE(indStr);
	    itl_FreeVarVal(index);
	} else if ((nextC != '.') && !isspace(nextC) && nextC) {
	    int restLen;
	    boolean_t eof;
	    char *restOfName;
		
	    (void) itl_MatchCharacter('.');
	    restOfName = itl_GetVariableName(&eof, &restLen);
	    ITL_UTIL_REALLOC(name, char *, curLen + restLen + 2);
	    name[curLen++] = '.';
	    ITL_UTIL_MEMCOPY(restOfName, &name[curLen], restLen + 1);
	    curLen += restLen;
	    name[curLen] = '\0';
	    ITL_UTIL_FREE(restOfName);
	    break;
	} else {
	    break;
	}
    }
    if (lenP)
	*lenP = curLen;
    return(name);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * 
 *  itlIO_ProcessComments():
 *	
 *    Skip over all white space and comments processing the comments
 *    that need to be processed (e.g., lines that begin
 *    with !* maybe typed out).
 *
 *    This routine is called by itl_GetCommand before it calls
 *    itl_GetString to get the actual commands.  It is possible, though
 *    may have some undesirable effects to always call this routine before
 *    calling itl_GetString:  this will enable comments between parameters,
 *    but may restrict the type of allowable words.
 *
 *    It should be safe to do this and to surround by double quotes any
 *    word that begins with a * or a | or a !.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
SHARED boolean_t itlIO_ProcessComments(promptStr)
IN char *promptStr;
{
  do {
       itl_PromptUser("%s",promptStr);
       if (itlIO_AdvanceToNonWhite())
	   return(TRUE);
       switch(itl_NextCharacter()) {
	  case '!':
	  case '|':
	    itl_EchoLine();
	    break;
	  case '*':
	    itl_CommentLine();
	    break;
	  default:
	    /*  Found a non comment non-white character */
	    return(FALSE);
       }
  } while(TRUE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_GetCommand()  
 *    return one word from the current location in the history buffer.
 *    It also skips all comments by calling itlIO_ProcessComments().
 *    On return, If the variable eofP is TRUE then EOF was reached and
 *    the function returns NULL.
 *    Otherwise, eofP returns FALSE and the function returns a dynamically
 *    allocated word.  It is up to the caller to free that pointer when it
 *    is no longer required.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT char *itl_GetCommand(promptStr, eofP)
IN char *promptStr;
OUT boolean_t *eofP;
{
    char *command;
    int commandLen;
    if (*eofP = itlIO_ProcessComments(promptStr))
	return(NULL);

    command = itl_GetString(eofP, &commandLen);

    return(command);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_GetQuotedString:
 *     Get a quoted string: a string surrounded by '"'s:
 *  It is assumed that the first " has already been read.  It reads
 *  a string and terminates at the first ".
 *  The string is not allowed to contain new lines.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
EXPORT char *itl_GetQuotedString(err, outLen)
  OUT boolean_t *err;
  INOUT int *outLen;
{
    char *start = itlIO_HistoryLoc();
    char *retVal, *curRet;
    boolean_t done = FALSE;

    /*
     * We may not need so much, but in most cases this is a good estimate
     */
    ITL_ALLOC(retVal, char *, strlen(start));
    curRet = retVal;

    while (!done) {
	switch(*start) {
	  case '\\': {
	      start++;
	      switch(*start) {
		case '"' :	*(curRet++) = '"';	break;
		case '\\' :	*(curRet++) = '\\';  	break;
		case 'n':	*(curRet++) = '\n';	break;
		case 't':	*(curRet++) = '\t';	break;
		default:
		  *(curRet++) = '\\';
		  *(curRet++) = *start;
		  break;
	      }
	      start++;
	      break;
	  }
	  case '"':	done = TRUE;  break;
	  case '\0':
	    done = TRUE;
	    itl_Error("EOL reached inside a string: %s\n",
		      itlIO_HistoryLoc());
	    *err = TRUE;
	    break;
	  default:	*(curRet++) = *(start++);	break;
	}
    }
    *(curRet++) = '\0';

    itlIO_AdvanceIndex(start - itlIO_HistoryLoc() + 1);
    if (outLen)
	*outLen = curRet - retVal - 1;
    return(retVal);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_GetSegment:
 *      Read a segment from the input, allocate a string for
 *      it dynamically and return that string terminated by
 *      a NULL.
 *        This routine is used to define script procedures and
 *      to define undo descriptions.  It is up to the caller
 *      to deallocate the string when it is not longer necessary.
 *
 *	If the variable, interpret, is true, then
 *	in the segment that is returned, variables are interpreted
 *	and their current value is inserted in their place.  '@'
 *	characters are replaced by '$'.  If the variable is false the
 *	lines are uninterpreted.
 *
 *   ASSUMPTIONS:
 *      It is perfectly thread safe.
 *      Calls itlIO_GetLine to get one line at a time.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT char *itl_GetSegment(terminator, promptStr, interpret, lengthP)
  IN char *terminator;
  IN char *promptStr;
  IN boolean_t interpret;
  OUT int *lengthP;
{
    char line[MAX_STRING_LENGTH];
    char *outString;
    int curLen = 0;
    int lineLen;
    boolean_t done;
    ITL_UTIL_ALLOC(outString, char *, 1);	/* Initialize  */
    *outString = '\0';
    do {
	itl_PromptUser("%s",promptStr);

	if (interpret) {
	    done = itlIO_GetLine(line, terminator, &lineLen);
	}
	else {
	    done = itlIO_GetUninterpretedLine(line, terminator, &lineLen);
	}
	itl_Trace(ITL_IO_TRACE,
		  "Getsegment[%d] = <%s>(%d)\n",
		  curLen, line, lineLen);
	ITL_UTIL_REALLOC(outString, char *, curLen + lineLen + 2);
	ITL_UTIL_MEMCOPY(line, &outString[curLen], lineLen);
	curLen += lineLen;
	outString[curLen++] = '\n';
	outString[curLen] = '\0';
    } while (!done);
    if (lengthP)
	*lengthP = curLen;
    itl_Trace(ITL_IO_TRACE,
	      "GetSegment is returning a string of length %d:\n%s\n",
	      strlen(outString), outString);
    return(outString);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  coloopInfo_t
 *	Information passed from the parent to the child when
 *	starting a coloop.
 *
 *  CoLoops are implemented so that each thread is told to execute
 *  the routine performIteration and is given a pointer to this
 *  structure for initializing all the thread values.
 *
 *  The coLoop iteration returns its final location in the history buffer
 *  in the last two entries of this structure:
 *	finalIndex, and finalOffset.
 *  If an itl IO error was detected in the thread, this information is passed
 *  on to the parent in the variable IOError.
 */
typedef struct {
    char *varName;		/*  The name of the loop variable           */
    int varVal;			/*  The (integer) value for this iteration  */
    int	numThreadBlocks;	/*  Number of thread blocks in array below  */
    pointer_t *threadBlocks;	/*  Array of extended thread state	    */
    int level;			/*  Nesting Level			    */
    /*  OUT parameters  */
    int finalIndex;		/*  Final Index (after the iteration ended) */
    int finalOffset;		/*  Final Offset (after the iteration ended)*/
    boolean_t IOError;		/*  Was there an ITL IO error in the thread */
} coloopInfo_t;

/*
 *  Function declarations for private functions.  These were
 *  added to make the compiler happy.
 */
PRIVATE void ClientThread	_TAKES((IN pointer_t argP));
PRIVATE void PerformCoIteration _TAKES((IN pointer_t argP));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *     performCoIteration() -- 
 *  Perform one iteration of a coLoop.
 *
 *  All the information necessary for this iteration is passed
 *  in iterationInfoP.  The information there contains all the
 *  initialization values for the thread specific values,
 *  such as the startIndex and offset in the history buffer,
 *  and whether or not this thread is supposed
 *  to perform IO or not.  If the thread is not supposed to
 *  perform IO whenever it needs data and the hostory buffer
 *  is empty, it will wait for data to appear.  Make sure that
 *  at least one thread is told to perform IO.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
PRIVATE void PerformCoIteration(argP)
  IN pointer_t argP;
{
    coloopInfo_t *iterationInfoP = (coloopInfo_t *) argP;
    char loopPrompt[30];
    char *retVal;
    pthread_t pthreadId = pthread_self();

    setIntVariable(iterationInfoP->varName, iterationInfoP->varVal, var_local);

    sprintf(loopPrompt, "[%d] coLoop[%d]=> ",
	    pthread_getunique_np(&pthreadId), iterationInfoP->level);

    /*
     *  Let each thread block do its starting actions in the new thread
     */
    itlThread_PerformStart(iterationInfoP->threadBlocks,
			   iterationInfoP->numThreadBlocks);

    itl_Trace(ITL_VERBOSE_TRACE,
	      "CoIteration ==> Loop Variable (%s) is %d\n",
	      iterationInfoP->varName,
	      iterationInfoP->varVal);

    itl_Trace(ITL_VERBOSE_TRACE, "CoIteration starting.\n");
    /*
     *  We don't need TRY {..} here since we have no cleanup to do
     */
    retVal = itl_CommandLoop(loopPrompt);
    
    if (retVal && 
	(strcmp(retVal, "endLoop") != 0) &&
	(strcmp(retVal, "quit") != 0)) {
	itl_Error("coLoop command ended with %s\n", retVal);
    }

    /*
     *  Inform the parent of the new location in the history
     *  And cleanup.
     */
    itlIO_CurrentHistLoc(&iterationInfoP->finalIndex,
			 &iterationInfoP->finalOffset);
    iterationInfoP->IOError = itlIO_ParsingError() || !retVal;
    itlThread_PerformCleanup(iterationInfoP->threadBlocks,
			     iterationInfoP->numThreadBlocks);

    itl_Trace(ITL_IO_TRACE,
	      "CoIteration returning index %d and offset %d.\n",
	      iterationInfoP->finalIndex,
	      iterationInfoP->finalOffset);
}


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
EXPORT boolean_t HandleIfThenElse(param)
  IN int param;
{
    int cond;
    var_val_t *condVal;
 
    itl_PromptUser("Condition: ");
    condVal = itl_GetVarVal();
    cond = itl_VarValueToBoolean(condVal);
    itl_FreeVarVal(condVal);

    /*
     *  Parse (and execute if needed), the 'then' part
     */
    if (itl_OptionalClause("then=> ", NULL, cond) == -1)
	return(FALSE);

    /*
     *  Parse if it is there (and execute if needed), the 'else' part
     */
    if (itl_OptionalClause("'else' or cmd=> ", "else", !cond) == -1)
	return(FALSE);

    return(FALSE);
}

/*
 *  The '}' command:
 *	Start or Return from a recursive brace nesting.
 *	Increment (or Decrement) the brace level
 */
SHARED boolean_t HandleBraceCmd(param)
  IN int param;
{
    (void) itlIO_IncBraceLevel(param ? 1 : -1);
    return(FALSE);
}

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
EXPORT boolean_t itl_RepeatCommand(param)
  IN int param;
{
    int startIndex;
    int startOffset;
    int cond;
    var_val_t *condVal;

    itlIO_CurrentHistLoc(&startIndex, &startOffset);
    do {
	char *quitCmd;
	itlIO_GotoHistLoc(startIndex, startOffset);
	/*
	 *  No need to catch any exceptions here.
	 */
	quitCmd = itl_CommandLoop("Repeat=> ");
	if (!quitCmd) {
	    return(TRUE);
	} else if (strcmp(quitCmd, "while") != 0) {
	    itl_Error("repeat command terminated with %s\n", quitCmd);
	}
	/*
	 *  If the command loop returns without an error, it returns
	 *  immediately after reading the 'while' command before reading
	 *  the codition (an expression).
	 *  We read the condition here and decide whether we loop or not.
	 */
	itl_PromptUser("Condition: ");
	condVal = itl_GetVarVal();
	cond = itl_VarValueToBoolean(condVal);
	itl_FreeVarVal(condVal);
    } while (cond);
    return(FALSE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  StartCoLooping:
 *	This routine uses pthred Threads and therefore SHOULD not be
 *	called inside a tran-C transaction.  Other routines are
 *	avaialable for that.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
PRIVATE void StartCoLooping(varName, startVal, incVal, maxVal)
IN char *varName;
IN int startVal, incVal, maxVal;
{
    coloopInfo_t iterationInfo, *threadInfoList;
    int multiplier = incVal < 0 ? -1 : 1;
    int numThreads;
    int numIterations = ((maxVal - startVal)/incVal) + 1;
    pthread_t *threadList;
    pointer_t *argList;
    int resumeIndex, resumeOffset;

    if (numIterations <= 0)
	numIterations = 1;

    ITL_UTIL_ALLOC(threadList, pthread_t *,
		   numIterations * sizeof(pthread_t));
    ITL_UTIL_ALLOC(threadInfoList, coloopInfo_t *,
		   numIterations * sizeof(coloopInfo_t));
    ITL_UTIL_ALLOC(argList, pointer_t *,
		   numIterations * sizeof(pointer_t));

    iterationInfo.varName = varName;
    iterationInfo.level = itlIO_NestingLevel() + 1;

    itl_Trace(ITL_VERBOSE_TRACE,
	      "CoLooping from %d to %d by %d\n",
	      startVal, maxVal, incVal);

    startVal *= multiplier;
    incVal *= multiplier;
    maxVal *= multiplier;
    
    for (numThreads=0;
	 startVal <= maxVal || (numThreads == 0);
	 startVal += incVal, numThreads++) {
	iterationInfo.varVal = startVal * multiplier;
	/*
	 * Each thread needs a pointer to its own version of the
	 * thread starting information.
	 */
	threadInfoList[numThreads] = iterationInfo;

	/* 
	 *  Let each thread block do its initializing actions
	 *  in the  current thread
	 */
	itlThread_PerformInit(&threadInfoList[numThreads].threadBlocks,
			      &threadInfoList[numThreads].numThreadBlocks);

	argList[numThreads] = (pointer_t)&threadInfoList[numThreads];
    }
    /*
     *  We now have all the information
     *  Start the threads:
     */
    TRY {
	(*itlIO_CreateSeveralThreadsP)(PerformCoIteration,
				       numThreads,
				       argList);
    } FINALLY {

	/*
	 *  Each thread returns its index to the history buffer at the end of
	 *  its execution.  We must set the values for the calling thread so
	 *  that it does not unnecesarily execute the code again.
	 */
	itlIO_GotoHistLoc(threadInfoList[0].finalIndex,
			  threadInfoList[0].finalOffset);
	if (threadInfoList[0].IOError)
	    itl_SetErrorStatus(TRUE);

	resumeIndex = threadInfoList[0].finalIndex;
	resumeOffset = threadInfoList[0].finalOffset;
	ITL_FREE(threadList);
	ITL_FREE(threadInfoList);
	ITL_FREE(argList);
    } ENDTRY;
    itl_Trace(ITL_IO_TRACE,
	      "Thread resuming at index %d and offset %d.\n",
	      resumeIndex, resumeOffset);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  StartLooping:
 *     Perform a loop: A number of iterations (at least one)
 *     in a single thread.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
PRIVATE void StartLooping(varName, startVal, incVal, maxVal)
  IN char *varName;
  IN int startVal, incVal, maxVal;
{
    int startIndex;
    int startOffset;
    int multiplier = incVal < 0 ? -1 : 1;
    boolean_t cur_Interactive = itlIO_IsInteractive();
    boolean_t doneOnce = FALSE;  /* Force at least one iteration  */
    char loopPrompt[30];
    pthread_t pthreadId = pthread_self();
    
    itlIO_CurrentHistLoc(&startIndex, &startOffset);

    sprintf(loopPrompt, "[%d] loop[%d]=> ",
	    pthread_getunique_np(&pthreadId), itlIO_NestingLevel());
    itl_Trace(ITL_VERBOSE_TRACE,
	      "Looping from %d to %d by %d\n",
	      startVal, maxVal, incVal);

    startVal *= multiplier;
    incVal *= multiplier;
    maxVal *= multiplier;
    
    for (; startVal <= maxVal || !doneOnce;
         startVal += incVal) {
	char *quitStr;
	doneOnce = TRUE;
	itlIO_GotoHistLoc(startIndex, startOffset);
	setIntVariable(varName, startVal * multiplier, var_local);

	TRY {
	    itlIO_IncNestingLevel(1);
	    quitStr = itl_CommandLoop(loopPrompt);
	} FINALLY {
	    itlIO_IncNestingLevel(-1);
	    itl_SetInteractive(cur_Interactive);
	} ENDTRY;

	if (!quitStr) {
	    itl_SetErrorStatus(TRUE);
	    break;
	} else if ((strcmp(quitStr, "endLoop") != 0) &&
	           (strcmp(quitStr, "quit") != 0)) {
	    itl_Error("loop command ended with %s\n", quitStr);
	}
	itl_SetInteractive(FALSE);
    }
    itl_SetInteractive(cur_Interactive);
}

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
EXPORT boolean_t itlIO_StartLooping(varName, startVal, incVal, maxVal,
				    multiThreaded)
  IN char *varName;
  IN int startVal, incVal, maxVal;
  IN boolean_t multiThreaded;
{
    if (incVal == 0) {
	itl_Error("Loops cannot have an increment value equal to 0\n");
	return(TRUE);
    }

    if (multiThreaded)
	StartCoLooping(varName, startVal, incVal, maxVal);
    else
	StartLooping(varName, startVal, incVal, maxVal);
    return(FALSE);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_HandleLoopCommand
 *	Handle the looping script commands:
 */
SHARED boolean_t itlIO_HandleLoopCommand(multiThreaded)
  IN int multiThreaded;
{
  int LoopVar, LoopInc, LoopMax;
  char *loopVarName;
  boolean_t eof;
  int len;
  
  itl_PromptUser("Loop Variable Name: ");
  loopVarName = itl_GetString(&eof, &len);
  if (eof)
      return(TRUE);
  itl_PromptUser("Init Value: ");
  LoopVar = GetInteger();
  itl_PromptUser("Increment: ");
  LoopInc = GetInteger();
  itl_PromptUser("Max Value: ");
  LoopMax = GetInteger();
  
  TRY {
      itlIO_StartLooping(loopVarName, LoopVar, LoopInc,
			 LoopMax, multiThreaded);
  } FINALLY {
      ITL_FREE(loopVarName);
  } ENDTRY;

  return(FALSE);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunnableValue
 *	Return the string the varaible stores, if it has a string
 *	runnable value.  return NULL otherwise.
 */
EXPORT char *itl_RunnableVar(varP)
  IN itl_var_t *varP;
{
    if (!varP)
	return(NULL);
    else if (VAR_TYPE(varP) == itl_procType)
	return(VAR_PROC_VALUE(varP));
    else if (VAR_TYPE(varP) == itl_stringType)
	return (VAR_STR_VALUE(varP));
    else
	return NULL;
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_ExecuteVar
 *	Execute the string stored in the provided script variable.
 *	If the pointer is NULL or the variable pointed to is not
 *	runnable (either a string or a procedure) an error message is
 *	printed out and the function returns.
 *	  Otherwise, it executes the string.
 *
 */
EXPORT void itl_ExecuteVar(ptr)
  IN pointer_t ptr;
{
    itl_var_t *varP = (itl_var_t *)ptr;
    char *cmdStr = itl_RunnableVar(varP);
    pointer_t procContext;

    if (!cmdStr) {
	if (varP)
	    itl_Error("%s is not executable.\n", VAR_NAME(varP));
	else
	    itl_Error("The variable does not exist\n");
	return;
    }

    itlStds_PreProc(varP, &procContext);

    itl_RunFromString(cmdStr);

    itlStds_PostProc(varP, procContext);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_HandleExecProc
 *	Execute a script procedure either in the same thread or
 *	in an indepedent thread.
 */
SHARED boolean_t itlIO_HandleExecProc(param)
  IN int param;
{
    char *procName;
    boolean_t err;
    itl_var_t *procVar;
    pointer_t procContext;
    int startOffset, startIndex;
    int finalOffset, finalIndex;

    itl_PromptUser("Script procedure name: ");
    procName = itl_GetVariableName(&err, NULL);
    procVar = itl_LocateVariable(procName);
    if (!procVar) {
	itl_Error("No such variable: %s\n", procName);
	ITL_FREE(procName);
	return FALSE;
    }
    ITL_FREE(procName);

    switch (param) {
      case 0:
	itl_ExecuteVar((pointer_t)procVar);
	break;
      case 1:
	/*  
	 *  We call itlStds_PreProc so that the input parameters for
	 *  the procedure can be read in the current thread.
	 *  But then, we move the history buffer back so that the
	 *  thread can read them as well.
	 */
	itlIO_CurrentHistLoc(&startOffset, &startIndex);
	
	itlStds_PreProc(procVar, &procContext);
	itlStds_PostProc(procVar, procContext);

	itlIO_CurrentHistLoc(&finalOffset, &finalIndex);

	itlIO_GotoHistLoc(startOffset, startIndex);

	itl_StartIndThread(itl_ExecuteVar, (pointer_t)procVar);

	itlIO_GotoHistLoc(finalOffset, finalIndex);
	break;
    }

    return(FALSE);
}



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_MatchBrace
 *	Match a single brace in the input.
 *	If the correct Brace is found this function returns TRUE
 *	and updates the brace level.  Otherwise, the function returns
 *	FALSE.
 *  Parameters:
 *	braceType:  boolean_t: TRUE for open Brace and FALSE for closing.
 */
EXPORT boolean_t itlIO_MatchBrace(braceType)
  IN boolean_t braceType;
{
    if (braceType) {
	if (itl_MatchCharacter('{')) {
	    itlIO_IncBraceLevel(1);
	    return(TRUE);
	} else {
	    return(FALSE);
	}
    } else {
	if (itl_MatchCharacter('}')) {
	    itlIO_IncBraceLevel(-1);
	    return(TRUE);
	} else {
	    return(FALSE);
	}
    }
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_OptionalClause
 *	Parse an optional clause:
 *
 *  Parameters:
 *	prompt - the prompt string to use to prompt for the keyword
 *	keyWord - The keyword to expect to trigger the clause
 *	flag - Boolean telling whether to execute the clause or not.
 *
 *  Description
 *	This function prompts the given prompt string and then peeks
 *	at the input to see if the keyWord provided is there.  If it
 *	It reads the key word and then expects an open brace {.
 *	After the open brace it either executes all the commands
 *	till the matching brace (if the flag is TRUE) or skips them
 *	(if the flag is FALSE);
 *
 *	If the keyWord is NULL, the function does not look for the
 *	keyword and expects to find the { immediately.
 *
 *	The keyWord is used in generating the command prompt for the
 *	recursive call (if flag is TRUE) by appending to it the string
 *	"=> ".  If the keyword is NULL, the promptStr is used (verbatim)
 *	instead.
 *
 *  Return values:
 *	The function return 0 if it did not find the cluse, 1 if it
 *	found the clause and -1 if there was an error executing the
 *	clause.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT int itl_OptionalClause(promptStr, keyWord, flag)
  IN char *promptStr;
  IN char *keyWord;
  IN boolean_t flag;
{
    int len, retVal = 1;
    boolean_t eof;
    char *nextWord;
    boolean_t found;
    int braceLevel = itlIO_BraceLevel();

    if (itlIO_ParsingError()) {
	return(-1);
    }

    if (keyWord && *keyWord) {
	itl_PromptUser("%s", promptStr);
	nextWord = itl_PeekWord(&len, &eof);
	if (found = (strcasecmp(nextWord, keyWord) == 0)) {
	    /*
	     *  We must skip over the keyword
	     */
	    ITL_FREE(nextWord);
	    nextWord = itl_GetWord(&len, &eof);
	}
 	ITL_FREE(nextWord);
    } else {
	found = TRUE;
    }
    
    if (found) {
	itl_PromptUser("Expecting the required '{': ");
	if (!itlIO_MatchBrace(TRUE)) {
	    itl_Error("Expected a '{' after the %s key word.\n", keyWord);
	    return(-1);
	}
	if (flag) {
	    /*
	     * We have to execute the clause
	     */
	    char *endStr;
	    char *clausePrompt = NULL;
	    TRY {
		if (keyWord) {
		    ITL_ALLOC(clausePrompt, char *, strlen(keyWord)+4);
		    sprintf(clausePrompt, "%s=> ", keyWord);
		    endStr = itl_CommandLoop(clausePrompt);
		} else {
		    /*
		     *  No need to catch exceptions here
		     */
		    endStr = itl_CommandLoop(promptStr);
		}
	    } FINALLY {
		if (clausePrompt)
		    ITL_FREE(clausePrompt);
	    } ENDTRY;

	    if (!endStr) {
		retVal = -1;
	    } else if (strcmp(endStr, "quit") == 0) {
	        /*
		 *  An include file or a procedure was aborted in the middle.
		 *  We need to goto the matching brace before returning
		 */
	        if (itlIO_GotoBraceLevel(braceLevel, FALSE, &eof)) {
		  if (eof) {
		    itl_Error("EOF Encountered searching for closing '}'\n");
		  } else {
		    itl_Error("Error Encountered searching for closing '}'\n");
 		  }
		  retVal = -1;
		}
	    } else if (*endStr != '}') {
		itl_Error("clause ended with %s instead of '}'\n", endStr);
		retVal = -1;
	    }
	    if (braceLevel != itlIO_BraceLevel()) {
		retVal = -1;
	    } 
	} else {
	    /*
	     *  We do not want to evaluate the clause, skip over it.
	     */
	    if (itlIO_GotoBraceLevel(braceLevel, FALSE, &eof)) {
		if (eof) {
		    itl_Error("EOF Encountered searching for closing '}'\n");
		} else {
		    itl_Error("Error Encountered searching for closing '}'\n");
		}
		retVal = -1;
	    }
	}
    } else {
	/*
	 *  Did not find the keyword
	 */
	retVal = 0;
    }
    return(retVal);
}



/*
 *  CreateIndThread
 *	Create an independent thread:
 *	Takes a pointer to a function to be executed in the thread
 *	and a pointer to the argument to be passed to the function
 *	and starts a thread with those parameters.
 */
PRIVATE void CreateIndThread(fn, argP)
  IN void (*fn) _TAKES((void*));
  IN pointer_t argP;
{
    pthread_attr_t threadAttributes;
    pthread_t threadId;
    PRIVATE pointer_t (*func)();
    int retCode;

    *((pointer_t *)&func) = (pointer_t)fn;


    retCode = pthread_attr_create(&threadAttributes);
    ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_ATTR_CREATE);
    pthread_attr_setstacksize(&threadAttributes, 
			      (long) itl_coLoopStackSize);
    retCode = pthread_create(&threadId, threadAttributes, 
			     (pthread_startroutine_t) func,
			     (pthread_addr_t) argP);
    ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_THREAD_CREATE);
    /*
     *  Don't care about these thread attributes, so clean them up.
     */
    pthread_attr_delete(&threadAttributes);
}

/*
 *  CreateSeveralThreads
 *	Create several threads and then join with them.
 *	Takes a pointer to a function to be executed by the threads
 *	and a list of arguments to be passed to the function.  Thread
 *	i will get the i'th argument on the list.
 *	It starts the desired number of threads and then joins with them.
 */
PRIVATE void CreateSeveralThreads(fn, numThreads, argList)
  IN void (*fn) _TAKES((pointer_t));
  IN int numThreads;
  IN pointer_t *argList;
{
    PRIVATE pointer_t (*func)();
    pthread_attr_t threadAttributes;
    pthread_t *threadList;
    pthread_addr_t exitStatus;
    int retCode;
    int i;

    ITL_UTIL_ALLOC(threadList, pthread_t *, numThreads * sizeof(pthread_t));

    *((pointer_t *)&func) = (pointer_t)fn;

    retCode = pthread_attr_create(&threadAttributes);
    ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_ATTR_CREATE);
    pthread_attr_setstacksize(&threadAttributes, 
			      (long) itl_coLoopStackSize);

    for (i = 0; i < numThreads; i++) {
	retCode = pthread_create(&threadList[i], threadAttributes, 
				 (pthread_startroutine_t) func,
				 (pthread_addr_t) argList[i]);
	ITL_CHECK_DCE_CODE(retCode, ITL_PTHR_THREAD_CREATE);
    }

    /*
     *  Don't care about these thread attributes, so clean them up.
     */
    pthread_attr_delete(&threadAttributes);

    for (i = 0; i < numThreads; i++) {
	itl_Trace(ITL_IO_TRACE,
		  "Waiting for thread %d (thread id %d)",
		  i, threadList[i]);
	pthread_join(threadList[i], &exitStatus);
    }
    ITL_UTIL_FREE(threadList);
}

typedef struct {
    FILE *threadIn;
    FILE *threadOut;
    FILE *threadErr;
} clientThreadInfo_t;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  ClientThread - 
 *   Start a client thread - redirecting input and output.
 *   The thread uses its own history buffer by calling itl_LocalCommandLoop().
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
PRIVATE void ClientThread(argP)
  IN pointer_t argP;
{
    clientThreadInfo_t *infoP = (clientThreadInfo_t *) argP;
    itlIO_info_t *threadIO = Current_IO;

    itl_SetInteractive(TRUE);
    threadIO->io_out         = infoP->threadOut;
    threadIO->io_error       = infoP->threadErr;
    threadIO->defaultInput   = infoP->threadIn;

    itl_Trace(ITL_IO_TRACE,
	      "Independent Thread Starting a local commandLoop\n");

    /*
     *  No need to catch exceptions here.
     *  It will never be called inside a tran-C transaction.
     */
    (void) itl_LocalCommandLoop("Client=> ");
    itl_Trace(ITL_IO_TRACE, "Independent Thread completed- closing files\n");

    fclose(infoP->threadIn);
    fclose(infoP->threadOut);
    fclose(infoP->threadErr);
    ITL_FREE(infoP);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_StartIndependentThread:
 *      Start a thread with its own input, output, and error file
 *	descriptors.  The thread also uses its own new local history
 *	buffer.
 *
 *	This routine is used to start a thread on a unix socket
 *	for the itl_client program.
 *
 *	After the thread is created the routine returns and lets
 *	the thread run without waiting for it.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_StartIndependentThread(Fin, Fout, Ferr)
  IN FILE *Fin;
  IN FILE *Fout;
  IN FILE *Ferr;
{
    clientThreadInfo_t *infoP;

    ITL_NEW(infoP, clientThreadInfo_t);
    infoP->threadIn = Fin;
    infoP->threadOut = Fout;
    infoP->threadErr = Ferr;

    itl_Trace(ITL_SERVER_TRACE, "Starting an independent thread\n");
    itl_StartIndThread(ClientThread, (pointer_t)infoP);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunStringInThread:
 *      Run an ITL procedure in an independent thread.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_RunStringInThread(itlProc)
  IN char *itlProc;
{
    itl_var_t *tempVarP;

    tempVarP = itl_SetStrVariable(" itl ThreadName", itlProc, var_local);
    assert(tempVarP);

    itl_StartIndThread(itl_ExecuteVar, (pointer_t)tempVarP);
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_StartServerThread - 
 *    Start the server thread.  This thread sits there listening
 *    on a socket waiting for other processes to attempt to speak
 *    to the test program.  It then creates an independent thread
 *    and instructs it to communicate with the socket.
 *
 *    We create our own copy of th input string portPrefixP by
 *    setting a local variable with that value and passing a
 *    pointer to its value to the procedure.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
EXPORT void itl_StartServerThread(portPrefixP)
  IN char *portPrefixP;
{
    itl_var_t *varP;

    varP = itl_SetStrVariable(" itl PortPrefix", portPrefixP, var_local);
    itl_StartIndThread(itlServer_Start, VAR_VALUE(varP));
}

EXPORT boolean_t itlIO_Init()
{
    pthread_t pthreadId = pthread_self();

    itl_SetErrorStatus(FALSE);
    itl_LastSignalReceived = 0;
    itl_SetIntResult(&threadID, (int) pthread_getunique_np(&pthreadId));
    return(FALSE);
}

/******************************************************************
 ******************************************************************
 ********
 ********    Routines that handle commands related to the
 ********    IO module, such as "path", and "include"
 ********
 ******************************************************************
 ******************************************************************
 */

/*
 * IO_HandleSetPath - set the search path for include files.
 *    The new path is dynamically allocated and NOT freed.
 *    It will be stored on the thread slot and freed when a new
 *    path is defined, or when the thread exits.
 */
EXPORT boolean_t IO_HandleSetPath(param)
  IN int param;
{
    char *newDir;
    boolean_t eof;
    int len;

    itl_PromptUser("New path: ");
    newDir = itl_GetString(&eof, &len);
    itlIO_SetIncludePath(newDir);
    return(FALSE);
}

/*
 *  IO_HandleIncludeFile - Param:
 *    perform the "include" command: Prompt  for a file name and direct
 *    the test program to recieve input from that file.
 *    0: prompt for a file and read from that file
 *    1: read from stdin.
 */
EXPORT boolean_t IO_HandleIncludeFile(param)
  IN int param;
{
    char *fileName;
    int len;
    boolean_t eof;
    boolean_t retVal;
	    
    if (param == 0) {
	itl_PromptUser("File name: ");
	fileName = itl_GetString(&eof, &len);
	if (eof)
	    return(TRUE);
	TRY {
	    retVal = itl_RunFromFile(fileName, "Include=> ");
	} FINALLY {
	    ITL_FREE(fileName);
	} ENDTRY;
    } else {
	retVal = itl_RunFromInput("Input=> ");
    }
    return(FALSE);
}

/*
 *  itlIO_ParsingError - return whether an IO parsing error has occurred.
 *  When such an error occurs, the routine itl_SetErrorStatus(TRUE)
 *  is called.
 */
EXPORT boolean_t itlIO_ParsingError()
{
    itl_var_t *varP = itl_LocateVariable(" itl ParsingError");
    return(varP && VAR_INT_VALUE(varP));
}

/*
 *  itl_SetErrorStatus
 *     Set or reset the status of the parsing error.
 *     This routine is used when an error occurs while parsing a command
 *     and the user wishes the error to propogate to the outer levels
 *     until the program gets to an interactive layer.
 */
EXPORT void itl_SetErrorStatus(flag)
  IN boolean_t flag;
{
    itl_SetIntVariable(" itl ParsingError", flag, var_local);
}

/*
 *  itl_HandleSignal
 *	Handle an incoming signal
 */
EXPORT void itl_HandleSignal(signalCode)
  IN int signalCode;
{
    itl_LastSignalReceived = signalCode;
}

/*
 *  randomSeed command:
 *	Set the seed for the random number generator
 */
EXPORT boolean_t itlIO_RandomSeed(param)
  IN int param;
{
    itl_PromptUser("Seed: ");
    srand48((long) itl_GetInteger(NULL));
    return(FALSE);
}

/*
 *  itlIO_HandleHistoryCommand
 *     Display the last n lines in the history buffer.
 */
EXPORT boolean_t itlIO_HandleHistoryCommand(param)
  IN int param;
{
    itl_var_t *tempVar;
    int numLines = 20;

    if ((tempVar = itl_LocateVariable("history")) &&
	(VAR_TYPE(tempVar) == itl_intType)) {
	numLines = VAR_INT_VALUE(tempVar);
    }
    itlIO_PrintHist(numLines);
    return(FALSE);
}
