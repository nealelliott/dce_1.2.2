/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: get_creds.c,v $
 * Revision 1.1.8.3  1996/02/18  00:14:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:39  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:59:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/15  00:03 UTC  rps
 * 	merge up to nightly
 * 	[1995/04/10  17:46 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:12 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/1  1995/03/20  14:44 UTC  mullan_s
 * 	krb5_get_credntials_dce: remove fields arg as it is no longer needed
 * 	with binary compat work. Add new arg, tgt_authdata, which is used
 * 	to find the right TGT with matching authdata.
 * 	[1995/12/08  16:59:51  root]
 * 
 * Revision 1.1.4.1  1994/09/28  21:34:53  burati
 * 	CR12179 (slowness) New rtn krb5_get_credentials_dce() w/flags arg
 * 	so callers can specify that they only want to match on time fields
 * 	[1994/09/28  21:13:05  burati]
 * 
 * Revision 1.1.2.2  1992/12/29  15:56:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  get_creds.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/get_creds.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * krb5_get_credentials()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_get_creds_c[] =
"$Id: get_creds.c,v 1.1.8.3 1996/02/18 00:14:00 marty Exp $";
#endif	/* !lint & !SABER */


/*
 Attempts to use the credentials cache or TGS exchange to get an additional
 ticket for the
 client identified by creds->client, the server identified by
 creds->server, with options options, expiration date specified in
 creds->times.endtime (0 means as long as possible), session key type
 specified in creds->keyblock.keytype (if non-zero)

 Any returned ticket and intermediate ticket-granting tickets are
 stored in ccache.

 returns errors from encryption routines, system errors
 */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

krb5_error_code krb5_get_credentials(
    const krb5_flags options,
    krb5_ccache ccache,
    krb5_creds *creds
)
{
    return krb5_get_credentials_dce(options, ccache, creds, NULL);
}

/*
 * krb5_get_credentials_dce
 *
 * Specialized krb5_get_credentials routine for DCE. This routine
 * is needed so we can find a correct TGT if we need to look
 * for one. It takes a new parameter, tgt_authdata, which is
 * expected to contain the authdata which we want to match against
 * if we need to look for a TGT. For DCE 1.1, an entry for
 * DCE authorization data will contain a V1 pac followed by a
 * version number. tgt_authdata is passed to krb5_get_cred_from_kdc_dce.
 * If there is no need to pass in tgt_authdata, NULL should be
 * passed as the arg and it will be ignored.
 * 
 * Assumptions: creds->authdata contains DCE authorization data
 * needed to find the correct service ticket. For DCE 1.1, this
 * is simply a version number. This routine will never find DCE
 * 1.0.x service tickets if that assumption holds.
 */
krb5_error_code krb5_get_credentials_dce(
    const krb5_flags options,
    krb5_ccache ccache,
    krb5_creds *creds,
    krb5_authdata **tgt_authdata
)
{
    krb5_error_code retval, rv2;
    krb5_creds **tgts;
    krb5_creds mcreds, ncreds;
    krb5_flags fields;

    if (!creds || !creds->server || !creds->client)
	    return -EINVAL;

    memset((char *)&mcreds, 0, sizeof(mcreds));
    mcreds.server = creds->server;
    mcreds.client = creds->client;
    mcreds.times.endtime = creds->times.endtime;
    mcreds.keyblock = creds->keyblock;
    mcreds.authdata = creds->authdata;

    fields = KRB5_TC_MATCH_TIMES /*XXX |KRB5_TC_MATCH_SKEY_TYPE */
        | KRB5_TC_MATCH_AUTHDATA;
    
    if (options & KRB5_GC_USER_USER) {
	/* also match on identical 2nd tkt and tkt encrypted in a
	   session key */
	fields |= KRB5_TC_MATCH_2ND_TKT|KRB5_TC_MATCH_IS_SKEY;
	mcreds.is_skey = TRUE;
	mcreds.second_ticket = creds->second_ticket;
	if (!creds->second_ticket.length)
	    return KRB5_NO_2ND_TKT;
    }

    retval = krb5_cc_retrieve_cred(ccache, fields, &mcreds, &ncreds);
    if (retval == 0) {
	    krb5_free_cred_contents(creds);
	    *creds = ncreds;
    }
    if (retval != KRB5_CC_NOTFOUND || options & KRB5_GC_CACHED)
	return retval;

    retval = krb5_get_cred_from_kdc_dce(ccache, creds, &tgts, tgt_authdata);
    if (tgts) {
	register int i = 0;
	while (tgts[i]) {
	    if (rv2 = krb5_cc_store_cred(ccache, tgts[i])) {
		retval = rv2;
		break;
	    }
	    i++;
	}
	krb5_free_tgt_creds(tgts);
    }
    if (!retval)
	retval = krb5_cc_store_cred(ccache, creds);
    return retval;
}
