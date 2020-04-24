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
 * $Log: tclRegexp.h,v $
 * Revision 1.1.6.2  1996/02/18  23:32:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:33:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:04:36  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:04:53  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:47  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:49:47  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:45:13  rousseau]
 * 
 * $EndLog$
 */
/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */

#ifndef _TCL
#include "tcl.h"
#endif
#ifndef _REGEXP
#define _REGEXP 1

#define NSUBEXP  10
typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *TclRegComp _ANSI_ARGS_((char *exp));
extern int TclRegExec _ANSI_ARGS_((regexp *prog, char *string, char *start));
extern void TclRegSub _ANSI_ARGS_((regexp *prog, char *source, char *dest));
extern void TclRegError _ANSI_ARGS_((char *msg));

#endif /* REGEXP */
