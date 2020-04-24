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
 * $Log: tclCmdIL.c,v $
 * Revision 1.1.10.3  1996/02/18  19:18:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:09  marty]
 *
 * Revision 1.1.10.2  1995/12/08  21:34:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  20:25 UTC  dnguyen
 * 	Merge cleaning up of I18N-commented code
 * 
 * 	HP revision /main/dnguyen_mothra/1  1995/04/11  16:54 UTC  dnguyen
 * 	Clean up I18N comments
 * 	[1995/12/08  18:05:32  root]
 * 
 * Revision 1.1.6.1  1994/10/21  21:38:56  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:30:47  mori_m]
 * 
 * Revision 1.1.2.3  1994/09/23  20:26:48  mori_m
 * 	CR 11259:  One message was forgotten to be separated into a message catalog.
 * 	[1994/09/23  20:24:11  mori_m]
 * 
 * Revision 1.1.2.2  1994/08/25  14:37:22  mori_m
 * 	CR 11876: Fixed typo in message symbolic name.
 * 	[1994/08/25  14:36:59  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:01  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:06:11  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclCmdIL.c --
 *
 *	This file contains the top-level command routines for most of
 *	the Tcl built-in commands whose names begin with the letters
 *	I through L.  It contains only commands in the generic core
 *	(i.e. those that don't depend much upon UNIX facilities).
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
#include "patchlevel.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * The variables below are used to implement the "lsort" command.
 * Unfortunately, this use of static variables prevents "lsort"
 * from being thread-safe, but there's no alternative given the
 * current implementation of qsort.  In a threaded environment
 * these variables should be made thread-local if possible, or else
 * "lsort" needs internal mutual exclusion.
 */

static Tcl_Interp *sortInterp;		/* Interpreter for "lsort" command. */
static enum {ASCII, INTEGER, REAL, COMMAND} sortMode;
					/* Mode for sorting: compare as strings,
					 * compare as numbers, or call
					 * user-defined command for
					 * comparison. */
static Tcl_DString sortCmd;		/* Holds command if mode is COMMAND.
					 * pre-initialized to hold base of
					 * command. */
static int sortIncreasing;		/* 0 means sort in decreasing order,
					 * 1 means increasing order. */
static int sortCode;			/* Anything other than TCL_OK means a
					 * problem occurred while sorting; this
					 * executing a comparison command, so
					 * the sort was aborted. */

/*
 * Forward declarations for procedures defined in this file:
 */

static int		SortCompareProc _ANSI_ARGS_((CONST VOID *first,
			    CONST VOID *second));

/*
 *----------------------------------------------------------------------
 *
 * Tcl_IfCmd --
 *
 *	This procedure is invoked to process the "if" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_IfCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int i, result, value;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    i = 1;
    while (1) {
	/*
	 * At this point in the loop, argv and argc refer to an expression
	 * to test, either for the main expression or an expression
	 * following an "elseif".  The arguments after the expression must
	 * be "then" (optional) and a script to execute if the expression is
	 * true.
	 */

	if (i >= argc) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_no_expression_after);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[i-1])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[i-1]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	result = Tcl_ExprBoolean(interp, argv[i], &value);
	if (result != TCL_OK) {
	    return result;
	}
	i++;
	if ((i < argc) && (strcmp(argv[i], "then") == 0)) {
	    i++;
	}
	if (i >= argc) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_no_scrip_following);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[i-1])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[i-1]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (value) {
	    return Tcl_Eval(interp, argv[i]);
	}

	/*
	 * The expression evaluated to false.  Skip the command, then
	 * see if there is an "else" or "elseif" clause.
	 */

	i++;
	if (i >= argc) {
	    return TCL_OK;
	}
	if ((argv[i][0] == 'e') && (strcmp(argv[i], "elseif") == 0)) {
	    i++;
	    continue;
	}
	break;
    }

    /*
     * Couldn't find a "then" or "elseif" clause to execute.  Check now
     * for an "else" clause.  We know that there's at least one more
     * argument when we get here.
     */

    if (strcmp(argv[i], "else") == 0) {
	i++;
	if (i >= argc) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_no_else);
	    Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
	    free(i18n_msg_format);

	    return TCL_ERROR;
	}
    }
    return Tcl_Eval(interp, argv[i]);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_IncrCmd --
 *
 *	This procedure is invoked to process the "incr" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
