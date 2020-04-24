/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: util_signal.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:25:27  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/10  18:58 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts11/1  1995/02/10  18:57 UTC  truitt
 * 	CHFts14285 partial: Add support to pass the error
 * 	code back to main, instead of just the error message.
 * 	[1995/12/08  17:58:27  root]
 * 
 * Revision 1.1.2.9  1994/09/14  19:32:26  coren
 * 	Wrapped entire file in "#ifdef _DCECP_TEST" so as not to expose
 * 	thread-unsafe signal-handling code in "production" version. (CR 11832)
 * 	[1994/09/13  18:27:39  coren]
 * 
 * Revision 1.1.2.8  1994/08/23  15:02:10  rousseau
 * 	Back up to previous version (CR 11823).
 * 	[1994/08/23  15:01:54  rousseau]
 * 
 * Revision 1.1.2.6  1994/07/13  18:00:07  mori_m
 * 	CR 11268  Tcl_SignalId() needs a new flag as argument, since an error
 * 		  message returned from dce_msg_get needs to be freed later.
 * 	[1994/07/13  17:58:31  mori_m]
 * 
 * Revision 1.1.2.5  1994/06/09  16:02:40  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:17  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/06  15:46:11  rousseau
 * 	Moved signal messahes to sams file.
 * 	[1994/06/06  15:44:00  rousseau]
 * 
 * Revision 1.1.2.3  1994/01/10  19:54:55  rousseau
 * 	Fix rios build problems.
 * 	[1994/01/10  19:53:37  rousseau]
 * 
 * Revision 1.1.2.2  1993/12/21  19:44:26  rousseau
 * 	Moved definition of tclXWrongArgs here from util_signal.h .
 * 	[1993/12/21  19:43:43  rousseau]
 * 
 * Revision 1.1.2.1  1993/12/21  16:19:25  rousseau
 * 	Initial version.
 * 	[1993/12/21  16:19:12  rousseau]
 * 
 * $EndLog$
 */

/*
 * tclXsignal.c --
 *
 * Tcl Unix signal support routines and the signal and commands.
 *-----------------------------------------------------------------------------
 * Copyright 1991-1993 Karl Lehenbauer and Mark Diekhans.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies.  Karl Lehenbauer and
 * Mark Diekhans make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *-----------------------------------------------------------------------------
 * $Id: util_signal.c,v 1.1.4.2 1996/02/18 19:15:23 marty Exp $
 *-----------------------------------------------------------------------------
 */

/* Don't compile any of this if _DCECP_TEST not defined */

#ifdef _DCECP_TEST

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <util_signal.h>
#include <util_var.h>


#ifndef SIGCLD
#   define SIGCLD SIGCHLD
#endif
#ifndef SIGCHLD
#   define SIGCHLD SIGCLD
#endif

#ifndef MAXSIG
#  ifdef NSIG
#    define MAXSIG NSIG
#  else
#    define MAXSIG 32
#  endif
#endif

/*
 * Signal name table maps name to number.
 */

#define SIG_NAME_MAX 9  /* Maximum length of any signal name */

static struct {char *name;
        short num;
       } sigNameTable [] = {
#ifdef SIGABRT
    "ABRT",    SIGABRT,
#endif
#ifdef SIGALRM
    "ALRM",    SIGALRM,
#endif
#ifdef SIGBUS
    "BUS",     SIGBUS,
#endif
#ifdef SIGCHLD
    "CHLD",    SIGCHLD,
#endif
#ifdef SIGCLD
    "CLD",     SIGCLD,
#endif
#ifdef SIGCONT
    "CONT",    SIGCONT,
#endif
#ifdef SIGEMT
    "EMT",     SIGEMT,
#endif
#ifdef SIGFPE
    "FPE",     SIGFPE,
#endif
#ifdef SIGHUP
    "HUP",     SIGHUP,
#endif
#ifdef SIGILL
    "ILL",     SIGILL,
#endif
#ifdef SIGINT
    "INT",     SIGINT,
#endif
#ifdef SIGIO
    "IO",      SIGIO,
#endif
#ifdef SIGIOT
    "IOT",     SIGIOT,
#endif
#ifdef SIGKILL
    "KILL",    SIGKILL,
#endif
#ifdef SIGLOST
    "LOST",    SIGLOST,
#endif
#ifdef SIGPIPE
    "PIPE",    SIGPIPE,
#endif
#ifdef SIGPOLL
    "POLL",    SIGPOLL,
#endif
#ifdef SIGPROF
    "PROF",    SIGPROF,
#endif
#ifdef SIGPWR
    "PWR",     SIGPWR,
#endif
#ifdef SIGQUIT
    "QUIT",    SIGQUIT,
#endif
#ifdef SIGSEGV
    "SEGV",    SIGSEGV,
#endif
#ifdef SIGSTOP
    "STOP",    SIGSTOP,
#endif
#ifdef SIGSYS
    "SYS",     SIGSYS,
#endif
#ifdef SIGTERM
    "TERM",    SIGTERM,
#endif
#ifdef SIGTRAP
    "TRAP",    SIGTRAP,
#endif
#ifdef SIGTSTP
    "TSTP",    SIGTSTP,
#endif
#ifdef SIGTTIN
    "TTIN",    SIGTTIN,
#endif
#ifdef SIGTTOU
    "TTOU",    SIGTTOU,
#endif
#ifdef SIGURG
    "URG",     SIGURG,
#endif
#ifdef SIGUSR1
    "USR1",    SIGUSR1,
#endif
#ifdef SIGUSR2
    "USR2",    SIGUSR2,
#endif
#ifdef SIGVTALRM
    "VTALRM",  SIGVTALRM,
#endif
#ifdef SIGWINCH
    "WINCH",   SIGWINCH,
#endif
#ifdef SIGXCPU
    "XCPU",    SIGXCPU,
#endif
#ifdef SIGXFSZ
    "XFSZ",    SIGXFSZ,
#endif
    NULL,         -1};

