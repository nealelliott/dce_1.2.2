/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pwd_mgmt.c,v $
 * Revision 1.1.7.3  1996/02/18  00:07:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:14  marty]
 *
 * Revision 1.1.7.2  1995/12/08  18:06:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:13 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:50 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add ept_s_not_registered to compatibility status
 * 	check.
 * 	[1995/05/02  20:29 UTC  greg  /main/greg_sec_migrate2/1]
 * 
 * Revision 1.1.2.3  1994/08/31  16:04:38  mullan_s
 * 	[OT11775] Initialize and free contents of gen_info_out properly.
 * 	[1994/08/31  16:04:13  mullan_s]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:41  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:59  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  14:58:54  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:28  mdf]
 * 
 * 	/main/ODESSA_2/1  1994/06/15  17:47 UTC  mullan_s
 * 	Merge -from mullan_el_bl19 -to ODESSA_2
 * 
 * 	/main/mullan_el_bl19/12  1994/06/13  19:02 UTC  mullan_s
 * 	Update retry_status with sec_pwd_mgmt errors.
 * 
 * 	/main/mullan_el_bl19/11  1994/06/10  21:33 UTC  mullan_s
 * 	Change extensible datatypes in rsec_pwd_mgmt_gen_pwd to pickles.
 * 
 * 	/main/mullan_el_bl19/10  1994/06/10  17:56 UTC  mullan_s
 * 	Add support for multiple password generation to sec_pwd_mgmt_gen_pwd.
 * 
 * 	/main/mullan_el_bl19/9  1994/06/10  13:34 UTC  mullan_s
 * 	Update rsec_pwd_gen interface.
 * 
 * 	/main/mullan_el_bl19/8  1994/06/08  14:48 UTC  mullan_s
 * 	Check for null pointers before freeing.
 * 
 * 	/main/mullan_el_bl19/7  1994/06/01  20:46 UTC  mullan_s
 * 	In sec_pwd_mgmt_setup, set pwd_val_type to none if using
 * 	an old server.
 * 
 * 	/main/mullan_el_bl19/6  1994/06/01  17:49 UTC  mullan_s
 * 	Add comments.
 * 
 * 	/main/mullan_el_bl19/5  1994/05/31  16:58 UTC  mullan_s
 * 	Add policy argument to rs_pwd_mgmt_setup.
 * 
 * 	/main/mullan_el_bl19/4  1994/05/19  16:53 UTC  mullan_s
 * 	Change gen_pwd to a sec_passwd_rec_t.
 * 
 * 	/main/mullan_el_bl19/3  1994/05/17  19:21 UTC  mullan_s
 * 	Change gen_pwd arg of sec_pwd_mgmt_gen_pwd to a char **.
 * 
 * 	/main/mullan_el_bl19/1  1994/05/13  18:12 UTC  mullan_s
 * 	Client sec_pwd_mgmt APIs.
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * sec_pwd_mgmt APIs
 */

#include <sec_pwd_mgmt_pvt.h>

#include <stdlib.h>
#include <string.h>

#include <macros.h>
#include <rca_pvt.h>
#include <rs_pwd_mgmt.h>
#include <sec_attr_tools.h>
#include <sec_pwd_mgmt.h>
#include <u_str.h>

#include <dce/rpc.h>
#include <dce/rsec_pwd_mgmt.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_login.h>

#define Crs_pwd_mgmt_setup(CH,b,c,d,e,STATUS)\
        (*rs_pwd_mgmt_v1_0_c_epv.rs_pwd_mgmt_setup) (HANDLE(CH),b,c,d,e,STATUS)


