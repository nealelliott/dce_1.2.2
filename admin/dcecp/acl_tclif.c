/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_tclif.c,v $
 * Revision 1.1.4.2  1996/02/18  19:11:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:19:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/10/19  17:10 UTC  truitt
 * 	Merge local work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts50/1  1995/10/19  17:08 UTC  truitt
 * 	CHFts16529: Correct order of acl options.  Fix malloc's and free's.
 * 
 * 	HP revision /main/HPDCE02/4  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/3  1995/09/24  07:16 UTC  tatsu_s
 * 	Removed useless -local options.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:49 UTC  tatsu_s
 * 	Added illegal option checks.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/21  15:30 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/18  19:40 UTC  jrr
 * 	Fix input checking.
 * 	[1995/07/18  18:50 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/01  18:49 UTC  truitt
 * 	Merge changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  18:48 UTC  truitt
 * 	CHFts15085: Fix some memory leaks.
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/14  14:17 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chfts19/1  1995/03/14  14:15 UTC  truitt
 * 	CHFts14241: Add the interp parameter to the free
 * 	acl manager table function.  Change it from void
 * 	to int.
 * 	[1995/12/08  17:54:01  root]
 * 
 * Revision 1.1.2.23  1994/09/08  14:00:40  salamone
 * 	CR11968-Setting authentication on binding handles.
 * 	[1994/09/08  13:59:13  salamone]
 * 
 * Revision 1.1.2.22  1994/08/25  15:32:59  salamone
 * 	CR11635 - Handle 1 string binding specification for an ACL object.
 * 	[1994/08/25  15:28:37  salamone]
 * 
 * Revision 1.1.2.21  1994/08/08  18:51:39  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:05  melman]
 * 
 * Revision 1.1.2.20  1994/08/05  19:37:20  salamone
 * 	Free ACL manager table for each ACL object specified.
 * 	[1994/08/05  19:36:48  salamone]
 * 
 * Revision 1.1.2.19  1994/07/29  18:44:34  salamone
 * 	CR11205 - Maintain mask_calc_type for each ACL mgr.
 * 	[1994/07/29  18:28:06  salamone]
 * 
 * Revision 1.1.2.18  1994/07/07  14:35:40  salamone
 * 	Provide support for more than one ACL manager.
 * 	[1994/07/07  14:31:44  salamone]
 * 
 * Revision 1.1.2.17  1994/05/18  20:39:04  salamone
 * 	CR10621 - "acl show", "acl check", and "acl permissions" should
 * 		   display the appropriate info if unauthenticated.
 * 	[1994/05/13  20:30:41  salamone]
 * 
 * Revision 1.1.2.16  1994/05/06  17:35:51  salamone
 * 	CR10211: -mask calc is ignored.
 * 	[1994/05/06  17:33:03  salamone]
 * 
 * Revision 1.1.2.15  1994/05/05  14:09:41  salamone
 * 	Free argv properly.
 * 	[1994/05/05  14:08:22  salamone]
 * 
 * Revision 1.1.2.14  1994/05/04  21:50:37  salamone
 * 	CR10094 - Remove -ic and -io switches from acl check.
 * 	          Remove completely from the argv table.
 * 	[1994/05/04  21:15:35  salamone]
 * 
 * Revision 1.1.2.13  1994/03/31  22:04:30  salamone
 * 	CR10094 - Remove -ic and -io switches from acl check.
 * 	[1994/03/31  21:50:27  salamone]
 * 
 * Revision 1.1.2.12  1994/03/04  14:46:05  salamone
 * 	CR10062: Removed dcp_s_acl_no_posix_semantics msg and check.
 * 	[1994/03/04  14:44:44  salamone]
 * 
 * Revision 1.1.2.11  1994/03/02  20:38:32  salamone
 * 	CR10044: mask_obj entry cannot be added to ACL object that supports it.
 * 	[1994/03/02  20:35:24  salamone]
 * 
 * Revision 1.1.2.10  1994/02/15  20:14:04  salamone
 * 	CR9950 Enhanced message descriptions for dcecp acl support
 * 	[1994/02/15  20:12:00  salamone]
 * 
 * Revision 1.1.2.9  1994/02/11  19:58:38  salamone
 * 	CR9944 arg_table for acl modify initialized incorrectly.
 * 	[1994/02/11  19:56:32  salamone]
 * 
 * Revision 1.1.2.8  1994/02/09  18:58:58  salamone
 * 	CR9883 "acl help" and "acl <operations> help" support new help functions.
 * 	[1994/02/09  18:35:32  salamone]
 * 
 * Revision 1.1.2.7  1994/02/09  16:00:18  salamone
 * 	Added "acl replace" and "acl delete" support.
 * 	Added delegation support.
 * 	[1994/02/09  15:36:06  salamone]
 * 
 * Revision 1.1.2.6  1994/01/24  22:23:12  salamone
 * 	Merged with changes from 1.1.2.5
 * 	[1994/01/24  22:23:06  salamone]
 * 
 * 	Added support for "acl_modify"
 * 	[1994/01/24  22:17:14  salamone]
 * 
 * Revision 1.1.2.5  1994/01/10  19:54:51  rousseau
 * 	Fix rios build problems.
 * 	[1994/01/10  19:53:33  rousseau]
 * 
 * Revision 1.1.2.4  1994/01/05  19:46:26  salamone
 * 	Merged with changes from 1.1.2.3
 * 	[1994/01/05  19:46:20  salamone]
 * 
 * 	Added "acl show" support
 * 	[1994/01/05  19:41:51  salamone]
 * 
 * Revision 1.1.2.3  1993/12/30  19:58:46  rousseau
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:58:01  rousseau]
 * 
 * Revision 1.1.2.2  1993/12/29  17:28:22  salamone
 * 	Added "acl check" and "acl permissions" support
 * 	[1993/12/29  17:27:59  salamone]
 * 
 * 	Added "acl permissions" support
 * 	[1993/12/28  22:28:44  salamone]
 * 
 * Revision 1.1.2.1  1993/12/27  20:47:48  salamone
 * 	Initial version of acl_tclif.c
 * 	[1993/12/27  20:43:17  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * acl_tclif.c
 *
 * DESCRIPTION:
 * This module contains all the ACL operations to edit or list an objects 
 * ACLs thru dcecp.
 */

/*
 * INCLUDE FILES
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/binding.h>
#include <dce/daclif.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_error.h>

#include <dce/tcl.h>
#include <dcecp.h>
#include <util_var.h>
#include <util_help.h>
#include <util_signal.h>
#include <util_general.h>

#include <acl_tclif.h>
#include <acl_edit.h>

/*
 * DECLARATIONS 
 */

/*
 * INTERNAL VARIABLES 
 */
static dcp_acl_info_t dcp_acl_info, *dcp_acl_info_p; 

/*
 * PROTOTYPES 
 */


/*
 * FUNCTION:
 * dcp_acl_check()
 *
 * OVERVIEW:
 * This routine returns the permissions granted in the specified
 * object's ACL to the principal that invoked the command.
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
int dcp_acl_check(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int        i;  
    char       **largv;
    int        largc;
    int        rc;
    char       *mgr_type_string = NULL;
    int        namespace_entry = 0;
    boolean32  bind_to_entry = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-entry", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_namespace_entry_help}, 
        {"-type", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_mgr_type_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&namespace_entry;
    arg_table[1].dst = (char *)&mgr_type_string;

    dcp_acl_info_p = &dcp_acl_info;

    /*
     * Process command line arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /* 
     * Check to see if the user just wants help 
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_acl_no_object);
    }
    DCP_CHECK_EXTRA_ARGS()

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /*
     * Determine the ACL type.  Set initially to the ACL object
     * itself.  Determine whether to bind to ACL of the namespace
     * entry.  Set initially to bind to the ACL object itself.
     */
    dcp_acl_info.sec_acl_type = sec_acl_type_object;
    dcp_acl_info.is_local = FALSE;

    if (namespace_entry) {
        bind_to_entry = TRUE;
    }

    /*
     * Determine ACL manager.  Set initially to the first ACL
     * manager for the object.
     */
    dcp_acl_info_p->mgr_type_index = 0;
    if (mgr_type_string == NULL) {
        dcp_acl_info_p->mgr_type_specified[0] = '\0';
    } else {
        strncpy(dcp_acl_info_p->mgr_type_specified, mgr_type_string, sec_acl_printstring_len);
    }

    /* 
     * Cycle through all ACL objects that were specified to be listed 
     */
    if (dcp_acl_list_objects(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_acl_info.sec_acl_named_object = dce_strdup((char *)largv[i]);
        rc = dcp_acl_bind_named_object(interp, dcp_acl_info_p, bind_to_entry);
        if (rc != TCL_OK) {
            break;
        }

        rc = dcp_acl_do_check(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            break;
        }

        if (i != (largc - 1)) {
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
        }
        rc = dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            free((char *)largv);
            return rc;
        }
    }

    /*
     * List of elements are no longer needed
     */
    if (rc != TCL_OK) {
        dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
    }
    free((char *)largv);
    return(rc);
}


