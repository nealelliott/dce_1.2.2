/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_utils.c,v $
 * Revision 1.1.116.1  1996/10/17  18:28:50  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:30  damon]
 *
 * Revision 1.1.111.1  1994/02/04  20:46:34  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:05  devsrc]
 * 
 * Revision 1.1.109.1  1993/12/07  17:45:38  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:33:27  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/11  18:32:25  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:13:24  htf]
 * 
 * Revision 1.1.2.4  1992/12/09  21:33:03  jaffe
 * 	Transarc delta: fred-ot6132-check-for-short-io 1.1
 * 	  Selected comments:
 * 	    To fix the read and write commands to check for short I/O.  Previously the
 * 	    user had to to this in the ITL script by looking at the CommandResult
 * 	    variable.  This option is still available if the user sets verifyStatus to
 * 	    false.
 * 	    Added the function, itlu_IsVerifyTrue() that returns the value of the
 * 	    verify status variable.
 * 	[1992/12/07  13:36:16  jaffe]
 * 
 * Revision 1.1.2.3  1992/11/18  22:02:33  jaffe
 * 	Transarc delta: itl-sanzi-initial-spoke-library 1.1
 * 	  Selected comments:
 * 	    Addition of Spoke Library to the suite of ITL libraries.
 * 	    Port of file_test and its libs to the Sun.
 * 	    Port this file to the Sun.
 * 	[1992/11/18  16:36:01  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:59:40  jaffe
 * 	Transarc delta: fred-itl-add-fcntl-flock 1.3
 * 	  Selected comments:
 * 	    To add fcntl and flock to the ITL file stuff.
 * 	    Also will fix some minor bugs, and convert to use Jeff Prem's utilities.
 * 	    This is a checkpoint.  I need to import a delta from Jeff.
 * 	    Update error codes to compile on OSF/1.
 * 	    Change init function to use pthread_once so it can safely be called multiple
 * 	    times.
 * 	    Complete the integration of Jeff's utility stuff and finish up work on
 * 	    fcntl and flock.
 * 	    Changed CheckResult to be more generic.
 * 	    Fixed VerifyStatus to set the correct ITL variable.
 * 	    Forgot to add the files that implement the file locking stuff. What
 * 	    a dufus.
 * 	Transarc delta: fred-itl-move-hashing-to-utils 1.1
 * 	  Selected comments:
 * 	    Moved the name hashing stuff from the itl/file sub-dir to itl/utils
 * 	    sub-dir so it is accessible to anyone writing ITL C code.
 * 	    Added itlu_NameHashInit() call to the itlu_Init() function.
 * 	Transarc delta: jdp-add-itl-stuff 1.8
 * 	  Selected comments:
 * 	    Add code for three new itl libraries: libitlUtils.a, libitlAggr.a, and
 * 	    libitlFset.a
 * 	    ITL utilities
 * 	    Update error message in itlu_CheckReturnCode() so that the offending command
 * 	    is printed.
 * 	    Print command name if an error is detected by itlu_CheckReturnCode().
 * 	    Added documentation for libitlUtils.a, fixed some bugs, and reorganized.
 * 	    Reorganized.
 * 	    Changed interface and implementation of result checking routines.
 * 	    Change name and usage of the variable used to control result checking.
 * 	    Provide routine for converting a value back to a name, assuming that
 * 	    the value--name mapping was constructed using itlu_ProcessValueMap().
 * 	    Provide itlu_ValueToName().
 * 	    Checkpointing in order to import another delta.
 * 	    Fix misc. compilation problems and close inorder to open another delta.
 * 	    Make this stuff compile on the pmax.
 * 	[1992/10/28  12:55:18  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992,1993 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <dce/dce_error.h>
#include <dcedfs/icl.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_io.h>

#include "itl_utils.h"
#include "itl_utilsNameHash.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/utils/itl_utils.c,v 1.1.116.1 1996/10/17 18:28:50 damon Exp $")


#define MAX_INVERSE_NAME	128

/*
 *  Space at front of check result variable names means it won't show up 
 *  in variable listings with the 'show' or 'showall' command.
 */
#define CHECK_RESULT_VAR_NAME	" itlu_CheckResult"
#define CHECK_RESULT_DEFAULT	TRUE

#define STATUS_VAR_NAME         "ITL_Status"

PRIVATE itlu_valueMap_t systemErrorCodes[] = {
    {"UKNOWN_ERROR",	ITLU_UNKNOWN_ERROR},
    {"EPERM",           EPERM},
    {"ENOENT",          ENOENT},
    {"ESRCH",           ESRCH},
    {"EINTR",           EINTR},
    {"EIO",             EIO},
    {"ENXIO",           ENXIO},
    {"E2BIG",           E2BIG},
    {"ENOEXEC",         ENOEXEC},
    {"EBADF",           EBADF},
    {"ECHILD",          ECHILD},
    {"EAGAIN",          EAGAIN},
    {"ENOMEM",          ENOMEM},
    {"EACCES",          EACCES},
    {"EFAULT",          EFAULT},
    {"ENOTBLK",         ENOTBLK},
    {"EBUSY",           EBUSY},
    {"EEXIST",          EEXIST},
    {"EXDEV",           EXDEV},
    {"ENODEV",          ENODEV},
    {"ENOTDIR",         ENOTDIR},
    {"EISDIR",          EISDIR},
    {"EINVAL",          EINVAL},
    {"ENFILE",          ENFILE},
    {"EMFILE",          EMFILE},
    {"ENOTTY",          ENOTTY},
    {"ETXTBSY",         ETXTBSY},
    {"EFBIG",           EFBIG},
    {"ENOSPC",          ENOSPC},
    {"ESPIPE",          ESPIPE},
    {"EROFS",           EROFS},
    {"EMLINK",          EMLINK},
    {"EPIPE",           EPIPE},
    {"EDOM",            EDOM},
    {"ERANGE",          ERANGE},
    {"ENOMSG",          ENOMSG},
    {"EIDRM",           EIDRM},
    {"EDEADLK",         EDEADLK},
    {"ENOLCK",          ENOLCK},
    {"ESTALE",          ESTALE},
    {"EINPROGRESS",     EINPROGRESS},
    {"EALREADY",        EALREADY},
    {"ENOTSOCK",        ENOTSOCK},
    {"EDESTADDRREQ",    EDESTADDRREQ},
    {"EMSGSIZE",        EMSGSIZE},
    {"EPROTOTYPE",      EPROTOTYPE},
    {"ENOPROTOOPT",     ENOPROTOOPT},
    {"EPROTONOSUPPORT", EPROTONOSUPPORT},
    {"ESOCKTNOSUPPORT", ESOCKTNOSUPPORT},
    {"EOPNOTSUPP",      EOPNOTSUPP},
    {"EPFNOSUPPORT",    EPFNOSUPPORT},
    {"EAFNOSUPPORT",    EAFNOSUPPORT},
    {"EADDRINUSE",      EADDRINUSE},
    {"EADDRNOTAVAIL",   EADDRNOTAVAIL},
    {"ENETDOWN",        ENETDOWN},
    {"ENETUNREACH",     ENETUNREACH},
    {"ENETRESET",       ENETRESET},
    {"ECONNABORTED",    ECONNABORTED},
    {"ECONNRESET",      ECONNRESET},
    {"ENOBUFS",         ENOBUFS},
    {"EISCONN",         EISCONN},
    {"ENOTCONN",        ENOTCONN},
    {"ESHUTDOWN",       ESHUTDOWN},
    {"ETIMEDOUT",       ETIMEDOUT},
    {"ECONNREFUSED",    ECONNREFUSED},
    {"EHOSTDOWN",       EHOSTDOWN},
    {"EHOSTUNREACH",    EHOSTUNREACH},
#if !defined(SUNOS5) && !defined(__hpux)
    {"EPROCLIM",        EPROCLIM},
#endif
    {"EUSERS",          EUSERS},
    {"ELOOP",           ELOOP},
    {"ENAMETOOLONG",    ENAMETOOLONG},
#ifndef	SUNOS5
    {"EDQUOT",          EDQUOT},
#endif
    {"EREMOTE",         EREMOTE},
    {"ENOSYS",          ENOSYS},
#ifdef EMEDIA    
    {"EMEDIA",          EMEDIA},
#endif
#ifdef ESOFT    
    {"ESOFT",           ESOFT},
#endif    
    {"ETOOMANYREFS",    ETOOMANYREFS},
    {"ENOSR",           ENOSR},
    {"ETIME",           ETIME},
    {"EBADMSG",         EBADMSG},
    {"EPROTO",          EPROTO},
#ifdef ENODATA    
    {"ENODATA",         ENODATA},
#endif    
    {"ENOSTR",          ENOSTR},
    {"EWOULDBLOCK",     EWOULDBLOCK},
    {"ENOTEMPTY",       ENOTEMPTY},

#ifdef _AIX
    {"ERESTART",        ERESTART},
    {"ESAD",            ESAD},
    {"ENOTRUST",        ENOTRUST},
    {"ECANCELED",       ECANCELED},
    {"EWRPROTECT",      EWRPROTECT},
    {"ENOCONNECT",      ENOCONNECT},
    {"ECLONEME",        ERESTART},
#endif  /* _AIX */

    {NULL}
};

/*
 *  The variables doTraceDump and traceDumpFP are used for doing ICL
 *  dumps when errors occur.  If doTraceDump is TRUE, we do the dumps, 
 *  with the output going to the traceDumpFP file pointer.  This is set
 *  in the itlu_TraceDumpOnError() function.
 */
PRIVATE boolean_t doTraceDump = FALSE;
PRIVATE FILE *traceDumpFP = (FILE *) 0;
PRIVATE unsigned int traceErrorCounter = 0;
PRIVATE pthread_mutex_t traceDumpMutex;

PRIVATE boolean_t itlu_ICLDump _TAKES((
				       void
				       ));


PRIVATE char itlu_InfoMsg[] = 
"The ITL Utilites module provides commands for common commands used by all \
file testing programs.";

EXPORT itl_commandTable_t itlu_Commands[] = {
    {"verifyStatus", itlu_VerifyStatus, 0, FALSE, COMMAND_MODE_ALL,
     "Switch to determine if file function codes should be verified.",
     "file_util",
     ""},
    {"iclDump", itlu_ICLDump, 0, FALSE, COMMAND_MODE_ALL,
     "Create an ICL kernel dump.",
     "file_util",
     ""}
};


PRIVATE boolean_t CmdWrapper _TAKES((
    IN int	rock
));

PRIVATE void UtilsInit _TAKES((
			       void
			       ));

PRIVATE void
UtilsInit()
{
    long code;

    (void)itl_SetIntVariable(CHECK_RESULT_VAR_NAME, CHECK_RESULT_DEFAULT,
                             var_local);
    (void)itl_SetIntVariable(STATUS_VAR_NAME, 0, var_local);

    if (code = itlu_DefineSystemErrorCodes())
    {
        itl_Error("UtilsInit - failed to define system error codes, [%ld]\n",
                  code);
        exit(-1);
    }

    itlu_NameHashInit();
    pthread_mutex_init(&traceDumpMutex, pthread_mutexattr_default);

    (void) itl_AddModuleCmds("file_util",
			     itlu_Commands,
			     sizeof(itlu_Commands) / 
			     sizeof(itl_commandTable_t),
			     COMMAND_MODE_ALL,
			     itlu_InfoMsg);

}       /* UtilsInit() */


EXPORT long
itlu_Init()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) UtilsInit))
        itl_Error("pthread_once failed in UtilsInit\n");

    return 0;
}

	
EXPORT boolean_t
itlu_VerifyStatus()
{
    int value;

    /*
     *  Force the value of the status variable to be 0.
     */
    (void)itl_SetIntVariable(STATUS_VAR_NAME, 0, var_local);
    value = itl_GetBoolean("Verify Status", "true", "false", "");
    (void)itl_SetIntVariable(CHECK_RESULT_VAR_NAME, value, var_local);
    itl_Message("Status verification is %s\n", (value ? "ON" : "OFF"));
    return(FALSE);
}


