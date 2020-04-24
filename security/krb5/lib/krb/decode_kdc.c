/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decode_kdc.c,v $
 * Revision 1.1.8.1  1996/10/03  15:02:05  arvind
 * 	Deal with API changes.
 * 	[1996/09/16  21:02 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:13:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:25  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:58:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:42  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:54:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  decode_kdc.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/decode_kdc.c,v $
 * $Author: arvind $
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
 * krb5_decode_kdc_rep() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_decode_kdc_c[] =
"$Id: decode_kdc.c,v 1.1.8.1 1996/10/03 15:02:05 arvind Exp $";
#endif	/* !lint & !SABER */


#include <krb5/krb5.h>
#include <krb5/asn1.h>

#include <krb5/ext-proto.h>

/*
 Takes a KDC_REP message and decrypts encrypted part using etype and
 *key, putting result in *rep.
 dec_rep->client,ticket,session,last_req,server,caddrs
 are all set to allocated storage which should be freed by the caller
 when finished with the response.

 If the response isn't a KDC_REP (tgs or as), it returns an error from
 the decoding routines (usually ISODE_50_LOCAL_ERR_BADDECODE).

 returns errors from encryption routines, system errors
 */

krb5_error_code
krb5_decode_kdc_rep(krb5_data * enc_rep,
		    const krb5_keyblock * key,
		    const krb5_enctype etype,
		    krb5_kdc_rep ** dec_rep)
{
    krb5_error_code retval;
    krb5_kdc_rep *local_dec_rep;

    if (krb5_is_as_rep(enc_rep))
	retval = decode_krb5_as_rep(enc_rep, &local_dec_rep);
    else if (krb5_is_tgs_rep(enc_rep))
	retval = decode_krb5_tgs_rep(enc_rep, &local_dec_rep);
    else
	return KRB5KRB_AP_ERR_MSG_TYPE;

    if (retval)
	return retval;

    if (local_dec_rep->enc_part.etype != etype) {
	krb5_free_kdc_rep(local_dec_rep);
	return KRB5_WRONG_ETYPE;
    }
    if (retval = krb5_kdc_rep_decrypt_proc(NULL, key, 0, local_dec_rep)) {
	krb5_free_kdc_rep(local_dec_rep);
	return(retval);
    }
    *dec_rep = local_dec_rep;
    return 0;
}

