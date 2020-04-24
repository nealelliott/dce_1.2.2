/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_auth.c,v $
 * Revision 1.1.18.1  1996/05/10  13:21:50  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/29  18:57 UTC  hanfei
 * 	add NULL_PTR as cell_uuids and princ_uuids when call
 * 	rs_log_pgo_add_member() for local cell operation.
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  16:22 UTC  hanfei
 * 	multi-cell group work: add NULL_PTR as cell_uuid for rs_tuil_pgo_add() call.
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:16 UTC  psn
 * 	Merge fix for CHFts16632
 * 	[1995/10/18  18:42 UTC  mullan_s  /main/HPDCE02/6]
 *
 * 	check_acct_policy_values: pass in creator_ids as input argument.
 * 	[1995/10/18  18:32 UTC  mullan_s  /main/HPDCE02/mullan_chfts16632/1]
 *
 * 	Merge name based auth fix from mb_mothra6
 * 	[1995/08/31  19:09 UTC  burati  /main/HPDCE02/5]
 *
 * 	Fix name based anonymous authorization for CHFts16164
 * 	[1995/08/29  15:04 UTC  burati  /main/HPDCE02/mb_mothra6/1]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  20:53 UTC  burati  /main/HPDCE02/4]
 *
 * 	Alias ACL fix
 * 	[1995/07/13  20:47 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 *
 * 	CHFts14466 (ot12799) bug fix
 * 	[1995/03/27  19:33 UTC  cuti  /main/HPDCE02/2]
 *
 * 	Change dummy_handle to unauth_handle for better naming
 * 	[1995/03/24  20:40 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/2]
 *
 * 	OT12799 (DTS 14466) Create dummy cred handle before listener thread started
 * 	[1995/03/23  18:10 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/1]
 *
 * 	[1994/12/15  03:29 UTC  rps  /main/HPDCE02/1]
 *
 * 	[OT12669] cell renaming
 * 	[1994/12/15  00:01 UTC  rps  /main/rps_mothra/1]
 *
 * Revision 1.1.10.8  1994/09/27  15:06:58  burati
 * 	CR12346 Remove bad calls to sec_encode_epac_set_free()
 * 	[1994/09/26  21:45:15  burati]
 * 
 * Revision 1.1.10.7  1994/08/29  16:49:17  mdf
 * 	Allow dce-rgy, and the cell principal account and policy
 * 	information to be modified durring locksmith mode
 * 	[1994/08/29  13:55:32  mdf]
 * 
 * Revision 1.1.10.6  1994/08/19  17:59:23  mdf
 * 	1994/08/15  sekhar
 * 	[ OT 10164 ]  Put back the fix which seems to have been overwritten.
 * 	              ( check for authorization before quota ).
 * 	[1994/08/19  13:10:52  mdf]
 * 
 * Revision 1.1.10.5  1994/08/12  17:37:46  mdf
 * 	     1994/08/10 sekhar
 * 	     [ OT 11581 ]  Handle incorrect user response correctly in locksmith mode
 * 	                   log locksmith messages using SVC calls.
 * 	[1994/08/12  14:15:43  mdf]
 * 
 * Revision 1.1.10.4  1994/08/04  16:13:26  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:13  mdf]
 * 
 * Revision 1.1.10.3  1994/07/15  15:01:17  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:19:13  mdf]
 * 
 * Revision 1.1.10.2  1994/06/02  21:24:59  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:15  mdf]
 * 
 * Revision 1.1.10.1  1994/03/25  18:04:23  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 
 * 	     Generate an unauthorized error message instead of principal's
 * 	     quota exhausted when a user is unauthenticated.
 * 	[1994/03/25  18:02:57  sekhar]
 * 
 * Revision 1.1.8.2  1993/05/12  19:56:02  burati
 * 	CR7938 Init PAC's groups & foreign_groups ptrs to NULL too.
 * 	[1993/05/12  19:16:18  burati]
 * 
 * Revision 1.1.4.11  1993/03/22  21:49:58  burati
 * 	7556 Make copy of SEC_RGY_SERVER_NAME to pass to rsdb_pgo_get_by_name
 * 	because even though it says it's an [IN], it does modify the data...
 * 	[1993/03/22  21:37:34  burati]
 * 
 * Revision 1.1.4.10  1993/03/19  22:20:24  emartin
 * 	CR 7472 rs_auth_setup_locksmith
 * 	 after rsdb_acct_add generate unix_passwd wire rep, pass it
 * 	 to rs_log_acct_add.
 * 	 rsdb_acct_replace takes gen_unix_passwd flag (true).
 * 	 after rsdb_acct_replace - if passwd being changed, generate
 * 	 unix_passwd_wire rep and pass it to rs_log_acct_replace.
 * 	[1993/03/19  18:56:18  emartin]
 * 
 * Revision 1.1.4.9  1993/02/19  16:19:26  ahop
 * 	7191: fix typo in non-ANSI prototype
 * 	[1993/02/19  04:37:58  ahop]
 * 
 * 	7191: log locksmith updates
 * 	[1993/02/19  03:52:13  ahop]
 * 
 * Revision 1.1.4.8  1993/02/03  22:06:58  ahop
 * 	bl6e: change BEGIN_READ in rs_get_auth_key to allow uninitialized slave access
 * 	[1993/01/22  22:42:53  ahop]
 * 
 * Revision 1.1.4.7  1992/12/29  16:34:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:18  zeliff]
 * 
 * Revision 1.1.4.6  1992/12/18  22:11:43  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:55  sekhar]
 * 
 * Revision 1.1.6.2  1992/12/11  22:33:15  emartin
 * 	bl6c: use new (shorter) name for server_bindings_client_is_local
 * 
 * Revision 1.1.4.5  1992/10/19  14:17:14  burati
 * 	CR5540 Allow rpc_c_protect_level_privacy
 * 	[1992/10/16  21:28:07  burati]
 * 
 * Revision 1.1.4.4  1992/10/07  20:32:26  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:06  sekhar]
 * 
 * Revision 1.1.5.2  1992/10/06  16:08:43  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.4.3  1992/09/29  20:12:37  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:36:41  sekhar]
 * 
 * Revision 1.1.4.2  1992/08/31  18:24:05  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:26  sekhar]
 * 
 * Revision 1.1.2.3  1992/07/13  20:01:44  ahop
 * 	 3903: update rsdb_acct_get_auth_key_by_name params
 * 	[1992/06/30  22:30:29  ahop]
 * 
 * Revision 1.1.2.2  1992/03/30  16:33:18  ahop
 * 	 support locksmith mode
 * 	[1992/03/27  23:57:02  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1994, 1995
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * Check authentication attributes and apply access control checks.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_auth.c	21 - 01/06/92";
#endif

#include <dce/idlbase.h>
#include <dce/rpc.h>
#include <dce/passwd.h>
#include <dce/sec_cred.h>
#include <sec_cred_internal.h>
#include <dce/sec_daclmgr.h>

#include <rgymacro.h>
#include <rsdb_pvt.h>
#include <rsdb_acct.h>
#include <rsdb_pgo.h>
#include <rsdb_policy.h>
#include <rsdb_name_util.h>
#include <rsdb_util.h>
#include <rsdb_acl.h>
#include <rsdb_misc.h>
#include <rs_pvt.h>
#include <rs.h>
#include <rs_pwd.h>
#include <rs_util.h>
#include <rs_auth.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_secidmap.h>
#include <rs_rep_auth.h>
#include <rs_log_acct.h>
#include <rca_pwd.h>
#include <crypt.h>
#include <sec_krb_util.h>

#include <bstring.h>
#include <un_strng.h>
#include <un_maloc.h>
#include <un_time.h>

#include <sec_def_flags.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <stdio.h>

/* Locksmith mode variable */
PRIVATE rsdb_locksmith_info_t locksmith_info;
PRIVATE rpc_authz_cred_handle_t dummy_cred_h;

/* 
 * e p a c _ d a t a _ t o _ c r e d _ h a n d l e
 *
 * Converts the epac_data into a cred_h.
 */
PRIVATE void epac_data_to_cred_handle
(
    sec_id_epac_data_t	    *epac_data,			/* [in] */
    boolean32		    authenticated,		/* [in] */
    rpc_authz_cred_handle_t *cred_h,			/* [out] */
    error_status_t	    *stp			/* [out] */
)
{
    sec_id_epac_t           epac;
    sec_id_epac_set_t       epac_set;
    sec_bytes_t             epac_chain;
    sec__cred_auth_info_t   auth_info;

    sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, epac_data,
        &epac.pickled_epac_data.num_bytes, &epac.pickled_epac_data.bytes,
        stp);
    if (BAD_STATUS(stp)) return;
    epac.seals = NULL;
    epac_set.num_epacs = 1;
    epac_set.epacs = &epac;
    sec_id_epac_set_encode((void *(*)(unsigned32))malloc, free, &epac_set,
        &epac_chain.num_bytes,	&epac_chain.bytes, stp);
    if (BAD_STATUS(stp)) {
	sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);
	return;
    }

    /* Clean up temporary epac buffers */
    sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);

    /* Now generate the cred handle */
    auth_info.handle_type = sec__cred_handle_type_client;
    auth_info.authenticated = authenticated;
    auth_info.authz_svc = rpc_c_authz_dce;
    auth_info.authn_svc = rpc_c_authn_dce_secret;
    *stp = sec__cred_create_authz_handle(&auth_info, &epac_chain, NULL, NULL,
        cred_h);
    sec_encode_buffer_free(free, &epac_chain.bytes);
}


