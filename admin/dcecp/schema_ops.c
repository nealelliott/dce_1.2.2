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
 * $Log: schema_ops.c,v $
 * Revision 1.1.6.1  1996/08/09  11:51:13  arvind
 * 	Merge changes.
 * 	[1996/06/12  14:55 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/03/28  18:58 UTC  truitt  /main/HPDCE02/10]
 *
 * 	Merge changes.
 * 	[1996/03/28  18:57 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf72/1]
 *
 * 	CHFts17168: Trap an empty argument list correctly.
 * 	[1996/03/18  18:12 UTC  truitt  /main/HPDCE02/9]
 *
 * 	Merge changes.
 * 	[1996/03/18  18:11 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf69/1]
 *
 * 	CHFts17225: Modify some on-line help messages to match current functionality.
 * 	CHFts17407: Condense some of the sam messages that are duplicates.
 * 	[1996/02/15  18:13 UTC  truitt  /main/HPDCE02/8]
 *
 * 	Merge brief changes.
 * 	[1996/02/15  18:11 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts17451: Remove include of value.h so duplicate macro
 * 	warning does not appear during HP build.  The include is
 * 	required on other platforms.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  14:35 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:15:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:10  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:23:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/06/16  18:11 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:09 UTC  truitt
 * 	CHFts15504: Remove unneeded includes.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/22  14:34 UTC  jrr
 * 	Fix freeing of schema binding.
 * 	[1995/05/22  14:33 UTC  jrr  /main/HPDCE02/jrr_sec_chfts15310/2]
 * 
 * 	Fix a memory leak.
 * 	[1995/05/19  20:09 UTC  jrr  /main/HPDCE02/jrr_sec_chfts15310/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/01  19:16 UTC  truitt
 * 	Merge era work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts26/1  1995/05/01  19:15 UTC  truitt
 * 	CHFts14653: Allow encoding type any to be used.
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/05  19:49 UTC  truitt
 * 	Merge schema message work.
 * 
 * 	HP revision /main/HPDCE02/truitt_mothra_chfts24/1  1995/04/05  19:48 UTC  truitt
 * 	CHFts14652: Improve 'schema entry not found' error message.
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/14  16:33 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:32 UTC  truitt
 * 	CHFts14285: Replace macro for checking no schema args
 * 	with the macro that checks for no rgy args, since both
 * 	are the same.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/10  19:14 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  19:13 UTC  truitt
 * 	Some variables were not being initialized or free'd
 * 	correctly.
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  20:28 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  20:27 UTC  truitt
 * 	Fix schema command switches.
 * 	[1995/12/08  17:57:21  root]
 * 
 * Revision 1.1.2.10  1994/09/30  15:31:43  truitt
 * 	OT12434: Changed a few malloc's.
 * 	[1994/09/30  15:00:43  truitt]
 * 
 * Revision 1.1.2.9  1994/09/29  21:28:36  kevins
 * 	OT 12415 fixed suspect malloc lengths
 * 	[1994/09/29  21:27:57  kevins]
 * 
 * Revision 1.1.2.8  1994/09/26  19:39:56  truitt
 * 	OT12339: Removed some 'free_ptrs' calls which caused bus errors.
 * 	[1994/09/26  19:38:09  truitt]
 * 
 * Revision 1.1.2.7  1994/09/26  18:41:13  salamone
 * 	CR11409 - Determine security registry group name on the fly.
 * 		  Improved efficiency of error checking in schema_ops.c.
 * 		  Fix 486 compile warnings in schema_utils.c.
 * 	[1994/09/26  18:26:29  salamone]
 * 
 * Revision 1.1.2.6  1994/09/23  18:43:16  ajayb
 * 	Merged with changes from 1.1.2.5
 * 	[1994/09/23  18:43:04  ajayb]
 * 
 * 	Fixed several memory leaks.
 * 	[1994/09/22  20:39:34  ajayb]
 * 
 * Revision 1.1.2.5  1994/09/23  16:27:03  truitt
 * 	OT12285: Add the open registry site function to the schema
 * 	show and schema catalog commands.  Change the was a binding
 * 	handle is retrieved when the schema command is using the
 * 	generic or hosts schema.
 * 	[1994/09/23  14:30:42  truitt]
 * 
 * Revision 1.1.2.4  1994/09/15  14:18:36  truitt
 * 	OT11682: Changed the way the space for schema name
 * 	was being allocated in the create function.
 * 	[1994/09/15  12:38:37  truitt]
 * 
 * Revision 1.1.2.3  1994/09/13  19:12:50  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:54  kevins]
 * 
 * Revision 1.1.2.2  1994/09/07  20:36:45  ajayb
 * 	set_attr_schema_binding(): Set prot and auth level to
 * 	none when the user is not logged in.
 * 	[1994/09/07  17:51:11  ajayb]
 * 
 * Revision 1.1.2.1  1994/09/01  19:04:55  truitt
 * 	Put schema commands into their own files.
 * 	[1994/09/01  18:10:42  truitt]
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

/* GLOBALS */

char                          *attr_schema_name = NULL;
dce_attr_sch_handle_t         attr_schema_binding_handle = NULL;
extern dcp_login_context_p_t  global_context_list;

/* PROTOTYPES */

void set_attr_schema_binding(dce_attr_component_name_t, error_status_t *);


/*
 * FUNCTION: schema_show
 *
 * OVERVIEW:
 *  This function will take as input the names of one or more schema
 *  attribute names and display all the information associated with
 *  the specified schema attributes.
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
 */
int schema_show(ClientData clientData, 
                Tcl_Interp *interp, 
                int argc, 
                char **argv)
{
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  residual_name;
    int             result = TCL_OK;
    int             i;
    int             pargc;
    char            **pargv;

    /*
     * Check to see if the user just wants help.
     */
    DCP_NO_COMMAND_HELP();

    /* 
     * Terminate the command if not enough arguments are passed in.
     * If the correct number of args has been supplied, set the
     * namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(schema);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Check that the schema attribute name information supplied by the
     * user is in the correct format.  Multiple schema attribute names
     * are allowed.
     */
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

    /*
     * Repeat display for each schema attribute name requested.
     */
    for (i = 0; i < pargc; i++) {
        strcpy((char *)global_name, pargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        /*
         * If we have multiple schema attribute names, put a
         * space between the attributes display of each one.
         */
        if (strlen(interp->result)) {
            Tcl_AppendResult(interp, "\n\n", NULL);
        }

        /*
         * Pass the schema attribute name to the show function,
         * which will do a lookup by name and format the output
         * buffer with the proper values.
         */
        result = show_schema_info(interp, pargv[i]);
    }

    free((char *)pargv);
    return result;
}


/*
 * FUNCTION: schema_catalog
 *
 * OVERVIEW:
 *  This function will take as input one or more schema object names
 *  and display all the schema attribute names associated with that
 *  schema object.
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
 */
int schema_catalog(ClientData clientData, 
                   Tcl_Interp *interp, 
                   int argc, 
                   char **argv)
{
    sec_rgy_name_t  global_name;
    sec_rgy_name_t  cell_name;
    sec_rgy_name_t  residual_name;
    int             result = TCL_OK;
    int             i;
    int             pargc;
    int             simplename = 0;
    char            **pargv;
    char            *full_schema_name = NULL;
    error_status_t  status;

    dcp_ArgvInfo arg_table[] = {
        {"-simplename", DCP_ARGV_CONSTANT, (char *)1, NULL,
            dcp_t_rgy_schema_cat_simple_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0}
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Set the variables for the Parser. We need to cast them
     * all as char pointers. 
     */
    arg_table[0].dst = (char *)&simplename;

    if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* 
     * Terminate the command if not enough arguments are passed in.
     * If the correct number of args has been supplied, set the
     * namelist accordingly.
     */
    DCP_CHECK_RGY_NO_ARG(schema);
    DCP_CHECK_EXTRA_ARGS();
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * Check that the schema name information supplied by the user
     * is in the correct format.  Multiple schema names are allowed.
     */
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

    /*
     * Repeat display for each schema name requested.
     */
    for (i = 0; i < pargc; i++) {
        strcpy((char *)global_name, pargv[i]);
        if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
            free((char *)pargv);
            return TCL_ERROR;
        }

        /*
         * If we have multiple schema names, put a space
         * between the attributes display of each one.
         */
        if (strlen(interp->result)) {
            Tcl_AppendResult(interp, "\n\n", NULL);
        }

        /*
         * If not displaying simplenames, expand the schema name to
         * the full schema name, providing an expanded cell name.
         */
        if (!simplename) {
            full_schema_name = expand_cell_name(pargv[i], &status);
            DCP_CHECK_RPC_ERROR_FREE_ONE(status, pargv);
        }

        /*
         * Pass the schema name to the show function, which
         * will do a lookup and format the output buffer
         * with the proper values.
         */
        result = catalog_schema_info(interp, pargv[i], full_schema_name);

        /*
         * Free the current full_schema_name if necessary.
         */
        if (!simplename)
            free(full_schema_name);
    }

    free((char *)pargv);
    return result;
}


/*
 * FUNCTION: schema_create
 *
 * OVERVIEW:
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
 */
int schema_create(ClientData clientData, 
                  Tcl_Interp *interp, 
                  int argc, 
                  char **argv)
{
   unsigned32               result = TCL_OK;
   sec_rgy_name_t           global_name;
   sec_rgy_name_t           cell_name;
   sec_rgy_name_t           residual_name;
   sec_attr_schema_entry_t  schema_entry;
   error_status_t           status;
   char                     *attribute = NULL;
   char                     *attr_id = NULL;
   char                     *attr_encoding = NULL;
   char                     *acl_mgr_set = NULL;
   char                     *unique = NULL;
   char                     *multi_valued = NULL;
   char                     *reserved= NULL;
   char                     *apply_defaults = NULL;
   char                     *intercell_action = NULL;
   char                     *trig_types = NULL;
   char                     *trig_binding = NULL;
   char                     *scope = NULL;
   char                     *comment = NULL;
   char                     *schema_name = NULL;
   char                     *schema_attr_name = NULL;
   char                     *full_schema_name = NULL;
   char                     *full_sec_group_name = NULL;
   unsigned_char_p_t        rs_ns_sec_group_name = NULL;
   int                      i;
   int                      cargc = 0;
   char                     **cargv;

   dcp_ArgvInfo arg_table[] = {
      {"-aclmgr", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_aclmgr_help},
      {"-annotation", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_annotation_help},
      {"-applydefs", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_applydefs_help},
      {"-attribute", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_attribute_help},
      {"-encoding", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_encoding_help},
      {"-intercell", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_intercell_help},
      {"-multivalued", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_multivalued_help},
      {"-reserved", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_reserved_help},
      {"-scope", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_scope_help},
      {"-trigbind", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_trigbind_help},
      {"-trigtype", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_trigtype_help},
      {"-unique", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_unique_help},
      {"-uuid", DCP_ARGV_STRING, NULL, NULL, 
           dcp_t_rgy_schema_uuid_help},
      {NULL, DCP_ARGV_END, NULL, NULL, 0}
   };

   /*
    * Check to see if the user just wants help.
    */
   DCP_CHECK_COMMAND_HELP();

   /*
    * Set the variables for the Parser. We need to cast them
    * all as char pointers. 
    */
   arg_table[0].dst = (char *)&acl_mgr_set;
   arg_table[1].dst = (char *)&comment;
   arg_table[2].dst = (char *)&apply_defaults;
   arg_table[3].dst = (char *)&attribute;
   arg_table[4].dst = (char *)&attr_encoding;
   arg_table[5].dst = (char *)&intercell_action;
   arg_table[6].dst = (char *)&multi_valued;
   arg_table[7].dst = (char *)&reserved;
   arg_table[8].dst = (char *)&scope;
   arg_table[9].dst = (char *)&trig_binding;
   arg_table[10].dst = (char *)&trig_types;
   arg_table[11].dst = (char *)&unique;
   arg_table[12].dst = (char *)&attr_id;

   if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR) {
      return TCL_ERROR;
   }

   /*
    * Verify that the user provided a schema name with the command. 
    */
   DCP_CHECK_RGY_NO_ARG(schema);
   DCP_CHECK_EXTRA_ARGS();
   DCP_GENERAL_SET_NAMELIST(argv[1]);

   /*
    * The user cannot specify an attribute list and a variety of switches.
    * (ie "schema create foo -unique yes -attribute {reserved yes}" is
    * illegal).
    */
   if ((attr_id != NULL || attr_encoding != NULL || acl_mgr_set != NULL ||
       unique != NULL || multi_valued != NULL || reserved != NULL ||
       apply_defaults != NULL || intercell_action != NULL || trig_types != NULL ||
       trig_binding != NULL || scope != NULL || comment != NULL) && (attribute != NULL)) {
      DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
   }

   /*
    * Initialize schema_entry.
    */
   memset(&schema_entry, 0, sizeof(schema_entry));

   /*
    * Parse the attributes specified via a command line option.
    */
   parse_schema_switch_list(interp, attr_id, attr_encoding, acl_mgr_set, unique, multi_valued,
                            reserved, apply_defaults, intercell_action, trig_types,
                            trig_binding, scope, comment, &schema_entry, &result);
   DCP_CHECK_SEC_ERROR(result);

   /*
    * Parse those attributes specified via the attribute switch.
    */
   parse_schema_attr_list(interp, attribute, &schema_entry, &result);
   DCP_CHECK_SEC_ERROR(result);

   /*
    * Check the schema data entered for missing values.  If certain
    * key fields have not been entered by the user, then this
    * is an error.
    */
   if (schema_entry.attr_encoding <= -1) {
      sec_attr_util_sch_ent_free_ptrs(&schema_entry);
      DCP_SET_MISC_ERROR(dcp_s_rgy_missing_encoding_type);
   }

   if (schema_entry.acl_mgr_set->num_acl_mgrs <= 0) {
      sec_attr_util_sch_ent_free_ptrs(&schema_entry);
      DCP_SET_MISC_ERROR(dcp_s_rgy_missing_acl_mgr_set);
   }

   /*
    * If the schema name is a list, instead of an element, split
    * the list into its component elements.
    */
   if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
      sec_attr_util_sch_ent_free_ptrs(&schema_entry);
      return TCL_ERROR;
   }
   else {
      /*
       * Check that we did not receive an empty argument list.
       */
      if ((cargc == 0) && (cargv[0] == NULL)) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         DCP_SET_MISC_ERROR(dcp_s_null_arg);
      }
   }

   /*
    * Apply the attributes to each name the user entered.
    */
   for (i = 0; i < cargc; i++) {
      strcpy((char *)global_name, cargv[i]);
      if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         return TCL_ERROR;
      }

      if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         return TCL_ERROR;
      }

      /*
       * After obtaining the registry context of the specified
       * argument, obtain the security group name for the cell.
       */
      if (dcp_schema_read_group_name(interp, &rs_ns_sec_group_name) != TCL_OK) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         return TCL_ERROR;
      }
      full_sec_group_name = (char *)malloc(sizeof(char) *
         (strlen((char *)rs_ns_sec_group_name) + sizeof(XATTRSCHEMA) + 2));
      if (full_sec_group_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      sprintf(full_sec_group_name,
              "%s/%s",
              (char *)rs_ns_sec_group_name,
              (char *)XATTRSCHEMA);
      rpc_string_free(&rs_ns_sec_group_name, &status);

      /*
       * The schema name entered should be a fully qualified schema name,
       * consisting of two parts, the schema name and the schema attribute
       * name.  Split the name into its two component parts.
       */
      schema_name = malloc(sizeof(char) * (strlen(cargv[i]) + 1));
      if (schema_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      schema_attr_name = malloc(sizeof(char) * (strlen(cargv[i]) + 1));
      if (schema_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      split_schema_name(cargv[i], schema_name, schema_attr_name, &status);
      if (BAD_STATUS(&status)) { 
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }

      full_schema_name = expand_cell_name(schema_name, &status);
      if (BAD_STATUS(&status)) { 
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }

      /*
       * Initialize the schema attribute name parameter correctly.
       */
      schema_entry.attr_name = malloc(sizeof(char) * (strlen(schema_attr_name) + 1));
      if (schema_entry.attr_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         free(full_schema_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }
      strcpy((char*)schema_entry.attr_name, (char *)schema_attr_name);

      /*
       * Which schema create call to use will depend
       * on the value of the full_schema_name variable.
       */
      if (strcmp(full_schema_name, full_sec_group_name) == 0) {
         free((char *)full_schema_name);
         free((char *)full_sec_group_name);
         /*
          * Use the default registry call.
          */
         sec_rgy_attr_sch_create_entry(rgy_context,
                                       (sec_attr_component_name_t)schema_name,
                                       &schema_entry,
                                       &status);
         if (BAD_STATUS(&status)) { 
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }
      }
      else {
         free((char *)full_schema_name);
         free((char *)full_sec_group_name);
         set_attr_schema_binding((dce_attr_component_name_t) schema_name, &status);
         if (BAD_STATUS(&status)) { 
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }
         dce_attr_sch_create_entry(attr_schema_binding_handle,
                                   (dce_attr_schema_entry_t *) &schema_entry,
                                   &status);
         if (BAD_STATUS(&status)) { 
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }
      }

      /*
       * Free the local variables.
       */
      free(schema_name);
      free(schema_attr_name);
   }

   /*
    * Free the internal pointers in the schema entry structure.
    */
   free((char *)cargv);
   sec_attr_util_sch_ent_free_ptrs(&schema_entry);

   return TCL_OK;
}


/*
 * FUNCTION: schema_delete
 *
 * OVERVIEW:
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
 */
int schema_delete(ClientData clientData,
                  Tcl_Interp *interp,
                  int argc,
                  char **argv)
{
   error_status_t           status;
   sec_rgy_name_t           global_name;
   sec_rgy_name_t           cell_name;
   sec_rgy_name_t           residual_name;
   char                     *schema_name = NULL;
   char                     *schema_attr_name = NULL;
   char                     *full_schema_name = NULL;
   char                     *full_sec_group_name = NULL;
   unsigned_char_p_t        rs_ns_sec_group_name = NULL;
   sec_attr_schema_entry_t  schema_entry;
   int                      i;
   int                      cargc = 0;
   char                     **cargv;

   /*
    * Check to see if the user just wants help.
    */
   DCP_NO_COMMAND_HELP();

   /*
    * Verify that the user provided a schema name with the command.
    */
   DCP_CHECK_RGY_NO_ARG(schema);
   DCP_CHECK_EXTRA_ARGS();
   DCP_GENERAL_SET_NAMELIST(argv[1]);

   /*
    * If the schema name is a list, instead of an element, split
    * the list into its component elements.
    */
   if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
      return TCL_ERROR;
   }
   else {
      /*
       * Check that we did not receive an empty argument list.
       */
      if ((cargc == 0) && (cargv[0] == NULL)) {
         DCP_SET_MISC_ERROR(dcp_s_null_arg);
      }
   }

   /*
    * Apply the attributes to each name the user entered.
    */
   for (i = 0; i < cargc; i++) {
      strcpy((char *)global_name, cargv[i]);
      if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK) {
         free((char *)cargv);
         return TCL_ERROR;
      }

      if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
         free((char *)cargv);
         return TCL_ERROR;
      }

      /*
       * After obtaining the registry context of the specified
       * argument, obtain the security group name for the cell.
       */
      if (dcp_schema_read_group_name(interp, &rs_ns_sec_group_name) != TCL_OK) {
         free((char *)cargv);
         return TCL_ERROR;
      }
      full_sec_group_name = (char *)malloc(sizeof(char) *
         (strlen((char *)rs_ns_sec_group_name) + sizeof(XATTRSCHEMA) + 2));
      if (full_sec_group_name == NULL) {
         free((char *)cargv);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }
      sprintf(full_sec_group_name,
              "%s/%s",
              (char *)rs_ns_sec_group_name,
              (char *)XATTRSCHEMA);
      rpc_string_free(&rs_ns_sec_group_name, &status);

      /*
       * The schema name entered should be a fully qualified schema name,
       * consisting of two parts, the schema name and the schema attribute
       * name.  Split the name into its two component parts.
       */
      schema_name = malloc(sizeof(char) * (strlen(cargv[i]) + 1));
      if (schema_name == NULL) {
         free((char *)cargv);
         free((char *)full_sec_group_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      schema_attr_name = malloc(sizeof(char) * (strlen(cargv[i]) + 1));
      if (schema_attr_name == NULL) {
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      split_schema_name(cargv[i], schema_name, schema_attr_name, &status);
      if (BAD_STATUS(&status)) { 
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }
      full_schema_name = expand_cell_name(schema_name, &status);
      if (BAD_STATUS(&status)) { 
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }

      /*
       * Which schema delete call to use will depend
       * on the value of the schema_name variable.
       */
      if (strcmp(full_schema_name, full_sec_group_name) == 0) {
         free((char *)full_schema_name);
         free((char *)full_sec_group_name);
         /*
          * Given the schema attribute name,
          * look up the schema attribute id.
          */
         sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                         (sec_attr_component_name_t)schema_name,
                                         (idl_char *)schema_attr_name,
                                         &schema_entry,
                                         &status);
         if (BAD_STATUS(&status)) { 
            free(schema_name);
            free(schema_attr_name);
            if (status == sec_attr_sch_entry_not_found) {
               DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)cargv[i]);
               free((char *)cargv);
               return TCL_ERROR;
            }
            else {
               free((char *)cargv);
               DCP_SET_MISC_ERROR(status);
            }
         }

         /*
          * Use the default registry call.
          */
         sec_rgy_attr_sch_delete_entry(rgy_context,
                                       (sec_attr_component_name_t)schema_name,
                                       &schema_entry.attr_id,
                                       &status);
         if (BAD_STATUS(&status)) { 
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            sec_attr_util_sch_ent_free_ptrs(&schema_entry); 
            DCP_SET_MISC_ERROR(status);
         }
      }
      else {
         free((char *)full_schema_name);
         free((char *)full_sec_group_name);
         set_attr_schema_binding((dce_attr_component_name_t)schema_name, &status);
         if (BAD_STATUS(&status)) { 
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }

         /*
          * Given the schema attribute name,
          * look up the schema attribute id.
          */
         dce_attr_sch_lookup_by_name(attr_schema_binding_handle,
                                     (idl_char *)schema_attr_name,
                                     (dce_attr_schema_entry_t *)&schema_entry,
                                     &status);
         if (BAD_STATUS(&status)) { 
            free(schema_name);
            free(schema_attr_name);
            if (status == db_s_key_not_found) {
               DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)cargv[i]);
               free((char *)cargv);
               return TCL_ERROR;
            }
            else {
               free((char *)cargv);
               DCP_SET_MISC_ERROR(status);
            }
         }
         dce_attr_sch_delete_entry(attr_schema_binding_handle,
                                   &(schema_entry.attr_id),
                                   &status);
         if (BAD_STATUS(&status)) { 
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            sec_attr_util_sch_ent_free_ptrs(&schema_entry); 
            DCP_SET_MISC_ERROR(status);
         }
      }

      /*
       * Free the local variables and the internal pointers
       * to the schema entry structure.
       */
      free(schema_name);
      free(schema_attr_name);
      sec_attr_util_sch_ent_free_ptrs(&schema_entry); 
   }

   free((char *)cargv);

   return TCL_OK;
}


