/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: f_enc_kdc.c,v $
 * Revision 1.1.4.2  1996/02/18  00:11:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:52:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:52  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:27:15  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:08:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  f_enc_kdc.c V=2 10/24/91 //littl/prgy/krb5/lib/free
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/free/f_enc_kdc.c,v $
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
 * krb5_free_enc_kdc_rep_part()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_f_enc_kdc_c [] =
"$Id: f_enc_kdc.c,v 1.1.4.2 1996/02/18 00:11:41 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

void
krb5_free_enc_kdc_rep_part(val)
register krb5_enc_kdc_rep_part *val;
{
    if (val->session)
	krb5_free_keyblock(val->session);
    if (val->last_req)
	krb5_free_last_req(val->last_req);
    if (val->server)
	krb5_free_principal(val->server);
    if (val->caddrs)
	krb5_free_addresses(val->caddrs);
    xfree(val);
    return;
}
