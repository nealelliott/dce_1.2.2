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
 * $Log: copy_princ.c,v $
 * Revision 1.1.2.1  1996/06/05  20:59:26  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:56:56  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/copy_princ.c
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
 * krb5_copy_principal()
 */

#include "k5-int.h"

/*
 * Copy a principal structure, with fresh allocation.
 */
krb5_error_code
krb5_copy_principal(context, inprinc, outprinc)
    krb5_context context;
    krb5_const_principal inprinc;
    krb5_principal *outprinc;
{
    register krb5_principal tempprinc;
    register int i, nelems;

    tempprinc = (krb5_principal)malloc(sizeof(krb5_principal_data));

    if (tempprinc == 0)
	return ENOMEM;

#ifdef HAVE_C_STRUCTURE_ASSIGNMENT
    *tempprinc = *inprinc;	/* Copy all of the non-allocated pieces */
#else
    memcpy(tempprinc, inprinc, sizeof(krb5_principal_data));
#endif

    nelems = (int) krb5_princ_size(context, inprinc);
    tempprinc->data = malloc(nelems * sizeof(krb5_data));

    if (tempprinc->data == 0) {
	free((char *)tempprinc);
	return ENOMEM;
    }

    for (i = 0; i < nelems; i++) {
	int len = krb5_princ_component(context, inprinc, i)->length;
	krb5_princ_component(context, tempprinc, i)->length = len;
	if (((krb5_princ_component(context, tempprinc, i)->data =
	      malloc(len)) == 0) && len) {
	    while (--i >= 0)
		free(krb5_princ_component(context, tempprinc, i)->data);
	    free (tempprinc->data);
	    free (tempprinc);
	    return ENOMEM;
	}
	if (len)
	    memcpy(krb5_princ_component(context, tempprinc, i)->data,
		   krb5_princ_component(context, inprinc, i)->data, len);
    }

    tempprinc->realm.data =
	    malloc(tempprinc->realm.length = inprinc->realm.length);
    if (!tempprinc->realm.data && tempprinc->realm.length) {
	    for (i = 0; i < nelems; i++)
		    free(krb5_princ_component(context, tempprinc, i)->data);
	    free(tempprinc->data);
	    free(tempprinc);
	    return ENOMEM;
    }
    if (tempprinc->realm.length)
	memcpy(tempprinc->realm.data, inprinc->realm.data,
	       inprinc->realm.length);
    
    *outprinc = tempprinc;
    return 0;
}
