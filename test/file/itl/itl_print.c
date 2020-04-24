/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * ID: $Id: itl_print.c,v 1.1.616.1 1996/10/17 18:27:36 damon Exp $
 *
 * COMPONENT_NAME: Encina Development Infrastructure
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
 * FUNCTIONS:
 *    SHARED   itlPrint_Init
 *    
 *    EXPORT   itl_Message, itl_fMessage, itl_AdviseUser, itl_fAdviseUser, 
 *             itl_Trace, itl_fTrace, itl_PrintError, itl_fPrintError, 
 *             itl_PromptUser, itl_fPromptUser
 *    
 *    PRIVATE  PrintMessage, PrintAMessage, PromptUser
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
 * $Log: itl_print.c,v $
 * Revision 1.1.616.1  1996/10/17  18:27:36  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:50  damon]
 *
 * Revision 1.1.611.1  1994/02/04  20:45:34  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:37  devsrc]
 * 
 * Revision 1.1.609.1  1993/12/07  17:44:47  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:28:48  jaffe]
 * 
 * Revision 1.1.5.2  1993/05/11  17:40:36  jaffe
 * 	Cleanup log
 * 	[1993/05/11  17:38:46  jaffe]
 * 
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:38  htf]
 * 
 * Revision 1.1.2.4  1993/01/11  18:31:18  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:38  htf]
 * 
 * Revision 1.1.2.3  1992/12/09  21:32:56  jaffe
 * 	Transarc delta: fred-ot-6193-move-procid-to-base-itl 1.1
 * 	  Selected comments:
 * 	    To make the process id and system page size variables available to all test
 * 	    programs but placing their initialization in the ITL library and not the
 * 	    itlFile library.
 * 	    Also incorporated fixes to allow parameters to be passed to procedures
 * 	    started in detached threads.
 * 	    Fix HandleExecProc to do the right thing for threadExec.  These are fixes
 * 	    from Ofer.
 * 	[1992/12/07  13:34:48  jaffe]
 * 
 * Revision 1.1.3.2  1992/12/07  13:34:48  jaffe
 * 	Transarc delta: fred-ot-6193-move-procid-to-base-itl 1.1
 * 	  Selected comments:
 * 	    To make the process id and system page size variables available to all test
 * 	    programs but placing their initialization in the ITL library and not the
 * 	    itlFile library.
 * 	    Also incorporated fixes to allow parameters to be passed to procedures
 * 	    started in detached threads.
 * 	    Fix HandleExecProc to do the right thing for threadExec.  These are fixes
 * 	    from Ofer.
 * 
 * 	$TALog: itl_print.c,v $
 * 	Revision 12.3  1995/11/30  20:00:11  berman
 * 	For solaris 2.5, upgrade the uses of gettimeofday to
 * 	match prototypes.
 * 
 * 	Upgrade gettimeofday calls.
 * 	[from r1.9 by delta berman-db6975-clean-gettimeofday, r1.1]
 * 
 * Revision 1.9  1994/11/01  22:29:54  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.8 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.8  1993/05/19  12:56:30  jaffe
 * 	[merge of changes from 1.5 to 1.7 into 1.6]
 * 
 * Revision 1.6  1993/03/08  13:52:01  fred
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
 * 	[from r1.5 by delta fred-db3202-import-encina-itl-fixes, r1.1]
 * 
 * Revision 1.5  1993/01/14  17:38:33  jaffe
 * 	revdrop of latest OSF copyright changes to these files
 * 	[from r1.4 by delta osf-revdrop-01-12-92, r1.1]
 * 
 * Revision 1.3  1992/11/10  19:49:20  jaffe
 * 	Sync up with the OSF source as of Nov 9.  This delta fixes mostly test
 * 	code problems related to missing OSF copyrights.
 * 	[from revision 1.2 by delta osf-sync-with-nov-9-drop, r1.1]
 * 
 * Revision 1.2  1992/10/02  20:32:09  jaffe
 * 	Sync with OSF sources.
 * 
 * 	Added correct OSF headers.
 * 	[from revision 1.1 by delta osf-cleanup-from-sept-25-92-drop-test, r1.1]
 * 
 * Revision 1.1  1992/08/10  20:11:19  fred
 * 	Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	Initial checkin.
 * 
 * 	Initial checkin.
 * 	[added by delta fred-checkin-itl, revision 1.1]
 * 
 * Revision 1.4  1992/03/27  21:15:53  oz
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
 * Revision 1.3  92/03/23  10:11:19  oz
 * 	- New function: itlPrint_ElapsedTime
 * 	[92/03/23  09:55:41  oz]
 * 
 * Revision 1.2  92/03/04  15:14:30  oz
 * 	Initial Checkin
 * 	[92/02/22  18:05:44  oz]
 * 
 * $EndLog$
 */