EXPORT long
itlu_ProcessValueMap(valueMap, inverseVarName)
    itlu_valueMap_t*	valueMap;
    char*		inverseVarName;
{
    itlu_valueMap_t*	mapEntryP;

    for (mapEntryP = valueMap; mapEntryP->name != NULL; mapEntryP++) {
	(void)itl_SetIntVariable(mapEntryP->name, mapEntryP->value, var_same);
	if (inverseVarName != NULL) {
	    char	varBuf[MAX_INVERSE_NAME];

	    (void)sprintf(varBuf, "%s[%ld]", inverseVarName, mapEntryP->value);
	    (void)itl_SetStrVariable(varBuf, mapEntryP->name, var_same);
	}
    }

    return 0;
}	/* itlu_ProcessValueMap() */


EXPORT long
itlu_ValueToName(value, inverseVarName, nameP)
    IN long	value;
    IN char*	inverseVarName;
    OUT char**	nameP;
{
    char	varBuf[MAX_INVERSE_NAME];

    (void)sprintf(varBuf, "%s[%ld]", inverseVarName, value);
    *nameP = itl_GetStringVariable(varBuf);

    return 0;
}	/* itlu_ValueToName() */


EXPORT long
itlu_DefineSystemErrorCodes()
{
    return itlu_ExtendKnownErrorCodes(systemErrorCodes);
}	/* itlu_DefineSystemErrors() */


