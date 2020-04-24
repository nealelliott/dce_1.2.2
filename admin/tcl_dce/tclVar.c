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
 * $Log: tclVar.c,v $
 * Revision 1.1.8.3  1996/02/18  19:19:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:57  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:34:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  20:23 UTC  dnguyen
 * 	Merge cleaning up of I18N-commented code
 * 
 * 	HP revision /main/dnguyen_mothra/1  1995/04/11  16:49 UTC  dnguyen
 * 	Clean up I18N comments
 * 	[1995/12/08  18:06:31  root]
 * 
 * Revision 1.1.4.1  1994/10/21  21:39:35  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:34:12  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:45  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:16:16  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclVar.c --
 *
 *	This file contains routines that implement Tcl variables
 *	(both scalars and arrays).
 *
 *	The implementation of arrays is modelled after an initial
 *	implementation by Mark Diekhans and Karl Lehenbauer.
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
 * The strings below are used to indicate what went wrong when a
 * variable access is denied.
 */

/*
 * Creation flag values passed in to LookupVar:
 *
 * CRT_PART1 -		1 means create hash table entry for part 1 of
 *			name, if it doesn't already exist.  0 means
 *			return an error if it doesn't exist.
 * CRT_PART2 -		1 means create hash table entry for part 2 of
 *			name, if it doesn't already exist.  0 means
 *			return an error if it doesn't exist.
 */

#define CRT_PART1	1
#define CRT_PART2	2

/*
 * Forward references to procedures defined later in this file:
 */

static  char *		CallTraces _ANSI_ARGS_((Interp *iPtr, Var *arrayPtr,
			    Var *varPtr, char *part1, char *part2,
			    int flags));
static void		CleanupVar _ANSI_ARGS_((Var *varPtr, Var *arrayPtr));
static void		DeleteSearches _ANSI_ARGS_((Var *arrayVarPtr));
static void		DeleteArray _ANSI_ARGS_((Interp *iPtr, char *arrayName,
			    Var *varPtr, int flags));
static Var *		LookupVar _ANSI_ARGS_((Tcl_Interp *interp, char *part1,
			    char *part2, int flags, char *msg, int create,
			    Var **arrayPtrPtr));
static int		MakeUpvar _ANSI_ARGS_((Interp *iPtr,
			    CallFrame *framePtr, char *otherP1,
			    char *otherP2, char *myName));
static int		_WMakeUpvar _ANSI_ARGS_((Interp *iPtr,
			    CallFrame *framePtr, wchar_t *wotherP1,
			    wchar_t *wotherP2, wchar_t *wmyName));
static Var *		NewVar _ANSI_ARGS_((void));
static ArraySearch *	ParseSearchId _ANSI_ARGS_((Tcl_Interp *interp,
			    Var *varPtr, char *varName, char *string));
static void		VarErrMsg _ANSI_ARGS_((Tcl_Interp *interp,
			    char *part1, char *part2, char *operation,
			    char *reason));

