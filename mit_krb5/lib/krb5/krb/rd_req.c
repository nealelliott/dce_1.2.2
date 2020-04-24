/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: rd_req.c,v $
 * Revision 1.1.2.1  1996/06/05  21:06:13  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:50  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rd_req.c,v $
 * Revision 1.1.2.1  1996/06/05  21:06:13  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:50  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:02:50  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:18 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:42 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2  1996/05/17  14:18 UTC  mullan_s
 * 	Remove #ifdef OSF_DCE - just a bug fix, not an enhancement.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:06 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/krb/rd_req.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_rd_req()
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include "k5-int.h"
#include "auth_con.h"

/*
 *  Parses a KRB_AP_REQ message, returning its contents.
 * 
 *  server specifies the expected server's name for the ticket.
 * 
 *  keyproc specifies a procedure to generate a decryption key for the
 *  ticket.  If keyproc is non-NULL, keyprocarg is passed to it, and the result
 *  used as a decryption key. If keyproc is NULL, then fetchfrom is checked;
 *  if it is non-NULL, it specifies a parameter name from which to retrieve the
 *  decryption key.  If fetchfrom is NULL, then the default key store is
 *  consulted.
 * 
 *  returns system errors, encryption errors, replay errors
 */

krb5_error_code
krb5_rd_req(context, auth_context, inbuf, server, keytab, 
	    ap_req_options, ticket)
    krb5_context 	  context;
    krb5_auth_context  ** auth_context;
    const krb5_data 	* inbuf;
    krb5_const_principal  server;	/* XXX do we really need this */
    krb5_keytab		  keytab;
    krb5_flags		* ap_req_options;
    krb5_ticket	       ** ticket;
{
    krb5_error_code 	  retval;
    krb5_ap_req 	* request;
    krb5_auth_context	* new_auth_context;
    krb5_keytab           new_keytab = NULL;

    if (!krb5_is_ap_req(inbuf))
	return KRB5KRB_AP_ERR_MSG_TYPE;
    if (retval = decode_krb5_ap_req(inbuf, &request)) {
    	switch (retval) {
	case KRB5_BADMSGTYPE:
	    return KRB5KRB_AP_ERR_BADVERSION; 
	default:
	    return(retval);
	}
    }

    /* Get an auth context if necessary. */
    new_auth_context = NULL;
    if (*auth_context == NULL) {
	if (retval = krb5_auth_con_init(context, &new_auth_context))
	    goto cleanup_request;
        *auth_context = new_auth_context;
    }

    /* Get an rcache if necessary. */
    if (((*auth_context)->rcache == NULL) && server) {
	if (retval = krb5_get_server_rcache(context,
            krb5_princ_component(context,server,0), &(*auth_context)->rcache))
	    goto cleanup_auth_context;
    }

    /* Get a keytab if necessary. */
    if (keytab == NULL) {
	if (retval = krb5_kt_default(context, &new_keytab))
	    goto cleanup_auth_context;
	keytab = new_keytab;
    }

    retval = krb5_rd_req_decoded(context, auth_context, request, server, 
				 keytab, ap_req_options, ticket);

    if (new_keytab != NULL)
        (void) krb5_kt_close(context, new_keytab);

cleanup_auth_context:
    if (new_auth_context && retval) {
	krb5_auth_con_free(context, new_auth_context);
	*auth_context = NULL;
    }

cleanup_request:
    krb5_free_ap_req(context, request);
    return retval;
}

