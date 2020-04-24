/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_auth.c,v $
 * Revision 1.1.20.1  1996/10/03  15:10:44  arvind
 * 	stamp out sec_krb_name_parsed_cmp
 * 	[1996/09/16  23:05 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.18.3  1996/02/18  00:19:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:46  marty]
 * 
 * Revision 1.1.18.2  1995/12/08  17:55:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/10/04  19:18 UTC  mullan_s
 * 	Merge fix for CHFts16546
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16496/1  1995/10/04  18:28 UTC  mullan_s
 * 	Call pac_to_epac_chain_bytes instead of pac_to_epac
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/24  19:27 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat10/1  1995/05/23  19:01 UTC  mullan_s
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/04  20:59 UTC  cuti
 * 	Add one signature to sec_login_pvt_release_context.
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/1  1995/04/01  22:05 UTC  cuti
 * 	Reflect locin_context structure change and release_context signature change
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/03  22:13 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat2/1  1995/03/20  14:57 UTC  mullan_s
 * 	Update call to sec_login_util_get_krb_ptgt
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  03:33 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  01:33 UTC  rps
 * 	[OT12669] cell renaming
 * 	[CHFts13832] security hole
 * 	[CHFts09900] memory leak
 * 	[1995/12/08  17:21:18  root]
 * 
 * Revision 1.1.14.1  1994/10/24  18:32:57  greg
 * 	CR12159: Add a generation uuid to the login context and incorporate it
 * 	into the login context export token.  This allows the  CDS clerk to
 * 	recognize changes to the *fffffff* host machine credentials immediately.
 * 	[1994/10/24  18:06:34  greg]
 * 
 * Revision 1.1.9.5  1994/10/05  18:03:28  sekhar
 * 	[ OT 12466 ] Check return status from krb5_copy_{principal,keyblock_contents}
 * 	[1994/10/05  14:59:11  sekhar]
 * 
 * Revision 1.1.9.4  1994/08/04  16:13:52  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:06  mdf]
 * 
 * Revision 1.1.9.2  1994/06/02  21:23:07  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:34  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:40  mdf]
 * 
 * Revision 1.1.3.10  1993/04/21  20:58:32  sommerfeld
 * 	[7700] Avoid making RPC calls out of the key callback functions.
 * 	Note that there were some fairly major restructuring needed for this.
 * 	[1993/04/21  20:41:47  sommerfeld]
 * 
 * Revision 1.1.3.9  1993/03/25  16:18:43  emartin
 * 	CR  7521. Oops forget to make the change
 * 	[1993/03/24  22:01:33  emartin]
 * 
 * 	CR  7521. rs_rep_auth_dispose_handle - set *handle_p to NULL.
 * 	[1993/03/24  20:39:33  emartin]
 * 
 * Revision 1.1.3.8  1993/02/03  22:09:07  ahop
 * 	bl6e: fix rios compile error; implement access check in check_replica;
 * 	      rgy_princ name in binding_set_auth shouldn't contain /.../
 * 	[1993/02/03  20:48:47  ahop]
 * 
 * Revision 1.1.3.7  1993/02/01  20:37:00  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:15:45  hinman]
 * 
 * Revision 1.1.3.6  1993/01/26  23:10:40  emartin
 * 	bl6d: cache login context in create_context
 * 	[1993/01/18  23:50:24  emartin]
 * 	Revision 1.1.7.2  1993/01/11  16:13:21  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * 	bl6d: fix code accidently commented out by a typo
 * 	[1993/01/18  23:31:01  emartin]
 * 
 * 	support rs_bypass_auth; change auth_h to *auth_h for memset call
 * 	[1993/01/13  16:30:45  emartin]
 * 
 * Revision 1.1.3.5  1992/12/29  16:36:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:45  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:12:39  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:15  sekhar]
 * 
 * Revision 1.1.4.3  1992/12/16  20:05:25  emartin
 * 	bl6c: remove redundant pointer free
 * 	Revision 9.12.3.2  92/11/23  16:28:12  meg
 * 	Incorporated osf rev 1.1.3.3
 * 
 * Revision 1.1.4.2  1992/12/11  22:36:01  emartin
 * 	bl6c: some comment re locking
 * 	Revision 9.12.1.3  92/10/26  12:36:11  bein
 * 	PTC - DCOSX specific changes.
 * 	Revision 9.13.1.2  92/11/23  11:20:02  root
 * 	Auto checkin of OSF rev 1.1.3.3
 * 
 * Revision 1.1.3.3  1992/11/04  19:38:23  ahop
 * 	Revision 9.12.2.2  92/10/26  09:34:14  bein
 * 	PTC - our compiler does not (yet) handle initialization of automatic structs.
 * 	Revision 1.1.3.3  92/11/04  19:38:23  ahop
 * 	Third replication drop: bl6a
 * 	 Add gen_init_ekey, get_init_ekey, destroy_ekey
 * 	[1992/11/03  18:44:48  ahop]
 * 
 * Revision 9.12.1.2  92/10/19  15:35:10  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * Revision 1.1.3.2  1992/10/07  20:36:17  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:43  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:24:35  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992, 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry propagation authentication utilities
 */
