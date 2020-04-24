/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tclWParse.c,v $
 * Revision 1.1.6.3  1996/02/18  19:19:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:13  marty]
 *
 * Revision 1.1.6.2  1995/12/08  21:35:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:06:50  root]
 * 
 * Revision 1.1.2.2  1994/10/25  18:40:00  rousseau
 * 	Fix function definitions (CR 12719).
 * 	[1994/10/25  18:39:47  rousseau]
 * 
 * Revision 1.1.2.1  1994/10/21  21:39:49  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:35:42  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclWParse.c --
 *
 *	This file contains a collection of procedures that are used
 *	to parse Tcl commands or parts of commands (like quoted
 *	strings or nested sub-commands) in wide character format.
 */

#include "tclInt.h"
#include "tclWInt.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * Function prototypes for procedures local to this file:
 */

static wchar_t *_WQuoteEnd _ANSI_ARGS_((Tcl_Interp *interp, wchar_t *wstring, wchar_t wterm));
static wchar_t *_WVarNameEnd _ANSI_ARGS_((Tcl_Interp *interp, wchar_t *wstring));

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WBackslash --
 *
 *      Figure out how to handle a backslash sequence.  This is an
 *	equivalent routine for Tcl_Backslash for wide character interface.
 *
 * Results:
 *      The return value is the character that should be substituted
 *      in place of the backslash sequence that starts at src.  If
 *      readPtr isn't NULL then it is filled in with a count of the
 *      number of characters in the backslash sequence.
 *
 * Side effects:
 *      None.
 *
 *----------------------------------------------------------------------
 */

wchar_t
_Tcl_WBackslash(interp, src, readPtr)
    Tcl_Interp *interp;
    wchar_t *src;		/* Points to the backslash character of
				 * a backslash sequence. */
    int *readPtr;		/* Fill in with number of characters read
				 * from src, unless NULL. */
{
    wchar_t *wp = src+1;
    wchar_t result;
    char    temp_cbuf[10];
    int count, temp_num;
    unsigned char *i18n_msg_format;

    count = 2;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return i18n_special_chars.w_null;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
	return i18n_special_chars.w_null;
    }

    if (*wp == i18n_parse_chars.w_a)
            result = i18n_special_chars.w_esc_a;
    else if (*wp == i18n_parse_chars.w_b)
            result = i18n_special_chars.w_esc_b;
    else if (*wp == i18n_parse_chars.w_f)
            result = i18n_special_chars.w_esc_f;
    else if (*wp == i18n_parse_chars.w_n)
            result = i18n_special_chars.w_esc_n;
    else if (*wp == i18n_parse_chars.w_r)
            result = i18n_special_chars.w_esc_r;
    else if (*wp == i18n_parse_chars.w_t)
            result = i18n_special_chars.w_esc_t;
    else if (*wp == i18n_parse_chars.w_v)
            result = i18n_special_chars.w_esc_v;
    else if (*wp == i18n_parse_chars.w_x)
    {
            if (iswxdigit((wint_t)wp[1])) {
                wchar_t *end;

                result = wcstoul((const wchar_t *)wp+1, &end, 16);
                count = end - src;
            } else {
                count = 2;
                result = i18n_parse_chars.w_x;
            }
    }
    else if (*wp == i18n_special_chars.w_esc_n)
    {
            do {
                wp++;
            } while (iswspace ((wint_t)*wp));
            result = i18n_special_chars.w_space;
            count = wp - src;
   }
   else if (*wp == i18n_special_chars.w_esc_zero)
   {
            result = i18n_special_chars.w_bsla;
            count = 1;
   }
   else 
   {
            if (iswdigit((wint_t)*wp)) 
	    {
		/* convert wide character to character */
		temp_num = wctomb(temp_cbuf, *wp);
		if (temp_num == -1)
		{
			i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed);
			Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *)NULL);
			return i18n_special_chars.w_null;
		}
		result = temp_cbuf[0] - '0';
                wp++;
                if (!iswdigit((wint_t)*wp))
                    goto DONE;

                count = 3;
		temp_num = wctomb(temp_cbuf, *wp);
		if (temp_num == -1)
		{
			i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed);
			Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *)NULL);
			return i18n_special_chars.w_null;
		}
		result = (result << 3) + temp_cbuf[0] - '0';
                /* result = (result << 3) + (*wp - '0');
		 */
                wp++;
                if (!iswdigit((wint_t)*wp))
                    goto DONE;

                count = 4;
		temp_num = wctomb(temp_cbuf, *wp);
		if (temp_num == -1)
		{
			i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed);
			Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *)NULL);
			return i18n_special_chars.w_null;
		}
		result = (result << 3) + temp_cbuf[0] - '0';
                /* result = (result << 3) + (*wp - '0');
		 */
                goto DONE;
            }
	    result = *wp;
            count = 2;
    }
    DONE:

    if (readPtr != NULL) {
        *readPtr = count;
    }
    return result;
}

/*
 *--------------------------------------------------------------
 *
 * _TclWParseQuotes --
 *
 *	This procedure parses a double-quoted string such as a
 *	quoted Tcl command argument or a quoted value in a Tcl
 *	expression.  This procedure is also used to parse array
 *	element names within parentheses, or anything else that
 *	needs all the substitutions that happen in quotes.
 *
 * Results:
 *	The return value is a standard Tcl result, which is
 *	TCL_OK unless there was an error while parsing the
 *	quoted string.  If an error occurs then interp->result
 *	contains a standard error message.  *TermPtr is filled
 *	in with the address of the character just after the
 *	last one successfully processed;  this is usually the
 *	character just after the matching close-quote.  The
 *	fully-substituted contents of the quotes are stored in
 *	standard fashion in *pvPtr, null-terminated with
 *	pvPtr->next pointing to the terminating null character.
 *
 * Side effects:
 *	The buffer space in pvPtr may be enlarged by calling its
 *	expandProc.
 *
 *--------------------------------------------------------------
 */

