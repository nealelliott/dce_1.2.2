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
 * $Log: dts_ops.c,v $
 * Revision 1.1.8.1  1996/08/09  11:47:55  arvind
 * 	Merge changes.
 * 	[1996/06/11  15:05 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/03/07  16:55 UTC  truitt  /main/HPDCE02/3]
 *
 * 	Merge changes.
 * 	[1996/03/07  16:54 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf66/1]
 *
 * 	CHFts17732: Add support to distinguish between local and global dts servers
 * 	during a 'dts catalog' command.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  14:43 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.2  1996/02/18  19:13:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:14  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  21:21:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/14  16:06 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:05 UTC  truitt
 * 	CHFts14285: When setting error codes, use the right
 * 	macros, to ensure the _e var sets set correctly.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  20:19 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  20:16 UTC  truitt
 * 	Fix dts command switches.
 * 	[1995/12/08  17:55:58  root]
 * 
 * Revision 1.1.4.20  1994/09/26  20:25:37  kevins
 * 	OT 11780 modified clock compare and dts catalog commands.
 * 	[1994/09/26  20:19:43  kevins]
 * 
 * Revision 1.1.4.19  1994/09/26  19:30:52  rousseau
 * 	Ignore output of the deactiavate in dts_stop (CR 12338).
 * 	[1994/09/26  19:30:25  rousseau]
 * 
 * Revision 1.1.4.18  1994/09/23  23:18:48  rousseau
 * 	Internal rebinding changes (CR 12282).
 * 	[1994/09/23  23:18:24  rousseau]
 * 
 * Revision 1.1.4.17  1994/09/01  14:16:52  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:13  rousseau]
 * 
 * Revision 1.1.4.16  1994/08/21  19:45:29  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:44  melman]
 * 
 * Revision 1.1.4.15  1994/08/19  22:15:47  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:15:00  rousseau]
 * 
 * Revision 1.1.4.14  1994/07/28  20:34:46  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:20  rousseau]
 * 
 * Revision 1.1.4.13  1994/07/19  18:09:56  rousseau
 * 	Added 'dts catalog' and 'clock compare' (CR 11249 11251).
 * 	[1994/07/19  18:08:56  rousseau]
 * 
 * Revision 1.1.4.12  1994/06/24  22:07:33  rousseau
 * 	Do a deactivate before stop (CR 10658) and fix RIOS counters problem.
 * 	[1994/06/24  21:53:06  rousseau]
 * 
 * Revision 1.1.4.11  1994/06/23  20:26:27  rousseau
 * 	Fixed casting problems (CR 11000).
 * 	[1994/06/23  20:26:17  rousseau]
 * 
 * Revision 1.1.4.10  1994/06/09  16:02:08  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:40  devsrc]
 * 
 * Revision 1.1.4.9  1994/05/20  20:41:45  rousseau
 * 	Fix core dump (CR 10722).
 * 	[1994/05/20  20:37:38  rousseau]
 * 
 * Revision 1.1.4.8  1994/05/16  20:47:43  rousseau
 * 	Fix build problem (CR 10642).
 * 	[1994/05/16  20:39:10  rousseau]
 * 
 * Revision 1.1.4.7  1994/05/13  22:22:42  melman
 * 	renamed dcecp's dts.h to dcpdts.h
 * 	[1994/05/13  22:18:18  melman]
 * 
 * Revision 1.1.4.6  1994/05/09  20:41:36  rousseau
 * 	Updated to new control interface. Enabled talking to remote daemons (CR 10434).
 * 	[1994/05/09  20:41:19  rousseau]
 * 
 * Revision 1.1.4.5  1994/03/17  21:34:58  rousseau
 * 	Removed overly restrictive check for authentication (CR 10144).
 * 	[1994/03/17  21:34:40  rousseau]
 * 
 * Revision 1.1.4.4  1994/02/10  23:29:12  rousseau
 * 	Help changes for CR 9937.
 * 	[1994/02/10  23:29:03  rousseau]
 * 
 * Revision 1.1.4.3  1994/02/02  19:43:52  rousseau
 * 	Changed to support help functions.
 * 	[1994/02/02  19:40:17  rousseau]
 * 
 * Revision 1.1.4.2  1993/12/27  19:15:48  rousseau
 * 	Removed -epoch option from dts_modify.
 * 	[1993/12/27  19:15:43  rousseau]
 * 
 * Revision 1.1.4.1  1993/12/21  19:29:18  rousseau
 * 	Style guide changes.
 * 	[1993/12/21  19:28:01  rousseau]
 * 
 * Revision 1.1.2.6  1993/12/01  16:57:58  rousseau
 * 	Added dts_modify.
 * 	[1993/12/01  16:57:36  rousseau]
 * 
 * Revision 1.1.2.5  1993/11/19  22:47:23  rousseau
 * 	Added full functionality to the show command.
 * 	[1993/11/19  22:46:51  rousseau]
 * 
 * Revision 1.1.2.4  1993/11/15  00:30:49  rousseau
 * 	Removed obsolete time command.
 * 	Added synchronize command.
 * 	[1993/11/15  00:30:45  rousseau]
 * 
 * Revision 1.1.2.3  1993/11/11  13:57:43  rousseau
 * 	Changed RPC call order in 'show'.
 * 	[1993/11/11  13:57:38  rousseau]
 * 
 * Revision 1.1.2.2  1993/11/09  18:20:13  rousseau
 * 	Added framework for dts show commands.
 * 	[1993/11/09  18:20:07  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/05  15:34:42  rousseau
 * 	Initial version.
 * 	[1993/11/05  15:32:59  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: dts_ops.c
 *
 * DESCRIPTION:
 *   This module contains all of the functions that implement the
 * dcecp dts commands. Helper functions are found in dts_util.c.
 *
 */

/* INCLUDE FILES  */

#include <dcp_dts.h>
#include <dts_proto.h>
#include <util_general.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

/* PROTOTYPES */


/*
 * FUNCTION:    dts_activate
 *
 * OVERVIEW:    Changes a DTS entity to an active state. Change state 
 *              to 'on'. Begin synchronizing with other DTS servers. By 
 *              default, the time will be adjusted gradually. Specifying 
 *              -abruptly will cause the time to be set on the first 
 *              synchronization.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_activate(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    boolean32                   abrupt_flag = FALSE;
    char                        *hostname;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;
    
    dcp_ArgvInfo arg_table[] = {
        {"-abruptly", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_dts_activate_abruptly_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&abrupt_flag;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }
    
    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;

    /* Make the RPC to the server */
    for (;;) {
        EnableCmd(dcp_dts_binding_h, 
                  (signed32 *)&abrupt_flag, 
                  &st_block, 
                  &comm_st);
        if (comm_st != rpc_s_ok) {
            if (retry >= DCP_DTS_MAX_RETRIES) {
                dts_check_status(interp, st_block, comm_st);
                return TCL_ERROR;
            }
            else {
                /* 
                 * Set up for another pass at the RPC, nuke the previous
                 * result and force a rebind.
                 */
                retry++;
                Tcl_ResetResult(interp);
                dcp_dts_valid_binding = FALSE;
                if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                    return TCL_ERROR;
                }
            }
        } 
        else {
            if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                return TCL_ERROR;
            }
            break;
        }
    }

    return TCL_OK;
}


