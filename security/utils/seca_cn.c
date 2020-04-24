/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seca_cn.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:16  root]
 * 
 * Revision 1.1.2.3  1994/09/21  19:07:43  sommerfeld
 * 	[OT10413] on client, retain & check timestamps for CN mutual authentication.
 * 	[1994/09/21  19:06:16  sommerfeld]
 * 
 * Revision 1.1.2.2  1994/06/02  21:18:05  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:21:16  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	HP revision /main/ODESSA_2/2  1994/05/04  20:52 UTC  sommerfeld
 * 	Real delegation support in RPC/sec_authn.
 * 
 * 	HP revision /main/ODESSA_2/sommerfeld_rpcdlg/1  1994/04/27  05:30 UTC  sommerfeld
 * 	rpc delegation support; partially working checkpoint
 * 
 * 	HP revision /main/ODESSA_2/1  1994/01/26  20:04  root
 * 	Move ODESSA stake from kk_beta1 to kk_final
 * 
 * $EndLog$
 */

#include <sec_authnp.h>

/*
 * S E C _ K R B _ C N _ B U I L D _ M E S S A G E
 */

/*
 * Build a connection-oriented message.  Note that in the case of authn_level
 * not equal to "none", that there is nothing in send_cksum.contents when
 * calling krb5_mk_req_extended.  If this is the correct behavior, 
 * optimizations can be made to the following routine because we probably
 * don't need out_data.
 */

error_status_t sec_krb_cn_build_message (ccache, 
                                         cred,
                                         authn_level, 
                                         authz_proto, 
					 authz_fmt,
                                         key,
					 ctime,
					 cusec,
                                         message)

    sec_krb_ccache ccache;
    sec_krb_cred cred;
    unsigned32 authn_level;
    unsigned32 authz_proto;
    unsigned32 authz_fmt;
    sec_des_key *key;
    sec_krb_timestamp *ctime;
    unsigned32 *cusec;
    sec_krb_message *message;
{
    krb5_checksum send_cksum;
    krb5_ccache kccache;
    krb5_creds *kcred = (krb5_creds *) cred;
    char out_data[1];
    rpc_mp_t mp;
    error_status_t   st;
    
    CLEAR_STATUS(&st);

    sec_authn_pvt_get_krb_ccache(ccache, authz_proto, &kccache, &st);
    if (BAD_STATUS(&st)) {
        return st;
    }

    if (authn_level == rpc_c_authn_level_none) {
	return sec_krb_build_message_none (ccache, cred, authz_proto, 
					   authz_fmt, message);
    } else {
	sec_krb_msgstamp msgstamp;
	msgstamp.seq_number = 0;
	msgstamp.ctime = 0;
	msgstamp.cusec = 0;

	/* zero length checksum */
        send_cksum.checksum_type = 0x8002;
        send_cksum.length = 0;
        send_cksum.contents = (unsigned char *)out_data;

	st = sec_krb_build_message_common (ccache, 
	    cred, 
	    &send_cksum, 
	    authz_proto,
 	    authz_fmt,					   
	    &msgstamp,
	    message);

	*ctime = msgstamp.ctime;
	*cusec = msgstamp.cusec;

        if (GOOD_STATUS(&st)) {
            assert (kcred->keyblock.length == sizeof (sec_des_key));
            memcpy (key, kcred->keyblock.contents, sizeof (sec_des_key));
            return rpc_s_ok;
        }
        else
            return st;
    }
}



/*
 * S E C _ K R B _ C N _ D E C O D E _ M E S S A G E
 */

/*
 * Decode a connection-oriented message.
 */

error_status_t sec_krb_cn_decode_message (message, 
					  authz_fmt,
                                          client_name, 
                                          client_pac, 
                                          client_creds, 
                                          server_name,
                                          authz_proto,
                                          key,
                                          expiration,
                                          ctime,
                                          cusec)
    sec_krb_message           *message;
    unsigned32                 authz_fmt;
    unsigned char             **client_name;
    sec_id_pac_t              *client_pac;
    rpc_authz_cred_handle_t  *client_creds;
    sec_krb_parsed_name       *server_name;
    unsigned32                *authz_proto;
    sec_des_key               *key;
    unsigned32                *expiration;
    sec_krb_timestamp         *ctime;
    unsigned32                *cusec;