/*
 * itl_print.c -- 
 *    The module handling output for ITL.
 *
 *
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <pwd.h>
#include <time.h>

#include "itl_stds.h"

#include <utils/itl/itl.h>
#include <utils/itl/itl_io.h>

PRIVATE struct timeval itlPrint_StartTime;

#if defined(__STDC__)
#	include <stdarg.h>
#define VA_START(ap,first) va_start(ap,first)
#else /* ! __STDC__ */
#	include <varargs.h>
#define VA_START(ap,first) va_start(ap)
#endif

#include <pthread.h>


/* 
 * SubtractTime() --
 *
 * Assumption: t1 >= t2.  If t1 < t2, the result will be correct, but
 *    the tv_sec field will be negative.
 */
PRIVATE void SubtractTime(t1, t2, result)
  IN struct timeval *t1;
  IN struct timeval *t2;
  IN struct timeval *result;
{
    if (t1->tv_sec == t2->tv_sec) {
	result->tv_sec = 0;
	result->tv_usec = t1->tv_usec - t2->tv_usec;
    } else {
	result->tv_sec  = t1->tv_sec  - t2->tv_sec - 1;
	result->tv_usec = t1->tv_usec - t2->tv_usec + 1000000;
	while (result->tv_usec > 1000000) {
	    result->tv_sec  += 1;
	    result->tv_usec -= 1000000;
	}
    }
}

/*
 * TimeToANSI
 *
 * 	Convert the time representation of seconds and micro-seconds to
 *      just seconds.
 */	
PRIVATE void TimeToANSI(time, ansiTime)
  IN struct timeval * time;
  OUT time_t * ansiTime;
{
    /* use local vars so the given structure is not changed */
    long secs = time->tv_sec;
    long usecs = time->tv_usec;

    while (usecs > 1000000) {
	secs  += 1;
	usecs -= 1000000;
    }
    *ansiTime = secs;
}


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *	Routines for outputting messages to the output
 */
/*
 * PrintMessage
 *	Print a message to the specified output file and prepend
 *	the thread ID to it.
 *
 */
#define LOCAL_FORMAT_SIZE 250
PRIVATE void PrintMessage _TAKES((IN FILE *, IN char *, IN va_list));
PRIVATE void PrintMessage(file, format, argsP)
  IN FILE *file;
  IN char *format;
  IN va_list argsP;
{
    int curLen = strlen(format);
    char localFmt[LOCAL_FORMAT_SIZE];
    char *fmtUsed = &localFmt[0];
    pthread_t pthreadId = pthread_self();
    
    if ((curLen + 15) > LOCAL_FORMAT_SIZE) {
	/*
	 *  We should not call ITL_ALLOC here since it may call
	 *  This function back to trace its parameters.
	 */
	ITL_UTIL_ALLOC(fmtUsed, char *, curLen + 15);
    }
    sprintf(fmtUsed, "[%d] %s", pthread_getunique_np(&pthreadId), format);
    vfprintf(file, fmtUsed, argsP);
    fflush(file);
    if (fmtUsed != localFmt)
	ITL_UTIL_FREE(fmtUsed);
}

/*
 *  traceStringCodes
 *	Characters added to a trace format string as a function
 *	of the first bit set of the string.
 */
PRIVATE char *traceStringCodes[33] ={
    "---",	/**  0  **/
    "<->",	/**  1  **/
    "[-]",
    "(-)",
    "{-}",
    "<+>",	/**  5  **/
    "[+]",
    "(+)",
    "{+}",
    "<*>",	/**  9  **/
    "[*]",
    "(*)",
    "{*}",
    "<@>",	/**  13  **/
    "[@]",
    "(@)",
    "{@}",
    "<#>",	/**  17  **/
    "[#]",
    "(#)",
    "{#}",
    "<|>",	/**  21  **/
    "[|]",
    "(|)",
    "{|}",
    "<_>",	/**  25  **/
    "[_]",
    "(_)",
    "{_}",
    "<=>",	/**  29  **/
    "[=]",
    "(=)",
    "{=}"
};

/*
 * PrintAMessage
 *	Print a message to the specified output file and prepend
 *	to it the thread ID and time.
 *	Mode: 1 - regular message (code ---)
 *	Mode: 2 - Error message (code ### ^G)
 *		  Also, set the error status.
 *	Mode: 3 - Trace message (the three letter trace code)
 *
 */
#define LOCAL_FORMAT_SIZE 250
PRIVATE void PrintAMessage _TAKES((IN int mode,
				   IN unsigned long,
				   IN FILE *,
				   IN char *,
				   IN va_list));