/*
 * r s _ i s _ a u t h o r i z e d _ b y _ e p a c _ d a t a
 *
 * Converts the epac data into a cred and then return 
 * true if authorization is granted, otherwise return false.
 */
PUBLIC boolean32 rs_is_authorized_by_epac_data (
    sec_id_epac_data_t	    *epac_data,			/* [in] */
    sec_acl_permset_t       requested_perms,		/* [in] */
    sec_acl_permset_t       parent_perms,		/* [in] */
    sec_rgy_name_t          object_name,		/* [in] */
    uuid_t                  *mgr_type,			/* [in] */
    sec_rgy_foreign_id_t    *client_ids,		/* [in] */
    error_status_t          *stp			/* [out] */
)
{

    rpc_authz_cred_handle_t	cred_h;
    error_status_t		tmp_st;
    boolean32			result = false;

    epac_data_to_cred_handle(epac_data, true, &cred_h, stp);
    if (GOOD_STATUS(stp)) {
        result = rs_is_authorized_by_creds_no_locksmith(cred_h, requested_perms,
	    parent_perms, object_name, mgr_type, client_ids, stp);
        tmp_st = sec__cred_free_cred_handle(&cred_h);
    } 
    return result;
}

PUBLIC void rs_auth_init_unauth_cred_handle(
	error_status_t  *stp
)
{

    sec_id_epac_data_t      epac_data;

 /* Generate a dummy EPAC for unauthenticated calls into rs_is_authorized */
    memset(&epac_data, 0, sizeof(sec_id_epac_data_t));
    epac_data_to_cred_handle(&epac_data, false, &dummy_cred_h, stp);

}

/*
 * r s _ a u t h _ i n i t
 *
 * Register for DCE authentication. !!! Be sure to use "princ@realm"
 * format for the registry server name to avoid calling sec_id_name_parse
 * in the rpc runtime
 */
PUBLIC void rs_auth_init (
    error_status_t  *stp
)
{
    sec_rgy_properties_t    cell_properties;
    sec_rgy_name_t          princ_name;
    char                    *rp;

    rsdb_properties_get_info(&cell_properties, stp);
    if (BAD_STATUS(stp)) return;

    rp = (char *) cell_properties.realm;
    if (strncmp(rp, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) == 0) {
        rp = rp + GLOBAL_DIR_ROOT_LEN;
    }

    /* register registry server name */
    strcpy( (char *) princ_name, SEC_RGY_SERVER_NAME);
    strcat( (char *) princ_name, "@");
    strcat( (char *) princ_name, rp);
    rpc_server_register_auth_info(princ_name, rpc_c_authn_dce_secret,
                                  (rpc_auth_key_retrieval_fn_t)rs_get_auth_key,
                                  (void *)NULL, stp);
    if (BAD_STATUS(stp)) return;

    /* regsiter priv server name, so priv server can use real auth RPC */
    strcpy( (char *) princ_name, SEC_PRIV_SERVER_NAME);
    strcat( (char *) princ_name, "@");
    strcat( (char *) princ_name, rp);
    rpc_server_register_auth_info(princ_name, rpc_c_authn_dce_secret,
                                  (rpc_auth_key_retrieval_fn_t)rs_get_auth_key,
                                  (void *)NULL, stp);
    if (BAD_STATUS(stp)) return;

}

/*  c o n s t r u c t _ c r e d h _ f r o m _ l o c a l _ n a m e
 *
 *  Parse global_namep to get cell and principal names and uuids.
 *  IFF the cell is local, retrieve the principal's account and projlist
 *  information in order to construct a PAC.
 *  Return success only if a PAC was constructed.
 *  Caller is not responsible for freeing pac resources because
 *  rsc_ss_allocated buffers get freed automatically.
 *  Currently, rs_login_get_info is the only operation utilizing this.
 */
PRIVATE void construct_credh_from_local_name (
    sec_rgy_name_t          global_namep,
    rpc_authz_cred_handle_t *cred_h,
    error_status_t          *stp
)
{
    rsec_id_output_selector_t   selector;
    sec_rgy_name_t              cell_namep, princ_namep;
    uuid_t                      cell_uuid, princ_uuid;
    sec_rgy_login_name_t        login_name;
    sec_rgy_cursor_t            cursor;
    sec_rgy_sid_t               sid;
    sec_rgy_unix_sid_t          unix_sid;
    sec_rgy_acct_key_t          key_parts;
    sec_rgy_acct_user_t         user_part;
    sec_rgy_acct_admin_t        admin_part;
    signed32                    num_returned, num_projects;
    uuid_t                      *projlist;
    sec_id_epac_data_t          epac_data;
    sec_id_epac_t               epac;
    sec_id_epac_set_t           epac_set;
    sec_bytes_t                 epac_chain;
    signed32                    *unix_projlist, i;
    sec__cred_auth_info_t       auth_info;

    static signed32             max_num = 10;
    CLEAR_STATUS(stp);
    cell_namep[0] = '\0';
    princ_namep[0] = '\0';
    selector = rsec_id_output_select_pname | rsec_id_output_select_cuuid
             | rsec_id_output_select_puuid;
    rs_sec_id_parse_name(sec_rgy_domain_person, global_namep, selector,
        cell_namep, &cell_uuid, princ_namep, &princ_uuid, stp);
    /* If not local, return status won't be 0 (rpc_status_ok). */
    if (BAD_STATUS(stp)) {
        return;
    }

    CLEAR_LOGIN_NAME(&login_name);
    ustrcpy(login_name.pname, princ_namep);
    rs_util_clear_cursor(&cursor);
    rsdb_acct_lookup(&login_name, &cursor, &sid, &unix_sid, &key_parts,
                        &user_part, &admin_part, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    rs_util_clear_cursor(&cursor);
    projlist = (uuid_t *)malloc(max_num * sizeof(*projlist));
    if (projlist == NULL) {
        SET_STATUS(stp, sec_rgy_cant_allocate_memory);
        return;
    }
    unix_projlist = (signed32 *)malloc(max_num * sizeof(*unix_projlist));
    if (unix_projlist == NULL) {
	free(projlist);
        SET_STATUS(stp, sec_rgy_cant_allocate_memory);
        return;
    }
    rsdb_acct_get_projlist(&login_name, &cursor, max_num,
                           &num_returned, projlist, unix_projlist,
                           &num_projects, stp);
    while ( STATUS_OK(stp) && (num_projects > max_num) ) {
        free(projlist);
        free(unix_projlist);
        max_num *= 2;
        rs_util_clear_cursor(&cursor);
        projlist = (uuid_t *)malloc(max_num * sizeof(*projlist));
        if (projlist == NULL) {
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
            return;
        }
        unix_projlist = (signed32 *)malloc(max_num * sizeof(*unix_projlist));
        if (unix_projlist == NULL) {
            free(projlist);
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
            return;
        }
        rsdb_acct_get_projlist(&login_name, &cursor, max_num,
                               &num_returned, projlist, unix_projlist,
                               &num_projects, stp);
    }
    if (BAD_STATUS(stp)) {
        free(projlist);
        free(unix_projlist);
        return;
    }

    /* Construct an EPAC from the data */
    memset(&epac_data, 0, sizeof(sec_id_epac_data_t));

    epac_data.pa.realm.uuid       = cell_uuid;
    epac_data.pa.realm.name       = (unsigned_char_p_t)NULL;
    epac_data.pa.principal.uuid   = princ_uuid;
    epac_data.pa.principal.name   = (unsigned_char_p_t)NULL;
    epac_data.pa.group.uuid       = sid.group;
    epac_data.pa.group.name       = (unsigned_char_p_t)NULL;
    epac_data.pa.num_groups       = num_returned;
    epac_data.pa.num_foreign_groupsets = 0;
    epac_data.pa.groups             = (sec_id_t *)NULL;
    epac_data.pa.foreign_groupsets  = (sec_id_foreign_groupset_t *)NULL;
    if (num_returned > 0) {
        epac_data.pa.groups = (sec_id_t *)
            rpc_ss_allocate(num_returned * sizeof(*(epac_data.pa.groups)));
        if (epac_data.pa.groups == NULL) {
            free(projlist);
            free(unix_projlist);
            SET_STATUS(stp, sec_rgy_cant_allocate_memory);
            return;
        }
        for( i = 0; i < num_returned; i++ ) {
            epac_data.pa.groups[i].uuid = projlist[i];
            epac_data.pa.groups[i].name = (unsigned_char_p_t)NULL;
        }
    }
    free(projlist);
    free(unix_projlist);
    sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, &epac_data,
        &epac.pickled_epac_data.num_bytes, &epac.pickled_epac_data.bytes,
        stp);
    if (BAD_STATUS(stp)) return;
    epac.seals = NULL;
    epac_set.num_epacs = 1;
    epac_set.epacs = &epac;
    sec_id_epac_set_encode((void *(*)(unsigned32))malloc, free, &epac_set,
        &epac_chain.num_bytes,	&epac_chain.bytes, stp);
    if (BAD_STATUS(stp)) {
	sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);
	return;
    }

    /* Clean up temporary epac buffers */
    sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);

    /* Now generate the dummy handle which will stick around for unauth calls*/
    auth_info.handle_type = sec__cred_handle_type_client;
    auth_info.authenticated = true;
    auth_info.authz_svc = rpc_c_authz_dce;
    auth_info.authn_svc = rpc_c_authn_dce_secret;
    *stp = sec__cred_create_authz_handle(&auth_info, &epac_chain, NULL, NULL,
        cred_h);
    sec_encode_buffer_free(free, &epac_chain.bytes);
}


