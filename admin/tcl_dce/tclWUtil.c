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
 * $Log: tclWUtil.c,v $
 * Revision 1.1.6.3  1996/02/18  19:19:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:17  marty]
 *
 * Revision 1.1.6.2  1995/12/08  21:35:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/10  14:49 UTC  dnguyen
 * 	Merge up I18N'ed TCL fix.
 * 
 * 	HP revision /main/dnguyen_mothra/1  1995/04/07  15:15 UTC  dnguyen
 * 	In _Tcl_WScanElement, return the number of characters
 * 	multiplied by sizeof(wchar_t), not MB_CUR_MAX
 * 	[1995/12/08  18:06:53  root]
 * 
 * Revision 1.1.2.1  1994/10/21  21:39:52  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:35:57  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclWUtil.c --
 *
 *	This file contains utility procedures that are used by many Tcl
 *	commands (specific to wide character processing).
 */

#include <tcl.h>
#include "tclInt.h"
#include "tclWInt.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * The following values are used in the flags returned by Tcl_ScanElement
 * and used by Tcl_ConvertElement.  The value TCL_DONT_USE_BRACES is also
 * defined in tcl.h;  make sure its value doesn't overlap with any of the
 * values below.
 *
 * TCL_DONT_USE_BRACES -        1 means the string mustn't be enclosed in
 *                              braces (e.g. it contains unmatched braces,
 *                              or ends in a backslash character, or user
 *                              just doesn't want braces);  handle all
 *                              special characters by adding backslashes.
 * USE_BRACES -                 1 means the string contains a special
 *                              character that can be handled simply by
 *                              enclosing the entire argument in braces.
 * BRACES_UNMATCHED -           1 means that braces aren't properly matched
 *                              in the argument.
 */

#define USE_BRACES              2
#define BRACES_UNMATCHED        4



/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WScanElement --
 *
 *	This is a wide character version of Tcl_ScanElement.
 *
 *	This procedure is a companion procedure to Tcl_ConvertElement.
 *	It scans a string to see what needs to be done to it (e.g.
 *	add backslashes or enclosing braces) to make the string into
 *	a valid Tcl list element.
 *
 * Results:
 *	The return value is an overestimate of the number of characters
 *	that will be needed by Tcl_ConvertElement to produce a valid
 *	list element from string.  The word at *flagPtr is filled in
 *	with a value needed by Tcl_ConvertElement when doing the actual
 *	conversion.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WScanElement(interp, wstring, flagPtr, conv_err)
    Tcl_Interp *interp;		/* Current interpreter. */
    wchar_t *wstring;		/* String to convert to Tcl list element. */
    int *flagPtr;		/* Where to store information to guide
				 * Tcl_ConvertElement. */
    int *conv_err;
{
    int flags, nestingLevel;
    wchar_t *wp;

    nestingLevel = 0;
    flags = 0;
    *conv_err = TCL_OK;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
		*conv_err = TCL_CONV_ERROR;
		return 0;
    }

    if (wstring == (wchar_t)NULL) 
    {
	*wstring = i18n_special_chars.w_esc_zero;
    }

    wp = wstring;
    if ((*wp == i18n_special_chars.w_op_cbra) 
		|| (*wp == i18n_special_chars.w_dob_qo) 
		|| (*wp == i18n_special_chars.w_esc_zero))
    {
	flags |= USE_BRACES;
    }
    for ( ; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_cbra)
	{
		nestingLevel++;
	}
	else if (*wp == i18n_special_chars.w_cl_cbra)
	{
		nestingLevel--;
		if (nestingLevel < 0) {
		    flags |= TCL_DONT_USE_BRACES|BRACES_UNMATCHED;
		}
	}
	else if (*wp == i18n_special_chars.w_op_sbr ||
		 *wp == i18n_special_chars.w_dollar ||
		 *wp == i18n_special_chars.w_sem_col ||
		 *wp == i18n_special_chars.w_space ||
		 *wp == i18n_special_chars.w_esc_f ||
		 *wp == i18n_special_chars.w_esc_n ||
		 *wp == i18n_special_chars.w_esc_r ||
		 *wp == i18n_special_chars.w_esc_t ||
		 *wp == i18n_special_chars.w_esc_v)
	{
		flags |= USE_BRACES;
	}
	else if (*wp == i18n_special_chars.w_bsla)
	{
		if ((wp[1] == i18n_special_chars.w_esc_zero) 
			|| (wp[1] == i18n_special_chars.w_esc_n)) 
		{
		    flags = TCL_DONT_USE_BRACES;
		} 
		else 
		{
		    int size;
		    wchar_t err;

		    err = _Tcl_WBackslash(interp, wp, &size);
		    if (err == i18n_special_chars.w_null)
				return TCL_CONV_ERROR;
		    wp += size-1;
		    flags |= USE_BRACES;
		}
	}
    }
    if (nestingLevel != 0) 
    {
	flags = TCL_DONT_USE_BRACES | BRACES_UNMATCHED;
    }
    *flagPtr = flags;

    /*
     * Allow enough space to backslash every character plus leave
     * two spaces for braces.
     */

    return (2 * (wp - wstring) + 2) * sizeof(wchar_t);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WConvertElement --
 *
 *	This is an internal procedure for wide character processing.
 *	This is an equivalent of Tcl_ConvertElement().
 *
 *	This is a companion procedure to Tcl_ScanElement.  Given the
 *	information produced by Tcl_ScanElement, this procedure converts
 *	a string to a list element equal to that string.
 *
 * Results:
 *	Information is copied to *dst in the form of a list element
 *	identical to src (i.e. if Tcl_SplitList is applied to dst it
 *	will produce a string identical to src).  The return value is
 *	a count of the number of wide characters copied (not including the
 *	terminating NULL character).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WConvertElement(interp, wsrc, wdst, flags, err)
    Tcl_Interp *interp;		/* Current interpreter. */
    wchar_t *wsrc;		/* Source information for list element. */
    wchar_t *wdst;		/* Place to put list-ified element. */
    int flags;			/* Flags produced by Tcl_ScanElement. */
    int *err;			/* Indicate if conversion error occured */
{
    wchar_t *wp = wdst;
    *err = TCL_OK;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
		*err = TCL_CONV_ERROR;
		return 0;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
		*err = TCL_CONV_ERROR;
		return 0;
    }

    if (wsrc == (wchar_t)NULL) {
	*wsrc = i18n_special_chars.w_esc_zero;
    }

    if ((flags & USE_BRACES) && !(flags & TCL_DONT_USE_BRACES)) {
	*wp = i18n_special_chars.w_op_cbra;
	wp++;
	for ( ; *wsrc != i18n_special_chars.w_esc_zero; wsrc++, wp++) {
	    *wp = *wsrc;
	}
	*wp = i18n_special_chars.w_cl_cbra;
	wp++;
    } 
    else if (*wsrc == i18n_special_chars.w_esc_zero) 
    {
	/*
	 * If string is empty but can't use braces, then use special
	 * backslash sequence that maps to empty string.
	 */

	wp[0] = i18n_special_chars.w_bsla;
	wp[1] = i18n_parse_chars.w_0;
	wp += 2;
    } else {
	if (*wsrc == i18n_special_chars.w_op_cbra) {
	    /*
	     * Can't have a leading brace unless the whole element is
	     * enclosed in braces.  Add a backslash before the brace.
	     * Furthermore, this may destroy the balance between open
	     * and close braces, so set BRACES_UNMATCHED.
	     */

	    wp[0] = i18n_special_chars.w_bsla;
	    wp[1] = i18n_special_chars.w_op_cbra;
	    wp += 2;
	    wsrc++;
	    flags |= BRACES_UNMATCHED;
	}
	for (; *wsrc != i18n_special_chars.w_esc_zero ; wsrc++) 
	{
		if (*wsrc == i18n_special_chars.w_cl_sbr ||
		    *wsrc == i18n_special_chars.w_op_sbr ||
		    *wsrc == i18n_special_chars.w_dollar ||
		    *wsrc == i18n_special_chars.w_sem_col ||
		    *wsrc == i18n_special_chars.w_space ||
		    *wsrc == i18n_special_chars.w_bsla ||
		    *wsrc == i18n_special_chars.w_dob_qo)
		{
		    *wp = i18n_special_chars.w_bsla;
		    wp++;
		}
		else if (*wsrc == i18n_special_chars.w_op_cbra ||
			 *wsrc == i18n_special_chars.w_cl_cbra)
		{
		    /*
		     * It may not seem necessary to backslash braces, but
		     * it is.  The reason for this is that the resulting
		     * list element may actually be an element of a sub-list
		     * enclosed in braces (e.g. if Tcl_DStringStartSublist
		     * has been invoked), so there may be a brace mismatch
		     * if the braces aren't backslashed.
		     */

		    if (flags & BRACES_UNMATCHED) {
			*wp = i18n_special_chars.w_bsla;
			wp++;
		    }
		}
		else if (*wsrc == i18n_special_chars.w_esc_f)
		{
		    *wp = i18n_special_chars.w_bsla;
		    wp++;
		    *wp = i18n_parse_chars.w_f;
		    wp++;
		    continue;
		}
		else if (*wsrc == i18n_special_chars.w_esc_n)
		{
		    *wp = i18n_special_chars.w_bsla;
		    wp++;
		    *wp = i18n_parse_chars.w_n;
		    wp++;
		    continue;
		}
		else if (*wsrc == i18n_special_chars.w_esc_r)
		{
		    *wp = i18n_special_chars.w_bsla;
		    wp++;
		    *wp = i18n_parse_chars.w_r;
		    wp++;
		    continue;
		}
		else if (*wsrc == i18n_special_chars.w_esc_t)
		{
		    *wp = i18n_special_chars.w_bsla;
		    wp++;
		    *wp = i18n_parse_chars.w_t;
		    wp++;
		    continue;
		}
		else if (*wsrc == i18n_special_chars.w_esc_v)
		{
		    *wp = i18n_special_chars.w_bsla;
		    wp++;
		    *wp = i18n_parse_chars.w_v;
		    wp++;
		    continue;
		}
	    *wp = *wsrc;
	    wp++;
	}
    }
    *wp = i18n_special_chars.w_esc_zero;
    return wp - wdst;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WStringMatch --
 *
 *      See if a particular wide string matches a particular pattern.
 *	This is an internal routine to process wide character string matching.
 *
 * Results:
 *      The return value is 1 if wide string matches pattern, and
 *      0 otherwise.  The matching operation permits the following
 *      special characters in the pattern: *?\[] (see the manual
 *      entry for details on what these mean).
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WStringMatch(interp, wstring, wpattern)
    Tcl_Interp *interp;		/* Current interpreter. */
    wchar_t *wstring;		/* String. */
    wchar_t *wpattern;		/* Pattern, which may contain
				 * special characters. */
{
    wchar_t c2;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
		return TCL_CONV_ERROR;
    }

    while (1) {
        /* See if we're at the end of both the pattern and the string.
         * If so, we succeeded.  If we're at the end of the pattern
         * but not at the end of the string, we failed.
         */

        if (*wpattern == i18n_special_chars.w_esc_zero) {
            if (*wstring == i18n_special_chars.w_esc_zero) {
                return 1;
            } else {
                return 0;
            }
        }
        if ((*wstring == i18n_special_chars.w_esc_zero) 
		&& (*wpattern != i18n_special_chars.w_star)) {
            return 0;
        }

        /* Check for a "*" as the next pattern character.  It matches
         * any substring.  We handle this by calling ourselves
         * recursively for each postfix of string, until either we
         * match or we reach the end of the string.
         */

        if (*wpattern == i18n_special_chars.w_star) {
            wpattern += 1;
            if (*wpattern == i18n_special_chars.w_esc_zero) {
                return 1;
            }
            while (1) {
		if (_Tcl_WStringMatch(interp, wstring, wpattern)) 
		{
                    return 1;
                }
                if (*wstring == i18n_special_chars.w_esc_zero) {
                    return 0;
                }
                wstring += 1;
            }
        }

        /* Check for a "?" as the next pattern character.  It matches
         * any single character.
         */

        if (*wpattern == i18n_special_chars.w_question) 
	{
            goto thisCharOK;
        }

        /* Check for a "[" as the next pattern character.  It is followed
         * by a list of characters that are acceptable, or by a range
         * (two characters separated by "-").
         */

        if (*wpattern == i18n_special_chars.w_op_sbr) 
	{
            wpattern += 1;
            while (1) {
                if ((*wpattern == i18n_special_chars.w_cl_sbr) 
			|| (*wpattern == i18n_special_chars.w_esc_zero)) {
                    return 0;
                }
                if (*wpattern == *wstring) {
                    break;
                }
                if (wpattern[1] == i18n_special_chars.w_minus) {
                    c2 = wpattern[2];
                    if (c2 == i18n_special_chars.w_esc_zero) {
                        return 0;
                    }
		    /* assuming character continuation.  potential problem! */
                    if ((*wpattern <= *wstring) && (c2 >= *wstring)) {
                        break;
                    }
                    if ((*wpattern >= *wstring) && (c2 <= *wstring)) {
                        break;
                    }
                    wpattern += 2;
                }
                wpattern += 1;
            }
            while ((*wpattern != i18n_special_chars.w_cl_sbr) 
			&& (*wpattern != i18n_special_chars.w_esc_zero)) {
                wpattern += 1;
            }
            goto thisCharOK;
        }

        /* If the next pattern character is '/', just strip off the '/'
         * so we do exact matching on the character that follows.
         */

        if (*wpattern == i18n_special_chars.w_slash) {
            wpattern += 1;
            if (*wpattern == i18n_special_chars.w_esc_zero) {
                return 0;
            }
        }

        /* There's no special character.  Just make sure that the next
         * characters of each string match.
         */

        if (*wpattern != *wstring) {
            return 0;
        }

        thisCharOK: wpattern += 1;
        wstring += 1;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WAppendElement --
 *
 *      Convert a string to a valid Tcl list element and append it
 *      to the current result (which is ostensibly a list).
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      The result in the interpreter given by the first argument
 *      is extended with a list element converted from string.  A
 *      separator space is added before the converted list element
 *      unless the current result is empty, contains the single
 *      character "{", or ends in " {".
 *
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WAppendElement(interp, wstring)
    Tcl_Interp *interp;		/* Interpreter whose result is to be
				 * extended. */
    wchar_t *wstring;		/* String to convert to list element and
				 * add to result. */
{
    register Interp *iPtr = (Interp *) interp;
    int size, flags, conv_err;
    wchar_t *wdst, wtemp1, wtemp2;
    char *dst, *ctemp;
    char carr[2];
    size_t csl, st;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * See how much space is needed, and grow the append buffer if
     * needed to accommodate the list element.
     */

    size = _Tcl_WScanElement(interp, wstring, &flags, &conv_err) + 1;

    if ((iPtr->result != iPtr->appendResult)
            || (iPtr->appendResult[iPtr->appendUsed] != 0)
            || ((size + iPtr->appendUsed) >= iPtr->appendAvl)) {
       SetupAppendBuffer(iPtr, size+iPtr->appendUsed);
    }

    /*
     * Convert the string into a list element and copy it to the
     * buffer that's forming.
     */

    wdst = (wchar_t *)ckalloc(size);
    dst = iPtr->appendResult + iPtr->appendUsed;
    carr[1] = '\0';
    carr[0] = dst[-1];
    mbtowc(&wtemp1, (const char *)carr, (size_t)1);
    carr[0] = dst[-2];
    mbtowc(&wtemp2, (const char *)carr, (size_t)1);

    if ((iPtr->appendUsed > 0) && ((wtemp1 != i18n_special_chars.w_op_cbra)
	|| ((iPtr->appendUsed > 1) && (wtemp2 == i18n_special_chars.w_bsla)))) 
    {
        iPtr->appendUsed++;
        *dst = ' ';
        dst++;
    }
    size = _Tcl_WConvertElement(interp, wstring, wdst, flags, &conv_err);
    if (conv_err == (size_t)-1)
    {
	ckfree(wdst);
	return TCL_CONV_ERROR;
    }

    /*
     * Convert wide characters to multibyte characters
     */
    _WChar_To_Multibyte(interp, csl, wdst, ctemp, st)
    if (st == (size_t)-1)
    {
	ckfree(wdst);
	return TCL_CONV_ERROR;
    }

    strcpy(dst, ctemp);
    iPtr->appendUsed += csl;
    ckfree(wdst);
    ckfree(ctemp);

    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WMerge --
 *
 *      Given a collection of strings, merge them together into a
 *      single string that has proper Tcl list structured (i.e.
 *      Tcl_SplitList may be used to retrieve strings equal to the
 *      original elements, and Tcl_Eval will parse the string back
 *      into its original elements).
 *
 * Results:
 *      The return value is the address of a dynamically-allocated
 *      string containing the merged list.
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 */

wchar_t *
_Tcl_WMerge(interp, argc, argv, cerr)
    Tcl_Interp *interp;	
    int argc;                   /* How many strings to merge. */
    char **argv;                /* Array of string values. */
    int *cerr;
{
#   define LOCAL_SIZE 20
    int localFlags[LOCAL_SIZE], *flagPtr;
    int numChars, i;
    wchar_t	*wdst, *wstring, *wresult;
    size_t	wsn, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    *cerr = TCL_OK;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	*cerr = TCL_CONV_ERROR;
	return (wchar_t *)NULL;
    }

    /*
     * Pass 1: estimate space, gather flags.
     */

    if (argc <= LOCAL_SIZE) {
        flagPtr = localFlags;
    } else {
        flagPtr = (int *) ckalloc((unsigned) argc*sizeof(int));
    }
    numChars = 1;
    for (i = 0; i < argc; i++) 
    {
	/* 
	 * Convert argv[i] to wide character string 
	 */
	_Multibyte_to_WChar(interp, wsn, argv[i], wstring, st)
	if (st == (size_t)-1)
	{
		*cerr = TCL_CONV_ERROR;
		return (wchar_t *)NULL;
	}
        numChars += _Tcl_WScanElement(interp, wstring, &flagPtr[i], cerr) + 1;
	ckfree(wstring);
    }

    /*
     * Pass two: copy into the result area.
     */

    wresult = (wchar_t *)ckalloc((unsigned) numChars);
    wdst = wresult;
    for (i = 0; i < argc; i++) {
	int err;

	/* 
	 * Convert argv[i] to wide character string 
	 */
	_Multibyte_to_WChar(interp, wsn, argv[i], wstring, st)
	if (st == (size_t)-1)
	{
		*cerr = TCL_CONV_ERROR;
		return (wchar_t *)NULL;
	}
        numChars = _Tcl_WConvertElement(interp, wstring, wdst, flagPtr[i], &err);
	ckfree(wstring);
	wdst += numChars;
        *wdst = i18n_special_chars.w_space;
        wdst++;
    }
    if (wdst == wresult) {
        *wdst = i18n_special_chars.w_esc_zero;
    } else {
        wdst[-1] = i18n_special_chars.w_esc_zero;
    }

    if (flagPtr != localFlags) {
        ckfree((char *) flagPtr);
    }
    return wresult;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WDStringAppendElement --
 *
 *	Append a list element to the current value of a dynamic string.
 *
 * Results:
 *	The return value is a pointer to the dynamic string's new value.
 *
 * Side effects:
 *	String is reformatted as a list element and added to the current
 *	value of the string.  Memory gets reallocated if needed to
 *	accomodate the string's new size.
 *
 *----------------------------------------------------------------------
 */

char *
_Tcl_WDStringAppendElement(interp, dsPtr, string)
    Tcl_Interp *interp;
    register Tcl_DString *dsPtr;	/* Structure describing dynamic
					 * string. */
    char *string;			/* String to append.  Must be
					 * null-terminated. */
{
    int newSize, flags, conv_err;
    char *dst, *newString, *ctemp;
    char carr[2];
    wchar_t *wstring, *wdst, wtemp1, wtemp2;
    size_t wlen, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    _Multibyte_to_WChar(interp, wlen, string, wstring, st)
    if (st == (size_t)-1)
	return NULL;

    newSize = _Tcl_WScanElement(interp, wstring, &flags, &conv_err) 
							+ dsPtr->length + 1;
    if (conv_err == TCL_CONV_ERROR)
    {
	ckfree(wstring);
	return NULL;
    }


    /*
     * Allocate a larger buffer for the string if the current one isn't
     * large enough.  Allocate extra space in the new buffer so that there
     * will be room to grow before we have to allocate again.
     */

    if (newSize >= dsPtr->spaceAvl) {
	dsPtr->spaceAvl = newSize*2;
	newString = (char *) ckalloc((unsigned) dsPtr->spaceAvl);
	strcpy(newString, dsPtr->string);
	if (dsPtr->string != dsPtr->staticSpace) {
	    ckfree(dsPtr->string);
	}
	dsPtr->string = newString;
    }

    /*
     * Convert the new string to a list element and copy it into the
     * buffer at the end.  Add a space separator unless we're at the
     * start of the string or just after an unbackslashed "{".
     */

    wdst = (wchar_t *)ckalloc(newSize);
    dst = dsPtr->string + dsPtr->length;
    carr[1] = '\0';
    carr[0] = dst[-1];
    mbtowc(&wtemp1, (const char *)carr, (size_t)1);
    carr[0] = dst[-2];
    mbtowc(&wtemp2, (const char *)carr, (size_t)1);

    if ((dsPtr->length > 0) && (( wtemp1 != i18n_special_chars.w_op_cbra)
	    || ((dsPtr->length > 1) && (wtemp2 == i18n_special_chars.w_bsla))))
    {
	*dst = ' ';
	dst++;
	dsPtr->length++;
    }

    newSize = _Tcl_WConvertElement(interp, wstring, wdst, flags, &conv_err);
    if (conv_err == TCL_CONV_ERROR)
    {
	ckfree(wstring);
	ckfree(wdst);
	return NULL;
    }

    _WChar_To_Multibyte(interp, wlen, wdst, ctemp, st)
    if (st == (size_t)-1)
    {
	ckfree(wstring);
	ckfree(wdst);
	return NULL;
    }

    strcpy(dst, ctemp);
    dsPtr->length += wlen;
    ckfree(wdst);
    ckfree(wstring);
    ckfree(ctemp);

    return dsPtr->string;
}

/*
 *----------------------------------------------------------------------
 *
 * _TclWFindElement --
 *
 *	Given a pointer into a Tcl list, locate the first (or next)
 *	element in the list.
 *
 * Results:
 *	The return value is normally TCL_OK, which means that the
 *	element was successfully located.  If TCL_ERROR is returned
 *	it means that list didn't have proper list structure;
 *	interp->result contains a more detailed error message.
 *
 *	If TCL_OK is returned, then *elementPtr will be set to point
 *	to the first element of list, and *nextPtr will be set to point
 *	to the character just after any white space following the last
 *	character that's part of the element.  If this is the last argument
 *	in the list, then *nextPtr will point to the NULL character at the
 *	end of list.  If sizePtr is non-NULL, *sizePtr is filled in with
 *	the number of characters in the element.  If the element is in
 *	braces, then *elementPtr will point to the character after the
 *	opening brace and *sizePtr will not include either of the braces.
 *	If there isn't an element in the list, *sizePtr will be zero, and
 *	both *elementPtr and *termPtr will refer to the null character at
 *	the end of list.  Note:  this procedure does NOT collapse backslash
 *	sequences.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_TclWFindElement(interp, wlist, welementPtr, wnextPtr, sizePtr, bracePtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    register wchar_t *wlist;	/* String containing Tcl list with zero
				 * or more elements (possibly in braces). */
    wchar_t **welementPtr;	/* Fill in with location of first significant
				 * character in first element of list. */
    wchar_t **wnextPtr;		/* Fill in with location of character just
				 * after all white space following end of
				 * argument (i.e. next argument or end of
				 * list). */
    int *sizePtr;		/* If non-zero, fill in with size of
				 * element. */
    int *bracePtr;		/* If non-zero fill in with non-zero/zero
				 * to indicate that arg was/wasn't
				 * in braces. */
{
    register wchar_t *wp;
    int openBraces = 0;
    int inQuotes = 0;
    int size;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * Skim off leading white space and check for an opening brace or
     * quote.   Note:  use of "isascii" below and elsewhere in this
     * procedure is a temporary hack (7/27/90) because Mx uses characters
     * with the high-order bit set for some things.  This should probably
     * be changed back eventually, or all of Tcl should call isascii.
     */

    while (iswspace(*wlist)) 
    {
	wlist++;
    }
    if (*wlist == i18n_special_chars.w_op_cbra) 
    {
	openBraces = 1;
	wlist++;
    } 
    else if (*wlist == i18n_special_chars.w_dob_qo) 
    {
	inQuotes = 1;
	wlist++;
    }

    if (bracePtr != 0) 
    {
	*bracePtr = openBraces;
    }
    wp = wlist;

    /*
     * Find the end of the element (either a space or a close brace or
     * the end of the string).
     */

    while (1) 
    {
	if (*wp == i18n_special_chars.w_op_cbra)
	{
	    /*
	     * Open brace: don't treat specially unless the element is
	     * in braces.  In this case, keep a nesting count.
	     */
	    if (openBraces != 0) 
	    {
	        openBraces++;
	    }
	}
	else if (*wp == i18n_special_chars.w_cl_cbra)
	{
	    /*
	     * Close brace: if element is in braces, keep nesting
	     * count and quit when the last close brace is seen.
	     */
	    if (openBraces == 1) 
	    {
	    	wchar_t *wp2;
		char *ctemp;
		size_t clen,st;

		size = wp - wlist;
		wp++;
		if (iswspace(*wp) || (*wp == i18n_special_chars.w_esc_zero)) 
		{
			goto done;
		}
		for (wp2 = wp; (*wp2 != i18n_special_chars.w_esc_zero) 
			&& (!iswspace(*wp2)) && (wp2 < wp+20); wp2++) 
		{
			/* null body */
		}
		_WChar_To_Multibyte(interp, clen, wp, ctemp, st)
		if (st == (size_t)-1)
			return TCL_CONV_ERROR;

		Tcl_ResetResult(interp);
		i18n_msg_format = dce_msg_get(tcl_s_tclutil_list_ele_in_braces);
		sprintf(interp->result, (char *)i18n_msg_format, wp2-wp, ctemp);
	    	free(i18n_msg_format);
		ckfree(ctemp);

		return TCL_ERROR;
	    }
	    else if (openBraces != 0) 
	    {
	    	openBraces--;
	    }
	}
	else if (*wp == i18n_special_chars.w_bsla)
	{
	    /*
	     * Backslash:  skip over everything up to the end of the
	     * backslash sequence.
	     */
	    int n_size;

	    (void) _Tcl_WBackslash(interp, wp, &n_size);
	    wp += n_size - 1;
	}
	else if (*wp == i18n_special_chars.w_space ||
		 *wp == i18n_special_chars.w_esc_f ||
		 *wp == i18n_special_chars.w_esc_n ||
		 *wp == i18n_special_chars.w_esc_r ||
		 *wp == i18n_special_chars.w_esc_t ||
		 *wp == i18n_special_chars.w_esc_v)
	{
	    /*
	     * Space: ignore if element is in braces or quotes;  otherwise
	     * terminate element.
	     */
	    if ((openBraces == 0) && !inQuotes) 
	    {
		    size = wp - wlist;
		    goto done;
	    }
	}
	else if (*wp == i18n_special_chars.w_dob_qo)
	{
	    /*
	     * Double-quote:  if element is in quotes then terminate it.
	     */
	    if (inQuotes) {
	    	wchar_t *wp2;
		char *ctemp;
		size_t clen, st;

	    	size = wp-wlist;
	    	wp++;
	    	if (iswspace(*wp) || (*wp == i18n_special_chars.w_esc_zero)) 
		{
			goto done;
	    	}
	    	for (wp2 = wp; (*wp2 != i18n_special_chars.w_esc_zero) 
			&& (!iswspace(*wp2)) && (wp2 < wp+20); wp2++) 
		{
			/* null body */
	    	}
	    	Tcl_ResetResult(interp);
		_WChar_To_Multibyte(interp, clen, wp, ctemp, st)
		if (st == (size_t)-1)
			return TCL_CONV_ERROR;

		i18n_msg_format = dce_msg_get(tcl_s_tclutil_list_ele_in_quotes);
		sprintf(interp->result, (char *)i18n_msg_format, wp2-wp, ctemp);
		free(i18n_msg_format);
		ckfree(ctemp);

		return TCL_ERROR;
	    }
	}
	else if (*wp == i18n_special_chars.w_esc_zero)
	{
	    /*
	     * End of list:  terminate element.
	     */
	    if (openBraces != 0) 
	    {
	        i18n_msg_format = dce_msg_get(tcl_s_tclutil_unmatch_open_brace);
	        Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
	        free(i18n_msg_format);

	        return TCL_ERROR;
	    } 
	    else if (inQuotes) 
	    {
	        i18n_msg_format = dce_msg_get(tcl_s_tclutil_unmatch_open_quote);
	        Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
	        free(i18n_msg_format);

	        return TCL_ERROR;
	    }
	    size = wp - wlist;
	    goto done;
	}
	wp++;
    }

    done:
    while (iswspace(*wp)) 
    {
	wp++;
    }
    *welementPtr = wlist;
    *wnextPtr = wp;
    if (sizePtr != 0) 
    {
	*sizePtr = size;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _TclWCopyAndCollapse --
 *
 *	Copy a string and eliminate any backslashes that aren't in braces.
 *
 * Results:
 *	There is no return value.  Count chars. get copied from src
 *	to dst.  Along the way, if backslash sequences are found outside
 *	braces, the backslashes are eliminated in the copy.
 *	After scanning count chars. from source, a null character is
 *	placed at the end of dst.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
_TclWCopyAndCollapse(interp, count, wsrc, wdst)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    int count;			/* Total number of wide characters to copy
				 * from src. */
    register wchar_t *wsrc;	/* Copy from here... */
    register wchar_t *wdst;	/* ... to here. */
{
    register wchar_t wc;
    int numRead;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return;
    }

    for (wc = *wsrc; count > 0; wsrc++, wc = *wsrc, count--) {
	if (wc == i18n_special_chars.w_bsla) {
	    *wdst = _Tcl_WBackslash(interp, wsrc, &numRead);
	    wdst++;
	    wsrc += numRead-1;
	    count -= numRead-1;
	} else {
	    *wdst = wc;
	    wdst++;
	}
    }
    *wdst = i18n_special_chars.w_esc_zero;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSplitList --
 *
 *	Splits a list up into its constituent fields.
 *
 * Results
 *	The return value is normally TCL_OK, which means that
 *	the list was successfully split up.  If TCL_ERROR is
 *	returned, it means that "list" didn't have proper list
 *	structure;  interp->result will contain a more detailed
 *	error message.
 *
 *	*argvPtr will be filled in with the address of an array
 *	whose elements point to the elements of list, in order.
 *	*argcPtr will get filled in with the number of valid elements
 *	in the array.  A single block of memory is dynamically allocated
 *	to hold both the argv array and a copy of the list (with
 *	backslashes and braces removed in the standard way).
 *	The caller must eventually free this memory by calling free()
 *	on *argvPtr.  Note:  *argvPtr and *argcPtr are only modified
 *	if the procedure returns normally.
 *
 * Side effects:
 *	Memory is allocated.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WSplitList(interp, wlist, argcPtr, wargvPtr)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    wchar_t *wlist;		/* Pointer to string with list structure. */
    int *argcPtr;		/* Pointer to location to fill in with
				 * the number of elements in the list. */
    wchar_t ***wargvPtr;	/* Pointer to place to store pointer to array
				 * of pointers to list elements. */
{
    wchar_t **wargv;
    wchar_t *wp;
    int size, i, result, elSize, brace;
    wchar_t *welement;
    unsigned char *i18n_msg_format;
char *ctemp;
size_t clen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * Figure out how much space to allocate.  There must be enough
     * space for both the array of pointers and also for a copy of
     * the list.  To estimate the number of pointers needed, count
     * the number of space characters in the list.
     */

    for (size = 1, wp = wlist; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (iswspace(*wp)) 
	{
	    size++;
	}
    }
    size++;			/* Leave space for final NULL pointer. */
    wargv = (wchar_t **) ckalloc((unsigned)((size * sizeof(wchar_t *)) 
		+ (wp - wlist) + (2 * sizeof(wchar_t))));

    for (i = 0, wp = ((wchar_t *) wargv) + size*sizeof(wchar_t *);
	    *wlist != i18n_special_chars.w_esc_zero; i++) 
    {
	result = _TclWFindElement(interp, wlist, &welement, &wlist, &elSize, &brace);

_WChar_To_Multibyte(interp, clen, welement, ctemp, st)
ckfree(ctemp);
ctemp = NULL;

	if (result != TCL_OK) 
	{
	    ckfree((wchar_t *) wargv);
	    return result;
	}
	if (*welement == i18n_special_chars.w_esc_zero) 
	{
	    break;
	}
	if (i >= size) 
	{
	    ckfree((wchar_t *) wargv);
	    i18n_msg_format = dce_msg_get(tcl_s_tclutil_internal_err_splitlist);
	    Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
	    free(i18n_msg_format);

	    return TCL_ERROR;
	}
	/* wargv[i] = wp; */
	if (brace) {
	    wcsncpy(wp, welement, elSize);
	    wargv[i] = wp;
	    wp += elSize;
	    *wp = i18n_special_chars.w_esc_zero;
	    wp++;
	} 
	else 
	{
_WChar_To_Multibyte(interp, clen, welement, ctemp, st)
ckfree(ctemp);
ctemp = NULL;
	    _TclWCopyAndCollapse(interp, elSize, welement, wp);
	    wargv[i] = wp;
	    wp += elSize+1;
	}
_WChar_To_Multibyte(interp, clen, wargv[i], ctemp, st)
ckfree(ctemp);
ctemp = NULL;
    }

    wargv[i] = (wchar_t *)NULL;
    *wargvPtr = wargv;
    *argcPtr = i;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WConcat --
 *
 *	Concatenate a set of strings into a single large string.
 *
 * Results:
 *	The return value is dynamically-allocated string containing
 *	a concatenation of all the strings in argv, with spaces between
 *	the original argv elements.
 *
 * Side effects:
 *	Memory is allocated for the result;  the caller is responsible
 *	for freeing the memory.
 *
 *----------------------------------------------------------------------
 */

wchar_t *
_Tcl_WConcat(interp, argc, wargv)
    Tcl_Interp *interp;		/* Interpreter to use for error reporting. */
    int argc;			/* Number of strings to concatenate. */
    wchar_t **wargv;		/* Array of strings to concatenate. */
{
    int totalSize, i;
    wchar_t *wp, *wresult;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return (wchar_t *)NULL;
    }

    for (totalSize = 1, i = 0; i < argc; i++) 
    {
	totalSize += wcslen(wargv[i]) + 1;
    }
    wresult = (wchar_t *) ckalloc((unsigned) totalSize * sizeof(wchar_t));
    if (argc == 0) 
    {
	*wresult = i18n_special_chars.w_esc_zero;
	return wresult;
    }
    for (wp = wresult, i = 0; i < argc; i++) 
    {
	wchar_t *welement;
	int length;

	/*
	 * Clip white space off the front and back of the string
	 * to generate a neater result, and ignore any empty
	 * elements.
	 */

	welement = wargv[i];

	while (iswspace(*welement)) 
	{
	    welement++;
	}
	for (length = wcslen(welement);
		(length > 0) && (iswspace(welement[length-1])); length--) 
	{
	    /* Null loop body. */
	}
	if (length == 0) {
	    continue;
	}
	(void) wcsncpy(wp, welement, length);
	wp += length;

	*wp = i18n_special_chars.w_space;
	wp++;
    }

    if (wp != wresult) 
    {
	wp[-1] = i18n_special_chars.w_esc_zero;
    } 
    else 
    {
	*wp = i18n_special_chars.w_esc_zero;
    }
    return wresult;
}

/*
 *----------------------------------------------------------------------
 *
 * _TclWGetListIndex --
 *
 *	Parse a list index, which may be either an integer or the
 *	value "end".
 *
 * Results:
 *	The return value is either TCL_OK or TCL_ERROR.  If it is
 *	TCL_OK, then the index corresponding to string is left in
 *	*indexPtr.  If the return value is TCL_ERROR, then string
 *	was bogus;  an error message is returned in interp->result.
 *	If a negative index is specified, it is rounded up to 0.
 *	The index value may be larger than the size of the list
 *	(this happens when "end" is specified).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_TclWGetListIndex(interp, wstring, indexPtr)
    Tcl_Interp *interp;			/* Interpreter for error reporting. */
    wchar_t *wstring;			/* String containing list index. */
    int *indexPtr;			/* Where to store index. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp;
    size_t clen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    _WChar_To_Multibyte(interp, clen, wstring, ctemp, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    if (iswdigit(*wstring) || (*wstring == i18n_special_chars.w_minus)) 
    {
	if (_Tcl_WGetInt(interp, wstring, indexPtr) != TCL_OK) 
	{
	    ckfree(ctemp);
	    return TCL_ERROR;
	}
	if (*indexPtr < 0) 
	{
	    *indexPtr = 0;
	}
	ckfree(ctemp);
    } 
    else if (strncmp(ctemp, "end", strlen(ctemp)) == 0) 
    {
	*indexPtr = 1<<30;
	ckfree(ctemp);
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclutil_bad_index);
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
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WRegExpMatch --
 *
 *	See if a string matches a regular expression.
 *
 * Results:
 *	If an error occurs during the matching operation then -1
 *	is returned and interp->result contains an error message.
 *	Otherwise the return value is 1 if "string" matches "pattern"
 *	and 0 otherwise.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WRegExpMatch(interp, wstring, wpattern)
    Tcl_Interp *interp;		/* Used for error reporting. */
    wchar_t *wstring;		/* String. */
    wchar_t *wpattern;		/* Regular expression to match against
				 * string. */
{
    regexp *regexpPtr;
    int match;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *string, *pattern;
    size_t clen, st;

    _WChar_To_Multibyte(interp, clen, wpattern, pattern, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    _WChar_To_Multibyte(interp, clen, wstring, string, st)
    if (st == (size_t)-1)
    {
	ckfree(wpattern);
	return TCL_ERROR;
    }

    regexpPtr = TclCompileRegexp(interp, pattern);
    if (regexpPtr == NULL) 
    {
	return -1;
    }
    tclRegexpError = NULL;
    match = TclRegExec(regexpPtr, string, string);
    if (tclRegexpError != NULL) 
    {
	Tcl_ResetResult(interp);
	i18n_msg_format = dce_msg_get(tcl_s_tclutil_reg_match_err);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(tclRegexpError)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, tclRegexpError);
	free(tclRegexpError);

	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(pattern);
	ckfree(string);

	return -1;
    }
    ckfree(pattern);
    ckfree(string);
    return match;
}

/*
 *----------------------------------------------------------------------
 *
 * _InitI18NSpecChars --
 *
 *	This is an internal procedure for initializing wide character
 *	equivalence of special characters.
 *
 *
 * Results:
 *	Each wide char is computed for an equivalent single character.
 *	init_flag is set to one, assuring this routine is only called once.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_InitI18NSpecChars(interp)
Tcl_Interp *interp;
{
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;
    unsigned char tc[2];

    tc[1] = '\0';
    /*
     * Create wide character equivalences for special characters
     */
    tc[0] = (unsigned char)0x0;
    if ((mbtowc(&i18n_special_chars.w_null, (const char *)tc, (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "null character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    tc[0] = (unsigned char)0x9;
    if ((mbtowc(&i18n_special_chars.w_tab, (const char *)tc, (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "TAB character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    tc[0] = (unsigned char)0xa;
    if ((mbtowc(&i18n_special_chars.w_lf, (const char *)tc, (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "LF character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    tc[0] = (unsigned char)0xb;
    if ((mbtowc(&i18n_special_chars.w_vt, (const char *)tc, (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "VT character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    tc[0] = (unsigned char)0xc;
    if ((mbtowc(&i18n_special_chars.w_ff, (const char *)tc, (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "FF character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    tc[0] = (unsigned char)0xd;
    if ((mbtowc(&i18n_special_chars.w_cr, (const char *)tc, (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "CR character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_empty, (const char *)"", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("empty character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "empty character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_op_cbra, (const char *)"{", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '{');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_cl_cbra, (const char *)"}", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '}');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_bsla, (const char *)"\\", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '\\');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_op_sbr, (const char *)"[", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '[');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_cl_sbr, (const char *)"]", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ']');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_dollar, (const char *)"$", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '$');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_sem_col, (const char *)";", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ';');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_space, (const char *)" ", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("space character") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "space character");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_dob_qo, (const char *)"\"", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '\"');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_a, (const char *)"\a", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape a") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape a");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_b, (const char *)"\b", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape b") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape b");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_f, (const char *)"\f", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape f") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape f");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_n, (const char *)"\n", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape n") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape n");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_r, (const char *)"\r", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape r") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape r");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_t, (const char *)"\t", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape t") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape t");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_v, (const char *)"\v", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape v") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape v");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_esc_zero, (const char *)"\0", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen("escape 0") + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "escape 0");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_slash, (const char *)"/", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '/');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_question, (const char *)"?", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '?');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_star, (const char *)"*", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '*');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_comma, (const char *)",", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ',');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_minus, (const char *)"-", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '-');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_plus, (const char *)"+", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '+');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_period, (const char *)".", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '.');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_op_paren, (const char *)"(", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '(');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_cl_paren, (const char *)")", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ')');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_percent, (const char *)"%", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '%');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_colon, (const char *)":", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ':');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_less_t, (const char *)"<", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '<');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_grea_t, (const char *)">", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '>');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_equal, (const char *)"=", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '=');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_bang, (const char *)"!", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '!');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_and, (const char *)"&", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '&');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_mnt, (const char *)"^", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '^');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_wall, (const char *)"|", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '|');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_tilde, (const char *)"~", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '~');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_unscore, (const char *)"_", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '_');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_sharp, (const char *)"#", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '#');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_special_chars.w_at, (const char *)"@", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '@');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    i18n_special_chars.init_flag = 1;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _InitI18NParseChars --
 *
 *	This is an internal procedure for initializing wide character
 *	equivalence of special characters.
 *
 *
 * Results:
 *	Each wide char is computed for an equivalent single character.
 *	init_flag is set to one, assuring this routine is only called once.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_InitI18NParseChars(interp)
