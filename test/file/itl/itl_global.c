/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_global.c,v 1.1.420.1 1996/10/17 18:27:17 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    SHARED   isDisplayLines
 *    
 *    EXPORT   itlIO_SetDebug, setVerbose, setDisplayLines, 
 *             toggleDisplayLines, itl_SetIntResult, itl_SetStrResult, 
 *             itl_DumpBytes, itl_LookupTable, itl_GetBoolean
 *    
 *    extern   itlIO_IsDebug, Itl_Util_Realloc, initializeSpecialVars
 *    
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1990, 1991, 1992, 1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: itl_global.c,v $
 * Revision 1.1.420.1  1996/10/17  18:27:17  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:41  damon]
 *
 * Revision 1.1.415.1  1994/02/04  20:45:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:29  devsrc]
 * 
 * Revision 1.1.413.1  1993/12/07  17:44:35  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:26:35  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:30:53  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:09:45  htf]
 * 
 * Revision 1.1.2.2  1992/09/25  17:16:09  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	  Selected comments:
 * 	    Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	    Initial checkin.
 * 	[1992/09/23  21:22:56  jaffe]
 * 
 * 	$TALog: itl_global.c,v $
 * 	Revision 1.20  1994/11/01  22:29:33  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.19 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.19  1993/07/29  21:01:51  jess
 * 	This delta fies the problem of running file_test on solaris and
 * 	also some merge problems for the file itl_global.c.
 * 
 * 	See above.
 * 	[from r1.18 by delta jess-db4002-fix-itl-spoke-solaris, r1.1]
 * 
 * Revision 1.18  1993/06/03  16:48:40  fred
 * 	[merge of changes from 1.8 to 1.11 into 1.17]
 * 
 * Revision 1.11  1993/04/21  19:12:44  srinivas
 * 	changes for solaris
 * 
 * 	changes for solaris
 * 	[from r1.8 by delta srinivas-itl-1, r1.1]
 * 
 * Revision 1.17  1993/06/03  15:30:45  fred
 * 	[merge of changes from 1.15 to 1.16 into 1.14]
 * 
 * Revision 1.16  1993/06/03  12:50:09  fred
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
 * 	[from r1.15 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.14  1993/05/06  17:46:04  jaffe
 * 	[merge of changes from 1.8 to 1.13 into 1.10]
 * 
 * Revision 1.13  1993/04/29  21:28:10  jess
 * 	The semantics of isDisplayLine is not what we want for auto_spoke.
 * 	This delta add a function "getDisplayLine" in itl_global.c file.
 * 
 * 	New function of getDisplayLines.
 * 	[from r1.8 by delta jess-db3335-add-auto_spoke, r1.7]
 * 
 * Revision 1.10  1993/03/24  20:41:29  bruce
 * 	This delta added several options to the file test program.  There purpose
 * 	is to provide support to a HUB to automatically start up several spokes
 * 	on other machines.
 * 
 * 	Added here were support for the new menu options.
 * 	[from r1.8 by delta bruce-add-auto_spoke_calls, r1.1]
 * 
 * Revision 1.8  1993/03/08  13:51:54  fred
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
 * 	[from r1.7 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.7  1993/03/01  14:48:23  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.6 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 6.2  1993/01/28  22:16:25  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Cast key destructor argument properly.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * Revision 1.21  1992/03/31  22:54:53  oz
 * 	Include itl_exp.h in itl_global.c (gcc complained).
 * 
 * 	Included itl_exp.h in itl_global.c (gcc complained).
 * 	[from revision 1.20 by delta oz-2227:include-itl_exp.h-in-itl_global.c, revision 1.1]
 * 
 * Revision 1.20  1992/03/26  12:53:35  jaffe
 * 	Prologue Changes for 1.0.1
 * 
 * 	[from revision 1.19 by delta jaffe-ttools-prologue, revision 1.1]
 * 
 * Revision 1.19  1992/02/11  14:30:20  fred
 * 	Changed UTIL_ASSERT to ASSERT.
 * 	[92/02/11  14:28:12  fred]
 * 
 * Revision 1.17  92/02/07  14:22:46  shi
 * 	added a new function itl_LookupTable
 * 
 * Revision 1.16  92/02/07  11:23:23  oz
 * 	 - setDebug    ==> itlIO_SetDebug
 * 	 - isDebug     ==> itlIO_IsDebug
 * 	[92/02/06  15:06:10  oz]
 * 
 * 	 - Removed DebugLevel
 * 	 - Removed statusCodes and MapStatusToString (This is leftover
 * 	   bagage from rec_test)
 * 	[92/02/06  13:13:43  oz]
 * 
 * Revision 1.15  92/02/05  19:50:50  oz
 * 	- new function: itl_GetEnv
 * 
 * Revision 1.14  92/02/04  22:16:55  oz
 * 	 - SetStrResult: Does not take the length but sets it to strlen()+1
 * 	[92/02/03  14:05:43  oz]
 * 
 * Revision 1.13  92/01/20  08:30:30  oz
 * 	 - Using
 * 		itl_AdviseUser  instead of itl_Advise
 * 		itl_Message	instead of itl_PrintMsg
 * 	[92/01/18  18:08:55  oz]
 * 
 * 	 - Using:
 * 		itl_Trace 	instead of itl_TracePrintf
 * 		itl_PromptUser	instead of itl_Prompt
 * 		itl_Error	instead of itl_PrintErrMsg
 * 	[92/01/18  13:21:40  oz]
 * 
 * Revision 1.12  92/01/02  09:58:32  oz
 * 	Moved CommandMode to itl_command.c
 * 	[91/12/26  10:44:21  oz]
 * 
 * Revision 1.11  91/12/12  13:55:33  oz
 * 	Modified SetResult:
 * 	  The scope of results is now always local
 * 	  Works for any type, not only the basic types.
 * 	  The scope of the variable version is always global
 * 	[91/12/11  12:58:27  oz]
 * 
 * Revision 1.10  91/11/21  15:33:06  oz
 * 	 - New Value Types
 * 	[91/11/11  16:37:36  oz]
 * 
 * Revision 1.9  91/11/13  16:42:23  fred
 * 	Installed a new version of itl_DumpBytes().  It no longer repeats lines
 * 	that are the same, thus saving screen space.  It also prints the
 * 	addresses as an offset from 0 rather than the buffer address.
 * 
 * Revision 1.8  91/11/06  16:30:35  oz
 * 	Removed itl_traceHandle
 * 
 * Revision 1.7  91/10/18  13:42:35  oz
 * 	Moved lastTranId to itl_tran.c
 * 
 * Revision 1.6  91/10/18  11:06:37  oz
 * 	 - Removed FreeDescrPages it was left over there from REC.
 * 
 * Revision 1.5  91/10/16  19:15:10  oz
 * 	- the type specialVar_t  was replaced by itl_specialVar_t
 * 	- Renamed:
 * 	       setIntResult     ->  itl_SetIntResult
 * 	       setResult        ->  SetResult
 * 	       setStrResult     ->  itl_SetStrResult
 * 	       IntToString      ->  itl_IntToStaticStr
 * 	       DumpBytes        ->  itl_DumpBytes
 * 	       GetBoolean       ->  itl_GetBoolean
 * 	       GetPointer       ->  MACRO
 * 	       checkBdeStatus   ->  itl_CheckBdeStatus
 * 
 * Revision 1.4  91/10/03  15:41:10  oz
 * 	New Command:toggleDisplayLines
 * 
 * Revision 1.3  91/10/03  12:50:58  oz
 * 	Removed unused variable i.
 * 	Removed unnecesary section:
 * 	   initialization
 * 	   setting special status variables.
 * 
 * Revision 1.2  91/09/16  12:11:40  mss
 * 	Renamed
 * 	 printMsg => itl_PrintMsg
 * 	 advise => itl_Advise
 * 	 isInteractive => itlIO_IsInteractive
 * 	 tracePrintf => itl_TracePrintf
 * 	 setIntVariable => itl_SetIntVariable
 * 	 setStrVariable => itl_SetStrVariable
 * 	 setProcVariable => itl_SetProcVariable
 * 	 prompt => itl_Prompt
 * 	 IO_GetWord => itl_GetWord
 * 	 printErrMsg  => itl_PrintErrMsg
 * 	 currentErr => itl_CurrentErr
 * 
 * Revision 1.1  91/08/22  17:20:38  mss
 * 	Modified REC tester to make separate library
 * 
 * 	$EndTALog$
 * 
 * $EndLog$
 */

/*
 * Test Program:  itl_global.c
 *
 * Description:
 *     Global functions used by the Recovery Service test programs.
 *
 */

#include <errno.h>
#include <time.h>
#include <stdlib.h>

#ifndef __GNUC__
#include <memory.h>       /* built-ins in gcc */
#endif   /* __GNUC__ */

#include <pthread.h>
#include <utils/itl/itl_private.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_threadStatic.h>

static char RCSid[] = "$Id: itl_global.c,v 1.1.420.1 1996/10/17 18:27:17 damon Exp $";

#define VERSION_STRING "Version 1.5"

/*
 * GLOBAL VARIABLES
 */
EXPORT char               *itlName;
EXPORT unsigned long	  itl_traceLevel = 0;

unsigned int       currentFileNum = 0;

itl_specialVar_t  commandResult =   {"CommandResultVar", "CommandResult"};
itl_specialVar_t  threadID =        {"ThreadIDVar", "ThreadId"};
itl_specialVar_t  currentFileName = {"CurrentFileVar", "CurrentFile"};


/*
 *  Declare keys for thread data area(s) used by this module.
 */
PRIVATE pthread_key_t ansiStrKey;

/*
 *  Declare any mutexes used by this module.
 */
PRIVATE LATCH_TYPE timeMutex;



/*
 * itl_InitGlobal() -- 
 *    Initializes the global definitions module.  Needed for pthread 
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
SHARED boolean_t itl_InitGlobal()
{
#ifdef	SUNOS5
    extern void free();
#endif
    boolean_t returnVal = FALSE;

    if (pthread_keycreate(&ansiStrKey, (pthread_destructor_t) free) != 0) 
    {
	/*
	 *  Use fprintf cause we are initializing, and ITL routines may
	 *  not be ready.
	 */
	fprintf(stderr, "Key creation [1] failed in itl_InitGlobal, errno = %d\n",
		errno);
	returnVal = TRUE;
    }
    LATCH_INIT(&timeMutex);

    return(returnVal);
}

