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
 * $Log: aud_util.c,v $
 * Revision 1.1.11.3  1996/02/18  19:12:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:24  marty]
 *
 * Revision 1.1.11.2  1995/12/08  21:19:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/8  1995/08/18  19:53 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/HPDCE02/8  1995/08/18  19:52 UTC  kline_s
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/08/17  21:30 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/07/06  19:54 UTC  kline_s
 * 	Fix international release audit test problems in using rpc_c_authn_level_pkt_privacy
 * 	as the level of protection for the rpc_binding_set/_auth_info() and
 * 	rpc_binding_inq_auth_client() calls.  This uses DES encryption, which is not supported
 * 	in the international version.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/15  17:41 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:40 UTC  truitt
 * 	CHFts15237: Make sure _e variable is set when the
 * 	aud empty ec list condition is hit.
 * 
 * 	HP revision /main/HPDCE02/5  1994/11/18  14:49 UTC  truitt
 * 	Merge unintegrated
 * 	[1995/12/08  17:54:13  root]
 * 
 * Revision 1.1.7.1  1994/10/19  17:31:16  rousseau
 * 	Move frees in the one_or_more routines (CR 12665).
 * 	[1994/10/19  17:27:09  rousseau]
 * 
 * 	HP revision /main/HPDCE02/4  1994/10/11  13:34 UTC  jane
 * 	Merge Hpdce02_04 to HPDCE02 branch
 * 
 * Revision 1.1.2.24  1994/10/05  17:57:57  rousseau
 * 	Fix multi-guide logic (CR 12502).
 * 	[1994/10/05  17:57:31  rousseau]
 * 
 * Revision 1.1.2.23  1994/10/02  19:13:28  rousseau
 * 	Fix up a few more free's
 * 	[1994/10/02  19:13:11  rousseau]
 * 
 * Revision 1.1.2.22  1994/10/02  17:57:23  rousseau
 * 	Remove bad free
 * 	[1994/10/02  17:54:48  rousseau]
 * 
 * 	HP revision /main/HPDCE02/3  1994/10/03  18:31 UTC  jane
 * 	merge from Hpdce02_03
 * 
 * Revision 1.1.2.21  1994/09/23  20:40:06  rousseau
 * 	Secure binding changes (CR 12214).
 * 	[1994/09/23  20:36:47  rousseau]
 * 
 * Revision 1.1.2.20  1994/09/15  23:24:29  rousseau
 * 	Fix bad free.
 * 	[1994/09/15  23:13:06  rousseau]
 * 
 * Revision 1.1.2.19  1994/09/15  22:24:11  rousseau
 * 	Cache binding handles correctly (CR 12213).
 * 	[1994/09/15  22:23:53  rousseau]
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/22  15:50 UTC  jane
 * 	merge from Hpdce02_02
 * 
 * Revision 1.1.2.18  1994/09/01  14:16:45  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:03  rousseau]
 * 
 * Revision 1.1.2.17  1994/08/26  18:31:43  kevins
 * 	OT 11888 replace  obsolete constants
 * 	[1994/08/26  18:22:32  kevins]
 * 
 * Revision 1.1.2.16  1994/08/25  15:51:09  rousseau
 * 	Change _POSIX_PATH_MAX to DCECP_PATH_MAX (CR 11866).
 * 	[1994/08/25  15:50:46  rousseau]
 * 
 * Revision 1.1.2.15  1994/08/17  19:16:26  rousseau
 * 	Added new condition pending (CR 11679).
 * 	[1994/08/17  19:15:48  rousseau]
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/19  21:27 UTC  pulsinelli
 * 	change _POSIX_PATH_MAX to MAXPATHLEN
 * 
 * Revision 1.1.2.14  1994/08/12  17:40:15  rousseau
 * 	Moved ec table init earlier in aud_bind_to_server() (CR 11126).
 * 	[1994/08/12  17:40:05  rousseau]
 * 
 * Revision 1.1.2.13  1994/08/09  19:56:28  rousseau
 * 	Merged with changes from 1.1.2.12
 * 	[1994/08/09  19:56:18  rousseau]
 * 
 * 	Change calls of sec_rgy_site_open() to sec_rgy_cell_bind() (CR 11560).
 * 	[1994/08/09  19:55:35  rousseau]
 * 
 * Revision 1.1.2.12  1994/08/08  21:04:10  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:01:40  melman]
 * 
 * Revision 1.1.2.11  1994/07/28  20:39:13  rousseau
 * 	64 bit changes (CR 11451).
 * 	Fix reading of audit daemon bindings (CR 11466).
 * 	[1994/07/28  20:38:04  rousseau]
 * 
 * Revision 1.1.2.10  1994/07/19  18:06:27  rousseau
 * 	Implemented reomte control and fixed ~ in paths (CR 11178 11179 10221).
 * 	[1994/07/19  18:05:41  rousseau]
 * 
 * Revision 1.1.2.9  1994/06/09  16:01:35  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:46:57  devsrc]
 * 
 * Revision 1.1.2.8  1994/05/18  21:17:57  rousseau
 * 	Initialize guides to NULL before every RPC in audfilter create and modify (CR 10373).
 * 	[1994/05/18  21:17:36  rousseau]
 * 
 * Revision 1.1.2.7  1994/05/09  19:26:03  rousseau
 * 	Fixes for:
 * 	10394   Need better error messages for list problems.
 * 	10393   core dump in dcp_aud_check_multi_guides()
 * 	10391   audf create doesn't catch invalid condition.
 * 	10383   audfilter create dies w/o -attribute option
 * 	10293   Memory allocated by dce_sprinf is not released
 * 	9869    the dcecp audit component is not complete
 * 	[1994/05/09  19:23:04  rousseau]
 * 
 * Revision 1.1.2.6  1994/04/18  16:58:39  rousseau
 * 	Second drop of audit functionality (CR 9691).
 * 	[1994/04/18  16:55:16  rousseau]
 * 
 * Revision 1.1.2.3  1994/03/31  18:39:49  rousseau
 * 	Changes to all audfilter commands (CR 9869).
 * 	[1994/03/31  18:39:30  rousseau]
 * 
 * Revision 1.1.2.2  1994/02/02  20:00:36  rousseau
 * 	Initial implementation of audfilter, audtrail and audevents.
 * 	     Includes help functions.
 * 	[1994/02/02  19:57:04  rousseau]
 * 
 * Revision 1.1.2.1  1994/01/06  16:51:00  rousseau
 * 	Initial version.
 * 	[1994/01/06  16:49:24  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: aud_util.c
 *
 * DESCRIPTION:
 *   This module contains internal functions used to implement the
 * audit operations in dcecp.
 *
 */

/* INCLUDE FILES  */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <dce/dce.h>
#include <dce/secidmap.h>
#include <dce/eslp.h>
#include <dce/dce_cf.h>
#include <aud.h>
#include <util_binding.h>
#include <util_general.h>


/* DECLARATIONS */

/* INTERNAL VARIABLES */

boolean32                       dcp_aud_init = FALSE;
boolean32                       dcp_aud_valid_binding = FALSE;
rpc_binding_handle_t            dcp_aud_binding_h = NULL;
char                            *dcp_aud_binding_site = NULL;

char *Type[aud_e_esl_max] =
{
    "principal",
    "foreign_principal",
    "group",
    "foreign_group",
    "cell",
    "cell_overridable",
    "world",
    "world_overridable"
};

aud_index_entry_t aud_g_cp_condition[] = {
    {(unsigned char *)"success", 0x00000001},
    {(unsigned char *)"failure", 0x00000002},
    {(unsigned char *)"denial", 0x00000004},
    {(unsigned char *)"pending", 0x00000008},
    {(unsigned char *)"all", 0x0000000F}
};

aud_index_entry_t aud_g_cp_action[] = {
    {(unsigned char *)"none", 0x0},
    {(unsigned char *)"log", 0x00000001},
    {(unsigned char *)"alarm", 0x00000002},
    {(unsigned char *)"all", 0x00000003}
};

/* PROTOTYPES */

extern boolean32 get_ec_name_from_number(unsigned32, char *);
extern int dcp_id_parse_name(Tcl_Interp *, sec_rgy_name_t, 
                             sec_rgy_name_t, sec_rgy_name_t);
int dcp_aud_read_bindings(Tcl_Interp *, rpc_binding_vector_t **);
int dcp_aud_read_identity(Tcl_Interp *, unsigned_char_t **);
int dcp_aud_is_guide(Tcl_Interp *, char *);
int dcp_aud_one_or_more_conds(Tcl_Interp *, char *);
int dcp_aud_one_or_more_actions(Tcl_Interp *, char *);
int dcp_aud_is_condition(char *);
int dcp_aud_is_action   (char *);


/*
 * FUNCTION:    aud_bind_to_server
 *
 * OVERVIEW:    Get a binding handle to the running auditd.
 * 
 *
 * INPUTS:
 *  user_site
 *    A namespace entry or a string binding used to find the correct server.
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
aud_bind_to_server(Tcl_Interp *interp, char *user_site)
{
    char                        *s_var = NULL, *site = NULL;
    char                        *tmp = NULL;
    char                        *host_name = NULL, *p;
    sec_rgy_name_t              residual_name;
    unsigned_char_t             *aud_princ_name;
    rpc_ns_handle_t             import_context;
    char                        **sbv, *str_bind;
    rpc_binding_vector_t        *bind_vec = 0;
    int                         sbc;
    boolean32                   site_is_str_binding = 0;
    error_status_t              st;


    /* Check for and cannonicalize any passed in site name or _s(aud) */
    if (user_site != NULL) {
        tmp = expand_cell_name(user_site, &st);
        if ((tmp != NULL) && (st == rpc_s_ok)) {
            site = tmp;
        }
        else {
            site = dce_strdup(user_site);
        }

        /* Set $_n to the site passed in */
        DCP_GENERAL_SET_NAMELIST(site);
    }
    else {
        /* Get the value of _s(aud) */
        s_var = DCP_GENERAL_GET_S_VAR("aud");

        /* If someone has unset _s(aud), squash the result */
        if (s_var == NULL) {
            Tcl_ResetResult(interp);
        }
        else if (strlen(s_var) > 0) {
            tmp = expand_cell_name(s_var, &st);    
            if ((tmp != NULL) && (st == rpc_s_ok)) {
                site = tmp;
            }
            else {
                site = dce_strdup(s_var);
            }
        }
    }
    
    /* Preload dce_aud_binding_site if it has never been used */
    if (dcp_aud_binding_site == NULL) {
        dcp_aud_binding_site = dce_strdup("NoSite");
    }

    if (dcp_aud_valid_binding) {
        if (site && (strcmp(site, dcp_aud_binding_site) == 0)) {
            free(site);
            return TCL_OK;
        }
        else if ((site == NULL) 
                 && (strcmp(dcp_aud_binding_site, "local") == 0)) {
            return TCL_OK;
        }
    }

    dcp_aud_valid_binding = FALSE;

    /* Set $_n to the site passed in */
    if (site != NULL) {
        DCP_GENERAL_SET_NAMELIST(site);
    }

    if (site == NULL) {
        /* Set cache to the local site */
        free(dcp_aud_binding_site);
        dcp_aud_binding_site = dce_strdup("local");

        /*
         * Read the local bindings file, and set the global audit
         * binding handle upon success.
         */
        if (dcp_aud_read_bindings(interp, &bind_vec) != TCL_OK) {
            return TCL_ERROR;
        }
        
        /* If we have one or more bindings, just pick the first one */
        if (bind_vec->count > 0) {
            dcp_aud_binding_h = bind_vec->binding_h[0];
        }
    } /* if (site == NULL) */
    else {
        /* Cache the site we are binding to */
        free(dcp_aud_binding_site);
        dcp_aud_binding_site = dce_strdup(site);

	if (!strchr(site, ' ')) {
	    /* See if we have a single string syntax binding */
	    rpc_binding_from_string_binding((unsigned_char_t *)site,
                                            &dcp_aud_binding_h,
                                            &st);
            if (st != rpc_s_ok) {
                /* See if we have a namespace entry */
                rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
                                            (unsigned_char_t *)site,
                                            AUD_SERVER_INTERFACE,
                                            NULL,
                                            &import_context,
                                            &st);
                /* 
                 * If it's not a string binding or a namespace entry,
                 * give up.
                 */
                DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
                
                rpc_ns_binding_import_next(import_context, 
                                           &dcp_aud_binding_h, 
                                           &st);
                DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);

                rpc_ns_binding_import_done(&import_context, &st);
                DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
                
            } /* if (st != rpc_s_ok) */
            else {
                site_is_str_binding = TRUE;
            }
        } else {
	    /* We have a single TCL syntax binding */
	    if (Tcl_SplitList(interp, site, &sbc, &sbv) != TCL_OK) {
                free(site);
		return(TCL_ERROR);
            }

	    str_bind = dcp_binding_tcl_to_string(sbc, sbv, &st);
            if (str_bind == NULL) {
                free(site);
                return TCL_ERROR;
            }

	    rpc_binding_from_string_binding((unsigned_char_t *)str_bind, 
                                            &dcp_aud_binding_h, 
                                            &st);
	    rpc_string_free((unsigned char **)&str_bind, &st);
            free((char *)sbv);
	    DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
            site_is_str_binding = TRUE;
	} /* End of evaluating site */
    } /* if (site == NULL) */

    /* Conjure up the audit daemon principal name. */
    if (site_is_str_binding) {
        rpc_mgmt_inq_server_princ_name(dcp_aud_binding_h,
                                       rpc_c_authn_dce_secret,
                                       &aud_princ_name,
                                       &st);
        DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
    }
    else {
        if (site == NULL) {
            if (dcp_aud_read_identity(interp, &aud_princ_name) != TCL_OK) {
                return TCL_ERROR;
            }
        } 
        else {
            if (dcp_id_parse_name(interp, 
                                  (unsigned_char_t *)site, 
                                  NULL, 
                                  residual_name) != TCL_OK) {
                free(site);
                return TCL_ERROR;
            }

            host_name = dce_strdup((char *)residual_name);
            p = strrchr(host_name, '/');
            *p = '\0';
            dce_cf_prin_name_from_host(host_name,
                                       (char **)&aud_princ_name,
                                       &st);
            free(host_name);
            if (st != dce_cf_st_ok) {
                DCP_SET_RESULT_CODE(st);
                free(site);
                return TCL_ERROR;
            }
        }
    }
        
    /* All done with site */
    free(site);

    if (dcp_is_logged_in()) {
        rpc_binding_set_auth_info(dcp_aud_binding_h,
                                  aud_princ_name,
				  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_default,
                                  NULL,
                                  rpc_c_authz_dce,
                                  &st);
    }
    else {
        rpc_binding_set_auth_info(dcp_aud_binding_h,
                                  aud_princ_name,
                                  rpc_c_authn_level_none,
                                  rpc_c_authn_none,
                                  NULL,
                                  rpc_c_authz_dce,
                                  &st);
    }
    DCP_CHECK_RPC_ERROR(st);

    if (site_is_str_binding) {
        rpc_string_free(&aud_princ_name, &st);
        DCP_CHECK_RPC_ERROR(st);
    }
    else {
        free(aud_princ_name);
    }

    /* Do some one-time initialization */
    if (dcp_aud_init == FALSE) {
        /* Init svc */
        dce_aud_init_svc();
        
        /* Init ec list */
        input_ec_table();
        
        dcp_aud_init = TRUE;
    }
    
    dcp_aud_valid_binding = TRUE;
    return TCL_OK;
}


