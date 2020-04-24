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
 * $Log: gssapi-supp.c,v $
 * Revision 1.1.8.2  1996/10/03  14:49:24  arvind
 * 	Don't fall off the end of gss_DES_MAC
 * 	[1996/09/16  23:35 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	plug Memory-leak in gssapi key-schedule calculation
 * 	[1996/03/11  19:23 UTC  pdd  /main/HPDCE02/pdd_CHFts17691/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:14  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/09/29  20:48 UTC  jrr  /main/HPDCE02/2]
 *
 * 	Merge
 * 	[1995/12/08  17:18:24  root]
 *
 * Revision 1.1.8.1  1996/05/10  13:14:39  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/11  20:01 UTC  pdd
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 	[1995/09/29  20:48 UTC  jrr  /main/HPDCE02/2]
 * 
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 	[1995/09/29  20:46 UTC  jrr  /main/HPDCE02/jrr_dcecp_01/1]
 * 
 * 	Merge
 * 	[1994/11/02  21:42 UTC  hopkins  /main/HPDCE02/1]
 * 
 * 	Merge
 * 	[1994/11/02  21:39 UTC  hopkins  /main/hopkins_eslit/2]
 * 
 * 	HP revision /main/HPDCE02/pdd_CHFts17691/1  1996/03/11  19:23 UTC  pdd
 * 	plug Memory-leak in gssapi key-schedule calculation
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  20:48 UTC  jrr
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_01/1  1995/09/29  20:46 UTC  jrr
 * 	Change dce_svc_printf's to DCE_SVC_DEBUG's to increase performance
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/02  21:42 UTC  hopkins
 * 	Merge
 * 
 * 	HP revision /main/hopkins_eslit/2  1994/11/02  21:39 UTC  hopkins
 * 	Merge
 * 	[1996/05/09  20:48:40  arvind]
 * 
 * Revision 1.1.2.7  1994/10/07  18:04:23  wray
 * 	CR 12565
 * 	[1994/10/07  18:04:02  wray]
 * 
 * Revision 1.1.2.6  1994/08/22  13:27:01  wray
 * 	CRs 11376, 11762, 11763, 11765
 * 	[1994/08/22  13:24:02  wray]
 * 
 * Revision 1.1.2.5  1994/06/28  20:00:46  wray
 * 	GSSAPI BL3 (OT 10824)
 * 	[1994/06/28  19:24:21  wray]
 * 
 * Revision 1.1.2.4  1994/06/17  18:41:53  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:08:28  devsrc]
 * 
 * Revision 1.1.2.3  1994/03/23  16:47:25  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:43:07  wray]
 * 
 * 	GSSAPI BL2 (OT#10179)
 * 
 * Revision 1.1.2.2  1994/03/10  21:11:03  wray
 * 	Fix for RIOS (OT#10117)
 * 	[1994/03/10  21:09:18  wray]
 * 
 * Revision 1.1.2.1  1994/03/09  15:50:48  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:38:51  wray]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1994 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Generic Security Service API (GSSAPI)
 *
 * ABSTRACT:	This module contains the low-level "glue" code, connecting
 *              GSSAPI to the security component.
 *	
 * ENVIRONMENT:	portable
 *
 * AUTHOR:
 *              John C Wray
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	February 1994
 *
 */


#include <gssapi-int.h>
#include <gssapi-support.h>


#if defined(DEBUG)
#define DEBUG_OID
#define DEBUG_LIST
#define DEBUG_TIME
#define DEBUG_RGY
#define DEBUG_CRED
#define DEBUG_AUTH
#define DEBUG_PROT
#endif

#if defined(DEBUG) || defined(DEBUG_OID) || defined(DEBUG_TIME) \
    || defined(DEBUG_LIST) || defined(DEBUG_RGY) || defined(DEBUG_CRED) \
    || defined(DEBUG_AUTH) || defined(DEBUG_PROT)
#include <stdio.h>
#include <stdlib.h>
#define DEBUG_ANY
#endif

DCE_SVC_DEFINE_HANDLE(gss__svc_handle, gss__svc_table, "gss")



static /* const */ gss_OID_desc GSSDCE_C_OID_KRBV5_DES_ID_desc = {
5, "\x2b\x5\x1\x5\x2"	    /* 1.3.5.1.5.2				    */
};

gss_OID GSSDCE_C_OID_KRBV5_DES_ID = &GSSDCE_C_OID_KRBV5_DES_ID_desc;


static /* const */ gss_OID_desc GSSDCE_C_OID_KRBV5_DES_RFC_desc = {
9, "\x2a\x83\x48\x86\xf7\x12\x1\x2\x2"	    /* 1.2.840.113554.1.2.2	    */
};

gss_OID GSSDCE_C_OID_KRBV5_DES_RFC = &GSSDCE_C_OID_KRBV5_DES_RFC_desc;



static /* const */ gss_OID_desc GSSDCE_C_OID_DCE_KRBV5_DES_desc = {
4, "\x2b\x18\x09\x08"	    /* 1.3.24.9.8				    */
};

gss_OID GSSDCE_C_OID_DCE_KRBV5_DES = &GSSDCE_C_OID_DCE_KRBV5_DES_desc;

gss_OID GSSDCE_C_OID_KRBV5_DES = 
#ifdef USE_RFC_PROTOCOL_ID
    &GSSDCE_C_OID_KRBV5_DES_RFC_desc;
#else
    &GSSDCE_C_OID_KRBV5_DES_ID_desc;
#endif

static /* const */ gss_OID_desc GSSDCE_C_OID_DCENAME_desc = {
4, "\x2b\x18\x09\x09"	    /* 1.3.24.9.9				    */
};

gss_OID GSSDCE_C_OID_DCENAME = &GSSDCE_C_OID_DCENAME_desc;


static sec_des_block zero_block = {0,0,0,0,0,0,0,0};

static unsigned char double_zero_block[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



/****************************************************************************/
/*									    */
/*			  GSS_SERVER_REGISTER_AUTH_INFO			    */
/*									    */
/****************************************************************************/

void gss_server_register_auth_info(
      /* [in] */ unsigned_char_p_t server_princ_name,
      /* [in] */ unsigned32 auth_svc,
      /* [in] */ rpc_auth_key_retrieval_fn_t get_key_func,
      /* [in] */ idl_void_p_t arg,
      /* [out] */ unsigned32 *st ) {

    rpc_server_register_auth_info(server_princ_name,
				  auth_svc,
				  get_key_func,
				  arg,
				  st);
}


/****************************************************************************/
/*									    */
/*			  LOOKUP_REG_SERVER_ARG 			    */
/*									    */
/****************************************************************************/

void lookup_reg_server_arg(sec_rgy_name_t server_name,
		          void ** arg,
		          error_status_t * err) {
/* Return the "arg" associated with the specified registered server.	    */
    rpc_auth_key_retrieval_fn_t get_key_fn;

    *err = sec_krb_inq_server(server_name,
			      &get_key_fn,
			      arg);

}




/****************************************************************************/
/*									    */
/*			    HASH_CHANNEL_BINDINGS			    */
/*									    */
/****************************************************************************/

void hash_channel_bindings(gss_channel_bindings_t channel_bindings,
                           unsigned char chan_binding_hash[16])
/* Convert a gss_channel_bindings_t structure into an 16-byte hash value.    */
{
    MD5_CTX context;
    unsigned char temp32[4];


    if (channel_bindings != NULL) {

	MD5Init(&context);

	temp32[0] = (channel_bindings->initiator_addrtype) & 0x0ff;
	temp32[1] = (channel_bindings->initiator_addrtype >> 8) & 0x0ff;
	temp32[2] = (channel_bindings->initiator_addrtype >> 16) & 0x0ff;
	temp32[3] = (channel_bindings->initiator_addrtype >> 24) & 0x0ff;
        MD5Update(&context,
                  temp32,
                  4);

	temp32[0] = (channel_bindings->initiator_address.length) & 0x0ff;
	temp32[1] = (channel_bindings->initiator_address.length >> 8) & 0x0ff;
	temp32[2] = (channel_bindings->initiator_address.length >> 16) & 0x0ff;
	temp32[3] = (channel_bindings->initiator_address.length >> 24) & 0x0ff;
        MD5Update(&context,
                  temp32,
                  4);

        if (channel_bindings->initiator_address.value != NULL) {
            MD5Update(&context,
		  (unsigned char *)channel_bindings->initiator_address.value,
                      channel_bindings->initiator_address.length);
        };

	temp32[0] = (channel_bindings->acceptor_addrtype) & 0x0ff;
	temp32[1] = (channel_bindings->acceptor_addrtype >> 8) & 0x0ff;
	temp32[2] = (channel_bindings->acceptor_addrtype >> 16) & 0x0ff;
	temp32[3] = (channel_bindings->acceptor_addrtype >> 24) & 0x0ff;
        MD5Update(&context,
                  temp32,
                  4);


	temp32[0] = (channel_bindings->acceptor_address.length) & 0x0ff;
	temp32[1] = (channel_bindings->acceptor_address.length >> 8) & 0x0ff;
	temp32[2] = (channel_bindings->acceptor_address.length >> 16) & 0x0ff;
	temp32[3] = (channel_bindings->acceptor_address.length >> 24) & 0x0ff;
        MD5Update(&context,
                  temp32,
                  4);
        if (channel_bindings->acceptor_address.value != NULL) {
            MD5Update(&context,
                      (unsigned char *)channel_bindings->acceptor_address.value,
                      channel_bindings->acceptor_address.length);
        };


	temp32[0] = (channel_bindings->application_data.length) & 0x0ff;
	temp32[1] = (channel_bindings->application_data.length >> 8) & 0x0ff;
	temp32[2] = (channel_bindings->application_data.length >> 16) & 0x0ff;
	temp32[3] = (channel_bindings->application_data.length >> 24) & 0x0ff;
        MD5Update(&context,
                  temp32,
                  4);
        if (channel_bindings->application_data.value != NULL) {
            MD5Update(&context,
                      (unsigned char *)channel_bindings->application_data.value,
                      channel_bindings->application_data.length);
        };

	MD5Final(&context);

	memcpy(chan_binding_hash, context.digest, 16);
    } else {
	memset(chan_binding_hash, 0, 16);
    };

}



/****************************************************************************/
/*									    */
/*			GSS__FIXUP_PRIV_KEY_INFO			    */
/*									    */
/****************************************************************************/

static error_status_t gss__fixup_priv_key_info(ctx_info * ctx_ptr) {
    error_status_t st;
    int i, j;
    unsigned char mask;


/* This routine takes the auth_session_key and calculates the		    */
/* priv_session_key and corresponding key-schedule.			    */


    if (ctx_ptr->priv_key_schedule != NULL) return 0;


#   if defined(DEBUG_PROT)
	fprintf(stderr, "Constructing priv key-schedule for context\n");
	fprintf(stderr, "Auth key: ");
	for (i=0; i<8; i++) {
	    fprintf(stderr, 
		    "%2.2X", 
		    ((sec_des_key *)(ctx_ptr->auth_session_key->contents))->bits[i]);
	};
	fprintf(stderr, "\n");
	fflush(stderr);
#   endif

    krb5_copy_keyblock(ctx_ptr->auth_session_key,
		       &ctx_ptr->priv_session_key);

/* The key used for privacy is formed by XORing the authentication key      */
/* with 0xf0f0f0f0f0f0f0f0:                                                 */

    for (i=0; i<=7; i++) {
        ctx_ptr->priv_session_key->contents[i] ^= 0xf0;
    };
 

    if ((ctx_ptr->priv_key_schedule = 
	    (sec_des_block *)(malloc(sizeof(sec_des_key_schedule)))
	) == NULL) {
#	if defined(DEBUG_PROT)
	    fprintf(stderr, "No memory to allocate privacy key-schedule\n");
	    fflush(stderr);
#	endif
	return rpc_s_no_memory;
    };
    if ((st = sec_des_key_sched((sec_des_key *)(ctx_ptr->priv_session_key->contents),
			        ctx_ptr->priv_key_schedule)) != rpc_s_ok) {
	free(ctx_ptr->priv_key_schedule);
#	if defined(DEBUG_PROT)
	    fprintf(stderr,
		    "sec_des_key_sched failed for privacy key (%lu)\n",
		    st);
	    fflush(stderr);
#	endif
	return st;
    };
#   if defined(DEBUG_PROT)
	fprintf(stderr, "Privacy key-schedule: ");
	for (i=0; i<16; i++) {
	    fprintf(stderr, "\n%2.2u: ", i);
	    for (j=0; j<8; j++) {
		fprintf(stderr, "%2.2X", ctx_ptr->priv_key_schedule[i].bits[j]);
	    };
	};
	fprintf(stderr, "\n");
	fflush(stderr);
#   endif

    return rpc_s_ok;
}


/****************************************************************************/
/*									    */
/*			GSS__FIXUP_SAFE_KEY_INFO			    */
/*									    */
/****************************************************************************/

static error_status_t gss__fixup_safe_key_info(ctx_info * ctx_ptr) {
    error_status_t st;
    int i, j;
    unsigned char mask;


/* This routine takes the auth_session_key and calculates the		    */
/* safe_session_key and corresponding key-schedule.			    */


    if (ctx_ptr->safe_key_schedule != NULL) return 0;


#   if defined(DEBUG_PROT)
	fprintf(stderr, "Constructing safe key-schedule for context\n");
	fprintf(stderr, "Auth key: ");
	for (i=0; i<8; i++) {
	    fprintf(stderr, 
		    "%2.2X", 
		    ((sec_des_key *)(ctx_ptr->auth_session_key->contents))->bits[i]);
	};
	fprintf(stderr, "\n");
	fflush(stderr);
#   endif

    krb5_copy_keyblock(ctx_ptr->auth_session_key,
		       &ctx_ptr->safe_session_key);

/* The key used for integrity is the same as the session key...		    */


    if ((ctx_ptr->safe_key_schedule = 
	    (sec_des_block *)(malloc(sizeof(sec_des_key_schedule)))
	) == NULL) {
#	if defined(DEBUG_PROT)
	    fprintf(stderr, "No memory to allocate integrity key-schedule\n");
	    fflush(stderr);
#	endif
	return rpc_s_no_memory;
    };
    if ((st = sec_des_key_sched((sec_des_key *)(ctx_ptr->safe_session_key->contents),
			        ctx_ptr->safe_key_schedule)) != rpc_s_ok) {
	free(ctx_ptr->safe_key_schedule);
#	if defined(DEBUG_PROT)
	    fprintf(stderr,
		    "sec_des_key_sched failed for integrity key (%lu)\n",
		    st);
	    fflush(stderr);
#	endif
	return st;
    };
#   if defined(DEBUG_PROT)
	fprintf(stderr, "Integrity key-schedule: ");
	for (i=0; i<16; i++) {
	    fprintf(stderr, "\n%2.2u: ", i);
	    for (j=0; j<8; j++) {
		fprintf(stderr, "%2.2X", ctx_ptr->safe_key_schedule[i].bits[j]);
	    };
	};
	fprintf(stderr, "\n");
	fflush(stderr);
#   endif

    return rpc_s_ok;
}


/****************************************************************************/
/*									    */
/*			  GSS_ENCRYPT_DES_CBC				    */
/*									    */
/****************************************************************************/

error_status_t gss_encrypt_DES_CBC(unsigned char   iv[8],
				   unsigned char * clear_text,
				   size_t          clear_length,
				   ctx_info *      key_info,
				   int		   key_type,
				   unsigned char * cipher_text) {
    error_status_t st;

    switch(key_type) {
    case PRIVACY_KEY:
	if ((st = gss__fixup_priv_key_info(key_info)) != 0) return st;
	break;
    case INTEGRITY_KEY:
	if ((st = gss__fixup_safe_key_info(key_info)) != 0) return st;
	break;
    default:
	return gssdce_s_no_such_key;
    };

    if (clear_length % 8 != 0) {
	st = sec_s_pgmerr;
	goto gss_encrypt_DES_CBC_return;
    };

    sec_des_cbc_encrypt((sec_des_block *)clear_text,
		        (sec_des_block *)cipher_text,
			clear_length,
			((key_type == PRIVACY_KEY)
			? key_info->priv_key_schedule
			: key_info->safe_key_schedule),
			(sec_des_block *)iv,
			sec_des_encrypt);
	
    st = 0;

gss_encrypt_DES_CBC_return:
    return st;
}


/****************************************************************************/
/*									    */
/*			  GSS_DECRYPT_DES_CBC				    */
/*									    */
/****************************************************************************/

error_status_t gss_decrypt_DES_CBC(unsigned char   iv[8],
				   unsigned char * cipher_text,
				   size_t          cipher_length,
				   ctx_info *      key_info,
				   int		   key_type,
				   unsigned char * clear_text) {
    error_status_t st;

    switch (key_type) {
    case PRIVACY_KEY:
	if ((st = gss__fixup_priv_key_info(key_info)) != 0) return st;
	break;
    case INTEGRITY_KEY:
	if ((st = gss__fixup_safe_key_info(key_info)) != 0) return st;
	break;
    default:
	return gssdce_s_no_such_key;
    };

    if (cipher_length % 8 != 0) {
	st = sec_s_pgmerr;
	goto gss_decrypt_DES_CBC_return;
    };

    sec_des_cbc_encrypt((sec_des_block *)cipher_text,
		        (sec_des_block *)clear_text,
			cipher_length,
			((key_type == PRIVACY_KEY)
			 ? key_info->priv_key_schedule
			 : key_info->safe_key_schedule),
			(sec_des_block *)iv,
			sec_des_decrypt);
	
    st = 0;

gss_decrypt_DES_CBC_return:
    return st;
}


/****************************************************************************/
/*									    */
/*				GSS_DES_MAC				    */
/*									    */
/****************************************************************************/
error_status_t gss_DES_MAC(unsigned char   iv[8],
			   unsigned char * data,
			   size_t          length,
			   ctx_info *      key_info,
			   unsigned char * mac) {
    error_status_t st;
    if ((st = gss__fixup_safe_key_info(key_info)) != 0) return st;
    sec_des_cbc_cksum((sec_des_block *)data,
		      (sec_des_block *)mac,
		      length,
		      key_info->safe_key_schedule,
		      (sec_des_block *)iv);
    return 0;
}


/****************************************************************************/
/*									    */
/*			  GSS_GENERATE_RANDOM_BLOCK			    */
/*									    */
/****************************************************************************/
error_status_t gss_generate_random_block(unsigned char block[8]) {
    sec_des_generate_random_block((sec_des_block *)block);
    return 0;
}



/****************************************************************************/
/*									    */
/*			    GSS_FIXUP_MD_INIT				    */
/*									    */
/****************************************************************************/
void gss_fixup_md_init(ctx_info * ctx_ptr) {
/* Set up the initializer for MD5 hashes				    */

    error_status_t st;
    int i;
    unsigned char md_initializer[16];
    sec_des_key_schedule md_schedule;
    sec_des_key md_key;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, "Entering gss_fixup_md_init"));

    if (ctx_ptr->md_init != NULL) return; /* Nothing to do		    */
    
    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, "Copying session key"));

    for (i=0; i<sizeof(md_key); i++) {
	md_key.bits[i] = ctx_ptr->auth_session_key->contents[7-i];
    };

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, "Calculating DES key schedule"));

    if ((st = sec_des_key_sched(&md_key,
    			        (sec_des_block *)md_schedule)) != rpc_s_ok) {
	dce_svc_printf(GSS_S_FMI_KSCHED_FAILED_MSG, st);
	return;

    };


    ctx_ptr->md_init_length = 16;

    DCE_SVC_DEBUG((gss__svc_handle, gss_s_auth, svc_c_debug9, "Encrypting init-block"));

    sec_des_cbc_encrypt((sec_des_block *)double_zero_block,
		        (sec_des_block *)md_initializer,
			ctx_ptr->md_init_length,
			(sec_des_block *)md_schedule,
			&zero_block,
			sec_des_encrypt);

    
    if ((ctx_ptr->md_init = (unsigned char *) malloc(ctx_ptr->md_init_length)) == NULL) {
	dce_svc_printf(GSS_S_FMI_MD5_NOMEM_MSG);
	return;
    };

    memcpy(ctx_ptr->md_init,
	   &md_initializer,
	   ctx_ptr->md_init_length);


}