/*
 *  Set ITL trace level and display the current value
 */
EXPORT boolean_t itlIO_SetDebug(flag)
  IN int flag;
{
  itl_traceLevel = flag;
  itl_AdviseUser("itl trace level is 0x%x\n", itl_traceLevel);
  return(FALSE);
}

boolean_t itlIO_IsDebug()
{
    return(itl_traceLevel & ITL_DEBUG_TRACE);
}
/*
 *  Return true if running in verbose mode
 */
boolean_t isVerbose()
{
  return(itl_traceLevel & ITL_VERBOSE_TRACE);
}

/*
 *  Set whether to run in verbose or not
 */
EXPORT boolean_t setVerbose(flag)
IN boolean_t flag;
{
  if (flag) itl_traceLevel |= ITL_VERBOSE_TRACE;
  else itl_traceLevel &= ~ITL_VERBOSE_TRACE;
  return(FALSE);
}

PRIVATE boolean_t DisplayLines = FALSE;

EXPORT boolean_t setDisplayLines(flag)
  IN boolean_t flag;
{
    DisplayLines = flag;
    itl_AdviseUser("  Display lines mode is %s\n",
		   DisplayLines ? "on" : "off");
    return(FALSE);
}

EXPORT boolean_t getDisplayLines()
{
    return(DisplayLines);
}