/*
 *----------------------------------------------------------------------
 *
 * LookupVar --
 *
 *	This procedure is used by virtually all of the variable
 *	code to locate a variable given its name(s).
 *
 * Results:
 *	The return value is a pointer to the variable indicated by
 *	part1 and part2, or NULL if the variable couldn't be found.
 *	If the variable is found, *arrayPtrPtr is filled in with
 *	the address of the array that contains the variable (or NULL
 *	if the variable is a scalar).  Note:  it's possible that the
 *	variable returned may be VAR_UNDEFINED, even if CRT_PART1 and
 *	CRT_PART2 are specified (these only cause the hash table entry
 *	and/or array to be created).
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static Var *
LookupVar(interp, part1, part2, flags, msg, create, arrayPtrPtr)
    Tcl_Interp *interp;		/* Interpreter to use for lookup. */
    char *part1;		/* If part2 is NULL, this is name of scalar
				 * variable.  Otherwise it is name of array. */
    char *part2;		/* Name of an element within array, or NULL. */
    int flags;			/* Only the TCL_GLOBAL_ONLY and
				 * TCL_LEAVE_ERR_MSG bits matter. */
    char *msg;			/* Verb to use in error messages, e.g.
				 * "read" or "set".  Only needed if
				 * TCL_LEAVE_ERR_MSG is set in flags. */
    int create;			/* OR'ed combination of CRT_PART1 and
				 * CRT_PART2.  Tells which entries to create
				 * if they don't already exist. */
    Var **arrayPtrPtr;		/* If part2 is non-NULL, *arrayPtrPtr gets
				 * filled in with address of array variable. */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_HashTable *tablePtr;
    Tcl_HashEntry *hPtr;
    Var *varPtr;
    int new;
    unsigned char *i18n_msg_format;

    /*
     * Lookup part1.
     */

    *arrayPtrPtr = NULL;
    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == NULL)) {
	tablePtr = &iPtr->globalTable;
    } else {
	tablePtr = &iPtr->varFramePtr->varTable;
    }
    if (create & CRT_PART1) {
	hPtr = Tcl_CreateHashEntry(tablePtr, part1, &new);
	if (new) {
	    varPtr = NewVar();
	    Tcl_SetHashValue(hPtr, varPtr);
	    varPtr->hPtr = hPtr;
	}
    } else {
	hPtr = Tcl_FindHashEntry(tablePtr, part1);
	if (hPtr == NULL) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_var);
		VarErrMsg(interp, part1, part2, msg, (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	    return NULL;
	}
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) {
	varPtr = varPtr->value.upvarPtr;
    }

    if (part2 == NULL) {
	return varPtr;
    }

    /*
     * We're dealing with an array element, so make sure the variable
     * is an array and lookup the element (create it if desired).
     */

    if (varPtr->flags & VAR_UNDEFINED) {
	if (!(create & CRT_PART1)) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_var);
		VarErrMsg(interp, part1, part2, msg, (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	    return NULL;
	}
	varPtr->flags = VAR_ARRAY;
	varPtr->value.tablePtr = (Tcl_HashTable *)
		ckalloc(sizeof(Tcl_HashTable));
	Tcl_InitHashTable(varPtr->value.tablePtr, TCL_STRING_KEYS);
    } else if (!(varPtr->flags & VAR_ARRAY)) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_need_array);
	    VarErrMsg(interp, part1, part2, msg, (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
	return NULL;
    }
    *arrayPtrPtr = varPtr;
    if (create & CRT_PART2) {
	hPtr = Tcl_CreateHashEntry(varPtr->value.tablePtr, part2, &new);
	if (new) {
	    if (varPtr->searchPtr != NULL) {
		DeleteSearches(varPtr);
	    }
	    varPtr = NewVar();
	    Tcl_SetHashValue(hPtr, varPtr);
	    varPtr->hPtr = hPtr;
	}
    } else {
	hPtr = Tcl_FindHashEntry(varPtr->value.tablePtr, part2);
	if (hPtr == NULL) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_ele);
		VarErrMsg(interp, part1, part2, msg, (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	    return NULL;
	}
    }
    return (Var *) Tcl_GetHashValue(hPtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetVar --
 *
 *	Return the value of a Tcl variable.
 *
 * Results:
 *	The return value points to the current value of varName.  If
 *	the variable is not defined or can't be read because of a clash
 *	in array usage then a NULL pointer is returned and an error
 *	message is left in interp->result if the TCL_LEAVE_ERR_MSG
 *	flag is set.  Note:  the return value is only valid up until
 *	the next call to Tcl_SetVar or Tcl_SetVar2;  if you depend on
 *	the value lasting longer than that, then make yourself a private
 *	copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_GetVar(interp, varName, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    char *varName;		/* Name of a variable in interp. */
    int flags;			/* OR-ed combination of TCL_GLOBAL_ONLY
				 * or TCL_LEAVE_ERR_MSG bits. */
{
    register char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    char *result;
	    char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_GetVar2(interp, varName, open+1, flags);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_GetVar2(interp, varName, (char *) NULL, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetVar2 --
 *
 *	Return the value of a Tcl variable, given a two-part name
 *	consisting of array name and element within array.
 *
 * Results:
 *	The return value points to the current value of the variable
 *	given by part1 and part2.  If the specified variable doesn't
 *	exist, or if there is a clash in array usage, then NULL is
 *	returned and a message will be left in interp->result if the
 *	TCL_LEAVE_ERR_MSG flag is set.  Note:  the return value is
 *	only valid up until the next call to Tcl_SetVar or Tcl_SetVar2;
 *	if you depend on the value lasting longer than that, then make
 *	yourself a private copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_GetVar2(interp, part1, part2, flags)
    Tcl_Interp *interp;		/* Command interpreter in which variable is
				 * to be looked up. */
    char *part1;		/* Name of array (if part2 is NULL) or
				 * name of variable. */
    char *part2;		/* If non-null, gives name of element in
				 * array. */
    int flags;			/* OR-ed combination of TCL_GLOBAL_ONLY
				 * or TCL_LEAVE_ERR_MSG bits. */
{
    Var *varPtr, *arrayPtr;
    Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format;

    varPtr = LookupVar(interp, part1, part2, flags, "read", CRT_PART2,
	    &arrayPtr);
    if (varPtr == NULL) {
	return NULL;
    }

    /*
     * Invoke any traces that have been set for the variable.
     */

    if ((varPtr->tracePtr != NULL)
	    || ((arrayPtr != NULL) && (arrayPtr->tracePtr != NULL))) {
	char *msg;

	msg = CallTraces(iPtr, arrayPtr, varPtr, part1, part2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_READS);
	if (msg != NULL) {
	    VarErrMsg(interp, part1, part2, "read", msg);
	    goto cleanup;
	}
    }
    if (!(varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR|VAR_ARRAY))) {
	return varPtr->value.string;
    }
    if (flags & TCL_LEAVE_ERR_MSG) 
    {
	if ((varPtr->flags & VAR_UNDEFINED) && (arrayPtr != NULL)
		&& !(arrayPtr->flags & VAR_UNDEFINED)) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_ele);
	    VarErrMsg(interp, part1, part2, "read", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	} 
	else 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_var);
	    VarErrMsg(interp, part1, part2, "read", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
    }

    /*
     * If the variable doesn't exist anymore and no-one's using it,
     * then free up the relevant structures and hash table entries.
     */

    cleanup:
    if (varPtr->flags & VAR_UNDEFINED) {
	CleanupVar(varPtr, arrayPtr);
    }
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetVar --
 *
 *	Change the value of a variable.
 *
 * Results:
 *	Returns a pointer to the malloc'ed string holding the new
 *	value of the variable.  The caller should not modify this
 *	string.  If the write operation was disallowed then NULL
 *	is returned;  if the TCL_LEAVE_ERR_MSG flag is set, then
 *	an explanatory message will be left in interp->result.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	its value is changed to newValue.  If varName isn't currently
 *	defined, then a new global variable by that name is created.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_SetVar(interp, varName, newValue, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    char *varName;		/* Name of a variable in interp. */
    char *newValue;		/* New value for varName. */
    int flags;			/* Various flags that tell how to set value:
				 * any of TCL_GLOBAL_ONLY, TCL_APPEND_VALUE,
				 * TCL_LIST_ELEMENT, or TCL_LEAVE_ERR_MSG. */
{
    register char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    char *result;
	    char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_SetVar2(interp, varName, open+1, newValue, flags);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_SetVar2(interp, varName, (char *) NULL, newValue, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetVar2 --
 *
 *	Given a two-part variable name, which may refer either to a
 *	scalar variable or an element of an array, change the value
 *	of the variable.  If the named scalar or array or element
 *	doesn't exist then create one.
 *
 * Results:
 *	Returns a pointer to the malloc'ed string holding the new
 *	value of the variable.  The caller should not modify this
 *	string.  If the write operation was disallowed because an
 *	array was expected but not found (or vice versa), then NULL
 *	is returned;  if the TCL_LEAVE_ERR_MSG flag is set, then
 *	an explanatory message will be left in interp->result.
 *
 * Side effects:
 *	The value of the given variable is set.  If either the array
 *	or the entry didn't exist then a new one is created.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_SetVar2(interp, part1, part2, newValue, flags)
    Tcl_Interp *interp;		/* Command interpreter in which variable is
				 * to be looked up. */
    char *part1;		/* If part2 is NULL, this is name of scalar
				 * variable.  Otherwise it is name of array. */
    char *part2;		/* Name of an element within array, or NULL. */
    char *newValue;		/* New value for variable. */
    int flags;			/* Various flags that tell how to set value:
				 * any of TCL_GLOBAL_ONLY, TCL_APPEND_VALUE,
				 * TCL_LIST_ELEMENT, or TCL_LEAVE_ERR_MSG . */
{
    register Var *varPtr;
    register Interp *iPtr = (Interp *) interp;
    int length, listFlags;
    Var *arrayPtr;
    char *result;
    unsigned char *i18n_msg_format;

    varPtr = LookupVar(interp, part1, part2, flags, "set", CRT_PART1|CRT_PART2,
	    &arrayPtr);
    if (varPtr == NULL) {
	return NULL;
    }

    /*
     * If the variable's hPtr field is NULL, it means that this is an
     * upvar to an array element where the array was deleted, leaving
     * the element dangling at the end of the upvar.  Generate an error
     * (allowing the variable to be reset would screw up our storage
     * allocation and is meaningless anyway).
     */

    if (varPtr->hPtr == NULL) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_dangling_upvar);
	    VarErrMsg(interp, part1, part2, "set", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
	return NULL;
    }

    /*
     * Clear the variable's current value unless this is an
     * append operation.
     */

    if (varPtr->flags & VAR_ARRAY) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_is_array);
	    VarErrMsg(interp, part1, part2, "set", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
	return NULL;
    }
    if (!(flags & TCL_APPEND_VALUE) || (varPtr->flags & VAR_UNDEFINED)) {
	varPtr->valueLength = 0;
    }

    /*
     * Compute how many total bytes will be needed for the variable's
     * new value (leave space for a separating space between list
     * elements).  Allocate new space for the value if needed.
     */

    if (flags & TCL_LIST_ELEMENT) {
	length = Tcl_ScanElement(newValue, &listFlags) + 1;
    } else {
	length = strlen(newValue);
    }
    length += varPtr->valueLength;
    if (length >= varPtr->valueSpace) {
	char *newValue;
	int newSize;

	newSize = 2*varPtr->valueSpace;
	if (newSize <= length) {
	    newSize = length + 1;
	}
	if (newSize < 24) {
	    /*
	     * Don't waste time with teensy-tiny variables;  we'll
	     * just end up expanding them later.
	     */

	    newSize = 24;
	}
	newValue = ckalloc((unsigned) newSize);
	if (varPtr->valueSpace > 0) {
	    strcpy(newValue, varPtr->value.string);
	    ckfree(varPtr->value.string);
	}
	varPtr->valueSpace = newSize;
	varPtr->value.string = newValue;
    }

    /*
     * Append the new value to the variable, either as a list
     * element or as a string.
     */

    if (flags & TCL_LIST_ELEMENT) {
	char *dst = varPtr->value.string + varPtr->valueLength;

	if ((varPtr->valueLength > 0) && ((dst[-1] != '{')
		|| ((varPtr->valueLength > 1) && (dst[-2] == '\\')))) {
	    *dst = ' ';
	    dst++;
	    varPtr->valueLength++;
	}
	varPtr->valueLength += Tcl_ConvertElement(newValue, dst, listFlags);
    } else {
	strcpy(varPtr->value.string + varPtr->valueLength, newValue);
	varPtr->valueLength = length;
    }
    varPtr->flags &= ~VAR_UNDEFINED;

    /*
     * Invoke any write traces for the variable.
     */

    if ((varPtr->tracePtr != NULL)
	    || ((arrayPtr != NULL) && (arrayPtr->tracePtr != NULL))) {
	char *msg;

	msg = CallTraces(iPtr, arrayPtr, varPtr, part1, part2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_WRITES);
	if (msg != NULL) {
	    VarErrMsg(interp, part1, part2, "set", msg);
	    result = NULL;
	    goto cleanup;
	}
    }

    /*
     * If the variable was changed in some gross way by a trace (e.g.
     * it was unset and then recreated as an array) then just return
     * an empty string;  otherwise return the variable's current
     * value.
     */

    if (!(varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR|VAR_ARRAY))) {
	return varPtr->value.string;
    }
    result = "";

    /*
     * If the variable doesn't exist anymore and no-one's using it,
     * then free up the relevant structures and hash table entries.
     */

    cleanup:
    if (varPtr->flags & VAR_UNDEFINED) {
	CleanupVar(varPtr, arrayPtr);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnsetVar --
 *
 *	Delete a variable, so that it may not be accessed anymore.
 *
 * Results:
 *	Returns TCL_OK if the variable was successfully deleted, TCL_ERROR
 *	if the variable can't be unset.  In the event of an error,
 *	if the TCL_LEAVE_ERR_MSG flag is set then an error message
 *	is left in interp->result.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	it is deleted.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_UnsetVar(interp, varName, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    char *varName;		/* Name of a variable in interp.  May be
				 * either a scalar name or an array name
				 * or an element in an array. */
    int flags;			/* OR-ed combination of any of
				 * TCL_GLOBAL_ONLY or TCL_LEAVE_ERR_MSG. */
{
    register char *p;
    int result;

    /*
     * Figure out whether this is an array reference, then call
     * Tcl_UnsetVar2 to do all the real work.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_UnsetVar2(interp, varName, open+1, flags);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_UnsetVar2(interp, varName, (char *) NULL, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnsetVar2 --
 *
 *	Delete a variable, given a 2-part name.
 *
 * Results:
 *	Returns TCL_OK if the variable was successfully deleted, TCL_ERROR
 *	if the variable can't be unset.  In the event of an error,
 *	if the TCL_LEAVE_ERR_MSG flag is set then an error message
 *	is left in interp->result.
 *
 * Side effects:
 *	If part1 and part2 indicate a local or global variable in interp,
 *	it is deleted.  If part1 is an array name and part2 is NULL, then
 *	the whole array is deleted.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_UnsetVar2(interp, part1, part2, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    char *part1;		/* Name of variable or array. */
    char *part2;		/* Name of element within array or NULL. */
    int flags;			/* OR-ed combination of any of
				 * TCL_GLOBAL_ONLY or TCL_LEAVE_ERR_MSG. */
{
    Var *varPtr, dummyVar;
    Interp *iPtr = (Interp *) interp;
    Var *arrayPtr;
    ActiveVarTrace *activePtr;
    int result;
    unsigned char *i18n_msg_format;

    varPtr = LookupVar(interp, part1, part2, flags, "unset", 0,  &arrayPtr);
    if (varPtr == NULL) {
	return TCL_ERROR;
    }
    result = (varPtr->flags & VAR_UNDEFINED) ? TCL_ERROR : TCL_OK;

    if ((part2 != NULL) && (arrayPtr->searchPtr != NULL)) {
	DeleteSearches(arrayPtr);
    }

    /*
     * The code below is tricky, because of the possibility that
     * a trace procedure might try to access a variable being
     * deleted.  To handle this situation gracefully, do things
     * in three steps:
     * 1. Copy the contents of the variable to a dummy variable
     *    structure, and mark the original structure as undefined.
     * 2. Invoke traces and clean up the variable, using the copy.
     * 3. If at the end of this the original variable is still
     *    undefined and has no outstanding references, then delete
     *	  it (but it could have gotten recreated by a trace).
     */

    dummyVar = *varPtr;
    varPtr->valueSpace = 0;
    varPtr->flags = VAR_UNDEFINED;
    varPtr->tracePtr = NULL;

    /*
     * Call trace procedures for the variable being deleted and delete
     * its traces.  Be sure to abort any other traces for the variable
     * that are still pending.  Special tricks:
     * 1. Increment varPtr's refCount around this:  CallTraces will
     *    use dummyVar so it won't increment varPtr's refCount.
     * 2. Turn off the VAR_TRACE_ACTIVE flag in dummyVar: we want to
     *    call unset traces even if other traces are pending.
     */

    if ((dummyVar.tracePtr != NULL)
	    || ((arrayPtr != NULL) && (arrayPtr->tracePtr != NULL))) {
	varPtr->refCount++;
	dummyVar.flags &= ~VAR_TRACE_ACTIVE;
	(void) CallTraces(iPtr, arrayPtr, &dummyVar, part1, part2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_UNSETS);
	while (dummyVar.tracePtr != NULL) {
	    VarTrace *tracePtr = dummyVar.tracePtr;
	    dummyVar.tracePtr = tracePtr->nextPtr;
	    ckfree((char *) tracePtr);
	}
	for (activePtr = iPtr->activeTracePtr; activePtr != NULL;
		activePtr = activePtr->nextPtr) {
	    if (activePtr->varPtr == varPtr) {
		activePtr->nextTracePtr = NULL;
	    }
	}
	varPtr->refCount--;
    }

    /*
     * If the variable is an array, delete all of its elements.  This
     * must be done after calling the traces on the array, above (that's
     * the way traces are defined).
     */

    if (dummyVar.flags & VAR_ARRAY) {
	DeleteArray(iPtr, part1, &dummyVar,
	    (flags & TCL_GLOBAL_ONLY) | TCL_TRACE_UNSETS);
    }
    if (dummyVar.valueSpace > 0) {
	ckfree(dummyVar.value.string);
    }
    if (result == TCL_ERROR) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    if (part2 == NULL) {
	    	i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_var);
	    	VarErrMsg(interp, part1, part2, "unset", (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	    else {
	    	i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_ele);
	    	VarErrMsg(interp, part1, part2, "unset", (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	}
    }

    /*
     * Finally, if the variable is truly not in use then free up its
     * record and remove it from the hash table.
     */

    CleanupVar(varPtr, arrayPtr);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TraceVar --
 *
 *	Arrange for reads and/or writes to a variable to cause a
 *	procedure to be invoked, which can monitor the operations
 *	and/or change their actions.
 *
 * Results:
 *	A standard Tcl return value.
 *
 * Side effects:
 *	A trace is set up on the variable given by varName, such that
 *	future references to the variable will be intermediated by
 *	proc.  See the manual entry for complete details on the calling
 *	sequence for proc.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_TraceVar(interp, varName, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which variable is
				 * to be traced. */
    char *varName;		/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* OR-ed collection of bits, including any
				 * of TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure to call when specified ops are
				 * invoked upon varName. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    register char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    int result;
	    char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_TraceVar2(interp, varName, open+1, flags,
		    proc, clientData);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_TraceVar2(interp, varName, (char *) NULL, flags,
	    proc, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TraceVar2 --
 *
 *	Arrange for reads and/or writes to a variable to cause a
 *	procedure to be invoked, which can monitor the operations
 *	and/or change their actions.
 *
 * Results:
 *	A standard Tcl return value.
 *
 * Side effects:
 *	A trace is set up on the variable given by part1 and part2, such
 *	that future references to the variable will be intermediated by
 *	proc.  See the manual entry for complete details on the calling
 *	sequence for proc.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_TraceVar2(interp, part1, part2, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which variable is
				 * to be traced. */
    char *part1;		/* Name of scalar variable or array. */
    char *part2;		/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* OR-ed collection of bits, including any
				 * of TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure to call when specified ops are
				 * invoked upon varName. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    Var *varPtr, *arrayPtr;
    register VarTrace *tracePtr;

    varPtr = LookupVar(interp, part1, part2, (flags | TCL_LEAVE_ERR_MSG),
	    "trace", CRT_PART1|CRT_PART2, &arrayPtr);
    if (varPtr == NULL) {
	return TCL_ERROR;
    }

    /*
     * Set up trace information.
     */

    tracePtr = (VarTrace *) ckalloc(sizeof(VarTrace));
    tracePtr->traceProc = proc;
    tracePtr->clientData = clientData;
    tracePtr->flags = flags &
	    (TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS);
    tracePtr->nextPtr = varPtr->tracePtr;
    varPtr->tracePtr = tracePtr;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UntraceVar --
 *
 *	Remove a previously-created trace for a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If there exists a trace for the variable given by varName
 *	with the given flags, proc, and clientData, then that trace
 *	is removed.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_UntraceVar(interp, varName, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter containing traced variable. */
    char *varName;		/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* OR-ed collection of bits describing
				 * current trace, including any of
				 * TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    register char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    Tcl_UntraceVar2(interp, varName, open+1, flags, proc, clientData);
	    *open = '(';
	    *p = ')';
	    return;
	}
    }

    scalar:
    Tcl_UntraceVar2(interp, varName, (char *) NULL, flags, proc, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UntraceVar2 --
 *
 *	Remove a previously-created trace for a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If there exists a trace for the variable given by part1
 *	and part2 with the given flags, proc, and clientData, then
 *	that trace is removed.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_UntraceVar2(interp, part1, part2, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter containing traced variable. */
    char *part1;		/* Name of variable or array. */
    char *part2;		/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* OR-ed collection of bits describing
				 * current trace, including any of
				 * TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    register VarTrace *tracePtr;
    VarTrace *prevPtr;
    Var *varPtr, *arrayPtr;
    Interp *iPtr = (Interp *) interp;
    ActiveVarTrace *activePtr;

    varPtr = LookupVar(interp, part1, part2, flags & TCL_GLOBAL_ONLY,
	    (char *) NULL, 0, &arrayPtr);
    if (varPtr == NULL) {
	return;
    }

    flags &= (TCL_TRACE_READS | TCL_TRACE_WRITES | TCL_TRACE_UNSETS);
    for (tracePtr = varPtr->tracePtr, prevPtr = NULL; ;
	    prevPtr = tracePtr, tracePtr = tracePtr->nextPtr) {
	if (tracePtr == NULL) {
	    return;
	}
	if ((tracePtr->traceProc == proc) && (tracePtr->flags == flags)
		&& (tracePtr->clientData == clientData)) {
	    break;
	}
    }

    /*
     * The code below makes it possible to delete traces while traces
     * are active:  it makes sure that the deleted trace won't be
     * processed by CallTraces.
     */

    for (activePtr = iPtr->activeTracePtr; activePtr != NULL;
	    activePtr = activePtr->nextPtr) {
	if (activePtr->nextTracePtr == tracePtr) {
	    activePtr->nextTracePtr = tracePtr->nextPtr;
	}
    }
    if (prevPtr == NULL) {
	varPtr->tracePtr = tracePtr->nextPtr;
    } else {
	prevPtr->nextPtr = tracePtr->nextPtr;
    }
    ckfree((char *) tracePtr);

    /*
     * If this is the last trace on the variable, and the variable is
     * unset and unused, then free up the variable.
     */

    if (varPtr->flags & VAR_UNDEFINED) {
	CleanupVar(varPtr, (Var *) NULL);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_VarTraceInfo --
 *
 *	Return the clientData value associated with a trace on a
 *	variable.  This procedure can also be used to step through
 *	all of the traces on a particular variable that have the
 *	same trace procedure.
 *
 * Results:
 *	The return value is the clientData value associated with
 *	a trace on the given variable.  Information will only be
 *	returned for a trace with proc as trace procedure.  If
 *	the clientData argument is NULL then the first such trace is
 *	returned;  otherwise, the next relevant one after the one
 *	given by clientData will be returned.  If the variable
 *	doesn't exist, or if there are no (more) traces for it,
 *	then NULL is returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

ClientData
Tcl_VarTraceInfo(interp, varName, flags, proc, prevClientData)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    char *varName;		/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* 0 or TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData prevClientData;	/* If non-NULL, gives last value returned
				 * by this procedure, so this call will
				 * return the next trace after that one.
				 * If NULL, this call will return the
				 * first trace. */
{
    register char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    ClientData result;
	    char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_VarTraceInfo2(interp, varName, open+1, flags, proc,
		prevClientData);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_VarTraceInfo2(interp, varName, (char *) NULL, flags, proc,
	    prevClientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_VarTraceInfo2 --
 *
 *	Same as Tcl_VarTraceInfo, except takes name in two pieces
 *	instead of one.
 *
 * Results:
 *	Same as Tcl_VarTraceInfo.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

ClientData
Tcl_VarTraceInfo2(interp, part1, part2, flags, proc, prevClientData)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    char *part1;		/* Name of variable or array. */
    char *part2;		/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* 0 or TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData prevClientData;	/* If non-NULL, gives last value returned
				 * by this procedure, so this call will
				 * return the next trace after that one.
				 * If NULL, this call will return the
				 * first trace. */
{
    register VarTrace *tracePtr;
    Var *varPtr, *arrayPtr;

    varPtr = LookupVar(interp, part1, part2, flags & TCL_GLOBAL_ONLY,
	    (char *) NULL, 0, &arrayPtr);
    if (varPtr == NULL) {
	return NULL;
    }

    /*
     * Find the relevant trace, if any, and return its clientData.
     */

    tracePtr = varPtr->tracePtr;
    if (prevClientData != NULL) {
	for ( ; tracePtr != NULL; tracePtr = tracePtr->nextPtr) {
	    if ((tracePtr->clientData == prevClientData)
		    && (tracePtr->traceProc == proc)) {
		tracePtr = tracePtr->nextPtr;
		break;
	    }
	}
    }
    for ( ; tracePtr != NULL; tracePtr = tracePtr->nextPtr) {
	if (tracePtr->traceProc == proc) {
	    return tracePtr->clientData;
	}
    }
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetCmd --
 *
 *	This procedure is invoked to process the "set" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_SetCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc == 2) {
	char *value;

	value = Tcl_GetVar(interp, argv[1], TCL_LEAVE_ERR_MSG);
	if (value == NULL) {
	    return TCL_ERROR;
	}
	interp->result = value;
	return TCL_OK;
    } else if (argc == 3) {
	char *result;

	result = Tcl_SetVar(interp, argv[1], argv[2], TCL_LEAVE_ERR_MSG);
	if (result == NULL) {
	    return TCL_ERROR;
	}
	interp->result = result;
	return TCL_OK;
    } else {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_newval);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnsetCmd --
 *
 *	This procedure is invoked to process the "unset" Tcl command.
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
Tcl_UnsetCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int i;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 2) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    for (i = 1; i < argc; i++) {
	if (Tcl_UnsetVar(interp, argv[i], TCL_LEAVE_ERR_MSG) != TCL_OK) {
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AppendCmd --
 *
 *	This procedure is invoked to process the "append" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_AppendCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int i;
    char *result = NULL;		/* (Initialization only needed to keep
					 * the compiler from complaining) */
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_val_val);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    for (i = 2; i < argc; i++) {
	result = Tcl_SetVar(interp, argv[1], argv[i],
		TCL_APPEND_VALUE|TCL_LEAVE_ERR_MSG);
	if (result == NULL) {
	    return TCL_ERROR;
	}
    }
    interp->result = result;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LappendCmd --
 *
 *	This procedure is invoked to process the "lappend" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LappendCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int i;
    char *result = NULL;		/* (Initialization only needed to keep
					 * the compiler from complaining) */
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_val_val);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    for (i = 2; i < argc; i++) {
	result = Tcl_SetVar(interp, argv[1], argv[i],
		TCL_APPEND_VALUE|TCL_LIST_ELEMENT|TCL_LEAVE_ERR_MSG);
	if (result == NULL) {
	    return TCL_ERROR;
	}
    }
    interp->result = result;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ArrayCmd --
 *
 *	This procedure is invoked to process the "array" Tcl command.
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
Tcl_ArrayCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    int length;
    char c;
    Var *varPtr;
    Tcl_HashEntry *hPtr;
    Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_opt_arrname_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Locate the array variable (and it better be an array).
     */

    if (iPtr->varFramePtr == NULL) {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, argv[2]);
    } else {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, argv[2]);
    }
    if (hPtr == NULL) {
	notArray:
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_isnt_array);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[2])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[2]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) {
	varPtr = varPtr->value.upvarPtr;
    }
    if (!(varPtr->flags & VAR_ARRAY)) {
	goto notArray;
    }

    /*
     * Dispatch based on the option.
     */

    c = argv[1][0];
    length = strlen(argv[1]);
    if ((c == 'a') && (strncmp(argv[1], "anymore", length) == 0)) {
	ArraySearch *searchPtr;

	if (argc != 4) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_anymore_arrname_search);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	searchPtr = ParseSearchId(interp, varPtr, argv[2], argv[3]);
	if (searchPtr == NULL) {
	    return TCL_ERROR;
	}
	while (1) {
	    Var *varPtr2;

	    if (searchPtr->nextEntry != NULL) {
		varPtr2 = (Var *) Tcl_GetHashValue(searchPtr->nextEntry);
		if (!(varPtr2->flags & VAR_UNDEFINED)) {
		    break;
		}
	    }
	    searchPtr->nextEntry = Tcl_NextHashEntry(&searchPtr->search);
	    if (searchPtr->nextEntry == NULL) {
		interp->result = "0";
		return TCL_OK;
	    }
	}
	interp->result = "1";
	return TCL_OK;
    } else if ((c == 'd') && (strncmp(argv[1], "donesearch", length) == 0)) {
	ArraySearch *searchPtr, *prevPtr;

	if (argc != 4) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_donesearch_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	searchPtr = ParseSearchId(interp, varPtr, argv[2], argv[3]);
	if (searchPtr == NULL) {
	    return TCL_ERROR;
	}
	if (varPtr->searchPtr == searchPtr) {
	    varPtr->searchPtr = searchPtr->nextPtr;
	} else {
	    for (prevPtr = varPtr->searchPtr; ; prevPtr = prevPtr->nextPtr) {
		if (prevPtr->nextPtr == searchPtr) {
		    prevPtr->nextPtr = searchPtr->nextPtr;
		    break;
		}
	    }
	}
	ckfree((char *) searchPtr);
    } else if ((c == 'n') && (strncmp(argv[1], "names", length) == 0)
	    && (length >= 2)) {
	Tcl_HashSearch search;
	Var *varPtr2;

	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_names_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr2->flags & VAR_UNDEFINED) {
		continue;
	    }
	    Tcl_AppendElement(interp,
		    Tcl_GetHashKey(varPtr->value.tablePtr, hPtr));
	}
    } else if ((c == 'n') && (strncmp(argv[1], "nextelement", length) == 0)
	    && (length >= 2)) {
	ArraySearch *searchPtr;
	Tcl_HashEntry *hPtr;

	if (argc != 4) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_nextele_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	searchPtr = ParseSearchId(interp, varPtr, argv[2], argv[3]);
	if (searchPtr == NULL) {
	    return TCL_ERROR;
	}
	while (1) {
	    Var *varPtr2;

	    hPtr = searchPtr->nextEntry;
	    if (hPtr == NULL) {
		hPtr = Tcl_NextHashEntry(&searchPtr->search);
		if (hPtr == NULL) {
		    return TCL_OK;
		}
	    } else {
		searchPtr->nextEntry = NULL;
	    }
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (!(varPtr2->flags & VAR_UNDEFINED)) {
		break;
	    }
	}
	interp->result = Tcl_GetHashKey(varPtr->value.tablePtr, hPtr);
    } else if ((c == 's') && (strncmp(argv[1], "size", length) == 0)
	    && (length >= 2)) {
	Tcl_HashSearch search;
	Var *varPtr2;
	int size;

	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_size_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	size = 0;
	for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr2->flags & VAR_UNDEFINED) {
		continue;
	    }
	    size++;
	}
	sprintf(interp->result, "%d", size);
    } else if ((c == 's') && (strncmp(argv[1], "startsearch", length) == 0)
	    && (length >= 2)) {
	ArraySearch *searchPtr;

	if (argc != 3) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_startsearch_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	searchPtr = (ArraySearch *) ckalloc(sizeof(ArraySearch));
	if (varPtr->searchPtr == NULL) {
	    searchPtr->id = 1;
	    Tcl_AppendResult(interp, "s-1-", argv[2], (char *) NULL);
	} else {
	    char string[20];

	    searchPtr->id = varPtr->searchPtr->id + 1;
	    sprintf(string, "%d", searchPtr->id);
	    Tcl_AppendResult(interp, "s-", string, "-", argv[2],
		    (char *) NULL);
	}
	searchPtr->varPtr = varPtr;
	searchPtr->nextEntry = Tcl_FirstHashEntry(varPtr->value.tablePtr,
		&searchPtr->search);
	searchPtr->nextPtr = varPtr->searchPtr;
	varPtr->searchPtr = searchPtr;
    } else {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_bad_opt_array);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[1])
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[1]);
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
 * MakeUpvar --
 *
 *	This procedure does all of the work of the "global" and "upvar"
 *	commands.
 *
 * Results:
 *	A standard Tcl completion code.  If an error occurs then an
 *	error message is left in iPtr->result.
 *
 * Side effects:
 *	The variable given by myName is linked to the variable in
 *	framePtr given by otherP1 and otherP2, so that references to
 *	myName are redirected to the other variable like a symbolic
*	link.
 *
 *----------------------------------------------------------------------
 */

static int
MakeUpvar(iPtr, framePtr, otherP1, otherP2, myName)
    Interp *iPtr;		/* Interpreter containing variables.  Used
				 * for error messages, too. */
    CallFrame *framePtr;	/* Call frame containing "other" variable.
				 * NULL means use global context. */
    char *otherP1, *otherP2;	/* Two-part name of variable in framePtr. */
    char *myName;		/* Name of variable in local table, which
				 * will refer to otherP1/P2.  Must be a
				 * scalar. */
{
    Tcl_HashEntry *hPtr;
    Var *otherPtr, *varPtr, *arrayPtr;
    CallFrame *savedFramePtr;
    int new;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    /*
     * In order to use LookupVar to find "other", temporarily replace
     * the current frame pointer in the interpreter.
     */

    savedFramePtr = iPtr->varFramePtr;
    iPtr->varFramePtr = framePtr;
    otherPtr = LookupVar((Tcl_Interp *) iPtr, otherP1, otherP2,
	    TCL_LEAVE_ERR_MSG, "access", CRT_PART1|CRT_PART2, &arrayPtr);
    iPtr->varFramePtr = savedFramePtr;
    if (otherPtr == NULL) {
	return TCL_ERROR;
    }
    if (iPtr->varFramePtr != NULL) {
	hPtr = Tcl_CreateHashEntry(&iPtr->varFramePtr->varTable, myName, &new);
    } else {
	hPtr = Tcl_CreateHashEntry(&iPtr->globalTable, myName, &new);
    }
    if (new) {
	varPtr = NewVar();
	Tcl_SetHashValue(hPtr, varPtr);
	varPtr->hPtr = hPtr;
    } else {
	/*
	 * The variable already exists.  If it's not an upvar then it's
	 * an error.  If it is an upvar, then just disconnect it from the
	 * thing it currently refers to.
	 */

	varPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (varPtr->flags & VAR_UPVAR) {
	    Var *upvarPtr;

	    upvarPtr = varPtr->value.upvarPtr;
	    if (upvarPtr == otherPtr) {
		return TCL_OK;
	    }
	    upvarPtr->refCount--;
	    if (upvarPtr->flags & VAR_UNDEFINED) {
		CleanupVar(upvarPtr, (Var *) NULL);
	    }
	} else if (!(varPtr->flags & VAR_UNDEFINED)) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_var_already_exists);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(myName)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, myName);
	    Tcl_AppendResult((Tcl_Interp *) iPtr, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
    }
    varPtr->flags = (varPtr->flags & ~VAR_UNDEFINED) | VAR_UPVAR;
    varPtr->value.upvarPtr = otherPtr;
    otherPtr->refCount++;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GlobalCmd --
 *
 *	This procedure is invoked to process the "global" Tcl command.
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
Tcl_GlobalCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 2) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_varname);
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

    for (argc--, argv++; argc > 0; argc--, argv++) {
	if (MakeUpvar(iPtr, (CallFrame *) NULL, *argv, (char *) NULL, *argv)
		!= TCL_OK) {
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UpvarCmd --
 *
 *	This procedure is invoked to process the "upvar" Tcl command.
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
Tcl_UpvarCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int result;
    CallFrame *framePtr;
    register char *p;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) {
	upvarSyntax:
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_level_othervar);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(argv[0])
			+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, argv[0]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Find the hash table containing the variable being referenced.
     */

    result = TclGetFrame(interp, argv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    argc -= result+1;
    if ((argc & 1) != 0) {
	goto upvarSyntax;
    }
    argv += result+1;

    /*
     * Iterate over all the pairs of (other variable, local variable)
     * names.  For each pair, divide the other variable name into two
     * parts, then call MakeUpvar to do all the work of creating linking
     * it to the local variable.
     */

    for ( ; argc > 0; argc -= 2, argv += 2) {
	for (p = argv[0]; *p != 0; p++) {
	    if (*p == '(') {
		char *open = p;

		do {
		    p++;
		} while (*p != '\0');
		p--;
		if (*p != ')') {
		    goto scalar;
		}
		*open = '\0';
		*p = '\0';
		result = MakeUpvar(iPtr, framePtr, argv[0], open+1, argv[1]);
		*open = '(';
		*p = ')';
		goto checkResult;
	    }
	}
	scalar:
	result = MakeUpvar(iPtr, framePtr, argv[0], (char *) NULL, argv[1]);

	checkResult:
	if (result != TCL_OK) {
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * CallTraces --
 *
 *	This procedure is invoked to find and invoke relevant
 *	trace procedures associated with a particular operation on
 *	a variable.  This procedure invokes traces both on the
 *	variable and on its containing array (where relevant).
 *
 * Results:
 *	The return value is NULL if no trace procedures were invoked, or
 *	if all the invoked trace procedures returned successfully.
 *	The return value is non-zero if a trace procedure returned an
 *	error (in this case no more trace procedures were invoked after
 *	the error was returned).  In this case the return value is a
 *	pointer to a static string describing the error.
 *
 * Side effects:
 *	Almost anything can happen, depending on trace;  this procedure
 *	itself doesn't have any side effects.
 *
 *----------------------------------------------------------------------
 */

static char *
CallTraces(iPtr, arrayPtr, varPtr, part1, part2, flags)
    Interp *iPtr;			/* Interpreter containing variable. */
    register Var *arrayPtr;		/* Pointer to array variable that
					 * contains the variable, or NULL if
					 * the variable isn't an element of an
					 * array. */
    Var *varPtr;			/* Variable whose traces are to be
					 * invoked. */
    char *part1, *part2;		/* Variable's two-part name. */
    int flags;				/* Flags to pass to trace procedures:
					 * indicates what's happening to
					 * variable, plus other stuff like
					 * TCL_GLOBAL_ONLY and
					 * TCL_INTERP_DESTROYED. */
{
    register VarTrace *tracePtr;
    ActiveVarTrace active;
    char *result;

    /*
     * If there are already similar trace procedures active for the
     * variable, don't call them again.
     */

    if (varPtr->flags & VAR_TRACE_ACTIVE) {
	return NULL;
    }
    varPtr->flags |= VAR_TRACE_ACTIVE;
    varPtr->refCount++;

    /*
     * Invoke traces on the array containing the variable, if relevant.
     */

    result = NULL;
    active.nextPtr = iPtr->activeTracePtr;
    iPtr->activeTracePtr = &active;
    if (arrayPtr != NULL) {
	arrayPtr->refCount++;
	active.varPtr = arrayPtr;
	for (tracePtr = arrayPtr->tracePtr;  tracePtr != NULL;
		tracePtr = active.nextTracePtr) {
	    active.nextTracePtr = tracePtr->nextPtr;
	    if (!(tracePtr->flags & flags)) {
		continue;
	    }
	    result = (*tracePtr->traceProc)(tracePtr->clientData,
		    (Tcl_Interp *) iPtr, part1, part2, flags);
	    if (result != NULL) {
		if (flags & TCL_TRACE_UNSETS) {
		    result = NULL;
		} else {
		    goto done;
		}
	    }
	}
    }

    /*
     * Invoke traces on the variable itself.
     */

    if (flags & TCL_TRACE_UNSETS) {
	flags |= TCL_TRACE_DESTROYED;
    }
    active.varPtr = varPtr;
    for (tracePtr = varPtr->tracePtr; tracePtr != NULL;
	    tracePtr = active.nextTracePtr) {
	active.nextTracePtr = tracePtr->nextPtr;
	if (!(tracePtr->flags & flags)) {
	    continue;
	}
	result = (*tracePtr->traceProc)(tracePtr->clientData,
		(Tcl_Interp *) iPtr, part1, part2, flags);
	if (result != NULL) {
	    if (flags & TCL_TRACE_UNSETS) {
		result = NULL;
	    } else {
		goto done;
	    }
	}
    }

    /*
     * Restore the variable's flags, remove the record of our active
     * traces, and then return.
     */

    done:
    if (arrayPtr != NULL) {
	arrayPtr->refCount--;
    }
    varPtr->flags &= ~VAR_TRACE_ACTIVE;
    varPtr->refCount--;
    iPtr->activeTracePtr = active.nextPtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * NewVar --
 *
 *	Create a new variable with a given amount of storage
 *	space.
 *
 * Results:
 *	The return value is a pointer to the new variable structure.
 *	The variable will not be part of any hash table yet.  Its
 *	initial value is empty.
 *
 * Side effects:
 *	Storage gets allocated.
 *
 *----------------------------------------------------------------------
 */

static Var *
NewVar()
{
    register Var *varPtr;

    varPtr = (Var *) ckalloc(sizeof(Var));
    varPtr->valueLength = 0;
    varPtr->valueSpace = 0;
    varPtr->value.string = NULL;
    varPtr->hPtr = NULL;
    varPtr->refCount = 0;
    varPtr->tracePtr = NULL;
    varPtr->searchPtr = NULL;
    varPtr->flags = VAR_UNDEFINED;
    return varPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * ParseSearchId --
 *
 *	This procedure translates from a string to a pointer to an
 *	active array search (if there is one that matches the string).
 *
 * Results:
 *	The return value is a pointer to the array search indicated
 *	by string, or NULL if there isn't one.  If NULL is returned,
 *	interp->result contains an error message.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static ArraySearch *
ParseSearchId(interp, varPtr, varName, string)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    Var *varPtr;		/* Array variable search is for. */
    char *varName;		/* Name of array variable that search is
				 * supposed to be for. */
    char *string;		/* String containing id of search.  Must have
				 * form "search-num-var" where "num" is a
				 * decimal number and "var" is a variable
				 * name. */
{
    char *end;
    int id;
    ArraySearch *searchPtr;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    /*
     * Parse the id into the three parts separated by dashes.
     */

    if ((string[0] != 's') || (string[1] != '-')) {
	syntax:
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_illegal_search_id);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
			+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return NULL;
    }
    id = strtoul(string+2, &end, 10);
    if ((end == (string+2)) || (*end != '-')) {
	goto syntax;
    }
    if (strcmp(end+1, varName) != 0) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_search_id_var);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
			+ strlen(varName)
			+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string, varName);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return NULL;
    }

    /*
     * Search through the list of active searches on the interpreter
     * to see if the desired one exists.
     */

    for (searchPtr = varPtr->searchPtr; searchPtr != NULL;
	    searchPtr = searchPtr->nextPtr) {
	if (searchPtr->id == id) {
	    return searchPtr;
	}
    }
    i18n_msg_format = dce_msg_get(tcl_s_tclvar_illegal_search);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
			+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
    free(i18n_msg_format);
    ckfree(i18n_msg_buf);

    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * DeleteSearches --
 *
 *	This procedure is called to free up all of the searches
 *	associated with an array variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory is released to the storage allocator.
 *
 *----------------------------------------------------------------------
 */

static void
DeleteSearches(arrayVarPtr)
    register Var *arrayVarPtr;		/* Variable whose searches are
					 * to be deleted. */
{
    ArraySearch *searchPtr;

    while (arrayVarPtr->searchPtr != NULL) {
	searchPtr = arrayVarPtr->searchPtr;
	arrayVarPtr->searchPtr = searchPtr->nextPtr;
	ckfree((char *) searchPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclDeleteVars --
 *
 *	This procedure is called to recycle all the storage space
 *	associated with a table of variables.  For this procedure
 *	to work correctly, it must not be possible for any of the
 *	variable in the table to be accessed from Tcl commands
 *	(e.g. from trace procedures).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Variables are deleted and trace procedures are invoked, if
 *	any are declared.
 *
 *----------------------------------------------------------------------
 */

void
TclDeleteVars(iPtr, tablePtr)
    Interp *iPtr;		/* Interpreter to which variables belong. */
    Tcl_HashTable *tablePtr;	/* Hash table containing variables to
				 * delete. */
{
    Tcl_HashSearch search;
    Tcl_HashEntry *hPtr;
    register Var *varPtr;
    Var *upvarPtr;
    int flags;
    ActiveVarTrace *activePtr;

    flags = TCL_TRACE_UNSETS;
    if (tablePtr == &iPtr->globalTable) {
	flags |= TCL_INTERP_DESTROYED | TCL_GLOBAL_ONLY;
    }
    for (hPtr = Tcl_FirstHashEntry(tablePtr, &search); hPtr != NULL;
	    hPtr = Tcl_NextHashEntry(&search)) {
	varPtr = (Var *) Tcl_GetHashValue(hPtr);

	/*
	 * For global/upvar variables referenced in procedures, decrement
	 * the reference count on the variable referred to, and free up
	 * the referenced variable if it's no longer needed.
	 */

	if (varPtr->flags & VAR_UPVAR) {
	    upvarPtr = varPtr->value.upvarPtr;
	    upvarPtr->refCount--;
	    if (upvarPtr->flags & VAR_UNDEFINED) {
		CleanupVar(upvarPtr, (Var *) NULL);
	    }
	}

	/*
	 * Invoke traces on the variable that is being deleted, then
	 * free up the variable's space (no need to free the hash entry
	 * here, unless we're dealing with a global variable:  the
	 * hash entries will be deleted automatically when the whole
	 * table is deleted).
	 */

	if (varPtr->tracePtr != NULL) {
	    (void) CallTraces(iPtr, (Var *) NULL, varPtr,
		    Tcl_GetHashKey(tablePtr, hPtr), (char *) NULL, flags);
	    while (varPtr->tracePtr != NULL) {
		VarTrace *tracePtr = varPtr->tracePtr;
		varPtr->tracePtr = tracePtr->nextPtr;
		ckfree((char *) tracePtr);
	    }
	    for (activePtr = iPtr->activeTracePtr; activePtr != NULL;
		    activePtr = activePtr->nextPtr) {
		if (activePtr->varPtr == varPtr) {
		    activePtr->nextTracePtr = NULL;
		}
	    }
	}
	if (varPtr->flags & VAR_ARRAY) {
	    DeleteArray(iPtr, Tcl_GetHashKey(tablePtr, hPtr), varPtr, flags);
	}
	if (varPtr->valueSpace > 0) {
	    /*
	     * SPECIAL TRICK:  it's possible that the interpreter's result
	     * currently points to this variable (for example, a "set" or
	     * "lappend" command was the last command in a procedure that's
	     * being returned from).  If this is the case, then just pass
	     * ownership of the value string to the Tcl interpreter.
	     */

	    if (iPtr->result == varPtr->value.string) {
		iPtr->freeProc = (Tcl_FreeProc *) free;
	    } else {
		ckfree(varPtr->value.string);
	    }
	    varPtr->valueSpace = 0;
	}
	varPtr->hPtr = NULL;
	varPtr->tracePtr = NULL;
	varPtr->flags = VAR_UNDEFINED;
	if (varPtr->refCount == 0) {
	    ckfree((char *) varPtr);
	}
    }
    Tcl_DeleteHashTable(tablePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * DeleteArray --
 *
 *	This procedure is called to free up everything in an array
 *	variable.  It's the caller's responsibility to make sure
 *	that the array is no longer accessible before this procedure
 *	is called.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	All storage associated with varPtr's array elements is deleted
 *	(including the hash table).  Delete trace procedures for
 *	array elements are invoked.
 *
 *----------------------------------------------------------------------
 */

static void
DeleteArray(iPtr, arrayName, varPtr, flags)
    Interp *iPtr;			/* Interpreter containing array. */
    char *arrayName;			/* Name of array (used for trace
					 * callbacks). */
    Var *varPtr;			/* Pointer to variable structure. */
    int flags;				/* Flags to pass to CallTraces:
					 * TCL_TRACE_UNSETS and sometimes
					 * TCL_INTERP_DESTROYED and/or
					 * TCL_GLOBAL_ONLY. */
{
    Tcl_HashSearch search;
    register Tcl_HashEntry *hPtr;
    register Var *elPtr;
    ActiveVarTrace *activePtr;

    DeleteSearches(varPtr);
    for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
	    hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) {
	elPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (elPtr->valueSpace != 0) {
	    /*
	     * SPECIAL TRICK:  it's possible that the interpreter's result
	     * currently points to this element (for example, a "set" or
	     * "lappend" command was the last command in a procedure that's
	     * being returned from).  If this is the case, then just pass
	     * ownership of the value string to the Tcl interpreter.
	     */

	    if (iPtr->result == elPtr->value.string) {
		iPtr->freeProc = (Tcl_FreeProc *) free;
	    } else {
		ckfree(elPtr->value.string);
	    }
	    elPtr->valueSpace = 0;
	}
	elPtr->hPtr = NULL;
	if (elPtr->tracePtr != NULL) {
	    elPtr->flags &= ~VAR_TRACE_ACTIVE;
	    (void) CallTraces(iPtr, (Var *) NULL, elPtr, arrayName,
		    Tcl_GetHashKey(varPtr->value.tablePtr, hPtr), flags);
	    while (elPtr->tracePtr != NULL) {
		VarTrace *tracePtr = elPtr->tracePtr;
		elPtr->tracePtr = tracePtr->nextPtr;
		ckfree((char *) tracePtr);
	    }
	    for (activePtr = iPtr->activeTracePtr; activePtr != NULL;
		    activePtr = activePtr->nextPtr) {
		if (activePtr->varPtr == elPtr) {
		    activePtr->nextTracePtr = NULL;
		}
	    }
	}
	elPtr->flags = VAR_UNDEFINED;
	if (elPtr->refCount == 0) {
	    ckfree((char *) elPtr);
	}
    }
    Tcl_DeleteHashTable(varPtr->value.tablePtr);
    ckfree((char *) varPtr->value.tablePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * CleanupVar --
 *
 *	This procedure is called when it looks like it may be OK
 *	to free up the variable's record and hash table entry, and
 *	those of its containing parent.  It's called, for example,
 *	when a trace on a variable deletes the variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If the variable (or its containing array) really is dead then
 *	its record, and possibly its hash table entry, gets freed up.
 *
 *----------------------------------------------------------------------
 */

static void
CleanupVar(varPtr, arrayPtr)
    Var *varPtr;		/* Pointer to variable that may be a
				 * candidate for being expunged. */
    Var *arrayPtr;		/* Array that contains the variable, or
				 * NULL if this variable isn't an array
				 * element. */
{
    if ((varPtr->flags & VAR_UNDEFINED) && (varPtr->refCount == 0)
	    && (varPtr->tracePtr == NULL)) {
	if (varPtr->hPtr != NULL) {
	    Tcl_DeleteHashEntry(varPtr->hPtr);
	}
	ckfree((char *) varPtr);
    }
    if (arrayPtr != NULL) {
	if ((arrayPtr->flags & VAR_UNDEFINED) && (arrayPtr->refCount == 0)
		&& (arrayPtr->tracePtr == NULL)) {
	    if (arrayPtr->hPtr != NULL) {
		Tcl_DeleteHashEntry(arrayPtr->hPtr);
	    }
	    ckfree((char *) arrayPtr);
	}
    }
    return;
}

/*
 *----------------------------------------------------------------------
 *
 * VarErrMsg --
 *
 *	Generate a reasonable error message describing why a variable
 *	operation failed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Interp->result is reset to hold a message identifying the
 *	variable given by part1 and part2 and describing why the
 *	variable operation failed.
 *
 *----------------------------------------------------------------------
 */

static void
VarErrMsg(interp, part1, part2, operation, reason)
    Tcl_Interp *interp;		/* Interpreter in which to record message. */
    char *part1, *part2;	/* Variable's two-part name. */
    char *operation;		/* String describing operation that failed,
				 * e.g. "read", "set", or "unset". */
    char *reason;		/* String describing why operation failed. */
{
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, "can't ", operation, " \"", part1, (char *) NULL);
    if (part2 != NULL) {
	Tcl_AppendResult(interp, "(", part2, ")", (char *) NULL);
    }
    Tcl_AppendResult(interp, "\": ", reason, (char *) NULL);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGetVar --
 *
 *	Return the value of a Tcl variable.
 *
 * Results:
 *	The return value points to the current value of varName.  If
 *	the variable is not defined or can't be read because of a clash
 *	in array usage then a NULL pointer is returned and an error
 *	message is left in interp->result if the TCL_LEAVE_ERR_MSG
 *	flag is set.  Note:  the return value is only valid up until
 *	the next call to Tcl_SetVar or Tcl_SetVar2;  if you depend on
 *	the value lasting longer than that, then make yourself a private
 *	copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
_Tcl_WGetVar(interp, wvarName, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    wchar_t *wvarName;		/* Name of a variable in interp. */
    int flags;			/* OR-ed combination of TCL_GLOBAL_ONLY
				 * or TCL_LEAVE_ERR_MSG bits. */
{
    register wchar_t *wp;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return NULL;
    }

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (wp = wvarName; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_paren) 
	{
	    char *result;
	    wchar_t *wopen = wp;

	    do {
		wp++;
	    } while (*wp != i18n_special_chars.w_esc_zero);
	    wp--;
	    if (*wp != i18n_special_chars.w_cl_paren) {
		goto scalar;
	    }
	    *wopen = i18n_special_chars.w_esc_zero;
	    *wp = i18n_special_chars.w_esc_zero;
	    result = _Tcl_WGetVar2(interp, wvarName, wopen+1, flags);
	    *wopen = i18n_special_chars.w_op_paren;
	    *wp = i18n_special_chars.w_cl_paren;
	    return result;
	}
    }

    scalar:
    return _Tcl_WGetVar2(interp, wvarName, (wchar_t *) NULL, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGetVar2 --
 *
 *	Return the value of a Tcl variable, given a two-part name
 *	consisting of array name and element within array.
 *
 * Results:
 *	The return value points to the current value of the variable
 *	given by part1 and part2.  If the specified variable doesn't
 *	exist, or if there is a clash in array usage, then NULL is
 *	returned and a message will be left in interp->result if the
 *	TCL_LEAVE_ERR_MSG flag is set.  Note:  the return value is
 *	only valid up until the next call to Tcl_SetVar or Tcl_SetVar2;
 *	if you depend on the value lasting longer than that, then make
 *	yourself a private copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
_Tcl_WGetVar2(interp, wpart1, wpart2, flags)
    Tcl_Interp *interp;		/* Command interpreter in which variable is
				 * to be looked up. */
    wchar_t *wpart1;		/* Name of array (if wpart2 is NULL) or
				 * name of variable. */
    wchar_t *wpart2;		/* If non-null, gives name of element in
				 * array. */
    int flags;			/* OR-ed combination of TCL_GLOBAL_ONLY
				 * or TCL_LEAVE_ERR_MSG bits. */
{
    Var *varPtr, *arrayPtr;
    Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format;
    char *ctemp1, *ctemp2;
    size_t clen, st;

    _WChar_To_Multibyte(interp, clen, wpart1, ctemp1, st)
    if (st == (size_t)-1)
	return NULL;

    if (wpart2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, clen, wpart2, ctemp2, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1);
		return NULL;
	}
    }
    else
	ctemp2 = (char *)NULL;

    varPtr = LookupVar(interp, ctemp1, ctemp2, flags, "read", CRT_PART2,
	    &arrayPtr);
    if (varPtr == NULL) {
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return NULL;
    }

    /*
     * Invoke any traces that have been set for the variable.
     */

    if ((varPtr->tracePtr != NULL)
	    || ((arrayPtr != NULL) && (arrayPtr->tracePtr != NULL))) {
	char *msg;

	msg = CallTraces(iPtr, arrayPtr, varPtr, ctemp1, ctemp2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_READS);
	if (msg != NULL) {
	    VarErrMsg(interp, ctemp1, ctemp2, "read", msg);
	    goto cleanup;
	}
    }
    if (!(varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR|VAR_ARRAY))) {
	return varPtr->value.string;
    }
    if (flags & TCL_LEAVE_ERR_MSG) 
    {
	if ((varPtr->flags & VAR_UNDEFINED) && (arrayPtr != NULL)
		&& !(arrayPtr->flags & VAR_UNDEFINED)) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_ele);
	    VarErrMsg(interp, ctemp1, ctemp2, "read", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	} else {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_var);
	    VarErrMsg(interp, ctemp1, ctemp2, "read", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
    }

    /*
     * If the variable doesn't exist anymore and no-one's using it,
     * then free up the relevant structures and hash table entries.
     */

    cleanup:
    if (varPtr->flags & VAR_UNDEFINED) {
	CleanupVar(varPtr, arrayPtr);
    }
    ckfree(ctemp1);
    if (ctemp2 != NULL)
	ckfree(ctemp2);
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSetVar --
 *
 *	Change the value of a variable.
 *
 * Results:
 *	Returns a pointer to the malloc'ed string holding the new
 *	value of the variable.  The caller should not modify this
 *	string.  If the write operation was disallowed then NULL
 *	is returned;  if the TCL_LEAVE_ERR_MSG flag is set, then
 *	an explanatory message will be left in interp->result.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	its value is changed to newValue.  If varName isn't currently
 *	defined, then a new global variable by that name is created.
 *
 *----------------------------------------------------------------------
 */

char *
_Tcl_WSetVar(interp, wvarName, wnewValue, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    wchar_t *wvarName;		/* Name of a variable in interp. */
    wchar_t *wnewValue;		/* New value for varName. */
    int flags;			/* Various flags that tell how to set value:
				 * any of TCL_GLOBAL_ONLY, TCL_APPEND_VALUE,
				 * TCL_LIST_ELEMENT, or TCL_LEAVE_ERR_MSG. */
{
    register wchar_t *wp;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return NULL;
    }

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (wp = wvarName; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_paren) 
	{
	    char *result;
	    wchar_t *wopen = wp;

	    do {
		wp++;
	    } while (*wp != i18n_special_chars.w_esc_zero);
	    wp--;
	    if (*wp != i18n_special_chars.w_cl_paren) 
	    {
		goto scalar;
	    }
	    *wopen = i18n_special_chars.w_esc_zero;
	    *wp = i18n_special_chars.w_esc_zero;
	    result = _Tcl_WSetVar2(interp, wvarName, wopen+1, wnewValue, flags);
	    *wopen = i18n_special_chars.w_op_paren;
	    *wp = i18n_special_chars.w_cl_paren;
	    return result;
	}
    }

    scalar:
    return _Tcl_WSetVar2(interp, wvarName, (wchar_t *) NULL, wnewValue, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSetVar2 --
 *
 *	Given a two-part variable name, which may refer either to a
 *	scalar variable or an element of an array, change the value
 *	of the variable.  If the named scalar or array or element
 *	doesn't exist then create one.
 *
 * Results:
 *	Returns a pointer to the malloc'ed string holding the new
 *	value of the variable.  The caller should not modify this
 *	string.  If the write operation was disallowed because an
 *	array was expected but not found (or vice versa), then NULL
 *	is returned;  if the TCL_LEAVE_ERR_MSG flag is set, then
 *	an explanatory message will be left in interp->result.
 *
 * Side effects:
 *	The value of the given variable is set.  If either the array
 *	or the entry didn't exist then a new one is created.
 *
 *----------------------------------------------------------------------
 */

char *
_Tcl_WSetVar2(interp, wpart1, wpart2, wnewValue, flags)
    Tcl_Interp *interp;		/* Command interpreter in which variable is
				 * to be looked up. */
    wchar_t *wpart1;		/* If part2 is NULL, this is name of scalar
				 * variable.  Otherwise it is name of array. */
    wchar_t *wpart2;		/* Name of an element within array, or NULL. */
    wchar_t *wnewValue;		/* New value for variable. */
    int flags;			/* Various flags that tell how to set value:
				 * any of TCL_GLOBAL_ONLY, TCL_APPEND_VALUE,
				 * TCL_LIST_ELEMENT, or TCL_LEAVE_ERR_MSG . */
{
    register Var *varPtr;
    register Interp *iPtr = (Interp *) interp;
    int length, listFlags, cerr;
    Var *arrayPtr;
    char *result, *ctemp, *ctemp1, *ctemp2;
    unsigned char *i18n_msg_format;
    size_t clen, st;

    _WChar_To_Multibyte(interp, clen, wpart1, ctemp1, st)
    if (st == (size_t)-1)
	return NULL;

    if (wpart2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, clen, wpart2, ctemp2, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1);
		return NULL;
    	}
    }
    else
	ctemp2 = (char *)NULL;

    varPtr = LookupVar(interp, ctemp1, ctemp2, flags, "set", 
		CRT_PART1|CRT_PART2, &arrayPtr);
    if (varPtr == NULL) {
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return NULL;
    }

    /*
     * If the variable's hPtr field is NULL, it means that this is an
     * upvar to an array element where the array was deleted, leaving
     * the element dangling at the end of the upvar.  Generate an error
     * (allowing the variable to be reset would screw up our storage
     * allocation and is meaningless anyway).
     */

    if (varPtr->hPtr == NULL) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_dangling_upvar);
	    VarErrMsg(interp, ctemp1, ctemp2, "set", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return NULL;
    }

    /*
     * Clear the variable's current value unless this is an
     * append operation.
     */

    if (varPtr->flags & VAR_ARRAY) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_is_array);
	    VarErrMsg(interp, ctemp1, ctemp2, "set", (char *)i18n_msg_format);
	    free(i18n_msg_format);
	}
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return NULL;
    }
    if (!(flags & TCL_APPEND_VALUE) || (varPtr->flags & VAR_UNDEFINED)) {
	varPtr->valueLength = 0;
    }

    /*
     * Compute how many total bytes will be needed for the variable's
     * new value (leave space for a separating space between list
     * elements).  Allocate new space for the value if needed.
     */

    if (flags & TCL_LIST_ELEMENT) 
    {
	length = _Tcl_WScanElement(interp, wnewValue, &listFlags, &cerr) + 1;
	if (cerr == TCL_CONV_ERROR)
	{
	    ckfree(ctemp1);
	    if (ctemp2 != NULL)
		ckfree(ctemp2);
	    return NULL;
	}
    } 
    else 
    {
	length = wcslen(wnewValue) * sizeof(wchar_t);
    }
    length += varPtr->valueLength;
    if (length >= varPtr->valueSpace) {
	char *newValue;
	int newSize;

	newSize = 2*varPtr->valueSpace;
	if (newSize <= length) {
	    newSize = length + 1;
	}
	if (newSize < 24) {
	    /*
	     * Don't waste time with teensy-tiny variables;  we'll
	     * just end up expanding them later.
	     */

	    newSize = 24;
	}
	newValue = ckalloc((unsigned) newSize);
	if (varPtr->valueSpace > 0) {
	    strcpy(newValue, varPtr->value.string);
	    ckfree(varPtr->value.string);
	}
	varPtr->valueSpace = newSize;
	varPtr->value.string = newValue;
    }

    /*
     * Append the new value to the variable, either as a list
     * element or as a string.
     */

    if (flags & TCL_LIST_ELEMENT) {
	wchar_t *wdst = (wchar_t *)ckalloc(length);
	char carr[2];
	wchar_t warr1, warr2;
	char *dst = varPtr->value.string + varPtr->valueLength;
	int err;

	carr[1] = '\0';
	carr[0] = dst[-1];
	mbtowc(&warr1, (const char *)carr, (size_t)1);
	carr[0] = dst[-2];
	mbtowc(&warr2, (const char *)carr, (size_t)1);
	if ((varPtr->valueLength > 0) 
		&& ((warr1 != i18n_special_chars.w_op_cbra)
		|| ((varPtr->valueLength > 1) 
		&& (warr2 == i18n_special_chars.w_bsla)))) 
	{
	    *dst = ' ';
	    dst++;
	    varPtr->valueLength++;
	}
	(void)_Tcl_WConvertElement(interp, wnewValue, wdst, listFlags, &err);
	if(err == TCL_CONV_ERROR)
	{
		ckfree(ctemp1); 
		if (ctemp2 != NULL)
			ckfree(ctemp2);
		ckfree(wdst);
		return NULL;
	}
	_WChar_To_Multibyte(interp, clen, wdst, ctemp, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1); 
		if (ctemp2 != NULL)
			ckfree(ctemp2);
		ckfree(wdst);
		return NULL;
	}
	strcpy(dst, ctemp);
	varPtr->valueLength += strlen(ctemp);
	ckfree(wdst);
	ckfree(ctemp);
    } 
    else 
    {
	_WChar_To_Multibyte(interp, clen, wnewValue, ctemp, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1); 
		if (ctemp2 != NULL)
			ckfree(ctemp2);
		return NULL;
	}
	strcpy(varPtr->value.string + varPtr->valueLength, ctemp);
	varPtr->valueLength += strlen(ctemp);
	ckfree(ctemp);
    }
    varPtr->flags &= ~VAR_UNDEFINED;

    /*
     * Invoke any write traces for the variable.
     */

    if ((varPtr->tracePtr != NULL)
	    || ((arrayPtr != NULL) && (arrayPtr->tracePtr != NULL))) {
	char *msg;

	msg = CallTraces(iPtr, arrayPtr, varPtr, ctemp1, ctemp2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_WRITES);
	if (msg != NULL) {
	    VarErrMsg(interp, ctemp1, ctemp2, "set", msg);
	    result = NULL;
	    goto cleanup;
	}
    }

    /*
     * If the variable was changed in some gross way by a trace (e.g.
     * it was unset and then recreated as an array) then just return
     * an empty string;  otherwise return the variable's current
     * value.
     */

    if (!(varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR|VAR_ARRAY))) {
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return varPtr->value.string;
    }
    result = "";

    /*
     * If the variable doesn't exist anymore and no-one's using it,
     * then free up the relevant structures and hash table entries.
     */

    cleanup:
    if (varPtr->flags & VAR_UNDEFINED) {
	CleanupVar(varPtr, arrayPtr);
    }
    ckfree(ctemp1);
    if (ctemp2 != NULL)
    	ckfree(ctemp2);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUnsetVar --
 *
 *	Delete a variable, so that it may not be accessed anymore.
 *
 * Results:
 *	Returns TCL_OK if the variable was successfully deleted, TCL_ERROR
 *	if the variable can't be unset.  In the event of an error,
 *	if the TCL_LEAVE_ERR_MSG flag is set then an error message
 *	is left in interp->result.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	it is deleted.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WUnsetVar(interp, wvarName, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    wchar_t *wvarName;		/* Name of a variable in interp.  May be
				 * either a scalar name or an array name
				 * or an element in an array. */
    int flags;			/* OR-ed combination of any of
				 * TCL_GLOBAL_ONLY or TCL_LEAVE_ERR_MSG. */
{
    register wchar_t *wp;
    int result;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * Figure out whether this is an array reference, then call
     * Tcl_UnsetVar2 to do all the real work.
     */

    for (wp = wvarName; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_paren) 
	{
	    wchar_t *wopen = wp;

	    do {
		wp++;
	    } while (*wp != i18n_special_chars.w_esc_zero);
	    wp--;
	    if (*wp != i18n_special_chars.w_cl_paren) 
	    {
		goto scalar;
	    }
	    *wopen = i18n_special_chars.w_esc_zero;
	    *wp = i18n_special_chars.w_esc_zero;
	    result = _Tcl_WUnsetVar2(interp, wvarName, wopen+1, flags);
	    *wopen = i18n_special_chars.w_op_paren;
	    *wp = i18n_special_chars.w_cl_paren;
	    return result;
	}
    }

    scalar:
    return _Tcl_WUnsetVar2(interp, wvarName, (wchar_t *) NULL, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUnsetVar2 --
 *
 *	Delete a variable, given a 2-part name.
 *
 * Results:
 *	Returns TCL_OK if the variable was successfully deleted, TCL_ERROR
 *	if the variable can't be unset.  In the event of an error,
 *	if the TCL_LEAVE_ERR_MSG flag is set then an error message
 *	is left in interp->result.
 *
 * Side effects:
 *	If part1 and part2 indicate a local or global variable in interp,
 *	it is deleted.  If part1 is an array name and part2 is NULL, then
 *	the whole array is deleted.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WUnsetVar2(interp, wpart1, wpart2, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    wchar_t *wpart1;		/* Name of variable or array. */
    wchar_t *wpart2;		/* Name of element within array or NULL. */
    int flags;			/* OR-ed combination of any of
				 * TCL_GLOBAL_ONLY or TCL_LEAVE_ERR_MSG. */
{
    Var *varPtr, dummyVar;
    Interp *iPtr = (Interp *) interp;
    Var *arrayPtr;
    ActiveVarTrace *activePtr;
    int result;
    unsigned char *i18n_msg_format;
    char *ctemp1, *ctemp2;
    size_t clen, st;

    _WChar_To_Multibyte(interp, clen, wpart1, ctemp1, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    if (wpart2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, clen, wpart2, ctemp2, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1);
		return TCL_CONV_ERROR;
	}
    }
    else
	ctemp2 = (char *)NULL;

    varPtr = LookupVar(interp, ctemp1, ctemp2, flags, "unset", 0,  &arrayPtr);
    if (varPtr == NULL) {
	return TCL_ERROR;
    }
    result = (varPtr->flags & VAR_UNDEFINED) ? TCL_ERROR : TCL_OK;

    if ((ctemp2 != NULL) && (arrayPtr->searchPtr != NULL)) {
	DeleteSearches(arrayPtr);
    }

    /*
     * The code below is tricky, because of the possibility that
     * a trace procedure might try to access a variable being
     * deleted.  To handle this situation gracefully, do things
     * in three steps:
     * 1. Copy the contents of the variable to a dummy variable
     *    structure, and mark the original structure as undefined.
     * 2. Invoke traces and clean up the variable, using the copy.
     * 3. If at the end of this the original variable is still
     *    undefined and has no outstanding references, then delete
     *	  it (but it could have gotten recreated by a trace).
     */

    dummyVar = *varPtr;
    varPtr->valueSpace = 0;
    varPtr->flags = VAR_UNDEFINED;
    varPtr->tracePtr = NULL;

    /*
     * Call trace procedures for the variable being deleted and delete
     * its traces.  Be sure to abort any other traces for the variable
     * that are still pending.  Special tricks:
     * 1. Increment varPtr's refCount around this:  CallTraces will
     *    use dummyVar so it won't increment varPtr's refCount.
     * 2. Turn off the VAR_TRACE_ACTIVE flag in dummyVar: we want to
     *    call unset traces even if other traces are pending.
     */

    if ((dummyVar.tracePtr != NULL)
	    || ((arrayPtr != NULL) && (arrayPtr->tracePtr != NULL))) {
	varPtr->refCount++;
	dummyVar.flags &= ~VAR_TRACE_ACTIVE;
	(void) CallTraces(iPtr, arrayPtr, &dummyVar, ctemp1, ctemp2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_UNSETS);
	while (dummyVar.tracePtr != NULL) {
	    VarTrace *tracePtr = dummyVar.tracePtr;
	    dummyVar.tracePtr = tracePtr->nextPtr;
	    ckfree((char *) tracePtr);
	}
	for (activePtr = iPtr->activeTracePtr; activePtr != NULL;
		activePtr = activePtr->nextPtr) {
	    if (activePtr->varPtr == varPtr) {
		activePtr->nextTracePtr = NULL;
	    }
	}
	varPtr->refCount--;
    }

    /*
     * If the variable is an array, delete all of its elements.  This
     * must be done after calling the traces on the array, above (that's
     * the way traces are defined).
     */

    if (dummyVar.flags & VAR_ARRAY) {
	DeleteArray(iPtr, ctemp1, &dummyVar,
	    (flags & TCL_GLOBAL_ONLY) | TCL_TRACE_UNSETS);
    }
    if (dummyVar.valueSpace > 0) {
	ckfree(dummyVar.value.string);
    }
    if (result == TCL_ERROR) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    if (ctemp2 == NULL) {
	    	i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_var);
	    	VarErrMsg(interp, ctemp1, ctemp2, "unset", (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	    else {
	    	i18n_msg_format = dce_msg_get(tcl_s_tclvar_no_such_ele);
	    	VarErrMsg(interp, ctemp1, ctemp2, "unset", (char *)i18n_msg_format);
		free(i18n_msg_format);
	    }
	}
    }

    /*
     * Finally, if the variable is truly not in use then free up its
     * record and remove it from the hash table.
     */

    CleanupVar(varPtr, arrayPtr);
    ckfree(ctemp1);
    if (ctemp2 != NULL)
    	ckfree(ctemp2);
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WTraceVar --
 *
 *	Arrange for reads and/or writes to a variable to cause a
 *	procedure to be invoked, which can monitor the operations
 *	and/or change their actions.
 *
 * Results:
 *	A standard Tcl return value.
 *
 * Side effects:
 *	A trace is set up on the variable given by varName, such that
 *	future references to the variable will be intermediated by
 *	proc.  See the manual entry for complete details on the calling
 *	sequence for proc.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WTraceVar(interp, wvarName, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which variable is
				 * to be traced. */
    wchar_t *wvarName;		/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* OR-ed collection of bits, including any
				 * of TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure to call when specified ops are
				 * invoked upon varName. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    register wchar_t *wp;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (wp = wvarName; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_paren) 
	{
	    int result;
	    wchar_t *wopen = wp;

	    do {
		wp++;
	    } while (*wp != i18n_special_chars.w_esc_zero);
	    wp--;
	    if (*wp != i18n_special_chars.w_cl_paren) 
	    {
		goto scalar;
	    }
	    *wopen = i18n_special_chars.w_esc_zero;
	    *wp = i18n_special_chars.w_esc_zero;
	    result = _Tcl_WTraceVar2(interp, wvarName, wopen+1, flags,
		    proc, clientData);
	    *wopen = i18n_special_chars.w_op_paren;
	    *wp = i18n_special_chars.w_cl_paren;
	    return result;
	}
    }

    scalar:
    return _Tcl_WTraceVar2(interp, wvarName, (wchar_t *) NULL, flags,
	    proc, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WTraceVar2 --
 *
 *	Arrange for reads and/or writes to a variable to cause a
 *	procedure to be invoked, which can monitor the operations
 *	and/or change their actions.
 *
 * Results:
 *	A standard Tcl return value.
 *
 * Side effects:
 *	A trace is set up on the variable given by part1 and part2, such
 *	that future references to the variable will be intermediated by
 *	proc.  See the manual entry for complete details on the calling
 *	sequence for proc.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WTraceVar2(interp, wpart1, wpart2, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which variable is
				 * to be traced. */
    wchar_t *wpart1;		/* Name of scalar variable or array. */
    wchar_t *wpart2;		/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* OR-ed collection of bits, including any
				 * of TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure to call when specified ops are
				 * invoked upon varName. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    Var *varPtr, *arrayPtr;
    register VarTrace *tracePtr;
    char *ctemp1, *ctemp2;
    size_t clen, st;
    unsigned char *i18n_msg_format;

    _WChar_To_Multibyte(interp, clen, wpart1, ctemp1, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    if (wpart2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, clen, wpart2, ctemp2, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1);
		return TCL_CONV_ERROR;
	}
    }
    else
	ctemp2 = (char *)NULL;

    varPtr = LookupVar(interp, ctemp1, ctemp2, (flags | TCL_LEAVE_ERR_MSG),
	    "trace", CRT_PART1|CRT_PART2, &arrayPtr);
    if (varPtr == NULL) {
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return TCL_ERROR;
    }
    ckfree(ctemp1);
    if (ctemp2 != NULL)
	ckfree(ctemp2);

    /*
     * Set up trace information.
     */

    tracePtr = (VarTrace *) ckalloc(sizeof(VarTrace));
    tracePtr->traceProc = proc;
    tracePtr->clientData = clientData;
    tracePtr->flags = flags &
	    (TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS);
    tracePtr->nextPtr = varPtr->tracePtr;
    varPtr->tracePtr = tracePtr;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUntraceVar --
 *
 *	Remove a previously-created trace for a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If there exists a trace for the variable given by varName
 *	with the given flags, proc, and clientData, then that trace
 *	is removed.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WUntraceVar(interp, wvarName, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter containing traced variable. */
    wchar_t *wvarName;		/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* OR-ed collection of bits describing
				 * current trace, including any of
				 * TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    register wchar_t *wp;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_CONV_ERROR;
    }

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (wp = wvarName; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_paren) 
	{
	    wchar_t *wopen = wp;

	    do {
		wp++;
	    } while (*wp != i18n_special_chars.w_esc_zero);
	    wp--;
	    if (*wp != i18n_special_chars.w_cl_paren) 
	    {
		goto scalar;
	    }
	    *wopen = i18n_special_chars.w_esc_zero;
	    *wp = i18n_special_chars.w_esc_zero;
	    _Tcl_WUntraceVar2(interp, wvarName, wopen+1, flags, proc, clientData);
	    *wopen = i18n_special_chars.w_op_paren;
	    *wp = i18n_special_chars.w_cl_paren;
	    return TCL_CONV_ERROR;
	}
    }

    scalar:
    return _Tcl_WUntraceVar2(interp, wvarName, (wchar_t *) NULL, flags, proc, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUntraceVar2 --
 *
 *	Remove a previously-created trace for a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If there exists a trace for the variable given by part1
 *	and part2 with the given flags, proc, and clientData, then
 *	that trace is removed.
 *
 *----------------------------------------------------------------------
 */

int
_Tcl_WUntraceVar2(interp, wpart1, wpart2, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter containing traced variable. */
    wchar_t *wpart1;		/* Name of variable or array. */
    wchar_t *wpart2;		/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* OR-ed collection of bits describing
				 * current trace, including any of
				 * TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData clientData;	/* Arbitrary argument to pass to proc. */
{
    register VarTrace *tracePtr;
    VarTrace *prevPtr;
    Var *varPtr, *arrayPtr;
    Interp *iPtr = (Interp *) interp;
    ActiveVarTrace *activePtr;
    char *ctemp1, *ctemp2;
    size_t clen, st;
    unsigned char *i18n_msg_format;

    _WChar_To_Multibyte(interp, clen, wpart1, ctemp1, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    if (wpart2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, clen, wpart2, ctemp2, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1);
		return TCL_CONV_ERROR;
	}
    }
    else
	ctemp2 = (char *)NULL;


    varPtr = LookupVar(interp, ctemp1, ctemp2, flags & TCL_GLOBAL_ONLY,
	    (char *) NULL, 0, &arrayPtr);
    if (varPtr == NULL) {
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return TCL_OK;
    }

    flags &= (TCL_TRACE_READS | TCL_TRACE_WRITES | TCL_TRACE_UNSETS);
    for (tracePtr = varPtr->tracePtr, prevPtr = NULL; ;
	    prevPtr = tracePtr, tracePtr = tracePtr->nextPtr) {
	if (tracePtr == NULL) {
	    ckfree(ctemp1);
	    if (ctemp2 != NULL)
		ckfree(ctemp2);
	    return TCL_OK;
	}
	if ((tracePtr->traceProc == proc) && (tracePtr->flags == flags)
		&& (tracePtr->clientData == clientData)) {
	    break;
	}
    }

    /*
     * The code below makes it possible to delete traces while traces
     * are active:  it makes sure that the deleted trace won't be
     * processed by CallTraces.
     */

    for (activePtr = iPtr->activeTracePtr; activePtr != NULL;
	    activePtr = activePtr->nextPtr) {
	if (activePtr->nextTracePtr == tracePtr) {
	    activePtr->nextTracePtr = tracePtr->nextPtr;
	}
    }
    if (prevPtr == NULL) {
	varPtr->tracePtr = tracePtr->nextPtr;
    } else {
	prevPtr->nextPtr = tracePtr->nextPtr;
    }
    ckfree((char *) tracePtr);
    ckfree(ctemp1);
    if (ctemp2 != NULL)
	ckfree(ctemp2);

    /*
     * If this is the last trace on the variable, and the variable is
     * unset and unused, then free up the variable.
     */

    if (varPtr->flags & VAR_UNDEFINED) {
	CleanupVar(varPtr, (Var *) NULL);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WVarTraceInfo --
 *
 *	Return the clientData value associated with a trace on a
 *	variable.  This procedure can also be used to step through
 *	all of the traces on a particular variable that have the
 *	same trace procedure.
 *
 * Results:
 *	The return value is the clientData value associated with
 *	a trace on the given variable.  Information will only be
 *	returned for a trace with proc as trace procedure.  If
 *	the clientData argument is NULL then the first such trace is
 *	returned;  otherwise, the next relevant one after the one
 *	given by clientData will be returned.  If the variable
 *	doesn't exist, or if there are no (more) traces for it,
 *	then NULL is returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

ClientData
_Tcl_WVarTraceInfo(interp, wvarName, flags, proc, prevClientData)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    wchar_t *wvarName;		/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* 0 or TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData prevClientData;	/* If non-NULL, gives last value returned
				 * by this procedure, so this call will
				 * return the next trace after that one.
				 * If NULL, this call will return the
				 * first trace. */
{
    register wchar_t *wp;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return (ClientData)NULL;
    }

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (wp = wvarName; *wp != i18n_special_chars.w_esc_zero; wp++) 
    {
	if (*wp == i18n_special_chars.w_op_paren) 
	{
	    ClientData result;
	    wchar_t *wopen = wp;

	    do {
		wp++;
	    } while (*wp != i18n_special_chars.w_esc_zero);
	    wp--;
	    if (*wp != i18n_special_chars.w_cl_paren) 
	    {
		goto scalar;
	    }
	    *wopen = i18n_special_chars.w_esc_zero;
	    *wp = i18n_special_chars.w_esc_zero;
	    result = _Tcl_WVarTraceInfo2(interp, wvarName, wopen+1, flags, proc,
		prevClientData);
	    *wopen = i18n_special_chars.w_op_paren;
	    *wp = i18n_special_chars.w_cl_paren;
	    return result;
	}
    }

    scalar:
    return _Tcl_WVarTraceInfo2(interp, wvarName, (wchar_t *) NULL, flags, proc,
	    prevClientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_VarTraceInfo2 --
 *
 *	Same as Tcl_VarTraceInfo, except takes name in two pieces
 *	instead of one.
 *
 * Results:
 *	Same as Tcl_VarTraceInfo.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

ClientData
_Tcl_WVarTraceInfo2(interp, wpart1, wpart2, flags, proc, prevClientData)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    wchar_t *wpart1;		/* Name of variable or array. */
    wchar_t *wpart2;		/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* 0 or TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    ClientData prevClientData;	/* If non-NULL, gives last value returned
				 * by this procedure, so this call will
				 * return the next trace after that one.
				 * If NULL, this call will return the
				 * first trace. */
{
    register VarTrace *tracePtr;
    Var *varPtr, *arrayPtr;
    char *ctemp1, *ctemp2;
    size_t clen, st;
    unsigned char *i18n_msg_format;

    _WChar_To_Multibyte(interp, clen, wpart1, ctemp1, st)
    if (st == (size_t)-1)
	return (ClientData)NULL;

    if (wpart2 != (wchar_t *)NULL)
    {
	_WChar_To_Multibyte(interp, clen, wpart2, ctemp2, st)
	if (st == (size_t)-1)
	{
		ckfree(ctemp1);
		return (ClientData)NULL;
	}
    }
    else
	ctemp2 = (char *)NULL;


    varPtr = LookupVar(interp, ctemp1, ctemp2, flags & TCL_GLOBAL_ONLY,
	    (char *) NULL, 0, &arrayPtr);
    if (varPtr == NULL) {
	ckfree(ctemp1);
	if (ctemp2 != NULL)
		ckfree(ctemp2);
	return NULL;
    }

    /*
     * Find the relevant trace, if any, and return its clientData.
     */

    tracePtr = varPtr->tracePtr;
    if (prevClientData != NULL) {
	for ( ; tracePtr != NULL; tracePtr = tracePtr->nextPtr) {
	    if ((tracePtr->clientData == prevClientData)
		    && (tracePtr->traceProc == proc)) {
		tracePtr = tracePtr->nextPtr;
		break;
	    }
	}
    }
    for ( ; tracePtr != NULL; tracePtr = tracePtr->nextPtr) {
	if (tracePtr->traceProc == proc) {
	    ckfree(ctemp1);
	    if (ctemp2 != NULL)
		ckfree(ctemp2);
	    return tracePtr->clientData;
	}
    }
    if (ctemp2 != NULL)
	ckfree(ctemp2);
    return NULL;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WSetCmd --
 *
 *	This procedure is invoked to process the "set" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WSetCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc == 2) 
    {
	char *value;

	value = _Tcl_WGetVar(interp, wargv[1], TCL_LEAVE_ERR_MSG);
	if (value == NULL) 
	{
	    return TCL_ERROR;
	}
	interp->result = value;
	return TCL_OK;
    } 
    else if (argc == 3) 
    {
	char *result;

	result = _Tcl_WSetVar(interp, wargv[1], wargv[2], TCL_LEAVE_ERR_MSG);
	if (result == NULL) 
	{
	    return TCL_ERROR;
	}
	interp->result = result;
	return TCL_OK;
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_newval);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("set")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "set");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUnsetCmd --
 *
 *	This procedure is invoked to process the "unset" Tcl command.
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
_Tcl_WUnsetCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int i;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 2) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("unset")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "unset");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    for (i = 1; i < argc; i++) 
    {
	if (_Tcl_WUnsetVar(interp, wargv[i], TCL_LEAVE_ERR_MSG) != TCL_OK) 
	{
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WAppendCmd --
 *
 *	This procedure is invoked to process the "append" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WAppendCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int i;
    char *result = NULL;		/* (Initialization only needed to keep
					 * the compiler from complaining) */
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_val_val);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("append")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "append");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    for (i = 2; i < argc; i++) 
    {
	result = _Tcl_WSetVar(interp, wargv[1], wargv[i],
		TCL_APPEND_VALUE|TCL_LEAVE_ERR_MSG);
	if (result == NULL) 
	{
	    return TCL_ERROR;
	}
    }
    interp->result = result;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WLappendCmd --
 *
 *	This procedure is invoked to process the "lappend" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
_Tcl_WLappendCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int i;
    char *result = NULL;		/* (Initialization only needed to keep
					 * the compiler from complaining) */
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_val_val);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("lappend")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "lappend");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    for (i = 2; i < argc; i++) 
    {
	result = _Tcl_WSetVar(interp, wargv[1], wargv[i],
		TCL_APPEND_VALUE|TCL_LIST_ELEMENT|TCL_LEAVE_ERR_MSG);
	if (result == NULL) 
	{
	    return TCL_ERROR;
	}
    }
    interp->result = result;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WArrayCmd --
 *
 *	This procedure is invoked to process the "array" Tcl command.
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
_Tcl_WArrayCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    int length;
    char c, *ctemp1, *ctemp2, *ctemp3;
    Var *varPtr;
    Tcl_HashEntry *hPtr;
    Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    size_t clen, st;

    if (argc < 3) 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_opt_arrname_arg);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Locate the array variable (and it better be an array).
     */
    _WChar_To_Multibyte(interp, clen, wargv[2], ctemp2, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    if (iPtr->varFramePtr == NULL) 
    {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, ctemp2);
    } 
    else 
    {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, ctemp2);
    }
    if (hPtr == NULL) 
    {
	notArray:
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_isnt_array);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp2)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp2);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(ctemp2);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) 
    {
	varPtr = varPtr->value.upvarPtr;
    }
    if (!(varPtr->flags & VAR_ARRAY)) 
    {
	goto notArray;
    }

    /*
     * Dispatch based on the option.
     */

    _WChar_To_Multibyte(interp, clen, wargv[1], ctemp1, st)
    if (st == (size_t)-1)
    {
	ckfree(ctemp2);
	return TCL_ERROR;
    }

    c = *ctemp1;
    length = strlen(ctemp1);
    if ((c == 'a') && (strncmp(ctemp1, "anymore", length) == 0)) 
    {
	ArraySearch *searchPtr;

	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_anymore_arrname_search);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    ckfree(ctemp1);
	    ckfree(ctemp2);
	    return TCL_ERROR;
	}
	_WChar_To_Multibyte(interp, clen, wargv[3], ctemp3, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp1);
	    ckfree(ctemp2);
	    return TCL_ERROR;
	}

	searchPtr = ParseSearchId(interp, varPtr, ctemp2, ctemp3);
	if (searchPtr == NULL) {
	    ckfree(ctemp1);
	    ckfree(ctemp2);
	    ckfree(ctemp3);
	    return TCL_ERROR;
	}
	while (1) 
	{
	    Var *varPtr2;

	    if (searchPtr->nextEntry != NULL) 
	    {
		varPtr2 = (Var *) Tcl_GetHashValue(searchPtr->nextEntry);
		if (!(varPtr2->flags & VAR_UNDEFINED)) 
		{
		    break;
		}
	    }
	    searchPtr->nextEntry = Tcl_NextHashEntry(&searchPtr->search);
	    if (searchPtr->nextEntry == NULL) 
	    {
		interp->result = "0";
	        ckfree(ctemp1);
	        ckfree(ctemp2);
	        ckfree(ctemp3);
		return TCL_OK;
	    }
	}
	interp->result = "1";
	ckfree(ctemp1);
	ckfree(ctemp2);
	ckfree(ctemp3);
	return TCL_OK;
    } 
    else if ((c == 'd') && (strncmp(ctemp1, "donesearch", length) == 0)) 
    {
	ArraySearch *searchPtr, *prevPtr;

	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_donesearch_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}

	_WChar_To_Multibyte(interp, clen, wargv[3], ctemp3, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp1);
	    ckfree(ctemp2);
	    return TCL_ERROR; 
	} 
	searchPtr = ParseSearchId(interp, varPtr, ctemp2, ctemp3); 
	if (searchPtr == NULL) 
	{
	    return TCL_ERROR;
	}
	if (varPtr->searchPtr == searchPtr) 
	{
	    varPtr->searchPtr = searchPtr->nextPtr;
	} 
	else 
	{
	    for (prevPtr = varPtr->searchPtr; ; prevPtr = prevPtr->nextPtr) 
	    {
		if (prevPtr->nextPtr == searchPtr) 
		{
		    prevPtr->nextPtr = searchPtr->nextPtr;
		    break;
		}
	    }
	}
	ckfree((char *) searchPtr);
	ckfree(ctemp1);
	ckfree(ctemp2);
	ckfree(ctemp3);
    } 
    else if ((c == 'n') && (strncmp(ctemp1, "names", length) == 0) && (length >= 2)) 
    {
	Tcl_HashSearch search;
	Var *varPtr2;
	wchar_t *wtemp;
	char *ctemp;
	size_t wlen;

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_names_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp1);
	    ckfree(ctemp2);

	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr2->flags & VAR_UNDEFINED) 
	    {
		continue;
	    }

	    ctemp = Tcl_GetHashKey(varPtr->value.tablePtr, hPtr);
	    _Multibyte_to_WChar(interp, wlen, ctemp, wtemp, st)
	    if (st == (size_t)-1)
	    {
		ckfree(ctemp1);
		ckfree(ctemp2);
		return TCL_ERROR;
	    }
	    _Tcl_WAppendElement(interp, wtemp);
	    ckfree(wtemp);
	}
	ckfree(ctemp1);
	ckfree(ctemp2);
    } 
    else if ((c == 'n') && (strncmp(ctemp1, "nextelement", length) == 0)
	    && (length >= 2)) 
    {
	ArraySearch *searchPtr;
	Tcl_HashEntry *hPtr;

	if (argc != 4) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_nextele_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}

	_WChar_To_Multibyte(interp, clen, wargv[3], ctemp3, st)
	if (st == (size_t)-1)
	{
	    ckfree(ctemp1);
	    ckfree(ctemp2);
	    return TCL_ERROR;
	}

	searchPtr = ParseSearchId(interp, varPtr, ctemp2, ctemp3);
	if (searchPtr == NULL) {
	    ckfree(ctemp1);
	    ckfree(ctemp2);
	    ckfree(ctemp3);
	    return TCL_ERROR;
	}
	while (1) 
	{
	    Var *varPtr2;

	    hPtr = searchPtr->nextEntry;
	    if (hPtr == NULL) 
	    {
		hPtr = Tcl_NextHashEntry(&searchPtr->search);
		if (hPtr == NULL) 
		{
		    ckfree(ctemp1);
		    ckfree(ctemp2);
		    ckfree(ctemp3);
		    return TCL_OK;
		}
	    } 
	    else 
	    {
		searchPtr->nextEntry = NULL;
	    }
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (!(varPtr2->flags & VAR_UNDEFINED)) 
	    {
		break;
	    }
	}
	interp->result = Tcl_GetHashKey(varPtr->value.tablePtr, hPtr);
	ckfree(ctemp1);
	ckfree(ctemp2);
	ckfree(ctemp3);
    } 
    else if ((c == 's') && (strncmp(ctemp1, "size", length) == 0)
	    && (length >= 2)) 
    {
	Tcl_HashSearch search;
	Var *varPtr2;
	int size;

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_size_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	size = 0;
	for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
		hPtr != NULL; hPtr = Tcl_NextHashEntry(&search)) 
	{
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr2->flags & VAR_UNDEFINED) 
	    {
		continue;
	    }
	    size++;
	}
	sprintf(interp->result, "%d", size);
	ckfree(ctemp1);
	ckfree(ctemp2);
    } 
    else if ((c == 's') && (strncmp(ctemp1, "startsearch", length) == 0)
	    && (length >= 2)) 
    {
	ArraySearch *searchPtr;

	if (argc != 3) 
	{
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_startsearch_arrname);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen("array")
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "array");
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    return TCL_ERROR;
	}
	searchPtr = (ArraySearch *) ckalloc(sizeof(ArraySearch));
	if (varPtr->searchPtr == NULL) {
	    searchPtr->id = 1;
	    Tcl_AppendResult(interp, "s-1-", ctemp2, (char *) NULL);
	} 
	else 
	{
	    char string[20];

	    searchPtr->id = varPtr->searchPtr->id + 1;
	    sprintf(string, "%d", searchPtr->id);
	    Tcl_AppendResult(interp, "s-", string, "-", ctemp2, (char *) NULL);
	}
	searchPtr->varPtr = varPtr;
	searchPtr->nextEntry = Tcl_FirstHashEntry(varPtr->value.tablePtr,
		&searchPtr->search);
	searchPtr->nextPtr = varPtr->searchPtr;
	varPtr->searchPtr = searchPtr;
	ckfree(ctemp1);
	ckfree(ctemp2);
    } 
    else 
    {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_bad_opt_array);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp1)
				+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp1);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp1);
	ckfree(ctemp2);

	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _WMakeUpvar --
 *
 *	This procedure does all of the work of the "global" and "upvar"
 *	commands.
 *
 * Results:
 *	A standard Tcl completion code.  If an error occurs then an
 *	error message is left in iPtr->result.
 *
 * Side effects:
 *	The variable given by myName is linked to the variable in
 *	framePtr given by otherP1 and otherP2, so that references to
 *	myName are redirected to the other variable like a symbolic
