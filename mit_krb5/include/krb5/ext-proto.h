/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ext-proto.h,v $
 * Revision 1.1.2.1  1996/06/05  20:26:59  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:26:51  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5/ext-proto.h
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * Prototypes for external (libc) funtions.
 */


#ifndef KRB5_EXT_PROTO__
#define KRB5_EXT_PROTO__

#ifdef HAS_STDLIB_H
#include <stdlib.h>
#else
#if defined(__STDC__) || defined(_WINDOWS)
#ifdef NO_STDLIB_H
#include <fake-stdlib.h>
#else
#include <stdlib.h>
#endif /* NO_STDLIB_H */
#else
extern char *malloc(), *realloc(), *calloc();
extern char *getenv();
#endif /* ! __STDC__ */
#endif /* HAS_STDLIB_H */

#ifdef USE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#ifndef HAS_STRDUP
extern char *strdup PROTOTYPE((const char *));
#endif

#ifdef HAS_UNISTD_H
#include <unistd.h>
#endif

#endif /* KRB5_EXT_PROTO__ */
