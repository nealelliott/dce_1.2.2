/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mk_req.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:00:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:59  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:57:15  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  mk_req.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/mk_req.c,v $
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
 * krb5_mk_req() routine.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_mk_req_c[] =
"$Id: mk_req.c,v 1.1.6.2 1996/02/18 00:14:18 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

/*
 Formats a KRB_AP_REQ message into outbuf.

 server specifies the principal of the server to receive the message; if
 credentials are not present in the credentials cache for this server, the
 TGS request with default parameters is used in an attempt to obtain
 such credentials, and they are stored in ccache.

 kdc_options specifies the options requested for the 
 ap_req_options specifies the KRB_AP_REQ options desired.

 checksum specifies the checksum to be used in the authenticator.

 The outbuf buffer storage is allocated, and should be freed by the
 caller when finished.

 returns system errors
*/

extern krb5_flags krb5_kdc_default_options;

krb5_error_code
krb5_mk_req(server, ap_req_options, checksum, ccache, outbuf)
krb5_const_principal server;
const krb5_flags ap_req_options;
const krb5_checksum *checksum;
krb5_ccache ccache;
krb5_data *outbuf;
{
    krb5_error_code retval;
    krb5_creds creds;

    /* obtain ticket & session key */

    memset((char *)&creds, 0, sizeof(creds));
    creds.server = (krb5_principal) server;
    if (retval = krb5_cc_get_principal(ccache, &creds.client))
	return(retval);
    /* creds.times.endtime = 0; -- memset 0 takes care of this
     				   zero means "as long as possible" */
    /* creds.keyblock.keytype = 0; -- as well as this.
       				      zero means no session keytype
				      preference */

    if (retval = krb5_get_credentials(krb5_kdc_default_options,
				      ccache,
				      &creds))
	return(retval);

    retval = krb5_mk_req_extended(ap_req_options,
				  checksum,
				  krb5_kdc_default_options,
				  0,	/* no sequence number */
				  0,	/* no sub-key */
				  ccache,
				  &creds,
				  0, 	/* We don't need the authenticator */
				  outbuf);
    /* creds.server and the rest of the creds.* fields are filled in
       by the ccache fetch or the kdc fetch, so we should allow it to be
       freed */
    krb5_free_cred_contents(&creds);
    return retval;
}