#include <stdio.h>

#include <dce/dce.h>
#include <dce/rpc.h>
#include <dce/dce_cf.h>
#include <dce/rplbase.h>
#include <dce/aclbase.h>
#include <dce/sec_cred.h>
#include <sec_cred_internal.h>
#include <rs_server.h>
#include <rs_pvt.h>
#include <rs_rep_auth.h>
#include <rs_rep_vmcc.h>
#include <dce/priv.h>
#include <rs_login.h>
#include <rs_mkey.h>
#include <rsdb_acl.h>
#include <rsdb_acct.h>
#include <rsdb_policy.h>
#include <rsdb_util.h>
#include <sec_krb_util.h>
#include <sec_pkl.h>
#include <priv_krb_util.h>
#include <sec_id_pac_util.h>
#include <sec_login_pvt.h>
#include <sec_login_util.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <krb5/func-proto.h>
#include <krb5/asn1.h>
#include <sec_svc.h>

/* internal structure of an rs_rep_auth_handle_t */
typedef  struct  rs_rep_auth_pvt_handle_t {
    krb5_data           encoded_ptgt;
    sec_login_handle_t  login_handle;
} rs_rep_auth_pvt_handle_t;

PRIVATE pthread_mutex_t creds_mutex;
PRIVATE krb5_creds slave_service_creds;

/* used for key encryption during initialization account propagation */
PRIVATE sec_passwd_rec_t    init_encrypt_key; 
PRIVATE boolean32           init_encrypt_key_valid = false; 

#ifdef RS_REP_NAUTH
PRIVATE sec_passwd_des_key_t bypass_auth_encrypt_key = 
{
    0x9e,0xc1,0x85,0x2f,0x61,0x62,0x9d,0x79
};
#endif

PRIVATE get_auth_info_creds (
    krb5_creds      *creds,   /* [out] */
    error_status_t  *status
)
{
    sec_login_handle_t  lc;
    sec_rgy_name_t      cell_name;
    krb5_creds          key_creds;

    rs_login_get_host_login_context(&lc, status);
    if (GOOD_STATUS(status)) {
        memset(&key_creds, 0, sizeof(key_creds));
        memset(creds, 0, sizeof(*creds));
        rsdb_properties_get_realm_name(cell_name);

        /* Make sure we have a valid PTGT */
	/* 
	 * !!! We may not need to do the following here, which would
	 * eliminate a dependancy we really don't need..
	 */
        sec_login_util_get_krb_ptgt(lc, cell_name, rpc_c_authn_dce_secret,
                          rpc_c_authz_dce, NULL, status);
        if (GOOD_STATUS(status)) {
            sec_krb_ptgs_parsed_name((char *)cell_name, &key_creds.client,
                                     status);
        }
        if (GOOD_STATUS(status)) {
            sec_krb_tgs_parsed_name((char *)cell_name, &key_creds.server, 
                                    status);
        }
        if (GOOD_STATUS(status)) {
            krb5_timestamp       now;
            krb5_flags           flags;
            krb5_ccache          cache = NULL;
            sec_login_context_t  *lcp = (sec_login_context_t *) lc;

            (void) krb5_timeofday(&now);

            /* don't want a ticket that's expired. */
            key_creds.times.endtime = now;
            flags = 0 | KRB5_TC_MATCH_TIMES;
            cache = lcp->krb_info.cache;

            *status = krb5_cc_retrieve_cred(cache, flags, &key_creds, creds);
            krb5_free_cred_contents(&key_creds);
        }
    }
}


/* 
 * Slave-only routines
 */

/* rs_rep_auth_check
 *
 * Verify that the caller is another registry server in our cell
 * (accept only tickets in the name of the architecturally-defined 
 * registry server principal).
 * This routine does not look in the rgy database so the database 
 * state does not need to be checked and database locks do not 
 * need to be taken by the caller.
 *
 */
