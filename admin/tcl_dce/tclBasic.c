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
 * $Log: tclBasic.c,v $
 * Revision 1.1.10.2  1996/10/03  17:55:33  arvind
 * 	Conditionalize XPG fix for field width specifier so that it is not applied to AIX to avoid problems caused
 * 	by AIX's lack of support for it.
 * 	[1996/08/13  16:44 UTC  psn  /main/DCE_1.2.2/psn_bugfix/3]
 *
 * 	Cleanup.
 * 	[1996/08/06  18:10 UTC  psn  /main/DCE_1.2.2/psn_bugfix/2]
 *
 * 	Fix field width specifier bug to use correctly generated format string by sams.
 * 	[1996/08/05  15:23 UTC  psn  /main/DCE_1.2.2/psn_bugfix/1]
 *
 * Revision 1.1.10.1  1996/06/04  21:43:29  arvind
 * 	Make sure i18n_msg_buf copied from i18n_msg_format is null terminated
 * 	[1995/12/18  19:40 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/2]
 * 
 * 	Don't mix sprintf named and implicit parameter selection on AIX
 * 	[1995/12/12  22:43 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Merge fix for _w * command coredump (CHFts15573)
 * 	Check for NULL command pointer before executing it.
 * 	[1995/07/20  19:08 UTC  dnguyen  /main/HPDCE02/3]
 * 
 * 	Merge cleaning up of I18N-commented code
 * 	[1995/04/28  20:22 UTC  dnguyen  /main/HPDCE02/2]
 * 
 * 	Clean up I18N comments
 * 	[1995/04/11  16:56 UTC  dnguyen  /main/dnguyen_mothra/2]
 * 
 * 	Merge up I18N'ed TCL fix.
 * 	[1995/04/10  14:48 UTC  dnguyen  /main/HPDCE02/1]
 * 
 * 	Initialize wtemp to NULL in _Tcl_WEval
 * 	[1995/04/07  15:12 UTC  dnguyen  /main/dnguyen_mothra/1]
 * 
 * Revision 1.1.8.4  1996/02/18  19:18:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:00  marty]
 * 
 * Revision 1.1.8.3  1995/12/13  21:19:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	Revision /main/HPDCE02/jrr_1.2_mothra/1  1995/12/12  22:43 UTC  dat
 * 	Don't mix sprintf named and implicit parameter selection on AIX
 * 
 * 	Revision /main/HPDCE02/3  1995/07/20  19:08 UTC  dnguyen
 * 	Merge fix for _w * command coredump (CHFts15573)
 * 	Check for NULL command pointer before executing it.
 * 
 * 	Revision /main/HPDCE02/2  1995/04/28  20:22 UTC  dnguyen
 * 	Merge cleaning up of I18N-commented code
 * 
 * 	Revision /main/dnguyen_mothra/2  1995/04/11  16:56 UTC  dnguyen
 * 	Clean up I18N comments
 * 
 * 	Revision /main/HPDCE02/1  1995/04/10  14:48 UTC  dnguyen
 * 	Merge up I18N'ed TCL fix.
 * 
 * 	Revision /main/dnguyen_mothra/1  1995/04/07  15:12 UTC  dnguyen
 * 	Initialize wtemp to NULL in _Tcl_WEval
 * 	[1995/12/13  21:18:14  root]
 * 
 * Revision 1.1.4.1  1994/10/21  21:38:54  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:30:31  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:18:54  mori_m
 * 	R 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:03:00  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclBasic.c --
 *
 *	Contains the basic facilities for TCL command interpretation,
 *	including interpreter creation and deletion, command creation
 *	and deletion, and command parsing and execution.
 *
 * Copyright (c) 1987-1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "tclInt.h"
#include "tclWInt.h"
#ifndef TCL_GENERIC_ONLY
#   include "tclUnix.h"
#endif

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * The following structure defines all of the commands in the Tcl core,
 * and the C procedures that execute them.
 */

typedef struct {
    char *name;			/* Name of command. */
    Tcl_CmdProc *proc;		/* Procedure that executes command. */
    _Tcl_WCmdProc *wproc;	/* Procedure that executes wide char command. */
} CmdInfo;

/*
 * Built-in commands, and the procedures associated with them:
 */

