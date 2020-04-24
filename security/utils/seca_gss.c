/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: seca_gss.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:18  root]
 * 
 * Revision 1.1.2.6  1994/06/28  20:01:03  wray
 * 	GSSAPI BL3 (OT 10824)
 * 	[1994/06/28  19:25:25  wray]
 * 
 * Revision 1.1.2.5  1994/06/17  18:43:07  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:20:23  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/02  21:18:07  mdf
 * 	     hp_sec_to_osf_3 drop, merge up with latest.
 * 	     [1994/05/24  15:21:22  mdf]
 * 
 * 	     hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.3  1994/03/23  16:47:33  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:44:10  wray]
 * 
 * 	GSSAPI BL2 (OT#10179)
 * 
 * Revision 1.1.2.2  1994/03/09  15:51:00  wray
 * 	Miscellaneous bugfixes (OT#10019)
 * 	[1994/03/09  15:39:25  wray]
 * 
 * Revision 1.1.2.1  1994/01/28  23:11:41  burati
 * 	Initial version, sec_authn restructuring (dlg_bl1)
 * 	[1994/01/20  03:30:27  burati]
 * 
 * $EndLog$
 */

#include <sec_authnp.h>
#include <gssapi-int.h>
#include <gssapi-support.h>


/****************************************************************************/
/*									    */
/*			SEC_KRB_GSS_BUILD_REP_MESSAGE			    */
/*									    */
/****************************************************************************/

error_status_t sec_krb_gss_build_rep_message(
		    unsigned32 authn_level,
		    sec_krb_timestamp ctime,
		    unsigned32 cusec,
		    krb5_keyblock * key,	    /* input		    */
		    unsigned32 seq_number,	    /* input		    */
		    sec_krb_message * message_out)
{

    sec_krb_msgstamp msgstamp;

    msgstamp.seq_number = seq_number;
    msgstamp.ctime = ctime;
    msgstamp.cusec = cusec;

    return sec_krb_build_rep_common (authn_level, key, &msgstamp,
	message_out);
}



/****************************************************************************/
/*									    */
/*			SEC_KRB_GSS_DECODE_REP_MESSAGE			    */
/*									    */
/****************************************************************************/

error_status_t sec_krb_gss_decode_rep_message(
		    unsigned32 authn_level,
		    sec_krb_message message,
		    krb5_keyblock * key,	    /* input		    */
		    sec_krb_timestamp ctime,
		    unsigned32 cusec,
		    unsigned32 * seq_number)	    /* output		    */
{
    error_status_t code;
    sec_krb_msgstamp msgstamp;
    code = sec_krb_decode_rep_common (authn_level, &message, key, &msgstamp);
    *seq_number = msgstamp.seq_number;
    if (ctime != msgstamp.ctime ||
	cusec != msgstamp.cusec) {
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
	   GSS_MSG("Timestamp mismatch (received: {%u.%u}, expected: {%u.%u})"),
	   msgstamp.ctime, msgstamp.cusec,
	   ctime, cusec);
#else
	dce_svc_printf(GSS_S_SECA_TIMESTAMP_MSG,
		       msgstamp.ctime, msgstamp.cusec,
		       ctime, cusec);
#endif
	code = gssdce_s_authentication_error;
    };
    return code;
}


/****************************************************************************/
/*									    */
/*			SEC_KRB_GSS_DECODE_MESSAGE			    */
/*									    */
/****************************************************************************/

error_status_t sec_krb_gss_decode_message(
		    sec_krb_message * message,
		    unsigned char ** ticket_name,
		    sec_id_pac_t * client_pac,
		    rpc_authz_cred_handle_t * client_creds,
		    sec_krb_parsed_name * server_name,
		    unsigned32 *authn_proto,	    /* output		    */
		    unsigned32 *authz_proto,	    /* output		    */
		    krb5_keyblock ** key,	    /* output		    */
		    unsigned32 * expiration,	    /* output		    */
		    gss_buffer_t bindings,	    /* output		    */
		    unsigned32 * context_flags,     /* output		    */
		    unsigned32 *out_seq_number,	    /* output		    */
		    sec_krb_timestamp * ctime,
		    unsigned32 * cusec)
{


    error_status_t st;
    unsigned char * ump;
    unsigned char * checksum_contents;
    unsigned int checksum_length;
    unsigned int checksum_type;
    krb5_tkt_authent * authent = NULL;
    idl_pkl_t * pickled_pac;
    krb5_principal ps_princ;
    boolean32 is_ps;
    sec_id_pac_t loc_pac;
    sec_krb_msgstamp msgstamp;
    unsigned32  authz_fmt = SEC_AUTHZ_FMT_V1_1;
    
#   if defined(DEBUG_AUTH)
	fprintf(stderr, "Info - Entering sec_krb_gss_decode_msg\n"); 
	fflush(stderr);
#   endif

#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_sev_debug9,
		   GSS_MSG("Entered sec_krb_gss_decode_message"));
#else
    dce_svc_printf(GSS_S_SECA_SKGDM_MSG);
#endif

    if (bindings == NULL) {
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
		       GSS_MSG("NULL bindings in sec_krb_gss_decode_message!"));
#else
	dce_svc_printf(GSS_S_SECA_NULL_BINDINGS_MSG);
#endif

    } else {
	bindings->value = NULL;
	bindings->length = 0;
    };