EXPORT long
itlu_ExtendKnownErrorCodes(errorCodeMap)
    itlu_valueMap_t*	errorCodeMap;
{
    return itlu_ProcessValueMap(errorCodeMap, ITLU_ERROR_CODE_TO_NAME_VAR);
}	/* itlu_ExtendKnownErrorCodes() */


EXPORT long
itlu_GetInteger(resultP, shouldFreeP)
    OUT opaque*		resultP;
    OUT boolean_t*	shouldFreeP;
{
    boolean_t	parseError = FALSE;

    *resultP = (opaque)itl_GetInteger(&parseError);
    *shouldFreeP = FALSE;

    return parseError ? ITLU_UNKNOWN_ERROR : 0;
}	/* itlu_GetInteger() */


EXPORT long
itlu_GetString(resultP, shouldFreeP)
    OUT opaque*		resultP;
    OUT boolean_t*	shouldFreeP;
{
    boolean_t	parseError = FALSE;

    *resultP = itl_GetString(&parseError, 0);
    *shouldFreeP = TRUE;

    return parseError ? ITLU_UNKNOWN_ERROR : 0;
}	/* itlu_GetString() */


EXPORT long
itlu_Cnvt2StdCmdTbl(utilCmdTable, mode, class, stdCmdTableP, numEntriesP)
    IN itlu_commandTable_t*	utilCmdTable;
    IN unsigned long		mode;
    IN char*			class;
    OUT itl_commandTable_t**	stdCmdTableP;
    OUT unsigned*		numEntriesP;
{
    unsigned			i;
    unsigned			numEntries = 0;
    itl_commandTable_t*		stdCmdTable;
    itl_commandTable_t*		stdEntryP;
    itlu_commandTable_t*	utilEntryP = utilCmdTable;
    
    while(utilEntryP->command != NULL) {
	numEntries++;
	utilEntryP++;
    }

    ITL_UTIL_ALLOC(stdCmdTable, itl_commandTable_t*,
		    numEntries * sizeof *stdCmdTable);

    utilEntryP = utilCmdTable;
    stdEntryP = stdCmdTable;
    while (utilEntryP->command != NULL) {
	stdEntryP->command = utilEntryP->command;
	stdEntryP->function = CmdWrapper;
	stdEntryP->parameter = (int)utilEntryP;	/* XXX Bogus cast */
	stdEntryP->breakCom = utilEntryP->breakCom;
	stdEntryP->mode = mode;
	stdEntryP->description = utilEntryP->description;
	stdEntryP->class = class;
	stdEntryP->help = "";
	stdEntryP->nextCmdP = NULL;

	utilEntryP++;
	stdEntryP++;
    }

    *stdCmdTableP = stdCmdTable;
    *numEntriesP = numEntries;

    return 0;
}	/* itlu_Cnvt2StdCmdTbl() */


