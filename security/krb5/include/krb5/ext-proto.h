/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ext-proto.h,v $
 * Revision 1.2.4.2  1996/02/18  23:02:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:55  marty]
 *
 * Revision 1.2.4.1  1995/12/08  17:42:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:02  root]
 * 
 * Revision 1.2.2.2  1992/12/29  13:59:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:25  zeliff]
 * 
 * Revision 1.2  1992/01/19  22:13:49  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/

/*
 *  OSF DCE Version 1.0 
 */

/*  ext-proto.h V=5 11/20/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/ext-proto.h,v $
 * $Author: marty $
 * $Id: ext-proto.h,v 1.2.4.2 1996/02/18 23:02:24 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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

#ifdef OSF_DCE
#   include <krb5/osf_dce.h>
#endif

#ifdef HAS_STDLIB_H
#include <stdlib.h>
#else
#ifdef __STDC__
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

/* strdup is defined in OSF's <string.h>. */

#if !defined (OSF_DCE)
extern char *strdup PROTOTYPE((const char *));
#endif

#endif /* KRB5_EXT_PROTO__ */
