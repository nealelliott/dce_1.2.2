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
 * $Log: util_login.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:25:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/03  19:47 UTC  jrr
 * 	Move init of login mutex to main.c (CHFts15101).
 * 
 * 	HP revision /main/jrr_dcecp_chfts15101/1  1995/04/26  22:56 UTC  jrr
 * 	Move init of global login list mutex to Tcl_AppInit().
 * 	[1995/12/08  17:58:23  root]
 * 
 * Revision 1.1.2.12  1994/09/29  19:58:52  baum
 * 	Added + 1 to a couple malloc(strlen(p)) constructs (OT 12395)
 * 	[1994/09/29  19:58:38  baum]
 * 
 * Revision 1.1.2.11  1994/09/01  14:16:56  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:17  rousseau]
 * 
 * Revision 1.1.2.10  1994/08/09  19:56:32  rousseau
 * 	Change calls of sec_rgy_site_open() to sec_rgy_cell_bind() (CR 11560).
 * 	[1994/08/09  19:55:38  rousseau]
 * 
 * Revision 1.1.2.9  1994/07/28  20:34:54  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:31  rousseau]
 * 
 * Revision 1.1.2.8  1994/07/11  21:16:11  rousseau
 * 	Remove call to sec_login_certify_identity() (CR 11242).
 * 	[1994/07/11  21:15:54  rousseau]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:38  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:15  devsrc]
 * 
 * Revision 1.1.2.6  1994/06/06  18:39:27  rousseau
 * 	Defuncted old functions. Code review changes.
 * 	[1994/06/06  18:39:06  rousseau]
 * 
 * Revision 1.1.2.5  1994/04/12  13:54:10  rousseau
 * 	Add mutex unlock call to error exit (CR 10317).
 * 	[1994/04/12  13:53:38  rousseau]
 * 
 * Revision 1.1.2.4  1994/03/31  16:34:23  rousseau
 * 	Fix handling of _c and _u (CR 10100 10101).
 * 	[1994/03/31  16:31:45  rousseau]
 * 
 * Revision 1.1.2.3  1994/03/17  21:42:30  rousseau
 * 	Removed overly restrictive check for authentication (CR 10144).
 * 	Logout should set $u to NULL, nut unset it (CR 10104).
 * 	Changed action of get_login_context when no context exists (CR 10075).
 * 	[1994/03/17  21:41:18  rousseau]
 * 
 * Revision 1.1.2.2  1993/12/27  16:29:27  rousseau
 * 	Updated convenience variable support.
 * 	[1993/12/27  16:29:21  rousseau]
 * 
 * Revision 1.1.2.1  1993/12/21  16:17:49  rousseau
 * 	Style guide changes.
 * 	[1993/12/21  16:17:26  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.7  1993/11/18  18:25:22  rousseau
 * 	Put in check to prevent release_login_context() from
 * 	calling sec_login_release_context() with a NULL handle.
 * 	[1993/11/18  18:25:16  rousseau]
 *
 * Revision 1.1.2.6  1993/11/11  19:52:38  rousseau
 * 	Added delete_all_login_contexts() for cleanup.
 * 	[1993/11/11  19:51:46  rousseau]
 * 
 * Revision 1.1.2.5  1993/11/11  14:31:04  rousseau
 * 	Added initialization fo global_context_list from dcecp_login.h
 * 	[1993/11/11  14:29:37  rousseau]
 * 
 * Revision 1.1.2.4  1993/11/11  13:55:30  rousseau
 * 	Added release_login_context(). Moved check_initial_auth() here
 * 	from main.c .
 * 	[1993/11/11  13:55:24  rousseau]
 * 
 * Revision 1.1.2.3  1993/11/10  21:58:08  melman
 * 	added return TCL_OK to add_login_context
 * 	[1993/11/10  21:57:50  melman]
 * 
 * Revision 1.1.2.2  1993/11/10  18:01:30  rousseau
 * 	Fixed malloc of new_context->princ.
 * 	[1993/11/10  18:01:23  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/05  15:37:36  rousseau
 * 	Initial version.
 * 	[1993/11/05  15:30:05  rousseau]
 * 
 */

/*
 * MODULE: util_login.c
 *
 * DESCRIPTION:
 *   This module contains routines that maintain the global stack
 * of login contexts.
 *
 */

/* INCLUDE FILES  */

#include <stdlib.h>
#include <util_login.h>

/* DECLARATIONS */

dcp_login_context_p_t global_context_list = NULL;
boolean32 initial_context_flag = FALSE;

/* INTERNAL VARIABLES */

/* PROTOTYPES */


/*
 * FUNCTION:    add_login_context
 *
 * OVERVIEW:    Allocate a new login context structure and attach it to 
 *              the head of the global list. 
 * 
 *
 * INPUTS:
 *  login_context_h
 *    The login handle to be added to the global list.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
add_login_context(Tcl_Interp *interp, sec_login_handle_t login_context_h)
{
    dcp_login_context_p_t       new_context;
    
    /* Create a new login context structure */
    new_context = (dcp_login_context_t *)malloc(sizeof(dcp_login_context_t));
    if (new_context == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_no_memory);
        return TCL_ERROR;
    }
    
    if (dcp_get_princ_and_cell(interp, 
                               login_context_h, 
                               &new_context->princ, 
                               &new_context->cell) != TCL_OK) {
        return TCL_ERROR;
    }
    new_context->context = login_context_h;
    
    /* Add the new structure to the head of the global_context_list */
    /* LOCK login_list */
    if (pthread_mutex_lock(&login_list) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_lock_fail);
        free(new_context->princ);
        free(new_context->cell);
        free(new_context);
        return TCL_ERROR;
    }
    
    new_context->next = global_context_list;
    global_context_list = new_context;
    
    /* UNLOCK login_list */
    if (pthread_mutex_unlock(&login_list) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_unlock_fail);
        return TCL_ERROR;
    }
    
    /*  Set the _u and _c conv. variables */
    DCP_GENERAL_SET_U_VAR(global_context_list->princ);
    DCP_GENERAL_SET_C_VAR(global_context_list->cell);
    
    return TCL_OK;
}


