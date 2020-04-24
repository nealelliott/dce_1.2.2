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
 * $Log: tclWInt.h,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.4.1  1995/02/17  15:56:10  levy
 * 	Submission of 1.1 unintegrated.
 * 	[1995/02/17  15:51:59  levy]
 *
 * Revision 1.1.2.1  1994/10/21  21:39:47  mori_m
 * 	CR 11259: Tcl I18N work.  Added wchar_t data type handling logic.
 * 	[1994/10/21  21:35:29  mori_m]
 * 
 * $EndLog$
 */
/*
 * tclWInt.h --
 *
 *	Declarations of things used internally by the Tcl interpreter.
 *	Only contains wide character related declarations (i18n).
 */

#ifndef _TCLWINT
#define _TCLWINT

#include <tcl.h>
#include "tclInt.h"
#include <stdlib.h>
#include <wchar.h>

typedef struct _WParseValue {
	wchar_t *buffer;	/* Address of first character in
				 * output buffer. */
	wchar_t *next;		/* Place to store next character in
				 * output buffer. */
	wchar_t *end;		/* Address of the last usable character
				 * in the buffer. */
	void (*expandProc) _ANSI_ARGS_((struct _WParseValue *pvPtr, int needed));
				/* Procedure to call when space runs out;
				 * it will make more space. */
	ClientData clientData;	/* Arbitrary information for use of
				 * expandProc. */
} _WParseValue;

/*
 *----------------------------------------------------------------
 * Procedures shared among Tcl modules but not used by the outside
 * world:
 *----------------------------------------------------------------
 */

extern int		_Tcl_WStringMatch _ANSI_ARGS_((Tcl_Interp *interp,
				wchar_t *wstring, wchar_t *wpattern));
extern int		_Tcl_WAppendElement _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring));
extern int		_Tcl_WConvertElement _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wsrc, wchar_t *wdst, int flags, int *err));
extern int		_Tcl_WScanElement _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *string, int *flagPtr, int *conv_err));
extern int		_InitI18NSpecChars _ANSI_ARGS_((Tcl_Interp *interp));
extern wchar_t 		_Tcl_WBackslash _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wsrc, int *readPtr));
extern void		SetupAppendBuffer _ANSI_ARGS_((Interp *iPtr,
                            int newSpace));
extern wchar_t *	_Tcl_WMerge _ANSI_ARGS_((Tcl_Interp *interp, int argc,
			    char **argv, int *conv_error));
extern int		_Tcl_WCtype _ANSI_ARGS_((Tcl_Interp *interp, wchar_t wc));
extern int		_TclWParseQuotes _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, wchar_t wtermpChar,
		            int flags, wchar_t **termPtr, _WParseValue *pvPtr));
extern void 		_TclWExpandParseValue _ANSI_ARGS_((
			    _WParseValue *wpvPtr, int needed));
extern int		_TclWParseNestedCmd _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, int flags, wchar_t **wtermPtr,
			    _WParseValue *wpvPtr));
extern wchar_t *	_TclWWordEnd _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstart, int nested, int *semiPtr));
extern int 		_Tcl_WCommandComplete _ANSI_ARGS_((Tcl_Interp *interp,
			    char *cmd));
extern int		_TclWParseBraces _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, wchar_t **wtermPtr,
			    _WParseValue *wpvPtr));
extern wchar_t *	_Tcl_WParseVar _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, wchar_t **wtermPtr));
extern int		_Tcl_WEval _ANSI_ARGS_((Tcl_Interp *interp, 
			    wchar_t *wcmd));
extern int		_i18n_main _ANSI_ARGS_((Tcl_Interp *interp,
			    int argc, char **argv));
extern int		_Tcl_WEvalFile _ANSI_ARGS_((Tcl_Interp *interp, 
			    char *filename));
extern char *		_Tcl_WTildeSubst _ANSI_ARGS_((Tcl_Interp *interp,
			    char *name, Tcl_DString *bufferPtr));