/*
 * a u t h _ l e v e l s _ o k
 *
 * Verify authentication method and levels selected.  Guarantee that the
 * caller has a PAC to work with - if the authorization service was anything
 * other than authz_name or authz_dce, a reference to a PAC that is marked as
 * unauthenticated and anonymous is returned.
 * If name_ok == true (name-based authentication is valid) and the
 * authorization service is authz_name and the client is local, provide full
 * dce authorization for the client by contructing a full PAC for the client.
 */
PRIVATE boolean32 auth_levels_ok (
    rpc_authz_cred_handle_t *privs,
    unsigned32              authn_level,
    unsigned32              authn_svc,
    unsigned32              authz_svc,
    boolean32               name_ok,
    boolean32               *free_cred_h_p,
    error_status_t          *stp
)
{
    unsigned_char_p_t global_namep;
    CLEAR_STATUS(stp);

    if (authz_svc != rpc_c_authz_dce) {
        if (authz_svc == rpc_c_authz_name && name_ok) {
            sec_cred_get_client_princ_name(*privs, &global_namep, stp);

            /* If client is local, construct a PAC from the name*/
            if (GOOD_STATUS(stp)) {
                construct_credh_from_local_name(global_namep, privs, stp);
                if (GOOD_STATUS(stp))
                    *free_cred_h_p = true;
            }
            if (BAD_STATUS(stp)) {
                CLEAR_STATUS(stp);
                /* Set privs to an anonymous/dummy cred handle */
                *privs = dummy_cred_h;
            }
        } else {
            /* Set privs to an anonymous/dummy cred handle */
            *privs = dummy_cred_h;
	}
    }

    /*
     * Future: require protect_level_pkt_integ only for update operations.
     */
    /* Protection level must be pkt_integrity and authentication service
     * must be dce_secret except in unauthenticated case.
     */
    if ((authz_svc != rpc_c_authz_none) &&
        ((authn_level != rpc_c_protect_level_pkt_integ &&
          authn_level != rpc_c_protect_level_pkt_privacy) ||
         authn_svc != rpc_c_authn_dce_secret)) {
        SET_STATUS(stp, sec_rgy_not_authorized);
        return false;
    }

    return true;
}


/*
 * a n c e s t o r s _ a u t h o r i z e d
 *
 * Return true if the leaf (right-most) directory provides leaf_perms
 * according to the client privileges in the credential block.
 *
 * Dir_name must name an ACL-ed directory object. The first component of
 * dir_name is the domain name. Dir_name must neither begin, nor end,
 * with a slash ('/').
 */
PRIVATE boolean32 ancestors_authorized (
    rpc_authz_cred_handle_t cred_h,
    sec_acl_permset_t       leaf_perms,
    sec_rgy_name_t          dir_name,
    error_status_t          *stp
)
{
    uuid_t              *dir_mgr_type;
    sec_id_t            user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t      ids;
    sec_acl_permset_t   requested_perms;
    sec_acl_permset_t   net_rights;

    CLEAR_STATUS(stp);
    dir_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_dir]);

    user_p = &user_obj;
    group_p = &group_obj;
    rsdb_acl_get_ids_from_name(dir_name, sec_acl_type_object,
                          dir_mgr_type, &user_p, &group_p, &ids, stp);
    if (BAD_STATUS(stp)) {
        if (STATUS_EQUAL(stp, sec_acl_object_not_found)) {
            SET_STATUS(stp, sec_rgy_object_not_found);
        }
        return false;
    }
    requested_perms = leaf_perms;
    sec_acl_mgr_get_access((sec_acl_mgr_handle_t) NULL, cred_h,
        (sec_acl_key_t) &ids, dir_mgr_type, user_p, group_p, &net_rights, stp);
    if (BAD_STATUS(stp) || net_rights == (sec_acl_permset_t) 0) {
        SET_STATUS(stp, sec_rgy_object_not_found);
        return false;
    }
    if (requested_perms != (net_rights & requested_perms)) {
        SET_STATUS(stp, sec_rgy_not_authorized);
        return false;
    }

    return true;
}


PRIVATE boolean32 check_locksmith(
    rpc_binding_handle_t    h,
    rpc_authz_cred_handle_t cred_h,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t	    *stp
)
{
    sec_cred_pa_handle_t    pa_h;
    sec_id_pa_t             *pa_p;
    boolean32               retval = false;

    pa_h = sec_cred_get_initiator(cred_h, stp);
    if (BAD_STATUS(stp))
	return false;
    pa_p = sec_cred_get_pa_data(pa_h, stp);
    if (BAD_STATUS(stp))
	return false;

    /* Copy UUIDs from epac. */
    client_ids->principal = pa_p->principal.uuid;
    client_ids->cell = pa_p->realm.uuid;

    /* If secd is in locksmith mode and pac identifies the locksmith
     * principal, skip normal access checking.
     */
    if (locksmith_info.locksmith_mode) {
        if (uuid_equal(&(locksmith_info.uuid), &(pa_p->principal.uuid), stp)) {
            /* If remote locksmith not allowed, verify that client is local */
            if (!locksmith_info.remote_ok) {
                if (!rs_server_bindings_local_client(h, stp)) {
                    SET_STATUS(stp, sec_rgy_object_not_found);
                    return false;
                }
            }
            retval = true;
        }
    }
    return retval;
}

/*
 * r s _ i s _ a u t h o r i z e d _ b y _ c r e d s _ n o _ l o c k s m i t h
 *
 * Return true if the client represented by sec_id_pac_t pac has the
 * requested_perms access to object_name.  The client must also have
 * parent_perms access to the parent directory, if any, of object_name.
 * If no special parent permissions are required, input parent_perms
 * should be NULL. Mgr_type is provided to distinquish between objects of
 * different types with the same name.
 *
 * Object_name should be of the form to name an ACL-ed object For example,
 * for the RGY object, object_name is "policy". For a pgo or directory,
 * object_name looks like: domain_name/<dir_list>/<pgo_name> (i.e.
 * person/r_d/joe).  Object_name must not contain initial slashes ('/').
 *
 * Certain rs-level operations (namely rs_pgo_add and rs_acct_add) must
 * check the quota field of the client principal. This service is
 * performed if check_quota is true and if authorization has succeeded.
 *
 * A false return is always accompanied by a bad status code. True is always
 * returned with successful status code.
 *
 * Status Codes: sec_rgy_not_authorized - client not authorized for
 * requested perms. sec_rgy_object_not_found - name didn't identify any
 * object OR no x-permission on ancestors.
 * sec_acl_unknown_manager_type - input manager type uuid not
 * legal. sec_acl_invalid_manager_type - input manager type not valid for
 * object. sec_acl_invalid_acl_type - acl_type invalid for named object.
 * sec_acl_cant_allocate_memory - malloc failure during operation.
 * sec_rgy_bad_domain - 1st component of object_name not valid as domain.
 * sec_rgy_bad_name - invalid object_name. sec_rgy_quota_exhausted -
 * client's quota exhausted. sec_rgy_foreign_quota_exhausted - client's
 * cell's quota exhausted.
 */
