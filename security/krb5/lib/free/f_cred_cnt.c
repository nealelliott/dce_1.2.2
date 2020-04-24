/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: f_cred_cnt.c,v $
 * Revision 1.1.4.2  1996/02/18  00:11:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:50  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:27:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:08:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  f_cred_cnt.c V=3 10/24/91 //littl/prgy/krb5/lib/free
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/free/f_cred_cnt.c,v $
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
 * krb5_free_cred_contents()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_f_cred_cnt_c [] =
"$Id: f_cred_cnt.c,v 1.1.4.2 1996/02/18 00:11:37 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * krb5_free_cred_contents zeros out the session key, and then frees
 * the credentials structures 
 */

void
krb5_free_cred_contents(val)
krb5_creds *val;
{
    if (val->client)
	krb5_free_principal(val->client);
    if (val->server)
	krb5_free_principal(val->server);
    if (val->keyblock.contents) {
	memset((char *)val->keyblock.contents, 0, val->keyblock.length);
	xfree(val->keyblock.contents);
    }
    if (val->ticket.data)
	xfree(val->ticket.data);
    if (val->second_ticket.data)
	xfree(val->second_ticket.data);
    if (val->addresses)
	krb5_free_addresses(val->addresses);
    if (val->authdata)
	krb5_free_authdata(val->authdata);
    return;
}
