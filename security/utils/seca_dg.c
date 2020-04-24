/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seca_dg.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:17  root]
 * 
 * Revision 1.1.2.3  1994/08/09  17:32:55  burati
 * 	DFS/EPAC/KRPC/dfsbind changes
 * 	[1994/08/08  22:05:06  burati]
 * 
 * Revision 1.1.2.2  1994/06/02  21:18:06  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:21:20  mdf]
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

error_status_t sec_krb_dg_decode_message (
    sec_krb_message		*message,
    sec_des_block		*challenge,
    unsigned32			authz_fmt,
    unsigned char		**client_name,
    sec_id_pac_t		*client_pac,
    rpc_authz_cred_handle_t	*client_creds,
    sec_krb_parsed_name		*server_name,
    unsigned32			*authn_level,
    unsigned32			*authz_proto,
    unsigned32			*key_seq,
    sec_des_key			*key,
    sec_des_block		*ivec,
    unsigned32			*expiration
)
{
    return( sec_krb_dg_decode_msg_frags(message, challenge, authz_fmt,
        client_name, client_pac, NULL, client_creds, server_name, authn_level,
        authz_proto, key_seq, key, ivec, expiration) );
}


error_status_t sec_krb_dg_decode_msg_frags (
    sec_krb_message		*message,
    sec_des_block		*challenge,
    unsigned32			authz_fmt,
    unsigned char		**client_name,
    sec_id_pac_t		*client_pac,
    sec_bytes_t			*raw_epac_setP,
    rpc_authz_cred_handle_t	*client_creds,
    sec_krb_parsed_name		*server_name,
    unsigned32			*authn_level,
    unsigned32			*authz_proto,
    unsigned32			*key_seq,
    sec_des_key			*key,
    sec_des_block		*ivec,
    unsigned32			*expiration
)
{
    error_status_t code;
    sec_des_block nonce2;
    krb5_tkt_authent *authent = NULL;
    rpc_mp_t ump;
    unsigned long  level;
    unsigned long  ksno2;
    unsigned long  keytype;
    unsigned long  keylen;
    error_status_t  st;

    code = sec_krb_decode_msg_common_frags (message, authz_fmt, server_name,
	client_name, client_pac, raw_epac_setP, client_creds, expiration,
	NULL, &authent, authz_proto);
    if (code != error_status_ok) 
    {
	return code;
    }
    if (authent == NULL) 
    {
	*authn_level = rpc_c_authn_level_none;
	return code;
    }

    /* XXX check length of checksum here */
    
    ump = (rpc_mp_t) authent->authenticator->checksum->contents;

    /*
     * The format of the "checksum" field is:
     *          +--------------------------------+
     *      0:  |  Nonce                         |
     *          +----------------+---------------+
     *      8:  |  Level         |
     *          +----------------+
     *     12:  |  Key seq       |
     *          +----------------+
     *     16:  |  Key type      |
     *          +----------------+
     *     20:  |  Key length    |
     *          +----------------+---------------+
     *     24:  |  Key..                         |
     *          +--------------------------------+
     */
    
    memcpy (&nonce2, ump, 8);
    rpc_advance_mp(ump, 8);
    
    rpc_convert_be_long_int(ump, level);
    rpc_advance_mp(ump, 4);
    
    rpc_convert_be_long_int(ump, ksno2);
    rpc_advance_mp(ump, 4);
    
    rpc_convert_be_long_int(ump, keytype);
    rpc_advance_mp(ump, 4);
    
    rpc_convert_be_long_int(ump, keylen);
    rpc_advance_mp(ump, 4);

    /*
     * Decode key/initialization vector.
     *
     * If the length is not what we expected, or we receive an ivec
     * and don't know where to put it, give up in disgust.
     */
    
    switch (keylen) {
    case 16:
        if (ivec == NULL) {
        default:
            st = sec_s_keytype_unsupp;
            goto out;
        }
        memcpy (ivec, ump, 8);
        rpc_advance_mp(ump, 8);
        /* fall through */
    case 8:
        memcpy (key, ump, 8);
        rpc_advance_mp(ump, 8);
        break;
    }
    /*
     * Verify that all the fields match what we're expecting.
     */

    if (memcmp(&nonce2, challenge, sizeof(nonce2)) != 0)
    {
#ifdef DEBUG
        printf("given nonce: %08x%08x; packet nonce: %08x%08x\n",
          ((unsigned32 *)(challenge))[0], ((unsigned32 *)(challenge))[1]
          ((unsigned32 *)(&nonce2))[0], ((unsigned32 *)(&nonce2))[1]);
#endif

        st = sec_s_bad_nonce;
        goto out;
    }

    *authn_level = level;
    *key_seq = ksno2;
    krb5_free_tkt_authent(authent);
    return rpc_s_ok;
out:
    /* !!! what, if anything, needs to be freed here? */
    if (authent)
        krb5_free_tkt_authent(authent);
    return st;
}