static CmdInfo builtInCmds[] = {
    /*
     * Commands in the generic core:
     */

    {"append",		Tcl_AppendCmd,	(_Tcl_WCmdProc *)NULL},
    {"array",		Tcl_ArrayCmd,	(_Tcl_WCmdProc *)NULL},
    {"break",		Tcl_BreakCmd,	(_Tcl_WCmdProc *)NULL},
    {"case",		Tcl_CaseCmd,	(_Tcl_WCmdProc *)NULL},
    {"catch",		Tcl_CatchCmd,	(_Tcl_WCmdProc *)NULL},
    {"concat",		Tcl_ConcatCmd,	(_Tcl_WCmdProc *)NULL},
    {"continue",	Tcl_ContinueCmd, (_Tcl_WCmdProc *)NULL},
    {"error",		Tcl_ErrorCmd,	(_Tcl_WCmdProc *)NULL},
    {"eval",		Tcl_EvalCmd,	(_Tcl_WCmdProc *)NULL},
    {"expr",		Tcl_ExprCmd,	(_Tcl_WCmdProc *)NULL},
    {"for",		Tcl_ForCmd,	(_Tcl_WCmdProc *)NULL},
    {"foreach",		Tcl_ForeachCmd,	(_Tcl_WCmdProc *)NULL},
    {"format",		Tcl_FormatCmd,	(_Tcl_WCmdProc *)NULL},
    {"global",		Tcl_GlobalCmd,	(_Tcl_WCmdProc *)NULL},
    {"history",		Tcl_HistoryCmd,	(_Tcl_WCmdProc *)NULL},
    {"if",		Tcl_IfCmd,	(_Tcl_WCmdProc *)NULL},
    {"incr",		Tcl_IncrCmd,	(_Tcl_WCmdProc *)NULL},
    {"info",		Tcl_InfoCmd,	(_Tcl_WCmdProc *)NULL},
    {"join",		Tcl_JoinCmd,	(_Tcl_WCmdProc *)NULL},
    {"lappend",		Tcl_LappendCmd,	(_Tcl_WCmdProc *)NULL},
    {"lindex",		Tcl_LindexCmd,	(_Tcl_WCmdProc *)NULL},
    {"linsert",		Tcl_LinsertCmd,	(_Tcl_WCmdProc *)NULL},
    {"list",		Tcl_ListCmd,	(_Tcl_WCmdProc *)NULL},
    {"llength",		Tcl_LlengthCmd,	(_Tcl_WCmdProc *)NULL},
    {"lrange",		Tcl_LrangeCmd,	(_Tcl_WCmdProc *)NULL},
    {"lreplace",	Tcl_LreplaceCmd, (_Tcl_WCmdProc *)NULL},
    {"lsearch",		Tcl_LsearchCmd,	(_Tcl_WCmdProc *)NULL},
    {"lsort",		Tcl_LsortCmd,	(_Tcl_WCmdProc *)NULL},
    {"proc",		Tcl_ProcCmd,	(_Tcl_WCmdProc *)NULL},
    {"regexp",		Tcl_RegexpCmd,	(_Tcl_WCmdProc *)NULL},
    {"regsub",		Tcl_RegsubCmd,	(_Tcl_WCmdProc *)NULL},
    {"rename",		Tcl_RenameCmd,	(_Tcl_WCmdProc *)NULL},
    {"return",		Tcl_ReturnCmd,	(_Tcl_WCmdProc *)NULL},
    {"scan",		Tcl_ScanCmd,	(_Tcl_WCmdProc *)NULL},
    {"set",		Tcl_SetCmd,	(_Tcl_WCmdProc *)NULL},
    {"split",		Tcl_SplitCmd,	(_Tcl_WCmdProc *)NULL},
    {"string",		Tcl_StringCmd,	(_Tcl_WCmdProc *)NULL},
    {"switch",		Tcl_SwitchCmd,	(_Tcl_WCmdProc *)NULL},
    {"trace",		Tcl_TraceCmd,	(_Tcl_WCmdProc *)NULL},
    {"unset",		Tcl_UnsetCmd,	(_Tcl_WCmdProc *)NULL},
    {"uplevel",		Tcl_UplevelCmd,	(_Tcl_WCmdProc *)NULL},
    {"upvar",		Tcl_UpvarCmd,	(_Tcl_WCmdProc *)NULL},
    {"while",		Tcl_WhileCmd,	(_Tcl_WCmdProc *)NULL},

    /*
     * Commands in the UNIX core:
     */

#ifndef TCL_GENERIC_ONLY
    {"cd",		Tcl_CdCmd,	(_Tcl_WCmdProc *)NULL},
    {"close",		Tcl_CloseCmd,	(_Tcl_WCmdProc *)NULL},
    {"eof",		Tcl_EofCmd,	(_Tcl_WCmdProc *)NULL},
    {"exec",		Tcl_ExecCmd,	(_Tcl_WCmdProc *)NULL},
    {"exit",		Tcl_ExitCmd,	(_Tcl_WCmdProc *)NULL},
    {"file",		Tcl_FileCmd,	(_Tcl_WCmdProc *)NULL},
    {"flush",		Tcl_FlushCmd,	(_Tcl_WCmdProc *)NULL},
    {"gets",		Tcl_GetsCmd,	(_Tcl_WCmdProc *)NULL},
    {"glob",		Tcl_GlobCmd,	(_Tcl_WCmdProc *)NULL},
    {"open",		Tcl_OpenCmd,	(_Tcl_WCmdProc *)NULL},
    {"pid",		Tcl_PidCmd,	(_Tcl_WCmdProc *)NULL},
    {"puts",		Tcl_PutsCmd,	(_Tcl_WCmdProc *)NULL},
    {"pwd",		Tcl_PwdCmd,	(_Tcl_WCmdProc *)NULL},
    {"read",		Tcl_ReadCmd,	(_Tcl_WCmdProc *)NULL},
    {"seek",		Tcl_SeekCmd,	(_Tcl_WCmdProc *)NULL},
    {"source",		Tcl_SourceCmd,	(_Tcl_WCmdProc *)NULL},
    {"tell",		Tcl_TellCmd,	(_Tcl_WCmdProc *)NULL},
    {"time",		Tcl_TimeCmd,	(_Tcl_WCmdProc *)NULL},
#endif /* TCL_GENERIC_ONLY */
    /*
     * Commands for I18N operation.
     */
    {"_wappend",	(Tcl_CmdProc *)NULL,	_Tcl_WAppendCmd},
    {"_warray",		(Tcl_CmdProc *)NULL,	_Tcl_WArrayCmd},
    {"_wbreak",		(Tcl_CmdProc *)NULL,	_Tcl_WBreakCmd},
    {"_wcase",		(Tcl_CmdProc *)NULL,	_Tcl_WCaseCmd},
    {"_wcatch",		(Tcl_CmdProc *)NULL,	_Tcl_WCatchCmd},
    {"_wconcat",	(Tcl_CmdProc *)NULL,	_Tcl_WConcatCmd},
    {"_wcontinue",	(Tcl_CmdProc *)NULL,	_Tcl_WContinueCmd},
    {"_werror",		(Tcl_CmdProc *)NULL,	_Tcl_WErrorCmd},
    {"_weval",		(Tcl_CmdProc *)NULL,	_Tcl_WEvalCmd},
    {"_wexpr",		(Tcl_CmdProc *)NULL,	_Tcl_WExprCmd},
    {"_wfor",		(Tcl_CmdProc *)NULL,	_Tcl_WForCmd},
    {"_wforeach",	(Tcl_CmdProc *)NULL,	_Tcl_WForeachCmd},
    {"_wformat",	(Tcl_CmdProc *)NULL,	_Tcl_WFormatCmd},
    {"_wglobal",	(Tcl_CmdProc *)NULL,	_Tcl_WGlobalCmd},
    {"_whistory",	(Tcl_CmdProc *)NULL,	_Tcl_WHistoryCmd},
    {"_wif",		(Tcl_CmdProc *)NULL,	_Tcl_WIfCmd},
    {"_wincr",		(Tcl_CmdProc *)NULL,	_Tcl_WIncrCmd},
    {"_winfo",		(Tcl_CmdProc *)NULL,	_Tcl_WInfoCmd},
    {"_wjoin",		(Tcl_CmdProc *)NULL,	_Tcl_WJoinCmd},
    {"_wlappend",	(Tcl_CmdProc *)NULL,	_Tcl_WLappendCmd},
    {"_wlindex",	(Tcl_CmdProc *)NULL,	_Tcl_WLindexCmd},
    {"_wlinsert",	(Tcl_CmdProc *)NULL,	_Tcl_WLinsertCmd},
    {"_wlist",		(Tcl_CmdProc *)NULL,	_Tcl_WListCmd},
    {"_wllength",	(Tcl_CmdProc *)NULL,	_Tcl_WLlengthCmd},
    {"_wlrange",	(Tcl_CmdProc *)NULL,	_Tcl_WLrangeCmd},
    {"_wlreplace",	(Tcl_CmdProc *)NULL,	_Tcl_WLreplaceCmd},
    {"_wlsearch",	(Tcl_CmdProc *)NULL,	_Tcl_WLsearchCmd},
    {"_wlsort",		(Tcl_CmdProc *)NULL,	_Tcl_WLsortCmd},
    {"_wproc",		(Tcl_CmdProc *)NULL,	_Tcl_WProcCmd},
    {"_wregexp",	(Tcl_CmdProc *)NULL,	_Tcl_WRegexpCmd},
    {"_wregsub",	(Tcl_CmdProc *)NULL,	_Tcl_WRegsubCmd},
    {"_wrename",	(Tcl_CmdProc *)NULL,	_Tcl_WRenameCmd},
    {"_wreturn",	(Tcl_CmdProc *)NULL,	_Tcl_WReturnCmd},
    {"_wscan",		(Tcl_CmdProc *)NULL,	_Tcl_WScanCmd},
    {"_wset",		(Tcl_CmdProc *)NULL,	_Tcl_WSetCmd},
    {"_wsplit",		(Tcl_CmdProc *)NULL,	_Tcl_WSplitCmd},
    {"_wstring",	(Tcl_CmdProc *)NULL,	_Tcl_WStringCmd},
    {"_wswitch",	(Tcl_CmdProc *)NULL,	_Tcl_WSwitchCmd},
    {"_wtrace",		(Tcl_CmdProc *)NULL,	_Tcl_WTraceCmd},
    {"_wunset",		(Tcl_CmdProc *)NULL,	_Tcl_WUnsetCmd},
    {"_wuplevel",	(Tcl_CmdProc *)NULL,	_Tcl_WUplevelCmd},
    {"_wupvar",		(Tcl_CmdProc *)NULL,	_Tcl_WUpvarCmd},
    {"_wwhile",		(Tcl_CmdProc *)NULL,	_Tcl_WWhileCmd},
#ifndef TCL_GENERIC_ONLY
    {"_wcd",		(Tcl_CmdProc *)NULL,	_Tcl_WCdCmd},
    {"_wclose",		(Tcl_CmdProc *)NULL,	_Tcl_WCloseCmd},
    {"_weof",		(Tcl_CmdProc *)NULL,	_Tcl_WEofCmd},
    {"_wexec",		(Tcl_CmdProc *)NULL,	_Tcl_WExecCmd},
    {"_wexit",		(Tcl_CmdProc *)NULL,	_Tcl_WExitCmd},
    {"_wfile",		(Tcl_CmdProc *)NULL,	_Tcl_WFileCmd},
    {"_wflush",		(Tcl_CmdProc *)NULL,	_Tcl_WFlushCmd},
    {"_wgets",		(Tcl_CmdProc *)NULL,	_Tcl_WGetsCmd},
    {"_wglob",		(Tcl_CmdProc *)NULL,	_Tcl_WGlobCmd},
    {"_wopen",		(Tcl_CmdProc *)NULL,	_Tcl_WOpenCmd},
    {"_wpid",		(Tcl_CmdProc *)NULL,	_Tcl_WPidCmd},
    {"_wputs",		(Tcl_CmdProc *)NULL,	_Tcl_WPutsCmd},
    {"_wpwd",		(Tcl_CmdProc *)NULL,	_Tcl_WPwdCmd},
    {"_wread",		(Tcl_CmdProc *)NULL,	_Tcl_WReadCmd},
    {"_wseek",		(Tcl_CmdProc *)NULL,	_Tcl_WSeekCmd},
    {"_wsource",	(Tcl_CmdProc *)NULL,	_Tcl_WSourceCmd},
    {"_wtell",		(Tcl_CmdProc *)NULL,	_Tcl_WTellCmd},
    {"_wtime",		(Tcl_CmdProc *)NULL,	_Tcl_WTimeCmd},
#endif /* TCL_GENERIC_ONLY */
    {NULL,		(Tcl_CmdProc *)NULL,	(_Tcl_WCmdProc *)NULL}
};

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateInterp --
 *
 *	Create a new TCL command interpreter.
 *
 * Results:
 *	The return value is a token for the interpreter, which may be
 *	used in calls to procedures like Tcl_CreateCmd, Tcl_Eval, or
 *	Tcl_DeleteInterp.
 *
 * Side effects:
 *	The command interpreter is initialized with an empty variable
 *	table and the built-in commands.  SIGPIPE signals are set to
 *	be ignored (see comment below for details).
 *
 *----------------------------------------------------------------------
 */

