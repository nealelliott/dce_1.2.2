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
 * $Log: rgy_sa_tclif.c,v $
 * Revision 1.1.9.1  1996/08/09  11:50:20  arvind
 * 	Merge changes.
 * 	[1996/06/12  14:16 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:24 UTC  truitt  /main/HPDCE02/20]
 *
 * 	Merge changes.
 * 	[1996/03/28  18:22 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	CHFts17975: The change to the access method is incorrect for a couple
 * 	commands.  We have to get the open_site and open_site_by_access
 * 	functions to work correctly, with the correct security api calls.  It would be
 * 	wonderful if the security api calls were returning the binding of an UP replica
 * 	each time, but supposedly there is no guarantee of that.  Unbelievable.  So,
 * 	we've got to jump through some hoops to make sure we make the correct call
 * 	to the correct replica (or cell)  under the correct circumstances, or forget it.
 * 	[1996/03/20  21:07 UTC  jrr  /main/HPDCE02/19]
 *
 * 	Have registry catalog and registry show bind with site_query
 * 	access. This will allow both operations to succeed when security
 * 	servers are down. open_rgy_site() will upgrade access to
 * 	site_specific if a site is specified.
 * 	[1996/03/20  21:06 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:13 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:43 UTC  truitt  /main/HPDCE02/17]
 *
 * 	Merge replist changes.
 * 	[1996/02/15  18:39 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts16871 (OT13316): Have the registry show operation support
 * 	return of slave replica list information.
 * 	[1996/02/05  15:57 UTC  truitt  /main/HPDCE02/16]
 *
 * 	Merge changes.
 * 	[1996/02/05  15:55 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 *
 * 	CHFts17406: Clean up some of the internal function names.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  13:19 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.3  1996/02/18  19:14:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:50  marty]
 * 
 * Revision 1.1.6.2  1995/12/08  21:22:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/15  1995/10/18  21:39 UTC  rps
 * 	CHFts16642 intercell config broken
 * 	[1995/10/18  17:03 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	HP revision /main/HPDCE02/14  1995/09/11  14:13 UTC  truitt
 * 	Merge registry changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  14:10 UTC  truitt
 * 	CHFts16100: Comprehensive clean-up of registry commands.
 * 
 * 	HP revision /main/HPDCE02/13  1995/05/16  12:31 UTC  mothra
 * 	change rs_repadm ifspec to latest.
 * 
 * 	HP revision /main/HPDCE02/12  1995/05/15  17:18 UTC  truitt
 * 	Merge warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:16 UTC  truitt
 * 	CHFts15237: Misuse of sam message.
 * 	Also, merge in final warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/SEC_migrate_merge/3  1995/05/13  17:45 UTC  greg
 * 	Yet another mergeout from HPDCE02 --> SEC_migrate_merge.
 * 
 * 	HP revision /main/HPDCE02/11  1995/05/10  19:07 UTC  mdf
 * 	Cleanup warning message
 * 	[1995/05/03  13:26 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_4/1]
 * 
 * 	HP revision /main/HPDCE02/SEC_migrate_merge/2  1995/05/13  02:40 UTC  greg
 * 	Mergeout from HPDCE02 fto SEC_migrate_merge.
 * 
 * 	HP revision /main/HPDCE02/9  1995/03/30  17:50 UTC  truitt
 * 	Merge some warranty patch work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts23/1  1995/03/30  17:48 UTC  truitt
 * 	CHFts14675: New registry functionality to support warranty
 * 	patch work.
 * 
 * 	HP revision /main/HPDCE02/8  1995/03/21  19:22 UTC  truitt
 * 	Merge registry changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts20/1  1995/03/21  19:20 UTC  truitt
 * 	In registry checkpoint, fix the check of the argument
 * 	to allow the cellname to be legal.
 * 
 * 	HP revision /main/HPDCE02/7  1995/03/09  14:29 UTC  truitt
 * 	New checkpoint functionality.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts18/1  1995/03/09  14:28 UTC  truitt
 * 	CHFts14531: Allow the registry checkpoint command to
 * 	specify the registry to use on the command line as an
 * 	optional argument.
 * 
 * 	HP revision /main/HPDCE02/6  1995/02/14  16:29 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:28 UTC  truitt
 * 	CHFts14285: Replace direct set of error with call
 * 	to correct macro.
 * 
 * 	HP revision /main/HPDCE02/5  1995/02/10  19:20 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  19:19 UTC  truitt
 * 	CHFts14285: Make sure the error code convenience
 * 	variable gets set when an error occurs.
 * 
 * 	HP revision /main/HPDCE02/4  1994/12/21  19:29 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_05/1  1994/12/21  19:28 UTC  truitt
 * 	Back out password visibility work.
 * 
 * 	HP revision /main/HPDCE02/3  1994/12/21  14:37 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_04/1  1994/12/21  14:36 UTC  truitt
 * 	Add a switch to the registry connect command to allow
 * 	the passwords to be visible on the command line.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/20  14:20 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chk_03/1  1994/12/20  14:14 UTC  truitt
 * 	Put option lists in order; add new error message
 * 	for when the cpi time is invalid.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:31 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/3  1994/12/07  17:16 UTC  hanfei
 * 	take out redundant error checking
 * 
 * 	HP revision /main/hanfei_cpi_bl1/2  1994/12/05  14:48 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/11/30  17:41 UTC  hanfei
 * 	added checkpoint time control.
 * 	[1995/12/08  17:56:58  root]
 * 
 * Revision 1.1.2.29  1994/10/05  19:12:22  kevins
 * 	OT 12464 modified registry connect to regenerate/reuse princs and accounts
 * 	[1994/10/05  19:10:24  kevins]
 * 
 * Revision 1.1.2.28  1994/09/30  17:07:04  kevins
 * 	OT 12376 added registry server unavail message for connect.
 * 	[1994/09/30  17:03:35  kevins]
 * 
 * Revision 1.1.2.27  1994/09/29  11:41:41  kevins
 * 	OT 12397 connect now works with hierarchical cell names.
 * 	[1994/09/29  11:41:25  kevins]
 * 
 * Revision 1.1.2.26  1994/09/28  17:54:40  kevins
 * 	OT 12376 modified registry unavailable message delivery
 * 	[1994/09/28  17:51:01  kevins]
 * 
 * Revision 1.1.2.25  1994/09/20  18:00:32  kevins
 * 	OT 12262 commands that have passwords in them now fail when called in -c mode
 * 	[1994/09/20  17:59:12  kevins]
 * 
 * Revision 1.1.2.24  1994/09/16  20:22:14  salamone
 * 	CR11985 - Set access_type to "site_specific" for registry catalog.
 * 	CR12185 - regi del -force doesn't work when the server is down.
 * 	[1994/09/16  20:02:01  salamone]
 * 
 * Revision 1.1.2.23  1994/09/13  19:12:42  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:46  kevins]
 * 
 * Revision 1.1.2.22  1994/09/12  17:58:01  kevins
 * 	OT 11968 Modified the registry opening process to aquire the
 * 	  rgy site handle with authentication that corresponds
 * 	  to the users login context. Removed site_master_query since
 * 	  it is no longer necessary.
 * 	[1994/09/12  13:51:02  kevins]
 * 
 * Revision 1.1.2.21  1994/09/06  16:22:02  kevins
 * 	OT 11985 - The site_specific flag now give precedence to site name over cell name.
 * 	[1994/09/06  15:44:12  kevins]
 * 
 * Revision 1.1.2.20  1994/08/31  23:07:17  salamone
 * 	CR11916 - Add _b(sec) support.
 * 	[1994/08/31  23:06:55  salamone]
 * 
 * Revision 1.1.2.19  1994/08/26  18:31:44  kevins
 * 	OT 11888 replace  obsolete constants
 * 	[1994/08/26  18:22:35  kevins]
 * 
 * Revision 1.1.2.18  1994/08/24  15:04:44  salamone
 * 	Merged with changes from 1.1.2.17
 * 	[1994/08/24  15:04:35  salamone]
 * 
 * 	CR11838 - Add support for "registry set" command.
 * 	[1994/08/23  21:26:37  salamone]
 * 
 * Revision 1.1.2.17  1994/08/23  21:20:52  kevins
 * 	OT 11789 finished registry connect and cleanup functions
 * 	[1994/08/23  20:56:47  kevins]
 * 
 * Revision 1.1.2.16  1994/08/15  23:48:18  melman
 * 	Added various casts
 * 	[1994/08/15  23:47:59  melman]
 * 
 * Revision 1.1.2.15  1994/08/15  17:28:38  salamone
 * 	CR11651: Generate errors with bad option or bad argument.
 * 	[1994/08/15  17:26:30  salamone]
 * 
 * 	CR11636: Allow registry catalog to take an argument.
 * 	[1994/08/15  16:19:39  salamone]
 * 
 * Revision 1.1.2.14  1994/08/12  20:03:43  kevins
 * 	OT 9691 Add registry connect command
 * 	[1994/08/12  20:00:20  kevins]
 * 
 * Revision 1.1.2.13  1994/08/08  18:01:02  kevins
 * 	OT 11071 - modified the manipulation of the site_cache to reflect
 * 	        the modifications. We now store two registry handles. The
 * 	        invalidation of the site_cache needs to invalidate the correct
 * 	        one.
 * 	[1994/08/08  15:40:53  kevins]
 * 
 * Revision 1.1.2.12  1994/07/22  18:03:34  salamone
 * 	CR11393 - Accept setting _s(sec) with a TCL syntax string binding
 * 	[1994/07/22  17:57:32  salamone]
 * 
 * Revision 1.1.2.11  1994/07/20  19:28:01  salamone
 * 	Add "registry verify" support.
 * 	[1994/07/20  19:18:38  salamone]
 * 
 * Revision 1.1.2.10  1994/07/05  15:45:36  kevins
 * 	OT11118 registry policy attributes are now show and modifyable
 * 	[1994/07/05  15:44:15  kevins]
 * 
 * Revision 1.1.2.9  1994/06/24  20:02:03  salamone
 * 	CR11056 - Slave replica specification not accepted.
 * 	[1994/06/24  20:01:06  salamone]
 * 
 * Revision 1.1.2.8  1994/06/20  21:03:01  salamone
 * 	Merged with changes from 1.1.2.7
 * 	[1994/06/20  21:02:42  salamone]
 * 
 * 	Added "registry show" and "registry dump" support.
 * 	[1994/06/20  19:43:46  salamone]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:22  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:56  devsrc]
 * 
 * Revision 1.1.2.6  1994/06/06  16:50:55  salamone
 * 	Enhanced registry modify support.
 * 	[1994/06/06  16:49:34  salamone]
 * 
 * Revision 1.1.2.5  1994/06/02  15:55:16  salamone
 * 	Moved and renamed  rgy_modify() and rgy_show() functions in
 * 	rgy.c to rgy_sa_tclif.c.
 * 	For all regi commands, if replica specified, then only one
 * 	can be specified.
 * 	For "regi stop" commands, must have an argument.
 * 	[1994/06/02  15:52:35  salamone]
 * 
 * Revision 1.1.2.4  1994/05/26  17:56:36  salamone
 * 	"regi delete" and "regi stop" need to mark the binding
 * 	cache invalid.
 * 	Some rgy cmds need to use the site_specific option to
 * 	open_rgy_site().
 * 	[1994/05/26  16:40:25  salamone]
 * 
 * Revision 1.1.2.3  1994/05/24  19:31:27  salamone
 * 	Added registry synchronize and delete support.
 * 	[1994/05/24  19:30:31  salamone]
 * 
 * Revision 1.1.2.2  1994/05/20  11:23:37  kevins
 * 	OT 10663 Added an argument into open_rgy_site()
 * 	[1994/05/20  08:59:58  kevins]
 * 
 * Revision 1.1.2.1  1994/05/12  18:29:39  salamone
 * 	Initial version of rgy_sa_tclif.c
 * 	[1994/05/12  18:29:00  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * rgy_sa_tclif.c
 *
 * DESCRIPTION:
 * This module contains all the "rgy" operations that deal with
 * replication information from a specified registry thru dcecp.
 */

/*
 * INCLUDE FILES
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dce/dns_record.h>
#ifdef FLAG_SET
#undef FLAG_SET
#endif
#include <dce/macros.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_error.h>

#include <dce/tcl.h>
#include <dcecp.h>
#include <util_var.h>
#include <util_general.h>
#include <util_help.h>
#include <util_signal.h>

#include <dce/policy.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <rgy_sa_replist.h>
#include <dce/keymgmt.h>
#include <dce/dce_cf_const.h>
#include <dce/rs_repadm.h>

/*
 * DECLARATIONS 
 */
         
/*
 * PROTOTYPES 
 */


/*
 * FUNCTION:
 * dcp_rgy_sa_catalog()
 *
 * OVERVIEW:
 * This routine returns a list of the names of the security servers
 * (i.e. each copy of the registry) running in the cell. This is
 * also known as the replica list. The order of output is arbitrary.
 * This command can accept one argument; a cell name or registry server.
 *
 * REGISTRY BINDING:
 *   If a replica is specified,
 *      access_type = site_specific
 *   If a cell is specified, and the cell is a foreign cell,
 *      access_type = site_specific
 *   If a cell is specified, and it is the local cell,
 *      access_type = site_query
 *   If nothing is specified,
 *      access_type = site_query
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_catalog(ClientData clientData, 
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    char            **largv;
    int             largc;
    boolean32       master_option = FALSE;
    uuid_t          rep_id;
    site_access_t   access_type;
    error_status_t  status = TCL_OK;

    dcp_ArgvInfo arg_table[] = {
        {"-master", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_sa_catalog_master_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    arg_table[0].dst = (char *)&master_option;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Set convenience variable if necessary.
     */
    if (argv[1] != NULL) {
        DCP_GENERAL_SET_NAMELIST(argv[1]);
    }

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Set the correct access type.
     */
    if (argv[1] != NULL) {
        char            *tmp_local_name = NULL;
        char            *tmp_cat_name = NULL;
        error_status_t  tmp_status;

        tmp_local_name = expand_cell_name(LOCAL_CELL, &tmp_status);
        tmp_cat_name = expand_cell_name(largv[0], &tmp_status);
        if (strcmp(tmp_local_name, tmp_cat_name) == 0) {
            access_type = site_query;
        }
        else {
            access_type = site_specific;
        }
        free(tmp_local_name);
        if (tmp_cat_name != NULL)
            free(tmp_cat_name);
    }
    else {
        access_type = site_query;
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], access_type) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    if ((*largv[0] != '\0') &&
        (strcmp(largv[0], (char *)LOCAL_CELL) != 0) &&
        (strcmp(largv[0], (char *)site_cache[realtime_cache].cell_name) != 0)) {
        status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                                  FALSE,
                                                  &rep_id);
        if (status == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
            free((char *)largv);
            return TCL_ERROR;
        }
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * The specified server is available and legal.  Display the
     * list of registry replicas.
     */
    dcp_rgy_sa_do_catalog(interp, master_option);

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_connect()
 *
 * OVERVIEW: This command will provide the necessary pgo and account information
 *    in two cells to provide for authenticated access between the cells.
 *    This command will create the group or organization if they do not exist.
 *    If this command fails it will remove the organization/group, if created,
 *    and remove the relevant principals.
 *
 * REGISTRY BINDING:
 *   A foreign cell name is required, but bind to master of local cell so
 *      access_type = site_update
 *
 * INPUTS:
 *  ClientData clientData
 *  int argc - number of arguments
 *  char **argv - arguments
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - error processing/parsing
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success
 */
int
dcp_rgy_sa_connect(ClientData clientData, 
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    error_status_t            result;
    error_status_t            tmp_result;
    sec_rgy_pgo_item_t        foreign_pgo_item;
    sec_rgy_pgo_item_t        local_pgo_item;
    sec_rgy_login_name_t      foreign_name;
    sec_rgy_login_name_t      local_name;
    sec_rgy_name_t            foreign_principal;
    boolean32                 reset_pwd;
    sec_rgy_name_t            cell_name;
    rgy_connect_info_t        connect;
    boolean32                 valid = FALSE;
    boolean32                 fvalid = FALSE;
    sec_rgy_bind_auth_info_t  auth_info;
    sec_login_auth_src_t      auth_src;
    sec_rgy_properties_t      foreign_properties;
    sec_rgy_properties_t      local_properties;
    sec_login_handle_t        flogin_context;
    sec_rgy_handle_t          foreign_context;
    sec_passwd_rec_t          pwd;
    sec_passwd_rec_t          *des_pwd;
    sec_passwd_rec_t          tmp_des_pwd;
    sec_passwd_str_t          tmp_pwd;
    char                      *tmp_name;
    extern boolean32          check_visible_pw;
    boolean32                 fprinc_created = FALSE;
    boolean32                 lprinc_created = FALSE;
    boolean32                 cell_valid = FALSE;
    boolean32                 context_valid = FALSE;
    char                      *command;
    char                      *local_command = "account modify /.:/%s -acctvalid yes";
    char                      *local_uncommand = "account modify /.:/%s -acctvalid no";
    char                      *foreign_command = "login %s/%s -p %s; account modify %s/%s -acctvalid yes";

    dcp_ArgvInfo arg_table[] = {
        {"-acctvalid", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_sa_connect_acctvalid_help},
        {"-expdate", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_expdate_help},
        {"-facct", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_facct_help},
        {"-facctpwd", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_facctpwd_help},
        {"-facctvalid", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_sa_connect_facctvalid_help},
        {"-fgroup", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_fgroup_help},
        {"-forg", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_forg_help},
        {"-group", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_group_help},
        {"-mypwd", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_mypwd_help},
        {"-org", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_sa_connect_org_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    /*
     * Initialize the relevant data structures.
     */
    memset(&foreign_name, 0, sizeof(foreign_name));
    memset(&local_name, 0, sizeof(local_name));
    memset(&connect, 0, sizeof(connect));
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&valid;
    arg_table[1].dst = (char *)&connect.expdate;
    arg_table[2].dst = (char *)&connect.facct;
    arg_table[3].dst = (char *)&connect.facctpwd;
    arg_table[4].dst = (char *)&fvalid;
    arg_table[5].dst = (char *)&connect.fgroup;
    arg_table[6].dst = (char *)&connect.forg;
    arg_table[7].dst = (char *)&connect.group;
    arg_table[8].dst = (char *)&connect.mypwd;
    arg_table[9].dst = (char *)&connect.org;

    /*
     * Parse the arguments.
     */
    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_cell_name);
    }
    strcpy((char *)cell_name, argv[1]);

    /*
     * Verify that the appropriate arguments have been provided.
     */
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(cell_name);

    if (!connect.mypwd) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_mypwd);
    }
    else if (check_visible_pw) {
        DCP_SET_MISC_ERROR(dcp_s_illegal_command);
    }

    if (!connect.facctpwd) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_fmypwd);
    }
    else if (check_visible_pw) {
        DCP_SET_MISC_ERROR(dcp_s_illegal_command);
    }

    if (!connect.org) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_org);
    }
    if (!connect.group) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_group);
    }
    if (!connect.forg) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_forg);
    }
    if (!connect.facct) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_faccount);
    }
    if (!connect.fgroup) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_fgroup);
    }

    /*
     * Open the local registry for update and create a connection to the 
     * foreign cell to get the properties of the cell. This is necessary 
     * to aquire the realm_uuid of the foreign cell. 
     */
    if (open_rgy_site(interp, (unsigned char *)LOCAL_CELL, site_update) != TCL_OK)
        return TCL_ERROR;

    auth_info.info_type = sec_rgy_bind_auth_none;
    sec_rgy_site_bind_update((unsigned char*)cell_name,
                             &auth_info,
                             &foreign_context,
                             &result);
    if (result == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, cell_name);
        return TCL_ERROR;
    }
    DCP_CHECK_SEC_ERROR(result);

    sec_rgy_properties_get_info(foreign_context, &foreign_properties, &result);
    sec_rgy_site_close(foreign_context, &tmp_result);
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Get the cell name from the argument passed in.
     */
    if (cell_name[GLOBAL_DIR_ROOT_LEN - 1] != '/') {
        DCP_APPEND_RESULT_CODE_ONE_ARG(dcp_s_rgy_con_bad_cell, cell_name);
        return TCL_ERROR; 
    }
    else
        tmp_name = (char *)&cell_name[GLOBAL_DIR_ROOT_LEN];

    /* 
     * Generate a principal pgo_item for creating the foreign principal     
     * in the local cell. Give the principal to be created the foreign 
     * cell's uuid. 
     */
    sprintf((char *)local_name.pname, "krbtgt/%s", tmp_name);
    local_pgo_item.id = foreign_properties.realm_uuid;
    local_pgo_item.unix_num = -1;
    local_pgo_item.quota = 0;
    local_pgo_item.flags = 0;
    strcpy((char *)local_pgo_item.fullname, (char *)cell_name);
    sec_rgy_pgo_add(rgy_context, 
                    sec_rgy_domain_person, 
                    local_name.pname, 
                    &local_pgo_item,
                    &result);
    if (result != sec_rgy_name_exists && result != error_status_ok) {
        DCP_SET_MISC_ERROR(result);
    }
    else if (result != sec_rgy_name_exists)
        lprinc_created = TRUE;

    sec_rgy_properties_get_info(rgy_context, &local_properties, &result);
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Generate a random key to use for account generation.
     */
    tmp_name = (char *)&local_properties.realm[GLOBAL_DIR_ROOT_LEN];
    sprintf((char *)foreign_name.pname, "krbtgt/%s", tmp_name);

    tmp_des_pwd.key.key_type = sec_passwd_des;
    sec_key_mgmt_gen_rand_key(rpc_c_authn_dce_secret,
                              (void *)NULL,
                              foreign_name.pname,
                              &tmp_des_pwd.key.key_type,
                              sec_passwd_c_version_none,
                              (void **)&des_pwd,
                              &result);
    DCP_CHECK_SEC_ERROR(result);
    pwd.key.key_type = sec_passwd_plain;
    pwd.version_number = sec_passwd_c_version_none;
    pwd.pepper = NULL;

    /*
     * Generate the foreign account on the local cell.
     * This must be done before obtaining a login context on the
     * foreign cell because this account is checked for validity.
     */
    strcpy((char *)local_name.gname, connect.group);
    strcpy((char *)local_name.oname, connect.org);
    pwd.key.tagged_union.plain = (unsigned char *)connect.mypwd;

    result = add_cell_account(rgy_context, local_name, pwd, des_pwd);
    if (result == sec_rgy_object_exists)
        result = replace_cell_account(rgy_context, local_name, pwd, des_pwd);
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Set the foreign account on the local cell 'valid', temporarily.
     */
    command = malloc(strlen(local_command) + strlen((char *)local_name.pname) + 1);
    sprintf(command, local_command, local_name.pname);
    if (Tcl_Eval(interp, command) != TCL_OK) {
        free(command);
        DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_set_valid);
    }
    free(command);

    /*
     * Generate a login context on the foreign cell so that we can create 
     * the necessary principal and group/org information. 
     */
    strncpy((char *)tmp_pwd, connect.facctpwd, sec_passwd_str_max_len);
    tmp_pwd[sec_passwd_str_max_len] = '\0';
    pwd.key.tagged_union.plain = &(tmp_pwd[0]);
    sprintf((char *)foreign_principal, "%s/%s", foreign_properties.realm, connect.facct);

    if (sec_login_setup_identity(foreign_principal, 
                                 sec_login_no_flags, 
                                 &flogin_context, 
                                 &result)) {
        if (sec_login_validate_identity(flogin_context,
                                        &pwd,
                                        &reset_pwd,
                                        &auth_src,
                                        &result)) {
            if (auth_src != sec_login_auth_src_network) {
                DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_cred);
            }
            context_valid = TRUE;
            DCP_CHECK_SEC_ERROR(result);
            auth_info.info_type = sec_rgy_bind_auth_dce;
            auth_info.tagged_union.dce_info.identity = flogin_context;
            auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
            auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
            auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
            sec_rgy_site_bind_update((unsigned char *)cell_name,
                                     &auth_info,
                                     &foreign_context,
                                     &result);
            DCP_CHECK_SEC_ERROR(result);
            cell_valid = TRUE;
        }
    }
    DCP_CHECK_SEC_ERROR(result);

    /* 
     * Add the local principal to the foreign cell. Use the local 
     * cell uuid for the foreign principal uuid. 
     */
    foreign_pgo_item.id = local_properties.realm_uuid;
    foreign_pgo_item.unix_num = -1;
    foreign_pgo_item.quota = 0;
    foreign_pgo_item.flags = 0;
    strcpy((char *)foreign_pgo_item.fullname, (char *)local_properties.realm);

    sec_rgy_pgo_add(foreign_context,
                    sec_rgy_domain_person,
                    foreign_name.pname,
                    &foreign_pgo_item,
                    &result);

    if (result != sec_rgy_status_ok && result != sec_rgy_name_exists) {
        if (lprinc_created) {
            sec_rgy_pgo_delete(rgy_context,
                               sec_rgy_domain_person,
                               local_name.pname,
                               &tmp_result);
            }
        DCP_CHECK_SEC_ERROR(result);
    }
    else if (result != sec_rgy_name_exists)
        fprinc_created = TRUE;

    /*
     * Generate the local account on the foreign cell.
     */
    strcpy((char *)foreign_name.gname, connect.fgroup);
    strcpy((char *)foreign_name.oname, connect.forg);
    pwd.key.tagged_union.plain = (unsigned char *)connect.facctpwd;

    result = add_cell_account(foreign_context, foreign_name, pwd, des_pwd);
    if (result == sec_rgy_object_exists)
        result = replace_cell_account(foreign_context, foreign_name, pwd, des_pwd);
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Set the local account on the foreign cell 'valid', if requested.
     * This must be done before foreign cell logins are potentially disabled.
     */
    if (fvalid == TRUE) {
        command = malloc(strlen(foreign_command) + (2 * strlen((char *)cell_name)) +
                         strlen((char *)connect.facct) + strlen((char *)connect.facctpwd) +
                         strlen((char *)foreign_name.pname) + 1);
        sprintf(command, foreign_command, cell_name, connect.facct,
                connect.facctpwd, cell_name, foreign_name.pname);
        if (Tcl_Eval(interp, command) != TCL_OK) {
            free(command);
            if (context_valid)
                sec_login_purge_context(&flogin_context, &tmp_result);
            if (cell_valid)
                sec_rgy_site_close(foreign_context, &tmp_result);
            DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_set_fvalid);
        }
        free(command);
    }

    /*
     * Reset the foreign account on the local cell 'invalid', unless requested.
     */
    if (valid == FALSE) {
        command = malloc(strlen(local_uncommand) + strlen((char *)local_name.pname) + 1);
        sprintf(command, local_uncommand, local_name.pname);
        if (Tcl_Eval(interp, command) != TCL_OK) {
            free(command);
            if (context_valid)
                sec_login_purge_context(&flogin_context, &tmp_result);
            if (cell_valid)
                sec_rgy_site_close(foreign_context, &tmp_result);
            DCP_SET_MISC_ERROR(dcp_s_rgy_con_no_set_valid);
        }
        free(command);
    }

    /*
     * Purge context for the foreign cell.
     */
    if (context_valid)
        sec_login_purge_context(&flogin_context, &tmp_result);

    /*
     * Close out the foreign cell.
     */
    if (cell_valid)
        sec_rgy_site_close(foreign_context, &tmp_result);

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_delete()
 *
 * OVERVIEW:
 * This routine, when called with no options, performs an orderly 
 * deletion of a security replica specified as an argument. That is, 
 * it binds to the master registry, marks the specified replica as 
 * deleted, propagates this deletion to the other replicas, sends a 
 * message to the specified replica to delete itself, and finally 
 * removes the replica from the master's replica list.
 * 
 * If the specified replica is unreachable, the -force option can be
 * used. This option causes the master to delete the specified replica
 * from the replica list and then propagates that deletion to the
 * other replicas on the list. The specified replica is NOT contacted.
 * When that replica is again accessible, the "registry destroy"
 * command should be called, specifying the replica. This causes that
 * replica to delete its copy of the registry database and stop running.
 *
 * REGISTRY BINDING:
 *   A replica name is required, but bind to master so
 *       access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_delete(ClientData clientData, 
                  Tcl_Interp *interp,
                  int argc,
                  char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  residual;
    error_status_t  status = TCL_OK;
    int             force_option = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-force", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_rgy_sa_delete_force_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&force_option;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica); 
    } 

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Must be bound to the master registry site for the cell.
     * Use the specified replica name to pull out the cell name,
     * so we bind to the correct cell master.
     */
    if (dcp_id_parse_name(interp,
                          (unsigned_char_p_t)largv[0],
                          cell_name,
                          residual) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Bind to master using cell name, not specified name.
     */
    if (dcp_rgy_sa_bind_named_object(interp, (char *)&cell_name, site_update) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0], 
                                              FALSE,
                                              &rep_id);
    if (status == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
        free((char *)largv);
        return TCL_ERROR;
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Delete the replica.
     */
    sec_rgy_replist_delete_replica(rgy_context, &rep_id, force_option, &status);

    /*
     * Invalidate the cache to the specified registry.
     */
    site_cache[realtime_cache].valid = FALSE;
    site_cache[realtime_cache].site[0] = '\0';

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_replace()
 *
 * OVERVIEW:
 *
 * REGISTRY BINDING:
 *   A replica name is required, but bind to master so
 *       access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_replace(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    int                     largc;
    char                    **largv;
    uuid_t                  rep_id;
    sec_rgy_name_t          residual;
    sec_rgy_name_t          cell_name;
    error_status_t          status = TCL_OK;
    int                     i;
    int		            address_listc;
    char                    **address_listv;
    char	            *address_list = NULL;
    rpc_binding_handle_t    binding;
    rpc_tower_vector_p_t    tower_vector_p;
    rs_replica_twr_vec_p_t  rep_twrs;
    unsigned32              cnt;
    unsigned32              rpcstatus;

    dcp_ArgvInfo arg_table[] = {
        {"-address", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_replace_address_help},
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&address_list;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica); 
    } 

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Must be bound to the master registry site for the cell.
     * Use the specified replica name to pull out the cell name,
     * so we bind to the correct cell master.
     */
    if (dcp_id_parse_name(interp,
                          (unsigned_char_p_t)largv[0],
                          cell_name,
                          residual) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Bind to master using cell name, not specified name.
     */
    if (dcp_rgy_sa_bind_named_object(interp, (char *)&cell_name, site_update) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0], 
                                              FALSE,
                                              &rep_id);
    if (status == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
        free((char *)largv);
        return TCL_ERROR;
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Make sure an address is specified.
     */
    if (address_list == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_missing_req_opts);
    }

    if (Tcl_SplitList(interp, address_list, &address_listc, &address_listv) != TCL_OK) {
        return TCL_ERROR;
    }

    rep_twrs = malloc(sizeof(rep_twrs) + address_listc * sizeof(rs_replica_twr_vec_t));
    if (rep_twrs == NULL) {
        free((char *)address_listv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    rep_twrs->num_towers = 0;

    cnt = 0;
    for (i = 0; i < address_listc; i++) {
        rpc_binding_from_string_binding((unsigned_char_p_t)address_listv[i],
                                        &binding,
                                        &status);
        if (BAD_STATUS(&status)) {
            free((char *)address_listv);
            free(rep_twrs);
            DCP_SET_MISC_ERROR(status); 
        }
        rpc_tower_vector_from_binding(rs_repadm_v1_2_c_ifspec,
                                      binding,
                                      &tower_vector_p,
                                      &rpcstatus);
        if (rpcstatus != rpc_s_ok) {
            free((char *)address_listv);
            free(rep_twrs);
            rpc_binding_free(&binding, &status);
            DCP_SET_MISC_ERROR(rpcstatus); 
        }

        if (tower_vector_p->count >= 1) {
            /*
             * Copy it to stub memory.
             */
            rep_twrs->towers[cnt] = malloc(
        	    tower_vector_p->tower[0]->tower_length + sizeof(twr_t));
            if (rep_twrs->towers[cnt]) {
                COPY_bytes(tower_vector_p->tower[0],
                           rep_twrs->towers[cnt],
                           tower_vector_p->tower[0]->tower_length + sizeof(twr_t));
                ++cnt;
            } else {
                free((char *)address_listv);
                free(rep_twrs);
                rpc_binding_free(&binding, &status);
                rpc_tower_vector_free(&tower_vector_p, &rpcstatus);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
        }
    }
    rep_twrs->num_towers = cnt;
    rpc_binding_free(&binding, &status);
    rpc_tower_vector_free(&tower_vector_p, &rpcstatus);

    /*
     * Replace the replica address information.
     */
    sec_rgy_replist_replace_replica(rgy_context, 
                                    &rep_id, 
                                    (unsigned_char_p_t)residual, 
                                    rep_twrs, 
                                    &status);
    free((char *)address_listv);
    for (i = 0 ; i < cnt; i++)
        free(rep_twrs->towers[i]);
    free(rep_twrs);

    /*
     * Invalidate the cache to the specified registry.
     */
    site_cache[realtime_cache].valid = FALSE;
    site_cache[realtime_cache].site[0] = '\0';

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_disable()
 *
 * OVERVIEW:
 * This routine disables the master registry for updates. This is 
 * called "maintenance mode" in sec_admin. The argument is a single
 * registry master replica to be disabled.
 *
 * REGISTRY BINDING:
 *   A replica name is optional, but bind to master so
 *       access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_disable(ClientData clientData, 
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    error_status_t  status = TCL_OK;

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Set convenience variable if necessary.
     */
    if (argv[1] != NULL) {
        DCP_GENERAL_SET_NAMELIST(argv[1]);
    }

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_update) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is the master replica; rep_id is ignored.
     */
    if (*largv[0] != '\0') {
        status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                                  TRUE,
                                                  &rep_id);
        if (status == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
            free((char *)largv);
            return TCL_ERROR;
        }
    } 
    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    sec_rgy_rep_admin_maint(rgy_context, TRUE, &status);
    DCP_CHECK_SEC_ERROR(status);

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_enable()
 *
 * OVERVIEW:
 * This routine enables the master registry for updates. This is 
 * called "server mode" in sec_admin. The argument is a single
 * registry master replica to be enabled.  Read-only copies are
 * not allowed, but the name of other cells can be used.
 *
 * REGISTRY BINDING:
 *   A replica name is optional, but bind to master so
 *       access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_enable(ClientData clientData, 
                  Tcl_Interp *interp,
                  int argc,
                  char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    error_status_t  status = TCL_OK;

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;
	       
    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Set convenience variable if necessary.
     */
    if (argv[1] != NULL) {
        DCP_GENERAL_SET_NAMELIST(argv[1]);
    }

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
	DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_update) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is the master replica; rep_id is ignored.
     */
    if (*largv[0] != '\0') {
        status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                                  TRUE,
                                                  &rep_id);
        if (status == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
            free((char *)largv);
            return TCL_ERROR;
        }
    } 
    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    sec_rgy_rep_admin_maint(rgy_context, FALSE, &status);
    DCP_CHECK_SEC_ERROR(status);

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_stop()
 *
 * OVERVIEW:
 * This routine stops the specified security server process.
 * The argument is a single registry server.
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * REGISTRY BINDING:
 *   A replica name is required, bind to specified replica
 *       access_type = site_specific
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_stop(ClientData clientData,
                Tcl_Interp *interp,
                int argc,
                char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    error_status_t  status = TCL_OK;

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica); 
    } 

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_specific) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0], 
                                              FALSE,
                                              &rep_id);
    if (status == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
        free((char *)largv);
        return TCL_ERROR;
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Stop the specified replica.
     */
    sec_rgy_rep_admin_stop(rgy_context, &status);

    /*
     * Invalidate the cache to the specified registry.
     */
    site_cache[realtime_cache].valid = FALSE;
    site_cache[realtime_cache].site[0] = '\0';

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_synchronize()
 *
 * OVERVIEW:
 * This routine re-initializes the specified replicas with an 
 * up-to-date copy of the database. The argument is a single
 * registry replica to operate on. 
 *
 * This command binds to the master, marks the specified replica
 * for re-initialization, sends a message to the replica informing it
 * to reinitialize itself and providing a list fo other replicas
 * with up-to-date copies of the registry. The specified replica
 * then selects a replica from the list and asks fo a copy of the 
 * database.
 *
 * REGISTRY BINDING:
 *   A replica name is required, but bind to master so
 *       access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_synchronize(ClientData clientData, 
                       Tcl_Interp *interp,
                       int argc,
                       char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    sec_rgy_name_t  residual;
    sec_rgy_name_t  cell_name;
    error_status_t  status = TCL_OK;

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica); 
    } 

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Must be bound to the master registry site for the cell.
     * Use the specified replica name to pull out the cell name,
     * so we bind to the correct cell master.
     */
    if (dcp_id_parse_name(interp,
                          (unsigned_char_p_t)largv[0],
                          cell_name,
                          residual) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Bind to master using cell name, not specified name.
     */
    if (dcp_rgy_sa_bind_named_object(interp, (char *)&cell_name, site_update) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0], 
                                              FALSE,
                                              &rep_id);
    if (status == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
        free((char *)largv);
        return TCL_ERROR;
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Do the synchronize.
     */
    sec_rgy_rep_admin_init_replica(rgy_context, &rep_id, &status);

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_show()
 *
 * OVERVIEW: 
 * This routine returns information about the registry and its
 * replicas. Takes an optional argument specifying a single 
 * registry replica to contact. Specifying an argument is the
 * same as setting the _s(sec) convenience variable before
 * executing a "registry show" command without any argument.
 *
 * If called with no options or with the '-attributes' option, 
 * returns an attribute list of all the registry-wide attributes.
 *
 * If called with the '-policies' option, returns an attribute list of
 * all the registry-wide policies.
 *
 * If called with the '-replica' option, returns replica data
 * that is kept by the replica specified in the argument or in
 * _s(sec).  Returns data from the 'rep_admin_info_full()' call.
 *
 * If called with the '-master' option, the master is contacted and
 * the propagation information that the master keeps for each
 * slave is returned.  In this case the argument, if present, must
 * be the master, or an error is returned.  Returns data from the
 * 'replist_read_full()' call.
 *
 * If called with the '-replica' and '-verbose' options, the specified
 * replica is contacted and propagation information that the replica
 * keeps for itself and the other replicas is returned.  The argument
 * can be the master or a slave.  Returns data from the 'replist_read()'
 * call.
 *
 * All the options to this command are mutually exclusive.
 *
 * REGISTRY BINDING:
 *   If the master option is selected,
 *      access_type = site_update
 *   If a replica is specified,
 *      access_type = site_specific
 *   If a cell is specified, and the cell is a foreign cell,
 *      access_type = site_specific
 *   If a cell is specified, and it is the local cell,
 *      access_type = site_query
 *   If nothing is specified,
 *      access_type = site_query
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp_rgy_sa_show(ClientData clientData, 
                Tcl_Interp *interp, 
                int argc,
                char **argv)
{
    sec_rgy_name_t             global_name;  
    sec_rgy_login_name_t       account_name;
    error_status_t             result = TCL_OK;
    sec_rgy_properties_t       properties;
    sec_rgy_plcy_t             reg_policy;
    sec_rgy_plcy_auth_t        auth_policy;
    boolean32                  attributes = FALSE;
    boolean32                  policies = FALSE;
    boolean32                  master_option = FALSE;
    boolean32                  replica_option = FALSE;
    boolean32                  verbose_option = FALSE;
    int                        rc = TCL_OK;
    int                        largc;
    char                       **largv;
    int                        option_cnt = 0;
    int                        access_type = site_query;
    uuid_t                     rep_id;
    rs_replica_info_t          rgy_info;
    rs_replica_twr_vec_p_t     rtwrs = NULL;
    rs_replica_twr_vec_p_t     mtwrs = NULL;
    unsigned32                 num_vers = 0;
    rs_replica_sw_vers_info_t  *sw_version;
    unsigned32                 current_vers = 0;

    dcp_ArgvInfo arg_table[] = {
        {"-attributes", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
             dcp_t_rgy_sa_show_attr_help},
        {"-master", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
             dcp_t_rgy_sa_show_master_help}, 
        {"-policies", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
             dcp_t_rgy_sa_show_policies_help},
        {"-replica", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
             dcp_t_rgy_sa_show_replica_help}, 
        {"-verbose", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
             dcp_t_rgy_sa_show_verbose_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0} 
    };

    arg_table[0].dst = (char *)&attributes;
    arg_table[1].dst = (char *)&master_option;
    arg_table[2].dst = (char *)&policies;
    arg_table[3].dst = (char *)&replica_option;
    arg_table[4].dst = (char *)&verbose_option;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check for syntax errors.  If no options are specified, just
     * display the registry attributes.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Check that 'verbose' isn't being used with anything
     * but 'replica'.
     */
    if (verbose_option && (attributes || policies || master_option)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_bad_use_of_verbose);
    }

    /*
     * Check that if 'verbose' is present, 'replica' is also present.
     */
    if (verbose_option && !replica_option) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_verbose_missing_replica);
    }

    /*
     * Check other mutually exclusive options.
     */
    if (attributes) {
        option_cnt++;
    } 
    if (policies) {
        option_cnt++;
    }
    if (replica_option) {
        option_cnt++;
    }
    if (master_option) {
        option_cnt++;
        access_type = site_update;
    }

    if (option_cnt > 1) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }
    else if (option_cnt == 0) {
        attributes = TRUE;
    }

    /*
     * Set convenience variable if necessary.
     */
    if (argv[1] != NULL) {
        DCP_GENERAL_SET_NAMELIST(argv[1]);
    }

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Set the correct access type.
     */
    if ((!master_option) && (argv[1] != NULL)) {
        char            *tmp_local_name = NULL;
        char            *tmp_show_name = NULL;
        error_status_t  tmp_status;

        tmp_local_name = expand_cell_name(LOCAL_CELL, &tmp_status);
        tmp_show_name = expand_cell_name(largv[0], &tmp_status);
        if (strcmp(tmp_local_name, tmp_show_name) == 0) {
            access_type = site_query;
        }
        else {
            access_type = site_specific;
        }
        free(tmp_local_name);
        if (tmp_show_name != NULL)
            free(tmp_show_name);
    }
    else {
        access_type = site_query;
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], access_type) != TCL_OK) {
       free((char *)largv);
       return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    if ((*largv[0] != '\0') &&
        (strcmp(largv[0], (char *)LOCAL_CELL) != 0) &&
        (strcmp(largv[0], (char *)site_cache[realtime_cache].cell_name) != 0)) {
        result = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                                  master_option,
                                                  &rep_id);
        if (result == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
            free((char *)largv);
            return TCL_ERROR;
        }
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Display registry or replica information, depending
     * on the command line options specified.
     */
    if (master_option) {
        rc = dcp_rgy_sa_do_show_master_info(interp);
    }
    else if (replica_option && verbose_option) {
        rc = dcp_rgy_sa_do_show_replist_info(interp);
    }
    else if (replica_option) {
        rc = dcp_rgy_sa_do_show_replica_info(interp);
    }
    else if (attributes) { 
        sec_rgy_properties_get_info(rgy_context, &properties, &result);
        DCP_CHECK_SEC_ERROR(result);

        /*
         * The security server version is one of the attributes required,
         * but is not available through the 'properties' data structure.
         * Use the following call to get the current security server version.
         */
        sec_rgy_rep_admin_info_vers(rgy_context,
                                    &rgy_info,
                                    &rtwrs,
                                    &mtwrs,
                                    &num_vers,
                                    &sw_version,
                                    &current_vers,
                                    &result);
        DCP_CHECK_SEC_ERROR(result);
    }
    else if (policies) {
        global_name[0] = '\0';
        sec_rgy_plcy_get_info(rgy_context, global_name, &reg_policy, &result);
        DCP_CHECK_SEC_ERROR(result);

        memset((char *)&account_name, 0, sizeof(sec_rgy_login_name_t));
        sec_rgy_auth_plcy_get_info(rgy_context,
                                   &account_name,
                                   &auth_policy,
                                   &result);
        DCP_CHECK_SEC_ERROR(result);
    }
    if (policies || attributes) {
        rc = format_registry_prop(interp,
                                  attributes,
                                  policies,
                                  &properties,
                                  &rgy_info.sw_rev,
                                  &reg_policy,
                                  &auth_policy);
    }

    return rc;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_dump()
 *
 * OVERVIEW:
 * This routine returns the replica information for each replica in
 * the cell.  _b(sec) is set to the name of the last replica
 * displayed.  Replicas are displayed with a blank line between them.
 * It takes one argument, and the order of output is arbitrary.
 *
 * REGISTRY BINDING:
 *   This function is a tcl script which uses 'registry show' and
 *   'registry catalog', so the access type will depend on those
 *   functions.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_dump(ClientData clientData, 
                Tcl_Interp *interp,
                int argc,
                char **argv)
{
    unsigned32  result = error_status_ok;
    int         largc;
    char        **largv;
    char        command[1024];

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Invoke internal dcecp procedure _dcp_registry_dump.
     */
    if (argv[1] == NULL) {
        sprintf(command, "_dcp_registry_dump NULL");
    }
    else {
        sprintf(command, "_dcp_registry_dump %s", largv[0]);
    }
    if ((result = Tcl_Eval(interp, command)) != TCL_OK) {
        free((char *)largv);
        DCP_SET_RESULT(interp->result);
        return TCL_ERROR;
    }

    free((char *)largv);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_verify()
 *
 * OVERVIEW:
 * This routine checks if all registry replicas are up-to-date.
 * If so, returns an empty string. If not, a list of out-of-date 
 * replica names (or bindings if names are not available)
 * is returned.  Note that one element from this list is suitable for 
 * use as an argument to the "registry synchronize" command.
 * _b(sec) is set to the name of the last replica displayed.  
 * It takes one argument, and the order of output is arbitrary.
 *
 * REGISTRY BINDING:
 *   This function is a tcl script which uses 'registry show' and
 *   'registry catalog', so the access type will depend on those
 *   functions.
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_verify(ClientData clientData, 
                  Tcl_Interp *interp,
                  int argc,
                  char **argv)
{
    unsigned32  result = error_status_ok;
    int         largc;
    char        **largv;
    char        command[1024];

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * Invoke internal dcecp procedure _dcp_registry_verify.
     */
    if (argv[1] == NULL) {
        sprintf(command, "_dcp_registry_verify NULL");
    }
    else {
        sprintf(command, "_dcp_registry_verify %s", largv[0]);
    }
    if ((result = Tcl_Eval(interp, command)) != TCL_OK) {
        free((char *)largv);
        DCP_SET_RESULT(interp->result);
        return TCL_ERROR;
    }

    free((char *)largv);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_modify()
 *
 * OVERVIEW: 
 * This command is used to change attributes of the master registry
 * only. Using the command for this purpose, it takes no arguments.
 *
 * The -change option can be used to modify the value of any one of
 * the following attributes (mingid, minorgid, minuid, maxuid), any
 * one of the following policies (deftktlife, mintktlife, hidepwd),
 * or any registry ERA (when supported).
 * Also, supports attribute options. The -add and -remove options
 * can only be used to specify any registry ERA's, but not the above
 * listed attributes, as each of them must always have a value.
 *
 * Also accepts the -key option to generate a new master key for 
 * the replica specified in the argument and to encrypt account
 * keys using the new key. The new master key is randomly 
 * generated. Note that each replica (master and slave) maintains
 * its own master key used to access the data in its copy
 * of the database.
 *
 * REGISTRY BINDING:
 *   If the -key option is used,
 *      access_type = site_specific
 *   Otherwise, for all other modify commands,
 *      access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */

int dcp_rgy_sa_modify(ClientData clientData, 
                      Tcl_Interp *interp, 
                      int argc,
                      char **argv)
{
    error_status_t        result = TCL_OK;
    char                  *change = NULL;
    sec_rgy_name_t        global_name;
    sec_rgy_login_name_t  login_name;
    rgy_policy_info_t     policy;
    rgy_policy_info_t     zero_policy;
    sec_rgy_properties_t  properties;
    sec_rgy_plcy_t        reg_policy;
    sec_rgy_plcy_auth_t   auth_policy;
    int                   master_replica_key = 0;
    int                   largc;
    char                  **largv;
    uuid_t                rep_id;

    dcp_ArgvInfo arg_table[] = {
        {"-acctlife", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_acctlife_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_change_help},
        {"-deftktlife", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_deftktlife_help},
        {"-hidepwd", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_hidepwd_help},
        {"-key", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
             dcp_t_rgy_sa_mod_key_help}, 
        {"-maxtktlife", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_maxtktlife_help},
        {"-maxtktrenew", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_maxtktrenew_help},
        {"-maxuid", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_maxuid_help},
        {"-mingid", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_mingid_help},
        {"-minorgid", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_minorgid_help},
        {"-mintktlife", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_mintktlife_help},
        {"-minuid", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_minuid_help},
        {"-pwdalpha", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_pwdalpha_help},
        {"-pwdexpdate", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_pwdexpdate_help},
        {"-pwdlife", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_pwdlife_help},
        {"-pwdminlen", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_pwdminlen_help},
        {"-pwdspaces", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_pwdspaces_help},
        {"-version", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_rgy_sa_mod_version_help},
       {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    memset(&policy, 0, sizeof(policy));
    memset(&zero_policy, 0, sizeof(zero_policy));
    memset(&login_name, 0, sizeof(login_name));

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&policy.acctlife;
    arg_table[1].dst = (char *)&change;
    arg_table[2].dst = (char *)&policy.deftktlife;
    arg_table[3].dst = (char *)&policy.hidepwd;
    arg_table[4].dst = (char *)&master_replica_key;
    arg_table[5].dst = (char *)&policy.maxtktlife;
    arg_table[6].dst = (char *)&policy.maxtktrenew;
    arg_table[7].dst = (char *)&policy.maxuid;
    arg_table[8].dst = (char *)&policy.mingid;
    arg_table[9].dst = (char *)&policy.minorgid;
    arg_table[10].dst = (char *)&policy.mintktlife;
    arg_table[11].dst = (char *)&policy.minuid;
    arg_table[12].dst = (char *)&policy.pwdalpha;
    arg_table[13].dst = (char *)&policy.expdate;
    arg_table[14].dst = (char *)&policy.pwdlife;
    arg_table[15].dst = (char *)&policy.pwdminlen;
    arg_table[16].dst = (char *)&policy.pwdspaces;
    arg_table[17].dst = (char *)&policy.version;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if ((!master_replica_key) &&
        (!change) &&
        (!memcmp(&policy, &zero_policy, sizeof(policy)))) {
        DCP_SET_MISC_ERROR(dcp_s_need_opt);
    }
    else if ((argc < 2) && (master_replica_key)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica);
    } 
    else if ((master_replica_key) && 
             ((change) || (memcmp(&policy, &zero_policy, sizeof(policy))))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Set convenience variable if necessary.
     */
    if (argv[1] != NULL) {
        DCP_GENERAL_SET_NAMELIST(argv[1]);
    }

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    if (master_replica_key) {
        result = dcp_rgy_sa_do_modify_key(interp, (char *)largv[0]);
        free((char *)largv);
        return result;
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_update) != TCL_OK) {
       free((char *)largv);
       return TCL_ERROR;
    }

    /*
     * Determine if specified replica is the master replica; rep_id is ignored.
     */
    if (*largv[0] != '\0') {
        result = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                                  TRUE,
                                                  &rep_id);
        if (result == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
            free((char *)largv);
            return TCL_ERROR;
        }
    } 
    free((char *)largv);
    DCP_CHECK_SEC_ERROR(result);

    sec_rgy_properties_get_info(rgy_context, &properties, &result);
    DCP_CHECK_SEC_ERROR(result);
    policy.properties = &properties; 

    global_name[0] = '\0';
    sec_rgy_plcy_get_info(rgy_context, global_name, &reg_policy, &result);
    DCP_CHECK_SEC_ERROR(result);
    policy.policy = &reg_policy;

    sec_rgy_auth_plcy_get_info(rgy_context, 
                               &login_name, 
                               &auth_policy, 
                               &result);
    DCP_CHECK_SEC_ERROR(result);
    policy.auth_policy = &auth_policy;

    policy.list = change;

    if (parse_registry_prop_list(interp, &policy) != TCL_OK)
        return TCL_ERROR;

    if (update_registry_properties(interp, &policy) != TCL_OK)
        return TCL_ERROR;

    sec_rgy_properties_set_info(rgy_context, &properties, &result);
    DCP_CHECK_SEC_ERROR(result);
    sec_rgy_plcy_set_info(rgy_context, global_name, &reg_policy, &result);
    DCP_CHECK_SEC_ERROR(result);

    sec_rgy_auth_plcy_set_info(rgy_context, 
                               &login_name, 
                               &auth_policy, 
                               &result);
    DCP_CHECK_SEC_ERROR(result);

    if (change)
        free_policy_attr(&policy);

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_designate()
 *
 * OVERVIEW:
 * This routine changes which replica is the master.  The argument is
 * the name of a single registry replica. The command returns the
 * empty string on success.
 *
 * When called with no options, the command changes the replica named 
 * in the argument to be the new master for the cell and changes
 * the existing master to be a slave.
 * 
 * Can also be called with either a -slave or -master option in which 
 * case it changes the type of replica specified as an argument to what 
 * the option specifies.  
 * If the -master option is used, other slaves are checked to see
 * if they are more up-to-date than the one specified in the argument.
 * If any are, the command fails with an error unless the -force option 
 * is used.
 *
 * The value of _b(sec) is changed in the following ways.
 * If called with no options, then it is set to specify the old master
 * replica.  If called with either -slave or -master then it is set to 
 * the replica specified in the argument.  
 *
 * REGISTRY BINDING:
 *   If the -master or -slave options are used,
 *      access_type = site_specific
 *   Otherwise,
 *      access_type = site_update
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_designate(ClientData clientData, 
                     Tcl_Interp *interp,
                     int argc,
                     char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    sec_rgy_name_t  residual;
    sec_rgy_name_t  cell_name;
    error_status_t  status = TCL_OK;
    int             force_option = FALSE;
    int             master_option = FALSE;
    int             slave_option = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-force", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_rgy_sa_designate_force_help}, 
        {"-master", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_rgy_sa_designate_master_help}, 
        {"-slave", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_rgy_sa_designate_slave_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&force_option;
    arg_table[1].dst = (char *)&master_option;
    arg_table[2].dst = (char *)&slave_option;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica); 
    }

    /*
     * Some options are mutually exclusive.
     */
    if ((master_option) && (slave_option)) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }
    if ((force_option) && (!master_option)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_force_set_master); 
    }

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    /*
     * If no option is specified, must be bound to the master replica
     * of the local cell.  If the '-master' or '-slave' options are used,
     * then we bind to the replica specified on the command line.
     */
    if ((!master_option) && (!slave_option)) {
        /*
         * Must be bound to the master registry site for the cell.
         * Use the specified replica name to pull out the cell name,
         * so we bind to the correct cell master.
         */
        if (dcp_id_parse_name(interp,
                              (unsigned_char_p_t)largv[0],
                              cell_name,
                              residual) != TCL_OK) {
            free((char *)largv);
            return TCL_ERROR;
        }

        /*
         * Bind to master using cell name, not specified name.
         */
        if (dcp_rgy_sa_bind_named_object(interp, (char *)&cell_name, site_update) != TCL_OK) {
            free((char *)largv);
            return TCL_ERROR;
        }
    }
    else {
        /*
         * Bind to the replica specified on the command line.
         */
        if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_specific) != TCL_OK) {
            free((char *)largv);
            return TCL_ERROR;
        }
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0], 
                                              FALSE,
                                              &rep_id);
    if (status == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
        free((char *)largv);
        return TCL_ERROR;
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    if (master_option) {
        /*
         * Doing a 'become master'.  The replica specified is to
         * change from slave to master.  With the force option,
         * just make the correct call.  Without the force option,
         * check to see if the slave being changed to the master
         * is the most up to date.
         */
        if (!force_option) {
            if (dcp_rgy_sa_check_last_upd_seqno(interp) != TCL_OK) {
                return TCL_ERROR;
            }
        }
        sec_rgy_rep_admin_become_master(rgy_context, &status);
    }
    else if (slave_option) {
        /*
         * Doing a 'become slave'.  The replica specified is to
         * change from master to slave.
         */
        sec_rgy_rep_admin_become_slave(rgy_context, &status);
    }
    else {
        /*
         * Doing a 'change master'.  The replica specified is to
         * change from slave to master, while the current master
         * will change to a slave.  This is the preferred way of
         * doing things, since it is done in an orderly manner.
         */
        if (dcp__rgy_sa_find_master_by_uuid(&rep_id)) {
            DCP_SET_MISC_ERROR(dcp_s_rgy_sa_already_master); 
        }
        sec_rgy_rep_admin_change_master(rgy_context, &rep_id, &status);
    }

    /*
     * Invalidate the cache to the specified registry.
     */
    site_cache[realtime_cache].valid = FALSE;
    site_cache[realtime_cache].site[0] = '\0';

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_destroy()
 *
 * OVERVIEW:
 * This routine will cause the specified replica to destroy itself.
 * That is, to destroy its copy of the registry database and stop
 * running.  It does not contact or inform the master of its
 * changed status.  Use the registry delete command to perform
 * a more orderly deletion of a security replica.
 *
 * REGISTRY BINDING:
 *   A replica name is required, bind to specified replica
 *       access_type = site_specific
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguements.
 *
 * INPUT/OUTPUTS:
 * interp  Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_destroy(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    char            **largv;
    int             largc;
    uuid_t          rep_id;
    error_status_t  status = TCL_OK;

    dcp_ArgvInfo arg_table[] = {
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica);
    }

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica);
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_specific) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                              FALSE,
                                              &rep_id);
    if (status == sec_rgy_server_unavailable) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
        free((char *)largv);
        return TCL_ERROR;
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Destroy the specified replica.
     */
    sec_rgy_rep_admin_destroy(rgy_context, &status);

    /*
     * Invalidate the cache to the specified registry.
     */
    site_cache[realtime_cache].valid = FALSE;
    site_cache[realtime_cache].site[0] = '\0';

    DCP_CHECK_SEC_ERROR(status);
    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_checkpoint()
 *
 * OVERVIEW:
 * This routine changes the rgy checkpoint behavior.  The command 
 * returns the empty string on success.
 *
 * When called with no options, the command issue a rgy checkpoint
 * immediately.
 *
 * The command requires as an argument the name of the registry to
 * perform the operation on.  If no registry name is specified,
 * an error is generated.
 *
 * Can also be called with either a -now or in conjunction with 
 * -cpi or -at option in which cases:
 * 	-now : issue a rgy checkpoint immediately (no args)
 *	-cpi : issue a new checkpoint interval, its value in arg
 *             and its syntax is <number> (in seconds) or <number>m
 *             (in minutes) or <number>h (in hours).
 *	-at  : issue a checkpoint at specific time of the day
 *	       	the time is in arg and its syntax is
 *		HH:MM where HH is [0-23 *] and MM is [0-59 *]
 *
 * REGISTRY BINDING:
 *   A replica name is required, bind to specified replica
 *       access_type = site_specific
 *
 * INPUTS:
 *  clientData Client specific data.
 *  argc       The number of command line arguments passed into function.
 *  argv       The actual arguments.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convienience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_rgy_sa_checkpoint(ClientData clientData, 
                      Tcl_Interp *interp,
                      int argc,
                      char **argv)
{
    char            **largv;
    int             largc;
    error_status_t  status = TCL_OK;
    int             now_option = FALSE;
    char            *at_time_str = NULL;
    char            *cpi_time_str = NULL;
    char            time_str_null = '\0';
    signed32        do_checkpt_now = FALSE;
    unsigned32      new_interval = 0;
    unsigned32      wait_time = 0;
    uuid_t          rep_id;

    dcp_ArgvInfo arg_table[] = {
        {"-at", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_rgy_sa_checkpoint_at_help}, 
        {"-cpi", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_rgy_sa_checkpoint_cpi_help}, 
        {"-now", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_rgy_sa_checkpoint_now_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&at_time_str;
    arg_table[1].dst = (char *)&cpi_time_str;
    arg_table[2].dst = (char *)&now_option;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check for syntax errors.
     */
    DCP_CHECK_EXTRA_ARGS();

    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_no_replica); 
    } 

    /*
     * Set convenience variable.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Some options are mutually exclusive.
     */
    if ((at_time_str != NULL) && (cpi_time_str != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Split the list of registry replicas properly.
     */
    if (dcp_rgy_sa_list_replicas(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((largc == 0) && (largv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    /*
     * Only one registry replica can be specified.
     */
    if (largc > 1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_sa_only_one_replica); 
    }

    if (now_option) {
        do_checkpt_now = TRUE;
    }

    if (cpi_time_str != NULL) {
        /*
         * Convert this string to a time in seconds.
         */
        if (dcp_rgy_checkpt_parse_cpi_time(interp, cpi_time_str, &new_interval) == TCL_ERROR) {
            free((char *)largv);
            DCP_SET_MISC_ERROR(dcp_s_rgy_checkpoint_bad_cpi);
        }
    }

    if (at_time_str == NULL) {
        at_time_str = &time_str_null;
    }

    /*
     * Bind to the replica specified on the command line.
     */
    if (dcp_rgy_sa_bind_named_object(interp, largv[0], site_specific) != TCL_OK) {
        free((char *)largv);
        return TCL_ERROR;
    }

    /*
     * Determine if specified replica is in the master replica list.
     */
    if ((*largv[0] != '\0') &&
        (strcmp(largv[0], (char *)LOCAL_CELL) != 0) &&
        (strcmp(largv[0], (char *)site_cache[realtime_cache].cell_name) != 0)) {
        status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)largv[0],
                                                  FALSE,
                                                  &rep_id);
        if (status == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable, largv[0]);
            free((char *)largv);
            return TCL_ERROR;
        }
    }

    free((char *)largv);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Set the checkpoint interval.
     */
    sec_rgy_checkpt_reset_interval(rgy_context,
                                   do_checkpt_now,
                                   new_interval,
                                   at_time_str,
                                   &status);
    DCP_CHECK_SEC_ERROR(status);

    return TCL_OK;
}
