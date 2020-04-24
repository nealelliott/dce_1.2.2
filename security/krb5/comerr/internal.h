/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: internal.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:42  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:56:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  internal.h V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * internal include file for com_err package
 */
#ifndef __STDC__
#undef const
#define const
#endif

extern int errno;
extern char const * const sys_errlist[];
extern const int sys_nerr;

#if !defined(ultrix) && !defined(_AIX)
/* perror is type void on ultrix 4 & co. */
#if !defined(_ANSI_C_SOURCE) && !defined(hpux) && !defined(apollo)
#ifdef __STDC__
void perror (const char *);
#else
void perror ();
#endif
#endif
#endif
