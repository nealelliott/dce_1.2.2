/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * @HP_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994 Hewlett Packard Company.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: princ_fcomp.c,v $
 * Revision 1.1.4.3  1996/02/18  00:14:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:01  marty]
 *
 * Revision 1.1.4.2  1995/12/08  18:00:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:04 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:21 UTC  rps  /main/MOTHRA_panic/3]
 * 
 * 	Avoid duplicate caching, at the cost of some more mallocs and frees
 * 	and munges.
 * 	we may wish to back this change out if it shows up on profiling..
 * 	[1995/04/03  23:51 UTC  sommerfeld  /main/MOTHRA_panic/2]
 * 
 * 	flush rcsid.
 * 	allow "primary_realm" to return NULL.
 * 	[1995/02/11  00:09 UTC  sommerfeld  /main/sommerfeld_krb5_alias_1/2]
 * 
 * 	fuzzy principal compare: first cut coding.
 * 	[1995/02/02  19:53 UTC  sommerfeld  /main/sommerfeld_krb5_alias_1/1]
 * 
 * $EndLog$
 */
/*
*/

/*  princ_comp.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/princ_fcomp.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * compare two principals, returning a krb5_boolean true if equal, false if
 * not.
 */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif /* min */

krb5_boolean krb5_realm_equiv (krb5_data * r1, krb5_data * r2)
{
    krb5_data pr1, pr2;
    krb5_boolean val;
    int isr1, isr2;		/* is rN aliased? */
    
    /* short circuit compare for normal case, before we bother with aliases */
    if (r1->length == r2->length) 
    {
	if (memcmp(r1->data,r2->data, r1->length) == 0)
	    return TRUE;
    }
    isr1 = krb5_primary_realm(r1, &pr1);
    isr2 = krb5_primary_realm(r2, &pr2);

    if ((!isr1) && (!isr2))
	return FALSE;

    if (isr1) r1 = &pr1;
    if (isr2) r2 = &pr2;

    val = FALSE;
    if (r1->length == r2->length) 
    {
	if (memcmp(r1->data,r2->data, r1->length) == 0)
	    val = TRUE;
    }
    if (isr1 && (pr1.data != NULL))
    {
	free(pr1.data);
    }

    if (isr2 && (pr2.data != NULL))
    {
	free(pr2.data);	
    }
    return val;
}

krb5_boolean
krb5_principal_equiv(princ1, princ2)
krb5_const_principal princ1;
krb5_const_principal princ2;
{
    register krb5_data * const *p1, * const *p2;

    if (!krb5_realm_equiv (*princ1, *princ2))
	return FALSE;
    
    for (p1 = princ1+1, p2 = princ2+1; *p1  && *p2; p1++, p2++) {
	if ((*p1)->length != (*p2)->length)
	    return FALSE;
	if (memcmp((*p1)->data, (*p2)->data, (*p1)->length))
						
	    return FALSE;
    }
    if (*p1 || *p2)			/* didn't both run out of components
					   at once */
	return FALSE;
    return TRUE;
}