extern int		_Tcl_WRecordAndEval _ANSI_ARGS_((Tcl_Interp *interp,
			    char *cmd, int flags));
extern char *		_Tcl_WDStringAppendElement 
			    _ANSI_ARGS_((Tcl_Interp *interp, 
			    Tcl_DString *dsPtr, char *string));
extern char *		_Tcl_WSetVar _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wvarName, wchar_t *wnewValue, int flags));
extern char *		_Tcl_WSetVar2 _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wpart1, wchar_t *wpart2,
			    wchar_t *wnewValue, int flags));
extern char *		_Tcl_WGetVar _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wvarName, int flags));
extern char *		_Tcl_WGetVar2 _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wpart1, wchar_t *wpart2, int flags));
extern int		_Tcl_WUnsetVar _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wvarName, int flags));
extern int		_Tcl_WUnsetVar2 _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wpart1, wchar_t *wpart2, int flags));
extern int		_Tcl_WTraceVar _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wvarName, int flags, 
			    Tcl_VarTraceProc *proc, ClientData clientData));
extern int		_Tcl_WTraceVar2 _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wpart1, wchar_t *wpart2, int flags,
			    Tcl_VarTraceProc *proc, ClientData clientData));
extern int		_Tcl_WUntraceVar _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wvarName, int flags, 
			    Tcl_VarTraceProc *proc, ClientData clientData));
extern int		_Tcl_WUntraceVar2 _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wpart1, wchar_t *wpart2, int flags,
			    Tcl_VarTraceProc *proc, ClientData clientData));
extern ClientData	_Tcl_WVarTraceInfo _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wvarName, int flags, 
			    Tcl_VarTraceProc *proc, ClientData prevClientData));
extern ClientData 	_Tcl_WVarTraceInfo2 _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wpart1, wchar_t *wpart2, int flags,
			    Tcl_VarTraceProc *procPtr,
			    ClientData prevClientData));
extern int 		_Tcl_WGetInt _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, int *intPtr));
extern int		_Tcl_WGetBoolean _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, int *boolPtr));
extern int		_Tcl_WGetDouble _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, double *doublePtr));
extern int		_Tcl_WExprBoolean _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, int *ptr));
extern int		_Tcl_WLinkVar _ANSI_ARGS_((Tcl_Interp *interp,
			    char *varName, char *addr, int type));
extern void		_Tcl_WUnlinkVar _ANSI_ARGS_((Tcl_Interp *interp,
			    char *varName));
extern int		_TclWFindElement _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wlist, wchar_t **welementPtr,
			    wchar_t **wnextPtr, int *sizePtr, int *bracePtr));
extern void		_TclWCopyAndCollapse _ANSI_ARGS_((Tcl_Interp *interp,
			    int count, wchar_t *wsrc, wchar_t *wdst));
extern int		_Tcl_WSplitList _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wlist, int *argcPtr, wchar_t ***wargvPtr));
extern wchar_t *	_Tcl_WConcat _ANSI_ARGS_((Tcl_Interp *interp,
			    int argc, wchar_t **wargv));
extern int		_TclWGetFrame _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, CallFrame **framePtrPtr));
extern int		_Tcl_WExprString _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring));
extern int		_TclWGetListIndex _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, int *indexPtr));
extern int		_Tcl_WRegExpMatch _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, wchar_t *wpattern));
extern int		_Tcl_WAppInit _ANSI_ARGS_((Tcl_Interp *interp));
extern int		_Tcl_WInit _ANSI_ARGS_((Tcl_Interp *interp));
extern int		_InitI18NParseChars _ANSI_ARGS_((Tcl_Interp *interp));
extern int		_TclWParseWords _ANSI_ARGS_((Tcl_Interp *interp,
			    wchar_t *wstring, int flags, int maxWords, 
			    wchar_t **wtermPtr, int *argcPtr, wchar_t **wargv, 
			    _WParseValue *wpvPtr));


/*
 *----------------------------------------------------------------
 * Command procedures in the generic core:
 *----------------------------------------------------------------
 */