#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_sev_debug9,
		   GSS_MSG("Calling sec_krb_decode_message_common"));
#else
    dce_svc_printf(GSS_S_SECA_SKDMC_MSG);
#endif

    st = sec_krb_decode_message_common (message,
	authz_fmt,
	server_name,
	ticket_name,
	client_pac,
	client_creds,
	expiration,
	&msgstamp,
	&authent,
	authz_proto);

#   if defined(DEBUG_AUTH)
	fprintf(stderr, "Info - Returned from sec_krb_decode_msg_common\n"); 
	fflush(stderr);
#   endif
#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_sev_debug9,
		   GSS_MSG("Returned from sec_krb_gss_decode_msg_common"));
#else
    dce_svc_printf(GSS_S_SECA_SKDMC_RET_MSG);
#endif        

    if (BAD_STATUS(&st)) {
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Error - krb5_decode_msg_common failed\n"); 
	    fprintf(stderr, "        Code: %lu: %s\n",
		    st, gss__display_krb_error(st));

	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
		       GSS_MSG("krb_decode_msg_common failed (%x: %s)"),
		       st, gss__display_krb_error(st));
#else
	dce_svc_printf(GSS_S_SECA_SKDMC_FAILED_MSG,
		       st, gss__display_krb_error(st));
#endif
	goto gss_decode_message_return;
    };

#   if defined(DEBUG_AUTH)
	fprintf(stderr, "Info - Extracting checksum\n"); 
	fflush(stderr);
#   endif
#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_sev_debug9,
		   GSS_MSG("Extracting checksum..."));
#else
    dce_svc_printf(GSS_S_SECA_EXT_CKSUM_MSG);
#endif
    if (authent == NULL || authent->authenticator == NULL) {
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Error - No checksum!\n"); 
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
		       GSS_MSG("No checksum found in authenticator!"));
#else
	dce_svc_printf(GSS_S_SECA_NO_CKSUM_MSG);
#endif
	st = gssdce_s_authentication_error;
	goto gss_decode_message_return;
    };

    if (authent->authenticator->checksum) {

#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Info - Got (presumably) DCE-style checksum\n"); 
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_debug9,
		       GSS_MSG("Found authenticator checksum"));
#else
	dce_svc_printf(GSS_S_SECA_GOT_CKSUM_MSG);
#endif
	checksum_contents = 
	    (unsigned char *)authent->authenticator->checksum->contents;
	checksum_length = authent->authenticator->checksum->length;
	checksum_type = authent->authenticator->checksum->checksum_type;

#	if defined(DEBUG_AUTH)
	    fprintf(stderr, 
		    "Info - Checksum is type %4.4x, %d bytes\n",
		    checksum_type,
		    checksum_length); 
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_debug9,
		       GSS_MSG("Checksum type: %4.4x, %d bytes"),
		       checksum_type, checksum_length);