/*
 * FUNCTION:    dcp_aud_read_bindings
 *
 * OVERVIEW:    The user wants to talk to the local auditd, so save some
 *              cycles and read the bindings from the local filesystem.
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  bv
 *    A pointer to a pointer that contains a structure of binding vectors.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_aud_read_bindings(Tcl_Interp *interp, rpc_binding_vector_t **bv)
{
    static char                 aud_daemon_binding_file[DCECP_PATH_MAX];
    FILE                        *fp = 0;
    error_status_t              st;

    /* Since name is static, it will be 0. Use this to check for init */
    if (aud_daemon_binding_file[0] == '\0') {
        sprintf(aud_daemon_binding_file, 
                "%s/var/audit/daemon_binding", 
                dcelocal_path);
    }

    if ((fp = fopen(aud_daemon_binding_file, "r")) == NULL) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_dmn_binding_file_open_fail,
                                    aud_daemon_binding_file);
        return TCL_ERROR;
    }
    
    dce_bindings_from_file(fp, bv, &st);
    DCP_CHECK_RPC_ERROR(st);
        
    (void)fclose(fp);

    return TCL_OK;
}


/*
 * FUNCTION:    dcp_aud_read_identity
 *
 * OVERVIEW:    The user wants to talk to the local auditd, read the
 *              daemons identity from the local file.
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  identity
 *    A pointer to the principal name of the local audit daemon.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_aud_read_identity(Tcl_Interp *interp, unsigned_char_t **pname)
{
    static char                 aud_daemon_identity_file[DCECP_PATH_MAX];
    FILE                        *fp = 0;
    char                        buff[256], *p;

    /* Since name is static, it will be 0. Use this to check for init */
    if (aud_daemon_identity_file[0] == '\0') {
        sprintf(aud_daemon_identity_file, 
                "%s/var/audit/daemon_identity", 
                dcelocal_path);
    }

    if ((fp = fopen(aud_daemon_identity_file, "r")) == NULL) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_id_file_open_fail,
                                    aud_daemon_identity_file);
        return TCL_ERROR;
    }
    
    if (fgets(buff, sizeof(buff), fp) == NULL) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_id_file_read_fail,
                                    aud_daemon_identity_file);
        return TCL_ERROR;
    }

    /* Nuke the \n */
    if ((p = strchr(buff, '\n')) != NULL)
        *p = '\0';
    
    (void)fclose(fp);

    if (strlen(buff) > 0) {
        *pname = (unsigned_char_t *)dce_strdup(buff);
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_id_file_read_fail,
                                    aud_daemon_identity_file);
        return TCL_ERROR;
    }
        
    return TCL_OK;
}