PUBLIC  void  rs_rep_auth_check (
    handle_t         handle,
    error_status_t   *status
)
{
    unsigned_char_p_t  sname;
    rpc_authz_handle_t cname;
    unsigned32         authn_level;
    unsigned32         authn_svc;
    unsigned32         authz_svc;
    int                len, slen, clen;
    error_status_t     xst;
 
    CLEAR_STATUS(status);

#ifdef RS_REP_NAUTH
    if (rs_bypass_auth) return;
#endif

    /* inquire client authentication info */
    rpc_binding_inq_auth_client(handle, &cname, &sname, &authn_level,
                                &authn_svc, &authz_svc, status);
    if (GOOD_STATUS(status)) {
        /* 
         * insist on the folowing:
         *    authn_level >= rpc_s_authn_level_pkt
         *    authn_svc   == rpc_c_authn_dce_private
         *    authz_svc   == rpc_c_authz_name
         *    server_princ_name == client_princ_name == "/<cell_name>/dce-rgy"
	 *
	 * The server canonicalizes its registered cell name to the primary,
	 * so we need to do the comparison in two inelegant steps.
         */
        if (authn_level < rpc_c_protect_level_pkt_integ
            || authn_svc != rpc_c_authn_dce_secret
            || authz_svc != rpc_c_authz_name) {
            SET_STATUS(status, sec_rgy_not_authorized);
        }
        if (GOOD_STATUS(status)) {
	    unsigned_char_p_t  cpname, spname;
	    char *cell_name;

	    len = strlen(SEC_RGY_SERVER_NAME);
	    slen = u_strlen(sname);
	    clen = u_strlen(cname);

	    cell_name = strdup(cname);

	    if (clen <= len || slen <= len || !cell_name ||
		u_strcmp(cell_name + clen - len, SEC_RGY_SERVER_NAME) ||
		u_strcmp(sname + slen - len, SEC_RGY_SERVER_NAME) ||
		cell_name[clen - len - 1] != PATH_COMPONENT_SEPARATOR ||
		sname[slen - len - 1] != PATH_COMPONENT_SEPARATOR
		)
	    {
		rpc_string_free(&sname, status);
		SET_STATUS(status, sec_rgy_not_authorized);
		return;
	    }

	    cell_name[clen - len - 1] = '\0';
	    sname[slen - len - 1] = '\0';

	    if (! rsdb_util_is_realm_local(cell_name, NULL, status) ||
		BAD_STATUS(status) ||
		! rsdb_util_is_realm_local((char *)sname, NULL, status) ||
		BAD_STATUS(status)
		)
	    {
		SET_STATUS(status, sec_rgy_not_authorized);
	    }

	    free(cell_name);
        }
	rpc_string_free(&sname, &xst);
    }
}


/* rs_rep_auth_get_auth_info
 * 
 * Called by a slave to acquire its rs_replica_auth_t info, which it then
 * passes to the (possibly surrogate) master so the master may authenticate
 * to the slave using the the session key sealed in the auth info. The returned
 * auth_info data should be freed by calling rs_rep_auth_free_auth_info.
 * This routine queries the database to obtain the replica's cell name.  The
 * caller must provide the approparite concurreny control.
 */
PUBLIC  void  rs_rep_auth_get_auth_info (
    rs_replica_auth_t  **auth_info,
    error_status_t     *status
)
{
    krb5_creds  creds; 
    int         size ;

    get_auth_info_creds(&creds, status);
    if (GOOD_STATUS(status)) {

        if (auth_info != NULL) {
            size = sizeof(rs_replica_auth_t) + creds.ticket.length;
            *auth_info = (rs_replica_auth_t *) malloc(size);
            if (*auth_info != NULL) {
                (*auth_info)->info_type = rs_c_auth_info_krb5_ptgt;
                (*auth_info)->info_len = creds.ticket.length;
                memcpy((char *)(*auth_info)->info, creds.ticket.data,
                    creds.ticket.length);
            } else {
                SET_STATUS(status, sec_s_no_memory);
            }
        }
	pthread_mutex_lock(&creds_mutex);
	krb5_free_cred_contents(&slave_service_creds);
	memset (&slave_service_creds, 0, sizeof(slave_service_creds));
	krb5_move_cred_contents(&slave_service_creds, &creds);
	pthread_mutex_unlock(&creds_mutex);
    }
}


PUBLIC  void  rs_rep_auth_free_auth_info (
    rs_replica_auth_t  **auth_info,
    error_status_t     *status
)
{
    CLEAR_STATUS(status);
    if (auth_info != NULL) {
        if (*auth_info != NULL) {
            free((char *) *auth_info);
            *auth_info = NULL;
        }
    }
}


/* rs_rep_auth_get_init_skey
 *
 * Look up the slave's PTGT credential and return the session key.  This
 * routine merely returns the last cached session key from the last time
 * a master contacted us.
 *
 * This routine may query the database to obtain the replica's cell name.
 * The caller must provide the appropriate concurrency control.
 */
PUBLIC  void  rs_rep_auth_get_init_skey (
    sec_passwd_rec_t  **skey,
    error_status_t    *status
)
{
    if (slave_service_creds.keyblock.contents == NULL) {
        /* 
	 * above is hint that we haven't been called before; RPC wants to
	 * seed the crypto RNG.
	 */
        rs_rep_auth_get_auth_info (NULL, status);
    }
    pthread_mutex_lock (&creds_mutex);
    if (slave_service_creds.keyblock.contents == NULL) 
    {
	*status = KRB5_KT_NOTFOUND; /* for compatibility with normal keytab */
    } else 
    {
        *skey = krb_to_sec_key(&slave_service_creds.keyblock, status);	
    }
    pthread_mutex_unlock (&creds_mutex);
}


/* rs_rep_auth_gen_init_ekey
 *
 * Use the master_key to randomly generate a DES key to be used to
 * encrypt/decrypt account auth_keys during initialization account propagation.
 * The master key must have been initialized already (in rsdb_initialize).
 * This key is stored in "init_encrypt_key," private to this file, so that it
 * may be accessed by other modules using the get_init_ekey call.
 * The init_encrypt_key should be destroyed, after it has fulfilled its
 * purpose, by calling destroy_ekey.
 *
 * This operation is called by an initializing slave to obtain the
 * encryption_key to pass to a (surrogate) master in rs_rep_mgr_copy_all.
 * This operation returns a pointer to the static init_encrypt_key.
 * The pointer should be reset to NULL after the caller is finished with it.
 */