*	link.
 *
 *----------------------------------------------------------------------
 */

static int
_WMakeUpvar(iPtr, framePtr, wotherP1, wotherP2, wmyName)
    Interp *iPtr;		/* Interpreter containing variables.  Used
				 * for error messages, too. */
    CallFrame *framePtr;	/* Call frame containing "other" variable.
				 * NULL means use global context. */
    wchar_t *wotherP1, *wotherP2;	
				/* Two-part name of variable in framePtr. */
    wchar_t *wmyName;		/* Name of variable in local table, which
				 * will refer to otherP1/P2.  Must be a
				 * scalar. */
{
    Tcl_HashEntry *hPtr;
    Tcl_Interp *interp = (Tcl_Interp *)iPtr;
    Var *otherPtr, *varPtr, *arrayPtr;
    CallFrame *savedFramePtr;
    int new;
    unsigned char *i18n_msg_format, *i18n_msg_buf;
    char *ctemp1, *ctemp2, *cmyName;
    size_t clen, st;

    _WChar_To_Multibyte(interp, clen, wotherP1, ctemp1, st)
    if (st == (size_t)-1)
	return TCL_ERROR;

    _WChar_To_Multibyte(interp, clen, wotherP2, ctemp2, st)
    if (st == (size_t)-1)
    {
	ckfree(ctemp1);
	return TCL_ERROR;
    }

    _WChar_To_Multibyte(interp, clen, wmyName, cmyName, st)
    if (st == (size_t)-1)
    {
	ckfree(ctemp1);
	ckfree(ctemp2);
	return TCL_ERROR;
    }

    /*
     * In order to use LookupVar to find "other", temporarily replace
     * the current frame pointer in the interpreter.
     */

    savedFramePtr = iPtr->varFramePtr;
    iPtr->varFramePtr = framePtr;
    otherPtr = LookupVar((Tcl_Interp *) iPtr, ctemp1, ctemp2,
	    TCL_LEAVE_ERR_MSG, "access", CRT_PART1|CRT_PART2, &arrayPtr);
    iPtr->varFramePtr = savedFramePtr;
    if (otherPtr == NULL) {
	ckfree(ctemp1);
	ckfree(ctemp2);
	ckfree(cmyName);
	return TCL_ERROR;
    }
    if (iPtr->varFramePtr != NULL) {
	hPtr = Tcl_CreateHashEntry(&iPtr->varFramePtr->varTable, cmyName, &new);
    } else {
	hPtr = Tcl_CreateHashEntry(&iPtr->globalTable, cmyName, &new);
    }
    if (new) {
	varPtr = NewVar();
	Tcl_SetHashValue(hPtr, varPtr);
	varPtr->hPtr = hPtr;
    } else {
	/*
	 * The variable already exists.  If it's not an upvar then it's
	 * an error.  If it is an upvar, then just disconnect it from the
	 * thing it currently refers to.
	 */

	varPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (varPtr->flags & VAR_UPVAR) {
	    Var *upvarPtr;

	    upvarPtr = varPtr->value.upvarPtr;
	    if (upvarPtr == otherPtr) {
		ckfree(ctemp1);
		ckfree(ctemp2);
		ckfree(cmyName);
		return TCL_OK;
	    }
	    upvarPtr->refCount--;
	    if (upvarPtr->flags & VAR_UNDEFINED) {
		CleanupVar(upvarPtr, (Var *) NULL);
	    }
	} else if (!(varPtr->flags & VAR_UNDEFINED)) {
	    i18n_msg_format = dce_msg_get(tcl_s_tclvar_var_already_exists);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(cmyName)
				+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cmyName);
	    Tcl_AppendResult((Tcl_Interp *) iPtr, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(cmyName);
	    ckfree(ctemp1);
	    ckfree(ctemp2);

	    return TCL_ERROR;
	}
    }
    varPtr->flags = (varPtr->flags & ~VAR_UNDEFINED) | VAR_UPVAR;
    varPtr->value.upvarPtr = otherPtr;
    otherPtr->refCount++;
    ckfree(cmyName);
    ckfree(ctemp1);
    ckfree(ctemp2);
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WGlobalCmd --
 *
 *	This procedure is invoked to process the "global" Tcl command.
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
_Tcl_WGlobalCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if (argc < 2) {
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_varname_varname);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("global")
			+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "global");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if (iPtr->varFramePtr == NULL) {
	return TCL_OK;
    }

    for (argc--, wargv++; argc > 0; argc--, wargv++) {
	if (_WMakeUpvar(iPtr, (CallFrame *) NULL, *wargv, (wchar_t *) NULL, *wargv)
		!= TCL_OK) 
	{
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _Tcl_WUpvarCmd --
 *
 *	This procedure is invoked to process the "upvar" Tcl command.
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
_Tcl_WUpvarCmd(dummy, interp, argc, wargv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    wchar_t **wargv;			/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int result;
    CallFrame *framePtr;
    register wchar_t *wp;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
	return TCL_ERROR;
    }

    if (argc < 3) {
	upvarSyntax:
	i18n_msg_format = dce_msg_get(tcl_s_tclvar_wnum_args_level_othervar);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen("upvar")
			+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, "upvar");
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }

    /*
     * Find the hash table containing the variable being referenced.
     */

    result = _TclWGetFrame(interp, wargv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    argc -= result+1;
    if ((argc & 1) != 0) {
	goto upvarSyntax;
    }
    wargv += result+1;

    /*
     * Iterate over all the pairs of (other variable, local variable)
     * names.  For each pair, divide the other variable name into two
     * parts, then call MakeUpvar to do all the work of creating linking
     * it to the local variable.
     */

    for ( ; argc > 0; argc -= 2, wargv += 2) 
    {
	for (wp = wargv[0]; *wp != i18n_special_chars.w_esc_zero; wp++) 
	{
	    if (*wp == i18n_special_chars.w_op_paren) 
	    {
		wchar_t *wopen = wp;

		do {
		    wp++;
		} while (*wp != i18n_special_chars.w_esc_zero);
		wp--;
		if (*wp != i18n_special_chars.w_cl_paren) 
		{
		    goto scalar;
		}
		*wopen = i18n_special_chars.w_esc_zero;
		*wp = i18n_special_chars.w_esc_zero;
		result = _WMakeUpvar(iPtr, framePtr, wargv[0], wopen+1, wargv[1]);
		*wopen = i18n_special_chars.w_op_paren; 
		*wp = i18n_special_chars.w_cl_paren; 
		goto checkResult; 
	    } 
		
	} 
	scalar: 
	result = _WMakeUpvar(iPtr, framePtr, wargv[0], (wchar_t *) NULL, wargv[1]);

	checkResult:
	if (result != TCL_OK) 
	{
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}
