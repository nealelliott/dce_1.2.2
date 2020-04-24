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
 * $Log: tclProc.c,v $
 * Revision 1.1.8.3  1996/02/18  19:19:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:39  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:34:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/28  20:26 UTC  dnguyen
 * 	Merge cleaning up of I18N-commented code
 * 
 * 	HP revision /main/HPDCE02/dnguyen_mothra/1  1995/04/11  16:43 UTC  dnguyen
 * 	Clean up I18N comments
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  17:12 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/12/05  17:10 UTC  truitt
 * 	Internal algorithm used to determine whether an object is
 * 	a procedure or not was using && instead of ||.
 * 	[1995/12/08  18:06:08  root]
 * 
 * Revision 1.1.4.1  1994/10/21  21:39:20  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:33:00  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:30  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:13:42  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclProc.c --
 *
 *	This file contains routines that implement Tcl procedures,
 *	including the "proc" and "uplevel" commands.
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
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * Forward references to procedures defined later in this file:
 */

static void	CleanupProc _ANSI_ARGS_((Proc *procPtr));
static  int	InterpProc _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, char **argv));
static  void	ProcDeleteProc _ANSI_ARGS_((ClientData clientData));
static  int	_WInterpProc _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, char **argv));

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ProcCmd --
 *
 *	This procedure is invoked to process the "proc" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A new procedure gets created.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ProcCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    register Proc *procPtr;
    int result, argCount, i;
    char **argArray = NULL;
    Arg *lastArgPtr;
    register Arg *argPtr = NULL;	/* Initialization not needed, but
					 * prevents compiler warning. */
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc != 4) {
	i18n_msg_format = dce_msg_get(tcl_s_proc_wnum_args_name_args_body);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    procPtr = (Proc *) ckalloc(sizeof(Proc));
    procPtr->iPtr = iPtr;
    procPtr->refCount = 1;
    procPtr->command = (char *) ckalloc((unsigned) strlen(argv[3]) + 1);
    strcpy(procPtr->command, argv[3]);
    procPtr->argPtr = NULL;

    /*
     * Break up the argument list into argument specifiers, then process
     * each argument specifier.
     */

    result = Tcl_SplitList(interp, argv[2], &argCount, &argArray);
    if (result != TCL_OK) {
	goto procError;
    }
    lastArgPtr = NULL;
    for (i = 0; i < argCount; i++) {
	int fieldCount, nameLength, valueLength;
	char **fieldValues;

	/*
	 * Now divide the specifier up into name and default.
	 */

	result = Tcl_SplitList(interp, argArray[i], &fieldCount,
		&fieldValues);
	if (result != TCL_OK) {
	    goto procError;
	}
	if (fieldCount > 2) {
	    ckfree((char *) fieldValues);
	    i18n_msg_format = dce_msg_get(tcl_s_proc_too_many_flds_in_arg);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argArray[i])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argArray[i]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto procError;
	}
	if ((fieldCount == 0) || (*fieldValues[0] == 0)) {
	    ckfree((char *) fieldValues);
	    i18n_msg_format = dce_msg_get(tcl_s_proc_no_name_arg);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto procError;
	}
	nameLength = strlen(fieldValues[0]) + 1;
	if (fieldCount == 2) {
	    valueLength = strlen(fieldValues[1]) + 1;
	} else {
	    valueLength = 0;
	}
	argPtr = (Arg *) ckalloc((unsigned)
		(sizeof(Arg) - sizeof(argPtr->name) + nameLength
		+ valueLength));
	if (lastArgPtr == NULL) {
	    procPtr->argPtr = argPtr;
	} else {
	    lastArgPtr->nextPtr = argPtr;
	}
	lastArgPtr = argPtr;
	argPtr->nextPtr = NULL;
	strcpy(argPtr->name, fieldValues[0]);
	if (fieldCount == 2) {
	    argPtr->defValue = argPtr->name + nameLength;
	    strcpy(argPtr->defValue, fieldValues[1]);
	} else {
	    argPtr->defValue = NULL;
	}
	ckfree((char *) fieldValues);
    }

    Tcl_CreateCommand(interp, argv[1], InterpProc, (ClientData) procPtr,
	    ProcDeleteProc);
    ckfree((char *) argArray);
    return TCL_OK;

    procError:
    ckfree(procPtr->command);
    while (procPtr->argPtr != NULL) {
	argPtr = procPtr->argPtr;
	procPtr->argPtr = argPtr->nextPtr;
	ckfree((char *) argPtr);
    }
    ckfree((char *) procPtr);
    if (argArray != NULL) {
	ckfree((char *) argArray);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * TclGetFrame --
 *
 *	Given a description of a procedure frame, such as the first
 *	argument to an "uplevel" or "upvar" command, locate the
 *	call frame for the appropriate level of procedure.
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the
 *	frame (in this case an error message is left in interp->result).
 *	1 is returned if string was either a number or a number preceded
 *	by "#" and it specified a valid frame.  0 is returned if string
 *	isn't one of the two things above (in this case, the lookup
 *	acts as if string were "1").  The variable pointed to by
 *	framePtrPtr is filled in with the address of the desired frame
 *	(unless an error occurs, in which case it isn't modified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
TclGetFrame(interp, string, framePtrPtr)
    Tcl_Interp *interp;		/* Interpreter in which to find frame. */
    char *string;		/* String describing frame. */
    CallFrame **framePtrPtr;	/* Store pointer to frame here (or NULL
				 * if global frame indicated). */
{
    register Interp *iPtr = (Interp *) interp;
    int curLevel, level, result;
    CallFrame *framePtr;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Parse string to figure out which level number to go to.
     */

    result = 1;
    curLevel = (iPtr->varFramePtr == NULL) ? 0 : iPtr->varFramePtr->level;
    if (*string == '#') {
	if (Tcl_GetInt(interp, string+1, &level) != TCL_OK) {
	    return -1;
	}
	if (level < 0) {
	    levelError:
	    i18n_msg_format = dce_msg_get(tcl_s_proc_bad_level);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return -1;
	}
    } else if (isdigit(UCHAR(*string))) {
	if (Tcl_GetInt(interp, string, &level) != TCL_OK) {
	    return -1;
	}
	level = curLevel - level;
    } else {
	level = curLevel - 1;
	result = 0;
    }

    /*
     * Figure out which frame to use, and modify the interpreter so
     * its variables come from that frame.
     */

    if (level == 0) {
	framePtr = NULL;
    } else {
	for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		framePtr = framePtr->callerVarPtr) {
	    if (framePtr->level == level) {
		break;
	    }
	}
	if (framePtr == NULL) {
	    goto levelError;
	}
    }
    *framePtrPtr = framePtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UplevelCmd --
 *
 *	This procedure is invoked to process the "uplevel" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_UplevelCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int result;
    CallFrame *savedVarFramePtr, *framePtr;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 2) {
	uplevelSyntax:
	i18n_msg_format = dce_msg_get(tcl_s_proc_wnum_args_level_cmd_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Find the level to use for executing the command.
     */

    result = TclGetFrame(interp, argv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    argc -= (result+1);
    if (argc == 0) {
	goto uplevelSyntax;
    }
    argv += (result+1);

    /*
     * Modify the interpreter state to execute in the given frame.
     */

    savedVarFramePtr = iPtr->varFramePtr;
    iPtr->varFramePtr = framePtr;

    /*
     * Execute the residual arguments as a command.
     */

    if (argc == 1) {
	result = Tcl_Eval(interp, argv[0]);
    } else {
	char *cmd;

	cmd = Tcl_Concat(argc, argv);
	result = Tcl_Eval(interp, cmd);
	ckfree(cmd);
    }
    if (result == TCL_ERROR) {
	char msg[60];
	i18n_msg_format = dce_msg_get(tcl_s_proc_uplevel_body_line);
	sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
	free(i18n_msg_format);

	Tcl_AddErrorInfo(interp, msg);
    }

    /*
     * Restore the variable frame, and return.
     */

    iPtr->varFramePtr = savedVarFramePtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * TclFindProc --
 *
 *	Given the name of a procedure, return a pointer to the
 *	record describing the procedure.
 *
 * Results:
 *	NULL is returned if the name doesn't correspond to any
 *	procedure.  Otherwise the return value is a pointer to
 *	the procedure's record.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Proc *
TclFindProc(iPtr, procName)
    Interp *iPtr;		/* Interpreter in which to look. */
    char *procName;		/* Name of desired procedure. */
{
    Tcl_HashEntry *hPtr;
    Command *cmdPtr;

    hPtr = Tcl_FindHashEntry(&iPtr->commandTable, procName);
    if (hPtr == NULL) {
	return NULL;
    }
    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
    if (cmdPtr->proc != InterpProc &&
	cmdPtr->proc != _WInterpProc) {
	return NULL;
    }
    return (Proc *) cmdPtr->clientData;
}

/*
 *----------------------------------------------------------------------
 *
 * TclIsProc --
 *
 *	Tells whether a command is a Tcl procedure or not.
 *
 * Results:
 *	If the given command is actuall a Tcl procedure, the
 *	return value is the address of the record describing
 *	the procedure.  Otherwise the return value is 0.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

Proc *
TclIsProc(cmdPtr)
    Command *cmdPtr;		/* Command to test. */
{
    if (cmdPtr->proc == InterpProc ||
	cmdPtr->proc == _WInterpProc) {
	return (Proc *) cmdPtr->clientData;
    }
    return (Proc *) 0;
}

/*
 *----------------------------------------------------------------------
 *
 * InterpProc --
 *
 *	When a Tcl procedure gets invoked, this routine gets invoked
 *	to interpret the procedure.
 *
 * Results:
 *	A standard Tcl result value, usually TCL_OK.
 *
 * Side effects:
 *	Depends on the commands in the procedure.
 *
 *----------------------------------------------------------------------
 */

static int
InterpProc(clientData, interp, argc, argv)
    ClientData clientData;	/* Record describing procedure to be
				 * interpreted. */
    Tcl_Interp *interp;		/* Interpreter in which procedure was
				 * invoked. */
    int argc;			/* Count of number of arguments to this
				 * procedure. */
    char **argv;		/* Argument values. */
{
    register Proc *procPtr = (Proc *) clientData;
    register Arg *argPtr;
    register Interp *iPtr;
    char **args;
    CallFrame frame;
    char *value;
    int result;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Set up a call frame for the new procedure invocation.
     */

    iPtr = procPtr->iPtr;
    Tcl_InitHashTable(&frame.varTable, TCL_STRING_KEYS);
    if (iPtr->varFramePtr != NULL) {
	frame.level = iPtr->varFramePtr->level + 1;
    } else {
	frame.level = 1;
    }
    frame.argc = argc;
    frame.argv = argv;
    frame.callerPtr = iPtr->framePtr;
    frame.callerVarPtr = iPtr->varFramePtr;
    iPtr->framePtr = &frame;
    iPtr->varFramePtr = &frame;
    iPtr->returnCode = TCL_OK;

    /*
     * Match the actual arguments against the procedure's formal
     * parameters to compute local variables.
     */

    for (argPtr = procPtr->argPtr, args = argv+1, argc -= 1;
	    argPtr != NULL;
	    argPtr = argPtr->nextPtr, args++, argc--) {

	/*
	 * Handle the special case of the last formal being "args".  When
	 * it occurs, assign it a list consisting of all the remaining
	 * actual arguments.
	 */

	if ((argPtr->nextPtr == NULL)
		&& (strcmp(argPtr->name, "args") == 0)) {
	    if (argc < 0) {
		argc = 0;
	    }
	    value = Tcl_Merge(argc, args);
	    Tcl_SetVar(interp, argPtr->name, value, 0);
	    ckfree(value);
	    argc = 0;
	    break;
	} else if (argc > 0) {
	    value = *args;
	} else if (argPtr->defValue != NULL) {
	    value = argPtr->defValue;
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_proc_no_val_given_to_par);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argPtr->name)
					+ strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argPtr->name, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto procDone;
	}
	Tcl_SetVar(interp, argPtr->name, value, 0);
    }
    if (argc > 0) {
	i18n_msg_format = dce_msg_get(tcl_s_proc_called_with_too_many_args);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	result = TCL_ERROR;
	goto procDone;
    }

    /*
     * Invoke the commands in the procedure's body.
     */

    procPtr->refCount++;
    result = Tcl_Eval(interp, procPtr->command);
    procPtr->refCount--;
    if (procPtr->refCount <= 0) {
	CleanupProc(procPtr);
    }
    if (result == TCL_RETURN) {
	result = iPtr->returnCode;
	iPtr->returnCode = TCL_OK;
	if (result == TCL_ERROR) {
	    Tcl_SetVar2(interp, "errorCode", (char *) NULL,
		    (iPtr->errorCode != NULL) ? iPtr->errorCode : "NONE",
		    TCL_GLOBAL_ONLY);
	    iPtr->flags |= ERROR_CODE_SET;
	    if (iPtr->errorInfo != NULL) {
		Tcl_SetVar2(interp, "errorInfo", (char *) NULL,
			iPtr->errorInfo, TCL_GLOBAL_ONLY);
		iPtr->flags |= ERR_IN_PROGRESS;
	    }
	}
    } else if (result == TCL_ERROR) {
	char msg[100];

	/*
	 * Record information telling where the error occurred.
	 */
	i18n_msg_format = dce_msg_get(tcl_s_proc_proc_line);
	sprintf(msg, (char *)i18n_msg_format, argv[0], iPtr->errorLine);
	free(i18n_msg_format);

	Tcl_AddErrorInfo(interp, msg);
    } else if (result == TCL_BREAK) {
	iPtr->result = (char *)dce_msg_get(tcl_s_proc_break_outside_loop);
	iPtr->freeProc = (Tcl_FreeProc *) free;

	result = TCL_ERROR;
    } else if (result == TCL_CONTINUE) {
	iPtr->result = (char *)dce_msg_get(tcl_s_proc_continue_outside_loop);
	iPtr->freeProc = (Tcl_FreeProc *) free;

	result = TCL_ERROR;
    }

    /*
     * Delete the call frame for this procedure invocation (it's
     * important to remove the call frame from the interpreter
     * before deleting it, so that traces invoked during the
     * deletion don't see the partially-deleted frame).
     */

    procDone:
    iPtr->framePtr = frame.callerPtr;
    iPtr->varFramePtr = frame.callerVarPtr;
    TclDeleteVars(iPtr, &frame.varTable);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * ProcDeleteProc --
 *
 *	This procedure is invoked just before a command procedure is
 *	removed from an interpreter.  Its job is to release all the
 *	resources allocated to the procedure.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets freed, unless the procedure is actively being
 *	executed.  In this case the cleanup is delayed until the
 *	last call to the current procedure completes.
 *
 *----------------------------------------------------------------------
 */

static void
ProcDeleteProc(clientData)
    ClientData clientData;		/* Procedure to be deleted. */
{
    Proc *procPtr = (Proc *) clientData;

    procPtr->refCount--;
    if (procPtr->refCount <= 0) {
	CleanupProc(procPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * CleanupProc --
 *
 *	This procedure does all the real work of freeing up a Proc
 *	structure.  It's called only when the structure's reference
 *	count becomes zero.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory gets freed.
 *
 *----------------------------------------------------------------------
 */

static void
CleanupProc(procPtr)
    register Proc *procPtr;		/* Procedure to be deleted. */
{
    register Arg *argPtr;

    ckfree((char *) procPtr->command);
    for (argPtr = procPtr->argPtr; argPtr != NULL; ) {
	Arg *nextPtr = argPtr->nextPtr;

	ckfree((char *) argPtr);
	argPtr = nextPtr;
    }
    ckfree((char *) procPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * _TclWGetFrame --
 *
 *	Given a description of a procedure frame, such as the first
 *	argument to an "uplevel" or "upvar" command, locate the
 *	call frame for the appropriate level of procedure.
 *
 * Results:
 *	The return value is -1 if an error occurred in finding the
 *	frame (in this case an error message is left in interp->result).
 *	1 is returned if string was either a number or a number preceded
 *	by "#" and it specified a valid frame.  0 is returned if string
 *	isn't one of the two things above (in this case, the lookup
 *	acts as if string were "1").  The variable pointed to by
 *	framePtrPtr is filled in with the address of the desired frame
 *	(unless an error occurs, in which case it isn't modified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_TclWGetFrame(interp, wstring, framePtrPtr)
    Tcl_Interp *interp;		/* Interpreter in which to find frame. */
    wchar_t *wstring;		/* String describing frame. */
    CallFrame **framePtrPtr;	/* Store pointer to frame here (or NULL
				 * if global frame indicated). */
{
    register Interp *iPtr = (Interp *) interp;
    int curLevel, level, result;
    CallFrame *framePtr;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return -1;
    }

    /*
     * Parse string to figure out which level number to go to.
     */

    result = 1;
    curLevel = (iPtr->varFramePtr == NULL) ? 0 : iPtr->varFramePtr->level;
    if (*wstring == i18n_special_chars.w_sharp) 
    {
	if (_Tcl_WGetInt(interp, wstring+1, &level) != TCL_OK) 
	{
	    return -1;
	}
	if (level < 0) 
	{
	    char *ctemp;
	    size_t clen, st;

	    levelError:

	    _WChar_To_Multibyte(interp, clen, wstring, ctemp, st)
	    if (st == (size_t)-1)
		return -1;

	    i18n_msg_format = dce_msg_get(tcl_s_proc_bad_level);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return -1;
	}
    } 
    else if (iswdigit(*wstring)) 
    {
	if (_Tcl_WGetInt(interp, wstring, &level) != TCL_OK) 
	{
	    return -1;
	}
	level = curLevel - level;
    } 
    else 
    {
	level = curLevel - 1;
	result = 0;
    }

    /*
     * Figure out which frame to use, and modify the interpreter so
     * its variables come from that frame.
     */

    if (level == 0) 
    {
	framePtr = NULL;
    } 
    else 
    {
	for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		framePtr = framePtr->callerVarPtr) 
	{
	    if (framePtr->level == level) 
	    {
		break;
	    }
	}
	if (framePtr == NULL) 
	{
	    goto levelError;
	}
    }
    *framePtrPtr = framePtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _WInterpProc --
 *
 *	When a Tcl procedure gets invoked, this routine gets invoked
 *	to interpret the procedure.
 *
 * Results:
 *	A standard Tcl result value, usually TCL_OK.
 *
 * Side effects:
 *	Depends on the commands in the procedure.
 *
 *----------------------------------------------------------------------
 */

static int
_WInterpProc(clientData, interp, argc, argv)
    ClientData clientData;	/* Record describing procedure to be
				 * interpreted. */
    Tcl_Interp *interp;		/* Interpreter in which procedure was
				 * invoked. */
    int argc;			/* Count of number of arguments to this
				 * procedure. */
    char **argv;		/* Argument values. */
{
    register Proc *procPtr = (Proc *) clientData;
    register Arg *argPtr;
    register Interp *iPtr;
    char **args;
    CallFrame frame;
    wchar_t *wvalue, *wtemp;
    int result, cerr;
    size_t wlen, st;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Set up a call frame for the new procedure invocation.
     */

    iPtr = procPtr->iPtr;
    Tcl_InitHashTable(&frame.varTable, TCL_STRING_KEYS);
    if (iPtr->varFramePtr != NULL) {
	frame.level = iPtr->varFramePtr->level + 1;
    } else {
	frame.level = 1;
    }
    frame.argc = argc;
    frame.argv = argv;
    frame.callerPtr = iPtr->framePtr;
    frame.callerVarPtr = iPtr->varFramePtr;
    iPtr->framePtr = &frame;
    iPtr->varFramePtr = &frame;
    iPtr->returnCode = TCL_OK;

    /*
     * Match the actual arguments against the procedure's formal
     * parameters to compute local variables.
     */

    for (argPtr = procPtr->argPtr, args = argv+2, argc -= 2;
	    argPtr != NULL;
	    argPtr = argPtr->nextPtr, args++, argc--) 
    {
	/*
	 * Handle the special case of the last formal being "args".  When
	 * it occurs, assign it a list consisting of all the remaining
	 * actual arguments.
	 */

	if ((argPtr->nextPtr == NULL) && (strcmp(argPtr->name, "args") == 0)) 
	{
	    if (argc < 0) 
	    {
		argc = 0;
	    }
	    wvalue = _Tcl_WMerge(interp, argc, args, &cerr);
	    if (cerr == TCL_CONV_ERROR)
	    {
		result = TCL_ERROR;
		goto procDone;
	    }

	    _Multibyte_to_WChar(interp, wlen, argPtr->name, wtemp, st)
	    if (st == (size_t)TCL_CONV_ERROR)
	    {
		result = TCL_ERROR;
		goto procDone;
	    }

	    _Tcl_WSetVar(interp, wtemp, wvalue, 0);
	    ckfree(wvalue);
	    ckfree(wtemp);
	    argc = 0;
	    break;
	} 
	else if (argc > 0) 
	{
	    _Multibyte_to_WChar(interp, wlen, *args, wvalue, st)
	    if (st == (size_t)TCL_CONV_ERROR)
	    {
		result = TCL_ERROR;
		goto procDone;
	    }
	} 
	else if (argPtr->defValue != NULL) 
	{
	    _Multibyte_to_WChar(interp, wlen, argPtr->defValue, wvalue, st)
	    if (st == (size_t)TCL_CONV_ERROR)
	    {
		result = TCL_ERROR;
		goto procDone;
	    }
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_proc_no_val_given_to_par);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argPtr->name)
					+ strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argPtr->name, argv[1]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto procDone;
	}

	_Multibyte_to_WChar(interp, wlen, argPtr->name, wtemp, st)
	if (st == (size_t)TCL_CONV_ERROR)
	{
		result = TCL_ERROR;
		goto procDone;
	}
	_Tcl_WSetVar(interp, wtemp, wvalue, 0);
	ckfree(wvalue);
	ckfree(wtemp);
    }

    if (argc > 0) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_proc_called_with_too_many_args);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	result = TCL_ERROR;
	goto procDone;
    }

    /*
     * Invoke the commands in the procedure's body.
     */

    procPtr->refCount++;
    _Multibyte_to_WChar(interp, wlen, procPtr->command, wtemp, st)
    if (st == (size_t)TCL_CONV_ERROR)
    {
	result = TCL_ERROR;
	goto procDone;
    }
    result = _Tcl_WEval(interp, wtemp);
    ckfree(wtemp);
    procPtr->refCount--;
    if (procPtr->refCount <= 0) 
    {
	CleanupProc(procPtr);
    }
    if (result == TCL_RETURN) 
    {
	result = iPtr->returnCode;
	iPtr->returnCode = TCL_OK;
	if (result == TCL_ERROR) 
	{
	    Tcl_SetVar2(interp, "errorCode", (char *) NULL,
		    (iPtr->errorCode != NULL) ? iPtr->errorCode : "NONE",
		    TCL_GLOBAL_ONLY);
	    iPtr->flags |= ERROR_CODE_SET;
	    if (iPtr->errorInfo != NULL) 
	    {
		Tcl_SetVar2(interp, "errorInfo", (char *) NULL,
			iPtr->errorInfo, TCL_GLOBAL_ONLY);
		iPtr->flags |= ERR_IN_PROGRESS;
	    }
	}
    } 
    else if (result == TCL_ERROR) 
    {
	char msg[100];

	/*
	 * Record information telling where the error occurred.
	 */
	i18n_msg_format = dce_msg_get(tcl_s_proc_proc_line);
	sprintf(msg, (char *)i18n_msg_format, argv[1], iPtr->errorLine);
	free(i18n_msg_format);

	Tcl_AddErrorInfo(interp, msg);
    } 
    else if (result == TCL_BREAK) 
    {
	iPtr->result = (char *)dce_msg_get(tcl_s_proc_break_outside_loop);
	iPtr->freeProc = (Tcl_FreeProc *) free;

	result = TCL_ERROR;
    } 
    else if (result == TCL_CONTINUE) 
    {
	iPtr->result = (char *)dce_msg_get(tcl_s_proc_continue_outside_loop);
	iPtr->freeProc = (Tcl_FreeProc *) free;

	result = TCL_ERROR;
    }

    /*
     * Delete the call frame for this procedure invocation (it's
     * important to remove the call frame from the interpreter
     * before deleting it, so that traces invoked during the
     * deletion don't see the partially-deleted frame).
     */

    procDone:
    iPtr->framePtr = frame.callerPtr;
    iPtr->varFramePtr = frame.callerVarPtr;
    TclDeleteVars(iPtr, &frame.varTable);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WProcCmd --
 *
 *	This procedure is invoked to process the "proc" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A new procedure gets created.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WProcCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    register Proc *procPtr;
    int result, argCount, i;
    wchar_t **wargArray = NULL;
    Arg *lastArgPtr;
    register Arg *argPtr = NULL;	/* Initialization not needed, but
					 * prevents compiler warning. */
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp, *ctemp1;
    size_t clen, st;

    if (argc != 4) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_proc_wnum_args_name_args_body);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("proc")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "proc");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    _WChar_To_Multibyte(interp, clen, wargv[3], ctemp, st)
    if (st == (size_t)-1)
    {
	return TCL_ERROR;
    }
    procPtr = (Proc *) ckalloc(sizeof(Proc));
    procPtr->iPtr = iPtr;
    procPtr->refCount = 1;
    procPtr->command = (char *) ckalloc((unsigned) strlen(ctemp) + 1);
    strcpy(procPtr->command, ctemp);
    procPtr->argPtr = NULL;
    ckfree(ctemp);

    /*
     * Break up the argument list into argument specifiers, then process
     * each argument specifier.
     */

    result = _Tcl_WSplitList(interp, wargv[2], &argCount, &wargArray);
    if (result != TCL_OK) 
    {
	goto procError;
    }
    lastArgPtr = NULL;
    for (i = 0; i < argCount; i++) 
    {
	int fieldCount, nameLength, valueLength;
	wchar_t **wfieldValues;

	/*
	 * Now divide the specifier up into name and default.
	 */

	result = _Tcl_WSplitList(interp, wargArray[i], &fieldCount,
		&wfieldValues);
	if (result != TCL_OK) 
	{
	    goto procError;
	}
	if (fieldCount > 2) 
	{
	    ckfree((wchar_t *) wfieldValues);
	    _WChar_To_Multibyte(interp, clen, wargArray[i], ctemp, st)
	    if (st == (size_t)-1)
	    {
		result = TCL_ERROR;
		goto procError;
	    }
	    i18n_msg_format = dce_msg_get(tcl_s_proc_too_many_flds_in_arg);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    result = TCL_ERROR;
	    goto procError;
	}
	if ((fieldCount == 0) 
		|| (*wfieldValues[0] == i18n_special_chars.w_esc_zero)) 
	{
	    ckfree((wchar_t *) wfieldValues);
	    _WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
	    if (st == (size_t)-1)
	    {
		result = TCL_ERROR;
		goto procError;
	    }
	    i18n_msg_format = dce_msg_get(tcl_s_proc_no_name_arg);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    result = TCL_ERROR;
	    goto procError;
	}

	_WChar_To_Multibyte(interp, clen, wfieldValues[0], ctemp, st)
	if (st == (size_t)-1)
	{
	    result = TCL_ERROR;
	    goto procError;
	}

	if (fieldCount == 2) 
	{
	    _WChar_To_Multibyte(interp, clen, wfieldValues[1], ctemp1, st)
	    if (st == (size_t)-1)
	    {
		ckfree(ctemp);
		result = TCL_ERROR;
		goto procError;
	    }
	}

	nameLength = strlen(ctemp) + 1;
	if (fieldCount == 2) 
	{
	    valueLength = strlen(ctemp1) + 1;
	} 
	else 
	{
	    valueLength = 0;
	}
	argPtr = (Arg *) ckalloc((unsigned)
		(sizeof(Arg) - sizeof(argPtr->name) + nameLength
		+ valueLength));
	if (lastArgPtr == NULL) 
	{
	    procPtr->argPtr = argPtr;
	} 
	else 
	{
	    lastArgPtr->nextPtr = argPtr;
	}
	lastArgPtr = argPtr;
	argPtr->nextPtr = NULL;
	strcpy(argPtr->name, ctemp);
	if (fieldCount == 2) 
	{
	    argPtr->defValue = argPtr->name + nameLength;
	    strcpy(argPtr->defValue, ctemp1);
	} 
	else 
	{
	    argPtr->defValue = NULL;
	}
	ckfree((wchar_t *) wfieldValues);
	ckfree(ctemp);
	if (fieldCount == 2) 
	    ckfree(ctemp1);
    }

    _WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
    if (st == (size_t)-1)
    {
	result = TCL_ERROR;
	goto procError;
    }
    Tcl_CreateCommand(interp, ctemp, _WInterpProc, (ClientData) procPtr,
	    ProcDeleteProc);
    ckfree((wchar_t *) wargArray);
    ckfree(ctemp);
    return TCL_OK;

    procError:
    ckfree(procPtr->command);
    while (procPtr->argPtr != NULL) 
    {
	argPtr = procPtr->argPtr;
	procPtr->argPtr = argPtr->nextPtr;
	ckfree((char *) argPtr);
    }
    ckfree((char *) procPtr);
    if (wargArray != NULL) 
    {
	ckfree((wchar_t *) wargArray);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUplevelCmd --
 *
 *	This procedure is invoked to process the "uplevel" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WUplevelCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int result;
    CallFrame *savedVarFramePtr, *framePtr;
    unsigned char *i18n_msg_buf, *i18n_msg_format;

    if (argc < 2) 
    {
	uplevelSyntax:
	i18n_msg_format = dce_msg_get(tcl_s_proc_wnum_args_level_cmd_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("uplevel")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "uplevel");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Find the level to use for executing the command.
     */

    result = _TclWGetFrame(interp, wargv[1], &framePtr);
    if (result == -1) 
    {
	return TCL_ERROR;
    }
    argc -= (result+1);
    if (argc == 0) 
    {
	goto uplevelSyntax;
    }
    wargv += (result+1);

    /*
     * Modify the interpreter state to execute in the given frame.
     */

    savedVarFramePtr = iPtr->varFramePtr;
    iPtr->varFramePtr = framePtr;

    /*
     * Execute the residual arguments as a command.
     */

    if (argc == 1) 
    {
	result = _Tcl_WEval(interp, wargv[0]);
    } 
    else 
    {
	wchar_t *wcmd;

	wcmd = _Tcl_WConcat(interp, argc, wargv);
	result = _Tcl_WEval(interp, wcmd);
	ckfree(wcmd);
    }
    if (result == TCL_ERROR) 
    {
	char msg[60];
	i18n_msg_format = dce_msg_get(tcl_s_proc_uplevel_body_line);
	sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
	free(i18n_msg_format);

	Tcl_AddErrorInfo(interp, msg);
    }

    /*
     * Restore the variable frame, and return.
     */

    iPtr->varFramePtr = savedVarFramePtr;
    return result;
}
