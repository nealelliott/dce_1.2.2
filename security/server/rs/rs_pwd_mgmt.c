/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_pwd_mgmt.c,v $
 * Revision 1.1.6.2  1996/10/03  15:10:25  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:44 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Change sec_passwd_privkey to sec_passwd_pubkey
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1996/01/03  19:39 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.6.1  1996/08/09  12:10:03  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1996/01/03  19:39 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	Merge fix for CHFts15858
 * 	[1995/10/30  15:40 UTC  mullan_s  /main/HPDCE02/3]
 * 
 * 	CHFts15858: Always bind to pwd_strengthd with name-based
 * 	authorization to avoid pwd_strengthd sec_id_gen_name() callback.
 * 	[1995/10/23  20:26 UTC  mullan_s  /main/HPDCE02/mullan_chfts15858/1]
 * 
 * 	[1994/12/05  19:46 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	[CHFts13749]: Ignore all errors returned by lookup of pwd_val_type ERA.
 * 	This is because pwd_val_type may not exist as a schema type.
 * 	[1994/11/28  18:14 UTC  mullan_s  /main/mullan_mothra_bugfix/1]
 * 
 * Revision 1.1.4.2  1996/02/18  00:19:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:44  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:55:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/30  15:40 UTC  mullan_s
 * 	Merge fix for CHFts15858
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15858/1  1995/10/23  20:26 UTC  mullan_s
 * 	CHFts15858: Always bind to pwd_strengthd with name-based
 * 	authorization to avoid pwd_strengthd sec_id_gen_name() callback.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  19:46 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_bugfix/1  1994/11/28  18:14 UTC  mullan_s
 * 	[CHFts13749]: Ignore all errors returned by lookup of pwd_val_type ERA.
 * 	This is because pwd_val_type may not exist as a schema type.
 * 	[1995/12/08  17:21:17  root]
 * 
 * Revision 1.1.2.6  1994/09/09  20:41:09  kline_s
 * 	OT 12028: I18N code cleanup for Security.
 * 	[1994/09/09  20:35:01  kline_s]
 * 
 * Revision 1.1.2.5  1994/09/01  19:53:02  mullan_s
 * 	[OT11775] Should free both instances of str_info_out.bytes.
 * 	[1994/09/01  19:51:35  mullan_s]
 * 
 * Revision 1.1.2.4  1994/08/31  16:01:21  mullan_s
 * 	[OT11775] Free memory contained within str_info_out.
 * 	[1994/08/31  16:00:33  mullan_s]
 * 
 * Revision 1.1.2.3  1994/07/15  15:02:05  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:21:04  mdf]
 * 
 * Revision 1.1.2.2  1994/06/17  18:42:50  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:16:16  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/11  19:31:02  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Contains interfaces for password strength and generation checks.
 * 
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:45:57  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994.
 * Unpublished work. All Rights Reserved.
 */

#include <dce/dce.h>
#include <rs_pwd_mgmt_pvt.h>

#include <rgymacro.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_tools.h>
#include <rs_pwd_mgmt.h>

#include <dce/rsec_pwd_mgmt.h>

/* Globals */
enum     pwd_val_type { none, user_select, user_can_select,
                        generation_required };

/*
 * This routine determines if the error returned by the
 * RPC runtime when binding to a server is fatal or non-fatal.
 */