int
#ifdef _ANSI_ARGS_
_TclWParseQuotes(Tcl_Interp *interp,
                 wchar_t *wstring,
                 wchar_t wtermChar,
                 int flags,
                 wchar_t **wtermPtr,
                 _WParseValue *wpvPtr)
#else
_TclWParseQuotes(interp, wstring, wtermChar, flags, wtermPtr, wpvPtr)
    Tcl_Interp *interp;		/* Interpreter to use for nested command
				 * evaluations and error messages. */
    wchar_t *wstring;		/* Character just after opening double-
				 * quote. */
    wchar_t wtermChar;		/* Character that terminates "quoted" string
				 * (usually double-quote, but sometimes
				 * right-paren or something else). */
    int flags;			/* Flags to pass to nested Tcl_Eval calls. */
    wchar_t **wtermPtr;		/* Store address of terminating character
				 * here. */
    _WParseValue *wpvPtr;	/* Information about where to place
				 * fully-substituted result of parse. */
#endif
{
    wchar_t *wsrc, *wdst, wc;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    wsrc = wstring;
    wdst = wpvPtr->next;

    while (1) {
	if (wdst == wpvPtr->end) 
	{
	    /*
	     * Target buffer space is about to run out.  Make more space.
	     */

	    wpvPtr->next = wdst;
	    (*wpvPtr->expandProc)(wpvPtr, 1);
	    wdst = wpvPtr->next;
	}

	wc = *wsrc;
	wsrc++;
	if (wc == wtermChar) 
	{
	    *wdst = i18n_special_chars.w_esc_zero;
	    wpvPtr->next = wdst;
	    *wtermPtr = wsrc;
	    return TCL_OK;
	} 
	else if (_Tcl_WCtype(interp, wc) == TCL_NORMAL) 
	{
	    copy:
	    *wdst = wc;
	    wdst++;
	    continue;
	} 
	else if (wc == i18n_special_chars.w_dollar) 
	{
	    int length;
	    wchar_t *value;

	    value = _Tcl_WParseVar(interp, wsrc-1, wtermPtr);
	    if (value == (wchar_t *)NULL) 
	    {
		return TCL_ERROR;
	    }
	    wsrc = *wtermPtr;
	    length = wcslen(value);
	    if ((wpvPtr->end - wdst) <= length) 
	    {
		wpvPtr->next = wdst;
		(*wpvPtr->expandProc)(wpvPtr, length);
		wdst = wpvPtr->next;
	    }
	    wcscpy(wdst, value);
	    ckfree(value); 	/* return from WParseVar */
	    wdst += length;
	    continue;
	} 
	else if (wc == i18n_special_chars.w_op_sbr) 
	{
	    int result;

	    wpvPtr->next = wdst;
	    result = _TclWParseNestedCmd(interp, wsrc, flags, wtermPtr, wpvPtr);
	    if (result != TCL_OK) 
	    {
		return result;
	    }
	    wsrc = *wtermPtr;
	    wdst = wpvPtr->next;
	    continue;
	} 
	else if (wc == i18n_special_chars.w_bsla) 
	{
	    int numRead;

	    wsrc--;
	    *wdst = _Tcl_WBackslash(interp, wsrc, &numRead);
	    if (*wdst == i18n_special_chars.w_null)
		return TCL_CONV_ERROR;

	    wdst++;
	    wsrc += numRead;
	    continue;
	} 
	else if (wc == i18n_special_chars.w_esc_zero) 
	{
	    char ctemp[10];

	    if ((wctomb(ctemp, wtermChar)) == -1)
		return TCL_CONV_ERROR;

	    Tcl_ResetResult(interp);
	    i18n_msg_format = dce_msg_get(tcl_s_parse_missing_c);
	    sprintf(interp->result, (char *)i18n_msg_format, ctemp[0]);
	    free(i18n_msg_format);
	    ckfree(ctemp);

	    *wtermPtr = wstring-1;
	    return TCL_ERROR;
	} 
	else 
	{
	    goto copy;
	}
    }
}

/*
 *--------------------------------------------------------------
 *
 * _TclWParseNestedCmd --
 *
 *	This procedure parses a nested Tcl command between
 *	brackets, returning the result of the command.
 *
 * Results:
 *	The return value is a standard Tcl result, which is
 *	TCL_OK unless there was an error while executing the
 *	nested command.  If an error occurs then interp->result
 *	contains a standard error message.  *TermPtr is filled
 *	in with the address of the character just after the
 *	last one processed;  this is usually the character just
 *	after the matching close-bracket, or the null character
 *	at the end of the string if the close-bracket was missing
 *	(a missing close bracket is an error).  The result returned
 *	by the command is stored in standard fashion in *pvPtr,
 *	null-terminated, with pvPtr->next pointing to the null
 *	character.
 *
 * Side effects:
 *	The storage space at *pvPtr may be expanded.
 *
 *--------------------------------------------------------------
 */

