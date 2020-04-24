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
 * $Log: tclExpr.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.6.1  1995/02/16  22:00:18  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/16  20:40:13  levy]
 *
 * Revision 1.1.4.1  1994/10/21  21:38:59  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:31:07  mori_m]
 * 
 * Revision 1.1.2.1  1994/07/12  21:19:08  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:07:16  mori_m]
 * 
 * $EndLog$
 */
/* 
 * tclExpr.c --
 *
 *	This file contains the code to evaluate expressions for
 *	Tcl.
 *
 *	This implementation of floating-point support was modelled
 *	after an initial implementation by Bill Carpenter.
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
#ifdef NO_FLOAT_H
#   include "compat/float.h"
#else
#   include <float.h>
#endif
#ifndef TCL_NO_MATH
#include <math.h>
#endif
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 * The stuff below is a bit of a hack so that this file can be used
 * in environments that include no UNIX, i.e. no errno.  Just define
 * errno here.
 */

#ifndef TCL_GENERIC_ONLY
#include "tclUnix.h"
extern int errno;
#else
#define NO_ERRNO_H
#endif

#ifdef NO_ERRNO_H
int errno;
#define EDOM 33
#define ERANGE 34
#endif

/*
 * The data structure below is used to describe an expression value,
 * which can be either an integer (the usual case), a double-precision
 * floating-point value, or a string.  A given number has only one
 * value at a time.
 */

#define STATIC_STRING_SPACE 150

typedef struct {
    long intValue;		/* Integer value, if any. */
    double  doubleValue;	/* Floating-point value, if any. */
    ParseValue pv;		/* Used to hold a string value, if any. */
    char staticSpace[STATIC_STRING_SPACE];
				/* Storage for small strings;  large ones
				 * are malloc-ed. */
    int type;			/* Type of value:  TYPE_INT, TYPE_DOUBLE,
				 * or TYPE_STRING. */
} Value;

typedef struct {
    long intValue;		/* Integer value, if any. */
    double  doubleValue;	/* Floating-point value, if any. */
    _WParseValue wpv;		/* Used to hold a wide character string value, 
				 * if any. */
    wchar_t wstaticSpace[STATIC_STRING_SPACE];
				/* Storage for small strings;  large ones
				 * are malloc-ed. */
    int type;			/* Type of value:  TYPE_INT, TYPE_DOUBLE,
				 * or TYPE_STRING. */
} _WValue;

/*
 * Valid values for type:
 */

#define TYPE_INT	0
#define TYPE_DOUBLE	1
#define TYPE_STRING	2

/*
 * The data structure below describes the state of parsing an expression.
 * It's passed among the routines in this module.
 */

typedef struct {
    char *originalExpr;		/* The entire expression, as originally
				 * passed to Tcl_ExprString et al. */
    char *expr;			/* Position to the next character to be
				 * scanned from the expression string. */
    int token;			/* Type of the last token to be parsed from
				 * expr.  See below for definitions.
				 * Corresponds to the characters just
				 * before expr. */
} ExprInfo;

typedef struct {
    wchar_t *woriginalExpr;	/* The entire expression, as originally
				 * passed to Tcl_ExprString et al. */
    wchar_t *wexpr;		/* Position to the next character to be
				 * scanned from the expression string. */
    int token;			/* Type of the last token to be parsed from
				 * expr.  See below for definitions.
				 * Corresponds to the characters just
				 * before expr. */
} _WExprInfo;

/*
 * The token types are defined below.  In addition, there is a table
 * associating a precedence with each operator.  The order of types
 * is important.  Consult the code before changing it.
 */

#define VALUE		0
#define OPEN_PAREN	1
#define CLOSE_PAREN	2
#define COMMA		3
#define END		4
#define UNKNOWN		5

/*
 * Binary operators:
 */

#define MULT		8
#define DIVIDE		9
#define MOD		10
#define PLUS		11
#define MINUS		12
#define LEFT_SHIFT	13
#define RIGHT_SHIFT	14
#define LESS		15
#define GREATER		16
#define LEQ		17
#define GEQ		18
#define EQUAL		19
#define NEQ		20
#define BIT_AND		21
#define BIT_XOR		22
#define BIT_OR		23
#define AND		24
#define OR		25
#define QUESTY		26
#define COLON		27

/*
 * Unary operators:
 */

#define	UNARY_MINUS	28
#define NOT		29
#define BIT_NOT		30

/*
 * Precedence table.  The values for non-operator token types are ignored.
 */

int precTable[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    11, 11, 11,				/* MULT, DIVIDE, MOD */
    10, 10,				/* PLUS, MINUS */
    9, 9,				/* LEFT_SHIFT, RIGHT_SHIFT */
    8, 8, 8, 8,				/* LESS, GREATER, LEQ, GEQ */
    7, 7,				/* EQUAL, NEQ */
    6,					/* BIT_AND */
    5,					/* BIT_XOR */
    4,					/* BIT_OR */
    3,					/* AND */
    2,					/* OR */
    1, 1,				/* QUESTY, COLON */
    12, 12, 12				/* UNARY_MINUS, NOT, BIT_NOT */
};

/*
 * Mapping from operator numbers to strings;  used for error messages.
 */

char *operatorStrings[] = {
    "VALUE", "(", ")", "END", "UNKNOWN", "5", "6", "7",
    "*", "/", "%", "+", "-", "<<", ">>", "<", ">", "<=",
    ">=", "==", "!=", "&", "^", "|", "&&", "||", "?", ":",
    "-", "!", "~"
};

/*
 * The following slight modification to DBL_MAX is needed because of
 * a compiler bug on Sprite (4/15/93).
 */

#ifdef sprite
#undef DBL_MAX
#define DBL_MAX 1.797693134862316e+307
#endif

/*
 * Macros for testing floating-point values for certain special
 * cases.  Test for not-a-number by comparing a value against
 * itself;  test for infinity by comparing against the largest
 * floating-point value.
 */

#define IS_NAN(v) ((v) != (v))
#ifdef DBL_MAX
#   define IS_INF(v) (((v) > DBL_MAX) || ((v) < -DBL_MAX))
#else
#   define IS_INF(v) 0
#endif

/*
 * The following global variable is use to signal matherr that Tcl
 * is responsible for the arithmetic, so errors can be handled in a
 * fashion appropriate for Tcl.  Zero means no Tcl math is in
 * progress;  non-zero means Tcl is doing math.
 */

int tcl_MathInProgress = 0;

/*
 * The variable below serves no useful purpose except to generate
 * a reference to matherr, so that the Tcl version of matherr is
 * linked in rather than the system version.  Without this reference
 * the need for matherr won't be discovered during linking until after
 * libtcl.a has been processed, so Tcl's version won't be used.
 */

#ifdef NEED_MATHERR
extern int matherr();
int (*tclMatherrPtr)() = matherr;
#endif

/*
 * Declarations for local procedures to this file:
 */

static int		ExprAbsFunc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tcl_Value *args,
			    Tcl_Value *resultPtr));
static int		ExprBinaryFunc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tcl_Value *args,
			    Tcl_Value *resultPtr));
static int		ExprDoubleFunc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tcl_Value *args,
			    Tcl_Value *resultPtr));
static void		ExprFloatError _ANSI_ARGS_((Tcl_Interp *interp,
			    double value));
static int		ExprGetValue _ANSI_ARGS_((Tcl_Interp *interp,
			    ExprInfo *infoPtr, int prec, Value *valuePtr));
static int		_WExprGetValue _ANSI_ARGS_((Tcl_Interp *interp,
			    _WExprInfo *infoPtr, int prec, _WValue *valuePtr));
static int		ExprIntFunc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tcl_Value *args,
			    Tcl_Value *resultPtr));
static int		ExprLex _ANSI_ARGS_((Tcl_Interp *interp,
			    ExprInfo *infoPtr, Value *valuePtr));
static int		_WExprLex _ANSI_ARGS_((Tcl_Interp *interp,
			    _WExprInfo *infoPtr, _WValue *valuePtr));
static void		ExprMakeString _ANSI_ARGS_((Tcl_Interp *interp,
			    Value *valuePtr));
static int		_WExprMakeString _ANSI_ARGS_((Tcl_Interp *interp,
			    _WValue *valuePtr));
static int		ExprMathFunc _ANSI_ARGS_((Tcl_Interp *interp,
			    ExprInfo *infoPtr, Value *valuePtr));
static int		_WExprMathFunc _ANSI_ARGS_((Tcl_Interp *interp,
			    _WExprInfo *infoPtr, _WValue *valuePtr));
static int		ExprParseString _ANSI_ARGS_((Tcl_Interp *interp,
			    char *string, Value *valuePtr));
static int		_WExprParseString _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *string, _WValue *valuePtr));
static int		ExprRoundFunc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tcl_Value *args,
			    Tcl_Value *resultPtr));
static int		ExprTopLevel _ANSI_ARGS_((Tcl_Interp *interp,
			    char *string, Value *valuePtr));
static int		_WExprTopLevel _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, _WValue *valuePtr));
static int		ExprUnaryFunc _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, Tcl_Value *args,
			    Tcl_Value *resultPtr));

/*
 * Built-in math functions:
 */

typedef struct {
    char *name;			/* Name of function. */
    int numArgs;		/* Number of arguments for function. */
    Tcl_ValueType argTypes[MAX_MATH_ARGS];
				/* Acceptable types for each argument. */
    Tcl_MathProc *proc;		/* Procedure that implements this function. */
    ClientData clientData;	/* Additional argument to pass to the function
				 * when invoking it. */
} BuiltinFunc;

static BuiltinFunc funcTable[] = {
#ifndef TCL_NO_MATH
    {"acos", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) acos},
    {"asin", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) asin},
    {"atan", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) atan},
    {"atan2", 2, {TCL_DOUBLE, TCL_DOUBLE}, ExprBinaryFunc, (ClientData) atan2},
    {"ceil", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) ceil},
    {"cos", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) cos},
    {"cosh", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) cosh},
    {"exp", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) exp},
    {"floor", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) floor},
    {"fmod", 2, {TCL_DOUBLE, TCL_DOUBLE}, ExprBinaryFunc, (ClientData) fmod},
    {"hypot", 2, {TCL_DOUBLE, TCL_DOUBLE}, ExprBinaryFunc, (ClientData) hypot},
    {"log", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) log},
    {"log10", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) log10},
    {"pow", 2, {TCL_DOUBLE, TCL_DOUBLE}, ExprBinaryFunc, (ClientData) pow},
    {"sin", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) sin},
    {"sinh", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) sinh},
    {"sqrt", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) sqrt},
    {"tan", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) tan},
    {"tanh", 1, {TCL_DOUBLE}, ExprUnaryFunc, (ClientData) tanh},
#endif
    {"abs", 1, {TCL_EITHER}, ExprAbsFunc, 0},
    {"double", 1, {TCL_EITHER}, ExprDoubleFunc, 0},
    {"int", 1, {TCL_EITHER}, ExprIntFunc, 0},
    {"round", 1, {TCL_EITHER}, ExprRoundFunc, 0},

    {0},
};