Tcl_IncrCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int value;
    char *oldString, *result;
    char newString[30];
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((argc != 2) && (argc != 3)) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_varname_incr);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    oldString = Tcl_GetVar(interp, argv[1], TCL_LEAVE_ERR_MSG);
    if (oldString == NULL) {
	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, oldString, &value) != TCL_OK) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_reading_value_of_incr);
	Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
	free(i18n_msg_format);

	return TCL_ERROR;
    }
    if (argc == 2) {
	value += 1;
    } else {
	int increment;

	if (Tcl_GetInt(interp, argv[2], &increment) != TCL_OK) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_reading_incr);
	    Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
	    free(i18n_msg_format);

	    return TCL_ERROR;
	}
	value += increment;
    }
    sprintf(newString, "%d", value);
    result = Tcl_SetVar(interp, argv[1], newString, TCL_LEAVE_ERR_MSG);
    if (result == NULL) {
	return TCL_ERROR;
    }
    interp->result = result;
    return TCL_OK; 
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_InfoCmd --
 *
 *	This procedure is invoked to process the "info" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_InfoCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int length;
    char c;
    Arg *argPtr;
    Proc *procPtr;
    Var *varPtr;
    Command *cmdPtr;
    Tcl_HashEntry *hPtr;
    Tcl_HashSearch search;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 2) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_opt_arg_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    c = argv[1][0];
    length = strlen(argv[1]);
    if ((c == 'a') && (strncmp(argv[1], "args", length)) == 0) {
	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_args_procname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	procPtr = TclFindProc(iPtr, argv[2]);
	if (procPtr == NULL) {
	    infoNoSuchProc:
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_is_not_a_proc);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (argPtr = procPtr->argPtr; argPtr != NULL;
		argPtr = argPtr->nextPtr) {
	    Tcl_AppendElement(interp, argPtr->name);
	}
	return TCL_OK;
    } else if ((c == 'b') && (strncmp(argv[1], "body", length)) == 0) {
	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_body_procname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	procPtr = TclFindProc(iPtr, argv[2]);
	if (procPtr == NULL) {
	    goto infoNoSuchProc;
	}
	iPtr->result = procPtr->command;
	return TCL_OK;
    } else if ((c == 'c') && (strncmp(argv[1], "cmdcount", length) == 0)
	    && (length >= 2)) {
	if (argc != 2) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_cmdcount);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	sprintf(iPtr->result, "%d", iPtr->cmdCount);
	return TCL_OK;
    } else if ((c == 'c') && (strncmp(argv[1], "commands", length) == 0)
	    && (length >= 4)) {
	if (argc > 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_cmd_pattern);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->commandTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    char *name = Tcl_GetHashKey(&iPtr->commandTable, hPtr);
	    if ((argc == 3) && !Tcl_StringMatch(name, argv[2])) {
		continue;
	    }
	    Tcl_AppendElement(interp, name);
	}
	return TCL_OK;
    } else if ((c == 'c') && (strncmp(argv[1], "complete", length) == 0)
	    && (length >= 4)) {
	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_complete_cmd);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (Tcl_CommandComplete(argv[2])) {
	    interp->result = "1";
	} else {
	    interp->result = "0";
	}
	return TCL_OK;
    } else if ((c == 'd') && (strncmp(argv[1], "default", length)) == 0) {
	if (argc != 5) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_def_proc_arg_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	procPtr = TclFindProc(iPtr, argv[2]);
	if (procPtr == NULL) {
	    goto infoNoSuchProc;
	}
	for (argPtr = procPtr->argPtr; ; argPtr = argPtr->nextPtr) {
	    if (argPtr == NULL) {
		i18n_msg_format = dce_msg_get(tcl_s_cmdil_proc_no_argument);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen(argv[3])
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2], argv[3]);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);

		return TCL_ERROR;
	    }
	    if (strcmp(argv[3], argPtr->name) == 0) {
		if (argPtr->defValue != NULL) {
		    if (Tcl_SetVar((Tcl_Interp *) iPtr, argv[4],
			    argPtr->defValue, 0) == NULL) {
			defStoreError:
			i18n_msg_format = dce_msg_get(tcl_s_cmdil_could_not_store_def_val);
			i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[4])
						+ strlen((char *)i18n_msg_format) + 1);
			sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[4]);
			Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
			free(i18n_msg_format);
			ckfree(i18n_msg_buf);

			return TCL_ERROR;
		    }
		    iPtr->result = "1";
		} else {
		    if (Tcl_SetVar((Tcl_Interp *) iPtr, argv[4], "", 0)
			    == NULL) {
			goto defStoreError;
		    }
		    iPtr->result = "0";
		}
		return TCL_OK;
	    }
	}
    } else if ((c == 'e') && (strncmp(argv[1], "exists", length) == 0)) {
	char *p;
	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_exists_varname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	p = Tcl_GetVar((Tcl_Interp *) iPtr, argv[2], 0);

	/*
	 * The code below handles the special case where the name is for
	 * an array:  Tcl_GetVar will reject this since you can't read
	 * an array variable without an index.
	 */

	if (p == NULL) {
	    Tcl_HashEntry *hPtr;
	    Var *varPtr;

	    if (strchr(argv[2], '(') != NULL) {
		noVar:
		iPtr->result = "0";
		return TCL_OK;
	    }
	    if (iPtr->varFramePtr == NULL) {
		hPtr = Tcl_FindHashEntry(&iPtr->globalTable, argv[2]);
	    } else {
		hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, argv[2]);
	    }
	    if (hPtr == NULL) {
		goto noVar;
	    }
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & VAR_UPVAR) {
		varPtr = varPtr->value.upvarPtr;
	    }
	    if (!(varPtr->flags & VAR_ARRAY)) {
		goto noVar;
	    }
	}
	iPtr->result = "1";
	return TCL_OK;
    } else if ((c == 'g') && (strncmp(argv[1], "globals", length) == 0)) {
	char *name;

	if (argc > 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_globals_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->globalTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & VAR_UNDEFINED) {
		continue;
	    }
	    name = Tcl_GetHashKey(&iPtr->globalTable, hPtr);
	    if ((argc == 3) && !Tcl_StringMatch(name, argv[2])) {
		continue;
	    }
	    Tcl_AppendElement(interp, name);
	}
	return TCL_OK;
    } else if ((c == 'l') && (strncmp(argv[1], "level", length) == 0)
	    && (length >= 2)) {
	if (argc == 2) {
	    if (iPtr->varFramePtr == NULL) {
		iPtr->result = "0";
	    } else {
		sprintf(iPtr->result, "%d", iPtr->varFramePtr->level);
	    }
	    return TCL_OK;
	} else if (argc == 3) {
	    int level;
	    CallFrame *framePtr;

	    if (Tcl_GetInt(interp, argv[2], &level) != TCL_OK) {
		return TCL_ERROR;
	    }
	    if (level <= 0) {
		if (iPtr->varFramePtr == NULL) {
		    levelError:
		    i18n_msg_format = dce_msg_get(tcl_s_cmdil_bad_level);
		    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen((char *)i18n_msg_format) + 1);
		    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2]);
		    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		    free(i18n_msg_format);
		    ckfree(i18n_msg_buf);

		    return TCL_ERROR;
		}
		level += iPtr->varFramePtr->level;
	    }
	    for (framePtr = iPtr->varFramePtr; framePtr != NULL;
		    framePtr = framePtr->callerVarPtr) {
		if (framePtr->level == level) {
		    break;
		}
	    }
	    if (framePtr == NULL) {
		goto levelError;
	    }
	    iPtr->result = Tcl_Merge(framePtr->argc, framePtr->argv);
	    iPtr->freeProc = (Tcl_FreeProc *) free;
	    return TCL_OK;
	}
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_level_num);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    } else if ((c == 'l') && (strncmp(argv[1], "library", length) == 0)
	    && (length >= 2)) {
	if (argc != 2) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_library);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	interp->result = getenv("TCL_LIBRARY");
	if (interp->result == NULL) {
#ifdef TCL_LIBRARY
	    interp->result = TCL_LIBRARY;
#else
	    interp->result = dce_msg_get(tcl_s_cmdil_no_tcl_library);
	    interp->freeProc = (Tcl_FreeProc *) free;
	    return TCL_ERROR;
#endif
	}
	return TCL_OK;
    } else if ((c == 'l') && (strncmp(argv[1], "locals", length) == 0)
	    && (length >= 2)) {
	char *name;

	if (argc > 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_locals_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (iPtr->varFramePtr == NULL) {
	    return TCL_OK;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->varFramePtr->varTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR)) {
		continue;
	    }
	    name = Tcl_GetHashKey(&iPtr->varFramePtr->varTable, hPtr);
	    if ((argc == 3) && !Tcl_StringMatch(name, argv[2])) {
		continue;
	    }
	    Tcl_AppendElement(interp, name);
	}
	return TCL_OK;
    } else if ((c == 'p') && (strncmp(argv[1], "patchlevel", length) == 0)
	    && (length >= 2)) {
	if (argc != 2) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_patchlevel);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	sprintf(interp->result, "%d", TCL_PATCH_LEVEL);
	return TCL_OK;
    } else if ((c == 'p') && (strncmp(argv[1], "procs", length) == 0)
	    && (length >= 2)) {
	if (argc > 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_procs_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->commandTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    char *name = Tcl_GetHashKey(&iPtr->commandTable, hPtr);

	    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
	    if (!TclIsProc(cmdPtr)) {
		continue;
	    }
	    if ((argc == 3) && !Tcl_StringMatch(name, argv[2])) {
		continue;
	    }
	    Tcl_AppendElement(interp, name);
	}
	return TCL_OK;
    } else if ((c == 's') && (strncmp(argv[1], "script", length) == 0)) {
	if (argc != 2) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_script);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (iPtr->scriptFile != NULL) {
	    /*
	     * Can't depend on iPtr->scriptFile to be non-volatile:
	     * if this command is returned as the result of the script,
	     * then iPtr->scriptFile will go away.
	     */

	    Tcl_SetResult(interp, iPtr->scriptFile, TCL_VOLATILE);
	}
	return TCL_OK;
    } else if ((c == 't') && (strncmp(argv[1], "tclversion", length) == 0)) {
	if (argc != 2) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_tclversion);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}

	/*
	 * Note:  TCL_VERSION below is expected to be set with a "-D"
	 * switch in the Makefile.
	 */

	strcpy(iPtr->result, TCL_VERSION);
	return TCL_OK;
    } else if ((c == 'v') && (strncmp(argv[1], "vars", length)) == 0) {
	Tcl_HashTable *tablePtr;
	char *name;

	if (argc > 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_vars_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (iPtr->varFramePtr == NULL) {
	    tablePtr = &iPtr->globalTable;
	} else {
	    tablePtr = &iPtr->varFramePtr->varTable;
	}
	for (hPtr = Tcl_FirstHashEntry(tablePtr, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & VAR_UNDEFINED) {
		continue;
	    }
	    name = Tcl_GetHashKey(tablePtr, hPtr);
	    if ((argc == 3) && !Tcl_StringMatch(name, argv[2])) {
		continue;
	    }
	    Tcl_AppendElement(interp, name);
	}
	return TCL_OK;
    } else {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_info_bad_option);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_JoinCmd --
 *
 *	This procedure is invoked to process the "join" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_JoinCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char *joinString;
    char **listArgv;
    int listArgc, i;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc == 2) {
	joinString = " ";
    } else if (argc == 3) {
	joinString = argv[2];
    } else {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_joinstr);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if (Tcl_SplitList(interp, argv[1], &listArgc, &listArgv) != TCL_OK) {
	return TCL_ERROR;
    }
    for (i = 0; i < listArgc; i++) {
	if (i == 0) {
	    Tcl_AppendResult(interp, listArgv[0], (char *) NULL);
	} else  {
	    Tcl_AppendResult(interp, joinString, listArgv[i], (char *) NULL);
	}
    }
    ckfree((char *) listArgv);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LindexCmd --
 *
 *	This procedure is invoked to process the "lindex" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