/*
 * FUNCTION:
 * dcp_acl_delete()
 *
 * OVERVIEW:
 * This routine removes all ACL entries from the object, except the 
 * user_obj entry, if it exists.
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
int dcp_acl_delete(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int        i;
    char       **largv;
    int        largc;
    int        rc;
    char       *mgr_type_string = NULL;
    int        init_container = 0;
    int        init_object = 0;
    int        namespace_entry = 0;
    int        acl_type_cnt = 0;
    boolean32  bind_to_entry = FALSE;
    boolean32  local_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-entry", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_namespace_entry_help}, 
        {"-ic", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_container_help}, 
        {"-io", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_object_help}, 
        {"-local", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_local_help},
        {"-type", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_mgr_type_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&namespace_entry;
    arg_table[1].dst = (char *)&init_container;
    arg_table[2].dst = (char *)&init_object;
    arg_table[3].dst = (char *)&local_only;
    arg_table[4].dst = (char *)&mgr_type_string;

    dcp_acl_info_p = &dcp_acl_info;

    /*
     * Process command line arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

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
        DCP_SET_MISC_ERROR(dcp_s_acl_no_object);
    }
    DCP_CHECK_EXTRA_ARGS()

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Determine ACL type.  Set initially to the ACL object
     * itself.  Determine whether to bind to ACL of the namespace
     * entry.  Set initially to bind to the ACL object itself.
     */
    dcp_acl_info.sec_acl_type = sec_acl_type_object;
    dcp_acl_info.is_local = local_only;

    if (namespace_entry) {
        bind_to_entry = TRUE;
        acl_type_cnt++;
    }
    if (init_container) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_container;
        acl_type_cnt++;
    }
    if (init_object) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_object;
        acl_type_cnt++;
    }
    if (acl_type_cnt > 1) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_types);
    }

    /*
     * Determine ACL manager.
     * Set initially to the first ACL manager for the object.
     */
    dcp_acl_info_p->mgr_type_index = 0;
    if (mgr_type_string == NULL) {
        dcp_acl_info_p->mgr_type_specified[0] = '\0';
    } else {
        strncpy(dcp_acl_info_p->mgr_type_specified, mgr_type_string, sec_acl_printstring_len);
    }

    /* 
     * Cycle through all ACL objects that were specified to be listed 
     */
    if (dcp_acl_list_objects(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_acl_info.sec_acl_named_object = dce_strdup((char *)largv[i]);
        rc = dcp_acl_bind_named_object(interp, dcp_acl_info_p, bind_to_entry);
        if (rc != TCL_OK) {
            break;
        }

        /*
         * Process list of ACL entries to delete.
         */
        rc = dcp_acl_do_delete_entries(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            break;
        }

        /*
         * Save the ACL if no errors occurred.
         */
        rc = dcp_acl_do_save_sec_acl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            break;
        }

        rc = dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            free((char *)largv);
            return rc;
        }

    }

    /*
     * List of elements are no longer needed
     */
    if (rc != TCL_OK) {
        dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
    }
    free((char *)largv);
    return(rc);
}


