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
 * $Log: dhd_secval.c,v $
 * Revision 1.1.4.2  1996/02/18  19:13:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:20:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/07  22:11 UTC  dat
 * 	Change initial assignment of "delay" to 0 instead of NULL
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/07  21:57 UTC  dat
 * 	Cast initial assignment of "delay" variable to satisfy AIX compiler
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/22  20:40 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:39 UTC  truitt
 * 	CHFts15953: Code cleanup.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/04  17:25 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/18  15:58 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/truitt_dcecp_chfts01/1  1995/01/18  15:56 UTC  truitt
 * 	Part of CHFts14059.  The output format for the ping
 * 	command (when it has multiple input parameters) is
 * 	incorrect.  Set the spacing properly.
 * 	[1995/12/08  17:55:45  root]
 * 
 * Revision 1.1.2.9  1994/09/19  19:34:16  baum
 * 	Fixed string binding handling (OTs 11938, 12226)
 * 	[1994/09/19  19:33:58  baum]
 * 
 * Revision 1.1.2.8  1994/09/12  16:12:09  baum
 * 	Fixed compilation warnings
 * 	[1994/09/12  16:11:32  baum]
 * 
 * 	Fix return from secval status (OT 12126)
 * 	[1994/09/12  15:48:56  baum]
 * 
 * Revision 1.1.2.7  1994/08/24  16:56:19  baum
 * 	Added secval status and string bindings to secval cmds
 * 	[1994/08/24  16:54:07  baum]
 * 
 * Revision 1.1.2.6  1994/07/08  21:18:08  pwang
 * 	Allowed unauthenticated access to dced [OT#11214]
 * 	[1994/07/08  21:15:23  pwang]
 * 
 * Revision 1.1.2.5  1994/06/22  16:08:24  pwang
 * 	Checked the return of dced_binding_create() properly
 * 	[OT#10948]
 * 
 * Revision 1.1.2.4  1994/06/09  16:02:03  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:34  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/18  21:57:28  pwang
 * 	Changes due to -simplename (fullname) support.
 * 	[1994/05/18  21:31:39  pwang]
 * 
 * Revision 1.1.2.2  1994/05/05  14:57:05  pwang
 * 	Used DCP_ARGV_NO_OPT_LEFTOVERS flag to detect illegal options.
 * 	[1994/05/05  14:53:11  pwang]
 * 
 * Revision 1.1.2.1  1994/04/26  20:03:53  pwang
 * 	Initial Implementation
 * 	[1994/04/26  19:37:54  pwang]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * dhd_secval.c
 *
 * DESCRIPTION:
 * This module contains all the dced secval related operations 
 * on a specified host thru dcecp.
 */

/*
 * INCLUDE FILES
 */
#include <dhd.h>
#include <util_binding.h>

/*
 * DECLARATIONS 
 */

/*
 * INTERNAL VARIABLES 
 */


/*
 * FUNCTION:    dcp_sv_ping 
 *
 * OVERVIEW:    Contact the dced secval to validate the DCE
 *		security service.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
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
dcp_sv_ping(ClientData clientData, 
            Tcl_Interp *interp, 
            int argc, 
            char **argv)
{
    char                   *entryname;
    char                   *hostname;
    int                    argc1, argc2;
    char                   **argv1, **argv2;
    int                    i;
    int                    number_of_hosts;
    dced_binding_handle_t  sv_handle;
    boolean32		   is_string_binding;
    error_status_t         status;
    rpc_binding_handle_t   rpc_handle;	

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table,
                      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (argc < 2) {
        hostname = NULL;
        number_of_hosts = 1;
    }
    else {
        /*
         * Set convenience variables.
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

        if (dcp_general_split_list(interp, argv[1], &argc1, &argv1) != TCL_OK) {
            return TCL_ERROR;
        }
        hostname = argv1[0];
        number_of_hosts = argc1;
    }

    /*
     * Loop through the hostname list.
     */
    i = 0;
    do {
        /*
         * Check for string binding first.  
         */
        is_string_binding = FALSE;
        status = error_status_ok;
        if (hostname != NULL ) {
            if (strchr(hostname, ' ' )) {
                /*
                 * Space in the name means might be TCL style string binding.
                 */
                if (dcp_general_split_list(interp, hostname, &argc2, &argv2) != TCL_OK) {
                    return TCL_ERROR;
                }
                hostname = argv2[0];
            }

            rpc_binding_from_string_binding((unsigned_char_t *)hostname, 
                                            &rpc_handle,
                                            &status);
            if (status == rpc_s_ok) {
                is_string_binding = TRUE;
                dced_binding_from_rpc_binding(dced_e_service_type_secval, 
                                              rpc_handle,
                                              &sv_handle,
                                              &status);
                DCP_DHD_CHECK_STATUS_BRK(status);
            }
        }

        if (!is_string_binding) {
            /*
             * Parse the hostnames.
             */
            if (dhd__parse_name(interp, NULL, dced_c_service_secval,
                                dced_c_service_secval_len, hostname,
                                &entryname, NULL, NULL) != TCL_OK) {
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }

            /*
             * Call dced_binding_create() to get binding.
             */
            dced_binding_create((dced_string_t) entryname, 0, &sv_handle, &status);
            free(entryname);
            DCP_DHD_CHECK_STATUS_BRK(status);
        }

        /*
         * Call dced_secval_validate() to validate the caller's credential
         * via a DCE security server at the specified host.
         */
        dced_secval_validate(sv_handle, &status); 
        if (DCP_DHD_GOOD_STATUS(status)) {
            if (i) {
                Tcl_AppendResult(interp, " 1", NULL);
            }
            else {
                Tcl_AppendResult(interp, "1", NULL);
            }
        }
        else {
            if (i) {
                Tcl_AppendResult(interp, " 0", NULL);
            }
            else {
                Tcl_AppendResult(interp, "0", NULL);
            }
        }
        DCP_DHD_RESET_STATUS(status);

        /*
         * Call dced_binding_free to free the dced binding handle.
         */
        dced_binding_free(sv_handle, &status);

        /*
         * Breat out of do loop, an error has been found.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    } while ((++i < number_of_hosts) && (hostname = argv1[i]));

    /*
     * Free the memory and return correct status.
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}

/*
 * FUNCTION:    dcp_sv_status
 *
 * OVERVIEW:    Ask the dced secval service if it is active.  (No auth necc.)
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
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
dcp_sv_status(ClientData clientData, 
              Tcl_Interp *interp, 
              int argc, 
              char **argv)
{
    char                   *entryname;
    char                   *hostname;
    int                    argc1, argc2;
    char                   **argv1, **argv2;
    int                    i;
    int                    number_of_hosts;
    dced_binding_handle_t  sv_handle;
    rpc_binding_handle_t   rpc_handle;	
    error_status_t         status;
    boolean32		   secval_is_active;
    boolean32              is_string_binding;
    error_status_t  	   status2;

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    DCP_DHD_CLEAR_STATUS(status);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
                     DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (argc < 2) {
        hostname = NULL;
        number_of_hosts = 1;
    }
    else {
        /*
         * Set convenience variables.
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

        if (dcp_general_split_list(interp, argv[1], &argc1, &argv1) != TCL_OK) {
            return TCL_ERROR;
        }
        hostname = argv1[0];
        number_of_hosts = argc1;
    }

    /*
     * Loop through the hostname list.
     */
    i = 0;
    do {
        /*
         * Check for string binding first.  
         */
        is_string_binding = FALSE;
        status = error_status_ok;
        if (hostname != NULL) {
            if (strchr(hostname, ' ' )) {
                /*
                 * Space in the name means might be TCL style string binding.
                 */
                if (dcp_general_split_list(interp, hostname, &argc2, &argv2) != TCL_OK) {
                    return TCL_ERROR;
                }
                hostname = argv2[0];
            }

            rpc_binding_from_string_binding((unsigned_char_t *)hostname,
                                            &rpc_handle,
                                            &status );
            if (status == rpc_s_ok) {
                is_string_binding = TRUE;
                dced_binding_from_rpc_binding(dced_e_service_type_secval, 
                                              rpc_handle, &sv_handle, &status);
                DCP_DHD_CHECK_STATUS_BRK(status);
            }
        }
	
        if (!is_string_binding) {
            /*
             * Parse the hostnames.
             */
            if (dhd__parse_name(interp, NULL, dced_c_service_secval,
                                dced_c_service_secval_len, hostname,
                                &entryname, NULL, NULL) != TCL_OK) {
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }

            /*
             * Call dced_binding_create() to get binding
             */
           dced_binding_create((dced_string_t) entryname, 0, &sv_handle, &status);
           free(entryname);
           DCP_DHD_CHECK_STATUS_BRK(status);
        }

        /*
         * Call dced_secval_status() to ask dced-secval if it is active.
         */
        dced_secval_status(sv_handle, &secval_is_active, &status); 
        if (DCP_DHD_GOOD_STATUS(status)) {
            if (secval_is_active) {
                if (i) {
                    DCP_APPEND_RESULT(" 1");
                }
                else {
                    DCP_SET_RESULT("1");
                }
            }
            else {
                if (i) {
                    DCP_APPEND_RESULT(" 0");
                }
                else {
                    DCP_SET_RESULT("0");
                }
            }
        }

        /*
         * Call dced_binding_free to free the dced binding handle.
         */
        dced_binding_free(sv_handle, &status2);

        /*
         * Out of do loop, an error has been found.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    } while ((++i < number_of_hosts) && (hostname = argv1[i]));

    /*
     * Free the memory and return correct status.
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sv_activate 
 *
 * OVERVIEW:    Enable the dced secval service 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
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
dcp_sv_activate(ClientData clientData, 
           	Tcl_Interp *interp, 
             	int argc, 
             	char **argv)
{
    char                   *entryname;
    char                   *hostname;
    int                    argc1, argc2;
    char                   **argv1, **argv2;
    int                    i;
    int                    number_of_hosts;
    dced_binding_handle_t  sv_handle;
    sec_login_handle_t	   login_context;
    boolean32		   authn_user = FALSE;
    error_status_t         status;
    error_status_t         status1;
    rpc_binding_handle_t   rpc_handle;
    boolean32		   is_string_binding;

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
                      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors.
     */
    if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (argc < 2) {
        hostname = NULL;
        number_of_hosts = 1;
    }
    else {
        /*
         * Set convenience variables.
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

        /*
         * Split the hostname list.
         */
        if (dcp_general_split_list(interp, argv[1], &argc1, &argv1) != TCL_OK) {
            return TCL_ERROR;
        }
        hostname = argv1[0];
        number_of_hosts = argc1;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated.
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop through the hostname list.
     */
    i = 0;
    do {
        /*
         * Check for string binding first.  
         */
        is_string_binding = FALSE;
        status = error_status_ok;
        if (hostname != NULL) {
            if (strchr( hostname, ' ' )) {
                /*
                 * Space in the name means might be TCL style string binding.
                 */
                if (dcp_general_split_list(interp, hostname, &argc2, &argv2) != TCL_OK) {
                    return TCL_ERROR;
                }
                hostname = argv2[0];
            }

            rpc_binding_from_string_binding((unsigned_char_t *)hostname, 
                                            &rpc_handle,
                                            &status);
            if (status == rpc_s_ok) {
                is_string_binding = TRUE;
                dced_binding_from_rpc_binding(dced_e_service_type_secval, 
                                              rpc_handle, &sv_handle, &status);
                DCP_DHD_CHECK_STATUS_BRK(status);
            }
        }

        if (!is_string_binding) {
            /*
             * Parse the hostnames.
             */
            if (dhd__parse_name(interp, NULL, dced_c_service_secval,
                                dced_c_service_secval_len, hostname,
                                &entryname, NULL, NULL) != TCL_OK) {
                DCP_DHD_SET_STATUS(status,  ~error_status_ok);
                break;
            }

            /*
             * Call dced_binding_create() to get binding.
             */
            dced_binding_create((dced_string_t) entryname, 0, &sv_handle, &status);
            free(entryname);
            DCP_DHD_CHECK_STATUS_BRK(status);
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
            dced_binding_set_auth_info(sv_handle,
                                       dhd_c_prot_level,
                                       dhd_c_authn_svc,
                                       (rpc_auth_identity_handle_t)login_context,
                                       dhd_c_authz_svc,
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_secval_start() to starts secval service on the 
         * specified host.
         */
        dced_secval_start(sv_handle, &status);

out:
        /*
         * Call dced_binding_free to free the dced binding handle.
         */
        dced_binding_free(sv_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        /*
         * Out of do loop, an error has been found.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    } while ((++i < number_of_hosts) && (hostname = argv1[i]));

    /*
     * Free the memory and return correct status.
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sv_deactivate 
 *
 * OVERVIEW:    Disable the dced secval service 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
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
dcp_sv_deactivate(ClientData clientData, 
             	  Tcl_Interp *interp, 
             	  int argc, 
             	  char **argv)
{
    char                   *entryname;
    char                   *hostname;
    int                    argc1, argc2;
    char                   **argv1, **argv2;
    int                    i;
    int                    number_of_hosts;
    dced_binding_handle_t  sv_handle;
    sec_login_handle_t	   login_context;
    boolean32		   authn_user = FALSE;
    error_status_t         status;
    error_status_t         status1;
    rpc_binding_handle_t   rpc_handle;
    boolean32		   is_string_binding;

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
                      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (argc < 2) {
        hostname = NULL;
        number_of_hosts = 1;
    }
    else {
        /*
         * Set convenience variables.
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

        /*
         * Split the hostname list.
         */
        if (dcp_general_split_list(interp, argv[1], &argc1, &argv1) != TCL_OK) {
            return TCL_ERROR;
        }
        hostname = argv1[0];
        number_of_hosts = argc1;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated.
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop through the hostname list.
     */
    i = 0;
    do {
        /*
         * Check for string binding first.  
         */
        is_string_binding = FALSE;
        status = error_status_ok;
        if (hostname != NULL) {
            if (strchr(hostname, ' ' )) {
                /*
                 * Space in the name means might be TCL style string binding.
                 */
                if (dcp_general_split_list(interp, hostname, &argc2, &argv2) != TCL_OK) {
                    return TCL_ERROR;
                }
                hostname = argv2[0];
            }

            rpc_binding_from_string_binding((unsigned_char_t *)hostname, 
                                            &rpc_handle, &status);
            if (status == rpc_s_ok) {
                is_string_binding = TRUE;
                dced_binding_from_rpc_binding(dced_e_service_type_secval, 
                                              rpc_handle, &sv_handle, &status );
                DCP_DHD_CHECK_STATUS_BRK(status);
            }
        }

        if (!is_string_binding) {
            /*
             * Parse the hostnames
             */
            if (dhd__parse_name(interp, NULL, dced_c_service_secval,
                                dced_c_service_secval_len, hostname,
                                &entryname, NULL, NULL) != TCL_OK) {
                DCP_DHD_SET_STATUS(status,  ~error_status_ok);
                break;
            }

            /*
             * Call dced_binding_create() to get binding
             */
            dced_binding_create((dced_string_t)entryname, 0, &sv_handle, &status);
            free(entryname);
            DCP_DHD_CHECK_STATUS_BRK(status);
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
            dced_binding_set_auth_info(sv_handle,
                                       dhd_c_prot_level,
                                       dhd_c_authn_svc, 
                                       (rpc_auth_identity_handle_t)login_context,
                                       dhd_c_authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_secval_stops() to stop secval service on the 
         * specified host.
         */
        dced_secval_stop(sv_handle, &status);

out:
        /*
         * Call dced_binding_free to free the dced binding handle.
         */
        dced_binding_free(sv_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        /*
         * Out of do loop, an error has been found.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    } while ((++i < number_of_hosts) && (hostname = argv1[i]));

    /*
     * Free the memory and return correct status.
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sv_update 
 *
 * OVERVIEW:    Update the pe_site file secval service 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied
 *    arguments.
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
dcp_sv_update(ClientData clientData, 
              Tcl_Interp *interp, 
              int argc, 
              char **argv)
{
    char                   *entryname;
    char                   *hostname;
    int                    argc1, argc2;
    char                   **argv1, **argv2;
    int                    i;
    int                    number_of_hosts;
    dced_binding_handle_t  sv_handle;
    sec_login_handle_t	   login_context;
    boolean32		   authn_user = FALSE;
    error_status_t         status;
    error_status_t         status1;
    rpc_binding_handle_t   rpc_handle;
    boolean32		   is_string_binding;
    signed32		   delay = 0;

    dcp_ArgvInfo arg_table[] = {
        {"-pesite", DCP_ARGV_INT, NULL, NULL,
            dcp_t_sv_update_pe_site_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&delay;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
                      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors.
     */
    if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (argc < 2) {
        hostname = NULL;
        number_of_hosts = 1;
    }
    else {
        /*
         * Set convenience variables.
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

        /*
         * Split the hostname list.
         */
        if (dcp_general_split_list(interp, argv[1], &argc1, &argv1) != TCL_OK) {
            return TCL_ERROR;
        }
        hostname = argv1[0];
        number_of_hosts = argc1;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated.
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop thru the hostname list
     */
    i = 0;
    do {
        /*
         * Check for string binding first.  
         */
        is_string_binding = FALSE;
        status = error_status_ok;
        if (hostname != NULL) {
            if (strchr(hostname, ' ' )) {
                /*
                 * Space in the name means might be TCL style string binding.
                 */
                if (dcp_general_split_list(interp, hostname, &argc2, &argv2) != TCL_OK) {
                    return TCL_ERROR;
                }
                hostname = argv2[0];
            }

            rpc_binding_from_string_binding((unsigned_char_t *)hostname, 
                                            &rpc_handle,
                                            &status );
            if (status == rpc_s_ok) {
                is_string_binding = TRUE;
                dced_binding_from_rpc_binding(dced_e_service_type_secval, 
                                              rpc_handle, &sv_handle, &status );
                DCP_DHD_CHECK_STATUS_BRK(status);
            }
        }

        if (!is_string_binding) {
            /*
             * Parse the hostnames.
             */
            if (dhd__parse_name(interp, NULL, dced_c_service_secval,
                                dced_c_service_secval_len, hostname,
                                &entryname, NULL, NULL) != TCL_OK) {
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }

            /*
             * Call dced_binding_create() to get binding
             */
            dced_binding_create((dced_string_t)entryname, 0, &sv_handle, &status);
            free(entryname);
            DCP_DHD_CHECK_STATUS_BRK(status);
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
            dced_binding_set_auth_info(sv_handle,
                                       dhd_c_prot_level,
                                       dhd_c_authn_svc, 
                                       (rpc_auth_identity_handle_t)login_context,
                                       dhd_c_authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_secval_update_pesite() to starts update_pesite service
 	 * on the specified host.
         */
        dced_secval_update_pesite(sv_handle, delay, &status);

out:
        /*
         * Call dced_binding_free to free the dced binding handle.
         */
        dced_binding_free(sv_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        /*
         * Out of do loop, an error has been found.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    } while ((++i < number_of_hosts) && (hostname = argv1[i]));

    /*
     * Free the memory and return correct status.
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}
