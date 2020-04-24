/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: daclbind.c,v $
 * Revision 1.1.16.2  1996/03/11  13:25:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:02  marty]
 *
 * Revision 1.1.16.1  1995/12/08  17:54:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/17  23:22 UTC  psn
 * 	Fix ACL core dump.
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/17  23:20 UTC  mullan_s
 * 	Fix ACL core dump.
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/17  22:09 UTC  psn
 * 	Fix ACL core dump.
 * 
 * 	HP revision /main/HPDCE02/4  1995/11/16  20:40 UTC  bartf
 * 	fix mem leaks
 * 	[1995/11/16  20:19 UTC  bartf  /main/HPDCE02/bartf_sec_aclmemleak.90/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  20:31 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/20  13:15 UTC  tatsu_s
 * 	Fixed sec_acl_bind_local()'s prototype.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/13  20:25 UTC  tatsu_s
 * 	Added sec_acl_bind_local().
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/13  17:55 UTC  burati
 * 	Merge fix adding sec_acl_bind_nsi_only to Mothra
 * 
 * 	HP revision /main/mb_mothra4/1  1995/06/12  19:42 UTC  burati
 * 	Add sec_acl_bind_nsi() for DCE core usage
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/09  10:22 UTC  sekhar
 * 	Merge to mothra.
 * 
 * 	HP revision /main/sekhar_mothra_fix/2  1995/05/24  12:51 UTC  sekhar
 * 	Fix sec__acl_check_binding() to check for additional rpc status codes.
 * 	In setup_auth() call rpc_ep_resolve_binding() when auth is sec_acl_bind_auth_none .
 * 
 * 	HP revision /main/sekhar_mothra_fix/1  1995/05/13  14:17 UTC  sekhar
 * 	[ CHFts14105 ]  call rpc_ep_resolve_binding in setup_auth() when unauthenticated.
 * 	[1995/12/08  16:28:02  root]
 * 
 * Revision 1.1.11.2  1994/08/23  15:14:44  burati
 * 	CR10633 Unable to bind to ACL object with non ascii PCS chars
 * 	CR8266 Mem allocation error and mem leak in sec_acl_bind
 * 	CR8037 sec_acl_bind not initializing ACL handle to NULL
 * 	[1994/08/22  22:41:36  burati]
 * 
 * Revision 1.1.11.1  1994/01/28  23:09:59  burati
 * 	Fix non-ANSI prototype for sec_acl_bind_auth()
 * 	[1994/01/27  04:44:35  burati]
 * 
 * 	Delegation/EPAC changes - add sec_acl_bind_auth (dlg_bl1)
 * 	[1994/01/19  19:57:35  burati]
 * 
 * Revision 1.1.9.2  1993/05/11  15:07:34  burati
 * 	CR7661 Don't try to bind to a DFS ACL if bind_to_entry is true
 * 	[1993/05/10  20:08:25  burati]
 * 
 * Revision 1.1.7.10  1993/03/18  22:43:24  burati
 * 	CR7458 Don't clear status if not rebound in sec__acl_check_binding()
 * 	[1993/03/18  22:18:49  burati]
 * 
 * Revision 1.1.7.9  1993/03/12  16:46:00  burati
 * 	CR7458 Try harder to rebind on failures during sec_acl_bind()
 * 	[1993/03/12  15:53:09  burati]
 * 
 * Revision 1.1.7.8  1993/01/21  22:23:47  burati
 * 	CR6760 Don't fall back to unauthenticated NSI manually.
 * 	[1993/01/21  19:54:26  burati]
 * 
 * Revision 1.1.7.7  1993/01/07  16:21:42  burati
 * 	CR6687 Wasn't initializing status to ok, so unauth rebind failed
 * 	[1993/01/06  15:53:36  burati]
 * 
 * Revision 1.1.7.6  1992/12/29  12:36:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:22  zeliff]
 * 
 * Revision 1.1.7.5  1992/12/14  21:02:33  burati
 * 	CR5899 Inquire object uuid again on rebind, without cache
 * 	[1992/12/14  19:45:38  burati]
 * 
 * Revision 1.1.7.4  1992/11/16  18:28:49  burati
 * 	CR4812 Allow unauthenticated binding
 * 	CR4729 Cleanup errors for sec_acl_bind{_to_addr}
 * 	[1992/11/14  00:43:00  burati]
 * 
 * Revision 1.1.7.3  1992/10/09  15:15:01  burati
 * 	CR4981 Make sure we only pass back maloc'd pointers that will be freed
 * 	[1992/10/09  14:38:27  burati]
 * 
 * Revision 1.1.7.2  1992/09/29  21:14:18  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:18:01  sekhar]
 * 
 * Revision 1.1.2.4  1992/04/28  16:02:09  burati
 * 	CR755 Needed to get new import context before setting exp age to 0 on rebind
 * 	[1992/04/28  02:04:37  burati]
 * 
 * Revision 1.1.2.3  1992/04/07  14:12:13  burati
 * 	CR755 Set exp_age to 0 to avoid cache if rebind is necessary.
 * 	[1992/04/07  02:09:15  burati]
 * 
 * 	CR767 Don't release import handle cuz it's needed to rebind.
 * 	CR764 Only pass back bad check_binding sts if caller's status was ok.
 * 	[1992/04/07  01:51:35  burati]
 * 
 * 	CR761 sec_login_no_current_context -> sec_acl_unable_to_authenticate
 * 	CR762 rpc_s_incomplete_name -> sec_acl_object_not_found
 * 	CR762 rpc_s_no_privilege -> sec_acl_not_authorized
 * 	[1992/04/06  21:43:51  burati]
 * 
 * Revision 1.1.2.2  1992/03/18  21:59:18  burati
 * 	CR744 Do not modify input argument 'entry_name'.
 * 	[1992/03/18  20:10:31  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  daclbind.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
** sec_acl - binding context management
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)daclbind.c	27 - 02/24/92";
#endif

#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dce/daclif.h>
#include <rdaclif.h>
#include <rdaclifv0.h>
#include <dce/dfs_dce_acl.h>
#include <dce/dce_utils.h>

#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dacl_pvt.h>
#include <macros.h>
#include <un_maloc.h>

#ifdef IDL_PROTOTYPES
#define PROTOTYPE(x) x
#else
#define PROTOTYPE(x) ()
#endif

#define FREE_UNRES_NAME(p) \
    if (free_flag) \
	free((*p)); \
    else { \
        unsigned32 st; \
        rpc_string_free( (p), &st ); \
    }