/*
 * FUNCTION:    delete_all_login_contexts
 *
 * OVERVIEW:    Delete all login contexts off of the global list by calling
 *              delete_login_context() for each context on the list. This is
 *              primarily used for exit cleanup.
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
delete_all_login_contexts(Tcl_Interp *interp)
{
    while(global_context_list != NULL) {
        if (initial_context_flag) {
            if (global_context_list->next == NULL) {
                return TCL_OK;
            }
        }
        
        if (delete_login_context(interp) != TCL_OK) {
            return TCL_ERROR;
        }
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    delete_login_context
 *
 * OVERVIEW:    Delete a login context from the head of the global list. 
 *              Set the current context (via sec_login_set_current_context())
 *              to the next login context on the list (if any). Also set the
 *              value of $u to the current principal name, or unset it if
 *              we are deleting the last login context. This routine is
 *              careful about not deleting a login context inhereted from
 *              the shell.
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
delete_login_context(Tcl_Interp *interp)
{
    dcp_login_context_p_t       old_context;
    error_status_t              st;
    
    if (global_context_list == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_not_logged_in);
        return TCL_ERROR;
    }
    
    /* LOCK login_list */
    if (pthread_mutex_lock(&login_list) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_lock_fail);
        return TCL_ERROR;
    }
    
    /* 
     * Check if we are trying to purge the context we inhereted
     * from the shell.
     */
    if (initial_context_flag) {
        if (global_context_list->next == NULL) {
            pthread_mutex_unlock(&login_list);
            DCP_SET_RESULT_CODE(dcp_s_purge_initial_context);
            return TCL_ERROR;
        }
    }
    
    old_context = global_context_list;
    global_context_list = global_context_list->next;
    
    /* UNLOCK login_list */
    if (pthread_mutex_unlock(&login_list) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_unlock_fail);
        return TCL_ERROR;
    }
    
    /* Set the new context and $u */  
    if (global_context_list == NULL) {
        DCP_GENERAL_SET_U_VAR("");
        DCP_GENERAL_SET_C_VAR("");
    }
    else {
        sec_login_set_context(global_context_list->context, &st);
        DCP_CHECK_SEC_ERROR(st);
        DCP_GENERAL_SET_U_VAR(global_context_list->princ);
        DCP_GENERAL_SET_C_VAR(global_context_list->cell);
    }
    
    /* Destroy the old context */
    sec_login_purge_context(&old_context->context, &st);
    DCP_CHECK_SEC_ERROR(st);
    
    free(old_context->princ);
    free(old_context->cell);
    free(old_context);
    
    return TCL_OK;
}