Tcl_Interp *interp;
{
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Create wide character equivalences for special characters
     */

    if ((mbtowc(&i18n_parse_chars.w_0, (const char *)"0", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '0');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_2, (const char *)"2", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '2');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_3, (const char *)"3", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '3');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_4, (const char *)"4", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '4');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_5, (const char *)"5", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '5');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_6, (const char *)"6", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '6');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_7, (const char *)"7", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '7');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_8, (const char *)"8", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '8');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_9, (const char *)"9", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, '9');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_E, (const char *)"E", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'E');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_L, (const char *)"L", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'L');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_X, (const char *)"X", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'X');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_a, (const char *)"a", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'a');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_b, (const char *)"b", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'b');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_c, (const char *)"c", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'c');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_d, (const char *)"d", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'd');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_e, (const char *)"e", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'e');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_f, (const char *)"f", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'f');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_g, (const char *)"g", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'g');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_h, (const char *)"h", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'h');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_i, (const char *)"i", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'i');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_l, (const char *)"l", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'l');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_n, (const char *)"n", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'n');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_o, (const char *)"o", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'o');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_r, (const char *)"r", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'r');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_s, (const char *)"s", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 's');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_t, (const char *)"t", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 't');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_u, (const char *)"u", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'u');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_v, (const char *)"v", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'v');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_x, (const char *)"x", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'x');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    if ((mbtowc(&i18n_parse_chars.w_w, (const char *)"w", (size_t)1)) == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_single_multibyte_conv_failed);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + 2);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, 'w');
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	return TCL_CONV_ERROR;
    }

    i18n_parse_chars.init_flag = 1;
    return TCL_OK;
}