#ifndef RETSIGTYPE
#   define RETSIGTYPE void
#endif

typedef RETSIGTYPE (*signalProcPtr_t) _ANSI_ARGS_((int));

/*
 * Class of actions that can be set by the signal command.
 */
#define SIGACT_SET     1   /* Set the signal     */
#define SIGACT_GET     2   /* Get the signal     */
#define SIGACT_BLOCK   3   /* Block the signal   */
#define SIGACT_UNBLOCK 4   /* Unblock the signal */

/*
 * Defines if this is not Posix.
 */
#ifndef SIG_BLOCK
#   define SIG_BLOCK       1
#   define SIG_UNBLOCK     2
#endif

/*
 * Structure used to save error state of the interpreter.
 */
typedef struct {
    char  *result;
    char  *errorInfo;
    char  *errorCode;
} errState_t;

/*
 * Table containing a interpreters and there Async handler cookie.
 */
typedef struct {
    Tcl_Interp       *interp;
    Tcl_AsyncHandler  handler;
} interpHandler_t;

static interpHandler_t *interpTable = NULL;
static int              interpTableSize  = 0;
static int              numInterps  = 0;

/*
 * A flag indicating that an "error" signal has occured.  This is used to
 * flush interactive input in commands and is only cleared there.  Also an
 * application-supplied function to call if a error signal occurs.  This
 * normally flushes command input.
 */
int tclGotErrorSignal = FALSE;
void (*tclErrorSignalProc) _ANSI_ARGS_((int signalNum)) = NULL;

/*
 * Counters of signals that have occured but have not been processed.
 */
static unsigned signalsReceived [MAXSIG];

/*
 * Table of commands to evaluate when a signal occurs.  If the command is
 * NULL and the signal is received, an error is returned.
 */
static char *signalTrapCmds [MAXSIG];

/*
 * Pointer to background error handler (normally NULL or Tk_BackgroundError).
 */
void (*tclSignalBackgroundError) _ANSI_ARGS_((Tcl_Interp *interp)) = NULL;

/*
 * Prototypes of internal functions.
 */
static int
SigNameToNum _ANSI_ARGS_((char *sigName));

static int
GetSignalState _ANSI_ARGS_((int              signalNum,
                            signalProcPtr_t *sigProcPtr));

static int
SetSignalAction _ANSI_ARGS_((int             signalNum,
                             signalProcPtr_t sigFunc));

static RETSIGTYPE
TclSignalTrap _ANSI_ARGS_((int signalNum));

static int
FormatTrapCode  _ANSI_ARGS_((Tcl_Interp  *interp,
                             int          signalNum,
                             Tcl_DString *command));

static errState_t *
SaveErrorState _ANSI_ARGS_((Tcl_Interp *interp));

static void
RestoreErrorState _ANSI_ARGS_((Tcl_Interp *interp,
                               errState_t *errStatePtr));

static int
EvalTrapCode _ANSI_ARGS_((Tcl_Interp *interp,
                          int         signalNum));

static int
ProcessASignal _ANSI_ARGS_((Tcl_Interp *interp,
                            int         signalNum));

static int
ParseSignalList _ANSI_ARGS_((Tcl_Interp *interp,
                             char       *signalListStr,
                             int         signalList []));

static char *
SignalBlocked _ANSI_ARGS_((Tcl_Interp  *interp,
                           int          signalNum));

static int
GetSignalStates  _ANSI_ARGS_((Tcl_Interp *interp,
                              int         signalListSize,
                              int         signalList [MAXSIG]));

static int
SetSignalStates  _ANSI_ARGS_((Tcl_Interp      *interp,
                              int              signalListSize,
                              int              signalList [MAXSIG],
                              signalProcPtr_t  actionFunc,
                              char            *command));

static int
BlockSignals _ANSI_ARGS_((Tcl_Interp  *interp,
                          int          action,
                          int          signalListSize,
                          int          signalList [MAXSIG]));

static void
SignalCmdCleanUp _ANSI_ARGS_((ClientData  clientData,
                              Tcl_Interp *interp));

extern void
panic _ANSI_ARGS_(());



/*
 *-----------------------------------------------------------------------------
 *
 * SigNameToNum --
 *     Converts a UNIX signal name to its number, returns -1 if not found.
 *     the name may be upper or lower case and may optionally have the 
 *     leading "SIG" omitted.
 *
 *-----------------------------------------------------------------------------
 */
static int
SigNameToNum (sigName)
    char *sigName;
{
    char  sigNameUp [SIG_NAME_MAX+1];  /* Upshifted signal name */
    char *sigNamePtr; 
    int   idx;

    /*
     * Copy and upshift requested name.
     */

    if (strlen (sigName) > SIG_NAME_MAX)
        return -1;   /* Name too long */

    Tcl_UpShift (sigNameUp, sigName);

    if (STRNEQU (sigNameUp, "SIG", 3))
        sigNamePtr = &sigNameUp [3];
    else
        sigNamePtr = sigNameUp;

    for (idx = 0; sigNameTable [idx].num != -1; idx++)
        if (STREQU (sigNamePtr, sigNameTable [idx].name))
            break;

    return sigNameTable [idx].num;
}