/*
 * FUNCTION: schema_modify
 *
 * OVERVIEW: This function modifies a schema entry in a given schema.
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
 */
int schema_modify(ClientData clientData, 
                  Tcl_Interp *interp, 
                  int argc, 
                  char **argv)
{
   unsigned32                     result = TCL_OK;
   sec_rgy_name_t                 global_name;
   sec_rgy_name_t                 cell_name;
   sec_rgy_name_t                 residual_name;
   sec_attr_schema_entry_t        schema_entry;
   sec_attr_schema_entry_t        schema_entry_orig;
   sec_attr_schema_entry_parts_t  modify_parts = 0;
   error_status_t                 status;
   char                           *change = NULL;
   char                           *acl_mgr_set = NULL;
   char                           *reserved= NULL;
   char                           *apply_defaults = NULL;
   char                           *intercell_action = NULL;
   char                           *trig_binding = NULL;
   char                           *comment = NULL;
   char                           *schema_name = NULL;
   char                           *schema_attr_name = NULL;
   char                           *full_schema_name = NULL;
   char                           *full_sec_group_name = NULL;
   unsigned_char_p_t              rs_ns_sec_group_name = NULL;
   int                            i;
   int                            cargc = 0;
   char                           **cargv;

   dcp_ArgvInfo arg_table[] = {
      {"-aclmgr", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_aclmgr_help},
      {"-annotation", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_annotation_help},
      {"-applydefs", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_applydefs_help},
      {"-change", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_mod_change_help},
      {"-intercell", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_intercell_help},
      {"-reserved", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_reserved_help},
      {"-trigbind", DCP_ARGV_STRING, NULL, NULL,
           dcp_t_rgy_schema_trigbind_help},
      {NULL, DCP_ARGV_END, NULL, NULL, 0}
   };

   /*
    * Check to see if the user just wants help.
    */
   DCP_CHECK_COMMAND_HELP();

   /*
    * Set the variables for the Parser. We need to cast them
    * all as char pointers. 
    */
   arg_table[0].dst = (char *)&acl_mgr_set;
   arg_table[1].dst = (char *)&comment;
   arg_table[2].dst = (char *)&apply_defaults;
   arg_table[3].dst = (char *)&change;
   arg_table[4].dst = (char *)&intercell_action;
   arg_table[5].dst = (char *)&reserved;
   arg_table[6].dst = (char *)&trig_binding;

   if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR) {
      return TCL_ERROR;
   }

   /*
    * Verify that the user provided a schema name with the command. 
    */
   DCP_CHECK_RGY_NO_ARG(schema);
   DCP_CHECK_EXTRA_ARGS();
   DCP_GENERAL_SET_NAMELIST(argv[1]);

   /*
    * The user cannot specify an attribute list and a variety of switches.
    * (ie "schema create foo -unique yes -change {reserved yes}" is
    * illegal).
    */
   if ((acl_mgr_set != NULL || reserved != NULL || apply_defaults != NULL || 
        intercell_action != NULL || trig_binding != NULL || comment != NULL) && (change != NULL)) {
      DCP_SET_MISC_ERROR(dcp_s_rgy_list_and_switches);
   }

   /*
    * Initialize schema_entry to all nulls.  All fields except the ones to be modified
    * must be set to null before schema_entry is passed to the API.
    */
   memset(&schema_entry, 0, sizeof(schema_entry));

   /*
    * Parse the attributes specified via a command line option.
    */
   parse_modify_schema_switch_list(interp, acl_mgr_set, reserved, apply_defaults, intercell_action, 
                                   trig_binding, comment, &schema_entry, &modify_parts, &result);
   DCP_CHECK_SEC_ERROR(result);

   if (change != NULL) {
      /*
       * Parse those attributes specified via the attribute switch.
       */
      parse_modify_schema_attr_list(interp, change, &schema_entry, &modify_parts, &result);
      DCP_CHECK_SEC_ERROR(result);
   }

   /*
    * If the schema name is a list, instead of an element, split
    * the list into its component elements.
    */
   if (Tcl_SplitList(interp, argv[1], &cargc, &cargv) != TCL_OK) {
      sec_attr_util_sch_ent_free_ptrs(&schema_entry);
      return TCL_ERROR;
   }
   else {
      /*
       * Check that we did not receive an empty argument list.
       */
      if ((cargc == 0) && (cargv[0] == NULL)) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         DCP_SET_MISC_ERROR(dcp_s_null_arg);
      }
   }

   /*
    * Apply the attributes to each name the user entered.
    */
   for (i = 0; i < cargc; i++) {
      strcpy((char *)global_name, cargv[i]);
      if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         return TCL_ERROR;
      }

      if (open_rgy_site(interp, cell_name, site_update) != TCL_OK) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         return TCL_ERROR;
      }

      /*
       * After obtaining the registry context of the specified
       * argument, obtain the security group name for the cell.
       */
      if (dcp_schema_read_group_name(interp, &rs_ns_sec_group_name) != TCL_OK) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         return TCL_ERROR;
      }
      full_sec_group_name = (char *)malloc(sizeof(char) *
         (strlen((char *)rs_ns_sec_group_name) + sizeof(XATTRSCHEMA) + 2));
      if (full_sec_group_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }
      sprintf(full_sec_group_name,
              "%s/%s",
              (char *)rs_ns_sec_group_name,
              (char *)XATTRSCHEMA);
      rpc_string_free(&rs_ns_sec_group_name, &status);

      /*
       * The schema name entered should be a fully qualified schema name,
       * consisting of two parts, the schema name and the schema attribute
       * name.  Split the name into its two component parts.
       */
      schema_name = malloc(sizeof(char) * (strlen(cargv[i]) + 1));
      if (schema_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      schema_attr_name = malloc(sizeof(char) * (strlen(cargv[i]) + 1));
      if (schema_attr_name == NULL) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         DCP_SET_MISC_ERROR(dcp_s_no_memory);
      }

      split_schema_name(cargv[i], schema_name, schema_attr_name, &status);
      if (BAD_STATUS(&status)) { 
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }
      full_schema_name = expand_cell_name(schema_name, &status);
      if (BAD_STATUS(&status)) { 
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free((char *)cargv);
         free((char *)full_sec_group_name);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }

      /*
       * Since the security api just replaces the specified aclmgr set with the 
       * one specified and since acl managers may only be appended, append the
       * specified aclmgr to the ones already in the schema entry.
       */
      if (modify_parts & sec_attr_schema_part_acl_mgrs)  {
         if ((build_proper_aclmgr_set(interp, full_schema_name, schema_attr_name, full_sec_group_name, &schema_entry)) != TCL_OK) {
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free((char *)cargv);
            free((char *)full_sec_group_name);
            free(schema_name);
            free(schema_attr_name);
            free((char *)full_schema_name);
            return TCL_ERROR;
         }
      }

      /*
       * Which schema create call to use will depend
       * on the value of the schema_name variable.
       */
      if (strcmp(full_schema_name, full_sec_group_name) == 0) {
         free((char *)full_schema_name);
         free((char *)full_sec_group_name);
         /*
          * Use the default registry call.
          */
         sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                         (sec_attr_component_name_t)schema_name,
                                         (idl_char *)schema_attr_name,
                                         &schema_entry_orig,
                                         &status);
         if (BAD_STATUS(&status)) { 
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free(schema_name);
            free(schema_attr_name);
            if (status == sec_attr_sch_entry_not_found) {
               DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)cargv[i]);
               free((char *)cargv);
               return TCL_ERROR;
            }
            else {
               free((char *)cargv);
               DCP_SET_MISC_ERROR(status);
            }
         }
         schema_entry.attr_id = schema_entry_orig.attr_id;
         sec_rgy_attr_sch_update_entry(rgy_context,
                                       (sec_attr_component_name_t)schema_name,
                                       modify_parts,
                                       &schema_entry,
                                       &status);
         if (BAD_STATUS(&status)) { 
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }
      }
      else {
         free((char *)full_schema_name);
         free((char *)full_sec_group_name);
         set_attr_schema_binding((dce_attr_component_name_t)schema_name, &status);
         if (BAD_STATUS(&status)) { 
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }

         /*
          * Given the schema attribute name,
          * look up the schema attribute id.
          */
         dce_attr_sch_lookup_by_name(attr_schema_binding_handle,
                                     (idl_char *)schema_attr_name,
                                     (dce_attr_schema_entry_t *)&schema_entry_orig,
                                     &status);
         if (BAD_STATUS(&status)) {
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            free(schema_name);
            free(schema_attr_name);
            if (status == db_s_key_not_found) {
               DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)cargv[i]);
               free((char *)cargv);
               return TCL_ERROR;
            }
            else {
               free((char *)cargv);
               DCP_SET_MISC_ERROR(status);
            }
         }
         schema_entry.attr_id = schema_entry_orig.attr_id;
         dce_attr_sch_update_entry(attr_schema_binding_handle,
                                   (dce_attr_schema_entry_parts_t)modify_parts,
                                   (dce_attr_schema_entry_t *)&schema_entry,
                                   &status);
         if (BAD_STATUS(&status)) {
            sec_attr_util_sch_ent_free_ptrs(&schema_entry);
            sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);
            free((char *)cargv);
            free(schema_name);
            free(schema_attr_name);
            DCP_SET_MISC_ERROR(status);
         }
      }

      /*
       * Free the local variables.
       */
      free(schema_name);
      free(schema_attr_name);
      sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);
   }

   /*
    * Free the internal pointers in the schema entry structure.
    */
   free((char *)cargv);
   sec_attr_util_sch_ent_free_ptrs(&schema_entry);

   return TCL_OK;
}