Tcl_Interp *
Tcl_CreateInterp()
{
    register Interp *iPtr;
    register Command *cmdPtr;
    register CmdInfo *cmdInfoPtr;
    int i;
    static int firstInterp = 1;

    iPtr = (Interp *) ckalloc(sizeof(Interp));
    iPtr->result = iPtr->resultSpace;
    iPtr->freeProc = 0;
    iPtr->errorLine = 0;
    Tcl_InitHashTable(&iPtr->commandTable, TCL_STRING_KEYS);
    Tcl_InitHashTable(&iPtr->mathFuncTable, TCL_STRING_KEYS);
    Tcl_InitHashTable(&iPtr->globalTable, TCL_STRING_KEYS);
    iPtr->numLevels = 0;
    iPtr->maxNestingDepth = 1000;
    iPtr->framePtr = NULL;
    iPtr->varFramePtr = NULL;
    iPtr->activeTracePtr = NULL;
    iPtr->returnCode = TCL_OK;
    iPtr->errorInfo = NULL;
    iPtr->errorCode = NULL;
    iPtr->numEvents = 0;
    iPtr->events = NULL;
    iPtr->curEvent = 0;
    iPtr->curEventNum = 0;
    iPtr->revPtr = NULL;
    iPtr->historyFirst = NULL;
    iPtr->revDisables = 1;
    iPtr->evalFirst = iPtr->evalLast = NULL;
    iPtr->appendResult = NULL;
    iPtr->appendAvl = 0;
    iPtr->appendUsed = 0;
    for (i = 0; i < NUM_REGEXPS; i++) {
	iPtr->patterns[i] = NULL;
	iPtr->patLengths[i] = -1;
	iPtr->regexps[i] = NULL;
    }
    strcpy(iPtr->pdFormat, DEFAULT_PD_FORMAT);
    iPtr->pdPrec = DEFAULT_PD_PREC;
    iPtr->cmdCount = 0;
    iPtr->noEval = 0;
    iPtr->evalFlags = 0;
    iPtr->scriptFile = NULL;
    iPtr->flags = 0;
    iPtr->tracePtr = NULL;
    iPtr->deleteCallbackPtr = NULL;
    iPtr->resultSpace[0] = 0;
    iPtr->whistoryFirst = (wchar_t *)NULL;
    iPtr->wevalFirst = iPtr->wevalLast = NULL;
    iPtr->wcmd = (wchar_t *)NULL;
    iPtr->wtermPtr = (wchar_t *)NULL;

    /*
     * Create the built-in commands.  Do it here, rather than calling
     * Tcl_CreateCommand, because it's faster (there's no need to
     * check for a pre-existing command by the same name).
     */

    for (cmdInfoPtr = builtInCmds; cmdInfoPtr->name != NULL; cmdInfoPtr++) {
	int new;
	Tcl_HashEntry *hPtr;

	hPtr = Tcl_CreateHashEntry(&iPtr->commandTable,
		cmdInfoPtr->name, &new);
	if (new) {
	    cmdPtr = (Command *) ckalloc(sizeof(Command));
	    cmdPtr->proc = cmdInfoPtr->proc;
	    cmdPtr->wproc = cmdInfoPtr->wproc;
	    cmdPtr->clientData = (ClientData) NULL;
	    cmdPtr->deleteProc = NULL;
	    cmdPtr->deleteData = (ClientData) NULL;
	    Tcl_SetHashValue(hPtr, cmdPtr);
	}
    }

#ifndef TCL_GENERIC_ONLY
    TclSetupEnv((Tcl_Interp *) iPtr);

    /*
     * The code below causes SIGPIPE (broken pipe) errors to
     * be ignored.  This is needed so that Tcl processes don't
     * die if they create child processes (e.g. using "exec" or
     * "open") that terminate prematurely.  The signal handler
     * is only set up when the first interpreter is created; 
     * after this the application can override the handler with
     * a different one of its own, if it wants.
     */

    if (firstInterp) {
	(void) signal(SIGPIPE, SIG_IGN);
	firstInterp = 0;
    }
#endif

    Tcl_TraceVar2((Tcl_Interp *) iPtr, "tcl_precision", (char *) NULL,
	    TCL_GLOBAL_ONLY|TCL_TRACE_WRITES|TCL_TRACE_UNSETS,
	    TclPrecTraceProc, (ClientData) NULL);
    return (Tcl_Interp *) iPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_Init --
 *
 *	This procedure is typically invoked by Tcl_AppInit procedures
 *	to perform additional initialization for a Tcl interpreter,
 *	such as sourcing the "init.tcl" script.
 *
 * Results:
 *	Returns a standard Tcl completion code and sets interp->result
 *	if there is an error.
 *
 * Side effects:
 *	Depends on what's in the init.tcl script.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_Init(interp)
    Tcl_Interp *interp;		/* Interpreter to initialize. */
{
    static char initCmd[] =
	"if [file exists [info library]/init.tcl] {\n\
	    source [info library]/init.tcl\n\
	} else {\n\
	    set msg \"%s\"\n\
	    error $msg\n\
	}";
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    int tcl_eval_return;

    i18n_msg_format = dce_msg_get(tcl_s_basic_interp_init_error);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)initCmd)
				+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, initCmd, (char *)i18n_msg_format);
    free(i18n_msg_format);

    tcl_eval_return =  Tcl_Eval(interp, (char *)i18n_msg_buf);

    ckfree(i18n_msg_buf);
    return tcl_eval_return;
}

/*
 *--------------------------------------------------------------
 *
 * Tcl_CallWhenDeleted --
 *
 *	Arrange for a procedure to be called before a given
 *	interpreter is deleted.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	When Tcl_DeleteInterp is invoked to delete interp,
 *	proc will be invoked.  See the manual entry for
 *	details.
 *
 *--------------------------------------------------------------
 */