/*
 *-----------------------------------------------------------------------------
 *
 * GetSignalState --
 *     Get the current state of the specified signal.
 * Parameters:
 *   o signalNum (I) - Signal number to query.
 *   o sigProcPtr (O) - The signal function is returned here.
 * Results
 *   TCL_OK or TCL_ERROR (check errno).
 *-----------------------------------------------------------------------------
 */
static int
GetSignalState (signalNum, sigProcPtr)
    int              signalNum;
    signalProcPtr_t *sigProcPtr;
{
#ifdef HAVE_SIGACTION
    struct sigaction currentState;

    if (sigaction (signalNum, NULL, &currentState) < 0)
        return TCL_ERROR;
    *sigProcPtr = currentState.sa_handler;
    return TCL_OK;
#else
    signalProcPtr_t  actionFunc;

    if (signalNum == SIGKILL)
         actionFunc = SIG_DFL;
    else
        actionFunc = signal (signalNum, SIG_DFL);
    if (actionFunc == SIG_ERR)
        return TCL_ERROR;
    if (actionFunc != SIG_DFL)
        signal (signalNum, actionFunc);  /* reset */
    *sigProcPtr = actionFunc;
    return TCL_OK;
#endif
}

/*
 *-----------------------------------------------------------------------------
 *
 * SetSignalAction --
 *     Set the action to occur when a signal is received.
 * Parameters:
 *   o signalNum (I) - Signal number to query.
 *   o sigFunc (O) - The signal function or SIG_DFL or SIG_IGN.
 * Results
 *   TCL_OK or TCL_ERROR (check errno).
 *-----------------------------------------------------------------------------
 */
static int
SetSignalAction (signalNum, sigFunc)
    int             signalNum;
    signalProcPtr_t sigFunc;
{
#ifdef HAVE_SIGACTION
    struct sigaction newState;
    
    newState.sa_handler = sigFunc;
    sigfillset (&newState.sa_mask);
    newState.sa_flags = 0;

    if (sigaction (signalNum, &newState, NULL) < 0)
        return TCL_ERROR;

    return TCL_OK;
#else
    if (signal (signalNum, sigFunc) == SIG_ERR)
        return TCL_ERROR;
    else
        return TCL_OK;
#endif
}

/*
 *-----------------------------------------------------------------------------
 *
 * TclSignalTrap --
 *
 *   Trap handler for UNIX signals.  Sets tells all registered interpreters
 * that a trap has occured and saves the trap info.  The first interpreter to
 * call it's async signal handler will process all pending signals.
 *-----------------------------------------------------------------------------
 */
static RETSIGTYPE
TclSignalTrap (signalNum)
    int signalNum;
{
    int idx;

    /*
     * Record the count of the number of this type of signal that has occured
     * and tell all the interpreters to call the async handler when safe.
     */
    signalsReceived [signalNum]++;

    for (idx = 0; idx < numInterps; idx++)
        Tcl_AsyncMark (interpTable [idx].handler);

    /*
     * Flag used by command input functions to flush input.
     */
    if (signalTrapCmds [signalNum] == NULL) {
        tclGotErrorSignal = TRUE;
        if (tclErrorSignalProc != NULL)
            (*tclErrorSignalProc) (signalNum);
    }
#ifndef HAVE_SIGACTION
    /*
     * For old-style Unix signals, the signal must be explictly re-enabled.
     * Not done for SIGCHLD, as we would continue to the signal until the
     * wait is done.  This is fixed by Posix signals and is not necessary under
     * BSD, but it done this way for consistency.
     */
    if (signalNum != SIGCHLD) {
        if (SetSignalAction (signalNum, TclSignalTrap) == TCL_ERROR)
            panic ((char *)dce_sprintf(dcp_s_signal_bug));
    }
#endif
}

/*
 *-----------------------------------------------------------------------------
 *
 * SaveErrorState --
 *  
 *   Save the error state of the interpreter (result, errorInfo and errorCode).
 *
 * Parameters:
 *   o interp (I) - The interpreter to save. Result will be reset.
 * Returns:
 *   A dynamically allocated structure containing all three strings,  Its
 * allocated as a single malloc block.
 *-----------------------------------------------------------------------------
 */
static errState_t *
SaveErrorState (interp)
    Tcl_Interp *interp;
{
    errState_t *errStatePtr;
    char       *errorInfo, *errorCode, *nextPtr;
    int         len;

    errorInfo = Tcl_GetVar (interp, "errorInfo", TCL_GLOBAL_ONLY);
    errorCode = Tcl_GetVar (interp, "errorCode", TCL_GLOBAL_ONLY);

    len = sizeof (errState_t) + strlen (interp->result) + 1;
    if (errorInfo != NULL)
        len += strlen (errorInfo) + 1;
    if (errorCode != NULL)
        len += strlen (errorCode) + 1;


    errStatePtr = (errState_t *) ckalloc (len);
    nextPtr = ((char *) errStatePtr) + sizeof (errState_t);

    errStatePtr->result = nextPtr;
    strcpy (errStatePtr->result, interp->result);
    nextPtr += strlen (interp->result) + 1;

    errStatePtr->errorInfo = NULL;
    if (errorInfo != NULL) {
        errStatePtr->errorInfo = nextPtr;
        strcpy (errStatePtr->errorInfo, errorInfo);
        nextPtr += strlen (errorInfo) + 1;
    }

    errStatePtr->errorCode = NULL;
    if (errorCode != NULL) {
        errStatePtr->errorCode = nextPtr;
        strcpy (errStatePtr->errorCode, errorCode);
        nextPtr += strlen (errorCode) + 1;
    }

    Tcl_ResetResult (interp);
    return errStatePtr;
}