PUBLIC boolean32 rs_is_authorized_by_creds_no_locksmith (
    rpc_authz_cred_handle_t cred_h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
)
{
    sec_id_t            user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t      ids;
    sec_acl_permset_t   net_rights;
    sec_rgy_name_t      parent_name;

    /*
     * If object_name has more than one component, check authorization for
     * parent of object_name.
     */
    if (rsdb_name_util_get_parent(object_name, parent_name)
         && parent_perms != NULL ) {
        /*
         * object_name has more than one component; thus parent
         * directories must be processed.
         */
        if (!ancestors_authorized(cred_h, parent_perms, parent_name, stp)) {
            /* Pass thru status from ancestors_authorized. */
            return false;
        }
    }

    /* Now check permissions on the object itself. */
    user_p = &user_obj;
    group_p = &group_obj;
    rsdb_acl_get_ids_from_name(object_name, sec_acl_type_object,
                               mgr_type, &user_p, &group_p, &ids, stp);
    if (BAD_STATUS(stp)) {
        if (STATUS_EQUAL(stp, sec_acl_object_not_found)) {
            SET_STATUS(stp, sec_rgy_object_not_found);
        }
        return false;
    }
    sec_acl_mgr_get_access((sec_acl_mgr_handle_t) NULL, cred_h,
	(sec_acl_key_t) &ids, mgr_type, user_p, group_p, &net_rights, stp);
    if (BAD_STATUS(stp) || net_rights == (sec_acl_permset_t) 0) {
        SET_STATUS(stp, sec_rgy_object_not_found);
        return false;
    }
    if (requested_perms != (net_rights & requested_perms)) {
        SET_STATUS(stp, sec_rgy_not_authorized);
        return false;
    }
    return true;
}

/*
 * r s _ i s _ a u t h o r i z e d _ b y _ c r e d s
 *
 * Return true if the client represented by sec_id_pac_t pac has the
 * requested_perms access to object_name.  The client must also have
 * parent_perms access to the parent directory, if any, of object_name.
 * If no special parent permissions are required, input parent_perms
 * should be NULL. Mgr_type is provided to distinquish between objects of
 * different types with the same name.
 *
 * Object_name should be of the form to name an ACL-ed object For example,
 * for the RGY object, object_name is "policy". For a pgo or directory,
 * object_name looks like: domain_name/<dir_list>/<pgo_name> (i.e.
 * person/r_d/joe).  Object_name must not contain initial slashes ('/').
 *
 * Certain rs-level operations (namely rs_pgo_add and rs_acct_add) must
 * check the quota field of the client principal. This service is
 * performed if check_quota is true and if authorization has succeeded.
 *
 * A false return is always accompanied by a bad status code. True is always
 * returned with successful status code.
 *
 * Status Codes: sec_rgy_not_authorized - client not authorized for
 * requested perms. sec_rgy_object_not_found - name didn't identify any
 * object OR no x-permission on ancestors.
 * sec_acl_unknown_manager_type - input manager type uuid not
 * legal. sec_acl_invalid_manager_type - input manager type not valid for
 * object. sec_acl_invalid_acl_type - acl_type invalid for named object.
 * sec_acl_cant_allocate_memory - malloc failure during operation.
 * sec_rgy_bad_domain - 1st component of object_name not valid as domain.
 * sec_rgy_bad_name - invalid object_name. sec_rgy_quota_exhausted -
 * client's quota exhausted. sec_rgy_foreign_quota_exhausted - client's
 * cell's quota exhausted.
 */
PUBLIC boolean32 rs_is_authorized_by_creds (
    handle_t                h,
    rpc_authz_cred_handle_t cred_h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
)
{
    boolean32           client_is_locksmith = false;

    client_is_locksmith = check_locksmith(h, cred_h, client_ids, stp);
    if (BAD_STATUS(stp))
	return false;

    /* If client is the locksmith, skip access check - but check quota */
    if (client_is_locksmith) {
        if ( check_quota && (!rsdb_util_quota_avail(
                                  &(client_ids->principal),
                                  &(client_ids->cell), stp)))
            return false;
        return true;
    }

    /* check for authorization before checking for quota */
    if (! rs_is_authorized_by_creds_no_locksmith(cred_h, requested_perms, 
		parent_perms, object_name, mgr_type, client_ids, stp))
        return false ;

    if (check_quota && !rsdb_util_quota_avail(
                            &(client_ids->principal),
                            &(client_ids->cell), stp))
        return false ;

    return true ;
}


/*
 * r s _ i s _ a u t h o r i z e d
 *
 * Return true if the client represented in handle h is both properly
 * authenticated for DCE and has the requested_perms access to object_name.
 * The client must also have parent_perms access to the parent directory (if
 * any) of object_name.  If no special parent permissions are required,
 * parent_perms should be set to NULL. Mgr_type is provided to distinquish
 * between objects of different types with the same name.
 *
 * In addition to the permissions on object_name and (optionally) its
 * parent, the client must have execute (x) permission on every ancestor
 * directory in object_name.  This is simply to enable initial access to
 * object_name.  If one of the ancestors doesn't provide execute permissions,
 * access is denied, even if object_name itself provides full privileges.
 *
 * Object_name should be of the form to name an ACL-ed object For example,
 * for the RGY object, object_name is NULL. For a pgo or directory,
 * object_name looks like: domain_name/<dir_list>/<pgo_name> (i.e.
 * person/r_d/joe).  Object_name must not contain initial slashes ('/').
 *
 * Certain rs-level operations (namely rs_pgo_add and rs_acct_add) must
 * check the quota field of the client principal. This service is
 * performed is check_quota is true and if authorization has succeeded.
 *
 * A false return is always accompanied by a bad status code. True is always
 * returned with successful status code.
 *
 *  Status Codes:
 *      sec_rgy_not_authorized - client not authorized for requested perms
 *      sec_rgy_object_not_found - name didn't identify any object.
 *      sec_acl_unknown_manager_type - input manager type uuid not legal.
 *      sec_acl_invalid_manager_type - input manager type not valid for object
 *      sec_acl_invalid_acl_type - acl_type invalid for named object.
 *      sec_acl_cant_allocate_memory - malloc failure during operation.
 *      sec_rgy_bad_domain - 1st component of object_name not valid as domain.
 *      sec_rgy_bad_name - invalid object_name.
 *      sec_rgy_quota_exhausted - client's quota exhausted.
 *      sec_rgy_foreign_quota_exhausted - client's cell's quota exhausted.
 */
PUBLIC boolean32 rs_is_authorized (
    handle_t                h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
)
{
    rpc_authz_cred_handle_t cred_h;
    if (!(rs_auth_get_creds_from_handle(h, &cred_h, stp))) {
        return false;
    }

    return (rs_is_authorized_by_creds(h, cred_h, requested_perms, parent_perms,
                    object_name, mgr_type, check_quota, client_ids, stp));
}


/*
 * r s _ a u t h _ i s _ a u t h o r i z e d
 *
 * This operation is identical to rs_is_authorized with the exception that the
 * input object_name is replaced by two inputs: a domain and an object_name
 * within that domain. See rs_is_authorized for description and status codes.
 */
PUBLIC boolean32 rs_auth_is_authorized (
    handle_t                h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_domain_t        domain,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
)
{
    sec_rgy_name_t  full_obj_name;
    CLEAR_STATUS(stp);

    rsdb_name_util_complete_name(domain, object_name, full_obj_name, stp);

    return (rs_is_authorized(h, requested_perms, parent_perms, full_obj_name,
                             mgr_type, check_quota, client_ids, stp));
}


/*
 * r s _ a u t h _ i s _ a u t h _ b y _ n a m e _ o k
 *
 *  Try to perform standard authorization check.
 *  If standard authorization fails and if the client's authorization service
 *  is rpc_c_authz_name, allow PAC to be constructed and continue authorization
 *  check.
 *  CAUTION: This operation depends on the construct_pac_from_local_name
 *  operation which uses rpc_ss_allocate for pac resources.  Because of this,
 *  this operation must be called as part of a wire operation so that the
 *  necessary support for rpc_ss_allocate and automagical freeing takes place.
 *  Currently, rs_login_get_info is the only operation utilizing this.
 */
PUBLIC boolean32 rs_auth_is_auth_by_name_ok (
    handle_t                h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_domain_t        domain,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
)
{
    sec_rgy_name_t          full_obj_name;
    rpc_authz_cred_handle_t creds;
    error_status_t          tmp_st;
    boolean32               name_ok = true, retval, free_cred_h = false;

    CLEAR_STATUS(stp);

    rsdb_name_util_complete_name(domain, object_name, full_obj_name, stp);

    if (!(rs_auth_get_creds_from_hdl_name_ok( h, name_ok, &creds,
	&free_cred_h, stp ))) {
        return false;
    }

    retval = rs_is_authorized_by_creds(h, creds, requested_perms, parent_perms,
        full_obj_name, mgr_type, check_quota, client_ids, stp);
    
    if (free_cred_h)
        tmp_st = sec__cred_free_cred_handle(&creds);
    return retval;
}


/*
 * r s _ a u t h _ g e t _ c r e d s _ f r o m _ h a n d l e
 *
 * Get the creds from the input handle.
 */
