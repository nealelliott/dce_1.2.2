/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktbase.c,v $
 * Revision 1.1.4.2  1996/02/18  00:13:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:56:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:26  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:52:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:12:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ktbase.c V=3 10/24/91 //littl/prgy/krb5/lib/keytab
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/keytab/ktbase.c,v $
 * $Author: marty $
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
 * Registration functions for keytab.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_ktbase_c[] =
"$Id: ktbase.c,v 1.1.4.2 1996/02/18 00:13:04 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

struct krb5_kt_typelist
 {
  krb5_kt_ops *ops;
  struct krb5_kt_typelist *next;
 };
static struct krb5_kt_typelist krb5_kt_typelist_dfl = { &krb5_kt_dfl_ops, 0 };
static struct krb5_kt_typelist *kt_typehead = &krb5_kt_typelist_dfl;

/*
 * Register a new key table type
 * don't replace if it already exists; return an error instead.
 */

krb5_error_code
krb5_kt_register(ops)
krb5_kt_ops *ops;
{
    struct krb5_kt_typelist *t;
    for (t = kt_typehead;t && strcmp(t->ops->prefix,ops->prefix);t = t->next)
	;
    if (t) {
	return KRB5_KT_TYPE_EXISTS;
    }
    if (!(t = (struct krb5_kt_typelist *) malloc(sizeof(*t))))
	return ENOMEM;
    t->next = kt_typehead;
    t->ops = ops;
    kt_typehead = t;
    return 0;
}

/*
 * Resolve a key table name into a keytab object.
 *
 * The name is currently constrained to be of the form "type:residual";
 *
 * The "type" portion corresponds to one of the registered key table
 * types, while the "residual" portion is specific to the
 * particular keytab type.
 */

krb5_error_code krb5_kt_resolve (name, ktid)
    const char *name;
    krb5_keytab *ktid;
{
    struct krb5_kt_typelist *tlist;
    char *pfx, *resid, *cp;
    int pfxlen;
    
    cp = strchr (name, ':');
    if (!cp)
	return KRB5_KT_BADNAME;

    pfxlen = cp - (char *)name;
    resid = (char *)name + pfxlen + 1;
	
    pfx = malloc (pfxlen+1);
    if (!pfx)
	return ENOMEM;

    memcpy (pfx, name, pfxlen);
    pfx[pfxlen] = '\0';

    *ktid = (krb5_keytab) 0;

    for (tlist = kt_typehead; tlist; tlist = tlist->next) {
	if (strcmp (tlist->ops->prefix, pfx) == 0) {
	    free(pfx);
	    return (*tlist->ops->resolve)(resid, ktid);
	}
    }
    free(pfx);
    return KRB5_KT_UNKNOWN_TYPE;
}