/*
 *-----------------------------------------------------------------------------
 *
 * RestoreErrorState --
 *  
 *   Restore the error state of the interpreter that was saved by
 * SaveErrorState.
 *
 * Parameters:
 *   o interp (I) - The interpreter to save.
 *   o errStatePtr (I) - Error state from SaveErrorState.  This structure will
 *     be freed. 
 * Returns:
 *   A dynamically allocated structure containing all three strings,  Its
 * allocated as a single malloc block.
 *-----------------------------------------------------------------------------
 */
static void
RestoreErrorState (interp, errStatePtr)
    Tcl_Interp *interp;
    errState_t *errStatePtr;
{
    Tcl_SetResult (interp, errStatePtr->result, TCL_VOLATILE);
    if (errStatePtr->errorInfo != NULL)
        Tcl_SetVar (interp, "errorInfo", errStatePtr->errorInfo,
                    TCL_GLOBAL_ONLY);
    if (errStatePtr->errorCode != NULL)
        Tcl_SetVar (interp, "errorCode", errStatePtr->errorCode,
                    TCL_GLOBAL_ONLY);

    ckfree (errStatePtr);
}

/*
 *-----------------------------------------------------------------------------
 *
 * FormatTrapCode --
 *     Format the signal name into the signal trap command.  Replacing %S with
 * the signal name.
 *
 * Parameters:
 *   o interp (I/O) - The interpreter to return errors in.
 *   o signalNum (I) - The signal number of the signal that occured.
 *   o command (O) - The resulting command adter the formatting.
 *-----------------------------------------------------------------------------
 */
static int
FormatTrapCode (interp, signalNum, command)
    Tcl_Interp  *interp;
    int          signalNum;
    Tcl_DString *command;
{
    char  *signalName, *copyPtr, *scanPtr;
    int   i18n_result = 0;

    /*
     * Force name to always be SIGCHLD, even if system defines only SIGCLD.
     */
    if (signalNum == SIGCHLD)
        signalName = "SIGCHLD";
    else
        signalName = Tcl_SignalId (signalNum, &i18n_result);

    Tcl_DStringInit (command);

    copyPtr = scanPtr = signalTrapCmds [signalNum];

    while (*scanPtr != '\0') {
        if (*scanPtr != '%') {
            scanPtr++;
            continue;
        }
        if (scanPtr [1] == '%') {
            scanPtr += 2;
            continue;
        }
        Tcl_DStringAppend (command, copyPtr, (scanPtr - copyPtr));

        switch (scanPtr [1]) {
          case 'S': {
              Tcl_DStringAppend (command, signalName, -1);
	      if (i18n_result)
		  free(signalName);
              break;
          }
          default:
            goto badSpec;
        }
        scanPtr += 2;
        copyPtr = scanPtr;
    }
    Tcl_DStringAppend (command, copyPtr, copyPtr - scanPtr);

    return TCL_OK;

    /*
     * Handle bad % specification currently pointed to by scanPtr.
     */
  badSpec:
    {
        char badSpec [2];
        
        badSpec [0] = scanPtr [1];
        badSpec [1] = '\0';
        DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_bad_signal_format, badSpec);
        return TCL_ERROR;
    }
}

/*
 *-----------------------------------------------------------------------------
 *
 * EvalTrapCode --
 *     Run code as the result of a signal.  The symbolic signal name is
 * formatted into the command replacing %S with the symbolic signal name.
 *
 * Parameters:
 *   o interp (I) - The interpreter to run the signal in. If an error
 *     occures, then the result will be left in the interp.
 *   o signalNum (I) - The signal number of the signal that occured.
 * Return:
 *   TCL_OK or TCL_ERROR.
 *-----------------------------------------------------------------------------
 */
static int
EvalTrapCode (interp, signalNum)
    Tcl_Interp *interp;
    int         signalNum;
{
    int          result;
    char         *tmp;
    Tcl_DString  command;

    Tcl_ResetResult (interp);

    /*
     * Format the signal name into the command.  This also allows the signal
     * to be reset in the command.
     */

    result = FormatTrapCode (interp,
                             signalNum,
                             &command);
    if (result == TCL_OK)
        result = Tcl_GlobalEval (interp, 
                                 command.string);

    Tcl_DStringFree (&command);

    if (result == TCL_ERROR) {
        char errorInfo [64];
	char *i18n_signal_name;
	int  i18n_result = 0;

	i18n_signal_name = Tcl_SignalId(signalNum, &i18n_result);
        tmp = (char *)dce_sprintf(dcp_t_signal_trap_info, i18n_signal_name);
        DCP_GENERAL_SET_E_VAR(dcp_t_signal_trap_info);
	if (i18n_result)
		free(i18n_signal_name);
        sprintf(errorInfo, tmp);
        free(tmp);

        Tcl_AddErrorInfo (interp, errorInfo);
        return TCL_ERROR;
    }
    
    Tcl_ResetResult (interp);
    return TCL_OK;
}

/*
 *-----------------------------------------------------------------------------
 *
 * ProcessASignal --
 *  
 *   Do processing on the specified signal.
 *
 * Parameters:
 *   o interp (O) - Result will contain the result of the signal handling
 *     code that was evaled.
 *   o signalNum - The signal to process.
 * Returns:
 *   TCL_OK or TCL_ERROR.
 *-----------------------------------------------------------------------------
 */
