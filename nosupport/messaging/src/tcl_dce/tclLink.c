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
 * $Log: tclLink.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.6.1  1995/02/16  22:00:31  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/16  20:40:22  levy]
 *
 * Revision 1.1.4.1  1994/10/21  21:39:12  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:32:19  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:23  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:12:34  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclLink.c --
 *
 *	This file implements linked variables (a C variable that is
 *	tied to a Tcl variable).  The idea of linked variables was
 *	first suggested by Andreas Stocke and this implementation is
 *	based heavily on a prototype implementation provided by
 *	him.
 *
 * Copyright (c) 1993 The Regents of the University of California.
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
#include <string.h>

/*
 * For each linked variable there is a data structure of the following
 * type, which describes the link and is the clientData for the trace
 * set on the Tcl variable.
 */

typedef struct Link {
    Tcl_Interp *interp;		/* Interpreter containing Tcl variable. */
    char *addr;			/* Location of C variable. */
    int type;			/* Type of link (TCL_LINK_INT, etc.). */
    int writable;		/* Zero means Tcl variable is read-only. */
    union {
	int i;
	double d;
    } lastValue;		/* Last known value of C variable;  used to
				 * avoid string conversions. */
} Link;

/*
 * Forward references to procedures defined later in this file:
 */

static char *		LinkTraceProc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, char *name1, char *name2,
			    int flags));
static char *		_WLinkTraceProc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, char *name1, char *name2,
			    int flags));