EXPORT boolean_t toggleDisplayLines(param)
  IN int param;
{
    DisplayLines = !DisplayLines;
    itl_AdviseUser("Display lines mode is %s\n",
		   DisplayLines ? "on" : "off");
    return(FALSE);
}

SHARED boolean_t isDisplayLines()
{
    return(!itlIO_IsInteractive() &&
	   (DisplayLines || itlIO_IsDebug() || isVerbose() ||
	    (itl_traceLevel & ITL_IO_TRACE)));
}

/*
 *   SetResult:
 *      Set a program defined result given to this routine by a specialVar_t.
 *
 *   Some variables are automatically set by the test program, e.g.,
 *   The status returned from REC.  The status is written to a variable
 *   called RecStatus.  However, the user can redefine the variable to which
 *   The status is written by defining a string variable called
 *   RecStatusVar.  This information is contained in a structure
 *   of type  specialVar_t.
 *
 *   The special Var holds two names: The second is the name of the
 *   default variable to which to write the result.  The first is the name
 *   of a variable which overides the first: i.e., if the first 
 *   variable, varDescr->varName, exists, and is a string variable,
 *   its value is used as the name of the result string, otherwise, 
 *   the name of the result string is in the string
 *   varDescr->varDescr->Default.
 */
PRIVATE void SetResult(varDescr, value, size, type)
IN itl_specialVar_t *varDescr;
IN pointer_t value;
IN int size;
IN int type;
{
    itl_var_t *nameVar = locateVariable(varDescr->varName);
    char *varName = (nameVar && (VAR_TYPE(nameVar) == itl_stringType)) ?
	VAR_STR_VALUE(nameVar) : varDescr->Default;

    if (type == itl_intType) {
	itl_Trace(ITL_VAR_TRACE,
		  "Writing result (%d) to variable %s\n",
		  *(int *)value, varName);
	itl_SetIntVariable(varName, *(int *)value, var_local);
    }
    else if (type == itl_stringType) {
	itl_Trace(ITL_VAR_TRACE,
		  "Writing result (%s) to variable %s\n",
		  (char *)value, varName);
	itl_SetStrVariable(varName, (char *)value, var_local);
    }
    else if (type == itl_procType) {
	itl_Trace(ITL_VAR_TRACE,
		  "Writing result a procedure Def to variable %s\n",
		  varName);
	itl_SetProcVariable(varName, (char *)value, var_local);
    }
    else {
	itlExp_SetVarValue(varName, type, size, value, var_local); 
    }
}