PRIVATE void PrintAMessage(mode, mask, file, format, argsP)
  IN int mode;
  IN unsigned long mask;
  IN FILE *file;
  IN char *format;
  IN va_list argsP;
{
    int curLen = strlen(format);
    char localFmt[LOCAL_FORMAT_SIZE];
    char *fmtUsed = &localFmt[0];
    struct timeval currentTime;
    int hours, minutes, seconds;
    pthread_t pthreadId = pthread_self();
    unsigned int threadId;
    
#if	defined(_SVID_GETTOD)
    gettimeofday(&currentTime);
#else	/* _SVID_GETTOD */
    gettimeofday(&currentTime, (struct timezone *) 0);
#endif	/* _SVID_GETTOD */
    currentTime.tv_sec -= itlPrint_StartTime.tv_sec;
    seconds = currentTime.tv_sec % 60;
    currentTime.tv_sec /= 60;
    minutes = currentTime.tv_sec % 60;
    hours = currentTime.tv_sec / 60;
    threadId = pthread_getunique_np(&pthreadId);
    
    if ((curLen + 35) > LOCAL_FORMAT_SIZE) {
	ITL_ALLOC(fmtUsed, char *, curLen + 35);
    }
    switch (mode) {
      case 1:
	if (hours == 0) {
	    sprintf(fmtUsed, "[%d] (%d:%.2d) --- %s",
		    threadId,
		    minutes, seconds,
		    format);
	}
	else {
	    sprintf(fmtUsed, "[%d] (%d:%.2d:%.2d) --- %s",
		    threadId,
		    hours, minutes, seconds,
		    format);
	}
	break;
      case 2:
	itl_SetErrorStatus(TRUE);
	if (hours == 0) {
	    sprintf(fmtUsed, "[%d] \007(%d:%.2d) ### %s",
		    threadId,
		    minutes, seconds,
		    format);
	}
	else {
	    sprintf(fmtUsed, "[%d] \007(%d:%.2d:%.2d) ### %s",
		    threadId,
		    hours, minutes, seconds,
		    format);
	}
	break;
      case 3:
	if (hours == 0) {
	    sprintf(fmtUsed, "[%d] (%d:%.2d) %s %s",
		    threadId,
		    minutes, seconds,
		    traceStringCodes[ffs(mask)],
		    format);
	}
	else {
	    sprintf(fmtUsed, "[%d] (%d:%.2d:%.2d) %s %s",
		    threadId,
		    hours, minutes, seconds,
		    traceStringCodes[ffs(mask)],
		    format);
	}
	break;
    }
    vfprintf(file, fmtUsed, argsP);
    fflush(file);
    if (fmtUsed != localFmt)
	ITL_FREE(fmtUsed);
}

#ifdef __STDC__
EXPORT void itl_Message(IN char *format,...)
#else
EXPORT void itl_Message(format, va_alist)
    IN char *format;
    IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);

    PrintAMessage(1, 0, Cur_Out, format, ap);

    va_end(ap);
    return;
}
#ifdef __STDC__
EXPORT void itl_fMessage(IN FILE * file, IN char *format,...)
#else
EXPORT void itl_fMessage(file, format, va_alist)
  IN FILE *file;
  IN char *format;
  IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);

    PrintAMessage(1, 0, file, format, ap);

    va_end(ap);
    return;
}
#ifdef __STDC__
EXPORT void itl_AdviseUser(IN char *format,...)
#else
EXPORT void itl_AdviseUser(format, va_alist)
    IN char *format;
    IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);

    if (itl_PrintAdviseP()) {
	PrintAMessage(1, 0, Cur_Out, format, ap);
    }

    va_end(ap);
    return;
}
#ifdef __STDC__
EXPORT void itl_fAdviseUser(IN FILE * file, IN char *format,...)
#else
EXPORT void itl_fAdviseUser(file, format, va_alist)
  IN FILE *file;
  IN char *format;
  IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);

    if (itl_PrintAdviseP()) {
	PrintAMessage(1, 0, Cur_Out, format, ap);
    }

    va_end(ap);
    return;
}

#ifdef __STDC__
EXPORT void itl_Trace(IN unsigned int mask, IN char *format,...)
#else
EXPORT void itl_Trace(mask, format, va_alist)
    IN unsigned int mask;
    IN char *format;
    IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);
    if (mask & itl_traceLevel) {
	PrintAMessage(3, mask, Cur_Err, format, ap);
    }
    va_end(ap);
    return;
}
#ifdef __STDC__
EXPORT void itl_fTrace(IN FILE * file, IN unsigned int mask,
		       IN char *format,...)
#else
EXPORT void itl_fTrace(file, mask, format, va_alist)
  IN FILE *file;
  IN unsigned int mask;
  IN char *format;
  IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);
    if (mask & itl_traceLevel) {
	PrintAMessage(3, mask, file, format, ap);
    }
    va_end(ap);
    return;
}