{
    krb5_tkt_authent *authent = NULL;
    int i;
    rpc_mp_t ump;
    unsigned32  level;
    unsigned32  keytype;
    unsigned32  keylen;
    error_status_t  st;
    sec_krb_msgstamp msgstamp;
    
    *ctime = 0xdeadbeef;
    *cusec = 0xfeedface;
    
    st = sec_krb_decode_message_common (message, authz_fmt,
	server_name, client_name, client_pac, client_creds, expiration,
	&msgstamp, &authent, authz_proto);
    if (st != error_status_ok) 
    {
	return st;
    }
    if (authent == NULL) 
    {
	return st;
    }
    *ctime = msgstamp.ctime;
    *cusec = msgstamp.cusec;
    
    /* this assert call must appear as one unbroken line */
    assert (authent->ticket->enc_part2->session->length == sizeof (sec_des_key));
    memcpy (key, authent->ticket->enc_part2->session->contents,
            sizeof (sec_des_key));
    krb5_free_tkt_authent(authent);
    return rpc_s_ok;
out:
    if (authent)
        krb5_free_tkt_authent(authent);
    return st;
}


/*
 * S E C _ K R B _ C N _ B U I L D _ E R R _ M E S S A G E
 */

/*
 * Build a KRB_AP_ERROR message.
 */
error_status_t sec_krb_cn_build_err_message (authn_level,
                                             error,
                                             ctime, 
                                             cusec,
                                             err_message,
                                             key)

unsigned32              authn_level;
unsigned32              error;
sec_krb_timestamp       ctime;
unsigned32              cusec;
sec_krb_message         *err_message;
sec_des_key             *key;

{
    krb5_error_code             code;
    krb5_data                   outbuf;
    krb5_error                  dec_err;
    krb5_data                   enc_err;
    unsigned32                  wire_error;
    boolean32                   krb_error;
    rpc_mp_t                    mp;
    krb5_data                   client_name;
    krb5_data                   server_name;
    krb5_data                   *client_name_p[2];
    krb5_data                   *server_name_p[2];
    krb5_principal              client;
    krb5_principal              server;

    client_name.length = 1;
    client_name.data = "";
    client_name_p[0] = &client_name;
    client_name_p[1] = NULL;
    client = &client_name_p[0];
    if (sec__n_idents)
    {
        server_name.length = strlen 
            ((char *)sec__serv_id_table[0].server_global);
        server_name.data = (char *) sec__serv_id_table[0].server_global;
        server_name_p[0] = &server_name;
        server_name_p[1] = NULL;
        server = &server_name_p[0];
    }
    else
    {
        server_name.length = 1;
        server_name.data = "";
        server_name_p[0] = &server_name;
        server_name_p[1] = NULL;
        server = &server_name_p[0];
    }

    /*
     * Set up the output argument which will contain the formatted
     * KRB_ERROR.
     */
    outbuf.data = NULL;
    outbuf.length = 0;

    /*
     * Convert the error status code to something which can be placed
     * "on the wire".
     */
    wire_error = sec_krb_status_to_wire (error, &krb_error);

    /*
     * Build a KRB_ERROR message if the authentication level is
     * anything other than "none".
     */
    if (authn_level != rpc_c_authn_level_none)
    {
        dec_err.ctime = (krb5_timestamp) ctime;
        dec_err.cusec = (krb5_int32) cusec;

        /*
         * Build the error code into the KRB_ERROR message.
         */
        dec_err.error = (krb5_ui_4) wire_error;
        dec_err.client = client;
        dec_err.server = server;
        dec_err.text.length = 0;
        dec_err.e_data.length = 0;
        code = krb5_mk_error (&dec_err, &outbuf);
        err_message->data = (unsigned char *)outbuf.data;
        err_message->length = outbuf.length;
    }
    else
    {
        /*
         * Use the status code is big endian format into as the
         * message. The status code given must also be converted in
         * an "architected" status code before being place on the wire.
         */
        err_message->data = (byte_p_t) malloc (4);
        mp = (rpc_mp_t) err_message->data;
        rpc_marshall_be_long_int (mp, wire_error);
        err_message->length = 4;
        code = rpc_s_ok;
    }
    return (code);
}