/*
 *--------------------------------------------------------------
 *
 * ExprParseString --
 *
 *	Given a string (such as one coming from command or variable
 *	substitution), make a Value based on the string.  The value
 *	will be a floating-point or integer, if possible, or else it
 *	will just be a copy of the string.
 *
 * Results:
 *	TCL_OK is returned under normal circumstances, and TCL_ERROR
 *	is returned if a floating-point overflow or underflow occurred
 *	while reading in a number.  The value at *valuePtr is modified
 *	to hold a number, if possible.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
ExprParseString(interp, string, valuePtr)
    Tcl_Interp *interp;		/* Where to store error message. */
    char *string;		/* String to turn into value. */
    Value *valuePtr;		/* Where to store value information. 
				 * Caller must have initialized pv field. */
{
    char *term, *p, *start;

    if (*string != 0) {
	valuePtr->type = TYPE_INT;
	errno = 0;

	/*
	 * Note: use strtoul instead of strtol for integer conversions
	 * to allow full-size unsigned numbers, but don't depend on
	 * strtoul to handle sign characters;  it won't in some
	 * implementations.
	 */

	for (p = string; isspace(UCHAR(*p)); p++) {
	    /* Empty loop body. */
	}
	if (*p == '-') {
	    start = p+1;
	    valuePtr->intValue = -strtoul(start, &term, 0);
	} else if (*p == '+') {
	    start = p+1;
	    valuePtr->intValue = strtoul(start, &term, 0);
	} else {
	    start = p;
	    valuePtr->intValue = strtoul(start, &term, 0);
	}
	if (errno == ERANGE) {
	    /*
	     * This procedure is sometimes called with string in
	     * interp->result, so we have to clear the result before
	     * logging an error message.
	     */

	    Tcl_ResetResult(interp);
	    /* i18n
	    interp->result = "integer value too large to represent";
	     */
	    interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW", interp->result,
		    (char *) NULL);
	    return TCL_ERROR;
	}
	if ((term != start) && (*term == '\0')) {
	    return TCL_OK;
	}
	errno = 0;
	valuePtr->doubleValue = strtod(p, &term);
	if ((term != p) && (*term == '\0')) {
	    if (errno != 0) {
		Tcl_ResetResult(interp);
		ExprFloatError(interp, valuePtr->doubleValue);
		return TCL_ERROR;
	    }
	    valuePtr->type = TYPE_DOUBLE;
	    return TCL_OK;
	}
    }

    /*
     * Not a valid number.  Save a string value (but don't do anything
     * if it's already the value).
     */

    valuePtr->type = TYPE_STRING;
    if (string != valuePtr->pv.buffer) {
	int length, shortfall;

	length = strlen(string);
	valuePtr->pv.next = valuePtr->pv.buffer;
	shortfall = length - (valuePtr->pv.end - valuePtr->pv.buffer);
	if (shortfall > 0) {
	    (*valuePtr->pv.expandProc)(&valuePtr->pv, shortfall);
	}
	strcpy(valuePtr->pv.buffer, string);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * ExprLex --
 *
 *	Lexical analyzer for expression parser:  parses a single value,
 *	operator, or other syntactic element from an expression string.
 *
 * Results:
 *	TCL_OK is returned unless an error occurred while doing lexical
 *	analysis or executing an embedded command.  In that case a
 *	standard Tcl error is returned, using interp->result to hold
 *	an error message.  In the event of a successful return, the token
 *	and field in infoPtr is updated to refer to the next symbol in
 *	the expression string, and the expr field is advanced past that
 *	token;  if the token is a value, then the value is stored at
 *	valuePtr.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
ExprLex(interp, infoPtr, valuePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    register ExprInfo *infoPtr;		/* Describes the state of the parse. */
    register Value *valuePtr;		/* Where to store value, if that is
					 * what's parsed from string.  Caller
					 * must have initialized pv field
					 * correctly. */
{
    register char *p;
    char *var, *term;
    int result;

    p = infoPtr->expr;
    while (isspace(UCHAR(*p))) {
	p++;
    }
    if (*p == 0) {
	infoPtr->token = END;
	infoPtr->expr = p;
	return TCL_OK;
    }

    /*
     * First try to parse the token as an integer or floating-point number.
     * A couple of tricky points:
     *
     * 1. Can't just check for leading digits to see if there's a number
     *    there, because it could be a special value like "NaN".
     * 2. Don't want to check for a number if the first character is "+"
     *    or "-".  If we do, we might treat a binary operator as unary
     *    by mistake, which will eventually cause a syntax error.
     * 3. First see if there's an integer, then if there's stuff after
     *    the integer that looks like it could be a floating-point number
     *    (or if there wasn't even a sensible integer), then try to parse
     *    as a floating-point number.  The check for the characters '8'
     *	  or '9' is to handle floating-point numbers like 028.6:  the
     *    leading zero causes strtoul to interpret the number as octal
     *    and stop when it gets to the 8.
     */

    if ((*p != '+')  && (*p != '-')) {
	errno = 0;
	valuePtr->intValue = strtoul(p, &term, 0);
	if ((term == p) || (*term == '.') || (*term == 'e') ||
		(*term == 'E') || (*term == '8') || (*term == '9')) {
	    char *term2;
    
	    /*
	     * The code here is a bit tricky:  we want to use a floating-point
	     * number if there is one, but if there isn't then fall through to
	     * use the integer that was already parsed, if there was one.
	     */
    
	    errno = 0;
	    valuePtr->doubleValue = strtod(p, &term2);
	    if (term2 != p) {
		if (errno != 0) {
		    ExprFloatError(interp, valuePtr->doubleValue);
		    return TCL_ERROR;
		}
		infoPtr->token = VALUE;
		infoPtr->expr = term2;
		valuePtr->type = TYPE_DOUBLE;
		return TCL_OK;
	    }
	    if (term != p) {
		/* i18n
		interp->result = "poorly-formed floating-point value";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_poor_flat_val_form);
		interp->freeProc = (Tcl_FreeProc *) free;

		return TCL_ERROR;
	    }
	}
	if (term != p) {
	    /*
	     * No floating-point number, but there is an integer.
	     */
    
	    if (errno == ERANGE) {
		/* i18n
		interp->result = "integer value too large to represent";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
		interp->freeProc = (Tcl_FreeProc *) free;

		Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW", interp->result,
			(char *) NULL);
		return TCL_ERROR;
	    }
	    infoPtr->token = VALUE;
	    infoPtr->expr = term;
	    valuePtr->type = TYPE_INT;
	    return TCL_OK;
	}
    }

    infoPtr->expr = p+1;
    switch (*p) {
	case '$':

	    /*
	     * Variable.  Fetch its value, then see if it makes sense
	     * as an integer or floating-point number.
	     */

	    infoPtr->token = VALUE;
	    var = Tcl_ParseVar(interp, p, &infoPtr->expr);
	    if (var == NULL) {
		return TCL_ERROR;
	    }
	    Tcl_ResetResult(interp);
	    if (((Interp *) interp)->noEval) {
		valuePtr->type = TYPE_INT;
		valuePtr->intValue = 0;
		return TCL_OK;
	    }
	    return ExprParseString(interp, var, valuePtr);

	case '[':
	    infoPtr->token = VALUE;
	    ((Interp *) interp)->evalFlags = TCL_BRACKET_TERM;
	    result = Tcl_Eval(interp, p+1);
	    infoPtr->expr = ((Interp *) interp)->termPtr;
	    if (result != TCL_OK) {
		return result;
	    }
	    infoPtr->expr++;
	    if (((Interp *) interp)->noEval) {
		valuePtr->type = TYPE_INT;
		valuePtr->intValue = 0;
		Tcl_ResetResult(interp);
		return TCL_OK;
	    }
	    result = ExprParseString(interp, interp->result, valuePtr);
	    if (result != TCL_OK) {
		return result;
	    }
	    Tcl_ResetResult(interp);
	    return TCL_OK;

	case '"':
	    infoPtr->token = VALUE;
	    result = TclParseQuotes(interp, infoPtr->expr, '"', 0,
		    &infoPtr->expr, &valuePtr->pv);
	    if (result != TCL_OK) {
		return result;
	    }
	    Tcl_ResetResult(interp);
	    return ExprParseString(interp, valuePtr->pv.buffer, valuePtr);

	case '{':
	    infoPtr->token = VALUE;
	    result = TclParseBraces(interp, infoPtr->expr, &infoPtr->expr,
		    &valuePtr->pv);
	    if (result != TCL_OK) {
		return result;
	    }
	    Tcl_ResetResult(interp);
	    return ExprParseString(interp, valuePtr->pv.buffer, valuePtr);

	case '(':
	    infoPtr->token = OPEN_PAREN;
	    return TCL_OK;

	case ')':
	    infoPtr->token = CLOSE_PAREN;
	    return TCL_OK;

	case ',':
	    infoPtr->token = COMMA;
	    return TCL_OK;

	case '*':
	    infoPtr->token = MULT;
	    return TCL_OK;

	case '/':
	    infoPtr->token = DIVIDE;
	    return TCL_OK;

	case '%':
	    infoPtr->token = MOD;
	    return TCL_OK;

	case '+':
	    infoPtr->token = PLUS;
	    return TCL_OK;

	case '-':
	    infoPtr->token = MINUS;
	    return TCL_OK;

	case '?':
	    infoPtr->token = QUESTY;
	    return TCL_OK;

	case ':':
	    infoPtr->token = COLON;
	    return TCL_OK;

	case '<':
	    switch (p[1]) {
		case '<':
		    infoPtr->expr = p+2;
		    infoPtr->token = LEFT_SHIFT;
		    break;
		case '=':
		    infoPtr->expr = p+2;
		    infoPtr->token = LEQ;
		    break;
		default:
		    infoPtr->token = LESS;
		    break;
	    }
	    return TCL_OK;

	case '>':
	    switch (p[1]) {
		case '>':
		    infoPtr->expr = p+2;
		    infoPtr->token = RIGHT_SHIFT;
		    break;
		case '=':
		    infoPtr->expr = p+2;
		    infoPtr->token = GEQ;
		    break;
		default:
		    infoPtr->token = GREATER;
		    break;
	    }
	    return TCL_OK;

	case '=':
	    if (p[1] == '=') {
		infoPtr->expr = p+2;
		infoPtr->token = EQUAL;
	    } else {
		infoPtr->token = UNKNOWN;
	    }
	    return TCL_OK;

	case '!':
	    if (p[1] == '=') {
		infoPtr->expr = p+2;
		infoPtr->token = NEQ;
	    } else {
		infoPtr->token = NOT;
	    }
	    return TCL_OK;

	case '&':
	    if (p[1] == '&') {
		infoPtr->expr = p+2;
		infoPtr->token = AND;
	    } else {
		infoPtr->token = BIT_AND;
	    }
	    return TCL_OK;

	case '^':
	    infoPtr->token = BIT_XOR;
	    return TCL_OK;

	case '|':
	    if (p[1] == '|') {
		infoPtr->expr = p+2;
		infoPtr->token = OR;
	    } else {
		infoPtr->token = BIT_OR;
	    }
	    return TCL_OK;

	case '~':
	    infoPtr->token = BIT_NOT;
	    return TCL_OK;

	default:
	    if (isalpha(UCHAR(*p))) {
		infoPtr->expr = p;
		return ExprMathFunc(interp, infoPtr, valuePtr);
	    }
	    infoPtr->expr = p+1;
	    infoPtr->token = UNKNOWN;
	    return TCL_OK;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * ExprGetValue --
 *
 *	Parse a "value" from the remainder of the expression in infoPtr.
 *
 * Results:
 *	Normally TCL_OK is returned.  The value of the expression is
 *	returned in *valuePtr.  If an error occurred, then interp->result
 *	contains an error message and TCL_ERROR is returned.
 *	InfoPtr->token will be left pointing to the token AFTER the
 *	expression, and infoPtr->expr will point to the character just
 *	after the terminating token.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
ExprGetValue(interp, infoPtr, prec, valuePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    register ExprInfo *infoPtr;		/* Describes the state of the parse
					 * just before the value (i.e. ExprLex
					 * will be called to get first token
					 * of value). */
    int prec;				/* Treat any un-parenthesized operator
					 * with precedence <= this as the end
					 * of the expression. */
    Value *valuePtr;			/* Where to store the value of the
					 * expression.   Caller must have
					 * initialized pv field. */
{
    Interp *iPtr = (Interp *) interp;
    Value value2;			/* Second operand for current
					 * operator.  */
    int operator;			/* Current operator (either unary
					 * or binary). */
    int badType;			/* Type of offending argument;  used
					 * for error messages. */
    int gotOp;				/* Non-zero means already lexed the
					 * operator (while picking up value
					 * for unary operator).  Don't lex
					 * again. */
    int result;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * There are two phases to this procedure.  First, pick off an initial
     * value.  Then, parse (binary operator, value) pairs until done.
     */

    gotOp = 0;
    value2.pv.buffer = value2.pv.next = value2.staticSpace;
    value2.pv.end = value2.pv.buffer + STATIC_STRING_SPACE - 1;
    value2.pv.expandProc = TclExpandParseValue;
    value2.pv.clientData = (ClientData) NULL;
    result = ExprLex(interp, infoPtr, valuePtr);
    if (result != TCL_OK) {
	goto done;
    }
    if (infoPtr->token == OPEN_PAREN) {

	/*
	 * Parenthesized sub-expression.
	 */

	result = ExprGetValue(interp, infoPtr, -1, valuePtr);
	if (result != TCL_OK) {
	    goto done;
	}
	if (infoPtr->token != CLOSE_PAREN) {
	    /* i18n
	    Tcl_AppendResult(interp, "unmatched parentheses in expression \"",
		    infoPtr->originalExpr, "\"", (char *) NULL);
	     */
	    i18n_msg_format = dce_msg_get(tcl_s_expr_unmatch_parens_in_expr);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(infoPtr->originalExpr)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, infoPtr->originalExpr);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);

	    result = TCL_ERROR;
	    goto done;
	}
    } else {
	if (infoPtr->token == MINUS) {
	    infoPtr->token = UNARY_MINUS;
	}
	if (infoPtr->token >= UNARY_MINUS) {

	    /*
	     * Process unary operators.
	     */

	    operator = infoPtr->token;
	    result = ExprGetValue(interp, infoPtr, precTable[infoPtr->token],
		    valuePtr);
	    if (result != TCL_OK) {
		goto done;
	    }
	    switch (operator) {
		case UNARY_MINUS:
		    if (valuePtr->type == TYPE_INT) {
			valuePtr->intValue = -valuePtr->intValue;
		    } else if (valuePtr->type == TYPE_DOUBLE){
			valuePtr->doubleValue = -valuePtr->doubleValue;
		    } else {
			badType = valuePtr->type;
			goto illegalType;
		    } 
		    break;
		case NOT:
		    if (valuePtr->type == TYPE_INT) {
			valuePtr->intValue = !valuePtr->intValue;
		    } else if (valuePtr->type == TYPE_DOUBLE) {
			/*
			 * Theoretically, should be able to use
			 * "!valuePtr->intValue", but apparently some
			 * compilers can't handle it.
			 */
			if (valuePtr->doubleValue == 0.0) {
			    valuePtr->intValue = 1;
			} else {
			    valuePtr->intValue = 0;
			}
			valuePtr->type = TYPE_INT;
		    } else {
			badType = valuePtr->type;
			goto illegalType;
		    }
		    break;
		case BIT_NOT:
		    if (valuePtr->type == TYPE_INT) {
			valuePtr->intValue = ~valuePtr->intValue;
		    } else {
			badType  = valuePtr->type;
			goto illegalType;
		    }
		    break;
	    }
	    gotOp = 1;
	} else if (infoPtr->token != VALUE) {
	    goto syntaxError;
	}
    }

    /*
     * Got the first operand.  Now fetch (operator, operand) pairs.
     */

    if (!gotOp) {
	result = ExprLex(interp, infoPtr, &value2);
	if (result != TCL_OK) {
	    goto done;
	}
    }
    while (1) {
	operator = infoPtr->token;
	value2.pv.next = value2.pv.buffer;
	if ((operator < MULT) || (operator >= UNARY_MINUS)) {
	    if ((operator == END) || (operator == CLOSE_PAREN)
		    || (operator == COMMA)) {
		result = TCL_OK;
		goto done;
	    } else {
		goto syntaxError;
	    }
	}
	if (precTable[operator] <= prec) {
	    result = TCL_OK;
	    goto done;
	}

	/*
	 * If we're doing an AND or OR and the first operand already
	 * determines the result, don't execute anything in the
	 * second operand:  just parse.  Same style for ?: pairs.
	 */

	if ((operator == AND) || (operator == OR) || (operator == QUESTY)) {
	    if (valuePtr->type == TYPE_DOUBLE) {
		valuePtr->intValue = valuePtr->doubleValue != 0;
		valuePtr->type = TYPE_INT;
	    } else if (valuePtr->type == TYPE_STRING) {
		badType = TYPE_STRING;
		goto illegalType;
	    }
	    if (((operator == AND) && !valuePtr->intValue)
		    || ((operator == OR) && valuePtr->intValue)) {
		iPtr->noEval++;
		result = ExprGetValue(interp, infoPtr, precTable[operator],
			&value2);
		iPtr->noEval--;
	    } else if (operator == QUESTY) {
		if (valuePtr->intValue != 0) {
		    valuePtr->pv.next = valuePtr->pv.buffer;
		    result = ExprGetValue(interp, infoPtr, precTable[operator],
			    valuePtr);
		    if (result != TCL_OK) {
			goto done;
		    }
		    if (infoPtr->token != COLON) {
			goto syntaxError;
		    }
		    value2.pv.next = value2.pv.buffer;
		    iPtr->noEval++;
		    result = ExprGetValue(interp, infoPtr, precTable[operator],
			    &value2);
		    iPtr->noEval--;
		} else {
		    iPtr->noEval++;
		    result = ExprGetValue(interp, infoPtr, precTable[operator],
			    &value2);
		    iPtr->noEval--;
		    if (result != TCL_OK) {
			goto done;
		    }
		    if (infoPtr->token != COLON) {
			goto syntaxError;
		    }
		    valuePtr->pv.next = valuePtr->pv.buffer;
		    result = ExprGetValue(interp, infoPtr, precTable[operator],
			    valuePtr);
		}
	    } else {
		result = ExprGetValue(interp, infoPtr, precTable[operator],
			&value2);
	    }
	} else {
	    result = ExprGetValue(interp, infoPtr, precTable[operator],
		    &value2);
	}
	if (result != TCL_OK) {
	    goto done;
	}
	if ((infoPtr->token < MULT) && (infoPtr->token != VALUE)
		&& (infoPtr->token != END) && (infoPtr->token != COMMA)
		&& (infoPtr->token != CLOSE_PAREN)) {
	    goto syntaxError;
	}

	/*
	 * At this point we've got two values and an operator.  Check
	 * to make sure that the particular data types are appropriate
	 * for the particular operator, and perform type conversion
	 * if necessary.
	 */

	switch (operator) {

	    /*
	     * For the operators below, no strings are allowed and
	     * ints get converted to floats if necessary.
	     */

	    case MULT: case DIVIDE: case PLUS: case MINUS:
		if ((valuePtr->type == TYPE_STRING)
			|| (value2.type == TYPE_STRING)) {
		    badType = TYPE_STRING;
		    goto illegalType;
		}
		if (valuePtr->type == TYPE_DOUBLE) {
		    if (value2.type == TYPE_INT) {
			value2.doubleValue = value2.intValue;
			value2.type = TYPE_DOUBLE;
		    }
		} else if (value2.type == TYPE_DOUBLE) {
		    if (valuePtr->type == TYPE_INT) {
			valuePtr->doubleValue = valuePtr->intValue;
			valuePtr->type = TYPE_DOUBLE;
		    }
		}
		break;

	    /*
	     * For the operators below, only integers are allowed.
	     */

	    case MOD: case LEFT_SHIFT: case RIGHT_SHIFT:
	    case BIT_AND: case BIT_XOR: case BIT_OR:
		 if (valuePtr->type != TYPE_INT) {
		     badType = valuePtr->type;
		     goto illegalType;
		 } else if (value2.type != TYPE_INT) {
		     badType = value2.type;
		     goto illegalType;
		 }
		 break;

	    /*
	     * For the operators below, any type is allowed but the
	     * two operands must have the same type.  Convert integers
	     * to floats and either to strings, if necessary.
	     */

	    case LESS: case GREATER: case LEQ: case GEQ:
	    case EQUAL: case NEQ:
		if (valuePtr->type == TYPE_STRING) {
		    if (value2.type != TYPE_STRING) {
			ExprMakeString(interp, &value2);
		    }
		} else if (value2.type == TYPE_STRING) {
		    if (valuePtr->type != TYPE_STRING) {
			ExprMakeString(interp, valuePtr);
		    }
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    if (value2.type == TYPE_INT) {
			value2.doubleValue = value2.intValue;
			value2.type = TYPE_DOUBLE;
		    }
		} else if (value2.type == TYPE_DOUBLE) {
		     if (valuePtr->type == TYPE_INT) {
			valuePtr->doubleValue = valuePtr->intValue;
			valuePtr->type = TYPE_DOUBLE;
		    }
		}
		break;

	    /*
	     * For the operators below, no strings are allowed, but
	     * no int->double conversions are performed.
	     */

	    case AND: case OR:
		if (valuePtr->type == TYPE_STRING) {
		    badType = valuePtr->type;
		    goto illegalType;
		}
		if (value2.type == TYPE_STRING) {
		    badType = value2.type;
		    goto illegalType;
		}
		break;

	    /*
	     * For the operators below, type and conversions are
	     * irrelevant:  they're handled elsewhere.
	     */

	    case QUESTY: case COLON:
		break;

	    /*
	     * Any other operator is an error.
	     */

	    default:
		/* i18n
		interp->result = "unknown operator in expression";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_unknown_oper_in_expr);
		interp->freeProc = (Tcl_FreeProc *) free;

		result = TCL_ERROR;
		goto done;
	}

	/*
	 * If necessary, convert one of the operands to the type
	 * of the other.  If the operands are incompatible with
	 * the operator (e.g. "+" on strings) then return an
	 * error.
	 */

	switch (operator) {
	    case MULT:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue *= value2.intValue;
		} else {
		    valuePtr->doubleValue *= value2.doubleValue;
		}
		break;
	    case DIVIDE:
	    case MOD:
		if (valuePtr->type == TYPE_INT) {
		    int divisor, quot, rem, negative;
		    if (value2.intValue == 0) {
			divideByZero:
			/* i18n
			interp->result = "divide by zero";
			 */
			interp->result = (char *)dce_msg_get(tcl_s_expr_divide_by_zero);
			interp->freeProc = (Tcl_FreeProc *) free;

			Tcl_SetErrorCode(interp, "ARITH", "DIVZERO",
				interp->result, (char *) NULL);
			result = TCL_ERROR;
			goto done;
		    }

		    /*
		     * The code below is tricky because C doesn't guarantee
		     * much about the properties of the quotient or
		     * remainder, but Tcl does:  the remainder always has
		     * the same sign as the divisor and a smaller absolute
		     * value.
		     */

		    divisor = value2.intValue;
		    negative = 0;
		    if (divisor < 0) {
			divisor = -divisor;
			valuePtr->intValue = -valuePtr->intValue;
			negative = 1;
		    }
		    quot = valuePtr->intValue / divisor;
		    rem = valuePtr->intValue % divisor;
		    if (rem < 0) {
			rem += divisor;
			quot -= 1;
		    }
		    if (negative) {
			rem = -rem;
		    }
		    valuePtr->intValue = (operator == DIVIDE) ? quot : rem;
		} else {
		    if (value2.doubleValue == 0.0) {
			goto divideByZero;
		    }
		    valuePtr->doubleValue /= value2.doubleValue;
		}
		break;
	    case PLUS:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue += value2.intValue;
		} else {
		    valuePtr->doubleValue += value2.doubleValue;
		}
		break;
	    case MINUS:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue -= value2.intValue;
		} else {
		    valuePtr->doubleValue -= value2.doubleValue;
		}
		break;
	    case LEFT_SHIFT:
		valuePtr->intValue <<= value2.intValue;
		break;
	    case RIGHT_SHIFT:
		/*
		 * The following code is a bit tricky:  it ensures that
		 * right shifts propagate the sign bit even on machines
		 * where ">>" won't do it by default.
		 */

		if (valuePtr->intValue < 0) {
		    valuePtr->intValue =
			    ~((~valuePtr->intValue) >> value2.intValue);
		} else {
		    valuePtr->intValue >>= value2.intValue;
		}
		break;
	    case LESS:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue =
			valuePtr->intValue < value2.intValue;
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    valuePtr->intValue =
			valuePtr->doubleValue < value2.doubleValue;
		} else {
		    valuePtr->intValue =
			    strcmp(valuePtr->pv.buffer, value2.pv.buffer) < 0;
		}
		valuePtr->type = TYPE_INT;
		break;
	    case GREATER:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue =
			valuePtr->intValue > value2.intValue;
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    valuePtr->intValue =
			valuePtr->doubleValue > value2.doubleValue;
		} else {
		    valuePtr->intValue =
			    strcmp(valuePtr->pv.buffer, value2.pv.buffer) > 0;
		}
		valuePtr->type = TYPE_INT;
		break;
	    case LEQ:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue =
			valuePtr->intValue <= value2.intValue;
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    valuePtr->intValue =
			valuePtr->doubleValue <= value2.doubleValue;
		} else {
		    valuePtr->intValue =
			    strcmp(valuePtr->pv.buffer, value2.pv.buffer) <= 0;
		}
		valuePtr->type = TYPE_INT;
		break;
	    case GEQ:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue =
			valuePtr->intValue >= value2.intValue;
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    valuePtr->intValue =
			valuePtr->doubleValue >= value2.doubleValue;
		} else {
		    valuePtr->intValue =
			    strcmp(valuePtr->pv.buffer, value2.pv.buffer) >= 0;
		}
		valuePtr->type = TYPE_INT;
		break;
	    case EQUAL:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue =
			valuePtr->intValue == value2.intValue;
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    valuePtr->intValue =
			valuePtr->doubleValue == value2.doubleValue;
		} else {
		    valuePtr->intValue =
			    strcmp(valuePtr->pv.buffer, value2.pv.buffer) == 0;
		}
		valuePtr->type = TYPE_INT;
		break;
	    case NEQ:
		if (valuePtr->type == TYPE_INT) {
		    valuePtr->intValue =
			valuePtr->intValue != value2.intValue;
		} else if (valuePtr->type == TYPE_DOUBLE) {
		    valuePtr->intValue =
			valuePtr->doubleValue != value2.doubleValue;
		} else {
		    valuePtr->intValue =
			    strcmp(valuePtr->pv.buffer, value2.pv.buffer) != 0;
		}
		valuePtr->type = TYPE_INT;
		break;
	    case BIT_AND:
		valuePtr->intValue &= value2.intValue;
		break;
	    case BIT_XOR:
		valuePtr->intValue ^= value2.intValue;
		break;
	    case BIT_OR:
		valuePtr->intValue |= value2.intValue;
		break;

	    /*
	     * For AND and OR, we know that the first value has already
	     * been converted to an integer.  Thus we need only consider
	     * the possibility of int vs. double for the second value.
	     */

	    case AND:
		if (value2.type == TYPE_DOUBLE) {
		    value2.intValue = value2.doubleValue != 0;
		    value2.type = TYPE_INT;
		}
		valuePtr->intValue = valuePtr->intValue && value2.intValue;
		break;
	    case OR:
		if (value2.type == TYPE_DOUBLE) {
		    value2.intValue = value2.doubleValue != 0;
		    value2.type = TYPE_INT;
		}
		valuePtr->intValue = valuePtr->intValue || value2.intValue;
		break;

	    case COLON:
		/* i18n
		interp->result = "can't have : operator without ? first";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_colon_without_question);
		interp->freeProc = (Tcl_FreeProc *) free;

		result = TCL_ERROR;
		goto done;
	}
    }

    done:
    if (value2.pv.buffer != value2.staticSpace) {
	ckfree(value2.pv.buffer);
    }
    return result;

    syntaxError:
    /* i18n
    Tcl_AppendResult(interp, "syntax error in expression \"",
	    infoPtr->originalExpr, "\"", (char *) NULL);
     */
     i18n_msg_format = dce_msg_get(tcl_s_expr_syntax_err);
     i18n_msg_buf = (unsigned char *)ckalloc(strlen(infoPtr->originalExpr)
				+ strlen((char *)i18n_msg_format) + 1);
     sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, infoPtr->originalExpr);
     Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
     free(i18n_msg_format);
     ckfree(i18n_msg_buf);

    result = TCL_ERROR;
    goto done;

    illegalType:
    /* i18n
    Tcl_AppendResult(interp, "can't use ", (badType == TYPE_DOUBLE) ?
	    "floating-point value" : "non-numeric string",
	    " as operand of \"", operatorStrings[operator], "\"",
	    (char *) NULL);
     */
    if (badType == TYPE_DOUBLE)
    {
	i18n_msg_format = dce_msg_get(tcl_s_expr_float_non_numeric_as_op);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(operatorStrings[operator])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, operatorStrings[operator]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

    } else
    {
	i18n_msg_format = dce_msg_get(tcl_s_expr_non_numeric_non_numeric_as_op);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(operatorStrings[operator])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, operatorStrings[operator]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

    }
    result = TCL_ERROR;
    goto done;
}