/*
 *   Set an integer result by calling setResult.
 */
EXPORT void itl_SetIntResult(varDescr, value)
IN itl_specialVar_t *varDescr;
IN int value;
{
    SetResult(varDescr, &value, sizeof(int), itl_intType);
}

/*
 *   Set a string result by calling setResult.
 */
EXPORT void itl_SetStrResult(varDescr, value)
  IN itl_specialVar_t *varDescr;
  IN char *value;
{
    SetResult(varDescr, value, strlen(value) + 1, itl_stringType);
}

PRIVATE void InitAnsiStr(strPP)
  IN char **strPP;
{
    *strPP = (char *) 0;
}

/*
 *  ansiTimeToStr - convert an ansi time to a string
 *      This is simply a thread safe version of ctime.
 *
 *     The string is allocated on a thread safe static
 *     variable.
 */
EXPORT char *ansiTimeToStr(ansiTime)
  IN time_t *ansiTime;
{
    char *ctimeStr;
    unsigned int outputSize;
    char **ansiTimeStr;

    ITL_THREAD_STATIC_INIT(ansiStrKey, ansiTimeStr, char *, InitAnsiStr);

    LATCH_LOCK(&timeMutex);
    ctimeStr = ctime(ansiTime);
    outputSize = strlen(ctimeStr) + 1;
    if (!(*ansiTimeStr)) 
	ITL_UTIL_ALLOC(*ansiTimeStr, char *, outputSize);
    else
	ITL_UTIL_REALLOC(*ansiTimeStr, char *, outputSize);
    ITL_UTIL_MEMCOPY(ctimeStr, *ansiTimeStr, outputSize);
    LATCH_UNLOCK(&timeMutex);
    return(*ansiTimeStr);
}


/*
 * itl_GetEnv - get the value of an environment variable.
 */

EXPORT itl_var_t *itl_GetEnv(varName, scriptVarName)
  IN char *varName;
  IN char *scriptVarName;
{
    extern char  *getenv();
    char *varValue;
    itl_var_t *varP;

    varValue = getenv(varName);

    if (varValue) {
	varP = itl_SetStrVariable(scriptVarName, varValue, var_global);
    } else {
	varP = itl_SetStrVariable(scriptVarName, "", var_global);
    }
    return(varP);
}
    
/*
 * FUNCTIONS for performing I/O.
 */


/*
 * itl_DumpBytes() -- 
 *    Dump a buffer of memory.  Both the hex and ascii values are printed.
 *    (Thanks to Rami Heled)
 *    This code is now replaced by the DisplayBytes() function.
 * 
 * ARGUMENTS: 
 *    buf - address where dumping is to begin.
 *    length - number of bytes to dump.
 *
 * RETURN CODES: none
 */