void
Tcl_CallWhenDeleted(interp, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter to watch. */
    Tcl_InterpDeleteProc *proc;	/* Procedure to call when interpreter
				 * is about to be deleted. */
    ClientData clientData;	/* One-word value to pass to proc. */
{
    DeleteCallback *dcPtr, *prevPtr;
    Interp *iPtr = (Interp *) interp;

    dcPtr = (DeleteCallback *) ckalloc(sizeof(DeleteCallback));
    dcPtr->proc = proc;
    dcPtr->clientData = clientData;
    dcPtr->nextPtr = NULL;
    if (iPtr->deleteCallbackPtr == NULL) {
	iPtr->deleteCallbackPtr = dcPtr;
    } else {
	prevPtr = iPtr->deleteCallbackPtr;
	while (prevPtr->nextPtr != NULL) {
	    prevPtr = prevPtr->nextPtr;
	}
	prevPtr->nextPtr = dcPtr;
    }
}

/*
 *--------------------------------------------------------------
 *
 * Tcl_DontCallWhenDeleted --
 *
 *	Cancel the arrangement for a procedure to be called when
 *	a given interpreter is deleted.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If proc and clientData were previously registered as a
 *	callback via Tcl_CallWhenDeleted, they are unregistered.
 *	If they weren't previously registered then nothing
 *	happens.
 *
 *--------------------------------------------------------------
 */

void
Tcl_DontCallWhenDeleted(interp, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter to watch. */
    Tcl_InterpDeleteProc *proc;	/* Procedure to call when interpreter
				 * is about to be deleted. */
    ClientData clientData;	/* One-word value to pass to proc. */
{
    DeleteCallback *prevPtr, *dcPtr;
    Interp *iPtr = (Interp *) interp;

    for (prevPtr = NULL, dcPtr = iPtr->deleteCallbackPtr;
	    dcPtr != NULL; prevPtr = dcPtr, dcPtr = dcPtr->nextPtr) {
	if ((dcPtr->proc != proc) || (dcPtr->clientData != clientData)) {
	    continue;
	}
	if (prevPtr == NULL) {
	    iPtr->deleteCallbackPtr = dcPtr->nextPtr;
	} else {
	    prevPtr->nextPtr = dcPtr->nextPtr;
	}
	ckfree((char *) dcPtr);
	break;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DeleteInterp --
 *
 *	Delete an interpreter and free up all of the resources associated
 *	with it.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The interpreter is destroyed.  The caller should never again
 *	use the interp token.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_DeleteInterp(interp)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    Tcl_HashSearch search;
    register Command *cmdPtr;
    DeleteCallback *dcPtr;
    int i;

    /*
     * If the interpreter is in use, delay the deletion until later.
     */

    iPtr->flags |= DELETED;
    if (iPtr->numLevels != 0) {
	return;
    }

    /*
     * Invoke deletion callbacks.
     */

    while (iPtr->deleteCallbackPtr != NULL) {
	dcPtr = iPtr->deleteCallbackPtr;
	iPtr->deleteCallbackPtr = dcPtr->nextPtr;
	(*dcPtr->proc)(dcPtr->clientData, interp);
	ckfree((char *) dcPtr);
    }

    /*
     * Free up any remaining resources associated with the
     * interpreter.
     */

    for (hPtr = Tcl_FirstHashEntry(&iPtr->commandTable, &search);
	    hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
	if (cmdPtr->deleteProc != NULL) { 
	    (*cmdPtr->deleteProc)(cmdPtr->deleteData);
	}
	ckfree((char *) cmdPtr);
    }
    Tcl_DeleteHashTable(&iPtr->commandTable);
    for (hPtr = Tcl_FirstHashEntry(&iPtr->mathFuncTable, &search);
	    hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	ckfree((char *) Tcl_GetHashValue(hPtr));
    }
    Tcl_DeleteHashTable(&iPtr->mathFuncTable);
    TclDeleteVars(iPtr, &iPtr->globalTable);

    /*
     * Free up the result *after* deleting variables, since variable
     * deletion could have transferred ownership of the result string
     * to Tcl.
     */

    Tcl_FreeResult(interp);
    if (iPtr->errorInfo != NULL) {
	ckfree(iPtr->errorInfo);
    }
    if (iPtr->errorCode != NULL) {
	ckfree(iPtr->errorCode);
    }
    if (iPtr->events != NULL) {
	int i;

	for (i = 0; i < iPtr->numEvents; i++) {
	    ckfree(iPtr->events[i].command);
	}
	ckfree((char *) iPtr->events);
    }
    while (iPtr->revPtr != NULL) {
	HistoryRev *nextPtr = iPtr->revPtr->nextPtr;

	if ((wchar_t *)iPtr->revPtr->wnewChars != (wchar_t *)NULL)
		ckfree((wchar_t *)iPtr->revPtr->wnewChars);
	ckfree((char *) iPtr->revPtr);
	iPtr->revPtr = nextPtr;
    }
    if (iPtr->appendResult != NULL) {
	ckfree(iPtr->appendResult);
    }
    for (i = 0; i < NUM_REGEXPS; i++) {
	if (iPtr->patterns[i] == NULL) {
	    break;
	}
	ckfree(iPtr->patterns[i]);
	ckfree((char *) iPtr->regexps[i]);
    }
    while (iPtr->tracePtr != NULL) {
	Trace *nextPtr = iPtr->tracePtr->nextPtr;

	ckfree((char *) iPtr->tracePtr);
	iPtr->tracePtr = nextPtr;
    }
    /* I18N cleanup */
    if (iPtr->wcmd != (wchar_t *)NULL)
	ckfree(iPtr->wcmd);
    if (iPtr->whistoryFirst != (wchar_t *)NULL)
	ckfree(iPtr->whistoryFirst);

    ckfree((char *) iPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateCommand --
 *
 *	Define a new command in a command table.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If a command named cmdName already exists for interp, it is
 *	deleted.  In the future, when cmdName is seen as the name of
 *	a command by Tcl_Eval, proc will be called.  When the command
 *	is deleted from the table, deleteProc will be called.  See the
 *	manual entry for details on the calling sequence.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_CreateCommand(interp, cmdName, proc, clientData, deleteProc)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    char *cmdName;		/* Name of command. */
    Tcl_CmdProc *proc;		/* Command procedure to associate with
				 * cmdName. */
    ClientData clientData;	/* Arbitrary one-word value to pass to proc. */
    Tcl_CmdDeleteProc *deleteProc;
				/* If not NULL, gives a procedure to call when
				 * this command is deleted. */
{
    Interp *iPtr = (Interp *) interp;
    register Command *cmdPtr;
    Tcl_HashEntry *hPtr;
    int new;

    hPtr = Tcl_CreateHashEntry(&iPtr->commandTable, cmdName, &new);
    if (!new) {
	/*
	 * Command already exists:  delete the old one.
	 */

	cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
	if (cmdPtr->deleteProc != NULL) {
	    (*cmdPtr->deleteProc)(cmdPtr->deleteData);
	}
    } else {
	cmdPtr = (Command *) ckalloc(sizeof(Command));
	Tcl_SetHashValue(hPtr, cmdPtr);
    }
    cmdPtr->proc = proc;
    cmdPtr->clientData = clientData;
    cmdPtr->deleteProc = deleteProc;
    cmdPtr->deleteData = clientData;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetCommandInfo --
 *
 *	Modifies various information about a Tcl command.
 *
 * Results:
 *	If cmdName exists in interp, then the information at *infoPtr
 *	is stored with the command in place of the current information
 *	and 1 is returned.  If the command doesn't exist then 0 is
 *	returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_SetCommandInfo(interp, cmdName, infoPtr)
    Tcl_Interp *interp;			/* Interpreter in which to look
					 * for command. */
    char *cmdName;			/* Name of desired command. */
    Tcl_CmdInfo *infoPtr;		/* Where to store information about
					 * command. */
{
    Tcl_HashEntry *hPtr;
    Command *cmdPtr;

    hPtr = Tcl_FindHashEntry(&((Interp *) interp)->commandTable, cmdName);
    if (hPtr == NULL) {
	return 0;
    }
    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
    cmdPtr->proc = infoPtr->proc;
    cmdPtr->clientData = infoPtr->clientData;
    cmdPtr->deleteProc = infoPtr->deleteProc;
    cmdPtr->deleteData = infoPtr->deleteData;
    return 1;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetCommandInfo --
 *
 *	Returns various information about a Tcl command.
 *
 * Results:
 *	If cmdName exists in interp, then *infoPtr is modified to
 *	hold information about cmdName and 1 is returned.  If the
 *	command doesn't exist then 0 is returned and *infoPtr isn't
 *	modified.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_GetCommandInfo(interp, cmdName, infoPtr)
    Tcl_Interp *interp;			/* Interpreter in which to look
					 * for command. */
    char *cmdName;			/* Name of desired command. */
    Tcl_CmdInfo *infoPtr;		/* Where to store information about
					 * command. */
{
    Tcl_HashEntry *hPtr;
    Command *cmdPtr;

    hPtr = Tcl_FindHashEntry(&((Interp *) interp)->commandTable, cmdName);
    if (hPtr == NULL) {
	return 0;
    }
    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
    infoPtr->proc = cmdPtr->proc;
    infoPtr->clientData = cmdPtr->clientData;
    infoPtr->deleteProc = cmdPtr->deleteProc;
    infoPtr->deleteData = cmdPtr->deleteData;
    return 1;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DeleteCommand --
 *
 *	Remove the given command from the given interpreter.
 *
 * Results:
 *	0 is returned if the command was deleted successfully.
 *	-1 is returned if there didn't exist a command by that
 *	name.
 *
 * Side effects:
 *	CmdName will no longer be recognized as a valid command for
 *	interp.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_DeleteCommand(interp, cmdName)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    char *cmdName;		/* Name of command to remove. */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    Command *cmdPtr;

    hPtr = Tcl_FindHashEntry(&iPtr->commandTable, cmdName);
    if (hPtr == NULL) {
	return -1;
    }
    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
    if (cmdPtr->deleteProc != NULL) {
	(*cmdPtr->deleteProc)(cmdPtr->deleteData);
    }
    ckfree((char *) cmdPtr);
    Tcl_DeleteHashEntry(hPtr);
    return 0;
}

/*
 *-----------------------------------------------------------------
 *
 * Tcl_Eval --
 *
 *	Parse and execute a command in the Tcl language.
 *
 * Results:
 *	The return value is one of the return codes defined in tcl.hd
 *	(such as TCL_OK), and interp->result contains a string value
 *	to supplement the return code.  The value of interp->result
 *	will persist only until the next call to Tcl_Eval:  copy it or
 *	lose it! *TermPtr is filled in with the character just after
 *	the last one that was part of the command (usually a NULL
 *	character or a closing bracket).
 *
 * Side effects:
 *	Almost certainly;  depends on the command.
 *
 *-----------------------------------------------------------------
 */

int
Tcl_Eval(interp, cmd)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    char *cmd;			/* Pointer to TCL command to interpret. */
{
    /*
     * The storage immediately below is used to generate a copy
     * of the command, after all argument substitutions.  Pv will
     * contain the argv values passed to the command procedure.
     */

#   define NUM_CHARS 200
    char copyStorage[NUM_CHARS];
    ParseValue pv;
    char *oldBuffer;

    /*
     * This procedure generates an (argv, argc) array for the command,
     * It starts out with stack-allocated space but uses dynamically-
     * allocated storage to increase it if needed.
     */

#   define NUM_ARGS 10
    char *(argStorage[NUM_ARGS]);
    char **argv = argStorage;
    int argc;
    int argSize = NUM_ARGS;

    register char *src;			/* Points to current character
					 * in cmd. */
    char termChar;			/* Return when this character is found
					 * (either ']' or '\0').  Zero means
					 * that newlines terminate commands. */
    int flags;				/* Interp->evalFlags value when the
					 * procedure was called. */
    int result;				/* Return value. */
    register Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    Command *cmdPtr;
    char *termPtr;			/* Contains character just after the
					 * last one in the command. */
    char *cmdStart;			/* Points to first non-blank char. in
					 * command (used in calling trace
					 * procedures). */
    char *ellipsis = "";		/* Used in setting errorInfo variable;
					 * set to "..." to indicate that not
					 * all of offending command is included
					 * in errorInfo.  "" means that the
					 * command is all there. */
    register Trace *tracePtr;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Initialize the result to an empty string and clear out any
     * error information.  This makes sure that we return an empty
     * result if there are no commands in the command string.
     */

    Tcl_FreeResult((Tcl_Interp *) iPtr);
    iPtr->result = iPtr->resultSpace;
    iPtr->resultSpace[0] = 0;
    result = TCL_OK;

    /*
     * Initialize the area in which command copies will be assembled.
     */

    pv.buffer = copyStorage;
    pv.end = copyStorage + NUM_CHARS - 1;
    pv.expandProc = TclExpandParseValue;
    pv.clientData = (ClientData) NULL;

    src = cmd;
    flags = iPtr->evalFlags;
    iPtr->evalFlags = 0;
    if (flags & TCL_BRACKET_TERM) {
	termChar = ']';
    } else {
	termChar = 0;
    }
    termPtr = src;
    cmdStart = src;

    /*
     * Check depth of nested calls to Tcl_Eval:  if this gets too large,
     * it's probably because of an infinite loop somewhere.
     */

    iPtr->numLevels++;
    if (iPtr->numLevels > iPtr->maxNestingDepth) {
	iPtr->numLevels--;
	iPtr->result =  (char *)dce_msg_get(tcl_s_basic_too_many_nested_calls);
	iPtr->freeProc = (Tcl_FreeProc *) free;

	iPtr->termPtr = termPtr;
	return TCL_ERROR;
    }

    /*
     * There can be many sub-commands (separated by semi-colons or
     * newlines) in one command string.  This outer loop iterates over
     * individual commands.
     */

    while (*src != termChar) {
	iPtr->flags &= ~(ERR_IN_PROGRESS | ERROR_CODE_SET);

	/*
	 * Skim off leading white space and semi-colons, and skip
	 * comments.
	 */

	while (1) {
	    register char c = *src;

	    if ((CHAR_TYPE(c) != TCL_SPACE) && (c != ';') && (c != '\n')) {
		break;
	    }
	    src += 1;
	}
	if (*src == '#') {
	    for (src++; *src != 0; src++) {
		if ((*src == '\n') && (src[-1] != '\\')) {
		    src++;
		    break;
		}
	    }
	    continue;
	}
	cmdStart = src;

	/*
	 * Parse the words of the command, generating the argc and
	 * argv for the command procedure.  May have to call
	 * TclParseWords several times, expanding the argv array
	 * between calls.
	 */

	pv.next = oldBuffer = pv.buffer;
	argc = 0;
	while (1) {
	    int newArgs, maxArgs;
	    char **newArgv;
	    int i;

	    /*
	     * Note:  the "- 2" below guarantees that we won't use the
	     * last two argv slots here.  One is for a NULL pointer to
	     * mark the end of the list, and the other is to leave room
	     * for inserting the command name "unknown" as the first
	     * argument (see below).
	     */

	    maxArgs = argSize - argc - 2;
	    result = TclParseWords((Tcl_Interp *) iPtr, src, flags,
		    maxArgs, &termPtr, &newArgs, &argv[argc], &pv);
	    src = termPtr;
	    if (result != TCL_OK) {
		ellipsis = "...";
		goto done;
	    }

	    /*
	     * Careful!  Buffer space may have gotten reallocated while
	     * parsing words.  If this happened, be sure to update all
	     * of the older argv pointers to refer to the new space.
	     */

	    if (oldBuffer != pv.buffer) {
		int i;

		for (i = 0; i < argc; i++) {
		    argv[i] = pv.buffer + (argv[i] - oldBuffer);
		}
		oldBuffer = pv.buffer;
	    }
	    argc += newArgs;
	    if (newArgs < maxArgs) {
		argv[argc] = (char *) NULL;
		break;
	    }

	    /*
	     * Args didn't all fit in the current array.  Make it bigger.
	     */

	    argSize *= 2;
	    newArgv = (char **)
		    ckalloc((unsigned) argSize * sizeof(char *));
	    for (i = 0; i < argc; i++) {
		newArgv[i] = argv[i];
	    }
	    if (argv != argStorage) {
		ckfree((char *) argv);
	    }
	    argv = newArgv;
	}

	/*
	 * If this is an empty command (or if we're just parsing
	 * commands without evaluating them), then just skip to the
	 * next command.
	 */

	if ((argc == 0) || iPtr->noEval) {
	    continue;
	}
	argv[argc] = NULL;

	/*
	 * Save information for the history module, if needed.
	 */

	if (flags & TCL_RECORD_BOUNDS) {
	    iPtr->evalFirst = cmdStart;
	    iPtr->evalLast = src-1;
	}

	/*
	 * Find the procedure to execute this command.  If there isn't
	 * one, then see if there is a command "unknown".  If so,
	 * invoke it instead, passing it the words of the original
	 * command as arguments.
	 */

	hPtr = Tcl_FindHashEntry(&iPtr->commandTable, argv[0]);
	if (hPtr == NULL) {
	    int i;

	    hPtr = Tcl_FindHashEntry(&iPtr->commandTable, "unknown");
	    if (hPtr == NULL) {
		Tcl_ResetResult(interp);
		i18n_msg_format = dce_msg_get(tcl_s_basic_invalid_cmd_name);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		result = TCL_ERROR;
		goto done;
	    }
	    for (i = argc; i >= 0; i--) {
		argv[i+1] = argv[i];
	    }
	    argv[0] = "unknown";
	    argc++;
	}
	cmdPtr = (Command *) Tcl_GetHashValue(hPtr);

	/*
	 * Call trace procedures, if any.
	 */

	for (tracePtr = iPtr->tracePtr; tracePtr != NULL;
		tracePtr = tracePtr->nextPtr) {
	    char saved;

	    if (tracePtr->level < iPtr->numLevels) {
		continue;
	    }
	    saved = *src;
	    *src = 0;
	    (*tracePtr->proc)(tracePtr->clientData, interp, iPtr->numLevels,
		    cmdStart, cmdPtr->proc, cmdPtr->clientData, argc, argv);
	    *src = saved;
	}

	/*
	 * At long last, invoke the command procedure.  Reset the
	 * result to its default empty value first (it could have
	 * gotten changed by earlier commands in the same command
	 * string).
	 */

	iPtr->cmdCount++;
	Tcl_FreeResult(iPtr);
	iPtr->result = iPtr->resultSpace;
	iPtr->resultSpace[0] = 0;
	if ((*cmdPtr->proc) != NULL)
	    result = (*cmdPtr->proc)(cmdPtr->clientData, interp, argc, argv);
	else {
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_basic_invalid_cmd_name);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0]) + strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    result = TCL_ERROR;
	    goto done;
	}
	if (tcl_AsyncReady) {
	    result = Tcl_AsyncInvoke(interp, result);
	}
	if (result != TCL_OK) {
	    break;
	}
    }

    /*
     * Free up any extra resources that were allocated.
     */

    done:
    if (pv.buffer != copyStorage) {
	ckfree((char *) pv.buffer);
    }
    if (argv != argStorage) {
	ckfree((char *) argv);
    }
    iPtr->numLevels--;
    if (iPtr->numLevels == 0) {
	if (result == TCL_RETURN) {
	    result = TCL_OK;
	}
	if ((result != TCL_OK) && (result != TCL_ERROR)) {
	    Tcl_ResetResult(interp);
	    if (result == TCL_BREAK) {
		iPtr->result = (char *)dce_msg_get(tcl_s_basic_break_outside_of_loop);
		iPtr->freeProc = (Tcl_FreeProc *) free;
	    } else if (result == TCL_CONTINUE) {
		iPtr->result = (char *)dce_msg_get(tcl_s_basic_continue_outside_of_loop);
		iPtr->freeProc = (Tcl_FreeProc *) free;
	    } else {
		iPtr->result = iPtr->resultSpace;
		i18n_msg_format = dce_msg_get(tcl_s_basic_cmd_return_bad_code);
		sprintf(iPtr->resultSpace, (char *)i18n_msg_format, result);
		free(i18n_msg_format);
	    }
	    result = TCL_ERROR;
	}
	if (iPtr->flags & DELETED) {
	    Tcl_DeleteInterp(interp);
	}
    }

    /*
     * If an error occurred, record information about what was being
     * executed when the error occurred.
     */

    if ((result == TCL_ERROR) && !(iPtr->flags & ERR_ALREADY_LOGGED)) {
	int numChars;
	register char *p;

	/*
	 * Compute the line number where the error occurred.
	 */

	iPtr->errorLine = 1;
	for (p = cmd; p != cmdStart; p++) {
	    if (*p == '\n') {
		iPtr->errorLine++;
	    }
	}
	for ( ; isspace(UCHAR(*p)) || (*p == ';'); p++) {
	    if (*p == '\n') {
		iPtr->errorLine++;
	    }
	}

	/*
	 * Figure out how much of the command to print in the error
	 * message (up to a certain number of characters, or up to
	 * the first new-line).
	 */

	numChars = src - cmdStart;
	if (numChars > (NUM_CHARS-50)) {
	    numChars = NUM_CHARS-50;
	    ellipsis = " ...";
	}

#ifndef AIX32
        /*
         * This fix is excluded for AIX due to missing support for
         * XPG compliant field width specifier in format string.
         */
        /*
         * Use properly generated format string and necessary argument list
         * to print message. So for,
         *                 "%.*s" ---sams------> "%1$.*2$s"
         * So sprintf should be passed 'cmdStart, numChars' arguments for
         *                 "%.*s"
         * where,
         *         numChars indicates the field width, and
         *         cmdStart indicates the string to be printed
         */
	if (!(iPtr->flags & ERR_IN_PROGRESS)) {
	    i18n_msg_format = dce_msg_get(tcl_s_basic_while_executing);
	    sprintf(copyStorage, (char *)i18n_msg_format, cmdStart, numChars,
                    ellipsis);
	    free(i18n_msg_format);
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_basic_invoked_from_within);
	    sprintf(copyStorage, (char *)i18n_msg_format, cmdStart, numChars,
                    ellipsis);
	    free(i18n_msg_format);
	}
#else /* defined AIX32 */
	/* 
	 * AIX cannot process sprintf using implicit next parameter ('*s')
	 * and numbered parameter ('%1$') in the same call, so need to
	 * do prior substitution.  Adding space for 3 digits (plus null)
	 * is enough because numChars can't exceed NUM_CHARS (200) 
	 */
	if (!(iPtr->flags & ERR_IN_PROGRESS)) {
	    unsigned char *formatPtr, *bufPtr;
	    i18n_msg_format = dce_msg_get(tcl_s_basic_while_executing);
	    i18n_msg_buf = (unsigned char *)
		            ckalloc(strlen((char *)i18n_msg_format) + 4);
	    for (formatPtr = i18n_msg_format, bufPtr = i18n_msg_buf;
		 *formatPtr != '\0'; formatPtr++ ) {
		if (*formatPtr == '*') {
		    bufPtr += sprintf ((char *)bufPtr, "%d", numChars);
		} else {
		    *bufPtr++ = *formatPtr;
		}
	    }
	    *bufPtr = '\0';

	    sprintf(copyStorage, (char *)i18n_msg_buf, cmdStart, ellipsis);
	    ckfree(i18n_msg_buf);
	    free(i18n_msg_format);
	} else {
	    unsigned char *formatPtr, *bufPtr;
	    i18n_msg_format = dce_msg_get(tcl_s_basic_invoked_from_within);
	    i18n_msg_buf = (unsigned char *)
		            ckalloc(strlen((char *)i18n_msg_format) + 4);
	    for (formatPtr = i18n_msg_format, bufPtr = i18n_msg_buf;
		 *formatPtr != '\0'; formatPtr++ ) {
		if (*formatPtr == '*') {
		    bufPtr += sprintf ((char *)bufPtr, "%d", numChars);
		} else {
		    *bufPtr++ = *formatPtr;
		}
	    }
	    *bufPtr = '\0';

	    sprintf(copyStorage, 
			(char *)i18n_msg_buf, cmdStart, ellipsis);
	    ckfree(i18n_msg_buf);
	    free(i18n_msg_format);
	}
#endif /* AIX32 */
	Tcl_AddErrorInfo(interp, copyStorage);
	iPtr->flags &= ~ERR_ALREADY_LOGGED;
    } else {
	iPtr->flags &= ~ERR_ALREADY_LOGGED;
    }
    iPtr->termPtr = termPtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateTrace --
 *
 *	Arrange for a procedure to be called to trace command execution.
 *
 * Results:
 *	The return value is a token for the trace, which may be passed
 *	to Tcl_DeleteTrace to eliminate the trace.
 *
 * Side effects:
 *	From now on, proc will be called just before a command procedure
 *	is called to execute a Tcl command.  Calls to proc will have the
 *	following form:
 *
 *	void
 *	proc(clientData, interp, level, command, cmdProc, cmdClientData,
 *		argc, argv)
 *	    ClientData clientData;
 *	    Tcl_Interp *interp;
 *	    int level;
 *	    char *command;
 *	    int (*cmdProc)();
 *	    ClientData cmdClientData;
 *	    int argc;
 *	    char **argv;
 *	{
 *	}
 *
 *	The clientData and interp arguments to proc will be the same
 *	as the corresponding arguments to this procedure.  Level gives
 *	the nesting level of command interpretation for this interpreter
 *	(0 corresponds to top level).  Command gives the ASCII text of
 *	the raw command, cmdProc and cmdClientData give the procedure that
 *	will be called to process the command and the ClientData value it
 *	will receive, and argc and argv give the arguments to the
 *	command, after any argument parsing and substitution.  Proc
 *	does not return a value.
 *
 *----------------------------------------------------------------------
 */

Tcl_Trace
Tcl_CreateTrace(interp, level, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which to create the trace. */
    int level;			/* Only call proc for commands at nesting level
				 * <= level (1 => top level). */
    Tcl_CmdTraceProc *proc;	/* Procedure to call before executing each
				 * command. */
    ClientData clientData;	/* Arbitrary one-word value to pass to proc. */
{
    register Trace *tracePtr;
    register Interp *iPtr = (Interp *) interp;

    tracePtr = (Trace *) ckalloc(sizeof(Trace));
    tracePtr->level = level;
    tracePtr->proc = proc;
    tracePtr->clientData = clientData;
    tracePtr->nextPtr = iPtr->tracePtr;
    iPtr->tracePtr = tracePtr;

    return (Tcl_Trace) tracePtr;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_DeleteTrace --
 *
 *	Remove a trace.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	From now on there will be no more calls to the procedure given
 *	in trace.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_DeleteTrace(interp, trace)
    Tcl_Interp *interp;		/* Interpreter that contains trace. */
    Tcl_Trace trace;		/* Token for trace (returned previously by
				 * Tcl_CreateTrace). */
{
    register Interp *iPtr = (Interp *) interp;
    register Trace *tracePtr = (Trace *) trace;
    register Trace *tracePtr2;

    if (iPtr->tracePtr == tracePtr) {
	iPtr->tracePtr = tracePtr->nextPtr;
	ckfree((char *) tracePtr);
    } else {
	for (tracePtr2 = iPtr->tracePtr; tracePtr2 != NULL;
		tracePtr2 = tracePtr2->nextPtr) {
	    if (tracePtr2->nextPtr == tracePtr) {
		tracePtr2->nextPtr = tracePtr->nextPtr;
		ckfree((char *) tracePtr);
		return;
	    }
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AddErrorInfo --
 *
 *	Add information to a message being accumulated that describes
 *	the current error.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The contents of message are added to the "errorInfo" variable.
 *	If Tcl_Eval has been called since the current value of errorInfo
 *	was set, errorInfo is cleared before adding the new message.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_AddErrorInfo(interp, message)
    Tcl_Interp *interp;		/* Interpreter to which error information
				 * pertains. */
    char *message;		/* Message to record. */
{
    register Interp *iPtr = (Interp *) interp;

    /*
     * If an error is already being logged, then the new errorInfo
     * is the concatenation of the old info and the new message.
     * If this is the first piece of info for the error, then the
     * new errorInfo is the concatenation of the message in
     * interp->result and the new message.
     */

    if (!(iPtr->flags & ERR_IN_PROGRESS)) {
	Tcl_SetVar2(interp, "errorInfo", (char *) NULL, interp->result,
		TCL_GLOBAL_ONLY);
	iPtr->flags |= ERR_IN_PROGRESS;

	/*
	 * If the errorCode variable wasn't set by the code that generated
	 * the error, set it to "NONE".
	 */

	if (!(iPtr->flags & ERROR_CODE_SET)) {
	    (void) Tcl_SetVar2(interp, "errorCode", (char *) NULL, "NONE",
		    TCL_GLOBAL_ONLY);
	}
    }
    Tcl_SetVar2(interp, "errorInfo", (char *) NULL, message,
	    TCL_GLOBAL_ONLY|TCL_APPEND_VALUE);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_VarEval --
 *
 *	Given a variable number of string arguments, concatenate them
 *	all together and execute the result as a Tcl command.
 *
 * Results:
 *	A standard Tcl return result.  An error message or other
 *	result may be left in interp->result.
 *
 * Side effects:
 *	Depends on what was done by the command.
 *
 *----------------------------------------------------------------------
 */
	/* VARARGS2 */ /* ARGSUSED */
int
#ifndef lint
Tcl_VarEval(va_alist)
#else
Tcl_VarEval(iPtr, p, va_alist)
    Tcl_Interp *iPtr;		/* Interpreter in which to execute command. */
    char *p;			/* One or more strings to concatenate,
				 * terminated with a NULL string. */
#endif
    va_dcl
{
    va_list argList;
#define FIXED_SIZE 200
    char fixedSpace[FIXED_SIZE+1];
    int spaceAvl, spaceUsed, length;
    char *string, *cmd;
    Tcl_Interp *interp;
    int result;

    /*
     * Copy the strings one after the other into a single larger
     * string.  Use stack-allocated space for small commands, but if
     * the command gets too large than call ckalloc to create the
     * space.
     */

    va_start(argList);
    interp = va_arg(argList, Tcl_Interp *);
    spaceAvl = FIXED_SIZE;
    spaceUsed = 0;
    cmd = fixedSpace;
    while (1) {
	string = va_arg(argList, char *);
	if (string == NULL) {
	    break;
	}
	length = strlen(string);
	if ((spaceUsed + length) > spaceAvl) {
	    char *new;

	    spaceAvl = spaceUsed + length;
	    spaceAvl += spaceAvl/2;
	    new = ckalloc((unsigned) spaceAvl);
	    memcpy((VOID *) new, (VOID *) cmd, spaceUsed);
	    if (cmd != fixedSpace) {
		ckfree(cmd);
	    }
	    cmd = new;
	}
	strcpy(cmd + spaceUsed, string);
	spaceUsed += length;
    }
    va_end(argList);
    cmd[spaceUsed] = '\0';

    result = Tcl_Eval(interp, cmd);
    if (cmd != fixedSpace) {
	ckfree(cmd);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GlobalEval --
 *
 *	Evaluate a command at global level in an interpreter.
 *
 * Results:
 *	A standard Tcl result is returned, and interp->result is
 *	modified accordingly.
 *
 * Side effects:
 *	The command string is executed in interp, and the execution
 *	is carried out in the variable context of global level (no
 *	procedures active), just as if an "uplevel #0" command were
 *	being executed.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_GlobalEval(interp, command)
    Tcl_Interp *interp;		/* Interpreter in which to evaluate command. */
    char *command;		/* Command to evaluate. */
{
    register Interp *iPtr = (Interp *) interp;
    int result;
    CallFrame *savedVarFramePtr;

    savedVarFramePtr = iPtr->varFramePtr;
    iPtr->varFramePtr = NULL;
    result = Tcl_Eval(interp, command);
    iPtr->varFramePtr = savedVarFramePtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetRecursionLimit --
 *
 *	Set the maximum number of recursive calls that may be active
 *	for an interpreter at once.
 *
 * Results:
 *	The return value is the old limit on nesting for interp.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_SetRecursionLimit(interp, depth)
    Tcl_Interp *interp;			/* Interpreter whose nesting limit
					 * is to be set. */
    int depth;				/* New value for maximimum depth. */
{
    Interp *iPtr = (Interp *) interp;
    int old;

    old = iPtr->maxNestingDepth;
    if (depth > 0) {
	iPtr->maxNestingDepth = depth;
    }
    return old;
}

/*
 *-----------------------------------------------------------------
 *
 * _Tcl_WEval --
 *
 *	Parse and execute a command in the Tcl language.
 *
 * Results:
 *	The return value is one of the return codes defined in tcl.hd
 *	(such as TCL_OK), and interp->result contains a string value
 *	to supplement the return code.  The value of interp->result
 *	will persist only until the next call to Tcl_Eval:  copy it or
 *	lose it! *TermPtr is filled in with the character just after
 *	the last one that was part of the command (usually a NULL
 *	character or a closing bracket).
 *
 * Side effects:
 *	Almost certainly;  depends on the command.
 *
 *-----------------------------------------------------------------
 */

int
_Tcl_WEval(interp, wcmd)
    Tcl_Interp *interp;		/* Token for command interpreter (returned
				 * by a previous call to Tcl_CreateInterp). */
    wchar_t *wcmd;		/* Pointer to TCL command to interpret. */
{
    /*
     * The storage immediately below is used to generate a copy
     * of the command, after all argument substitutions.  Pv will
     * contain the argv values passed to the command procedure.
     */

#   define NUM_CHARS 200
    wchar_t wcopyStorage[NUM_CHARS];
    _WParseValue wpv;
    wchar_t *woldBuffer;

    /*
     * This procedure generates an (argv, argc) array for the command,
     * It starts out with stack-allocated space but uses dynamically-
     * allocated storage to increase it if needed.
     */

#   define NUM_ARGS 10
    wchar_t *(wargStorage[NUM_ARGS]);
    wchar_t **wargv = wargStorage;
    int argc;
    int argSize = NUM_ARGS;

    wchar_t *wsrc;			/* Points to current character
					 * in cmd. */
    wchar_t wtermChar;			/* Return when this character is found
					 * (either ']' or '\0').  Zero means
					 * that newlines terminate commands. */
    int flags;				/* Interp->evalFlags value when the
					 * procedure was called. */
    int result;				/* Return value. */
    register Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    Command *cmdPtr;
    wchar_t *wtermPtr;			/* Contains character just after the
					 * last one in the command. */
    wchar_t *wcmdStart;			/* Points to first non-blank char. in
					 * command (used in calling trace
					 * procedures). */
    char *ellipsis = "";		/* Used in setting errorInfo variable;
					 * set to "..." to indicate that not
					 * all of offending command is included
					 * in errorInfo.  "" means that the
					 * command is all there. */
    register Trace *tracePtr;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    wchar_t	*wtemp = NULL;		/* wtemp and wlen are used to hold */
    int	char_buf_used = 0;		/* Flag for unknown and user procs */
    char *t_char_buf;			/* t_char_buf and clen are used for */
    size_t clen, st;			/* wide string to multibyte  string */
					/* conversion. */
    char *char_buf;			/* For unknown and user procs */
    char **cargv, *ctemp;
    int i, j, err;

    /*
     * Initialize the result to an empty string and clear out any
     * error information.  This makes sure that we return an empty
     * result if there are no commands in the command string.
     */

    Tcl_FreeResult((Tcl_Interp *) iPtr);
    iPtr->result = iPtr->resultSpace;
    iPtr->resultSpace[0] = 0;
    result = TCL_OK;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * Initialize the area in which command copies will be assembled.
     */

    wpv.buffer = wcopyStorage;
    wpv.end = wcopyStorage + NUM_CHARS - 1;
    wpv.expandProc = _TclWExpandParseValue;
    wpv.clientData = (ClientData) NULL;

    wsrc = wcmd;
    flags = iPtr->evalFlags;
    iPtr->evalFlags = 0;
    if (flags & TCL_BRACKET_TERM) 
    {
	wtermChar = i18n_special_chars.w_cl_sbr;
    } 
    else 
    {
	wtermChar = i18n_special_chars.w_esc_zero;
    }
    wtermPtr = wsrc;
    wcmdStart = wsrc;

    /*
     * Check depth of nested calls to _Tcl_WEval:  if this gets too large,
     * it's probably because of an infinite loop somewhere.
     */

    iPtr->numLevels++;
    if (iPtr->numLevels > iPtr->maxNestingDepth) 
    {
	iPtr->numLevels--;
	iPtr->result =  (char *)dce_msg_get(tcl_s_basic_too_many_nested_calls);
	iPtr->freeProc = (Tcl_FreeProc *) free;

	iPtr->wtermPtr = wtermPtr;
	return TCL_ERROR;
    }

    /*
     * There can be many sub-commands (separated by semi-colons or
     * newlines) in one command string.  This outer loop iterates over
     * individual commands.
     */

    while (*wsrc != wtermChar) 
    {
	iPtr->flags &= ~(ERR_IN_PROGRESS | ERROR_CODE_SET);

	/*
	 * Skim off leading white space and semi-colons, and skip
	 * comments.
	 */

	while (1) 
	{
	    register wchar_t wc = *wsrc;

	    if ((_Tcl_WCtype(interp, wc) != TCL_SPACE) 
			&& (wc != i18n_special_chars.w_sem_col) 
			&& (wc != i18n_special_chars.w_esc_n)) 
	    {
		break;
	    }
	    wsrc += 1;
	}
	if (*wsrc == i18n_special_chars.w_sharp) 
	{
	    for (wsrc++; *wsrc != i18n_special_chars.w_esc_zero; wsrc++) 
	    {
		if ((*wsrc == i18n_special_chars.w_esc_n) 
			&& (wsrc[-1] != i18n_special_chars.w_bsla)) 
		{
		    wsrc++;
		    break;
		}
	    }
	    continue;
	}
	wcmdStart = wsrc;

	/*
	 * Parse the words of the command, generating the argc and
	 * argv for the command procedure.  May have to call
	 * TclParseWords several times, expanding the argv array
	 * between calls.
	 */

	wpv.next = woldBuffer = wpv.buffer;
	argc = 0;
	while (1) 
	{
	    int newArgs, maxArgs;
	    wchar_t **wnewArgv;
	    int i;

	    /*
	     * Note:  the "- 2" below guarantees that we won't use the
	     * last two argv slots here.  One is for a NULL pointer to
	     * mark the end of the list, and the other is to leave room
	     * for inserting the command name "unknown" as the first
	     * argument (see below).
	     */

	    maxArgs = argSize - argc - 2;
	    result = _TclWParseWords((Tcl_Interp *) iPtr, wsrc, flags,
		    maxArgs, &wtermPtr, &newArgs, &wargv[argc], &wpv);
	    wsrc = wtermPtr;

	    if (result != TCL_OK) 
	    {
		ellipsis = "...";
		goto done;
	    }

	    /*
	     * Careful!  Buffer space may have gotten reallocated while
	     * parsing words.  If this happened, be sure to update all
	     * of the older argv pointers to refer to the new space.
	     */

	    if (woldBuffer != wpv.buffer) 
	    {
		int i;

		for (i = 0; i < argc; i++) 
		{
		    wargv[i] = wpv.buffer + (wargv[i] - woldBuffer);
		}
		woldBuffer = wpv.buffer;
	    }
	    argc += newArgs;
	    if (newArgs < maxArgs) 
	    {
		wargv[argc] = (wchar_t *) NULL;
		break;
	    }

	    /*
	     * Args didn't all fit in the current array.  Make it bigger.
	     */

	    argSize *= 2;
	    wnewArgv = (wchar_t **)
		    ckalloc((unsigned) argSize * sizeof(wchar_t *));
	    for (i = 0; i < argc; i++) 
	    {
		wnewArgv[i] = wargv[i];
	    }
	    if (wargv != wargStorage) 
	    {
		ckfree((char *) wargv);
	    }
	    wargv = wnewArgv;
	}

	/*
	 * If this is an empty command (or if we're just parsing
	 * commands without evaluating them), then just skip to the
	 * next command.
	 */

	if ((argc == 0) || iPtr->noEval) 
	{
	    continue;
	}
	wargv[argc] = (wchar_t *)NULL;

	/*
	 * Save information for the history module, if needed.
	 */

	if (flags & TCL_RECORD_BOUNDS) 
	{
	    iPtr->wevalFirst = wcmdStart;
	    iPtr->wevalLast = wsrc-1;
	}

	/*
	 * Find the procedure to execute this command.  If there isn't
	 * one, then see if there is a command "unknown".  If so,
	 * invoke it instead, passing it the words of the original
	 * command as arguments.
	 */

	clen = wcstombs((char *)NULL, (const wchar_t *)wargv[0], wcslen(wargv[0]));
	t_char_buf = (char *)ckalloc((clen+4) * sizeof(char));
	char_buf = (char *)ckalloc((clen+1) * sizeof(char));
	if ((wcstombs(t_char_buf+2, (const wchar_t *)wargv[0], clen+1) == (size_t)-1))
	{
		ckfree(t_char_buf);
		ckfree(char_buf);
		i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *)NULL);
		free(i18n_msg_format);
		goto done;
	}
	t_char_buf[0] = '_';		/* create _w* command name */
	t_char_buf[1] = 'w';
	t_char_buf[clen+2] = '\0';

	if ((wcstombs(char_buf, (const wchar_t *)wargv[0], clen+1) == (size_t)-1))
	{
		ckfree(t_char_buf);
		ckfree(char_buf);
		i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *)NULL);
		free(i18n_msg_format);
		goto done;
	}
	char_buf[clen] = '\0';

	hPtr = Tcl_FindHashEntry(&iPtr->commandTable, t_char_buf);
	if (hPtr == NULL) 
	{
	    int i;

	    hPtr = Tcl_FindHashEntry(&iPtr->commandTable, char_buf);
	    if (hPtr == NULL) 
	    {
		hPtr = Tcl_FindHashEntry(&iPtr->commandTable, "unknown");
		if (hPtr == NULL) 
		{
		    Tcl_ResetResult(interp);
		    i18n_msg_format = dce_msg_get(tcl_s_basic_invalid_cmd_name);
		    i18n_msg_buf = (unsigned char *)ckalloc(strlen(t_char_buf + 2) + strlen((char *)i18n_msg_format) + 1);
		    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, t_char_buf + 2);
		    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		    free(i18n_msg_format);
		    ckfree(i18n_msg_buf);
		    ckfree(t_char_buf);
		    ckfree(char_buf);

		    result = TCL_ERROR;
		    goto done;
		}
	    }
	    for (i = argc; i >= 0; i--) 
	    {
		wargv[i+1] = wargv[i];
	    }

	    char_buf_used = 1;
	    wargv[0] = wtemp;
	    argc++;
	}
	cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
	ckfree(t_char_buf);
	ckfree(char_buf);

	/*
	 * Call trace procedures, if any.
	 */
	for (tracePtr = iPtr->tracePtr; tracePtr != NULL;
		tracePtr = tracePtr->nextPtr) 
	{
	    wchar_t wsaved;
	    char *cmdStart;

	    err = 0;
	    if (tracePtr->level < iPtr->numLevels) 
	    {
		continue;
	    }
	    wsaved = *wsrc;
	    *wsrc = i18n_special_chars.w_esc_zero;

	    _WChar_To_Multibyte(interp, clen, wcmdStart, cmdStart, st)
	    if (st == (size_t)-1)
		goto done;

	    cargv = ckalloc(argc * sizeof(char *));
	    for (i=0, j=1; i < argc; i++, j++)
	    {
		/* generate cargv */
		_WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
		if (st == (size_t)-1)
		{
		    err = 1;
		    goto cargvErr;
		}
		cargv[i] = ctemp;
	    }

	    (*tracePtr->proc)(tracePtr->clientData, interp, iPtr->numLevels,
		    cmdStart, cmdPtr->proc, cmdPtr->clientData, argc, cargv);
	    *wsrc = wsaved;

	    cargvErr:
		ckfree(cmdStart);
		while (j--)
		    ckfree(cargv[j-1]);
		ckfree(*cargv);

		if (err)
		    goto done;
	}

	/*
	 * At long last, invoke the command procedure.  Reset the
	 * result to its default empty value first (it could have
	 * gotten changed by earlier commands in the same command
	 * string).
	 */

	iPtr->cmdCount++;
	Tcl_FreeResult(iPtr);
	iPtr->result = iPtr->resultSpace;
	iPtr->resultSpace[0] = 0;
	/* 
	 * invoke wide character command 
	 */
	if (char_buf_used)
	{
	    cargv = ckalloc(argc * sizeof(char *));
	    for (i=0, j=0; i < argc; i++, j++)
	    {
		/* generate cargv */
		if (wargv[i] != (wchar_t)NULL)
		{
		    _WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
		    if (st == (size_t)-1)
		    {
			err = 1;
			goto cargvErr;
		    }
		    cargv[i] = ctemp;
		}
		else
		{
		    cargv[i] = NULL;
		}
	    }
	    result = (*cmdPtr->proc)(cmdPtr->clientData, interp, argc, cargv);

	    while (j--)
	    {
		if (cargv[j] != NULL)
		    ckfree(cargv[j]);
	    }
	    ckfree(cargv);
	}
	else
	    result = (*cmdPtr->wproc)(cmdPtr->clientData, interp, argc, wargv);

	if (tcl_AsyncReady) 
	{
	    result = Tcl_AsyncInvoke(interp, result);
	}
	if (result != TCL_OK) 
	{
	    break;
	}
    }

    /*
     * Free up any extra resources that were allocated.
     */

    done:
    if (wpv.buffer != wcopyStorage) 
    {
	ckfree((char *) wpv.buffer);
    }
    if (wargv != wargStorage) 
    {
	ckfree((char *) wargv);
    }
    iPtr->numLevels--;
    if (iPtr->numLevels == 0) 
    {
	if (result == TCL_RETURN) 
	{
	    result = TCL_OK;
	}
	if ((result != TCL_OK) && (result != TCL_ERROR)) 
	{
	    Tcl_ResetResult(interp);
	    if (result == TCL_BREAK) 
	    {
		iPtr->result = (char *)dce_msg_get(tcl_s_basic_break_outside_of_loop);
		iPtr->freeProc = (Tcl_FreeProc *) free;
	    } 
	    else if (result == TCL_CONTINUE) 
	    {
		iPtr->result = (char *)dce_msg_get(tcl_s_basic_continue_outside_of_loop);
		iPtr->freeProc = (Tcl_FreeProc *) free;
	    } 
	    else 
	    {
		iPtr->result = iPtr->resultSpace;
		i18n_msg_format = dce_msg_get(tcl_s_basic_cmd_return_bad_code);
		sprintf(iPtr->resultSpace, (char *)i18n_msg_format, result);
		free(i18n_msg_format);
	    }
	    result = TCL_ERROR;
	}
	if (iPtr->flags & DELETED) 
	{
	    Tcl_DeleteInterp(interp);
	}
    }

    /*
     * If an error occurred, record information about what was being
     * executed when the error occurred.
     */

    if ((result == TCL_ERROR) && !(iPtr->flags & ERR_ALREADY_LOGGED)) 
    {
	int numChars;
	wchar_t *wp;
	char *cmdStart, *copyStorage;
	size_t len;

	/*
	 * Compute the line number where the error occurred.
	 */

	iPtr->errorLine = 1;
	for (wp = wcmd; wp != wcmdStart; wp++) 
	{
	    if (*wp == i18n_special_chars.w_esc_n) 
	    {
		iPtr->errorLine++;
	    }
	}
	for ( ; iswspace(*wp) || (*wp == i18n_special_chars.w_sem_col); wp++) 
	{
	    if (*wp == i18n_special_chars.w_esc_n) 
	    {
		iPtr->errorLine++;
	    }
	}

	/*
	 * Figure out how much of the command to print in the error
	 * message (up to a certain number of characters, or up to
	 * the first new-line).
	 */

	numChars = wsrc - wcmdStart;
	if (numChars > (NUM_CHARS-50)) 
	{
	    numChars = NUM_CHARS-50;
	    ellipsis = " ...";
	}

	/* convert wcmdStart to character form */
	_WChar_To_Multibyte(interp, len, wcmdStart, cmdStart, st)
	if (st == (size_t)-1)
		goto end;

#ifndef AIX32
        /*
         * This fix is excluded for AIX due to missing support for
         * XPG compliant field width specifier in format string.
         */
        /*
         * Use properly generated format string and necessary argument list
         * to print message. So for,
         *                 "%.*s" ---sams------> "%1$.*2$s"
         * So sprintf should be passed 'cmdStart, numChars' arguments for
         *                 "%.*s"
         * where,
         *         numChars indicates the field width, and
         *         cmdStart indicates the string to be printed
         */
	if (!(iPtr->flags & ERR_IN_PROGRESS)) {
	    i18n_msg_format = dce_msg_get(tcl_s_basic_while_executing);
	    sprintf(copyStorage, (char *)i18n_msg_format, cmdStart, numChars,
                    ellipsis);
	    free(i18n_msg_format);
	    ckfree(cmdStart);
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_basic_invoked_from_within);
	    sprintf(copyStorage, (char *)i18n_msg_format, cmdStart, numChars,
                    ellipsis);
	    free(i18n_msg_format);
	    ckfree(cmdStart);
	}
#else /* defined AIX32 */
	/* 
	 * AIX cannot process sprintf using implicit next parameter ('*s')
	 * and numbered parameter ('%1$') in the same call, so need to
	 * do prior substitution.  Adding space for 3 digits (plus null)
	 * is enough because numChars can't exceed NUM_CHARS (200) 
	 */
	if (!(iPtr->flags & ERR_IN_PROGRESS)) 
	{
	    unsigned char *formatPtr, *bufPtr;
	    i18n_msg_format = dce_msg_get(tcl_s_basic_while_executing);
	    copyStorage = (char *)ckalloc(strlen((char *)i18n_msg_format) + len + 10);
	    i18n_msg_buf = (unsigned char *)
		            ckalloc(strlen((char *)i18n_msg_format) + 4);
	    for (formatPtr = i18n_msg_format, bufPtr = i18n_msg_buf;
		 *formatPtr != '\0'; formatPtr++ ) {
		if (*formatPtr == '*') {
		    bufPtr += sprintf ((char *)bufPtr, "%d", numChars);
		} else {
		    *bufPtr++ = *formatPtr;
		}
	    }
	    *bufPtr = '\0';

	    sprintf(copyStorage, 
		    (char *)i18n_msg_buf, cmdStart, ellipsis);
	    ckfree(i18n_msg_buf);
	    free(i18n_msg_format);
	    ckfree(cmdStart);
	} 
	else 
	{
	    unsigned char *formatPtr, *bufPtr;
	    i18n_msg_format = dce_msg_get(tcl_s_basic_invoked_from_within);
	    copyStorage = (char *)ckalloc(strlen((char *)i18n_msg_format) + len + 10);
	    i18n_msg_buf = (unsigned char *)
		            ckalloc(strlen((char *)i18n_msg_format) + 4);
	    for (formatPtr = i18n_msg_format, bufPtr = i18n_msg_buf;
		 *formatPtr != '\0'; formatPtr++ ) {
		if (*formatPtr == '*') {
		    bufPtr += sprintf ((char *)bufPtr, "%d", numChars);
		} else {
		    *bufPtr++ = *formatPtr;
		}
	    }
	    *bufPtr = '\0';

	    sprintf(copyStorage, (char *)i18n_msg_buf, cmdStart, ellipsis);
	    ckfree(i18n_msg_buf);
	    free(i18n_msg_format);
	    ckfree(cmdStart);
	}
#endif /* AIX32 */
	Tcl_AddErrorInfo(interp, copyStorage);
	ckfree(copyStorage);
	iPtr->flags &= ~ERR_ALREADY_LOGGED;
    } 
    else 
    {
	iPtr->flags &= ~ERR_ALREADY_LOGGED;
    }

    end:

    iPtr->wtermPtr = wtermPtr;
    iPtr->wcmd = wcmd;

    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WInit --
 *
 *	This procedure is typically invoked by Tcl_AppInit procedures
 *	to perform additional initialization for a Tcl interpreter,
 *	such as sourcing the "init.tcl" script.
 *
 * Results:
 *	Returns a standard Tcl completion code and sets interp->result
 *	if there is an error.
 *
 * Side effects:
 *	Depends on what's in the init.tcl script.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WInit(interp)
    Tcl_Interp *interp;		/* Interpreter to initialize. */
{
    static char initCmd[] =
	"if [file exists [info library]/init.tcl] {\n\
	    source [info library]/init.tcl\n\
	} else {\n\
	    set msg \"%s\"\n\
	    error $msg\n\
	}";
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    int tcl_eval_return;
    wchar_t *wcmd;
    size_t wlen, st;

    i18n_msg_format = dce_msg_get(tcl_s_basic_interp_init_error);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)initCmd)
				+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, initCmd, (char *)i18n_msg_format);
    free(i18n_msg_format);

    _Multibyte_to_WChar(interp, wlen, (char *)i18n_msg_buf, wcmd, st)
    if (st == (size_t)-1)
    {
	ckfree(i18n_msg_buf);
	return TCL_ERROR;
    }
    ckfree(i18n_msg_buf);

    tcl_eval_return =  _Tcl_WEval(interp, wcmd);

    ckfree(wcmd);
    return tcl_eval_return;
}