extern int	_Tcl_WAppendCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WArrayCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WBreakCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WCaseCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WCatchCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WConcatCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WContinueCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WErrorCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WEvalCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WExprCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WForCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WForeachCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WFormatCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WGlobalCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WHistoryCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WIfCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WIncrCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WInfoCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WJoinCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLappendCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLindexCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLinsertCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLlengthCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WListCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLrangeCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLreplaceCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLsearchCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WLsortCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WProcCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WRegexpCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WRegsubCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WRenameCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WReturnCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WScanCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WSetCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WSplitCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WStringCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WSwitchCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WTraceCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WUnsetCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WUplevelCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WUpvarCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WWhileCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));

/*
 *----------------------------------------------------------------
 * Command procedures in the UNIX core:
 *----------------------------------------------------------------
 */

extern int	_Tcl_WCdCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WCloseCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WEofCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WExecCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WExitCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WFileCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WFlushCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WGetsCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WGlobCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WOpenCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WPutsCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WPidCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WPwdCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WReadCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WSeekCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WSourceCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WTellCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));
extern int	_Tcl_WTimeCmd _ANSI_ARGS_((ClientData clientData,
		    Tcl_Interp *interp, int argc, wchar_t **argv));

/*
 *----------------------------------------------------------------
 * Global (read-only) variable for special wide characters
 *----------------------------------------------------------------
 */
/*
 * The structure is wide character equivalence of special
 * characters.  These characters are often used to parse string.
 */

typedef struct i18nSpecChars {
	int	init_flag;	/* non-zero if all fields are set */
	wchar_t w_null;		/* NUL */
	wchar_t w_tab;		/* TAB */
	wchar_t	w_lf;		/* LF  */
	wchar_t	w_vt;		/* VT  */
	wchar_t	w_ff;		/* FF  */
	wchar_t	w_cr;		/* CR  */
	wchar_t	w_empty;	/* "" */
	wchar_t w_op_cbra;	/* '{' */
	wchar_t w_cl_cbra;	/* '}' */
	wchar_t w_bsla;		/* '\\' */
	wchar_t	w_op_sbr;	/* '[' */
	wchar_t	w_cl_sbr;	/* ']' */
	wchar_t	w_dollar;	/* '$' */
	wchar_t w_sem_col;	/* ';' */
	wchar_t	w_space;	/* ' ' */
	wchar_t w_dob_qo;	/* '"' */
	wchar_t	w_esc_a;	/* '\a' */
	wchar_t	w_esc_b;	/* '\b' */
	wchar_t	w_esc_f;	/* '\f' */
	wchar_t	w_esc_n;	/* '\n' */
	wchar_t w_esc_r;	/* '\r' */
	wchar_t w_esc_t;	/* '\t' */
	wchar_t w_esc_v;	/* '\v' */
	wchar_t	w_esc_zero;	/* '\0' */
	wchar_t	w_slash;	/* '/' */
	wchar_t w_question;	/* '?' */
	wchar_t w_star;		/* '*' */
	wchar_t w_comma;	/* ',' */
	wchar_t w_minus;	/* '-' */
	wchar_t w_plus;		/* '+' */
	wchar_t w_period;	/* '.' */
	wchar_t	w_op_paren;	/* '(' */
	wchar_t	w_cl_paren;	/* ')' */
	wchar_t	w_percent;	/* '%' */
	wchar_t w_colon;	/* ':' */
	wchar_t w_less_t;	/* '<' */
	wchar_t w_grea_t;	/* '>' */
	wchar_t w_equal;	/* '=' */
	wchar_t w_bang;		/* '!' */
	wchar_t w_and;		/* '&' */
	wchar_t w_mnt;		/* '^' */
	wchar_t w_wall;		/* '|' */
	wchar_t w_tilde;	/* '~' */
	wchar_t w_unscore;	/* '_' */
	wchar_t w_sharp;	/* '#' */
	wchar_t w_at;		/* '@' */
} i18nSpecChars;

#define I18N_SpecChars(interp, i18nChars) (i18nChars.init_flag ? TCL_OK : _InitI18NSpecChars(interp))