/*
 * FUNCTION:
 * dcp_acl_modify()
 *
 * OVERVIEW:
 * This routine changes one or more individual ACL entries. The
 * specific operation to perform is described with one or more of the
 * following switches:
 *    i) -add
 *	Adds the ACL entries to the ACL. The value of this switch is a
 *	list of ACL entries with permissions filled in.
 *   ii) -remove
 *	Removes the ACL entries from the ACL. The value of this switch 
 *	is a list of ACL entries without permissions filled in.
 *  iii) -change
 *	Changes existing ACL entries in the ACL. The value of this switch 
 *	is a list of ACL entries with permissions filled in. The
 *      permissions will be the new permissions placed on the specified 
 *      ACL entries. The ACL entries must exist in the ACL or an error 
 *      occurs.
 *   iv) -purge
 *      Purges all masked permissions (before any modifications are
 *      made), in all ACL entries except: user_obj, other_obj, 
 *      mask_obj, and unauthenticated if they exist. This option is 
 *      useful only for ACLs that contain an entry of type mask_obj.
 *    v) -mask
 *      If a modify operation causes a mask recalculation that 
 *      unintentionally adds permissions to an existing acl entry, the
 *      modify operation will abort with an error unless you specify
 *      the -mask switch with a value of either calc or nocalc, or a
 *      unique abbreviation of one of these values. The -mask switch 
 *      should only be valid with the -add and -change switches on
 *      the "acl modify" operation.
 *
 * The modify operation can change the value of the cell attribute
 * using an attribute switch called -cell and giving the new default
 * cell as the value. It must be one value not a list.
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
int dcp_acl_modify(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int        i;  
    char       **largv;
    int        largc;
    int        rc;
    int        init_container = 0;
    int        init_object = 0;
    int        namespace_entry = 0;
    int        acl_type_cnt = 0;
    int        purge_switch = 0;
    char       *mgr_type_string = (char *)NULL;
    char       *mask_string = (char *)NULL;
    char       *add_entries_list = (char *)NULL;
    char       *remove_entries_list = (char *)NULL;
    char       *change_entries_list = (char *)NULL;
    char       *change_cell_attribute = (char *)NULL;
    boolean32  bind_to_entry = FALSE;
    boolean32  local_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-add", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_modify_add_help},
        {"-cell", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_modify_cell_help},
        {"-change", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_modify_change_help}, 
        {"-entry", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_namespace_entry_help}, 
        {"-ic", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_container_help}, 
        {"-io", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_object_help}, 
        {"-local", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_local_help},
        {"-mask", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_modify_mask_help}, 
        {"-purge", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_modify_purge_help}, 
        {"-remove", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_modify_remove_help}, 
        {"-type", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_mgr_type_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&add_entries_list;
    arg_table[1].dst = (char *)&change_cell_attribute;
    arg_table[2].dst = (char *)&change_entries_list;
    arg_table[3].dst = (char *)&namespace_entry;
    arg_table[4].dst = (char *)&init_container;
    arg_table[5].dst = (char *)&init_object;
    arg_table[6].dst = (char *)&local_only;
    arg_table[7].dst = (char *)&mask_string;
    arg_table[8].dst = (char *)&purge_switch;
    arg_table[9].dst = (char *)&remove_entries_list;
    arg_table[10].dst = (char *)&mgr_type_string;

    dcp_acl_info_p = &dcp_acl_info;

    /*
     * Process command line arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

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
        DCP_SET_MISC_ERROR(dcp_s_acl_no_object);
    }
    DCP_CHECK_EXTRA_ARGS()

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * At least one of these switches must be specified.
     */
    if (!add_entries_list && 
        !remove_entries_list && 
        !change_entries_list &&
        !purge_switch &&
        !change_cell_attribute) {
 	DCP_SET_MISC_ERROR(dcp_s_acl_no_acl_modify_op);
    }

    /*
     * Determine ACL type.  Set initially to the ACL object
     * itself.  Determine whether to bind to ACL of the namespace
     * entry.  Set initially to bind to the ACL object itself.
     */
    dcp_acl_info.sec_acl_type = sec_acl_type_object;
    dcp_acl_info.is_local = local_only;

    if (namespace_entry) {
        bind_to_entry = TRUE;
        acl_type_cnt++;
    }
    if (init_container) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_container;
        acl_type_cnt++;
    }
    if (init_object) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_object;
        acl_type_cnt++;
    }
    if (acl_type_cnt > 1) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_types);
    }

    /*
     * Determine ACL manager.
     * Set initially to the first ACL manager for the object.
     */
    dcp_acl_info_p->mgr_type_index = 0;
    if (mgr_type_string == NULL) {
        dcp_acl_info_p->mgr_type_specified[0] = '\0';
    } else {
        strncpy(dcp_acl_info_p->mgr_type_specified, mgr_type_string, sec_acl_printstring_len);
    }

    /*
     * The -mask switch can only be entered on a particular ACL entry 
     * specification: -add & -change switches
     */
    if ((mask_string != NULL) && (purge_switch) &&
        (add_entries_list == NULL) && (change_entries_list == NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_acl_no_mask_with_option);
    }

    if ((mask_string != NULL) && (change_cell_attribute != NULL) &&
        (add_entries_list == NULL) && (change_entries_list == NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_acl_no_mask_with_option);
    }

    if ((mask_string != NULL) && (remove_entries_list != NULL) &&
        (add_entries_list == NULL) && (change_entries_list == NULL)) {
        DCP_SET_MISC_ERROR(dcp_s_acl_no_mask_with_option);
    }

    /* 
     * Cycle through all ACL objects that were specified to be listed 
     */
    if (dcp_acl_list_objects(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_acl_info.sec_acl_named_object = dce_strdup((char *)largv[i]);
        rc = dcp_acl_bind_named_object(interp, dcp_acl_info_p, bind_to_entry);
        if (rc != TCL_OK) {
            break;
        } 

        /*
         * Determine if -mask option specified
         */
        if (mask_string != NULL) {
            if (strstr("nocalc", mask_string)) {
                if (*mask_string == 'n') {
    	            DCP_C_ACL_MGR.mask_calc_type = NO_MASK_OBJ_CALC;
                }
                else if (strstr("calc", mask_string)) {
    	    	    DCP_C_ACL_MGR.mask_calc_type = CALC_MASK_OBJ_AFTER;
                }
                else {
                    free((char *)largv);
                    dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
                    DCP_SET_MISC_ERROR(dcp_s_acl_inv_mask_option_value);
                }
            } else {
                free((char *)largv);
                dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
                DCP_SET_MISC_ERROR(dcp_s_acl_inv_mask_option_value);
            }
        }

        /*
         * Process the cell modification first before processing
         * any ACL entry modifications.
         */
        if (change_cell_attribute != NULL) {
            rc = dcp_acl_do_modify_cell(change_cell_attribute, interp, dcp_acl_info_p);
            if (rc != TCL_OK) {
                break;
            }
        }

        if (add_entries_list || remove_entries_list ||
            change_entries_list || purge_switch) {
            rc = dcp_acl_do_modify_entries(purge_switch,
                                           mask_string,
                                           remove_entries_list,
                                           change_entries_list,
                                           add_entries_list,
                                           interp, 
                                           dcp_acl_info_p);
            if (rc != TCL_OK) {
                break;
            }
        }

        /*
         * Save the ACL if no errors occurred.
         */
        rc = dcp_acl_do_save_sec_acl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            break;
        }

        rc = dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            free((char *)largv);
            return rc;
        }
    }

    /*
     * List of elements are no longer needed
     */
    if (rc != TCL_OK) {
        dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
    }
    free((char *)largv);
    return(rc);
}


