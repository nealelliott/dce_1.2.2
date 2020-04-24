/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: exp_regexp.h,v $
 * Revision 1.1.2.1  1996/08/09  19:13:19  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:57:10  arvind]
 *
 * $EndLog$
 */

/* access to regexp internals */
#define regbol		exp_regbol
#define regtry		exp_regtry
#define regexec		exp_regexec
#define regerror	TclRegError
extern char *regbol;
int regtry();

