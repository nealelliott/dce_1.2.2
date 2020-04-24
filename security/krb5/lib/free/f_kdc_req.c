/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: f_kdc_req.c,v $
 * Revision 1.1.4.2  1996/02/18  00:11:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:52:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:55  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:27:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:08:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  f_kdc_req.c V=3 10/24/91 //littl/prgy/krb5/lib/free
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/free/f_kdc_req.c,v $
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
 * krb5_free_kdc_req()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_f_kdc_req_c [] =
"$Id: f_kdc_req.c,v 1.1.4.2 1996/02/18 00:11:52 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

void
krb5_free_kdc_req(val)
krb5_kdc_req *val;
{
    if (val->padata)
	krb5_free_pa_data(val->padata);
    if (val->client)
	krb5_free_principal(val->client);
    if (val->server)
	krb5_free_principal(val->server);
    if (val->etype)
	xfree(val->etype);
    if (val->addresses)
	krb5_free_addresses(val->addresses);
    if (val->authorization_data.ciphertext.data)
	xfree(val->authorization_data.ciphertext.data);
    if (val->unenc_authdata)
	krb5_free_authdata(val->unenc_authdata);
    if (val->second_ticket)
	krb5_free_tickets(val->second_ticket);
    xfree(val);
    return;
}
