/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bld_pr_ext.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:33  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:53:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  bld_pr_ext.c V=1 06/06/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/bld_pr_ext.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Build a principal from a list of lengths and strings
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_bld_princ_c [] =
"$Id: bld_pr_ext.c,v 1.1.6.2 1996/02/18 00:13:21 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#if defined(__STDC__) || defined(STDARG_PROTOTYPES) || defined(ultrix)
#include <stdarg.h>
#else
#include <varargs.h>
#endif

krb5_error_code
#if defined(__STDC__) || defined(STDARG_PROTOTYPES) || defined(ultrix)
krb5_build_principal_ext(krb5_principal *princ, int rlen,
			 const char *realm, ...)
#else
krb5_build_principal_ext(princ, rlen, realm, va_alist)
krb5_principal *princ;
int rlen;
const char *realm;
va_dcl
#endif
{
    va_list ap;
    register int i, count = 0, size;
    register char *next;
    krb5_principal princ_ret;

#if defined(__STDC__) || defined(STDARG_PROTOTYPES) || defined(ultrix)
    va_start(ap, realm);
#else
    va_start(ap);
#endif
    /* count up */
    while (va_arg(ap, int) != 0) {
	va_arg(ap, char *);		/* pass one up */
	count++;
    }
    va_end(ap);

    /* we do a 2-pass to avoid the need to guess on allocation needs
       cf. bld_princ.c */
    /* get space for array and realm, and insert realm */
    princ_ret = (krb5_principal) malloc(sizeof(*princ_ret) * (count + 2));
    if (!princ_ret)
	return ENOMEM;
    if (!(princ_ret[0] = (krb5_data *) malloc(sizeof(*princ_ret[0])))) {
	xfree(princ_ret);
	return ENOMEM;
    }
    princ_ret[0]->length = rlen;
    princ_ret[0]->data = malloc(rlen);
    if (!princ_ret[0]->data) {
	xfree(princ_ret[0]);
	xfree(princ_ret);
	return ENOMEM;
    }	
    memcpy(princ_ret[0]->data, realm, rlen);

    /* process rest of components */
#if defined(__STDC__) || defined(STDARG_PROTOTYPES) || defined(ultrix)
    va_start(ap, realm);
#else
    va_start(ap);
#endif
    for (i = 1; i <= count; i++) {
	if (!(princ_ret[i] =
	      (krb5_data *) malloc(sizeof(*princ_ret[i])))) {
	free_out:
	    for (i--; i >= 0; i--) {
		xfree(princ_ret[i]->data);
		xfree(princ_ret[i]);
	    }
	    xfree(princ_ret);
	    va_end(ap);
	    return (ENOMEM);
	}
	size = va_arg(ap, int);
	next = va_arg(ap, char *);
	princ_ret[i]->length = size;
	princ_ret[i]->data = malloc(size);
	if (!princ_ret[i]->data) {
	    xfree(princ_ret[i]);
	    goto free_out;
	}
	memcpy(princ_ret[i]->data, next, size);
    }
    princ_ret[count+1] = 0;
    va_end(ap);
    *princ = princ_ret;
    return 0;
}