/*
 *--------------------------------------------------------------
 *
 * ExprMakeString --
 *
 *	Convert a value from int or double representation to
 *	a string.
 *
 * Results:
 *	The information at *valuePtr gets converted to string
 *	format, if it wasn't that way already.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static void
ExprMakeString(interp, valuePtr)
    Tcl_Interp *interp;			/* Interpreter to use for precision
					 * information. */
    register Value *valuePtr;		/* Value to be converted. */
{
    int shortfall;

    shortfall = 150 - (valuePtr->pv.end - valuePtr->pv.buffer);
    if (shortfall > 0) {
	(*valuePtr->pv.expandProc)(&valuePtr->pv, shortfall);
    }
    if (valuePtr->type == TYPE_INT) {
	sprintf(valuePtr->pv.buffer, "%ld", valuePtr->intValue);
    } else if (valuePtr->type == TYPE_DOUBLE) {
	Tcl_PrintDouble(interp, valuePtr->doubleValue, valuePtr->pv.buffer);
    }
    valuePtr->type = TYPE_STRING;
}

/*
 *--------------------------------------------------------------
 *
 * ExprTopLevel --
 *
 *	This procedure provides top-level functionality shared by
 *	procedures like Tcl_ExprInt, Tcl_ExprDouble, etc.
 *
 * Results:
 *	The result is a standard Tcl return value.  If an error
 *	occurs then an error message is left in interp->result.
 *	The value of the expression is returned in *valuePtr, in
 *	whatever form it ends up in (could be string or integer
 *	or double).  Caller may need to convert result.  Caller
 *	is also responsible for freeing string memory in *valuePtr,
 *	if any was allocated.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
ExprTopLevel(interp, string, valuePtr)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    char *string;			/* Expression to evaluate. */
    Value *valuePtr;			/* Where to store result.  Should
					 * not be initialized by caller. */
{
    ExprInfo info;
    int result;
    unsigned char *i18n_msg_format;
    unsigned char *i18n_msg_buf;

    /*
     * Create the math functions the first time an expression is
     * evaluated.
     */

    if (!(((Interp *) interp)->flags & EXPR_INITIALIZED)) {
	BuiltinFunc *funcPtr;

	((Interp *) interp)->flags |= EXPR_INITIALIZED;
	for (funcPtr = funcTable; funcPtr->name != NULL;
		funcPtr++) {
	    Tcl_CreateMathFunc(interp, funcPtr->name, funcPtr->numArgs,
		    funcPtr->argTypes, funcPtr->proc, funcPtr->clientData);
	}
    }

    info.originalExpr = string;
    info.expr = string;
    valuePtr->pv.buffer = valuePtr->pv.next = valuePtr->staticSpace;
    valuePtr->pv.end = valuePtr->pv.buffer + STATIC_STRING_SPACE - 1;
    valuePtr->pv.expandProc = TclExpandParseValue;
    valuePtr->pv.clientData = (ClientData) NULL;

    result = ExprGetValue(interp, &info, -1, valuePtr);
    if (result != TCL_OK) {
	return result;
    }
    if (info.token != END) {
	/* i18n
	Tcl_AppendResult(interp, "syntax error in expression \"",
		string, "\"", (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_expr_syntax_err);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(string)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, string);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    if ((valuePtr->type == TYPE_DOUBLE) && (IS_NAN(valuePtr->doubleValue)
	    || IS_INF(valuePtr->doubleValue))) {
	/*
	 * IEEE floating-point error.
	 */

	ExprFloatError(interp, valuePtr->doubleValue);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * Tcl_ExprLong, Tcl_ExprDouble, Tcl_ExprBoolean --
 *
 *	Procedures to evaluate an expression and return its value
 *	in a particular form.
 *
 * Results:
 *	Each of the procedures below returns a standard Tcl result.
 *	If an error occurs then an error message is left in
 *	interp->result.  Otherwise the value of the expression,
 *	in the appropriate form, is stored at *resultPtr.  If
 *	the expression had a result that was incompatible with the
 *	desired form then an error is returned.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

int
Tcl_ExprLong(interp, string, ptr)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    char *string;			/* Expression to evaluate. */
    long *ptr;				/* Where to store result. */
{
    Value value;
    int result;

    result = ExprTopLevel(interp, string, &value);
    if (result == TCL_OK) {
	if (value.type == TYPE_INT) {
	    *ptr = value.intValue;
	} else if (value.type == TYPE_DOUBLE) {
	    *ptr = value.doubleValue;
	} else {
	    /* i18n
	    interp->result = "expression didn't have numeric value";
	     */
	    interp->result = (char *)dce_msg_get(tcl_s_expr_non_numeric_exp);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    result = TCL_ERROR;
	}
    }
    if (value.pv.buffer != value.staticSpace) {
	ckfree(value.pv.buffer);
    }
    return result;
}

int
Tcl_ExprDouble(interp, string, ptr)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    char *string;			/* Expression to evaluate. */
    double *ptr;			/* Where to store result. */
{
    Value value;
    int result;

    result = ExprTopLevel(interp, string, &value);
    if (result == TCL_OK) {
	if (value.type == TYPE_INT) {
	    *ptr = value.intValue;
	} else if (value.type == TYPE_DOUBLE) {
	    *ptr = value.doubleValue;
	} else {
	    /* i18n
	    interp->result = "expression didn't have numeric value";
	     */
	    interp->result = (char *)dce_msg_get(tcl_s_expr_non_numeric_exp);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    result = TCL_ERROR;
	}
    }
    if (value.pv.buffer != value.staticSpace) {
	ckfree(value.pv.buffer);
    }
    return result;
}

int
Tcl_ExprBoolean(interp, string, ptr)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    char *string;			/* Expression to evaluate. */
    int *ptr;				/* Where to store 0/1 result. */
{
    Value value;
    int result;

    result = ExprTopLevel(interp, string, &value);
    if (result == TCL_OK) {
	if (value.type == TYPE_INT) {
	    *ptr = value.intValue != 0;
	} else if (value.type == TYPE_DOUBLE) {
	    *ptr = value.doubleValue != 0.0;
	} else {
	    result = Tcl_GetBoolean(interp, value.pv.buffer, ptr);
	}
    }
    if (value.pv.buffer != value.staticSpace) {
	ckfree(value.pv.buffer);
    }
    return result;
}

/*
 *--------------------------------------------------------------
 *
 * Tcl_ExprString --
 *
 *	Evaluate an expression and return its value in string form.
 *
 * Results:
 *	A standard Tcl result.  If the result is TCL_OK, then the
 *	interpreter's result is set to the string value of the
 *	expression.  If the result is TCL_OK, then interp->result
 *	contains an error message.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

int
Tcl_ExprString(interp, string)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    char *string;			/* Expression to evaluate. */
{
    Value value;
    int result;

    result = ExprTopLevel(interp, string, &value);
    if (result == TCL_OK) {
	if (value.type == TYPE_INT) {
	    sprintf(interp->result, "%ld", value.intValue);
	} else if (value.type == TYPE_DOUBLE) {
	    Tcl_PrintDouble(interp, value.doubleValue, interp->result);
	} else {
	    if (value.pv.buffer != value.staticSpace) {
		interp->result = value.pv.buffer;
		interp->freeProc = (Tcl_FreeProc *) free;
		value.pv.buffer = value.staticSpace;
	    } else {
		Tcl_SetResult(interp, value.pv.buffer, TCL_VOLATILE);
	    }
	}
    }
    if (value.pv.buffer != value.staticSpace) {
	ckfree(value.pv.buffer);
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_CreateMathFunc --
 *
 *	Creates a new math function for expressions in a given
 *	interpreter.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The function defined by "name" is created;  if such a function
 *	already existed then its definition is overriden.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_CreateMathFunc(interp, name, numArgs, argTypes, proc, clientData)
    Tcl_Interp *interp;			/* Interpreter in which function is
					 * to be available. */
    char *name;				/* Name of function (e.g. "sin"). */
    int numArgs;			/* Nnumber of arguments required by
					 * function. */
    Tcl_ValueType *argTypes;		/* Array of types acceptable for
					 * each argument. */
    Tcl_MathProc *proc;			/* Procedure that implements the
					 * math function. */
    ClientData clientData;		/* Additional value to pass to the
					 * function. */
{
    Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    MathFunc *mathFuncPtr;
    int new, i;

    hPtr = Tcl_CreateHashEntry(&iPtr->mathFuncTable, name, &new);
    if (new) {
	Tcl_SetHashValue(hPtr, ckalloc(sizeof(MathFunc)));
    }
    mathFuncPtr = (MathFunc *) Tcl_GetHashValue(hPtr);
    if (numArgs > MAX_MATH_ARGS) {
	numArgs = MAX_MATH_ARGS;
    }
    mathFuncPtr->numArgs = numArgs;
    for (i = 0; i < numArgs; i++) {
	mathFuncPtr->argTypes[i] = argTypes[i];
    }
    mathFuncPtr->proc = proc;
    mathFuncPtr->clientData = clientData;
}

/*
 *----------------------------------------------------------------------
 *
 * ExprMathFunc --
 *
 *	This procedure is invoked to parse a math function from an
 *	expression string, carry out the function, and return the
 *	value computed.
 *
 * Results:
 *	TCL_OK is returned if all went well and the function's value
 *	was computed successfully.  If an error occurred, TCL_ERROR
 *	is returned and an error message is left in interp->result.
 *	After a successful return infoPtr has been updated to refer
 *	to the character just after the function call, the token is
 *	set to VALUE, and the value is stored in valuePtr.
 *
 * Side effects:
 *	Embedded commands could have arbitrary side-effects.
 *
 *----------------------------------------------------------------------
 */

static int
ExprMathFunc(interp, infoPtr, valuePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    register ExprInfo *infoPtr;		/* Describes the state of the parse.
					 * infoPtr->expr must point to the
					 * first character of the function's
					 * name. */
    register Value *valuePtr;		/* Where to store value, if that is
					 * what's parsed from string.  Caller
					 * must have initialized pv field
					 * correctly. */
{
    Interp *iPtr = (Interp *) interp;
    MathFunc *mathFuncPtr;		/* Info about math function. */
    Tcl_Value args[MAX_MATH_ARGS];	/* Arguments for function call. */
    Tcl_Value funcResult;		/* Result of function call. */
    Tcl_HashEntry *hPtr;
    char *p, *funcName;
    int i, savedChar, result;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    /*
     * Find the end of the math function's name and lookup the MathFunc
     * record for the function.
     */

    p = funcName = infoPtr->expr;
    while (isalnum(UCHAR(*p)) || (*p == '_')) {
	p++;
    }
    infoPtr->expr = p;
    result = ExprLex(interp, infoPtr, valuePtr);
    if ((result != TCL_OK) || (infoPtr->token != OPEN_PAREN)) {
	goto syntaxError;
    }
    savedChar = *p;
    *p = 0;
    hPtr = Tcl_FindHashEntry(&iPtr->mathFuncTable, funcName);
    if (hPtr == NULL) {
	/* i18n
	Tcl_AppendResult(interp, "unknown math function \"", funcName,
		"\"", (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_expr_unknown_math_func);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(funcName)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, funcName);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	*p = savedChar;
	return TCL_ERROR;
    }
    *p = savedChar;
    mathFuncPtr = (MathFunc *) Tcl_GetHashValue(hPtr);

    /*
     * Scan off the arguments for the function, if there are any.
     */

    if (mathFuncPtr->numArgs == 0) {
	result = ExprLex(interp, infoPtr, valuePtr);
	if ((result != TCL_OK) || (infoPtr->token != CLOSE_PAREN)) {
	    goto syntaxError;
	}
    } else {
	for (i = 0; ; i++) {
	    valuePtr->pv.next = valuePtr->pv.buffer;
	    result = ExprGetValue(interp, infoPtr, -1, valuePtr);
	    if (result != TCL_OK) {
		return result;
	    }
	    if (valuePtr->type == TYPE_STRING) {
		/* i18n
		interp->result =
			"argument to math function didn't have numeric value";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_arg_math_func_non_numeric);
		interp->freeProc = (Tcl_FreeProc *) free;

		return TCL_ERROR;
	    }
    
	    /*
	     * Copy the value to the argument record, converting it if
	     * necessary.
	     */
    
	    if (valuePtr->type == TYPE_INT) {
		if (mathFuncPtr->argTypes[i] == TCL_DOUBLE) {
		    args[i].type = TCL_DOUBLE;
		    args[i].doubleValue = valuePtr->intValue;
		} else {
		    args[i].type = TCL_INT;
		    args[i].intValue = valuePtr->intValue;
		}
	    } else {
		if (mathFuncPtr->argTypes[i] == TCL_INT) {
		    args[i].type = TCL_INT;
		    args[i].intValue = valuePtr->doubleValue;
		} else {
		    args[i].type = TCL_DOUBLE;
		    args[i].doubleValue = valuePtr->doubleValue;
		}
	    }
    
	    /*
	     * Check for a comma separator between arguments or a close-paren
	     * to end the argument list.
	     */
    
	    if (i == (mathFuncPtr->numArgs-1)) {
		if (infoPtr->token == CLOSE_PAREN) {
		    break;
		}
		if (infoPtr->token == COMMA) {
		    /* i18n
		    interp->result = "too many arguments for math function";
		     */
		    interp->result = (char *)dce_msg_get(tcl_s_expr_many_args_for_math_func);
		    interp->freeProc = (Tcl_FreeProc *) free;

		    return TCL_ERROR;
		} else {
		    goto syntaxError;
		}
	    }
	    if (infoPtr->token != COMMA) {
		if (infoPtr->token == CLOSE_PAREN) {
		    /* i18n
		    interp->result = "too few arguments for math function";
		     */
		    interp->result = (char *)dce_msg_get(tcl_s_expr_few_args_for_math_func);
		    interp->freeProc = (Tcl_FreeProc *) free;

		    return TCL_ERROR;
		} else {
		    goto syntaxError;
		}
	    }
	}
    }

    /*
     * Invoke the function and copy its result back into valuePtr.
     */

    tcl_MathInProgress++;
    result = (*mathFuncPtr->proc)(mathFuncPtr->clientData, interp, args,
	    &funcResult);
    tcl_MathInProgress--;
    if (result != TCL_OK) {
	return result;
    }
    if (funcResult.type == TCL_INT) {
	valuePtr->type = TYPE_INT;
	valuePtr->intValue = funcResult.intValue;
    } else {
	valuePtr->type = TYPE_DOUBLE;
	valuePtr->doubleValue = funcResult.doubleValue;
    }
    infoPtr->token = VALUE;
    return TCL_OK;

    syntaxError:
    /* i18n
    Tcl_AppendResult(interp, "syntax error in expression \"",
	    infoPtr->originalExpr, "\"", (char *) NULL);
     */
    i18n_msg_format = dce_msg_get(tcl_s_expr_syntax_err);
    i18n_msg_buf = (unsigned char *)ckalloc(strlen(infoPtr->originalExpr)
					+ strlen((char *)i18n_msg_format) + 1);
    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, infoPtr->originalExpr);
    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
    free(i18n_msg_format);
    ckfree(i18n_msg_buf);

    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * ExprFloatError --
 *
 *	This procedure is called when an error occurs during a
 *	floating-point operation.  It reads errno and sets
 *	interp->result accordingly.
 *
 * Results:
 *	Interp->result is set to hold an error message.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static void
ExprFloatError(interp, value)
    Tcl_Interp *interp;		/* Where to store error message. */
    double value;		/* Value returned after error;  used to
				 * distinguish underflows from overflows. */
{
    char buf[20];
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((errno == EDOM) || (value != value)) {
	/* i18n
	interp->result = "domain error: argument not in valid range";
	 */
	interp->result = (char *)dce_msg_get(tcl_s_expr_domain_err);
	interp->freeProc = (Tcl_FreeProc *) free;

	Tcl_SetErrorCode(interp, "ARITH", "DOMAIN", interp->result,
		(char *) NULL);
    } else if ((errno == ERANGE) || IS_INF(value)) {
	if (value == 0.0) {
	    /* i18n
	    interp->result = "floating-point value too small to represent";
	     */
	    interp->result = (char *)dce_msg_get(tcl_s_expr_float_val_too_small);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    Tcl_SetErrorCode(interp, "ARITH", "UNDERFLOW", interp->result,
		    (char *) NULL);
	} else {
	    /* i18n
	    interp->result = "floating-point value too large to represent";
	     */
	    interp->result = (char *)dce_msg_get(tcl_s_expr_float_val_too_large);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    Tcl_SetErrorCode(interp, "ARITH", "OVERFLOW", interp->result,
		    (char *) NULL);
	}
    } else {
	sprintf(buf, "%d", errno);
	/* i18n
	Tcl_AppendResult(interp, "unknown floating-point error, ",
		"errno = ", buf, (char *) NULL);
	 */
	i18n_msg_format = dce_msg_get(tcl_s_expr_unknown_float_point_err);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(buf)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, buf);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	Tcl_SetErrorCode(interp, "ARITH", "UNKNOWN", interp->result,
		(char *) NULL);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Math Functions --
 *
 *	This page contains the procedures that implement all of the
 *	built-in math functions for expressions.
 *
 * Results:
 *	Each procedure returns TCL_OK if it succeeds and places result
 *	information at *resultPtr.  If it fails it returns TCL_ERROR
 *	and leaves an error message in interp->result.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
ExprUnaryFunc(clientData, interp, args, resultPtr)
    ClientData clientData;		/* Contains address of procedure that
					 * takes one double argument and
					 * returns a double result. */
    Tcl_Interp *interp;
    Tcl_Value *args;
    Tcl_Value *resultPtr;
{
    double (*func)() = (double (*)()) clientData;

    errno = 0;
    resultPtr->type = TCL_DOUBLE;
    resultPtr->doubleValue = (*func)(args[0].doubleValue);
    if (errno != 0) {
	ExprFloatError(interp, resultPtr->doubleValue);
	return TCL_ERROR;
    }
    return TCL_OK;
}

static int
ExprBinaryFunc(clientData, interp, args, resultPtr)
    ClientData clientData;		/* Contains address of procedure that
					 * takes two double arguments and
					 * returns a double result. */
    Tcl_Interp *interp;
    Tcl_Value *args;
    Tcl_Value *resultPtr;
{
    double (*func)() = (double (*)()) clientData;

    errno = 0;
    resultPtr->type = TCL_DOUBLE;
    resultPtr->doubleValue = (*func)(args[0].doubleValue, args[1].doubleValue);
    if (errno != 0) {
	ExprFloatError(interp, resultPtr->doubleValue);
	return TCL_ERROR;
    }
    return TCL_OK;
}

	/* ARGSUSED */
static int
ExprAbsFunc(clientData, interp, args, resultPtr)
    ClientData clientData;
    Tcl_Interp *interp;
    Tcl_Value *args;
    Tcl_Value *resultPtr;
{
    resultPtr->type = TCL_DOUBLE;
    if (args[0].type == TCL_DOUBLE) {
	resultPtr->type = TCL_DOUBLE;
	if (args[0].doubleValue < 0) {
	    resultPtr->doubleValue = -args[0].doubleValue;
	} else {
	    resultPtr->doubleValue = args[0].doubleValue;
	}
    } else {
	resultPtr->type = TCL_INT;
	if (args[0].intValue < 0) {
	    resultPtr->intValue = -args[0].intValue;
	    if (resultPtr->intValue < 0) {
		/* i18n
		interp->result = "integer value too large to represent";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
		interp->freeProc = (Tcl_FreeProc *) free;

		Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW", interp->result,
			(char *) NULL);
		return TCL_ERROR;
	    }
	} else {
	    resultPtr->intValue = args[0].intValue;
	}
    }
    return TCL_OK;
}

	/* ARGSUSED */
static int
ExprDoubleFunc(clientData, interp, args, resultPtr)
    ClientData clientData;
    Tcl_Interp *interp;
    Tcl_Value *args;
    Tcl_Value *resultPtr;
{
    resultPtr->type = TCL_DOUBLE;
    if (args[0].type == TCL_DOUBLE) {
	resultPtr->doubleValue = args[0].doubleValue;
    } else {
	resultPtr->doubleValue = args[0].intValue;
    }
    return TCL_OK;
}

	/* ARGSUSED */
static int
ExprIntFunc(clientData, interp, args, resultPtr)
    ClientData clientData;
    Tcl_Interp *interp;
    Tcl_Value *args;
    Tcl_Value *resultPtr;
{
    resultPtr->type = TCL_INT;
    if (args[0].type == TCL_INT) {
	resultPtr->intValue = args[0].intValue;
    } else {
	if (args[0].doubleValue < 0) {
	    if (args[0].doubleValue < (double) (long) LONG_MIN) {
		tooLarge:
		/* i18n
		interp->result = "integer value too large to represent";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
		interp->freeProc = (Tcl_FreeProc *) free;

		Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW",
			interp->result, (char *) NULL);
		return TCL_ERROR;
	    }
	} else {
	    if (args[0].doubleValue > (double) LONG_MAX) {
		goto tooLarge;
	    }
	}
	resultPtr->intValue = args[0].doubleValue;
    }
    return TCL_OK;
}

	/* ARGSUSED */
static int
ExprRoundFunc(clientData, interp, args, resultPtr)
    ClientData clientData;
    Tcl_Interp *interp;
    Tcl_Value *args;
    Tcl_Value *resultPtr;
{
    resultPtr->type = TCL_INT;
    if (args[0].type == TCL_INT) {
	resultPtr->intValue = args[0].intValue;
    } else {
	if (args[0].doubleValue < 0) {
	    if (args[0].doubleValue <= (((double) (long) LONG_MIN) - 0.5)) {
		tooLarge:
		/* i18n
		interp->result = "integer value too large to represent";
		 */
		interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
		interp->freeProc = (Tcl_FreeProc *) free;

		Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW",
			interp->result, (char *) NULL);
		return TCL_ERROR;
	    }
	    resultPtr->intValue = (args[0].doubleValue - 0.5);
	} else {
	    if (args[0].doubleValue >= (((double) LONG_MAX + 0.5))) {
		goto tooLarge;
	    }
	    resultPtr->intValue = (args[0].doubleValue + 0.5);
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _WExprLex --
 *
 *	Lexical analyzer for expression parser:  parses a single value,
 *	operator, or other syntactic element from an expression string.
 *	This is ExprLex routine for wide character.
 *
 * Results:
 *	TCL_OK is returned unless an error occurred while doing lexical
 *	analysis or executing an embedded command.  In that case a
 *	standard Tcl error is returned, using interp->result to hold
 *	an error message.  In the event of a successful return, the token
 *	and field in infoPtr is updated to refer to the next symbol in
 *	the expression string, and the expr field is advanced past that
 *	token;  if the token is a value, then the value is stored at
 *	valuePtr.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
_WExprLex(interp, winfoPtr, wvaluePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    register _WExprInfo *winfoPtr;	/* Describes the state of the parse. */
    register _WValue *wvaluePtr;	/* Where to store value, if that is
					 * what's parsed from string.  Caller
					 * must have initialized pv field
					 * correctly. */
{
    wchar_t *wp;
    wchar_t *wvar, *wterm;
    int result;
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
        return TCL_CONV_ERROR;
    }

    if ((I18N_ParseChars(interp, i18n_parse_chars)) != TCL_OK)
    {
        return TCL_CONV_ERROR;
    }

    wp = winfoPtr->wexpr;
    while (iswspace((wint_t)*wp)) {
	wp++;
    }

    if (*wp == i18n_special_chars.w_esc_zero) {
	winfoPtr->token = END;
	winfoPtr->wexpr = wp;
	return TCL_OK;
    }

    /*
     * First try to parse the token as an integer or floating-point number.
     * A couple of tricky points:
     *
     * 1. Can't just check for leading digits to see if there's a number
     *    there, because it could be a special value like "NaN".
     * 2. Don't want to check for a number if the first character is "+"
     *    or "-".  If we do, we might treat a binary operator as unary
     *    by mistake, which will eventually cause a syntax error.
     * 3. First see if there's an integer, then if there's stuff after
     *    the integer that looks like it could be a floating-point number
     *    (or if there wasn't even a sensible integer), then try to parse
     *    as a floating-point number.  The check for the characters '8'
     *	  or '9' is to handle floating-point numbers like 028.6:  the
     *    leading zero causes strtoul to interpret the number as octal
     *    and stop when it gets to the 8.
     */

    if ((*wp != i18n_special_chars.w_plus)  && 
			(*wp != i18n_special_chars.w_minus)) 
    {
	errno = 0;
	wvaluePtr->intValue = wcstoul((const wchar_t *)wp, &wterm, 0);
	if ((wterm == wp) || (*wterm == i18n_special_chars.w_period) || 
		(*wterm == i18n_parse_chars.w_e) ||
		(*wterm == i18n_parse_chars.w_E) || 
	        (*wterm == i18n_parse_chars.w_8) || 
	        (*wterm == i18n_parse_chars.w_9)) 
	{
	    wchar_t *wterm2;
    
	    /*
	     * The code here is a bit tricky:  we want to use a floating-point
	     * number if there is one, but if there isn't then fall through to
	     * use the integer that was already parsed, if there was one.
	     */
    
	    errno = 0;
	    wvaluePtr->doubleValue = wcstod((const wchar_t *)wp, &wterm2);
	    if (wterm2 != wp) 
	    {
		if (errno != 0) 
	        {
		    ExprFloatError(interp, wvaluePtr->doubleValue);
		    return TCL_ERROR;
		}
		winfoPtr->token = VALUE;
		winfoPtr->wexpr = wterm2;
		wvaluePtr->type = TYPE_DOUBLE;
		return TCL_OK;
	    }
	    if (wterm != wp) 
	    {
		interp->result = (char *)dce_msg_get(tcl_s_expr_poor_flat_val_form);
		interp->freeProc = (Tcl_FreeProc *) free;

		return TCL_ERROR;
	    }
	}
	if (wterm != wp) 
	{
	    /*
	     * No floating-point number, but there is an integer.
	     */
    
	    if (errno == ERANGE) 
	    {
		interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
		interp->freeProc = (Tcl_FreeProc *) free;

		Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW", interp->result,
			(char *) NULL);
		return TCL_ERROR;
	    }
	    winfoPtr->token = VALUE;
	    winfoPtr->wexpr = wterm;
	    wvaluePtr->type = TYPE_INT;
	    return TCL_OK;
	}
    }

    winfoPtr->wexpr = wp+1;
    if (*wp == i18n_special_chars.w_dollar)
    {
	    /*
	     * Variable.  Fetch its value, then see if it makes sense
	     * as an integer or floating-point number.
	     */

	    winfoPtr->token = VALUE;
	    wvar = _Tcl_WParseVar(interp, wp, &winfoPtr->wexpr);
	    if (wvar == (wchar_t *)NULL) 
	    {
		return TCL_ERROR;
	    }
	    Tcl_ResetResult(interp);
	    if (((Interp *) interp)->noEval) 
	    {
		wvaluePtr->type = TYPE_INT;
		wvaluePtr->intValue = 0;
		return TCL_OK;
	    }
	    result = _WExprParseString(interp, wvar, wvaluePtr);
	    ckfree(wvar);	/* return from WParseVar */
	    return result;
    } 
    else if (*wp == i18n_special_chars.w_op_sbr)
    {
	    size_t wlen, st;
	    wchar_t *t_2wstr;

	    winfoPtr->token = VALUE;
	    ((Interp *) interp)->evalFlags = TCL_BRACKET_TERM;
	    result = _Tcl_WEval(interp, wp+1);

	    winfoPtr->wexpr = ((Interp *) interp)->wtermPtr;

	    if (result != TCL_OK) {
		return result;
	    }
	    winfoPtr->wexpr++;
	    if (((Interp *) interp)->noEval) {
		wvaluePtr->type = TYPE_INT;
		wvaluePtr->intValue = 0;

		Tcl_ResetResult(interp);
		return TCL_OK;
	    }

	    _Multibyte_to_WChar(interp, wlen, interp->result, t_2wstr, st)
	    if (st == (size_t)-1)
	    {
		return TCL_CONV_ERROR;
	    }
	    result = _WExprParseString(interp, t_2wstr, wvaluePtr);
	    ckfree(t_2wstr);

	    if (result != TCL_OK) {
		return result;
	    }
	    Tcl_ResetResult(interp);
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_dob_qo)
    {
	    wchar_t	wparse;

	    wparse = *wp;

	    winfoPtr->token = VALUE;
	    result = _TclWParseQuotes(interp, winfoPtr->wexpr, wparse, 0,
		    &winfoPtr->wexpr, &wvaluePtr->wpv);
	    if (result != TCL_OK) {
		return result;
	    }
	    Tcl_ResetResult(interp);
	    return _WExprParseString(interp, wvaluePtr->wpv.buffer, wvaluePtr);
    } 
    else if (*wp == i18n_special_chars.w_op_cbra)
    {
	    winfoPtr->token = VALUE;
	    result = _TclWParseBraces(interp, winfoPtr->wexpr, &winfoPtr->wexpr,
		    &wvaluePtr->wpv);
	    if (result != TCL_OK) {
		return result;
	    }
	    Tcl_ResetResult(interp);
	    return _WExprParseString(interp, wvaluePtr->wpv.buffer, wvaluePtr);
    } 
    else if (*wp == i18n_special_chars.w_op_paren)
    {
	    winfoPtr->token = OPEN_PAREN;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_cl_paren)
    {
	    winfoPtr->token = CLOSE_PAREN;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_comma)
    {
	    winfoPtr->token = COMMA;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_star)
    {
	    winfoPtr->token = MULT;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_slash)
    {
	    winfoPtr->token = DIVIDE;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_percent)
    {
	    winfoPtr->token = MOD;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_plus)
    {
	    winfoPtr->token = PLUS;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_minus)
    {
	    winfoPtr->token = MINUS;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_question)
    {
	    winfoPtr->token = QUESTY;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_colon)
    {
	    winfoPtr->token = COLON;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_less_t)
    {
	    if (wp[1] == i18n_special_chars.w_less_t)
	    {
		    winfoPtr->wexpr = wp+2;
		    winfoPtr->token = LEFT_SHIFT;
	    }
	    else if (wp[1] == i18n_special_chars.w_equal)
	    {
		    winfoPtr->wexpr = wp+2;
		    winfoPtr->token = LEQ;
	    }
	    else 
	    {
		    winfoPtr->token = LESS;
	    }
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_grea_t)
    {
	    if (wp[1] == i18n_special_chars.w_grea_t)
            {
		    winfoPtr->wexpr = wp+2;
		    winfoPtr->token = RIGHT_SHIFT;
	    }
	    else if (wp[1] == i18n_special_chars.w_equal)
            {
		    winfoPtr->wexpr = wp+2;
		    winfoPtr->token = GEQ;
	    }
	    else
	    {
		    winfoPtr->token = GREATER;
	    }
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_equal)
    {
	    if (wp[1] == i18n_special_chars.w_equal) {
		winfoPtr->wexpr = wp+2;
		winfoPtr->token = EQUAL;
	    } else {
		winfoPtr->token = UNKNOWN;
	    }
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_bang)
    {
	    if (wp[1] == i18n_special_chars.w_bang) {
		winfoPtr->wexpr = wp+2;
		winfoPtr->token = NEQ;
	    } else {
		winfoPtr->token = NOT;
	    }
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_and)
    {
	    if (wp[1] == i18n_special_chars.w_and) {
		winfoPtr->wexpr = wp+2;
		winfoPtr->token = AND;
	    } else {
		winfoPtr->token = BIT_AND;
	    }
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_mnt)
    {
	    winfoPtr->token = BIT_XOR;
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_wall)
    {
	    if (wp[1] == i18n_special_chars.w_wall) {
		winfoPtr->wexpr = wp+2;
		winfoPtr->token = OR;
	    } else {
		winfoPtr->token = BIT_OR;
	    }
	    return TCL_OK;
    } 
    else if (*wp == i18n_special_chars.w_tilde)
    {
	    winfoPtr->token = BIT_NOT;
	    return TCL_OK;
    }
    else
    {
	    if (iswalpha((wint_t)*wp)) 
            {
		winfoPtr->wexpr = wp;
		return _WExprMathFunc(interp, winfoPtr, wvaluePtr);
	    }
	    winfoPtr->wexpr = wp+1;
	    winfoPtr->token = UNKNOWN;
	    return TCL_OK;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * _WExprGetValue --
 *
 *	Parse a "value" from the remainder of the expression in infoPtr.
 *
 * Results:
 *	Normally TCL_OK is returned.  The value of the expression is
 *	returned in *valuePtr.  If an error occurred, then interp->result
 *	contains an error message and TCL_ERROR is returned.
 *	InfoPtr->token will be left pointing to the token AFTER the
 *	expression, and infoPtr->expr will point to the character just
 *	after the terminating token.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static int
_WExprGetValue(interp, winfoPtr, prec, wvaluePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    _WExprInfo *winfoPtr;		/* Describes the state of the parse
					 * just before the value (i.e. ExprLex
					 * will be called to get first token
					 * of value). */
    int prec;				/* Treat any un-parenthesized operator
					 * with precedence <= this as the end
					 * of the expression. */
    _WValue *wvaluePtr;			/* Where to store the value of the
					 * expression.   Caller must have
					 * initialized pv field. */
{
    Interp *iPtr = (Interp *) interp;
    _WValue wvalue2;			/* Second operand for current
					 * operator.  */
    int operator;			/* Current operator (either unary
					 * or binary). */
    int badType;			/* Type of offending argument;  used
					 * for error messages. */
    int gotOp;				/* Non-zero means already lexed the
					 * operator (while picking up value
					 * for unary operator).  Don't lex
					 * again. */
    int result, w_ret;
    char *ctemp;
    size_t clen, st;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
        return TCL_CONV_ERROR;
    }

    /*
     * There are two phases to this procedure.  First, pick off an initial
     * value.  Then, parse (binary operator, value) pairs until done.
     */

    gotOp = 0;
    wvalue2.wpv.buffer = wvalue2.wpv.next = wvalue2.wstaticSpace;
    wvalue2.wpv.end = wvalue2.wpv.buffer + STATIC_STRING_SPACE - 1;
    wvalue2.wpv.expandProc = _TclWExpandParseValue;
    wvalue2.wpv.clientData = (ClientData) NULL;
    result = _WExprLex(interp, winfoPtr, wvaluePtr);
    if (result != TCL_OK) {
	goto done;
    }
    if (winfoPtr->token == OPEN_PAREN) {

	/*
	 * Parenthesized sub-expression.
	 */

	result = _WExprGetValue(interp, winfoPtr, -1, wvaluePtr);
	if (result != TCL_OK) {
	    goto done;
	}
	if (winfoPtr->token != CLOSE_PAREN) {
	    _WChar_To_Multibyte(interp, clen, winfoPtr->woriginalExpr, ctemp, st)
	    if (st == (size_t)-1)
		return TCL_CONV_ERROR;
		
	    i18n_msg_format = dce_msg_get(tcl_s_expr_unmatch_parens_in_expr);
	    i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	    sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	    Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	    free(i18n_msg_format);
	    ckfree(i18n_msg_buf);
	    ckfree(ctemp);

	    result = TCL_ERROR;
	    goto done;
	}
    } else {
	if (winfoPtr->token == MINUS) {
	    winfoPtr->token = UNARY_MINUS;
	}
	if (winfoPtr->token >= UNARY_MINUS) {

	    /*
	     * Process unary operators.
	     */

	    operator = winfoPtr->token;
	    result = _WExprGetValue(interp, winfoPtr, precTable[winfoPtr->token],
		    wvaluePtr);
	    if (result != TCL_OK) {
		goto done;
	    }
	    switch (operator) {
		case UNARY_MINUS:
		    if (wvaluePtr->type == TYPE_INT) {
			wvaluePtr->intValue = -wvaluePtr->intValue;
		    } else if (wvaluePtr->type == TYPE_DOUBLE){
			wvaluePtr->doubleValue = -wvaluePtr->doubleValue;
		    } else {
			badType = wvaluePtr->type;
			goto illegalType;
		    } 
		    break;
		case NOT:
		    if (wvaluePtr->type == TYPE_INT) {
			wvaluePtr->intValue = !wvaluePtr->intValue;
		    } else if (wvaluePtr->type == TYPE_DOUBLE) {
			/*
			 * Theoretically, should be able to use
			 * "!valuePtr->intValue", but apparently some
			 * compilers can't handle it.
			 */
			if (wvaluePtr->doubleValue == 0.0) {
			    wvaluePtr->intValue = 1;
			} else {
			    wvaluePtr->intValue = 0;
			}
			wvaluePtr->type = TYPE_INT;
		    } else {
			badType = wvaluePtr->type;
			goto illegalType;
		    }
		    break;
		case BIT_NOT:
		    if (wvaluePtr->type == TYPE_INT) {
			wvaluePtr->intValue = ~wvaluePtr->intValue;
		    } else {
			badType  = wvaluePtr->type;
			goto illegalType;
		    }
		    break;
	    }
	    gotOp = 1;
	} else if (winfoPtr->token != VALUE) {
	    goto syntaxError;
	}
    }

    /*
     * Got the first operand.  Now fetch (operator, operand) pairs.
     */

    if (!gotOp) {
	result = _WExprLex(interp, winfoPtr, &wvalue2);
	if (result != TCL_OK) {
	    goto done;
	}
    }
    while (1) {
	operator = winfoPtr->token;
	wvalue2.wpv.next = wvalue2.wpv.buffer;
	if ((operator < MULT) || (operator >= UNARY_MINUS)) {
	    if ((operator == END) || (operator == CLOSE_PAREN)
		    || (operator == COMMA)) {
		result = TCL_OK;
		goto done;
	    } else {
		goto syntaxError;
	    }
	}
	if (precTable[operator] <= prec) {
	    result = TCL_OK;
	    goto done;
	}

	/*
	 * If we're doing an AND or OR and the first operand already
	 * determines the result, don't execute anything in the
	 * second operand:  just parse.  Same style for ?: pairs.
	 */

	if ((operator == AND) || (operator == OR) || (operator == QUESTY)) {
	    if (wvaluePtr->type == TYPE_DOUBLE) {
		wvaluePtr->intValue = wvaluePtr->doubleValue != 0;
		wvaluePtr->type = TYPE_INT;
	    } else if (wvaluePtr->type == TYPE_STRING) {
		badType = TYPE_STRING;
		goto illegalType;
	    }
	    if (((operator == AND) && !wvaluePtr->intValue)
		    || ((operator == OR) && wvaluePtr->intValue)) {
		iPtr->noEval++;
		result = _WExprGetValue(interp, winfoPtr, precTable[operator],
			&wvalue2);
		iPtr->noEval--;
	    } else if (operator == QUESTY) {
		if (wvaluePtr->intValue != 0) {
		    wvaluePtr->wpv.next = wvaluePtr->wpv.buffer;
		    result = _WExprGetValue(interp, winfoPtr, precTable[operator],
			    wvaluePtr);
		    if (result != TCL_OK) {
			goto done;
		    }
		    if (winfoPtr->token != COLON) {
			goto syntaxError;
		    }
		    wvalue2.wpv.next = wvalue2.wpv.buffer;
		    iPtr->noEval++;
		    result = _WExprGetValue(interp, winfoPtr, precTable[operator],
			    &wvalue2);
		    iPtr->noEval--;
		} else {
		    iPtr->noEval++;
		    result = _WExprGetValue(interp, winfoPtr, precTable[operator],
			    &wvalue2);
		    iPtr->noEval--;
		    if (result != TCL_OK) {
			goto done;
		    }
		    if (winfoPtr->token != COLON) {
			goto syntaxError;
		    }
		    wvaluePtr->wpv.next = wvaluePtr->wpv.buffer;
		    result = _WExprGetValue(interp, winfoPtr, precTable[operator],
			    wvaluePtr);
		}
	    } else {
		result = _WExprGetValue(interp, winfoPtr, precTable[operator],
			&wvalue2);
	    }
	} else {
	    result = _WExprGetValue(interp, winfoPtr, precTable[operator],
		    &wvalue2);
	}
	if (result != TCL_OK) {
	    goto done;
	}
	if ((winfoPtr->token < MULT) && (winfoPtr->token != VALUE)
		&& (winfoPtr->token != END) && (winfoPtr->token != COMMA)
		&& (winfoPtr->token != CLOSE_PAREN)) {
	    goto syntaxError;
	}

	/*
	 * At this point we've got two values and an operator.  Check
	 * to make sure that the particular data types are appropriate
	 * for the particular operator, and perform type conversion
	 * if necessary.
	 */

	switch (operator) {

	    /*
	     * For the operators below, no strings are allowed and
	     * ints get converted to floats if necessary.
	     */

	    case MULT: case DIVIDE: case PLUS: case MINUS:
		if ((wvaluePtr->type == TYPE_STRING)
			|| (wvalue2.type == TYPE_STRING)) {
		    badType = TYPE_STRING;
		    goto illegalType;
		}
		if (wvaluePtr->type == TYPE_DOUBLE) {
		    if (wvalue2.type == TYPE_INT) {
			wvalue2.doubleValue = wvalue2.intValue;
			wvalue2.type = TYPE_DOUBLE;
		    }
		} else if (wvalue2.type == TYPE_DOUBLE) {
		    if (wvaluePtr->type == TYPE_INT) {
			wvaluePtr->doubleValue = wvaluePtr->intValue;
			wvaluePtr->type = TYPE_DOUBLE;
		    }
		}
		break;

	    /*
	     * For the operators below, only integers are allowed.
	     */

	    case MOD: case LEFT_SHIFT: case RIGHT_SHIFT:
	    case BIT_AND: case BIT_XOR: case BIT_OR:
		 if (wvaluePtr->type != TYPE_INT) {
		     badType = wvaluePtr->type;
		     goto illegalType;
		 } else if (wvalue2.type != TYPE_INT) {
		     badType = wvalue2.type;
		     goto illegalType;
		 }
		 break;

	    /*
	     * For the operators below, any type is allowed but the
	     * two operands must have the same type.  Convert integers
	     * to floats and either to strings, if necessary.
	     */

	    case LESS: case GREATER: case LEQ: case GEQ:
	    case EQUAL: case NEQ:
		if (wvaluePtr->type == TYPE_STRING) {
		    if (wvalue2.type != TYPE_STRING) {
			w_ret = _WExprMakeString(interp, &wvalue2);
			if (w_ret != TCL_OK)
				return w_ret;
		    }
		} else if (wvalue2.type == TYPE_STRING) {
		    if (wvaluePtr->type != TYPE_STRING) {
			_WExprMakeString(interp, wvaluePtr);
			if (w_ret != TCL_OK)
				return w_ret;
		    }
		} else if (wvaluePtr->type == TYPE_DOUBLE) {
		    if (wvalue2.type == TYPE_INT) {
			wvalue2.doubleValue = wvalue2.intValue;
			wvalue2.type = TYPE_DOUBLE;
		    }
		} else if (wvalue2.type == TYPE_DOUBLE) {
		     if (wvaluePtr->type == TYPE_INT) {
			wvaluePtr->doubleValue = wvaluePtr->intValue;
			wvaluePtr->type = TYPE_DOUBLE;
		    }
		}
		break;

	    /*
	     * For the operators below, no strings are allowed, but
	     * no int->double conversions are performed.
	     */

	    case AND: case OR:
		if (wvaluePtr->type == TYPE_STRING) {
		    badType = wvaluePtr->type;
		    goto illegalType;
		}
		if (wvalue2.type == TYPE_STRING) {
		    badType = wvalue2.type;
		    goto illegalType;
		}
		break;

	    /*
	     * For the operators below, type and conversions are
	     * irrelevant:  they're handled elsewhere.
	     */

	    case QUESTY: case COLON:
		break;

	    /*
	     * Any other operator is an error.
	     */

	    default:
		interp->result = (char *)dce_msg_get(tcl_s_expr_unknown_oper_in_expr);
		interp->freeProc = (Tcl_FreeProc *) free;

		result = TCL_ERROR;
		goto done;
	}

	/*
	 * If necessary, convert one of the operands to the type
	 * of the other.  If the operands are incompatible with
	 * the operator (e.g. "+" on strings) then return an
	 * error.
	 */

	switch (operator) {
	    case MULT:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue *= wvalue2.intValue;
		} else {
		    wvaluePtr->doubleValue *= wvalue2.doubleValue;
		}
		break;
	    case DIVIDE:
	    case MOD:
		if (wvaluePtr->type == TYPE_INT) {
		    int divisor, quot, rem, negative;
		    if (wvalue2.intValue == 0) {
			divideByZero:
			interp->result = (char *)dce_msg_get(tcl_s_expr_divide_by_zero);
			interp->freeProc = (Tcl_FreeProc *) free;

			Tcl_SetErrorCode(interp, "ARITH", "DIVZERO",
				interp->result, (char *) NULL);
			result = TCL_ERROR;
			goto done;
		    }

		    /*
		     * The code below is tricky because C doesn't guarantee
		     * much about the properties of the quotient or
		     * remainder, but Tcl does:  the remainder always has
		     * the same sign as the divisor and a smaller absolute
		     * value.
		     */

		    divisor = wvalue2.intValue;
		    negative = 0;
		    if (divisor < 0) {
			divisor = -divisor;
			wvaluePtr->intValue = -wvaluePtr->intValue;
			negative = 1;
		    }
		    quot = wvaluePtr->intValue / divisor;
		    rem = wvaluePtr->intValue % divisor;
		    if (rem < 0) {
			rem += divisor;
			quot -= 1;
		    }
		    if (negative) {
			rem = -rem;
		    }
		    wvaluePtr->intValue = (operator == DIVIDE) ? quot : rem;
		} else {
		    if (wvalue2.doubleValue == 0.0) {
			goto divideByZero;
		    }
		    wvaluePtr->doubleValue /= wvalue2.doubleValue;
		}
		break;
	    case PLUS:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue += wvalue2.intValue;
		} else {
		    wvaluePtr->doubleValue += wvalue2.doubleValue;
		}
		break;
	    case MINUS:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue -= wvalue2.intValue;
		} else {
		    wvaluePtr->doubleValue -= wvalue2.doubleValue;
		}
		break;
	    case LEFT_SHIFT:
		wvaluePtr->intValue <<= wvalue2.intValue;
		break;
	    case RIGHT_SHIFT:
		/*
		 * The following code is a bit tricky:  it ensures that
		 * right shifts propagate the sign bit even on machines
		 * where ">>" won't do it by default.
		 */

		if (wvaluePtr->intValue < 0) {
		    wvaluePtr->intValue =
			    ~((~wvaluePtr->intValue) >> wvalue2.intValue);
		} else {
		    wvaluePtr->intValue >>= wvalue2.intValue;
		}
		break;
	    case LESS:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue =
			wvaluePtr->intValue < wvalue2.intValue;
		} 
		else if (wvaluePtr->type == TYPE_DOUBLE) {
		    wvaluePtr->intValue =
			wvaluePtr->doubleValue < wvalue2.doubleValue;
		} 
		else {
		    wvaluePtr->intValue =
			    wcscmp(wvaluePtr->wpv.buffer, wvalue2.wpv.buffer) < 0;
		}
		wvaluePtr->type = TYPE_INT;
		break;
	    case GREATER:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue =
			wvaluePtr->intValue > wvalue2.intValue;
		} else if (wvaluePtr->type == TYPE_DOUBLE) {
		    wvaluePtr->intValue =
			wvaluePtr->doubleValue > wvalue2.doubleValue;
		} else {
		    wvaluePtr->intValue =
			    wcscmp(wvaluePtr->wpv.buffer, wvalue2.wpv.buffer) > 0;
		}
		wvaluePtr->type = TYPE_INT;
		break;
	    case LEQ:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue =
			wvaluePtr->intValue <= wvalue2.intValue;
		} else if (wvaluePtr->type == TYPE_DOUBLE) {
		    wvaluePtr->intValue =
			wvaluePtr->doubleValue <= wvalue2.doubleValue;
		} else {
		    wvaluePtr->intValue =
			    wcscmp(wvaluePtr->wpv.buffer, wvalue2.wpv.buffer) <= 0;
		}
		wvaluePtr->type = TYPE_INT;
		break;
	    case GEQ:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue =
			wvaluePtr->intValue >= wvalue2.intValue;
		} else if (wvaluePtr->type == TYPE_DOUBLE) {
		    wvaluePtr->intValue =
			wvaluePtr->doubleValue >= wvalue2.doubleValue;
		} else {
		    wvaluePtr->intValue =
			    wcscmp(wvaluePtr->wpv.buffer, wvalue2.wpv.buffer) >= 0;
		}
		wvaluePtr->type = TYPE_INT;
		break;
	    case EQUAL:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue =
			wvaluePtr->intValue == wvalue2.intValue;
		} else if (wvaluePtr->type == TYPE_DOUBLE) {
		    wvaluePtr->intValue =
			wvaluePtr->doubleValue == wvalue2.doubleValue;
		} else {
		    wvaluePtr->intValue =
			    wcscmp(wvaluePtr->wpv.buffer, wvalue2.wpv.buffer) == 0;
		}
		wvaluePtr->type = TYPE_INT;
		break;
	    case NEQ:
		if (wvaluePtr->type == TYPE_INT) {
		    wvaluePtr->intValue =
			wvaluePtr->intValue != wvalue2.intValue;
		} else if (wvaluePtr->type == TYPE_DOUBLE) {
		    wvaluePtr->intValue =
			wvaluePtr->doubleValue != wvalue2.doubleValue;
		} else {
		    wvaluePtr->intValue =
			    wcscmp(wvaluePtr->wpv.buffer, wvalue2.wpv.buffer) != 0;
		}
		wvaluePtr->type = TYPE_INT;
		break;
	    case BIT_AND:
		wvaluePtr->intValue &= wvalue2.intValue;
		break;
	    case BIT_XOR:
		wvaluePtr->intValue ^= wvalue2.intValue;
		break;
	    case BIT_OR:
		wvaluePtr->intValue |= wvalue2.intValue;
		break;

	    /*
	     * For AND and OR, we know that the first value has already
	     * been converted to an integer.  Thus we need only consider
	     * the possibility of int vs. double for the second value.
	     */

	    case AND:
		if (wvalue2.type == TYPE_DOUBLE) {
		    wvalue2.intValue = wvalue2.doubleValue != 0;
		    wvalue2.type = TYPE_INT;
		}
		wvaluePtr->intValue = wvaluePtr->intValue && wvalue2.intValue;
		break;
	    case OR:
		if (wvalue2.type == TYPE_DOUBLE) {
		    wvalue2.intValue = wvalue2.doubleValue != 0;
		    wvalue2.type = TYPE_INT;
		}
		wvaluePtr->intValue = wvaluePtr->intValue || wvalue2.intValue;
		break;

	    case COLON:
		interp->result = (char *)dce_msg_get(tcl_s_expr_colon_without_question);
		interp->freeProc = (Tcl_FreeProc *) free;

		result = TCL_ERROR;
		goto done;
	}
    }

    done:
    if (wvalue2.wpv.buffer != wvalue2.wstaticSpace) {
	ckfree(wvalue2.wpv.buffer);
    }
    return result;

    syntaxError:

     _WChar_To_Multibyte(interp, clen, winfoPtr->woriginalExpr, ctemp, st)
     if (st == (size_t)-1)
	return TCL_CONV_ERROR;

     i18n_msg_format = dce_msg_get(tcl_s_expr_syntax_err);
     i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
				+ strlen((char *)i18n_msg_format) + 1);
     sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
     Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
     free(i18n_msg_format);
     ckfree(i18n_msg_buf);
     ckfree(ctemp);

    result = TCL_ERROR;
    goto done;

    illegalType:
    if (badType == TYPE_DOUBLE)
    {
	i18n_msg_format = dce_msg_get(tcl_s_expr_float_non_numeric_as_op);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(operatorStrings[operator])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, operatorStrings[operator]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

    } 
    else
    {
	i18n_msg_format = dce_msg_get(tcl_s_expr_non_numeric_non_numeric_as_op);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(operatorStrings[operator])
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, operatorStrings[operator]);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

    }
    result = TCL_ERROR;
    goto done;
}

/*
 *--------------------------------------------------------------
 *
 * _WExprParseString --
 *
 *	Given a string (such as one coming from command or variable
 *	substitution), make a Value based on the string.  The value
 *	will be a floating-point or integer, if possible, or else it
 *	will just be a copy of the string.
 *
 * Results:
 *	TCL_OK is returned under normal circumstances, and TCL_ERROR
 *	is returned if a floating-point overflow or underflow occurred
 *	while reading in a number.  The value at *valuePtr is modified
 *	to hold a number, if possible.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
_WExprParseString(interp, wstring, wvaluePtr)
    Tcl_Interp *interp;		/* Where to store error message. */
    wchar_t *wstring;		/* String to turn into value. */
    _WValue *wvaluePtr;		/* Where to store value information. 
				 * Caller must have initialized pv field. */
{
    wchar_t *wterm, *wp, *wstart;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
        return TCL_CONV_ERROR;
    }

    if (*wstring != i18n_special_chars.w_esc_zero) 
    {
	wvaluePtr->type = TYPE_INT;
	errno = 0;

	/*
	 * Note: use strtoul instead of strtol for integer conversions
	 * to allow full-size unsigned numbers, but don't depend on
	 * strtoul to handle sign characters;  it won't in some
	 * implementations.
	 */

	for (wp = wstring; iswspace(*wp); wp++) {
	    /* Empty loop body. */
	}
	/* if (*p == '-') { */
	if (*wp == i18n_special_chars.w_minus) {
	    wstart = wp+1;
	    wvaluePtr->intValue = -wcstoul(wstart, &wterm, 0);
	/* } else if (*p == '+') { */
	} else if (*wp == i18n_special_chars.w_plus) {
	    wstart = wp+1;
	    wvaluePtr->intValue = wcstoul(wstart, &wterm, 0);
	} else {
	    wstart = wp;
	    wvaluePtr->intValue = wcstoul(wstart, &wterm, 0);
	}
	if (errno == ERANGE) {
	    /*
	     * This procedure is sometimes called with string in
	     * interp->result, so we have to clear the result before
	     * logging an error message.
	     */

	    Tcl_ResetResult(interp);
	    interp->result = (char *)dce_msg_get(tcl_s_expr_int_val_too_large);
	    interp->freeProc = (Tcl_FreeProc *) free;

	    Tcl_SetErrorCode(interp, "ARITH", "IOVERFLOW", interp->result,
		    (char *) NULL);
	    return TCL_ERROR;
	}
	if ((wterm != wstart) && (*wterm == i18n_special_chars.w_esc_zero)) {
	    return TCL_OK;
	}
	errno = 0;
	wvaluePtr->doubleValue = wcstod(wp, &wterm);
	if ((wterm != wp) && (*wterm == i18n_special_chars.w_esc_zero)) {
	    if (errno != 0) {
		Tcl_ResetResult(interp);
		ExprFloatError(interp, wvaluePtr->doubleValue);
		return TCL_ERROR;
	    }
	    wvaluePtr->type = TYPE_DOUBLE;
	    return TCL_OK;
	}
    }

    /*
     * Not a valid number.  Save a string value (but don't do anything
     * if it's already the value).
     */

    wvaluePtr->type = TYPE_STRING;
    if (wstring != wvaluePtr->wpv.buffer) {
	int length, shortfall;

	length = wcslen(wstring);
	wvaluePtr->wpv.next = wvaluePtr->wpv.buffer;
	shortfall = length - (wvaluePtr->wpv.end - wvaluePtr->wpv.buffer);
	if (shortfall > 0) {
	    (*wvaluePtr->wpv.expandProc)(&wvaluePtr->wpv, shortfall);
	}
	wcscpy(wvaluePtr->wpv.buffer, wstring);
    }
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * _WExprMakeString --
 *
 *	Convert a value from int or double representation to
 *	a string.
 *
 * Results:
 *	The information at *valuePtr gets converted to string
 *	format, if it wasn't that way already.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
_WExprMakeString(interp, wvaluePtr)
    Tcl_Interp *interp;			/* Interpreter to use for precision
					 * information. */
    _WValue *wvaluePtr;			/* Value to be converted. */
{
    int shortfall, i;
    char temp_buf[STATIC_STRING_SPACE];
    unsigned char *i18n_msg_format, *i18n_msg_buf;

    shortfall = 150 - (wvaluePtr->wpv.end - wvaluePtr->wpv.buffer);
    if (shortfall > 0) {
	(*wvaluePtr->wpv.expandProc)(&wvaluePtr->wpv, shortfall);
    }
    if (wvaluePtr->type == TYPE_INT) 
    {
	for (i=0; i < STATIC_STRING_SPACE; i++)
		temp_buf[i] = '\0';
	sprintf(temp_buf, "%ld", wvaluePtr->intValue);
	if ((mbstowcs(wvaluePtr->wpv.buffer, (const char *)temp_buf, strlen(temp_buf))) == (size_t)-1)
	{
		i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen(temp_buf) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, temp_buf);		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *)NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		return TCL_CONV_ERROR;
	}
    } 
    else if (wvaluePtr->type == TYPE_DOUBLE) 
    {
	for (i=0; i < STATIC_STRING_SPACE; i++)
		temp_buf[i] = '\0';
	Tcl_PrintDouble(interp, wvaluePtr->doubleValue, temp_buf);

	if ((mbstowcs(wvaluePtr->wpv.buffer, (const char *)temp_buf, strlen(temp_buf))) == (size_t)-1)
	{
		i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed);
		i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format) + strlen(temp_buf) + 1);
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, temp_buf);		Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *)NULL);
		free(i18n_msg_format);
		ckfree(i18n_msg_buf);
		return TCL_CONV_ERROR;
	}
    }
    wvaluePtr->type = TYPE_STRING;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * _WExprMathFunc --
 *
 *	This procedure is invoked to parse a math function from an
 *	expression string, carry out the function, and return the
 *	value computed.
 *
 * Results:
 *	TCL_OK is returned if all went well and the function's value
 *	was computed successfully.  If an error occurred, TCL_ERROR
 *	is returned and an error message is left in interp->result.
 *	After a successful return infoPtr has been updated to refer
 *	to the character just after the function call, the token is
 *	set to VALUE, and the value is stored in valuePtr.
 *
 * Side effects:
 *	Embedded commands could have arbitrary side-effects.
 *
 *----------------------------------------------------------------------
 */

static int
_WExprMathFunc(interp, winfoPtr, wvaluePtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    _WExprInfo *winfoPtr;		/* Describes the state of the parse.
					 * infoPtr->expr must point to the
					 * first character of the function's
					 * name. */
    _WValue *wvaluePtr;			/* Where to store value, if that is
					 * what's parsed from string.  Caller
					 * must have initialized pv field
					 * correctly. */
{
    Interp *iPtr = (Interp *) interp;
    MathFunc *mathFuncPtr;		/* Info about math function. */
    Tcl_Value args[MAX_MATH_ARGS];	/* Arguments for function call. */
    Tcl_Value funcResult;		/* Result of function call. */
    Tcl_HashEntry *hPtr;
    char funcName[STATIC_STRING_SPACE], *ctemp;
    wchar_t *wp, *wfuncName, wSaveChar;
    int i, result;
    size_t wlen, st;
    unsigned char *i18n_msg_buf;
    unsigned char *i18n_msg_format;

    if ((I18N_SpecChars(interp, i18n_special_chars)) != TCL_OK)
    {
        return TCL_CONV_ERROR;
    }

    /*
     * Find the end of the math function's name and lookup the MathFunc
     * record for the function.
     */

    wp = wfuncName = winfoPtr->wexpr;
    while (iswalnum(*wp) || (*wp == i18n_special_chars.w_unscore)) {
	wp++;
    }
    winfoPtr->wexpr = wp;
    result = _WExprLex(interp, winfoPtr, wvaluePtr);
    if ((result != TCL_OK) || (winfoPtr->token != OPEN_PAREN)) {
	goto syntaxError;
    }

    wSaveChar = *wp;
    *wp = i18n_special_chars.w_esc_zero;
    wlen =  wcstombs(funcName, (const wchar_t *)wfuncName, wcslen(wfuncName));
    if (wlen == (size_t)-1)
    {
	i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed);
	Tcl_AppendResult(interp, (char *)i18n_msg_format, (char *)NULL);
	free(i18n_msg_format);
        *wp = wSaveChar;
	return TCL_CONV_ERROR;
    }
    *wp = wSaveChar;
    funcName[wlen] = '\0';
 
    hPtr = Tcl_FindHashEntry(&iPtr->mathFuncTable, funcName);

    if (hPtr == NULL) {
	i18n_msg_format = dce_msg_get(tcl_s_expr_unknown_math_func);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(funcName)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, funcName);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);

	return TCL_ERROR;
    }
    mathFuncPtr = (MathFunc *) Tcl_GetHashValue(hPtr);

    /*
     * Scan off the arguments for the function, if there are any.
     */

    if (mathFuncPtr->numArgs == 0) {
	result = _WExprLex(interp, winfoPtr, wvaluePtr);
	if ((result != TCL_OK) || (winfoPtr->token != CLOSE_PAREN)) {
	    goto syntaxError;
	}
    } else {
	for (i = 0; ; i++) {
	    wvaluePtr->wpv.next = wvaluePtr->wpv.buffer;
	    result = _WExprGetValue(interp, winfoPtr, -1, wvaluePtr);
	    if (result != TCL_OK) {
		return result;
	    }
	    if (wvaluePtr->type == TYPE_STRING) {
		interp->result = (char *)dce_msg_get(tcl_s_expr_arg_math_func_non_numeric);
		interp->freeProc = (Tcl_FreeProc *) free;

		return TCL_ERROR;
	    }
    
	    /*
	     * Copy the value to the argument record, converting it if
	     * necessary.
	     */
    
	    if (wvaluePtr->type == TYPE_INT) {
		if (mathFuncPtr->argTypes[i] == TCL_DOUBLE) {
		    args[i].type = TCL_DOUBLE;
		    args[i].doubleValue = wvaluePtr->intValue;
		} else {
		    args[i].type = TCL_INT;
		    args[i].intValue = wvaluePtr->intValue;
		}
	    } else {
		if (mathFuncPtr->argTypes[i] == TCL_INT) {
		    args[i].type = TCL_INT;
		    args[i].intValue = wvaluePtr->doubleValue;
		} else {
		    args[i].type = TCL_DOUBLE;
		    args[i].doubleValue = wvaluePtr->doubleValue;
		}
	    }
    
	    /*
	     * Check for a comma separator between arguments or a close-paren
	     * to end the argument list.
	     */
    
	    if (i == (mathFuncPtr->numArgs-1)) {
		if (winfoPtr->token == CLOSE_PAREN) {
		    break;
		}
		if (winfoPtr->token == COMMA) {
		    interp->result = (char *)dce_msg_get(tcl_s_expr_many_args_for_math_func);
		    interp->freeProc = (Tcl_FreeProc *) free;

		    return TCL_ERROR;
		} else {
		    goto syntaxError;
		}
	    }
	    if (winfoPtr->token != COMMA) {
		if (winfoPtr->token == CLOSE_PAREN) {
		    interp->result = (char *)dce_msg_get(tcl_s_expr_few_args_for_math_func);
		    interp->freeProc = (Tcl_FreeProc *) free;

		    return TCL_ERROR;
		} else {
		    goto syntaxError;
		}
	    }
	}
    }

    /*
     * Invoke the function and copy its result back into valuePtr.
     */

    tcl_MathInProgress++;
    result = (*mathFuncPtr->proc)(mathFuncPtr->clientData, interp, args,
	    &funcResult);
    tcl_MathInProgress--;
    if (result != TCL_OK) {
	return result;
    }
    if (funcResult.type == TCL_INT) {
	wvaluePtr->type = TYPE_INT;
	wvaluePtr->intValue = funcResult.intValue;
    } else {
	wvaluePtr->type = TYPE_DOUBLE;
	wvaluePtr->doubleValue = funcResult.doubleValue;
    }
    winfoPtr->token = VALUE;
    return TCL_OK;

    syntaxError:

    _WChar_To_Multibyte(interp, wlen, winfoPtr->woriginalExpr, ctemp, st)
    if (st == (size_t)-1)
	return TCL_CONV_ERROR;

    i18n_msg_format = dce_msg_get(tcl_s_expr_syntax_err);
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
 *--------------------------------------------------------------
 *
 * _WExprTopLevel --
 *
 *	This procedure provides top-level functionality shared by
 *	procedures like Tcl_ExprInt, Tcl_ExprDouble, etc.
 *
 * Results:
 *	The result is a standard Tcl return value.  If an error
 *	occurs then an error message is left in interp->result.
 *	The value of the expression is returned in *valuePtr, in
 *	whatever form it ends up in (could be string or integer
 *	or double).  Caller may need to convert result.  Caller
 *	is also responsible for freeing string memory in *valuePtr,
 *	if any was allocated.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int
_WExprTopLevel(interp, wstring, wvaluePtr)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    wchar_t *wstring;			/* Expression to evaluate. */
    _WValue *wvaluePtr;			/* Where to store result.  Should
					 * not be initialized by caller. */
{
    _WExprInfo winfo;
    int result;
    unsigned char *i18n_msg_format;
    unsigned char *i18n_msg_buf;

    /*
     * Create the math functions the first time an expression is
     * evaluated.
     */

    if (!(((Interp *) interp)->flags & EXPR_INITIALIZED)) {
	BuiltinFunc *funcPtr;

	((Interp *) interp)->flags |= EXPR_INITIALIZED;
	for (funcPtr = funcTable; funcPtr->name != NULL;
		funcPtr++) {
	    Tcl_CreateMathFunc(interp, funcPtr->name, funcPtr->numArgs,
		    funcPtr->argTypes, funcPtr->proc, funcPtr->clientData);
	}
    }

    winfo.woriginalExpr = wstring;
    winfo.wexpr = wstring;
    wvaluePtr->wpv.buffer = wvaluePtr->wpv.next = wvaluePtr->wstaticSpace;
    wvaluePtr->wpv.end = wvaluePtr->wpv.buffer + STATIC_STRING_SPACE - 1;
    wvaluePtr->wpv.expandProc = _TclWExpandParseValue;
    wvaluePtr->wpv.clientData = (ClientData) NULL;

    result = _WExprGetValue(interp, &winfo, -1, wvaluePtr);
    if (result != TCL_OK) {
	return result;
    }
    if (winfo.token != END) {
	char *ctemp;
	size_t clen, st;

	_WChar_To_Multibyte(interp, clen, wstring, ctemp, st)
	if (st == (size_t)-1)
		return TCL_CONV_ERROR;

	i18n_msg_format = dce_msg_get(tcl_s_expr_syntax_err);
	i18n_msg_buf = (unsigned char *)ckalloc(strlen(ctemp)
					+ strlen((char *)i18n_msg_format) + 1);
	sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, ctemp);
	Tcl_AppendResult(interp, (char *)i18n_msg_buf, (char *) NULL);
	free(i18n_msg_format);
	ckfree(i18n_msg_buf);
	ckfree(ctemp);

	return TCL_ERROR;
    }
    if ((wvaluePtr->type == TYPE_DOUBLE) && (IS_NAN(wvaluePtr->doubleValue)
	    || IS_INF(wvaluePtr->doubleValue))) {
	/*
	 * IEEE floating-point error.
	 */

	ExprFloatError(interp, wvaluePtr->doubleValue);
	return TCL_ERROR;
    }
    return TCL_OK;
}

