/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gssapi-support.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:25  root]
 * 
 * Revision 1.1.2.3  1994/06/17  18:41:54  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  16:47:26  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:43:10  wray]
 * 
 * 	GSSAPI BL2 (OT#10179)
 * 
 * Revision 1.1.2.1  1994/03/09  15:50:50  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:37:53  wray]
 * 
 * $EndLog$
 */
#include "gssapi-int.h"
#include "sec_krb_util.h"

/*             This document is OSF Confidential			    */

#define PRIVACY_KEY 0
#define INTEGRITY_KEY 1

extern void gss_server_register_auth_info(
      /* [in] */ unsigned_char_p_t server_princ_name,
      /* [in] */ unsigned32 auth_svc,
      /* [in] */ rpc_auth_key_retrieval_fn_t get_key_func,
      /* [in] */ idl_void_p_t arg,
      /* [out] */ unsigned32 *st
  );


extern void lookup_reg_server_arg(sec_rgy_name_t server_name,
			          void ** arg,
			          error_status_t * err);
/* Return the "arg" associated with the specified registered server.	    */


extern void hash_channel_bindings(gss_channel_bindings_t channel_bindings,
			          unsigned char chan_binding_hash[16]);
/* Convert a gss_channel_bindings_t structure into an 8-byte hash value.    */

#define DCE_FORMAT 0
#define KRB_FORMAT 1

extern error_status_t sec_krb_gss_build_message(
		    sec_krb_ccache ccache,	/* Credential cache	    */
		    sec_krb_cred cred,		/* Cred for target	    */
		    unsigned32 format,
		    unsigned32 authn_level,
		    unsigned32 authz_proto,
		    unsigned32 seq_number,	/* input		    */
		    gss_buffer_t bindings,	/* input		    */
		    unsigned32 context_flags,   /* input		    */
		    krb5_keyblock ** key,	/* output		    */
		    sec_krb_timestamp * ctime,
		    unsigned32 * cusec,
		    sec_krb_message * message_out);


extern error_status_t sec_krb_gss_decode_message(
		    sec_krb_message * message,
		    unsigned char ** client_name,
		    sec_id_pac_t * client_pac,
		    rpc_authz_cred_handle_t * client_creds,
		    sec_krb_parsed_name * server_name,
		    unsigned32 *authn_level,	    /* output		    */
		    unsigned32 *authz_proto,	    /* output		    */
		    krb5_keyblock ** key,	    /* output		    */
		    unsigned32 * expiration,	    /* output		    */
		    gss_buffer_t bindings,	    /* output		    */
		    unsigned32 * context_flags,	    /* output		    */
		    unsigned32 *out_seq_number,	    /* output		    */
		    sec_krb_timestamp * ctime,
		    unsigned32 * cusec);

extern error_status_t sec_krb_gss_build_rep_message(
		    unsigned32 authn_level,
		    sec_krb_timestamp ctime,
		    unsigned32 cusec,
		    krb5_keyblock * key,	    /* input		    */
		    unsigned32 seq_number,	    /* input		    */
		    sec_krb_message * message_out);


extern error_status_t sec_krb_gss_decode_rep_message(
		    unsigned32 authn_level,
		    sec_krb_message message,
		    krb5_keyblock * key,	    /* input		    */
		    sec_krb_timestamp ctime,
		    unsigned32 cusec,
		    unsigned32 * seq_number);	    /* output		    */


extern error_status_t gss_encrypt_DES_CBC(unsigned char   iv[8],
					  unsigned char * clear_text,
					  size_t	  clear_length,
					  ctx_info *      key_info,
					  int		  key_type,
					  unsigned char * cipher_text);

extern error_status_t gss_decrypt_DES_CBC(unsigned char	  iv[8],
					  unsigned char * cipher_text,
					  size_t	  cipher_length,
					  ctx_info *      key_info,
					  int		  key_type,
					  unsigned char * clear_text);

extern error_status_t gss_DES_MAC(unsigned char	  iv[8],
				  unsigned char * data,
			          size_t          length,
			          ctx_info *	  key_info,
			          unsigned char * mac);

extern error_status_t gss_generate_random_block(unsigned char block[8]);

extern void gss_fixup_md_init(ctx_info * ctx_ptr);

#define gss_MD5Init(x) MD5Init(x)
#define gss_MD5Update(x,y,z) MD5Update(x,y,z)
#define gss_MD5Final(x) MD5Final(x)

/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI-SUPPORT.H */
/*  *8    18-MAR-1994 13:53:24 WRAY "For BL2" */
/*  *7    11-MAR-1994 14:35:17 WRAY "OSF BL1 version" */
/*  *6    20-JAN-1994 12:24:00 WRAY "" */
/*  *5    17-JAN-1994 12:37:19 WRAY "" */
/*  *4    18-NOV-1993 15:29:51 WRAY "" */
/*  *3    11-NOV-1993 14:21:51 WRAY "Fix kerb/DCE parameters" */
/*  *2    22-SEP-1993 16:42:22 WRAY "Incorporate into ODE" */
/*  *1     1-MAR-1993 10:33:06 WRAY "Prototypes for GSSAPI-SUPPORT.C" */
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI-SUPPORT.H */