/*
 * FUNCTION:
 * dcp_acl_permissions()
 *
 * OVERVIEW:
 * This routine returns a list describing the permissions associated
 * with the object. Each element of the list is a list of two items,
 * the first is the permission token, the second is a description of
 * the permission.
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
int dcp_acl_permissions(ClientData clientData, 
                        Tcl_Interp *interp, 
                        int argc, 
                        char **argv)
{
    int        i;  
    char       **largv;
    int        largc;
    int        rc;
    char       *mgr_type_string = NULL;
    int        init_container = 0;
    int        init_object = 0;
    int        namespace_entry = 0;
    int        acl_type_cnt = 0;
    boolean32  bind_to_entry = FALSE;
    boolean32  local_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-entry", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_namespace_entry_help}, 
        {"-ic", DCP_ARGV_CONSTANT, (char *)1,(char *)NULL,
            dcp_t_acl_init_container_help},
        {"-io", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_object_help}, 
        {"-local", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_local_help},
        {"-type", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_mgr_type_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&namespace_entry;
    arg_table[1].dst = (char *)&init_container;
    arg_table[2].dst = (char *)&init_object;
    arg_table[3].dst = (char *)&local_only;
    arg_table[4].dst = (char *)&mgr_type_string;

    dcp_acl_info_p = &dcp_acl_info;

    /*
     * Process command line arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

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
        DCP_SET_MISC_ERROR(dcp_s_acl_no_object);
    }
    DCP_CHECK_EXTRA_ARGS()

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Determine ACL type.  Set initially to the ACL object
     * itself.  Determine whether to bind to ACL of the namespace
     * entry.  Set initially to bind to the ACL object itself.
     */
    dcp_acl_info.sec_acl_type = sec_acl_type_object;
    dcp_acl_info.is_local = local_only;

    if (namespace_entry) {
        bind_to_entry = TRUE;
        acl_type_cnt++;
    }
    if (init_container) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_container;
        acl_type_cnt++;
    }
    if (init_object) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_object;
        acl_type_cnt++;
    }
    if (acl_type_cnt > 1) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_types);
    }

    /*
     * Determine ACL manager.
     * Set initially to the first ACL manager for the object.
     */
    dcp_acl_info_p->mgr_type_index = 0;
    if (mgr_type_string == NULL) {
        dcp_acl_info_p->mgr_type_specified[0] = '\0';
    } else {
        strncpy(dcp_acl_info_p->mgr_type_specified, mgr_type_string, sec_acl_printstring_len);
    }

    /* 
     * Cycle through all ACL objects that were specified to be listed 
     */
    if (dcp_acl_list_objects(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_acl_info.sec_acl_named_object = dce_strdup((char *)largv[i]);
        rc = dcp_acl_bind_named_object(interp, dcp_acl_info_p, bind_to_entry);
        if (rc != TCL_OK) {
            break;
        }

        dcp_acl_do_permissions(interp, dcp_acl_info_p);

        if (i != (largc - 1)) {
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
        }

        rc = dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            free((char *)largv);
            return rc;
        }
    }

    /*
     * List of elements are no longer needed
     */
    if (rc != TCL_OK) {
        dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
    }
    free((char *)largv);
    return(rc);
}