PRIVATE boolean32 retry_status (
    error_status_t *op_stp
)
{
    boolean32 retval;

    switch(*op_stp) {
        case error_status_ok:
            retval = false;
            break;

        case rpc_s_wrong_boot_time:
        case rpc_s_comm_failure:
        case rpc_s_endpoint_not_found:  /* ??? */
        case rpc_s_connection_closed:
        case rpc_s_connect_timed_out:
        case rpc_s_connect_rejected:
        case rpc_s_network_unreachable:
        case rpc_s_connect_no_resources:
        case rpc_s_rem_network_shutdown:
        case rpc_s_too_many_rem_connects:
        case rpc_s_no_rem_endpoint:
        case rpc_s_rem_host_down:
        case rpc_s_host_unreachable:
        case rpc_s_access_control_info_inv: /* ??? */
        case rpc_s_connect_closed_by_rem:
        case rpc_s_rem_host_crashed:
        case rpc_s_tsyntaxes_unsupported:
        case rpc_s_cant_getpeername:
        case rpc_s_unknown_reject:      /* ??? */
        case rpc_s_wrong_kind_of_binding:
        case rpc_s_call_timeout:
        case rpc_s_server_too_busy:
        case rpc_s_unknown_error:
        case rpc_s_socket_failure:
        case rpc_s_auth_skew:
        case rpc_s_not_listening:
        case rpc_s_rpcd_comm_failure:
        case rpc_s_object_not_found:
        case rpc_s_cthread_not_found:
        case rpc_s_invalid_rpc_protseq: /* ??? */
        case rpc_s_max_descs_exceeded:  /* ??? */
        case rpc_s_unknown_if:
        case rpc_s_unsupported_type:
        case rpc_s_loc_connect_aborted:
        case rpc_s_unknown_mgr_type:
        case ept_s_not_registered:

            /* XXX Serviceability - save status! */
            SET_STATUS(op_stp, sec_pwd_mgmt_svr_unavail);
            retval = true;
            break;

        case sec_pwd_mgmt_not_authorized:
        case sec_pwd_mgmt_svr_error:
        case sec_pwd_mgmt_proxy_req:
            retval = false;
            break;

        /* 
         * This probably means the password management server received
         * an internal error (hopefully, this case will be rare)
         */
        default:
            /* XXX Serviceability - save status! */
            SET_STATUS(op_stp, sec_pwd_mgmt_svr_error);
            retval = false;
            break;
    }

    return retval;
}


/*
 * set_auth_info sets up the binding handle to the password
 * management server to be authenticated.
 */
PRIVATE void set_auth_info (
    rpc_binding_handle_t    handle,                  /* [in, out] */
    unsigned char           *svr_princ_name,          /* [in] */
    unsigned32              protect_level,            /* [in] */
    unsigned32              authn_svc,                /* [in] */
    unsigned32              authz_svc,                /* [in] */
    error_status_t          *stp                      /* [out] */
)
{
    CLEAR_STATUS(stp);

    /*
     * Ignore authorization level stored in the pwd_mgmt_binding ERA
     * and always use name-based authorization to avoid pwd_strengthd
     * callback to secd via sec_id_gen_name() because it could cause
     * deadlock if there are many simultaneous strength requests.
     */
    rpc_binding_set_auth_info(handle, svr_princ_name,
        protect_level, authn_svc, NULL, rpc_c_authz_name, stp);

    if (BAD_STATUS(stp))
        return;
}


/*
 * string_bind_to_pwd_strength_server sets up a binding handle
 * to the password management server using string binding
 * information.
 */
