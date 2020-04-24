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
 * $Log: dhd_server.c,v $
 * Revision 1.1.6.1  1996/08/09  11:47:41  arvind
 * 	Merge changes.
 * 	[1996/06/11  14:31 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/18  17:42 UTC  truitt  /main/HPDCE02/12]
 *
 * 	Merge changes.
 * 	[1996/03/18  17:41 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1995/11/13  14:37 UTC  truitt  /main/HPDCE02/11]
 *
 * 	Merge fix of build warning.
 * 	[1995/11/13  14:35 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts51/1]
 *
 * 	CHFts16678: Fix build warning.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  14:04 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:13:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:07  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:20:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/10  1995/10/19  17:03 UTC  truitt
 * 	Merge local work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts50/1  1995/10/19  17:00 UTC  truitt
 * 	CHFts16529: Correct order of server options.  Fix some malloc's and free's.
 * 
 * 	HP revision /main/HPDCE02/9  1995/10/17  20:31 UTC  nav
 * 	merge from the private branch.
 * 
 * 	HP revision /main/HPDCE02/nav_dcecp_ping/3  1995/10/17  19:19 UTC  nav
 * 	make server ping more reliable
 * 
 * 	HP revision /main/HPDCE02/nav_dcecp_ping/2  1995/10/16  20:11 UTC  nav
 * 	add help text for -timeout option added to server ping.
 * 
 * 	HP revision /main/HPDCE02/nav_dcecp_ping/1  1995/10/16  20:09 UTC  nav
 * 	add an option, -timeout, to server ping to make "server ping" more reliable when the user
 * 	intends to.
 * 
 * 	HP revision /main/HPDCE02/8  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:51 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/7  1995/04/28  17:09 UTC  nav
 * 	merge from the private branch
 * 
 * 	HP revision /main/HPDCE02/nav_mothra_2/1  1995/04/20  15:02 UTC  nav
 * 	add object uuid support to server ping
 * 
 * 	HP revision /main/HPDCE02/6  1995/04/05  20:17 UTC  truitt
 * 	Merge server modify work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  20:16 UTC  truitt
 * 	CHFts14295: A 'server modify -add' test is core dumping
 * 	because the variable attr_count is not being set correctly.
 * 
 * 	HP revision /main/HPDCE02/5  1995/02/21  16:24 UTC  truitt
 * 	Merge help message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts14/1  1995/02/21  16:23 UTC  truitt
 * 	CHFts14293: Corrected two more server help messages.
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/31  19:46 UTC  truitt
 * 	Merge message change.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts06/1  1995/01/31  19:45 UTC  truitt
 * 	Corrected the message for the server modify change
 * 	command.
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/18  16:01 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts01/1  1995/01/18  16:00 UTC  truitt
 * 	CHFts14059: The server ping command, when given multiple
 * 	host names as input, formats the output incorrectly.
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/11  18:25 UTC  nav
 * 	merge from private branch
 * 
 * 	HP revision /main/HPDCE02/nav_mothra/1  1995/01/11  18:22 UTC  nav
 * 	added interface id server ping
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/19  18:18 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chk_02/1  1994/12/19  18:15 UTC  truitt
 * 	Changed footprint for modify_era_info() call to
 * 	identify which schema (security or generic) the
 * 	function is to operate on.  Also fixed an 'if'
 * 	statement in the modify function to do the
 * 	comparison correctly.
 * 	[1995/12/08  17:55:48  root]
 * 
 * Revision 1.1.2.23  1994/10/04  16:11:03  pwang
 * 	Treat runtime exceptional errors as server unreachable
 * 	[OT#12478]
 * 
 * Revision 1.1.2.22  1994/09/22  21:31:12  pwang
 * 	Accepted tcl binding in ping cmd [OT#12240]
 * 	[1994/09/22  21:30:54  pwang]
 * 
 * Revision 1.1.2.21  1994/09/09  20:29:36  pwang
 * 	Removed -era option from server create [OT#12104]
 * 	[1994/09/09  20:10:49  pwang]
 * 
 * Revision 1.1.2.20  1994/09/01  20:09:55  pwang
 * 	Changed dcp_sr_modify to match with the changes in
 * 	modify_era_info() and fixed compiler warnings [OT#11970]
 * 	[1994/09/01  20:04:16  pwang]
 * 
 * Revision 1.1.2.19  1994/08/31  12:47:22  truitt
 * 	Added a couple fake variables in the modify_era_info()
 * 	call, so it would compile using the new prototype.
 * 	This must be changed.
 * 	[1994/08/31  12:45:16  truitt]
 * 
 * Revision 1.1.2.18  1994/08/25  20:45:15  pwang
 * 	Eched the option(s) which is not entered [OT#11710]
 * 	[1994/08/25  20:43:22  pwang]
 * 
 * Revision 1.1.2.17  1994/08/24  21:01:51  pwang
 * 	Added new entryname attr to server [OT #11398]
 * 	[1994/08/24  20:56:12  pwang]
 * 
 * Revision 1.1.2.16  1994/08/22  19:11:46  pwang
 * 	Made server modify cmd same as one in rgy [OT#11424]
 * 	[1994/08/22  18:49:28  pwang]
 * 
 * Revision 1.1.2.15  1994/08/17  15:37:07  melman
 * 	added 4th param to sec_att_util_alloc_copy
 * 	[1994/08/17  15:36:48  melman]
 * 
 * Revision 1.1.2.14  1994/08/08  18:51:47  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:16  melman]
 * 
 * Revision 1.1.2.13  1994/07/25  21:10:41  pwang
 * 	Added misssed parentheses to the size arguemnt of a malloc
 * 	macro [OT#11418]
 * 	[1994/07/25  21:06:25  pwang]
 * 
 * Revision 1.1.2.12  1994/07/22  21:40:47  pwang
 * 	Replaced intermediate attr schema with dce_attr
 * 	schema interface [OT#10262]
 * 	[1994/07/22  19:43:11  pwang]
 * 
 * Revision 1.1.2.11  1994/07/08  21:18:10  pwang
 * 	Allowed unauthenticated access to dced [OT#11214]
 * 	[1994/07/08  21:15:46  pwang]
 * 
 * Revision 1.1.2.10  1994/07/05  16:30:09  pwang
 * 	Added missed options, properly released binding of attr_schema
 * 	and corrected mis-useed attributes [OT#11072, #11058, #11020]
 * 	[1994/07/05  16:24:22  pwang]
 * 
 * Revision 1.1.2.9  1994/06/22  16:08:25  pwang
 * 	Used attr_schema of dced and Era routines of rgy_util.c
 * 	and checked the return of dced_binding_create() properly
 * 	and implemented -add and -remove switches for server
 * 	modify command and added newline between output of
 * 	different objects [OT# 10464 10948 10262 10928]
 * 	[1994/06/22  16:02:11  pwang]
 * 
 * Revision 1.1.2.8  1994/06/09  16:02:05  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:35  devsrc]
 * 
 * Revision 1.1.2.7  1994/06/07  20:04:29  pwang
 * 	Called dced_inq_id in dcp_sr_show and Used nil uuid
 * 	if the uuid is not given at command line in
 * 	dcp_sr_start() (OT# 10846)
 * 	[1994/06/07  20:02:27  pwang]
 * 
 * Revision 1.1.2.6  1994/05/31  21:10:01  pwang
 * 	Checked rpc_s_entry_not_found in binding import loop and
 * 	set minimal timeout on binding while pinging a server.
 * 	[1994/05/31  20:59:46  pwang]
 * 
 * Revision 1.1.2.5  1994/05/18  21:57:34  pwang
 * 	Updated to changes in RFC and added attributes options/swiches
 * 	add added -simplename (fullname) support and added -uuid to
 * 	server start and adjusted ERA printout, etc.
 * 	[1994/05/18  21:34:12  pwang]
 * 
 * Revision 1.1.2.4  1994/05/06  14:21:42  pwang
 * 	Changed the status code dced_s_no_more_entry to
 * 	dced_s_no_more_entries [OT#10554]
 * 	[1994/05/06  14:20:52  pwang]
 * 
 * Revision 1.1.2.3  1994/05/05  14:56:58  pwang
 * 	Used DCP_ARGV_NO_OPT_LEFTOVERS flag to detect illegal options.
 * 	Made corresponding name changes due to the changes in dced.
 * 	[1994/05/05  14:53:30  pwang]
 * 
 * Revision 1.1.2.2  1994/04/26  20:03:55  pwang
 * 	Used sr as server's abbreviated name instead of sv.
 * 	[1994/04/26  19:40:13  pwang]
 * 
 * Revision 1.1.2.1  1994/04/22  19:59:29  pwang
 * 	Initial Implementation
 * 	[1994/04/22  19:52:09  pwang]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * dhd_server.c
 *
 * DESCRIPTION:
 * This module contains all the dced server (srvrconf, srvrexec)
 * operations to access and control server related objects (or daemons)
 * on a specified host thru dcecp.
 */

/*
 * INCLUDE FILES
 */
#include <dhd.h>
#include <util_binding.h>
#include <dce/rs_attr_schema.h>

extern rs_attr_schema_v0_0_epv_t	dced_rs_attr_schema_v0_0_epv;

/*
 * DECLARATIONS 
 */
#define UDP_PROTSEQ    "ncadg_ip_udp"
#define RPCD_ENDPOINT  "135"

/*
 * INTERNAL VARIABLES 
 */
static server_t                   s_null_server;
static dce_attr_bind_auth_info_t  s_null_auth_info;


/*
 * FUNCTION:    dcp_sr_catalog 
 *
 * OVERVIEW:    Returns a list of the names of all server (srvrconf or
 *		srvrexec) objects on the specified host.
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
dcp_sr_catalog(ClientData clientData, 
               Tcl_Interp *interp, 
               int        argc, 
               char       **argv)
{
    char                    *entryname, *hostname;
    boolean32		    srvrexec = FALSE;
    int                     argc1;
    char                    **argv1;
    boolean32		    simplename = FALSE;
    boolean32		    local_only = FALSE;
    char		    *fullname;
    int                     i, rt, number_of_hosts;
    dced_binding_handle_t   sr_handle;
    sec_login_handle_t	    login_context;
    boolean32		    authn_user = FALSE;
    dced_cursor_t           sr_cursor;
    dced_entry_t            *entry_p;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-executing", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_executing_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_local_help},
        {"-simplename", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_simplename_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&srvrexec;
    arg_table[1].dst = (char *)&local_only;
    arg_table[2].dst = (char *)&simplename;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

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
         * Set convenience variables
         */
        DCP_GENERAL_SET_NAMELIST(argv[1]);

        /*
         * Split the hostname list 
         */
        if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
            return TCL_ERROR;
        }
        hostname = argv1[0];
        number_of_hosts = argc1;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop thr the hostname list
     */
    for (i = 0; i < number_of_hosts; i++) {
        if (i) {
            Tcl_AppendResult(interp, "\n", (char *) NULL);
            hostname = argv1[i];
        }

        /*
         * Parse the hostnames
         */
        if (srvrexec) {
            rt = dhd__parse_name(interp, NULL, dced_c_service_srvrexec,
                                 dced_c_service_srvrexec_len, hostname,
                                 &entryname, NULL, 
                                 (simplename) ? NULL : &fullname);
        }
        else {
            rt = dhd__parse_name(interp, NULL, dced_c_service_srvrconf,
                                 dced_c_service_srvrconf_len, hostname,
                                 &entryname, NULL, 
                                 (simplename) ? NULL : &fullname);
        }
        if (rt != TCL_OK) {
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
            break;
        }

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t)entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &sr_handle,
                            &status);
        free(entryname);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            if (!simplename) {
                free(fullname);
            }
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
            dced_binding_set_auth_info(sr_handle,
                                       dhd_c_prot_level,
                                       dhd_c_authn_svc, 
                                       (rpc_auth_identity_handle_t)login_context,
                                       dhd_c_authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_initialize_cursor() to get the cursor of 
         * the object list
         */
        dced_initialize_cursor(sr_handle, &sr_cursor, &status); 
        DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_entry_get_next() to traverse the object list
         */
        for ( ; ; ) {
            dced_entry_get_next(sr_cursor, &entry_p, &status);
            if (DCP_DHD_GOOD_STATUS(status)) {
                /*
                 * Display the object list
                 */
                dhd__show_entry(interp, 
                                entry_p, 
                                (simplename) ? NULL : fullname, 
                                dhd_e_show_name); 
            }
            else if (DCP_DHD_STATUS_EQUAL(status, dced_s_no_more_entries)) {
                /*
                 * Out of for ( ; ; ) loop, no more to read.
                 */
                DCP_DHD_RESET_STATUS(status);
                break;
            }
            else {
                /*
                 * an error has been seen
                 */
                break;
            }
        }

        /*
         * Call dced_release_cursor() to release the cursor
         */
        dced_release_cursor(&sr_cursor, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

out:
        /*
         * Call dced_binding_free to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        if (!simplename) {
            free(fullname);
        }

        /*
         * Out of do loop, an error has been met.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free the memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sr_create 
 *
 * OVERVIEW:    Create a hostdata entry and the data associated
 *              with it on the specified host.
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
 *    UUID of the created object 
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_create(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char                           *arguments = NULL, *attribute = NULL,
                                   *directory = NULL, *entry_name = NULL,
                                   *era_attr = NULL,  *gid = NULL, 
                                   *keytabs = NULL,   *prerequisites = NULL, 
                                   *program = NULL,   *principals = NULL, 
                                   *services = NULL,  *starton = NULL, 
                                   *uid = NULL,       *uuid = NULL;
    char                           *name, *entryname;
    dced_binding_handle_t          sr_handle;
    sec_login_handle_t             login_context;
    extern dce_attr_sch_handle_t   attr_schema_binding;
    dce_attr_component_name_t      schema_name;
    dce_attr_bind_auth_info_t      auth_info;
    server_t                       dced_server;
    int                            argc1;
    char                           **argv1;
    int                            i;
    error_status_t                 status, status1;
    boolean32			   local_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-arguments", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_arguments_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_attribute_help},
        {"-directory", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_directory_help},
        {"-entryname", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_sr_entryname_help},
        {"-gid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_gid_help},
        {"-keytabs", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_keytabs_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_local_help},
        {"-prerequisites", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_prerequisites_help},
        {"-principals", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_principals_help},
        {"-program", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_program_help},
        {"-services", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_services_help},
        {"-starton", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_sr_starton_help},
        {"-uid", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_sr_uid_help},
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_sr_uuid_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&arguments;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&directory;
    arg_table[3].dst = (char *)&entry_name;
    arg_table[4].dst = (char *)&gid;
    arg_table[5].dst = (char *)&keytabs;
    arg_table[6].dst = (char *)&local_only;
    arg_table[7].dst = (char *)&prerequisites;
    arg_table[8].dst = (char *)&principals;
    arg_table[9].dst = (char *)&program;
    arg_table[10].dst = (char *)&services;
    arg_table[11].dst = (char *)&starton;
    arg_table[12].dst = (char *)&uid; 
    arg_table[13].dst = (char *)&uuid; 

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if ((attribute) &&
        ((arguments) || (directory) || (entry_name) || (gid) || 
         (keytabs) || (program) || (prerequisites) || (principals) || 
         (services) || (starton) || (uid) || (uuid))) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_too_many_opts);
    }

    /*
     * Parse the attributes (into server object)
     */
    if (attribute) {
        if (dhd__parse_server(interp, attribute, &dced_server, &era_attr) != TCL_OK) {
            return TCL_ERROR;
        }
    }
    else {
        dced_server = s_null_server;

        /* 
         * Get server arguments
         */
        if (arguments) {
            if (Tcl_SplitList(interp, arguments, &argc1, &argv1) != TCL_OK) {
                return TCL_ERROR;
            }
            DCP_DHD_MALLOC_LIST_RESET(&dced_server.fixed.arguments,
                                      dced_string_t,
                                      argc1,
                                      status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                DCP_DHD_ARGV_FREE(argc1, argv1);
                return TCL_ERROR;
            }
            for (i=0; i<argc1; i++)  {
                dced_server.fixed.arguments.list[i] = (dced_string_t) dce_strdup(argv1[i]);
            }
            DCP_DHD_ARGV_FREE(argc1, argv1);
        }

        /*
         * Get thw working directory
         */
        if (directory) {
            dced_server.fixed.posix_dir = (dced_string_t) dce_strdup(directory);
        }

        /*
         * Get server gid
         */
        if (gid) {
            dced_server.fixed.posix_gid = atol(gid);
        }

        /*
         * Get server NS entryname 
         */
        if (entry_name) {
            dced_server.entryname = (dced_string_t) dce_strdup(entry_name);
        }
        /*
         * Get keytables
         */

        if (keytabs) {
            if (Tcl_SplitList(interp, keytabs, &argc1, &argv1) != TCL_OK) {
                dced_server_free_ptrs(&dced_server);
                return TCL_ERROR;
            }
            DCP_DHD_MALLOC_LIST_RESET(&dced_server.fixed.keytables,
                                      uuid_t,
                                      argc1,
                                      status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                DCP_DHD_ARGV_FREE(argc1, argv1);
                dced_server_free_ptrs(&dced_server);
                return TCL_ERROR;
            }
            for (i=0; i<argc1; i++)  {
                uuid_from_string((unsigned char *) argv1[i],
                                  &dced_server.fixed.keytables.list[i],
                                  &status);
                if (!DCP_DHD_GOOD_STATUS(status)) {
                    DCP_DHD_ARGV_FREE(argc1, argv1);
                    dced_server_free_ptrs(&dced_server);
                    DCP_SET_MISC_ERROR(status);
                }
            }
            DCP_DHD_ARGV_FREE(argc1, argv1);
        }

        /*
         * Get prerequisites of the server
         */
        if (prerequisites) {
            if (Tcl_SplitList(interp, prerequisites, &argc1, &argv1) != TCL_OK) {
                dced_server_free_ptrs(&dced_server);
                return TCL_ERROR;
            }
            DCP_DHD_MALLOC_LIST_RESET(&dced_server.fixed.prerequisites,
                                      uuid_t,
                                      argc1,
                                      status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                DCP_DHD_ARGV_FREE(argc1, argv1);
                dced_server_free_ptrs(&dced_server);
                return TCL_ERROR;
            }
            for (i = 0; i < argc1; i++)  {
                uuid_from_string((unsigned char *) argv1[i],
                                 &dced_server.fixed.prerequisites.list[i],
                                 &status);
                if (!DCP_DHD_GOOD_STATUS(status)) {
                    DCP_DHD_ARGV_FREE(argc1, argv1);
                    dced_server_free_ptrs(&dced_server);
                    DCP_SET_MISC_ERROR(status);
                }
            }
            DCP_DHD_ARGV_FREE(argc1, argv1);
        }

        /*
         * Get principal names
         */
        if (principals) {
            if (Tcl_SplitList(interp, principals, &argc1, &argv1) != TCL_OK) {
                dced_server_free_ptrs(&dced_server);
                return TCL_ERROR;
            }
            DCP_DHD_MALLOC_LIST_RESET(&dced_server.prin_names,
                                      dced_string_t,
                                      argc1,
                                      status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                DCP_DHD_ARGV_FREE(argc1, argv1); 
                dced_server_free_ptrs(&dced_server); 
                return TCL_ERROR; 
            } 
            for (i=0; i<argc1; i++) { 
                dced_server.prin_names.list[i] = (dced_string_t) dce_strdup(argv1[i]); 
            }
            DCP_DHD_ARGV_FREE(argc1, argv1);
        }

        /*
         * Get server program
         */
        if (program) {
            dced_server.fixed.program = (dced_string_t) dce_strdup(program);
        }
        /*
         * Get services
         */
        if ((services) && 
            (dhd__parse_services(interp, services, &dced_server.services) != TCL_OK)) {
            dced_server_free_ptrs(&dced_server);
            return TCL_ERROR;
        }

        /*
         * Get startup flags
         */
        if (starton) {
            if (Tcl_SplitList(interp, starton, &argc1, &argv1) != TCL_OK) {
                dced_server_free_ptrs(&dced_server);
                return TCL_ERROR;
            }
            for (i=0; i<argc1; i++)  {
                if (!strcasecmp(dhd_c_starton_boot_token, argv1[i])) {
                    dced_server.fixed.startupflags |= server_c_startup_at_boot;
                }
                if (!strcasecmp(dhd_c_starton_auto_token, argv1[i])) {
                    dced_server.fixed.startupflags |= server_c_startup_auto;
                }
                if (!strcasecmp(dhd_c_starton_explicit_token, argv1[i])) {
                    dced_server.fixed.startupflags |= server_c_startup_explicit;
                }
                if (!strcasecmp(dhd_c_starton_failure_token, argv1[i])) {
                    dced_server.fixed.startupflags |= server_c_startup_on_failure;
                }
                if (!strcasecmp(dhd_c_starton_schedule_token, argv1[i])) {
                    dced_server.fixed.startupflags |= server_c_startup_on_schedule;
                }
            }
            DCP_DHD_ARGV_FREE(argc1, argv1);
        }

        /*
         * Get server uid
         */
        if (uid) {
            dced_server.fixed.posix_uid = atol(uid);
        }

        /*
         * Get server uuid
         */
        if (uuid) {
            uuid_from_string((unsigned char *) uuid, &dced_server.id, &status);
            if (!DCP_DHD_GOOD_STATUS(status)) {
                dced_server_free_ptrs(&dced_server);
                DCP_SET_MISC_ERROR(status);
            }
        }
    }
        
    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list 
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        if ((attribute) && (era_attr)) {
            free(era_attr);
        }
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    auth_info = s_null_auth_info;
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        auth_info.info_type = sec_attr_bind_auth_dce;
        auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
        auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
        auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
    }

    for (i = 0; i < argc1; i++) {
        /*
         * Parse the input argument
         */
        if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrconf, 
                            dced_c_service_srvrconf_len, NULL,
                            &entryname, &name, NULL) != TCL_OK) {
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
            break;
        }

        /*
         * If era attribute is part of attributes req to create
         * the server object, it need to be parsed into server
         * object here because dce attr schema interface need to be
         * invoked.
         */
        if (era_attr) {
            /*
             * Create the dce_attr_sch binding
             */
            if (local_only) {
                dce_attr_sch_bind_local(0,
                                        &dced_rs_attr_schema_v0_0_epv,
                                        (dce_attr_component_name_t)dhd_c_attr_schema_token,
                                        &attr_schema_binding,
                                        &status);
            }
            else {
                if (dhd__schema_name_from_dced_name(interp, entryname,
                                                    &schema_name) != TCL_OK) {
                    free(entryname);
                    free(name);
                    DCP_DHD_SET_STATUS(status, ~error_status_ok);
                    break;
                }
                dce_attr_sch_bind(schema_name,
                                  &auth_info, 
                                  &attr_schema_binding,
                                  &status);
                free(schema_name);
            }
            if (!DCP_DHD_GOOD_STATUS(status)) {
                free(entryname);
                free(name);
                DCP_SET_RESULT_CODE(status);
                break;
            }

            if (i) {
                dced_attr_list_free_ptrs(&dced_server.attributes);
            }
            if (dhd__parse_sec_attr(interp, era_attr, &dced_server.attributes, 
                                                     NULL, FALSE) != TCL_OK) {
                free(entryname);
                free(name);
                dce_attr_sch_bind_free(&attr_schema_binding, &status1);
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }
        }

        DCP_DHD_STRING_FREE(dced_server.name);
        dced_server.name = (dced_string_t)name;

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t)entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &sr_handle,
                            &status);
        free(entryname);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            if (era_attr) {
                dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            }
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (auth_info.info_type == sec_attr_bind_auth_dce) {
            dced_binding_set_auth_info(
                                sr_handle, 
                                auth_info.tagged_union.dce_info.protect_level,
                                auth_info.tagged_union.dce_info.authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                auth_info.tagged_union.dce_info.authz_svc, 
                                &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_server_create() to create a server object 
         */
        dced_server_create(sr_handle, &dced_server, &status);
        DCP_DHD_CHECK_STATUS_OUT(status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        if (era_attr) {
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
                DCP_DHD_SET_STATUS(status, status1);
            }
        }

        /*
         * If a error has been encountered and out of for loop
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    if ((attribute) && (era_attr)) {
        free(era_attr);
    }
    dced_server_free_ptrs(&dced_server);
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sr_delete 
 *
 * OVERVIEW:    Delete a server(srvrconf) object on the specified host.
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_delete(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char                    *name, *entryname;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   sr_handle;
    sec_login_handle_t	    login_context;
    boolean32		    authn_user = FALSE;
    boolean32		    local_only = FALSE;
    uuid_t                  id;
    int                     i;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-local", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
            dcp_t_sr_local_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&local_only;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i = 0; i < argc1; i++) {

        /*
         * Parse the input argument
         */
	if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrconf, 
			    dced_c_service_srvrconf_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &sr_handle,
                            &status);
        free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
            free(name);
            DCP_SET_RESULT_CODE(status);
            break;
        }

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
                dced_binding_set_auth_info(
                                sr_handle, 
                                dhd_c_prot_level,
                                dhd_c_authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                dhd_c_authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       
		
        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call delete routine.
         */
         dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
	 DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_server_delete() to delete a server object  
         */
        dced_server_delete(sr_handle, &id, &status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sr_disable 
 *
 * OVERVIEW:    Disable a server(srvrexec) on the specified host.
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_disable(ClientData clientData, 
               Tcl_Interp *interp, 
               int        argc, 
               char       **argv)
{
    char                    *name, *entryname;
    char		    *interface = NULL;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   sr_handle;
    sec_login_handle_t	    login_context;
    boolean32		    authn_user = FALSE;
    uuid_t                  id;
    rpc_if_id_t		    *rpc_interface_id;
    int                     i, j, num_if_id;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_interface_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&interface;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (!interface) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "interface");
        return TCL_ERROR;
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Parse the interface list
     */
    if (dhd__parse_interface_list(interp, interface, 
				  &num_if_id, &rpc_interface_id) != TCL_OK) {
	DCP_DHD_ARGV_FREE(argc1, argv1);
    	return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop thr all servers (hosts)
     */
    for (i=0; i<argc1; i++) {

	/*
	 * Parse the input argument
	 */
	if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrexec, 
			    dced_c_service_srvrexec_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname, 0, 
                            &sr_handle, &status);
	free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
		free(name);
		DCP_SET_RESULT_CODE(status);
		break;
        }

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
                dced_binding_set_auth_info(
                                sr_handle, 
                                dhd_c_prot_level,
                                dhd_c_authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                dhd_c_authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       
		
        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call disable routines.
         */
         dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
	 DCP_DHD_CHECK_STATUS_OUT(status);

	/*
	 * Loop thr the interface list
	 */
	for (j=0; j<num_if_id; j++) {

	        /*
       	         * Call dced_server_disable_if() to disable a server interface  
         	 */
        	dced_server_disable_if(sr_handle, &id, 
				       &rpc_interface_id[j], &status);
		DCP_DHD_CHECK_STATUS_OUT(status);
	}
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}
	 
	free(name);
        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    free(rpc_interface_id);
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sr_enable 
 *
 * OVERVIEW:    Enable a server interfaces on the specified host.
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_enable(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char                    *name, *entryname;
    char		    *interface = NULL;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   sr_handle;
    sec_login_handle_t	    login_context;
    boolean32		    authn_user = FALSE;
    uuid_t                  id;
    rpc_if_id_t		    *rpc_interface_id;
    int                     i, j, num_if_id;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-interface", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_interface_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&interface;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if (!interface) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "interface");
        return TCL_ERROR;
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Parse the interface list
     */
    if (dhd__parse_interface_list(interp, interface,
				  &num_if_id, &rpc_interface_id) != TCL_OK) {
	DCP_DHD_ARGV_FREE(argc1, argv1);
	return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    /*
     * Loop thr all servers (hosts)
     */
    for (i=0; i<argc1; i++) {

	/*
	 * Parse the input argument
	 */
	if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrexec, 
			    dced_c_service_srvrexec_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname, 0, 
                            &sr_handle, &status);
	free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
	    free(name);
	    DCP_SET_RESULT_CODE(status);
            break;
        }

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
                dced_binding_set_auth_info(
                                sr_handle, 
                                dhd_c_prot_level,
                                dhd_c_authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                dhd_c_authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call enable routines.
         */
         dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
	 DCP_DHD_CHECK_STATUS_OUT(status);

	/*
	 * Loop thr the interface list
	 */
	for (j=0; j<num_if_id; j++) {

	        /*
       	         * Call dced_server_enable_if() to enable a server interface
         	 */
        	dced_server_enable_if(sr_handle, &id, 
				      &rpc_interface_id[j], &status);
		DCP_DHD_CHECK_STATUS_OUT(status);
	}
out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}

	free(name);
        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    free(rpc_interface_id);
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_sr_enable() */


/*
 * FUNCTION:    dcp_sr_modify 
 *
 * OVERVIEW:    modify the specified server object's variable attribute
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_modify(ClientData clientData, 
              Tcl_Interp *interp, 
              int        argc, 
              char       **argv)
{
    char                          *add_attr = (char *) NULL;
    char                          *rem_attr = (char *) NULL;
    char                          *chg_attr = (char *) NULL;
    char                          *attributes;
    char                          *name, *entryname;
    dced_binding_handle_t         sr_handle;
    sec_login_handle_t            login_context;
    int                           argc1;
    char                          **argv1; 
    int                           i, j, k;
    int                           total_count = 0;
    int                           attr_count = 0;
    int                           exist_count = 0;
    boolean32                     called_from = 0;
    dced_attr_list_t              new_attr_list;
    dced_attr_list_t              exist_attr_list;
    dced_attr_list_t              del_attr_list;
    dced_attr_list_t              dced_attr_list;
    extern dce_attr_sch_handle_t  attr_schema_binding;
    dce_attr_component_name_t     schema_name;
    dce_attr_bind_auth_info_t     auth_info;
    uuid_t                        id;
    server_t                      *serv_obj;
    era_info_t                    *era_info;
    unsigned32                    modify_type;
    boolean32                     read_obj = FALSE, on_type = FALSE;
    boolean32			  local_only = FALSE;
    error_status_t                status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-add", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_add_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_change_help},
        {"-local", DCP_ARGV_CONSTANT, (char *) TRUE, NULL,
            dcp_t_sr_local_help},
        {"-remove", DCP_ARGV_STRING, NULL, NULL, 
            dcp_t_sr_remove_help},
        {"-types", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_types_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &add_attr;
    arg_table[1].dst = (char *) &chg_attr;
    arg_table[2].dst = (char *)&local_only;
    arg_table[3].dst = (char *) &rem_attr;
    arg_table[4].dst = (char *) &on_type;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);
    modify_type = modify_type_none;

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    if ((!add_attr) && (!chg_attr) && (!rem_attr)) {
        DCP_DHD_SET_RESULT(dcp_s_sr_missing_req_opt, "add, change or remove");
        return TCL_ERROR;
    }

    if (((add_attr) && ((chg_attr) || (rem_attr))) ||
        ((chg_attr) && (rem_attr))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    auth_info = s_null_auth_info;
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        auth_info.info_type = sec_attr_bind_auth_dce;
        auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
        auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
        auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
    }

    /*
     * Initialize the attr lists before we start real work
     */
    if (add_attr) {
        modify_type = modify_type_add;
        attributes = add_attr;
    }
    else if (chg_attr) {
        modify_type = modify_type_change;
        attributes = chg_attr;
    }
    else {
        modify_type = modify_type_remove;
        attributes = rem_attr;
    }
    era_info = NULL;
    new_attr_list.count = 0;
    new_attr_list.list = NULL;
    exist_attr_list.count = 0;
    exist_attr_list.list = NULL;
    del_attr_list.count = 0;
    del_attr_list.list = NULL;
    dced_attr_list.count = 0;
    dced_attr_list.list = NULL; 

    for (i = 0; i < argc1; i++) {
        /*
         * Parse the input argument
         */
        if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrconf, 
                            dced_c_service_srvrconf_len, NULL,
                            &entryname, &name, NULL) != TCL_OK) {
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
            break;
        }

        /*
         * Create the dce_attr_sch binding
         */
        if (local_only) {
            dce_attr_sch_bind_local(0,
                                    &dced_rs_attr_schema_v0_0_epv,
                                    (dce_attr_component_name_t)dhd_c_attr_schema_token,
                                    &attr_schema_binding,
                                    &status);
        }
        else {
            if (dhd__schema_name_from_dced_name(interp, entryname, &schema_name) != TCL_OK) {
                free(entryname);
                free(name);
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }
            dce_attr_sch_bind(schema_name, &auth_info, &attr_schema_binding, &status);
            free(schema_name);
        }
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(entryname);
            free(name);
            DCP_SET_RESULT_CODE(status);
            break;
        }
 
        if (i) {
            dced_attr_list_free_ptrs(&new_attr_list);
            dced_attr_list_free_ptrs(&exist_attr_list);
            dced_attr_list_free_ptrs(&del_attr_list);
            dced_attr_list_free_ptrs(&dced_attr_list);
            if (modify_type == modify_type_add) {
                free(era_info);
            }
        }

        /*
         * Parse the attribute
         */
        if (dhd__parse_sec_attr(interp, attributes, &new_attr_list, 
               (modify_type == modify_type_add) ? &era_info : NULL, on_type) != TCL_OK) {
            free(entryname);
            free(name);
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            DCP_DHD_SET_STATUS(status, ~error_status_ok);
            break;
        }

        /*
         * If the req. list is empty, we're done
         */
        if (DCP_DHD_EMPTY_LIST(&new_attr_list)) {
            free(entryname);
            free(name);
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            continue;
        }
 
        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &sr_handle,
                            &status);
        free(entryname);
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(name);
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            DCP_SET_RESULT_CODE(status);
            break;
        }
 
        /*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (auth_info.info_type == sec_attr_bind_auth_dce) {
            dced_binding_set_auth_info(sr_handle,
                                       auth_info.tagged_union.dce_info.protect_level,
                                       auth_info.tagged_union.dce_info.authn_svc, 
                                       (rpc_auth_identity_handle_t) login_context,
                                       auth_info.tagged_union.dce_info.authz_svc, 
                                       &status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * needed to call attribute modify routine of dced.
         */
        dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
        DCP_DHD_CHECK_STATUS_OUT(status);
 
        /*
         * Read attributes attached with the object under request out
         */
        dced_object_read(sr_handle, &id, (void **)&serv_obj, &status);
        DCP_DHD_CHECK_STATUS_OUT(status);
        read_obj = TRUE;

        /*
         * Allocate space for the attrs which already exist
         * and are under consideration to be modified.
         */
        if (serv_obj->attributes.count > 0) {
            DCP_DHD_MALLOC_LIST_RESET(&exist_attr_list, sec_attr_t,
                                      serv_obj->attributes.count, status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        for (j = 0; j < serv_obj->attributes.count; j++) {
            for (k = 0; k < new_attr_list.count; k++) {
                if (uuid_equal(&serv_obj->attributes.list[j].attr_id,
                               &new_attr_list.list[k].attr_id, &status1)) {
                    sec_attr_util_alloc_copy(NULL,
                                             &serv_obj->attributes.list[j],
                                             &exist_attr_list.list[exist_count++],
                                             &status);
                    DCP_DHD_CHECK_STATUS_OUT(status);

                    /*
                     * Mark the attr which is copied out
                     */
                    uuid_create_nil(&serv_obj->attributes.list[j].attr_id, &status1);
                    break;
                }
            }
        }
        exist_attr_list.count = exist_count;
 
        /*
         * The modify_era_info() need a del_attrs[] if modify_type is
         * modify_type_remove. Even we don't use it (because 
         * dced_server_modify_attributes() does replace), it has been 
         * allocated to be able to use modify_era_info()
         */
        if ((modify_type == modify_type_remove) && (exist_count)) {
            DCP_DHD_MALLOC_LIST_RESET(&del_attr_list, sec_attr_t, exist_count, status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Allocate the array for the attrs after modification
         */
        if (modify_type == modify_type_add) {
            attr_count += new_attr_list.count;
        }
        if ((attr_count != 0) || (serv_obj->attributes.count != 0)) {
            total_count = attr_count + serv_obj->attributes.count;
            DCP_DHD_MALLOC_LIST_RESET(&dced_attr_list, sec_attr_t, total_count, status);
            DCP_DHD_CHECK_STATUS_OUT(status);
        }

        /*
         * Ask modify_era_info() to do the work
         */
        status = modify_era_info(interp,
                                 called_from,
                                 modify_type,
                                 new_attr_list.count,
                                 on_type,
                                 new_attr_list.list,
                                 era_info,
                                 exist_count,
                                 (exist_count) ? exist_attr_list.list : NULL,
                                 (unsigned32 *) &attr_count,
                                 dced_attr_list.list,
                                 (unsigned32 *) &del_attr_list.count,
                                 del_attr_list.list);
        DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Copy over the attrs which will stay
         */
        for (j=0; j<serv_obj->attributes.count; j++) {
            if (!uuid_is_nil(&serv_obj->attributes.list[j].attr_id, &status1)) {
                sec_attr_util_alloc_copy(NULL,
                                         &serv_obj->attributes.list[j],
                                         &dced_attr_list.list[attr_count++],
                                         &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
            }
        }
        dced_attr_list.count = attr_count;
        dced_objects_release(sr_handle, 1, (void *)serv_obj, &status);
        DCP_DHD_CHECK_STATUS_OUT(status);
        read_obj = FALSE;

        /*
         * Call dced_server_modify_attributes() to replace old
         * attribute value with new one.
         */
        dced_server_modify_attributes(sr_handle, &id, &dced_attr_list, &status);

out:
        /*
         * If the object has been read out, release it first
         */
        if (read_obj) {
            dced_objects_release(sr_handle, 1, (void *) serv_obj, &status1);
            if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
                DCP_DHD_SET_STATUS(status, status1);
            }
        }

        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        dce_attr_sch_bind_free(&attr_schema_binding, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }

        free(name);

        /*
         * Get out the for loop if a error has been met up to now
         * and abort the rest of work.
         */
        DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    if ((modify_type == modify_type_add) && (era_info)) {
        free(era_info);
    }
    dced_attr_list_free_ptrs(&new_attr_list);
    dced_attr_list_free_ptrs(&exist_attr_list);
    dced_attr_list_free_ptrs(&del_attr_list);
    dced_attr_list_free_ptrs(&dced_attr_list);
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sr_ping 
 *
 * OVERVIEW:    Ping a server to see if it is receciving requests
 *              on the specified host.
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_ping(ClientData    clientData, 
            Tcl_Interp    *interp, 
            int           argc, 
            char          **argv)
{
    int                     argc1, argc2;
    char                    **argv1, **argv2;
    char		    *string_b;
    rpc_ns_handle_t	    import_context;
    rpc_binding_handle_t    rpc_binding;
    unsigned_char_t         *stringBindingp;
    char		    *comma_place;
    char		    *next_comma_place;
    int                     i;
    char                    *timeout = NULL;
    unsigned32              timeout_value;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-timeout", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_ping_timeout_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV_2(argc1, argv1, argc2, argv2);

    arg_table[0].dst = (char *)&timeout;

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
 
    timeout_value = rpc_c_binding_min_timeout;
    if (timeout) {
	if (!strncasecmp(timeout, dhd_c_ping_m_timeout_min_token, 
			 dhd_c_ping_m_timeout_min_token_len)) {
            timeout_value = rpc_c_binding_min_timeout;
	}
	else if (!strncasecmp(timeout, dhd_c_ping_m_timeout_max_token, 
			 dhd_c_ping_m_timeout_max_token_len)) {
            timeout_value = rpc_c_binding_max_timeout;
	}
	else if (!strncasecmp(timeout, dhd_c_ping_m_timeout_default_token, 
			 dhd_c_ping_m_timeout_default_token_len)) {
            timeout_value = rpc_c_binding_default_timeout;
	}
	else {
            DCP_SET_MISC_ERROR(dcp_s_sr_wrong_timeout_code);
	}
    }

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (dcp_general_split_list(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    for (i=0; i<argc1; i++) {
        /*
         * Parse name -):
         *  If there is '/', the input name is a server entry in the name
	 *  space to be imported from, if there is space " ", it's tcl binding,
         *  otherwise it is a string binding.
         */
        if (strrchr(argv1[i], '/')) {
		/*
		 * Import the binding from the entryname
		 */
		rpc_ns_binding_import_begin(dhd_c_entry_syntax,
					    (unsigned char *) argv1[i],
					    (rpc_if_handle_t) NULL,
					    (uuid_t *) NULL, 
					    &import_context, 
					    &status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}

		/*
		 * Loop to get a valid rpc binding
		 */
		rpc_ns_binding_import_next(import_context, 
					   &rpc_binding, 
					   &status);

		rpc_ns_binding_import_done(&import_context, &status1);
		if (DCP_DHD_GOOD_STATUS(status) && 
		    !DCP_DHD_GOOD_STATUS(status1)) {
			DCP_DHD_SET_STATUS(status, status1);
        		rpc_binding_free(&rpc_binding, &status1);
		}
		
		/*
		 * Failed to get a rpc binding, return now
		 */
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
        } 
        else if (strchr(argv1[i], ' ' )) {
		if (Tcl_SplitList(interp, argv1[i], &argc2, &argv2) != TCL_OK) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
            		return TCL_ERROR;
		}
		string_b = dcp_binding_tcl_to_string(argc2, argv2, &status);
		DCP_DHD_ARGV_FREE(argc2, argv2);
		if (!DCP_DHD_GOOD_STATUS(status)) {	
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
	       /* 
		* Get a rpc binding handle 
		*/
		rpc_binding_from_string_binding(
				(unsigned char *) string_b,
				(rpc_binding_handle_t *) &rpc_binding,
				&status);
		rpc_string_free((unsigned char **) &string_b, &status1);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
	}
        else if ((comma_place=strchr(argv1[i], ',' ))) {
		/* support for the following syntax:
		 *   server ping interface_id,hostname   or
		 *   server ping interface_id,object_uuid,hostname
		 * No support for exact version number matching.
		 */
		rpc_ep_inq_handle_t  inqContext;
		rpc_if_id_t          IfID; 
		char                 *epBinding;
		unsigned_char_t      *protseq;
		unsigned_char_t      *endpoint;
		char                 *hostname;
		char                 interface_id[UUID_STRING_LEN+1];
		int                  num_char_to_copy;
		uuid_t               object_uuid;
		char                 object_uuid_str[UUID_STRING_LEN+1];
		int	             index;

		for (index = 0; index < UUID_STRING_LEN+1; index ++) 
			object_uuid_str[index] = '\0';
		hostname = comma_place+1;
		num_char_to_copy = strlen(argv1[i]) - strlen(comma_place);
		if (num_char_to_copy > UUID_STRING_LEN) 
			num_char_to_copy = UUID_STRING_LEN; 
		strncpy(interface_id, argv1[i], num_char_to_copy);
		*(interface_id + num_char_to_copy) = '\0';

		/* see if there is any object-uuid supplied */

		if ((next_comma_place = strchr(hostname, ','))) {
			/* object uuid specified */
			num_char_to_copy = strlen(hostname) - strlen(next_comma_place);
			if (num_char_to_copy > UUID_STRING_LEN) 
				num_char_to_copy = UUID_STRING_LEN; 
			strncpy(object_uuid_str, hostname, num_char_to_copy);
			*(object_uuid_str + num_char_to_copy) = '\0';
			hostname = next_comma_place + 1;
		}

		/* get rpcd string binding */

		protseq = (unsigned_char_t *) strdup(UDP_PROTSEQ);
		endpoint = (unsigned_char_t *) strdup(RPCD_ENDPOINT);
		rpc_string_binding_compose ( (unsigned_char_t *)0,
                                 protseq,
				 (unsigned_char_t *) hostname,
                                 endpoint, 
                                 (unsigned_char_t *)0,
                                 &stringBindingp,
                                 &status );
		free(protseq);
		free(endpoint);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}

		/* get endpint map binding */
		rpc_binding_from_string_binding (
            		(unsigned_char_p_t) stringBindingp,
            		(rpc_binding_handle_t *) &epBinding,
            		&status);
		rpc_string_free((unsigned char **) &stringBindingp, &status1);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
		/* convert string interface uuid to real uuid */

		uuid_from_string ((unsigned_char_t *)interface_id, &IfID.uuid, &status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
		IfID.vers_major = 0; /* don't care */
		IfID.vers_minor = 0; /* don't care */

		if (strlen(object_uuid_str)) {
			uuid_from_string ((unsigned_char_t *)object_uuid_str, &object_uuid, &status);
		    	if (!DCP_DHD_GOOD_STATUS(status)) {
				DCP_DHD_ARGV_FREE(argc1, argv1);
				DCP_SET_MISC_ERROR(status);
		    	}
		}

		if (strlen(object_uuid_str)) { 
			/* both interface id and uuid specified */
			/* call inq_begin */
			rpc_mgmt_ep_elt_inq_begin ( (rpc_binding_handle_t) epBinding,
			    rpc_c_ep_match_by_both,
			    &IfID,
			    rpc_c_vers_all,
			    &object_uuid,
			    &inqContext,
			    &status );
	 	} else {
			/* only interface id sepcified */
		    	rpc_mgmt_ep_elt_inq_begin (
			    (rpc_binding_handle_t) epBinding,
			    rpc_c_ep_match_by_if,
			    &IfID,
			    rpc_c_vers_all,
			    NULL,
			    &inqContext,
			    &status );
		}
		rpc_string_free((unsigned char **) &epBinding, &status1);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
		
		/* call inq_next */

		rpc_mgmt_ep_elt_inq_next (
		    inqContext,
		    &IfID,
		    &rpc_binding,
		    NULL,
		    NULL,
		    &status );
		rpc_mgmt_ep_elt_inq_done(&inqContext, &status1);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
	}
        else {
	       /* 
		* Get a rpc binding handle from the argument 
		*/
		rpc_binding_from_string_binding(
				(unsigned char *) argv1[i],
				(rpc_binding_handle_t *) &rpc_binding,
				&status);
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}
        } /* End of if (strrchr(argv1[i], '/')) */

        /*
         * make the rpc call to ping the server 
         */
	rpc_mgmt_set_com_timeout(rpc_binding, 
				 timeout_value, 
				 &status);
	if (!DCP_DHD_GOOD_STATUS(status)) {
		DCP_DHD_ARGV_FREE(argc1, argv1);
		DCP_SET_MISC_ERROR(status);
	}
	if (rpc_mgmt_is_server_listening(rpc_binding, &status)) {
		if (i) {
                        Tcl_AppendResult(interp, " 1", NULL);
		}
		else {
                        Tcl_AppendResult(interp, "1", NULL);
		}
	}
	else {
		/*
		 * Treat all runtime exceptional errors as the server 
		 * unreachable
		 */
		if (!DCP_DHD_GOOD_STATUS(status) &&
		    (DCP_DHD_STATUS_EQUAL(status, rpc_s_invalid_binding) || 
		     DCP_DHD_STATUS_EQUAL(status, rpc_s_binding_incomplete) || 
		     DCP_DHD_STATUS_EQUAL(status, rpc_s_mgmt_op_disallowed) || 
		     DCP_DHD_STATUS_EQUAL(status, rpc_s_wrong_kind_of_binding)))
		{
			DCP_DHD_ARGV_FREE(argc1, argv1);
			DCP_SET_MISC_ERROR(status);
		}		
		if (i) {
                        Tcl_AppendResult(interp, " 0", NULL);
		}
		else {
                        Tcl_AppendResult(interp, "0", NULL);
		}
		if (!DCP_DHD_GOOD_STATUS(status)) {
			DCP_DHD_RESET_STATUS(status);
		}
	}

	/*
	 * Call rpc_binding_free to release the binding and ignore the
	 * status returned.
	 */
	rpc_binding_free(&rpc_binding, &status);

    } /* End of for (i=0; i<argc1; i++) */

    /*
     * Free memory and return right status
     */
    DCP_DHD_ARGV_FREE(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_sr_ping() */


/*
 * FUNCTION:    dcp_sr_show 
 *
 * OVERVIEW:    Show the contents of a server object
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_show(ClientData  clientData, 
            Tcl_Interp  *interp, 
            int         argc, 
            char        **argv)
{
    char                    		*name, *entryname;
    boolean32               		srvrexec = FALSE;
    boolean32				local_only = FALSE;
    extern dce_attr_sch_handle_t        attr_schema_binding;
    dce_attr_component_name_t           schema_name;
    dce_attr_bind_auth_info_t           auth_info;
    dced_binding_handle_t   		sr_handle;
    sec_login_handle_t			login_context;
    server_t                		*serv_obj;
    int                     		i;
    uuid_t                  		id;
    int                     		argc1;
    char                    		**argv1;
    error_status_t          		status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-executing", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_executing_help},
        {"-local", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
            dcp_t_sr_local_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&srvrexec;
    arg_table[1].dst = (char *)&local_only;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check if the local option can be used.
     */
    if (local_only && !dcp_general_is_bound_locally(interp)) {
        DCP_SET_MISC_ERROR(dcp_s_bad_use_of_local);
    }

    /*
     * Check for syntax errors.
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Options are mutually exclusive.
     */
    if (local_only && srvrexec) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    auth_info = s_null_auth_info;
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        auth_info.info_type = sec_attr_bind_auth_dce;
        auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
        auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
        auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
    }

    for (i = 0; i < argc1; i++) {

        /*
         * Parse the input argument
         */
	if (dhd__parse_name(interp, 
	    		    argv1[i], 
	    		    (srvrexec)? dced_c_service_srvrexec: 
					dced_c_service_srvrconf, 
			    (srvrexec)? dced_c_service_srvrexec_len:
					dced_c_service_srvrconf_len, 
			    NULL,
			    &entryname,
			    &name,
			    NULL) != TCL_OK) {
            DCP_DHD_SET_STATUS(status,  ~error_status_ok);
            break;
	}

        /*
         * Create the dce_attr_sch binding
         */
        if (local_only) {
            dce_attr_sch_bind_local(0,
                                    &dced_rs_attr_schema_v0_0_epv,
                                    (dce_attr_component_name_t)dhd_c_attr_schema_token,
                                    &attr_schema_binding,
                                    &status);
        }
        else {
            if (dhd__schema_name_from_dced_name(interp, entryname, &schema_name) != TCL_OK) {
                free(entryname);
                free(name);
                DCP_DHD_SET_STATUS(status, ~error_status_ok);
                break;
            }               
            dce_attr_sch_bind(schema_name, &auth_info, &attr_schema_binding, &status);
            free(schema_name);
        }
        if (!DCP_DHD_GOOD_STATUS(status)) {
            free(entryname);
            free(name);
            DCP_SET_RESULT_CODE(status);
            break;
        }

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname,
                            ((local_only) ? dced_c_binding_syntax_local :
                                            dced_c_binding_syntax_default),
                            &sr_handle,
                            &status);
        free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
            free(name);
            dce_attr_sch_bind_free(&attr_schema_binding, &status1);
            DCP_SET_RESULT_CODE(status);
            break;
        }

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (auth_info.info_type == sec_attr_bind_auth_dce) {
                dced_binding_set_auth_info(
                                sr_handle, 
                                auth_info.tagged_union.dce_info.protect_level,
                                auth_info.tagged_union.dce_info.authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                auth_info.tagged_union.dce_info.authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       
		
	/*
	 * Call dced_inq_id() to convert the name into uuid which is
	 * needed call the server read routine.
	 */
	dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Insert a newline between output of objects
         */
        if (i) {
            Tcl_AppendResult(interp, "\n", (char *) NULL);
        }

        /*
         * Call dced_object_read() to read the server object. 
         */
	dced_object_read(sr_handle, &id, (void **) &serv_obj, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

	/*
	 * Show the hostdata object if read success
  	 */
        dhd__show_object(interp, (void *) serv_obj, 
			 (srvrexec)? dced_e_service_type_srvrexec:
				     dced_e_service_type_srvrconf, 
                         dhd_e_show_all);   
	/*
	 * Relase the hostdata object after showing
	 */
	dced_objects_release(sr_handle, 1, (void *) serv_obj, &status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
	}

        dce_attr_sch_bind_free(&attr_schema_binding, &status1);
        if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
            DCP_DHD_SET_STATUS(status, status1);
        }
	free(name);

        /*
         * If a error has been met up to now, abort the
         * operation to show rest of names in the list
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free the memory and return right status 
     */
    DCP_DHD_CLEAR_ARGV(argc1, argv1);
    DCP_DHD_RETURN(status);
}


/*
 * FUNCTION:    dcp_sr_start 
 *
 * OVERVIEW:    start a server(srvrconf) object on the specified host.
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_start(ClientData clientData, 
             Tcl_Interp *interp, 
             int        argc, 
             char       **argv)
{
    char                    *name, *entryname;
    char		    *uuid = NULL;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   sr_handle;
    sec_login_handle_t	    login_context;
    boolean32               authn_user = FALSE;
    uuid_t                  id, rt_id;
    unsigned char	    *uuid_string;
    int                     i;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_uuid_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &uuid;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * If uuid is given for the starting server instance, only one
     * server can be start with this uuid
     */
    if ((uuid) && (argc1 > 1)) {
	DCP_DHD_ARGV_FREE(argc1, argv1);
	DCP_SET_MISC_ERROR(dcp_s_sr_uuid_mul_used);
    }

    if (uuid) {
	uuid_from_string((unsigned char *) uuid, &rt_id, &status);   
    }
    else {
	uuid_create_nil(&rt_id, &status);

    }
    if (!DCP_DHD_GOOD_STATUS(status)) {
    	DCP_DHD_ARGV_FREE(argc1, argv1);
    	DCP_SET_MISC_ERROR(status);
    } 

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	/*
	 * Parse the input argument
	 */
	if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrconf, 
			    dced_c_service_srvrconf_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname, 0, 
                            &sr_handle, &status);
        free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
	    free(name);
	    DCP_SET_RESULT_CODE(status);
            break;
        }

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
                dced_binding_set_auth_info(
                                sr_handle, 
                                dhd_c_prot_level,
                                dhd_c_authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                dhd_c_authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       
		
        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call start routine.
         */
         dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
	 DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_server_start() to start a server 
         */
        dced_server_start(sr_handle, &id, NULL, &rt_id, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);

        /*
	 * Returns the started server uuid
	 */
	uuid_to_string(&rt_id, &uuid_string, &status);
	DCP_DHD_CHECK_STATUS_OUT(status);
	DCP_APPEND_RESULT(uuid_string);
	rpc_string_free(&uuid_string, &status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}
	free(name);

        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    DCP_DHD_CLEAR_ARGV(argc1, argv1);
    DCP_DHD_RETURN(status);
} /* End of dcp_sr_start() */


/*
 * FUNCTION:    dcp_sr_stop 
 *
 * OVERVIEW:    Stop a server(srvrexec) daemon on the specified host.
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
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_sr_stop(ClientData    clientData, 
            Tcl_Interp    *interp, 
            int           argc, 
            char          **argv)
{
    char		    *method = (char *) NULL;
    char                    *name, *entryname;
    int                     argc1;
    char                    **argv1;
    dced_binding_handle_t   sr_handle;
    sec_login_handle_t	    login_context;
    boolean32               authn_user = FALSE;
    srvrexec_stop_method_t  stop_flag;
    uuid_t                  id;
    int                     i;
    error_status_t          status, status1;

    dcp_ArgvInfo arg_table[] = {
        {"-method", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_sr_method_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *) &method;

    DCP_DHD_CLEAR_STATUS_2(status, status1);
    DCP_DHD_CLEAR_ARGV(argc1, argv1);

    /*
     * Parse the input arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 
		      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if the user just wants help
     */
    DCP_CHECK_COMMAND_HELP();
   
    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_dhd_no_object);
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set right stop method flag
     */
    if (method) {
	if (!strncasecmp(method, dhd_c_stop_m_rpc_token, 
			 dhd_c_stop_m_rpc_token_len)) {
		stop_flag = srvrexec_stop_rpc;
	}
	else if (!strncasecmp(method, dhd_c_stop_m_soft_token,
			      dhd_c_stop_m_soft_token_len)) {
		stop_flag = srvrexec_stop_soft;
	}
	else if (!strncasecmp(method, dhd_c_stop_m_hard_token,
			      dhd_c_stop_m_hard_token_len)) {
		stop_flag = srvrexec_stop_hard;
	}
	else if (!strncasecmp(method, dhd_c_stop_m_error_token,
			      dhd_c_stop_m_error_token_len)) {
		stop_flag = srvrexec_stop_error;
	}
	else {
		DCP_SET_MISC_ERROR(dcp_s_sr_wrong_stop_mthd);
	}
    }
    else {
	/*
	 * default is srvrecec_c_stop_rpc
	 */
	stop_flag = srvrexec_stop_rpc;
    }

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the name list
     */
    if (Tcl_SplitList(interp, argv[1], &argc1, &argv1) != TCL_OK) {
        return TCL_ERROR;
    }

    /*
     * Check to see if this user is unauthenticated or authenticated 
     */
    sec_login_get_current_context(&login_context, &status);
    if (DCP_DHD_GOOD_STATUS(status)) {
        authn_user = TRUE;
    }

    for (i=0; i<argc1; i++) {

	/*
	 * Parse the input argument
	 */
	if (dhd__parse_name(interp, argv1[i], dced_c_service_srvrexec, 
			    dced_c_service_srvrexec_len, NULL,
			    &entryname, &name, NULL) != TCL_OK) {
		DCP_DHD_SET_STATUS(status,  ~error_status_ok);
		break;
	}

        /*
         * Call dced_binding_create() to get binding
         */
        dced_binding_create((dced_string_t) entryname, 0, 
                            &sr_handle, &status);
        free(entryname);
	if (!DCP_DHD_GOOD_STATUS(status)) {
	    free(name);
	    DCP_SET_RESULT_CODE(status);
            break;
        }

	/*
         * Call dced_binding_set_auth_info to set the dced binding
         * if dce_login-ed
         */
        if (authn_user) {
                dced_binding_set_auth_info(
                                sr_handle, 
                                dhd_c_prot_level,
                                dhd_c_authn_svc, 
                                (rpc_auth_identity_handle_t) login_context,
                                dhd_c_authz_svc, 
                                &status);
                DCP_DHD_CHECK_STATUS_OUT(status);
        }       

        /*
         * Call dced_inq_id() to convert the name into uuid which is
         * need to call stop routine.
         */
         dced_inq_id(sr_handle, (dced_string_t) name, &id, &status);
	 DCP_DHD_CHECK_STATUS_OUT(status);

        /*
         * Call dced_server_stop() to stop a server 
         */
        dced_server_stop(sr_handle, &id, stop_flag, &status);

out:
        /*
         * Call dced_binding_free() to free the dced binding handle
         */
        dced_binding_free(sr_handle, &status1);
	if (DCP_DHD_GOOD_STATUS(status) && !DCP_DHD_GOOD_STATUS(status1)) {
		DCP_DHD_SET_STATUS(status, status1);
	}
	free(name);

        /*
         * If a error has been met up to now, leave the for loop
         * and abort the rest of work.
         */
	DCP_DHD_CHECK_STATUS_BRK(status);
    }

    /*
     * Free memory and return right status
     */
    DCP_DHD_CLEAR_ARGV(argc1, argv1);
    DCP_DHD_RETURN(status);
}