PRIVATE void free_sec_attr_bind_info (
    sec_attr_bind_info_t     *bind_info    /* [in] */
)
{
    int                      i, j;

    if (bind_info == NULL)
        return;

    if (SA_BND_AUTH_INFO_TYPE(bind_info) ==
        sec_attr_bind_auth_dce) {
        if (SA_BND_AUTH_SVR_PNAME_P(bind_info)) {
            free(SA_BND_AUTH_SVR_PNAME_P(bind_info));
        }
    }
    for (i = 0; i < SA_BND_NUM(bind_info); i++) {
        switch (SA_BND_TYPE(bind_info, i)) {
        case sec_attr_bind_type_string:
            if (SA_BND_STRING_P(bind_info, i)) {
                free(SA_BND_STRING_P(bind_info, i));
            }
            break;
        case sec_attr_bind_type_twrs:
            if (SA_BND_TWRSET_P(bind_info, i)) {
                for (j = 0;
                     j < SA_BND_TWRSET_COUNT(bind_info, i);
                     j++) {
                    if (SA_BND_TWR_P(bind_info, i, j)) {
                        free(SA_BND_TWR_P(bind_info, i, j));
                    }
                }
                free(SA_BND_TWRSET_P(bind_info, i));
            }
        case sec_attr_bind_type_svrname:
            if (SA_BND_SVRNAME_P(bind_info, i)) {
                if (SA_BND_SVRNAME_NAME_P(bind_info, i)) {
                    free(SA_BND_SVRNAME_NAME_P(bind_info, i));
                }
                free(SA_BND_SVRNAME_P(bind_info, i));
            }
            break;
        } /* end switch on bind_type */
    } /* end for each binding */
}


PRIVATE void free_pwd_mgmt_info (
    sec_pwd_mgmt_info_t      **pwd_mgmt_info      /* [in]  */
)
{
    rs_pwd_mgmt_plcy_t       *plcy_p;
    int                      i;

    if (*pwd_mgmt_info) {
        free_sec_attr_bind_info((*pwd_mgmt_info)->pwd_mgmt_bind_info);
        if ((*pwd_mgmt_info)->pwd_mgmt_bind_info)
            free((*pwd_mgmt_info)->pwd_mgmt_bind_info);
        if ((*pwd_mgmt_info)->plcy_p) {
            plcy_p = (*pwd_mgmt_info)->plcy_p;
            for (i = 0; i < plcy_p->num_plcy_args; i++)
                sec_attr_util_inst_free_ptrs(&plcy_p->plcy[i]);
            free(plcy_p);
        }
        free(*pwd_mgmt_info);
    }
}


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


PRIVATE void set_auth_info (
    rpc_binding_handle_t    handle,                   /* [in, out] */
    unsigned char           *svr_princ_name,          /* [in] */
    unsigned32              protect_level,            /* [in] */
    unsigned32              authn_svc,                /* [in] */
    unsigned32              authz_svc,                /* [in] */
    sec_login_handle_t      auth_identity,            /* [in] */
    error_status_t          *stp                      /* [out] */
)
{
    CLEAR_STATUS(stp);

    rpc_binding_set_auth_info(handle, svr_princ_name,
        protect_level, authn_svc, auth_identity, authz_svc, stp);

    if (BAD_STATUS(stp))
        return;
}


PRIVATE void string_bind_to_pwd_mgmt_server (
    sec_attr_bind_info_t     *bind_info,
    int                      bind_num,
    sec_rgy_name_t           princ_name,
    sec_login_handle_t       auth_identity,
    unsigned32               num_attrs,
    sec_attr_t               attrs[],
    unsigned32               num_pwds,
    unsigned32               *num_returned,
    sec_passwd_rec_t         *gen_pwds,
    sec_bytes_t              *gen_info_out,
    error_status_t           *stp
)
{
    rpc_binding_handle_t     handle;
    error_status_t           lst;
    sec_bytes_t              gen_info_in;

    CLEAR_STATUS(stp);
    memset(&gen_info_in, 0, sizeof(sec_bytes_t));

    /*
     * get handle using string binding info stored in the ERA
     */
    rpc_binding_from_string_binding(SA_BND_STRING_P(bind_info, bind_num),
        &handle, stp);
    if (BAD_STATUS(stp))
        return;

    /*
     * set up authentication information
     */
    set_auth_info(handle, SA_BND_AUTH_SVR_PNAME_P(bind_info),
                  SA_BND_AUTH_PROT_LEVEL(bind_info),
                  SA_BND_AUTH_AUTHN_SVC(bind_info),
                  SA_BND_AUTH_AUTHZ_SVC(bind_info), 
                  auth_identity, stp);
    if (BAD_STATUS(stp)) {
        rpc_binding_free(&handle, &lst);
        return;
    }

    /*
     * call password generation operation
     */
    rsec_pwd_mgmt_gen_pwd(handle, princ_name, num_attrs, attrs, gen_info_in,
        num_pwds, num_returned, gen_pwds, gen_info_out, stp);
    rpc_binding_free(&handle, &lst);

    if (BAD_STATUS(stp)) {
        (void) retry_status(stp);
        return;
    }
}