int
_TclWParseNestedCmd(interp, wstring, flags, wtermPtr, wpvPtr)
    Tcl_Interp *interp;		/* Interpreter to use for nested command
				 * evaluations and error messages. */
    wchar_t *wstring;		/* Character just after opening bracket. */
    int flags;			/* Flags to pass to nested Tcl_Eval. */
    wchar_t **wtermPtr;		/* Store address of terminating character
				 * here. */
    _WParseValue *wpvPtr;	/* Information about where to place
				 * result of command. */
{
    int result, length, shortfall;
    Interp *iPtr = (Interp *) interp;
    wchar_t *wres;
    size_t  wlen, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    iPtr->evalFlags = flags | TCL_BRACKET_TERM;
    result = _Tcl_WEval(interp, wstring);
    *wtermPtr = iPtr->wtermPtr;
    if (result != TCL_OK) {
	/*
	 * The increment below results in slightly cleaner message in
	 * the errorInfo variable (the close-bracket will appear).
	 */

	if (**wtermPtr == i18n_special_chars.w_cl_sbr) {
	    *wtermPtr += 1;
	}
	return result;
    }
    (*wtermPtr) += 1;
    length = strlen(iPtr->result);
    shortfall = length + 1 - (wpvPtr->end - wpvPtr->next);
    if (shortfall > 0) {
	(*wpvPtr->expandProc)(wpvPtr, shortfall);
    }

    _Multibyte_to_WChar(interp, wlen, iPtr->result, wres, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    wcscpy(wpvPtr->next, wres);
    wpvPtr->next += wlen;
    ckfree(wres);

    Tcl_FreeResult(iPtr);
    iPtr->result = iPtr->resultSpace;
    iPtr->resultSpace[0] = '\0';
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * _TclWParseBraces --
 *
 *	This procedure scans the information between matching
 *	curly braces.
 *
 * Results:
 *	The return value is a standard Tcl result, which is
 *	TCL_OK unless there was an error while parsing string.
 *	If an error occurs then interp->result contains a
 *	standard error message.  *TermPtr is filled
 *	in with the address of the character just after the
 *	last one successfully processed;  this is usually the
 *	character just after the matching close-brace.  The
 *	information between curly braces is stored in standard
 *	fashion in *pvPtr, null-terminated with pvPtr->next
 *	pointing to the terminating null character.
 *
 * Side effects:
 *	The storage space at *pvPtr may be expanded.
 *
 *--------------------------------------------------------------
 */

int
_TclWParseBraces(interp, wstring, wtermPtr, wpvPtr)
    Tcl_Interp *interp;		/* Interpreter to use for nested command
				 * evaluations and error messages. */
    wchar_t *wstring;		/* Character just after opening bracket. */
    wchar_t **wtermPtr;		/* Store address of terminating character
				 * here. */
    _WParseValue *wpvPtr;	/* Information about where to place
				 * result of command. */
{
    int level;
    wchar_t *wsrc, *wdst, *wend;
    wchar_t wc;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    wsrc = wstring;
    wdst = wpvPtr->next;
    wend = wpvPtr->end;
    level = 1;

    /*
     * Copy the characters one at a time to the result area, stopping
     * when the matching close-brace is found.
     */

    while (1) 
    {
	wc = *wsrc;
	wsrc++;
	if (wdst == wend) 
	{
	    wpvPtr->next = wdst;
	    (*wpvPtr->expandProc)(wpvPtr, 20);
	    wdst = wpvPtr->next;
	    wend = wpvPtr->end;
	}
	*wdst = wc;
	wdst++;
	if (_Tcl_WCtype(interp, wc) == TCL_NORMAL) 
	{
	    continue;
	} 
	else if (wc == i18n_special_chars.w_op_cbra) 
	{
	    level++;
	} 
	else if (wc == i18n_special_chars.w_cl_cbra) 
	{
	    level--;
	    if (level == 0) 
	    {
		wdst--;		/* Don't copy the last close brace. */
		break;
	    }
	} 
	else if (wc == i18n_special_chars.w_bsla) 
	{
	    int count;

	    /*
	     * Must always squish out backslash-newlines, even when in
	     * braces.  This is needed so that this sequence can appear
	     * anywhere in a command, such as the middle of an expression.
	     */

	    if (*wsrc == i18n_special_chars.w_esc_n) 
	    {
		wdst[-1] = _Tcl_WBackslash(interp, wsrc-1, &count);
		wsrc += count - 1;
	    } 
	    else 
	    {
		(void) _Tcl_WBackslash(interp, wsrc-1, &count);
		while (count > 1) 
		{
                    if (wdst == wend) 
		    {
                        wpvPtr->next = wdst;
                        (*wpvPtr->expandProc)(wpvPtr, 20);
                        wdst = wpvPtr->next;
                        wend = wpvPtr->end;
                    }
		    *wdst = *wsrc;
		    wdst++;
		    wsrc++;
		    count--;
		}
	    }
	} 
	else if (wc == i18n_special_chars.w_esc_zero) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_parse_missing_close_brace);
	    Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
	    free(i18n_msg_format);

	    *wtermPtr = wstring-1;
	    return TCL_ERROR;
	}
    }

    *wdst = i18n_special_chars.w_esc_zero;
    wpvPtr->next = wdst;
    *wtermPtr = wsrc;
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * _TclWParseWords --
 *
 *	This procedure parses one or more words from a command
 *	string and creates argv-style pointers to fully-substituted
 *	copies of those words.
 *
 * Results:
 *	The return value is a standard Tcl result.
 *	
 *	*argcPtr is modified to hold a count of the number of words
 *	successfully parsed, which may be 0.  At most maxWords words
 *	will be parsed.  If 0 <= *argcPtr < maxWords then it
 *	means that a command separator was seen.  If *argcPtr
 *	is maxWords then it means that a command separator was
 *	not seen yet.
 *
 *	*TermPtr is filled in with the address of the character
 *	just after the last one successfully processed in the
 *	last word.  This is either the command terminator (if
 *	*argcPtr < maxWords), the character just after the last
 *	one in a word (if *argcPtr is maxWords), or the vicinity
 *	of an error (if the result is not TCL_OK).
 *	
 *	The pointers at *argv are filled in with pointers to the
 *	fully-substituted words, and the actual contents of the
 *	words are copied to the buffer at pvPtr.
 *
 *	If an error occurrs then an error message is left in
 *	interp->result and the information at *argv, *argcPtr,
 *	and *pvPtr may be incomplete.
 *
 * Side effects:
 *	The buffer space in pvPtr may be enlarged by calling its
 *	expandProc.
 *
 *--------------------------------------------------------------
 */

int
_TclWParseWords(interp, wstring, flags, maxWords, wtermPtr, argcPtr, wargv, wpvPtr)
    Tcl_Interp *interp;		/* Interpreter to use for nested command
				 * evaluations and error messages. */
    wchar_t *wstring;		/* First character of word. */
    int flags;			/* Flags to control parsing (same values as
				 * passed to Tcl_Eval). */
    int maxWords;		/* Maximum number of words to parse. */
    wchar_t **wtermPtr;		/* Store address of terminating character
				 * here. */
    int *argcPtr;		/* Filled in with actual number of words
				 * parsed. */
    wchar_t **wargv;		/* Store addresses of individual words here. */
    register _WParseValue *wpvPtr;	
				/* Information about where to place
				 * fully-substituted word. */
{
    wchar_t *wsrc, *wdst;
    wchar_t	  wc;
    int type, result, argc;
    wchar_t *woldBuffer;	/* Used to detect when pvPtr's buffer gets
				 * reallocated, so we can adjust all of the
				 * argv pointers. */
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    wsrc = wstring;
    woldBuffer = wpvPtr->buffer;
    wdst = wpvPtr->next;
    for (argc = 0; argc < maxWords; argc++) 
    {
	wargv[argc] = wdst;

	/*
	 * Skip leading space.
	 */
    
	skipSpace:
	wc = *wsrc;
	type = _Tcl_WCtype(interp, wc);
	while (type == TCL_SPACE) 
	{
	    wsrc++;
	    wc = *wsrc;
	    type = _Tcl_WCtype(interp, wc);
	}
    
	/*
	 * Handle the normal case (i.e. no leading double-quote or brace).
	 */

	if (type == TCL_NORMAL) 
	{
	    normalArg:
	    while (1) 
	    {
		if (wdst == wpvPtr->end) 
		{
		    /*
		     * Target buffer space is about to run out.  Make
		     * more space.
		     */
	
		    wpvPtr->next = wdst;
		    (*wpvPtr->expandProc)(wpvPtr, 1);
		    wdst = wpvPtr->next;
		}
	
		if (type == TCL_NORMAL) 
		{
		    copy:
		    *wdst = wc;
		    wdst++;
		    wsrc++;
		} 
		else if (type == TCL_SPACE) 
		{
		    goto wordEnd;
		} 
		else if (type == TCL_DOLLAR) 
		{
		    int length;
		    wchar_t *wvalue;
	
		    wvalue = _Tcl_WParseVar(interp, wsrc, wtermPtr);
		    if (wvalue == (wchar_t *)NULL) 
		    {
			return TCL_ERROR;
		    }
		    wsrc = *wtermPtr;
		    length = wcslen(wvalue);
		    if ((wpvPtr->end - wdst) <= length) 
		    {
			wpvPtr->next = wdst;
			(*wpvPtr->expandProc)(wpvPtr, length);
			wdst = wpvPtr->next;
		    }
		    wcscpy(wdst, wvalue);
		    ckfree(wvalue);	/* return from WParseVar */
		    wdst += length;
		} 
		else if (type == TCL_COMMAND_END) 
		{
		    if ((wc == i18n_special_chars.w_cl_sbr) 
				&& !(flags & TCL_BRACKET_TERM)) 
		    {
			goto copy;
		    }

		    /*
		     * End of command;  simulate a word-end first, so
		     * that the end-of-command can be processed as the
		     * first thing in a new word.
		     */

		    goto wordEnd;
		} 
	        else if (type == TCL_OPEN_BRACKET) 
	        {
		    wpvPtr->next = wdst;
		    result = _TclWParseNestedCmd(interp, wsrc+1, flags, 
				wtermPtr, wpvPtr);
		    if (result != TCL_OK) 
		    {
			return result;
		    }
		    wsrc = *wtermPtr;
		    wdst = wpvPtr->next;
		} 
		else if (type == TCL_BACKSLASH) 
		{
		    int numRead;
    
		    *wdst = _Tcl_WBackslash(interp, wsrc, &numRead);
		    if (*wdst == i18n_special_chars.w_esc_zero)
			return TCL_CONV_ERROR;

		    /*
		     * The following special check allows a backslash-newline
		     * to be treated as a word-separator, as if the backslash
		     * and newline had been collapsed before command parsing
		     * began.
		     */

		    if (wsrc[1] == i18n_special_chars.w_esc_n) 
		    {
			wsrc += numRead;
			goto wordEnd;
		    }
		    wsrc += numRead;
		    wdst++;
		} 
		else 
		{
		    goto copy;
		}
		wc = *wsrc;
		type = _Tcl_WCtype(interp, wc);
	    }
	} 
	else 
	{
	    /*
	     * Check for the end of the command.
	     */
	    if (type == TCL_COMMAND_END) 
	    {
		if (flags & TCL_BRACKET_TERM) 
		{
		    if (wc == i18n_special_chars.w_esc_zero) 
		    {
			i18n_msg_format = dce_msg_get(tcl_s_parse_missing_close_bracket);
			Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
			free(i18n_msg_format);

			return TCL_ERROR;
		    }
		} 
		else 
		{
		    if (wc == i18n_special_chars.w_cl_sbr) 
		    {
			goto normalArg;
		    }
		}
		goto done;
	    }
	
	    /*
	     * Now handle the special cases: open braces, double-quotes,
	     * and backslash-newline.
	     */

	    wpvPtr->next = wdst;
	    if (type == TCL_QUOTE) 
	    {
		result = _TclWParseQuotes(interp, wsrc+1, 
			i18n_special_chars.w_dob_qo, flags, wtermPtr, wpvPtr);
	    } 
	    else if (type == TCL_OPEN_BRACE) 
	    {
		result = _TclWParseBraces(interp, wsrc+1, wtermPtr, wpvPtr);
	    } 
	    else if ((type == TCL_BACKSLASH) 
			&& (wsrc[1] == i18n_special_chars.w_esc_n)) 
	    {
		/*
		 * This code is needed so that a backslash-newline at the
		 * very beginning of a word is treated as part of the white
		 * space between words and not as a space within the word.
		 */

		wsrc += 2;
		goto skipSpace;
	    } 
	    else 
	    {
		goto normalArg;
	    }
	    if (result != TCL_OK) 
	    {
		return result;
	    }
	
	    /*
	     * Back from quotes or braces;  make sure that the terminating
	     * character was the end of the word.  Have to be careful here
	     * to handle continuation lines (i.e. lines ending in backslash).
	     */
	
	    wc = **wtermPtr;
	    if ((wc == i18n_special_chars.w_bsla) 
		&& ((*wtermPtr)[1] == i18n_special_chars.w_esc_n)) 
	    {
		wc = (*wtermPtr)[2];
	    }
	    type = _Tcl_WCtype(interp, wc);
	    if ((type != TCL_SPACE) && (type != TCL_COMMAND_END)) 
	    {
		if (*wsrc == i18n_special_chars.w_dob_qo) {
		    i18n_msg_format = dce_msg_get(tcl_s_parse_extra_char_quote);
		    Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
		    free(i18n_msg_format);

		} 
	 	else 
	 	{
		    i18n_msg_format = dce_msg_get(tcl_s_parse_extra_char_brace);
		    Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
		    free(i18n_msg_format);
		}
		return TCL_ERROR;
	    }
	    wsrc = *wtermPtr;
	    wdst = wpvPtr->next;

	}

	/*
	 * We're at the end of a word, so add a null terminator.  Then
	 * see if the buffer was re-allocated during this word.  If so,
	 * update all of the argv pointers.
	 */

	wordEnd:
	*wdst = i18n_special_chars.w_esc_zero;
	wdst++;
	if (woldBuffer != wpvPtr->buffer) {
	    int i;

	    for (i = 0; i <= argc; i++) {
		wargv[i] = wpvPtr->buffer + (wargv[i] - woldBuffer);
	    }
	    woldBuffer = wpvPtr->buffer;
	}
    }

    done:
    wpvPtr->next = wdst;
    *wtermPtr = wsrc;
    *argcPtr = argc;
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * _TclWExpandParseValue --
 *
 *	This procedure is commonly used as the value of the
 *	expandProc in a ParseValue.  It uses malloc to allocate
 *	more space for the result of a parse.
 *
 * Results:
 *	The buffer space in *pvPtr is reallocated to something
 *	larger, and if pvPtr->clientData is non-zero the old
 *	buffer is freed.  Information is copied from the old
 *	buffer to the new one.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

void
_TclWExpandParseValue(wpvPtr, needed)
    register _WParseValue *wpvPtr;	/* Information about buffer that
					 * must be expanded.  If the clientData
					 * in the structure is non-zero, it
					 * means that the current buffer is
					 * dynamically allocated. */
    int needed;				/* Minimum amount of additional space
					 * to allocate. */
{
    int newSpace;
    wchar_t *new;
    size_t  copySize;

    /*
     * Either double the size of the buffer or add enough new space
     * to meet the demand, whichever produces a larger new buffer.
     */

    newSpace = (wpvPtr->end - wpvPtr->buffer) + sizeof(wchar_t);
    if (newSpace < needed) {
	newSpace += needed * sizeof(wchar_t);
    } else {
	newSpace += newSpace;
    }
    new = (wchar_t *) ckalloc((unsigned) newSpace);

    /*
     * Copy from old buffer to new, free old buffer if needed, and
     * mark new buffer as malloc-ed.
     */

    /* copySize = wpvPtr->next - wpvPtr->buffer + sizeof(wchar_t); */
    copySize = wpvPtr->next - wpvPtr->buffer;
    memcpy((VOID *) new, (VOID *) wpvPtr->buffer, copySize);
    wpvPtr->next = new + (wpvPtr->next - wpvPtr->buffer);
    if (wpvPtr->clientData != 0) {
    		ckfree(wpvPtr->buffer);
    }
    wpvPtr->buffer = new;
    wpvPtr->end = new + newSpace - 1;
    wpvPtr->clientData = (ClientData) 1;
}

/*
 *----------------------------------------------------------------------
 *
 * _TclWWordEnd --
 *
 *	Given a pointer into a Tcl command, find the end of the next
 *	word of the command.
 *
 * Results:
 *	The return value is a pointer to the last character that's part
 *	of the word pointed to by "start".  If the word doesn't end
 *	properly within the string then the return value is the address
 *	of the null character at the end of the string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

wchar_t *
_TclWWordEnd(interp, wstart, nested, semiPtr)
    Tcl_Interp *interp;
    wchar_t *wstart;		/* Beginning of a word of a Tcl command. */
    int nested;			/* Zero means this is a top-level command.
				 * One means this is a nested command (close
				 * brace is a word terminator). */
    int *semiPtr;		/* Set to 1 if word ends with a command-
				 * terminating semi-colon, zero otherwise.
				 * If NULL then ignored. */
{
    wchar_t *wp;
    int count;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return (wchar_t *)NULL;
    }

    if (semiPtr != NULL) 
    {
	*semiPtr = 0;
    }

    /*
     * Skip leading white space (backslash-newline must be treated like
     * white-space, except that it better not be the last thing in the
     * command).
     */

    for (wp = wstart; ; wp++) 
    {
	if (iswspace(*wp)) 
	{
	    continue;
	}
	if ((wp[0] == i18n_special_chars.w_bsla) 
			&& (wp[1] == i18n_special_chars.w_esc_n)) 
	{
	    if (wp[2] == i18n_special_chars.w_esc_zero) 
	    {
		return wp+2;
	    }
	    continue;
	}
	break;
    }

    /*
     * Handle words beginning with a double-quote or a brace.
     */

    if (*wp == i18n_special_chars.w_dob_qo) 
    {
	wp = _WQuoteEnd(interp, wp+1, i18n_special_chars.w_dob_qo);
	if (*wp == i18n_special_chars.w_esc_zero) 
	{
	    return wp;
	}
	wp++;
    } 
    else if (*wp == i18n_special_chars.w_op_cbra) 
    {
	int braces = 1;
	while (braces != 0) 
	{
	    wp++;
	    while (*wp == i18n_special_chars.w_bsla) 
	    {
		(void) _Tcl_WBackslash(interp, wp, &count);
		wp += count;
	    }
	    if (*wp == i18n_special_chars.w_cl_cbra) 
	    {
		braces--;
	    } 
	    else if (*wp == i18n_special_chars.w_op_cbra) 
	    {
		braces++;
	    } 
	    else if (*wp == i18n_special_chars.w_esc_zero) 
	    {
		return wp;
	    }
	}
	wp++;
    }

    /*
     * Handle words that don't start with a brace or double-quote.
     * This code is also invoked if the word starts with a brace or
     * double-quote and there is garbage after the closing brace or
     * quote.  This is an error as far as Tcl_Eval is concerned, but
     * for here the garbage is treated as part of the word.
     */

    while (1) 
    {
	if (*wp == i18n_special_chars.w_op_sbr) 
	{
	    for (wp++; *wp != i18n_special_chars.w_cl_sbr; wp++) 
	    {
		wp = _TclWWordEnd(interp, wp, 1, (int *) NULL);
		if (*wp == i18n_special_chars.w_esc_zero) 
		{
		    return wp;
		}
	    }
	    wp++;
	} 
	else if (*wp == i18n_special_chars.w_bsla) 
	{
	    (void) _Tcl_WBackslash(interp, wp, &count);
	    wp += count;
	    if ((*wp == i18n_special_chars.w_esc_zero) 
			&& (count == 2) 
			&& (wp[-1] == i18n_special_chars.w_esc_n)) 
	    {
		return wp;
	    }
	} 
	else if (*wp == i18n_special_chars.w_dollar) 
	{
	    wp = _WVarNameEnd(interp, wp);
	    if (*wp == i18n_special_chars.w_esc_zero) 
	    {
		return wp;
	    }
	    wp++;
	} 
	else if (*wp == i18n_special_chars.w_sem_col) 
	{
	    /*
	     * Include the semi-colon in the word that is returned.
	     */

	    if (semiPtr != NULL) 
	    {
		*semiPtr = 1;
	    }
	    return wp;
	} 
	else if (iswspace(*wp)) 
	{
	    return wp-1;
	} 
	else if ((*wp == i18n_special_chars.w_cl_sbr) && nested) 
	{
	    return wp-1;
	} 
	else if (*wp == i18n_special_chars.w_esc_zero) 
	{
	    if (nested) 
	    {
		/*
		 * Nested commands can't end because of the end of the
		 * string.
		 */
		return wp;
	    }
	    return wp-1;
	} 
	else 
	{
	    wp++;
	}
    }
}

/*
 *----------------------------------------------------------------------
 *
 * _WQuoteEnd --
 *
 *	Given a pointer to a string that obeys the parsing conventions
 *	for quoted things in Tcl, find the end of that quoted thing.
 *	The actual thing may be a quoted argument or a parenthesized
 *	index name.
 *
 * Results:
 *	The return value is a pointer to the last character that is
 *	part of the quoted string (i.e the character that's equal to
 *	term).  If the quoted string doesn't terminate properly then
 *	the return value is a pointer to the null character at the
 *	end of the string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static wchar_t *
#ifdef _ANSI_ARGS_
_WQuoteEnd(Tcl_Interp *interp, wchar_t *wstring, wchar_t wterm)
#else
_WQuoteEnd(interp, wstring, wterm)
    Tcl_Interp *interp;
    wchar_t *wstring;		/* Pointer to character just after opening
				 * "quote". */
    wchar_t wterm;		/* This character will terminate the
				 * quoted string (e.g. '"' or ')'). */
#endif
{
    wchar_t *wp = wstring;
    int count;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return (wchar_t *)NULL;
    }

    while (*wp != wterm) {
	if (*wp == i18n_special_chars.w_bsla) 
	{
	    (void) _Tcl_WBackslash(interp, wp, &count);
	    wp += count;
	} 
	else if (*wp == i18n_special_chars.w_op_sbr) 
	{
	    for (wp++; *wp != i18n_special_chars.w_cl_sbr; wp++) 
	    {
		wp = _TclWWordEnd(interp, wp, 1, (int *) NULL);
		if (*wp == i18n_special_chars.w_esc_zero) 
		{
		    return wp;
		}
	    }
	    wp++;
	} 
	else if (*wp == i18n_special_chars.w_dollar) 
	{
	    wp = _WVarNameEnd(interp, wp);
	    if (*wp == i18n_special_chars.w_esc_zero) 
	    {
		return wp;
	    }
	    wp++;
	} 
	else if (*wp == i18n_special_chars.w_esc_zero) 
	{
	    return wp;
	} 
	else 
	{
	    wp++;
	}
    }
    return wp-1;
}

