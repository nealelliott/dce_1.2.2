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
 * Revision 1.1.6.2  1996/02/18  23:01:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:13  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:39:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:31  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:12  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:23  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:55:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  com_err.h V=3 11/11/91 //littl/prgy/krb5/comerr
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
extern void com_err (const char *, long, const char *, ...);
extern char const *error_message (int);
extern void (*com_err_hook) (const char *, long, const char *, va_list);
extern void (*set_com_err_hook (void (*) (const char *, long, const char *, va_list)))
    (const char *, long, const char *, va_list);
extern void (*reset_com_err_hook (void)) (const char *, long, const char *, va_list);
#else
/* no prototypes */
extern void com_err ();
extern char *error_message ();
extern void (*com_err_hook) ();
extern void (*set_com_err_hook ()) ();
extern void (*reset_com_err_hook ()) ();
#endif

#define __COM_ERR_H
#endif /* ! defined(__COM_ERR_H) */