PRIVATE void svrname_bind_to_pwd_mgmt_server (
    sec_attr_bind_info_t     *bind_info,
    int                      bind_num,
    sec_rgy_name_t           princ_name,
    sec_login_handle_t       auth_identity,
    unsigned32               num_attrs, 
    sec_attr_t               attrs[], 
    unsigned32               num_pwds,
    unsigned32               *num_returned,
    sec_passwd_rec_t         *gen_pwds,
    sec_bytes_t              *gen_info_out,
    error_status_t           *stp
)
{
    rpc_ns_handle_t          import_context;
    rpc_binding_handle_t     handle;
    error_status_t           lst;
    sec_bytes_t              gen_info_in;

    CLEAR_STATUS(stp);
    memset(&gen_info_in, 0, sizeof(sec_bytes_t));

    /*
     * Retrieve binding information from name service
     */
    rpc_ns_binding_import_begin(
        SA_BND_SVRNAME_P(bind_info, bind_num)->name_syntax,
        SA_BND_SVRNAME_P(bind_info, bind_num)->name,
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
        set_auth_info(handle, SA_BND_AUTH_SVR_PNAME_P(bind_info),
                      SA_BND_AUTH_PROT_LEVEL(bind_info),
                      SA_BND_AUTH_AUTHN_SVC(bind_info),
                      SA_BND_AUTH_AUTHZ_SVC(bind_info),
                      auth_identity, stp);
        if (BAD_STATUS(stp)) {
            rpc_binding_free(&handle, &lst);
            return;
        }

        /* call password generation operation */
        rsec_pwd_mgmt_gen_pwd(handle, princ_name, num_attrs, attrs, gen_info_in,
            num_pwds, num_returned, gen_pwds, gen_info_out, stp);
        rpc_binding_free(&handle, &lst);
    } while (retry_status(stp));

    rpc_ns_binding_import_done(&import_context, &lst);
}


PRIVATE void bind_to_pwd_mgmt_server (
    sec_attr_bind_info_t     *bind_info,
    sec_rgy_name_t           princ_name,
    sec_login_handle_t       auth_identity,
    unsigned32               num_attrs,
    sec_attr_t               attrs[],
    unsigned32               num_pwds,
    unsigned32               *num_returned,
    sec_passwd_rec_t         *gen_pwds,
    sec_bytes_t              *gen_info_out,
    error_status_t           *stp
)
{
    int                      i;

    CLEAR_STATUS(stp);

    /*
     * loop through all bindings until a good one
     * is found.
     */
    for (i = 0; i < SA_BND_NUM(bind_info); i++) {
        switch (SA_BND_TYPE(bind_info, i)) {

        case sec_attr_bind_type_string:
            string_bind_to_pwd_mgmt_server(bind_info, i, princ_name, 
                auth_identity, num_attrs, attrs, num_pwds, num_returned,
                gen_pwds, gen_info_out, stp);
            break;
        case sec_attr_bind_type_svrname:
            svrname_bind_to_pwd_mgmt_server(bind_info, i, princ_name, 
                auth_identity, num_attrs, attrs, num_pwds, num_returned,
                gen_pwds, gen_info_out, stp);
            break;
        case sec_attr_bind_type_twrs:
            SET_STATUS(stp, sec_s_not_implemented);
            return;
        default:
            /* XXX error?? */
            break;
        }

        /* if successful connection was made, break out and return */
        if (GOOD_STATUS(stp))
            break;
    }
}