#else
	dce_svc_printf(GSS_S_SECA_CKSUM_TYPE_MSG,
		       checksum_type,
		       checksum_length);
#endif

	if (checksum_type != GSSAPI_CHECKSUM_TYPE) {
	    st = rpc_s_invalid_checksum;
#	    if defined(DEBUG_AUTH)
		fprintf(stderr, "Error - checksum type wrong\n"); 
		fflush(stderr);
#	    endif
#ifdef GSS_SVC_CODE
	    gss_svc_printf(svc_c_sev_error,
			   GSS_MSG("Error - checksum type wrong"));
#else
	    dce_svc_printf(GSS_S_SECA_CKSUM_TYPE_WRONG_MSG);
#endif
	    goto gss_decode_message_return;
	};

	if (checksum_length < 8) {
	    st = rpc_s_invalid_checksum;
#	    if defined(DEBUG_AUTH)
		fprintf(stderr, "Error - Checksum too short\n"); 
		fflush(stderr);
#	    endif
#ifdef GSS_SVC_CODE
	    gss_svc_printf(svc_c_sev_error,
			   GSS_MSG("Error - Checksum too short"));
#else
	    dce_svc_printf(GSS_S_SECA_CKSUM_TOO_SHORT_MSG);
#endif
	    goto gss_decode_message_return;
	};
    
/* 0..3:    Length of bindings hash (16)				    */
/* 4..19:   MD5 hash of bindings					    */
/* 20..23;  context flags						    */

	ump = checksum_contents;

	bindings->length = ump[0] +
			   (ump[1] << 8) +
			   (ump[2] << 16) +
			   (ump[3] << 24);

	ump += 4;


	if (checksum_length < 8 + bindings->length) {
	    st = rpc_s_invalid_checksum;
#	    if defined(DEBUG_AUTH)
		fprintf(stderr, "Error - checksum length wrong\n"); 
		fflush(stderr);
#	    endif
#ifdef GSS_SVC_CODE
	    gss_svc_printf(svc_c_sev_error,
			   GSS_MSG("Error - Checksum length wrong"));
#else
	    dce_svc_printf(GSS_S_SECA_CKSUM_LENGTH_WRONG_MSG);
#endif
	    goto gss_decode_message_return;
	};

	if (bindings->length != 0) {    
#	    if defined(DEBUG_AUTH)
		fprintf(stderr, 
			"Info - Bindings length = %d\n",
			bindings->length); 
		fflush(stderr);
#	    endif

	    if ((bindings->value = (void *)malloc(bindings->length)) == NULL) {
		st = rpc_s_no_memory;
#		if defined(DEBUG_AUTH)
		    fprintf(stderr, "Error - Couldn't extract bindings\n"); 
		    fflush(stderr);
#		endif
#ifdef GSS_SVC_CODE
		gss_svc_printf(svc_c_sev_error,
			       GSS_MSG("No memory for bindings (%d bytes)"),
			       bindings->length);
#else
		dce_svc_printf(GSS_S_SECA_NOMEM1_MSG, bindings->length);
#endif
		goto gss_decode_message_return;
	    };

	    memcpy(bindings->value, ump, bindings->length);
	    ump += bindings->length;
	} else {
#	    if defined(DEBUG_AUTH)
		fprintf(stderr, "Error - Zero-length bindings\n"); 
		fflush(stderr);
#	    endif
#ifdef GSS_SVC_CODE
	    gss_svc_printf(svc_c_sev_error,
			   GSS_MSG("Error - Zero-length channel bindings"));
#else
	    dce_svc_printf(GSS_S_SECA_ZERO_CHANNEL_BINDING_MSG);
#endif
	    bindings->value = NULL;
	    st = gssdce_s_authentication_error;
	    goto gss_decode_message_return;

	};

	*authn_proto = rpc_c_authn_dce_secret;

	*context_flags = ump[0] +
		        (ump[1] << 8) +
			(ump[2] << 16) +
			(ump[3] << 24);

	ump += 4;

    } else {

#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
		       GSS_MSG("No checksum found in authenticator"));
