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
 * $Log: princ_comp.c,v $
 * Revision 1.1.2.1  1996/06/05  21:05:18  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:01  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/princ_comp.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * compare two principals, returning a krb5_boolean true if equal, false if
 * not.
 */

#include "k5-int.h"

krb5_boolean
krb5_realm_compare(context, princ1, princ2)
    krb5_context context;
    krb5_const_principal princ1;
    krb5_const_principal princ2;
{
    if (krb5_princ_realm(context, princ1)->length != 
	krb5_princ_realm(context, princ2)->length ||
	memcmp (krb5_princ_realm(context, princ1)->data, 
	 	krb5_princ_realm(context, princ2)->data,
		krb5_princ_realm(context, princ2)->length))
	return FALSE;

    return TRUE;
}

krb5_boolean
krb5_principal_compare(context, princ1, princ2)
    krb5_context context;
    krb5_const_principal princ1;
    krb5_const_principal princ2;
{
    register int i;
    krb5_int32 nelem;

    nelem = krb5_princ_size(context, princ1);
    if (nelem != krb5_princ_size(context, princ2))
	return FALSE;

    if (! krb5_realm_compare(context, princ1, princ2))
	return FALSE;

    for (i = 0; i < (int) nelem; i++) {
	register const krb5_data *p1 = krb5_princ_component(context, princ1, i);
	register const krb5_data *p2 = krb5_princ_component(context, princ2, i);
	if (p1->length != p2->length ||
	    memcmp(p1->data, p2->data, p1->length))
	    return FALSE;
    }
    return TRUE;
}