static int
ProcessASignal (interp, signalNum)
    Tcl_Interp *interp;
    int         signalNum;
{
    char *signalName;
    int   result = TCL_OK;
    int   i18n_result = 0;

    /*
     * Either return an error or evaluate code associated with this signal.
     * If evaluating code, call it for each time the signal occured.
     */
    if (signalTrapCmds [signalNum] == NULL) {
        signalsReceived [signalNum] = 0;

        /*
         * Force name to always be SIGCHLD, even if system defines only SIGCLD.
         */
        if (signalNum == SIGCHLD)
            signalName = "SIGCHLD";
        else
            signalName = Tcl_SignalId (signalNum, &i18n_result);

        Tcl_SetErrorCode (interp, "POSIX", "SIG", signalName, (char*) NULL);
	if (i18n_result)
		free(signalName);
        DCP_APPEND_RESULT_CODE(dcp_t_signal_received); 
        Tcl_SetVar (interp, "errorInfo", "", TCL_GLOBAL_ONLY);
        result = TCL_ERROR;
    } else {
        while (signalsReceived [signalNum] > 0) {
            (signalsReceived [signalNum])--;
            result = EvalTrapCode (interp, signalNum);
            if (result == TCL_ERROR)
                break;
        }
    }
    return result;
}

/*
 *-----------------------------------------------------------------------------
 *
 * Tcl_ProcessSignals --
 *  
 *   Called by Tcl_Eval, etc to process pending signals in a safe state
 * interpreter state.  This is often called just after a command completes.
 * The results of the command are passed to this procedure and may be altered
 * by it.  If trap code is specified for the signal that was received, then
 * the trap will be executed, otherwise an error result will be returned
 * indicating that the signal occured.  If an error is returned, clear the
 * errorInfo variable.  This makes sure it exists and that it is empty,
 * otherwise bogus or non-existant information will be returned if this
 * routine was called somewhere besides Tcl_Eval.  If a signal was received
 * multiple times and a trap is set on it, then that trap will be executed for
 * each time the signal was received.
 * 
 * Parameters:
 *   o clientData (I) - Not used.
 *   o interp (I/O) - interp->result should contain the result for
 *     the command that just executed.  This will either be restored or
 *     replaced with a new result.  If this is NULL, the no interpreter
 *     is directly available (i.e. Tk event loop).  In this case, the first
 *     interpreter in internal interpreter table is used.  If an error occurs,
 *     it is handled via the error handler registerd in the global variable
 *     "tclSignalBackgroundError"
 *   o cmdResultCode (I) - The integer result returned by the command that
 *     Tcl_Eval just completed.  Should be TCL_OK if not called from
 *     Tcl_Eval.
 * Returns:
 *   Either the original result code, an error result if one of the
 *   trap commands returned an error, or an error indicating the
 *   a signal occured.
 *-----------------------------------------------------------------------------
 */
int
Tcl_ProcessSignals (clientData, interp, cmdResultCode)
    ClientData  clientData;
    Tcl_Interp *interp;
    int         cmdResultCode;
{
    Tcl_Interp *sigInterp;
    errState_t *errStatePtr;
    int         signalNum, result, idx;

    /*
     * Get the interpreter is it wasn't supplied, if none is available,
     * bail out.
     */
    if (interp == NULL) {
        if (numInterps == 0)
            return cmdResultCode;
        sigInterp = interpTable [0].interp;
    } else {
        sigInterp = interp;
    }

    errStatePtr = SaveErrorState (sigInterp);

    /*
     * Process all signals.  Don't process any more if one returns an error.
     */
    result = TCL_OK;

    for (signalNum = 1; signalNum < MAXSIG; signalNum++) {
        if (signalsReceived [signalNum] == 0)
            continue;
        result = ProcessASignal (sigInterp, signalNum);
        if (result == TCL_ERROR)
            break;
    }

    /*
     * Restore result and error state if we didn't get an error in signal
     * handling.
     */
    if (result != TCL_ERROR) {
        RestoreErrorState (sigInterp, errStatePtr);
    } else {
        ckfree (errStatePtr);
        cmdResultCode = TCL_ERROR;
    }

    /*
     * Reset the signal received flag in case more signals are pending.
     */
    for (signalNum = 1; signalNum < MAXSIG; signalNum++) {
        if (signalsReceived [signalNum] != 0)
            break;
    }
    if (signalNum < MAXSIG) {
        for (idx = 0; idx < numInterps; idx++)
            Tcl_AsyncMark (interpTable [idx].handler);
    }

    /*
     * If we got an error and an interpreter was not supplied, call the
     * background error handler (if available).  Otherwise, lose the error.
     */
    if ((result == TCL_ERROR) && (interp == NULL) &&
        (tclSignalBackgroundError != NULL))
        (*tclSignalBackgroundError) (sigInterp);

    return cmdResultCode;
}

/*
 *-----------------------------------------------------------------------------
 *
 * ParseSignalList --
 *  
 *   Parse a list of signal names or numbers.
 * 
 * Parameters:
 *   o interp (O) - Interpreter for returning errors.
 *   o signalListStr (I) - The Tcl list of signals to convert.
 *   o signalList (O) - The list of converted signal numbers, must be
 *     big enough to hold MAXSIG signals.
 *     Tcl_Eval just completed.
 * Returns:
 *   The number of signals converted, or -1 if an error occures.
 *-----------------------------------------------------------------------------
 */
