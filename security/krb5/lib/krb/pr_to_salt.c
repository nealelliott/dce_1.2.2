/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pr_to_salt.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:57  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:00:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:03  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:57:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  pr_to_salt.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/pr_to_salt.c,v $
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
 * krb5_principal2salt()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_pr_to_salt_c[] =
"$Id: pr_to_salt.c,v 1.1.6.2 1996/02/18 00:14:24 marty Exp $";
#endif  /* !lint & !SABER */


#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * Convert a krb5_principal into the default salt for that principal.
 */

krb5_error_code
krb5_principal2salt(pr, ret)
krb5_const_principal pr;
krb5_data *ret;
{
    int size, offset;
    krb5_data * const * prp;
    

    if (pr == 0) {
        ret->length = 0;
        ret->data = 0;
    } else {
        for (size = 0, prp = pr; *prp; prp++)
            size += (*prp)->length;

        ret->length = size;
        if (!(ret->data = malloc (size+1)))
	    return ENOMEM;

        for (offset=0, prp=pr; *prp; prp++)
        {
            memcpy(&ret->data[offset],(*prp)->data, (*prp)->length);
            offset += (*prp)->length;
        }
    }
    return 0;
}