/*
 * FUNCTION:    dts_configure
 *
 * OVERVIEW:    Configures the current DTS server to act as a global server
 *              or not. On success, sets result to 'global' or 'notglobal'.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_configure(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    boolean32                   glob_flag = FALSE, nglob_flag = FALSE;
    char                        *hostname;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;
    
    dcp_ArgvInfo arg_table[] = {
        {"-global", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_dts_configure_global_help},
        {"-notglobal", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_dts_configure_notglobal_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&glob_flag;
    arg_table[1].dst = (char *)&nglob_flag;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Error if both flags set */
    if (glob_flag && nglob_flag) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        return TCL_ERROR;
    }
    
    /* Error if neither flag set */
    if ((!glob_flag) && (!nglob_flag)) {
        DCP_SET_RESULT_CODE(dcp_s_missing_req_opts);
        return TCL_ERROR;
    }
    
    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }
    
    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;
    
    /* Make the RPC to the server */
    if (glob_flag) {
        for (;;) {
            AdvertiseCmd(dcp_dts_binding_h, &st_block, &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, 
                                               hostname) == TCL_ERROR) {
                        return TCL_ERROR;
                    }
                }
            } 
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    return TCL_ERROR;
                }
                break;
            }
        }
    }
    else {
        for (;;) {
            UnadvertiseCmd(dcp_dts_binding_h, &st_block, &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, 
                                               hostname) == TCL_ERROR) {
                        return TCL_ERROR;
                    }
                }
            } 
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    return TCL_ERROR;
                }
                break;
            }
        }
    }

    if (glob_flag) {
        Tcl_SetResult(interp, "global", TCL_STATIC);
    }
    else {
        Tcl_SetResult(interp, "notglobal", TCL_STATIC);
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_catalog
 *
 * OVERVIEW:    A wrapper around a dcecp script the returns a list of all
 *              dts servers in the current (or specified) cell.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_catalog(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char  cmd[1024];

    dcp_ArgvInfo arg_table[] = {
        {"-global", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_dts_catalog_global_help},
        {"-simplename", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_dts_catalog_simplename_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };

    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    sprintf(cmd, "_dcp_dts_catalog %s", Tcl_Merge(argc, argv));
    return (Tcl_Eval(interp, cmd));
}


/*
 * FUNCTION:    dts_deactivate
 *
 * OVERVIEW:    Changes a DTS entity to an inactive state. Change state 
 *              to 'off'. Stop synchronizing with other DTS servers.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_deactivate(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *hostname = NULL;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }
    
    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;
    
    /* Make the RPC to the server */
    for (;;) {
        DisableCmd(dcp_dts_binding_h, &st_block, &comm_st);
        if (comm_st != rpc_s_ok) {
            if (retry >= DCP_DTS_MAX_RETRIES) {
                dts_check_status(interp, st_block, comm_st);
                return TCL_ERROR;
            }
            else {
                /* 
                 * Set up for another pass at the RPC, nuke the previous
                 * result and force a rebind.
                 */
                retry++;
                Tcl_ResetResult(interp);
                dcp_dts_valid_binding = FALSE;
                if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                    return TCL_ERROR;
                }
            }
        } 
        else {
            if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                return TCL_ERROR;
            }
            break;
        }
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_modify
 *
 * OVERVIEW:    Allows an attribute on the current DTS entity to be changed.
 *              The following options to change individual attributes are
 *              recognized:
 *              -globaltimeout
 *              -localtimeout
 *              -maxinaccuracy
 *              -minservers
 *              -queryattempts
 *              -syncinterval
 *              -tolerance
 *              -actcourierrole
 *              -checkinterval
 *              -courierrole
 *              -serverentry
 *              -servergroup
 *              
 *              Attributes can also be changed by the -change option.
 *              This option takes a TCL list of attribute-value pairs.
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_modify(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *attr_list = NULL;
    boolean32                   found_opt = FALSE;
    char                        *globaltimeout = NULL;
    char                        *localtimeout = NULL;
    char                        *maxinaccuracy = NULL;
    int                         *minservers = 0;
    int                         *queryattempts = 0;
    char                        *syncinterval = NULL;
    char                        *tolerance = NULL;
    char                        *checkinterval = NULL;
    char                        *courierrole = NULL;
    char                        *serverentry = NULL;
    char                        *servergroup = NULL;
    CtlSetData                  user_buffer;
    CtlSetAttr_t                set_attribute;
    char                        *hostname;
    int                         i;
    int                         attrc, attr_listc;
    char                        **attrv, **attr_listv;
    char                        *end_ptr, **tmp;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;
    int                         attr_index;
    int                         st;

    /* 
     * The order of the attributes in the following table and in the
     * dcp_ArgvInfo table is important. Because we need to check to
     * see if each option was specified, we just search down the
     * dts_attributes array. We depend on the fact that we find the
     * "minservers" (for example) in the same place in both tables. If you
     * need to modify the list of user modifyable attributes, make sure 
     * that both tables stay aligned.
     */
    dcp_dts_attributes_t dts_attributes[] = {
        {"checkinterval",  K_CHECK_INTERVAL_ATTR,      K_TYPE_BINRELTIME},
        {"courierrole",    K_COURIER_ROLE_ATTR,        K_TYPE_COURIER_ROLE},
        {"globaltimeout",  K_WAN_TIMEOUT_ATTR,         K_TYPE_BINRELTIME},
        {"localtimeout",   K_LAN_TIMEOUT_ATTR,         K_TYPE_BINRELTIME},
        {"maxinaccuracy",  K_MAXIMUM_INACCURACY_ATTR,  K_TYPE_BINRELTIME},
        {"minservers",     K_SERVERS_REQUIRED_ATTR,    K_TYPE_INTEGER32},
        {"queryattempts",  K_QUERY_ATTEMPTS_ATTR,      K_TYPE_INTEGER32},
        {"serverentry",    K_SERVER_ENTRY_NAME,        K_TYPE_STRING},
        {"servergroup",    K_SERVER_GROUP_NAME,        K_TYPE_STRING},
        {"syncinterval",   K_SYNCH_ATTR,               K_TYPE_BINRELTIME},
        {"tolerance",      K_ERROR_TOLLERANCE_ATTR,    K_TYPE_BINRELTIME}
    };

    /* This table is used for looking up attribute names and abbrevs */
    char *dts_attr_names[] = {
        "checkinterval",
        "courierrole",
        "globaltimeout",
        "localtimeout",
        "maxinaccuracy",
        "minservers",
        "queryattempts",
        "serverentry",
        "servergroup",
        "syncinterval",
        "tolerance"
    };

    dcp_ArgvInfo arg_table[] = {
        {"-checkinterval", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_checkinterval_help},
        {"-courierrole", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_courierrole_help},
        {"-globaltimeout", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_globaltimeout_help},
        {"-localtimeout", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_localtimeout_help},
        {"-maxinaccuracy", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_maxinaccuracy_help},
        {"-minservers", DCP_ARGV_INT, NULL, NULL,
             dcp_t_dts_modify_minservers_help},
        {"-queryattempts", DCP_ARGV_INT, NULL, NULL,
             dcp_t_dts_modify_queryattempts_help},
        {"-serverentry", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_serverentry_help},
        {"-servergroup", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_servergroup_help},
        {"-syncinterval", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_syncinterval_help},
        {"-tolerance", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_dts_modify_tolerance_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL, 
             dcp_t_dts_modify_change_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&checkinterval;
    arg_table[1].dst = (char *)&courierrole;
    arg_table[2].dst = (char *)&globaltimeout;
    arg_table[3].dst = (char *)&localtimeout;
    arg_table[4].dst = (char *)&maxinaccuracy;
    arg_table[5].dst = (char *)&minservers;
    arg_table[6].dst = (char *)&queryattempts;
    arg_table[7].dst = (char *)&serverentry;
    arg_table[8].dst = (char *)&servergroup;
    arg_table[9].dst = (char *)&syncinterval;
    arg_table[10].dst = (char *)&tolerance;
    arg_table[11].dst = (char *)&attr_list;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* 
     * Search through the arg_table to see if any mutually exclusive
     * options were specified. 
     *
     * If -change was not specified, don't bother with the rest. We
     * should always find -change at the end of the table (ie.
     * arg_table[DTS_NUM_RW_ATTR]).
     *
     * We can use a string compare on the first test because we know 
     * that -change is a string. We can't make that assumption later on 
     * (CR 11000).
     */
    if (strcmp(arg_table[DTS_NUM_RW_ATTR].dst, "") != 0) {
        for (i = 0; i < DTS_NUM_RW_ATTR; i++) {
            switch (arg_table[i].type) {
              case DCP_ARGV_INT:
                if (*((int *)arg_table[i].dst) != 0) {
                    DCP_SET_RESULT_CODE(dcp_s_dts_modify_excl_opts);
                    return TCL_ERROR;
                }
                break;
              case DCP_ARGV_STRING:
                if (strcmp(arg_table[i].dst, "") != 0) {
                    DCP_SET_RESULT_CODE(dcp_s_dts_modify_excl_opts);
                    return TCL_ERROR;
                }
                break;
            }
        }
    }

    /*
     * Work on the attribute list. Plug information into the arg_table[]
     * structure, and let the individual option code do the dirty work.
     * This also allows the most amount of input checking before we 
     * start making RPC's.
     */
    if (attr_list != NULL) {
        /* 
         * If we found some option. Used to check if no options were 
         * specified.
         */
        found_opt = TRUE;
        
        Tcl_SplitList(interp, attr_list, &attr_listc, &attr_listv);
        for (i = 0; i < attr_listc; i++) {
            Tcl_SplitList(interp, attr_listv[i], &attrc, &attrv);

            /* 
             * If we are passed a single two-element list, the second 
             * SplitList was a no-op. We need to check for this case
             * and modify our working variables accordingly.
             */
            if (strcmp(attrv[0], attr_listv[0]) == 0) {
                attrc = attr_listc;
                tmp = attrv;
                attrv = attr_listv;
                attr_listv = tmp;

                /* Change our loop index so we don't run through here again. */
                i = attr_listc;
            }

            if (attrc < 2) {
                DCP_SET_RESULT_CODE(dcp_s_too_few_list_elements);
                free((char *)attr_listv);
                free((char *)attrv);
                return TCL_ERROR;
            }

            if (attrc > 2) {
                DCP_SET_RESULT_CODE(dcp_s_too_many_list_elements);
                free((char *)attr_listv);
                free((char *)attrv);
                return TCL_ERROR;
            }

            if (dcp_general_find_attr_index(interp,
                                            attrv[0],
                                            DTS_NUM_RW_ATTR,
                                            dts_attr_names,
                                            &attr_index) != TCL_OK) {
                free((char *)attr_listv);
                free((char *)attrv);
                return TCL_ERROR;
            }

            switch (dts_attributes[attr_index].type) {
              case K_TYPE_INTEGER32:
                *((int *)arg_table[attr_index].dst) = strtol(attrv[1], 
                                                    &end_ptr, 
                                                    0);
                break;
              case K_TYPE_BINRELTIME:
              case K_TYPE_COURIER_ROLE:
              case K_TYPE_STRING:
                *((char **)arg_table[attr_index].dst) = attrv[1];
                break;
            }
        }

        /* Clean up */
        free((char *)attr_listv);
        free((char *)attrv);
    }

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }
    
    /* Work (and act) on the individual options. */
    for (i = 0; i < DTS_NUM_RW_ATTR; i++) {
        
        /* Check to see which options were specified */
        switch (arg_table[i].type) {
          case DCP_ARGV_INT:
            if (*((int *)arg_table[i].dst) == 0) {
                continue;
            }
            break;
          case DCP_ARGV_STRING:
            if (strcmp(arg_table[i].dst, "") == 0) {
                continue;
            }
            break;
        }
        
        /* 
         * If we found some option. Used to check if no options were 
         * specified.
         */
        found_opt = TRUE;
        
        /* Get the type of the attribute */
        dts_get_attr_type(dts_attributes[i].value, 
                          &set_attribute.attrValue.Data.type);
        set_attribute.setAttr.useDefault = FALSE;
        set_attribute.setAttr.attribute = dts_attributes[i].value;
        
        /* 
         * Set up the rest of the data structure for the RPC call
         * depending on attribute type. 
         */
        switch (dts_attributes[i].type) {
          case K_TYPE_BINRELTIME:
            st = utc_mkascreltime(&user_buffer.setData.userBinTime,
                                  *(char **)arg_table[i].dst);
            if (st != 0) {
                DCP_SET_RESULT_CODE(dcp_s_dts_time_conversion_fail);
                return TCL_ERROR;
            }
            
            set_attribute.attrValue.attrSize = sizeof(utc_t);
            break;
          case K_TYPE_COURIER_ROLE:
            if (strncmp(*((char **)arg_table[i].dst), 
                        "courier",
                        strlen(*((char **)arg_table[i].dst))) == 0) {
                user_buffer.setData.userCourierRole = 0;
            }
            else if (strncmp(*((char **)arg_table[i].dst), 
                             "noncourier",
                             strlen(*((char **)arg_table[i].dst))) == 0) {
                user_buffer.setData.userCourierRole = 1;
            }
            else if (strncmp(*((char **)arg_table[i].dst), 
                            "backup",
                            strlen(*((char **)arg_table[i].dst))) == 0) {
                user_buffer.setData.userCourierRole = 2;
            }
            else {
                DCP_SET_RESULT_CODE(dcp_s_dts_modify_invalid_courier_role);
                return TCL_ERROR;
            }
            
            set_attribute.attrValue.attrSize = 
                sizeof(user_buffer.setData.userCourierRole);
            break;
          case K_TYPE_INTEGER32:
            user_buffer.setData.userDecimal = *((int *)arg_table[i].dst);
            set_attribute.attrValue.attrSize = 
                sizeof(user_buffer.setData.userDecimal);
            break;
          case K_TYPE_STRING:
            set_attribute.attrValue.attrSize = 
                strlen(*(char **)arg_table[i].dst) + 1;
            memcpy((void *)user_buffer.setData.userString.buffer,
                   *(char **)arg_table[i].dst,
                   (set_attribute.attrValue.attrSize - 1));
            user_buffer.setData.userString.buffer
                [set_attribute.attrValue.attrSize - 1] = '\0';
            break;
        }
        
        /*
         * Attach the just-built user_buffer structure to the 
         * set_attribute structure for the RPC call.
         */
        set_attribute.attrValue.Data.setData = user_buffer.setData;
        
        /* Initialize the status block */
        st_block.status = 0;
        st_block.sysStatus = 0;
        retry = 0;

        /* Make the RPC call to the server */
        for (;;) {
            SetAttrCmd(dcp_dts_binding_h, &set_attribute, &st_block, &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, 
                                               hostname) == TCL_ERROR) {
                        return TCL_ERROR;
                    }
                }
            } 
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    return TCL_ERROR;
                }
                break;
            }
        }
    }

    if (!found_opt) {
        DCP_SET_RESULT_CODE(dcp_s_need_opt);
        return TCL_ERROR;
    }

    return TCL_OK;
}


