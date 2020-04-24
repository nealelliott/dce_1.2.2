/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdc_util.h,v $
 * Revision 1.1.10.3  1996/11/13  18:06:20  arvind
 * 	CHFts20536
 * 	[1996/10/28  20:24 UTC  aha  /main/DCE_1.2.2/aha_pk10/3]
 *
 * 	#ifdef OSF_DCE_FEATURE, add &client_enc_key parameter to
 * 	check_padata()
 * 	[1996/10/18  20:22 UTC  aha  /main/DCE_1.2.2/aha_pk10/2]
 *
 * 	Revert to standard get_preauth_hint_list signature
 * 	[1996/10/15  17:30 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	Merge pre-auth and policy work from aha_pk9_5
 * 	[1996/10/15  15:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/1]
 *
 * 	CHFts20121: get Third-Party Preauthentication working again; second pass
 * 	[1996/09/26  23:25 UTC  sommerfeld  /main/DCE_1.2.2/2]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE122_PK/1]
 *
 * Revision 1.1.10.2  1996/10/03  15:00:17  arvind
 * 	merge up stuff from beta6
 * 	[1996/09/16  20:49 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE122_PK/1]
 * 
 * Revision 1.1.10.1  1996/07/08  18:36:33  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	fix realm_compare prototype.
 * 	[1996/04/26  22:53 UTC  sommerfeld  /main/sommerfeld_CHFts17874/1]
 * 
 * Revision 1.1.8.2  1996/02/18  23:03:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:46  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:45:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:38  root]
 * 
 * Revision 1.1.6.1  1993/10/14  20:02:23  sommerfeld
 * 	Workaround MIT-DCE incompatibility in tgs request
 * 	[1993/10/14  15:52:56  sommerfeld]
 * 
 * Revision 1.1.4.2  1992/12/29  14:18:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:51  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/28  20:59:19  sommerfeld
 * 	[CR3650] Add prototype for krb5_add32_nowrap.
 * 	Add ADD macro to call it.
 * 	[1992/05/27  22:28:53  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:54:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  kdc_util.h V=4 11/14/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/kdc_util.h,v $
 * $Author: arvind $
 * $Id: kdc_util.h,v 1.1.10.3 1996/11/13 18:06:20 arvind Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Declarations for policy.c
 */

#include <krb5/copyright.h>

#ifndef __KRB5_KDC_UTIL__
#define __KRB5_KDC_UTIL__

#ifdef OSF_DCE
#define OSF_DCE_ARCHAISM
#define OSF_DCE_FEATURE
#endif

#ifdef OSF_DCE_ARCHAISM
#define krb5_klog_syslog syslog
#endif


krb5_error_code check_hot_list PROTOTYPE((krb5_ticket *));
krb5_boolean realm_compare PROTOTYPE((krb5_principal, krb5_principal));
krb5_boolean dce_security_service_principal PROTOTYPE((krb5_principal));
krb5_error_code add_to_transited PROTOTYPE((krb5_data *,
					    krb5_data *,
					    krb5_principal,
					    krb5_principal,
					    krb5_principal));
krb5_error_code compress_transited PROTOTYPE((krb5_data *,
					      krb5_principal,
					      krb5_data *));
krb5_error_code concat_authorization_data PROTOTYPE((krb5_authdata **,
						     krb5_authdata **,
						     krb5_authdata ***));
krb5_error_code fetch_last_req_info PROTOTYPE((krb5_db_entry *,
					       krb5_last_req_entry ***));

krb5_error_code kdc_convert_key PROTOTYPE((krb5_keyblock *,
					   krb5_keyblock *,
					   int));
krb5_error_code kdc_process_tgs_req PROTOTYPE((krb5_kdc_req *,
					       const krb5_fulladdr *,
					       krb5_data *,
					       krb5_tkt_authent **));

krb5_error_code kdc_get_server_key PROTOTYPE((krb5_ticket *,
					      krb5_keyblock **,
					      krb5_kvno *));
    
krb5_int32 krb5_add32_nowrap PROTOTYPE ((krb5_int32, krb5_int32));
#define ADD(x,y) krb5_add32_nowrap(x,y)

krb5_error_code
get_salt_from_key PROTOTYPE((krb5_context, krb5_principal,
#ifdef OSF_DCE_ARCHAISM
			     krb5_db_entry *,
#else
			     krb5_key_data *,
#endif
			     krb5_data *));


krb5_error_code verify_kdc_body_cksum (krb5_data *pkt,
				       krb5_kdc_req *request,
				       krb5_checksum *checksum,
				       krb5_keyblock *session);

/* do_as_req.c */
krb5_error_code process_as_req PROTOTYPE((krb5_kdc_req *,
					  const krb5_fulladdr *,
					  int,
					  krb5_data ** ));

/* do_tgs_req.c */
krb5_error_code process_tgs_req PROTOTYPE((krb5_kdc_req *,
					   const krb5_fulladdr *,
					   krb5_data *,
					   krb5_data ** ));
/* dispatch.c */
krb5_error_code dispatch PROTOTYPE((krb5_data *,
				    const krb5_fulladdr *,
				    int,
				    krb5_data **));

/* network.c */
krb5_error_code listen_and_process PROTOTYPE((const char *));
krb5_error_code setup_network PROTOTYPE((const char *));
krb5_error_code closedown_network PROTOTYPE((const char *));

/* kdc_preauth.c */
const char * missing_required_preauth
    PROTOTYPE((krb5_db_entry *client, krb5_db_entry *server,
	       krb5_enc_tkt_part *enc_tkt_reply));
void get_preauth_hint_list PROTOTYPE((krb5_kdc_req * request,
				      krb5_db_entry *client,
				      krb5_db_entry *server,
				      krb5_data *e_data));
#ifdef OSF_DCE_FEATURE
krb5_error_code check_padata
    PROTOTYPE((krb5_context context, krb5_db_entry *client,
	       krb5_kdc_req *request, krb5_enc_tkt_part *enc_tkt_reply,
               krb5_keyblock *client_enc_key));
#else
krb5_error_code check_padata
    PROTOTYPE((krb5_context context, krb5_db_entry *client,
	       krb5_kdc_req *request, krb5_enc_tkt_part *enc_tkt_reply));
#endif
    
#ifdef OSF_DCE_FEATURE
krb5_error_code return_padata
    PROTOTYPE((krb5_context context, krb5_db_entry *client,
	       krb5_kdc_req *request, krb5_kdc_rep *reply,
	       krb5_key_data *client_key, krb5_keyblock *client_enc_key));
#else
krb5_error_code return_padata
    PROTOTYPE((krb5_context context, krb5_db_entry *client,
	       krb5_kdc_req *request, krb5_kdc_rep *reply,
	       krb5_key_data *client_key, krb5_keyblock *encrypting_key));
#endif
    
/* replay.c */
krb5_boolean kdc_check_lookaside PROTOTYPE((krb5_data *, krb5_data **));
void kdc_insert_lookaside PROTOTYPE((krb5_data *, krb5_data *));

/* which way to convert key? */
#define CONVERT_INTO_DB	0
#define CONVERT_OUTOF_DB 1

#define isflagset(flagfield, flag) (flagfield & (flag))
#define setflag(flagfield, flag) (flagfield |= (flag))
#define clear(flagfield, flag) (flagfield &= ~(flag))

#define realm_of_tgt(ticket) krb5_princ_realm(ticket->server)
#ifdef KRB4
krb5_error_code process_v4 PROTOTYPE((const krb5_data *,
				      const krb5_fulladdr *,
				      krb5_data **));
#else
#define process_v4(foo,bar,foobar)	KRB5KRB_AP_ERR_BADVERSION
#endif

#define norealm_salt(princ, retdata) krb5_principal2salt(&(princ)[1], retdata)

#ifndef	min
#define	min(a, b)	((a) < (b) ? (a) : (b))
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif
    
#ifdef OSF_DCE_FEATURE

#include <dce/sec_psm.h>

extern krb5_error_code prepare_enc_tp_error_edata
    PROTOTYPE((krb5_kdc_req *,
	       int, int,
	       krb5_data *,
	       krb5_db_entry *));

krb5_error_code krb5_pkinit_decode_as_req (
    sec_psm_handle_t psm_h,
    krb5_pa_data *in,		/* [in] */
    krb5_principal kdc,		/* [in] */
    krb5_principal client,	/* [in] */    
    krb5_checksum *checksum,	/* [out] */
    krb5_int32 *cusec,		/* [out] */
    krb5_timestamp *ctime,	/* [out] */
    krb5_int32 *nonce		/* [out] */
);


krb5_error_code krb5_pkinit_sign_as_rep (
    sec_psm_handle_t psm_h,
    krb5_principal kdc,
    krb5_principal client,
    krb5_enctype useetype, /* sess key type */
    krb5_keyblock **reply_keyP, /* out */
    krb5_int32 nonce,
    krb5_pa_data *out);


#endif

#ifdef OSF_DCE_ARCHAISM
/* in B6, in libkrb5.a */
krb5_error_code
krb5_principal2salt_norealm(krb5_context, krb5_const_principal, krb5_data *);
#endif

#endif /* __KRB5_KDC_UTIL__ */