/* Macro to cleanup runtime allocated strings and return, on error */
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
#define CLEANUP_ON_ERROR(stp)   if (BAD_STATUS(stp)) { \
    if (STATUS_EQUAL(st_p, sec_login_s_no_current_context)) { \
        SET_STATUS(st_p, sec_acl_unable_to_authenticate); \
    }  else { \
        if (resolved_name != NULL) \
            rpc_string_free(&resolved_name, &st); \
        if (unresolved_name != NULL) \
	    FREE_UNRES_NAME(&unresolved_name); \
        if (tmp_entry_name != NULL) \
            free(tmp_entry_name); \
        if (residual != emptystr) \
	    free((char *)residual); \
        if (import_context != NULL) \
            rpc_ns_binding_import_done(&import_context, &st); \
        if (STATUS_EQUAL(st_p, rpc_s_no_ns_privilege)) { \
            SET_STATUS(st_p, sec_acl_not_authorized); \
        } \
        if (STATUS_EQUAL(st_p, rpc_s_incomplete_name)) { \
            SET_STATUS(st_p, sec_acl_object_not_found); \
        } \
        if (STATUS_EQUAL(st_p, rpc_s_no_more_bindings)) { \
            SET_STATUS(st_p, sec_acl_object_not_found); \
        } \
        return; \
    } \
}
#else
#define CLEANUP_ON_ERROR(stp)   if (BAD_STATUS(stp)) { \
    if (STATUS_EQUAL(st_p, sec_login_s_no_current_context)) { \
        SET_STATUS(st_p, sec_acl_unable_to_authenticate); \
    }  else { \
        if (resolved_name != NULL) \
            rpc_string_free(&resolved_name, &st); \
        if (STATUS_EQUAL(st_p, rpc_s_no_ns_privilege)) { \
            SET_STATUS(st_p, sec_acl_not_authorized); \
        } \
        if (STATUS_EQUAL(st_p, rpc_s_incomplete_name)) { \
            SET_STATUS(st_p, sec_acl_object_not_found); \
        } \
        if (STATUS_EQUAL(st_p, rpc_s_no_more_bindings)) { \
            SET_STATUS(st_p, sec_acl_object_not_found); \
        } \
        return; \
    } \
}
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

/* For debug tracing...  Will be replaced via 1.1 serviceability work */
int acl_binding_trace = 0;
boolean32 acl_trace_checked = false;

/*
 * Global (per process) variables 
 */
static boolean32 sec__acl_bind_nsi_only = false;

PRIVATE void acl_check_env_vars( void )
{
    char *trace;
    if (acl_trace_checked)
        return;
    acl_trace_checked = true;
    trace = getenv("DEBUG_ACL");
    if ((trace != NULL) && (trace[0] != '0')) {
        if (isdigit(trace[0]))
            acl_binding_trace = atoi(trace);
    }
}


/* Validate the string that we presume to be a leaf */
PRIVATE void chk_leaf (
    char            *lname,
    error_status_t  *st_p
)
{
    char *tname;
    CLEAR_STATUS(st_p);

    /* check for non-ascii char, or embedded '/' */
    tname = lname;
    while (*tname != '\0')
        if ((!isdcepcs(*tname++)) || (*tname == '/')) {
            SET_STATUS(st_p, sec_acl_invalid_entry_name);
            return;
        }
}