/*
 * FUNCTION:    aud_list_entries
 *
 * OVERVIEW:    List entries.
 * 
 *
 * INPUTS:
 *  esl_type
 *    Type of the entry to list.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  entry_ptr
 *    A pointer to the returned entry.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
aud_list_entries(Tcl_Interp *interp, aud_esl_type_t esl_type, void *entry_ptr)
{
    uuid_t                      cell_uuid;
    sec_rgy_name_t              subject_name;
    
    if (entry_ptr == NULL) {
        return TCL_OK;
    }

    if (FOREIGN_TYPE(esl_type)) {
        aud_esl_foreign_entry_p_t ep = (aud_esl_foreign_entry_p_t )entry_ptr;
        while (ep != NULL) {
            if (ep->guides != NULL) {
                if (aud_get_subject_name(interp, esl_type, ep->subject_uuid, 
                                 ep->cell_uuid, subject_name) == TCL_ERROR) {
                    return TCL_ERROR;
                }
                if (strcmp(interp->result, "") != 0) {
                    Tcl_AppendResult(interp, "\n", (char *)NULL);
                }
                Tcl_AppendResult(interp, "{", Type[esl_type], " ",
                                 subject_name, "}", (char *)NULL);
            } 
            ep = ep->next;
        }
    } 
    else { 
        aud_esl_entry_p_t ep = (aud_esl_entry_p_t )entry_ptr;
        while (ep != NULL) {
            if (ep->guides != NULL) {
                /* This smells like a bug. I think cell_uuid should be
                   ep->cell_uuid */
                if (aud_get_subject_name(interp, esl_type, ep->subject_uuid, 
                                 cell_uuid, subject_name) == TCL_ERROR) {
                    return TCL_ERROR;
                }
                if (strcmp(interp->result, "") != 0) {
                    Tcl_AppendResult(interp, "\n", (char *)NULL);
                }
                Tcl_AppendResult(interp, "{", Type[esl_type], " ",
                                 subject_name, "}", (char *)NULL);
            } 
            ep = ep->next;
        }
    }
    return TCL_OK;
}
    

