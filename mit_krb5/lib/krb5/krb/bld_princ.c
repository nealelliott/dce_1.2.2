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
 * $Log: bld_princ.c,v $
 * Revision 1.1.2.1  1996/06/05  20:57:15  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:55:08  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/bld_princ.c
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Build a principal from a list of strings
 */

/* Need <krb5/k5-config.h> for HAVE_STDARG_H */
#include "k5-int.h"

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h>
#endif

krb5_error_code
krb5_build_principal_va(context, princ, rlen, realm, ap)
    krb5_context context;
    krb5_principal princ;
    int rlen;
    const char *realm;
    va_list ap;
{
    register int i, count = 0;
    register char *next;
    char *tmpdata;
    krb5_data *data;

    /* guess at an initial sufficent count of 2 pieces */
    count = 2;

    /* get space for array and realm, and insert realm */
    data = (krb5_data *) malloc(sizeof(krb5_data) * count);
    if (data == 0)
	return ENOMEM;
    krb5_princ_set_realm_length(context, princ, rlen);
    tmpdata = malloc(rlen);
    if (!tmpdata) {
	free (data);
	return ENOMEM;
    }
    krb5_princ_set_realm_data(context, princ, tmpdata);
    memcpy(tmpdata, realm, rlen);

    /* process rest of components */

    for (i = 0, next = va_arg(ap, char *);
	 next;
	 next = va_arg(ap, char *), i++) {
	if (i == count) {
	    /* not big enough.  realloc the array */
	    krb5_data *p_tmp;
	    p_tmp = (krb5_data *) realloc((char *)data,
					  sizeof(krb5_data)*(count*2));
	    if (!p_tmp) {
	    free_out:
		    while (--i >= 0)
			krb5_xfree(data[i].data);
		    krb5_xfree(data);
		    krb5_xfree(tmpdata);
		    return (ENOMEM);
	    }
	    count *= 2;
	    data = p_tmp;
	}

	data[i].length = strlen(next);
	data[i].data = strdup(next);
	if (!data[i].data)
	    goto free_out;
    }
    princ->data = data;
    princ->length = i;
    princ->type = KRB5_NT_UNKNOWN;
    princ->magic = KV5M_PRINCIPAL;
    return 0;
}

krb5_error_code
#ifdef HAVE_STDARG_H
krb5_build_principal(krb5_context context,  krb5_principal * princ, int rlen,
    const char * realm, ...)
#else
krb5_build_principal(context, princ, rlen, realm, va_alist)
    krb5_context context;
    krb5_principal *princ;
    int rlen;
    const char *realm;
    va_dcl
#endif
{
    va_list ap;
    krb5_error_code retval;
    krb5_principal pr_ret = (krb5_principal)malloc(sizeof(krb5_principal_data));

    if (!pr_ret)
	return ENOMEM;

#ifdef HAVE_STDARG_H
    va_start(ap, realm);
#else
    va_start(ap);
#endif
    retval = krb5_build_principal_va(context, pr_ret, rlen, realm, ap);
    va_end(ap);
    if (retval == 0)
	*princ = pr_ret;
    return retval;
}
