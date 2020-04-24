/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: compiler.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:34  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:55:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  compiler.h V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * definitions common to the source files of the error table compiler
 */

#ifndef __STDC__
/* loser */
#undef const
#define const
#endif

enum lang {
    lang_C,			/* ANSI C (default) */
    lang_KRC,			/* C: ANSI + K&R */
    lang_CPP			/* C++ */
};

int debug;			/* dump debugging info? */
char *filename;			/* error table source */
enum lang language;
const char *whoami;