/*
 * FUNCTION:
 * dcp_acl_replace()
 *
 * OVERVIEW:
 * This routine replaces the entire ACL on the named object with the
 * value of the -acl switch. The syntax of the value of the -acl 
 * switch is a list of ACL entries with permissions filled in.
 * The -acl switch is required.
 *
 * The replace operation can replace the value of the cell attribute
 * using an attribute switch called -cell and giving the new default
 * cell as the value. It must be one value not a list. The -cell
 * switch is optional.
 *
 * In POSIX 1003.6 Draft 12 states that if replace (-i/-I)
 * and the new entries do NOT specify a "mask_obj" entry, then the mask 
 * shall be set as described by -c.
 *
 * The algorithm follows:
 * 1) There is be no -mask switch on "acl replace".
 * 2) If the "mask_obj" entry is specified, then no mask
 *    recalculation will occur (-n).
 * 3) If the "mask_obj" entry is NOT specified, then a mask
 *    recalculation will occur (-c). Note that -c is not
 *    following the POSIX 1003.6 steps 3,4,5,7,& 8 but the
 *    "mask_obj" entry will be calculated with the union of all entries
 *    except user_obj, other_obj, and unauthenticated.
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
int dcp_acl_replace(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int        i;  
    char       **largv;
    int        largc;
    int        rc;
    int        init_container = 0;
    int        init_object = 0;
    int        namespace_entry = 0;
    int        acl_type_cnt = 0;
    char       *mgr_type_string = (char *)NULL;
    char       *replace_entries_list = (char *)NULL;
    char       *replace_cell_attribute = (char *)NULL;
    boolean32  bind_to_entry = FALSE;
    boolean32  local_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-acl", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_replace_entries_help}, 
        {"-cell", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_replace_cell_help}, 
        {"-entry", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_namespace_entry_help}, 
        {"-ic", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_container_help}, 
        {"-io", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_object_help}, 
        {"-local", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_local_help},
        {"-type", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_mgr_type_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&replace_entries_list;
    arg_table[1].dst = (char *)&replace_cell_attribute;
    arg_table[2].dst = (char *)&namespace_entry;
    arg_table[3].dst = (char *)&init_container;
    arg_table[4].dst = (char *)&init_object;
    arg_table[5].dst = (char *)&local_only;
    arg_table[6].dst = (char *)&mgr_type_string;

    dcp_acl_info_p = &dcp_acl_info;

    /*
     * Process command line arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

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
        DCP_SET_MISC_ERROR(dcp_s_acl_no_object);
    }
    DCP_CHECK_EXTRA_ARGS()

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The -acl switch is required.
     */
    if (!replace_entries_list) {
        DCP_SET_MISC_ERROR(dcp_s_acl_no_acl_replace_op); 
    }

    /*
     * Determine ACL type.  Set initally to the ACL object
     * itself.  Determine whether to bind to ACL of the namespace
     * entry.  Set initially to bind to the ACL object itself.
     */
    dcp_acl_info.sec_acl_type = sec_acl_type_object;
    dcp_acl_info.is_local = local_only;

    if (namespace_entry) {
        bind_to_entry = TRUE;
        acl_type_cnt++;
    }
    if (init_container) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_container;
        acl_type_cnt++;
    }
    if (init_object) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_object;
        acl_type_cnt++;
    }
    if (acl_type_cnt > 1) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_types);
    }

    /*
     * Determine ACL manager.
     * Set initially to the first ACL manager for the object.
     */
    dcp_acl_info_p->mgr_type_index = 0;
    if (mgr_type_string == NULL) {
        dcp_acl_info_p->mgr_type_specified[0] = '\0';
    } else {
        strncpy(dcp_acl_info_p->mgr_type_specified, mgr_type_string, sec_acl_printstring_len);
    }

    /* 
     * Cycle through all ACL objects that were specified to be listed 
     */
    if (dcp_acl_list_objects(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_acl_info.sec_acl_named_object = dce_strdup((char *)largv[i]);
        rc = dcp_acl_bind_named_object(interp, dcp_acl_info_p, bind_to_entry);
        if (rc != TCL_OK) {
            break;
        }

        /*
         * For ACL replacement, the mask calculation type
         * shall be set initially to calculate the mask (-c)
         * if the ACL manager supports POSIX "mask_obj" semantics.
         */
        if (DCP_C_ACL_MGR.mask_calc_type == POSIX_MASK_OBJ_CALC) {
            DCP_C_ACL_MGR.mask_calc_type = CALC_MASK_OBJ_AFTER;
        }

        /*
         * Process the cell replacement first before processing
         * the ACL entry replacement.
         */
        if (replace_cell_attribute != NULL) {
            rc = dcp_acl_do_modify_cell(replace_cell_attribute, interp, dcp_acl_info_p);
            if (rc != TCL_OK) {
                break;
            }
        }

        /*
         * Process list of ACL entries to replace.
         */
        rc = dcp_acl_do_replace_entries(replace_entries_list, interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            break;
        }

        /*
         * Save the ACL if no errors occurred.
         */
        rc = dcp_acl_do_save_sec_acl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            break;
        }

        rc = dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            free((char *)largv);
            return rc;
        }
    }

    /*
     * List of elements are no longer needed
     */
    if (rc != TCL_OK) {
        dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
    }
    free((char *)largv);
    return(rc);

}