static char *		StringValue _ANSI_ARGS_((Link *linkPtr,
			    char *buffer));

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LinkVar --
 *
 *	Link a C variable to a Tcl variable so that changes to either
 *	one causes the other to change.
 *
 * Results:
 *	The return value is TCL_OK if everything went well or TCL_ERROR
 *	if an error occurred (interp->result is also set after errors).
 *
 * Side effects:
 *	The value at *addr is linked to the Tcl variable "varName",
 *	using "type" to convert between string values for Tcl and
 *	binary values for *addr.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_LinkVar(interp, varName, addr, type)
    Tcl_Interp *interp;		/* Interpreter in which varName exists. */
    char *varName;		/* Name of a global variable in interp. */
    char *addr;			/* Address of a C variable to be linked
				 * to varName. */
    int type;			/* Type of C variable: TCL_LINK_INT, etc. 
				 * Also may have TCL_LINK_READ_ONLY
				 * OR'ed in. */
{
    Link *linkPtr;
    char buffer[TCL_DOUBLE_SPACE];
    int code;

    linkPtr = (Link *) ckalloc(sizeof(Link));
    linkPtr->interp = interp;
    linkPtr->addr = addr;
    linkPtr->type = type & ~TCL_LINK_READ_ONLY;
    linkPtr->writable = (type & TCL_LINK_READ_ONLY) == 0;
    if (Tcl_SetVar(interp, varName, StringValue(linkPtr, buffer),
	    TCL_GLOBAL_ONLY|TCL_LEAVE_ERR_MSG) == NULL) {
	ckfree((char *) linkPtr);
	return TCL_ERROR;
    }
    code = Tcl_TraceVar(interp, varName, TCL_GLOBAL_ONLY|TCL_TRACE_READS
	    |TCL_TRACE_WRITES|TCL_TRACE_UNSETS, LinkTraceProc,
	    (ClientData) linkPtr);
    if (code != TCL_OK) {
	ckfree((char *) linkPtr);
    }
    return code;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnlinkVar --
 *
 *	Destroy the link between a Tcl variable and a C variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If "varName" was previously linked to a C variable, the link
 *	is broken to make the variable independent.  If there was no
 *	previous link for "varName" then nothing happens.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_UnlinkVar(interp, varName)
    Tcl_Interp *interp;		/* Interpreter containing variable to unlink. */
    char *varName;		/* Global variable in interp to unlink. */
{
    Link *linkPtr;

    linkPtr = (Link *) Tcl_VarTraceInfo(interp, varName, TCL_GLOBAL_ONLY,
	    LinkTraceProc, (ClientData) NULL);
    if (linkPtr == NULL) {
	return;
    }
    Tcl_UntraceVar(interp, varName,
	    TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS,
	    LinkTraceProc, (ClientData) linkPtr);
    ckfree((char *) linkPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * LinkTraceProc --
 *
 *	This procedure is invoked when a linked Tcl variable is read,
 *	written, or unset from Tcl.  It's responsible for keeping the
 *	C variable in sync with the Tcl variable.
 *
 * Results:
 *	If all goes well, NULL is returned; otherwise an error message
 *	is returned.
 *
 * Side effects:
 *	The C variable may be updated to make it consistent with the
 *	Tcl variable, or the Tcl variable may be overwritten to reject
 *	a modification.
 *
 *----------------------------------------------------------------------
 */

static char *
LinkTraceProc(clientData, interp, name1, name2, flags)
    ClientData clientData;	/* Contains information about the link. */
    Tcl_Interp *interp;		/* Interpreter containing Tcl variable. */
    char *name1;		/* First part of variable name. */
    char *name2;		/* Second part of variable name. */
    int flags;			/* Miscellaneous additional information. */
{
    Link *linkPtr = (Link *) clientData;
    int changed;
    char buffer[TCL_DOUBLE_SPACE];
    char *value, **pp;
    Tcl_DString savedResult;

    /*
     * If the variable is being unset, then just re-create it (with a
     * trace) unless the whole interpreter is going away.
     */

    if (flags & TCL_TRACE_UNSETS) {
	if (flags & TCL_INTERP_DESTROYED) {
	    ckfree((char *) linkPtr);
	}
	if (flags & TCL_TRACE_DESTROYED) {
	    Tcl_SetVar2(interp, name1, name2,
		    StringValue(linkPtr, buffer), TCL_GLOBAL_ONLY);
	    Tcl_TraceVar2(interp, name1, name2, TCL_GLOBAL_ONLY
		    |TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS,
		    LinkTraceProc, (ClientData) linkPtr);
	}
	return NULL;
    }

    /*
     * For read accesses, update the Tcl variable if the C variable
     * has changed since the last time we updated the Tcl variable.
     */

    if (flags & TCL_TRACE_READS) {
	switch (linkPtr->type) {
	    case TCL_LINK_INT:
	    case TCL_LINK_BOOLEAN:
		changed = *(int *)(linkPtr->addr) != linkPtr->lastValue.i;
		break;
	    case TCL_LINK_DOUBLE:
		changed = *(double *)(linkPtr->addr) != linkPtr->lastValue.d;
		break;
	    case TCL_LINK_STRING:
		changed = 1;
		break;
	    default:
		/* i18n
		return "internal error: bad linked variable type";
		 */
		return((char *)dce_msg_get(tcl_s_link_bad_linked_var_type));
	}
	if (changed) {
	    Tcl_SetVar2(interp, name1, name2, StringValue(linkPtr, buffer),
		    TCL_GLOBAL_ONLY);
	}
	return NULL;
    }

    /*
     * For writes, first make sure that the variable is writable.  Then
     * convert the Tcl value to C if possible.  If the variable isn't
     * writable or can't be converted, then restore the varaible's old
     * value and return an error.  Another tricky thing: we have to save
     * and restore the interpreter's result, since the variable access
     * could occur when the result has been partially set.
     */

    if (!linkPtr->writable) {
	Tcl_SetVar2(interp, name1, name2, StringValue(linkPtr, buffer),
		    TCL_GLOBAL_ONLY);
	/* i18n
	return "linked variable is read-only";
	 */
	return((char *)dce_msg_get(tcl_s_link_linked_var_read_only));
    }
    value = Tcl_GetVar2(interp, name1, name2, TCL_GLOBAL_ONLY);
    if (value == NULL) {
	/*
	 * This shouldn't ever happen.
	 */
	/* i18n
	return "internal error: linked variable couldn't be read";
	 */
	return((char *)dce_msg_get(tcl_s_link_linked_var_unreadable));
    }
    Tcl_DStringInit(&savedResult);
    Tcl_DStringAppend(&savedResult, interp->result, -1);
    Tcl_ResetResult(interp);
    switch (linkPtr->type) {
	case TCL_LINK_INT:
	    if (Tcl_GetInt(interp, value, &linkPtr->lastValue.i) != TCL_OK) {
		Tcl_DStringResult(interp, &savedResult);
		Tcl_SetVar2(interp, name1, name2, StringValue(linkPtr, buffer),
			TCL_GLOBAL_ONLY);
		/* i18n
		return "variable must have integer value";
		 */
		return((char *)dce_msg_get(tcl_s_link_var_must_be_integer));
	    }
	    *(int *)(linkPtr->addr) = linkPtr->lastValue.i;
	    break;
	case TCL_LINK_DOUBLE:
	    if (Tcl_GetDouble(interp, value, &linkPtr->lastValue.d)
		    != TCL_OK) {
		Tcl_DStringResult(interp, &savedResult);
		Tcl_SetVar2(interp, name1, name2, StringValue(linkPtr, buffer),
			TCL_GLOBAL_ONLY);
		/* i18n
		return "variable must have real value";
		 */
		return((char *)dce_msg_get(tcl_s_link_var_must_be_real));
	    }
	    *(double *)(linkPtr->addr) = linkPtr->lastValue.d;
	    break;
	case TCL_LINK_BOOLEAN:
	    if (Tcl_GetBoolean(interp, value, &linkPtr->lastValue.i)
		    != TCL_OK) {
		Tcl_DStringResult(interp, &savedResult);
		Tcl_SetVar2(interp, name1, name2, StringValue(linkPtr, buffer),
			TCL_GLOBAL_ONLY);
		/* i18n
		return "variable must have boolean value";
		 */
		return((char *)dce_msg_get(tcl_s_link_var_must_be_bool));
	    }
	    *(int *)(linkPtr->addr) = linkPtr->lastValue.i;
	    break;
	case TCL_LINK_STRING:
	    pp = (char **)(linkPtr->addr);
	    if (*pp != NULL) {
		ckfree(*pp);
	    }
	    *pp = ckalloc((unsigned) (strlen(value) + 1));
	    strcpy(*pp, value);
	    break;
	default:
	    /* i18n
	    return "internal error: bad linked variable type";
	     */
	    return((char *)dce_msg_get(tcl_s_link_bad_linked_var_type));
    }
    Tcl_DStringResult(interp, &savedResult);
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * StringValue --
 *
 *	Converts the value of a C variable to a string for use in a
 *	Tcl variable to which it is linked.
 *
 * Results:
 *	The return value is a pointer
 to a string that represents
 *	the value of the C variable given by linkPtr.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static char *
StringValue(linkPtr, buffer)
    Link *linkPtr;		/* Structure describing linked variable. */
    char *buffer;		/* Small buffer to use for converting
				 * values.  Must have TCL_DOUBLE_SPACE
				 * bytes or more. */
{
    char *p;

    switch (linkPtr->type) {
	case TCL_LINK_INT:
	    linkPtr->lastValue.i = *(int *)(linkPtr->addr);
	    sprintf(buffer, "%d", linkPtr->lastValue.i);
	    return buffer;
	case TCL_LINK_DOUBLE:
	    linkPtr->lastValue.d = *(double *)(linkPtr->addr);
	    Tcl_PrintDouble(linkPtr->interp, linkPtr->lastValue.d, buffer);
	    return buffer;
	case TCL_LINK_BOOLEAN:
	    linkPtr->lastValue.i = *(int *)(linkPtr->addr);
	    if (linkPtr->lastValue.i != 0) {
		return "1";
	    }
	    return "0";
	case TCL_LINK_STRING:
	    p = *(char **)(linkPtr->addr);
	    if (p == NULL) {
		return "NULL";
	    }
	    return p;
    }

    /*
     * This code only gets executed if the link type is unknown
     * (shouldn't ever happen).
     */

    return "??";
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLinkVar --
 *
 *	Link a C variable to a Tcl variable so that changes to either
 *	one causes the other to change.
 *
 * Results:
 *	The return value is TCL_OK if everything went well or TCL_ERROR
 *	if an error occurred (interp->result is also set after errors).
 *
 * Side effects:
 *	The value at *addr is linked to the Tcl variable "varName",
 *	using "type" to convert between string values for Tcl and
 *	binary values for *addr.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WLinkVar(interp, varName, addr, type)
    Tcl_Interp *interp;		/* Interpreter in which varName exists. */
    char *varName;		/* Name of a global variable in interp. */
    char *addr;			/* Address of a C variable to be linked
				 * to varName. */
    int type;			/* Type of C variable: TCL_LINK_INT, etc. 
				 * Also may have TCL_LINK_READ_ONLY
				 * OR'ed in. */
{
    Link *linkPtr;
    char buffer[TCL_DOUBLE_SPACE];
    int code;
    wchar_t *wvarName, *wval;
    char *cp;
    size_t wlen, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    linkPtr = (Link *) ckalloc(sizeof(Link));
    linkPtr->interp = interp;
    linkPtr->addr = addr;
    linkPtr->type = type & ~TCL_LINK_READ_ONLY;
    linkPtr->writable = (type & TCL_LINK_READ_ONLY) == 0;

    _Multibyte_to_WChar(interp, wlen, varName, wvarName, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    cp = StringValue(linkPtr, buffer);
    _Multibyte_to_WChar(interp, wlen, cp, wval, st)
    if (st == (size_t)-1)
    {
	ckfree(wvarName);
	return TCL_CONV_ERROR;
    }

    if (_Tcl_WSetVar(interp, wvarName, wval,
	    TCL_GLOBAL_ONLY|TCL_LEAVE_ERR_MSG) == NULL) 
    {
	ckfree((char *) linkPtr);
	ckfree(wvarName);
	ckfree(wval);
	return TCL_ERROR;
    }
    code = _Tcl_WTraceVar(interp, wvarName, TCL_GLOBAL_ONLY|TCL_TRACE_READS
	    |TCL_TRACE_WRITES|TCL_TRACE_UNSETS, _WLinkTraceProc,
	    (ClientData) linkPtr);
    if (code != TCL_OK) {
	ckfree(wvarName);
	ckfree(wval);
	ckfree((char *) linkPtr);
    }

    ckfree(wvarName);
    ckfree(wval);
    return code;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUnlinkVar --
 *
 *	Destroy the link between a Tcl variable and a C variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If "varName" was previously linked to a C variable, the link
 *	is broken to make the variable independent.  If there was no
 *	previous link for "varName" then nothing happens.
 *
 *----------------------------------------------------------------------
 */

void
_Tcl_WUnlinkVar(interp, varName)
    Tcl_Interp *interp;		/* Interpreter containing variable to unlink. */
    char *varName;		/* Global variable in interp to unlink. */
{
    Link *linkPtr;
    wchar_t *wvarName;
    size_t wlen, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    _Multibyte_to_WChar(interp, wlen, varName, wvarName, st)
    if (st == (size_t)-1)
	return;

    linkPtr = (Link *) _Tcl_WVarTraceInfo(interp, wvarName, TCL_GLOBAL_ONLY,
	    _WLinkTraceProc, (ClientData) NULL);
    if (linkPtr == NULL) {
	ckfree(wvarName);
	return;
    }
    _Tcl_WUntraceVar(interp, wvarName,
	    TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS,
	    _WLinkTraceProc, (ClientData) linkPtr);
    ckfree((char *) linkPtr);
    ckfree(wvarName);
}

/*
 *----------------------------------------------------------------------
 *
 * _WLinkTraceProc --
 *
 *	This procedure is invoked when a linked Tcl variable is read,
 *	written, or unset from Tcl.  It's responsible for keeping the
 *	C variable in sync with the Tcl variable.
 *
 * Results:
 *	If all goes well, NULL is returned; otherwise an error message
 *	is returned.
 *
 * Side effects:
 *	The C variable may be updated to make it consistent with the
 *	Tcl variable, or the Tcl variable may be overwritten to reject
 *	a modification.
 *
 *----------------------------------------------------------------------
 */

static char *
_WLinkTraceProc(clientData, interp, name1, name2, flags)
    ClientData clientData;	/* Contains information about the link. */
    Tcl_Interp *interp;		/* Interpreter containing Tcl variable. */
    char *name1;		/* First part of variable name. */
    char *name2;		/* Second part of variable name. */
    int flags;			/* Miscellaneous additional information. */
{
    Link *linkPtr = (Link *) clientData;
    int changed;
    char buffer[TCL_DOUBLE_SPACE];
    char *value, **pp;
    Tcl_DString savedResult;
    wchar_t *wname1, *wname2, *wcp, *wval;
    char *cp;
    size_t wlen, st;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    _Multibyte_to_WChar(interp, wlen, name1, wname1, st)
    if (st == (size_t)-1)
	return NULL;

    _Multibyte_to_WChar(interp, wlen, name2, wname2, st)
    if (st == (size_t)-1)
    {
	ckfree(wname1);
	return NULL;
    }

    /*
     * If the variable is being unset, then just re-create it (with a
     * trace) unless the whole interpreter is going away.
     */

    if (flags & TCL_TRACE_UNSETS) 
    {
	if (flags & TCL_INTERP_DESTROYED) 
	{
	    ckfree((char *) linkPtr);
	}
	if (flags & TCL_TRACE_DESTROYED) 
	{
	    cp = StringValue(linkPtr, buffer);
	    _Multibyte_to_WChar(interp, wlen, cp, wcp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(wname1);
		ckfree(wname2);
		return NULL;
	    }

	    _Tcl_WSetVar2(interp, wname1, wname2, wcp, TCL_GLOBAL_ONLY);
	    _Tcl_WTraceVar2(interp, wname1, wname2, TCL_GLOBAL_ONLY
		    |TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS,
		    _WLinkTraceProc, (ClientData) linkPtr);
	    ckfree(wcp);
	}
	ckfree(wname1);
	ckfree(wname2);
	return NULL;
    }

    /*
     * For read accesses, update the Tcl variable if the C variable
     * has changed since the last time we updated the Tcl variable.
     */

    if (flags & TCL_TRACE_READS) 
    {
	switch (linkPtr->type) 
	{
	    case TCL_LINK_INT:
	    case TCL_LINK_BOOLEAN:
		changed = *(int *)(linkPtr->addr) != linkPtr->lastValue.i;
		break;
	    case TCL_LINK_DOUBLE:
		changed = *(double *)(linkPtr->addr) != linkPtr->lastValue.d;
		break;
	    case TCL_LINK_STRING:
		changed = 1;
		break;
	    default:
		return((char *)dce_msg_get(tcl_s_link_bad_linked_var_type));
	}
	if (changed) 
        {
	    cp = StringValue(linkPtr, buffer);
	    _Multibyte_to_WChar(interp, wlen, cp, wcp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(wname1);
		ckfree(wname2);
		return NULL;
	    }

	    _Tcl_WSetVar2(interp, wname1, wname2, wcp, TCL_GLOBAL_ONLY);
	    ckfree(wcp);
	}
	ckfree(wname1);
	ckfree(wname2);
	return NULL;
    }

    /*
     * For writes, first make sure that the variable is writable.  Then
     * convert the Tcl value to C if possible.  If the variable isn't
     * writable or can't be converted, then restore the varaible's old
     * value and return an error.  Another tricky thing: we have to save
     * and restore the interpreter's result, since the variable access
     * could occur when the result has been partially set.
     */

    if (!linkPtr->writable) 
    {
	cp = StringValue(linkPtr, buffer);
	_Multibyte_to_WChar(interp, wlen, cp, wcp, st)
	if (st == (size_t)-1)
	{
		ckfree(wname1);
		ckfree(wname2);
		return NULL;
	}

	_Tcl_WSetVar2(interp, wname1, wname2, wcp, TCL_GLOBAL_ONLY);
	ckfree(wname1);
	ckfree(wname2);
	ckfree(wcp);
	return((char *)dce_msg_get(tcl_s_link_linked_var_read_only));
    }
    value = _Tcl_WGetVar2(interp, wname1, wname2, TCL_GLOBAL_ONLY);
    if (value == NULL) 
    {
        /*
         * This shouldn't ever happen.
         */
	ckfree(wname1);
	ckfree(wname2);
	return((char *)dce_msg_get(tcl_s_link_linked_var_unreadable));
    }
    Tcl_DStringInit(&savedResult);
    Tcl_DStringAppend(&savedResult, interp->result, -1);
    Tcl_ResetResult(interp);

    cp = StringValue(linkPtr, buffer);
    _Multibyte_to_WChar(interp, wlen, cp, wcp, st)
    if (st == (size_t)-1)
    {
	ckfree(wname1);
	ckfree(wname2);
	return NULL;
    }
    _Multibyte_to_WChar(interp, wlen, value, wval, st)
    if (st == (size_t)-1)
    {
	ckfree(wname1);
	ckfree(wname2);
	return NULL;
    }

    switch (linkPtr->type) 
    {
	case TCL_LINK_INT:
	    if (_Tcl_WGetInt(interp, wval, &linkPtr->lastValue.i) != TCL_OK) 
	    {
		Tcl_DStringResult(interp, &savedResult);
		_Tcl_WSetVar2(interp, wname1, wname2, wcp, TCL_GLOBAL_ONLY);
		ckfree(wname1);
		ckfree(wname1);
		ckfree(wcp);
		ckfree(wval);
		return((char *)dce_msg_get(tcl_s_link_var_must_be_integer));
	    }
	    *(int *)(linkPtr->addr) = linkPtr->lastValue.i;
	    break;
	case TCL_LINK_DOUBLE:
	    if (_Tcl_WGetDouble(interp, wval, &linkPtr->lastValue.d) != TCL_OK)
	    {
		Tcl_DStringResult(interp, &savedResult);
		_Tcl_WSetVar2(interp, wname1, wname2, wcp, TCL_GLOBAL_ONLY);
		ckfree(wname1);
		ckfree(wname1);
		ckfree(wcp);
		ckfree(wval);
		return((char *)dce_msg_get(tcl_s_link_var_must_be_real));
	    }
	    *(double *)(linkPtr->addr) = linkPtr->lastValue.d;
	    break;
	case TCL_LINK_BOOLEAN:
	    if (_Tcl_WGetBoolean(interp, wval, &linkPtr->lastValue.i) != TCL_OK) 
	    {
		Tcl_DStringResult(interp, &savedResult);
		_Tcl_WSetVar2(interp, wname1, wname2, wcp, TCL_GLOBAL_ONLY);
		ckfree(wname1);
		ckfree(wname1);
		ckfree(wcp);
		ckfree(wval);
		return((char *)dce_msg_get(tcl_s_link_var_must_be_bool));
	    }
	    *(int *)(linkPtr->addr) = linkPtr->lastValue.i;
	    break;
	case TCL_LINK_STRING:
	    pp = (char **)(linkPtr->addr);
	    if (*pp != NULL) {
		ckfree(*pp);
	    }
	    *pp = ckalloc((unsigned) (strlen(value) + 1));
	    strcpy(*pp, value);
	    break;
	default:
	    ckfree(wname1);
	    ckfree(wname1);
	    ckfree(wcp);
	    ckfree(wval);
	    return((char *)dce_msg_get(tcl_s_link_bad_linked_var_type));
    }
    Tcl_DStringResult(interp, &savedResult);
    ckfree(wname1);
    ckfree(wname2);
    ckfree(wcp);
    ckfree(wval);
    return NULL;
}
