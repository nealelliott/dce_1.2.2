/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ccbase.c,v $
 * Revision 1.1.4.2  1996/02/18  00:09:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:45  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:19:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:01:20  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ccbase.c V=3 10/24/91 //littl/prgy/krb5/lib/ccache
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/ccbase.c,v $
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
 * Registration functions for ccache.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_ccbase_c[] =
"$Id: ccbase.c,v 1.1.4.2 1996/02/18 00:09:04 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

extern krb5_cc_ops *krb5_cc_dfl_ops;
struct krb5_cc_typelist
 {
  krb5_cc_ops *ops;
  struct krb5_cc_typelist *next;
 };
static struct krb5_cc_typelist *cc_typehead = 0;

/*
 * Register a new credentials cache type
 * If override is set, replace any existing ccache with that type tag
 */

krb5_error_code
krb5_cc_register(DECLARG(krb5_cc_ops *,ops),
		 DECLARG(krb5_boolean,override))
OLDDECLARG(krb5_cc_ops *,ops)
OLDDECLARG(krb5_boolean,override)
{
    struct krb5_cc_typelist *t;
    for (t = cc_typehead;t && strcmp(t->ops->prefix,ops->prefix);t = t->next)
	;
    if (t) {
	if (override) {
	    t->ops = ops;
	    return 0;
	} else
	    return KRB5_CC_TYPE_EXISTS;
    }
    if (!(t = (struct krb5_cc_typelist *) malloc(sizeof(*t))))
	return ENOMEM;
    t->next = cc_typehead;
    t->ops = ops;
    cc_typehead = t;
    return 0;
}

/*
 * Resolve a credential cache name into a cred. cache object.
 *
 * The name is currently constrained to be of the form "type:residual";
 *
 * The "type" portion corresponds to one of the predefined credential
 * cache types, while the "residual" portion is specific to the
 * particular cache type.
 */

krb5_error_code krb5_cc_resolve (name, cache)
    char *name;
    krb5_ccache *cache;
{
    struct krb5_cc_typelist *tlist;
    char *pfx, *resid, *cp;
    int pfxlen;
    
    cp = strchr (name, ':');
    if (!cp)
	return KRB5_CC_BADNAME;

    pfxlen = cp - name;
    resid = name + pfxlen + 1;
	
    pfx = malloc (pfxlen+1);
    if (!pfx)
	return ENOMEM;

    memcpy (pfx, name, pfxlen);
    pfx[pfxlen] = '\0';

    *cache = (krb5_ccache) 0;

    for (tlist = cc_typehead; tlist; tlist = tlist->next) {
	if (strcmp (tlist->ops->prefix, pfx) == 0) {
	    free(pfx);
	    return (*tlist->ops->resolve)(cache, resid);
	}
    }
    if (krb5_cc_dfl_ops && !strcmp (pfx, krb5_cc_dfl_ops->prefix)) {
	free (pfx);
	return (*krb5_cc_dfl_ops->resolve)(cache, resid);
    }
    free(pfx);
    return KRB5_CC_UNKNOWN_TYPE;
}