/*
 * FUNCTION:
 * dcp_acl_show()
 *
 * OVERVIEW:
 * This routine returns a list of the ACl on the named object. If they
 * exist, the mask_obj and unauthenticated entries are displayed
 * first.
 * If called with the -cell switch returns the value of the default
 * cell for the ACL.
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
int dcp_acl_show(ClientData clientData, Tcl_Interp *interp,int argc, char **argv)
{
    int        i;
    char       **largv;
    int        largc;
    int        rc;
    char       *mgr_type_string = (char *)NULL;
    int        init_container = 0;
    int        init_object = 0;
    int        namespace_entry = 0;
    int        show_cell = 0;
    int        show_mgrs = 0;
    int        acl_type_cnt = 0;
    boolean32  bind_to_entry = FALSE;
    boolean32  local_only = FALSE;

    dcp_ArgvInfo arg_table[] = {
        {"-cell", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_show_cell_help}, 
        {"-entry", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_namespace_entry_help}, 
        {"-ic", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_container_help},
        {"-io", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_init_object_help}, 
        {"-local", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_local_help},
        {"-managers", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_acl_show_mgrs_help}, 
        {"-type", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_acl_mgr_type_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&show_cell;
    arg_table[1].dst = (char *)&namespace_entry;
    arg_table[2].dst = (char *)&init_container;
    arg_table[3].dst = (char *)&init_object;
    arg_table[4].dst = (char *)&local_only;
    arg_table[5].dst = (char *)&show_mgrs;
    arg_table[6].dst = (char *)&mgr_type_string;

    dcp_acl_info_p = &dcp_acl_info;

    /*
     * Process command line arguments
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

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
        DCP_SET_MISC_ERROR(dcp_s_acl_no_object);
    }
    DCP_CHECK_EXTRA_ARGS()

    /*
     * Set convenience variables
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Options are mutually exclusive.
     */
    if (((show_cell) && (show_mgrs)) || ((mgr_type_string) && (show_mgrs))) {
        DCP_SET_MISC_ERROR(dcp_s_mut_excl_opts);
    }

    /*
     * Determine ACL type.
     * Set initially to the ACL object itself.
     * Determine whether to bind to ACL of the namespace entry.
     * Set initially to bind to the ACL object itself.
     */
    dcp_acl_info.sec_acl_type = sec_acl_type_object;
    dcp_acl_info.is_local = local_only;

    if (namespace_entry) {
        bind_to_entry = TRUE;
        acl_type_cnt++;
    }
    if (init_container) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_container;
        acl_type_cnt++;
    }
    if (init_object) {
        dcp_acl_info.sec_acl_type = sec_acl_type_default_object;
        acl_type_cnt++;
    }
    if (acl_type_cnt > 1) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_types);
    }

    /*
     * Determine ACL manager.
     * Set initially to the first ACL manager for the object.
     */
    dcp_acl_info_p->mgr_type_index = 0;
    if (mgr_type_string == NULL) {
        dcp_acl_info_p->mgr_type_specified[0] = '\0';
    } else {
        strncpy(dcp_acl_info_p->mgr_type_specified, mgr_type_string, sec_acl_printstring_len);
    }

    /* 
     * Cycle through all ACL objects that were specified to be listed 
     */
    if (dcp_acl_list_objects(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_acl_info.sec_acl_named_object = dce_strdup((char *)largv[i]);
        rc = dcp_acl_bind_named_object(interp, dcp_acl_info_p, bind_to_entry);
        if (rc != TCL_OK) {
            break;
        }

        if ((!show_cell) && (!show_mgrs)) {
            rc = dcp_acl_do_show_entries(interp, dcp_acl_info_p);
            if (rc != TCL_OK) {
                break;
            }
        }
        if (show_cell) {
            dcp_acl_do_show_cell(interp, dcp_acl_info_p);
        }
        if (show_mgrs) {
            dcp_acl_do_show_mgrs(interp, dcp_acl_info_p);
        }

        if (i != (largc - 1)) {
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
        }

        rc = dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
        if (rc != TCL_OK) {
            free((char *)largv);
            return rc;
        }
    }

    /*
     * List of elements are no longer needed
     */
    if (rc != TCL_OK) {
        dcp_acl_free_acl_mgr_tbl(interp, dcp_acl_info_p);
    }
    free((char *)largv);
    return(rc);
}