extern i18nSpecChars i18n_special_chars;

typedef struct i18nParseChars {
	int	init_flag;	/* non-zero if all fields are set */
	wchar_t w_E;		/* 'E' */
	wchar_t	w_L;		/* 'L' */
	wchar_t w_X;		/* 'X' */
	wchar_t	w_a;		/* 'a' */
	wchar_t w_b;		/* 'b' */
	wchar_t w_c;		/* 'c  */
	wchar_t w_d;		/* 'd' */
	wchar_t w_e;		/* 'e' */
	wchar_t	w_f;		/* 'f' */
	wchar_t	w_g;		/* 'g' */
	wchar_t w_h;		/* 'h' */
	wchar_t	w_i;		/* 'i' */
	wchar_t w_l;		/* 'l' */
	wchar_t w_n;		/* 'n' */
	wchar_t	w_o;		/* 'o' */
	wchar_t w_r;		/* 'r' */
	wchar_t w_s;	 	/* 's' */
	wchar_t w_t;		/* 't' */
	wchar_t w_u;		/* 'u' */
	wchar_t w_v;		/* 'v' */
	wchar_t w_x;		/* 'x' */
	wchar_t w_w;		/* 'w' */
	wchar_t w_0;		/* '0' */
	wchar_t w_1;		/* '1' */
	wchar_t w_2;		/* '2' */
	wchar_t w_3;		/* '3' */
	wchar_t w_4;		/* '4' */
	wchar_t w_5;		/* '5' */
	wchar_t w_6;		/* '6' */
	wchar_t w_7;		/* '7' */
	wchar_t w_8;		/* '8' */
	wchar_t w_9;		/* '9' */
} i18nParseChars;

#define I18N_ParseChars(interp, i18nPChars) (i18nPChars.init_flag ? TCL_OK : _InitI18NParseChars(interp))

extern i18nParseChars i18n_parse_chars;


/*
 * Wide character to multibyte conversion macro
 */
#define _Multibyte_to_WChar(iPtr, wlen, cPtr, wPtr, st) \
	wlen = mbstowcs((wchar_t *)NULL, (const char *)cPtr, strlen(cPtr)); \
	wPtr = (wchar_t *)ckalloc((wlen + 1) * sizeof(wchar_t)); \
	st = mbstowcs(wPtr, (const char *)cPtr, wlen); \
	if (st == (size_t)-1) \
	{ \
		ckfree(wPtr); \
		i18n_msg_format = dce_msg_get(tcl_s_multibyte_conv_failed); \
		i18n_msg_buf = (unsigned char *)ckalloc(strlen((char *)i18n_msg_format)+ strlen(cPtr) + 1); \
		sprintf((char *)i18n_msg_buf, (char *)i18n_msg_format, cPtr); \
		Tcl_AppendResult(iPtr, (char *)i18n_msg_buf, (char *)NULL); \
		free(i18n_msg_format); \
		ckfree(i18n_msg_buf); \
	} \
	else \
	{ \
		wPtr[wlen] = i18n_special_chars.w_esc_zero; \
	}


	/* cPtr = (char *)ckalloc((clen+1) * sizeof(char)); \ */

#define _WChar_To_Multibyte(iPtr, clen, wPtr, cPtr, st) \
	clen = wcstombs((char *)NULL, (const wchar_t *)wPtr, (wcslen(wPtr) * sizeof(wchar_t))); \
	cPtr = (char *)ckalloc(clen + 1); \
	st = wcstombs(cPtr, (const wchar_t *)wPtr, clen); \
	if (st == (size_t)-1) \
	{ \
		ckfree(cPtr); \
		i18n_msg_format = dce_msg_get(tcl_s_wchar_conv_failed); \
		Tcl_AppendResult(iPtr, (char *)i18n_msg_format, (char *)NULL); \
		free(i18n_msg_format); \
	} \
	else \
	{ \
		cPtr[clen] = '\0'; \
	}

#endif /* _TCLWINT */