static int
ParseSignalList (interp, signalListStr, signalList)
    Tcl_Interp *interp;
    char       *signalListStr;
    int         signalList [];
{
    char         **signalListArgv;
    int            signalListSize, signalNum, idx;
    char          *signalName;
    int		i18n_result = 0;

    if (Tcl_SplitList (interp, signalListStr, &signalListSize, 
                       &signalListArgv) != TCL_OK)
        return -1;

    if (signalListSize > MAXSIG) {
        DCP_APPEND_RESULT_CODE(dcp_s_too_many_signals);
        goto errorExit;
    }

    if (signalListSize == 0) {
        DCP_APPEND_RESULT_CODE(dcp_s_signal_list_not_empty);
        goto errorExit;
    }

    for (idx = 0; idx < signalListSize; idx++) {
        signalName = signalListArgv [idx];

        if (Tcl_StrToInt (signalName, 0, &signalNum))
            signalName = Tcl_SignalId (signalNum, &i18n_result);
        else
            signalNum = SigNameToNum (signalName);

        if (signalName == NULL) {
            char numBuf [20];

            sprintf (numBuf, "%d", signalNum);
            DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_signal_invalid_num, numBuf);
            goto errorExit;
        }

        if ((signalNum < 1) || (signalNum > MAXSIG)) {
            DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_signal_invalid_name, 
                                           signalName);
	    if (i18n_result)
		free(signalName);
            goto errorExit;
        }
        signalList [idx] = signalNum;
    }

    ckfree ((char *) signalListArgv);
    return signalListSize;

  errorExit:
    ckfree ((char *) signalListArgv);
    return -1;

}

/*
 *-----------------------------------------------------------------------------
 *
 * SignalBlocked --
 *     
 *    Determine if a signal is blocked.  On non-Posix systems, always returns
 * "0".
 *
 * Parameters::
 *   o interp (O) - Error messages are returned in result.
 *   o signalNum (I) - The signal to determine the state for.
 * Returns:
 *   NULL if an error occured, or a pointer to a static string of "1" if the
 * signal is block, and a static string of "0" if it is not blocked.
 *-----------------------------------------------------------------------------
 */
static char *
SignalBlocked (interp, signalNum)
    Tcl_Interp  *interp;
    int          signalNum;
{
#ifdef HAVE_SIGACTION
    int      idx;
    sigset_t sigBlockSet;

    if (sigprocmask (SIG_BLOCK, NULL, &sigBlockSet)) {
        interp->result = Tcl_PosixError (interp);
        return NULL;
    }
    return sigismember (&sigBlockSet, signalNum) ? "1" : "0";
#else
    return "0";
#endif
}

/*
 *-----------------------------------------------------------------------------
 *
 * GetSignalStates --
 *     
 *    Return a keyed list containing the signal states for the specified
 * signals.
 *
 * Parameters::
 *   o interp (O) - The list is returned in the result.
 *   o signalListSize (I) - Number of signals in the signal list.
 *   o signalList (I) - List of signals of requested signals.
 * Returns:
 *   TCL_OK or TCL_ERROR, with error message in interp.
 *-----------------------------------------------------------------------------
 */
static int
GetSignalStates (interp, signalListSize, signalList)
    Tcl_Interp *interp;
    int         signalListSize;
    int         signalList [MAXSIG];
{
    int              idx, signalNum, actuallyDone = -1;
    char            *stateKeyedList [MAXSIG];
    char            *sigState [3], *sigEntry [2];
    signalProcPtr_t  actionFunc;
    int		    i18n_result = 0;

    for (idx = 0; idx < signalListSize; idx ++) {
        signalNum = signalList [idx];

        if (GetSignalState (signalNum, &actionFunc) == TCL_ERROR)
            goto unixSigError;
        
        sigState [2] = NULL;
        if (actionFunc == SIG_DFL)
            sigState [0]  = "default";
        else if (actionFunc == SIG_IGN)
            sigState [0] = "ignore";
        else if (actionFunc == TclSignalTrap) {
            if (signalTrapCmds [signalNum] == NULL)
                sigState [0] = "error";
            else {
                sigState [0] = "trap";
                sigState [2] = signalTrapCmds [signalNum];
            }
        } else {
            sigState [0] = "unknown";
        }

        sigState [1] = SignalBlocked (interp, signalNum);
        if (sigState [1] == NULL)
            goto unixSigError;

        sigEntry [0] = Tcl_SignalId (signalNum, &i18n_result);
        sigEntry [1] = Tcl_Merge ((sigState [2] == NULL) ? 2 : 3,
                                  sigState);

        stateKeyedList [idx] = Tcl_Merge (2, sigEntry);
        ckfree (sigEntry [1]);
	if (i18n_result)
		free(sigEntry [0]);

        actuallyDone = idx;

    }
    Tcl_SetResult (interp, Tcl_Merge (signalListSize, stateKeyedList),
                   TCL_DYNAMIC);

    for (idx = 0; idx <= actuallyDone; idx++)
        ckfree (stateKeyedList [idx]);

    return TCL_OK;

  unixSigError:
    for (idx = 0; idx <= actuallyDone; idx++)
        ckfree (stateKeyedList [idx]);

    interp->result = Tcl_PosixError (interp);
    return TCL_ERROR;
}

/*
 *-----------------------------------------------------------------------------
 *
 * SetSignalStates --
 *     
 *    Set the signal state for the specified signals.  
 *
 * Parameters::
 *   o interp (O) - The list is returned in the result.
 *   o signalListSize (I) - Number of signals in the signal list.
 *   o signalList (I) - List of signals of requested signals.
 *   o actionFunc (I) - The function to run when the signal is received.
 *   o command (I) - If the function is the "trap" function, this is the
 *     Tcl command to run when the trap occurs.  Otherwise, NULL.
 * Returns:
 *   TCL_OK or TCL_ERROR, with error message in interp.
 *-----------------------------------------------------------------------------
 */
