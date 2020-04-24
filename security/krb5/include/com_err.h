/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: com_err.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:50  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:57:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  com_err.h V=1 01/01/89 //littl/prgy/krb5/include
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Header file for common error description library.
 *
 * Copyright 1988, Student Information Processing Board of the
 * Massachusetts Institute of Technology.
 *
 * For copyright and distribution info, see the documentation supplied
 * with this package.
 */

#ifndef __COM_ERR_H

#ifdef __STDC__
#ifndef __HIGHC__		/* gives us STDC but not stdarg */
#include <stdarg.h>
#else
#include <varargs.h>
#endif
/* ANSI C -- use prototypes etc */
void com_err (const char *, long, const char *, ...);
char const *error_message (long);
void (*com_err_hook) (const char *, long, const char *, va_list);
void (*set_com_err_hook (void (*) (const char *, long, const char *, va_list)))
    (const char *, long, const char *, va_list);
void (*reset_com_err_hook ()) (const char *, long, const char *, va_list);
#else
/* no prototypes */
void com_err ();
char *error_message ();
void (*com_err_hook) ();
void (*set_com_err_hook ()) ();
void (*reset_com_err_hook ()) ();
#endif

#define __COM_ERR_H
#endif /* ! defined(__COM_ERR_H) */