/*
 * S E C _ K R B _ C N _ D E C O D E _ E R R _ M E S S A G E
 */

/*
 * Read a KRB_AP_ERROR message.
 */
error_status_t sec_krb_cn_decode_err_message (authn_level,
                                              err_message,
                                              error,
                                              key)

unsigned32              authn_level;
sec_krb_message         *err_message;
unsigned32              *error;
sec_des_key             *key;

{
    krb5_error_code             code;
    krb5_data                   enc_errbuf;
    krb5_error                  *dec_error;
    unsigned32                  tmp_error;
    rpc_mp_t                    ump;

    /*
     * If the authentication level is "none" the message is simply a
     * status code in big endian format. This will
     * have to be converted to the local representation and then
     * converted into a local status code. If the level is anything
     * other than "none" a KRB_ERROR message was encoded.
     */
    if (authn_level != rpc_c_authn_level_none)
    {
        enc_errbuf.length = err_message->length;
        enc_errbuf.data = (char *) err_message->data;
        code = krb5_rd_error (&enc_errbuf, &dec_error);
        if (!code)
        {
            tmp_error = (unsigned32) dec_error->error;
            *error = sec_krb_status_wire_to_rpc (tmp_error);
            if (dec_error->client != NULL)
            {
                krb5_free_principal (dec_error->client);
            }
            if (dec_error->server != NULL)
            {
                krb5_free_principal (dec_error->server);
            }
        krb5_free_error(dec_error);
            }
        }
    else
    {
        ump = (rpc_mp_t) err_message->data;
        rpc_convert_be_long_int (ump, tmp_error);
        *error = sec_krb_status_wire_to_rpc (tmp_error);
        code = rpc_s_ok;
    }
    return (code);
}

/*
 * S E C _ K R B _ C N _ D E C O D E _ R E P _ M E S S A G E
 */

/*
 * Decode either a KRB_AP_REP or KRB_ERROR message.
 */
error_status_t sec_krb_cn_decode_rep_message (authn_level,
                                              message,
					      ctime,
					      cusec,
                                              key)
    

unsigned32                      authn_level;
sec_krb_message                 *message;
sec_krb_timestamp		ctime;
unsigned32			cusec;
sec_des_key                     *key;

{
    error_status_t code;
    sec_krb_msgstamp msgstamp;
    krb5_keyblock               keyblock;

    msgstamp.ctime = 0;
    msgstamp.cusec = 0;
    msgstamp.seq_number = 0;

    keyblock.keytype = KEYTYPE_DES;
    keyblock.length = sizeof (sec_des_key);
    keyblock.contents = (krb5_octet *) key;

    code = sec_krb_decode_rep_common (authn_level, message, 
	&keyblock, &msgstamp);

    if (code == error_status_ok) {
	if ((msgstamp.cusec != cusec) ||
	    (msgstamp.ctime != ctime))
	    code = sec_s_bad_nonce;
    }
    return code;
}


/*
 * S E C _ K R B _ C N _ B U I L D _ R E P _ M E S S A G E
 */

/*
 * Build a KRB_AP_REP message.
 */
error_status_t sec_krb_cn_build_rep_message (authn_level,
                                             ctime, 
                                             cusec,
                                             rep_message,
                                             key)

unsigned32              authn_level;
sec_krb_timestamp       ctime;
unsigned32              cusec;
sec_krb_message         *rep_message;
sec_des_key             *key;

{
    sec_krb_msgstamp msgstamp;
    krb5_keyblock keyblock;
    
    msgstamp.seq_number = 0;
    msgstamp.ctime = ctime;
    msgstamp.cusec = cusec;

    keyblock.keytype = KEYTYPE_DES;
    keyblock.length = sizeof(sec_des_key);
    keyblock.contents = (krb5_octet *)key;
    
    return sec_krb_build_rep_common (authn_level, &keyblock, &msgstamp,
	rep_message);
}