/* Drop off the leaf of a pathname */
PRIVATE void drop_leaf (
    unsigned_char_p_t   entry_name,
    unsigned_char_p_t   *residual,
    error_status_t      *st_p
)
{
    char *tpath;
    int  index;

    CLEAR_STATUS(st_p);

    if ((entry_name == NULL) || (strlen((char *)entry_name) <= (size_t) 1)) {
        SET_STATUS(st_p, sec_acl_invalid_entry_name);
        return;
    }

    /* Strip off trailing '/'s if any */
    tpath = (char *) &(entry_name[strlen((char *) entry_name) -1]);
    while ((tpath != ((char *) entry_name)) && (*tpath == '/'))
        *tpath--='\0';

    for (index = (strlen((char *) entry_name)-1); index >= 0; index--)
        if (entry_name[index] == '/') {
            chk_leaf((char *) &(entry_name[index+1]), st_p);
            if (BAD_STATUS(st_p)) return;

            /* Make room for, and copy the residual */
            *residual = malloc(strlen((char *) &(entry_name[index+1])) + 1);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	    if (*residual == NULL) {
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		return;
	    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            strcpy((char *) *residual, (char *) &(entry_name[index+1]));

            /* If index is 0, then non-leaf component was just '/' */
            if (index > 0) entry_name[index] = '\0';
            else entry_name[index+1] = '\0';
            return;
        }

    /* If we got this far, that means that entry_name was a leaf
     * so there must be an error in the name somewhere
     */
    SET_STATUS(st_p, sec_acl_invalid_entry_name);
    return;

}

PRIVATE void get_server_principal (
    rpc_binding_handle_t handle,
    unsigned_char_t      **server_principal,
    error_status_t       *st
)
{
    rpc_mgmt_inq_server_princ_name(handle, rpc_c_authn_dce_secret,
                                    server_principal, st);
    SEC_DBG_PRINTF(2,
        ("get_server_principal: inq_server_princ_name status is %x\n", *st));
    if (*st == rpc_s_binding_incomplete) {
        rpc_ep_resolve_binding(handle, rdaclif_v0_0_c_ifspec, st);
        rpc_mgmt_inq_server_princ_name(handle, rpc_c_authn_dce_secret,
                                        server_principal, st);
        SEC_DBG_PRINTF(2,
            ("get_server_principal: 2nd inq_server_princ_name status is %x\n",
                *st));
    }
}

PRIVATE error_status_t  setup_auth (
    rpc_binding_handle_t     handle,
    sec_acl_bind_auth_info_t *auth_info
)
{
    error_status_t              st = error_status_ok;
    error_status_t              lst;
    rpc_auth_identity_handle_t  auth_ident;
    unsigned_char_t             *server_principal;

    if (   ( auth_info->info_type != sec_acl_bind_auth_none   ) 
        && ( auth_info->tagged_union.dce_info.identity == NULL)) {
        /* Setup the binding handle auth info, using the security login
         * context associated with the current address space.
         */
        sec_login_get_current_context(
            &auth_info->tagged_union.dce_info.identity, &st);
        SEC_DBG_PRINTF(1,
            ("setup_auth: sec_login_get_current_context status is %x\n", st));
    }
    if ( auth_info->info_type == sec_acl_bind_auth_none || BAD_STATUS(&st)) {  
        /*
         *  Either there is no login context or the binding is unauthenticated.
         *  In either case the RPC binding may only be partially bound. So
         *  resolve the binding before returning from this function.
         *
         */
        rpc_ep_resolve_binding(handle, rdaclif_v0_0_c_ifspec, &st);
        return st ;
    }
         
    if (GOOD_STATUS(&st)) {
        get_server_principal(handle, &server_principal, &st);
        if (GOOD_STATUS(&st)) {
            rpc_binding_set_auth_info(handle,
                                    server_principal,
                                    auth_info->tagged_union.dce_info.authn_lvl,
                                    auth_info->tagged_union.dce_info.authn_svc,
                                    auth_info->tagged_union.dce_info.identity,
                                    auth_info->tagged_union.dce_info.authz_svc,
                                    &st);
            SEC_DBG_PRINTF(1,
                ("setup_auth: rpc_binding_set_auth_info status is %x\n", st));
            rpc_string_free(&server_principal, &lst);
        }
    }
    return st;
}


/* sec_acl_bind_nsi_only
 * 
 * Tell the ACL binding code that this process wants to use NSI
 * binding only for ACLs (don't try to bind to an ACL via DFS).
 */
PUBLIC void sec_acl_bind_nsi_only (
    boolean32           nsi_only
)
{
    sec__acl_bind_nsi_only = nsi_only;
}


/* s e c _ a c l _ b i n d
 *
 * Establish a connection.  The entry_name is a resolvable pathname, to which
 * we should bind.  The bind_to_entry predicate is used, when the entry_name
 * denotes both an entry in the namespace and an object outside the namespace
 * (ie: the name of a filesystem has an acl in the nameserver and has it's own
 * acl at the filesystem itself).  If bind_to_entry is true in this case, then
 * we will bind to the entry in the namespace as opposed to the object itself.
 *
 * The sec_acl_handle_t is an opaque (to the client) handle in which the rpc
 * binding handle and the residual component name are stored.
 *
 * Error Status:
 */
PUBLIC void sec_acl_bind (
    unsigned_char_p_t   entry_name,
    boolean32           bind_to_entry,
    sec_acl_handle_t    *sech_p,
    error_status_t      *st_p
)
{
    sec_acl_bind_auth_info_t auth_info;
    auth_info.info_type = sec_acl_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_lvl = rpc_c_authn_level_default;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity = NULL;
    sec_acl_bind_auth(entry_name, bind_to_entry, &auth_info, sech_p, st_p);
}


/* s e c _ a c l _ b i n d _ a u t h
 *
 * Establish a connection.  The entry_name is a resolvable pathname, to which
 * we should bind.  The bind_to_entry predicate is used, when the entry_name
 * denotes both an entry in the namespace and an object outside the namespace
 * (ie: the name of a filesystem has an acl in the nameserver and has it's own
 * acl at the filesystem itself).  If bind_to_entry is true in this case, then
 * we will bind to the entry in the namespace as opposed to the object itself.
 *
 * The sec_acl_handle_t is an opaque (to the client) handle in which the rpc
 * binding handle and the residual component name are stored.
 *
 * Error Status:
 */

PUBLIC void sec_acl_bind_auth (
    unsigned_char_p_t           entry_name,
    boolean32                   bind_to_entry,
    sec_acl_bind_auth_info_t    *auth_info_p,
    sec_acl_handle_t            *sech_p,
    error_status_t              *st_p
)
{
    struct stat                 stat_buf;
    sec_acl_pvt_handle_blk_t    *new_context;
    rpc_binding_handle_t        handle;
    rpc_ns_inq_handle_t         inquiry_context;
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    rpc_ns_import_handle_t      import_context = NULL;
#else
    rpc_ns_import_handle_t      import_context;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    uuid_t                      obj_uuid;
    unsigned_char_p_t           tmp_entry_name;
    unsigned_char_p_t           resolved_name = NULL, unresolved_name = NULL;
    sec_acl_component_name_t    component_name = (unsigned_char_p_t) "";
    sec_acl_component_name_t    residual, emptystr = (unsigned_char_p_t) "";
    sec_acl_t                   sec_acl;
    error_status_t              st;
    boolean32                   authn = false, prevauthn;
    sec_acl_component_name_t	c_test = component_name, r_test = residual;

    /* if true, use free(), else use rpc_string_free() */
    boolean32 free_flag = false;

    CLEAR_STATUS(st_p);
    residual = emptystr;
    *sech_p = NULL;

    if ((entry_name == NULL) || (strlen((char *)entry_name) == 0)) {
        SET_STATUS(st_p, sec_acl_invalid_entry_name);
        return;
    }
    if (! acl_trace_checked)
        acl_check_env_vars();

    /* Make copy of entry_name for pathname manipulation */
    tmp_entry_name = (unsigned_char_p_t) malloc(
                        (strlen((char *)entry_name) + 1) * sizeof(char) );
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (tmp_entry_name == NULL) {
	SET_STATUS(st_p, sec_acl_cant_allocate_memory);
	return;
    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    strcpy((char *)tmp_entry_name, (char *)entry_name);

    /*
     * See if the specified object is a DFS object before attempting
     * to manipulate it via the remote DCE ACL interface.  DFS ACLs don't
     * have a concept of bind_to_entry, so don't bother looking for the
     * ACL in DFS if bind_to_entry is set to true.
     */
    if (!sec__acl_bind_nsi_only && !bind_to_entry) {
        st = dfs_dce_acl_lookup((char *)entry_name, sec_acl_type_object, NULL);
	if (GOOD_STATUS(&st)) {
	    SEC_DBG_PRINTF(1,("sec_acl_bind: %s is a file system object\n",
		entry_name));
	    /*
	     * Create a new handle and mark it for use by DFS only
	     */
	    new_context = NEW(sec_acl_pvt_handle_blk_t);
	    if (new_context != NULL) {
		new_context->is_dfs_acl         = true;
		new_context->rpc_handle_valid   = false;
		new_context->import_handle_valid= false;
		new_context->rebound            = false;
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
		new_context->lepv.epv		= NULL;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
		new_context->resolved_name      = (unsigned_char_p_t) NULL;
		new_context->component_name     = (unsigned_char_p_t) malloc(
		    (strlen((char *)entry_name)+1) * sizeof(char) );
		if (new_context->component_name) {
		    strcpy((char *)new_context->component_name,(char *)entry_name);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
		    CLEAR_STATUS(&(new_context->status));
		    *sech_p = (sec_acl_handle_t) new_context;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
		} else {
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
		    free(new_context);
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
		    SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		}
#ifdef	HPDCE_ACLMGR_LOCAL_BIND
		CLEAR_STATUS(&(new_context->status));
		*sech_p = (sec_acl_handle_t) new_context;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
	    } else {
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
	    }
	    /* Is DFS object, no need to do any remote rpc operations */
	    if (tmp_entry_name)
		free((char *)tmp_entry_name);
	    return;
	}
    } else if (bind_to_entry) {
        /* Caller has told us he is interested in the name space node
         * rather than the actual object, so strip the leaf name off
         * and accumulate it in the residual string.
         */
        drop_leaf(tmp_entry_name, &residual, st_p);
        CLEANUP_ON_ERROR(st_p);
    }

    /* Resolve as much of the entry name as possible */
    SEC_DBG_PRINTF(1,("sec_acl_bind: trying to resolve '%s'\n",
        tmp_entry_name));
    rpc_ns_entry_inq_resolution(rpc_c_ns_syntax_dce, tmp_entry_name,
                                &resolved_name, &unresolved_name, &st);
    SEC_DBG_PRINTF(1,("sec_acl_bind: resolution is %x\n", st));
    /* If fully resolved, then only check for bind_to_entry residual */
    if (STATUS_EQUAL(&st, rpc_s_ok)) {
        if (bind_to_entry)
            component_name = residual;

    } else if (STATUS_EQUAL(&st, rpc_s_partial_results)) {
        /* if leading slash on unresolved_name, remove it */
        if (unresolved_name)
            if (unresolved_name[0] == '/') {
                unsigned_char_p_t tmp_unresolved_name;
		/* Must relocate memory so that we can validly free
		 * unresolved_name pointer if we do component_name =
		 * unresolved_name later on.
		 */
		tmp_unresolved_name = (unsigned_char_p_t)malloc(strlen(
		    (char *)unresolved_name));
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
		if (tmp_unresolved_name == NULL) {
		    rpc_string_free(&unresolved_name, &st);
		    unresolved_name = NULL;
		    SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		    CLEANUP_ON_ERROR(st_p);
		}
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
		strcpy((char *)tmp_unresolved_name, (char *)(unresolved_name +
		    1));
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
		rpc_string_free(&unresolved_name, &st);
#else
		free(unresolved_name);
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
                unresolved_name = tmp_unresolved_name;
		free_flag = true;
	    }
        /* component_name = unresolved_name + optional residual */
        if (bind_to_entry) {
            component_name = (unsigned_char_p_t) malloc(
                                            strlen((char *) residual) + 2 +
                                            strlen((char *) unresolved_name));
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	    if (component_name == NULL) {
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		CLEANUP_ON_ERROR(st_p);
	    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            strcpy((char *) component_name, (char *) unresolved_name);
            strcat((char *) component_name, "/");
            strcat((char *) component_name, (char *) residual);
        }
        else {
            component_name = unresolved_name;
        }
    } else {
        /* Not resolved or partially resolved -> can't continue */
        SET_STATUS(st_p, st);               /* Save the local status */
        if (tmp_entry_name)
            free((char *)tmp_entry_name);
        if (residual != emptystr)
	    free((char *)residual);
        /* Pass back an acl status if possible */
        if (STATUS_EQUAL(st_p, sec_login_s_no_current_context)) {
            SET_STATUS(st_p, sec_acl_unable_to_authenticate);
        }
        if (STATUS_EQUAL(st_p, rpc_s_no_ns_privilege)) {
            SET_STATUS(st_p, sec_acl_not_authorized);
        }
        if (STATUS_EQUAL(st_p, rpc_s_incomplete_name)) {
            SET_STATUS(st_p, sec_acl_object_not_found);
        }
        if (STATUS_EQUAL(st_p, rpc_s_no_more_bindings)) {
            SET_STATUS(st_p, sec_acl_object_not_found);
        }
        return;
    }
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    /* If we had a residual string and it's not the component name, free it */
    if ((residual != emptystr) && (residual != component_name)) {
	free((char *)residual);
	residual = emptystr;
    }
    /* If we had an unresolved name and it's not the component name, free it */
    if ((unresolved_name != NULL) && (unresolved_name != component_name)) {
	FREE_UNRES_NAME(&unresolved_name);
	unresolved_name = NULL;
    }
#else
    /* If we had a residual string and it's not the component name, free it */
    if ((residual != emptystr) && (residual != component_name))
	free((char *)residual);
    /* If we had an unresolved name and it's not the component name, free it */
    if ((unresolved_name != NULL) && (unresolved_name != component_name))
	free((char *)unresolved_name);
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

    SEC_DBG_PRINTF(1,("sec_acl_bind: resolved name is '%s'\n",resolved_name));
    /* Get the uuid for the object
     * We assume that these calls will yield a status code
     * telling us if we could read the entry...
     */
    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce, resolved_name,
                                    &inquiry_context, st_p);
    SEC_DBG_PRINTF(2,("sec_acl_bind: entry_object_inq_begin status is '%x'\n",
        *st_p));
    CLEANUP_ON_ERROR(st_p);
    rpc_ns_entry_object_inq_next(inquiry_context, &obj_uuid, st_p);
    SEC_DBG_PRINTF(2,("sec_acl_bind: entry_object_inq_next status is '%x'\n",
        *st_p));
    SEC_OBJ_PRINTF(obj_uuid, "sec_acl_bind:");
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    rpc_ns_entry_object_inq_done(&inquiry_context, &st);
    if (STATUS_EQUAL(st_p, rpc_s_no_more_members)) {
        uuid_create_nil(&obj_uuid, st_p);
    } else {
        CLEANUP_ON_ERROR(st_p);
    }
    if (BAD_STATUS(&st)) {
        SET_STATUS(st_p, st);
        CLEANUP_ON_ERROR(st_p);
    }
#else
    if (STATUS_EQUAL(st_p, rpc_s_no_more_members)) {
        uuid_create_nil(&obj_uuid, st_p);
    } else {
        CLEANUP_ON_ERROR(st_p);
    }
    rpc_ns_entry_object_inq_done(&inquiry_context, &st);
    CLEANUP_ON_ERROR(st_p);
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

    /*
     * Now we have the uuid and name of an entry in the name service.
     */

    /* Get a binding to that entry */
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce, resolved_name,
                                (rpc_if_handle_t) NULL, &obj_uuid,
                                &import_context, st_p);
    SEC_DBG_PRINTF(1,("sec_acl_bind: import of '%s' yields status %x\n",
        resolved_name, *st_p));
    CLEANUP_ON_ERROR(st_p);
    rpc_ns_binding_import_next(import_context, &handle, st_p);
    SEC_DBG_PRINTF(1,("sec_acl_bind: import_next yields status %x\n",*st_p));
    CLEANUP_ON_ERROR(st_p);
    SEC_HANDLE_PRINTF(handle, "sec_acl_bind:");

    /*
     * Success.
     *
     * Create a new handle and fill it with the rpc handle and component name
     */
    new_context = NEW(sec_acl_pvt_handle_blk_t);
    if (new_context != NULL) {
        new_context->rpc_handle         = handle;
        new_context->rpc_handle_valid   = true;
        new_context->component_name     = component_name;
        new_context->resolved_name      = resolved_name;
        new_context->import_handle      = import_context;
        new_context->import_handle_valid= true;
        new_context->is_dfs_acl         = false;
        new_context->rebound            = false;
        new_context->obj_uuid           = obj_uuid;
        new_context->if_version         = 1;
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	new_context->lepv.epv		= NULL;
	rpc_binding_to_string_binding(handle,
				      &new_context->string_binding, st_p);
	if (BAD_STATUS(st_p)) {
	    rpc_binding_free(&handle, &st);
	    free(new_context);
	    CLEANUP_ON_ERROR(st_p);
	}
#else
	rpc_binding_to_string_binding(handle,&new_context->string_binding,&st);
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
        SEC_DBG_PRINTF(1, ("sec_acl_bind: obtained handle %s\n",
            new_context->string_binding));
        CLEAR_STATUS(&(new_context->status));

        /* Set up the binding handle auth info */
        *st_p = setup_auth(handle, auth_info_p);
        if ( STATUS_EQUAL(st_p, sec_login_s_no_current_context) ||
             STATUS_EQUAL(st_p, rpc_s_auth_tkt_expired) ) {

            SET_STATUS(st_p, sec_acl_unable_to_authenticate);
            SET_STATUS(&new_context->status, *st_p);/* save nonfatal error */
            new_context->auth_info.info_type = sec_acl_bind_auth_none;

        } else if (BAD_STATUS(st_p)) {
            int retry = 4;
	    
            /* recover from bad binding handle/bypass the cache */
            new_context->rpc_handle_valid = false;
            new_context->auth_info.info_type = auth_info_p->info_type;
            if (auth_info_p->info_type = sec_acl_bind_auth_dce) {
                new_context->auth_info.tagged_union.dce_info =
                    auth_info_p->tagged_union.dce_info;
            }

            /* Try to rebind to another server, if possible */
            while ((retry-- > 0) && (new_context->import_handle_valid)) {
                (void)sec__acl_check_binding((sec_acl_handle_t)new_context,
                    st_p);
            }

            /* If we don't have a good status at this point, cleanup */
            if (BAD_STATUS(st_p)) {
                free((char *)new_context);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
		new_context = NULL;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            }
        } else {
            new_context->auth_info.info_type = auth_info_p->info_type;
            if (auth_info_p->info_type = sec_acl_bind_auth_dce) {
                new_context->auth_info.tagged_union.dce_info =
                    auth_info_p->tagged_union.dce_info;
            }
        }
        *sech_p = (sec_acl_handle_t) new_context;
    } else {
        rpc_ns_binding_import_done(&import_context, st_p);
        SET_STATUS(st_p, sec_acl_cant_allocate_memory);
    }

    /* Free locally allocated memory */
    if (tmp_entry_name != NULL) {
        free((char *)tmp_entry_name);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	tmp_entry_name = NULL;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    }

    if ((GOOD_STATUS(st_p)) ||
        (STATUS_EQUAL(st_p, sec_acl_unable_to_authenticate)) ) {
        /* make sure we don't leave pointers to memory allocated on the stack */
        if (new_context->component_name == c_test
                || new_context->component_name == r_test)  {
            new_context->component_name = malloc(1);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
            if (new_context->component_name == NULL) {
		rpc_binding_free(&handle, &st);
		rpc_ns_binding_import_done(&import_context, st_p);
		free(new_context);
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		CLEANUP_ON_ERROR(st_p);
	    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            new_context->component_name[0] = '\0';
        }

        if (new_context->resolved_name == r_test
                || new_context->resolved_name == c_test)  {
            new_context->resolved_name = malloc(1);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
            if (new_context->resolved_name == NULL) {
		rpc_binding_free(&handle, &st);
		rpc_ns_binding_import_done(&import_context, st_p);
		free(new_context);
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		CLEANUP_ON_ERROR(st_p);
	    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            new_context->resolved_name[0] = '\0';
        }
    }

    if (STATUS_EQUAL(st_p, sec_login_s_no_current_context)) {
        SET_STATUS(st_p, sec_acl_unable_to_authenticate);
    }
    if (STATUS_EQUAL(st_p, rpc_s_no_ns_privilege)) {
        SET_STATUS(st_p, sec_acl_not_authorized);
    }
    if (STATUS_EQUAL(st_p, rpc_s_incomplete_name) ||
        STATUS_EQUAL(st_p, rpc_s_no_more_bindings)) {
        SET_STATUS(st_p, sec_acl_object_not_found);
    }
}

/* s e c _ a c l _ b i n d _ t o _ a d d r
 *
 * Establish a connection.  The site_name identifies the sec_acl mgr to use.
 *
 * NOTES: Eventually this function will use the NSI layer and resolve a
 * name from the name system.  (Will we preserve the ability to name a machine
 * or will names be exclusively from the name system?)
 *
 * Error Status:
 * Unable to connect to requested site.
 */

PUBLIC void sec_acl_bind_to_addr (
    unsigned_char_p_t           site_addr,
    sec_acl_component_name_t    component_name,
    sec_acl_handle_t            *sech_p,
    error_status_t              *st_p
)
{
    sec_acl_pvt_handle_blk_t    *new_context;
    rpc_binding_handle_t        handle;
    unsigned_char_p_t           str_obj             = NULL;
    unsigned_char_p_t           str_protseq         = NULL;
    unsigned_char_p_t           str_netaddr         = NULL;
    unsigned_char_p_t           str_endpoint        = NULL;
    unsigned_char_p_t           str_options         = NULL;
    unsigned_char_p_t           str_protseq_real    /* = NULL */;
    unsigned_char_p_t           str_endpoint_real   /* = NULL */;
    unsigned_char_p_t           str_binding         = NULL;
    unsigned_char_p_t           str_binding_real    /* = site_addr */;
    error_status_t              st;
    sec_acl_bind_auth_info_t    auth_info;
    auth_info.info_type = sec_acl_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_lvl = rpc_c_authn_level_default;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity = NULL;

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    *sech_p = NULL;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

    if (*site_addr == '\0') {
        SET_STATUS(st_p, sec_acl_invalid_site_name);
        return;
    }

    /* Create an empty acl handle context */
    new_context = NEW(sec_acl_pvt_handle_blk_t);
    if (new_context != NULL) {
        if (component_name != (unsigned_char_p_t) NULL) {
            new_context->component_name     =
                (unsigned_char_p_t) malloc(strlen((char *)component_name)+1);
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
            if (new_context->component_name == NULL) {
		free(new_context);
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		return;
	    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            strcpy((char *)new_context->component_name, (char *)component_name);
        } else {
            new_context->component_name     = (unsigned_char_p_t) NULL;
        }
        new_context->resolved_name          = (unsigned_char_p_t) NULL;
	new_context->string_binding         = (unsigned_char_p_t) NULL;
        new_context->import_handle_valid    = false;
        new_context->rebound                = false;
        new_context->is_dfs_acl             = false;
        new_context->status                 = error_status_ok;
        new_context->if_version             = 1;
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	new_context->lepv.epv		    = NULL;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
        SET_STATUS(&new_context->status, error_status_ok);
        *sech_p = (sec_acl_handle_t) new_context;
    } else {
        SET_STATUS(st_p, sec_acl_cant_allocate_memory);
        return;
    }

    CLEAR_STATUS(st_p);
    rpc_string_binding_parse(site_addr, &str_obj, &str_protseq, &str_netaddr,
                                &str_endpoint, &str_options, &st);
    if (GOOD_STATUS(&st)) {

        /*
         * If a simple name was used (unadorned with the proper protocol
         * sequence) we try a default sequence.
         */
        if (NULL_STRING(str_protseq)) {
            str_protseq_real = (unsigned_char_p_t) "ip";
        } else {
            str_protseq_real = str_protseq;
        }

        if (NULL_STRING(str_endpoint)) {
            str_endpoint_real = (unsigned_char_p_t) "";
        } else {
            str_endpoint_real = str_endpoint;
        }

        rpc_string_binding_compose(str_obj, str_protseq_real, str_netaddr,
                                    str_endpoint_real, str_options,
                                    &str_binding, &st);
        if (GOOD_STATUS(&st)) {
            str_binding_real = str_binding;
            rpc_binding_from_string_binding(str_binding_real, &handle, &st);
        }
    }

    if (GOOD_STATUS(&st)) {
        /*
         * Success.
         *
         * Create a new handle and fill it with the rpc handle
         */
        if (new_context != NULL) {
            new_context->rpc_handle             = handle;
            new_context->rpc_handle_valid       = true;
            new_context->string_binding         = str_binding;
        } else {
            SET_STATUS(st_p, sec_acl_cant_allocate_memory);
        }

        /* Set up the binding handle auth info */
        st = setup_auth(handle, &auth_info);
        if (! GOOD_STATUS(&st)) {
            SET_STATUS(st_p, sec_acl_unable_to_authenticate);
            SET_STATUS(&new_context->status, st);   /* save nonfatal error */
            new_context->auth_info.info_type = sec_acl_bind_auth_none;
        } else {
            new_context->auth_info.info_type = auth_info.info_type;
            if (auth_info.info_type == sec_acl_bind_auth_dce) {
                new_context->auth_info.tagged_union.dce_info =
                    auth_info.tagged_union.dce_info;
            }
        }

        /* Save the new context block into an opaque acl handle */
        *sech_p = (sec_acl_handle_t) new_context;
    } else {
        SET_STATUS(st_p, sec_acl_bind_error);       /* Return binding error */
        free((char *)new_context);
    }


    /*
     * Free the strings that may have been allocated by the RPC runtime
     */
    if (str_obj != NULL) {
        rpc_string_free(&str_obj, &st);
    }
    if (str_protseq != NULL) {
        rpc_string_free(&str_protseq, &st);
    }
    if (str_netaddr != NULL) {
        rpc_string_free(&str_netaddr, &st);
    }
    if (str_endpoint != NULL) {
        rpc_string_free(&str_endpoint, &st);
    }
    if (str_options != NULL) {
        rpc_string_free(&str_options, &st);
    }
    if (BAD_STATUS(st_p) && str_binding != NULL) {
        rpc_string_free(&str_binding, &st);
    }
}

static void sec_acl_tower_set_free (
    sec_acl_tower_set_t *tower_set,
    void (*free_func) PROTOTYPE((void *ptr))
)
{
    sec_acl_tower_set_t t=*tower_set;
    *tower_set = NULL;
    if (t) {
        int i;
        for (i=0; i<t->count; i++) {
            if (t->towers[i]) {
                (*free_func)(t->towers[i]);
            }
        }
        (*free_func)(t);
    }
}


/* s e c _ _ a c l _ r e b i n d _ u p d a t e
 *
 * Client agent got a read only error, and called us to rebind to an update
 * site.  Make a get_referral call and attempt to bind to that site.
 *
 * Error Status:
 */

PUBLIC void sec__acl_rebind_update(context, mgr_type, acl_type, stp)
sec_acl_handle_t    context;
uuid_t              *mgr_type;
sec_acl_type_t      acl_type;
error_status_t      *stp;
{
    rpc_binding_handle_t    binding;
    sec_acl_tower_set_t     tower_set;
    boolean32               rebound = false;
    int                     i;
    error_status_t          rpc_status;
    uuid_t                  binding_uuid;
    void (*free_func) PROTOTYPE((void *ptr));
    void *(*alloc_func) PROTOTYPE((unsigned long size));

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    /* Can't rebind to a local object */
    if (IS_LOCAL_ACL(context)) {
        SET_STATUS(stp, sec_acl_invalid_acl_handle);
        return;
    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

    (*rdaclif_v0_0_c_epv.rdacl_get_referral)
        (HANDLE(context), COMPONENT_NAME(context), mgr_type, acl_type,
                                                &tower_set, stp, &rpc_status);
    SEC_DBG_PRINTF(1,
        ("sec__acl_rebind_update: rdacl_get_referral returned status %x\n",
            *stp));
    if (BAD_STATUS(&rpc_status))
        SET_STATUS(stp, rpc_status);

    if (GOOD_STATUS(stp)) {
        if (tower_set->count < 1) {
            SET_STATUS(stp, sec_acl_no_update_sites);
        } else {
            /* Get current object uuid and set in new handle */
            rpc_binding_inq_object(HANDLE(context), &binding_uuid, stp);
            SEC_OBJ_PRINTF(binding_uuid, "sec__acl_rebind_update:");
            for (i = 0; (i < tower_set->count) && (!rebound); i++) {
                rpc_tower_to_binding(tower_set->towers[i]->tower_octet_string,
                                     &binding, stp);
                if (GOOD_STATUS(stp)) {
                    rpc_binding_set_object(binding, &binding_uuid, stp);
                }
                /* Set up authentication information if necessary */
                if (AUTH_INFO(context).info_type != sec_acl_bind_auth_none) {
                    *stp = setup_auth(binding, &AUTH_INFO(context));
		}
                if (GOOD_STATUS(stp)) {
                    /* Free previous binding */
                    rpc_binding_free(&(HANDLE(context)), &rpc_status);
                    if (IMPORT_HANDLE_VALID(context)) {
                        rpc_ns_binding_import_done(&(IMPORT_HANDLE(context)),
                                                    &rpc_status);
                        IMPORT_HANDLE_VALID(context) = false;
                    }
                    if (HANDLE_STR(context) != (unsigned_char_p_t)NULL) {
                        free((char *) HANDLE_STR(context));
                        HANDLE_STR(context) = (unsigned_char_p_t)NULL;
                    }
                    rebound = true;
                    HANDLE(context) = binding;
                    HANDLE_VALID(context) = true;
                    rpc_binding_to_string_binding(binding, 
                        &(HANDLE_STR(context)), &rpc_status);
                    if (GOOD_STATUS(&rpc_status)) {
                        SEC_DBG_PRINTF(1,
                            ("sec__acl_rebind_update: new handle is %s\n",
                                HANDLE_STR(context)));
                    }
                } else {
                    LAST_ERROR(context) = *stp;
                }
            }
            if (!rebound) {
                SET_STATUS(stp, sec_acl_no_update_sites);
            }
        }
    }
    sec_acl_tower_set_free(&tower_set, rpc_ss_client_free);
}


/*
 * equiv_acl_bdgs - Compare the obj ids, addrs and ports of the 2 bindings
 */
PRIVATE boolean32 equiv_acl_bdgs (
    unsigned_char_p_t hstr1,
    unsigned_char_p_t hstr2
)
{
    unsigned_char_p_t ostr1, ostr2, astr1, astr2, epstr1, epstr2;
    error_status_t    st;
    boolean32         match = true;
    ostr1 = ostr2 = astr1 = astr2 = epstr1 = epstr2 = (unsigned_char_p_t) NULL;

    /* Break the string bindings up into the pieces that we need to compare */
    rpc_string_binding_parse(hstr1,&ostr1,NULL,&astr1,&epstr1,NULL,&st);
    if (BAD_STATUS(&st)) {
        match = false;
    } else {
        rpc_string_binding_parse(hstr2,&ostr2,NULL,&astr2,&epstr2,NULL,&st);
        if (BAD_STATUS(&st)) {
            match =  false;
	}
    }
    /* Compare the pertinent pieces of the bindings */
    if (GOOD_STATUS(&st)) {
        if (strcmp((char *)astr1, (char *)astr2) != 0) {
            match = false;
	} else if (strcmp((char *)epstr1, (char *)epstr2) != 0) {
            match = false;
	} else if (strcmp((char *)ostr1, (char *)ostr2) != 0) {
            match = false;
	}
    }
        /* Cleanup */
    if (ostr1 != (unsigned_char_p_t) NULL)
        rpc_string_free(&ostr1, &st);
    if (ostr2 != (unsigned_char_p_t) NULL)
        rpc_string_free(&ostr2, &st);
    if (astr1 != (unsigned_char_p_t) NULL)
        rpc_string_free(&astr1, &st);
    if (astr2 != (unsigned_char_p_t) NULL)
        rpc_string_free(&astr2, &st);
    if (epstr1 != (unsigned_char_p_t) NULL)
        rpc_string_free(&epstr1, &st);
    if (epstr2 != (unsigned_char_p_t) NULL)
        rpc_string_free(&epstr2, &st);
    return match;
}


/*  sec__acl_check_status is only called by sec__acl_check_binding() . 
 *  If this function returns true, then sec__acl_check_binding will 
 *  revert back to version 1.0 of rdaclif interface.
 */

boolean32 sec__acl_check_status (
    error_status_t st
)
{
    switch(st) {
        case rpc_s_unknown_if:
        case ept_s_not_registered:
            return true;

        default: 
            return false ;
    }
}

/*
 * sec__acl_check_binding - Checks binding and rebinds if necessary/possible
 */
boolean32 sec__acl_check_binding (
    sec_acl_handle_t    context,
    error_status_t      *st_p
)
{
    rpc_binding_handle_t handle;
    unsigned_char_p_t    hstr = (unsigned_char_p_t) NULL;
    error_status_t       lst, lst2;

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    /* Can't rebind to a local object */
    if (IS_LOCAL_ACL(context)) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return false;
    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    /* CAN'T REBIND TO A DFS OBJECT */
    if (IS_DFS_ACL(context)) {
        return false;
    }

    CLEAR_STATUS(&lst);
    if (! HANDLE_VALID(context)) {
        SEC_DBG_PRINTF(1,("sec__acl_check_binding: !HANDLE_VALID\n"));
        if ( sec__acl_check_status(*st_p) && IF_VERSION(context) == 1) {
            SEC_DBG_PRINTF(1,
                ("sec__acl_check_binding: Reverting to version 0.0 interface\n"));
            IF_VERSION(context) = 0;
            HANDLE_VALID(context) = true;
            CLEAR_STATUS(st_p);
        } else if (IMPORT_HANDLE_VALID(context)) {
          /* Attempt rebind via NSI */
            SEC_DBG_PRINTF(1,("sec__acl_check_binding: IMPORT_HANDLE_VALID\n"));
            if (! (REBOUND_YET(context))) {
                rpc_ns_inq_handle_t         inq_ctxt;
		uuid_t 			    obj_uuid;
                SEC_DBG_PRINTF(1,("sec__acl_check_binding: !REBOUND_YET\n"));

                    /* Try to refresh the object uuid on the first refresh */
                rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce,
                                                RESOLVED_NAME(context),
                                                &inq_ctxt, &lst);
		SEC_DBG_PRINTF(2,("sec__acl_check_binding: entry_object_inq_begin returned %x\n", lst));
                if (STATUS_EQUAL(&lst, rpc_s_no_more_members)) {
                    uuid_create_nil(&obj_uuid, &lst);
                }
                if (GOOD_STATUS(&lst)) {
                    rpc_ns_mgmt_handle_set_exp_age(inq_ctxt, 0, &lst);
                }
		if (GOOD_STATUS(&lst)) {
		    rpc_ns_entry_object_inq_next(inq_ctxt, &obj_uuid, &lst);
                    SEC_DBG_PRINTF(2,("sec__acl_check_binding: entry_object_inq_next returned %x\n", lst));
		    rpc_ns_entry_object_inq_done(&inq_ctxt, &lst2);
		}
		if (GOOD_STATUS(&lst)) {
		    OBJ_UUID(context) = obj_uuid;
		    SEC_OBJ_PRINTF(obj_uuid, "sec__acl_check_binding:");
		}

		/* Can't trust NSI cache on rebind */
                rpc_ns_binding_import_done(&(IMPORT_HANDLE(context)), &lst);
                rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
                                            RESOLVED_NAME(context),
                                            (rpc_if_handle_t) NULL,
                                            &(OBJ_UUID(context)),
                                            &(IMPORT_HANDLE(context)), &lst);
                SEC_DBG_PRINTF(1, ("sec__acl_check_binding: binding_import_begin of %s returned %x\n", RESOLVED_NAME(context), lst));
                if (GOOD_STATUS(&lst)) {
                    rpc_ns_mgmt_handle_set_exp_age(IMPORT_HANDLE(context),
                                                    0, &lst);
                    SEC_DBG_PRINTF(2,("sec__acl_check_binding: handle_set_exp_age returned %x\n", lst));
                } else {
                    IMPORT_HANDLE_VALID(context) = false;
                }
                REBOUND_YET(context) = true;
            }

            /* Import the next binding handle via the context's NSI handle */
            if (GOOD_STATUS(&lst)) {
                do {
		    rpc_ns_binding_import_next(IMPORT_HANDLE(context), &handle,
                        &lst);
		    SEC_DBG_PRINTF(1,
                    ("sec__acl_check_binding: binding_import_next status %x\n",
                        lst));
                    if (GOOD_STATUS(&lst)) {
                        SEC_HANDLE_PRINTF(handle, "sec_acl_bind:");
			hstr = NULL;
                        rpc_binding_to_string_binding(handle, &hstr, &lst);

                        /* Don't rebind to the same place we just failed on */
                        if (HANDLE_STR(context) != ((unsigned_char_p_t)NULL)) {
			    if (GOOD_STATUS(&lst)) {
                                SEC_DBG_PRINTF(1,("sec__acl_check_binding: comparing new binding %s\n", hstr));
                                SEC_DBG_PRINTF(1,("sec__acl_check_binding: against old binding %s\n", HANDLE_STR(context)));
                                if (equiv_acl_bdgs(HANDLE_STR(context),hstr)) {
                                    SET_STATUS(&lst, sec_acl_object_not_found);
                                    SEC_DBG_PRINTF(1,
                                    ("sec__acl_check_binding: skip over identical binding\n"));
                                    free((char *)hstr);
				}
			    }
                        }
                    } else {
                        IMPORT_HANDLE_VALID(context) = false;
                    }
		} while (IMPORT_HANDLE_VALID(context) && BAD_STATUS(&lst));
            }
            if (GOOD_STATUS(&lst)) {

                /* Set up authentication information if necessary */
                if (AUTH_INFO(context).info_type != sec_acl_bind_auth_none) {
                    *st_p = setup_auth(handle, &AUTH_INFO(context));
                }
                if (GOOD_STATUS(st_p)) {
                    /* Free previous binding */
                    rpc_binding_free(&(HANDLE(context)), &lst);
                    HANDLE_VALID(context) = true;   /* Rebound successfully */
                    HANDLE(context) = handle;
                    if (HANDLE_STR(context) != (unsigned_char_p_t)NULL)
                        free((char *) HANDLE_STR(context));
                    HANDLE_STR(context) = hstr;
                    return true;
                } else {
                    return false;
                }

            } else {            /* Couldn't get next binding handle.  */

                /* Reset the rpc handle, so it can at least be tried again. */
                HANDLE_VALID(context) = true;
                rpc_binding_reset(HANDLE(context), &lst2);
                SEC_DBG_PRINTF(2,
                    ("sec__acl_check_binding: binding_reset returned %x\n",
                    lst));

                /* Can't import any more handles */
                IMPORT_HANDLE_VALID(context) = false;

                /* Only pass back our status if caller didnt have its own */
                if (GOOD_STATUS(st_p)) {
                    SET_STATUS(st_p, lst);
                }

                /* Make sure caller exits its loop so app can again if necc */
                return false;
            }

        } else {                            /* Can't rebind without NSI yet */
            /* Reset the handle to valid, so it can at least be tried again */
            HANDLE_VALID(context) = true;
            rpc_binding_reset(HANDLE(context), &lst2);
            SEC_DBG_PRINTF(2,
                ("sec__acl_check_binding: binding_reset returned %x\n", lst));

            /* but make sure it exits the caller's loop this time */
            return false;
        }
    }
    CLEAR_STATUS(st_p);
    return true;
}

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
/* s e c _ a c l _ b i n d _ l o c a l
 *
 * Establish a connection.
 * The if_version and lepv pair identify the sec_acl mgr to use.
 *
 * Error Status:
 */
PUBLIC void sec_acl_bind_local (
    /* [in] */ unsigned32 if_version,
    /* [in] */ idl_void_p_t lepv,
    /* [in] */ idl_char *entry_name,
    /* [out] */ sec_acl_handle_t *sech_p,
    /* [out] */ error_status_t *st_p
)
{
    sec_acl_pvt_handle_blk_t    *new_context;

    *sech_p = NULL;

    if (lepv == NULL || (if_version != 0 && if_version != 1)) {
        SET_STATUS(st_p, sec_acl_invalid_site_name);/* Need a better status! */
	return;
    }

    CLEAR_STATUS(st_p);

    /* Create an empty acl handle context */
    new_context = NEW(sec_acl_pvt_handle_blk_t);
    if (new_context != NULL) {
	memset(new_context, 0, sizeof(sec_acl_pvt_handle_blk_t));
        if (entry_name != (unsigned_char_p_t) NULL) {
            new_context->component_name     =
                (unsigned_char_p_t) malloc(strlen((char *)entry_name)+1);
	    if (new_context->component_name == NULL) {
		free(new_context);
		SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		return;
	    }
            strcpy((char *)new_context->component_name, (char *)entry_name);
        } else {
            new_context->component_name     = (unsigned_char_p_t) NULL;
        }
        new_context->resolved_name          = (unsigned_char_p_t) NULL;
	new_context->string_binding         = (unsigned_char_p_t) NULL;
        new_context->rpc_handle		    = NULL;
        new_context->import_handle	    = NULL;
        new_context->rpc_handle_valid	    = false;
        new_context->import_handle_valid    = false;
        new_context->rebound                = false;
        new_context->is_dfs_acl             = false;
        new_context->status                 = error_status_ok;
        new_context->if_version             = if_version;
	new_context->lepv.epv		    = lepv;
        SET_STATUS(&new_context->status, error_status_ok);
        *sech_p = (sec_acl_handle_t) new_context;
    } else {
        SET_STATUS(st_p, sec_acl_cant_allocate_memory);
    }
}
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
