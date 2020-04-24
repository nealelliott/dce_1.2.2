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
 * $Log: rgy_pgo_group.c,v $
 * Revision 1.1.6.2  1996/08/09  11:49:22  arvind
 * 	Merge changes.
 * 	[1996/06/11  17:03 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/2]
 *
 * 	Minor changes.
 * 	[1996/06/11  16:37 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:46 UTC  truitt  /main/HPDCE02/14]
 *
 * 	Merge changes.
 * 	[1996/03/28  18:44 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	CHFts17168: The account, pgo, registry and schema calls all have serious
 * 	problems when called with an argument of " ".  Note they work correctly if the
 * 	argument is {" "}.  Trap the first type of argument as illegal.
 * 	[1996/03/20  21:44 UTC  truitt  /main/HPDCE02/13]
 *
 * 	Merge changes.
 * 	[1996/03/20  21:43 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf70/1]
 *
 * 	CHFts17733: Some complication with registry cursors that currently cannot
 * 	be fixed is causing problems for dcecp in some rare occasions.  In order to
 * 	work around this problem until registry cursors are completely fixed, we'll watch
 * 	the output from the api calls that use the rgy cursor and make sure we trap
 * 	and handle the correct errors.
 * 	[1996/03/18  18:26 UTC  truitt  /main/HPDCE02/12]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:17 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:25 UTC  truitt  /main/HPDCE02/11]
 *
 * 	Merge brief changes.
 * 	[1996/02/15  18:23 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 * 	[1996/02/06  21:12 UTC  truitt  /main/HPDCE02/10]
 *
 * 	Merge changes.
 * 	[1996/02/06  21:11 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 *
 * 	CHFts16677: Make sure the correct error message is displayed
 * 	when an invalid cellname is passed to a 'catalog' operation.
 * 	[1996/02/05  16:06 UTC  truitt  /main/HPDCE02/9]
 *
 * 	Merge changes.
 * 	[1996/02/05  16:04 UTC  truitt  /main/HPDCE02/truitt_dcecp_17281/1]
 *
 * 	CHFts16995: Have 'inprojlist' default correctly.
 * 	CHFts17281: Use the correct information from the parsed
 * 	global name instead of resetting the cell name.
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:49 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	Modify rgy_group_list() to not strip foreign cell names with
 * 	-simplename, and prepend the correct cell name too.
 * 	[1996/03/01  18:25 UTC  jrr  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	multi-cell group work: modify rgy_group_add and rgy_group_remove to accept
 * 	global member names.
 * 	[1996/01/31  16:25 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:41  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:56:43  root]
 *
 * 	Merge.
 * 	HP 	[1995/06/16  18:11 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts35/1]
 *
 * 	CHFts15504: Remove unneeded includes.
 * 	HP 	[1995/05/01  19:05 UTC  truitt  /main/HPDCE02/7]
 *
 * 	Merge era work.
 * 	HP 	[1995/05/01  19:04 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts26/1]
 *
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 	HP 	[1995/04/05  19:52 UTC  truitt  /main/HPDCE02/6]
 *
 * 	Merge schema message work.
 * 	HP 	[1995/04/05  19:51 UTC  truitt  /main/HPDCE02/truitt_mothra_chfts24/1]
 *
 * 	CHFts14652: Add improved error message.
 * 	HP 	[1995/03/28  15:04 UTC  jrr  /main/HPDCE02/5]
 *
 * 	Make org list and group list more efficient (CR12822).
 * 	HP 	[1995/03/28  15:02 UTC  jrr  /main/HPDCE02/jrr_dcecp_cr12822/1]
 *
 * 	Make org list and group list more efficient (CR12822).
 * 	HP 	[1995/03/09  14:32 UTC  truitt  /main/HPDCE02/4]
 *
 * 	Check for valid cellname in group catalog.
 * 	HP 	[1995/03/09  14:31 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts18/1]
 *
 * 	CHFts14388: Fix the check for a proper cell name being passed
 * 	to the catalog command.
 * 	HP 	[1995/03/02  18:47 UTC  truitt  /main/HPDCE02/3]
 *
 * 	Merge init changes.
 * 	HP 	[1995/03/02  18:46 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts17/1]
 *
 * 	CHFts14504: Initialize attr_info structure correctly.
 * 	HP 	[1995/01/26  17:57 UTC  truitt  /main/HPDCE02/2]
 *
 * 	Merge.
 * 	HP 	[1995/01/26  17:55 UTC  truitt  /main/HP
 *
 * Revision 1.1.6.1  1996/05/10  13:06:28  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/01  18:25 UTC  jrr
 * 	Modify rgy_group_list() to not strip foreign cell names with
 * 	-simplename, and prepend the correct cell name too.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/01/31  16:25 UTC  hanfei
 * 	multi-cell group work: modify rgy_group_add and rgy_group_remove to accept
 * 	global member names.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:49 UTC  psn
 * 	Merge.
 * 	[1995/06/16  18:14 UTC  truitt  /main/HPDCE02/8]
 * 
 * 	CHFts15504: Remove unneeded includes.
 * 	[1995/06/16  18:11 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts35/1]
 * 
 * 	Merge era work.
 * 	[1995/05/01  19:05 UTC  truitt  /main/HPDCE02/7]
 * 
 * 	CHFts14301: Allocate memory for the sec_attr_t structures
 * 	dynamically.
 * 	[1995/05/01  19:04 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts26/1]
 * 
 * 	Merge schema message work.
 * 	[1995/04/05  19:52 UTC  truitt  /main/HPDCE02/6]
 * 
 * 	CHFts14652: Add improved error message.
 * 	[1995/04/05  19:51 UTC  truitt  /main/HPDCE02/truitt_mothra_chfts24/1]
 * 
 * 	Make org list and group list more efficient (CR12822).
 * 	[1995/03/28  15:04 UTC  jrr  /main/HPDCE02/5]
 * 
 * 	Make org list and group list more efficient (CR12822).
 * 	[1995/03/28  15:02 UTC  jrr  /main/HPDCE02/jrr_dcecp_cr12822/1]
 * 
 * 	Check for valid cellname in group catalog.
 * 	[1995/03/09  14:32 UTC  truitt  /main/HPDCE02/4]
 * 
 * 	CHFts14388: Fix the check for a proper cell name being passed
 * 	to the catalog command.
 * 	[1995/03/09  14:31 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts18/1]
 * 
 * 	Merge init changes.
 * 	[1995/03/02  18:47 UTC  truitt  /main/HPDCE02/3]
 * 
 * 	CHFts14504: Initialize attr_info structure correctly.
 * 	[1995/03/02  18:46 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts17/1]
 * 
 * 	Merge.
 * 	[1995/01/26  17:57 UTC  truitt  /main/HPDCE02/2]
 * 
 * 	Fix a couple free's in the show function.  Also, change
 * 	the way the options are processed in the show function.
 * 	[1995/01/26  17:55 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts05/1]
 * 
 * 	Initial HPDCE02 branch.
 * 	[1994/11/03  20:38 UTC  truitt  /main/HPDCE02/1]
 * 
 * 	Fix group command switches.
 * 	[1994/11/03  20:36 UTC  truitt  /main/truitt_dcecp_01/1]
 * 
 * Revision 1.1.2.11  1994/10/05  18:42:59  truitt
 * 	OT12492: Do not free output structures when call to
 * 	check_era_info() function fails.
 * 	[1994/10/05  18:38:00  truitt]
 * 
 * Revision 1.1.2.10  1994/09/30  15:26:29  truitt
 * 	OT12433: Properly chech the show_era() return status.
 * 	[1994/09/30  15:05:06  truitt]
 * 
 * Revision 1.1.2.9  1994/09/29  18:45:16  truitt
 * 	OT10967: Allow proper display of multiple valued attributes.
 * 	Also, cleaned up some of the 'free' code.
 * 	[1994/09/29  17:33:22  truitt]
 * 
 * Revision 1.1.2.8  1994/09/23  18:34:36  ajayb
 * 	Fixed several memory leaks.
 * 	[1994/09/22  20:39:24  ajayb]
 * 
 * Revision 1.1.2.7  1994/09/16  15:34:26  ajayb
 * 	rgy_group_show(): Don't print leading newline before ERAs,
 * 	if that is the only thing being displayed.
 * 	[1994/09/15  18:02:09  ajayb]
 * 
 * Revision 1.1.2.6  1994/09/15  13:06:37  kevins
 * 	OT 12197 modified list cell_name aquisition.
 * 	[1994/09/15  13:05:55  kevins]
 * 
 * Revision 1.1.2.5  1994/09/14  15:10:07  kevins
 * 	OT 12175 - The code now continues onto the next registry instead
 * 	   of returning. Also the result code is reset prior to
 * 	   accessing the registry in account and pgo catalog
 * 	   commands. The new method for allocating the output
 * 	   buffer needed one more byte. The output buffer is
 * 	   allocated based on the size of the cell_name and
 * 	   the maximum size of the name allowed by the registry.
 * 	   It needed one more byte though to accommodate the
 * 	   '/' that goes between them. I also included an
 * 	   empty line between multiple registry outputs.
 * 	[1994/09/14  15:06:24  kevins]
 * 
 * Revision 1.1.2.4  1994/09/13  22:38:59  rousseau
 * 	Catalog performance changes.
 * 	[1994/09/13  22:37:44  rousseau]
 * 
 * Revision 1.1.2.3  1994/09/13  19:12:38  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:39  kevins]
 * 
 * Revision 1.1.2.2  1994/09/12  17:58:00  kevins
 * 	OT 12076 Added two flags -all and -xattrs to aquire all
 * 	   attribute/policy and era info or to aquire only
 * 	   the attribute information.
 * 	[1994/09/12  13:47:18  kevins]
 * 
 * Revision 1.1.2.1  1994/09/06  16:22:00  kevins
 * 	11694 = Broke up rgy.c into
 * 	                        rgy_pgo_group.c
 * 	                        rgy_pgo_account.c
 * 	                        rgy.c
 * 	[1994/09/06  15:47:42  kevins]
 * 
 * $EndLog$
 */
/* INCLUDES */
#include <stdio.h>
#include <locale.h>
#include <errno.h>
/*
 * Don't inherit stubbase.h's use of the exception based pthrads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#ifndef SNI_SVR4_POSIX
#include <strings.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <dce/dce_error.h>
#include <sys/types.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/policy.h>
#include <dce/rgynbase.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dce_svc.h>
#include <dce/misc.h>
#include <dcecp.h>
#include <dce/tcl.h>
#include <dce/acct.h>
#include <dce/utc.h>
#include <dce/secidmap.h>
#include <rgy.h>
#include <util_var.h>
#include <util_general.h>
#include <util_help.h>
#include <util_binding.h>
#include <values.h>

#include <dce/dced.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_rgy_attr_sch.h>
#include <dce/sec_pwd_mgmt.h>
#include <dce/macros.h>
#include <dhd.h>
#include <util_login.h>

extern int skip_era_newline;


/*
 * FUNCTION: rgy_group_create()
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 */
int rgy_group_create(ClientData clientData, 
		     Tcl_Interp *interp, 
		     int argc, 
		     char **argv)
{
    error_status_t        result = TCL_OK;
    char                  *alias = NULL;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_name_t        group_name;
    rgy_attribute_info_t  attr_info;
    char                  **cargv;
    int                   cargc = 0;
    char                  **jargv;
    int                   jargc = 0;
    char                  **targv;
    int                   targc = 0;
    int                   arg_count = 0;
    int                   i;
    char                  *attribute = NULL;
    char                  *fullname = NULL;
    char                  *inprojlist = NULL;
    char                  *uuid = NULL;
    char                  *gid = NULL;
    sec_rgy_pgo_item_t    pgo_item;
    sec_attr_t            *attrs = NULL;
    sec_attr_t            *out_attrs = NULL;
    unsigned32            num_returned = 0;
    unsigned32            num_left = 0;
    unsigned32            space_avail = 0;
    signed32              failure_index;
    error_status_t        status;
    boolean32             is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-alias", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_alias_help},
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_attr_help}, 
        {"-fullname", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_fname_help},
        {"-gid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_gid_help},
        {"-inprojlist", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_prjlst_help},
        {"-uuid", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_uuid_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Set the variables for the Parser. We need to cast them all as 
     * char pointers. 
     */
    memset((char *)&pgo_item,0,sizeof(pgo_item));
    pgo_item.unix_num = -1;
    arg_table[0].dst = (char *)&alias;
    arg_table[1].dst = (char *)&attribute;
    arg_table[2].dst = (char *)&fullname;
    arg_table[3].dst = (char *)&gid;
    arg_table[4].dst = (char *)&inprojlist;
    arg_table[5].dst = (char *)&uuid;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided a group name with the command. 
     * If so initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Input can be by list or by option, but not by both.
     */
    if ((alias != NULL || fullname != NULL || gid != NULL && 
        inprojlist != NULL) && attribute != NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    /*
     * Quota for a group is always -1.  The default for
     * 'inprojlist' should be 'yes'.
     */
    pgo_item.quota = -1;
    pgo_item.flags = 0;
    pgo_item.flags |= sec_rgy_pgo_projlist_ok;

    /*   
     * Check the argument list to verify the group create conditions 
     * have been met. If the user is creating an alias then the gid must 
     * be set as the group the user wishes to alias to.  Also if 
     * alias is set the uuid_no must not be specified as the group
     * uuid already exists. 
     */
    attr_info.domain = sec_rgy_domain_group;
    attr_info.policy = FALSE;
    attr_info.pgo = FALSE;
    attr_info.types = 0;
    attr_info.era_count = 0;
    attr_info.del_count = 0;
    attr_info.modify_type = modify_type_none;
    attr_info.flags = pgo_create;
    attr_info.pgo_item = &pgo_item;
    attr_info.fullname = NULL;

    parse_item_switch_list(NULL, fullname, uuid, gid, alias, inprojlist,
                           &pgo_item, sec_rgy_domain_group, &attr_info, 
                           &result);

    if (result == dcp_s_rgy_quota_out_of_range) {
        DCP_SET_SEC_ERROR(result, MAXINT);
    }
    else if (result == dcp_s_rgy_uid_out_of_range) {
        DCP_SET_SEC_ERROR(dcp_s_rgy_gid_out_of_range, MAXINT);
    }
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Check size of attribute input to get a rough estimate of
     * how large the attrs structures will have to be.  Even if
     * no attribute are specified, malloc some space so the
     * following functions do not have any problems.
     */
    if (attribute != NULL) {
        if (Tcl_SplitList(interp, attribute, &jargc, &jargv) != TCL_OK) {
            return TCL_ERROR;
        }
        for (i = 0; i < jargc; i++) {
            if (Tcl_SplitList(interp, jargv[i], &targc, &targv) == TCL_OK) {
               arg_count = arg_count + targc;
               free((char *)targv);
            }
        }
        free((char *)jargv);
    }
    attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (attrs == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    out_attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (out_attrs == NULL) {
        free((sec_attr_t *)attrs);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    if (parse_attribute_list(interp, attribute, &pgo_item, NULL,
                             &attr_info, attrs) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        return TCL_ERROR;
    }

    /* 
     * Check to see if the correct number of switches have been set. 
     * If the user wants to generate an alias he/she must provide 
     * both the alias and a appropriate uid of an existing group.
     * if the user wants to specify an uuid then she/he cannot be specifying
     * a request for an alias. 
     */
    if ((pgo_item.unix_num == -1) && 
        (pgo_item.flags & sec_rgy_pgo_is_an_alias)) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_gid); 
    }
    if ((pgo_item.flags & sec_rgy_pgo_is_an_alias) && 
        !uuid_is_nil(&pgo_item.id,&result)) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_uuid_and_alias);
    }

    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((cargc == 0) && (cargv[0] == NULL)) {
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(cargc, cargv)) == -1) {
        free((char *)cargv);
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)out_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (i = 0; i < cargc ; i++) {
        /*
         * Initialize the name parameter
         */
        strcpy((char *)global_name,cargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            return TCL_ERROR;
        }
        sec_rgy_pgo_add(rgy_context,
                        sec_rgy_domain_group,
                        group_name,
                        &pgo_item,
                        (error_status_t *)&result);
        if (result != sec_rgy_status_ok) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)out_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        /*
         * Check if any extended attributes were specified during the
         * create.  If so, they are stored in the attrs structure.  Call
         * the proper function to get them processed.
         */
        if (attr_info.era_count > 0) {
            sec_rgy_attr_update(rgy_context,
                                sec_rgy_domain_group,
                                group_name,
                                attr_info.era_count,
                                space_avail,
                                attrs,
                                &num_returned,
                                out_attrs,
                                &num_left,
                                &failure_index,
                                &status);
            if (status != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)out_attrs);
                DCP_SET_MISC_ERROR(status);
            }
        }
    }
    free((char *)cargv);
    FREE_ATTR_PTRS(attr_info.era_count, attrs);
    free((sec_attr_t *)attrs);
    free((sec_attr_t *)out_attrs);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_delete()
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_group_delete(ClientData clientData, 
		     Tcl_Interp *interp, 
		     int argc, 
		     char **argv)
{
    int             result = TCL_OK;
    int             pargc;
    char            **pargv;
    int             i = 0;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  group_name;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = { 
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
    
    if (Tcl_SplitList(interp, argv[1], &pargc, &pargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((pargc == 0) && (pargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(pargc, pargv)) == -1) {
        free((char *)pargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (; i < pargc; i++) {
        strcpy((char *)global_name, pargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }
        sec_rgy_pgo_delete(rgy_context,
                           sec_rgy_domain_group,
                           group_name,
                           (error_status_t *)&result);
        if (result != sec_rgy_status_ok) {
            free((char *)pargv);
            DCP_SET_MISC_ERROR(result);
        }
    }

    free((char *)pargv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_catalog
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 *  Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                       will hold the error message (if any) generated from the
 *                       command. This is also the location of any convienience
 *                       variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_group_catalog(ClientData clientData, 
                      Tcl_Interp *interp, 
                      int argc, 
                      char **argv)
{
    error_status_t      result = TCL_OK;
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_cursor_t    item_cursor;
    char                *output;
    sec_rgy_name_t      name;
    sec_rgy_name_t      global_name;
    sec_rgy_name_t      name1;
    boolean32           simplename = FALSE;
    sec_rgy_name_t      cell_name;
    size_t              group_name_size;
    int                 gargc = 0;
    char                **gargv;
    int                 i = 0;
    int                 saved_length;
    int                 bad_rgy_cursor_count = 0;
    char                *expand_global_name;

    dcp_ArgvInfo arg_table[] = { 
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_group_cat_simple_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&simplename; 

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    global_name[0] = '\0';
    if (argc > 1) {
        DCP_CHECK_EXTRA_ARGS();
        DCP_GENERAL_SET_NAMELIST(argv[1]);
        if (dcp_general_split_list(interp, argv[1], &gargc, &gargv) != TCL_OK) {
            return TCL_ERROR;
        }
        else {
            /*
             * Check that we did not receive an empty argument list.
             */
            if ((gargc == 0) && (gargv[0] == NULL)) {
                DCP_SET_MISC_ERROR(dcp_s_null_arg);
            }
        }
    }
    else {
        gargc = 1;
        gargv = (char **)malloc(2 * sizeof(char *));
        if (gargv == NULL) {
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }
        gargv[0] = (char *)global_name;
        gargv[1] = NULL;
    }

    for (; i < gargc; i++) {
        strcpy((char *)global_name, gargv[i]);
        if (open_rgy_site(interp, global_name, site_query) != TCL_OK) {
            free((char *)gargv);
            DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
        }
        strcpy((char *)cell_name, (char *)site_cache[realtime_cache].cell_name);
        if (*gargv[i] != '\0') {
            if (global_name[0] == '/') {
                expand_global_name = expand_cell_name((char *)global_name, &result);
                if (result != rpc_s_incomplete_name) {
                    strcpy((char *)global_name, expand_global_name);
                    free(expand_global_name);
                }
            }
            if (strcmp((char *)global_name, (char *)cell_name) != 0) {
                free((char *)gargv);
                DCP_SET_SEC_ERROR(dcp_s_rgy_invalid_cell, global_name);
            }
        }
        strcpy((char *)name, "");
        strcpy((char *)name1, "");
        sec_rgy_cursor_reset(&item_cursor);

        group_name_size = strlen((char *)cell_name) + sec_rgy_pname_t_size + 1;
        output = (char *)malloc(group_name_size);
        if (output == NULL) {
            free((char *)gargv);
            DCP_SET_RESULT_CODE(dcp_s_no_memory);
            return TCL_ERROR;
        }

        if (strlen(interp->result)) {
            Tcl_AppendResult(interp, "\n", NULL);
            saved_length = strlen(interp->result);
        }
        else {
            saved_length = 0;
        }

        for (result = error_status_ok; result == error_status_ok;) {
            sec_rgy_pgo_get_next(rgy_context,
                                 sec_rgy_domain_group,
                                 name,
                                 &item_cursor,
                                 &pgo_item,
                                 name1,
                                 &result);
            if (result == sec_rgy_no_more_entries)
                continue;
            if ((result == sec_rgy_object_not_found) && (bad_rgy_cursor_count < 5)) {
                /*
                 * This is a very unexpected result, but if it occurs, the
                 * registry cursor has been trashed somehow, probably
                 * because the element it was pointing to was deleted out
                 * from under it.  The recommended recovery is to start
                 * again, but to avoid a possible endless loop, we'll only
                 * re-try a few times.  If we still cannot complete the
                 * command without the registry cursor being hosed, just
                 * forget it and report the error.
                 */
                bad_rgy_cursor_count++;
                strcpy((char *)name, "");
                strcpy((char *)name1, "");
                sec_rgy_cursor_reset(&item_cursor);
                interp->result[saved_length] = '\0';
                result = error_status_ok;
                continue;
            }
            if (result != sec_rgy_status_ok) {
                free((char *)gargv);
                free(output);
                DCP_SET_MISC_ERROR(result);
            }

            if (!simplename)
                sprintf(output, "%s/%s", cell_name, name1);
            else
                sprintf(output, "%s", name1);
            if (strlen(interp->result))
                Tcl_AppendResult(interp, "\n", NULL);
            Tcl_AppendResult(interp, output, NULL);
        }
        free(output);
    }

    free((char *)gargv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_rename
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This         
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_group_rename(ClientData clientData, 
		     Tcl_Interp *interp, 
		     int argc, 
		     char **argv)
{
    error_status_t  result = TCL_OK;
    char            *new_name_ptr = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  group_name;
    sec_rgy_name_t  new_name;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table[] = {
        {"-to", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_rename_to_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Set the variables for the Parser. We need to cast them all as 
     * char pointers. 
     */
    arg_table[0].dst = (char *)&new_name_ptr;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (new_name_ptr == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_name_to_rename);
    }

    result = error_status_ok;
    is_simple = is_simple_name(1, &argv[1]);

    strcpy((char *)global_name, argv[1]);
    if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK)
        return TCL_ERROR;

    if (open_rgy_site(interp, cell_name, site_update) != TCL_OK)
        return TCL_ERROR;

    strcpy((char *)global_name, new_name_ptr);
    if (dcp_id_parse_name(interp, global_name, cell_name, new_name) != TCL_OK)
        return TCL_ERROR;

    sec_rgy_pgo_rename(rgy_context,
                       sec_rgy_domain_group, 
                       group_name,
                       new_name,
                       &result);
    DCP_CHECK_SEC_ERROR(result);

    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_show
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_group_show(ClientData clientData, 
		   Tcl_Interp *interp, 
		   int argc, 
		   char **argv)
{
    int                 result = TCL_OK;
    error_status_t      status;
    sec_rgy_name_t      global_name;
    sec_rgy_name_t      cell_name;
    sec_rgy_name_t      group_name;
    char                *output;
    char                *alias_svc;       /* alias label */
    char                *gid_svc;         /* unix id */
    char                *uuid_svc;        /* uuid value */
    char                *fullname_svc;    /* administrative data */
    char                *inprojlist_svc;  /* project list label */
    char                *alias;
    char                *inprojlist;
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_cursor_t    item_cursor;
    int                 gargc;
    char                **gargv;
    char                uuid[UUID_STRING_LEN];
    int                 i = 0;
    boolean32           is_simple;
    boolean32           all = FALSE;
    boolean32           xattrs = FALSE;

    dcp_ArgvInfo arg_table[] = { 
        {"-all", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_group_show_all_help},
        {"-xattrs", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_group_show_era_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&all;
    arg_table[1].dst = (char *)&xattrs;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check that the user has specified the options correctly.
     * Only one of the options is allowed at a time.
     */
    if ((all == TRUE) && (xattrs == TRUE)) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /* 
     * Terminate the command if not enough arguments are passed in. 
     * If the correct number of args have been supplied, set the
     * namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);
    
    /*
     * The user is allowed to specify a list of groups to show.
     */
    if (Tcl_SplitList(interp, argv[1], &gargc, &gargv) == TCL_ERROR) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((gargc == 0) && (gargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(gargc, gargv)) == -1) {
        free((char *)gargv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    /*
     * Set up the field labels.
     */
    alias_svc = (char *)dce_sprintf(dcp_t_rgy_alias);
    gid_svc = (char *)dce_sprintf(dcp_t_rgy_gid);
    uuid_svc = (char *)dce_sprintf(dcp_t_rgy_uuid);
    inprojlist_svc = (char *)dce_sprintf(dcp_t_rgy_inprojlist);
    fullname_svc = (char *)dce_sprintf(dcp_t_rgy_fullname);
    if (!gid_svc || !uuid_svc || !inprojlist_svc || !fullname_svc || !alias_svc) {
        free((char *)gargv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /* 
     * Cycle through each of the specified groups names and show 
     * all of the available information. 
     */
    for (; i < gargc; i++) {
        if (strlen(interp->result))
            Tcl_AppendResult(interp, "\n\n", NULL);
        strcpy((char *)global_name, gargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)gargv);
            free(alias_svc);
            free(gid_svc);
            free(uuid_svc);
            free(inprojlist_svc);
            free(fullname_svc);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_query) != TCL_OK) {
            free((char *)gargv);
            free(alias_svc);
            free(gid_svc);
            free(uuid_svc);
            free(inprojlist_svc);
            free(fullname_svc);
            return TCL_ERROR;
        }

        if (xattrs == FALSE) {
            sec_rgy_pgo_get_by_name(rgy_context,
                                    sec_rgy_domain_group,
                                    group_name,
                                    &item_cursor,
                                    &pgo_item,
                                    &status);
            if (status != sec_rgy_status_ok) {
                free((char *)gargv);
                free(alias_svc);
                free(gid_svc);
                free(uuid_svc);
                free(inprojlist_svc);
                free(fullname_svc);
                DCP_SET_MISC_ERROR(status);
            }
            status = decode_uuid((uuid_t *)&pgo_item.id, uuid);

            /* 
             * Generate the output buffers. Install it in the interp result.
             * Check to see if the group is an alias. 
             */
            if (pgo_item.flags & sec_rgy_pgo_is_an_alias) 
                alias = (char *)dce_sprintf(dcp_t_rgy_yes);
            else
                alias = (char *)dce_sprintf(dcp_t_rgy_no);
            if (!alias) {
                free((char *)gargv);
                free(alias_svc);
                free(gid_svc);
                free(uuid_svc);
                free(inprojlist_svc);
                free(fullname_svc);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            if (pgo_item.flags & sec_rgy_pgo_projlist_ok) 
                inprojlist = (char *)dce_sprintf(dcp_t_rgy_yes);
            else
                inprojlist = (char *)dce_sprintf(dcp_t_rgy_no);
            if (!inprojlist) {
                free((char *)gargv);
                free(alias_svc);
                free(gid_svc);
                free(uuid_svc);
                free(inprojlist_svc);
                free(fullname_svc);
                free(alias);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            /* 
             * Set up output buffers and aquire svc labels for the 
             * group information.
             */
            output = (char *)malloc(strlen(alias_svc) + strlen(alias) + 
                                    strlen(gid_svc) + strlen(uuid_svc) + 
                                    strlen(uuid) + strlen(inprojlist_svc) + 
                                    strlen(inprojlist) + 30);
            if (output == NULL) {
                free((char *)gargv);
                free(alias_svc);
                free(gid_svc);
                free(uuid_svc);
                free(inprojlist_svc);
                free(fullname_svc);
                free(alias);
                free(inprojlist);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            memset(output,0,sizeof(output));

            sprintf(output, "{%s %s}\n{%s %d}\n{%s %s}\n{%s %s}", alias_svc, alias,
                    gid_svc, pgo_item.unix_num, uuid_svc, uuid, inprojlist_svc, inprojlist);
            Tcl_AppendResult(interp, output, NULL);
            if (strlen((char *)pgo_item.fullname))
                Tcl_AppendResult(interp, "\n{", fullname_svc, " {", pgo_item.fullname,
                                 "}}", NULL);
            else
                Tcl_AppendResult(interp, "\n{", fullname_svc, " {}}", NULL);

            /* 
             * free allocated memory. 
             */
            free(alias);
            free(inprojlist);
            free(output);
        }
        if (xattrs || all) {
            /*
             * Display any era information associated with this name.
             */
            if (!all)
                skip_era_newline = 1;
            status = show_era(interp, sec_rgy_domain_group, group_name);
            if ((status != sec_rgy_status_ok) && (status != rpc_s_unknown_if) &&
                (status != ept_s_not_registered)) {
                free((char *)gargv);
                free(alias_svc);
                free(gid_svc);
                free(uuid_svc);
                free(inprojlist_svc);
                free(fullname_svc);
                DCP_SET_MISC_ERROR(status);
            }
        }
    }

    /* 
     * Free allocated memory. 
     */
    free((char *)gargv);
    free(alias_svc);
    free(gid_svc);
    free(uuid_svc);
    free(inprojlist_svc);
    free(fullname_svc);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_modify()
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_group_modify(ClientData clientData, 
		     Tcl_Interp *interp, 
		     int argc, 
		     char **argv)
{
    error_status_t        result = TCL_OK;
    sec_rgy_name_t        global_name;
    sec_rgy_name_t        cell_name;
    sec_rgy_name_t        group_name;
    char                  *attribute = NULL;
    char                  *attribute1 = NULL;
    char                  *attribute2 = NULL;
    char                  *attribute3 = NULL;
    char                  *alias = NULL;
    char                  *inprojlist = NULL;
    char                  *fullname = NULL;
    char                  *types = NULL;
    sec_rgy_cursor_t      item_cursor;
    sec_rgy_pgo_item_t    pgo_item;
    sec_rgy_pgo_item_t    temp_pgo_item;
    rgy_attribute_info_t  attr_info;
    int                   i;
    int                   era_count_hold = 0;
    char                  **cargv;
    int                   cargc;
    char                  **jargv;
    int                   jargc = 0;
    char                  **targv;
    int                   targc = 0;
    int                   arg_count = 0;
    sec_attr_t            *attrs;
    sec_attr_t            *out_attrs;
    sec_attr_t            *del_attrs;
    sec_attr_t            *tmp_attrs;
    unsigned32            num_returned = 0;
    unsigned32            num_left = 0;
    unsigned32            space_avail = 0;
    signed32              failure_index;
    error_status_t        status;
    boolean32             is_simple;
    unsigned32            cur_num_attrs;
    sec_attr_cursor_t     attr_cursor;

    /*
     * Set up the parser argument table and initialize the variables.
     */
    dcp_ArgvInfo arg_table[] = { 
        {"-add", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_mod_add_help},
        {"-alias", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_alias_help},
        {"-change", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_mod_change_help},
        {"-fullname", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_fname_help},
        {"-inprojlist",DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_prjlst_help},
        {"-remove", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_mod_remove_help},
        {"-types", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_group_mod_types_help},
        {NULL,DCP_ARGV_END,NULL,NULL,0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&attribute1;
    arg_table[1].dst = (char *)&alias;
    arg_table[2].dst = (char *)&attribute2;
    arg_table[3].dst = (char *)&fullname;
    arg_table[4].dst = (char *)&inprojlist;
    arg_table[5].dst = (char *)&attribute3;
    arg_table[6].dst = (char *)&types;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * The user can not both promote and demote a group in the same 
     * operation. Also verify that a group name has been supplied.
     */
    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The user cannot specify combinations of add, change, and remove
     * on the same command line.  Only one at a time is allowed.
     */
    if ((attribute1 != NULL && attribute2 != NULL) ||
        (attribute1 != NULL && attribute3 != NULL) ||
        (attribute2 != NULL && attribute3 != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_era_illegal_switches);
    }

    /* 
     * The user also can not specify an attribute list and a variety of 
     * switches. (ie "group modify foo -alias yes -change {name foob}"
     * is illegal.)
     */
    if ((alias != NULL || fullname != NULL) &&
        (attribute1 != NULL || attribute2 != NULL || attribute3 != NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
    }

    /*
     * If the user didn't specify any type of modify command,
     * then this is an error.
     */
    if (alias == NULL && fullname == NULL && inprojlist == NULL &&
        attribute1 == NULL && attribute2 == NULL && attribute3 == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_modify_operation);
    }

    /*
     * If the user specified the types option with anything but
     * a remove option, then this is an error.
     */
    if (types != NULL && attribute3 == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_bad_use_of_types_option);
    }

    /*
     * Initialize the pgo_item to a known value
     */
    init_pgo_item(&temp_pgo_item);

    attr_info.domain = sec_rgy_domain_group;
    attr_info.policy = FALSE;
    attr_info.pgo = FALSE;
    attr_info.era_count = 0;
    attr_info.del_count = 0;
    attr_info.flags = pgo_modify;
    attr_info.pgo_item = &temp_pgo_item;
    attr_info.fullname = NULL;

    if (types == NULL)
       attr_info.types = FALSE;
    else
       attr_info.types = TRUE;

    if (attribute1 != NULL) {
       attr_info.modify_type = modify_type_add;
       attribute = attribute1;
    }
    else if (attribute2 != NULL) {
       attr_info.modify_type = modify_type_change;
       attribute = attribute2;
    }
    else if (attribute3 != NULL) {
       attr_info.modify_type = modify_type_remove;
       attribute = attribute3;
    }
    else {
        attr_info.modify_type = modify_type_none;
    }

    parse_item_switch_list(NULL, fullname, NULL, NULL, alias, inprojlist, &temp_pgo_item,
                           sec_rgy_domain_group, &attr_info, &result);
    DCP_CHECK_SEC_ERROR(result);

    /*
     * Check size of attribute input to get a rough estimate of
     * how large the attrs structures will have to be.  Even if
     * no attribute are specified, malloc some space so the
     * following functions do not have any problems.
     */
    if (attribute != NULL) {
        if (Tcl_SplitList(interp, attribute, &jargc, &jargv) != TCL_OK) {
            return TCL_ERROR;
        }
        for (i = 0; i < jargc; i++) {
            if (Tcl_SplitList(interp, jargv[i], &targc, &targv) == TCL_OK) {
               arg_count = arg_count + targc;
               free((char *)targv);
            }
        }
        free((char *)jargv);
    }
    attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (attrs == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    tmp_attrs = malloc((arg_count + 5) * sizeof(sec_attr_t));
    if (tmp_attrs == NULL) {
        free((sec_attr_t *)attrs);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * Parse the attribute list.  The list will apply to each
     * object being operated on if there are more than one.
     */
    if (parse_attribute_list(interp, attribute, &temp_pgo_item, NULL,
                             &attr_info, attrs) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        return TCL_ERROR;
    }

    /*
     * Modify each group object that was specified on the command line.
     */
    if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((cargc == 0) && (cargv[0] == NULL)) {
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(cargc, cargv)) == -1) {
        free((char *)cargv);
        FREE_ATTR_PTRS(attr_info.era_count, attrs);
        free((sec_attr_t *)attrs);
        free((sec_attr_t *)tmp_attrs);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (i = 0; i < cargc; i++) {
        /*
         * Copy the group name and attempt to aquire it from the 
         * registry.
         */
        strcpy((char *)global_name, cargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            return TCL_ERROR;
        }
        sec_rgy_pgo_get_by_name(rgy_context,
                                sec_rgy_domain_group,
                                group_name,
                                &item_cursor,
                                &pgo_item,
                                &result);
        if (result != sec_rgy_status_ok) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        copy_pgo_item_with_restrict(&pgo_item, &temp_pgo_item, &attr_info);

        sec_rgy_pgo_replace(rgy_context, 
                            sec_rgy_domain_group, 
                            group_name, 
                            &pgo_item, 
                            &result);
        if (result != sec_rgy_status_ok) {
            free((char *)cargv);
            FREE_ATTR_PTRS(attr_info.era_count, attrs);
            free((sec_attr_t *)attrs);
            free((sec_attr_t *)tmp_attrs);
            DCP_SET_MISC_ERROR(result);
        }

        /*
         * Check if any extended attributes were specified during the
         * modify.  If so, they are stored in the attrs structure.
         */
        if (attr_info.era_count > 0) {
            era_count_hold = attr_info.era_count;

            /*
             * We need to get a rough count of the existing attributes for
             * the given object.  To do this, we can initialize the attribute
             * attribute cursor, get the number, then release the cursor.
             */
            cur_num_attrs = 0;
            sec_rgy_attr_cursor_init(rgy_context,
                                     sec_rgy_domain_group,
                                     group_name,
                                     &cur_num_attrs,
                                     &attr_cursor,
                                     &result);
            if (result != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(result);
            }
            sec_rgy_attr_cursor_release(&attr_cursor, &result);
            if (result != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(attr_info.era_count, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(result);
            }

            /*
             * Based on the number of existing attributes, plus the number
             * of attributes the user is trying to modify, allocate memory
             * for the out_attrs and del_attrs structures.
             */
            out_attrs = malloc((attr_info.era_count + cur_num_attrs + 5) * sizeof(sec_attr_t));
            if (out_attrs == NULL) {
                free((char *)cargv);
                FREE_ATTR_PTRS(era_count_hold, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            del_attrs = malloc((attr_info.era_count + cur_num_attrs + 5) * sizeof(sec_attr_t));
            if (del_attrs == NULL) {
                free((char *)cargv);
                FREE_ATTR_PTRS(era_count_hold, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                free((sec_attr_t *)out_attrs);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            /*
             * The attrs structure must be changed to reflect the real
             * processing that will go on with the extended attributes.
             * Multiple instances, multiple values, and the differences
             * between -add, -change, and -remove make further processing
             * of the extended attributes a requirement.
             */
            result = check_era_info(interp,
                                    sec_rgy_domain_group,
                                    group_name,
                                    attrs,
                                    &attr_info,
                                    out_attrs,
                                    del_attrs);
            if (result != sec_rgy_status_ok) {
                free((char *)cargv);
                FREE_ATTR_PTRS(era_count_hold, attrs);
                free((sec_attr_t *)attrs);
                free((sec_attr_t *)tmp_attrs);
                free((sec_attr_t *)out_attrs);
                free((sec_attr_t *)del_attrs);
                DCP_SET_MISC_ERROR(result);
            }

            /*
             * If we managed to get through the check function without any
             * errors, and there are still attributes to process, do so.
             */
            if (attr_info.era_count > 0) {
                /*
                 * The out_attrs structure is filled with information
                 * to add or change, so use the update function.
                 */
                sec_rgy_attr_update(rgy_context,
                                    sec_rgy_domain_group,
                                    group_name,
                                    attr_info.era_count,
                                    space_avail,
                                    out_attrs,
                                    &num_returned,
                                    tmp_attrs,
                                    &num_left,
                                    &failure_index,
                                    &status);
                if (status != sec_rgy_status_ok) {
                    free((char *)cargv);
                    FREE_ATTR_PTRS(era_count_hold, attrs);
                    FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
                    FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
                    free((sec_attr_t *)attrs);
                    free((sec_attr_t *)tmp_attrs);
                    free((sec_attr_t *)out_attrs);
                    free((sec_attr_t *)del_attrs);
                    DCP_SET_MISC_ERROR(status);
                }
            }
            if (attr_info.del_count > 0) {
                /*
                 * The del_attrs structure is filled with information
                 * to remove from the database, so use the remove function.
                 */
                sec_rgy_attr_delete(rgy_context,
                                    sec_rgy_domain_group,
                                    group_name,
                                    attr_info.del_count,
                                    del_attrs,
                                    &failure_index,
                                    &status);
                if (status != sec_rgy_status_ok) {
                    free((char *)cargv);
                    FREE_ATTR_PTRS(era_count_hold, attrs);
                    FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
                    FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
                    free((sec_attr_t *)attrs);
                    free((sec_attr_t *)tmp_attrs);
                    free((sec_attr_t *)out_attrs);
                    free((sec_attr_t *)del_attrs);
                    DCP_SET_MISC_ERROR(status);
                }
            }

            /*
             * We successfully processed some era attributes.  Since we
             * did not run into any errors, we have some free'ing to do
             * of the sec_attr_t structures.  The next pass through the
             * loop will malloc the required space.
             */
            FREE_ATTR_PTRS(attr_info.era_count, out_attrs);
            FREE_ATTR_PTRS(attr_info.del_count, del_attrs);
            free((sec_attr_t *)out_attrs);
            free((sec_attr_t *)del_attrs);

            /*
             * Reset the era count.
             */
            attr_info.era_count = era_count_hold;
        }
    }
    free((char *)cargv);
    FREE_ATTR_PTRS(attr_info.era_count, attrs);
    free((sec_attr_t *)attrs);
    free((sec_attr_t *)tmp_attrs);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_add()
 *
 * OVERVIEW:
 * 
 *
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 */
int rgy_group_add(ClientData clientData, 
		  Tcl_Interp *interp, 
		  int argc, 
		  char **argv)
{
    int             result = TCL_OK;
    char            *member = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  group_name;
    sec_rgy_name_t  member_name;
    char            **aargv;
    int             aargc = 0;
    char            **margv;
    int             margc = 0;
    int             i = 0;
    int             j = 0;
    boolean32       is_simple;

    dcp_ArgvInfo arg_table [] = {
        {"-member", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_add_mem_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Set the variables for the Parser. We need to cast them all as 
     * char pointers. 
     */
    arg_table[0].dst = (char *)&member;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided a group name with the command. 
     * If so initialize the pgo name parameter.
     */
    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!member) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_member_name);
    }

    if (Tcl_SplitList(interp, argv[1], &aargc, &aargv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((aargc == 0) && (aargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if (Tcl_SplitList(interp, member, &margc, &margv) != TCL_OK) {
        free((char *)aargv);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty member list.
         */
        if ((margc == 0) && (margv[0] == NULL)) {
            free((char *)aargv);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(aargc, aargv)) == -1) {
        free((char *)aargv);
        free((char *)margv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (; i < aargc ; i++) {
        /*
         * Initialize the name parameter
         */
        strcpy((char *)global_name, aargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        /* 
         * Cycle through all members and initialize the member
         * parameter 
         */
        for (j = 0; j < margc; j++) {
            strcpy((char *)global_name, margv[j]);

            /* 
             * If the name passed in _appears_ to be fully qualified
             * (ie. starts with /.../), don't run it through 
             * dcp_id_parse_name(). If the name is foreign, it would
             * force a call to the remote security server. Once this
             * name is handed to secd in the sec_rgy_pgo_add_member()
             * call below, secd will call sec_id_parse_name() itself.
             */
            if (strncmp(GLOBAL_DIR_ROOT, (char *)global_name, GLOBAL_DIR_ROOT_LEN) != 0) {
            	if (dcp_id_parse_name(interp, global_name, cell_name, member_name) != TCL_OK) {
                    free((char *)aargv);
                    free((char *)margv);
                    return TCL_ERROR;
                }
            }
            else {
                strcpy((char *)member_name, (char *)global_name);
            }

            sec_rgy_pgo_add_member(rgy_context,
                                   sec_rgy_domain_group,
                                   group_name,
                                   member_name,
                                   (error_status_t *)&result);
            if (result != sec_rgy_status_ok) {
                free((char *)aargv);
                free((char *)margv);
                DCP_SET_MISC_ERROR(result);
            }
        }
    }
    free((char *)aargv);
    free((char *)margv);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_list()
 *
 * OVERVIEW:
 * 
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 *
 */
int rgy_group_list(ClientData clientData,
                   Tcl_Interp *interp,
                   int argc,
                   char **argv)
{
    error_status_t    result = TCL_OK;
    sec_rgy_cursor_t  item_cursor;
    char              *output;
    sec_rgy_name_t    global_name;
    sec_rgy_name_t    cell_name;
    sec_rgy_name_t    group_name;
    int               largc = 0;
    char              **largv;
    int               i = 0;
    int               j = 0;
    signed32          members_supplied = 0;
    signed32          members_avail = 0;
    signed32          max_members = 1024;
    signed32          total_supplied;
    sec_rgy_member_t  *member_list;
    boolean32         simplename = FALSE;
    size_t            group_name_size = 0;
    boolean32         is_simple;
    boolean32         member_is_foreign;

    dcp_ArgvInfo arg_table[] = { 
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_group_list_simple_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    arg_table[0].dst = (char *)&simplename;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (Tcl_SplitList(interp, argv[1], &largc, &largv) != TCL_OK) {
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

    if ((is_simple = is_simple_name(largc, largv)) == -1) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    member_list = (sec_rgy_member_t *)malloc(sizeof(sec_rgy_member_t) * max_members);
    if (member_list == NULL) {
        free((char *)largv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    for (; i < largc; i++) {
        strcpy((char *)global_name, largv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)largv);
            free(member_list);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_query) != TCL_OK) {
            free((char *)largv);
            free(member_list);
            return TCL_ERROR;
        }

        sec_rgy_cursor_reset(&item_cursor);
        total_supplied = 0;
        do {
            sec_rgy_pgo_get_members(rgy_context,
                                    sec_rgy_domain_group,
                                    group_name,
                                    &item_cursor,
                                    max_members,
                                    member_list,
                                    &members_supplied,
                                    &members_avail,
                                    &result);
            if (result != sec_rgy_status_ok) {
                free((char *)largv);
                free(member_list);
                DCP_SET_MISC_ERROR(result);
            }

            if ((strlen(interp->result)) && (total_supplied == 0)) {
                Tcl_AppendResult(interp, "\n", NULL);
            }

            group_name_size = strlen((char *)cell_name) + sec_rgy_pname_t_size + 1;
            output = (char *)malloc(group_name_size);
            if (output == NULL) {
                free((char *)largv);
                free(member_list);
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }

            total_supplied += members_supplied;
            for (j = 0 ; j <  members_supplied; j++) {
                member_is_foreign = false;
                if (strncmp(GLOBAL_DIR_ROOT, (char *)member_list[j], 
                            GLOBAL_DIR_ROOT_LEN) == 0) {
                    member_is_foreign = true;
                }

                if (strlen(interp->result))
                    Tcl_AppendResult(interp, "\n", NULL);

                /* 
                 * Foreign entries are always fully qualified, print them
                 * just as they are regardless of -simplename.
                 */
                if ((!simplename) && (!member_is_foreign))
                    sprintf(output, "%s/%s", cell_name, member_list[j]);
                else
                    sprintf(output, "%s", member_list[j]);
                Tcl_AppendResult(interp, output, NULL);
            }
            free(output);
        } while (members_avail > total_supplied);
    }
    free((char *)largv);
    free(member_list);
    return TCL_OK;
}


/*
 * FUNCTION: rgy_group_remove()
 *
 * OVERVIEW:
 * 
 * INPUTS:
 *  int argc - The number of command line arguments passed into function.
 *  char **argv - The actual arguements.
 *
 * INPUT/OUTPUTS:
 * Tcl_Interp *interp - Structure associated with the interpreter. This        
 *                      will hold the error message (if any) generated from the
 *                      command. This is also the location of any convienience
 *                      variables.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  int either TCL_OK or TCL_ERROR to notify the interpreter of successful 
 *      completion or command failure. 
 *    
 *
 */
int rgy_group_remove(ClientData clientData, 
                     Tcl_Interp *interp, 
                     int argc, 
                     char **argv)
{
    int             result = TCL_OK;
    char            *member = NULL;
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  group_name;
    sec_rgy_name_t  member_name;
    char            **aargv;
    int             aargc = 0;
    char            **margv;
    int             margc = 0;
    int             i = 0;
    int             j = 0;
    boolean32       is_simple;
    
    dcp_ArgvInfo arg_table[] = {
        {"-member", DCP_ARGV_STRING, NULL, NULL,
            dcp_t_rgy_group_remove_mem_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /* 
     * Set the variables for the Parser. We need to cast them all as 
     * char pointers. 
     */
    arg_table[0].dst = (char *)&member;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Verify that the user provided a group name with the command. 
     * Also check that no extra arguments were passed in. 
     * If so initialize the pgo name parameter and the namelist.
     */
    DCP_CHECK_RGY_NO_ARG(group);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    if (!member) {
        DCP_SET_MISC_ERROR(dcp_s_rgy_no_member_name);
    }

    /* 
     * Split both the member list and the list of objects (groups) to 
     * operate on. 
     */
    if (Tcl_SplitList(interp, argv[1], &aargc, &aargv) != TCL_OK) {
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((aargc == 0) && (aargv[0] == NULL)) {
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if (Tcl_SplitList(interp, member, &margc, &margv) != TCL_OK) {
        free((char *)aargv);
        return TCL_ERROR;
    }
    else {
        /*
         * Check that we did not receive an empty argument list.
         */
        if ((margc == 0) && (margv[0] == NULL)) {
            free((char *)aargv);
            DCP_SET_MISC_ERROR(dcp_s_null_arg);
        }
    }

    if ((is_simple = is_simple_name(aargc, aargv)) == -1) {
        free((char *)aargv);
        free((char *)margv);
        DCP_SET_MISC_ERROR(dcp_s_rgy_full_or_simple);
    }

    for (; i < aargc; i++) {
        /*
         * Initialize the name parameter
         */
        strcpy((char *)global_name, aargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, group_name) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)aargv);
            free((char *)margv);
            return TCL_ERROR;
        }
 
        /* 
         * Cycle through all members and initialize the member
         * parameter.
         */
        for (j = 0; j < margc; j++) {
            strcpy((char *)global_name, margv[j]);


            /* 
             * If the name passed in _appears_ to be fully qualified
             * (ie. starts with /.../), don't run it through 
             * dcp_id_parse_name(). If the name is foreign, it would
             * force a call to the remote security server. Once this
             * name is handed to secd in the sec_rgy_pgo_add_member()
             * call below, secd will call sec_id_parse_name() itself.
             */
            if (strncmp(GLOBAL_DIR_ROOT, (char *)global_name, GLOBAL_DIR_ROOT_LEN) != 0) {
            	if (dcp_id_parse_name(interp, global_name, cell_name, member_name) != TCL_OK) {
                    free((char *)aargv);
                    free((char *)margv);
                    return TCL_ERROR;
                }
            }
            else {
                strcpy((char *)member_name, (char *)global_name);
            }

            sec_rgy_pgo_delete_member(rgy_context,
                                      sec_rgy_domain_group,
                                      group_name,
                                      member_name,
                                      (error_status_t *)&result);
            if (result != sec_rgy_status_ok) {
                free((char *)aargv);
                free((char *)margv);
                DCP_SET_MISC_ERROR(result);
            }
        }
    }
    free((char *)aargv);
    free((char *)margv);
    return TCL_OK;
}