static int
SetSignalStates (interp, signalListSize, signalList, actionFunc, command)
    Tcl_Interp      *interp;
    int              signalListSize;
    int              signalList [MAXSIG];
    signalProcPtr_t  actionFunc;
    char            *command;

{
    int idx, signalNum;

    for (idx = 0; idx < signalListSize; idx ++) {
        signalNum = signalList [idx];

        if (signalTrapCmds [signalNum] != NULL) {
            ckfree (signalTrapCmds [signalNum]);
            signalTrapCmds [signalNum] = NULL;
        }
        if (SetSignalAction (signalNum, actionFunc) == TCL_ERROR)
            goto unixSigError;

        if (command != NULL)
            signalTrapCmds [signalNum] = ckstrdup (command);
    }

    return TCL_OK;

  unixSigError:
    interp->result = Tcl_PosixError (interp);
    return TCL_ERROR;
}

/*
 *-----------------------------------------------------------------------------
 *
 * BlockSignals --
 *     
 *    Block or unblock the specified signals.  Returns an error if not a Posix
 * system.
 *
 * Parameters::
 *   o interp (O) - Error messages are returned in result.
 *   o action (I) - SIG_BLOCK or SIG_UNBLOCK.
 *   o signalListSize (I) - Number of signals in the signal list.
 *   o signalList (I) - List of signals of requested signals.
 * Returns:
 *   TCL_OK or TCL_ERROR, with error message in interp.
 *-----------------------------------------------------------------------------
 */
static int
BlockSignals (interp, action, signalListSize, signalList)
    Tcl_Interp  *interp;
    int          action;
    int          signalListSize;
    int          signalList [MAXSIG];
{
#ifdef HAVE_SIGACTION
    int      idx;
    sigset_t sigBlockSet;

    sigemptyset (&sigBlockSet);

    for (idx = 0; idx < signalListSize; idx ++)
        sigaddset (&sigBlockSet, signalList [idx]);

    if (sigprocmask (action, &sigBlockSet, NULL)) {
        interp->result = Tcl_PosixError (interp);
        return TCL_ERROR;
    }

    return TCL_OK;
#else
    interp->result = (char *)dce_sprintf(dcp_s_signal_posix_unavailable);
    DCP_GENERAL_SET_E_VAR(dcp_s_signal_posix_unavailable);
    return TCL_ERROR;
#endif
}

/*
 *-----------------------------------------------------------------------------
 *
 * Tcl_SignalCmd --
 *     Implements the TCL signal command:
 *         signal action siglist ?command?
 *
 * Results:
 *      Standard TCL results, may return the UNIX system error message.
 *
 * Side effects:
 *	Signal handling states may be changed.
 *-----------------------------------------------------------------------------
 */
int
Tcl_SignalCmd (clientData, interp, argc, argv)
    char       *clientData;
    Tcl_Interp *interp;
    int         argc;
    char      **argv;
{
    int                  signalListSize;
    int                  signalList [MAXSIG], actionClass;
    signalProcPtr_t      actionFunc;
    char                *command = NULL;

    if ((argc < 3) || (argc > 4)) {
        DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_t_signal_usage, argv[0]);
        return TCL_ERROR;
    }

    signalListSize = ParseSignalList (interp, argv [2], signalList);
    if (signalListSize < 0)    
        return TCL_ERROR;

    /*
     * Determine the action to take on all of the signals.
     */
    if (STREQU (argv [1], "trap")) {
        actionFunc = TclSignalTrap;
        actionClass = SIGACT_SET;
        if (argc != 4) {
            DCP_APPEND_RESULT_CODE(dcp_s_signal_trap_req_cmd);
            return TCL_ERROR;
        }
        command = argv [3];
    } else {
        if (STREQU (argv [1], "default")) {
            actionFunc  = SIG_DFL;
            actionClass = SIGACT_SET;
        } else if (STREQU (argv [1], "ignore")) {
            actionFunc = SIG_IGN;
            actionClass = SIGACT_SET;
        } else if (STREQU (argv [1], "error")) {
            actionFunc = TclSignalTrap;
            actionClass = SIGACT_SET;
        } else if (STREQU (argv [1], "get")) {
            actionClass = SIGACT_GET;
        } else if (STREQU (argv [1], "block")) {
            actionClass = SIGACT_BLOCK;
        } else if (STREQU (argv [1], "unblock")) {
            actionClass = SIGACT_UNBLOCK;
        } else {
            DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_signal_invalid_action, 
                                           argv[1]);
            return TCL_ERROR;
        }
        if (argc != 3) {
            DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_signal_takes_no_cmd, argv[1]);
            return TCL_ERROR;
        }
    }

    /*
     * Process the specified action class.
     */
    switch (actionClass) {
      case SIGACT_SET:
        return SetSignalStates (interp, signalListSize, signalList,
                                actionFunc, command);
      case SIGACT_GET:
        return GetSignalStates (interp, signalListSize, signalList);
      case SIGACT_BLOCK:
        return BlockSignals (interp, SIG_BLOCK, signalListSize, signalList);
      case SIGACT_UNBLOCK:
        return BlockSignals (interp, SIG_UNBLOCK, signalListSize, signalList);
    }

}

/*
 *-----------------------------------------------------------------------------
 *
 *  SignalCmdCleanUp --
 *
 *   Clean up the signal data structure when an interpreter is deleted. If
 * this is the last interpreter, clean up all tables.
 *
 * Parameters:
 *   o clientData (I) - Not used.
 *   o interp (I) - Interp that is being deleted.
 *-----------------------------------------------------------------------------
 */