/*
 * FUNCTION:    dts_show
 *
 * OVERVIEW:    Creates a TCL list of the attributes and their values
 *              from the current DTS entity and places it in the result.
 * 
 *              The -attributes option will return the list of attributes
 *               and their values. This is the default.
 *
 *              The -counters option will return the list of counters and 
 *              their values.
 * 
 *              The -all option will return both counter and attribute 
 *              information.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_show(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    boolean32                   count_flag = FALSE, attr_flag = FALSE;
    boolean32                   all_flag = FALSE;
    char                        *hostname;
    CtlShowAllCntrs             all_counters;
    CtlShowAllChar              all_attributes;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;
    
    dcp_ArgvInfo arg_table[] = {
        {"-all", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_dts_show_all_help},
        {"-attributes", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_dts_show_attributes_help},
        {"-counters", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_dts_show_counters_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&all_flag;
    arg_table[1].dst = (char *)&attr_flag;
    arg_table[2].dst = (char *)&count_flag;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Default to -attributes */
    if (!count_flag && !attr_flag && !all_flag) {
        attr_flag = TRUE;
    }

    /* For -all set the other two */
    if (all_flag) {
        count_flag = TRUE; 
        attr_flag = TRUE;
    }

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }
    
    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;

    /* Make the RPC to the server to get the list of attributes. */
    if (attr_flag) {
        for (;;) {
            ShowAllCharsCmd(dcp_dts_binding_h, 
                            &all_attributes, 
                            &st_block, 
                            &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, 
                                               hostname) == TCL_ERROR) {
                        return TCL_ERROR;
                    }
                }
            } 
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    return TCL_ERROR;
                }
                break;
            }
        }
    }

    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;

    /* Make the RPC to the server to get the list of counters. */
    if (count_flag) {
        for (;;) {
            ShowAllCntrsCmd(dcp_dts_binding_h, 
                            &all_counters, 
                            &st_block, 
                            &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, 
                                               hostname) == TCL_ERROR) {
                        return TCL_ERROR;
                    }
                }
            } 
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    return TCL_ERROR;
                }
                break;
            }
        }
    }

    /* Show the attributes */
    if (attr_flag) {
        /* Show the main list of attributes, the status and UID attributes */
        if (dts_show_all_attr(interp, &all_attributes) != TCL_OK) {
            return TCL_ERROR;
        }

        if (dts_show_status(interp) != TCL_OK) {
            return TCL_ERROR;
        }

        /* Only look up the UUID if we are talking to a server */
        if (all_attributes.variant == 0) {
            if (dts_show_uuid(interp) != TCL_OK) {
                return TCL_ERROR;
            }
        }
        if (dts_show_global_servers(interp) != TCL_OK) {
            return TCL_ERROR;
        }
        if (dts_show_local_servers(interp) != TCL_OK) {
            return TCL_ERROR;
        }
    }        

    /* Show the counters */
    if (count_flag) {
        dts_show_all_counters(interp, &all_counters);
    }

    return TCL_OK;
}