/*
 *----------------------------------------------------------------------
 *
 * _WVarNameEnd --
 *
 *	Given a pointer to a variable reference using $-notation, find
 *	the end of the variable name spec.
 *
 * Results:
 *	The return value is a pointer to the last character that
 *	is part of the variable name.  If the variable name doesn't
 *	terminate properly then the return value is a pointer to the
 *	null character at the end of the string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static wchar_t *
_WVarNameEnd(interp, wstring)
    Tcl_Interp *interp;
    wchar_t *wstring;		/* Pointer to dollar-sign character. */
{
    wchar_t *wp = wstring+1;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return (wchar_t)NULL;
    }

    if (*wp == i18n_special_chars.w_op_cbra) 
    {
	for (wp++; (*wp != i18n_special_chars.w_cl_cbra) 
		&& (*wp != i18n_special_chars.w_esc_zero); wp++) 
	{
	    /* Empty loop body. */
	}
	return wp;
    }
    while (iswalnum(*wp) || (*wp == i18n_special_chars.w_unscore)) 
    {
	wp++;
    }
    if ((*wp == i18n_special_chars.w_op_paren) && (wp != wstring+1)) 
    {
	return _WQuoteEnd(interp, wp+1, i18n_special_chars.w_cl_paren);
    }
    return wp-1;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WParseVar --
 *
 *	Given a string starting with a $ sign, parse off a variable
 *	name and return its value.
 *
 * Results:
 *	The return value is the contents of the variable given by
 *	the leading characters of string.  If termPtr isn't NULL,
 *	*termPtr gets filled in with the address of the character
 *	just after the last one in the variable specifier.  If the
 *	variable doesn't exist, then the return value is NULL and
 *	an error message will be left in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

