/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: princ_comp.c,v $
 * Revision 1.1.8.1  1996/10/03  15:04:04  arvind
 * 	Add realm compare.
 * 	[1996/09/16  21:17 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:14:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:00  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  18:00:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:06  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:57:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  princ_comp.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/princ_comp.c,v $
 * $Author: arvind $
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

#if !defined(lint) && !defined(SABER)
static char rcsid_princ_comp_c[] =
"$Id: princ_comp.c,v 1.1.8.1 1996/10/03 15:04:04 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif /* min */

krb5_boolean
krb5_realm_compare(context, princ1, princ2)
    krb5_context context;
    krb5_const_principal princ1;
    krb5_const_principal princ2;
{
    if (krb5_princ_realm(princ1)->length != 
	krb5_princ_realm(princ2)->length ||
	memcmp (krb5_princ_realm(princ1)->data, 
	 	krb5_princ_realm(princ2)->data,
		krb5_princ_realm(princ2)->length))
	return FALSE;

    return TRUE;
}

krb5_boolean
krb5_principal_compare(princ1, princ2)
krb5_const_principal princ1;
krb5_const_principal princ2;
{
    register krb5_data * const *p1, * const *p2;

    for (p1 = princ1, p2 = princ2; *p1  && *p2; p1++, p2++) {
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