PRIVATE boolean_t
CmdWrapper(rock)
    IN int	rock;
{
    opaque*			argv;
    long			code = 0;
    unsigned			i;
    unsigned			numArgs = 0;
    boolean_t*			shouldFree;
    boolean_t			success = TRUE;
    itlu_commandTable_t*	utilEntryP = (itlu_commandTable_t*)rock;

    if (utilEntryP->argDesc) {
	itlu_argDesc_t*	argDescP = utilEntryP->argDesc;

	while (argDescP->name) {
	    numArgs++;
	    argDescP++;
	}

	ITL_UTIL_CALLOC(argv, opaque*, numArgs * sizeof *argv);
	ITL_UTIL_CALLOC(shouldFree, boolean_t*, numArgs * sizeof *shouldFree);
	
	for (i = 0; i < numArgs; i++) {
	    itl_PromptUser(utilEntryP->argDesc[i].prompt);
	    
	    code = (*utilEntryP->argDesc[i].parser)(&argv[i], &shouldFree[i]);
	    if (code) {
		char *errorMessage;

		itlu_ErrorMessage(code, &errorMessage);
		itl_PrintError("Error while trying to parse \"%s\": %s\n",
			       utilEntryP->argDesc[i].name, errorMessage);
		itlu_ErrorMessageFree(errorMessage);
		success = FALSE;
		break;
	    }
	}
    }

    if (success) {
	TRY {
	    code = (*utilEntryP->function)(utilEntryP->rock, argv);
	    success = itlu_CheckResult(code);
	} CATCH_ALL {
	    int		checkResult;
	    long	exception;
	    int		isStatus = 1;

	    checkResult = itl_GetIntVariable(CHECK_RESULT_VAR_NAME);
	    if (exc_get_status(THIS_CATCH, &exception) == -1) {
		exception = (long)THIS_CATCH;
		isStatus = 0;
	    }

	    if (checkResult) {
		if (isStatus)
		    itl_Error("COMMAND FAILURE, exception=%ld\n", exception);
		else
		    itl_Error("COMMAND FAILURE, exception=%#lx\n", exception);
		itlIO_TraceCurrentLocation();
		success = FALSE;
	    } else {
		success = TRUE;
	    }

	    (void)itl_SetIntVariable(STATUS_VAR_NAME, exception, var_local);
	} ENDTRY
    }

    if (utilEntryP->argDesc) {
	for (i = 0; i < numArgs; i++)
	    if (argv[i] && shouldFree[i])
		ITL_UTIL_FREE(argv[i]);

	ITL_UTIL_FREE(shouldFree);
	ITL_UTIL_FREE(argv);
    }

    return success;
}	/* CmdWrapper() */

