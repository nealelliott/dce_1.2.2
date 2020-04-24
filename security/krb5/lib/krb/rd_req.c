/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rd_req.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:09  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:58:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rd_req.c V=5 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/rd_req.c,v $
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
 * krb5_rd_req()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rd_req_c[] =
"$Id: rd_req.c,v 1.1.6.2 1996/02/18 00:14:37 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/asn1.h>
#include <krb5/ext-proto.h>

/*
 *  Parses a KRB_AP_REQ message, returning its contents.
 * 
 *  server specifies the expected server's name for the ticket.
 * 
 *  sender_addr specifies the address(es) expected to be present in the
 *  ticket.
 * 
 *  rcache specifies a replay detection cache used to store authenticators and
 *  server names
 * 
 *  keyproc specifies a procedure to generate a decryption key for the
 *  ticket.  If keyproc is non-NULL, keyprocarg is passed to it, and the result
 *  used as a decryption key. If keyproc is NULL, then fetchfrom is checked;
 *  if it is non-NULL, it specifies a parameter name from which to retrieve the
 *  decryption key.  If fetchfrom is NULL, then the default key store is
 *  consulted.
 * 
 *  authdat is set to point at allocated storage structures; the caller
 *  should free them when finished. 
 * 
 *  returns system errors, encryption errors, replay errors
 */

krb5_error_code
krb5_rd_req(inbuf, server, sender_addr, fetchfrom, keyproc, keyprocarg,
            rcache, authdat)
const krb5_data *inbuf;
krb5_const_principal server;
const krb5_address *sender_addr;
krb5_const_pointer fetchfrom;
krb5_rdreq_key_proc keyproc;
krb5_pointer keyprocarg;
krb5_rcache rcache;
krb5_tkt_authent **authdat;
{
    krb5_error_code retval;
    krb5_ap_req *request;

    if (!krb5_is_ap_req(inbuf))
        return KRB5KRB_AP_ERR_MSG_TYPE;
    if (retval = decode_krb5_ap_req(inbuf, &request)) {
        return(retval);
    }

    retval = krb5_rd_req_decoded(request, server, sender_addr, fetchfrom,
                                 keyproc, keyprocarg, rcache, authdat);
    krb5_free_ap_req(request);
    return retval;
}