PUBLIC  void  rs_rep_auth_gen_init_ekey (
    sec_passwd_rec_t  **ekey,
    error_status_t    *status
)
{
    CLEAR_STATUS(status);
    *ekey = NULL;

#ifdef RS_REP_NAUTH
    if (rs_bypass_auth) 
    {
        PASSWD_TYPE(&init_encrypt_key) = sec_passwd_des;
        bcopy(bypass_auth_encrypt_key, DES_PASSWD(&init_encrypt_key), 
                      sec_passwd_c_des_key_size);
    }
    else 
#endif
    {
        rs_mkey_random(&init_encrypt_key, status);
    }

    if(GOOD_STATUS(status)) {
        *ekey = &init_encrypt_key;
        init_encrypt_key_valid = true;
    }
}


/* rs_rep_auth_get_init_ekey
 *
 *  Return a pointer to the static init_encrypt_key.
 *  This operation is called by an initializing slave in rs_prop_acct_add to
 *  obtain the encryption_key in order to decrypt account auth_keys.
 *  The pointer should be reset to NULL after the caller is finished with it.
 */
PUBLIC  void  rs_rep_auth_get_init_ekey (
    sec_passwd_rec_t  **ekey,
    error_status_t    *status
)
{
    CLEAR_STATUS(status);

    if(!init_encrypt_key_valid) {
        SET_STATUS(status, sec_rgy_rep_init_ekey_invalid);
        return;
    }

    *ekey = &init_encrypt_key;
}


/* rs_rep_auth_destroy_init_ekey
 *
 *  Clear the memory associated with the init_encrypt_key.
 *  This operation is called by an initializing slave
 *  after completion (successful or not) of rs_rep_mgr_copy_all.
 */
PUBLIC  void  rs_rep_auth_destroy_init_ekey (
    error_status_t    *status
)
{
    CLEAR_STATUS(status);

    if(!init_encrypt_key_valid) {
        SET_STATUS(status, sec_rgy_rep_init_ekey_invalid);
        return;
    }
    memset(&init_encrypt_key, 0, sizeof(init_encrypt_key));
    init_encrypt_key_valid = false;
}




/*
 * Master-only routines
 */

/* decode_auth_info
 *
 * Used by the (possibly surrogate) master, to decode and decrypt a slave's
 * PTGT obtained via rs_replica_mgr_get_info_and_creds.  If the decoding/
 * decrypting is successful, return a krb5 ticket. 
 *
 * NOTE WELL:  We assume that the database is locked with a least a read lock
 *             because we need to look up the security server's key in the 
 *             key database.
 *
 * Errors:
 *    sec_rgy_not_authorized    PTGT is not valid
 */
PRIVATE  void  decode_auth_info (
    krb5_data           *encoded_ptgt,  /* [in] */
    krb5_ticket         **ticket,       /* [out] */
    error_status_t      *status         /* [out] */
)
{
    sec_rgy_name_t      my_cell;
    krb5_principal      tgs_princ = NULL;
    unsigned_char_t     *cell_name = NULL, *princ_name = NULL;
    krb5_keyblock       *tgs_key = NULL;
    krb5_encrypt_block  eblock;
    boolean32           default_pepper;
    krb5_kvno           mkey_version;

    *ticket = NULL;

    /* 
     * first we have to decode the auth_info "pickle."  In this case it's an
     * ASN1/BER pickle.
     */ 
    *status = decode_krb5_ticket(encoded_ptgt, ticket);

    if (GOOD_STATUS(status)) {

        /* 
         * Make sure we really have a ticket to the ticket-granting-ticket
         * service, not some random service.
         */
        rsdb_properties_get_realm_name(my_cell);
        sec_krb_tgs_parsed_name((char *)my_cell, &tgs_princ, status);
        if (GOOD_STATUS(status)) {
            if (!sec_krb_name_parsed_cmp_whole(tgs_princ, (*ticket)->server)) {
                SET_STATUS(status, sec_rgy_not_authorized);
                krb5_free_principal(tgs_princ);
            }
        }

        if (BAD_STATUS(status)) {
            krb5_free_ticket(*ticket);
            return;
        }
    }

    sec_dce_pair_from_krb_princ(tgs_princ, (char **)&cell_name, 
                                (char **)&princ_name, status);
    if (GOOD_STATUS(status)) {

        /* get the tgs principal's key */
	rsdb_acct_get_auth_key_by_name(princ_name, NULL,
            krb5_csarray[(*ticket)->enc_part.etype]->system->proto_keytype,
            (*ticket)->enc_part.kvno, &tgs_key, &default_pepper,
	    NULL, &mkey_version, status);

	free((char *) cell_name);

        if (GOOD_STATUS(status)) {
            krb5_use_cstype(&eblock, (*ticket)->enc_part.etype);
            *status = krb5_process_key(&eblock, tgs_key);
            if (GOOD_STATUS(status)) {
                 *status = krb5_decrypt_tkt_part(tgs_key, *ticket);
                 krb5_finish_key(&eblock);
            }
        }
    }

    if (GOOD_STATUS(status)) {
        /* 
         * make sure the client sealed in the ticket is the privilege
         * service 
         */
        krb5_principal  ptgs_princ;

        sec_krb_ptgs_parsed_name((char *)my_cell, &ptgs_princ, status);
        if (GOOD_STATUS(status)) {
            if (!sec_krb_name_parsed_cmp_whole(ptgs_princ, 
                                         (*ticket)->enc_part2->client)) {
                SET_STATUS(status, sec_rgy_not_authorized);
            } 
            krb5_free_principal(ptgs_princ);
        }
    }

    if (BAD_STATUS(status)) {
        if ((*ticket) != NULL) krb5_free_ticket(*ticket);
    } 

    if (tgs_princ != NULL) krb5_free_principal(tgs_princ); 
    if (princ_name != NULL) free((char *)princ_name); 
    if (tgs_key != NULL)  krb5_free_keyblock(tgs_key);
}


