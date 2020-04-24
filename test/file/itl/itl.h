/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl.h,v 1.1.115.1 1996/10/17 18:26:58 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
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
 * $Log: itl.h,v $
 * Revision 1.1.115.1  1996/10/17  18:26:58  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:32  damon]
 *
 * Revision 1.1.110.1  1994/02/04  20:45:00  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:22  devsrc]
 * 
 * Revision 1.1.108.1  1993/12/07  17:44:21  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:25:00  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:42:20  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:32:06  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:54:42  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:06:02  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  19:07:36  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:44:32  kissel]
 * 
 * Revision 1.1.3.2  1993/02/05  19:00:40  kissel
 * 	Add definitions for TRUE and FALSE for HPUX.
 * 	[1993/02/05  18:59:12  kissel]
 * 
 * Revision 1.1.2.6  1993/01/11  18:30:24  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:54  htf]
 * 
 * Revision 1.1.2.5  1992/12/09  21:32:26  jaffe
 * 	Transarc delta: fred-ot6132-check-for-short-io 1.1
 * 	  Selected comments:
 * 	    To fix the read and write commands to check for short I/O.  Previously the
 * 	    user had to to this in the ITL script by looking at the CommandResult
 * 	    variable.  This option is still available if the user sets verifyStatus to
 * 	    false.
 * 	    Moved the CALLOC macro from itl_utils.h to here.  It was commented that this
 * 	    should be done, so I did it.
 * 	[1992/12/07  13:31:56  jaffe]
 * 
 * Revision 1.1.2.4  1992/11/18  21:45:34  jaffe
 * 	Transarc delta: fred-itl-ofer-fixes-10-30 1.1
 * 	  Selected comments:
 * 	    Incorporates Ofer's changes that permit procedures to have parameters
 * 	    passed to them, and also includes the vector and valList data types.  The
 * 	    vector data type is needed for DFS testing.  It allows us to manipulate
 * 	    binary data.
 * 	    Added new type definitions.
 * 
 * 	@TRANSARC_COPYRIGHT@
 * 
 * 	$TALog: itl.h,v $
 * 	Revision 1.32  1994/11/01  22:29:06  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.31 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.31  1993/08/24  15:18:26  bwl
 * 	More changes for HP port.  Milestone:  got the low tests built;
 * 	can run cthon on DFS.  (But not on glued UFS.)
 * 
 * 	Changes for HP port.
 * 	[from r1.30 by delta bwl-o-db3961-port-103-to-HP, r1.11]
 * 
 * Revision 1.30  1993/06/17  19:54:46  shl
 * 	[merge of changes from 1.15 to 1.29 into 1.27]
 * 
 * Revision 1.27  1993/06/04  14:14:45  shl
 * 	[merge of changes from 1.21 to 1.22 into 1.24]
 * 
 * Revision 1.24  1993/06/03  19:53:33  shl
 * 	[merge of changes from 1.14 to 1.16 into 1.21]
 * 
 * Revision 1.21  1993/06/03  15:28:42  fred
 * 	[merge of changes from 1.19 to 1.20 into 1.18]
 * 
 * Revision 1.18  1993/04/29  21:28:07  jess
 * 	The semantics of isDisplayLine is not what we want for auto_spoke.
 * 	This delta add a function "getDisplayLine" in itl_global.c file.
 * 
 * 	export getDisplayLines.
 * 	[from r1.15 by delta jess-db3335-add-auto_spoke, r1.7]
 * 
 * Revision 1.15  1993/03/30  22:16:04  jess
 * 	This delta fixes cached cds data problem (db3306 fixed the cds name entry
 * 	problem) that causes a spoke get wrong binding infor. The fix is that
 * 	just before the ns_Connection call a rpc call is added to update the
 * 	local cached rpc data entry. This delta also adds a pair of function
 * 	which will allow different ITL components to register their cleanup
 * 	functions that will be executed at the time itl_CleanupItl function
 * 	executed and to unregister.
 * 
 * 	Export the pair of register functions.
 * 	[from r1.14 by delta jess-db3357-fix-itl-spoke-2, r1.1]
 * 
 * Revision 1.14  1993/03/08  13:51:46  fred
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
 * 	[from r1.13 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.13  1993/03/01  14:48:18  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.12 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 6.2  1993/01/28  22:16:13  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Conditionalize the inclusion of 'memory.h' to avoid conflicts
 * 	with gcc built-in function declarations.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * 	$EndTALog$
 * 
 * Revision 1.46  1992/04/27  21:46:04  oz
 * 	Continued modifications
 * 
 * 	Added itl_DisplayVarVal
 * 	[from revision 1.43 by delta oz-2495-ITL:Add-display-upcall-to-var-types, revision 1.2]
 * 
 * Revision 1.43  1992/04/23  21:12:23  oz
 * 	The third parameter to ITL_GET_STRING should be a pointer
 * 
 * 	[from revision 1.40 by delta oz-2178-Add_Get_Int_Get_String_macros, revision 1.4]
 * 
 * Revision 1.40  1992/04/07  19:14:04  oz
 * 	Modify the ITL_GET_STRING macro to allow the length to be optional
 * 
 * 	Modify the ITL_GET_STRING macro to allow the length to be optional
 * 	[from revision 1.37 by delta oz-2178-Add_Get_Int_Get_String_macros, revision 1.3]
 * 
 * Revision 1.37  1992/03/30  15:43:22  oz
 * 	Add a new function to itl: itl_PeekVarVal
 * 	It is a necessary function in order to allow defining arbitrary
 * 	complex user types for itl variables (which the sfs test intends to do)
 * 
 * 	Declare the new function.
 * 	[from revision 1.36 by delta oz-2193-Add:itl_PeekVarVal-function, revision 1.1]
 * 
 * Revision 1.36  1992/03/29  14:50:22  oz
 * 	Add Forgotten prompt string from ITL_GET_INTEGER macro
 * 
 * 	Add Forgotten prompt string from ITL_GET_INTEGER macro
 * 	[from revision 1.35 by delta oz-2178-Add_Get_Int_Get_String_macros, revision 1.2]
 * 
 * Revision 1.35  1992/03/28  22:03:11  oz
 * 	Add two useful macros
 * 
 * 	Added ITL_GET_INTEGER and ITL_GET_STRING as shorthand for
 * 	itl_promptUser(..)  followed by itl_GetInteger(..)  or itl_GetString(..)
 * 	It is less general since the prompt is a string and not a var arg
 * 	printf like statement.  But in most cases this is more useful.
 * 	[from revision 1.34 by delta oz-2178-Add_Get_Int_Get_String_macros, revision 1.1]
 * 
 * Revision 1.34  1992/03/27  21:15:39  oz
 * 	Changes which were made before 1.0.1 and were not supposed to be
 * 	incorporated in 1.0.1 and therefore were not readied and were
 * 	not moved to CML with the rest of the file.
 * 
 * 	These changes add tranC support for ITL as well as handling
 * 	transaction aborts (exceptions) in the lower level io modules.
 * 
 * 	See General Comment Above
 * 	[from revision 1.33 by delta oz-2172-itl-support-tranc, revision 1.1]
 * 
 * Revision 1.30  92/02/07  14:22:40  shi
 * 	added a new function itl_LookupTable
 * 
 * Revision 1.29  92/02/07  11:23:05  oz
 * 	 - isDebug  ==> itlIO_IsDebug
 * 	 - setDebug ==> itlIO_SetDebug
 * 	[92/02/06  15:05:11  oz]
 * 
 * 	- DebugLevel no longer exists - replaced by itl_traceLevel
 * 	[92/02/06  13:08:30  oz]
 * 
 * Revision 1.28  92/02/05  19:50:15  oz
 * 	 - New function:  itl_GetEnv
 * 
 * Revision 1.27  92/02/04  22:16:47  oz
 * 	 - SetStrResult: does not take the length as parameter
 * 	[92/02/03  14:08:00  oz]
 * 
 * Revision 1.26  92/02/02  09:21:57  oz
 * 	 - added: ITL_EXCEPTION_TRACE
 * 	[92/01/31  10:43:29  oz]
 * 
 * Revision 1.25  92/01/24  10:30:46  oz
 * 	- new function:
 * 		itl_OptionalClause
 * 	[92/01/24  09:41:40  oz]
 * 
 * Revision 1.24  92/01/23  18:12:45  oz
 * 	 - Corrected declarations of functions with no params.
 * 	 - New functions:
 * 		itl_MatchBrace
 * 		itl_PeekString
 * 	[92/01/23  11:33:10  oz]
 * 
 * Revision 1.23  92/01/20  08:30:08  oz
 * 	 - Added explanations to the various print functions.
 * 	 - Renamed itl_PrintAdvise to be itl_PrintAdviseP
 * 	 - Modified the itl_ memory functions (alloc, new, ...)
 * 	   to use itl_Trace instead of itl_TracePrintf
 * 	[92/01/19  08:48:57  oz]
 * 
 * 	 - Defined itl_AdviseUser and itl_fAdviseUser
 * 	 - Defined itl_Advise using itl_fMessage
 * 	[92/01/18  18:05:52  oz]
 * 
 * 	 - Defined:
 * 		itl_Message
 * 	        itl_fMessage
 * 	        itl_fTrace
 * 		itl_TracePrintf using ifl_fMessage
 * 		itl_PrintError
 * 	        itl_fPrintError
 * 	        itl_PrintErrMsg using itl_fPrintError
 * 		itl_Error 	to be itl_PrintError
 * 	        itl_PrintMsg 	using itl_fMessage
 * 	[92/01/18  12:57:43  oz]
 * 
 * 	 - itlIO_PrintPrompt changed
 * 	 - new function: itlIO_DonePrompting.
 * 	[92/01/12  09:47:22  oz]
 * 
 * 	 - Adding var-args routine for printing and prompting.
 * 	[92/01/11  09:23:55  oz]
 * 
 * Revision 1.22  92/01/16  16:41:00  oz
 * 	 - Added new tracing level: ITL_THREAD_SLOT_TRACE
 * 	 - Shifted all trace level by 8 bits.
 * 	[92/01/15  10:16:23  oz]
 * 
 * Revision 1.21  92/01/10  16:56:56  oz
 * 	 - Changed COMMAND_MODE_NORMAL    to 0x100
 * 	   and     COMMAND_MODE_INTERFACE to 0x200
 * 
 * Revision 1.20  92/01/09  10:46:52  oz
 * 	 - replaced ^G by \007 in printErrMsg
 * 
 * Revision 1.19  92/01/08  15:16:18  oz
 * 	 correct extern decl of itl_AddModuleCmds
 * 	[92/01/08  14:57:11  oz]
 * 
 * 	 - two new command modes: COMMAND_MODE_TRAN1, COMMAND_MODE_TRAN2
 * 	 - New function: itl_RunStringInThread:
 * 	[92/01/06  09:13:05  oz]
 * 
 * Revision 1.18  92/01/02  09:58:09  oz
 * 	 - Command mode all now sets all 32 bits.
 * 	 - new commandL itl_AddModuleCmds
 * 	[91/12/26  12:21:42  oz]
 * 
 * Revision 1.17  91/12/20  11:00:17  oz
 * 	Include utils/util.h
 * 
 * Revision 1.16  91/12/12  13:55:05  oz
 * 	- new function: itlIO_PrintPrompt
 * 	[91/12/10  19:41:35  oz]
 * 
 * Revision 1.15  91/11/22  09:09:58  oz
 * 	- Corrected variableVals_t
 * 	- Added #define for the old var type defs
 * 
 * Revision 1.14  91/11/21  15:32:30  oz
 * 	- define the new value types
 * 	[91/11/11  16:36:18  oz]
 * 
 * Revision 1.13  91/10/23  16:45:21  oz
 * 	- New commands:
 * 		itl_GetThreadState
 * 		itl_RestoreThreadState
 * 		itl_FreeThreadState
 * 
 * Revision 1.12  91/10/21  11:47:57  oz
 * 	Renamed isInterface	 =>  itlIO_IsInterface
 * 		setInterfaceMode =>  itlIO_SetInterfaceMode
 * 
 * Revision 1.11  91/10/18  18:27:30  oz
 * 	Added command modes.
 * 
 * Revision 1.10  91/10/17  15:04:52  fred
 * 	Added the ITL_UTIL_NEW macro.
 * 
 * Revision 1.9  91/10/17  10:50:34  oz
 * 	- Renamed
 * 		specialVar_t    =>  itl_specialVar_t;
 * 		itlIO_IsInterface	=>   isInterface
 * 		itlIO_SetInterfaceMode	=>   setInterfaceMode
 * 		itlIO_IsDisplayLines	=>   isDisplayLines
 * 		itlIO_SetDisplayLines	=>   setDisplayLines
 * 		itlIO_ToggleDisplayLines	=>   toggleDisplayLines
 * 		itlIO_IsDebug		=>   isDebug
 * 		itlIO_SetDebug		=>   setDebug
 * 	 - Removed  - outputToScreen
 * 	 - Added   -
 * 		itl_LocateVariable
 * 		itl_ShowVarVal
 * 		itl_SetVariable
 * 		itl_GetBoolean
 * 		itl_SetIntResult
 * 		itl_SetStrResult
 * 		itl_DumpBytes
 * 		itl_IntToStaticStr
 * 		itl_CheckBdeStatus
 * 		itl_AnsiTimeToStr
 * 
 * Revision 1.8  91/10/03  15:41:44  oz
 * 	New Command: toggleDisplayLines
 * 
 * Revision 1.7  91/10/03  12:54:15  oz
 * 	Added comment block for itl_CommandLoop
 * 	ilt_CommandLoop and itl_LocalCommandLoop now return strings
 * 
 * Revision 1.6  91/10/02  11:58:07  oz
 * 	Renamed itl_CleaupItl to be itl_CleanupItl
 * 	Added comments to itl_AddCmdTable
 * 
 * Revision 1.5  91/09/27  13:38:23  oz
 * 	Fixed comment before itl_CommentLine
 * 
 * Revision 1.4  91/09/22  10:08:21  oz
 * 	Fixed itl_Advise, itlPrintAdvise did not have ()
 * 	New Commands:
 * 	  - itl_EchoLine
 * 	  - itl_CommentLine
 * 
 * Revision 1.3  91/09/16  11:51:51  mss
 * 	Made interface decl match actual proc
 * 	(itl_SetStringVariable => itl_SetStrVariable)
 * 
 * Revision 1.2  91/09/12  17:34:59  oz
 * 	Cleaning up the REC test program and making it a general
 * 	library:  Many changes, mostly cosmetic:  changing the names
 * 	to follow the trasnarc standard.
 * 
 * Revision 1.1  91/08/22  17:19:35  mss
 * 	Modified REC tester to make separate library
 * 
 * $EndLog$
 */
/*
 * itl.h -- 
 *    Common header file included by components of itl programs.
 */

#ifndef TRANSARC_ITL_ITL_H
#define TRANSARC_ITL_ITL_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/signal.h>

#ifndef __GNUC__
#include <memory.h>       /* built-ins in gcc */
#endif   /* __GNUC__ */

#include <string.h>
#include <utils/itl/itl_stds.h>

#include <utils/cht_hash.h>            /* Defines CHT_GENERATE_ROUTINES */ 

extern FILE *itl_CurrentOutput _TAKES((void));
#define Cur_Out itl_CurrentOutput()

extern FILE *itl_CurrentErr _TAKES((void));
#define Cur_Err itl_CurrentErr()

#ifdef __hpux
  #ifndef TRUE
    #define TRUE 1
  #endif /* TRUE */
  #ifndef FALSE
    #define FALSE 0
  #endif /* FALSE */
#endif /* __hpux */

/*
 * Variable: itl_traceLevel
 *
 * Description:
 *     itl_traceLevel -- variable that defines the level of tracing in a
 *     REC application.
 *     See the definitions of the various levels above.
 */
extern unsigned long itl_traceLevel;
#define DebugLevel itl_traceLevel

/*  Itl Trace Levels  */
#define ITL_DEBUG_TRACE          0x00000100
#define ITL_VERBOSE_TRACE        0x00000200
#define ITL_MEMORY_TRACE         0x00000400
#define ITL_THREAD_SLOT_TRACE    0x00000800
#define ITL_FILE_TRACE		 0x00001000
#define ITL_VAR_TRACE            0x00002000
#define ITL_LOCK_TRACE           0x00004000
#define ITL_IO_DEBUG             0x08000000
#define ITL_IO_TRACE             0x00008000
#define ITL_EXP_TRACE            0x00010000
#define ITL_SERVER_TRACE         0x00040000
#define ITL_EXCEPTION_TRACE	 0x00080000
#define ITL_WARNING_TRACE	 0x00100000
#define ITL_ALL_TRACE            0xffffffff

#define ITL_NO_TRACING         0x0L
#define ITL_ALL_TRACING	TRACE_ALL
#define ITL_FUNC_ENTRY_TRACE   TRACE_ENTRY
#define ITL_FUNC_EXIT_TRACE    TRACE_ENTRY
#define ITL_FUNC_PARMS_TRACE   TRACE_PARAM

#define ITL_MAX_INDEXSIZE    10

/*
 *  Command modes are used to enable and disable
 *  commands.  Command may be compiled into the
 *  table and disabled and only enabled in certain
 *  conditions.  For example, you may have all the
 *  code for interface testing but disable the code
 *  except during interface testing not to clutter
 *  the command space.
 *
 *  Currently there are 32 bits in the command mode.
 *  the higher 24 bits are reserved for itl resident
 *  programs such as IO, VOL and some other libraries
 *  that are general enough that other tools may want
 *  to link them in.  The lower 8 bits are for individual
 *  test programs.  That should be enough.
 */
#define COMMAND_MODE_IO1	0x10000000
#define COMMAND_MODE_IO2	0x20000000
#define COMMAND_MODE_IO3	0x40000000
#define COMMAND_MODE_IO4	0x80000000
#define COMMAND_MODE_TID1	0x01000000
#define COMMAND_MODE_TID2	0x02000000
#define COMMAND_MODE_TMXA1	0x04000000
#define COMMAND_MODE_TMXA2	0x08000000
#define COMMAND_MODE_VOL1	0x00100000
#define COMMAND_MODE_VOL2	0x00200000
#define COMMAND_MODE_REC1	0x00400000
#define COMMAND_MODE_REC2	0x00800000
#define COMMAND_MODE_TC1	0x00010000
#define COMMAND_MODE_TC2	0x00020000
#define COMMAND_MODE_LOG1	0x00001000
#define COMMAND_MODE_LOG2	0x00002000
#define COMMAND_MODE_LOG3	0x00004000
#define COMMAND_MODE_LOG4	0x00008000
#define COMMAND_MODE_TRAN1	0x00000400
#define COMMAND_MODE_TRAN2	0x00000800

#define COMMAND_MODE_INTERFACE  0x00000200
#define COMMAND_MODE_NORMAL     0x00000100
#define COMMAND_MODE_ALL        0xffffffff
extern unsigned long CommandMode;

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * itl_RegisterCleanupFun
 *     This routin is used to register the user defined cleanup function
 *     as a node in the link list that will be executed by the function
 *     itl_CleanupItl().
 */

IMPORT void itl_RegisterCleanupFun _TAKES((void (*)(), char *));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * itl_UnRegisterCleanupFun
 *     This routin is used to unregister the user defined cleanup function
 *     in case the cleanup will be harmful to the component if done by
 *     itl_CleanupItl().
 */

IMPORT void itl_UnRegisterCleanupFun _TAKES((void (*)(), char *));



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_Message
 *	Write the provided format to Cur_Out and flushes Cur_Out.
 *	The output will be prefixed by the thread id, the time
 *	elapsed since the beginning of the program and the string
 *	" --- ".  E.g., itl_Message("Hello %s\n", "there") on
 *	thread 7 results in:
 *   [7] (0:26) --- Hello there
 */
IMPORT void itl_Message _TAKES((char *, ...));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_fMessage
 *	Similar to the itl_Message but takes the file as a parameter.
 *	It is provided mostly for backwards compatability (in order
 *	to print out calls using the macro itl_PrintMsg((...))  ).
 */
IMPORT void itl_fMessage _TAKES((FILE *, char *, ...));

#define itl_PrintMsg(args)  itl_fMessage args

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_Trace
 *	Write the provided format to Cur_Err and flushes Cur_Err
 *	only if the current ITL trace level and the provided mask
 *	(the first parameter) have a bit in common.
 *	The string is preceded by the thread Id, the elapsed time
 *	since the beginning of the program (in hours (only if not
 *	zero), minutes and seconds.  This is followed by a three
 *	letter code which depends on the index of the first bit set
 *	in the mask:
 *	The three letters are 'open brace' 'char' 'close brace':
 *
 *	 1:<-> [-] (-) {-}    5:<+> [+] (+) {+} 
 *	 9:<*> [*] (*) {*}   13:<%> [%] (%) {%} 
 *	17:<#> [#] (#) {#}   21:<|> [|] (|) {|} 
 *	25:<_> [_] (_) {_}   29:<=> [=] (=) {=}
 *
 *	So, if you turn on tracing and are interested in output for
 *	a particular trace bit, you can use grep on the output.
 */
IMPORT void itl_Trace _TAKES((unsigned int, char *, ...));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_fTrace
 *	Similar to the itl_Trace but takes the file as a parameter.
 */
IMPORT void itl_fTrace _TAKES((FILE *, unsigned int, char *, ...));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_PromptUser
 *	Prompt the user for input.
 *	The prompt is displayed as specified on Cur_Out.
 *	The thread id is not added (nor is the time).
 *	The prompt is displayed only if the program is in interactive
 *	mode and there is no input in the history buffer.  Also,
 *	at most one prompt will be displayed for each line of input
 *	so if multiple threads are active, the user will be prompted
 *	only once.
 *	Do not use this command for general printing (see itl_Advise).
 *
 *	After the prompt string is displayed the test program stores
 *	it in a history specifi global variable.  If the user enters an
 *	empty line, the last prompt is redisplayed.
 */
IMPORT void itl_PromptUser _TAKES((char *, ...));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_fPromptUser
 *	Similar to itl_PromptUser but takes the file as a parameter.
 */
IMPORT void itl_fPromptUser _TAKES((FILE *,char *, ...));

#define	itl_Prompt(args)  itl_fPromptUser args


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_PrintAdviseP
 *	Should advise be printed at this moment?
 */
extern boolean_t itl_PrintAdviseP _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_AdviseUser
 *	Advise the user: Print the given string only if
 *	we are in interactive mode and we are at the end
 *	of the history buffer.
 *	The output is printed to Cur_Out and Cur_Out is flushed.
 *	Output of advise is not limited to one per line of input
 *	as itl_PromptUser.
 *	The purpose for this command is to provide the interactive
 *	use for more information while keeping batch running terse.
 *	E.g., in REC when the client initializes a page the test
 *	program advises of the page size.  Similarly, when a transaction
 *	is begun in ITL the use is advised of the ID of the new transaction.
 *	If that information is needed in batch mode, it is available in
 *	script variables (which are set by the program) and can be
 *	explicitly printed using the echo command.
 */
IMPORT void itl_AdviseUser _TAKES((char *, ...));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_fAdviseUser
 *	Same as itl_AdviseUser but takes a file pointer as well.
 */
IMPORT void itl_fAdviseUser _TAKES((FILE *,char *, ...));

#define	itl_Advise(args) itl_fAdviseUser args

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_PrintError
 *	Display an error message to Cur_Err.
 *	It takes a format string and its arguments and prints
 *	them to Cur_Err prefixing them with the current thread
 *	id, the current time (hr:mn:sec), and the three letter
 *	code "###".
 *	This function also sets the thread global Error Status.
 */
IMPORT void itl_PrintError _TAKES((char *, ...));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_fPrintError
 *	Similar to itl_PrintError but takes the FILE pointer instead
 *	of writing to Cur_Err.  It was added for backwards compatability.
 */
IMPORT void itl_fPrintError _TAKES((FILE *, char *, ...));

/*
 *  itl_PrintErrMsg -
 *	The old style macro which takes (()) - Use the new one: itl_Error
 */
#define itl_PrintErrMsg(args)  itl_fPrintError args

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_Error
 *	Print an error message to Cur_Err (using itl_PrintError)
 *	and set the thread global error status.
 */
#define itl_Error itl_PrintError

/*
 *   Data structures for script variables.
 */

/*  variableVals_t
 *    An enumerated type for specifying a type of script variable.
 */
typedef int variableVals_t;
#define int_type	itl_intType
#define string_type	itl_stringType
#define proc_type	itl_procType


/*
 *    scope_t - the scope of a variable
 */
typedef enum {
    var_global,  /*  The variable is global  		*/
    var_local,   /*  It is local  			*/
    var_same,    /*  Don't care - leave it as it is 	*/
    var_default  /*  Use the default value:
		  *   - global if in the main thread
		  *   - local otherwise
		  */
    } scope_t;

/*  var_t
 *     A type of a script variable record.
 */
typedef struct {
    variableVals_t type;
    pointer_t value;
    int size;
} var_val_t;

#define VALUE_TYPE(_varValP)  ((_varValP)->type)
#define VALUE_VAL(_varValP)   ((_varValP)->value)
#define VALUE_SIZE(_varValP)  ((_varValP)->size)
#define INT_VALUE_VAL(_val) (*(int *)VALUE_VAL(_val))
#define STR_VALUE_VAL(_val) ((char *)VALUE_VAL(_val))
#define PROC_VALUE_VAL(_val) itlStds_ProcString(_val)

typedef enum {
    itl_var_threadLocal = 9,
    itl_var_global = 11,
    itl_var_procLocal = 33
} itl_varState_t;

typedef struct {
    char *name;
    pthread_key_t threadVal;
    var_val_t *val;
    itl_varState_t state;    
    cht_link_t chtLink;			/*  For CHT ADT */
} itl_var_t;

#define VAR_VALUE_REC(_varP)  itlExp_VarCurVal((_varP))
#define VAR_VALUE(_varP)      (VALUE_VAL(VAR_VALUE_REC(_varP)))
#define VAR_TYPE(_varP)       (VALUE_TYPE(VAR_VALUE_REC(_varP)))
#define VAR_SIZE(_varP)       (VALUE_SIZE(VAR_VALUE_REC(_varP)))
#define VAR_STR_VALUE(_varP)  ((char *)VAR_VALUE(_varP))
#define VAR_INT_VALUE(_varP)  (*((int *)VAR_VALUE(_varP)))
#define VAR_PROC_VALUE(_varP) itlStds_ProcString(VAR_VALUE_REC(_varP))
#define VAR_NAME(_varP)       ((_varP)->name)

SHARED int itl_intType;
SHARED int itl_booleanType;
SHARED int itl_stringType;
SHARED int itl_procType;

/*
 *  A specialVar_t: Special types of variables which
 *  are automatically set by the test program.
 */
typedef struct {
    char *varName;   /*  Name of a variable that holds the name to use.  */
                     /*  If this is not a valid variable or if it is     */
                     /*  not a string - use the default name.		 */
    char *Default;   /*  Variable name  */
}   itl_specialVar_t;

extern itl_specialVar_t commandResult;  /* For storing the value returned  */
                                        /* by the last command             */
extern itl_specialVar_t currentFileName;/* The current File Name  */


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itlExp_VarCurVal - The current value of a variable.
 *
 *     If the variable has a local value, the local value is returned.
 *     Otherwise, the list of the variable values of the parent is 
 *     searched and finally the global value is used.
 *     If the variable does not have a local value, the local value is
 *     assigned to be the new value found in the search and that value
 *     is returned.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern var_val_t *itlExp_VarCurVal _TAKES((IN itl_var_t *varP));

/*  Some Global Variables:  */
extern char *itlName;

extern boolean_t isVerbose _TAKES((void));
extern boolean_t setVerbose _TAKES((IN boolean_t flag));

extern boolean_t itlIO_IsInterface _TAKES((void));
#define isInterface() itlIO_IsInterface()

extern boolean_t itlIO_SetInterfaceMode _TAKES((IN boolean_t flag));
#define setInterfaceMode(flag) itlIO_SetInterfaceMode(flag)

extern boolean_t isDisplayLines     _TAKES((void));
#define itlIO_IsDisplayLines() isDisplayLines()
extern boolean_t setDisplayLines    _TAKES((IN boolean_t flag));
extern boolean_t getDisplayLines    _TAKES((void));
#define itlIO_SetDisplayLines(A) setDisplayLines(A)
extern boolean_t toggleDisplayLines _TAKES((IN int param));
#define itlIO_ToggleDisplayLines() toggleDisplayLines()

/*
 *  Is debug mode on?
 *	This is left here for backwards compatability.
 */
extern boolean_t itlIO_IsDebug _TAKES((void));
#define isDebug() itlIO_IsDebug()

/*
 *  itlIO_SetDebug
 *	Set the trace level for the ITL test program
 */
extern boolean_t itlIO_SetDebug _TAKES((IN int flag));
#define setDebug(A) itlIO_SetDebug(A)

/*
 *   Set and reset interactive mode.
 */
extern boolean_t itl_SetInteractive _TAKES((IN boolean_t flag));

typedef struct itl_commandTable_struct{
    char *command;
    boolean_t (*function)();
    int parameter;
    boolean_t breakCom;
    unsigned long mode;
    char *description;
    char *class;
    char *help;
    struct itl_commandTable_struct * nextCmdP;
} itl_commandTable_t;



/*
 *  itlIO_IsInteractive:
 *	Are we now in interactive mode or not?
 */
extern boolean_t itlIO_IsInteractive _TAKES((void));


/*
 *  itlIO_PrintPrompt
 *	return TRUE iff the current thread should prompt.
 *	if TRUE is returned, a latch is kept which should be
 *	freed by calling itlIO_DonePrompting.
 */
extern boolean_t itlIO_PrintPrompt _TAKES((void));

/*
 *  itlIO_DonePrompting
 *	When the user is is allowed to prompt, the call to
 *	itlIO_PrintPrompt leaves the promptLock locked.
 *	This realeases that lock.
 */
extern void itlIO_DonePrompting _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itlIO_LastPromptStr
 *	Return a pointer to the last prompt string associated
 *	with the history buffer used by this thread.
 */
extern char *itlIO_LastPromptStr _TAKES((void));



#define ITL_ALLOC ITL_UTIL_ALLOC

#define ITL_UTIL_ALLOC(_varP, _type, _size)                             \
      MACRO_BEGIN                                                       \
         (_varP) = (_type) malloc(_size);				\
         if (!(_varP)) {						\
	   itl_MemoryFailure(UTIL_STRING(ITL_UTIL_ALLOC), __FILE__,     \
			     __LINE__);	                                \
         }								\
         itl_Trace(ITL_MEMORY_TRACE,                                    \
                   "%x: Allocating %d for %s of type %s\n",             \
		   (_varP), (_size), UTIL_STRING(_varP), UTIL_STRING(_type)); \
      MACRO_END

#define ITL_CALLOC ITL_UTIL_CALLOC

#define ITL_UTIL_CALLOC(_varP, _type, _size)                            \
    MACRO_BEGIN								\
        (_varP) = (_type) calloc((_size), sizeof(char));		\
        if (!(_varP)) {							\
	    itl_MemoryFailure(UTIL_STRING(ITL_UTIL_CALLOC), __FILE__,   \
			      __LINE__);	                        \
        }								\
        itl_Trace(ITL_MEMORY_TRACE,					\
                   "%x: Allocating %d for %s of type %s\n",             \
		   (_varP), (_size), UTIL_STRING(_varP), UTIL_STRING(_type)); \
    MACRO_END

#define ITL_NEW ITL_UTIL_NEW

#define ITL_UTIL_NEW(_varP, _type)                                      \
      MACRO_BEGIN                                                       \
        (_varP) = (_type *) malloc(sizeof(_type));	                \
        if (!(_varP)) {						        \
	   itl_MemoryFailure(UTIL_STRING(ITL_UTIL_NEW), __FILE__,       \
			     __LINE__);	                                \
        }								\
        itl_Trace(ITL_MEMORY_TRACE,                                     \
		  "%x: Allocating %d for %s of type %s\n",              \
		  (_varP), sizeof(_type), UTIL_STRING(_varP),           \
		  UTIL_STRING(_type));                                  \
      MACRO_END


#define ITL_FREE ITL_UTIL_FREE

#define ITL_UTIL_FREE(_varP)        \
      MACRO_BEGIN                   \
         itl_Trace(ITL_MEMORY_TRACE,\
		   "%x: Freeing.\n",\
		   (_varP));        \
         free(_varP);               \
      MACRO_END

#define ITL_MEMCOPY ITL_UTIL_MEMCOPY

/*
 *  SUN doesn't support memmove(), so we define it as bcopy().  No
 *  it can't be memcpy() cause although SUN has this, it does not
 *  guarantee overlapping copies like memmove().
 *  This should only be temproary till support for 4.1 machines is dropped.
 */
#ifdef SUNOS4
#define memmove(to, from, bytes)      bcopy((from), (to), (bytes))
#endif  /* SUNOS4 */

#define ITL_UTIL_MEMCOPY(_from, _to, _size)                            \
      MACRO_BEGIN                                                      \
         itl_Trace(ITL_MEMORY_TRACE,                                   \
		   "%x: %s Copying %d from %x.\n",                     \
		   (_to), UTIL_STRING(_to), (_size), (_from));         \
         memmove((_to),(_from),(_size));                               \
      MACRO_END



#define ITL_REALLOC ITL_UTIL_REALLOC

#define ITL_UTIL_REALLOC(_pointer, _type, _size)                          \
        MACRO_BEGIN                                                       \
          pointer_t _reallocRet;                                          \
                                                                          \
          itl_Trace(ITL_MEMORY_TRACE,                                     \
		    "%x: Reallocating to size %d\n",                      \
		    (_pointer), (_size));                                 \
          _reallocRet = (pointer_t) realloc((_pointer), (_size));         \
          if ((_reallocRet == (pointer_t) 0)  && (_size) != 0) {          \
	     itl_MemoryFailure(UTIL_STRING(ITL_UTIL_REALLOC), __FILE__,   \
			       __LINE__);	                          \
          }								  \
          itl_Trace(ITL_MEMORY_TRACE,                                     \
		  "Reallocated, %x to %x\n",                              \
		    (_pointer), _reallocRet);                             \
          (_pointer) = (_type) _reallocRet;                               \
        MACRO_END


extern void itl_MemoryFailure _TAKES((
				      IN char *routine,
				      IN char *fileName,
				      IN int lineNum
				      ));

extern void itl_ErrorDCE _TAKES((
				 IN char *actionStrP
				 ));

/*
 *  ITL_CHECK_DCE_CODE --
 *    Macro that checks if a DCE function call failed.  It assumes that
 *    a negative return code implies failure.
 *
 *  Arguments:
 *    _rcode - return code from the invoked function.
 *    - descStrP - string describing function that was called.
 */
#define ITL_CHECK_DCE_CODE(_rcode, _descStrP)                           \
        MACRO_BEGIN                                                     \
          if ((_rcode) < 0)                                             \
             itl_ErrorDCE((_descStrP));                                 \
        MACRO_END


/* Defined exported interface */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_InitInternalIO()
 *
 *	Initialize the itlio_internal module.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_InitGlobal _TAKES((
					void
					));

extern boolean_t itl_InitItl _TAKES((
			     IN boolean_t interfaceMode,
			     IN boolean_t interactiveMode,
		             IN char * portPrefixP
				     ));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_CleanupItl -   Cleanup the program for a clean exit
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_CleanupItl _TAKES((void));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_AddCmdTable : Add commands to the command table.
 *
 *	Parameters:
 *	commandTableP - A pointer to the new commands.
 *	commandTableSize - The number of entries in the above table.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_AddCmdTable _TAKES((
			 INOUT itl_commandTable_t *commandTableP,
			 IN    int                 commandTableSize
					 ));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_AddModuleCmds :
 *	Add module commands to the command table.
 *
 *	Parameters:
 *	name - The name of the new module (for info purposes)
 *	commandTableP - A pointer to the new commands.
 *	commandTableSize - The number of entries in the above table.
 *	moduleMask - The default mask to or in for this module.
 *	moduleInfo - The info message for this module.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_AddModuleCmds _TAKES((
			  IN char 		*name,
			  IN itl_commandTable_t *commandTableP,
			  IN int                 commandTableSize,
			  IN int 		 moduleMask,
			  IN char 		*moduleInfo
					 ));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 * Function: itl_AddThreadState
 * 
 * Description:
 *     Adds another block of possible thread state to iterator
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_AddThreadState _TAKES((IN void (*InitThreadData)(),
					    IN void (*StartThreadData)(),
					    IN void (*CleanupThreadData)()
					    ));

/*
 * Interfaces for procedure parameters in AddThreadState:
 *
 *  void InitThreadData(IN OUT pointer_t*threadDataAreaPP);
 *  void StartThreadData(IN OUT pointer_t *threadaDataAreaPP);
 *  void CleanupThreadData(IN OUT pointer_t *threadaDataAreaPP);
 *
 */



/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_GetThreadState-
 *	Return an opaque copy of the thread data to the client.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_GetThreadState _TAKES((OUT pointer_t *ptrP));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_RestoreThreadState
 *	Restore the state of the thread from a previous
 *	state gotten through a call to itl_GetThreadState.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_RestoreThreadState _TAKES((IN pointer_t *ptrP));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_FreeThreadState
 *	Free the state of the thread received from a previous
 *	state gotten through a call to itl_GetThreadState.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_FreeThreadState _TAKES((IN pointer_t *ptrP));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_StartIndThread
 *      Start an itl thread (perform all the inializations, supply
 *	the IO information as well as the parent's info such
 *	as the parent's variable values).
 *	The thread performs the provides function and provides the
 *	function with the given parameter.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_StartIndThread _TAKES((IN void (*fn)(pointer_t),
				       IN pointer_t argP));


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
extern boolean_t itl_RunFromInput _TAKES( (IN char * promptStrP) );

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
extern boolean_t itl_RunFromFile _TAKES( (IN char* fileNameP,
					  IN char * promptStrP) );

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
extern boolean_t itl_RunFromString _TAKES( (IN char * inputStrP) );

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunnableVar
 *	Return the string the varaible stores, if it has a string
 *	runnable value.  return NULL otherwise.
 */
extern char *itl_RunnableVar _TAKES((IN itl_var_t *varP));


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
extern void itl_ExecuteVar _TAKES((IN pointer_t ptr));


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
extern void itl_StartIndependentThread _TAKES((
					       IN FILE *Fin,
					       IN FILE *Fout,
					       IN FILE *Ferr
					       ));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_RunStringInThread:
 *      Run an ITL procedure in an independent thread.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_RunStringInThread _TAKES((IN char *itlProc));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_StartServerThread - 
 *    Start the server thread.  This thread sits there listening
 *    on a socket waiting for other processes to attempt to speak
 *    to the test program.  It then creates an independent thread
 *    and instructs it to communicate with the socket.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_StartServerThread _TAKES((IN char * portPrefixP));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *    itl_CommandLoop -
 *  Start a new command loop using the currently defined history buffer.
 *  This command returns NULL if it encountered a parsing error.
 *  Otherwise, it returns a pointer to the name of the command, in
 *  the command table, which terminated the call.
 *  This command SHOULD NOT be modified.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern char *itl_CommandLoop _TAKES( (IN char * promptStrP ) );


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
 *  This routine starts a new history buffer, therefore,
 *  It makes the thread that called it, and thus created
 *  the history buffer an io_Thread responsible for the
 *  IO to this buffer.  The previous value of 
 *  Current_IO->io_Thread is rememebered and restored before
 *  returning.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern char *itl_LocalCommandLoop _TAKES ( (IN char * promptStrP) );

/*  
 *  itlIO_SkipWhites()
 *    Advance the index into the history buffer skipping over white spaces.
 *    return FALSE if a non white space was found and TRUE if it got
 *    to the end of the buffer.
 */
extern boolean_t itlIO_SkipWhites _TAKES((void));

/* Parsing interface */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *  itl_SetErrorStatus
 *     Set or reset the status of the parsing error.
 *     This routine is used when an error occurs while parsing a command
 *     and the user wishes the error to propogate to the outer levels
 *     until the program gets to an interactive layer.
 */
extern void itl_SetErrorStatus _TAKES((IN boolean_t flag));



extern int       itl_GetInteger _TAKES((OUT boolean_t *errP));

#define ITL_GET_INTEGER(_prompt, _var)\
   MACRO_BEGIN                        \
      boolean_t _eof;                 \
      itl_PromptUser("%s", _prompt);  \
      _var = itl_GetInteger(&_eof);   \
   MACRO_END

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_NextCharacter():
 *    return the next nonWhite character in the history buffer,
 *    reading from the input stream if necessary.
 */
extern char itl_NextCharacter _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_MatchCharacter(c)
 *     Check whether the next character in the input stream is the
 *     specified character.  If it is return TRUE and skip over the
 *     character.  Otherwise return FALSE and leave the character there.
 */
extern boolean_t itl_MatchCharacter _TAKES((IN int ch));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_MatchBrace
 *	Match a single brace in the input.
 *	If the correct Brace is found this function returns TRUE
 *	and updates the brace level.  Otherwise, the function returns
 *	FALSE.
 *  Parameters:
 *	braceType:  boolean_t: TRUE for open Brace and FALSE for closing.
 */
extern boolean_t itl_MatchBrace _TAKES((IN boolean_t braceType));

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
 *  Return values:
 *	The function return 0 if it did not find the cluse, 1 if it
 *	found the clause and -1 if there was an error executing the
 *	clause.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern int itl_OptionalClause _TAKES((
				      IN char *promptStr,
				      IN char *keyWord,
				      IN boolean_t flag
				      ));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *  itl_GetCharacter():
 *    return the next character in the history buffer - skipping over
 *    white spaces and reading from the input if necessary.  The history
 *    index is advanced to the next raw-character after the character
 *    that is returned.
 */
extern char itl_GetCharacter _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *   itl_PeekWord()  
 *    return one word from the current location in the history buffer.
 *    This routine returns the same word that would return from a call
 *    to itl_GetRawWord, but does not advance the location in the
 *    history buffer.
 *
 *    The word is allocated dynamically and it is up to the caller to
 *    free the space when it is no longer needed.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
extern char *itl_PeekWord _TAKES((
				     OUT boolean_t *eof,
				     INOUT int *len
				     ));

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
extern char *itl_PeekString _TAKES((OUT boolean_t *eofP, OUT int *lenP));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *  itl_GetString:
 *	Get a string from the input interpreting variables if necessary.
 */
extern char     *itl_GetString _TAKES((OUT boolean_t *errP, OUT int *lenP));

#define ITL_GET_STRING(_prompt, _var, _lenP)\
    MACRO_BEGIN                             \
       boolean_t _err;                      \
       itl_PromptUser("%s", _prompt);       \
       _var = itl_GetString(&_err, _lenP);  \
    MACRO_END

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_GetCommand()  
 *    return one word from the current location in the history buffer.
 *    It also skips all comments by calling itlIO_ProcessComments().
 *    On return, If the variable eofP is TRUE then EOF was reached and
 *    the function returns NULL.
 *    Otherwise, eofP returns FALSE and the function returns a dynamically
 *    allocated word.  It is up to the caller to free that pointer when it
 *    is no longer needed.
 */
extern char *itl_GetCommand _TAKES((
				    IN char *promptStr,
				    OUT boolean_t *eofP
				    ));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 *   itl_GetWord()  
 *    return one word from the current location in the history buffer.
 *    A word is consecutive characters, no character has special meaning.
 *    The word is allocated dynamically and it is up to the caller to
 *    free the space when it is no longer needed.
 *    Variables are not interpreted
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */
extern char *itl_GetWord _TAKES((
				 OUT boolean_t *eofP,
				 OUT int *lenP));

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
extern char     *itl_GetVariableName _TAKES((OUT boolean_t *errP,
					     INOUT int *lenP
					     ));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * itl_GetEnv -
 *	get the value of an environment variable.
 *	The string value of the variable is written into a script
 *	variable with name scriptVarName same name;
 */
extern itl_var_t *itl_GetEnv _TAKES((IN char *varName,
				     IN char *scriptVarName));
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * itl_LookupTable - look up a string from a table, which contains a list of
 *                   strings.  Return the number of strings that matchs the
 *                   given one, returns 0 if not found.  If found an exact
 *                   match, all the other matches are ignored and 1 is 
 *                   returned.  If found more match than ITL_MAX_INDEXSIZE,
 *                   only the first ITL_MAX_INDEXSIZE are put into indexList.
 */

extern int itl_LookupTable _TAKES((IN char *st,
				   IN char **table,
				   IN int tableSize,
				   OUT int *indexList));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_LocateVariable:
 *	Locate a variable by name and provide its itl_var_t structure.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern itl_var_t *itl_LocateVariable _TAKES((IN char * vName));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *   itl_GetVarVal - Get a variable value.
 */
extern var_val_t *itl_GetVarVal _TAKES((void));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *   itl_PeekVarVal - Peek at the next variable value.
 *
 *   This function returns the next variable value exactly as it would
 *   be returned if you called itl_GetVarVal, but does not move
 *   the history location.  The variable value is dynamically allocated
 *   and must be freed by calling itl_FreeVarVal.
 */
extern var_val_t *itl_PeekVarVal _TAKES((void));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *   itl_ShowVarVal - Show a variable value (short form).
 *	The scope is a character string printed after the variable
 *	value.
 */
extern boolean_t itl_ShowVarVal _TAKES((
				      IN char *name,
				      IN var_val_t *varVal,
				      IN char *scope));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *   itl_DisplayVarVal - Show a variable value (long form).
 *	The scope is a character string printed after the variable
 *	value.
 */
extern boolean_t itl_DisplayVarVal _TAKES((
				      IN char *name,
				      IN var_val_t *varVal,
				      IN char *scope));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_SetVariable - Set a variable to a particular value.
 *	name - The name of the variable
 *	value - The new value for the variable.
 *	scope - The scope of the variable:
 *		var_global: Global
 *		var_local:  Local
 *		var_same:   Whatever its current scope is or if it
 *			    has no value use the default scope.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern itl_var_t *itl_SetVariable _TAKES((
				  IN char *name,
				  IN var_val_t *value,
				  IN scope_t scope));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_ShowVariable
 *	display the variable named
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_ShowVariable _TAKES((IN char *name));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_ShowAllVariables
 *	display the current value of all the variables.
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern boolean_t itl_ShowAllVariables _TAKES((IN int param));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *   itl_GetSegment:
 *      Read a segment from the input, allocate a string for
 *      it dynamically and return that string terminated by
 *      a NULL.
 *        This routine is used to define script procedures.
 *      It is up to the caller to free the string when it is
 *	not longer necessary.
 *	 
 *	In the segment that is returned, variables are interpreted
 *	and their current value is inserted in their place.  '@'
 *	characters are replaced by '$'.
 *
 *	If the boolean variable, interpret is TRUE variables are
 *	interpreted, and @ are replaced by $.  Otherwise, the line
 *	is not interpreted.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern char     *itl_GetSegment _TAKES( (IN char *terminator,
					 IN char *promptStr,
					 IN boolean_t interpret,
					 OUT int *lengthP));


/*
 *  Prompt a user for a choice of two things, such as TRUE or FALSE
 *  or GOOD or BAD.
 *    If the user types in the second value, the routine returns FALSE,
 *  Otherwise it returns TRUE.
 *  The 'type' is written immediately after the prompt string in the prompt
 *  so:  itl_GetBoolean("First", "Good", "Bad", "Op")
 *  would prompt:   "First Op (Good, Bad): "
 */
extern boolean_t itl_GetBoolean _TAKES((
				    IN char *promptStr,
				    IN char *firstVal,
				    IN char *secondVal,
				    IN char *type));

/*
 *  Set a special variable to an integer result
 */
extern void itl_SetIntResult _TAKES((
				 IN itl_specialVar_t *varDescr,
				 IN int value));

/*
 *  Set a special variable to a string result
 */
extern void itl_SetStrResult _TAKES((
				 IN itl_specialVar_t *varDescr,
				 IN char *value));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_EchoLine():
 *    Print the rest of the line interpreting variables.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_EchoLine _TAKES((void));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_CommentLine - treat the rest of the line as a comment
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_CommentLine _TAKES((void));



/* Variable manipulation */
/* getIntVariable: get the value of an integer variable */
extern int itl_GetIntVariable _TAKES((IN char *name));
extern char * itl_GetStringVariable _TAKES( (IN char *name) );

/* setIntVariable: Set an integer variable to a given value */
extern itl_var_t *itl_SetIntVariable _TAKES((
			      IN char *name,
			      IN int value,
			      IN scope_t scope));


extern itl_var_t *itl_SetStrVariable _TAKES((
			      IN char *name,
			      IN char *string,
			      IN scope_t scope));

extern itl_var_t *itl_SetProcVariable _TAKES((
				      IN char *name,
				      IN char *string,
				      IN scope_t scope));




/*
 *  itl_FreeVarVal - Free a dynamically allocated variable value
 */
extern void itl_FreeVarVal _TAKES((IN var_val_t *varVal));

/*
 *   itl_NewVarVal - Create a new variable value
 */
extern var_val_t *itl_NewVarVal _TAKES((void));

/*
 *  itl_VarValueToString - Convert a varValue to string form.
 */
extern char *itl_VarValueToString _TAKES((
					  IN var_val_t *varVal,
					  INOUT int *outLen));

/*
 *  itl_VarValueToBoolean - Convert a varValue to boolean.
 */
extern boolean_t itl_VarValueToBoolean _TAKES((IN var_val_t *varVal));

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_DumpBytes:
 *	Hex dump of the given data.
 *	The routine collects internally whole lines and then prints
 *	the lines using itl_Message()
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern void itl_DumpBytes _TAKES((
			      IN char *bufP,
			      IN unsigned long length));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_DisplayBytes
 *	Display the contents of the buffer in hex format.
 *	if saveInString is TRUE the display is written into
 *	a dynamically allocated string which is returned by this command.
 *	The length of the string is returned in lengthP.
 *	If saveInString is FALSE, the data is dumped to stdout (via
 *	the itl_Message command)
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern char *itl_DisplayBytes _TAKES(( IN char *bufP,
				       IN unsigned long length,
				       IN boolean_t saveInString,
				       OUT unsigned long *lengthP));


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *  itl_AnsiTimeToStr - Convert an ansi time to a thread safe
 *	string that can be printed.
 *
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */
extern char *itl_AnsiTimeToStr _TAKES((IN time_t *ansiTime));

/*
 *  strcasestr: 
 *   returns a pointer to the first  occurrence  of  the
 *   pattern  string  s2  in  s1 ignorin case.   
 *   For example, if s1 is "string thing" and s2 is "ING",
 *   strcasestr() returns "ing thing".  If s2
 *   does not occur in s1, strstr() returns NULL.
 *
 */
SHARED char * strcasestr _TAKES((
				 IN char *str1,
				 IN char *str2
				 ));

#endif  /*  TRANSARC_ITL_ITL_H  */