/*
 * s e c _ p w d _ m g m t _ s e t u p
 *
 * This operation accepts as input or looks up the necessary
 * information regarding a user's password management policy. It 
 * returns a handle to this data.
 *
 * Input:
 *    context: a handle to a registry server used to obtain
 *           ERA information.
 *    login_name: the name of the account whose password is being
 *           created or changed.
 *    your_lc: the login context of the principal who is authorized
 *           to change or create the user's password. If NULL is 
 *           specified, the default security login context for the
 *           current address space is used.
 *    pwd_mgmt_bind_h: the binding handle to the password management
 *           server. If this value is NULL, it will retrieve a binding
 *           handle from the binding information contained in the
 *           pwd_mgmt_binding ERA.
 *
 * Output:
 *    pwd_mgmt_h: a handle to the data required to generate a password.
 *           This is an opaque pointer to a a sec_pwd_mgmt_info_t
 *           structure.
 *    stp: error status.
 *
 * Errors:
 *    sec_s_no_memory: unable to allocate memory.
 *    sec_rgy_server_unavailable: The DCE Registry Server is unavailable.
 *    error_status_ok: The call was successful.
 */
PUBLIC void sec_pwd_mgmt_setup (
    sec_pwd_mgmt_handle_t    *pwd_mgmt_h,       /* [out] */
    sec_rgy_handle_t         context,           /* [in]  */
    sec_rgy_login_name_t     login_name,        /* [in]  */
    sec_login_handle_t       your_lc,           /* [in]  */
    rpc_binding_handle_t     pwd_mgmt_bind_h,   /* [in]  */
    error_status_t           *stp               /* [out] */
)
{
    sec_pwd_mgmt_handle_t    save = *pwd_mgmt_h; /* just in case */
    sec_pwd_mgmt_info_t      **pwd_mgmt_info = 
                             (sec_pwd_mgmt_info_t **) pwd_mgmt_h;
    sec_pwd_mgmt_info_t      *pmip;
    sec_attr_bind_info_t     *pwd_mgmt_bind_info;
    signed32                 pwd_val_type;
    rs_pwd_mgmt_plcy_t       *plcy_p;

    *pwd_mgmt_h = NULL;
    CLEAR_STATUS(stp);

    if (pwd_mgmt_bind_h) {
        SET_STATUS(stp, sec_s_not_implemented);
        return;
    }

    /* allocate memory for pwd_mgmt_info */
    pmip = (sec_pwd_mgmt_info_t *) malloc(sizeof(sec_pwd_mgmt_info_t));
    if (pmip == NULL) {
        SET_STATUS(stp, sec_s_no_memory);
        return;
    }
    memset(pmip, 0, sizeof(sec_pwd_mgmt_info_t));

    /* copy login name */
    pmip->login_name = login_name;

    /* 
     * Copy login context handle. Note: if the user
     * purges or releases the context before it is
     * needed, there will be errors.
     */
    pmip->your_lc = your_lc;

    /* get rest of information from secd */
    SETUP_RETRY(rca_op_read, stp) {
        Crs_pwd_mgmt_setup(context, login_name, &pwd_mgmt_bind_info,
                           &plcy_p, &pwd_val_type, stp);
        NORMAL_CASE {
            pmip->pwd_val_type = pwd_val_type;
            pmip->pwd_mgmt_bind_info = pwd_mgmt_bind_info;
            pmip->plcy_p = plcy_p;
        }
        RETRY_CASE {
            /*
             * This is for avoiding compatibility problems
             * if a client accesses a server which does
             * not support the interface. The pwd val type is
             * set to none.
             */
            if (STATUS_EQUAL(stp, rpc_s_unknown_if)
	    || STATUS_EQUAL(stp, ept_s_not_registered)) {
                pmip->pwd_val_type = none;
                pmip->pwd_mgmt_bind_info = NULL;
                pmip->plcy_p = NULL;
                CLEAR_STATUS(stp);
                ABANDON_RETRY;
            }
        }
    }
    RETRY_END;

    if (BAD_STATUS(stp)) {
        free_pwd_mgmt_info(&pmip);
        return;
    }

    *pwd_mgmt_info = pmip;
}


/* s e c _ p w d _ m g m t _ g e t _ v a l _ t y p e
 *
 * Retrieves the value of the pwd_val_type ERA from the handle
 * specified by pwd_mgmt_h.
 *
 * Input:
 *    pwd_mgmt_h: a handle obtained from sec_pwd_mgmt_setup.
 *
 * Output:
 *    pwd_val_type: value of the pwd_val_type ERA. It can be one of
 *           4 values - NONE(0), USER_SELECT(1), USER_CAN_SELECT(2),
 *           GENERATION_REQUIRED(3).
 *    stp: error status.
 *
 * Errors:
 *    error_status_ok: The call was successful.
 *    No errors are defined at this time.
 */