/* check_replica
 *
 * check the PAC from a replica's decoded/decrypted ptgt
 * against the replica list acl to see if the replica is 
 * legitimate (i.e., has "I" rights to the replica list).
 */
PRIVATE  void  check_replica (
    krb5_ticket     *ptgt,     /* [in] */
    error_status_t  *status
)
{
    sec_id_pac_t            pac;
    idl_pkl_t               *pickled_pac;
    sec_acl_permset_t       required_perms = sec_acl_perm_Initialize;
    sec_rgy_foreign_id_t    client_ids;
    rpc_authz_cred_handle_t cred_h;
    sec_bytes_t             dlg_chain;
    sec__cred_auth_info_t   auth_info;

    CLEAR_STATUS(status);

    priv_krb_util_authdata_to_pac(ptgt->enc_part2->authorization_data,
                                  &pickled_pac, status);
    if (GOOD_STATUS(status)) {
        sec_id_pac_t_unpickle(pickled_pac, (void *(*)(unsigned32)) malloc,
				&pac, status);
        free( (char *) pickled_pac);

        if (GOOD_STATUS(status)) {
            pac_to_epac_chain_bytes(&pac, &dlg_chain, 0, NULL, 
		sec_id_deleg_type_none, sec_id_compat_mode_none, status);
            if (GOOD_STATUS(status)) {
                auth_info.handle_type = sec__cred_handle_type_client;
                auth_info.authenticated = pac.authenticated;
                auth_info.authz_svc = rpc_c_authz_dce;
                auth_info.authn_svc = rpc_c_authn_dce_secret;
                *status = sec__cred_create_authz_handle(&auth_info, &dlg_chain,
		    NULL, NULL, &cred_h);
            }

            /* See if the replica represented by "pac" has 'I'
             * rights on the replist object.
             */
	    if (GOOD_STATUS(status))
                rs_is_authorized_by_creds((handle_t)NULL, cred_h,
                    required_perms, (sec_acl_permset_t)NULL,
                    SEC_REPLIST_OBJ_NAME,
                    &rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_replist], false,
                    &client_ids, status);

            sec_id_pac_util_free(&pac);
        }
    }
}


/* build_creds
 *
 * construct master-to-slave krb5 credentials encrypted under the session key
 * from the PTGT supplied by the caller.  Assume we've already verified that
 * we have a valid PTGT.
 */