/*
 *  itlu_ErrorMessage() --
 *    This routine returns an error message for the given code.  It handles
 *    both DCE and UNIX (errno) errors.
 *
 *  NOTE:
 *    This function allocates memory for the errorMesgPP parameter.  This
 *    must be free'd by the caller via the itlu_ErrorMessageFree() function.
 */
EXPORT void itlu_ErrorMessage(errorCode, errorMesgPP)
  IN long errorCode;   /* error code */
  OUT char **errorMesgPP;        /* returned error message */
{
    int returnStatus;

    /*
     *  Assume that errno string will always be less than max. dce strings.
     */
    ITL_ALLOC(*errorMesgPP, char *, dce_c_error_string_len);

    /*
     *  First check that it is a valid error value.
     */
    if (errorCode == ITLU_UNKNOWN_ERROR) {
	strcpy(*errorMesgPP, "Uknown error");
    }
    else
    {
	dce_error_inq_text(errorCode, (unsigned char *) *errorMesgPP, 
			   &returnStatus);
	if (returnStatus)
	{
	    /*
	     *  Must be a errno error.
	     */
	    strcpy(*errorMesgPP, strerror((int) errorCode));
	}
    }
}

/*
 *  itlu_ErrorMessageFree() --
 *    Free the buffer allocated from a call to itlu_ErrorMessage()
 */
EXPORT void itlu_ErrorMessageFree(errorMesgP)
  IN char *errorMesgP;
{
    ITL_FREE(errorMesgP);
}

