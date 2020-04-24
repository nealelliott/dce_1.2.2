/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tclWCmdIL.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.4.1  1995/02/17  15:56:03  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/17  15:51:56  levy]
 *
 * Revision 1.1.2.1  1994/10/21  21:39:42  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:35:00  mori_m]
 * 
 * $EndLog$
 */
/* 
 * _tclWCmdIL.c --
 *
 *	This file contains the top-level command routines for most of
 *	the Tcl built-in commands whose names begin with the letters
 *	I through L.  It contains only commands in the generic core
 *	(i.e. those that don't depend much upon UNIX facilities).
 */

#include "tclInt.h"
#include "tclWInt.h"
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
static enum {ASCII, INTEGER, REAL, COMMAND, LOCALE} sortMode;
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
 *
 */
int             _WSortCompareProc _ANSI_ARGS_((CONST VOID *first,
                            CONST VOID *second));

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WIfCmd --
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
_Tcl_WIfCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int i, result, value;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    i = 1;
    while (1) 
    {
	/*
	 * At this point in the loop, argv and argc refer to an expression
	 * to test, either for the main expression or an expression
	 * following an "elseif".  The arguments after the expression must
	 * be "then" (optional) and a script to execute if the expression is
	 * true.
	 */

	if (i >= argc) 
	{
	    _WChar_To_Multibyte(interp, clen, wargv[i-1], ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_no_expression_after);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	result = _Tcl_WExprBoolean(interp, wargv[i], &value);
	if (result != TCL_OK) 
	{
	    return result;
	}
	i++;
	_WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if ((i < argc) && (strcmp(ctemp, "then") == 0)) 
	{
	    i++;
	}
	ckfree(ctemp);

	if (i >= argc) 
	{
	    _WChar_To_Multibyte(interp, clen, wargv[i-1], ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_no_scrip_following);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	if (value) 
	{
	    return _Tcl_WEval(interp, wargv[i]);
	}

	/*
	 * The expression evaluated to false.  Skip the command, then
	 * see if there is an "else" or "elseif" clause.
	 */

	i++;
	if (i >= argc) 
	{
	    return TCL_OK;
	}
	_WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if ((*ctemp == 'e') && (strcmp(ctemp, "elseif") == 0)) 
	{
	    i++;
	    ckfree(ctemp);
	    continue;
	}
	ckfree(ctemp);
	break;
    }

    /*
     * Couldn't find a "then" or "elseif" clause to execute.  Check now
     * for an "else" clause.  We know that there's at least one more
     * argument when we get here.
     */

    _WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (strcmp(ctemp, "else") == 0) 
    {
	i++;
	if (i >= argc) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_no_else);
	    Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
    }
    ckfree(ctemp);
    return _Tcl_WEval(interp, wargv[i]);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WIncrCmd --
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
_Tcl_WIncrCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int value;
    char *oldString, *result;
    char newString[30];
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    wchar_t *wtemp;
    size_t wlen, st;

    if ((argc != 2) && (argc != 3)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_varname_incr);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("incr")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "incr");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    oldString = _Tcl_WGetVar(interp, wargv[1], TCL_LEAVE_ERR_MSG);
    if (oldString == NULL) 
    {
	return TCL_ERROR;
    }
    _Multibyte_to_WChar(interp, wlen, oldString, wtemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (_Tcl_WGetInt(interp, wtemp, &value) != TCL_OK) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_reading_value_of_incr);
	Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
	free(i18n_msg_format);
	ckfree(wtemp);

	return TCL_ERROR;
    }
    ckfree(wtemp);

    if (argc == 2) 
    {
	value += 1;
    } 
    else 
    {
	int increment;

	if (_Tcl_WGetInt(interp, wargv[2], &increment) != TCL_OK) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_reading_incr);
	    Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
	    free(i18n_msg_format);

	    return TCL_ERROR;
	}
	value += increment;
    }
    sprintf(newString, "%d", value);

    _Multibyte_to_WChar(interp, wlen, newString, wtemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    result = _Tcl_WSetVar(interp, wargv[1], wtemp, TCL_LEAVE_ERR_MSG);
    if (result == NULL) 
    {
	ckfree(wtemp);
	return TCL_ERROR;
    }
    interp->result = result;
    ckfree(wtemp);
    return TCL_OK; 
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WInfoCmd --
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
_Tcl_WInfoCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int length, conv_err;
    char c;
    Arg *argPtr;
    Proc *procPtr;
    Var *varPtr;
    Command *cmdPtr;
    Tcl_HashEntry *hPtr;
    Tcl_HashSearch search;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *cargv1, *cargv2, *cargv3, *cargv4;
    wchar_t *wtemp;
    char *ctemp;
    size_t len, st;

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_opt_arg_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    _WChar_To_Multibyte(interp, len, wargv[1], cargv1, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    c = *cargv1;
    length = strlen(cargv1);

    if ((c == 'a') && (strncmp(cargv1, "args", length)) == 0) 
    {
	ckfree(cargv1);

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_args_procname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	procPtr = TclFindProc(iPtr, cargv2);
	if (procPtr == NULL) 
	{
	    infoNoSuchProc:
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_is_not_a_proc);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv2);

	    return TCL_ERROR;
	}
	for (argPtr = procPtr->argPtr; argPtr != NULL;
		argPtr = argPtr->nextPtr) 
	{
	    _Multibyte_to_WChar(interp, len, argPtr->name, wtemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if (_Tcl_WAppendElement(interp, wtemp) == TCL_CONV_ERROR)
	    {
	    	ckfree(wtemp);
		return TCL_ERROR;
	    }
	    else
	    	ckfree(wtemp);
	}
	return TCL_OK;
    } 
    else if ((c == 'b') && (strncmp(cargv1, "body", length)) == 0) 
    {
	ckfree(cargv1);

	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_body_procname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	procPtr = TclFindProc(iPtr, cargv2);
	if (procPtr == NULL) 
	{
	    goto infoNoSuchProc;
	}
	ckfree(cargv2);
	iPtr->result = procPtr->command;
	return TCL_OK;
    } 
    else if ((c == 'c') && (strncmp(cargv1, "cmdcount", length) == 0)
	    && (length >= 2)) 
    {
	ckfree(cargv1);

	if (argc != 2) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_cmdcount);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	sprintf(iPtr->result, "%d", iPtr->cmdCount);
	return TCL_OK;
    } 
    else if ((c == 'c') && (strncmp(cargv1, "commands", length) == 0)
	    && (length >= 4)) 
    {
	ckfree(cargv1);

	if (argc > 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_cmd_pattern);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->commandTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    char *name = Tcl_GetHashKey(&iPtr->commandTable, hPtr);

	    _Multibyte_to_WChar(interp, len, name, wtemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if ((argc == 3) && !_Tcl_WStringMatch(interp, wtemp, wargv[2])) 
	    {
		ckfree(wtemp);
		continue;
	    }
	    if (_Tcl_WAppendElement(interp, wtemp) == TCL_CONV_ERROR)
	    {
		ckfree(wtemp);
		return TCL_ERROR;
	    }
	    ckfree(wtemp);
	}
	return TCL_OK;
    } 
    else if ((c == 'c') && (strncmp(cargv1, "complete", length) == 0)
	    && (length >= 4)) 
    {
	ckfree(cargv1);

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_complete_cmd);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	if (_Tcl_WCommandComplete(interp, ctemp)) 
	{
	    interp->result = "1";
	} 
	else 
	{
	    interp->result = "0";
	}
	ckfree(ctemp);
	return TCL_OK;
    } 
    else if ((c == 'd') && (strncmp(cargv1, "default", length)) == 0) 
    {
	ckfree(cargv1);

	if (argc != 5) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_def_proc_arg_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	procPtr = TclFindProc(iPtr, cargv2);
	if (procPtr == NULL) 
	{
	    goto infoNoSuchProc;
	}

	_WChar_To_Multibyte(interp, len, wargv[3], cargv3, st)
	if (st == (size_t)-1)
	{
	    ckfree(cargv2);
	    return TCL_ERROR;
	}

	for (argPtr = procPtr->argPtr; ; argPtr = argPtr->nextPtr) 
	{
	    if (argPtr == NULL) 
	    {
		i18n_msg_format = dce_msg_get(tcl_s_cmdil_proc_no_argument);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen(cargv3)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2, cargv3);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		ckfree(cargv2);
		ckfree(cargv3);

		return TCL_ERROR;
	    }
	    if (strcmp(cargv3, argPtr->name) == 0) 
	    {
		if (argPtr->defValue != NULL) 
		{
		    _Multibyte_to_WChar(interp, len, argPtr->defValue, wtemp, st)
		    if (st == (size_t)-1)
		    {
			ckfree(cargv2);
			ckfree(cargv3);
			return TCL_ERROR;
		    }
		    if (_Tcl_WSetVar((Tcl_Interp *) iPtr, wargv[4],
			    wtemp, 0) == NULL) 
		    {
			defStoreError:
			_WChar_To_Multibyte(interp, len, wargv[4], cargv4, st)
		    	if (st == (size_t)-1)
		    	{
				ckfree(cargv2);
				ckfree(cargv3);
				return TCL_ERROR;
			}

			i18n_msg_format = dce_msg_get(tcl_s_cmdil_could_not_store_def_val);
			i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv4)
						+ strlen((char *)i18n_msg_format) + 1);
			sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv4);
			Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
			free(i18n_msg_format);
			ckfree(i18n_msg_buf);
			ckfree(cargv2);
			ckfree(cargv3);
			ckfree(cargv4);

			return TCL_ERROR;
		    }
		    iPtr->result = "1";
		} 
		else 
		{
		    if (_Tcl_WSetVar((Tcl_Interp *) iPtr, wargv[4], 
			(wchar_t *)"", 0) == NULL) 
		    {
			goto defStoreError;
		    }
		    iPtr->result = "0";
		}
		ckfree(cargv2);
		ckfree(cargv3);
		return TCL_OK;
	    }
	    ckfree(cargv2);
	    ckfree(cargv3);
	}
    } 
    else if ((c == 'e') && (strncmp(cargv1, "exists", length) == 0)) 
    {
	char *p;

	ckfree(cargv1);

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_exists_varname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	p = _Tcl_WGetVar((Tcl_Interp *) iPtr, wargv[2], 0);

	/*
	 * The code below handles the special case where the name is for
	 * an array:  Tcl_GetVar will reject this since you can't read
	 * an array variable without an index.
	 */

	if (p == NULL) 
	{
	    Tcl_HashEntry *hPtr;
	    Var *varPtr;

	    if (wcschr(wargv[2], i18n_special_chars.w_op_paren) != NULL) 
	    {
		noVar:
		iPtr->result = "0";
		return TCL_OK;
	    }
	    _WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if (iPtr->varFramePtr == NULL) 
	    {
		hPtr = Tcl_FindHashEntry(&iPtr->globalTable, cargv2);
	    } 
	    else 
	    {
		hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, cargv2);
	    }
	    ckfree(cargv2);

	    if (hPtr == NULL) 
	    {
		goto noVar;
	    }
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & VAR_UPVAR) 
	    {
		varPtr = varPtr->value.upvarPtr;
	    }
	    if (!(varPtr->flags & VAR_ARRAY)) 
	    {
		goto noVar;
	    }
	}
	iPtr->result = "1";
	return TCL_OK;
    } 
    else if ((c == 'g') && (strncmp(cargv1, "globals", length) == 0)) 
    {
	char *name;

	ckfree(cargv1);

	if (argc > 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_globals_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->globalTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & VAR_UNDEFINED) 
	    {
		continue;
	    }
	    name = Tcl_GetHashKey(&iPtr->globalTable, hPtr);
	    _Multibyte_to_WChar(interp, len, name, wtemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if ((argc == 3) && !_Tcl_WStringMatch(interp, wtemp, wargv[2])) 
	    {
		ckfree(wtemp);
		continue;
	    }
	    if (_Tcl_WAppendElement(interp, wtemp) == TCL_CONV_ERROR)
	    {
		ckfree(wtemp);
		return TCL_ERROR;
	    }
	    ckfree(wtemp);
	}
	return TCL_OK;
    } 
    else if ((c == 'l') && (strncmp(cargv1, "level", length) == 0)
	    && (length >= 2)) 
    {
	ckfree(cargv1);

	if (argc == 2) 
	{
	    if (iPtr->varFramePtr == NULL) 
	    {
		iPtr->result = "0";
	    } 
	    else 
	    {
		sprintf(iPtr->result, "%d", iPtr->varFramePtr->level);
	    }
	    return TCL_OK;
	} 
	else if (argc == 3) 
	{
	    int level;
	    CallFrame *framePtr;

	    if (_Tcl_WGetInt(interp, wargv[2], &level) != TCL_OK) 
	    {
		return TCL_ERROR;
	    }
	    if (level <= 0) 
	    {
		if (iPtr->varFramePtr == NULL) 
		{
		    levelError:
		    _WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
		    if (st == (size_t)-1)
			return TCL_ERROR;

		    i18n_msg_format = dce_msg_get(tcl_s_cmdil_bad_level);
		    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen((char *)i18n_msg_format) + 1);
		    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2);
		    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		    free(i18n_msg_format);
		    ckfree(i18n_msg_buf);
		    ckfree(cargv2);

		    return TCL_ERROR;
		}
		level += iPtr->varFramePtr->level;
	    }
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
	    wtemp = _Tcl_WMerge(interp, framePtr->argc, framePtr->argv, &conv_err);
	    if (conv_err == TCL_CONV_ERROR)
		return TCL_ERROR;

	    _WChar_To_Multibyte(interp, len, wtemp, iPtr->result, st)
	    ckfree(wtemp);
	    if (st == (size_t)-1)
	    {
	        iPtr->freeProc = (Tcl_FreeProc *) free;
		return TCL_ERROR;
	    }
	    iPtr->freeProc = (Tcl_FreeProc *) free;
	    return TCL_OK;
	}
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_level_num);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    } 
    else if ((c == 'l') && (strncmp(cargv1, "library", length) == 0)
	    && (length >= 2)) 
    {
	ckfree(cargv1);

	if (argc != 2) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_library);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	interp->result = getenv("TCL_LIBRARY");
	if (interp->result == NULL) 
	{
#ifdef TCL_LIBRARY
	    interp->result = TCL_LIBRARY;
#else
	    interp->result = dce_msg_get(tcl_s_cmdil_no_tcl_library);
	    interp->freeProc = (Tcl_FreeProc *) free;
	    return TCL_ERROR;
#endif
	}
	return TCL_OK;
    } 
    else if ((c == 'l') && (strncmp(cargv1, "locals", length) == 0)
	    && (length >= 2)) 
    {
	char *name;

	ckfree(cargv1);

	if (argc > 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_locals_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (iPtr->varFramePtr == NULL) 
	{
	    return TCL_OK;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->varFramePtr->varTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR)) 
	    {
		continue;
	    }
	    name = Tcl_GetHashKey(&iPtr->varFramePtr->varTable, hPtr);
	    _Multibyte_to_WChar(interp, len, name, wtemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if ((argc == 3) && !_Tcl_WStringMatch(interp, wtemp, wargv[2])) 
	    {
		continue;
	    }
	    if (_Tcl_WAppendElement(interp, wtemp) == TCL_CONV_ERROR)
	    {
		ckfree(wtemp);
		return TCL_ERROR;
	    }
	    ckfree(wtemp);
	}
	return TCL_OK;
    } 
    else if ((c == 'p') && (strncmp(cargv1, "patchlevel", length) == 0)
	    && (length >= 2)) 
    {
	ckfree(cargv1);

	if (argc != 2) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_patchlevel);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	sprintf(interp->result, "%d", TCL_PATCH_LEVEL);
	return TCL_OK;
    } 
    else if ((c == 'p') && (strncmp(cargv1, "procs", length) == 0)
	    && (length >= 2)) 
    {
	if (argc > 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_procs_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(&iPtr->commandTable, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    char *name = Tcl_GetHashKey(&iPtr->commandTable, hPtr);

	    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
	    if (!TclIsProc(cmdPtr)) 
	    {
		continue;
	    }
	    _Multibyte_to_WChar(interp, len, name, wtemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if ((argc == 3) && !_Tcl_WStringMatch(interp, wtemp, wargv[2])) 
	    {
		ckfree(wtemp);
		continue;
	    }
	    if (_Tcl_WAppendElement(interp, wtemp) == TCL_CONV_ERROR)
	    {
		ckfree(wtemp);
		return TCL_ERROR;
	    }
	    ckfree(wtemp);
	}
	return TCL_OK;
    } 
    else if ((c == 's') && (strncmp(cargv1, "script", length) == 0)) 
    {
	ckfree(cargv1);

	if (argc != 2) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_script);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (iPtr->scriptFile != NULL) 
	{
	    /*
	     * Can't depend on iPtr->scriptFile to be non-volatile:
	     * if this command is returned as the result of the script,
	     * then iPtr->scriptFile will go away.
	     */

	    Tcl_SetResult(interp, iPtr->scriptFile, TCL_VOLATILE);
	}
	return TCL_OK;
    } 
    else if ((c == 't') && (strncmp(cargv1, "tclversion", length) == 0)) 
    {
	ckfree(cargv1);

	if (argc != 2) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_tclversion);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
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
    } 
    else if ((c == 'v') && (strncmp(cargv1, "vars", length)) == 0) 
    {
	Tcl_HashTable *tablePtr;
	char *name;

	ckfree(cargv1);

	if (argc > 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_vars_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("info")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "info");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	if (iPtr->varFramePtr == NULL) 
	{
	    tablePtr = &iPtr->globalTable;
	} 
	else 
	{
	    tablePtr = &iPtr->varFramePtr->varTable;
	}
	for (hPtr = Tcl_FirstHashEntry(tablePtr, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr->flags & VAR_UNDEFINED) 
	    {
		continue;
	    }
	    name = Tcl_GetHashKey(tablePtr, hPtr);
	    _Multibyte_to_WChar(interp, len, name, wtemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    if ((argc == 3) && !_Tcl_WStringMatch(interp, wtemp, wargv[2])) 
	    {
		ckfree(wtemp);
		continue;
	    }
	    if (_Tcl_WAppendElement(interp, wtemp) == TCL_CONV_ERROR)
	    {
		ckfree(wtemp);
		return TCL_ERROR;
	    }
	    ckfree(wtemp);
	}
	return TCL_OK;
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_info_bad_option);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv1)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv1);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(cargv1);

	return TCL_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WJoinCmd --
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
_Tcl_WJoinCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    char *joinString;
    wchar_t **wlistArgv;
    int listArgc, i;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    if (argc == 2) 
    {
	joinString = " ";
    } 
    else if (argc == 3) 
    {
	_WChar_To_Multibyte(interp, clen, wargv[2], joinString, st)
	if (st == (size_t)-1)
		return TCL_ERROR;
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_joinstr);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("join")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "join");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if (_Tcl_WSplitList(interp, wargv[1], &listArgc, &wlistArgv) != TCL_OK) 
    {
	ckfree(joinString);
	return TCL_ERROR;
    }
    for (i = 0; i < listArgc; i++) 
    {
	if (i == 0) 
	{
	    _WChar_To_Multibyte(interp, clen, wlistArgv[0], ctemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(joinString);
		return TCL_ERROR;
	    }
	    Tcl_AppendResult(interp, ctemp, (char *) NULL);
	    ckfree(ctemp);
	} 
	else  
	{
	    _WChar_To_Multibyte(interp, clen, wlistArgv[i], ctemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(joinString);
		return TCL_ERROR;
	    }
	    Tcl_AppendResult(interp, joinString, ctemp, (char *) NULL);
	    ckfree(ctemp);
	}
    }
    ckfree(joinString);
    ckfree((wchar_t *) wlistArgv);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLindexCmd --
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
_Tcl_WLindexCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    wchar_t *wp, *welement, *wstr;
    int index, size, parenthesized, result, i, msize;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    if (argc != 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_index);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("lindex")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "lindex");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (_Tcl_WGetInt(interp, wargv[2], &index) != TCL_OK) 
    {
	return TCL_ERROR;
    }
    if (index < 0) 
    {
	return TCL_OK;
    }
    for (wp = wargv[1] ; index >= 0; index--) 
    {
	result = _TclWFindElement(interp, wp, &welement, &wp, &size,
		&parenthesized);
	if (result != TCL_OK) 
	{
	    return result;
	}
    }
    if (size == 0) 
    {
	return TCL_OK;
    }

    /* 
     * Figure out the actual multibyte size 
     */
    wstr = (wchar_t *)ckalloc((size+1) * sizeof(wchar_t));
    for (i=0; i < size; i++)
	wstr[i] = welement[i];
    wstr[i] = (wchar_t)0;
    _WChar_To_Multibyte(interp, clen, wstr, ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    msize = clen;

    if (parenthesized) 
    {
	if (size >= TCL_RESULT_SIZE) 
	{
	    interp->result = (char *) ckalloc((unsigned) msize+1);
	    interp->freeProc = (Tcl_FreeProc *) free;
	}

	memcpy((VOID *) interp->result, (VOID *) ctemp, msize);
	interp->result[msize] = 0;
	ckfree(ctemp);
    } 
    else 
    {
	wchar_t *wtemp;

	wtemp = (wchar_t *)ckalloc((size + 1) * sizeof(wchar_t));
	_TclWCopyAndCollapse(interp, size, welement, wtemp);
	
	_WChar_To_Multibyte(interp, clen, wtemp, ctemp, st)
	ckfree(wtemp);
	msize = clen;

	if (st == (size_t)-1)
	{
	    return TCL_ERROR;
	}

	if (size >= TCL_RESULT_SIZE) 
	{
	    interp->result = (char *) ckalloc((unsigned) msize+1);
	    interp->freeProc = (Tcl_FreeProc *) free;
	}

	memcpy((VOID *) interp->result, (VOID *) ctemp, msize);
	interp->result[msize] = 0;
	ckfree(ctemp);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLinsertCmd --
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
_Tcl_WLinsertCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    wchar_t *wp, *welement, wsavedChar;
    int i, index, count, result, size;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    if (argc < 4) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_idx_ele);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("linsert")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "linsert");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (_Tcl_WGetInt(interp, wargv[2], &index) != TCL_OK) 
    {
	return TCL_ERROR;
    }

    /*
     * Skip over the first "index" elements of the list, then add
     * all of those elements to the result.
     */

    size = 0;
    welement = wargv[1];
    for (count = 0, wp = wargv[1]; (count < index) 
		&& (*wp != i18n_special_chars.w_esc_zero); count++) 
    {
	result = _TclWFindElement(interp, wp, &welement, &wp, &size, (int *) NULL);
	if (result != TCL_OK) 
	{
	    return result;
	}
    }
    if (*wp == i18n_special_chars.w_esc_zero) 
    {
	_WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	Tcl_AppendResult(interp, ctemp, (char *) NULL);
	ckfree(ctemp);
    } 
    else 
    {
	wchar_t *wend;

	wend = welement+size;
	if (welement != wargv[1]) 
	{
	    while ((*wend != i18n_special_chars.w_esc_zero) 
			&& !iswspace(*wend)) 
	    {
		wend++;
	    }
	}
	wsavedChar = *wend;
	*wend = i18n_special_chars.w_esc_zero;
	_WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	Tcl_AppendResult(interp, ctemp, (char *) NULL);
	ckfree(ctemp);
	*wend = wsavedChar;
    }

    /*
     * Add the new list elements.
     */

    for (i = 3; i < argc; i++) 
    {
	_Tcl_WAppendElement(interp, wargv[i]);
    }

    /*
     * Append the remainder of the original list.
     */

    if (*wp != i18n_special_chars.w_esc_zero) 
    {
	_WChar_To_Multibyte(interp, clen, wp, ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	Tcl_AppendResult(interp, " ", ctemp, (char *) NULL);
	ckfree(ctemp);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WListCmd --
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
_Tcl_WListCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    wchar_t *wtemp;
    char *ctemp;
    char **cargv;
    unsigned char *i18n_msg_format;
    size_t clen, st;
    int i, j, conv_err, ret_code;

    ret_code = TCL_OK;	
    j = 0;
    if (argc >= 2) 
    { 
	cargv = (char **)ckalloc(argc * sizeof(char *));
	j = 1;
	i = argc-1;
	while (i--)
	{
	    _WChar_To_Multibyte(interp, clen, wargv[j], ctemp, st)
	    if (st == (size_t)-1)
	    {
		ret_code = TCL_ERROR;
		goto conv_error;
	    }
	    cargv[j-1] = (char *)ckalloc(strlen(ctemp) + 1);
	    strcpy(cargv[j-1], ctemp);
	    ckfree(ctemp);
	    j++;
	}
	cargv[j-1] = (char *)ckalloc(1);
	*cargv[j-1] = '\0';
	wtemp = _Tcl_WMerge(interp, argc-1, cargv, &conv_err);
        if (conv_err == TCL_CONV_ERROR)
	{
	    ret_code = TCL_ERROR;
	    goto conv_error;
	}

	_WChar_To_Multibyte(interp, clen, wtemp, ctemp, st)
	ckfree(wtemp);
	if (st == (size_t)-1)
	{
	    ret_code = TCL_ERROR;
	    goto conv_error;
	}
	interp->result = ctemp;
	interp->freeProc = (Tcl_FreeProc *) free;
    }

    conv_error:
	if (j == 0)
	    return ret_code;

	j--;
	while (j)
 	    ckfree(cargv[j--]);

	ckfree((char *)cargv);
	return ret_code;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLlengthCmd --
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
_Tcl_WLlengthCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int count, result;
    wchar_t *welement, *wp;
    unsigned char *i18n_msg_buf, *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc != 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("llength")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "llength");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    for (count = 0, wp = wargv[1]; *wp != i18n_special_chars.w_esc_zero ; count++) 
    {
	result = _TclWFindElement(interp, wp, &welement, &wp, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) 
	{
	    return result;
	}
	if (*welement == i18n_special_chars.w_esc_zero) 
	{
	    break;
	}
    }
    sprintf(interp->result, "%d", count);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLrangeCmd --
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
_Tcl_WLrangeCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int first, last, result;
    wchar_t *wbegin, *wend, wc, *wdummy;
    int count;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc != 4) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_first_last);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("lrange")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "lrange");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (_Tcl_WGetInt(interp, wargv[2], &first) != TCL_OK) 
    {
	return TCL_ERROR;
    }
    if (first < 0) 
    {
	first = 0;
    }
    _WChar_To_Multibyte(interp, clen, wargv[3], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if ((*ctemp == 'e') && (strncmp(ctemp, "end", strlen(ctemp)) == 0)) 
    {
	last = 1000000;
    } 
    else 
    {
	if (_Tcl_WGetInt(interp, wargv[3], &last) != TCL_OK) 
	{
	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_expect_int_or_end);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
    }
    ckfree(ctemp);

    if (first > last) 
    {
	return TCL_OK;
    }

    /*
     * Extract a range of fields.
     */

    for (count = 0, wbegin = wargv[1]; count < first; count++) 
    {
	result = _TclWFindElement(interp, wbegin, &wdummy, &wbegin, 
		(int *) NULL, (int *) NULL);
	if (result != TCL_OK) 
	{
	    return result;
	}
	if (*wbegin == i18n_special_chars.w_esc_zero) 
	{
	    break;
	}
    }
    for (count = first, wend = wbegin; (count <= last) 
		&& (*wend != i18n_special_chars.w_esc_zero); count++) 
    {
	result = _TclWFindElement(interp, wend, &wdummy, &wend, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) 
	{
	    return result;
	}
    }

    /*
     * Chop off trailing spaces.
     */

    while (iswspace(wend[-1])) 
    {
	wend--;
    }
    wc = *wend;
    *wend = i18n_special_chars.w_esc_zero;
    _WChar_To_Multibyte(interp, clen, wbegin, ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    Tcl_SetResult(interp, ctemp, TCL_VOLATILE);
    *wend = wc;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLreplaceCmd --
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
_Tcl_WLreplaceCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    wchar_t *wp1, *wp2, *welement, wsavedChar, *wdummy;
    int i, first, last, count, result, size;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    if (argc < 4) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_list_fst_lst_ele);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("lreplace")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "lreplace");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (_Tcl_WGetInt(interp, wargv[2], &first) != TCL_OK) 
    {
	return TCL_ERROR;
    }
    if (_TclWGetListIndex(interp, wargv[3], &last) != TCL_OK) 
    {
	return TCL_ERROR;
    }
    if (first < 0) 
    {
	first = 0;
    }
    if (last < 0) 
    {
	last = 0;
    }
    if (first > last) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_fst_idx_greater_than_second);
	Tcl_AppendResult(interp, i18n_msg_format, (char *) NULL);
	free(i18n_msg_format);

	return TCL_ERROR;
    }

    /*
     * Skip over the elements of the list before "first".
     */

    size = 0;
    welement = wargv[1];
    for (count = 0, wp1 = wargv[1]; (count < first) 
		&& (*wp1 != i18n_special_chars.w_esc_zero); count++) 
    {
	result = _TclWFindElement(interp, wp1, &welement, &wp1, &size,
		(int *) NULL);
	if (result != TCL_OK) 
	{
	    return result;
	}
    }
    if (*wp1 == i18n_special_chars.w_esc_zero) 
    {
	_WChar_To_Multibyte(interp, clen, wargv[2], ctemp, st)
	if (st == (size_t)-1)
		return TCL_ERROR;

	i18n_msg_format = dce_msg_get(tcl_s_cmdil_list_do_not_contain_ele);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }

    /*
     * Skip over the elements of the list up through "last".
     */

    for (wp2 = wp1 ; (count <= last) 
		&& (*wp2 != i18n_special_chars.w_esc_zero); count++) 
    {
	result = _TclWFindElement(interp, wp2, &wdummy, &wp2, (int *) NULL,
		(int *) NULL);
	if (result != TCL_OK) 
	{
	    return result;
	}
    }

    /*
     * Add the elements before "first" to the result.  Be sure to
     * include quote or brace characters that might terminate the
     * last of these elements.
     */

    wp1 = welement+size;
    if (welement != wargv[1]) 
    {
	while ((*wp1 != i18n_special_chars.w_esc_zero) && !iswspace(*wp1)) 
	{
	    wp1++;
	}
    }
    wsavedChar = *wp1;
    *wp1 = i18n_special_chars.w_esc_zero;
    _WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    Tcl_AppendResult(interp, ctemp, (char *) NULL);
    ckfree(ctemp);
    *wp1 = wsavedChar;

    /*
     * Add the new list elements.
     */

    for (i = 4; i < argc; i++) 
    {
	_Tcl_WAppendElement(interp, wargv[i]);
    }

    /*
     * Append the remainder of the original list.
     */

    if (*wp2 != i18n_special_chars.w_esc_zero) 
    {
	if (*interp->result == 0) 
	{
	    _WChar_To_Multibyte(interp, clen, wp2, ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    Tcl_SetResult(interp, ctemp, TCL_VOLATILE);
	} 
	else 
	{
	    _WChar_To_Multibyte(interp, clen, wp2, ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;

	    Tcl_AppendResult(interp, " ", ctemp, (char *) NULL);
	    ckfree(ctemp);
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLsearchCmd --
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
_Tcl_WLsearchCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
#define EXACT	0
#define GLOB	1
#define REGEXP	2
    int listArgc;
    wchar_t **wlistArgv;
    int i, match, mode, index;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;

    _WChar_To_Multibyte(interp, clen, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    mode = GLOB;
    if (argc == 4) 
    {
	if (strcmp(ctemp, "-exact") == 0) 
	{
	    mode = EXACT;
	} 
	else if (strcmp(ctemp, "-glob") == 0) 
	{
	    mode = GLOB;
	} 
	else if (strcmp(ctemp, "-regexp") == 0) 
	{
	    mode = REGEXP;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_bad_search_mode);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
    } 
    else if (argc != 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_mode_list_pat);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("lsearch")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "lsearch");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    ckfree(ctemp);

    if (_Tcl_WSplitList(interp, wargv[argc-2], &listArgc, &wlistArgv) != TCL_OK)
    {
	return TCL_ERROR;
    }
    index = -1;
    for (i = 0; i < listArgc; i++) 
    {
	match = 0;
	switch (mode) 
	{
	    case EXACT:
		match = (wcscmp(wlistArgv[i], wargv[argc-1]) == 0);
		break;
	    case GLOB:
		match = _Tcl_WStringMatch(interp, wlistArgv[i], wargv[argc-1]);
		break;
	    case REGEXP:
		match = _Tcl_WRegExpMatch(interp, wlistArgv[i], wargv[argc-1]);
		if (match < 0) 
		{
		    ckfree((wchar_t *) wlistArgv);
		    return TCL_ERROR;
		}
		break;
	}
	if (match) 
	{
	    index = i;
	    break;
	}
    }
    sprintf(interp->result, "%d", index);
    ckfree((wchar_t *) wlistArgv);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLsortCmd --
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
_Tcl_WLsortCmd(notUsed, interp, argc, wargv)
    ClientData notUsed;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int listArgc, i, length;
    char c;
    wchar_t **wlistArgv;
    char *command = NULL;		/* Initialization needed only to
					 * prevent compiler warning. */
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t clen, st;
    char **cargv;
    int j = 0;

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdil_wnum_args_ascii_int_real);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("lsort")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "lsort");
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
    for (i = 1; i < argc-1; i++) 
    {
	_WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
	if (st == (size_t)-1)
		return TCL_ERROR;

	length = strlen(ctemp);
	if (length < 2) 
	{
	    badSwitch:
	    i18n_msg_format = dce_msg_get(tcl_s_cmdil_bad_switch_ascii_int_real);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	c = *(ctemp+1);
	if ((c == 'a') && (strncmp(ctemp, "-ascii", length) == 0)) 
	{
	    sortMode = ASCII;
	} 
	else if ((c == 'c') && (strncmp(ctemp, "-command", length) == 0)) 
	{
	    if (i == argc-2) 
	    {
		i18n_msg_format = dce_msg_get(tcl_s_cmdil_command_not_with_comparison);
		Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *) NULL);
		free(i18n_msg_format);
		ckfree(ctemp);
		return TCL_ERROR;
	    }
	    sortMode = COMMAND;
	    _WChar_To_Multibyte(interp, clen, wargv[i+1], command, st)
	    if (st == (size_t)-1)
	    {
		ckfree(ctemp);
		return TCL_ERROR;
	    }
	    i++;
	} 
	else if ((c == 'd')
		&& (strncmp(ctemp, "-decreasing", length) == 0)) 
	{
	    sortIncreasing = 0;
	} 
	else if ((c == 'i') && (length >= 4)
		&& (strncmp(ctemp, "-increasing", length) == 0)) 
	{
	    sortIncreasing = 1;
	} 
	else if ((c == 'i') && (length >= 4)
		&& (strncmp(ctemp, "-integer", length) == 0)) 
	{
	    sortMode = INTEGER;
	} 
	else if ((c == 'r')
		&& (strncmp(ctemp, "-real", length) == 0)) 
	{
	    sortMode = REAL;
	} 
	else if ((c == 'l') && (strncmp(ctemp, "-locale", length) == 0)) 
	{
	    sortMode = LOCALE;
	} 
	else 
	{
	    goto badSwitch;
	}
	ckfree(ctemp);
    }
    if (sortMode == COMMAND) 
    {
	Tcl_DStringInit(&sortCmd);
	Tcl_DStringAppend(&sortCmd, command, -1);
	ckfree(command);
    }

    if (_Tcl_WSplitList(interp, wargv[argc-1], &listArgc, &wlistArgv) != TCL_OK)
    {
	return TCL_ERROR;
    }

    cargv = (char **)ckalloc((listArgc + 1) * sizeof(char *));
    j = 0;
    i = listArgc;
    while (i--)
    {
	_WChar_To_Multibyte(interp, clen, wlistArgv[j], ctemp, st)
	if (st == (size_t)-1)
	{
		sortCode = TCL_ERROR;
		goto conv_error;
	}
	cargv[j] = (char *)ckalloc(strlen(ctemp) + 1);
	strcpy(cargv[j], ctemp);
	ckfree(ctemp);
	j++;
    }
    cargv[j] = (char *)NULL;

    qsort((VOID *) cargv, listArgc, sizeof (char *), _WSortCompareProc);

    if (sortCode == TCL_OK) 
    {
	wchar_t *wtemp;
	int	conv_err;

	Tcl_ResetResult(interp);
	wtemp = _Tcl_WMerge(interp, listArgc, cargv, &conv_err);
	if (conv_err == TCL_CONV_ERROR)
	{
	    sortCode = TCL_ERROR;
	    goto conv_error;
	}
	_WChar_To_Multibyte(interp, clen, wtemp, ctemp, st);
	ckfree(wtemp);
	if (st == (size_t)-1)
	{
	    sortCode = TCL_ERROR;
	    goto conv_error;
	}

	interp->result = ctemp;
	interp->freeProc = (Tcl_FreeProc *) free;
    }
    if (sortMode == COMMAND) 
    {
	Tcl_DStringFree(&sortCmd);
    }
    ckfree((wchar_t *) wlistArgv);

conv_error:
    if (j == 0)
	return sortCode;
    j--;
    while(j != 0)
    {
	ckfree(cargv[j--]);
    }
   
    ckfree((char *)cargv);
    return sortCode;
}

/*
 *----------------------------------------------------------------------
 *
 * _WSortCompareProc --
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

int
_WSortCompareProc(first, second)
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
    } else if (sortMode == LOCALE) {
	order = strcoll(firstString, secondString);
    } else if (sortMode == INTEGER) {
	int a, b;

	if ((Tcl_GetInt(sortInterp, firstString, &a) != TCL_OK)
		|| (Tcl_GetInt(sortInterp, secondString, &b) != TCL_OK)) {
	    /* i18n
	    Tcl_AddErrorInfo(sortInterp,
		    "\n    (converting list element from string to integer)");
	     */
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
	    /* i18n
	    Tcl_AddErrorInfo(sortInterp,
		    "\n    (converting list element from string to real)");
	     */
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
	    /* i18n
	    Tcl_AddErrorInfo(sortInterp,
		    "\n    (user-defined comparison command)");
	     */
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
	    /* i18n
	    Tcl_AppendResult(sortInterp,
		    "comparison command returned non-numeric result",
		    (char *) NULL);
	     */
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