PRIVATE  build_creds (
    krb5_ticket     *ptgt,     /* [in] */
    krb5_creds      *creds,    /* [out] */
    error_status_t  *status
)
{
    krb5_ticket         *tkt;
    krb5_address        *noaddrarray[1];  /* no addresses in ticket */
    krb5_encrypt_block  eblock;
    krb5_pointer        rseed;
    krb5_data           *encoded_tkt;

    /* 
     * put together a ticket to the architecturally-defined security
     * service principal for our cell
     */
    memset(creds, 0, sizeof(krb5_creds));

    tkt = (krb5_ticket *) malloc(sizeof(krb5_ticket));
    if (tkt == NULL) {
        SET_STATUS(status, sec_s_no_memory);
         return;
    } else {
        memset(tkt, 0, sizeof(krb5_ticket));
    }
 
    tkt->enc_part2 = (krb5_enc_tkt_part *) malloc(sizeof(krb5_enc_tkt_part));
    if (tkt->enc_part2 == NULL) {
        SET_STATUS(status, sec_s_no_memory);
         return;
    } else {
        memset(tkt->enc_part2, 0, sizeof(krb5_enc_tkt_part));
        CLEAR_STATUS(status);
    }

    /* leave ticket flag field alone - it was filled with zeros above */

    /* no addresses in replication ticket and creds*/
    noaddrarray[0] = 0;
    tkt->enc_part2->caddrs = noaddrarray;

    /* 
     * generate a random session key.  we use the session key in the PTGT
     * to seed the random number generator
     */
    krb5_use_cstype(&eblock, ptgt->enc_part.etype);
    *status = krb5_process_key(&eblock, ptgt->enc_part2->session);
    if (GOOD_STATUS(status)) {
        *status = krb5_init_random_key(&eblock, ptgt->enc_part2->session,
                                       &rseed);
        if (GOOD_STATUS(status)) {
            *status = krb5_random_key(&eblock,rseed,&tkt->enc_part2->session);
            (void) krb5_finish_random_key(&eblock, &rseed);
        }

        /*
         * set both client and server fields of the ticket to dce-rgy@cell
         * where  "cell" is the realm from the caller-provided PTGT
         */
        if (GOOD_STATUS(status)) {
            char  *cell, *princ;

            sec_dce_pair_from_krb_princ(ptgt->server, &cell, &princ, status);
            if (GOOD_STATUS(status)) {
                sec_krb_princ_from_dce_pair(cell, SEC_RGY_SERVER_NAME, 
                                            &tkt->server, status);
                /* done with cell amd princ in any event */
                free(cell);
                free(princ);

                if (GOOD_STATUS(status)) {
                    *status = krb5_copy_principal(tkt->server, 
                          &tkt->enc_part2->client);
                }
            }
        }

        /* ticket times */
        if (GOOD_STATUS(status)) {

            /* take authtime from the PTGT */
            tkt->enc_part2->times.authtime = ptgt->enc_part2->times.authtime;
            /* ticket is valid starting now */
            krb5_timeofday(&tkt->enc_part2->times.starttime);
            /* ticket expires when the PTGT on which it is based expires */
            tkt->enc_part2->times.endtime = ptgt->enc_part2->times.endtime;
        }

        if (GOOD_STATUS(status)) {
            /* encrypt the ticket in the session key obtained from the PTGT */
            tkt->enc_part.kvno = 0;            /* we're using a session key */
            tkt->enc_part.etype = ptgt->enc_part.etype;
            *status = krb5_encrypt_tkt_part(ptgt->enc_part2->session, tkt);
        }

        *status = krb5_finish_key(&eblock);
    }

    /* encode the encrypted  portion of the ticket */
    if (GOOD_STATUS(status)) {
        *status  = encode_krb5_ticket(tkt, &encoded_tkt);
    }

    /* start filling in the creds */
    if (GOOD_STATUS(status)) {

        creds->ticket = *encoded_tkt;
        free((char *) encoded_tkt);

        creds->times = tkt->enc_part2->times;

        *status = krb5_copy_principal(tkt->enc_part2->client, &creds->client);
        if (GOOD_STATUS(status)) {
            *status = krb5_copy_principal(creds->client, &creds->server);
        }

        if (GOOD_STATUS(status)) {
            *status = krb5_copy_keyblock_contents(tkt->enc_part2->session,
                                                  &creds->keyblock);
        }
    }

    /* 
     * careful about addresses field - it points to stuff on the stack, 
     * not on the heap
     */
    tkt->enc_part2->caddrs = NULL;
    krb5_free_ticket(tkt); 

    if (BAD_STATUS(status)) {
        krb5_free_cred_contents(creds);
    }
}


PRIVATE  void  create_context (
    krb5_creds          *creds,    /* [in] */ 
    sec_login_handle_t  *lcp,      /* [out] */
    error_status_t      *status
)
{
    char                *princ, *cell;
    sec_id_pac_t        pac;
    krb5_ccache         ccache = NULL;
    sec_login_context_t *lc;
    error_status_t      lst;
    uuid_t              generation_id;

    ccache = rs_rep_vmcc_get_ccache(status);
    if (GOOD_STATUS(status)) {
        *status = krb5_cc_initialize(ccache, creds->client);
    }

    if (GOOD_STATUS(status)) {
        sec_dce_pair_from_krb_princ(creds->client, &cell, &princ, status);
        if (GOOD_STATUS(status)) {
            /*
             * don't bother constructing a real pac because we know
             * that it will never be needed (the virtual memory credential
             * cache has no TGT so any attempt to get a PTGT would fail
             * immediately).  Just fill in with NIL uuids.  Ditto for the
	     * generation_id, which is useless because vmcc-based creds 
	     * can't be exported.
             */

             memset(&pac, 0, sizeof(pac));
             pac.pac_type = sec_id_pac_format_v1;
             pac.authenticated = true;
             uuid_create_nil(&pac.realm.uuid, &lst);
             uuid_create_nil(&pac.principal.uuid, &lst);
             uuid_create_nil(&pac.group.uuid, &lst);
             uuid_create_nil(&generation_id, &lst);

             /* 
              * Create a raw context with mostly meaningless data.
              * All that really matters are the context/info states 
              * and the credential cache handle
              */
             sec_login_pvt_create_raw_context(cell, princ, &pac, ccache, 0,
                                              sec_login_auth_src_network, 
                                              valid, info_auth, true, NULL,
                                              &generation_id, lcp, status);
             free(cell);
             free(princ);
        }
    }

    if (BAD_STATUS(status)) {
         if (ccache != NULL) {
             lst = krb5_cc_close(ccache);
         }
    }
    lc = (sec_login_context_t *)(*lcp);
    lc->refcount = 1;
    lc->magic = lc;
    sec_login_cache_enter(lc);
}