error_status_t sec_krb_dg_build_message (ccache, cred, challenge,
    authn_level, authz_proto,
    ksno, key, ivec, authz_fmt,
    message)
    sec_krb_ccache ccache;
    sec_krb_cred cred;
    sec_des_block *challenge;
    unsigned32 authn_level;
    unsigned32 authz_proto;
    unsigned32 ksno;
    sec_des_key *key;
    sec_des_block *ivec;
    unsigned32 authz_fmt;
    sec_krb_message *message;
{
    rpc_mp_t mp;
    error_status_t   st;
    unsigned char out_data[1024]; /* !!! a little large.. */
    krb5_checksum send_cksum;
    
    CLEAR_STATUS(&st);

    if (authn_level == rpc_c_authn_level_none) {
	return sec_krb_build_message_none (ccache, cred, authz_proto, 
					   authz_fmt, message);
    } else {
	sec_krb_msgstamp msgstamp;
	msgstamp.seq_number = 0;
	msgstamp.ctime = 0;
	msgstamp.cusec = 0;

        /* set marshalling ptr to start of return block */
        mp = (rpc_mp_t) out_data;
        /* marshall challenge */
        memcpy(mp, challenge, 8);
        rpc_advance_mp(mp, 8);
        /* marshall level */
        rpc_marshall_be_long_int (mp, authn_level);
        rpc_advance_mp(mp, 4);
        /* marshall ksno */
        rpc_marshall_be_long_int (mp, ksno);
        rpc_advance_mp(mp, 4);
        /* marshall key type */
        rpc_marshall_be_long_int (mp, KEYTYPE_DES);
        rpc_advance_mp (mp, 4);

        /*
         * If ivec is NULL, then we're calling the old entry points.
         * For an attempt at wire compatibility, transmit only eight
         * bytes of key; otherwise transmit eight bytes of ivec
         * followed by eight bytes of key.
         */

        if (ivec == NULL) {
            rpc_marshall_be_long_int (mp, 8);
            rpc_advance_mp (mp, 4);
        } else {
            rpc_marshall_be_long_int (mp, 16);
            rpc_advance_mp (mp, 4);
            memcpy(mp, ivec, 8);
            rpc_advance_mp (mp, 8);
        }

        memcpy (mp, key, 8);
        rpc_advance_mp (mp, 8);

        send_cksum.checksum_type = 0x8001;
        send_cksum.length = mp - (rpc_mp_t) out_data;
        send_cksum.contents = (unsigned char *)out_data;

	st = sec_krb_build_message_common (ccache, 
	    cred, 
	    &send_cksum, 
	    authz_proto,
	    authz_fmt,
	    &msgstamp,
	    message);
    
        if (GOOD_STATUS(&st)) {
            return rpc_s_ok;
        }
        else
            return st;
    }
}
