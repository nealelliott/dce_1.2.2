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
 * $Log: string.h,v $
 * Revision 1.1.6.2  1996/02/18  23:32:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:50  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:28:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:00:36  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:03:32  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:00  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:38:13  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:37:52  rousseau]
 * 
 * $EndLog$
 */
/*
 * string.h --
 *
 *	Declarations of ANSI C library procedures for string handling.
 *
 * Copyright (c) 1991-1993 The Regents of the University of California.
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
 *
 * $Header: /u0/rcs_trees/dce/rcs/admin/tcl7.3/compat/string.h,v 1.1.6.2 1996/02/18 23:32:22 marty Exp $ SPRITE (Berkeley)
 */

#ifndef _STRING
#define _STRING

#include <tcl.h>

/*
 * The following #include is needed to define size_t. (This used to
 * include sys/stdtypes.h but that doesn't exist on older versions
 * of SunOS, e.g. 4.0.2, so I'm trying sys/types.h now.... hopefully
 * it exists everywhere)
 */

#include <sys/types.h>

extern char *		memchr _ANSI_ARGS_((CONST VOID *s, int c, size_t n));
extern int		memcmp _ANSI_ARGS_((CONST VOID *s1, CONST VOID *s2,
			    size_t n));
extern char *		memcpy _ANSI_ARGS_((VOID *t, CONST VOID *f, size_t n));
extern char *		memmove _ANSI_ARGS_((VOID *t, CONST VOID *f,
			    size_t n));
extern char *		memset _ANSI_ARGS_((VOID *s, int c, size_t n));

extern int		strcasecmp _ANSI_ARGS_((CONST char *s1,
			    CONST char *s2));
extern char *		strcat _ANSI_ARGS_((char *dst, CONST char *src));
extern char *		strchr _ANSI_ARGS_((CONST char *string, int c));
extern int		strcmp _ANSI_ARGS_((CONST char *s1, CONST char *s2));
extern char *		strcpy _ANSI_ARGS_((char *dst, CONST char *src));
extern size_t		strcspn _ANSI_ARGS_((CONST char *string,
			    CONST char *chars));
extern char *		strdup _ANSI_ARGS_((CONST char *string));
extern char *		strerror _ANSI_ARGS_((int error));
extern size_t		strlen _ANSI_ARGS_((CONST char *string));
extern int		strncasecmp _ANSI_ARGS_((CONST char *s1,
			    CONST char *s2, size_t n));
extern char *		strncat _ANSI_ARGS_((char *dst, CONST char *src,
			    size_t numChars));
extern int		strncmp _ANSI_ARGS_((CONST char *s1, CONST char *s2,
			    size_t nChars));
extern char *		strncpy _ANSI_ARGS_((char *dst, CONST char *src,
			    size_t numChars));
extern char *		strpbrk _ANSI_ARGS_((CONST char *string, char *chars));
extern char *		strrchr _ANSI_ARGS_((CONST char *string, int c));
extern size_t		strspn _ANSI_ARGS_((CONST char *string,
			    CONST char *chars));
extern char *		strstr _ANSI_ARGS_((CONST char *string,
			    CONST char *substring));
extern char *		strtok _ANSI_ARGS_((CONST char *s, CONST char *delim));

#endif /* _STRING */