EXPORT void itlu_DCEError(error, errorMessage)
  IN unsigned long error;   /* the error code */
  IN char *errorMessage;    /* caller's additional message to print. */
{
    char *dceErrorMessage;

    itlu_ErrorMessage(error, &dceErrorMessage);
    itl_Error("DCE ERROR: %s -\n\t[%d] %s\n", 
	      errorMessage, error, dceErrorMessage);
    itlu_ErrorMessageFree(dceErrorMessage);
}

/*
 *  itlu_ICLDump()
 *    Provides an interface for a script writer to invoke an ICL dump.
 */
PRIVATE boolean_t 
itlu_ICLDump()
{
    int status;
    char *fileName, *dumpStatus, fullFileName[256];
    boolean_t parseError;
    FILE *dumpFP;


    itl_PromptUser("ICL Dump File: ");
    fileName = itl_GetString(&parseError, 0);

    sprintf(fullFileName, "%s.%d", fileName, (int) getpid());
    ITL_FREE(fileName);
    assert((int) strlen(fullFileName) < 256);
    pthread_mutex_lock(&traceDumpMutex);
    if ((dumpFP = fopen(fullFileName, "a")) != (FILE *) 0)
    {
	traceErrorCounter++;
	itl_Message("Generating TRACE DUMP number %d, please wait.\n", 
		    traceErrorCounter);
	fprintf(dumpFP, "\nStart TRACE DUMP number %d\n\n",
		traceErrorCounter);

	if (icl_DumpKernel(dumpFP, NULL, ICL_DUMP_FORMATTED) == 0)
	    dumpStatus = "Successfully";
	else
	    dumpStatus = "UNSUCCESSFULLY";

	fprintf(dumpFP, "\n%s completed TRACE DUMP number %d\n\n",
		dumpStatus, traceErrorCounter);

	if ((status = fclose(dumpFP)) != 0)
	{
	    itl_Error("FAILED to close dump file, return status %d\n", 
		      status);
	}
    }
    pthread_mutex_unlock(&traceDumpMutex);

    return(FALSE);
}

EXPORT boolean_t
itlu_TraceDumpOnError(doDump, dumpFile)
  IN boolean_t doDump;
  IN char *dumpFile;     /* ignored if doDump is FALSE */
{
    boolean_t retValue = FALSE;
    char fullFileName[256];
    int status;

    /*
     *  Yes, the = below is correct.  We both set the value of doTraceDump
     *  and determine if it is TRUE in one statement.
     */
    if (doTraceDump = doDump)
    {
	if (traceDumpFP != (FILE *) 0)
	    itl_AdviseUser("Warning:  Overwriting existing dump file pointer.\n");
	sprintf(fullFileName, "%s.%d", dumpFile, (int) getpid());
	assert((int) strlen(fullFileName) < 256);
	if ((traceDumpFP = fopen(fullFileName, "w")) == (FILE *) 0)
	    retValue = TRUE;
    }
    else
    {
	if (traceDumpFP == (FILE *) 0)
	    itl_AdviseUser("Warning:  Trying to close un-opened dump file.\n");
	else
	{
	    if ((status = fclose(traceDumpFP)) != 0)
	    {
		itl_Error("FAILED to close dump file, return status %d\n", 
			  status);
		retValue = TRUE;
	    }
	    traceDumpFP = (FILE *) 0;
	}
    }

    return(retValue);
}


