/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_princ.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:20  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:40  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:54:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:33  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_princ.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_princ.c,v $
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
 * krb5_copy_principal()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_princ_c[] =
"$Id: copy_princ.c,v 1.1.6.2 1996/02/18 00:13:37 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

/*
 * Copy a principal structure, with fresh allocation.
 */
krb5_error_code
krb5_copy_principal(inprinc, outprinc)
krb5_const_principal inprinc;
krb5_principal *outprinc;
{
    krb5_error_code retval;
    krb5_principal tempprinc;
    register int nelems;

    for (nelems = 0; inprinc[nelems]; nelems++);

    /* one more for a null terminated list */
    if (!(tempprinc = (krb5_principal) calloc(nelems+1, sizeof(krb5_data *))))
	return ENOMEM;

    for (nelems = 0; inprinc[nelems]; nelems++)
	if (retval = krb5_copy_data(inprinc[nelems], &tempprinc[nelems])) {
	    krb5_free_principal(tempprinc);
	    return retval;
	}

    *outprinc = tempprinc;
    return 0;
}