PUBLIC boolean32 rs_auth_get_creds_from_handle (
    handle_t                h,
    rpc_authz_cred_handle_t *cred_hP,
    error_status_t          *stp
)
{
    boolean32 dummy;
    CLEAR_STATUS(stp);
    return(rs_auth_get_creds_from_hdl_name_ok(h, false, cred_hP, &dummy, stp));
}


/*
 * r s _ a u t h _ g e t _ c r e d s _ f r o m _ h d l _ n a m e _ o k
 *
 * Get the creds from the input handle.
 */
PRIVATE boolean32 rs_auth_get_creds_from_hdl_name_ok (
    handle_t                h,
    boolean32               name_ok,
    rpc_authz_cred_handle_t *privs,
    boolean32               *free_cred_h_p,
    error_status_t          *stp
)
{
    unsigned_char_p_t   svr_princ_name = NULL;
    unsigned32          authn_level, authn_svc, authz_svc;
    error_status_t      tmp_status;

    CLEAR_STATUS(stp);

    rpc_binding_inq_auth_caller(h, privs, &svr_princ_name, &authn_level,
                                &authn_svc, &authz_svc, stp);
    if (STATUS_EQUAL(stp, rpc_s_binding_has_no_auth)) {
        authz_svc   = rpc_c_authz_none;
        authn_svc   = rpc_c_authn_none;
        authn_level = rpc_c_authn_level_none;
        /* Dummy creds block for use in later access checks */
        *privs = dummy_cred_h;
    } else if (BAD_STATUS(stp)) {
        SET_STATUS(stp, sec_rgy_not_authorized);
        return false;
    }

    if (!auth_levels_ok(privs, authn_level, authn_svc, authz_svc, name_ok,
        free_cred_h_p, stp)) {
        SET_STATUS(stp, sec_rgy_not_authorized);
        if (svr_princ_name) {
            rpc_string_free(&svr_princ_name, &tmp_status);
        }
        return false;
    }

    if (svr_princ_name) {
        rpc_string_free(&svr_princ_name, &tmp_status);
    }
    return true;
}


/*
 * r s _ g e t _ a u t h _ k e y
 *
 * Retrieve the auth key from the account for the principal named by
 * svr_princ_name.  The value for svr_princ_name is the same as the
 * server name passed in via previous rpc_server_register_auth_info.
 * Currently, this function valid only for the registry server principal
 * Key versioning not yet implemented. Key gets freed by kerberos.
 */
PUBLIC rpc_auth_key_retrieval_fn_t rs_get_auth_key (
    void                    *arg,
    unsigned_char_p_t       svr_princ_name,
    unsigned32              key_type,
    unsigned32              key_version,
    void                    **key,
    unsigned32              *stp
)
{
    unsigned_char_p_t   namep = NULL;
    char                *cp;
    boolean32           default_pepper;
    krb5_kvno           mkey_version;

    CLEAR_STATUS(stp);

    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_db, stp) {

        /*
         * check for kerberos format string name (princ@realm)
         */
         cp =  strchr( (char *) svr_princ_name, '@');
         if (cp != NULL) {
             *cp = '\0';
             namep = svr_princ_name;
         } else {
             sec_rgy_name_t     cell_name;
             int                cell_name_len;

             /*
              *  Maybe it's the DCE global form of our server principal name
              *  in which case we strip the cell prefix
              */
             rsdb_properties_get_realm_name(cell_name);
             cell_name_len = ustrlen(cell_name);
             if (ustrncmp(svr_princ_name, cell_name, cell_name_len) == 0) {
                 namep = svr_princ_name + cell_name_len + 1/* for the "/" */;
             } else {
                 namep = svr_princ_name;   /* take it exactly as it came in */
             }
        }

        if ((ustrcmp(namep, SEC_RGY_SERVER_NAME) != 0) && 
            (ustrcmp(namep, SEC_PRIV_SERVER_NAME) != 0)) {
            SET_STATUS(stp, sec_rgy_bad_name);
        } else {
            if (key_version == 0) {
                sec_passwd_rec_t  *prec;
                /*
                 * the ticket came encrypted under a session key, which means
                 * the client might be the master registry replica, and is using
                 * *our* session key with the security service to authenticate.
                 * Go for the session key first.
                 */
                rs_rep_auth_get_init_skey(&prec, stp);
                if (GOOD_STATUS(stp)) {
                    *key = (void *)sec_to_krb_key(prec, stp);
                    sec_crypt_free_key(&prec);
                }
            }
            /*
             * If the key version is not zero, or the key version was zero but
             * the attempt to get the session key failed then try a database
             * lookup (which will return the current key if key version = 0)
             */
            if ((key_version == 0 && BAD_STATUS(stp)) || key_version != 0) {
		rsdb_acct_get_auth_key_by_name(namep, NULL,
					       key_type, key_version,
                                               (krb5_keyblock **) key,
					       &default_pepper, NULL,
                                               &mkey_version, stp);
            }
        }
    } END_READ_EVENT
}


/*
 * r s _ a u t h _ p g o _ u p d a t e _ c h e c k
 *
 * Compare the data in old_pgo with the data in new_pgo. If the data for a
 * given field differs between the two pgos, set the permission bit in
 * required_perms that write-protects that field.  If a field that does
 * not appear in the following table has changed, return sec_rgy_bad_data.
 *
 * Field                   Permission
 * -----                   ----------
 * unix_num                m   (modifiable for cell principals only)
 * quota                   m
 * flags                   m
 * fullname                f
 */
PUBLIC void rs_auth_pgo_update_check (
    sec_rgy_pgo_item_t      *old_pgo,
    sec_rgy_pgo_item_t      *new_pgo,
    boolean32               is_cell,
    sec_acl_permset_t       *required_perms,
    error_status_t          *stp
)
{
    CLEAR_STATUS(stp);
    *required_perms = NULL;

    if (old_pgo->unix_num != new_pgo->unix_num ||
        old_pgo->quota != new_pgo->quota || old_pgo->flags != new_pgo->flags) {
        *required_perms |= sec_acl_perm_mgmt_info;
    }
    if (ustrcmp(old_pgo->fullname, new_pgo->fullname) != 0) {
        *required_perms |= sec_acl_perm_fullname;
    }
}


/*
 * r s _ a u t h _ a c c t _ a d m i n _ u p d a t e _ c h e c k
 *
 * Compare the data in old_admin_part with the data in new_admin_part. If
 * the data for a given field differs between the two pgos, set the
 * permission bit in required_perms that write-protects that field.  If a
 * field that does not appear in the following table has changed, return
 * sec_rgy_bad_data.
 *
 * Field                   Permission
 * -----                   ----------
 * expiration_date         m
 * flags                   m
 * good_since_date         a
 * authentication_flags    a
 */
PUBLIC void rs_auth_acct_admin_update_check (
    sec_rgy_acct_admin_t    *old_admin_part,
    sec_rgy_acct_admin_t    *new_admin_part,
    sec_acl_permset_t       *required_perms,
    error_status_t          *stp
)
{
    CLEAR_STATUS(stp);
    *required_perms = NULL;

    if (old_admin_part->expiration_date != new_admin_part->expiration_date ||
        old_admin_part->flags != new_admin_part->flags) {
        *required_perms |= sec_acl_perm_mgmt_info;
    }
    if (old_admin_part->good_since_date != new_admin_part->good_since_date ||
        old_admin_part->authentication_flags !=
        new_admin_part->authentication_flags) {
        *required_perms |= sec_acl_perm_auth_info;
    }
}


/*
 * compute the effective account expiration date
 */
PRIVATE sec_timeval_sec_t effective_acct_expiration (
    sec_rgy_acct_admin_t  *admin,
    sec_rgy_plcy_t        *policy
)
{
    sec_timeval_sec_t  acct_exp;

    if (policy->acct_lifespan == 0 /* live forever */) {
        acct_exp = admin->expiration_date;
    } else if (admin->expiration_date == 0  /* never expire */) {
        acct_exp = admin->creation_date + policy->acct_lifespan;
    } else {
        acct_exp = MIN(admin->expiration_date,
                      (admin->creation_date + policy->acct_lifespan));
    }

    return acct_exp;
}


/*
 * compute the effective passwd expiration date
 */
PRIVATE sec_timeval_sec_t effective_passwd_expiration (
    sec_rgy_acct_user_t  *user,
    sec_rgy_plcy_t       *policy
)
{
    sec_timeval_sec_t    passwd_exp, tmp_exp;

    if (policy->passwd_lifetime == 0) {
        /* Use expiration date, unless password's been changed since then */
        passwd_exp = (policy->passwd_exp_date > user->passwd_dtm) ?
                                                policy->passwd_exp_date : 0;
    } else if (policy->passwd_exp_date == 0) {
        passwd_exp = user->passwd_dtm + policy->passwd_lifetime;
    } else {
        /* Expiration date, unless password's been changed since then */
        tmp_exp = (policy->passwd_exp_date > user->passwd_dtm) ?
                                                policy->passwd_exp_date : 0;
        if (tmp_exp == 0) {
            passwd_exp = user->passwd_dtm + policy->passwd_lifetime;
        } else {
            passwd_exp = MIN(tmp_exp,
                                (user->passwd_dtm + policy->passwd_lifetime));
        }
    }

    return passwd_exp;
}