PUBLIC void sec_pwd_mgmt_get_val_type (
    sec_pwd_mgmt_handle_t    pwd_mgmt_h,        /* [in]  */
    signed32                 *pwd_val_type,     /* [out] */
    error_status_t           *stp               /* [out] */
)
{
    sec_pwd_mgmt_info_t      *pmip;

    CLEAR_STATUS(stp);

    /*
     * Should the handle have a state? - setup, etc ...
     */

    pmip = (sec_pwd_mgmt_info_t *) pwd_mgmt_h;

    /*
     * It is the responsibility of the caller to determine
     * if pwd_val_type is bogus.
     */

    *pwd_val_type = pmip->pwd_val_type;
}


/* s e c _ p w d _ m g m t _ g e n _ p w d
 *
 * Generates a password by calling the password management server.
 *
 * Input:
 *    pwd_mgmt_h: a handle obtained from sec_pwd_mgmt_setup.
 *
 * Output:
 *    gen_pwd: the generated password.
 *    stp: error status.
 *
 * Errors:
 *    error_status_ok: The call was successful.
 *    sec_rgy_era_pwd_mgmt_auth_type: The pwd_mgmt_binding ERA
 *           must have authentication information
 *    rpc_binding_from_string_binding errors
 *    rpc_binding_set_auth_info errors
 *    sec_pwd_mgmt_svr_unavail: The Password Management server
 *           is unavailable
 *    sec_pwd_mgmt_svr_error: Password Management server generic
 *           error.
 *    rpc_ns_binding_import_begin errors
 *    rpc_ns_binding_import_next errors
 */
PUBLIC void sec_pwd_mgmt_gen_pwd (
    sec_pwd_mgmt_handle_t    pwd_mgmt_h,        /* [in]  */
    unsigned32               num_pwds,          /* [in]  */
    unsigned32               *num_returned,     /* [out] */
    sec_passwd_rec_t         gen_pwds[],        /* [out] */
    error_status_t           *stp               /* [out] */
)
{
    sec_pwd_mgmt_info_t      *pmip;
    sec_attr_bind_info_t     bind_info;
    unsigned32               num_attrs = 2;
    sec_attr_t               attrs[2];
    sec_bytes_t              gen_info_out;

    CLEAR_STATUS(stp);

    memset(&gen_info_out, 0, sizeof(sec_bytes_t));
    pmip = (sec_pwd_mgmt_info_t *) pwd_mgmt_h;

    /*
     * make sure auth type is sec_attr_bind_auth_dce, because
     * we cannot send a password over the network in the clear!
     */
    if (SA_BND_AUTH_INFO_TYPE(pmip->pwd_mgmt_bind_info)
                              != sec_attr_bind_auth_dce) {
        SET_STATUS(stp, sec_rgy_era_pwd_mgmt_auth_type);
        return;
    }

    /* bind to password management server */
    bind_to_pwd_mgmt_server(pmip->pwd_mgmt_bind_info, pmip->login_name.pname,
        pmip->your_lc, pmip->plcy_p->num_plcy_args, pmip->plcy_p->plcy, 
        num_pwds, num_returned, gen_pwds, &gen_info_out, stp);

    /* 
     * Currently no pickle types are defined, so free any info
     * in gen_info_out.
     */
    if (gen_info_out.bytes)
        free(gen_info_out.bytes);
}


/* s e c _ p w d _ m g m t _ f r e e _ h a n d l e
 *
 * Frees the handle returned by sec_pwd_mgmt_setup.
 *
 * Input:
 *    pwd_mgmt_h: a handle obtained from sec_pwd_mgmt_setup.
 * Output:
 *    stp: error status.
 * Errors:
 *    error_status_ok: The call was successful.
 *    No other errors are defined.
 *
 */
PUBLIC void sec_pwd_mgmt_free_handle (
    sec_pwd_mgmt_handle_t    *pwd_mgmt_h,       /* [in]  */
    error_status_t           *stp               /* [out] */
)
{
    sec_pwd_mgmt_info_t      *pmip;

    CLEAR_STATUS(stp);

    pmip = (sec_pwd_mgmt_info_t *) *pwd_mgmt_h;

    free_pwd_mgmt_info(&pmip);
}