EXPORT void itl_DumpBytes(bufP, length)
  IN char *bufP;
  IN unsigned long length;
{
    (void) itl_DisplayBytes(bufP, length, FALSE, NULL);
}

/*
 *  itl_DisplayBytes
 *	Display the contents of the buffer in hex format.
 *	if saveInString is TRUE the display is written into
 *	a dynamically allocated string which is returned by this command.
 *	The length of the string is returned in lengthP.
 *	If saveInString is FALSE, the data is dumped to stdout (via
 *	the itl_Message command)
 */
EXPORT char *itl_DisplayBytes(bufP, length, saveInString, lengthP)
  IN   char *bufP;
  IN   unsigned long length;
  IN   boolean_t saveInString;
  OUT  unsigned long *lengthP;
{
    unsigned	int	i,j,k;
    unsigned	char	c;
    char        line[80], *curPoint = &line[0];
    boolean_t     didAsterik = FALSE;  /* was the asterik printed */
    unsigned int  loopLimit;
    char         *result = NULL;
    unsigned long totalLen = 0;
    
    if (saveInString) {
	ITL_ALLOC(result, char *, 128);
    }
    /*
     * print 16 characters on each line, first in hex, then in ascii
     */
    loopLimit = (length / 16) + ((length % 16) != 0);
    for (i = 0; i < loopLimit; i++) {
	if ((i != 0) && ((i + 1) != loopLimit) && 
	    (memcmp((bufP + (i * 16)), (bufP + ((i - 1) * 16)), 16) == 0)) {
	    if (didAsterik == FALSE) {
		didAsterik = TRUE;
		if (saveInString) {
		    ITL_REALLOC(result, char *, (totalLen + 3));
		    sprintf(result + totalLen, "%c\n", '*');
		} else {
		    itl_Message("%c\n", '*');
		}
		totalLen += 2;
	    }
	    continue;
	}

	k = ((length - i*16 ) < 16 ? length - i*16 : 16 );
	for (j = 0; j < k; j++) {
	    if ( j == 0 ) {
		/*
		 * print the address at the beginning of the line
		 * Changed to print from 0, instead of buffer address.
		 */
		sprintf(curPoint, "%08x", i*16);
		curPoint += 8;
	    }
	    /*
	     * print a character in hexadecimal represantation,
	     * leave a blank space every 2 characters.
	     */
	    if ( !(j%2) ) {
		sprintf(curPoint, " ");
		curPoint++;
	    }
	    sprintf(curPoint, "%02x", (unsigned char)bufP[i*16 + j]);
	    curPoint += 2;
	}
	while (j++ < 16) {
	    sprintf(curPoint, "  ");
	    curPoint += 2;
	    if ( j%2 ) {
		sprintf(curPoint, " ");
		curPoint++;
	    }
	}
	sprintf(curPoint, "  ");
	curPoint += 2;
	for (j = 0; j < k; j++) {
	    /*
	     * print up to 16 characters in ascii
	     */
	    c = bufP[i*16 + j];
	    sprintf(curPoint, "%c", ( c < 32 || c > 127 ) ? '.' : c );
	    curPoint++;
	}
	while (j++ < 16) {
	    sprintf(curPoint, " ");
	    curPoint++;
	}
	*curPoint = '\0';
	/*
	 *  Compare this line with the last line.  If they are the same,
	 *  the current line is not printed.  Only do this after a line
	 *  has been printed (i != 0).  The comparison skips over the 
	 *  address, which is always different.
	 *  The last line is always printed ((i + 1) != loopLimit).
	 */
	if ((i != 0) && ((i + 1) != loopLimit) && 
	    (memcmp((bufP + (i * 16)), (bufP + ((i - 1) * 16)), 16) == 0)) {
	    if (didAsterik == FALSE) {
		didAsterik = TRUE;
		if (saveInString) {
		    ITL_REALLOC(result, char *, (totalLen + 3));
		    sprintf(result + totalLen, "%c\n", '*');
		} else {
		    itl_Message("%c\n", '*');
		}
		totalLen += 2;
	    }
	} else {
	    int lineLen = strlen(line);

	    if (saveInString) {
		ITL_REALLOC(result, char *, (totalLen + lineLen +2));
		sprintf(result + totalLen, "%s\n", line);
	    } else {
		itl_Message("%s\n", line);
		didAsterik = FALSE;
	    }
	    totalLen += lineLen + 1;
	}
	curPoint = &line[0];
    }
    if (lengthP) {
	*lengthP = totalLen + 1;
    }
    return(result);
}