/*  g e t _ y e s _ o r _ n o
 *
 *  Prompt the user for 'y' or 'n' input followed by <CR>.
 *  For 'y', set yes = true; for 'n', set yes = false.
 *  If just a <CR> is returned, return without changing
 *  value of yes (i.e. input value of yes is the default).
 */
PRIVATE void get_yes_or_no (
    unsigned32      num_tries,
    boolean32       *yes,
    error_status_t  *stp
)
{
    unsigned32      retries_left;
    int             i, m ;
    char            c    ;

    SET_STATUS(stp, sec_rgy_bad_data);
    retries_left = num_tries;
    while ( BAD_STATUS(stp) && retries_left > 0 ) {

        if ( (i = getchar()) == '\n') {
            CLEAR_STATUS(stp);
            return ;
        }
        c = i ; 
        if ( getchar() == '\n' &&  ((m = dce_rpmatch(&c)) != -1) ) {
                *yes = m ;
                CLEAR_STATUS(stp);
                return ;
        }
        else  /* discard chars from stdin */
            while (getchar() != '\n')
                ;

        if (--retries_left) {
            dce_printf(sec_lksm_bad_input);
            dce_printf( (*yes) ? sec_lksm_def_yes_prompt : sec_lksm_def_no_prompt );
        }
    }
}


/*
 *  g e t _ l o c k s m i t h _ p a s s w d
 *
 *  Caller must free passwd->pepper if non-null.
 */
PRIVATE void get_locksmith_passwd (
    sec_rgy_name_t      pname,
    signed32            passwd_str_len,
    unsigned_char_p_t   passwd_str,
    sec_passwd_rec_t    *passwd,
    error_status_t      *stp
)
{
    krb5_data           pepper;
    unsigned char       *pwd_prompt1 ;
    unsigned char       *pwd_prompt2 ;

    CLEAR_STATUS(stp);
    pepper.data = 0;
    pepper.length = 0;

    PASSWD_TYPE(passwd)	   = sec_passwd_plain;
    passwd->pepper         = NULL;
    passwd->version_number = sec_passwd_c_version_none;

    pwd_prompt1 = dce_sprintf(sec_lksm_passwd_prompt);
    pwd_prompt2 = dce_sprintf(sec_lksm_passwd_verify);
    if ( !pwd_prompt1 || !pwd_prompt2 ) { /* Error */
        SET_STATUS(stp, sec_svc_cant_get_msg);
        return;
    }
    
    *stp = krb5_read_password((char *) pwd_prompt1,  (char *)pwd_prompt2,
                              (char *) passwd_str, (int *) &passwd_str_len);
    if (BAD_STATUS(stp))
        return;
    PLAIN_PASSWD(passwd) = passwd_str;

    rsdb_acct_gen_default_pepper(pname, &pepper, stp);
    if (BAD_STATUS(stp))
        return;
    if (!(passwd->pepper = malloc(pepper.length + 1))) {
        SET_STATUS(stp, sec_rgy_cant_allocate_memory);
    } else {
        u_strncpy(passwd->pepper, pepper.data, pepper.length);
        passwd->pepper[pepper.length] = '\0';
    }
    if (pepper.data)
        free(pepper.data);
}


/*
 *  r s _ a u t h _ s e t u p _ l o c k s m _ c r e a t o r
 *
 *  Use the dce-rgy principal as the "owner" of all updates required for
 *  setup_locksmith.  Setup the creator_ids and the creator_key for use in
 *  locksmith updates and update logging.
 *  Ensure creator has adequate quota for adding objects to registry.
 *  Creator_key should be freed by caller with sec_crypt_free_key.
 */