/*
 * FUNCTION:    dts_stop
 *
 * OVERVIEW:    Stops the current DTS entity.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_stop(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *hostname;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;

    /* 
     * Send the RPC to have the server disable itself. Ignore any
     * returned info.
     */
    for (;;) {
        DisableCmd(dcp_dts_binding_h, &st_block, &comm_st);
        if (comm_st != rpc_s_ok) {
            if (retry >= DCP_DTS_MAX_RETRIES) {
                /* Only do a check_status if a comm failure occurs */
                dts_check_status(interp, st_block, comm_st);
                return TCL_ERROR;
            }
            else {
                /* 
                 * Set up for another pass at the RPC, nuke the previous
                 * result and force a rebind.
                 */
                retry++;
                Tcl_ResetResult(interp);
                dcp_dts_valid_binding = FALSE;
                if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                    return TCL_ERROR;
                }
            }
        }
        else {
            /* 
             * Do not do the customary dts_check_status here. We don't care
             * if the operation whines about already being disabled.
             */
            break;
        }
    }

    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;

    /* Send the RPC to have the server stop */
    for (;;) {
        DeleteCmd(dcp_dts_binding_h, &st_block, &comm_st);
        if (comm_st != rpc_s_ok) {
            if (retry >= DCP_DTS_MAX_RETRIES) {
                dts_check_status(interp, st_block, comm_st);
                return TCL_ERROR;
            }
            else {
                /* 
                 * Set up for another pass at the RPC, nuke the previous
                 * result and force a rebind.
                 */
                retry++;
                Tcl_ResetResult(interp);
                dcp_dts_valid_binding = FALSE;
                if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                    return TCL_ERROR;
                }
            }
        } 
        else {
            if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                return TCL_ERROR;
            }
            break;
        }
    }

    return TCL_OK;
}