#else
	dce_svc_printf(GSS_S_SECA_NO_CKSUM_MSG);
#endif
	st = rpc_s_invalid_checksum;
	goto gss_decode_message_return;

    };

#   if defined(DEBUG_AUTH)
	fprintf(stderr, "Info - Processed checksum\n");
	fflush(stderr);
#   endif
#ifdef GSS_SVC_CODE
    gss_svc_printf(svc_c_sev_debug9,
		   GSS_MSG("Processed checksum"));
#else
    dce_svc_printf(GSS_S_SECA_DONE_CKSUM_MSG);
#endif
    if (authent->ticket == NULL) {
#       if defined(DEBUG_AUTH)
            fprintf(stderr, "Error - Ticket is empty\n");
            fflush(stderr);
#       endif
        st = gssdce_s_authentication_error;
        goto gss_decode_message_return;
    } else if (authent->ticket->enc_part2 == NULL) {
#       if defined(DEBUG_AUTH)
            fprintf(stderr, "Error - enc_part2 of ticket is empty\n");
            fflush(stderr);
#       endif
        st = gssdce_s_authentication_error;
        goto gss_decode_message_return;
    };

    
#   if defined(DEBUG_AUTH)
	fprintf(stderr, "Info - Setting return parameters...\n");
	fflush(stderr);
#   endif

    if (ctime) {
	*ctime = msgstamp.ctime;
    };
    if (cusec) {
	*cusec = msgstamp.cusec;
    };
    if (out_seq_number) {
	*out_seq_number = msgstamp.seq_number;
    };
    if (authent->ticket->enc_part2->session->length != sizeof(sec_des_key)) {
	st = rpc_s_auth_nokey;
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Error - No session key\n"); 
	    fflush(stderr);
#	endif
	goto gss_decode_message_return;
    };

    st = krb5_copy_keyblock(authent->ticket->enc_part2->session,
			    key);	/* Return session keyblock to	    */
					/* caller.			    */

gss_decode_message_return:

    if (authent) krb5_free_tkt_authent(authent);

    if (BAD_STATUS(&st)) {
	if (bindings->value) free(bindings->value);
	bindings->value = NULL;
	bindings->length = 0;
    };

    return st;
}


/****************************************************************************/
/*									    */
/*			SEC_KRB_GSS_BUILD_MESSAGE			    */
/*									    */
/****************************************************************************/
		    
error_status_t sec_krb_gss_build_message(
		    sec_krb_ccache ccache,	/* Credential cache	    */
		    sec_krb_cred cred,		/* Cred for target	    */
		    unsigned32 format,		/* Either KRB_FORMAT or	    */
						/* DCE_FORMAT.  Currently   */
						/* makes no difference.	    */
		    unsigned32 authn_level,
		    unsigned32 authz_proto,
		    unsigned32 seq_number,	/* input		    */
		    gss_buffer_t bindings,	/* External, authenticator  */
		    unsigned32 context_flags,   /* input		    */
		    krb5_keyblock ** key,	/* Output		    */
		    sec_krb_timestamp * ctime,
		    unsigned32 * cusec,
		    sec_krb_message * message_out)
{

    krb5_checksum send_cksum;
    krb5_checksum * send_cksum_ptr;
    krb5_creds * kcred = (krb5_creds*) cred;
    unsigned char out_data[1024];
    unsigned char * mp;
    error_status_t st;
    krb5_authenticator authent;
    sec_krb_msgstamp msgstamp;
    unsigned32  authz_fmt = SEC_AUTHZ_FMT_V1_1;

    st = error_status_ok;

/* Build either a GSS or a Kerberos V5 message, depending on authz_proto    */
    if (authn_level == rpc_c_authn_level_none) {
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Error - authn_level_none unsupported\n");
	    fflush(stderr);
#	endif
	return rpc_s_unsupported_authn_level;
    };
/* We're only concerned with a non-null authn_level			    */

    if (format == KRB_FORMAT) {
/* Kerberos compatibility mode.						    */
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Info - Kerberos V5 authentication specified\n");
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_debug9,
		       GSS_MSG("Building KRB5 authenticator"));
#else
	dce_svc_printf(GSS_S_SECA_BUILDING_KRB_AUTHENT_MSG);
#endif
    } else if (format == DCE_FORMAT) {
/* DCE mode.								    */
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Info - DCE authentication specified\n");
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_debug9,
		       GSS_MSG("Building DCE authenticator"));