Tcl_LindexCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char *p, *element;
    int index, size, parenthesized, result;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc != 3) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_index);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[2], &index) != TCL_OK) {
	return TCL_ERROR;
    }
    if (index < 0) {
	return TCL_OK;
    }
    for (p = argv[1] ; index >= 0; index--) {
	result = TclFindElement(interp, p, &element, &p, &size,
		&parenthesized);
	if (result != TCL_OK) {
	    return result;
	}
    }
    if (size == 0) {
	return TCL_OK;
    }
    if (size >= TCL_RESULT_SIZE) {
	interp->result = (char *) ckalloc((unsigned) size+1);
	interp->freeProc = (Tcl_FreeProc *) free;
    }
    if (parenthesized) {
	memcpy((VOID *) interp->result, (VOID *) element, size);
	interp->result[size] = 0;
    } else {
	TclCopyAndCollapse(size, element, interp->result);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LinsertCmd --
 *
 *	This procedure is invoked to process the "linsert" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LinsertCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char *p, *element, savedChar;
    int i, index, count, result, size;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 4) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_idx_ele);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[2], &index) != TCL_OK) {
	return TCL_ERROR;
    }

    /*
     * Skip over the first "index" elements of the list, then add
     * all of those elements to the result.
     */

    size = 0;
    element = argv[1];
    for (count = 0, p = argv[1]; (count < index) && (*p != 0); count++) {
	result = TclFindElement(interp, p, &element, &p, &size, (int *) NULL);
	if (result != TCL_OK) {
	    return result;
	}
    }
    if (*p == 0) {
	Tcl_AppendResult(interp, argv[1], (char *) NULL);
    } else {
	char *end;

	end = element+size;
	if (element != argv[1]) {
	    while ((*end != 0) && !isspace(UCHAR(*end))) {
		end++;
	    }
	}
	savedChar = *end;
	*end = 0;
	Tcl_AppendResult(interp, argv[1], (char *) NULL);
	*end = savedChar;
    }

    /*
     * Add the new list elements.
     */

    for (i = 3; i < argc; i++) {
	Tcl_AppendElement(interp, argv[i]);
    }

    /*
     * Append the remainder of the original list.
     */

    if (*p != 0) {
	Tcl_AppendResult(interp, " ", p, (char *) NULL);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ListCmd --
 *
 *	This procedure is invoked to process the "list" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ListCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    if (argc >= 2) {
	interp->result = Tcl_Merge(argc-1, argv+1);
	interp->freeProc = (Tcl_FreeProc *) free;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LlengthCmd --
 *
 *	This procedure is invoked to process the "llength" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LlengthCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int count, result;
    char *element, *p;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc != 2) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    for (count = 0, p = argv[1]; *p != 0 ; count++) {
	result = TclFindElement(interp, p, &element, &p, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) {
	    return result;
	}
	if (*element == 0) {
	    break;
	}
    }
    sprintf(interp->result, "%d", count);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LrangeCmd --
 *
 *	This procedure is invoked to process the "lrange" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LrangeCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int first, last, result;
    char *begin, *end, c, *dummy;
    int count;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc != 4) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_first_last);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[2], &first) != TCL_OK) {
	return TCL_ERROR;
    }
    if (first < 0) {
	first = 0;
    }
    if ((*argv[3] == 'e') && (strncmp(argv[3], "end", strlen(argv[3])) == 0)) {
	last = 1000000;
    } else {
	if (Tcl_GetInt(interp, argv[3], &last) != TCL_OK) {
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_expect_int_or_end);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[3])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[3]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
    }
    if (first > last) {
	return TCL_OK;
    }

    /*
     * Extract a range of fields.
     */

    for (count = 0, begin = argv[1]; count < first; count++) {
	result = TclFindElement(interp, begin, &dummy, &begin, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) {
	    return result;
	}
	if (*begin == 0) {
	    break;
	}
    }
    for (count = first, end = begin; (count <= last) && (*end != 0);
	    count++) {
	result = TclFindElement(interp, end, &dummy, &end, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) {
	    return result;
	}
    }

    /*
     * Chop off trailing spaces.
     */

    while (isspace(UCHAR(end[-1]))) {
	end--;
    }
    c = *end;
    *end = 0;
    Tcl_SetResult(interp, begin, TCL_VOLATILE);
    *end = c;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LreplaceCmd --
 *
 *	This procedure is invoked to process the "lreplace" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LreplaceCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    char *p1, *p2, *element, savedChar, *dummy;
    int i, first, last, count, result, size;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 4) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_fst_lst_ele);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_GetInt(interp, argv[2], &first) != TCL_OK) {
	return TCL_ERROR;
    }
    if (TclGetListIndex(interp, argv[3], &last) != TCL_OK) {
	return TCL_ERROR;
    }
    if (first < 0) {
	first = 0;
    }
    if (last < 0) {
	last = 0;
    }
    if (first > last) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_fst_idx_greater_than_second);
	Tcl_AppendResult(interp, i18n_msg_format, (char *) NULL);
	free(i18n_msg_format);

	return TCL_ERROR;
    }

    /*
     * Skip over the elements of the list before "first".
     */

    size = 0;
    element = argv[1];
    for (count = 0, p1 = argv[1]; (count < first) && (*p1 != 0); count++) {
	result = TclFindElement(interp, p1, &element, &p1, &size,
		(int *) NULL);
	if (result != TCL_OK) {
	    return result;
	}
    }
    if (*p1 == 0) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_list_do_not_contain_ele);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Skip over the elements of the list up through "last".
     */

    for (p2 = p1 ; (count <= last) && (*p2 != 0); count++) {
	result = TclFindElement(interp, p2, &dummy, &p2, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) {
	    return result;
	}
    }

    /*
     * Add the elements before "first" to the result.  Be sure to
     * include quote or brace characters that might terminate the
     * last of these elements.
     */

    p1 = element+size;
    if (element != argv[1]) {
	while ((*p1 != 0) && !isspace(UCHAR(*p1))) {
	    p1++;
	}
    }
    savedChar = *p1;
    *p1 = 0;
    Tcl_AppendResult(interp, argv[1], (char *) NULL);
    *p1 = savedChar;

    /*
     * Add the new list elements.
     */

    for (i = 4; i < argc; i++) {
	Tcl_AppendElement(interp, argv[i]);
    }

    /*
     * Append the remainder of the original list.
     */

    if (*p2 != 0) {
	if (*interp->result == 0) {
	    Tcl_SetResult(interp, p2, TCL_VOLATILE);
	} else {
	    Tcl_AppendResult(interp, " ", p2, (char *) NULL);
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LsearchCmd --
 *
 *	This procedure is invoked to process the "lsearch" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LsearchCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
#define EXACT	0
#define GLOB	1
#define REGEXP	2
    int listArgc;
    char **listArgv;
    int i, match, mode, index;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    mode = GLOB;
    if (argc == 4) {
	if (strcmp(argv[1], "-exact") == 0) {
	    mode = EXACT;
	} else if (strcmp(argv[1], "-glob") == 0) {
	    mode = GLOB;
	} else if (strcmp(argv[1], "-regexp") == 0) {
	    mode = REGEXP;
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_bad_search_mode);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
    } else if (argc != 3) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_mode_list_pat);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (Tcl_SplitList(interp, argv[argc-2], &listArgc, &listArgv) != TCL_OK) {
	return TCL_ERROR;
    }
    index = -1;
    for (i = 0; i < listArgc; i++) {
	match = 0;
	switch (mode) {
	    case EXACT:
		match = (strcmp(listArgv[i], argv[argc-1]) == 0);
		break;
	    case GLOB:
		match = Tcl_StringMatch(listArgv[i], argv[argc-1]);
		break;
	    case REGEXP:
		match = Tcl_RegExpMatch(interp, listArgv[i], argv[argc-1]);
		if (match < 0) {
		    ckfree((char *) listArgv);
		    return TCL_ERROR;
		}
		break;
	}
	if (match) {
	    index = i;
	    break;
	}
    }
    sprintf(interp->result, "%d", index);
    ckfree((char *) listArgv);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LsortCmd --
 *
 *	This procedure is invoked to process the "lsort" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LsortCmd(notUsed, interp, argc, argv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int listArgc, i, c, length;
    char **listArgv;
    char *command = NULL;		/* Initialization needed only to
					 * prevent compiler warning. */
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if (argc < 2) {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_ascii_int_real);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Parse arguments to set up the mode for the sort.
     */

    sortInterp = interp;
    sortMode = ASCII;
    sortIncreasing = 1;
    sortCode = TCL_OK;
    for (i = 1; i < argc-1; i++) {
	length = strlen(argv[i]);
	if (length < 2) {
	    badSwitch:
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_bad_switch_ascii_int_real);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[i])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[i]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	c = argv[i][1];
	if ((c == 'a') && (strncmp(argv[i], "-ascii", length) == 0)) {
	    sortMode = ASCII;
	} else if ((c == 'c') && (strncmp(argv[i], "-command", length) == 0)) {
	    if (i == argc-2) {
		i18n_msg_format = dce_msg_get(tcl_s_cmdil_command_not_with_comparison);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);
		return TCL_ERROR;
	    }
	    sortMode = COMMAND;
	    command = argv[i+1];
	    i++;
	} else if ((c == 'd')
		&& (strncmp(argv[i], "-decreasing", length) == 0)) {
	    sortIncreasing = 0;
	} else if ((c == 'i') && (length >= 4)
		&& (strncmp(argv[i], "-increasing", length) == 0)) {
	    sortIncreasing = 1;
	} else if ((c == 'i') && (length >= 4)
		&& (strncmp(argv[i], "-integer", length) == 0)) {
	    sortMode = INTEGER;
	} else if ((c == 'r')
		&& (strncmp(argv[i], "-real", length) == 0)) {
	    sortMode = REAL;
	} else {
	    goto badSwitch;
	}
    }
    if (sortMode == COMMAND) {
	Tcl_DStringInit(&sortCmd);
	Tcl_DStringAppend(&sortCmd, command, -1);
    }

    if (Tcl_SplitList(interp, argv[argc-1], &listArgc, &listArgv) != TCL_OK) {
	return TCL_ERROR;
    }
    qsort((VOID *) listArgv, listArgc, sizeof (char *), SortCompareProc);
    if (sortCode == TCL_OK) {
	Tcl_ResetResult(interp);
	interp->result = Tcl_Merge(listArgc, listArgv);
	interp->freeProc = (Tcl_FreeProc *) free;
    }
    if (sortMode == COMMAND) {
	Tcl_DStringFree(&sortCmd);
    }
    ckfree((char *) listArgv);
    return sortCode;
}