/*
 * FUNCTION:    dts_synchronize
 *
 * OVERVIEW:    Causes the current DTS entity to synchronize with DTS 
 *              servers. The machine's clock is adjusted accordingly. 
 *              By default, the time will be adjusted gradually. Specifying
 *              -abruptly will cause the time to be synchronized immediately.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
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
dts_synchronize(ClientData clientData, Tcl_Interp *interp, 
                int argc, char **argv)
{
    boolean32                   abr_flag = FALSE;
    char                        *hostname;
    int                         retry;
    StatusBlk_t                 st_block;
    error_status_t              comm_st;

    dcp_ArgvInfo arg_table[] = {
        {"-abruptly", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_dts_sync_abruptly_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&abr_flag;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /* Initialize rpc communications with server. */
    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    retry = 0;

    /* Send the RPC to have the server synchronize */
    for (;;) {
        SynchronizeCmd(dcp_dts_binding_h, 
                       (signed32 *)&abr_flag, 
                       &st_block, 
                       &comm_st);
        if (comm_st != rpc_s_ok) {
            if (retry >= DCP_DTS_MAX_RETRIES) {
                dts_check_status(interp, st_block, comm_st);
                return TCL_ERROR;
            }
            else {
                /* 
                 * Set up for another pass at the RPC, nuke the previous
                 * result and force a rebind.
                 */
                retry++;
                Tcl_ResetResult(interp);
                dcp_dts_valid_binding = FALSE;
                if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                    return TCL_ERROR;
                }
            }
        } 
        else {
            if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                return TCL_ERROR;
            }
            break;
        }
    }

    return TCL_OK;
}