PRIVATE void string_bind_to_pwd_strength_server (
    sec_attr_bind_info_t     *bind_info_p,             /* [in] */
    sec_passwd_rec_t         *pwd,                     /* [in] */
    signed32                 pwd_val_type,             /* [in] */
    unsigned32               num_attrs,                /* [in] */
    sec_attr_t               attrs[],                  /* [in] */
    sec_rgy_name_t           princ,                    /* [in] */
    error_status_t           *stp                      /* [out] */
)
{
    rpc_binding_handle_t    handle;
    error_status_t          lst;
    boolean32               valid;
    sec_bytes_t             str_info_in,
                            str_info_out;

    CLEAR_STATUS(stp);
    memset(&str_info_in, 0, sizeof(sec_bytes_t));
    memset(&str_info_out, 0, sizeof(sec_bytes_t));

    /*
     * get handle using string binding info stored in the ERA
     */
    rpc_binding_from_string_binding(SA_BND_STRING_P(bind_info_p, 0),
        &handle, stp);
    if (BAD_STATUS(stp))
        return;

    /*
     * set up authentication information
     */
    set_auth_info(handle, SA_BND_AUTH_SVR_PNAME_P(bind_info_p),
                  SA_BND_AUTH_PROT_LEVEL(bind_info_p),
                  SA_BND_AUTH_AUTHN_SVC(bind_info_p),
                  SA_BND_AUTH_AUTHZ_SVC(bind_info_p), stp);
    if (BAD_STATUS(stp)) {
        rpc_binding_free(&handle, &lst);
        return;
    }

    /*
     * XXX - If protection level is not privacy, the password
     * should NOT be sent to the password server unencrypted
     * over the wire. It should send the password to a proxy
     * and inform the password management server that it should
     * get it via a proxy - it would do this by sending creating
     * a pickle type for this information and sending it in the
     * str_info_in argument of rsec_pwd_mgmt_str_chk. However, 
     * proxy functionality is unavailable, thus we will send 
     * the password unencrypted to the server.
     */

    /*
     * call password strength interface. Notice the NULL
     * parameters for the pickles. There are currently no
     * pickles defined so there is no need to create variables
     * for these arguments.
     */
    valid = rsec_pwd_mgmt_str_chk(handle, princ, pwd, 
        pwd_val_type, num_attrs, attrs, str_info_in, &str_info_out, stp);
    rpc_binding_free(&handle, &lst);

    /* convert status to something the client can understand */
    if (BAD_STATUS(stp)) {
        (void) retry_status(stp);
        return;
    }
    if (!valid) {
        SET_STATUS(stp, sec_pwd_mgmt_str_check_failed);
        return;
    }

    /*
     * Currently no pickle types are defined, so free any info
     * in str_info_out.
     */
    if (str_info_out.bytes)
        free(str_info_out.bytes);
}
 

/*
 * svrname_bind_to_pwd_strength_server sets up a binding handle
 * to the password management server using name service
 * information.
 */
PRIVATE void svrname_bind_to_pwd_strength_server (
    sec_attr_bind_info_t     *bind_info_p,             /* [in] */
    sec_passwd_rec_t         *pwd,                     /* [in] */
    signed32                 pwd_val_type,             /* [in] */
    unsigned32               num_attrs,                /* [in] */
    sec_attr_t               attrs[],                  /* [in] */
    sec_rgy_name_t           princ,                    /* [in] */
    error_status_t           *stp                      /* [out] */
)
{
    rpc_ns_handle_t         import_context;
    rpc_binding_handle_t    handle;
    unsigned_char_p_t       hstr;
    error_status_t          lst;
    boolean32               valid;
    sec_bytes_t             str_info_in,
                            str_info_out;

    CLEAR_STATUS(stp);
    memset(&str_info_in, 0, sizeof(sec_bytes_t));
    memset(&str_info_out, 0, sizeof(sec_bytes_t));

    /*
     * Retrieve binding information from name service
     */
    rpc_ns_binding_import_begin(
        SA_BND_SVRNAME_P(bind_info_p, 0)->name_syntax,
        SA_BND_SVRNAME_P(bind_info_p, 0)->name,
        rsec_pwd_mgmt_v1_0_c_ifspec,
        NULL,
        &import_context,
        stp
    );
    if (BAD_STATUS(stp))
        return;

    /*
     * Loop through all binding handles until a good connection
     * is made
     */
    do {
        /*
         * Retrieve binding handle using binding info
         */
        rpc_ns_binding_import_next(import_context, &handle, stp);
        if (BAD_STATUS(stp))
            break;

        /* 
         * set up authentication information
         */
        set_auth_info(handle, SA_BND_AUTH_SVR_PNAME_P(bind_info_p),
                      SA_BND_AUTH_PROT_LEVEL(bind_info_p),
                      SA_BND_AUTH_AUTHN_SVC(bind_info_p),
                      SA_BND_AUTH_AUTHZ_SVC(bind_info_p), stp);
        if (BAD_STATUS(stp)) {
            rpc_binding_free(&handle, &lst);
            break;
        }

    /*
     * XXX - If protection level is not privacy, the password
     * should NOT be sent to the password server unencrypted
     * over the wire. It should send the password to a proxy
     * and inform the password management server that it should
     * get it via a proxy - it would do this by sending creating
     * a pickle type for this information and sending it in the
     * str_info_in argument of rsec_pwd_mgmt_str_chk. However,
     * proxy functionality is unavailable, thus we will send
     * the password unencrypted to the server.
     */

        /* 
         * Call password strength interface.
         * If failed because of communications, go
         * back and retry (after freeing handle). Otherwise break.
         */
        valid = rsec_pwd_mgmt_str_chk(handle, princ, pwd, 
            pwd_val_type, num_attrs, attrs, str_info_in, &str_info_out, stp);
        rpc_binding_free(&handle, &lst);
        if (!valid) {
            SET_STATUS(stp, sec_pwd_mgmt_str_check_failed);
            break;
        }

    } while (retry_status(stp));

    rpc_ns_binding_import_done(&import_context, &lst);

    /*
     * Currently no pickle types are defined, so free any info
     * in str_info_out.
     */
    if (str_info_out.bytes)
        free(str_info_out.bytes);
}