/*
 * FUNCTION:    aud_get_subject_name
 *
 * OVERVIEW:    Get the subject name from the registry.
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
 *  void
 */
int
aud_get_subject_name(Tcl_Interp *interp,
                     aud_esl_type_t esl_type,
                     uuid_t subject_uuid,
                     uuid_t cell_uuid,
                     sec_rgy_name_t subject_name)
{
    unsigned_char_t             site_name[] = "/.:";
    sec_rgy_handle_t            rgy_context;
    sec_rgy_bind_auth_info_t    auth_info;
    sec_rgy_name_t              princ_name, cell_name;
    unsigned_char_t             *tmp_name, *tmp_cell, *tmp_group;
    error_status_t              st;

    /* Get a handle to the registry */
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;
    auth_info.info_type = sec_rgy_bind_auth_none;

    sec_rgy_cell_bind(site_name, &auth_info, &rgy_context, &st);
    DCP_CHECK_SEC_ERROR(st);

    switch (esl_type) {
    case aud_e_esl_princ:
        sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                               &subject_uuid, subject_name, &st);
        if (st == sec_rgy_object_not_found) {
            uuid_to_string(&subject_uuid, &tmp_name, &st);
            DCP_CHECK_UUID_ERROR(st);
            sprintf((char *)subject_name, "%s", tmp_name);
            rpc_string_free(&tmp_name, &st);
            DCP_CHECK_RPC_ERROR(st);
        }
        else {
            DCP_CHECK_SEC_ERROR(st);
        }
        break;

    case aud_e_esl_foreign_princ:
        sec_id_gen_name(rgy_context, &cell_uuid, &subject_uuid, subject_name,
                        cell_name, princ_name, &st);
        if (st == sec_rgy_object_not_found) {
            uuid_to_string(&cell_uuid, &tmp_cell, &st);
            DCP_CHECK_UUID_ERROR(st);
            uuid_to_string(&subject_uuid, &tmp_name, &st);
            DCP_CHECK_UUID_ERROR(st);
            sprintf((char *)subject_name, "%s/%s", tmp_cell, tmp_name);
            rpc_string_free(&tmp_cell, &st);
            rpc_string_free(&tmp_name, &st);
            DCP_CHECK_RPC_ERROR(st);
        }
        else {
            DCP_CHECK_SEC_ERROR(st);
        }
        break;

    case aud_e_esl_group:
        sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_group,
                               &subject_uuid, subject_name, &st);
        if (st == sec_rgy_object_not_found) {
            uuid_to_string(&subject_uuid, &tmp_name, &st);
            DCP_CHECK_UUID_ERROR(st);
            sprintf((char *)subject_name, "%s", tmp_name);
            rpc_string_free(&tmp_name, &st);
            DCP_CHECK_RPC_ERROR(st);
        }
        else {
            DCP_CHECK_SEC_ERROR(st);
        }
        break;

    case aud_e_esl_foreign_group:
        sec_id_gen_group(rgy_context, &cell_uuid, &subject_uuid, subject_name,
                         cell_name, princ_name, &st);
        if (st == sec_rgy_object_not_found) {
            uuid_to_string(&cell_uuid, &tmp_cell, &st);
            DCP_CHECK_UUID_ERROR(st);
            uuid_to_string(&subject_uuid, &tmp_group, &st);
            DCP_CHECK_UUID_ERROR(st);
            sprintf((char *)subject_name, "%s/%s", tmp_cell, tmp_group);
            rpc_string_free(&tmp_cell, &st);
            rpc_string_free(&tmp_group, &st);
            DCP_CHECK_RPC_ERROR(st);
        }
        else {
            DCP_CHECK_SEC_ERROR(st);
        }
        break;

    case aud_e_esl_cell:
    case aud_e_esl_cell_overridable:
        sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                               &subject_uuid, subject_name, &st);
        if (st == sec_rgy_object_not_found) {
            uuid_to_string(&subject_uuid, &tmp_name, &st);
            DCP_CHECK_UUID_ERROR(st);
            sprintf((char *)subject_name, "%s", tmp_name);
            rpc_string_free(&tmp_name, &st);
            DCP_CHECK_RPC_ERROR(st);
        }
        else {
            DCP_CHECK_SEC_ERROR(st);
        }
        break;

    default:
        break;
    }
    return TCL_OK;
}