static void
SignalCmdCleanUp (clientData, interp)
    ClientData  clientData;
    Tcl_Interp *interp;
{
    int  idx;

    for (idx = 0; idx < numInterps; idx++) {
        if (interpTable [idx].interp == interp)
            break;
    }
    if (idx == numInterps)
        panic ((char *)dce_sprintf(dcp_s_signal_lost_interp));
    interpTable [idx] = interpTable [--numInterps];

    /*
     * If there are no more interpreters, clean everything up.
     */
    if (numInterps == 0) {
        ckfree (interpTable);
        interpTable = NULL;
        interpTableSize = 0;

        for (idx = 0; idx < MAXSIG; idx++) {
            if (signalTrapCmds [idx] != NULL) {
                ckfree (signalTrapCmds [idx]);
                signalTrapCmds [idx] = NULL;
            }
        }
    }
}

/*
 *-----------------------------------------------------------------------------
 *
 * Tcl_SetupSigInt --
 *    Set up SIGINT to the "error" state if the current state is default.
 * This is done because shells set SIGINT to ignore for background processes
 * so that they don't die on signals generated by the user at the keyboard.
 * Tcl only enables SIGINT catching if it is an interactive session.
 *-----------------------------------------------------------------------------
 */
void
Tcl_SetupSigInt ()
{
    signalProcPtr_t  actionFunc;

    if ((GetSignalState (SIGINT, &actionFunc) == TCL_OK) &&
        (actionFunc == SIG_DFL))
        SetSignalAction (SIGINT, TclSignalTrap);
}

/*
 *-----------------------------------------------------------------------------
 *
 * Tcl_InitSignalHandling --
 *      Initializes singal handling for a interpreter.
 *-----------------------------------------------------------------------------
 */
void
Tcl_InitSignalHandling (interp)
    Tcl_Interp *interp;
{
    int              idx;
    interpHandler_t *newTable;

    /*
     * If this is the first interpreter, set everything up.
     */
    if (numInterps == 0) {
        interpTableSize = 4;
        interpTable = (interpHandler_t *)
            ckalloc (sizeof (interpHandler_t) * interpTableSize);

        for (idx = 0; idx < MAXSIG; idx++) {
            signalsReceived [idx] = 0;
            signalTrapCmds [idx] = NULL;
        }
    }

    /*
     * If there is not room in this table for another interp, expand it.
     */
    if (numInterps == interpTableSize) {
        newTable = (interpHandler_t *)
            ckalloc (sizeof (interpHandler_t) * interpTableSize * 2);
        memcpy (newTable, interpTable,
                sizeof (interpHandler_t) * interpTableSize);
        ckfree (interpTable);
        interpTable = newTable;
        interpTableSize *= 2;
    }

    /*
     * Add this interpreter to the list and set up a async handler.
     * Arange for clean up on the interpreter being deleted.
     */
    interpTable [numInterps].interp = interp;
    interpTable [numInterps].handler =
        Tcl_AsyncCreate (Tcl_ProcessSignals, (ClientData) NULL);
    numInterps++;

    Tcl_CallWhenDeleted (interp, SignalCmdCleanUp, (ClientData) NULL);

    Tcl_CreateCommand (interp, "signal", (Tcl_CmdProc *)Tcl_SignalCmd,
                       (ClientData )NULL, (Tcl_CmdDeleteProc *)NULL);
}


/* 
 * Utilities pulled in from TclXutil.c that are needed here
 */


/*
 *-----------------------------------------------------------------------------
 *
 * Tcl_StrToInt --
 *      Convert an Ascii string to an number of the specified base.
 *
 * Parameters:
 *   o string (I) - String containing a number.
 *   o base (I) - The base to use for the number 8, 10 or 16 or zero to decide
 *     based on the leading characters of the number.  Zero to let the number
 *     determine the base.
 *   o intPtr (O) - Place to return the converted number.  Will be 
 *     unchanged if there is an error.
 *
 * Returns:
 *      Returns 1 if the string was a valid number, 0 invalid.
 *-----------------------------------------------------------------------------
 */
int
Tcl_StrToInt (string, base, intPtr)
    CONST char *string;
    int         base;
    int        *intPtr;
{
    char *end;
    int   num;

    num = strtol(string, &end, base);
    while ((*end != '\0') && ISSPACE(*end)) {
        end++;
    }
    if ((end == string) || (*end != 0))
        return FALSE;
    *intPtr = num;
    return TRUE;

}


/*
 *-----------------------------------------------------------------------------
 *
 * Tcl_UpShift --
 *     Utility procedure to up-shift a string.
 *
 * Parameters:
 *   o targetStr (I) - String to store the up-shifted string in.  Must
 *     have enough space allocated to store the string.  If NULL is specified,
 *     then the string will be dynamicly allocated and returned as the
 *     result of the function. May also be the same as the source string to
 *     shift in place.
 *   o sourceStr (I) - The string to up-shift.
 *
 * Returns:
 *   A pointer to the up-shifted string
 *-----------------------------------------------------------------------------
 */
char *
Tcl_UpShift (targetStr, sourceStr)
    char       *targetStr;
    CONST char *sourceStr;
{
    register char theChar;

    if (targetStr == NULL)
        targetStr = ckalloc (strlen ((char *) sourceStr) + 1);

    for (; (theChar = *sourceStr) != '\0'; sourceStr++) {
        if (ISLOWER (theChar))
            theChar = _toupper (theChar);
        *targetStr++ = theChar;
    }
    *targetStr = '\0';
    return targetStr;
}

#endif  /* _DCECP_TEST */
