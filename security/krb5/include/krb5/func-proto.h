/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: func-proto.h,v $
 * Revision 1.1.14.1  1996/10/03  14:56:22  arvind
 * 	put context back in free_etype_info
 * 	[1996/09/16  21:57 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/3]
 *
 * 	bring in prototypes for code imported from beta6
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
 *
 * Revision 1.1.12.3  1996/02/18  23:02:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:00  marty]
 * 
 * Revision 1.1.12.2  1995/12/08  17:42:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/15  00:00 UTC  rps
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:09 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	Add krb5_get_cred_from_kdc_dce.
 * 	[1995/03/20  14:41 UTC  mullan_s  /main/mullan_mothra_bin_compat2/2]
 * 
 * 	krb5_get_credentials_dce: delete flags arg, add tgt_authdata arg.
 * 	[1995/02/03  15:43 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 * 
 * 	Add prototypes for fuzzy realm compare functions.
 * 	[1995/04/03  23:49 UTC  sommerfeld  /main/MOTHRA_panic/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:09 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/2  1995/03/20  14:41 UTC  mullan_s
 * 	Add krb5_get_cred_from_kdc_dce.
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/1  1995/02/03  15:43 UTC  mullan_s
 * 	krb5_get_credentials_dce: delete flags arg, add tgt_authdata arg.
 * 	[1995/12/08  16:57:05  root]
 * 
 * Revision 1.1.8.1  1994/10/25  22:02:44  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes: add/delete
 * 	parameters from functions to modularize code.
 * 	[1994/10/25  22:02:19  mullan_s]
 * 
 * Revision 1.1.4.4  1994/09/28  21:34:52  burati
 * 	CR12179 (slowness) New rtn krb5_get_credentials_dce() w/flags arg
 * 	[1994/09/28  21:12:58  burati]
 * 
 * Revision 1.1.4.3  1994/08/04  16:13:07  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:19  mdf]
 * 
 * Revision 1.1.4.2  1994/05/16  18:53:34  ahop
 * 	Provide second workaround to RIOS pre-processor bug
 * 	[1994/05/16  17:58:29  ahop]
 * 
 * 	Provide work-around to RIOS pre-processor bug
 * 	[1994/05/16  17:26:59  ahop]
 * 
 * Revision 1.1.4.1  1994/05/11  19:16:23  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:13:25  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  13:59:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  func-proto.h
**
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
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
 * Function prototypes for Kerberos V5 library.
 */

#ifndef KRB5_FUNC_PROTO__
#define KRB5_FUNC_PROTO__

/* For Kerberos V5 Beta 1 compatibility */
#ifndef KRB5_BETA1
#define KRB5_BETA1
#endif

/* libkrb.spec */
krb5_error_code krb5_kdc_rep_decrypt_proc
	PROTOTYPE((krb5_context,
		   const krb5_keyblock *,
		   krb5_const_pointer,
		   krb5_kdc_rep * ));
krb5_error_code krb5_encode_ticket
	PROTOTYPE((const krb5_ticket *,
		   krb5_data ** ));
krb5_error_code krb5_encrypt_tkt_part
	PROTOTYPE((const krb5_keyblock *,
		   krb5_ticket * ));
krb5_error_code krb5_decrypt_tkt_part
	PROTOTYPE((const krb5_keyblock *,
		   krb5_ticket * ));
krb5_error_code krb5_get_cred_from_kdc_dce
        PROTOTYPE((krb5_ccache,         /* not const, as reading may save
                                           state */
                   krb5_creds *,
                   krb5_creds ***,
                   krb5_authdata ** ));
krb5_error_code krb5_get_cred_from_kdc
	PROTOTYPE((krb5_ccache,		/* not const, as reading may save
					   state */
		   krb5_creds *,
		   krb5_creds *** ));
void krb5_free_tgt_creds
	PROTOTYPE((krb5_creds ** ));	/* XXX too hard to do with const */

#define	KRB5_GC_USER_USER	1	/* want user-user ticket */
#define	KRB5_GC_CACHED		2	/* want cached ticket only */

krb5_error_code krb5_get_credentials_dce
	PROTOTYPE((const krb5_flags,
		   krb5_ccache,
		   krb5_creds *,
                   krb5_authdata ** ));
krb5_error_code krb5_get_credentials
	PROTOTYPE((const krb5_flags,
		   krb5_ccache,
		   krb5_creds * ));
krb5_error_code krb5_mk_req
	PROTOTYPE((krb5_const_principal,
		   const krb5_flags,
		   const krb5_checksum *,
		   krb5_ccache,
		   krb5_data * ));

#ifdef KRB5_BETA1
krb5_error_code krb5_mk_req_extended
        PROTOTYPE((const krb5_flags,
                   const krb5_checksum *,
                   const krb5_flags,
                   krb5_int32,
                   krb5_keyblock *,
                   krb5_ccache,
                   krb5_creds *,
                   krb5_authenticator *,
                   krb5_data * ));
#else
krb5_error_code krb5_mk_req_extended
        PROTOTYPE((const krb5_flags,
                   const krb5_checksum *,
                   const krb5_flags,
                   krb5_int32,
                   krb5_keyblock **,
                   krb5_ccache,
                   krb5_creds *,
                   krb5_authenticator *,
                   krb5_data * ));
#endif /* KRB5_BETA1 */

krb5_error_code krb5_rd_req_simple
	PROTOTYPE((const krb5_data *,
		   krb5_const_principal,
		   const krb5_address *,
		   krb5_tkt_authent ** ));
krb5_error_code krb5_mk_rep
	PROTOTYPE((const krb5_ap_rep_enc_part *,
		   const krb5_keyblock *,
		   krb5_data *));
krb5_error_code krb5_rd_rep
	PROTOTYPE((const krb5_data *,
		   const krb5_keyblock *,
		   krb5_ap_rep_enc_part **));
krb5_error_code krb5_mk_error
	PROTOTYPE((const krb5_error *,
		   krb5_data * ));
krb5_error_code krb5_rd_error
	PROTOTYPE((const krb5_data *,
		   krb5_error ** ));
krb5_error_code krb5_rd_safe
	PROTOTYPE((const krb5_data *,
		   const krb5_keyblock *,
		   const krb5_address *,
		   const krb5_address *,
		   krb5_int32, krb5_int32,
		   krb5_rcache,
		   krb5_data * ));
krb5_error_code krb5_rd_priv
	PROTOTYPE((const krb5_data *,
		   const krb5_keyblock *,
		   const krb5_address *,
		   const krb5_address *,
		   krb5_int32, krb5_int32,
		   krb5_pointer,
		   krb5_rcache,
		   krb5_data * ));
krb5_error_code krb5_parse_name
	PROTOTYPE((const char *,
		   krb5_principal * ));
krb5_error_code krb5_unparse_name
	PROTOTYPE((krb5_const_principal,
		   char ** ));
krb5_error_code krb5_unparse_name_ext
	PROTOTYPE((krb5_const_principal,
		   char **,
		   int *));
krb5_boolean krb5_address_search
	PROTOTYPE((const krb5_address *,
		   krb5_address * const *));
krb5_boolean krb5_address_compare
	PROTOTYPE((const krb5_address *,
		   const krb5_address *));
int krb5_address_order
	PROTOTYPE((const krb5_address *,
		   const krb5_address *));
krb5_boolean krb5_realm_compare
	PROTOTYPE((krb5_context,
		   krb5_const_principal,
		   krb5_const_principal));
krb5_boolean krb5_principal_compare
	PROTOTYPE((krb5_const_principal,
		   krb5_const_principal));
int krb5_fulladdr_order
	PROTOTYPE((const krb5_fulladdr *,
		   const krb5_fulladdr *));
krb5_error_code krb5_copy_keyblock
    PROTOTYPE((const krb5_keyblock *,
	       krb5_keyblock **));
krb5_error_code krb5_copy_keyblock_contents
    PROTOTYPE((const krb5_keyblock *,
	       krb5_keyblock *));
krb5_error_code krb5_copy_creds
    PROTOTYPE((const krb5_creds *,
	       krb5_creds **));
krb5_error_code krb5_copy_data
    PROTOTYPE((const krb5_data *,
	       krb5_data **));
krb5_error_code krb5_copy_principal
    PROTOTYPE((krb5_const_principal,
	       krb5_principal *));
krb5_error_code krb5_copy_addresses
    PROTOTYPE((krb5_address * const *,
	       krb5_address ***));
krb5_error_code krb5_copy_ticket
    PROTOTYPE((const krb5_ticket *, krb5_ticket **));
krb5_error_code krb5_copy_authdata
    PROTOTYPE((krb5_authdata * const *,
	       krb5_authdata ***));
krb5_error_code krb5_copy_authenticator
    PROTOTYPE((const krb5_authenticator *,
	       krb5_authenticator **));
krb5_error_code krb5_copy_checksum
    PROTOTYPE((const krb5_checksum *,
	       krb5_checksum **));
void krb5_init_ets PROTOTYPE((void));
krb5_error_code krb5_generate_subkey
    PROTOTYPE((const krb5_keyblock *, krb5_keyblock **));
krb5_error_code krb5_generate_seq_number
    PROTOTYPE((const krb5_keyblock *, krb5_int32 *));
krb5_error_code krb5_get_server_rcache
#ifdef KRB5_BETA1
    PROTOTYPE((const char *, krb5_rcache *));
#else
    PROTOTYPE((const krb5_data *, krb5_rcache *));
#endif /* KRB5_BETA1 */
krb5_error_code krb5_build_principal_ext
    STDARG_P((krb5_principal *, int, const char *, ...));
krb5_error_code krb5_build_principal
    STDARG_P((krb5_principal *, int, const char *, ...));
#ifdef va_start
/* XXX depending on varargs include file defining va_start... */
krb5_error_code krb5_build_principal_va
    PROTOTYPE((krb5_principal *, int, const char *, va_list));
#endif

krb5_error_code krb5_425_conv_principal
	PROTOTYPE((const char *name, const char *instance, const char *realm,
		   krb5_principal *princ));

/* libkt.spec */
krb5_error_code krb5_kt_register
	PROTOTYPE((krb5_kt_ops * ));
krb5_error_code krb5_kt_resolve
	PROTOTYPE((const char *,
		   krb5_keytab * ));
krb5_error_code krb5_kt_default_name
	PROTOTYPE((char *,
		   int ));
krb5_error_code krb5_kt_default
	PROTOTYPE((krb5_keytab * ));
krb5_error_code krb5_kt_free_entry
	PROTOTYPE((krb5_keytab_entry * ));
/* remove and add are functions, so that they can return NOWRITE
   if not a writable keytab */
krb5_error_code krb5_kt_remove_entry
	PROTOTYPE((krb5_keytab,
		   krb5_keytab_entry * ));
krb5_error_code krb5_kt_add_entry
	PROTOTYPE((krb5_keytab,
		   krb5_keytab_entry * ));
krb5_error_code krb5_principal2salt
	PROTOTYPE((krb5_const_principal, krb5_data *));

/* librc.spec--see rcache.h */

/* libcc.spec */
krb5_error_code krb5_cc_resolve
	PROTOTYPE((char *,
		   krb5_ccache * ));
krb5_error_code krb5_cc_generate_new
	PROTOTYPE((krb5_cc_ops *,
		   krb5_ccache * ));
char *krb5_cc_default_name
	PROTOTYPE((void ));
krb5_error_code krb5_cc_default
	PROTOTYPE((krb5_ccache *));

/* free_rtree.c */
void krb5_free_realm_tree
	PROTOTYPE((krb5_principal *));

/* krb5_free.c */
void krb5_free_principal
	PROTOTYPE((krb5_principal ));
void krb5_free_authenticator
	PROTOTYPE((krb5_authenticator * ));
void krb5_free_addresses
	PROTOTYPE((krb5_address ** ));
void krb5_free_address
	PROTOTYPE((krb5_address * ));
void krb5_free_authdata
	PROTOTYPE((krb5_authdata ** ));
void krb5_free_enc_tkt_part
	PROTOTYPE((krb5_enc_tkt_part * ));
void krb5_free_ticket
	PROTOTYPE((krb5_ticket * ));
void krb5_free_tickets
	PROTOTYPE((krb5_ticket ** ));
void krb5_free_kdc_req
	PROTOTYPE((krb5_kdc_req * ));
void krb5_free_kdc_rep
	PROTOTYPE((krb5_kdc_rep * ));
void krb5_free_last_req
	PROTOTYPE((krb5_last_req_entry ** ));
void krb5_free_enc_kdc_rep_part
	PROTOTYPE((krb5_enc_kdc_rep_part * ));
void krb5_free_error
	PROTOTYPE((krb5_error * ));
void krb5_free_ap_req
	PROTOTYPE((krb5_ap_req * ));
void krb5_free_ap_rep
	PROTOTYPE((krb5_ap_rep * ));
void krb5_free_safe
	PROTOTYPE((krb5_safe * ));
void krb5_free_priv
	PROTOTYPE((krb5_priv * ));
void krb5_free_priv_enc_part
	PROTOTYPE((krb5_priv_enc_part * ));
void krb5_free_kdc_req
	PROTOTYPE((krb5_kdc_req * ));
void krb5_free_creds
	PROTOTYPE((krb5_creds *));
void krb5_free_cred_contents
	PROTOTYPE((krb5_creds *));
void krb5_free_checksum
	PROTOTYPE((krb5_checksum *));
void krb5_free_keyblock
	PROTOTYPE((krb5_keyblock *));
void krb5_free_pa_data
	PROTOTYPE((krb5_pa_data **));
void krb5_free_ap_rep_enc_part
	PROTOTYPE((krb5_ap_rep_enc_part *));
void krb5_free_tkt_authent
	PROTOTYPE((krb5_tkt_authent *));
void krb5_free_etype_info
    PROTOTYPE((krb5_context, krb5_etype_info_entry **));

#include <krb5/widen.h>

/* Only put things which don't have pointers to the narrow types in this
   section */

krb5_error_code krb5_encode_kdc_rep
	PROTOTYPE((const krb5_msgtype,
		   const krb5_enc_kdc_rep_part *,
		   const krb5_keyblock *,
		   krb5_kdc_rep *,
		   krb5_data ** ));

krb5_error_code krb5_send_tgs
	PROTOTYPE((const krb5_flags,
		   const krb5_ticket_times *,
		   const krb5_enctype,
		   const krb5_cksumtype,
		   krb5_const_principal,
		   krb5_address * const *,
		   krb5_authdata * const *,
		   krb5_pa_data * const *,
		   const krb5_data *,
		   krb5_creds *,
		   krb5_response * ));

krb5_error_code krb5_get_in_tkt
	PROTOTYPE((krb5_context, 
		   krb5_const krb5_flags,
		   krb5_address * krb5_const *,
		   krb5_enctype *,
		   krb5_preauthtype *,
		   krb5_error_code ( * )(krb5_context,
					krb5_const krb5_enctype,
                                        krb5_data *,
                                        krb5_const_pointer,
                                        krb5_keyblock **),
		   krb5_const_pointer,
		   krb5_error_code ( * )(krb5_context,
					krb5_const krb5_keyblock *,
					krb5_const_pointer,
					krb5_kdc_rep * ),
		   krb5_const_pointer,
		   krb5_creds *,
		   krb5_ccache,
		   krb5_kdc_rep ** ));

#define krb5_do_as_request @@NOWOBSOLETE@@
#define krb5_process_as_reply @@NOWOBSOLETE@@
#define krb5_cache_as_reply @@NOWOBSOLETE@@

#if 0
krb5_error_code krb5_do_as_request
	PROTOTYPE((const krb5_flags,
		   krb5_address * const *,
                   const krb5_preauthtype,
                   krb5_pa_data *,
		   const krb5_enctype,
                   const krb5_keytype,
		   krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
                   krb5_const_pointer,
                   krb5_int32 *,
		   krb5_creds *,
		   krb5_ccache,
		   krb5_data *, 
		   krb5_data **));

krb5_error_code krb5_process_as_reply
	PROTOTYPE((const krb5_data *,
		   const krb5_int32,
		   const krb5_flags,
		   krb5_address * const *,
		   const krb5_keytype,
		   krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
		   krb5_const_pointer,
                   krb5_const_pointer,
		   krb5_error_code (* )(const krb5_keyblock *,
					krb5_const_pointer,
					krb5_kdc_rep * ),
		   krb5_const_pointer,
		   krb5_creds *,
		   krb5_ccache,
		   krb5_kdc_rep ** ));

krb5_error_code krb5_cache_as_reply
	PROTOTYPE((const krb5_kdc_rep *,
		   krb5_creds *,
		   krb5_ccache ));
#endif

krb5_error_code krb5_get_in_tkt_with_password
	PROTOTYPE((krb5_context,
		   krb5_const krb5_flags,
		   krb5_address  * krb5_const  *,
		   krb5_enctype  *,
		   krb5_preauthtype  *,
		   krb5_const char  *,
		   krb5_ccache,
		   krb5_creds  *,
		   krb5_kdc_rep  *  * ));

krb5_error_code krb5_get_in_tkt_with_skey
	PROTOTYPE((krb5_context,
		   krb5_const krb5_flags,
		   krb5_address * krb5_const *,
		   krb5_enctype *,
		   krb5_preauthtype *,
		   krb5_const krb5_keyblock *,
		   krb5_ccache,
		   krb5_creds *,
		   krb5_kdc_rep ** ));

krb5_error_code krb5_get_in_tkt_with_keytab
	PROTOTYPE((krb5_context,
		   krb5_const krb5_flags,
		   krb5_address * krb5_const *,
		   krb5_enctype *,
		   krb5_preauthtype *,
		   krb5_const krb5_keytab,
		   krb5_ccache,
		   krb5_creds *,
		   krb5_kdc_rep ** ));


#if 0

krb5_error_code krb5_get_in_tkt_with_password
	PROTOTYPE((const krb5_flags,
		   krb5_address * const *,
		   const krb5_preauthtype pre_auth_type,
		   const krb5_enctype,
		   const krb5_keytype,
		   const char *,
		   krb5_ccache,
		   krb5_creds *,
		   krb5_kdc_rep ** ));

krb5_error_code krb5_get_in_tkt_with_skey
	PROTOTYPE((const krb5_flags,
		   krb5_address * const *,
		   const krb5_preauthtype pre_auth_type,
		   const krb5_enctype,
		   const krb5_keyblock *,
		   krb5_ccache,
		   krb5_creds *,
		   krb5_kdc_rep ** ));
#endif

krb5_error_code krb5_decode_kdc_rep
	PROTOTYPE((krb5_data *,
		   const krb5_keyblock *,
		   const krb5_enctype,

		   krb5_kdc_rep ** ));

#ifdef KRB5_BETA1
typedef krb5_error_code (*krb5_rdreq_key_proc) PROTOTYPE((krb5_pointer,
                                                          krb5_principal,
                                                          krb5_keytype,
                                                          krb5_kvno,
                                                          krb5_keyblock **));
#else
typedef krb5_error_code (*krb5_rdreq_key_proc) PROTOTYPE((krb5_pointer,
                                                          krb5_principal,
                                                          krb5_kvno,
                                                          krb5_keyblock **));
#endif /* KRB5_BETA1 */

krb5_error_code krb5_rd_req
	PROTOTYPE((const krb5_data *,
		   krb5_const_principal,
		   const krb5_address *,
		   krb5_const_pointer,
		   krb5_rdreq_key_proc,
		   krb5_pointer,
		   krb5_rcache,
		   krb5_tkt_authent ** ));

krb5_error_code krb5_rd_req_decoded
	PROTOTYPE((const krb5_ap_req *,
		   krb5_const_principal,
		   const krb5_address *,
		   krb5_const_pointer,
		   krb5_rdreq_key_proc,
		   krb5_pointer,
		   krb5_rcache,
		   krb5_tkt_authent ** ));

krb5_error_code krb5_kt_read_service_key
	PROTOTYPE((krb5_pointer,
		   krb5_principal,
		   krb5_kvno,
		   krb5_keyblock **));
krb5_error_code krb5_mk_safe
	PROTOTYPE((const krb5_data *,
		   const krb5_cksumtype ,
		   const krb5_keyblock *,
		   const krb5_address *,
		   const krb5_address *,
		   krb5_int32, krb5_int32,
		   krb5_rcache,
		   krb5_data * ));
krb5_error_code krb5_mk_priv
	PROTOTYPE((const krb5_data *,
		   const krb5_enctype,
		   const krb5_keyblock *,
		   const krb5_address *,
		   const krb5_address *,
		   krb5_int32, krb5_int32,
		   krb5_rcache,
		   krb5_pointer,
		   krb5_data * ));
krb5_error_code krb5_cc_register
	PROTOTYPE((krb5_cc_ops *,
		   krb5_boolean ));

krb5_error_code krb5_sendauth PROTOTYPE((krb5_pointer,
					 char *,
					 krb5_principal,
					 krb5_principal,
					 krb5_flags,
					 krb5_checksum *,
					 krb5_creds *,
					 krb5_ccache,
					 krb5_int32 *,
					 krb5_keyblock **,
					 krb5_error **,
					 krb5_ap_rep_enc_part **));
	
krb5_error_code krb5_recvauth PROTOTYPE((krb5_pointer,
					 char *,
					 krb5_principal,
					 krb5_address *,
					 krb5_pointer,
					 krb5_rdreq_key_proc,
					 krb5_pointer,
					 char *,
					 krb5_int32 *,
					 krb5_principal*,
					 krb5_ticket **,
					 krb5_authenticator **));


#ifndef KRB5_BETA1 /* Beta 1 declares it in int-proto.h */
#ifdef NARROW_PROTOTYPES
krb5_error_code krb5_walk_realm_tree
    PROTOTYPE((const krb5_data *,
	       const krb5_data *,
	       krb5_principal **,
	       char));
#else
krb5_error_code krb5_walk_realm_tree
    PROTOTYPE((const krb5_data *,
	       const krb5_data *,
	       krb5_principal **,
	       int));
#endif
#endif /* KRB5_BETA1 */

int krb5_primary_realm
    PROTOTYPE((krb5_data *,
	       krb5_data *));

krb5_boolean krb5_realm_equiv
    PROTOTYPE((krb5_data *,
	       krb5_data *));

krb5_boolean krb5_principal_equiv
    PROTOTYPE((krb5_const_principal,
	       krb5_const_principal));

#define KRB5_REALM_BRANCH_CHAR '.'

#include <krb5/narrow.h>

#endif /* KRB5_FUNC_PROTO__ */