/****************************************************************************/
/*									    */
/*			    Misc debugging routines			    */
/*									    */
/****************************************************************************/


char * gss__display_krb_error(unsigned32 st) {
    switch (st) {
    case KRB5KRB_AP_ERR_BAD_INTEGRITY: return "KRB5KRB_AP_ERR_BAD_INTEGRITY";
    case KRB5KRB_AP_ERR_TKT_EXPIRED: return "KRB5KRB_AP_ERR_TKT_EXPIRED";
    case KRB5KRB_AP_ERR_TKT_NYV: return "KRB5KRB_AP_ERR_TKT_NYV";
    case KRB5KRB_AP_ERR_REPEAT: return "KRB5KRB_AP_ERR_REPEAT";
    case KRB5KRB_AP_ERR_NOT_US: return "KRB5KRB_AP_ERR_NOT_US";
    case KRB5KRB_AP_ERR_BADMATCH: return "KRB5KRB_AP_ERR_BADMATCH";
    case KRB5KRB_AP_ERR_BADADDR: return "KRB5KRB_AP_ERR_BADADDR";
    case KRB5KRB_AP_ERR_BADVERSION: return "KRB5KRB_AP_ERR_BADVERSION";
    case KRB5KRB_AP_ERR_MSG_TYPE: return "KRB5KRB_AP_ERR_MSG_TYPE";
    case KRB5KRB_AP_ERR_MODIFIED: return "KRB5KRB_AP_ERR_MODIFIED";
    case KRB5KRB_AP_ERR_BADORDER: return "KRB5KRB_AP_ERR_BADORDER";
    case KRB5KRB_AP_ERR_BADKEYVER: return "KRB5KRB_AP_ERR_BADKEYVER";
    case KRB5KRB_AP_ERR_NOKEY: return "KRB5KRB_AP_ERR_NOKEY";
    case KRB5KRB_AP_ERR_MUT_FAIL: return "KRB5KRB_AP_ERR_MUT_FAIL";
    case KRB5KRB_AP_ERR_BADDIRECTION: return "KRB5KRB_AP_ERR_BADDIRECTION";
    case KRB5KRB_AP_ERR_METHOD: return "KRB5KRB_AP_ERR_METHOD";
    case KRB5KRB_AP_ERR_BADSEQ: return "KRB5KRB_AP_ERR_BADSEQ";
    case KRB5KRB_AP_ERR_INAPP_CKSUM: return "KRB5KRB_AP_ERR_INAPP_CKSUM";
    case KRB5KRB_ERR_GENERIC: return "KRB5KRB_ERR_GENERIC";
    case KRB5KRB_ERR_FIELD_TOOLONG: return "KRB5KRB_ERR_FIELD_TOOLONG";
    default:
	return "<Unknown status code>";
    };
}