/* rs_rep_auth_get_handle
 *
 * Used by the (possibly surrogate) master, to decode and decrypt a slave's
 * auth info obtained via rs_replica_mgr_get_info_and_creds.  If the decoding/
 * decrypting is successful, return an opaque handle on the decoded info.
 *
 * Caller should hold a read lock. 
 */
PUBLIC  void  rs_rep_auth_get_handle (
    rs_replica_auth_t     *auth_info,  /* [in] */
    rs_rep_auth_handle_t  *handle,     /* [out] */
    error_status_t        *status
)
{
    krb5_data                 encoded_ptgt;
    krb5_ticket               *ptgt = NULL;
    krb5_creds                creds;
    rs_rep_auth_pvt_handle_t  *auth_h = NULL;

    auth_h = (rs_rep_auth_pvt_handle_t *) malloc(sizeof(*auth_h));
    if (auth_h == NULL) {
        SET_STATUS(status, sec_s_no_memory);
        return;
    } else {
        memset(auth_h, 0, sizeof(*auth_h));
    }

    /* decipher the (supposed) slave's privilege ticket-granting ticket */
    encoded_ptgt.length = auth_info->info_len;
    encoded_ptgt.data = (char *)auth_info->info;
    decode_auth_info(&encoded_ptgt, &ptgt, status);

    if (GOOD_STATUS(status)) {
        /* make certain the ptgt represents an authentic replica */
        check_replica(ptgt, status);

        if (GOOD_STATUS(status)) {
            /* construct credentials encrypted under the ptgt session key */
            build_creds(ptgt, &creds, status);
        }

        if (GOOD_STATUS(status)) {
            /* construct a login context that references creds */
            create_context(&creds, &auth_h->login_handle, status);

            if (GOOD_STATUS(status)) {
                krb5_ccache           cache = NULL;
                sec_login_context_t   *lcp
                    = (sec_login_context_t *) auth_h->login_handle;

                /* cache creds in login context's cache */
                cache = lcp->krb_info.cache;
                if (cache != NULL) {
                    *status = krb5_cc_store_cred(cache, &creds);
                } else { 
                    dce_svc_printf(SEC_REP_CORRUPT_AUTH_HANDLE_MSG);
                }
            }

            /* done with creds, in any event */
            krb5_free_cred_contents(&creds);
        } 

        /* done with ticket, in any event */
        krb5_free_ticket(ptgt);
    }

    if (GOOD_STATUS(status)) {
        /* store a copy of the encoded ptgt in the auth handle */
        auth_h->encoded_ptgt.data = malloc(auth_info->info_len);
        if (auth_h->encoded_ptgt.data != NULL) {
            memcpy(auth_h->encoded_ptgt.data, auth_info->info,
                   auth_info->info_len);
            auth_h->encoded_ptgt.length = auth_info->info_len;
        } else {
            SET_STATUS(status, sec_s_no_memory);
        }
    }

    if (BAD_STATUS(status)) {
        rs_rep_auth_dispose_handle((rs_rep_auth_handle_t *) (&auth_h));
    } else {
        *handle = (rs_rep_auth_handle_t) auth_h;
    }
}


/*
 * refresh an existing replica auth handle 
 *
 * Caller should hold a read lock. 
 */
PUBLIC  void  rs_rep_auth_refresh_handle (
    rs_replica_auth_t     *auth_info,  /* [in] */
    rs_rep_auth_handle_t  handle,      /* [in] */
    error_status_t        *status
)
{
    krb5_data                 encoded_ptgt;
    krb5_ticket               *ptgt = NULL;
    krb5_creds                creds;
    rs_rep_auth_pvt_handle_t  *auth_h;

    auth_h = (rs_rep_auth_pvt_handle_t *) handle;

    if (auth_h == NULL || auth_h->login_handle == NULL) {
        dce_svc_printf(SEC_REP_INVALID_AUTH_HANDLE_MSG);
    }

    /* decipher the (supposed) slave's privilege ticket-granting ticket */
    encoded_ptgt.length = auth_info->info_len;
    encoded_ptgt.data = (char *)auth_info->info;
    decode_auth_info(&encoded_ptgt, &ptgt, status);

    if (GOOD_STATUS(status)) {
        /* make certain the ptgt represents an authentic replica */
        check_replica(ptgt, status);

        if (GOOD_STATUS(status)) {
            /* construct credentials encrypted under the ptgt session key */
            build_creds(ptgt, &creds, status);
        }

        if (GOOD_STATUS(status)) {
            krb5_ccache   cache = NULL;
            sec_login_context_t   *lcp = (sec_login_context_t *) auth_h->login_handle;

            /* cache creds in login context's cache */
            cache = lcp->krb_info.cache;
            if (cache != NULL) {
                *status = krb5_cc_store_cred(cache, &creds);
            } else { 
                dce_svc_printf(SEC_REP_CORRUPT_AUTH_HANDLE_MSG);
            }


            /* done with creds, in any event */
            krb5_free_cred_contents(&creds);
            }

        /* done with ticket, in any event */
        krb5_free_ticket(ptgt);
    }

    if (GOOD_STATUS(status)) {
#ifdef SNI_DCOSX
        krb5_data  prev_ptgt;
        prev_ptgt = auth_h->encoded_ptgt;
#else	/* SNI_DCOSX */
        krb5_data  prev_ptgt = auth_h->encoded_ptgt;
#endif	/* SNI_DCOSX */

        /* store a copy of the encoded ptgt in the auth handle */
        auth_h->encoded_ptgt.data = malloc(auth_info->info_len);
        if (auth_h->encoded_ptgt.data != NULL) {
            memcpy(auth_h->encoded_ptgt.data, auth_info->info,
                   auth_info->info_len);
            auth_h->encoded_ptgt.length = auth_info->info_len;

            free(prev_ptgt.data);
        } else {
            /* restore previous auth_info */
            auth_h->encoded_ptgt = prev_ptgt;
            SET_STATUS(status, sec_s_no_memory);
        }
    }
}


