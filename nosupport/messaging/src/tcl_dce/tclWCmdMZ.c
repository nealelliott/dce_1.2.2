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
 * $Log: tclWCmdMZ.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.4.1  1995/02/17  15:56:06  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/17  15:51:57  levy]
 *
 * Revision 1.1.2.1  1994/10/21  21:39:45  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:35:14  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclCmdMZ.c --
 *
 *	This file contains the top-level command routines for most of
 *	the Tcl built-in commands whose names begin with the letters
 *	M to Z.  It contains only commands in the generic core (i.e.
 *	those that don't depend much upon UNIX facilities).
 */

#include "tclInt.h"
#include "tclWInt.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * Structure used to hold information about variable traces:
 */

typedef struct {
    int flags;			/* Operations for which Tcl command is
				 * to be invoked. */
    char *errMsg;		/* Error message returned from Tcl command,
				 * or NULL.  Malloc'ed. */
    int length;			/* Number of non-NULL chars. in command. */
    char command[4];		/* Space for Tcl command to invoke.  Actual
				 * size will be as large as necessary to
				 * hold command.  This field must be the
				 * last in the structure, so that it can
				 * be larger than 4 bytes. */
} TraceVarInfo;

/*
 * Forward declarations for procedures defined in this file:
 */

static char *		_WTraceVarProc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, char *name1, char *name2,
			    int flags));

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WRegexpCmd --
 *
 *	This procedure is invoked to process the "regexp" Tcl command.
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
_Tcl_WRegexpCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int noCase = 0;
    int indices = 0;
    regexp *regexpPtr;
    wchar_t **wargPtr, *wstring, *wpattern;
    char *string, *pattern;
    int match = 0;			/* Initialization needed only to
					 * prevent compiler warning. */
    int i;
    Tcl_DString stringDString, patternDString;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp, *first_save;
    wchar_t *wtemp;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 3) 
    {
	wrongNumArgs:
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_swit_exp_str);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("regexp")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "regexp");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    wargPtr = wargv+1;
    argc--;
    _WChar_To_Multibyte(interp, len, wargPtr[0], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    while ((argc > 0) && (*ctemp == '-')) 
    {
	if (strcmp(ctemp, "-indices") == 0) 
	{
	    indices = 1;
	} 
	else if (strcmp(ctemp, "-nocase") == 0) 
	{
	    noCase = 1;
	} 
	else if (strcmp(ctemp, "--") == 0) 
	{
	    wargPtr++;
	    argc--;
	    ckfree(ctemp);
	    _WChar_To_Multibyte(interp, len, wargPtr[0], ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;
	    break;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_switch_indices_nocase);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	wargPtr++;
	argc--;

	ckfree(ctemp);
	_WChar_To_Multibyte(interp, len, wargPtr[0], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;
    }
    if (argc < 2) 
    {
	ckfree(ctemp);
	goto wrongNumArgs;
    }

    /*
     * Convert the string and pattern to lower case, if desired, and
     * perform the matching operation.
     */

    if (noCase) 
    {
	register wchar_t *wp;

	Tcl_DStringInit(&patternDString);
	Tcl_DStringAppend(&patternDString, ctemp, -1);
	pattern = Tcl_DStringValue(&patternDString);
	_Multibyte_to_WChar(interp, len, pattern, wpattern, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    Tcl_DStringFree(&patternDString);
	    return TCL_ERROR;
	}

	for (wp = wpattern; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (iswupper(*wp)) 
	    {
		*wp = towlower(*wp);
	    }
	}

	_WChar_To_Multibyte(interp, len, wargPtr[1], string, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    ckfree(wpattern);
	    Tcl_DStringFree(&patternDString);
	    return TCL_ERROR;
	}

	Tcl_DStringInit(&stringDString);
	Tcl_DStringAppend(&stringDString, string, -1);
	string = Tcl_DStringValue(&stringDString);
	_Multibyte_to_WChar(interp, len, string, wstring, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    ckfree(wpattern);
	    Tcl_DStringFree(&patternDString);
	    Tcl_DStringFree(&stringDString);
	    return TCL_ERROR;
	}

	for (wp = wstring; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (iswupper(*wp)) 
	    {
		*wp = towlower(*wp);
	    }
	}
	ckfree(ctemp);

	_WChar_To_Multibyte(interp, len, wpattern, pattern, st)
	if (st == (size_t)-1)
	{
		ckfree(wpattern);
		ckfree(wstring);
		return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wstring, string, st)
	if (st == (size_t)-1)
	{
		ckfree(wpattern);
		ckfree(pattern);
		ckfree(wstring);
		return TCL_ERROR;
	}
	ckfree(wpattern);
	ckfree(wstring);
	first_save = string;
    } 
    else 
    {
	ckfree(ctemp);

	_WChar_To_Multibyte(interp, len, wargPtr[0], pattern, st)
	if (st == (size_t)-1)
	{
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargPtr[1], string, st)
	if (st == (size_t)-1)
	{
	    ckfree(pattern);
	    return TCL_ERROR;
	}
	first_save = string;
    }

    regexpPtr = TclCompileRegexp(interp, pattern);
    if (regexpPtr != NULL) 
    {
	tclRegexpError = NULL;
	match = TclRegExec(regexpPtr, string, string);
    }
    if (noCase) 
    {
	Tcl_DStringFree(&stringDString);
	Tcl_DStringFree(&patternDString);
    }
    if (regexpPtr == NULL) 
    {
	ckfree(string);
	ckfree(pattern);
	return TCL_ERROR;
    }
    if (tclRegexpError != NULL) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_err_while_match_pat);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(tclRegexpError)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, tclRegexpError);
	free(tclRegexpError);		/* free regular expression error msg */

	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(string);
	ckfree(pattern);

	return TCL_ERROR;
    }
    if (!match) 
    {
	interp->result = "0";
	ckfree(string);
	ckfree(pattern);
	return TCL_OK;
    }

    /*
     * If additional variable names have been specified, return
     * index information in those variables.
     */

    argc -= 2;
    if (argc > NSUBEXP) 
    {
	interp->result = (char *)dce_msg_get(tcl_s_cmdmz_too_many_substr_var);
	interp->freeProc = (Tcl_FreeProc *) free;
	ckfree(string);
	ckfree(pattern);

	return TCL_ERROR;
    }
    _Multibyte_to_WChar(interp, len, "-1 -1", wtemp, st)
    if (st == (size_t)-1)
    {
	ckfree(string);
	ckfree(pattern);
	return TCL_ERROR;
    }
    
    for (i = 0; i < argc; i++) 
    {
	char *result, info[50];

	if (regexpPtr->startp[i] == NULL) 
	{
	    if (indices) 
	    {
		result = _Tcl_WSetVar(interp, wargPtr[i+2], wtemp, 0);
	    } 
	    else 
	    {
		result = _Tcl_WSetVar(interp, wargPtr[i+2], (wchar_t *)"", 0);
	    }
	} 
	else 
	{
	    if (indices) 
	    {
		ckfree(wtemp);
		sprintf(info, "%d %d", regexpPtr->startp[i] - string,
			regexpPtr->endp[i] - string - 1);

		_Multibyte_to_WChar(interp, len, info, wtemp, st)
		if (st == (size_t)-1)
		{
		    ckfree(string);
		    ckfree(pattern);
		    return TCL_ERROR;
		}
		result = _Tcl_WSetVar(interp, wargPtr[i+2], wtemp, 0);
	    } 
	    else 
	    {
		char savedChar, *first, *last;

		first = first_save + (regexpPtr->startp[i] - string);
		last = first_save + (regexpPtr->endp[i] - string);
		savedChar = *last;
		*last = 0;
		ckfree(wtemp);
		_Multibyte_to_WChar(interp, len, first, wtemp, st)
		if (st == (size_t)-1)
		{
		    ckfree(string);
		    ckfree(pattern);
		    return TCL_ERROR;
		}
		result = _Tcl_WSetVar(interp, wargPtr[i+2], wtemp, 0);
		*last = savedChar;
	    }
	}
	if (result == NULL) 
	{
	    _WChar_To_Multibyte(interp, len, wargPtr[i+2], ctemp, st)
	    if (st == (size_t)-1)
	    {
	        ckfree(wtemp);
	        ckfree(string);
	        ckfree(pattern);
		return TCL_ERROR;
	    }

	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_could_not_set_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(wtemp);
	    ckfree(ctemp);
	    ckfree(string);
	    ckfree(pattern);

	    return TCL_ERROR;
	}
    }
    interp->result = "1";
    ckfree(wtemp);
    ckfree(string);
    ckfree(pattern);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WRegsubCmd --
 *
 *	This procedure is invoked to process the "regsub" Tcl command.
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
_Tcl_WRegsubCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int noCase = 0, all = 0;
    regexp *regexpPtr;
    char *string, *pattern, *p, *newValue; 
    wchar_t *wfirstChar, *wsrc, wc;
    wchar_t **wargvPtr;
    int match, flags, code, anyMatches;
    register char *src, c;
    Tcl_DString stringDString, patternDString;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    wchar_t *wtemp, *wpattern, *wstring;
    char *string_save, *cptr;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 5) 
    {
	wrongNumArgs:
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_swit_subspec_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("regsub")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "regsub");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    wargvPtr = wargv+1;
    argc--;
    _WChar_To_Multibyte(interp, len, wargvPtr[0], ctemp, st)
    if (st == (size_t)-1)
    {
	return TCL_ERROR;
    }

    while (*ctemp == '-') 
    {
	if (strcmp(ctemp, "-nocase") == 0) 
	{
	    noCase = 1;
	} 
	else if (strcmp(ctemp, "-all") == 0) 
	{
	    all = 1;
	} 
	else if (strcmp(ctemp, "--") == 0) 
	{
	    wargvPtr++;
	    argc--;

	    ckfree(ctemp);
	    _WChar_To_Multibyte(interp, len, wargvPtr[0], ctemp, st)
	    if (st == (size_t)-1)
		return TCL_ERROR;
	    break;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_switch_all_nocase);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	wargvPtr++;
	argc--;

	ckfree(ctemp);
	_WChar_To_Multibyte(interp, len, wargvPtr[0], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;
    }

    if (argc != 4) 
    {
	ckfree(ctemp);
	goto wrongNumArgs;
    }

    /*
     * Convert the string and pattern to lower case, if desired.
     */

    if (noCase) 
    {
	wchar_t *wp;

	Tcl_DStringInit(&patternDString);
	Tcl_DStringAppend(&patternDString, ctemp, -1);
	pattern = Tcl_DStringValue(&patternDString);
	_Multibyte_to_WChar(interp, len, pattern, wpattern, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    Tcl_DStringFree(&patternDString);
	    return TCL_ERROR;
	}

	for (wp = wpattern; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (iswupper(*wp)) 
	    {
		*wp = towlower(*wp);
	    }
	}

	_WChar_To_Multibyte(interp, len, wargvPtr[1], string, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    ckfree(wpattern);
	    Tcl_DStringFree(&patternDString);
	    return TCL_ERROR;
	}

	Tcl_DStringInit(&stringDString);
	Tcl_DStringAppend(&stringDString, string, -1);
	string = Tcl_DStringValue(&stringDString);
	_Multibyte_to_WChar(interp, len, string, wstring, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    ckfree(wpattern);
	    Tcl_DStringFree(&patternDString);
	    Tcl_DStringFree(&stringDString);
	    return TCL_ERROR;
	}

	for (wp = wstring; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (iswupper(*wp)) 
	    {
		*wp = towlower(*wp);
	    }
	}
	ckfree(ctemp);

	_WChar_To_Multibyte(interp, len, wpattern, pattern, st)
	if (st == (size_t)-1)
	{
	    ckfree(wpattern);
	    ckfree(wstring);
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wstring, string, st)
	if (st == (size_t)-1)
	{
	    ckfree(wpattern);
	    ckfree(wstring);
	    ckfree(pattern);
	    return TCL_ERROR;
	}
	ckfree(wpattern);
	ckfree(wstring);
	string_save = string;
    } 
    else 
    {
	ckfree(ctemp);

	_WChar_To_Multibyte(interp, len, wargvPtr[0], pattern, st)
	if (st == (size_t)-1)
	{
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargvPtr[1], string, st)
	if (st == (size_t)-1)
	{
	    ckfree(pattern);
	    return TCL_ERROR;
	}
	string_save = string;
    }

    regexpPtr = TclCompileRegexp(interp, pattern);
    if (regexpPtr == NULL) 
    {
	ckfree(string_save);
	ckfree(pattern);
	code = TCL_ERROR;
	goto done;
    }

    /*
     * The following loop is to handle multiple matches within the
     * same source string;  each iteration handles one match and its
     * corresponding substitution.  If "-all" hasn't been specified
     * then the loop body only gets executed once.
     */

    flags = 0;
    anyMatches = 0;
    for (p = string; *p != 0; ) 
    {
	tclRegexpError = NULL;
	match = TclRegExec(regexpPtr, p, string);

	if (tclRegexpError != NULL) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_err_while_match_pat);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(tclRegexpError)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, tclRegexpError);
	    free(tclRegexpError);

	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(string_save);
	    ckfree(pattern);

	    code = TCL_ERROR;
	    goto done;
	}
	if (!match) 
	{
	    break;
	}
	anyMatches = 1;

	/*
	 * Copy the portion of the source string before the match to the
	 * result variable.
	 */
    
	src = string_save + (regexpPtr->startp[0] - string);
	c = *src;
	*src = 0;
	cptr = string_save + (p - string);
	_Multibyte_to_WChar(interp, len, cptr, wtemp, st)
	if (st == (size_t)-1)
	{
	    ckfree(string_save);
	    ckfree(pattern);
	    return TCL_ERROR;
	}
	
	newValue = _Tcl_WSetVar(interp, wargvPtr[3], wtemp, flags);
	ckfree(wtemp);
	*src = c;
	flags = TCL_APPEND_VALUE;
	if (newValue == NULL) 
	{
	    _WChar_To_Multibyte(interp, len, wargvPtr[3], ctemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(string_save);
		ckfree(pattern);
		return TCL_ERROR;
	    }
	    cantSet:
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_could_not_set_var);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(string_save);
	    ckfree(pattern);
	    ckfree(ctemp);

	    code = TCL_ERROR;
	    goto done;
	}
    
	/*
	 * Append the subSpec argument to the variable, making appropriate
	 * substitutions.  This code is a bit hairy because of the backslash
	 * conventions and because the code saves up ranges of characters in
	 * subSpec to reduce the number of calls to Tcl_SetVar.
	 */
    
	for (wsrc = wfirstChar = wargvPtr[2], wc = *wsrc; 
		wc != i18n_special_chars.w_esc_zero; wsrc++, wc = *wsrc) 
	{
	    int index;
    
	    if (wc == i18n_special_chars.w_and) 
	    {
		index = 0;
	    } 
	    else if (wc == i18n_special_chars.w_bsla) 
	    {
		wc = wsrc[1];
		if (wc == i18n_parse_chars.w_0)
		    index = 0;
		else if (wc == i18n_parse_chars.w_1)
		    index = 1;
		else if (wc == i18n_parse_chars.w_2)
		    index = 2;
		else if (wc == i18n_parse_chars.w_3)
		    index = 3;
		else if (wc == i18n_parse_chars.w_4)
		    index = 4;
		else if (wc == i18n_parse_chars.w_5)
		    index = 5;
		else if (wc == i18n_parse_chars.w_6)
		    index = 6;
		else if (wc == i18n_parse_chars.w_7)
		    index = 7;
		else if (wc == i18n_parse_chars.w_8)
		    index = 8;
		else if (wc == i18n_parse_chars.w_9)
		    index = 9;
		else if ((wc == i18n_special_chars.w_bsla) 
			|| (wc == i18n_special_chars.w_and)) 
		{
		    *wsrc = wc;
		    wsrc[1] = i18n_special_chars.w_esc_zero;
		    newValue = _Tcl_WSetVar(interp, wargvPtr[3], wfirstChar,
			    TCL_APPEND_VALUE);
		    *wsrc = i18n_special_chars.w_bsla;
		    wsrc[1] = wc;
		    if (newValue == NULL) 
		    {
			goto cantSet;
		    }
		    wfirstChar = wsrc+2;
		    wsrc++;
		    continue;
		} 
		else 
		{
		    continue;
		}
	    } 
	    else 
	    {
		continue;
	    }
	    if (wfirstChar != wsrc) 
	    {
		wc = *wsrc;
		*wsrc = i18n_special_chars.w_esc_zero;
		newValue = _Tcl_WSetVar(interp, wargvPtr[3], wfirstChar,
			TCL_APPEND_VALUE);
		*wsrc = wc;
		if (newValue == NULL) 
		{
		    goto cantSet;
		}
	    }
	    if ((index < NSUBEXP) && (regexpPtr->startp[index] != NULL)
		    && (regexpPtr->endp[index] != NULL)) 
	    {
		char *first, *last, saved;
		wchar_t *wtemp;

		first = string_save + (regexpPtr->startp[index] - string);
		last = string_save + (regexpPtr->endp[index] - string);
		saved = *last;
		*last = 0;
		_Multibyte_to_WChar(interp, len, first, wtemp, st)
		if (st == (size_t)-1)
		{
		    ckfree(string_save);
		    ckfree(pattern);
		    code = TCL_ERROR;
		    goto done;
		}
		newValue = _Tcl_WSetVar(interp, wargvPtr[3], wtemp,
			TCL_APPEND_VALUE);
		ckfree(wtemp);
		*last = saved;
		if (newValue == NULL) 
		{
		    goto cantSet;
		}
	    }
	    if (*wsrc == i18n_special_chars.w_bsla) 
	    {
		wsrc++;
	    }
	    wfirstChar = wsrc+1;
	}
	if (wfirstChar != wsrc) 
	{
	    if (_Tcl_WSetVar(interp, wargvPtr[3], wfirstChar,
		    TCL_APPEND_VALUE) == NULL) 
	    {
		goto cantSet;
	    }
	}
	if (regexpPtr->endp[0] == p) 
	{
	    wchar_t wtmp[2];
	    char temp_c[2];

	    /*
	     * Always consume at least one character of the input string
	     * in order to prevent infinite loops.
	     */

	    temp_c[0] = string_save[p - string];
	    temp_c[1] = 0;
	    mbtowc(&wtemp[0], (const char *)temp_c, (size_t)1);
	    wtmp[1] = i18n_special_chars.w_esc_zero;
	    newValue = _Tcl_WSetVar(interp, wargvPtr[3], wtmp, flags);
	    if (newValue == NULL) 
	    {
		goto cantSet;
	    }
	    p = regexpPtr->endp[0] + 1;
	} 
	else 
	{
	    p = regexpPtr->endp[0];
	}
	if (!all) 
	{
	    break;
	}
    }

    /*
     * Copy the portion of the source string after the last match to the
     * result variable.
     */

    if ((*p != 0) || !anyMatches)
    {
	char *tcp;
	wchar_t *wtemp;

	tcp = string_save + (p - string);
	_Multibyte_to_WChar(interp, len, tcp, wtemp, st)
	if (st == (size_t)-1)
	{
	    ckfree(string_save);
	    ckfree(pattern);
	    code = TCL_ERROR;
	    goto done;
	}

	if (_Tcl_WSetVar(interp, wargvPtr[3], wtemp, flags) == NULL) 
	{
	    ckfree(wtemp);
	    goto cantSet;
	}
	ckfree(wtemp);
    }
    if (anyMatches) 
    {
	interp->result = "1";
    } 
    else 
    {
	interp->result = "0";
    }
    ckfree(string_save);
    ckfree(pattern);
    code = TCL_OK;

    done:
    if (noCase) {
	Tcl_DStringFree(&stringDString);
	Tcl_DStringFree(&patternDString);
    }
    return code;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WRenameCmd --
 *
 *	This procedure is invoked to process the "rename" Tcl command.
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
_Tcl_WRenameCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register Command *cmdPtr;
    Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    int new;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *cargv1, *cargv2;
    size_t clen, st;

    if (argc != 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_oldn_newn);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("rename")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "rename");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, clen, wargv[1], cargv1, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    _WChar_To_Multibyte(interp, clen, wargv[2], cargv2, st)
    if (st == (size_t)-1)
    {
	ckfree(cargv1);
	return TCL_ERROR;
    }

    if (*cargv2 == '\0') 
    {
	if (Tcl_DeleteCommand(interp, cargv1) != 0) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_del_cmd_doesnot_exist);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv1)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv1);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv1);
	    ckfree(cargv2);

	    return TCL_ERROR;
	}
	return TCL_OK;
    }
    hPtr = Tcl_FindHashEntry(&iPtr->commandTable, cargv2);
    if (hPtr != NULL) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_rename_cmd_already_exist);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv2)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv2);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(cargv1);
	ckfree(cargv2);

	return TCL_ERROR;
    }
    hPtr = Tcl_FindHashEntry(&iPtr->commandTable, cargv1);
    if (hPtr == NULL) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_rename_cmd_doesnot_exist);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv1)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv1);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(cargv1);
	ckfree(cargv2);

	return TCL_ERROR;
    }
    cmdPtr = (Command *) Tcl_GetHashValue(hPtr);
    Tcl_DeleteHashEntry(hPtr);
    hPtr = Tcl_CreateHashEntry(&iPtr->commandTable, cargv2, &new);
    Tcl_SetHashValue(hPtr, cmdPtr);
    ckfree(cargv1);
    ckfree(cargv2);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WReturnCmd --
 *
 *	This procedure is invoked to process the "return" Tcl command.
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
_Tcl_WReturnCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    Interp *iPtr = (Interp *) interp;
    int c, code;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *cargv0, *cargv1;
    size_t clen, st;

    if (iPtr->errorInfo != NULL) 
    {
	ckfree(iPtr->errorInfo);
	iPtr->errorInfo = NULL;
    }
    if (iPtr->errorCode != NULL) 
    {
	ckfree(iPtr->errorCode);
	iPtr->errorCode = NULL;
    }
    code = TCL_OK;
    for (wargv++, argc--; argc > 1; wargv += 2, argc -= 2) 
    {
	_WChar_To_Multibyte(interp, clen, wargv[0], cargv0, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	_WChar_To_Multibyte(interp, clen, wargv[1], cargv1, st)
	if (st == (size_t)-1)
	{
	    ckfree(cargv0);
	    return TCL_ERROR;
	}

	if (strcmp(cargv0, "-code") == 0) 
	{
	    c = *cargv1;
	    if ((c == 'o') && (strcmp(cargv1, "ok") == 0)) 
	    {
		code = TCL_OK;
	    } 
	    else if ((c == 'e') && (strcmp(cargv1, "error") == 0)) 
	    {
		code = TCL_ERROR;
	    } 
	    else if ((c == 'r') && (strcmp(cargv1, "return") == 0)) 
	    {
		code = TCL_RETURN;
	    } 
	    else if ((c == 'b') && (strcmp(cargv1, "break") == 0)) 
	    {
		code = TCL_BREAK;
	    } 
	    else if ((c == 'c') && (strcmp(cargv1, "continue") == 0)) 
	    {
		code = TCL_CONTINUE;
	    } 
	    else if (_Tcl_WGetInt(interp, wargv[1], &code) != TCL_OK) 
	    {
		Tcl_ResetResult(interp);
		i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_comp_code_ok_err_ret_brk);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv1)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv1);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		ckfree(cargv0);
		ckfree(cargv1);

		return TCL_ERROR;
	    }
	} 
	else if (strcmp(cargv0, "-errorinfo") == 0) 
	{
	    iPtr->errorInfo = ckalloc((unsigned) (strlen(cargv1) + 1));
	    strcpy(iPtr->errorInfo, cargv1);
	} 
	else if (strcmp(cargv0, "-errorcode") == 0) 
	{
	    iPtr->errorCode = ckalloc((unsigned) (strlen(cargv1) + 1));
	    strcpy(iPtr->errorCode, cargv1);
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_opt_code_errcode_errinfo);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("return")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "return");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cargv0);
	    ckfree(cargv1);

	    return TCL_ERROR;
	}
	ckfree(cargv0);
	ckfree(cargv1);
    }
    if (argc == 1) 
    {
	_WChar_To_Multibyte(interp, clen, wargv[0], cargv0, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;

	Tcl_SetResult(interp, cargv0, TCL_VOLATILE);
    }
    iPtr->returnCode = code;
    return TCL_RETURN;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WScanCmd --
 *
 *	This procedure is invoked to process the "scan" Tcl command.
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
_Tcl_WScanCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
#   define MAX_FIELDS 20
    typedef struct {
	char fmt;			/* Format for field. */
	int size;			/* How many bytes to allow for
					 * field. */
	char *location;			/* Where field will be stored. */
    } Field;
    Field fields[MAX_FIELDS];		/* Info about all the fields in the
					 * format string. */
    register Field *curField;
    int numFields = 0;			/* Number of fields actually
					 * specified. */
    int suppress;			/* Current field is assignment-
					 * suppressed. */
    int totalSize = 0;			/* Number of bytes needed to store
					 * all results combined. */
    char *results;			/* Where scanned output goes.
					 * Malloced; NULL means not allocated
					 * yet. */
    int numScanned;			/* sscanf's result. */
    register wchar_t *wfmt;
    int i, widthSpecified, length, code;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp, *cargv1, *cfmtCopy;
    wchar_t *wstring;
    size_t len, st;

    /*
     * The variables below are used to hold a copy of the format
     * string, so that we can replace format specifiers like "%f"
     * and "%F" with specifiers like "%lf"
     */

#   define STATIC_SIZE 5
    wchar_t wcopyBuf[STATIC_SIZE], *wfmtCopy;
    register wchar_t *wdst;

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
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_str_format_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("scan")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "scan");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * This procedure operates in four stages:
     * 1. Scan the format string, collecting information about each field.
     * 2. Allocate an array to hold all of the scanned fields.
     * 3. Call sscanf to do all the dirty work, and have it store the
     *    parsed fields in the array.
     * 4. Pick off the fields from the array and assign them to variables.
     */

    code = TCL_OK;
    results = NULL;
    length = wcslen(wargv[2]) * 2 + 1;
    if (length < STATIC_SIZE) 
    {
	wfmtCopy = wcopyBuf;
    } 
    else 
    {
	wfmtCopy = ckalloc((unsigned) length * sizeof(wchar_t));
    }
    wdst = wfmtCopy;
    for (wfmt = wargv[2]; *wfmt != i18n_special_chars.w_esc_zero; wfmt++) 
    {
	*wdst = *wfmt;
	wdst++;
	if (*wfmt != i18n_special_chars.w_percent) 
	{
	    continue;
	}
	wfmt++;
	if (*wfmt == i18n_special_chars.w_percent) 
	{
	    *wdst = *wfmt;
	    wdst++;
	    continue;
	}
	if (*wfmt == i18n_special_chars.w_star) 
	{
	    suppress = 1;
	    *wdst = *wfmt;
	    wdst++;
	    wfmt++;
	} 
	else 
	{
	    suppress = 0;
	}
	widthSpecified = 0;
	while (iswdigit(*wfmt)) 
	{
	    widthSpecified = 1;
	    *wdst = *wfmt;
	    wdst++;
	    wfmt++;
	}
	if ((*wfmt == i18n_parse_chars.w_l) 
		|| (*wfmt == i18n_parse_chars.w_h) 
		|| (*wfmt == i18n_parse_chars.w_L)) 
	{
	    wfmt++;
	}
	*wdst = *wfmt;
	wdst++;
	if (suppress) 
	{
	    continue;
	}
	if (numFields == MAX_FIELDS) 
	{
	    interp->result = (char *)dce_msg_get(tcl_s_cmdmz_too_many_flds_to_scan);
	    interp->freeProc = (Tcl_FreeProc *) free;
	    code = TCL_ERROR;
	    goto done;
	}
	curField = &fields[numFields];
	numFields++;

	if (*wfmt == i18n_parse_chars.w_d || 	/* switch (*wfmt) */
	    *wfmt == i18n_parse_chars.w_i ||
	    *wfmt == i18n_parse_chars.w_o ||
	    *wfmt == i18n_parse_chars.w_x)
	{
		curField->fmt = 'd';
		curField->size = sizeof(int);
	}
	else if (*wfmt == i18n_parse_chars.w_u)
	{
		curField->fmt = 'u';
		curField->size = sizeof(int);
	}
	else if (*wfmt == i18n_parse_chars.w_s)
	{
		_WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto done;
		}
		curField->fmt = 's';
		curField->size = strlen(ctemp) + 1;
		ckfree(ctemp);
	}
	else if (*wfmt == i18n_parse_chars.w_c)
	{
                if (widthSpecified) 
		{
	            interp->result 
			= (char *)dce_msg_get(tcl_s_cmdmz_fld_width_not_be_specified_in_c_conv);
	            interp->freeProc = (Tcl_FreeProc *) free;

		    code = TCL_ERROR;
		    goto done;
                }
		curField->fmt = 'c';
		curField->size = sizeof(int);
	}
	else if (*wfmt == i18n_parse_chars.w_e ||
		 *wfmt == i18n_parse_chars.w_f ||
		 *wfmt == i18n_parse_chars.w_g)
	{
		wdst[-1] = i18n_parse_chars.w_l;
		wdst[0] = i18n_parse_chars.w_f;
		wdst++;
		curField->fmt = 'f';
		curField->size = sizeof(double);
	}
	else if (*wfmt == i18n_special_chars.w_op_sbr)
	{
		_WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
		if (st == (size_t)-1)
		{
		    return TCL_ERROR;
		}
		curField->fmt = 's';
		curField->size = strlen(ctemp) + 1;
		ckfree(ctemp);
		do {
		    wfmt++;
		    *wdst = *wfmt;
		    wdst++;
		} while (*wfmt != i18n_special_chars.w_cl_sbr);
	}
	else 
	{
		_WChar_To_Multibyte(interp, len, wfmt, ctemp, st)
		if (st == (size_t)-1)
		{
		    return TCL_ERROR;
		}
		i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_scan_conv_char);
		sprintf(interp->result, (char *)i18n_msg_format, *ctemp);
		free(i18n_msg_format);
		ckfree(ctemp);

		code = TCL_ERROR;
		goto done;
	}
	curField->size = TCL_ALIGN(curField->size);
	totalSize += curField->size;
    }
    *wdst = i18n_special_chars.w_esc_zero;

    if (numFields != (argc-3)) 
    {
	interp->result = (char *)dce_msg_get(tcl_s_cmdmz_diff_num_var_field);
	interp->freeProc = (Tcl_FreeProc *) free;

	code = TCL_ERROR;
	goto done;
    }

    /*
     * Step 2:
     */

    results = (char *) ckalloc((unsigned) totalSize);
    for (i = 0, totalSize = 0, curField = fields;
	    i < numFields; i++, curField++) 
    {
	curField->location = results + totalSize;
	totalSize += curField->size;
    }

    /*
     * Fill in the remaining fields with NULL;  the only purpose of
     * this is to keep some memory analyzers, like Purify, from
     * complaining.
     */

    for ( ; i < MAX_FIELDS; i++, curField++) 
    {
	curField->location = NULL;
    }

    /*
     * Step 3:
     */
    _WChar_To_Multibyte(interp, len, wargv[1], cargv1, st)
    if (st == (size_t)-1)
    {
	code = TCL_ERROR;
	goto done;
    }
    _WChar_To_Multibyte(interp, len, wfmtCopy, cfmtCopy, st)
    if (st == (size_t)-1)
    {
	code = TCL_ERROR;
	goto done;
    }

    numScanned = sscanf(cargv1, cfmtCopy,
	    fields[0].location, fields[1].location, fields[2].location,
	    fields[3].location, fields[4].location, fields[5].location,
	    fields[6].location, fields[7].location, fields[8].location,
	    fields[9].location, fields[10].location, fields[11].location,
	    fields[12].location, fields[13].location, fields[14].location,
	    fields[15].location, fields[16].location, fields[17].location,
	    fields[18].location, fields[19].location);

    ckfree(cargv1);
    ckfree(cfmtCopy);

    /*
     * Step 4:
     */

    if (numScanned < numFields) 
    {
	numFields = numScanned;
    }
    for (i = 0, curField = fields; i < numFields; i++, curField++) 
    {
	switch (curField->fmt) 
	{
	    char string[TCL_DOUBLE_SPACE];

	    case 'd':
		sprintf(string, "%d", *((int *) curField->location));

		_Multibyte_to_WChar(interp, len, string, wstring, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto done;
		}
		if (_Tcl_WSetVar(interp, wargv[i+3], wstring, 0) == NULL) 
		{
		    storeError:

		    _WChar_To_Multibyte(interp, len, wargv[i+3], ctemp, st)
		    if (st == (size_t)-1)
		    {
			ckfree(wstring);
		        code = TCL_ERROR;
		        goto done;
		    }
		    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_could_not_set_var);
		    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
		    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
		    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		    free(i18n_msg_format);
		    ckfree(i18n_msg_buf);
		    ckfree(wstring);
		    ckfree(ctemp);

		    code = TCL_ERROR;
		    goto done;
		}
		ckfree(wstring);
		break;

	    case 'u':
		sprintf(string, "%u", *((int *) curField->location));

		_Multibyte_to_WChar(interp, len, string, wstring, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto done;
		}
		if (_Tcl_WSetVar(interp, wargv[i+3], wstring, 0) == NULL) 
		{
		    ckfree(wstring);
		    goto storeError;
		}
		ckfree(wstring);
		break;

	    case 'c':
		sprintf(string, "%d", *((char *) curField->location) & 0xff);

		_Multibyte_to_WChar(interp, len, string, wstring, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto done;
		}
		if (_Tcl_WSetVar(interp, wargv[i+3], wstring, 0) == NULL) 
		{
		    ckfree(wstring);
		    goto storeError;
		}
		ckfree(wstring);
		break;

	    case 's':
		_Multibyte_to_WChar(interp, len, curField->location, wstring, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto done;
		}

		if (_Tcl_WSetVar(interp, wargv[i+3], wstring, 0) == NULL) 
		{
		    ckfree(wstring);
		    goto storeError;
		}
		ckfree(wstring);
		break;

	    case 'f':
		Tcl_PrintDouble(interp, *((double *) curField->location),
			string);

		_Multibyte_to_WChar(interp, len, string, wstring, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto done;
		}
		if (_Tcl_WSetVar(interp, wargv[i+3], wstring, 0) == NULL) 
		{
		    ckfree(wstring);
		    goto storeError;
		}
		ckfree(wstring);
		break;
	}
    }
    sprintf(interp->result, "%d", numScanned);
    done:
    if (results != NULL) 
    {
	ckfree(results);
    }
    if (wfmtCopy != wcopyBuf) 
    {
	ckfree(wfmtCopy);
    }
    return code;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSplitCmd --
 *
 *	This procedure is invoked to process the "split" Tcl command.
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
_Tcl_WSplitCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    wchar_t *wsplitChars;
    register wchar_t *wp, *wp2;
    wchar_t *welementStart;
    int alloc = 0;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc == 2) 
    {
	_Multibyte_to_WChar(interp, len, " \n\t\r", wsplitChars, st)
	if (st == (size_t)-1)
	return TCL_ERROR;
	alloc = 1;
    } 
    else if (argc == 3) 
    {
	wsplitChars = wargv[2];
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_str_splitchar);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("split")
			+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "split");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Handle the special case of splitting on every character.
     */

    if (*wsplitChars == i18n_special_chars.w_esc_zero) 
    {
	wchar_t wstring[2];
	wstring[1] = i18n_special_chars.w_esc_zero;
	for (wp = wargv[1]; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    wstring[0] = *wp;
	    _Tcl_WAppendElement(interp, wstring);
	}
	if (alloc)
	    ckfree(wsplitChars);

	return TCL_OK;
    }

    /*
     * Normal case: split on any of a given set of characters.
     * Discard instances of the split characters.
     */

    for (wp = welementStart = wargv[1]; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	wchar_t wc = *wp;
	for (wp2 = wsplitChars; *wp2 != i18n_special_chars.w_esc_zero; wp2++) 
	{
	    if (*wp2 == wc) 
	    {
		*wp = i18n_special_chars.w_esc_zero;
		_Tcl_WAppendElement(interp, welementStart);
		*wp = wc;
		welementStart = wp+1;
		break;
	    }
	}
    }
    if (wp != wargv[1]) 
    {
	_Tcl_WAppendElement(interp, welementStart);
    }
    if (alloc)
	ckfree(wsplitChars);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WStringCmd --
 *
 *	This procedure is invoked to process the "string" Tcl command.
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
_Tcl_WStringCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int length;
    register char c;
    int match;
    int first;
    int left = 0, right = 0;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp, *cargv2, *cargv3, *cargv4;
    wchar_t *wtemp;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_opt_arg_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    c = *ctemp;
    length = strlen(ctemp);
    if ((c == 'c') && (strncmp(ctemp, "compare", length) == 0)) 
    {
	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_comp_str_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	match = wcscmp(wargv[2], wargv[3]);
	if (match > 0) 
	{
	    interp->result = "1";
	} 
	else if (match < 0) 
	{
	    interp->result = "-1";
	} 
	else 
	{
	    interp->result = "0";
	}
	ckfree(ctemp);
	return TCL_OK;
    } 
    else if ((c == 'f') && (strncmp(ctemp, "first", length) == 0)) 
    {
	char *p;

	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_fst_str_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[3], cargv3, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}

	first = 1;

	firstLast:
	match = -1;
	c = *cargv2;
	length = strlen(cargv2);
	for (p = cargv3; *p != 0; p++) 
	{
	    if (*p != c) 
	    {
		continue;
	    }
	    if (strncmp(cargv2, p, length) == 0) 
	    {
		match = p-cargv3;
		if (first) 
		{
		    break;
		}
	    }
	}
	sprintf(interp->result, "%d", match);
	ckfree(ctemp);
	ckfree(cargv2);
	ckfree(cargv3);
	return TCL_OK;
    } 
    else if ((c == 'i') && (strncmp(ctemp, "index", length) == 0)) 
    {
	int index;

	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_idx_str_charidx);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	if (_Tcl_WGetInt(interp, wargv[3], &index) != TCL_OK) 
	{
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	if ((index >= 0) && (index < (int) strlen(cargv2))) 
	{
	    interp->result[0] = cargv2[index];
	    interp->result[1] = 0;
	}
	ckfree(ctemp);
	ckfree(cargv2);
	return TCL_OK;
    } 
    else if ((c == 'l') && (strncmp(ctemp, "last", length) == 0)
	    && (length >= 2)) 
    {
	if (argc != 4) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_last_str_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	first = 0;
	ckfree(ctemp);

	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[3], cargv3, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}

	goto firstLast;
    } 
    else if ((c == 'l') && (strncmp(ctemp, "length", length) == 0)
	    && (length >= 2)) 
    {
	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_len_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	sprintf(interp->result, "%d", strlen(cargv2));
	ckfree(ctemp);
	ckfree(cargv2);
	return TCL_OK;
    } 
    else if ((c == 'm') && (strncmp(ctemp, "match", length) == 0)) 
    {
	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_mat_pat_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	if (_Tcl_WStringMatch(interp, wargv[3], wargv[2]) != 0) 
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
    else if ((c == 'r') && (strncmp(ctemp, "range", length) == 0)) 
    {
	int first, last, stringLength;

	if (argc != 5) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_range_str_first);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	if (_Tcl_WGetInt(interp, wargv[3], &first) != TCL_OK) 
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	stringLength = strlen(cargv2);
	ckfree(ctemp);
	_WChar_To_Multibyte(interp, len, wargv[4], ctemp, st)
	if (st == (size_t)-1)
	{
	    ckfree(cargv2);
	    return TCL_ERROR;
	}

	if ((*ctemp == 'e')
		&& (strncmp(ctemp, "end", strlen(ctemp)) == 0)) 
	{
	    last = stringLength-1;
	} 
	else 
	{
	    if (_Tcl_WGetInt(interp, wargv[4], &last) != TCL_OK) 
	    {
		_WChar_To_Multibyte(interp, len, wargv[4], cargv4, st)
		if (st == (size_t)-1)
		{
		    ckfree(ctemp);
		    ckfree(cargv2);
		    return TCL_ERROR;
		}
		Tcl_ResetResult(interp);
		i18n_msg_format = dce_msg_get(tcl_s_cmdmz_expt_int_or_end);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(cargv4)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cargv4);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		ckfree(ctemp);
		ckfree(cargv2);
		ckfree(cargv4);

		return TCL_ERROR;
	    }
	}
	if (first < 0) 
	{
	    first = 0;
	}
	if (last >= stringLength) 
	{
	    last = stringLength-1;
	}
	if (last >= first) 
	{
	    char saved, *p;

	    p = cargv2 + last + 1;
	    saved = *p;
	    *p = 0;
	    Tcl_SetResult(interp, cargv2 + first, TCL_VOLATILE);
	    *p = saved;
	}
	ckfree(cargv2);
	ckfree(ctemp);
	return TCL_OK;
    } 
    else if ((c == 't') && (strncmp(ctemp, "tolower", length) == 0)
	    && (length >= 3)) 
    {
	register wchar_t *wp;
	wchar_t *wtemp;

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_lower_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}

	wtemp = ckalloc((wcslen(wargv[2]) + 1) * sizeof(wchar_t));
	wcscpy(wtemp, wargv[2]);

	for (wp = wtemp; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (iswupper(*wp)) 
	    {
		*wp = towlower(*wp);
	    }
	}
	_WChar_To_Multibyte(interp, len, wtemp, cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	Tcl_SetResult(interp, cargv2, TCL_VOLATILE);
	ckfree(ctemp);
	ckfree(wtemp);
	return TCL_OK;
    } 
    else if ((c == 't') && (strncmp(ctemp, "toupper", length) == 0)
	    && (length >= 3)) 
    {
	register wchar_t *wp;

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_upper_str);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}

	wtemp = ckalloc((wcslen(wargv[2]) + 1) * sizeof(wchar_t));
	wcscpy(wtemp, wargv[2]);

	for (wp = wtemp; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (iswlower(*wp)) 
	    {
		*wp = towupper(*wp);
	    }
	}
	_WChar_To_Multibyte(interp, len, wtemp, cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	Tcl_SetResult(interp, cargv2, TCL_VOLATILE);
	ckfree(ctemp);
	ckfree(wtemp);
	return TCL_OK;
    } 
    else if ((c == 't') && (strncmp(ctemp, "trim", length) == 0)
	    && (length == 4)) 
    {
	char *trimChars;
	register char *p, *checkPtr;
	int alloc = 0;

	left = right = 1;

	trim:
	if (argc == 4) 
	{
	    _WChar_To_Multibyte(interp, len, wargv[3], trimChars, st)
	    if (st == (size_t)-1)
	    {
		ckfree(ctemp);
		return TCL_ERROR;
	    }
	    alloc = 1;
	} 
	else if (argc == 3) 
	{
	    trimChars = " \t\n\r";
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_trim_str_chars);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("string")
					+ strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "string", ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, len, wargv[2], cargv2, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    if (alloc)
		ckfree(trimChars);
	    return TCL_ERROR;
	}
	p = cargv2;
	if (left) 
	{
	    for (c = *p; c != 0; p++, c = *p) 
	    {
		for (checkPtr = trimChars; *checkPtr != c; checkPtr++) 
		{
		    if (*checkPtr == 0) 
		    {
			goto doneLeft;
		    }
		}
	    }
	}
	doneLeft:
	Tcl_SetResult(interp, p, TCL_VOLATILE);
	ckfree(cargv2);
	if (right) 
	{
	    char *donePtr;

	    p = interp->result + strlen(interp->result) - 1;
	    donePtr = &interp->result[-1];
	    for (c = *p; p != donePtr; p--, c = *p) 
	    {
		for (checkPtr = trimChars; *checkPtr != c; checkPtr++) 
		{
		    if (*checkPtr == 0) 
		    {
			goto doneRight;
		    }
		}
	    }
	    doneRight:
	    p[1] = 0;
	}
	if (alloc)
	    ckfree(trimChars);
	return TCL_OK;
    } 
    else if ((c == 't') && (strncmp(ctemp, "trimleft", length) == 0)
	    && (length > 4)) 
    {
	left = 1;
	/* argv[1] = "trimleft"; */
	goto trim;
    } 
    else if ((c == 't') && (strncmp(ctemp, "trimright", length) == 0)
	    && (length > 4)) 
    {
	right = 1;
	/* argv[1] = "trimright"; */
	goto trim;
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_opt_string);
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

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSwitchCmd --
 *
 *	This procedure is invoked to process the "switch" Tcl command.
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
_Tcl_WSwitchCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
#define EXACT	0
#define GLOB	1
#define REGEXP	2
    int i, code, mode, matched;
    int body;
    wchar_t *wstring;
    int switchArgc, splitArgs;
    wchar_t **wswitchArgv;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    switchArgc = argc-1;
    wswitchArgv = wargv+1;
    mode = EXACT;
    _WChar_To_Multibyte(interp, len, wswitchArgv[0], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    while ((switchArgc > 0) && (*ctemp == '-')) 
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
	else if (strcmp(ctemp, "--") == 0) 
	{
	    switchArgc--;
	    wswitchArgv++;
	    break;
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_opt_switch);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	switchArgc--;
	wswitchArgv++;

	ckfree(ctemp);
	_WChar_To_Multibyte(interp, len, wswitchArgv[0], ctemp, st)
	if (st == (size_t)-1)
	    return TCL_ERROR;
    }
    if (switchArgc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_switch_str_pat);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("switch")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "switch");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    ckfree(ctemp);
    wstring = *wswitchArgv;
    switchArgc--;
    wswitchArgv++;

    /*
     * If all of the pattern/command pairs are lumped into a single
     * argument, split them out again.
     */

    splitArgs = 0;
    if (switchArgc == 1) 
    {
	code = _Tcl_WSplitList(interp, wswitchArgv[0], &switchArgc, &wswitchArgv);
	if (code != TCL_OK) 
	{
	    return code;
	}
	splitArgs = 1;
    }

    for (i = 0; i < switchArgc; i += 2) 
    {
	if (i == (switchArgc-1)) 
	{
	    interp->result = (char *)dce_msg_get(tcl_s_cmdmz_extra_switch_pat_no_body);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    code = TCL_ERROR;
	    goto cleanup;
	}

	/*
	 * See if the pattern matches the string.
	 */

	matched = 0;
	_WChar_To_Multibyte(interp, len, wswitchArgv[i], ctemp, st)
	if (st == (size_t)-1)
	{
	    code = TCL_ERROR;
	    goto cleanup;
	}
	if ((*ctemp == 'd') && (i == switchArgc-2)
		&& (strcmp(ctemp, "default") == 0)) 
	{
	    matched = 1;
	} 
	else 
	{
	    switch (mode) 
	    {
		case EXACT:
		    matched = (wcscmp(wstring, wswitchArgv[i]) == 0);
		    break;
		case GLOB:
		    matched = _Tcl_WStringMatch(interp, wstring, wswitchArgv[i]);
		    break;
		case REGEXP:
		    matched = _Tcl_WRegExpMatch(interp, wstring, wswitchArgv[i]);
		    if (matched < 0) 
		    {
			code = TCL_ERROR;
			ckfree(ctemp);
			goto cleanup;
		    }
		    break;
	    }
	}
	if (!matched) 
	{
	    continue;
	}
	ckfree(ctemp);

	/*
	 * We've got a match.  Find a body to execute, skipping bodies
	 * that are "-".
	 */

	for (body = i+1; ; body += 2) 
	{
	    if (body >= switchArgc) 
	    {
		_WChar_To_Multibyte(interp, len, wswitchArgv[0], ctemp, st)
		if (st == (size_t)-1)
		{
		    code = TCL_ERROR;
		    goto cleanup;
		}
		i18n_msg_format = dce_msg_get(tcl_s_cmdmz_no_body_spec_for_pat);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		ckfree(ctemp);

		code = TCL_ERROR;
		goto cleanup;
	    }
	    if ((wswitchArgv[body][0] != i18n_special_chars.w_minus) 
		    || (wswitchArgv[body][1] != i18n_special_chars.w_esc_zero)) 
	    {
		break;
	    }
	}
	code = _Tcl_WEval(interp, wswitchArgv[body]);
	if (code == TCL_ERROR) 
	{
	    char msg[100];
	    _WChar_To_Multibyte(interp, len, wswitchArgv[i], ctemp, st)
	    if (st == (size_t)-1)
	    {
		code = TCL_ERROR;
		goto cleanup;
	    }
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_arm_line);
	    sprintf(msg, (char *)i18n_msg_format, ctemp, interp->errorLine);
	    Tcl_AddErrorInfo(interp, msg);
	    free(i18n_msg_format);
	    ckfree(ctemp);
	}
	goto cleanup;
    }

    /*
     * Nothing matched:  return nothing.
     */

    code = TCL_OK;

    cleanup:
    if (splitArgs) 
    {
	ckfree((wchar_t *) wswitchArgv);
    }
    return code;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WTraceCmd --
 *
 *	This procedure is invoked to process the "trace" Tcl command.
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
_Tcl_WTraceCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    char c;
    int length;
    unsigned char *i18n_msg_buf, *i18n_msg_format;
    char *ctemp, *cargv4;
    size_t len, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_too_few_args_opt_arg_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("trace")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "trace");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    _WChar_To_Multibyte(interp, len, wargv[1], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    c = *(ctemp+1);
    length = strlen(ctemp);
    if ((c == 'a') && (strncmp(ctemp, "variable", length) == 0)
	    && (length >= 2)) 
    {
	wchar_t *wp;
	int flags, length;
	TraceVarInfo *tvarPtr;

	if (argc != 5) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_trace_var_name);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("trace")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "trace");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}

	flags = 0;
	for (wp = wargv[3] ; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (*wp == i18n_parse_chars.w_r) 
	    {
		flags |= TCL_TRACE_READS;
	    } 
	    else if (*wp == i18n_parse_chars.w_w) 
	    {
		flags |= TCL_TRACE_WRITES;
	    } 
	    else if (*wp == i18n_parse_chars.w_u) 
	    {
		flags |= TCL_TRACE_UNSETS;
	    } 
	    else 
	    {
		ckfree(ctemp);
		goto badOps;
	    }
	}
	if (flags == 0) {
	    ckfree(ctemp);
	    goto badOps;
	}

	_WChar_To_Multibyte(interp, len, wargv[4], cargv4, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}

	length = strlen(cargv4);
	tvarPtr = (TraceVarInfo *) ckalloc((unsigned)
		(sizeof(TraceVarInfo) - sizeof(tvarPtr->command) + length + 1));
	tvarPtr->flags = flags;
	tvarPtr->errMsg = NULL;
	tvarPtr->length = length;
	flags |= TCL_TRACE_UNSETS;
	strcpy(tvarPtr->command, cargv4);
	if (_Tcl_WTraceVar(interp, wargv[2], flags, _WTraceVarProc,
		(ClientData) tvarPtr) != TCL_OK) 
	{
	    ckfree((char *) tvarPtr);
	    ckfree(ctemp);
	    ckfree(cargv4);
	    return TCL_ERROR;
	}
	ckfree(cargv4);
	ckfree(ctemp);
    } 
    else if ((c == 'd') && (strncmp(ctemp, "vdelete", length)
	    && (length >= 2)) == 0) 
    {
	wchar_t *wp;
	int flags, length;
	TraceVarInfo *tvarPtr;
	ClientData clientData;

	if (argc != 5) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_trace_vdel_name);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("trace")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "trace");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}

	flags = 0;
	for (wp = wargv[3] ; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (*wp == i18n_parse_chars.w_r) 
	    {
		flags |= TCL_TRACE_READS;
	    } 
	    else if (*wp == i18n_parse_chars.w_w) 
	    {
		flags |= TCL_TRACE_WRITES;
	    } 
	    else if (*wp == i18n_parse_chars.w_u) 
	    {
		flags |= TCL_TRACE_UNSETS;
	    } 
	    else 
	    {
		ckfree(ctemp);
		goto badOps;
	    }
	}
	if (flags == 0) {
	    ckfree(ctemp);
	    goto badOps;
	}

	/*
	 * Search through all of our traces on this variable to
	 * see if there's one with the given command.  If so, then
	 * delete the first one that matches.
	 */

	_WChar_To_Multibyte(interp, len, wargv[4], cargv4, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}

	length = strlen(cargv4);
	clientData = 0;
	while ((clientData = _Tcl_WVarTraceInfo(interp, wargv[2], 0,
		_WTraceVarProc, clientData)) != 0) 
	{
	    tvarPtr = (TraceVarInfo *) clientData;
	    if ((tvarPtr->length == length) && (tvarPtr->flags == flags)
		    && (strncmp(cargv4, tvarPtr->command, length) == 0)) 
	    {
		_Tcl_WUntraceVar(interp, wargv[2], flags | TCL_TRACE_UNSETS,
			_WTraceVarProc, clientData);
		if (tvarPtr->errMsg != NULL) 
		{
		    ckfree(tvarPtr->errMsg);
		}
		ckfree((char *) tvarPtr);
		break;
	    }
	}
	ckfree(ctemp);
	ckfree(cargv4);
    } 
    else if ((c == 'i') && (strncmp(ctemp, "vinfo", length) == 0)
	    && (length >= 2)) 
    {
	ClientData clientData;
	char ops[4], *p;
	char *prefix = "{";

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_trace_vinfo_name);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("trace")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "trace");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    return TCL_ERROR;
	}
	clientData = 0;
	while ((clientData = _Tcl_WVarTraceInfo(interp, wargv[2], 0,
		_WTraceVarProc, clientData)) != 0) 
	{
	    TraceVarInfo *tvarPtr = (TraceVarInfo *) clientData;
	    p = ops;
	    if (tvarPtr->flags & TCL_TRACE_READS) 
	    {
		*p = 'r';
		p++;
	    }
	    if (tvarPtr->flags & TCL_TRACE_WRITES) 
	    {
		*p = 'w';
		p++;
	    }
	    if (tvarPtr->flags & TCL_TRACE_UNSETS) 
	    {
		*p = 'u';
		p++;
	    }
	    *p = '\0';
	    Tcl_AppendResult(interp, prefix, (char *) NULL);
	    Tcl_AppendElement(interp, ops);
	    Tcl_AppendElement(interp, tvarPtr->command);
	    Tcl_AppendResult(interp, "}", (char *) NULL);
	    prefix = " {";
	}
	ckfree(ctemp);
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_opt_var_vdel_vinfo);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    return TCL_OK;

    badOps:
    _WChar_To_Multibyte(interp, len, wargv[3], ctemp, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    i18n_msg_format = dce_msg_get(tcl_s_cmdmz_bad_opr_more_rwu);
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
 *----------------------------------------------------------------------
 *
 * _WTraceVarProc --
 *
 *	This procedure is called to handle variable accesses that have
 *	been traced using the "trace" command.
 *
 * Results:
 *	Normally returns NULL.  If the trace command returns an error,
 *	then this procedure returns an error string.
 *
 * Side effects:
 *	Depends on the command associated with the trace.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
static char *
_WTraceVarProc(clientData, interp, name1, name2, flags)
    ClientData clientData;	/* Information about the variable trace. */
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    char *name1;		/* Name of variable or array. */
    char *name2;		/* Name of element within array;  NULL means
				 * scalar variable is being referenced. */
    int flags;			/* OR-ed bits giving operation and other
				 * information. */
{
    TraceVarInfo *tvarPtr = (TraceVarInfo *) clientData;
    char *result;
    int code;
    Interp dummy;
    Tcl_DString cmd;
    char *p;
    wchar_t *wtemp;
    size_t len, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    result = NULL;
    if (tvarPtr->errMsg != NULL) 
    {
	ckfree(tvarPtr->errMsg);
	tvarPtr->errMsg = NULL;
    }
    if ((tvarPtr->flags & flags) && !(flags & TCL_INTERP_DESTROYED)) 
    {
	/*
	 * Generate a command to execute by appending list elements
	 * for the two variable names and the operation.  The five
	 * extra characters are for three space, the opcode character,
	 * and the terminating null.
	 */

	if (name2 == NULL) 
	{
	    name2 = "";
	}
	Tcl_DStringInit(&cmd);
	Tcl_DStringAppend(&cmd, tvarPtr->command, tvarPtr->length);
	Tcl_DStringAppendElement(&cmd, name1);
	Tcl_DStringAppendElement(&cmd, name2);
	if (flags & TCL_TRACE_READS) 
	{
	    Tcl_DStringAppend(&cmd, " r", 2);
	} 
	else if (flags & TCL_TRACE_WRITES) 
	{
	    Tcl_DStringAppend(&cmd, " w", 2);
	} 
	else if (flags & TCL_TRACE_UNSETS) 
	{
	    Tcl_DStringAppend(&cmd, " u", 2);
	}

	/*
	 * Execute the command.  Be careful to save and restore the
	 * result from the interpreter used for the command.
	 */

	if (interp->freeProc == 0) 
	{
	    dummy.freeProc = (Tcl_FreeProc *) 0;
	    dummy.result = "";
	    Tcl_SetResult((Tcl_Interp *) &dummy, interp->result, TCL_VOLATILE);
	} 
	else 
	{
	    dummy.freeProc = interp->freeProc;
	    dummy.result = interp->result;
	    interp->freeProc = (Tcl_FreeProc *) 0;
	}
	p = Tcl_DStringValue(&cmd);
	_Multibyte_to_WChar(interp, len, p, wtemp, st)
	if (st == (size_t)-1)
	{
	    Tcl_DStringFree(&cmd);
	    return NULL;
	}

	code = _Tcl_WEval(interp, wtemp);
	ckfree(wtemp);

	Tcl_DStringFree(&cmd);
	if (code != TCL_OK) 
	{
	    tvarPtr->errMsg = ckalloc((unsigned) (strlen(interp->result) + 1));
	    strcpy(tvarPtr->errMsg, interp->result);
	    result = tvarPtr->errMsg;
	    Tcl_ResetResult(interp);		/* Must clear error state. */
	}
	Tcl_SetResult(interp, dummy.result,
		(dummy.freeProc == 0) ? TCL_VOLATILE : dummy.freeProc);
    }
    if (flags & TCL_TRACE_DESTROYED) 
    {
	result = NULL;
	if (tvarPtr->errMsg != NULL) 
	{
	    ckfree(tvarPtr->errMsg);
	}
	ckfree((char *) tvarPtr);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WWhileCmd --
 *
 *	This procedure is invoked to process the "while" Tcl command.
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
_Tcl_WWhileCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int result, value;
    unsigned char *i18n_msg_buf, *i18n_msg_format;

    if (argc != 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_cmdmz_wnum_args_while_test_cmd);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("while")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "while");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    while (1) 
    {
	result = _Tcl_WExprBoolean(interp, wargv[1], &value);
	if (result != TCL_OK) 
	{
	    return result;
	}
	if (!value) 
	{
	    break;
	}
	result = _Tcl_WEval(interp, wargv[2]);
	if ((result != TCL_OK) && (result != TCL_CONTINUE)) 
	{
	    if (result == TCL_ERROR) 
	    {
		char msg[120];
		i18n_msg_format = dce_msg_get(tcl_s_cmdmz_while_body_line);
		sprintf(msg, (char *)i18n_msg_format, interp->errorLine);
		free(i18n_msg_format);

		Tcl_AddErrorInfo(interp, msg);
	    }
	    break;
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