EXPORT boolean_t
itlu_CheckResult(code)
    IN long	code;
{
    int		checkResult = itl_GetIntVariable(CHECK_RESULT_VAR_NAME);
    boolean_t	eof;
    char	*errorName, *errorMessage;
    boolean_t	success = TRUE;
    char	*x, *y;

    if (code)
	switch (checkResult) {
	  case TRUE:
	    (void)itlu_ValueToName(code, ITLU_ERROR_CODE_TO_NAME_VAR,
				   &errorName);
	    if (errorName == NULL)
		x = "?";
	    else
		x = errorName;
	    itlu_ErrorMessage(code, &errorMessage);
	    if (errorMessage == NULL)
		y = "?";
	    else
		y = errorMessage;
	    itl_Error("COMMAND FAILURE, code=%ld (%s: %s)\n",
		      code, x, y);
	    itlIO_TraceCurrentLocation();
	    /*
	     *  If user requested trace dumps, then do it.  Use a mutex
	     *  to ensure two threads don't dump to the file at the
	     *  same time.
	     */
	    if (doTraceDump)
	    {
		char *dumpStatus;
		int status;

		pthread_mutex_lock(&traceDumpMutex);
		traceErrorCounter++;
		itl_Message("Generating TRACE DUMP number %d, please wait.\n",
			    traceErrorCounter);
		assert(traceDumpFP);
		fprintf(traceDumpFP, "\nCOMMAND FAILURE, code=%ld (%s: %s)\n",
			  code, x, y);
		fprintf(traceDumpFP, "\nStart TRACE DUMP number %d\n\n",
			traceErrorCounter);

		if (icl_DumpKernel(traceDumpFP, NULL, ICL_DUMP_FORMATTED) == 0)
		    dumpStatus = "Successfully";
		else
		    dumpStatus = "UNSUCCESSFULLY";

		fprintf(traceDumpFP, "\n%s completed TRACE DUMP number %d\n\n",
			dumpStatus, traceErrorCounter);

		if ((status = fflush(traceDumpFP)) != 0)
		{
		    itl_Error("FAILED to flush dump file, return status %d\n", 
			      status);
		}

		pthread_mutex_unlock(&traceDumpMutex);
	    }
	    ITL_UTIL_FREE(errorName);
	    itlu_ErrorMessageFree(errorMessage);
	    success = FALSE;
	    break;

	  case FALSE:
	    break;

	  default:
	    itl_PrintError("Unknown value for $%s: %d\n",
			   CHECK_RESULT_VAR_NAME, checkResult);
	    success = FALSE;
	    break;
	}

    (void) itl_SetIntVariable(STATUS_VAR_NAME, code, var_local);

    return success;
}	/* itlu_CheckResult() */

/*
 *  itlu_IsVerifyTrue() --
 *    Returns the value of the CHECK_RESULT_VAR_NAME variable, which happens
 *    to be TRUE if it is set and FALSE if it is not.
 */
EXPORT boolean_t itlu_IsVerifyTrue()
{
    return(itl_GetIntVariable(CHECK_RESULT_VAR_NAME));
}

EXPORT long
itlu_System(command, argv)
    char*	command;
    char*	argv[];
{
    int childExitStatus;
    int	childStatus;
    int devNull;
    int signo;

    switch (fork()) {
      default:				/* Parent */
	if (wait(&childStatus) == -1) {
	    itl_PrintError("itlu_System: wait(2) failed\n");
	    return errno;
	}
	if ((childStatus & 0x7f) == 0x7f) {
	    signo = ((childStatus >> 8) & 0xff);
	    itl_Message("%s stopped, signo = %d\n", command, signo);
	    return signo;
	} else if (childStatus & 0xff) {
	    signo = (childStatus & 0x7f);
	    itl_Message("%s received a signal, signo = %d\n", command, signo);
	    return signo;
	} else if (childExitStatus = ((childStatus >> 8) & 0xff)) {
	    itl_Message("%s returned a non-zero exit status, status = %d\n",
			   command, childExitStatus);
	    return childExitStatus;
	}
	break;

      case 0:				/* Child */
	if ((devNull = open("/dev/null", O_WRONLY)) == -1) {
	    itl_PrintError("itlu_System: Can't open %s: %s\n",
			   "/dev/null", strerror(errno));
	    exit(errno);
	}

	/* Redirect noise (standard output) to /dev/null */
	if (dup2(devNull, 1) == -1) {
	    itl_PrintError("Dup2 failed: %s\n", strerror(errno));
	    exit(errno);
	}

	execvp(command, argv);

	/* Exec failed */
	itl_PrintError("Exec failed: %s\n", strerror(errno));
	exit(errno);
	break;

      case -1:				/* Fork failed */
	itl_PrintError("itlu_System: fork(2) failed\n");
	return errno;
	break;
    }

    return 0;
}	/* itlu_System() */