wchar_t *
_Tcl_WParseVar(interp, wstring, wtermPtr)
    Tcl_Interp *interp;			/* Context for looking up variable. */
    wchar_t *wstring;			/* String containing variable name.
					 * First character must be "$". */
    wchar_t **wtermPtr;			/* If non-NULL, points to word to fill
					 * in with character just after last
					 * one in the variable specifier. */

{
    wchar_t *wname1, *wname1End, *wresult, wc;
    wchar_t *wname2, *w_ret_value;
    char    *name1, *name2, *result;
#define NUM_CHARS 200
    wchar_t wcopyStorage[NUM_CHARS];
    _WParseValue wpv;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    size_t  clen, wlen, st;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return (wchar_t *)NULL;
    }

    /*
     * There are three cases:
     * 1. The $ sign is followed by an open curly brace.  Then the variable
     *    name is everything up to the next close curly brace, and the
     *    variable is a scalar variable.
     * 2. The $ sign is not followed by an open curly brace.  Then the
     *    variable name is everything up to the next character that isn't
     *    a letter, digit, or underscore.  If the following character is an
     *    open parenthesis, then the information between parentheses is
     *    the array element name, which can include any of the substitutions
     *    permissible between quotes.
     * 3. The $ sign is followed by something that isn't a letter, digit,
     *    or underscore:  in this case, there is no variable name, and "$"
     *    is returned.
     */

    wname2 = NULL;
    wstring++;
    if (*wstring == i18n_special_chars.w_op_cbra) 
    {
	wstring++;
	wname1 = wstring;
	while (*wstring != i18n_special_chars.w_cl_cbra) 
	{
	    if (*wstring == i18n_special_chars.w_esc_zero) 
	    {
		i18n_msg_format = dce_msg_get(tcl_s_parse_missing_close_brace_var);
		Tcl_SetResult(interp, (char *)i18n_msg_format, TCL_VOLATILE);
		free(i18n_msg_format);

		if (wtermPtr != NULL) 
		{
		    *wtermPtr = wstring;
		}
		return (wchar_t *)NULL;
	    }
	    wstring++;
	}
	wname1End = wstring;
	wstring++;
    } 
    else 
    {
	wname1 = wstring;
	while (iswalnum(*wstring) 
			|| (*wstring == i18n_special_chars.w_unscore)) 
	{
	    wstring++;
	}
	if (wstring == wname1) 
	{
	
	    if (wtermPtr != NULL) 
	    {
		*wtermPtr = wstring;
	    }
	    w_ret_value = (wchar_t *)ckalloc(2 * sizeof(wchar_t));
	    w_ret_value[0] = i18n_special_chars.w_dollar;
	    w_ret_value[1] = i18n_special_chars.w_esc_zero;
	    return w_ret_value;
	}
	wname1End = wstring;
	if (*wstring == i18n_special_chars.w_op_paren) 
	{
	    wchar_t *wend;

	    /*
	     * Perform substitutions on the array element name, just as
	     * is done for quotes.
	     */

	    wpv.buffer = wpv.next = wcopyStorage;
	    wpv.end = wcopyStorage + NUM_CHARS - 1;
	    wpv.expandProc = _TclWExpandParseValue;
	    wpv.clientData = (ClientData) NULL;
	    if (_TclWParseQuotes(interp, wstring+1, 
		     i18n_special_chars.w_cl_paren, 0, &wend, &wpv) != TCL_OK)
	    {
		char msg[200];
		char *ctemp1, *ctemp2;
		wchar_t	*temp;

		temp = (wchar_t *)(wstring - wname1);

		_WChar_To_Multibyte(interp, clen, temp, ctemp1, st)
	 	if (st == (size_t)-1)
		{
			result = NULL;
			goto done;
		}
		
		_WChar_To_Multibyte(interp, clen, wname1, ctemp2, st)
	 	if (st == (size_t)-1)
		{
			result = NULL;
			ckfree(ctemp1);
			goto done;
		}

		i18n_msg_format = dce_msg_get(tcl_s_parse_index_for_array);
		sprintf(msg, (char *)i18n_msg_format, ctemp1, ctemp2);
		free(i18n_msg_format);
		ckfree(ctemp1);
		ckfree(ctemp2);

		Tcl_AddErrorInfo(interp, msg);
		result = NULL;
		wname2 = wpv.buffer;
		if (wtermPtr != NULL) {
		    *wtermPtr = wend;
		}
		goto done;
	    }
	    Tcl_ResetResult(interp);
	    wstring = wend;
	    wname2 = wpv.buffer;
	}
    }
    if (wtermPtr != NULL) 
    {
	*wtermPtr = wstring;
    }

    if (((Interp *) interp)->noEval) {
	w_ret_value = (wchar_t *)ckalloc(2 * sizeof(wchar_t));
	w_ret_value[0] = i18n_special_chars.w_empty;
	w_ret_value[1] = i18n_special_chars.w_esc_zero;
	return w_ret_value;
    }
    wc = *wname1End;
    *wname1End = i18n_special_chars.w_esc_zero;

    /* Convert wide character name to multibyte name */
    _WChar_To_Multibyte(interp, wlen, wname1, name1, st)
    if (st == (size_t)-1)
	return (wchar_t *)NULL;

    if (wname2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, wlen, wname2, name2, st)
	if (st == (size_t)-1)
	{
	    ckfree(name1);
	    return (wchar_t *)NULL;
	}
    }
    else
	name2 = (char *)NULL;

    result = Tcl_GetVar2(interp, name1, name2, TCL_LEAVE_ERR_MSG);
    ckfree(name1);
    if (name2 != (char *)NULL)
	ckfree(name2);
    *wname1End = wc;

    done:
    if ((wname2 != NULL) && (wpv.buffer != wcopyStorage)) {
	ckfree(wpv.buffer);
    }

    if (result == NULL)
	return (wchar_t *)NULL;

    /* Convert multibyte character result to wide character result */
    _Multibyte_to_WChar(interp, wlen, result, wresult, st) 
    if (st == (size_t)-1)
	return (wchar_t *)NULL;

    return wresult;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WCommandComplete --
 *
 *	Given a partial or complete Tcl command, this procedure
 *	determines whether the command is complete in the sense
 *	of having matched braces and quotes and brackets.
 *
 * Results:
 *	1 is returned if the command is complete, 0 otherwise.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WCommandComplete(interp, cmd)
    Tcl_Interp *interp;
    char *cmd;			/* Command to check. */
{
    register char *p = cmd;
    int commentOK = 1;
    size_t	wsn, st;
    wchar_t	*wp, *wp_save;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /* Convert cmd to wide character string */
    _Multibyte_to_WChar(&interp, wsn, p, wp, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    wp_save = wp;
    while (1) 
    {
	while (iswspace(*wp)) 
	{
		if (*wp == i18n_special_chars.w_esc_n)
		{
			commentOK = 1;
		}
		wp++;
	}
	if ((*wp == i18n_special_chars.w_sharp) && commentOK)
	{
		do 
		{
			wp++;
		} while ((*wp != i18n_special_chars.w_esc_n) 
				&& (*wp != i18n_special_chars.w_esc_zero));
		continue;
	}
	if (*wp == i18n_special_chars.w_esc_zero) 
	{
		ckfree(wp_save);
		return 1;
	}
	wp = _TclWWordEnd(interp, wp, 0, &commentOK);
	if (*wp == i18n_special_chars.w_esc_zero) 
	{
		ckfree(wp_save);
		return 0;
	}
	wp++;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WCtype --
 *
 *	This replaces CHAR_TYPE macro for wide character type.
 *
 * Results:
 * 	Possible values returned:
 *
 * 	TCL_NORMAL -        All characters that don't have special significance
 *                          to the Tcl language.
 * 	TCL_SPACE -         Character is space, tab, or return.
 * 	TCL_COMMAND_END -   Character is newline or null or semicolon or
 *                          close-bracket.
 * 	TCL_QUOTE -         Character is a double-quote.
 * 	TCL_OPEN_BRACKET -  Character is a "[".
 * 	TCL_OPEN_BRACE -    Character is a "{".
 * 	TCL_CLOSE_BRACE -   Character is a "}".
 * 	TCL_BACKSLASH -     Character is a "\".
 * 	TCL_DOLLAR -        Character is a "$".
 *
 * Side effects:
 *	None.
 */
int
#ifdef _ANSI_ARGS_
_Tcl_WCtype(Tcl_Interp *interp,	wchar_t wc)
#else
_Tcl_WCtype(interp, wc)
	Tcl_Interp *interp;
	wchar_t		wc;
#endif
{
    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    if (wc == i18n_special_chars.w_null || 
	wc == i18n_special_chars.w_esc_zero)
	return TCL_COMMAND_END;
    else if (wc == i18n_special_chars.w_tab)
	return TCL_SPACE;
    else if (wc == i18n_special_chars.w_lf)
	return TCL_COMMAND_END;
    else if (wc == i18n_special_chars.w_vt)
	return TCL_SPACE;
    else if (wc == i18n_special_chars.w_ff)
	return TCL_SPACE;
    else if (wc == i18n_special_chars.w_space)
	return TCL_SPACE;
    else if (wc == i18n_special_chars.w_cr)
	return TCL_SPACE;
    else if (wc == i18n_special_chars.w_bsla)
	return TCL_BACKSLASH;
    else if (wc == i18n_special_chars.w_op_sbr)
	return TCL_OPEN_BRACKET;
    else if (wc == i18n_special_chars.w_cl_sbr)
	return TCL_COMMAND_END;
    else if (wc == i18n_special_chars.w_sem_col)
	return TCL_COMMAND_END;
    else if (wc == i18n_special_chars.w_dollar)
	return TCL_DOLLAR;
    else if (wc == i18n_special_chars.w_dob_qo)
	return TCL_QUOTE;
    else if (wc == i18n_special_chars.w_op_cbra)
	return TCL_OPEN_BRACE;
    else if (wc == i18n_special_chars.w_cl_cbra)
	return TCL_CLOSE_BRACE;
    else
	return TCL_NORMAL;
}