/*
 *  itl_MemoryFailure() --
 *    Routine to print message that some memory operation failed, and then
 *    its lights out.
 */
SHARED void itl_MemoryFailure(routine, fileName, lineNum)
  IN char *routine;
  IN char *fileName;
  IN int lineNum;
{
    itl_PrintError("*** FATAL *** [Memory Failure]\n\tFile: %s\n\tLine: %d\n",
		   fileName, lineNum);
    exit(-1);
}



/*
 *  NEW FUNCTIONS
 */

/*
 * itl_LookupTable - look up a string from a table, which contains a list of
 *                   strings.  Return the number of strings that matchs the
 *                   given one, returns 0 if not found.  If found an exact
 *                   match, all the other matches are ignored and 1 is 
 *                   returned.  If found more match than ITL_MAX_INDEXSIZE,
 *                   only the first ITL_MAX_INDEXSIZE are put into indexList.
 */

EXPORT int itl_LookupTable(st, table, tableSize, indexList)
  IN char *st;
  IN char **table;
  IN int  tableSize;
  OUT int *indexList;
{
    int i;
    int found = 0;
    extern int strncasecmp();

    for (i = 0; i < tableSize; i++) 
	if (((int) strlen(st) <= (int) strlen(table[i])) && 
	    (! strncasecmp(st, table[i], strlen(st)))) {
	    if (strlen(st) == strlen(table[i])) {
		/*
		 * found an exact match.
		 */
		indexList[0] = i;
		return 1;
	    }
	    if (found++ < ITL_MAX_INDEXSIZE) {
		indexList[found-1] = i;
	    }
	}
    return (found);
}

/*
 *  Prompt a user for a choice of two things, such as TRUE or FALSE
 *  or GOOD or BAD.
 *    If the user types in the second value, the routine returns FALSE,
 *  Otherwise it returns TRUE.
 */
EXPORT boolean_t itl_GetBoolean(promptStr, firstVal, secondVal, type)
  IN char *promptStr;
  IN char *firstVal, *secondVal;
  IN char *type;
{
    char *ptrString;
    boolean_t eof;
    int wordLen;
    extern int strncasecmp();
    int len2 = strlen(secondVal);

    itl_PromptUser("%s %s (%s, %s): ",
		   promptStr, type, firstVal, secondVal);
    ptrString = itl_GetWord(&eof, &wordLen);
    if (eof) {
	itl_Error("Encountered EOF when expecting a %s\n", type);
	return(FALSE);
    }
    itl_Trace(ITL_DEBUG_TRACE,
	      "%s: %s, length %d\n", type, ptrString, strlen(ptrString));
    if (!strncasecmp(ptrString, secondVal, 
		     MIN((int) strlen(ptrString), len2))) {
	itl_Trace(ITL_VERBOSE_TRACE,
		  "Using %s %s for %s.\n",
		  secondVal, type, promptStr);
	ITL_FREE(ptrString);
	return(FALSE);
    }
    else {
	itl_Trace(ITL_VERBOSE_TRACE,
		  "Using %s %s for %s.\n", firstVal, type, promptStr);
	ITL_FREE(ptrString);
	return(TRUE);
    }
}
    
void initializeSpecialVars()
{
    itl_Trace(ITL_VERBOSE_TRACE, "Initializing: %s\n", RCSid);

    /*  Initialize some special variables:  */
    (void) itl_SetStrVariable("Version",
			  VERSION_STRING,
			  var_global);
}    


SHARED void itl_ErrorDCE(actionStrP)
  IN char *actionStrP;
{
    itl_Error("\n***** DCE function failure:  %s\n", actionStrP);
    itl_Error("\tErno: %d\n\tFile: %s\n\tLine: %d\n\n", 
	      errno, __FILE__, __LINE__);
    exit(-1);
}