/*
 * bind_to_pwd_strength_server determines what type of
 * binding info is contained in the ERA and calls the
 * appropriate interface.
 *
 * XXX NOTE : This routine only handles 1 binding entry and
 * does not handle binding towers. This needs to be
 * enhanced.
 */
PRIVATE void bind_to_pwd_strength_server (
    sec_attr_bind_info_t     *bind_info_p,             /* [in] */
    sec_passwd_rec_t         *pwd,                     /* [in] */
    signed32                 pwd_val_type,             /* [in] */
    unsigned32               num_attrs,                /* [in] */
    sec_attr_t               attrs[],                  /* [in] */
    sec_rgy_name_t           princ,                    /* [in] */
    error_status_t           *stp                      /* [out] */
)
{
    CLEAR_STATUS(stp);

    /* XXX - enhance to check all entries in bind_info_p */
    switch (SA_BND_TYPE(bind_info_p, 0)) {

        case sec_attr_bind_type_string:
            string_bind_to_pwd_strength_server(bind_info_p, pwd,
                pwd_val_type, num_attrs, attrs, princ, stp);
            break;
        case sec_attr_bind_type_svrname:
            svrname_bind_to_pwd_strength_server(bind_info_p, pwd,
                pwd_val_type, num_attrs, attrs, princ, stp);
            break;
        case sec_attr_bind_type_twrs:
            SET_STATUS(stp, sec_s_not_implemented);
            return;
    }
}


/*
 * setup_policy_args looks for the well-known ERA pwd_plcy_args,
 * which is an attribute set. It adds the effective password
 * min length and password flags if they are not contained in
 * the ERA.
 *
 * XXX NOTE : this routine is far from complete. attribute sets
 * are not supported yet.
 */