/*
 * FUNCTION:    check_initial_auth
 *
 * OVERVIEW:    Quietly check to see if we inhereted a login context from the
 *              shell. If so, prime the global login context list with it.
 *              Set the global initial_context_flag if there is an 
 *              inhereted context so we don't blow it away when we clean 
 *              up upon exit.
 *
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
check_initial_auth(Tcl_Interp *interp)
{
    sec_login_handle_t          login_context_h;
    error_status_t              st;
    
    sec_login_get_current_context(&login_context_h, &st);
    /* If no context, just return. */
    if (st == sec_login_s_no_current_context) {
        return TCL_OK;
    }
    DCP_CHECK_SEC_ERROR(st);
    
    /* Create a new login context structure on the global list */
    if (add_login_context(interp, login_context_h) != TCL_OK) {
        return TCL_ERROR;
    }
    
    /* Create mutex for initial login context flag */
    if (pthread_mutex_init(&initial_login, pthread_mutexattr_default) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_init_fail);
        return TCL_ERROR;
    }
    
    /* LOCK initial_login flag */
    if (pthread_mutex_lock(&initial_login) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_lock_fail);
        return TCL_ERROR;
    }
    
    initial_context_flag = TRUE;
    
    /* UNLOCK initial_login_list flag */
    if (pthread_mutex_unlock(&initial_login) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_mutex_unlock_fail);
        return TCL_ERROR;
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dcp_get_princ_and_cell
 *
 * OVERVIEW:    Get the principal and home cell names from a login context.
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_get_princ_and_cell(Tcl_Interp *interp,
                       sec_login_handle_t login_context_h,
                       char **princ,
                       char **cell)
{
    unsigned_char_t             site_name[] = "/.:"; 
    sec_login_net_info_t        net_info;
    sec_id_pac_t                pac;
    sec_id_t                    princ_id, realm_id;
    sec_rgy_handle_t            rgy_handle;
    sec_rgy_bind_auth_info_t    auth_info;
    sec_rgy_name_t              global_princ, local_princ, cell_name;
    error_status_t              st;

    /* Get network information for this context */
    sec_login_inquire_net_info(login_context_h, &net_info, &st);
    if (st != sec_login_s_not_certified) {
        DCP_CHECK_SEC_ERROR(st);
    }

    pac = net_info.pac;
    princ_id = pac.principal;
    realm_id = pac.realm;

    /* Get a handle to the registry */
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;
    auth_info.info_type = sec_rgy_bind_auth_dce;

    sec_rgy_cell_bind(site_name, &auth_info, &rgy_handle, &st);
    DCP_CHECK_SEC_ERROR(st);
    
    sec_id_gen_name(rgy_handle, &realm_id.uuid, &princ_id.uuid, global_princ, 
                    cell_name, local_princ, &st);
    DCP_CHECK_SEC_ERROR(st);
    
    /* Allocate memory for the cell name and the principal */
    *princ = (char *)malloc(strlen((char *)local_princ) + 1 );
    *cell = (char *)malloc(strlen((char *)cell_name) + 1 );
    strcpy(*princ, (char *)local_princ);
    strcpy(*cell, (char *)cell_name);

    /* Free the net info structure and close the rgy */
    sec_login_free_net_info(&net_info);
    sec_rgy_site_close(rgy_handle, &st);

    return TCL_OK;
}


/*
 * FUNCTION:    dcp_is_logged_in
 *
 * OVERVIEW:    See if there are any contexts in the global list. Note that
 *              this function will lie to you if you start dcecp with '-s'.
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_is_logged_in(void)
{
    if (global_context_list == NULL) 
        return 0;
    else
        return 1;
}