/*
 *----------------------------------------------------------------------
 *
 * SortCompareProc --
 *
 *	This procedure is invoked by qsort to determine the proper
 *	ordering between two elements.
 *
 * Results:
 *	< 0 means first is "smaller" than "second", > 0 means "first"
 *	is larger than "second", and 0 means they should be treated
 *	as equal.
 *
 * Side effects:
 *	None, unless a user-defined comparison command does something
 *	weird.
 *
 *----------------------------------------------------------------------
 */

static int
SortCompareProc(first, second)
    CONST VOID *first, *second;		/* Elements to be compared. */
{
    int order;
    char *firstString = *((char **) first);
    char *secondString = *((char **) second);
    unsigned char *i18n_msg_format;

    order = 0;
    if (sortCode != TCL_OK) {
	/*
	 * Once an error has occurred, skip any future comparisons
	 * so as to preserve the error message in sortInterp->result.
	 */

	return order;
    }
    if (sortMode == ASCII) {
	order = strcmp(firstString, secondString);
    } else if (sortMode == INTEGER) {
	int a, b;

	if ((Tcl_GetInt(sortInterp, firstString, &a) != TCL_OK)
		|| (Tcl_GetInt(sortInterp, secondString, &b) != TCL_OK)) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_conv_str_to_int);
	    Tcl_AddErrorInfo(sortInterp, (char *)i18n_msg_format);
	    free(i18n_msg_format);

	    sortCode = TCL_ERROR;
	    return order;
	}
	if (a > b) {
	    order = 1;
	} else if (b > a) {
	    order = -1;
	}
    } else if (sortMode == REAL) {
	double a, b;

	if ((Tcl_GetDouble(sortInterp, firstString, &a) != TCL_OK)
		|| (Tcl_GetDouble(sortInterp, secondString, &b) != TCL_OK)) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_conv_str_to_real);
	    Tcl_AddErrorInfo(sortInterp, (char *)i18n_msg_format);
	    free(i18n_msg_format);

	    sortCode = TCL_ERROR;
	    return order;
	}
	if (a > b) {
	    order = 1;
	} else if (b > a) {
	    order = -1;
	}
    } else {
	int oldLength;
	char *end;

	/*
	 * Generate and evaluate a command to determine which string comes
	 * first.
	 */

	oldLength = Tcl_DStringLength(&sortCmd);
	Tcl_DStringAppendElement(&sortCmd, firstString);
	Tcl_DStringAppendElement(&sortCmd, secondString);
	sortCode = Tcl_Eval(sortInterp, Tcl_DStringValue(&sortCmd));
	Tcl_DStringTrunc(&sortCmd, oldLength);
	if (sortCode != TCL_OK) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_user_def_comp_cmd);
	    Tcl_AddErrorInfo(sortInterp, (char *)i18n_msg_format);
	    free(i18n_msg_format);

	    return order;
	}

	/*
	 * Parse the result of the command.
	 */

	order = strtol(sortInterp->result, &end, 0);
	if ((end == sortInterp->result) || (*end != 0)) {
	    Tcl_ResetResult(sortInterp);
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_com_cmd_ret_non_numeric);
	    Tcl_AppendResult(sortInterp, (char *)i18n_msg_format, (char *) NULL);
	    free(i18n_msg_format);

	    sortCode = TCL_ERROR;
	    return order;
	}
    }
    if (!sortIncreasing) {
	order = -order;
    }
    return order;
}