PRIVATE void setup_policy_args (
    sec_rgy_login_name_t    login_name,  /* [in]  */
    rs_pwd_mgmt_plcy_t      **plcy_p,    /* [out] */
    error_status_t          *stp         /* [out] */
)
{
    sec_rgy_plcy_t    policy;
    unsigned32        num_attrs;

    CLEAR_STATUS(stp);
    num_attrs = 0;

    /*
     * look up effective policy
     */
    rsdb_policy_get_effective(login_name.oname, &policy, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /* allocate memory for plcy_p  -number of elements should be 1 less than
       what we need */
    *plcy_p = (rs_pwd_mgmt_plcy_t *) rpc_ss_allocate(sizeof(rs_pwd_mgmt_plcy_t)
              + (sizeof(sec_attr_t) * 1));
    if (*plcy_p == NULL) {
        SET_STATUS(stp, sec_s_no_memory);
        return;
    }

    /* first entry is policy.passwd_min_len */
    (*plcy_p)->plcy[num_attrs].attr_value.attr_encoding = sec_attr_enc_integer;
    (*plcy_p)->plcy[num_attrs].attr_value.tagged_union.signed_int = policy.passwd_min_len;

    /* second entry is policy.passwd_flags */
    (*plcy_p)->plcy[++num_attrs].attr_value.attr_encoding = sec_attr_enc_integer;
    (*plcy_p)->plcy[num_attrs].attr_value.tagged_union.signed_int = policy.passwd_flags;
    
    (*plcy_p)->num_plcy_args = ++num_attrs;
}


/*
 * rs_pwd_mgmt_check_plain checks a user's plaintext password
 * against the effective policy stored in the registry.
 */
PUBLIC void rs_pwd_mgmt_check_plain (
    char                     *pwd_text,
    signed32                 pwd_textlen,
    sec_rgy_plcy_t           rgy_plcy,
    error_status_t           *stp
)
{
    boolean32    all_spaces = true,
                 alpha_num  = true;
    int          i;

    CLEAR_STATUS(stp);

    if (pwd_textlen < rgy_plcy.passwd_min_len) {
        SET_STATUS(stp, sec_rgy_passwd_too_short);
        return;
    }

    /* step through each character in the passwd_text looking for a non-space
     * or a non-alpha-numeric character and set the value of all_spaces
     * and alpha_num based on what we find
     */
    for (i = 0; i < pwd_textlen && (all_spaces || alpha_num);
         i++, pwd_text++ ) {
        if (!isdcepcs(*pwd_text)) {
            /* not ascii ==> not blank or alpha-num */
            alpha_num = false;
            all_spaces = false;
        } else {
            if (!isalnum(*pwd_text)) {
                /* not alpha-num but might be blank */
                alpha_num = false;
                if (!(*pwd_text == ' ')) /* nope, not a blank either */
                    all_spaces = false;
            } else                    /* is alpha-num ==> not a blank */
                all_spaces = false;
        }
    }

    if (((rgy_plcy.passwd_flags & sec_rgy_plcy_pwd_non_alpha) != 0)
        && (alpha_num)) {
        SET_STATUS(stp, sec_rgy_passwd_non_alpha);
        return;
    } else if (((rgy_plcy.passwd_flags & sec_rgy_plcy_pwd_no_spaces) != 0)
        && (all_spaces)) {
        SET_STATUS(stp, sec_rgy_passwd_spaces);
        return;
    }
}


/*
 * rs_pwd_mgmt_strength_check determines if a user's password should
 * be strength checked, and if so by the password management server
 * or just by the effective registry policy.
 */
PUBLIC void rs_pwd_mgmt_strength_check (
    sec_rgy_login_name_t      *login_name,  /* [in] */
    sec_passwd_rec_t          *pwd,         /* [in] */
    error_status_t            *stp          /* [out] */
)
{
    sec_rgy_name_t            fullname;
    rsdb_util_rgy_obj_info_t  obj_info;
    uuid_t                    obj_uuid;
    sec_attr_t                sec_attr;
    sec_rgy_plcy_t            policy;
    error_status_t            save_status;
    unsigned32                num_attrs;
    sec_attr_t                attrs[2];
    signed32                  pwd_val_type;

    CLEAR_STATUS(stp);
    CLEAR_STATUS(&save_status);

    /* Do not strength test private keys */
    if ((PASSWD_TYPE(pwd) == sec_passwd_pubkey) ||
        (PASSWD_TYPE(pwd) == sec_passwd_genprivkey))
        return;

    /* concatenate pname with domain name */
    rsdb_name_util_complete_name(sec_rgy_domain_person, login_name->pname,
        fullname, stp);
    if (BAD_STATUS(stp))
        return;

    /* retrieve uuid and internal rsdb identification info */
    rsdb_util_get_obj_info_by_name(fullname, rsdb_pgo_type,
		sec_acl_type_object, &obj_info, &obj_uuid, stp);
    if (BAD_STATUS(stp))
        return;

    /*
     * look up effective policy
     */
    rsdb_policy_get_effective(login_name->oname, &policy, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /* check password against effective policy */
    if (pwd->key.key_type == sec_passwd_plain) {
        rs_pwd_mgmt_check_plain((char *) PLAIN_PASSWD(pwd),
            u_strlen(PLAIN_PASSWD(pwd)), policy, &save_status);
    }

    /* look up password_validation_type ERA */
    rsdb_attr_lookup_by_name(&obj_info, "pwd_val_type", rpc_ss_allocate,
                             &sec_attr, stp);

    /*
     * if pwd_val_type = none or there is no ERA
     * instance, then no password strength checking is required. 
     * Therefore, we assume password is good and return.
     */
    if (BAD_STATUS(stp) || (SA_ATTR_INTEGER(&sec_attr) == none)) {
        SET_STATUS(stp, save_status);
        return;
    }
    pwd_val_type = SA_ATTR_INTEGER(&sec_attr);

    /* look up pwd_mgmt_binding ERA */
    rsdb_attr_lookup_by_name(&obj_info, "pwd_mgmt_binding",
                             rpc_ss_allocate, &sec_attr, stp);
    if (BAD_STATUS(stp)) {
        if (STATUS_EQUAL(stp, sec_attr_inst_not_found))
            SET_STATUS(stp, save_status);
        return;
    }

    /*
     * make sure auth type is sec_attr_bind_auth_dce, because
     * we cannot send a password over the network in the clear!
     */
    if (SA_BND_AUTH_INFO_TYPE(SA_ATTR_BND_INFO_P(&sec_attr)) 
                              != sec_attr_bind_auth_dce) {
        SET_STATUS(stp, sec_rgy_era_pwd_mgmt_auth_type);
        return;
    }

    /* first entry is policy.passwd_min_len */
    attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[0].attr_value.tagged_union.signed_int = policy.passwd_min_len;

    /* second entry is policy.passwd_flags */
    attrs[1].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[1].attr_value.tagged_union.signed_int = policy.passwd_flags;

    num_attrs = 2;

    /* bind to password strength server */
    bind_to_pwd_strength_server(SA_ATTR_BND_INFO_P(&sec_attr), 
        pwd, pwd_val_type, num_attrs, attrs, login_name->pname, stp);
}

PUBLIC void rs_pwd_mgmt_setup (
    handle_t              h,                    /* [in]  */
    sec_rgy_login_name_t  login_name,           /* [in]  */
    sec_attr_bind_info_t  **pwd_mgmt_bind_info, /* [out] */
    rs_pwd_mgmt_plcy_t    **plcy_p,             /* [out] */
    signed32              *pwd_val_type,        /* [out] */
    error_status_t        *status               /* [out] */
)
{
    sec_rgy_name_t            fullname;
    rsdb_util_rgy_obj_info_t  obj_info;
    uuid_t                    obj_uuid;
    sec_attr_t                pwd_val_type_attr;
    sec_attr_t                pwd_mgmt_binding_attr;
    unsigned32                num_attrs;
    rs_pwd_mgmt_plcy_t        *tmp_plcy_p;

    *pwd_val_type = none;
    *pwd_mgmt_bind_info = NULL;
    *plcy_p = NULL;
    CLEAR_STATUS(status);

    /* concatenate princ_name with domain name */
    rsdb_name_util_complete_name(sec_rgy_domain_person, login_name.pname,
        fullname, status);
    if (BAD_STATUS(status))
        return;

    /* retrieve uuid and internal rsdb identification info */
    rsdb_util_get_obj_info_by_name(fullname, rsdb_pgo_type,
                                   sec_acl_type_object, &obj_info,
                                   &obj_uuid, status);
    if (BAD_STATUS(status))
        return;

    /* look up password_validation_type ERA */
    rsdb_attr_lookup_by_name(&obj_info, "pwd_val_type", rpc_ss_allocate,
                             &pwd_val_type_attr, status);

    /*
     * If ERA does not exist or is equal to none, return.
     */
    if (BAD_STATUS(status) || (SA_ATTR_INTEGER(&pwd_val_type_attr) == none)) {
        CLEAR_STATUS(status);
        return;
    }
    *pwd_val_type = SA_ATTR_INTEGER(&pwd_val_type_attr);

    /* 
     * look up pwd_mgmt_binding ERA.
     */
    rsdb_attr_lookup_by_name(&obj_info, "pwd_mgmt_binding", rpc_ss_allocate, 
                             &pwd_mgmt_binding_attr, status);
    if (BAD_STATUS(status)) {
        return;
    }

    /* retrieve binding info */
    *pwd_mgmt_bind_info = SA_ATTR_BND_INFO_P(&pwd_mgmt_binding_attr);

    /* retrieve policy arguments */
    setup_policy_args(login_name, &tmp_plcy_p, status);
    if (GOOD_STATUS(status)) {
        *plcy_p = tmp_plcy_p;
    }
}