/*
 * FUNCTION:    aud_list_guides
 *
 * OVERVIEW:    List the guides in an audit filter.
 * 
 *
 * INPUTS:
 *  guides
 *    List of guides to show.
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
aud_list_guides(Tcl_Interp *interp, aud_esl_guides_p_t guides)
{
    aud_esl_guides_p_t          guides_ptr = guides;
    aud_esl_evt_classes_p_t     ec_ptr;
    boolean32                   first;
    char                        buf[1024];
    char                        tmpbuf[AUD_MAX_STRING_LENGTH];
    char                        *tmp;
    int                         i;

    while (guides_ptr != NULL) {
        if (strcmp(interp->result, "") != 0) {
            Tcl_AppendResult(interp, "\n", (char *)NULL);
        }
        Tcl_AppendResult(interp, "{", (char *)NULL);

        /* Output the event class list */
        first = TRUE;
        buf[0] = '\0';
        if ((ec_ptr = guides_ptr->ec_list) == NULL) {
            tmp = (char *)dce_sprintf(dcp_s_aud_empty_ec_list);
            Tcl_AppendResult(interp, tmp, "}", (char *)NULL);
            DCP_GENERAL_SET_E_VAR(dcp_s_aud_empty_ec_list);
            free(tmp);
            return TCL_ERROR;
        }

        while (ec_ptr != NULL) {
            if (get_ec_name_from_number(ec_ptr->evt_class, tmpbuf) == TRUE) {
                if (!first) {
                    strcat(buf, (const char *)" ");
                }
                first = FALSE;
                strcat(buf, (const char *)tmpbuf);
            }
            else {
                sprintf(buf, "%li", ec_ptr->evt_class);
                Tcl_AppendResult(interp, buf, (char *)NULL);
            }
            ec_ptr = ec_ptr->next;
        }
        Tcl_AppendElement(interp, buf);

        /* Output the audit condition */
        first = TRUE;
        buf[0] = '\0';
        if (aud_g_cp_condition[AUD_MAX_CONDITIONS - 1].number ==
	    guides_ptr->audit_condition) {
            tmp = (char *)dce_sprintf(dcp_t_aud_all_actions);
            Tcl_AppendResult(interp, " ", tmp, (char *)NULL);
            free(tmp);
        }
        else {
            for (i = 0; i < AUD_MAX_CONDITIONS - 1; i++) {
                if ((aud_g_cp_condition[i].number & 
                     guides_ptr->audit_condition)) {
                    if (!first) {
                        strcat(buf, (const char *)" ");
                    }
                    first = FALSE;
                    strcat(buf, (const char *)aud_g_cp_condition[i].name);
                }
            }
            Tcl_AppendElement(interp, buf);
        }

        /* Output the audit action */
	if (guides_ptr->audit_action == 0) {
            tmp = (char *)dce_sprintf(dcp_t_aud_no_action);
            Tcl_AppendResult(interp, " ", tmp, (char *)NULL);
            free(tmp);
	} 
        
        /*
         * If the 'all' bit is set, then just print out 'all', don't
         * print out all the values.
         */
        else {
            if (aud_g_cp_action[AUD_MAX_ACTIONS - 1].number == 
                guides_ptr->audit_action) {
                tmp = (char *)dce_sprintf(dcp_t_aud_all_actions);
                Tcl_AppendResult(interp, " ", tmp, (char *)NULL);
                free(tmp);
            }
            else {
                first = TRUE;
                buf[0] = '\0';
                for (i = 0; i < AUD_MAX_ACTIONS - 1; i++) {
                    if ((aud_g_cp_action[i].number &
                         guides_ptr->audit_action)) {
                        if (!first) {
                            strcat(buf, (const char *)" ");
                        }
                        first = FALSE;
                        strcat(buf, (const char *)aud_g_cp_action[i].name);
                    }
                }
                Tcl_AppendElement(interp, buf);
            }
	}
        guides_ptr = guides_ptr->next;
        Tcl_AppendResult(interp, "}", (char *)NULL);
    }

    return TCL_OK;
}


