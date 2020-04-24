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
 * $Log: tclGet.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.6.1  1995/02/16  22:00:22  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/16  20:40:15  levy]
 *
 * Revision 1.1.4.1  1994/10/21  21:39:03  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:31:23  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:11  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:10:19  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclGet.c --
 *
 *	This file contains procedures to convert strings into
 *	other forms, like integers or floating-point numbers or
 *	booleans, doing syntax checking along the way.
 *
 * Copyright (c) 1990-1993 The Regents of the University of California.
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
#include <langinfo.h>

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetInt --
 *
 *	Given a string, produce the corresponding integer value.
 *
 * Results:
 *	The return value is normally TCL_OK;  in this case *intPtr
 *	will be set to the integer value equivalent to string.  If
 *	string is improperly formed then TCL_ERROR is returned and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_GetInt(interp, string, intPtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    char *string;		/* String containing a (possibly signed)
				 * integer in a form acceptable to strtol. */
    int *intPtr;		/* Place to store converted result. */
{
    char *end, *p;
    int i;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Note: use strtoul instead of strtol for integer conversions
     * to allow full-size unsigned numbers, but don't depend on strtoul
     * to handle sign characters;  it won't in some implementations.
     */

    for (p = string; isspace(UCHAR(*p)); p++) {
	/* Empty loop body. */
    }
    if (*p == '-') {
	i = -strtoul(p+1, &end, 0);
    } else if (*p == '+') {
	i = strtoul(p+1, &end, 0);
    } else {
	i = strtoul(p, &end, 0);
    }
    while ((*end != '\0') && isspace(UCHAR(*end))) {
	end++;
    }
    if ((end == string) || (*end != 0)) {
	/* i18n
	Tcl_AppendResult(interp, "expected integer but got \"", string,
		"\"", (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_get_exp_integer_but);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    *intPtr = i;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetDouble --
 *
 *	Given a string, produce the corresponding double-precision
 *	floating-point value.
 *
 * Results:
 *	The return value is normally TCL_OK;  in this case *doublePtr
 *	will be set to the double-precision value equivalent to string.
 *	If string is improperly formed then TCL_ERROR is returned and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_GetDouble(interp, string, doublePtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    char *string;		/* String containing a floating-point number
				 * in a form acceptable to strtod. */
    double *doublePtr;		/* Place to store converted result. */
{
    char *end;
    double d;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    d = strtod(string, &end);
    while ((*end != '\0') && isspace(UCHAR(*end))) {
	end++;
    }
    if ((end == string) || (*end != 0)) {
	/* i18n
	Tcl_AppendResult(interp, "expected floating-point number but got \"",
		string, "\"", (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_get_exp_float_but);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    *doublePtr = d;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetBoolean --
 *
 *	Given a string, return a 0/1 boolean value corresponding
 *	to the string.
 *
 * Results:
 *	The return value is normally TCL_OK;  in this case *boolPtr
 *	will be set to the 0/1 value equivalent to string.  If
 *	string is improperly formed then TCL_ERROR is returned and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_GetBoolean(interp, string, boolPtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    char *string;		/* String containing a boolean number
				 * specified either as 1/0 or true/false or
				 * yes/no. */
    int *boolPtr;		/* Place to store converted result, which
				 * will be 0 or 1. */
{
    char c;
    char lowerCase[10];
    int i, length;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Convert the input string to all lower-case.
     */

    for (i = 0; i < 9; i++) {
	c = string[i];
	if (c == 0) {
	    break;
	}
	if ((c >= 'A') && (c <= 'Z')) {
	    c += 'a' - 'A';
	}
	lowerCase[i] = c;
    }
    lowerCase[i] = 0;

    length = strlen(lowerCase);
    c = lowerCase[0];
    if ((c == '0') && (lowerCase[1] == '\0')) {
	*boolPtr = 0;
    } else if ((c == '1') && (lowerCase[1] == '\0')) {
	*boolPtr = 1;
    } else if ((c == 'y') && (strncmp(lowerCase, "yes", length) == 0)) {
	*boolPtr = 1;
    } else if ((c == 'n') && (strncmp(lowerCase, "no", length) == 0)) {
	*boolPtr = 0;
    } else if ((c == 't') && (strncmp(lowerCase, "true", length) == 0)) {
	*boolPtr = 1;
    } else if ((c == 'f') && (strncmp(lowerCase, "false", length) == 0)) {
	*boolPtr = 0;
    } else if ((c == 'o') && (length >= 2)) {
	if (strncmp(lowerCase, "on", length) == 0) {
	    *boolPtr = 1;
	} else if (strncmp(lowerCase, "off", length) == 0) {
	    *boolPtr = 0;
	}
    } else {
	/* i18n
	Tcl_AppendResult(interp, "expected boolean value but got \"",
		string, "\"", (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_get_exp_boolean_but);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGetInt --
 *
 *	Given a string, produce the corresponding integer value.
 *
 * Results:
 *	The return value is normally TCL_OK;  in this case *intPtr
 *	will be set to the integer value equivalent to string.  If
 *	string is improperly formed then TCL_ERROR is returned and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WGetInt(interp, wstring, intPtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    wchar_t *wstring;		/* String containing a (possibly signed)
				 * integer in a form acceptable to strtol. */
    int *intPtr;		/* Place to store converted result. */
{
    wchar_t *wend, *wp;
    int i;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * Note: use strtoul instead of strtol for integer conversions
     * to allow full-size unsigned numbers, but don't depend on strtoul
     * to handle sign characters;  it won't in some implementations.
     */

    for (wp = wstring; iswspace(*wp); wp++) {
	/* Empty loop body. */
    }
    if (*wp == i18n_special_chars.w_minus) 
    {
	i = -wcstoul(wp+1, &wend, 0);
    } 
    else if (*wp == i18n_special_chars.w_plus) 
    {
	i = wcstoul(wp+1, &wend, 0);
    } 
    else 
    {
	i = wcstoul(wp, &wend, 0);
    }
    while ((*wend != i18n_special_chars.w_esc_zero) 
			&& iswspace(*wend)) 
    {
	wend++;
    }
    if ((wend == wstring) || (*wend != i18n_special_chars.w_esc_zero)) 
    {
	char *ctemp;
	size_t clen, st;

	_WChar_To_Multibyte(interp, clen, wstring, ctemp, st)
	if (st == (size_t)-1)
		return TCL_CONV_ERROR;

	i18n_msg_format = dce_msg_get(tcl_s_get_exp_integer_but);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    *intPtr = i;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGetDouble --
 *
 *	Given a string, produce the corresponding double-precision
 *	floating-point value.
 *
 * Results:
 *	The return value is normally TCL_OK;  in this case *doublePtr
 *	will be set to the double-precision value equivalent to string.
 *	If string is improperly formed then TCL_ERROR is returned and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WGetDouble(interp, wstring, doublePtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    wchar_t *wstring;		/* String containing a floating-point number
				 * in a form acceptable to strtod. */
    double *doublePtr;		/* Place to store converted result. */
{
    wchar_t *wend;
    double d;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    d = wcstod(wstring, &wend);
    while ((*wend != i18n_special_chars.w_esc_zero) && iswspace(*wend)) 
    {
	wend++;
    }
    if ((wend == wstring) || (*wend != i18n_parse_chars.w_0)) 
    {
	char *ctemp;
	size_t clen, st;

	_WChar_To_Multibyte(interp, clen, wstring, ctemp, st)
	if (st == (size_t)-1)
		return TCL_CONV_ERROR;

	i18n_msg_format = dce_msg_get(tcl_s_get_exp_float_but);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    *doublePtr = d;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGetBoolean --
 *
 *	Given a string, return a 0/1 boolean value corresponding
 *	to the string.
 *
 * Results:
 *	The return value is normally TCL_OK;  in this case *boolPtr
 *	will be set to the 0/1 value equivalent to string.  If
 *	string is improperly formed then TCL_ERROR is returned and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WGetBoolean(interp, wstring, boolPtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    wchar_t *wstring;		/* String containing a boolean number
				 * specified either as 1/0 or true/false or
				 * yes/no. */
    int *boolPtr;		/* Place to store converted result, which
				 * will be 0 or 1. */
{
    wchar_t wc;
    wchar_t wlowerCase[10];
    int i, length;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;
    char *cp, c;
    size_t clen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * Convert the input string to all lower-case.
     */

    for (i = 0; i < 9; i++) 
    {
	wc = wstring[i];
	if (wc == i18n_special_chars.w_esc_zero) 
	{
	    break;
	}
	if (iswupper(wc))
	{
	    wc = towlower(wc);
	}
	wlowerCase[i] = wc;
    }
    wlowerCase[i] = i18n_special_chars.w_esc_zero;

    _WChar_To_Multibyte(interp, clen, wlowerCase, cp, st)
    if (st == (size_t)-1)
 	return TCL_CONV_ERROR;

    length = strlen(cp);

    c = *cp;
    wc = wlowerCase[0];
    if ((wc == i18n_special_chars.w_esc_zero) 
		&& (wlowerCase[1] == i18n_special_chars.w_esc_zero)) 
    {
	*boolPtr = 0;
    } 
    else if ((c == '1') && (cp[1] == '\0')) 
    {
	*boolPtr = 1;
    } 
    else if ((c == 'y') && (strncmp(cp, "yes", length) == 0)) 
    {
	*boolPtr = 1;
    } 
    else if ((c == 'n') && (strncmp(cp, "no", length) == 0)) 
    {
	*boolPtr = 0;
    } 
    else if ((c == 't') && (strncmp(cp, "true", length) == 0)) 
    {
	*boolPtr = 1;
    } 
    else if ((c == 'f') && (strncmp(cp, "false", length) == 0)) 
    {
	*boolPtr = 0;
    } 
    else if ((c == 'o') && (length >= 2)) 
    {
	if (strncmp(cp, "on", length) == 0) 
	{
	    *boolPtr = 1;
	} 
	else if (strncmp(cp, "off", length) == 0) 
	{
	    *boolPtr = 0;
	}
    } 
    else 
    {
	char *ctemp;

	ctemp = nl_langinfo(YESSTR);
	if (strncmp(cp, ctemp, strlen(ctemp) == 0))
	{
	    *boolPtr = 1;
	    goto done;
	}

	ctemp = nl_langinfo(NOSTR);
	if (strncmp(cp, ctemp, strlen(ctemp) == 0))
	{
	    *boolPtr = 0;
	    goto done;
	}

	_WChar_To_Multibyte(interp, clen, wstring, ctemp, st)
	if (st == (size_t)-1)
	{
		ckfree(cp);
		return TCL_CONV_ERROR;
	}

	i18n_msg_format = dce_msg_get(tcl_s_get_exp_boolean_but);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(cp);

	return TCL_ERROR;
    }
done:
    ckfree(cp);
    return TCL_OK;
}
