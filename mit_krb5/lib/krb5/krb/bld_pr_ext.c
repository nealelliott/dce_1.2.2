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
 * $Log: bld_pr_ext.c,v $
 * Revision 1.1.2.1  1996/06/05  20:57:03  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:55:00  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/bld_pr_ext.c
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
 * Build a principal from a list of lengths and strings
 */

#include "k5-int.h"

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h>
#endif

krb5_error_code INTERFACE_C
#ifdef HAVE_STDARG_H
krb5_build_principal_ext(krb5_context context,  krb5_principal * princ, int rlen, const char * realm, ...)
#else
krb5_build_principal_ext(context, princ, rlen, realm, va_alist)
    krb5_context context;
    krb5_principal *princ;
    int rlen;
    const char *realm;
    va_dcl
#endif
{
    va_list ap;
    register int i, count = 0, size;
    register char *next;
    char *tmpdata;
    krb5_data *princ_data;
    krb5_principal princ_ret;

#ifdef HAVE_STDARG_H
    va_start(ap, realm);
#else
    va_start(ap);
#endif
    /* count up */
    while (va_arg(ap, int) != 0) {
	(void)va_arg(ap, char *);		/* pass one up */
	count++;
    }
    va_end(ap);

    /* we do a 2-pass to avoid the need to guess on allocation needs
       cf. bld_princ.c */
    /* get space for array */
    princ_data = (krb5_data *) malloc(sizeof(krb5_data) * count);
    if (!princ_data)
	return ENOMEM;
    princ_ret = (krb5_principal) malloc(sizeof(krb5_principal_data));
    if (!princ_ret) {
	krb5_xfree(princ_data);
	return ENOMEM;
    }
    princ_ret->data = princ_data;
    princ_ret->length = count;
    tmpdata = malloc(rlen+1);
    if (!tmpdata) {
	krb5_xfree(princ_data);
	krb5_xfree(princ_ret);
	return ENOMEM;
    }	
    krb5_princ_set_realm_length(context, princ_ret, rlen);
    krb5_princ_set_realm_data(context, princ_ret, tmpdata);
    memcpy(tmpdata, realm, rlen);
    tmpdata[rlen] = 0;

    /* process rest of components */
#ifdef HAVE_STDARG_H
    va_start(ap, realm);
#else
    va_start(ap);
#endif
    for (i = 0; i < count; i++) {
	size = va_arg(ap, int);
	next = va_arg(ap, char *);
	princ_data[i].length = size;
	princ_data[i].data = malloc(size+1);
	if (!princ_data[i].data)
	    goto free_out;
	memcpy(princ_data[i].data, next, size);
	princ_data[i].data[size] = 0;
    }
    va_end(ap);
    *princ = princ_ret;
    krb5_princ_type(context, princ_ret) = KRB5_NT_UNKNOWN;
    return 0;

free_out:
    while (i-- >= 0)
	krb5_xfree(princ_data[i].data);
    krb5_xfree(princ_data);
    krb5_xfree(princ_ret);
    va_end(ap);
    return ENOMEM;
}