int
_Tcl_WExprBoolean(interp, wstring, ptr)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    wchar_t *wstring;			/* Expression to evaluate. */
    int *ptr;				/* Where to store 0/1 result. */
{
    _WValue wvalue;
    int result;

    result = _WExprTopLevel(interp, wstring, &wvalue);
    if (result == TCL_OK) {
	if (wvalue.type == TYPE_INT) {
	    *ptr = wvalue.intValue != 0;
	} else if (wvalue.type == TYPE_DOUBLE) {
	    *ptr = wvalue.doubleValue != 0.0;
	} else {
	    result = _Tcl_WGetBoolean(interp, wvalue.wpv.buffer, ptr);
	}
    }
    if (wvalue.wpv.buffer != wvalue.wstaticSpace) {
	ckfree(wvalue.wpv.buffer);
    }
    return result;
}

/*
 *--------------------------------------------------------------
 *
 * _Tcl_WExprString --
 *
 *	Evaluate an expression and return its value in string form.
 *
 * Results:
 *	A standard Tcl result.  If the result is TCL_OK, then the
 *	interpreter's result is set to the string value of the
 *	expression.  If the result is TCL_OK, then interp->result
 *	contains an error message.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

int
_Tcl_WExprString(interp, wstring)
    Tcl_Interp *interp;			/* Context in which to evaluate the
					 * expression. */
    wchar_t *wstring;			/* Expression to evaluate. */
{
    _WValue wvalue;
    int result;
    char *ctemp;
    size_t clen, st;
    unsigned char *i18n_msg_format;

    result = _WExprTopLevel(interp, wstring, &wvalue);
    if (result == TCL_OK) 
    {
	if (wvalue.type == TYPE_INT) 
	{
	    sprintf(interp->result, "%ld", wvalue.intValue);
	} else if (wvalue.type == TYPE_DOUBLE) 
	{
	    Tcl_PrintDouble(interp, wvalue.doubleValue, interp->result);
	} 
	else 
	{
	    if (wvalue.wpv.buffer != wvalue.wstaticSpace) 
	    {
		_WChar_To_Multibyte(interp, clen, wvalue.wpv.buffer, ctemp, st)
		if (st == (size_t)-1)
			result = TCL_CONV_ERROR;

		interp->result = ctemp;
		interp->freeProc = (Tcl_FreeProc *) free;
		ckfree(wvalue.wpv.buffer);
		wvalue.wpv.buffer = wvalue.wstaticSpace;
	    } 
	    else 
	    {
		_WChar_To_Multibyte(interp, clen, wvalue.wpv.buffer, ctemp, st)
		if (st == (size_t)-1)
			result = TCL_CONV_ERROR;

		Tcl_SetResult(interp, ctemp, TCL_VOLATILE);
	    }
	}
    }
    if (wvalue.wpv.buffer != wvalue.wstaticSpace) 
    {
	ckfree(wvalue.wpv.buffer);
    }
    return result;
}