/* rs_rep_auth_binding_set_auth
 *
 * authenticate a binding handle using the credentials referenced
 * by auth_h, and the architecturally-defined registry service principal.
 */
PUBLIC  void  rs_rep_auth_binding_set_auth (
    rs_rep_auth_handle_t  auth_h,
    rpc_binding_handle_t  rep_handle,
    error_status_t        *status
)
{
    char                  *local_cell = "/.:";
    unsigned_char_t       *rgy_princ = (unsigned_char_t *)0;
    sec_login_handle_t    lc;

    /*
     * construct the registry service principal name.  Be sure to
     * use the principal@realm syntax to avoid calls to sec_id_parse_name
     */
    sec_krb_string_from_dce_pair(local_cell, SEC_RGY_SERVER_NAME,
                                 (char **)&rgy_princ, status);
    if(BAD_STATUS(status)) {
        return;
    }
    /* get the login context from the rep auth handle */
    rs_rep_auth_get_login_context(auth_h, &lc, status);
    if (GOOD_STATUS(status)) {
        rpc_binding_set_auth_info(rep_handle, rgy_princ,
                                  rpc_c_protect_level_pkt_integ,
                                  rpc_c_authn_dce_secret,
                                  (rpc_auth_identity_handle_t)lc,
                                  rpc_c_authz_name, status);
    }
    if(rgy_princ)
        free(rgy_princ);
}


PUBLIC  void  rs_rep_auth_get_login_context (
    rs_rep_auth_handle_t  handle,
    sec_login_handle_t    *login_handle_p,
    error_status_t        *status
)
{
    rs_rep_auth_pvt_handle_t  *auth_h;

    auth_h = (rs_rep_auth_pvt_handle_t *) handle;

    CLEAR_STATUS(status);

    if (auth_h == NULL) {
        dce_svc_printf(SEC_REP_INVALID_AUTH_HANDLE_MSG);
    }

    if (auth_h->login_handle == NULL) {
        SET_STATUS(status, sec_rgy_not_authorized);
    } else {
        *login_handle_p = auth_h->login_handle;
    }
}


PUBLIC  void  rs_rep_auth_dispose_handle (
    rs_rep_auth_handle_t  *handle_p
)
{
    rs_rep_auth_pvt_handle_t  *auth_h;

    if (handle_p == NULL || *handle_p == NULL) {
        return;
    }

    auth_h = (rs_rep_auth_pvt_handle_t *) *handle_p;

    if (auth_h->encoded_ptgt.data != NULL) {
        free((char *)auth_h->encoded_ptgt.data);
    }

    if (auth_h->login_handle != NULL) {
        sec_login_pvt_release_context(auth_h->login_handle, false);
        /* false to indeicate not force cache remove */
    }

    *handle_p = NULL;
}


/* rs_rep_auth_get_rep_skey
 *
 * Return the session key from a replica's auth info.
 * The session key should be freed by calling sec_crypt_free_key
 *
 * Caller should hold a read lock. 
 */
PUBLIC  void  rs_rep_auth_get_rep_skey (
    rs_rep_auth_handle_t  handle,    /* [in] */
    sec_passwd_rec_t      **skey,    /* [out] */
    error_status_t        *status
)
{
    krb5_ticket               *ptgt = NULL;
    rs_rep_auth_pvt_handle_t  *auth_h;

    auth_h = (rs_rep_auth_pvt_handle_t *) handle;

    if (auth_h == NULL) {
        dce_svc_printf(SEC_REP_INVALID_AUTH_HANDLE_MSG);
    }

    /* decipher the slave's privilege ticket-granting ticket */
    decode_auth_info(&auth_h->encoded_ptgt, &ptgt, status);

    if (GOOD_STATUS(status)) {
        *skey = krb_to_sec_key(ptgt->enc_part2->session, status);

        krb5_free_ticket(ptgt);
    }
}


void rs_rep_auth_init()
{
    pthread_mutex_init(&creds_mutex, pthread_mutexattr_default);
}