/*
 * FUNCTION: schema_rename
 *
 * OVERVIEW: This function renames a schema entry in a given schema.
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
 */
int schema_rename(ClientData clientData, 
                  Tcl_Interp *interp, 
                  int argc, 
                  char **argv)
{
   unsigned32                     result = TCL_OK;
   sec_rgy_name_t                 global_name;
   sec_rgy_name_t                 cell_name;
   sec_rgy_name_t                 residual_name;
   sec_attr_schema_entry_t        schema_entry;
   sec_attr_schema_entry_t        schema_entry_orig;
   sec_attr_schema_entry_parts_t  modify_parts = 0;
   error_status_t                 status;
   char                           *to = NULL;
   char                           *schema_name = NULL;
   char                           *schema_attr_name = NULL;
   char                           *full_schema_name = NULL;
   char                           *full_sec_group_name = NULL;
   unsigned_char_p_t              rs_ns_sec_group_name = NULL;

   dcp_ArgvInfo arg_table[] = {
      {"-to", DCP_ARGV_STRING, NULL, NULL,
          dcp_t_rgy_schema_rename_to_help},
      {NULL, DCP_ARGV_END, NULL, NULL, 0}
   };

   /*
    * Check to see if the user just wants help.
    */
   DCP_CHECK_COMMAND_HELP();

   /*
    * Set the variables for the Parser. We need to cast them
    * all as char pointers. 
    */
   arg_table[0].dst = (char *)&to;

   if ((result = dcp_ParseArgv(interp, &argc, argv, arg_table, 0)) == TCL_ERROR) {
      return TCL_ERROR;
   }

   /*
    * Verify that the user provided a schema name with the command. 
    */
   DCP_CHECK_RGY_NO_ARG(schema);
   DCP_CHECK_EXTRA_ARGS();
   DCP_GENERAL_SET_NAMELIST(argv[1]);

   if (to == NULL) {
      DCP_SET_MISC_ERROR(dcp_s_rgy_schema_rename_no_newname);
   }

   strcpy((char *)global_name, argv[1]);
   if (dcp_id_parse_name(interp, global_name, cell_name, residual_name) != TCL_OK)
      return TCL_ERROR;

   if (open_rgy_site(interp, cell_name, site_update) != TCL_OK)
      return TCL_ERROR;

   /*
    * After obtaining the registry context of the specified
    * argument, obtain the security group name for the cell.
    */
   if (dcp_schema_read_group_name(interp, &rs_ns_sec_group_name) != TCL_OK) {
      return TCL_ERROR;
   }

   full_sec_group_name = (char *)malloc(sizeof(char) *
      (strlen((char *)rs_ns_sec_group_name) + sizeof(XATTRSCHEMA) + 2));
   if (full_sec_group_name == NULL) {
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }
   sprintf(full_sec_group_name,
           "%s/%s",
           (char *)rs_ns_sec_group_name,
           (char *)XATTRSCHEMA);
   rpc_string_free(&rs_ns_sec_group_name, &status);

   schema_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
   if (schema_name == NULL) {
      free((char *)full_sec_group_name);
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }

   schema_attr_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
   if (schema_attr_name == NULL) {
      free((char *)full_sec_group_name);
      free(schema_name);
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }

   split_schema_name(argv[1], schema_name, schema_attr_name, &status);
   if (BAD_STATUS(&status)) { 
      free((char *)full_sec_group_name);
      free(schema_name);
      free(schema_attr_name);
      DCP_SET_MISC_ERROR(status);
   }
   full_schema_name = expand_cell_name(schema_name, &status);
   if (BAD_STATUS(&status)) { 
      free((char *)full_sec_group_name);
      free(schema_name);
      free(schema_attr_name);
      DCP_SET_MISC_ERROR(status);
   }

   /*
    * Initialize schema_entry to all nulls. All fields except the ones to be modified
    * must be set to null before schema_entry is passed to the API.
    */
   memset(&schema_entry, 0, sizeof(schema_entry));

   /*
    * Initialize the schema attribute name parameter correctly.
    */
   schema_entry.attr_name = malloc((sizeof(char) * (strlen(to)) + 1));
   if (schema_entry.attr_name == NULL) {
      free((char *)full_sec_group_name);
      free(schema_name);
      free(schema_attr_name);
      free((char *)full_schema_name);
      DCP_SET_MISC_ERROR(dcp_s_no_memory);
   }
   strcpy((char*)schema_entry.attr_name, (char *)to);
   modify_parts = modify_parts | sec_attr_schema_part_name;

   /*
    * Which schema create call to use will depend
    * on the value of the schema_name variable.
    */
   if (strcmp(full_schema_name, full_sec_group_name) == 0) {
      free((char *)full_sec_group_name);
      free((char *)full_schema_name);
      /*
       * Use the default registry call.
       */
      sec_rgy_attr_sch_lookup_by_name(rgy_context,
                                      (sec_attr_component_name_t)schema_name,
                                      (idl_char *)schema_attr_name,
                                      &schema_entry_orig,
                                      &status);
      if (BAD_STATUS(&status)) { 
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free(schema_name);
         free(schema_attr_name);
         if (status == sec_attr_sch_entry_not_found) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)argv[1]);
            return TCL_ERROR;
         }
         else {
            DCP_SET_MISC_ERROR(status);
         }
      }
      schema_entry.attr_id = schema_entry_orig.attr_id;
      sec_rgy_attr_sch_update_entry(rgy_context,
                                    (sec_attr_component_name_t)schema_name,
                                    modify_parts,
                                    &schema_entry,
                                    &status);
      if (BAD_STATUS(&status)) { 
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }
   }
   else {
      free((char *)full_schema_name);
      free((char *)full_sec_group_name);
      set_attr_schema_binding((dce_attr_component_name_t)schema_name, &status);
      if (BAD_STATUS(&status)) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }

      /*
       * Given the schema attribute name,
       * look up the schema attribute id.
       */
      dce_attr_sch_lookup_by_name(attr_schema_binding_handle,
                                  (idl_char *)schema_attr_name,
                                  (dce_attr_schema_entry_t *)&schema_entry_orig,
                                  &status);
      if (BAD_STATUS(&status)) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         free(schema_name);
         free(schema_attr_name);
         if (status == db_s_key_not_found) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sch_entry_not_found, (char *)argv[1]);
            return TCL_ERROR;
         }
         else {
            DCP_SET_MISC_ERROR(status);
         }
      }
      schema_entry.attr_id = schema_entry_orig.attr_id;
      dce_attr_sch_update_entry(attr_schema_binding_handle,
                                (dce_attr_schema_entry_parts_t)modify_parts,
                                (dce_attr_schema_entry_t *)&schema_entry,
                                &status);
      if (BAD_STATUS(&status)) {
         sec_attr_util_sch_ent_free_ptrs(&schema_entry);
         sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);
         free(schema_name);
         free(schema_attr_name);
         DCP_SET_MISC_ERROR(status);
      }
   }

   /*
    * Free the local variables.
    */
   free(schema_name);
   free(schema_attr_name);
   sec_attr_util_sch_ent_free_ptrs(&schema_entry_orig);

   /*
    * Free the internal pointers in the schema entry structure.
    */
   sec_attr_util_sch_ent_free_ptrs(&schema_entry);

   return TCL_OK;
}