/*
 * FUNCTION:    dcp_aud_check_multi_guides
 *
 * OVERVIEW:    Check if the list we are passed a list with multiple guides.
 * 
 *
 * INPUTS:
 *  list
 *    A pointer to the list to scan.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE if the list has multiple guides, FALSE if not.
 */
int
dcp_aud_check_multi_guides(Tcl_Interp *interp, char *list)
{
    int                         ac, i;
    char                        **av;

    if (dcp_aud_is_guide(interp, list)) {
        return FALSE;
    }
    else {
        Tcl_SplitList(interp, list, &ac, &av);
        
        for (i = 0; i < ac; i++) {
            if (!dcp_aud_is_guide(interp, av[i])) {
                free((char *)av);
                return FALSE;
            }
        }
        free((char *)av);
        return TRUE;
    }
}


/*
 * FUNCTION:    dcp_aud_is_guide
 *
 * OVERVIEW:    Test if the string passed is a guide.
 * 
 *
 * INPUTS:
 *  string
 *    The string to test.
 *    
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE if the string is a guide, FALSE if not.
 */
int
dcp_aud_is_guide(Tcl_Interp *interp, char *string)
{
    int                         ac;
    char                        **av;

    Tcl_SplitList(interp, string, &ac, &av);
    if (ac != 3) {
        free((char *)av);
        return FALSE;
    }

    if (!dcp_aud_one_or_more_conds(interp, av[1])) {
        free((char *)av);
        return FALSE;
    }

    if (!dcp_aud_one_or_more_actions(interp, av[2])) {
        free((char *)av);
        return FALSE;
    }

    free((char *)av);
    return TRUE;
}


