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
 * $Log: tclWCmdAH.c,v $
 * Revision 1.1.6.3  1996/02/18  19:19:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:03  marty]
 *
 * Revision 1.1.6.2  1995/12/08  21:34:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:06:37  root]
 * 
 * Revision 1.1.2.1  1994/10/21  21:39:39  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:34:47  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclWCmdAH.c --
 *
 *	This file contains the top-level command routines for most of
 *	the Tcl built-in commands whose names begin with the letters
 *	A to H.
 */

#include "tclInt.h"
#include "tclWInt.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>


/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WBreakCmd --
 *
 *	This procedure is invoked to process the "break" Tcl command.
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
_Tcl_WBreakCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 1) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("break")
                                + strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "break");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    return TCL_BREAK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WCaseCmd --
 *
 *	This procedure is invoked to process the "case" Tcl command.
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
_Tcl_WCaseCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int i, result, w_return;
    int body;
    wchar_t *wstring, *win, *wdefault;
    int caseArgc, splitArgs;
    wchar_t **wcaseArgv;
    unsigned char *i18n_msg_buf, *i18n_msg_format;

    size_t  wlen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_string_in_patList_body);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("case")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "case");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    wstring = wargv[1];
    body = -1;

    _Multibyte_to_WChar(interp, wlen, "in", win, st)
    if (st == (size_t)-1)
	return TCL_ERROR;;
    
    if (wcscmp(wargv[2], win) == 0) 
    {
	i = 3;
    } 
    else 
    {
	i = 2;
    }
    ckfree(win);
    caseArgc = argc - i;
    wcaseArgv = wargv + i;

    /*
     * If all of the pattern/command pairs are lumped into a single
     * argument, split them out again.
     */

    splitArgs = 0;
    if (caseArgc == 1) 
    {
	result = _Tcl_WSplitList(interp, wcaseArgv[0], &caseArgc, &wcaseArgv);
	if (result != TCL_OK) {
	    return result;
	}
	splitArgs = 1;
    }

    _Multibyte_to_WChar(interp, wlen, "default", wdefault, st)
    if (st == (size_t)-1)
    {
	result = TCL_ERROR;;
	goto cleanup;
    }

    for (i = 0; i < caseArgc; i += 2) 
    {
	int patArgc, j;
	wchar_t **wpatArgv;
	register wchar_t *wp;

	if (i == (caseArgc-1)) 
	{
	    interp->result = (char *)dce_msg_get(tcl_s_cmdah_extra_case_pattern_with_nobody);
	    interp->freeProc = (Tcl_FreeProc *) free;
	    result = TCL_ERROR;
	    goto cleanup;
	}

	/*
	 * Check for special case of single pattern (no list) with
	 * no backslash sequences.
	 */

	for (wp = wcaseArgv[i]; *wp != 0; wp++) 
	{
	    if (iswspace(*wp) || (*wp == i18n_special_chars.w_bsla)) 
	    {
		break;
	    }
	}
	if (*wp == i18n_special_chars.w_esc_zero) 
	{
	    if ((*wcaseArgv[i] == i18n_parse_chars.w_d)
		    && (wcscmp(wcaseArgv[i], wdefault) == 0)) 
	    {
		body = i+1;
	    }
	    w_return = _Tcl_WStringMatch(interp, wstring, wcaseArgv[i]);

	    if (w_return == TCL_CONV_ERROR) 
	    {
		    result = TCL_ERROR;;
		    goto cleanup;
	    }
	    else if (w_return == 1)		/* match found */
	    {
			body = i+1;
			goto match;
	    }
	    continue;
	}

	/*
	 * Break up pattern lists, then check each of the patterns
	 * in the list.
	 */

	result = _Tcl_WSplitList(interp, wcaseArgv[i], &patArgc, &wpatArgv);
	if (result != TCL_OK) {
	    goto cleanup;
	}
	for (j = 0; j < patArgc; j++) 
	{
		w_return = _Tcl_WStringMatch(interp, wstring, wpatArgv[j]);

		if (w_return == TCL_CONV_ERROR) 
		{
		    result = TCL_ERROR;;
		    goto cleanup;
		}
		if (w_return == 1) 
		{
			body = i+1;
			break;
		}
	}
	ckfree((wchar_t *)wpatArgv);
	if (j < patArgc) 
	{
	    break;
	}
    }

    match:
    if (body != -1) 
	{
	result = _Tcl_WEval(interp, wcaseArgv[body]);
	if (result == TCL_ERROR) {
	    char msg[200];
	    char *ctemp;

	    _WChar_To_Multibyte(interp, wlen, wcaseArgv[body-1], ctemp, st)
	    if (st == (size_t)-1)
		goto cleanup;

	    i18n_msg_format = dce_msg_get(tcl_s_cmdah_arm_line);
	    sprintf(msg, (char *)i18n_msg_format, ctemp, interp->errorLine);
	    free(i18n_msg_format);
	    ckfree(ctemp);
	    Tcl_AddErrorInfo(interp, msg);
	}
	goto cleanup;
    }

    /*
     * Nothing matched:  return nothing.
     */

    result = TCL_OK;

    cleanup:
    if (splitArgs) {
	ckfree((wchar_t *) wcaseArgv);
    }
    ckfree(wdefault);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WCatchCmd --
 *
 *	This procedure is invoked to process the "catch" Tcl command.
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
_Tcl_WCatchCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */

{
    int result;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((argc != 2) && (argc != 3)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_command_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("catch")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "catch");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    result = _Tcl_WEval(interp, wargv[1]);
    if (argc == 3) 
    {
	wchar_t	*wtemp;
	size_t wlen, st;

	_Multibyte_to_WChar(interp, wlen, interp->result, wtemp, st)
	if (st == (size_t)-1)
		return TCL_ERROR;;

	if (_Tcl_WSetVar(interp, wargv[2], wtemp, 0) == NULL) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdah_could_not_save_cmd_result);
	    Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
	    free(i18n_msg_format);
	    ckfree(wtemp);

	    return TCL_ERROR;
	}
	ckfree(wtemp);
    }
    Tcl_ResetResult(interp);
    sprintf(interp->result, "%d", result);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WConcatCmd --
 *
 *	This procedure is invoked to process the "concat" Tcl command.
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
_Tcl_WConcatCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    char *ctemp;
    wchar_t *wtemp;
    size_t len, st;
    unsigned char *i18n_msg_format;

    if (argc >= 2) 
    {
	wtemp = _Tcl_WConcat(interp, argc-1, wargv+1);
	_WChar_To_Multibyte(interp, len, wtemp, ctemp, st)
	if (st == (size_t)-1)
	{
	    ckfree(wtemp);
	    return TCL_ERROR;
	}
	ckfree(wtemp);
	interp->result = ctemp;
	interp->freeProc = (Tcl_FreeProc *) free;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WContinueCmd --
 *
 *	This procedure is invoked to process the "continue" Tcl command.
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
_Tcl_WContinueCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 1) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_continue);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("continue")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "continue");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    return TCL_CONTINUE;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WErrorCmd --
 *
 *	This procedure is invoked to process the "error" Tcl command.
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
_Tcl_WErrorCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    wchar_t *wtemp;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if ((argc < 2) || (argc > 4)) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_message_errinfo_errcode);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("error")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "error");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if ((argc >= 3) && (wargv[2][0] != i18n_special_chars.w_esc_zero)) 
    {
	_WChar_To_Multibyte(interp, len, wargv[2], ctemp, st)
	if (st == (size_t)-1)
		return TCL_ERROR;

	Tcl_AddErrorInfo(interp, ctemp);
	ckfree(ctemp);

	iPtr->flags |= ERR_ALREADY_LOGGED;
    }
    if (argc == 4) 
    {
	_Multibyte_to_WChar(interp, len, "errorCode", wtemp, st)
	if (st == (size_t)-1)
		return TCL_ERROR;

	_Tcl_WSetVar2(interp, wtemp, (wchar_t *) NULL, wargv[3],
		TCL_GLOBAL_ONLY);
	ckfree(wtemp);

	iPtr->flags |= ERROR_CODE_SET;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    Tcl_SetResult(interp, ctemp, TCL_VOLATILE);
    ckfree(ctemp);

    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WEvalCmd --
 *
 *	This procedure is invoked to process the "eval" Tcl command.
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
_Tcl_WEvalCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int result;
    wchar_t *wcmd;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_arg_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("eval")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "eval");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (argc == 2) 
    {
	result = _Tcl_WEval(interp, wargv[1]);
    } 
    else 
    {
    
	/*
	 * More than one argument:  concatenate them together with spaces
	 * between, then evaluate the result.
	 */
    
	wcmd = _Tcl_WConcat(interp, argc-1, wargv+1);
	result = _Tcl_WEval(interp, wcmd);
	ckfree(wcmd);
    }
    if (result == TCL_ERROR) 
    {
	char msg[60];
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_eval_body_line);
	sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
	free(i18n_msg_format);
	Tcl_AddErrorInfo(interp, msg);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WExprCmd --
 *
 *	This procedure is invoked to process the "expr" Tcl command.
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
_Tcl_WExprCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    Tcl_DString buffer;
    int i, result;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    wchar_t *wtemp;
    size_t len, st;

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_expr);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("expr")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "expr");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    if (argc == 2) 
    {
	return _Tcl_WExprString(interp, wargv[1]);
    }
    Tcl_DStringInit(&buffer);
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    Tcl_DStringAppend(&buffer, ctemp, -1);
    ckfree(ctemp);

    for (i = 2; i < argc; i++) {
	Tcl_DStringAppend(&buffer, " ", 1);
	_WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
	if (st == (size_t)-1)
		return TCL_ERROR;;
	Tcl_DStringAppend(&buffer, ctemp, -1);
	ckfree(ctemp);
    }
    _Multibyte_to_WChar(interp, len, buffer.string, wtemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    result = _Tcl_WExprString(interp, wtemp);
    ckfree(wtemp);
    Tcl_DStringFree(&buffer);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WForCmd --
 *
 *	This procedure is invoked to process the "for" Tcl command.
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
_Tcl_WForCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int result, value;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 5) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_start_test_next);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("for")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "for");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    result = _Tcl_WEval(interp, wargv[1]);
    if (result != TCL_OK) 
    {
	if (result == TCL_ERROR) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdah_for_initial_cmd);
	    Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
	return result;
    }
    while (1) 
    {
	result = _Tcl_WExprBoolean(interp, wargv[2], &value);
	if (result != TCL_OK) 
	{
	    return result;
	}
	if (!value) 
	{
	    break;
	}
	result = _Tcl_WEval(interp, wargv[4]);
	if ((result != TCL_OK) && (result != TCL_CONTINUE)) 
	{
	    if (result == TCL_ERROR) 
	    {
		char msg[120];
	    	i18n_msg_format = dce_msg_get(tcl_s_cmdah_for_body_line);
		sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
		free(i18n_msg_format);
		Tcl_AddErrorInfo(interp, msg);
	    }
	    break;
	}
	result = _Tcl_WEval(interp, wargv[3]);
	if (result == TCL_BREAK) 
	{
	    break;
	} 
	else if (result != TCL_OK) 
	{
	    if (result == TCL_ERROR) 
	    {
	    	i18n_msg_format = dce_msg_get(tcl_s_cmdah_for_loop_end_cmd);
		Tcl_AddErrorInfo(interp, (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	    return result;
	}
    }
    if (result == TCL_BREAK) 
    {
	result = TCL_OK;
    }
    if (result == TCL_OK) 
    {
	Tcl_ResetResult(interp);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WForeachCmd --
 *
 *	This procedure is invoked to process the "foreach" Tcl command.
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
_Tcl_WForeachCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int listArgc, i, result;
    wchar_t **wlistArgv;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc != 4) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_varname_list_com);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("foreach")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "foreach");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Break the list up into elements, and execute the command once
     * for each value of the element.
     */

    result = _Tcl_WSplitList(interp, wargv[2], &listArgc, &wlistArgv);
    if (result != TCL_OK) 
    {
	return result;
    }
    for (i = 0; i < listArgc; i++) 
    {
	if (_Tcl_WSetVar(interp, wargv[1], wlistArgv[i], 0) == NULL) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdah_could_not_set_loop_var);
	    Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
	    free(i18n_msg_format);
	    result = TCL_ERROR;
	    break;
	}

	result = _Tcl_WEval(interp, wargv[3]);
	if (result != TCL_OK) 
	{
	    if (result == TCL_CONTINUE) 
	    {
		result = TCL_OK;
	    } 
	    else if (result == TCL_BREAK) 
	    {
		result = TCL_OK;
		break;
	    } 
	    else if (result == TCL_ERROR) 
	    {
		char msg[200];
	    	i18n_msg_format = dce_msg_get(tcl_s_cmdah_foreach_body_line);
		sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
		free(i18n_msg_format);
		Tcl_AddErrorInfo(interp, msg);
		break;
	    } 
	    else 
	    {
		break;
	    }
	}
    }
    ckfree((wchar_t *) wlistArgv);
    if (result == TCL_OK) 
    {
	Tcl_ResetResult(interp);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WFormatCmd --
 *
 *	This procedure is invoked to process the "format" Tcl command.
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
_Tcl_WFormatCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register char *format, *format_save;
    char newFormat[40];		/* A new format specifier is generated here. */
    int width;			/* Field width from field specifier, or 0 if
				 * no width given. */
    int precision;		/* Field precision from field specifier, or 0
				 * if no precision given. */
    int size;			/* Number of bytes needed for result of
				 * conversion, based on type of conversion
				 * ("e", "s", etc.) and width from above. */
    char *oneWordValue = NULL;	/* Used to hold value to pass to sprintf, if
				 * it's a one-word value. */
    int oneWordFlag = 0;	/* Indicate oneWordValue needs free */
    double twoWordValue;	/* Used to hold value to pass to sprintf if
				 * it's a two-word value. */
    int useTwoWords;		/* 0 means use oneWordValue, 1 means use
				 * twoWordValue. */
    char *dst = interp->result;	/* Where result is stored.  Starts off at
				 * interp->resultSpace, but may get dynamically
				 * re-allocated if this isn't enough. */
    int dstSize = 0;		/* Number of non-null characters currently
				 * stored at dst. */
    int dstSpace = TCL_RESULT_SIZE;
				/* Total amount of storage space available
				 * in dst (not including null terminator. */
    int noPercent;		/* Special case for speed:  indicates there's
				 * no field specifier, just a string to copy. */ 
    int argIndex;		/* Index of argument to substitute next. */ 
    int gotXpg = 0;		/* Non-zero means that an XPG3 %n$-style 
				 * specifier has been seen. */ 
    int gotSequential = 0;	/* Non-zero means that a regular sequential
				 * (non-XPG3) conversion specifier has been
				 * seen. */
    int useShort;		/* Value to be printed is short (half word). */
    char *end;			/* Used to locate end of numerical fields. */
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    unsigned char *i18n_msg_format_temp, *i18n_msg_buf_temp;
    int i18n_temp_length;
    char *ctemp;
    size_t clen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    /*
     * This procedure is a bit nasty.  The goal is to use sprintf to
     * do most of the dirty work.  There are several problems:
     * 1. this procedure can't trust its arguments.
     * 2. we must be able to provide a large enough result area to hold
     *    whatever's generated.  This is hard to estimate.
     * 2. there's no way to move the arguments from argv to the call
     *    to sprintf in a reasonable way.  This is particularly nasty
     *    because some of the arguments may be two-word values (doubles).
     * So, what happens here is to scan the format string one % group
     * at a time, making many individual calls to sprintf.
     */

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdah_wnum_args_fmtstr_arg_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("format")
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "format");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    argIndex = 2;

    _WChar_To_Multibyte(interp, clen, wargv[1], format, st)
    if (st == (size_t)-1)
    {
	goto fmtError;
    }
    format_save = format;

    while (*format != 0) 
    {
	register char *newPtr = newFormat;

	width = precision = useTwoWords = noPercent = useShort = 0;

	/*
	 * Get rid of any characters before the next field specifier.
	 */

	if (*format != '%') 
	{
	    register char *p;

	    oneWordValue = p = format;
	    while ((*format != '%') && (*format != 0)) {
		*p = *format;
		p++;
		format++;
	    }
	    size = p - oneWordValue;
	    noPercent = 1;
	    goto doField;
	}

	if (format[1] == '%') 
	{
	    oneWordValue = format;
	    size = 1;
	    noPercent = 1;
	    format += 2;
	    goto doField;
	}

	/*
	 * Parse off a field specifier, compute how many characters
	 * will be needed to store the result, and substitute for
	 * "*" size specifiers.
	 */

	*newPtr = '%';
	newPtr++;
	format++;
	if (isdigit(UCHAR(*format))) 
	{
	    int tmp;

	    /*
	     * Check for an XPG3-style %n$ specification.  Note: there
	     * must not be a mixture of XPG3 specs and non-XPG3 specs
	     * in the same format string.
	     */

	    tmp = strtoul(format, &end, 10);
	    if (*end != '$') {
		goto notXpg;
	    }
	    format = end+1;
	    gotXpg = 1;
	    if (gotSequential) {
		goto mixedXPG;
	    }
	    argIndex = tmp+1;
	    if ((argIndex < 2) || (argIndex >= argc)) {
		goto badIndex;
	    }
	    goto xpgCheckDone;
	}

	notXpg:
	gotSequential = 1;
	if (gotXpg) 
	{
	    goto mixedXPG;
	}

	xpgCheckDone:
	while ((*format == '-') || (*format == '#') || (*format == '0')
		|| (*format == ' ') || (*format == '+')) 
	{
	    *newPtr = *format;
	    newPtr++;
	    format++;
	}
	if (isdigit(UCHAR(*format))) 
	{
	    width = strtoul(format, &end, 10);
	    format = end;
	} 
	else if (*format == '*') 
	{
	    if (argIndex >= argc) 
	    {
		goto badIndex;
	    }
	    _WChar_To_Multibyte(interp, clen, wargv[argIndex], ctemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(format_save);
		goto fmtError;
	    }	
	    if (Tcl_GetInt(interp, ctemp, &width) != TCL_OK) 
	    {
		ckfree(format_save);
		ckfree(ctemp);
		goto fmtError;
	    }
	    ckfree(ctemp);
	    argIndex++;
	    format++;
	}
	if (width != 0) 
	{
	    sprintf(newPtr, "%d", width);
	    while (*newPtr != 0) 
	    {
		newPtr++;
	    }
	}
	if (*format == '.') 
	{
	    *newPtr = '.';
	    newPtr++;
	    format++;
	}
	if (isdigit(UCHAR(*format))) 
	{
	    precision = strtoul(format, &end, 10);
	    format = end;
	} 
	else if (*format == '*') 
	{
	    if (argIndex >= argc) 
	    {
		goto badIndex;
	    }
	    _WChar_To_Multibyte(interp, clen, wargv[argIndex], ctemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(format_save);
		goto fmtError;
	    }	
	    if (Tcl_GetInt(interp, ctemp, &precision) != TCL_OK) 
	    {
		ckfree(format_save);
		ckfree(ctemp);
		goto fmtError;
	    }
	    ckfree(ctemp);
	    argIndex++;
	    format++;
	}
	if (precision != 0) 
	{
	    sprintf(newPtr, "%d", precision);
	    while (*newPtr != 0) 
	    {
		newPtr++;
	    }
	}
	if (*format == 'l') 
	{
	    format++;
	} 
	else if (*format == 'h') 
	{
	    useShort = 1;
	    *newPtr = 'h';
	    newPtr++;
	    format++;
	}
	*newPtr = *format;
	newPtr++;
	*newPtr = 0;
	if (argIndex >= argc) 
	{
	    goto badIndex;
	}
	switch (*format) 
	{
	    case 'i':
		newPtr[-1] = 'd';
	    case 'd':
	    case 'o':
	    case 'u':
	    case 'x':
	    case 'X':
		_WChar_To_Multibyte(interp, clen, wargv[argIndex], ctemp, st)
		if (st == (size_t)-1)
		{
	    	    ckfree(format_save);
		    goto fmtError;
		}	
		if (Tcl_GetInt(interp, ctemp, (int *) &oneWordValue) != TCL_OK) 
		{
		    ckfree(format_save);
		    ckfree(ctemp);
		    goto fmtError;
		}
		ckfree(ctemp);
		size = 40;
		break;
	    case 's':
		_WChar_To_Multibyte(interp, clen, wargv[argIndex], ctemp, st)
		if (st == (size_t)-1)
		{
	    	    ckfree(format_save);
		    goto fmtError;
		}	
		oneWordValue = ctemp;
		oneWordFlag = 1;
		size = strlen(ctemp);
		break;
	    case 'c':
		_WChar_To_Multibyte(interp, clen, wargv[argIndex], ctemp, st)
		if (st == (size_t)-1)
		{
	    	    ckfree(format_save);
		    goto fmtError;
		}	
		if (Tcl_GetInt(interp, ctemp, (int *) &oneWordValue) != TCL_OK) 
		{
		    ckfree(format_save);
		    ckfree(ctemp);
		    goto fmtError;
		}
		ckfree(ctemp);
		size = 1;
		break;
	    case 'e':
	    case 'E':
	    case 'f':
	    case 'g':
	    case 'G':
		_WChar_To_Multibyte(interp, clen, wargv[argIndex], ctemp, st)
		if (st == (size_t)-1)
		{
	    	    ckfree(format_save);
		    goto fmtError;
		}	
		if (Tcl_GetDouble(interp, ctemp, &twoWordValue) != TCL_OK) 
		{
		    ckfree(format_save);
		    ckfree(ctemp);
		    goto fmtError;
		}
		ckfree(ctemp);
		useTwoWords = 1;
		size = 320;
		if (precision > 10) 
		{
		    size += precision;
		}
		break;
	    case 0:
		interp->result = (char *)dce_msg_get(tcl_s_cmdah_fmt_str_ended_in_middle);
		interp->freeProc = (Tcl_FreeProc *) free;
		goto fmtError;
	    default:
		i18n_msg_format = dce_msg_get(tcl_s_cmdah_bad_field_spec);
		sprintf(interp->result, (char *)i18n_msg_format, *format);
		free(i18n_msg_format);
		goto fmtError;
	}
	argIndex++;
	format++;

	/*
	 * Make sure that there's enough space to hold the formatted
	 * result, then format it.
	 */

	doField:
	if (width > size) 
	{
	    size = width;
	}
	if ((dstSize + size) > dstSpace) 
	{
	    char *newDst;
	    int newSpace;

	    newSpace = 2*(dstSize + size);
	    newDst = (char *) ckalloc((unsigned) newSpace+1);
	    if (dstSize != 0) 
	    {
		memcpy((VOID *) newDst, (VOID *) dst, dstSize);
	    }
	    if (dstSpace != TCL_RESULT_SIZE) 
	    {
		ckfree(dst);
	    }
	    dst = newDst;
	    dstSpace = newSpace;
	}
	if (noPercent) 
	{
	    memcpy((VOID *) (dst+dstSize), (VOID *) oneWordValue, size);
	    if (oneWordFlag)
	    {
		ckfree(oneWordValue);
		oneWordFlag = 0;
	    }
	    dstSize += size;
	    dst[dstSize] = 0;
	} 
	else 
	{
	    if (useTwoWords) 
	    {
		sprintf(dst+dstSize, newFormat, twoWordValue);
	    } 
	    else if (useShort) 
	    {
		/*
		 * The double cast below is needed for a few machines
		 * (e.g. Pyramids as of 1/93) that don't like casts
		 * directly from pointers to shorts.
		 */

		sprintf(dst+dstSize, newFormat, (short) (int) oneWordValue);
	        if (oneWordFlag)
	        {
	    	    ckfree(oneWordValue);
		    oneWordFlag = 0;
	        }
	    } 
	    else 
	    {
		sprintf(dst+dstSize, newFormat, (char *) oneWordValue);
	        if (oneWordFlag)
	        {
	    	    ckfree(oneWordValue);
		    oneWordFlag = 0;
	        }
	    }
	    dstSize += strlen(dst+dstSize);
	}
	ckfree(format_save);
	if (oneWordFlag)
	{
	    ckfree(oneWordValue);
	    oneWordFlag = 0;
	}
    }

    interp->result = dst;
    if (dstSpace != TCL_RESULT_SIZE) 
    {
	interp->freeProc = (Tcl_FreeProc *) free;
    } 
    else 
    {
	interp->freeProc = 0;
    }
    return TCL_OK;

    mixedXPG:
    i18n_msg_format = dce_msg_get(tcl_s_cmdah_cannot_mix_conversion_specs);
    /*
     * Need fix to elimiate '1' and '2' from the error string, since
     * dce_msg_get() automatically makes XPG4 modification.
     */
    i18n_temp_length = strlen((char *)i18n_msg_format);
    i18n_msg_format_temp = i18n_msg_format;
    i18n_msg_buf = (unsigned char *)malloc(i18n_temp_length);
    i18n_msg_buf_temp = i18n_msg_buf;
    while (i18n_temp_length--)
    {
	if (*i18n_msg_format_temp == '1')
		i18n_msg_format_temp += 2;	/* skip 1$ */
	else if (*i18n_msg_format_temp == '2')
		i18n_msg_format_temp += 2;	/* skip 2$ */
	else
		*i18n_msg_buf_temp++ = *i18n_msg_format_temp++;
    }
    *i18n_msg_buf_temp = '\0';

    free(i18n_msg_format);
    interp->result  = (char *)i18n_msg_buf;
    interp->freeProc = (Tcl_FreeProc *) free;

    goto fmtError;

    badIndex:
    if (gotXpg) 
    {
	interp->result = (char *)dce_msg_get(tcl_s_cmdah_arg_index_out_of_range);
	interp->freeProc = (Tcl_FreeProc *) free;
    } 
    else 
    {
	interp->result = (char *)dce_msg_get(tcl_s_cmdah_not_enough_args);
	interp->freeProc = (Tcl_FreeProc *) free;
    }

    fmtError:
    if (dstSpace != TCL_RESULT_SIZE) 
    {
	ckfree(dst);
    }
    return TCL_ERROR;
}