/*
 * FUNCTION: set_attr_schema_binding
 *
 * OVERVIEW: This function sets an attribute schema binding.
 *
 * INPUTS:
 *  dce_attr_component_name_t schema_name - The name of the schema.
 *
 * INPUT/OUTPUTS:
 *
 * OUTPUTS:
 *  error_status_t status - Holds the result.
 *
 * RETURNS:
 *
 */
void set_attr_schema_binding(dce_attr_component_name_t schema_name,
                             error_status_t *status)
{
   dce_attr_bind_auth_info_t  auth_info;

   if ((attr_schema_binding_handle) &&
       (strcmp((char *)schema_name, (char *)attr_schema_name) == 0)) {
      *status = error_status_ok;
      return;
   }

   memset((char *)&auth_info, 0, sizeof(auth_info));

   if (dcp_is_logged_in()) {
      auth_info.info_type = sec_attr_bind_auth_dce;
      auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level;
      auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_svc;
      auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_svc;
   } else {
      auth_info.info_type = sec_attr_bind_auth_none;
      auth_info.tagged_union.dce_info.protect_level = dhd_c_prot_level_none;
      auth_info.tagged_union.dce_info.authn_svc = dhd_c_authn_none;
      auth_info.tagged_union.dce_info.authz_svc = dhd_c_authz_none;
   }

   /* Free the old binding handle before getting a new one */
   if (attr_schema_binding_handle != NULL) {
       dce_attr_sch_bind_free(&attr_schema_binding_handle, status);
   }

   dce_attr_sch_bind(schema_name, &auth_info, 
                     &attr_schema_binding_handle, status);
   if (*status == error_status_ok) {
      if (attr_schema_name != NULL)
         free(attr_schema_name);
      attr_schema_name = malloc(sizeof(char) * (strlen((char *)schema_name) + 1));
      strcpy((char *)attr_schema_name, (char *)schema_name);
   }
}