/*
 * FUNCTION:    dcp_aud_one_or_more_conds
 *
 * OVERVIEW:    Test if the string passed is one or more conditions.
 * 
 *
 * INPUTS:
 *  string
 *    The string to test.
 *    
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE if the string is a condition, FALSE if not.
 */
int
dcp_aud_one_or_more_conds(Tcl_Interp *interp, char *string)
{
    int                         ac, i;
    char                        **av;

    if (!dcp_aud_is_condition(string)) {
        Tcl_SplitList(interp, string, &ac, &av);
        for (i = 0; i < ac; i++) {
            if (!dcp_aud_is_condition(av[i])) {
                free((char *)av);
                return FALSE;
            }
        }
        free((char *)av);
    }
    return TRUE;
}


/*
 * FUNCTION:    dcp_aud_one_or_more_actions
 *
 * OVERVIEW:    Test if the string passed is one or more actions.
 * 
 *
 * INPUTS:
 *  string
 *    The string to test.
 *    
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE if the string is a condition, FALSE if not.
 */
int
dcp_aud_one_or_more_actions(Tcl_Interp *interp, char *string)
{
    int                         ac, i;
    char                        **av;

    if (!dcp_aud_is_action(string)) {
        Tcl_SplitList(interp, string, &ac, &av);
        for (i = 0; i < ac; i++) {
            if (!dcp_aud_is_action(av[i])) {
                free((char *)av);
                return FALSE;
            }
        }
        free((char *)av);
    }
    return TRUE;
}


/*
 * FUNCTION:    dcp_aud_is_condition
 *
 * OVERVIEW:    Test if the string passed is an audit condition.
 * 
 *
 * INPUTS:
 *  string
 *    The string to test.
 *    
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE if the string is a condition, FALSE if not.
 */
int
dcp_aud_is_condition(char *string)
{
    int                         i;

    for (i = 0; i < AUD_MAX_CONDITIONS; i++) {
        if (strcmp(string, (char *)aud_g_cp_condition[i].name) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}


/*
 * FUNCTION:    dcp_aud_is_action
 *
 * OVERVIEW:    Test if the string passed is an audit action.
 * 
 *
 * INPUTS:
 *  string
 *    The string to test.
 *    
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TRUE if the string is an action, FALSE if not.
 */
int
dcp_aud_is_action(char *string)
{
    int                         i;

    for (i = 0; i < AUD_MAX_ACTIONS; i++) {
        if (strcmp(string, (char *)aud_g_cp_action[i].name) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}