PRIVATE void rs_auth_setup_locksm_creator (
    sec_rgy_foreign_id_t    *creator_ids,
    sec_passwd_rec_t        **creator_key,
    error_status_t          *st_p
)
{
    sec_rgy_cursor_t        cursor;
    krb5_keyblock           *krb_key = 0;
    boolean32               default_pepper;
    krb5_kvno               mkey_version;
    sec_rgy_pgo_item_t      pgo;
    sec_rgy_properties_t    properties;
    char                    buf[64];
    sec_rgy_name_t          creator_name;

    CLEAR_STATUS(st_p);
    *creator_key = 0;

    /* Use dce-rgy as the creator of locksmith updates */
    cursor.valid = false;
    strcpy((char *)creator_name, (char *)SEC_RGY_SERVER_NAME);
    rsdb_pgo_get_by_name(sec_rgy_domain_person, creator_name,
                         &cursor, &pgo, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    if (pgo.quota != sec_rgy_quota_unlimited) {
        pgo.quota = sec_rgy_quota_unlimited;
        rsdb_pgo_replace(sec_rgy_domain_person,
                         (unsigned_char_p_t)SEC_RGY_SERVER_NAME, &pgo, st_p);
        if (GOOD_STATUS(st_p)) {
            sprintf(buf, "PGO replace (%s)", (char *)SEC_PRINCIPAL_OBJ_NAME);
            rs_audit_trail_log((handle_t)0, buf, (char *)SEC_RGY_SERVER_NAME);
            rs_log_pgo_replace(sec_rgy_domain_person,
                (char *)SEC_RGY_SERVER_NAME, &pgo, NULL_PTR);
        } else {
            return;
        }
    }

    creator_ids->principal = pgo.id;
    rsdb_properties_get_info( &properties, st_p );
    if (BAD_STATUS(st_p)) {
        return;
    }
    creator_ids->cell = properties.realm_uuid;

    /* Get the creator's key */
    rsdb_acct_get_auth_key_by_name((unsigned_char_p_t)SEC_RGY_SERVER_NAME,
	NULL,
        KEYTYPE_DES, sec_passwd_c_version_none, &krb_key, &default_pepper,
	NULL, &mkey_version, st_p);
    if (BAD_STATUS(st_p)) {
        krb_key = 0;
        return;
    }
    (*creator_key) = krb_to_sec_key(krb_key, st_p);
    krb5_free_keyblock(krb_key);
}

/*
 * Verify that acct and policy values are reasonable for
 * normal cell operation (at least in locksmith mode).
 */
PRIVATE void check_acct_policy_values
(
    sec_rgy_login_name_t 	*login_name,
    sec_rgy_cursor_t 		*cursor,
    sec_rgy_acct_key_t 		key_parts,
    sec_rgy_acct_user_t 	*acct_user,
    sec_rgy_acct_admin_t 	*acct_admin,
    sec_passwd_rec_t        	*passwd,
    sec_passwd_version_t    	*passwd_version_number,
    sec_passwd_rec_t        	*creator_key,
    rs_acct_parts_t 	        modify_parts,
    sec_timeval_sec_t       	creation_date,
    sec_rgy_foreign_id_t	creator_ids,
    error_status_t		*stp
)
{
    sec_rgy_plcy_t          policy;
    sec_timeval_sec_t       acct_expire_time;
    sec_timeval_sec_t       pwd_expire_time;
    sec_timeval_period_t    min_required_time = 3600;       /* one hour */
    boolean32               set_policy = false;
    char                    buf[64];
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    rs_acct_key_transmit_t  *pickled_key = NULL;
    rs_acct_key_transmit_t  *pickled_unix_key = NULL;

    CLEAR_STATUS(stp);

    if (!(IS_SET(acct_user->flags, sec_rgy_acct_user_passwd_valid))) {
        acct_user->flags |= sec_rgy_acct_user_passwd_valid;
        modify_parts |= rs_acct_part_user;
        dce_printf(sec_lksm_set_pwd_valid, login_name->pname);
    }
    if (!(IS_SET(acct_admin->flags, sec_rgy_acct_admin_client))) {
        acct_admin->flags |= sec_rgy_acct_admin_client;
        modify_parts |= rs_acct_part_admin;
        dce_printf(sec_lksm_set_client_valid, login_name->pname);
    }
    if (!(IS_SET(acct_admin->flags, sec_rgy_acct_admin_server))) {
        acct_admin->flags |= sec_rgy_acct_admin_server;
        modify_parts |= rs_acct_part_admin;
        dce_printf(sec_lksm_set_server_valid, login_name->pname);
    }
    if (!(IS_SET(acct_admin->flags, sec_rgy_acct_admin_valid))) {
        acct_admin->flags |= sec_rgy_acct_admin_valid;
        modify_parts |= rs_acct_part_admin;
        dce_printf(sec_lksm_set_acct_valid, login_name->pname);
    }
    if (acct_admin->good_since_date > creation_date) {
        acct_admin->good_since_date = creation_date;
        modify_parts |= rs_acct_part_admin;
        dce_printf(sec_lksm_set_date_now, login_name->pname);
    }
    rsdb_policy_get_effective(login_name->oname, &policy, stp);
    if (BAD_STATUS(stp))
        goto out;
    acct_expire_time = effective_acct_expiration(acct_admin, &policy);

    if ( acct_expire_time != 0 &&
        acct_expire_time < creation_date + min_required_time) {
        /* make sure policy.acct_lifespan is not the limiting factor */
        if ((policy.acct_lifespan != 0) &&
           (acct_admin->creation_date + policy.acct_lifespan <
            creation_date + min_required_time)) {
            policy.acct_lifespan =
                (creation_date + min_required_time) - acct_admin->creation_date;
            set_policy = true;
            dce_printf(sec_lksm_set_acct_span, 
                       (policy.acct_lifespan/60), (policy.acct_lifespan/3600),
                        (policy.acct_lifespan/(3600*24)));
        } else {
            policy.acct_lifespan = rsdb_policy_field_not_set;
        }
        acct_admin->expiration_date = creation_date + min_required_time;
        modify_parts |= rs_acct_part_admin;
        dce_printf(sec_lksm_set_expire, login_name->pname, 
                   (min_required_time/60));
    }
    pwd_expire_time = effective_passwd_expiration(acct_user, &policy);
    if ( pwd_expire_time != 0 &&
        pwd_expire_time < creation_date + min_required_time) {
        /* make sure policy.passwd_exp_date is not a limiting factor */
        if ((policy.passwd_exp_date != 0) &&
           (policy.passwd_exp_date > acct_user->passwd_dtm) &&
           (policy.passwd_exp_date < creation_date + min_required_time)) {
            policy.passwd_exp_date = creation_date + min_required_time;
            set_policy = true;
            dce_printf(sec_lksm_set_polpwd_expire_now,
                       (min_required_time/60));
        } else {
            policy.passwd_exp_date = rsdb_policy_field_not_set;
        }
        /* make sure policy.passwd_lifetime is not a limiting factor */
        if ((policy.passwd_lifetime != 0) &&
           (acct_user->passwd_dtm + policy.passwd_lifetime <
            creation_date + min_required_time)) {
            policy.passwd_lifetime =
                (creation_date + min_required_time) - acct_user->passwd_dtm;
            set_policy = true;
	    dce_printf(sec_lksm_set_polpwd_expire,
                       (policy.passwd_lifetime/60));
        } else {
            policy.passwd_lifetime = rsdb_policy_field_not_set;
        }
    }
    if (set_policy) {
        policy.passwd_min_len           = rsdb_policy_field_not_set;
        policy.passwd_flags             = rsdb_policy_field_not_set;
        rsdb_policy_set_less_restrictive(login_name->oname, &policy, stp);
        if (BAD_STATUS(stp))
            goto out;
    }
    if (modify_parts) {
        sec_timeval_sec_t  now;

        rs_util_apply_timestamp(&now);
        rsdb_acct_replace(login_name, key_parts, modify_parts,
                          acct_user, acct_admin, passwd, true,
                          &now, passwd_version_number, stp);

        if (GOOD_STATUS(stp)) {
            sprintf(buf, "Replace %s account (%d)",
                login_name->pname, sec_rgy_acct_key_person);
            sprintf(name_buf, "%s.%s.%s", (char *)login_name->pname,
                   (char *)login_name->gname, (char *)login_name->oname);
            rs_audit_trail_log((handle_t)NULL, buf, (char *) name_buf);

            if (modify_parts & rs_acct_part_passwd) {
                /* Generate the transmit_rep of the key for log */
                rca_pwd_gen_transmit_rep((sec_login_handle_t)0,
                    creator_key, passwd, &pickled_key, stp);

                if (GOOD_STATUS(stp)) {
                    rs_pwd_unix_pwd_to_xmit(creator_key,
                        acct_user->passwd, &pickled_unix_key, stp);
                }
            }

            if (GOOD_STATUS(stp)) {
                rs_log_acct_replace(login_name, modify_parts, acct_user,
                   acct_admin, pickled_key, &creator_ids, sec_passwd_des,
                    pickled_unix_key, &now, 0);
            }
        }
    }
out:
    if (pickled_key)
        rca_pwd_free_transmit_rep(&pickled_key);

    if (pickled_unix_key)
        rca_pwd_free_transmit_rep(&pickled_unix_key);

}

PUBLIC void rs_auth_setup_locksmith (
    rsdb_locksmith_info_t   *locksmith_info_p,
    unsigned_char_p_t       locksmith_name_p,
    boolean32               prompt_for_locksmith_key,
    error_status_t          *stp
)
{
    sec_rgy_login_name_t    login_name;
    sec_rgy_cursor_t        cursor;
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_acct_user_t     acct_user;
    sec_rgy_acct_admin_t    acct_admin;
    rs_acct_parts_t         modify_parts = 0;
    sec_passwd_rec_t        passwd;
    sec_passwd_version_t    passwd_version_number;
    char                    passwd_str[sec_passwd_str_t_size];
    signed32                passwd_len = sec_passwd_str_max_len;
    rs_acct_key_transmit_t  *pickled_key = NULL;
    rs_acct_key_transmit_t  *pickled_unix_key = NULL;
    sec_passwd_rec_t        *creator_key = 0;
    sec_rgy_pgo_item_t      pgo;
    sec_rgy_foreign_id_t    creator_ids;
    sec_timeval_sec_t       creation_date;
    struct timezone         tz;
    sec_timeval_t           curr_time;
    error_status_t          tmp_status;
    boolean32               yes = true, dummy, passwd_input = false;
    char                    buf[64], name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    unsigned_char_t         *hostname_p = 0;

    CLEAR_STATUS(stp);

    locksmith_info = *locksmith_info_p;
    passwd.pepper = NULL;
    CLEAR_LOGIN_NAME(&login_name);
    ustrcpy(login_name.pname, locksmith_name_p);
#ifdef SNI_SVR4_POSIX
    gettimeofday((struct timeval *) &curr_time);
#else
    gettimeofday((struct timeval *) &curr_time, &tz);
#endif
    creation_date = curr_time.sec;

    /* Use dce-rgy as the creator of locksmith updates */
    /* Don't forget to free creator_key when finished */
    rs_auth_setup_locksm_creator(&creator_ids, &creator_key, stp);
    if (BAD_STATUS(stp)) {
        goto out;
    }

    cursor.valid = false;
    rsdb_acct_lookup( &login_name, &cursor, &sid, &unix_sid,
                      &key_parts, &acct_user, &acct_admin, stp );
    if (BAD_STATUS(stp)) {
        dce_printf(sec_lksm_create_acct, locksmith_name_p);
        dce_printf(sec_lksm_def_yes_prompt);
        get_yes_or_no(5, &yes, &tmp_status);
        if (BAD_STATUS(&tmp_status) || !yes)
            goto out;      /* with status from acct_lookup */

        /* Create locksmith account, creating PGOs if necessary */
        ustrcpy(login_name.gname, "none");
        ustrcpy(login_name.oname, "none");

        /* Make sure principal exists */
        cursor.valid = false;
        rsdb_pgo_get_by_name(sec_rgy_domain_person, login_name.pname,
                             &cursor, &pgo, stp);
        if (STATUS_OK(stp)) {
            locksmith_info.uuid = pgo.id;
        } else {
            /* create pgo for locksmith principal */
            uuid_create_nil(&(pgo.id), stp);
            pgo.unix_num = sec_rgy_uxid_unknown;
            rsdb_unix_uuid_gen_check(sec_rgy_domain_person, true,
                                     (int *) &(pgo.unix_num), &(pgo.id), stp);
            if (BAD_STATUS(stp))
                goto out;
            locksmith_info.uuid = pgo.id;
            pgo.fullname[0] = '\0';
            pgo.quota = sec_rgy_quota_unlimited;
            pgo.flags = sec_rgy_pgo_projlist_ok;
            rs_util_pgo_add(sec_rgy_domain_person, login_name.pname,
                            &pgo, NULL_PTR, &creator_ids, true, &dummy, stp);
            if (STATUS_OK(stp)) {
                rs_audit_trail_log((handle_t)NULL, "Add locksmith principal",
                                   (char *)login_name.pname);
                rs_log_pgo_add(sec_rgy_domain_person, login_name.pname,
                               &pgo, &creator_ids, NULL_PTR);
            }

            if (BAD_STATUS(stp))
                goto out;
        }

        /* Make sure group exists */
        cursor.valid = false;
        rsdb_pgo_get_by_name(sec_rgy_domain_group, login_name.gname,
                             &cursor, &pgo, stp);
        if (BAD_STATUS(stp)) {
            /* create pgo for locksmith group */
            uuid_create_nil(&(pgo.id), stp);
            pgo.unix_num = sec_rgy_uxid_unknown;
            rsdb_unix_uuid_gen_check(sec_rgy_domain_group, true,
                                     (int *) &(pgo.unix_num), &(pgo.id), stp);
            if (BAD_STATUS(stp))
                goto out;
            pgo.fullname[0] = '\0';
            pgo.quota = 0;
            pgo.flags = sec_rgy_pgo_projlist_ok;
            rs_util_pgo_add(sec_rgy_domain_group, login_name.gname,
                            &pgo, NULL_PTR, &creator_ids, true, &dummy, stp);
            if (STATUS_OK(stp)) {
                rs_audit_trail_log((handle_t)NULL, "Add locksmith group",
                                   (char *)login_name.gname);
                rs_log_pgo_add(sec_rgy_domain_group, login_name.gname,
                               &pgo, &creator_ids, NULL_PTR);
            }
            if (BAD_STATUS(stp))
                goto out;
        }

        /* Make sure org exists */
        cursor.valid = false;
        rsdb_pgo_get_by_name(sec_rgy_domain_org, login_name.oname,
                             &cursor, &pgo, stp);
        if (BAD_STATUS(stp)) {
            /* create pgo for locksmith org */
            uuid_create_nil(&(pgo.id), stp);
            pgo.unix_num = sec_rgy_uxid_unknown;
            rsdb_unix_uuid_gen_check(sec_rgy_domain_org, true,
                                     (int *) &(pgo.unix_num), &(pgo.id), stp);
            if (BAD_STATUS(stp))
                goto out;
            pgo.fullname[0] = '\0';
            pgo.quota = 0;
            pgo.flags = sec_rgy_pgo_projlist_ok;
            rs_util_pgo_add(sec_rgy_domain_org, login_name.oname,
                            &pgo, NULL_PTR, &creator_ids, true, &dummy, stp);
            if (STATUS_OK(stp)) {
                rs_audit_trail_log((handle_t)NULL, "Add locksmith org",
                                   (char *)login_name.oname);
                rs_log_pgo_add(sec_rgy_domain_org, login_name.oname,
                               &pgo, &creator_ids, NULL_PTR);
            }
            if (BAD_STATUS(stp))
                goto out;
        }
        /* Make sure required memberships exist */
        rsdb_pgo_add_member( sec_rgy_domain_group, login_name.gname,
                             login_name.pname, stp);
        if (STATUS_OK(stp)) {
            sprintf(buf, "PGO add member (%s/%s)", (char *)SEC_GROUP_OBJ_NAME,
                    login_name.gname);
            rs_audit_trail_log((handle_t)NULL, buf, (char *)login_name.pname);
            rs_log_pgo_add_member(sec_rgy_domain_group, login_name.gname,
                                  login_name.pname, NULL_PTR, NULL_PTR,
				  NULL_PTR);
        }

        if (BAD_STATUS(stp))
            goto out;
        rsdb_pgo_add_member( sec_rgy_domain_org, login_name.oname,
                             login_name.pname, stp);
        if (STATUS_OK(stp)) {
            sprintf(buf, "PGO add member (%s/%s)", (char *)SEC_ORG_OBJ_NAME,
                    login_name.oname);
            rs_audit_trail_log((handle_t)NULL, buf, (char *)login_name.pname);
            rs_log_pgo_add_member(sec_rgy_domain_org, login_name.oname,
                                  login_name.pname, NULL_PTR, NULL_PTR,
				  NULL_PTR);
        }
        if (BAD_STATUS(stp))
            goto out;

        /* PGOs exist, now create account */
        acct_admin.creator              = creator_ids;
        acct_admin.creation_date        = creation_date;
        acct_admin.last_changer         = creator_ids;
        acct_admin.change_date          = creation_date;
        acct_admin.expiration_date      = 0;
        acct_admin.good_since_date      = creation_date;
        acct_admin.flags                = SEC_DEF_FLGS_ACCT_ADMIN_LOGIN;
        acct_admin.authentication_flags = SEC_DEF_FLGS_ACCT_AUTH;

        acct_user.gecos[0]              = '\0';
        ustrcpy( acct_user.homedir, "/" );
        ustrcpy( acct_user.shell, "" );
        acct_user.passwd_dtm            = creation_date;
        acct_user.flags                 = sec_rgy_acct_user_passwd_valid;

        get_locksmith_passwd(login_name.pname, passwd_len,
                             (unsigned_char_p_t)passwd_str, &passwd, stp);
        if (BAD_STATUS(stp))
            goto out;
        passwd_input = true;

        rsdb_acct_add(&login_name, sec_rgy_acct_key_person, &acct_user,
             &acct_admin, &passwd, true, &passwd_version_number, stp);
        if (GOOD_STATUS(stp)) {
            sprintf(buf, "Add locksmith account (%d)",sec_rgy_acct_key_person);
            sprintf(name_buf, "%s.%s.%s", (char *)login_name.pname,
                   (char *)login_name.gname, (char *)login_name.oname);
            rs_audit_trail_log((handle_t)NULL, buf, (char *) name_buf);

            /* Generate the transmit_rep of the key for log */
            /* Don't need a login_handle since pepper is provided in passwd */
            rca_pwd_gen_transmit_rep((sec_login_handle_t)0,
                creator_key, &passwd, &pickled_key, stp);

            /* Generate the unix passwd transmit_rep for the log */
            if (GOOD_STATUS(stp)) {
                rs_pwd_unix_pwd_to_xmit(creator_key,
                    acct_user.passwd, &pickled_unix_key, stp);
            }

            if (GOOD_STATUS(stp)) {
                rs_log_acct_add(&login_name, &acct_user, &acct_admin,
                    pickled_key, &creator_ids, sec_passwd_des,
                    pickled_unix_key, 0);
            }
        }

        if (BAD_STATUS(stp))
            goto out;

        if (pickled_key != NULL) {
            rca_pwd_free_transmit_rep(&pickled_key);
            pickled_key = NULL;
        }

        if (pickled_unix_key != NULL) {
            rca_pwd_free_transmit_rep(&pickled_unix_key);
            pickled_unix_key = NULL;
        }

    } else {    /* acct_lookup succeeded */
        locksmith_info.uuid = sid.person;
    }

    if (prompt_for_locksmith_key && !passwd_input) {
        get_locksmith_passwd(login_name.pname, passwd_len,
                             (unsigned_char_p_t)passwd_str,
                             &passwd, stp);
        if (BAD_STATUS(stp))
            goto out;
        acct_user.passwd_dtm = creation_date;
        modify_parts |= rs_acct_part_passwd | rs_acct_part_user;
    }

    check_acct_policy_values(&login_name, &cursor, key_parts,
	&acct_user, &acct_admin, &passwd, 
	&passwd_version_number,
	creator_key, modify_parts, 
	creation_date, creator_ids, stp);
    if (BAD_STATUS(stp))
	goto out;

    passwd.pepper = NULL;
    modify_parts = NULL;
    cursor.valid = false;
    CLEAR_LOGIN_NAME(&login_name);
    ustrcpy(login_name.pname, "dce-rgy");

    rsdb_acct_lookup( &login_name, &cursor, &sid, &unix_sid,
                      &key_parts, &acct_user, &acct_admin, stp );
    if (BAD_STATUS(stp))
	goto out;

    check_acct_policy_values(&login_name, &cursor, key_parts,
	&acct_user, &acct_admin, &passwd, 
	&passwd_version_number,
	creator_key, modify_parts, 
	creation_date, creator_ids, stp);
    if (BAD_STATUS(stp))
	goto out;

    passwd.pepper = NULL;
    modify_parts = NULL;
    cursor.valid = false;
    CLEAR_LOGIN_NAME(&login_name);

    /* Get the local host principal name */
    dce_cf_prin_name_from_host(0, (char **)&hostname_p, stp);

    if(BAD_STATUS(stp)) {
        goto out;
    } else {
        if(u_strncmp(CELL_DIR_ROOT, hostname_p, CELL_DIR_ROOT_LEN) == 0) {
            u_strcpy(login_name.pname, hostname_p + CELL_DIR_ROOT_LEN);
        } else {
            u_strcpy(login_name.pname, hostname_p);
        }
        free(hostname_p);
    }

    rsdb_acct_lookup( &login_name, &cursor, &sid, &unix_sid,
                      &key_parts, &acct_user, &acct_admin, stp );
    if (BAD_STATUS(stp))
	goto out;

    check_acct_policy_values(&login_name, &cursor, key_parts,
	&acct_user, &acct_admin, &passwd, 
	&passwd_version_number,
	creator_key, modify_parts, 
	creation_date, creator_ids, stp);

out:
    if (pickled_key)
        rca_pwd_free_transmit_rep(&pickled_key);

    if (pickled_unix_key)
        rca_pwd_free_transmit_rep(&pickled_unix_key);

    if (creator_key)
        sec_crypt_free_key(&creator_key);

    if (passwd.pepper)
        free(passwd.pepper);

    return;
}