#else
	dce_svc_printf(GSS_S_SECA_BUILDING_DCE_AUTHENT_MSG);
#endif
    } else {
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Error - Unsupported authentication format\n");
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
		       GSS_MSG("Unsupported authenticator format"));
#else
	dce_svc_printf(GSS_S_SECA_BAD_CKSUM_TYPE_MSG);
#endif
	return sec_s_authz_unsupp;
    };    

/* Currently, both DCE and Kerberos-compatibility use the same kind of	    */
/* checksum, calculated below.  If this changes, the different checksum	    */
/* calculations should be mnoved into the conditional above...		    */


/* Pack the channel bindings into the checksum:				    */
/* For DCE GSSAPI we use checksum type 0x8003 (0x8001 = DG-RPC, 0x8002 =    */
/* CN-RPC).  The format of the checksum block is:			    */
/*									    */
/* 0..3:    Length of bindings hash (16)				    */
/* 4..19:   MD5 hash of bindings					    */
/* 20..23;  flags							    */


    mp = out_data;	

    *mp++ =  bindings->length & 0x00ff;
    *mp++ =  (bindings->length >> 8) & 0x00ff;
    *mp++ =  (bindings->length >> 16) & 0x00ff;
    *mp++ =  (bindings->length >> 24) & 0x00ff;

    memcpy(mp,
	   bindings->value,
	   bindings->length);
    mp += bindings->length;

    *mp++ =  context_flags & 0x00ff;
    *mp++ =  (context_flags >> 8) & 0x00ff;
    *mp++ =  (context_flags >> 16) & 0x00ff;
    *mp++ =  (context_flags >> 24) & 0x00ff;

    send_cksum.checksum_type = GSSAPI_CHECKSUM_TYPE;
    send_cksum.length = mp - out_data;
    send_cksum.contents = out_data;
    send_cksum_ptr = &send_cksum;

    msgstamp.ctime = 0;
    msgstamp.cusec = 0;
    msgstamp.seq_number = seq_number;
    
    st = sec_krb_build_message_common (ccache, 
	cred, 
	&send_cksum, 
	authz_proto,
	authz_fmt,
	&msgstamp,
	message_out);
    
    if (st == error_status_ok) {
	*cusec = msgstamp.cusec;
	*ctime = msgstamp.ctime;
	st = krb5_copy_keyblock(&kcred->keyblock,
				key);	/* Return session keyblock to	    */
					/* caller.			    */

	if (st != error_status_ok) {
#	    if defined(DEBUG_AUTH)
		fprintf(stderr, "Error - krb5_copy_keyblock returned error\n");
		fprintf(stderr, "        Code: %lu\n", st);
		fflush(stderr);
#	    endif
#ifdef GSS_SVC_CODE
	    gss_svc_printf(svc_c_sev_error,
			   GSS_MSG("krb5_copy_keyblock failed(%X)"),
			   st);
#else
	    dce_svc_printf(GSS_S_SECA_COPY_KBLOCK_FAILED_MSG, st);
#endif
	    xfree(message_out->data);
	    message_out->data = NULL;
	    message_out->length = 0;
	};
    } else {
#	if defined(DEBUG_AUTH)
	    fprintf(stderr, "Error - sec_krb_build_message_common returned error\n");
	    fprintf(stderr, "        Code: %lu\n", st);
	    fflush(stderr);
#	endif
#ifdef GSS_SVC_CODE
	gss_svc_printf(svc_c_sev_error,
		       GSS_MSG("sec_krb_build_messaage_common failed (%X)"),
		       st);
#else
	dce_svc_printf(GSS_S_SECA_SKBMC_FAILED_MSG, st);
#endif
    };
    
    memset (&authent, 0, sizeof(authent)); /* Scribble over authenticator   */
					   /* in case it gets scavenged.    */
    return st;
}			       