/*
 *  Printing error messages:
 *	itl_PrintError -
 *	   A wrapper to PrintErrorMessage which calls PrintAMessage
 *	   providing Cur_Err as the file and 2 (error) as the mode.
 *	   Takes: Format, arguments.
 *	itl_fPrintError -
 *	   Similar to PrintError - for backwards compatability.
 */
#ifdef __STDC__
EXPORT void itl_PrintError(IN char *format,...)
#else
EXPORT void itl_PrintError(format, va_alist)
    IN char *format;
    IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);
    PrintAMessage(2, 0, Cur_Err, format, ap);
    va_end(ap);
}

#ifdef __STDC__
EXPORT void itl_fPrintError(IN FILE *file, IN char *format,...)
#else
EXPORT void itl_fPrintError(file, format, va_alist)
  IN FILE *file;
  IN char *format;
  IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);
    PrintAMessage(2, 0, file, format, ap);
    va_end(ap);
}

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *
 *	Routines for prompting the user:
 *
 *   PromptUser - A private function which is called with a FILE *
 *		  a format string and a var-arg list
 *   itl_PromptUser - Called with a print like format and calls
 *		      PromptUser providing Cur_Our as the FILE *
 *   itl_fPromptUser - Called with an fPrintf like format.
 *		       This should not be used but is provided for
 *		       backwards compatability.
 */

/*
 *  PromptUser
 *	Display a prompt to the user if necessary.
 *
 *  Parameters:
 *	file - The file pointer (FILE *)
 *	format - Format string to printf
 *	argsP - The var-arg list of parameters
 *
 *  Description:
 *      If necessary: The prompt is printed and a copy of it
 *	is kept in the thread safe global pointer lastPromptStrP
 *	associated with the buffer.  The global pointer is used if
 *	the user subsequently types an empty line.
 */
PRIVATE void PromptUser _TAKES((IN FILE *, IN char *, IN va_list));
PRIVATE void PromptUser(file, format, argsP)
  IN FILE *file;
  IN char *format;
  IN va_list argsP;
{
    if (itlIO_PrintPrompt()) {
	int pLen;
	char **lastPromptPP = itlIO_LastPromptStrP();
	pLen = vfprintf(Cur_Out, format, argsP);
	fflush(Cur_Out);

	/*
	 *  We printed the prompt string.  Now we know how long
	 *  it was.  We keep it around for additional prompting.
	 */
	if (*lastPromptPP) {
	    char *newPrompt;

	    /*
	     *  We cannot modify the prompt in place since the argsP
	     *  passed can contain a pointer to the current prompt.
	     */
	    ITL_ALLOC(newPrompt, char *, pLen + 1);
	    vsprintf(newPrompt, format, argsP);
	    ITL_FREE(*lastPromptPP);
	    *lastPromptPP = newPrompt;
	}
	itlIO_DonePrompting();
    }
}

#ifdef __STDC__
EXPORT void itl_PromptUser(IN char *format,...)
#else
EXPORT void itl_PromptUser(format, va_alist)
    IN char *format;
    IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);

    PromptUser(Cur_Out, format, ap);
    va_end(ap);
    return;
}

#ifdef __STDC__
EXPORT void itl_fPromptUser(IN FILE *file,IN char *format,...)
#else
EXPORT void itl_fPromptUser(file, format, va_alist)
    IN FILE *file;
    IN char *format;
    IN va_dcl
#endif /*__STDC__*/
{
    va_list ap;
    VA_START(ap, format);

    PromptUser(file, format, ap);
    va_end(ap);
    return;
}

SHARED void itlPrint_Init()
{
    time_t ansiTime;

#if	defined(_SVID_GETTOD)
    gettimeofday(&itlPrint_StartTime);
#else	/* _SVID_GETTOD */
    gettimeofday(&itlPrint_StartTime, (struct timezone *) 0);
#endif	/* _SVID_GETTOD */
    TimeToANSI(&itlPrint_StartTime, &ansiTime);
    itl_Message("The test is starting on %s", itl_AnsiTimeToStr(&ansiTime));
}

SHARED unsigned long itlPrint_ElapsedTime()
{
    unsigned long diff;
    struct timeval curTime, timeDiff;

#if	defined(_SVID_GETTOD)
    gettimeofday(&curTime);
#else	/* _SVID_GETTOD */
    gettimeofday(&curTime, (struct timezone *) 0);
#endif	/* _SVID_GETTOD */
    SubtractTime(&curTime, &itlPrint_StartTime, &timeDiff);
    diff = timeDiff.tv_usec / 1000 + timeDiff.tv_sec * 1000;
    return(diff);
}
