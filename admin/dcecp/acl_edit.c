/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_edit.c,v $
 * Revision 1.1.9.1  1996/08/09  11:44:38  arvind
 * 	Merge changes.
 * 	[1996/06/10  14:12 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge changes for OT12902.
 * 	[1996/02/09  14:59 UTC  truitt  /main/HPDCE02/7]
 *
 * 	Merge changes.
 * 	[1996/02/09  14:57 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	OT12902: Mask object re-calculation is broken whenever a new entry or
 * 	multiple entries are added.  The mask object re-calculation is also broken
 * 	whenever multiple existing entries are modified.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  13:49 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.7.2  1996/02/18  19:11:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:11  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  21:19:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/09/29  20:46 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:47 UTC  tatsu_s
 * 	Removed the debug fprintf().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/21  15:30 UTC  tatsu_s
 * 	Added -local option.
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/18  19:42 UTC  jrr
 * 	Output well formed lists for 'acl pe' and 'acl sh -managers'
 * 	[1995/07/18  19:41 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/05  18:40 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/05  18:38 UTC  truitt
 * 	CHFts15396: Clear the num_acls sub-field of the dcp_acl_info
 * 	structure when the sec_acl_lookup() call fails.  This will
 * 	prevent the cleanup function from passing bad data to the
 * 	sec_acl_release() call.
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/23  19:02 UTC  truitt
 * 	Merge unauthentication work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  19:01 UTC  truitt
 * 	CHFts15282: If the call to sec_acl_bind() returns a
 * 	valid handle but an 'unauthenticated' status, go ahead
 * 	and continue with the command instead of bailing out.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/30  14:54 UTC  nav
 * 	merge from private branch
 * 
 * 	HP revision /main/HPDCE02/nav_mothra_2/1  1995/04/28  17:10 UTC  nav
 * 	fix the acl check core dump for mothra-to-KK acl_checks
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/14  14:21 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chfts19/1  1995/03/14  14:19 UTC  truitt
 * 	CHFts14241: Add the sec_acl_release() call to the
 * 	dcp_acl_free_acl_mgr_table() function to complete
 * 	the cleanup of the acl managers.
 * 	[1995/12/08  17:53:51  root]
 * 
 * Revision 1.1.2.27  1994/10/06  21:52:40  salamone
 * 	CR12549 - Adding or changing 1 ACL entry without a key in TCL syntax
 * 	          does not work.
 * 	[1994/10/06  20:55:54  salamone]
 * 
 * Revision 1.1.2.26  1994/09/30  20:37:56  salamone
 * 	CR12425 - After freeing ACL mgr tbl, set ptr to NULL.
 * 	[1994/09/30  20:32:55  salamone]
 * 
 * Revision 1.1.2.25  1994/09/09  21:54:26  salamone
 * 	CR10167 - Ignore invalid default cell UUID.
 * 	[1994/09/09  21:53:47  salamone]
 * 
 * Revision 1.1.2.24  1994/09/08  14:00:36  salamone
 * 	CR11968-Setting authentication on binding handles.
 * 	[1994/09/08  13:59:08  salamone]
 * 
 * Revision 1.1.2.23  1994/08/25  21:18:29  salamone
 * 	CR10243 - acl ops on "obj" objects produce misleading message
 * 	[1994/08/25  21:17:47  salamone]
 * 
 * Revision 1.1.2.22  1994/08/25  15:32:56  salamone
 * 	Merged with changes from 1.1.2.21
 * 	[1994/08/25  15:32:43  salamone]
 * 
 * 	CR11635 - Handle 1 string binding specification for an ACL object.
 * 	[1994/08/25  15:28:25  salamone]
 * 
 * Revision 1.1.2.21  1994/08/21  19:45:19  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:31  melman]
 * 
 * Revision 1.1.2.20  1994/08/09  21:45:40  salamone
 * 	CR10217 - On input, ignore "effective perms" string in ACL entry.
 * 	[1994/08/09  21:43:55  salamone]
 * 
 * Revision 1.1.2.19  1994/08/08  18:51:34  melman
 * 	Merged with changes from 1.1.2.18
 * 	[1994/08/08  18:51:02  melman]
 * 
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:01  melman]
 * 
 * Revision 1.1.2.18  1994/08/08  18:20:48  salamone
 * 	CR10208 - Do not allow replacing an ACL with duplicate entries.
 * 	[1994/08/08  18:09:30  salamone]
 * 
 * Revision 1.1.2.17  1994/08/05  19:37:18  salamone
 * 	CR11309 - Allow abbreviations for ACL manager names.
 * 	[1994/08/05  19:35:33  salamone]
 * 
 * Revision 1.1.2.16  1994/07/29  18:44:30  salamone
 * 	CR11205 - Maintain mask_calc_type for each ACL mgr.
 * 	[1994/07/29  18:27:50  salamone]
 * 
 * Revision 1.1.2.15  1994/07/26  18:58:32  salamone
 * 	CR10045 - Provide string binding support for ACL objects.
 * 	[1994/07/26  18:48:20  salamone]
 * 
 * Revision 1.1.2.14  1994/07/07  14:35:36  salamone
 * 	Provide support for more than one ACL manager.
 * 	[1994/07/07  14:31:09  salamone]
 * 
 * Revision 1.1.2.13  1994/05/18  20:39:01  salamone
 * 	CR10621 - "acl show", "acl check", and "acl permissions" should
 * 		   display the appropriate info if unauthenticated.
 * 	[1994/05/13  20:30:16  salamone]
 * 
 * Revision 1.1.2.12  1994/05/06  17:35:54  salamone
 * 	CR10211: -mask calc is ignored.
 * 	[1994/05/06  17:32:46  salamone]
 * 
 * Revision 1.1.2.11  1994/05/05  18:33:43  salamone
 * 	CR10207-Replacing all ACL entries with 1 ACL entry causes
 * 		a core dump.
 * 	[1994/05/05  18:32:55  salamone]
 * 
 * Revision 1.1.2.10  1994/05/05  14:09:43  salamone
 * 	Free argv properly.
 * 	[1994/05/05  14:09:02  salamone]
 * 
 * Revision 1.1.2.9  1994/04/27  18:56:00  salamone
 * 	Fix compile warnings - Statement not reached.
 * 	[1994/04/27  18:55:40  salamone]
 * 
 * Revision 1.1.2.8  1994/04/07  10:22:02  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:42  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component dependency build issues
 * 	[1994/04/07  10:10:20  kevins]
 * 
 * Revision 1.1.2.7  1994/03/25  22:29:44  salamone
 * 	CR10206 - Provide support for 1 ACL entry specified in TCL syntax.
 * 	[1994/03/25  21:56:18  salamone]
 * 
 * Revision 1.1.2.6  1994/03/02  20:39:34  salamone
 * 	CR10044: mask_obj entry cannot be added to ACL object that supports it.
 * 	[1994/03/02  20:34:33  salamone]
 * 
 * Revision 1.1.2.5  1994/02/09  15:59:47  salamone
 * 	Added "acl replace" and "acl delete" support.
 * 	Added delegation support.
 * 	[1994/02/09  15:35:23  salamone]
 * 
 * Revision 1.1.2.4  1994/01/24  22:24:55  salamone
 * 	Added support for "acl modify"
 * 	[1994/01/24  22:16:04  salamone]
 * 
 * Revision 1.1.2.3  1994/01/05  19:44:39  salamone
 * 	Merged with changes from 1.1.2.2
 * 	[1994/01/05  19:44:34  salamone]
 * 
 * 	Added "acl show" support
 * 	[1994/01/05  19:41:18  salamone]
 * 
 * Revision 1.1.2.2  1993/12/30  19:58:45  rousseau
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:57:59  rousseau]
 * 
 * Revision 1.1.2.1  1993/12/29  16:55:54  salamone
 * 	Initial version of acl_edit.c
 * 	[1993/12/29  16:54:05  salamone]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * acl_edit.c
 *
 * DESCRIPTION:
 * This module contains the functions to edit or list an object's
 * ACLs.
 */

/*
 * INCLUDE FILES
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dce/macros.h>

/*
 * IDL FILES
 */
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
#ifndef	HPDCE_DCED_LOCAL
#include <dce/aclif.h>
#endif	/* HPDCE_DCED_LOCAL */

#include <dce/tcl.h>
#include <dcecp.h>
#include <acl_edit.h>
#include <util_binding.h>
#include <util_general.h>
#include <util_login.h>

/*
 * INTERNAL VARIABLES 
 */

/* 
 * Table of sorted ACL entry types to match ascii strings with 
 * equivalent types. The ACL entry display order is as follows:
 *
 * Note: Last entry is NULL, to end the loop that checks a given
 * string against all known types for a match.
 */
static dcp_acl_entry_type_table_t valid_entry_types[] = {
    { "mask_obj",       sec_acl_e_type_mask_obj         },
    { "unauthenticated",sec_acl_e_type_unauthenticated  },
    { "user_obj",       sec_acl_e_type_user_obj         },
    { "user",           sec_acl_e_type_user             },
    { "foreign_user",   sec_acl_e_type_foreign_user     },
    { "group_obj",      sec_acl_e_type_group_obj        },
    { "group",          sec_acl_e_type_group            },
    { "foreign_group",  sec_acl_e_type_foreign_group    },
    { "other_obj",      sec_acl_e_type_other_obj        },
    { "foreign_other",  sec_acl_e_type_foreign_other    },
    { "any_other",      sec_acl_e_type_any_other        },
    { "user_obj_delegate",      sec_acl_e_type_user_obj_deleg  },
    { "user_delegate",          sec_acl_e_type_user_deleg      },
    { "foreign_user_delegate",  sec_acl_e_type_for_user_deleg  },
    { "group_obj_delegate",     sec_acl_e_type_group_obj_deleg },
    { "group_delegate",         sec_acl_e_type_group_deleg     },
    { "foreign_group_delegate", sec_acl_e_type_for_group_deleg },
    { "other_obj_delegate",     sec_acl_e_type_other_obj_deleg },
    { "foreign_other_delegate", sec_acl_e_type_for_other_deleg },
    { "any_other_delegate",     sec_acl_e_type_any_other_deleg },
    { "extended",       sec_acl_e_type_extended         },
    { NULL,             sec_acl_e_type_extended         }
};


/*
 * PROTOTYPES 
 */
error_status_t dcp__acl_bind_registry(Tcl_Interp *, char *, sec_rgy_handle_t *);
int dcp__acl_get_all_managers(unsigned32, Tcl_Interp *, dcp_acl_info_p_t);
boolean32 dcp__acl_verify_manager_specified(dcp_acl_info_p_t);
int dcp__acl_bind_cell_rgys(Tcl_Interp *, dcp_acl_info_p_t);
void dcp__acl_print_permset(Tcl_Interp *, dcp_acl_info_p_t, sec_acl_permset_t);
void dcp__acl_print_effective_permset(Tcl_Interp *, dcp_acl_info_p_t, sec_acl_permset_t, sec_acl_permset_t);
void dcp__acl_print_entries(Tcl_Interp *, dcp_acl_info_p_t, sec_acl_entry_t *);
error_status_t dcp__acl_sort_entries(int, sec_acl_entry_t **);
char *dcp__acl_generate_extended_key(Tcl_Interp *, sec_acl_extend_info_t *);
error_status_t dcp__acl_validate_extended_info(char *, sec_acl_entry_t *);
char *dcp__acl_generate_key_name(dcp_acl_info_p_t, sec_acl_entry_t *);
error_status_t dcp__acl_generate_key_uuid(dcp_acl_info_p_t, char *, sec_acl_entry_t *);
error_status_t dcp__acl_validate_entry(dcp_acl_info_p_t, char *, boolean32, sec_acl_entry_t *);
error_status_t dcp__acl_do_remove_entries(sec_acl_entry_t *, dcp_acl_info_p_t);
error_status_t dcp__acl_do_change_entries(sec_acl_entry_t *, dcp_e_acl_mod_edit_type_t, dcp_acl_info_p_t, sec_acl_permset_t *);
error_status_t dcp__acl_do_add_entries(sec_acl_entry_t *, dcp_e_acl_mod_edit_type_t, dcp_acl_info_p_t, sec_acl_permset_t *);
boolean32 dcp__acl_entry_compare(sec_acl_entry_t *, sec_acl_entry_t *, boolean32);
void dcp__acl_calc_union_permissions(dcp_acl_info_p_t, boolean32, sec_acl_permset_t *);
error_status_t dcp__acl_do_purge_ineffective(dcp_acl_info_p_t);
void dcp__acl_make_lowercase(char *);  /* XXX - put in util_general.c */
void dcp__acl_free_specified_entries(char **, sec_acl_entry_t *, char **, sec_acl_entry_t *, char **, sec_acl_entry_t *);  
void dcp__acl_free_existing_entry_keys(dcp_acl_info_p_t);  
boolean32 dcp__acl_is_2_key_entry(sec_acl_entry_t *);
boolean32 dcp__acl_is_file_group_class(sec_acl_entry_t *);
int dcp__acl_list_entries(Tcl_Interp *, dcp_acl_info_p_t, boolean32, char *, int *, char ***);


/*
 * FUNCTION:
 * dcp_acl_bind_named_object()
 *
 * OVERVIEW:
 * This routine binds to the ACL handle and security database(s). In
 * addition, ACL information required to perform one ACL operation
 * are obtained.
 *
 * INPUTS:
 * bind_to_entry:
 *	    Bind indicator, for use when entry_name identifies both an
 *	    entry in the global namespace and an actual object. A TRUE 
 *	    value binds the handle to the entry in the namespace, while 
 * 	    FALSE binds the handle to the actual object. 
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 * 	     sec_acl_named_object: 
 *              The name of the target object. Subsequent ACL operations
 *              using the returned handle will affect the ACL of this object. 
 *           UPDATED:
 * 	     sec_acl_handle:
 *               A pointer to the sec_acl_handle_t variable to receive the
 *               returned ACL handle.  The other sec_acl routines use this 
 *               handle to refer to the ACL for the object specified with 
 *               named_object.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_bind_named_object(Tcl_Interp *interp,
			  dcp_acl_info_p_t dcp_acl_info_p,
			  boolean32 bind_to_entry)
{
    error_status_t  	    	status;
    int 			rc = TCL_OK;
    unsigned32                  try_nmgrs = 2;   /* Try 2 first */
    int                         name_argc;
    char                        **name_argv;
    char                        *sbind;
    char                        *temp_sbind;
    char                        *residual_name = (char *)NULL;
    rpc_binding_handle_t        server_handle;

    CLEAR_STATUS(&status);

    /*
     * Determine whether the specified ACL object is a 
     * name or string binding.
     */
    if (Tcl_SplitList(interp, dcp_acl_info_p->sec_acl_named_object,
			  &name_argc, &name_argv) != TCL_OK){
        return(TCL_ERROR);
    }
    if (name_argc > 1) {
	if (bind_to_entry) {
            free((char *) name_argv);
	    DCP_SET_MISC_ERROR(dcp_s_sbind_entry_option); 
	}
	name_argc--;   /* remove residual from string binding */
	residual_name = dce_strdup(name_argv[name_argc]);
	temp_sbind = dce_strdup(name_argv[0]);
	sbind = dcp_binding_tcl_to_string(name_argc, name_argv, &status);
	if (sbind != NULL) {
	    rpc_binding_from_string_binding((unsigned_char_p_t)sbind,
					    &server_handle,
					    &status);

	    if (GOOD_STATUS(&status)) {
	        temp_sbind = dce_strdup(sbind);
	    } /* End of only 1 binding specified in TCL syntax */
	    rpc_string_free((unsigned char **)&sbind, &status);
	} 
    } /* End of process string binding */

    free((char *) name_argv);

    /*
     * Then, bind to the specified object's ACL.
     */
    CLEAR_STATUS(&status);
    if (residual_name == NULL) {
#ifndef	HPDCE_DCED_LOCAL
	if (dcp_acl_info_p->is_local) {
	    sec_acl_bind_local(1, &dce_acl_v1_0_epv,
			       (unsigned char *)dcp_acl_info_p->sec_acl_named_object, 
			       &dcp_acl_info_p->sec_acl_handle,
			       &status);
	}
	else
#endif	/* HPDCE_DCED_LOCAL */
        sec_acl_bind((unsigned char *)dcp_acl_info_p->sec_acl_named_object, 
    		      bind_to_entry,
		      &dcp_acl_info_p->sec_acl_handle, 
		      &status);
    } else {
#ifndef	HPDCE_DCED_LOCAL
	if (dcp_acl_info_p->is_local) {
	    sec_acl_bind_local(1, &dce_acl_v1_0_epv,
			       (unsigned char *)residual_name,
			       &dcp_acl_info_p->sec_acl_handle, 
			       &status);
	}
	else
#endif	/* HPDCE_DCED_LOCAL */
        sec_acl_bind_to_addr((unsigned char *)temp_sbind,
    		             (unsigned char *)residual_name,
		             &dcp_acl_info_p->sec_acl_handle, 
		             &status);
    }
    if (STATUS_EQUAL(&status, sec_acl_unable_to_authenticate)) {
        CLEAR_STATUS(&status);
    }
    else if (STATUS_EQUAL(&status, rpc_s_not_rpc_entry)) {
        DCP_SET_MISC_ERROR(dcp_s_acl_not_rpc_entry);
    }
    else if (STATUS_EQUAL(&status, sec_s_none_registered)) {
        DCP_SET_MISC_ERROR(dcp_s_acl_could_not_bind);
    }
    else if (!(GOOD_STATUS(&status))) {
        DCP_SET_MISC_ERROR(status);
    }

    /*
     * Obtain all ACL information for each ACL manager
     * supported by the object.
     * If an ACL manager name was specified, validate it.
     */
    rc = dcp__acl_get_all_managers(try_nmgrs, 
				   interp, 
				   dcp_acl_info_p);
    if ((rc == TCL_OK) && (dcp_acl_info_p->num_acl_mgrs > try_nmgrs)) {
        rc = dcp__acl_get_all_managers(dcp_acl_info_p->num_acl_mgrs, 
				       interp, 
				       dcp_acl_info_p);
    }

    if (rc == TCL_OK) {
        if (dcp_acl_info_p->mgr_type_specified[0] != '\0') {
            if (! dcp__acl_verify_manager_specified(dcp_acl_info_p)) {
	         DCP_SET_MISC_ERROR(dcp_s_acl_mgr_not_found); 
	    }
        }
    }

    /* 
     * An ACL has 1 attribute associated with it: the cell.
     *
     * Bind to the local and default cell security databases. 
     * They contain security related info about all entities in 
     * their respective Registry.
     */
    if (rc == TCL_OK) {
        rc = dcp__acl_bind_cell_rgys(interp, dcp_acl_info_p);
    }

    return(rc);

} /* End of dcp_acl_bind_named_object() */


/*
 * FUNCTION:
 * dcp__acl_get_all_managers()
 *
 * OVERVIEW:
 * This routine will obtain all ACL information for each ACL manager
 * supported by the object.
 *
 * INPUTS:
 * tmp_nmgrs:
 *          Number of ACL managers to allocate space for and use
 *          to determine the true number of ACL managers after the
 *          Security API call.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 * 	     sec_acl_handle:
 *               A pointer to the sec_acl_handle_t variable to receive the
 *               returned ACL handle.  The other sec_acl routines use this 
 *               handle to refer to the ACL for the object specified with 
 *               named_object.
 *           sec_acl_type:
 *               ACL type; the object ACL itself, initial container
 *                         object ACL, or initial object ACL.
 *           UPDATED:
 *           sec_acl_mgr_type:
 *               The UUIDs identifying each ACL manager protecting 
 *               an object
 *           mask_calc_type:
 *               Mask calculation type
 *           num_acl_mgrs:
 *               Number of ACL managers for the specified object.
 *           manager_info.printstring:
 *               Each ACL manager name on an object.
 *           manager_info.helpstring:
 *               Help text describing each ACL manager.
 *           sec_acl_printstrings:
 *               An array of permission printstrings each ACL manager
 *               maintains on an object.
 *           sec_acl_num_printstrings:
 *               Length of permission printstrings array each ACL
 *               manager maintains on an object.
 *           sec_acl_list:
 *                Complete ACL each ACL manager maintains for the
 *                specified object.
 *           sec_acl:
 *                Each ACL manager's ACL to be operated on,
 *                list of ACL entries, the UUID of the default cell
 *                where authentication takes place (foreign entries 
 *                in the ACL contain the name of their home cell), 
 *                and the UUID of the ACL manager to interpret the list.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp__acl_get_all_managers(unsigned32 tmp_nmgrs,
			  Tcl_Interp *interp,
		          dcp_acl_info_p_t dcp_acl_info_p)
{
    error_status_t  	    	status;
    int                         i;
    sec_acl_posix_semantics_t   *posix_semantics = NULL; 
    unsigned32                  nmgrs_used, total_num_printstrings;
    boolean32                   tokenize;
    uuid_t                      manager_type_chain;

    CLEAR_STATUS(&status);

    /*
     * Clean up previous ACL manager table if it exists 
     * and allocate a new table.
     */

    posix_semantics = (sec_acl_posix_semantics_t *)malloc(
			tmp_nmgrs * sizeof(sec_acl_posix_semantics_t));
    if (posix_semantics == NULL) {
	DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    dcp_acl_info_p->sec_acl_mgr_type = (uuid_p_t)malloc(
					      tmp_nmgrs * sizeof(uuid_t));
    if (dcp_acl_info_p->sec_acl_mgr_type == NULL) {
        free((void *)posix_semantics);
	DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    dcp_acl_info_p->mgr_type_tbl = (dcp_acl_mgr_info_p_t)malloc(
				       tmp_nmgrs * sizeof(dcp_acl_mgr_info_t));
    if (dcp_acl_info_p->mgr_type_tbl == NULL) {
        free((void *)posix_semantics);
	DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }

    /*
     * Determine the ACL manager types that an ACL of type
     * "sec_acl_type" is using to protect the specified ACL object.
     */
    sec_acl_get_mgr_types_semantics(dcp_acl_info_p->sec_acl_handle, 
				    dcp_acl_info_p->sec_acl_type,
    				    tmp_nmgrs, 
    				    &nmgrs_used, 
    				    &dcp_acl_info_p->num_acl_mgrs, 
    				    dcp_acl_info_p->sec_acl_mgr_type, 
    				    posix_semantics, 
    				    &status); 

    /* 
     * Recover from out-of-range error:
     * Use old ACL manager types by using the 
     * sec_acl_get_manager_types() call
     */
    if (STATUS_EQUAL(&status, rpc_s_op_rng_error)) {
	sec_acl_get_manager_types(dcp_acl_info_p->sec_acl_handle, 
				  dcp_acl_info_p->sec_acl_type, 
				  tmp_nmgrs, 
				  &nmgrs_used, 
				  &dcp_acl_info_p->num_acl_mgrs, 
				  dcp_acl_info_p->sec_acl_mgr_type, 
				  &status);
	for (i = 0; i < nmgrs_used; i++) {
	    posix_semantics[i] = sec_acl_posix_no_semantics; 
	}
    } 

    /*
     * Determine if enough space was allocated for all ACL managers
     * If not, try again using "dcp_acl_info_p->num_acl_mgrs".
     */
    if (GOOD_STATUS(&status)) {
        if (dcp_acl_info_p->num_acl_mgrs > tmp_nmgrs) {
            free((void *)posix_semantics);
            if (dcp_acl_info_p->sec_acl_mgr_type != NULL) {
                free((void *)dcp_acl_info_p->sec_acl_mgr_type);
                dcp_acl_info_p->sec_acl_mgr_type = NULL;
            }
            if (dcp_acl_info_p->mgr_type_tbl != NULL) {
                free((void *)dcp_acl_info_p->mgr_type_tbl);
                dcp_acl_info_p->mgr_type_tbl = NULL;
            }
	    return(TCL_OK);
        }
    }
    else {
        /*
         * Some error message is going to pop up, most likely an
         * invalid handle error.  To ensure the acl cleanup goes
         * correctly, reset the number of acl managers to zero.
         */
        dcp_acl_info_p->num_acl_mgrs = 0;
    }

    /*
     * Build ACL manager table
     */
    for (i = 0; 
	 ((i < dcp_acl_info_p->num_acl_mgrs) && (GOOD_STATUS(&status))); i++) {

        /*
	 * Determine the mask calculation type
	 * Set initially to follow POSIX 1003.6 Draft 12
	 * Steps 3, 4, 5, 7, and 8.
	 */
	dcp_acl_info_p->mgr_type_tbl[i].mask_calc_type = POSIX_MASK_OBJ_CALC;

        /*
         * If the ACL manager of the specified object requests no 
         * POSIX "mask_obj" semantics, then the "mask_obj" calculation 
         * is bypassed.
         */
        if (! (FLAG_SET(posix_semantics[i], sec_acl_posix_mask_obj))){
	    dcp_acl_info_p->mgr_type_tbl[i].mask_calc_type = NO_MASK_OBJ_CALC;
        }

        /* 
         * Determine how to format for display the permissions 
         * supported by a specific manager.
         */
        sec_acl_get_printstring(dcp_acl_info_p->sec_acl_handle, 
		&dcp_acl_info_p->sec_acl_mgr_type[i],
		(unsigned32) DCP_C_ACL_PRINTSTRING_LEN, 
		&manager_type_chain, 
		&dcp_acl_info_p->mgr_type_tbl[i].manager_info,
		&tokenize, 
		&total_num_printstrings,
		&dcp_acl_info_p->mgr_type_tbl[i].sec_acl_num_printstrings, 
		dcp_acl_info_p->mgr_type_tbl[i].sec_acl_printstrings, 
		&status);

        /* 
         * Retrieve existing ACL
         */
        if (GOOD_STATUS(&status)) {
            sec_acl_lookup(dcp_acl_info_p->sec_acl_handle, 
		           &dcp_acl_info_p->sec_acl_mgr_type[i],
		           dcp_acl_info_p->sec_acl_type, 
		           &dcp_acl_info_p->mgr_type_tbl[i].sec_acl_list, 
		           &status);
	}
        if (!GOOD_STATUS(&status)) {
            dcp_acl_info_p->mgr_type_tbl[i].sec_acl_list.num_acls = 0;
        }

        /*
         * An object can have more than 1 ACL, but the ACL to be 
         * operated on was bound according to the -entry switch
         * and was obtained according to the ACL type switches;
         * -ic -io and no switch.
         */
        if (GOOD_STATUS(&status)) {
            dcp_acl_info_p->mgr_type_tbl[i].sec_acl_p = 
		dcp_acl_info_p->mgr_type_tbl[i].sec_acl_list.sec_acls[0];
        }

    } /* End of loop thru all ACL managers */

    free((void *)posix_semantics);
    DCP_CHECK_SEC_ERROR(status);
    return(TCL_OK);

} /* End of dcp__acl_get_all_managers() */


/*
 * FUNCTION:
 * dcp_acl_free_acl_mgr_tbl()
 *
 * OVERVIEW:
 * This routine frees the ACL manager table if it exists.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
int
dcp_acl_free_acl_mgr_tbl(Tcl_Interp *interp, dcp_acl_info_p_t dcp_acl_info_p)
{
    int             i,j;
    error_status_t  status;

    if (dcp_acl_info_p->sec_acl_mgr_type != NULL) {
        free((void *)dcp_acl_info_p->sec_acl_mgr_type);
        dcp_acl_info_p->sec_acl_mgr_type = NULL;
    }

    if (dcp_acl_info_p->mgr_type_tbl != NULL) {
        for (i = 0; i < dcp_acl_info_p->num_acl_mgrs; i++) {
            for (j = 0; j < dcp_acl_info_p->mgr_type_tbl[i].sec_acl_list.num_acls; j++) {
                sec_acl_release(dcp_acl_info_p->sec_acl_handle,
                                dcp_acl_info_p->mgr_type_tbl[i].sec_acl_list.sec_acls[j],
                                &status);
                DCP_CHECK_SEC_ERROR(status);
            }
        }
        free((void *)dcp_acl_info_p->mgr_type_tbl);
        dcp_acl_info_p->mgr_type_tbl = NULL;
    }
    return TCL_OK;

} /* End of dcp_acl_free_acl_mgr_tbl() routine */


/*
 * FUNCTION:
 * dcp__acl_verify_manager_specified()
 *
 * OVERVIEW:
 * This routine will verify that the ACL manager specified with 
 * the -type option is an ACL manager supported for the object.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           num_acl_mgrs:
 *               Number of ACL managers for the specified object.
 *           mgr_type_specified:
 *               ACL manager name specified on command line.
 *           manager_info.printstring[i]:
 *               ACL manager names on the object.
 *           UPDATED:
 *           mgr_type_index:
 *               Index into the array of ACL managers where a 
 *               match was found.
 *
 * OUTPUTS:
 * None 
 *
 * RETURNS:
 * TRUE     ACL manager is found.
 * FALSE    ACL manager is not found.
 */
boolean32
dcp__acl_verify_manager_specified(dcp_acl_info_p_t dcp_acl_info_p)
{
    int i;
    int matches = 0;

    /*
     * Determine if specified ACL manager found in ACL manager table
     */
    for (i = 0; i < dcp_acl_info_p->num_acl_mgrs; i++) {
	if (strncmp(
	       (char *)dcp_acl_info_p->mgr_type_specified,
	       (char *)dcp_acl_info_p->mgr_type_tbl[i].manager_info.printstring,
	       strlen(dcp_acl_info_p->mgr_type_specified)) == 0) {
	    dcp_acl_info_p->mgr_type_index = i;
	    matches++;
	}
    } /* End of loop thru all ACL managers */

    if (matches == 1) {
        return(TRUE);
    } else {
        return(FALSE);
    }

} /* End of dcp__acl_verify_manager_specified() */


/*
 * FUNCTION:
 * dcp__acl_bind_cell_rgys()
 *
 * OVERVIEW:
 * This routine will bind to the local cell registry. 
 * If the local cell UUID is not equal to the default cell UUID, then 
 * bind to the default cell registry. If errors occur obtaining default 
 * cell info, then the local cell registry will NOT be used for acl 
 * operations.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           None
 *           UPDATED:
 *           local_cell_rgy_handle:
 *                A registry server handle associated with the local
 *                cell (/.:).
 *           local_cell_name:
 *                Full name of local cell.
 *           local_cell_uuid:
 *                UUID of local cell.
 *           default_cell_rgy_handle:
 *                A registry server handle associated with the default
 *                cell.
 *           default_cell_name:
 *                Full name of default cell.
 *           default_cell_uuid:
 *                UUID of default cell.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp__acl_bind_cell_rgys(Tcl_Interp *interp, 
		        dcp_acl_info_p_t dcp_acl_info_p)
{
    error_status_t  	    	status;
    sec_rgy_handle_t            rgy_handle;
    uuid_t              	dummy_uuid;
    unsigned_char_p_t           name1 = NULL;

    CLEAR_STATUS(&status);

    /* 
     * Obtain a handle to the local cell's registry 
     */
    status = dcp__acl_bind_registry(interp, 
    				    (char *)DCP_C_ACL_LOCAL_CELL,
				    &rgy_handle);
    DCP_CHECK_SEC_ERROR(status);

    dcp_acl_info_p->local_cell_rgy_handle = rgy_handle;

    /* 
     * Obtain the name/uuid of the local cell 
     */
    sec_id_parse_name(dcp_acl_info_p->local_cell_rgy_handle, 
                      (unsigned_char_p_t)DCP_C_ACL_LOCAL_CELL,
		      dcp_acl_info_p->local_cell_name,
		      &dcp_acl_info_p->local_cell_uuid, 
		      NULL, 
		      NULL,
		      &status);
    DCP_CHECK_SEC_ERROR(status);

    /* 
     * Validate the name/uuid of the default cell held in
     * the ACL with the local cell registry.  
     * This is the name/uuid of the cell where the ACL was
     * created and where the principal/group is registered.
     *
     * default cell name:
     * dcp_acl_info_p->sec_acl_p->default_realm.name
     * default cell uuid:
     * dcp_acl_info_p->sec_acl_p->default_realm.uuid
     */

    sec_id_gen_name(dcp_acl_info_p->local_cell_rgy_handle, 
		      &DCP_C_ACL_MGR.sec_acl_p->default_realm.uuid,
		      &dummy_uuid,
		      NULL, 
		      dcp_acl_info_p->default_cell_name, 
		      NULL,
		      &status);

    if (GOOD_STATUS(&status)){
        dcp_acl_info_p->default_cell_uuid = 
	    DCP_C_ACL_MGR.sec_acl_p->default_realm.uuid;
        if (uuid_equal(&dcp_acl_info_p->default_cell_uuid, 
		       &dcp_acl_info_p->local_cell_uuid, 
		       &status)) {
	    dcp_acl_info_p->default_cell_rgy_handle = 
	        dcp_acl_info_p->local_cell_rgy_handle;
        } /* End of local and default cell are the same */
        else {
	    /* 
	     * Obtain a handle to the default cell's registry 
	     */
	    CLEAR_STATUS(&status);
	    status = dcp__acl_bind_registry(
			        interp, 
	   	   	        (char *)dcp_acl_info_p->default_cell_name,
			        &rgy_handle);
	    if (GOOD_STATUS(&status))
    	        dcp_acl_info_p->default_cell_rgy_handle = rgy_handle;
        } /* End of local and default cell are different */
    } /* End of good status occurred */
    else {
	/*
	 * If an error occurs trying to validate the default cell,
	 * the local cell registry will be used for operations.
	 * This way the user is able to view the ACL entries but 
	 * any entries with keys will have UUID strings and the
	 * default cell will also be a UUID string.
	 */
        dcp_acl_info_p->default_cell_rgy_handle = 
			dcp_acl_info_p->local_cell_rgy_handle;
        dcp_acl_info_p->default_cell_uuid = 
			DCP_C_ACL_MGR.sec_acl_p->default_realm.uuid;
        uuid_to_string(&dcp_acl_info_p->default_cell_uuid,
		       &name1,
	               &status);
        if (GOOD_STATUS(&status)){
           strcpy((char *)dcp_acl_info_p->default_cell_name,
                  (char *)name1);
           rpc_string_free(&name1, &status);
        } else {
           strcpy((char *)dcp_acl_info_p->default_cell_name,
                  "<UNKNOWN DEFAULT CELL>");
        }
        CLEAR_STATUS(&status); 
    } /* End of use local registry handle only */

    /*
     * If an error occurs trying to bind to the default cell's registry, 
     * the local cell registry will NOT be used for operations
     */
    DCP_CHECK_SEC_ERROR(status);

    return(TCL_OK);

} /* End of dcp__acl_bind_cell_rgys() */


/*
 * FUNCTION:
 * dcp__acl_bind_registry()
 *
 * OVERVIEW:
 * This routine returns a registry server handle associated with
 * the particular cell passed in.
 *
 * INPUTS:
 * cell_name:
 *           Name of cell to associate with registry server.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * rgy_handle:
 *           A registry server handle associated with the cell.
 *
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
error_status_t
dcp__acl_bind_registry(Tcl_Interp *interp, 
		       char *cell_name,
		       sec_rgy_handle_t *rgy_handle)
{
    error_status_t  	    	status;
    sec_rgy_bind_auth_info_t    auth_info;

    /* 
     * Bind to the registry in the cell and obtain
     * registry handle
     */
    if (dcp_is_logged_in()){
        auth_info.tagged_union.dce_info.authn_level =
				rpc_c_protect_level_default;
        auth_info.tagged_union.dce_info.authn_svc =
	    			rpc_c_authn_default;
        auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
        auth_info.info_type = sec_rgy_bind_auth_dce;
    } else {
        auth_info.tagged_union.dce_info.authn_level =
				rpc_c_protect_level_none;
        auth_info.tagged_union.dce_info.authn_svc =
	    			rpc_c_authn_none;
        auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_none;
        auth_info.info_type = sec_rgy_bind_auth_none;
    }
    auth_info.tagged_union.dce_info.identity  = NULL;

    CLEAR_STATUS(&status);
    sec_rgy_cell_bind((unsigned_char_p_t)cell_name, 
    		  &auth_info,
		  rgy_handle, 
		  &status);

    return(status);

} /* End of dcp__acl_bind_registry() */


/*
 * FUNCTION:
 * dcp__acl_generate_key_uuid()
 *
 * OVERVIEW:
 * This routine generates the UUID associated with a key name specified
 * by the user and updates the type sec_id_t and sec_id_foreign_t of an 
 * ACL entry accordingly.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           default_cell_rgy_handle:
 *                A registry server handle associated with the default
 *                cell.
 *           local_cell_rgy_handle:
 *                A registry server handle associated with the local
 *                cell(/.:).
 *
 * key_name:
 *           A key name specified by the user for an ACL entry
 *           modification on a specified object.
 *
 * INPUT/OUTPUTS:
 * sec_acl_entry_p:
 *           A pointer to a specified ACL entry for modification.
 *           REFERENCED:
 *           entry_info.entry_type:
 *               ACL type for the ACL entry.
 *           UPDATED:
 *           entry_info.tagged_union.id.uuid:
 *               UUID of the principal or group with type sec_id_t.
 *           entry_info.tagged_union.id.name:
 *               Name of the principal or group with type sec_id_t.
 *           entry_info.tagged_union.foreign_id.realm.uuid:
 *               UUID of the cell with type sec_foreign_id_t.
 *           entry_info.tagged_union.foreign_id.realm.name:
 *               Name of the cell with type sec_foreign_id_t.
 *           entry_info.tagged_union.foreign_id.id.uuid:
 *               UUID of the principal or group with type sec_foreign_id_t.
 *           entry_info.tagged_union.foreign_id.id.name:
 *               Name of the principal or group with type sec_foreign_id_t.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t 
dcp__acl_generate_key_uuid(dcp_acl_info_p_t dcp_acl_info_p,
			   char *key_name,
			   sec_acl_entry_t *sec_acl_entry_p)
{
    error_status_t  	    	status;
    sec_rgy_name_t              global_name;
    sec_rgy_name_t              cell_name;
    sec_rgy_name_t              prin_name;
    uuid_t                      dummy_uuid;
    uuid_t                      prin_uuid;
    uuid_t                      cell_uuid;

    CLEAR_STATUS(&status);

    /*
     * The empty string is not a valid key.
     */
    if ((key_name == NULL) || (*key_name == '\0')){
	SET_STATUS(&status, dcp_s_acl_inv_entry_key);
	return(status);
    }

    if (! (dcp__acl_is_2_key_entry(sec_acl_entry_p))){
	sec_acl_entry_p->entry_info.tagged_union.id.name =
     	    (unsigned_char_p_t)
	        malloc((strlen(key_name)+1) * sizeof(unsigned_char_t));
	if (sec_acl_entry_p->entry_info.tagged_union.id.name == NULL){
	    SET_STATUS(&status, dcp_s_no_memory);  
	    return(status);
	}
	strcpy((char *)sec_acl_entry_p->entry_info.tagged_union.id.name,
			    key_name);
	/*
	 * global_name used for "user", "user_delegate", "group",
	 * and "group_delegate" entry types only.
	 */
	sprintf((char *)global_name, "%s/%s", 
	    		    dcp_acl_info_p->default_cell_name, 
			    key_name);
    }

    if ((sec_acl_entry_p->entry_info.entry_type == sec_acl_e_type_user) ||
        (sec_acl_entry_p->entry_info.entry_type == sec_acl_e_type_user_deleg)){
	/*
	 * Obtain principal uuid for ACL type "user" or "user_delegate"
	 */
        sec_id_parse_name(dcp_acl_info_p->default_cell_rgy_handle, 
		          global_name, 
    		          NULL,
		          &dummy_uuid, 
			  prin_name, 
			  &prin_uuid, 
			  &status);
	if (GOOD_STATUS(&status))
            sec_acl_entry_p->entry_info.tagged_union.id.uuid = prin_uuid;
    }
    else if ((sec_acl_entry_p->entry_info.entry_type == 
					    sec_acl_e_type_group) ||
             (sec_acl_entry_p->entry_info.entry_type == 
					    sec_acl_e_type_group_deleg)){
	/*
	 * Obtain group uuid for ACL type "group" or "group_delegate"
	 */
        sec_id_parse_group(dcp_acl_info_p->default_cell_rgy_handle, 
		          global_name, 
    		          NULL,
		          &dummy_uuid, 
			  prin_name, 
			  &prin_uuid, 
			  &status);
	if (GOOD_STATUS(&status))
            sec_acl_entry_p->entry_info.tagged_union.id.uuid = prin_uuid;
    } else if ((sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_foreign_other) ||
               (sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_for_other_deleg)){
	/*
	 * Obtain cell uuid for ACL type "foreign_other" or 
	 * "foreign_other_delegate"
	 */
        sec_id_parse_name(dcp_acl_info_p->local_cell_rgy_handle, 
	     		  (unsigned_char_p_t)key_name, 
			  cell_name,
			  &cell_uuid,
		          NULL, 
			  &dummy_uuid,
			  &status);
	if (GOOD_STATUS(&status))
            sec_acl_entry_p->entry_info.tagged_union.id.uuid = cell_uuid;
    } else if ((sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_foreign_user) ||
               (sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_for_user_deleg)){
	/*
	 * Obtain cell and principal uuid for ACL type "foreign_user"
	 */
        sec_id_parse_name(dcp_acl_info_p->local_cell_rgy_handle, 
	     		  (unsigned_char_p_t)key_name, 
			  cell_name, 
			  &cell_uuid, 
			  prin_name, 
			  &prin_uuid, 
			  &status);
    } else if ((sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_foreign_group) ||
               (sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_for_group_deleg)){
	/*
	 * Obtain cell and group uuid for ACL type "foreign_group"
	 * or "foreign_group_delegate"
	 */
        sec_id_parse_group(dcp_acl_info_p->local_cell_rgy_handle, 
	     		  (unsigned_char_p_t)key_name, 
			  cell_name, 
			  &cell_uuid, 
			  prin_name, 
			  &prin_uuid, 
			  &status);
    }

    if ((GOOD_STATUS(&status)) && (dcp__acl_is_2_key_entry(sec_acl_entry_p))){
	/* 
	 * Set principal/group info 
	 */
	sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.name =
	  (unsigned_char_p_t) 
	    malloc((strlen((char *)prin_name)+1) * sizeof(unsigned_char_t));
       if (sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.name == NULL){
	    SET_STATUS(&status, dcp_s_no_memory);  
	    return(status);
	}
	strcpy((char *)
	    sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.name,
	    (char *)prin_name);
        sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.uuid = 
	    prin_uuid;
	/* 
	 * Set cell info 
	 */
	sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.name =
	    (unsigned_char_p_t) 
	      malloc((strlen((char *)cell_name)+1) * sizeof(unsigned_char_t));
       if (sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.name == NULL){
            free((void *)sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.name);
	    SET_STATUS(&status, dcp_s_no_memory);  
	    return(status);
	}
	strcpy((char *)
	    sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.name,
	    (char *)cell_name);
	sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.uuid =
	    cell_uuid;
    }

    return(status);

} /* End of dcp__acl_generate_key_uuid() */


/*
 * FUNCTION:
 * dcp__acl_generate_key_name()
 *
 * OVERVIEW:
 * This routine returns a key name with type sec_id_t and
 * sec_id_foreign_t of an ACL entry.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           default_cell_rgy_handle:
 *                A registry server handle associated with the default
 *                cell.
 *           default_cell_uuid:
 *                UUID of default cell.
 *           local_cell_rgy_handle:
 *                A registry server handle associated with the local
 *                cell(/.:).
 * sec_acl_entry_p:
 *           A pointer to one particular ACL entry of the
 *           specified object.
 *           REFERENCED:
 *           entry_info.entry_type:
 *               ACL type for the ACL entry.
 *           entry_info.tagged_union.id.uuid:
 *               UUID of the principal or group with type sec_id_t.
 *           entry_info.tagged_union.foreign_id.realm.uuid:
 *               UUID of the cell with type sec_foreign_id_t.
 *           entry_info.tagged_union.foreign_id.id.uuid:
 *               UUID of the principal or group with type sec_foreign_id_t.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * key_name: 
 *          Formatted key for the ACL entry type.
 */
char * 
dcp__acl_generate_key_name(dcp_acl_info_p_t dcp_acl_info_p,
			    sec_acl_entry_t *sec_acl_entry_p)	
{
    error_status_t  	    	status;
    sec_rgy_name_t              global_name_p;
    unsigned_char_p_t           name1 = NULL;
    unsigned_char_p_t           name2 = NULL;
    uuid_t                      dummy_uuid;
    char                        key_name[BUFSIZ];

    CLEAR_STATUS(&status);
    if ((sec_acl_entry_p->entry_info.entry_type == sec_acl_e_type_user) ||
        (sec_acl_entry_p->entry_info.entry_type == sec_acl_e_type_user_deleg)){
	/*
	 * Obtain principal name for ACL type "user" or "user_delegate"
	 */
        sec_id_gen_name(dcp_acl_info_p->default_cell_rgy_handle, 
		        &dcp_acl_info_p->default_cell_uuid, 
    		        &sec_acl_entry_p->entry_info.tagged_union.id.uuid,
		        NULL, NULL, global_name_p, &status);
    }
    else if ((sec_acl_entry_p->entry_info.entry_type == 
					    sec_acl_e_type_group) ||
             (sec_acl_entry_p->entry_info.entry_type == 
					    sec_acl_e_type_group_deleg)){
	/*
	 * Obtain group name for ACL type "group" or "group_delegate"
	 */
        sec_id_gen_group(dcp_acl_info_p->default_cell_rgy_handle, 
		         &dcp_acl_info_p->default_cell_uuid, 
    		         &sec_acl_entry_p->entry_info.tagged_union.id.uuid,
		         NULL, NULL, global_name_p, &status);
    } else if ((sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_foreign_other) ||
               (sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_for_other_deleg)){
	/*
	 * Obtain cell name for ACL type "foreign_other" or
	 * "foreign_other_delegate"
	 */
        sec_id_gen_name(dcp_acl_info_p->local_cell_rgy_handle, 
    		        &sec_acl_entry_p->entry_info.tagged_union.id.uuid,
			&dummy_uuid,
		        NULL, global_name_p, NULL, &status);
    } else if ((sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_foreign_user) ||
               (sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_for_user_deleg)){
	/*
	 * Obtain cell and principal name for ACL type "foreign_user"
	 * or "foreign_user_delegate"
	 */
        sec_id_gen_name(dcp_acl_info_p->local_cell_rgy_handle, 
    	     &sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
    	     &sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.uuid,
	     global_name_p, NULL, NULL, &status);
    } else if ((sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_foreign_group) ||
               (sec_acl_entry_p->entry_info.entry_type == 
				    sec_acl_e_type_for_group_deleg)){
	/*
	 * Obtain cell and group name for ACL type "foreign_group"
	 */
        sec_id_gen_group(dcp_acl_info_p->local_cell_rgy_handle, 
    	     &sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
    	     &sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.uuid,
	     global_name_p, NULL, NULL, &status);
    }

    if (GOOD_STATUS(&status)){  
	strcpy(key_name, (char *)global_name_p); 
    } /* End of key name formatted by security routine */
    else {
	/*
	 * If no name generated, 
	 * then show UUID string for a key
	 */
	if (! (dcp__acl_is_2_key_entry(sec_acl_entry_p))) {
	    /*
	     * Obtain applicable principal or group UUID string
	     */
	    CLEAR_STATUS(&status);
	    uuid_to_string(&sec_acl_entry_p->entry_info.tagged_union.id.uuid,
		       &name1,
		       &status);
    	    if (GOOD_STATUS(&status)){  
	        sprintf(key_name,
		        "%s",
		        (char *)name1);
                CLEAR_STATUS(&status);
                rpc_string_free(&name1, &status);
	    } else {
	        sprintf(key_name,
		        "%s",
		        "<ILLEGAL UUID>");
	    }
	}  /* End of id_t ACL entry type */
	else {
	    /*
	     * Obtain cell name UUID string
	     */
	    CLEAR_STATUS(&status);
	    uuid_to_string(
	       &sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.uuid,
	       &name1,
	       &status);
    	    if (GOOD_STATUS(&status)){  
	        /*
	         * Obtain applicable principal or group UUID string
	         */
	        CLEAR_STATUS(&status);
	        uuid_to_string(
		&sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.uuid,
		    &name2,
		    &status);
    	        if (GOOD_STATUS(&status)){  
	            sprintf(key_name,
		            "%s/%s",
		            (char *)name1,
		            (char *)name2);
                    CLEAR_STATUS(&status);
                    rpc_string_free(&name1, &status);
                    CLEAR_STATUS(&status);
                    rpc_string_free(&name2, &status);
		} else {
	            sprintf(key_name,
		            "%s/%s",
		            (char *)name1,
		            "<ILLEGAL UUID>");
                    CLEAR_STATUS(&status);
                    rpc_string_free(&name1, &status);
		}
	    } else {
	        sprintf(key_name,
		        "%s/%s",
		        "<ILLEGAL UUID>",
		        "<ILLEGAL UUID>");
	    }
	}  /* End of foreign_id_t ACL entry type */
    } /* End of format key name manually */

    return(key_name);

} /* End of dcp__acl_generate_key_name() */


/*
 * FUNCTION:
 * dcp_acl_do_permissions()
 *
 * OVERVIEW:
 * This routine lists the permissions associated with the specified 
 * object. Each element of the list is a list of two items,
 * the first is the permission token, the second is a description of
 * the permission.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           sec_acl_num_printstrings:
 *               Length of permission printstrings array.
 *           sec_acl_printstrings[i].printstring:
 *               A character string describing the printable 
 *               representation of a specified permission. 
 *           sec_acl_printstrings[i].helpstring:
 *               A character string containing some text that can 
 *               be used to describe the specified permission. 
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp_acl_do_permissions(Tcl_Interp *interp,
			 dcp_acl_info_p_t dcp_acl_info_p)
{
    int i;
    char output_string[BUFSIZ];

    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_num_printstrings; i++){
	sprintf(output_string,
		"{%s {%s}}",
	        DCP_C_ACL_MGR.sec_acl_printstrings[i].printstring,
	        DCP_C_ACL_MGR.sec_acl_printstrings[i].helpstring);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
	if (i < (DCP_C_ACL_MGR.sec_acl_num_printstrings - 1)){
            Tcl_AppendResult(interp, "\n", (char *)NULL);
	}    
    } /* End of for loop thru acl printstrings */

} /* End of dcp_acl_do_permissions() */


/*
 * FUNCTION:
 * dcp_acl_do_check()
 *
 * OVERVIEW:
 * This routine lists the ACL permission set that the caller has for
 * the specified object.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 * 	     sec_acl_handle:
 *               A pointer to the sec_acl_handle_t variable to receive the
 *               returned ACL handle.  The other sec_acl routines use this 
 *               handle to refer to the ACL for the object specified with 
 *               named_object.
 *           sec_acl_mgr_type:
 *               The UUID identifying the ACL manager type protecting 
 *               an object
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 * granted_permset:
 *           ACL permission set that the caller has for the specified object
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_check(Tcl_Interp *interp,
		 dcp_acl_info_p_t dcp_acl_info_p)
{
    error_status_t        status;
    sec_acl_permset_t     granted_permset;

    /*
     * Obtain the granted permission set 
     */
    CLEAR_STATUS(&status);
    sec_acl_get_access(dcp_acl_info_p->sec_acl_handle,
	     &dcp_acl_info_p->sec_acl_mgr_type[dcp_acl_info_p->mgr_type_index],
	     &granted_permset,
   	     &status);
    
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Display the granted permission set 
     */
    dcp__acl_print_permset(interp, dcp_acl_info_p, granted_permset);

    return(TCL_OK);

} /* End of dcp_acl_do_check() */


/*
 * FUNCTION:
 * dcp_acl_do_show_cell()
 *
 * OVERVIEW:
 * This routine returns the value of the default cell for the ACL.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           default_cell_name:
 *               Full name of default cell.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp_acl_do_show_cell(Tcl_Interp *interp,
		     dcp_acl_info_p_t dcp_acl_info_p)
{
    char output_string[BUFSIZ];

    sprintf(output_string, "%s", dcp_acl_info_p->default_cell_name);
    Tcl_AppendResult(interp, 
    	    	     output_string,
		     (char *)NULL);

} /* End of dcp_acl_do_show_cell() */


/*
 * FUNCTION:
 * dcp_acl_do_show_mgrs()
 *
 * OVERVIEW:
 * This routine returns the names of all ACL managers supported for
 * the specified object.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           num_acl_mgrs:
 *               Number of ACL managers for the specified object.
 *           manager_info.printstring:
 *               ACL manager name on an object.
 *           manager_info.helpstring:
 *               Help text describing the ACL manager.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp_acl_do_show_mgrs(Tcl_Interp *interp,
		     dcp_acl_info_p_t dcp_acl_info_p)
{
    char                        output_string[BUFSIZ];
    int                         i;

    for (i = 0; i < dcp_acl_info_p->num_acl_mgrs; i++){
        sprintf(output_string,
                "{%-16s {%s}}",
                dcp_acl_info_p->mgr_type_tbl[i].manager_info.printstring,
                dcp_acl_info_p->mgr_type_tbl[i].manager_info.helpstring);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

	if (i != (dcp_acl_info_p->num_acl_mgrs - 1))
	    Tcl_AppendResult(interp, "\n", (char *)NULL);

    } /* End of for loop thru acl printstrings */

} /* End of dcp_acl_do_show_mgrs() */


/*
 * FUNCTION:
 * dcp_acl_do_save_sec_acl()
 *
 * OVERVIEW:
 * This routine saves the ACL for the specified object. 
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_save_sec_acl(Tcl_Interp *interp,
		       dcp_acl_info_p_t dcp_acl_info_p)
{
    error_status_t              status;

    CLEAR_STATUS(&status);
    sec_acl_replace(dcp_acl_info_p->sec_acl_handle, 
        &dcp_acl_info_p->sec_acl_mgr_type[dcp_acl_info_p->mgr_type_index],
        dcp_acl_info_p->sec_acl_type,
        &DCP_C_ACL_MGR.sec_acl_list, 
	&status);
    DCP_CHECK_SEC_ERROR(status);

    return(TCL_OK);

} /* End of dcp_acl_do_save_sec_acl() routine */


/*
 * FUNCTION:
 * dcp_acl_do_modify_entries()
 *
 * OVERVIEW:
 * This routine modifies one or more ACL entries for the specified
 * object. 
 *
 * INPUTS:
 * purge_switch:
 *          A flag to indicate that the -purge switch was specified.
 * mask_string:
 *          A pointer to the value of the -mask switch.
 * remove_entries_tcllist:
 *          A pointer to a list of ACL entries specified to be removed.
 * change_entries_tcllist:
 *          A pointer to a list of ACL entries specified to be changed.
 * add_entries_tcllist:
 *          A pointer to a list of ACL entries specified to be added.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->num_entries:
 *              Number of ACL entries for the specified object.
 *           sec_acl_p->sec_acl_entries[i]:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_modify_entries(int purge_switch,
			  char *mask_string,   
			  char *remove_entries_tcllist,
			  char *change_entries_tcllist,
			  char *add_entries_tcllist,
			  Tcl_Interp *interp,
		          dcp_acl_info_p_t dcp_acl_info_p)
{
    int remove_entry_argc;
    char **remove_entry_argv = (char **)NULL;
    sec_acl_entry_t *remove_acl_entries = (sec_acl_entry_t *)NULL;
    int add_entry_argc;
    char **add_entry_argv = (char **)NULL;
    sec_acl_entry_t *add_acl_entries = (sec_acl_entry_t *)NULL;
    int change_entry_argc;
    char **change_entry_argv = (char **)NULL;
    sec_acl_entry_t *change_acl_entries = (sec_acl_entry_t *)NULL;
    sec_acl_entry_t new_mask_obj_entry;
    int i;
    error_status_t status;
    boolean32 group_class_modified;  /* Is this entry for modification 
                                      * a member of the file group class? 
				      */
    sec_acl_permset_t actual_perms, 
		      effective_perms,
                      masked_perms,
		      granted_perms,
		      ignore_perms,
		      granted_perms_from_chg,
		      granted_perms_from_add,
		      candidate_mask_perms;

    /* 
     * Perform initialization
     */
    CLEAR_STATUS(&status);
    group_class_modified = FALSE;
    actual_perms = effective_perms = masked_perms = 0;
    granted_perms = granted_perms_from_chg = granted_perms_from_add = 0;
    ignore_perms = candidate_mask_perms = 0;

    /*
     * Purge ineffective permissions from all file group class
     * entries first if requested.
     * STEP 2 POSIX 1003.6 Draft 12 
     */
    if (purge_switch){
        status = dcp__acl_do_purge_ineffective(dcp_acl_info_p);
	DCP_CHECK_SEC_ERROR(status); 
    } /* End of purge processing */

    /*
     * Validate all ACL entries specified to be modified
     */
    if (remove_entries_tcllist != NULL){
	/*
	 * Cycle through all ACL entries specified to be removed
	 */
	if (dcp__acl_list_entries(interp, dcp_acl_info_p, FALSE,
		    remove_entries_tcllist,
		    &remove_entry_argc, &remove_entry_argv) != TCL_OK){
		return(TCL_ERROR);
	}

	if (GOOD_STATUS(&status)){
	    remove_acl_entries = (sec_acl_entry_t *)malloc(
		    ((remove_entry_argc) * (sizeof(sec_acl_entry_t))));
	    if (remove_acl_entries == NULL){
	        SET_STATUS(&status, dcp_s_no_memory);
	    } 
	}

	for (i = 0; (i < remove_entry_argc && (GOOD_STATUS(&status))); i++){
	    status = dcp__acl_validate_entry(dcp_acl_info_p,
					 (char *)remove_entry_argv[i],
					 FALSE,
					 &(remove_acl_entries[i]));
	    if (GOOD_STATUS(&status)){
	        if (dcp__acl_is_file_group_class(&(remove_acl_entries[i])))
		    group_class_modified = TRUE;
		/*
		 * If the mask_obj ACL entry was explictly given,
		 * then do not automatically re-calculate the mask.
		 */
	        if (remove_acl_entries[i].entry_info.entry_type ==
		                                sec_acl_e_type_mask_obj){
		    DCP_C_ACL_MGR.mask_calc_type = NO_MASK_OBJ_CALC;
	        } /* End of mask_obj ACL entry check */
	    } /* End of good status */

	} /* End of loop thru entries to remove */
    } /* End of process entries to remove */


    if ((GOOD_STATUS(&status) && 
        (change_entries_tcllist != NULL))){
	/*
	 * Cycle through all ACL entries specified to be removed
	 */
	if (dcp__acl_list_entries(interp, dcp_acl_info_p, TRUE,
		change_entries_tcllist,
		&change_entry_argc, &change_entry_argv) != TCL_OK){
	    dcp__acl_free_specified_entries(remove_entry_argv, 
					    remove_acl_entries, 
					    change_entry_argv, 
					    change_acl_entries, 
					    add_entry_argv, 
					    add_acl_entries);
	    return(TCL_ERROR);
	}
	if (GOOD_STATUS(&status)){
	    change_acl_entries = (sec_acl_entry_t *)malloc(
		    ((change_entry_argc) * (sizeof(sec_acl_entry_t))));
	    if (change_acl_entries == NULL){
	        SET_STATUS(&status, dcp_s_no_memory);
	    }
	}

	for (i = 0; (i < change_entry_argc && (GOOD_STATUS(&status))); i++){
	    status = dcp__acl_validate_entry(dcp_acl_info_p,
					 (char *)change_entry_argv[i],
					 TRUE,
					 &(change_acl_entries[i]));
	    if (GOOD_STATUS(&status)){
	        if (dcp__acl_is_file_group_class(&(change_acl_entries[i])))
		    group_class_modified = TRUE;
		/*
		 * If the mask_obj ACL entry was explictly given,
		 * then do not automatically re-calculate the mask.
		 */
	        if (change_acl_entries[i].entry_info.entry_type ==
		                                sec_acl_e_type_mask_obj){
		    DCP_C_ACL_MGR.mask_calc_type = NO_MASK_OBJ_CALC;
		    if (mask_string != NULL)
			SET_STATUS(&status, dcp_s_acl_mask_obj_specified);
	        } /* End of mask_obj ACL entry specified */
	    } /* End of good status */

	} /* End of loop thru entries to change */
    } /* End of process entries to change */

    if ((GOOD_STATUS(&status) && 
        (add_entries_tcllist != NULL))){
	/*
	 * Cycle through all ACL entries specified to be added
	 */
	if (dcp__acl_list_entries(interp, dcp_acl_info_p, TRUE,
		add_entries_tcllist,
		&add_entry_argc, &add_entry_argv) != TCL_OK){
	    dcp__acl_free_specified_entries(remove_entry_argv, 
					    remove_acl_entries, 
					    change_entry_argv, 
					    change_acl_entries, 
					    add_entry_argv, 
					    add_acl_entries);
	    return(TCL_ERROR);
	}
	if (GOOD_STATUS(&status)){
	    add_acl_entries = (sec_acl_entry_t *)malloc(
		    ((add_entry_argc) * (sizeof(sec_acl_entry_t))));
	    if (add_acl_entries == NULL){
	        SET_STATUS(&status, dcp_s_no_memory);
	    }
	}

	for (i = 0; (i < add_entry_argc && (GOOD_STATUS(&status))); i++){
	    status = dcp__acl_validate_entry(dcp_acl_info_p,
					 (char *)add_entry_argv[i],
					 TRUE,
					 &(add_acl_entries[i]));
	    if (GOOD_STATUS(&status)) {
	        if (dcp__acl_is_file_group_class(&(add_acl_entries[i])))
		    group_class_modified = TRUE;
		/*
		 * If the mask_obj ACL entry was explictly given,
		 * then do not automatically re-calculate the mask.
		 */
	        if (add_acl_entries[i].entry_info.entry_type ==
		                                sec_acl_e_type_mask_obj){
		    DCP_C_ACL_MGR.mask_calc_type = NO_MASK_OBJ_CALC;
		    if (mask_string != NULL)
			SET_STATUS(&status, dcp_s_acl_mask_obj_specified);
	        } /* End of mask_obj ACL entry specified */
	    } /* End of good status */

	} /* End of loop thru entries to add */
    } /* End of process entries to add */


    /*
     * Perform requests to remove entries with -remove switch
     * STEP 2 POSIX 1003.6 Draft 12 
     */
    if ((GOOD_STATUS(&status) && 
        (remove_entries_tcllist != NULL))){
	for (i = 0; (i < remove_entry_argc && (GOOD_STATUS(&status))); i++) {
	    status = dcp__acl_do_remove_entries(&(remove_acl_entries[i]),
	    					dcp_acl_info_p);
	} /* End of loop thru entries to remove */
    } /* End of remove acl entry processing */

    /*
     * Perform requests to reduce permissions of existing entries
     * with -change switch.
     * STEP 2 POSIX 1003.6 Draft 12 
     */
    if ((GOOD_STATUS(&status) && 
        (change_entries_tcllist != NULL))){
	for (i = 0; (i < change_entry_argc && (GOOD_STATUS(&status))); i++) {
	    status = dcp__acl_do_change_entries(&(change_acl_entries[i]),
						 DECREASE_MOD,
	    					 dcp_acl_info_p,
	    					 &ignore_perms);
	} /* End of loop thru entries to change */
    } /* End of change acl entries to reduce perms */

    if ((GOOD_STATUS(&status) && 
        (DCP_C_ACL_MGR.mask_calc_type == POSIX_MASK_OBJ_CALC))) {
        /* 
         * Calculate the union of "actual" permissions of the 
         * existing remaining entries in the file group class.
         * STEP 3 POSIX 1003.6 Draft 12 
         */
	dcp__acl_calc_union_permissions(dcp_acl_info_p, 
					FALSE,
					&actual_perms);
	
        /* 
         * Calculate the union of "effective" permissions of the
         * existing remaining entries in the file group class.
         * STEP 4 POSIX 1003.6 Draft 12 
         */
	dcp__acl_calc_union_permissions(dcp_acl_info_p,
					TRUE,
					&effective_perms);

        /* 
         * "masked" permissions:
         * Calculate the permissions that are not granted when logical
         * exclusive OR of STEP3 (union of all actual permissions) 
         * and STEP4 (union of all effective permissions).
         * STEP 5 POSIX 1003.6 Draft 12 
         */
	masked_perms = actual_perms ^ effective_perms;

    } /* End of POSIX 1003.6 STEPS 3, 4, 5 */

    /* 
     * Perform additions and modifications that increase permissions 
     * of existing entries. The outcome of doing this is building a 
     * list of newly "granted" permissions.
     */
    /*
     * Perform requests to increase permissions of existing entries
     * with -change switch.
     * STEP 6 POSIX 1003.6 Draft 12 
     */
    if ((GOOD_STATUS(&status) && 
        (change_entries_tcllist != NULL))){

        sec_acl_permset_t  changed_perms;

	for (i = 0; (i < change_entry_argc && (GOOD_STATUS(&status))); i++) {
            /*
             * It is the callee's responsibility to intialize
             * 'changed_perms' correctly.
             */
	    status = dcp__acl_do_change_entries(&(change_acl_entries[i]),
						 INCREASE_MOD,
	    					 dcp_acl_info_p,
	    					 &changed_perms);
            granted_perms_from_chg |= changed_perms;
	} /* End of loop thru entries to change */
    } /* End of change acl entries to increase perms */

    /*
     * Perform requests to add new entries with -add switch.
     * STEP 6 POSIX 1003.6 Draft 12 
     */
    if ((GOOD_STATUS(&status) && 
        (add_entries_tcllist != NULL))){

        sec_acl_permset_t  added_perms;

	for (i = 0; (i < add_entry_argc && (GOOD_STATUS(&status))); i++) {
            /*
             * It is the callee's responsibility to intialize
             * 'added_perms' correctly.
             */
	    status = dcp__acl_do_add_entries(&(add_acl_entries[i]),
					     INCREASE_MOD,
	    				     dcp_acl_info_p,
	    				     &added_perms);
            granted_perms_from_add |= added_perms;
	} /* End of loop thru entries to add */
    } /* End of add acl entries processing */

    /*
     *"granted" permissions:
     * Permissions that are added due to performing any ACL additions
     * with -add switch and/or ACL modifications that may increase 
     * permissions.
     * STEP 6 POSIX 1003.6 Draft 12 
     */
    if (GOOD_STATUS(&status))
        granted_perms = granted_perms_from_chg | granted_perms_from_add;

    if ((GOOD_STATUS(&status)) && 
	(DCP_C_ACL_MGR.mask_calc_type == POSIX_MASK_OBJ_CALC) && 
	(group_class_modified)) { 
        /*
         * "candidate" new mask value:
         * Calculate the union of the newly "granted" permissions (STEP6) 
         * and the old effective permissions(STEP4).
         * STEP 7 POSIX 1003.6 Draft 12 
         */
	candidate_mask_perms = effective_perms | granted_perms;

        /*
         * If there are any permissions in the "candidate new mask"
         * (STEP7) that are also in the "masked" permissions(STEP5),
         * applying the "candidate new mask" would unexpectedly grant some
         * new right that the user did not intend. This condition will 
         * generate an error and the ACL will not be modified unless the user
         * specified -mask calc, -mask nocalc, or purge.  Otherwise, the 
         * "candidate new mask" is applied as the new "mask_obj" entry.
         * STEP 8 POSIX 1003.6 Draft 12 
         */
	if ((masked_perms & candidate_mask_perms) != 0) {
	    SET_STATUS(&status, dcp_s_acl_would_grant_perms); 
	} else {
	    new_mask_obj_entry.perms = candidate_mask_perms;
	    new_mask_obj_entry.entry_info.entry_type = 
				    sec_acl_e_type_mask_obj;
	    status = dcp__acl_do_add_entries(&new_mask_obj_entry, 
	    				     NORMAL_MOD, 
					     dcp_acl_info_p,
					     &ignore_perms);
	    if (BAD_STATUS(&status))
	        SET_STATUS(&status, dcp_s_acl_new_mask_obj_failed); 
	} /* End of apply new mask_obj entry */
    } /* End of POSIX 1003.6 STEPS 7 & 8 */


    /*
     * If -mask calc was specified:
     * POSIX 1003.6 Steps 3,4,5,7, and 8 are NOT followed but
     * the "mask_obj" will be calculated with the union of all
     * entries except user_obj, other_obj, and unauthenticated. 
     * 
     * Note that if the ACL manager does not support mask_obj type 
     * then the "-mask calc" switch is meaningless. 
     */
    if (GOOD_STATUS(&status)){  
	if (DCP_C_ACL_MGR.mask_calc_type == CALC_MASK_OBJ_AFTER)
	    sec_acl_calc_mask(&DCP_C_ACL_MGR.sec_acl_list,
		              &status);
    } 

    /*
     * Clean-up and return
     */
    dcp__acl_free_specified_entries(remove_entry_argv,
    			            remove_acl_entries,
				    change_entry_argv,
    			            change_acl_entries,
				    add_entry_argv,
    			            add_acl_entries);

    DCP_CHECK_SEC_ERROR(status); 
    return(TCL_OK);

} /* End of dcp_acl_do_modify_entries() */


/*
 * FUNCTION:
 * dcp_acl_do_replace_entries()
 *
 * OVERVIEW:
 * This routine replaces all ACL entries for the specified object with
 * the specified ACL entries. 
 *
 * INPUTS:
 * replace_entries_tcllist:
 *          A pointer to a list of ACL entries specified to be replaced.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->num_entries:
 *              Number of ACL entries for the specified object.
 *           sec_acl_p->sec_acl_entries:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_replace_entries(char *replace_entries_tcllist,
			  Tcl_Interp *interp,
		          dcp_acl_info_p_t dcp_acl_info_p)
{
    int replace_entry_argc;
    char **replace_entry_argv = (char **)NULL;
    sec_acl_entry_t *replace_acl_entries = (sec_acl_entry_t *)NULL;
    int i, j;
    error_status_t status;

    /* 
     * Perform initialization
     */
    CLEAR_STATUS(&status);

    /*
     * Cycle through all ACL entries specified to be replaced
     */
     if (dcp__acl_list_entries(interp, dcp_acl_info_p, TRUE,
	  replace_entries_tcllist,
          &replace_entry_argc, &replace_entry_argv) != TCL_OK){
          return(TCL_ERROR);
     }
    replace_acl_entries = (sec_acl_entry_t *)malloc(
                          ((replace_entry_argc) * (sizeof(sec_acl_entry_t))));
    if (replace_acl_entries == NULL){
        free((char *)replace_entry_argv);
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    } /* End of bad status */

    for (i = 0; i < replace_entry_argc; i++){
	status = dcp__acl_validate_entry(dcp_acl_info_p,
					 (char *)replace_entry_argv[i],
					 TRUE,
					 &(replace_acl_entries[i]));
	if (BAD_STATUS(&status)){
	    break;
	} /* End of bad status */

	/*
	 * If a mask_obj ACL entry was explicitly given and the
	 * ACL manager supports the mask_obj entry,
	 * then do not automatically re-calculate the mask.
	 */
	if (replace_acl_entries[i].entry_info.entry_type ==
		      		                 sec_acl_e_type_mask_obj) {
	    DCP_C_ACL_MGR.mask_calc_type = NO_MASK_OBJ_CALC;
	} /* End of mask_obj ACL entry specified */

    } /* End of loop thru entries to replace */

    /* 
     * After validation is complete, check for duplicate
     * entries that may have been specified.
     */
    if (replace_entry_argc > 1) {
        for (i = 0; ((i < replace_entry_argc) && GOOD_STATUS(&status)); i++) {
            for (j = (i + 1); j < replace_entry_argc; j++) {
	        if (dcp__acl_entry_compare(&replace_acl_entries[i],
			                   &replace_acl_entries[j], 
			                   FALSE)) {
	            SET_STATUS(&status, dcp_s_acl_replace_dup_entries);
		    break;
                } /* End of entry comparison */
            } /* End of rest of entry list */
        } /* End of loop thru entries to replace */
    } /* End of check for duplicate entries */

    /*
     * Perform request to replace entries with -acl switch.
     */
    if (GOOD_STATUS(&status)){  
	if (DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) {
	    dcp__acl_free_existing_entry_keys(dcp_acl_info_p); 
            free((void *) DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries);
	}
        DCP_C_ACL_MGR.sec_acl_p->num_entries = replace_entry_argc;
        DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries = replace_acl_entries;
    }

    /*
     * If a mask_obj entry was NOT specified, a mask
     * recalculation will occur (-c).
     * POSIX 1003.6 Steps 3,4,5,7, and 8 are NOT followed but
     * the "mask_obj" will be calculated with the union of all
     * entries except user_obj, other_obj, and unauthenticated. 
     */
    if ((GOOD_STATUS(&status)) &&
	(DCP_C_ACL_MGR.mask_calc_type == CALC_MASK_OBJ_AFTER)) {
	    sec_acl_calc_mask(&DCP_C_ACL_MGR.sec_acl_list,
		              &status);
    } 

    /*
     * Clean-up and return
     */
    free((char *)replace_entry_argv);

    DCP_CHECK_SEC_ERROR(status); 
    return(TCL_OK);

} /* End of dcp_acl_do_replace_entries() */


/*
 * FUNCTION:
 * dcp_acl_do_delete_entries()
 *
 * OVERVIEW:
 * This routine deletes all ACL entries, except the "user_obj" entry
 * if it exists from the specified object. 
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->num_entries:
 *              Number of ACL entries for the specified object.
 *           sec_acl_p->sec_acl_entries:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_delete_entries(Tcl_Interp *interp,
		          dcp_acl_info_p_t dcp_acl_info_p)
{
    int i;
    int new_num_entries = 0;
    sec_acl_entry_t *new_user_obj_entry = (sec_acl_entry_t *)NULL;

    /* 
     * We have existing ACL, now check for no ACL entries
     */
    if ((DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) &&
	(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries == NULL)){
	DCP_SET_MISC_ERROR(sec_acl_no_acl_found);
    }

    /*
     * Check for existence of the "user_obj" ACL entry type
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	if (DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].entry_info.entry_type 					    == sec_acl_e_type_user_obj) { 
	    new_user_obj_entry = (sec_acl_entry_t *)malloc(sizeof(sec_acl_entry_t));
	    if (new_user_obj_entry == NULL){
		DCP_SET_MISC_ERROR(dcp_s_no_memory);
	    }
	    new_user_obj_entry[new_num_entries++] = 
				DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i];
	    break;
	}
    } /* End of loop thru entries to find user_obj entry */

    /*
     * Perform request to delete entries 
     */
    if (DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) {
	dcp__acl_free_existing_entry_keys(dcp_acl_info_p); 
	free((void *) DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries);
    }
    DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries = new_user_obj_entry;
    DCP_C_ACL_MGR.sec_acl_p->num_entries = new_num_entries;

    return(TCL_OK);

} /* End of dcp_acl_do_delete_entries() */


/* 
 * FUNCTION:
 * dcp__acl_validate_entry()
 *
 * OVERVIEW:
 * This routine parses input entries which can have the following 2 forms:
 * type{:key}{:perms}
 * type{ key}{ perms}
 * And builds an ACL entry in the form of sec_acl_entry_t.
 *
 * Note that the ACL type specified by the user can be in lowercase,
 * uppercase, or any combination of lower and upper case.
 * Ex: user:me:r             or    USER:me:r     or   uSer:me:r
 *
 * Also, the permissions specified by the user can include dashes(-).
 * Ex: other_obj:r-------g   or    other_obj:rg
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           default_cell_rgy_handle:
 *                A registry server handle associated with the default
 *                cell.
 *           local_cell_rgy_handle:
 *                A registry server handle associated with the local
 *                cell.
 *           sec_acl_num_printstrings:
 *               Length of permission printstrings array.
 *           sec_acl_printstrings[i].printstring:
 *               A character string describing the printable 
 *               representation of a specified permission. 
 *           sec_acl_printstrings[i].permissions:
 *               Permissions that the ACL manager will suppport.
 *
 * input_acl_entry:
 *          Input ACL entry specified by the user which can have the 
 *          following 2 forms:
 * 	    type{:key}{:perms}
 * 	    type{ key}{ perms}
 *
 * permset_filled:
 * 	    Enables validation and translation of the permissions. 
 *          It is set to FALSE for the -remove switch, which needs
 * 	    to do a lookup by type{:key} or type{ key} only.  It is set 
 *          to TRUE whenever a full sec_acl_entry is necessary.
 *
 * INPUT/OUTPUTS:
 * sec_acl_entry_p:
 *           A pointer to a specified ACL entry for modification.
 *           UPDATED:
 *           entry_info.entry_type:
 *              ACL type for this ACL entry.
 *           entry_info.tagged_union:
 *              If applicable, Names and UUIDs for the key with 
 *              either type sec_id_t or sec_id_foreign_t or
 *              sec_acl_extend_info_t
 *           perms:
 *              If applicable, permissions granted for this ACL entry.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t
dcp__acl_validate_entry(dcp_acl_info_p_t dcp_acl_info_p,
			char *input_acl_entry,
			boolean32 permset_filled,
		        sec_acl_entry_t *sec_acl_entry_p)
{
    char *input_entry_minus_type;
    char *input_acl_key_name;
    int i;
    boolean32 found;
    error_status_t status;

    /* 
     * Initialization
     */
    CLEAR_STATUS(&status);
    sec_acl_entry_p->perms = 0;

    /*
     * Parse the input ACL entry
     */
    input_entry_minus_type = strchr(input_acl_entry, ' ');
    if (input_entry_minus_type == NULL){
        input_entry_minus_type = strchr(input_acl_entry, ':');
        if ((input_entry_minus_type == NULL) && permset_filled){
	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	    return(status);
	}
    }

    /*
     * Remove ":" or " " and save rest of acl entry string
     */
    if (input_entry_minus_type != NULL){
        if ((*input_entry_minus_type == ' ') ||
            (*input_entry_minus_type == ':')) {
	    *input_entry_minus_type = '\0';
	    input_entry_minus_type++;
        }
    }

    /*
     * Make sure ACL entry type is in lowercase and
     * validate it.  If valid, update the sec_acl.
     */
    dcp__acl_make_lowercase(input_acl_entry);
    found = FALSE;
    for (i = 0; 
	 (valid_entry_types[i].acl_string_name != NULL) && (! found); 
	 i++){
	if (strcmp(valid_entry_types[i].acl_string_name, 
		   input_acl_entry) == 0) {
	    found = TRUE;
	    sec_acl_entry_p->entry_info.entry_type =  
			valid_entry_types[i].acl_entry_type;
	}
    } /* End of loop thru valid entry type table */
    if (! found) {
	SET_STATUS(&status, dcp_s_acl_inv_entry_type);
	return(status);
    }

    /*  
     * Depending on the type, there may be a KEY expected. 
     */
    switch (sec_acl_entry_p->entry_info.entry_type) {
	/* No key for these */
	case sec_acl_e_type_mask_obj: 
	case sec_acl_e_type_unauthenticated:
	case sec_acl_e_type_user_obj:
	case sec_acl_e_type_other_obj:
	case sec_acl_e_type_group_obj:
	case sec_acl_e_type_any_other:
	case sec_acl_e_type_user_obj_deleg:
	case sec_acl_e_type_other_obj_deleg:
	case sec_acl_e_type_group_obj_deleg:
	case sec_acl_e_type_any_other_deleg:
	    break;
	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:
	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group: 
	case sec_acl_e_type_user_deleg:
	case sec_acl_e_type_group_deleg:
	case sec_acl_e_type_for_other_deleg:
	case sec_acl_e_type_for_user_deleg:
	case sec_acl_e_type_for_group_deleg:
	    input_acl_key_name = input_entry_minus_type;
	    if (input_acl_key_name == NULL){
		SET_STATUS(&status, dcp_s_acl_inv_entry_key);
		return(status);
	    }
	    input_entry_minus_type = strchr(input_acl_key_name, ' ');
	    if (input_entry_minus_type == NULL){
	        input_entry_minus_type = strchr(input_acl_key_name, ':');
	        if ((input_entry_minus_type == NULL) && (permset_filled)){
	    	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	    	    return(status);
		}
	    }
	    if (input_entry_minus_type != NULL){
	        *input_entry_minus_type = '\0';
	        input_entry_minus_type++;
	    }

	    status = dcp__acl_generate_key_uuid(dcp_acl_info_p,
						input_acl_key_name,
						sec_acl_entry_p);
	    if (BAD_STATUS(&status)) {
		SET_STATUS(&status, dcp_s_acl_inv_entry_key);
		return(status);
	    }
	    break;
	case sec_acl_e_type_extended:
	    input_acl_key_name = input_entry_minus_type;
	    input_entry_minus_type = strchr(input_acl_key_name, ' ');
	    if (input_entry_minus_type == NULL){
	        input_entry_minus_type = strchr(input_acl_key_name, ':');
	        if ((input_entry_minus_type == NULL) && (permset_filled)){
	    	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	    	    return(status);
		}
	    }
	    if (input_entry_minus_type != NULL){
	        *input_entry_minus_type = '\0';
	        input_entry_minus_type++;
	    }
	    status = dcp__acl_validate_extended_info(input_acl_key_name, 
						     sec_acl_entry_p);
	    if (BAD_STATUS(&status)) {
		SET_STATUS(&status, dcp_s_acl_inv_entry_key);
		return(status);
	    }
	    break;
	default:
	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	    return(status);
    } /* End of switch on entry type */

    /*
     *  Translate the permission string into a permset_t.
     *  For each permission character entered, compare against
     *  each possible printstring, for a match.  If found, add
     *  the permission bit for that printstring to the permset.
     *
     *  Don't care what's after the perm string: For example,
     *  {user cell_admin rwc effective r-c}
     */
    while((permset_filled) && 
	  (*input_entry_minus_type != '\0') &&
	  (*input_entry_minus_type != ' ')) {     
	found = FALSE;
	for (i = 0; 
	     (i < DCP_C_ACL_MGR.sec_acl_num_printstrings && !found); 
	     i++) {
	    if (*input_entry_minus_type == 
		    *DCP_C_ACL_MGR.sec_acl_printstrings[i].printstring){
	        found = TRUE;
		sec_acl_entry_p->perms = sec_acl_entry_p->perms |
			DCP_C_ACL_MGR.sec_acl_printstrings[i].permissions;
	    }
	} /* End of loop thru printstrings */
	if ((! found) && (*input_entry_minus_type != '-')) {
	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	    return(status);
	}
	input_entry_minus_type++;
    } /* End of loop thru permissions set specified */

    return(status);

} /* End of dcp__acl_validate_entry() routine */


/*
 * FUNCTION:
 * dcp_acl_do_modify_cell()
 *
 * OVERVIEW:
 * This routine modifies the value of the cell attribute of the ACL
 * for the specified object. 
 *
 * INPUTS:
 * new_cell_attribute:
 *           A pointer to the value of the default cell specified by
 *           the user.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->default_realm.name:
 *           default_cell_name:
 *               Full name of default cell.
 *           sec_acl_p->default_realm.uuid:
 *           default_cell_uuid:
 *               UUID of default cell.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_modify_cell(char *new_cell_attribute,
		       Tcl_Interp *interp,
		       dcp_acl_info_p_t dcp_acl_info_p)
{
    error_status_t              status;
    sec_rgy_name_t              full_cell_name; 
    uuid_t                      cell_uuid;
    sec_rgy_handle_t            rgy_handle;
    char **cell_argv;
    int cell_argc;

    /*
     * Determine if the -cell switch is used that there is only
     * one cell name specified.
     */
    if (Tcl_SplitList(interp, new_cell_attribute, 
		      &cell_argc, &cell_argv) != TCL_OK)
	return TCL_ERROR; 
    if (cell_argc > 1) {
	free((char *) cell_argv);
	DCP_SET_MISC_ERROR(dcp_s_acl_one_cell_attr);
    } /* End of -cell specification error */

    CLEAR_STATUS(&status);
    sec_id_parse_name(dcp_acl_info_p->local_cell_rgy_handle, 
		      (unsigned_char_p_t)new_cell_attribute, 
		      full_cell_name,  
    		      &cell_uuid, 
		      NULL, 
		      NULL, 
		      &status);

    if (GOOD_STATUS(&status)) {
        if (uuid_equal(&DCP_C_ACL_MGR.sec_acl_p->default_realm.uuid,
		       &cell_uuid, 
		       &status)){
	    SET_STATUS(&status, dcp_s_acl_inv_cell_name); 
        } 
    } /* End of check if good status and default cell name !changed */

    if (GOOD_STATUS(&status)) {
	if (DCP_C_ACL_MGR.sec_acl_p->default_realm.name) {
	    free((void *)DCP_C_ACL_MGR.sec_acl_p->default_realm.name);  
	}
	/* 
	 * Allocate the storage for, and copy the identifier 
	 */
	DCP_C_ACL_MGR.sec_acl_p->default_realm.name = (unsigned_char_p_t)
	    malloc((strlen((char *)full_cell_name)+1)*sizeof(unsigned_char_t));
	if (DCP_C_ACL_MGR.sec_acl_p->default_realm.name == NULL){
	    free((char *) cell_argv);
	    DCP_SET_MISC_ERROR(dcp_s_no_memory);
	}
	    
	strcpy((char *)DCP_C_ACL_MGR.sec_acl_p->default_realm.name,
	       (char *)full_cell_name);
	DCP_C_ACL_MGR.sec_acl_p->default_realm.uuid = cell_uuid;

	/* 
	 * Store it in the cache data too, for use by other rtns 
	 */
	dcp_acl_info_p->default_cell_uuid = cell_uuid;
	strcpy((char *)dcp_acl_info_p->default_cell_name,
	       (char *)full_cell_name);
	if (dcp_acl_info_p->default_cell_rgy_handle != 
	    		    	    dcp_acl_info_p->local_cell_rgy_handle){
	    sec_rgy_site_close(dcp_acl_info_p->default_cell_rgy_handle, 
			       &status);
	    CLEAR_STATUS(&status);
	}

        if (uuid_equal(&dcp_acl_info_p->default_cell_uuid, 
		       &dcp_acl_info_p->local_cell_uuid, 
		       &status)) {
	    dcp_acl_info_p->default_cell_rgy_handle = 
	        dcp_acl_info_p->local_cell_rgy_handle;
        } /* End of local and default cell are the same */
        else {
	    /* 
	     * Obtain a handle to the default cell's registry 
	     */
	    CLEAR_STATUS(&status);
	    status = dcp__acl_bind_registry(
			        interp, 
	   	   	        (char *)dcp_acl_info_p->default_cell_name,
			        &rgy_handle);
	    if (GOOD_STATUS(&status)){
    	        dcp_acl_info_p->default_cell_rgy_handle = rgy_handle;
	    }
        } /* End of local and default cell are different */
    } /* End of set default cell info */


    free((char *) cell_argv);

    if (BAD_STATUS(&status)){
	free((void *)DCP_C_ACL_MGR.sec_acl_p->default_realm.name);  
	DCP_SET_MISC_ERROR(status);
    }

    return(TCL_OK);

} /* End of dcp_acl_do_modify_cell() */


/*
 * FUNCTION:
 * dcp_acl_do_show_entries()
 *
 * OVERVIEW:
 * This routine returns a list of the ACL entries for the specified
 * object. 
 * ACL entry format:
 *   {TYPE[ KEY] PERMISSIONS}
 * Note that some ACL entries do not have "keys".
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           sec_acl_p->num_entries:
 *              Number of ACL entries for the specified object.
 *           sec_acl_p->sec_acl_entries[i]:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *           sec_acl_entries[i].entry_info.entry_type:
 *              ACL type for this ACL entry.
 *           sec_acl_entries[i].perms:
 *              Permissions granted for this ACL entry.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int
dcp_acl_do_show_entries(Tcl_Interp *interp,
		 dcp_acl_info_p_t dcp_acl_info_p)
{
    int mask_obj_entry_in_list = -1;
    int i;
    error_status_t status;

    /* 
     * Check for no ACL entries
     */
    if ((DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) &&
	(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries == NULL)){
	DCP_SET_MISC_ERROR(sec_acl_no_acl_found);
    }

    /* 
     * Sort the sec_acl_entries by order of importance 
     */
    status = dcp__acl_sort_entries(DCP_C_ACL_MGR.sec_acl_p->num_entries,
			  &DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries);  

    if (BAD_STATUS(&status))
	DCP_SET_MISC_ERROR(status);

    /*
     * Check for existence of the "mask_obj" ACL entry type
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
        if (DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].entry_info.entry_type == sec_acl_e_type_mask_obj){
    	    mask_obj_entry_in_list = i;
	    break;
        }
    } /* End of loop thru ACL entries for mask_obj type */

    /* 
     * Show all ACL entries for the specified object
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
        dcp__acl_print_entries(interp, 
			       dcp_acl_info_p,	 
			       &DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i]);
	/* 
	 * If mask_obj ACL entry exists, display effective perms too.
 	 * The "mask_obj" ACL entry denotes maximum permissions
         * allowed on an entry in the file group class.
         * Note that file group class includes all ACL entry types 
         * except mask_obj, user_obj, other_obj, and unauthenticated.
         * (POSIX 1003.6 Draft 12)
	 */
	if (mask_obj_entry_in_list != -1){
	    if (dcp__acl_is_file_group_class(&DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i])){
	        dcp__acl_print_effective_permset(
			interp,
			dcp_acl_info_p,
			DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms,
			DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[mask_obj_entry_in_list].perms);
	    } /* End of ACL entry is part of file group class */
	} /* End of show effective permissions */

        /* Enclose entire ACL entry and begin on new line */
        Tcl_AppendResult(interp, "}", (char *)NULL);
	if (i != (DCP_C_ACL_MGR.sec_acl_p->num_entries - 1))
            Tcl_AppendResult(interp, "\n", (char *)NULL);
    } /* End of for loop thru all ACL entries */

    return(TCL_OK);

} /* End of dcp_acl_do_show_entries() */


/*
 * FUNCTION:
 * dcp__acl_sort_entries()
 *
 * OVERVIEW:
 * This routine will sort all existing ACL entries in the order
 * of the valid_entry_types[] table for the "acl show" operation.
 * If there is a problem generating the sorted list, then the
 * ACL entry list will not be sorted.
 *
 * INPUTS:
 * num_entries:
 *              Number of ACL entries for the specified object.
 *
 * INPUT/OUTPUTS:
 * entries_p:
 *           A pointer to the list of all ACL entries belonging to the
 *           specified object.
 *           (*entries_p)[j]:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *           (*entries_p)[j].entry_info.entry_type:
 *              ACL type for this ACL entry.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t
dcp__acl_sort_entries(int num_entries, sec_acl_entry_t **entries_p)
{
    int i, j;
    sec_acl_entry_t *s_entries;
    int num_copied = 0;
    error_status_t status;
#ifndef	HPDCE_DCED_LOCAL
    rpc_ss_thread_handle_t thread_handle;
    idl_void_p_t (*alloc_func)(idl_size_t size);
    void (*free_func)(idl_void_p_t ptr);

    /* Find out the current memory allocation and freeing mechanism
       used by the client stubs. */
    TRY
	thread_handle = rpc_ss_get_thread_handle();
    CATCH(pthread_badparam_e);
        thread_handle = NULL;
    ENDTRY;
    if (thread_handle == NULL) {
        alloc_func = (idl_void_p_t (*)(idl_size_t size))malloc;
        free_func = (void (*)(idl_void_p_t ptr))free;
    }
    else {
	rpc_ss_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
				      (void (*)(idl_void_p_t ptr))free,
				      &alloc_func, &free_func);
	rpc_ss_set_client_alloc_free(alloc_func, free_func);
    }
#endif	/* HPDCE_DCED_LOCAL */

    CLEAR_STATUS(&status);

#ifndef	HPDCE_DCED_LOCAL
    s_entries = (*alloc_func)(num_entries * sizeof(sec_acl_entry_t));
#else
    s_entries = 
	   (sec_acl_entry_t *)malloc(num_entries * sizeof(sec_acl_entry_t));
#endif	/* HPDCE_DCED_LOCAL */
    if (s_entries == NULL){
	SET_STATUS(&status, dcp_s_no_memory);
	return(status);
    }

    for (i = 0; i < sec_acl_e_type_max_nbr; i++)
	for (j = 0; j < num_entries; j++)
	    if ((*entries_p)[j].entry_info.entry_type == 
				valid_entry_types[i].acl_entry_type)
		s_entries[num_copied++] = (*entries_p)[j];

    /* 
     * Only return new list if sort was successful 
     */
    if (num_entries == num_copied) {
#ifndef	HPDCE_DCED_LOCAL
        (*free_func)((void *) *entries_p);
#else
        free((void *) *entries_p);
#endif	/* HPDCE_DCED_LOCAL */
	*entries_p = s_entries;
    } 
    else {
#ifndef	HPDCE_DCED_LOCAL
	(*free_func)((void *) s_entries);
#else
        free((void *) s_entries);
#endif	/* HPDCE_DCED_LOCAL */
    }

    return(status);

} /* End of dcp__acl_sort_entries() routine */


/*
 * FUNCTION:
 * dcp__acl_print_entries()
 *
 * OVERVIEW:
 * This routine displays all ACL entries for the specified object.
 * ACL entry format:
 *   {TYPE[ KEY] PERMISSIONS}
 * Note that some ACL entries do not have "keys".
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           Pointer passed to dcp__acl_print_permset() routine.
 *
 * sec_acl_entry_p:
 *           A pointer to one particular ACL entry of the
 *           specified object.
 *           REFERENCED:
 *           entry_info.entry_type:
 *              ACL type for this ACL entry.
 *           perms:
 *              Permissions granted for this ACL entry.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp__acl_print_entries(Tcl_Interp *interp,
		       dcp_acl_info_p_t dcp_acl_info_p,
		       sec_acl_entry_t *sec_acl_entry_p)
{
    char  output_string[BUFSIZ];
    char  *key_name;

    switch(sec_acl_entry_p->entry_info.entry_type) {
	/*
	 * These ACL entry types do not have a key.
	 */
	case sec_acl_e_type_mask_obj:
	    /*
	     * {mask_obj permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[0].acl_string_name);
	    break;
	case sec_acl_e_type_user_obj:
	    /*
	     * {user_obj permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[2].acl_string_name);
	    break;
	case sec_acl_e_type_group_obj:
	    /*
	     * {group_obj permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[5].acl_string_name);
	    break;
	case sec_acl_e_type_other_obj:
	    /*
	     * {other_obj permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[8].acl_string_name);
	    break;
	case sec_acl_e_type_any_other:
	    /*
	     * {any_other permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[10].acl_string_name);
	    break;
	case sec_acl_e_type_user_obj_deleg:
	    /*
	     * {user_obj_delegate permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[11].acl_string_name);
	    break;
	case sec_acl_e_type_group_obj_deleg:
	    /*
	     * {group_obj_delegate permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[14].acl_string_name);
	    break;
	case sec_acl_e_type_other_obj_deleg:
	    /*
	     * {other_obj_delegate permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[17].acl_string_name);
	    break;
	case sec_acl_e_type_any_other_deleg:
	    /*
	     * {any_other_delegate permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[19].acl_string_name);
	    break;
	case sec_acl_e_type_unauthenticated:
	    /*
	     * {unauthenticated permissions}
	     */
	    sprintf(output_string, "{%s ", valid_entry_types[1].acl_string_name);
	    break;

	/*
	 * These ACL entry types have a key of the form:
	 *     sec_id_t 
	 */
	case sec_acl_e_type_user:
	    /*
	     * {user principal_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[3].acl_string_name, key_name);
	    break;
	case sec_acl_e_type_user_deleg:
	    /*
	     * {user_delegate principal_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[12].acl_string_name, key_name);
	    break;
        case sec_acl_e_type_group:
	    /*
	     * {group group_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[6].acl_string_name, key_name);
	    break;
        case sec_acl_e_type_group_deleg:
	    /*
	     * {group_delegate group_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[15].acl_string_name, key_name);
	    break;
	case sec_acl_e_type_foreign_other:
	    /*
	     * {foreign_other cell_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[9].acl_string_name, key_name);
	    break;
	case sec_acl_e_type_for_other_deleg:
	    /*
	     * {foreign_other_delegate cell_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[18].acl_string_name, key_name);
	    break;

	/*
	 * These ACL entry types have a key of the form:
	 *     sec_id_foreign_t 
	 */
	case sec_acl_e_type_foreign_user:
	    /*
	     * {foreign_user cell_name/principal_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[4].acl_string_name, key_name);
	    break;
	case sec_acl_e_type_for_user_deleg:
	    /*
	     * {foreign_user_delegate cell_name/principal_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[13].acl_string_name, key_name);
	    break;
	case sec_acl_e_type_foreign_group:
	    /*
	     * {foreign_group cell_name/group_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[7].acl_string_name, key_name);
	    break;
	case sec_acl_e_type_for_group_deleg:
	    /*
	     * {foreign_group_delegate cell_name/group_name permissions}
	     */
	    key_name = dcp__acl_generate_key_name(dcp_acl_info_p, sec_acl_entry_p);
	    sprintf(output_string, "{%s %s ", valid_entry_types[16].acl_string_name, key_name);
	    break;

	/*
	 * These ACL entry types have a key of the form:
	 *     sec_acl_extend_info_t 
	 */
	case sec_acl_e_type_extended:
	    /*
	     * {extended uuid.ndr.ndr.ndr.ndr.number_of_bytes.data permissions}
	     */
	    key_name = dcp__acl_generate_extended_key(interp,
	     	      sec_acl_entry_p->entry_info.tagged_union.extended_info);
	    sprintf(output_string, "{%s %s ", valid_entry_types[20].acl_string_name, key_name);
	    break;
	default:
	    sprintf(output_string,
		    "{%s ",
		    "<ILLEGAL ENTRY>");
	    break;

    }

    /* 
     * Show the ACL entry type and key if applicable
     */
    Tcl_AppendResult(interp, output_string, (char *)NULL);

    /* 
     * Show the ACL entry permissions
     */
     dcp__acl_print_permset(interp, 
			    dcp_acl_info_p,
     			    sec_acl_entry_p->perms);

}


/*
 * FUNCTION:
 * dcp__acl_print_permset()
 *
 * OVERVIEW:
 * This routine displays the permission set for an ACL entry.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           sec_acl_num_printstrings:
 *               Length of permission printstrings array.
 *           sec_acl_printstrings[i].printstring:
 *               A character string describing the printable 
 *               representation of a specified permission. 
 *           sec_acl_printstrings[i].permissions:
 *               Permissions that the ACL manager will suppport.
 *
 * granted_permset:
 *           ACL permission set for the particular ACL entry passed
 *           in.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp__acl_print_permset(Tcl_Interp *interp,
		       dcp_acl_info_p_t dcp_acl_info_p,
		       sec_acl_permset_t granted_permset)
{
    int i;
    char output_string[BUFSIZ];

    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_num_printstrings; i++){
	if (DCP_C_ACL_MGR.sec_acl_printstrings[i].permissions & granted_permset){
	    sprintf(output_string,
		    "%s",
		    DCP_C_ACL_MGR.sec_acl_printstrings[i].printstring);
	} else {
	    sprintf(output_string, "%s", "-");
	}
        Tcl_AppendResult(interp, output_string, (char *)NULL);

    } /* End of for loop thru acl printstrings */

} /* End of dcp__acl_print_permset() */


/*
 * FUNCTION:
 * dcp__acl_print_effective_permset()
 *
 * OVERVIEW:
 * This routine displays the effective permission set for an ACL 
 * entry if the mask_obj type exists for the ACL.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           sec_acl_num_printstrings:
 *               Length of permission printstrings array.
 *           sec_acl_printstrings[i].printstring:
 *               A character string describing the printable 
 *               representation of a specified permission. 
 *           sec_acl_printstrings[i].permissions:
 *               Permissions that the ACL manager will suppport.
 * granted_permset:
 *           ACL permission set for the ACL entry.
 * mask_obj_permset:
 *           The "mask_obj" ACL entry denotes maximum permissions
 *           allowed on an entry in the file group class.
 *           Note that file group class includes all ACL entry types 
 *           except mask_obj, user_obj, other_obj, and unauthenticated.
 *           (POSIX 1003.6 Draft 12)
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp__acl_print_effective_permset(Tcl_Interp *interp,
		       		 dcp_acl_info_p_t dcp_acl_info_p,
		       		 sec_acl_permset_t granted_permset,
		       		 sec_acl_permset_t mask_obj_permset)
{
    int                i;
    sec_acl_permset_t  effective_permset;
    char               *effective_svc;
    char               output_string[BUFSIZ];

    effective_permset = granted_permset & mask_obj_permset;

    /*
     * If the permission set of the ACL entry changes for "effective"
     * permissions, then it is displayed.
     */
    if (granted_permset != effective_permset) {
        effective_svc = (char *)dce_sprintf(dcp_t_rgy_effective);
        Tcl_AppendResult(interp, " ", effective_svc, " ", NULL);
        free(effective_svc);
        for (i = 0; i < DCP_C_ACL_MGR.sec_acl_num_printstrings; i++) {
            if (DCP_C_ACL_MGR.sec_acl_printstrings[i].permissions & effective_permset) {
                sprintf(output_string,
                        "%s",
                        DCP_C_ACL_MGR.sec_acl_printstrings[i].printstring);
            }
            else {
                sprintf(output_string, "%s", "-");
            }
            Tcl_AppendResult(interp, output_string, (char *)NULL);
        }
    }
}


/*
 * FUNCTION:
 * dcp__acl_generate_extended_key()
 *
 * OVERVIEW:
 * This routine builds the ACL entry "extended" key with type
 * sec_acl_extend_info_t for display.
 * Format:
 *    uuid.ndr.ndr.ndr.ndr.number_of_bytes.data
 *
 * INPUTS:
 * extend_info_p: 
 *           Contains information related to the ACL entry type
 *           "extended" key.
 *           REFERENCED:
 *           extension_type:
 *               The UUID of the extension type. 
 *	     format_label:
 *		 The format of the label, in ndr_format_t form. 
 *	     num_bytes:
 *		 An unsigned 32-bit integer indicating the number of 
 *               bytes containing the ``pickled'' data. 
 *           pickled_data:
 *		The byte array containing the ``pickled'' data. 
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * key_name: Formatted key for the ACL entry type "extended".
 *
 */
char *
dcp__acl_generate_extended_key(Tcl_Interp *interp,
		       sec_acl_extend_info_t *extend_info_p)
{
    error_status_t       	status;
    unsigned_char_p_t           name1;
    int                         i;
    char                        key_name[BUFSIZ];
    char                        fbyte[BUFSIZ];
    char                        pickled_data[BUFSIZ] = "";

    /*
     * Obtain the UUID string
     */
    CLEAR_STATUS(&status);
    uuid_to_string(&extend_info_p->extension_type,
		   &name1,
		   &status);

    /* 
     * Show the pickled data (array of bytes) 
     */
    for (i = 0; i < extend_info_p->num_bytes; i++){
	sprintf(fbyte, "%.2x", extend_info_p->pickled_data[i]);
	strcat(pickled_data, fbyte);
    }

    sprintf(key_name,
	    "%s.%x.%x.%x.%x.%d.%s", 
    	    (char *)name1,             /* extended UUID string */
	    extend_info_p->format_label.int_rep,   /* NDR info */
	    extend_info_p->format_label.char_rep,  /* NDR info */
	    extend_info_p->format_label.float_rep, /* NDR info */
	    extend_info_p->format_label.reserved,  /* NDR info */
    	    extend_info_p->num_bytes,    /* Nbr bytes in entry */
    	    pickled_data);            /* extended pickled_data */

    /*
     * Release allocated memory 
     */
    rpc_string_free(&name1, &status); 

    return(key_name);

} /* End of dcp__acl_generate_extended_key() */


/*
 * FUNCTION:
 * dcp__acl_validate_extended_info()
 *
 * OVERVIEW:
 * This routine parses input extended ACL entry key which can have the 
 * following format:
 *    uuid.ndr.ndr.ndr.ndr.number_of_bytes.data
 * And builds the extended ACL entry key in the form of sec_acl_extend_info_t.
 *
 * INPUTS:
 * string:
 *          Input extended ACL entry key which can have the following format:
 *              uuid.ndr.ndr.ndr.ndr.number_of_bytes.data
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * ep: 
 *           Contains information related to the ACL entry type
 *           "extended" key.
 *           UPDATED:
 *           extension_type:
 *               The UUID of the extension type. 
 *	     format_label:
 *		 The format of the label, in ndr_format_t form. 
 *	     num_bytes:
 *		 An unsigned 32-bit integer indicating the number of 
 *               bytes containing the ``pickled'' data. 
 *           pickled_data:
 *		The byte array containing the ``pickled'' data. 
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t 
dcp__acl_validate_extended_info(char *string, sec_acl_entry_t *ep)
{
    error_status_t      status;
    int                 i, num_bytes, tmp_val, f1, f2, f3, f4;
    char                *str1, *str2;   /* to scan the string with */
    uuid_t              uuid;
    ndr_format_t        tmp_format;

    CLEAR_STATUS(&status);

    /*
     * The empty string is not a valid key.
     */
    if ((string == NULL) || (*string == '\0')){
	SET_STATUS(&status, dcp_s_acl_inv_entry_key);
	return(status);
    }

    /* 
     * Find the end of the extension_type uuid 
     */
    str1 = string;
    str2 = strchr(string, '.');

    /* 
     * If no '.' was found, the string cannot be a extended entry 
     */
    if (str2 == NULL)
	SET_STATUS(&status, dcp_s_acl_inv_entry);
    else {
	if (*str2 == '.') {         /* Get rid of '.'               */
	    *str2 = '\0';           /* Terminate the entry_type str */
	    str2++;                 /* save the rest of the string  */
	}
        uuid_from_string((unsigned_char_p_t) str1, &uuid, &status);
    }

    /* 
     * If above was successful, parse the format label 
     * Min possible is 9 chars if number_of_bytes = 0 
     */
    if (GOOD_STATUS(&status)) {
        if (strlen(str2) < 9) {
	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	} else {
    	    /* 
	     * Advance str1 to end of ndr.ndr.ndr.ndr 
	     * str2 = ndr.ndr.ndr.ndr.number_of_bytes.data
	     */
	    str1 = str2;
	    for (i = 1; ((str1 != NULL) && (*str1 != '\0') && (i <= 4)); i++) {
	        str1 = strchr(str1, '.');
	        if ((str1 != NULL) && (i != 4)) 
	            str1++; /* Stop at the 4th '.' */
	    }
        }
    } /* End of GOOD_STATUS */

    if ((GOOD_STATUS(&status)) && (str1 != NULL)) {
	*str1 = '\0'; 
	str1++;           /* Terminate format label string    */
	/* 
	 * Scan the format label values and make sure 4 were found 
	 */
	if (sscanf(str2, "%x.%x.%x.%x", &f1, &f2, &f3, &f4) != 4) {
	    SET_STATUS(&status, dcp_s_acl_inv_entry);
	} else {
	    tmp_format.int_rep   = (ndr_byte) f1;
	    tmp_format.char_rep  = (ndr_byte) f2;
	    tmp_format.float_rep = (ndr_byte) f3;
	    tmp_format.reserved  = (ndr_byte) f4;
	}
    }

    /* 
     * If above was successful, get the number of bytes expected 
     */
    if (GOOD_STATUS(&status)) {
	str2 = strchr(str1, '.');
	if (str2 != NULL)
	    if (*str2 == '.') {         /* Get rid of '.' to */
		*str2 = '\0';           /* terminate the num_bytes str  */
		str2++;
	    }
	if (sscanf(str1, "%d", &num_bytes) != 1)
	    SET_STATUS(&status, dcp_s_acl_inv_entry);
    }

    /* 
     * If above was successful fill the struct & attempt to scan the bytes 
     */
    if (GOOD_STATUS(&status)) {
	/* 
	 * First, allocate the structure 
	 */
	ep->entry_info.tagged_union.extended_info = (sec_acl_extend_info_t *)
	   malloc(sizeof(uuid_t) + sizeof(ndr_format_t) + sizeof(unsigned32) + 
				   (num_bytes * sizeof(ndr_byte)));
	if (ep->entry_info.tagged_union.extended_info == NULL){
	    SET_STATUS(&status, dcp_s_no_memory);
	}
	if (GOOD_STATUS(&status)) {
	    ep->entry_info.tagged_union.extended_info->extension_type = uuid;
	    ep->entry_info.tagged_union.extended_info->format_label = 
								  tmp_format;
	    ep->entry_info.tagged_union.extended_info->num_bytes = num_bytes;

	    /* 
	     * Make sure there's something there to scan 
	     */
	    if ((str2 == NULL) && (num_bytes != 0))
	        SET_STATUS(&status, dcp_s_acl_inv_entry);
	    else if (strlen((char *) str2) != (2 * num_bytes))
	            SET_STATUS(&status, dcp_s_acl_inv_entry);
	} /* End of GOOD_STATUS */

	/* 
	 * Now attempt to scan the bytes 
	 */
	for (i = 0;
	     (i < num_bytes) && GOOD_STATUS(&status) && (str2 != NULL);
	     i++) {
	    if (1 > sscanf(str2, "%2x", &tmp_val)){
	        SET_STATUS(&status, dcp_s_acl_inv_entry);
	    } else {
		ep->entry_info.tagged_union.extended_info->pickled_data[i] = 
					   (ndr_byte) tmp_val;
		str2 = str2 + 2; 
	    } 
	} /* End of loop thru bytes of data */
    } /* End of GOOD_STATUS */

    if (BAD_STATUS(&status)){
	if (ep->entry_info.tagged_union.extended_info != NULL)
	    free((void *)ep->entry_info.tagged_union.extended_info);
    }

    return(status);

} /* End of dcp__acl_validate_extended_info() routine */


/*
 * FUNCTION:
 * dcp__acl_do_remove_entries()
 *
 * OVERVIEW:
 * This routine removes one ACL entry from the current ACL entry
 * list of the specified object.
 *
 * INPUTS:
 * sec_acl_entry_p:
 *           A pointer to a specified ACL entry for modification.
 *           REFERENCED:
 *           sec_acl_entry_p:
 *           Passed to dcp__acl_entry_compare(). 
 *
 * INPUT/OUTPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->num_entries:
 *           Number of entries for the ACL
 *           sec_acl_p->sec_acl_entries[i]:
 *           One particular ACL entry that will be removed.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t
dcp__acl_do_remove_entries(sec_acl_entry_t *sec_acl_entry_p,
			   dcp_acl_info_p_t dcp_acl_info_p)
{
    int i, j;
    boolean32       entry_found = FALSE;
    error_status_t  status;

    CLEAR_STATUS(&status);

    /* 
     * We have existing ACL, now check for no ACL entries
     */
    if ((DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) &&
	(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries == NULL)){
	SET_STATUS(&status, sec_acl_no_acl_found);
	return(status);
    }

    /* 
     * If the specified entry is found, remove it 
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	if (dcp__acl_entry_compare(
			     &(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i]),
			     sec_acl_entry_p, 
			     FALSE)) {
	    entry_found = TRUE;

            /* Shift any remaining entries down the list */
	    for (j = i; j < DCP_C_ACL_MGR.sec_acl_p->num_entries-1; j++)
	       DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[j] = 
		    DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[j+1];
	    DCP_C_ACL_MGR.sec_acl_p->num_entries--;
	} /* End of process entry found */
    } /* End of loop thru sec entries */

    if (! entry_found){
	SET_STATUS(&status, dcp_s_acl_entry_not_found);
    }

    return(status);

} /* End of dcp__acl_do_remove_entries() */


/*
 * FUNCTION:
 * dcp__acl_do_add_entries()
 *
 * OVERVIEW:
 * If the modification type is INCREASE_MOD, this routine adds one ACL 
 * entry to the current ACL entry list of the specified object and 
 * returns the newly granted permissions for the entry in the file
 * group class.
 *
 * If the modification type is NORMAL_MOD, this routine changes one ACL 
 * entry in the current ACL entry list of the specified object if it
 * exists. If the entry doesn't exist, it will be added. On
 * return, the "perms_changed_p" is ignored.
 *
 * INPUTS:
 * sec_acl_entry_p:
 *           A pointer to a specified ACL entry for modification.
 *           REFERENCED:
 *           sec_acl_entry_p:
 *           New entry to add or change to current ACL entry list
 * mod_type:
 *           Valid modification types for this routine are:
 *           NORMAL_MOD
 *           INCREASE_MOD
 *
 * INPUT/OUTPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->sec_acl_entries[sec_acl_p->num_entries]:
 *           One particular ACL entry that will be added or changed.
 *
 * OUTPUTS:
 * perms_changed_p:
 *           Permissions that are added due to performing ACL
 *           additions or ACL changes that may increase permissions.
 *           This is only associated with ACL entries in the file
 *           group class only.
 *           For modification types NORMAL_MOD and DECREASE_MOD,
 *           this field is ignored.
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t
dcp__acl_do_add_entries(sec_acl_entry_t *sec_acl_entry_p,
			dcp_e_acl_mod_edit_type_t mod_type,
			dcp_acl_info_p_t dcp_acl_info_p,
			sec_acl_permset_t *perms_changed_p)
{
    int i;
    error_status_t  status;
    sec_acl_entry_t *new_sec_acl_entries;
    boolean32 entry_found = FALSE;

    CLEAR_STATUS(&status);

    /*
     * It is this function's responsibility to always
     * initialize perms_changed_p to zero at the beginning.
     */
    *perms_changed_p = (sec_acl_permset_t) 0;

    /* 
     * If a specified entry exists and the modification type is
     * NORMAL_MOD, change it.
     * Otherwise if the specified entry does not exist and the 
     * modification type is INCREASE_MOD, add this entry to the 
     * existing ones.
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	if (dcp__acl_entry_compare(
			     &(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i]),
			     sec_acl_entry_p, 
			     FALSE)) {
	    entry_found = TRUE;
    	    if (mod_type == NORMAL_MOD){
		DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i] = 
					    *sec_acl_entry_p;
            } /* End of change mask_obj entry */
        } /* End of entry comparison */
    } /* End of loop thru sec entries */

    /*
     * The "mask_obj" entry exists and was modified. 
     * Note that the perms_changed_p is ignored on return.
     */
    if ((entry_found) && (mod_type == NORMAL_MOD)){
	return(status);
    }

    /*
     * The specified ACL entry to be added already exists. 
     */
    if ((entry_found) && (mod_type == INCREASE_MOD)){
	SET_STATUS(&status, dcp_s_acl_entry_already_exists);
	return(status);
    } 
    
    /* 
     * Allocate the space for 1 more entry 
     */
    new_sec_acl_entries = (sec_acl_entry_t *)malloc(
       (DCP_C_ACL_MGR.sec_acl_p->num_entries + 1) * sizeof(sec_acl_entry_t));
    if (new_sec_acl_entries == NULL){
	SET_STATUS(&status, dcp_s_no_memory);
	return(status);
    }

    /* 
     * Copy the existing entries 
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	new_sec_acl_entries[i] =
			DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i];
    }
    /*
     * Discard the old, existing entries
     */
    if (DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) 
	free((void *) DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries);

    /* 
     * Add the new entry 
     */
    new_sec_acl_entries[DCP_C_ACL_MGR.sec_acl_p->num_entries] = 
			*sec_acl_entry_p;
    DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries = new_sec_acl_entries;
    DCP_C_ACL_MGR.sec_acl_p->num_entries++;

    /*
     * Return the "newly" granted permissions for the entry in
     * the file group class.
     */
    if (dcp__acl_is_file_group_class(sec_acl_entry_p)) {
        *perms_changed_p = sec_acl_entry_p->perms;
    }

    return(status);

} /* End of dcp__acl_do_add_entries() routine */


/*
 * FUNCTION:
 * dcp__acl_do_change_entries()
 *
 * OVERVIEW:
 * If the modification type is INCREASE_MOD, this routine changes one ACL 
 * entry in the current ACL entry list of the specified object and 
 * returns the newly granted permissions for the entry in the file
 * group class.
 *
 * If the modification type is DECREASE_MOD, this routine changes one ACL 
 * entry in the current ACL entry list of the specified object. On
 * return, the "perms_changed_p" is ignored.
 *
 * INPUTS:
 * sec_acl_entry_p:
 *           A pointer to a specified ACL entry for modification.
 *           REFERENCED:
 *           sec_acl_entry_p:
 *           Passed to dcp__acl_entry_compare(). 
 * mod_type:
 *           Valid modification types for this routine are:
 *           INCREASE_MOD
 *           DECREASE_MOD
 *
 * INPUT/OUTPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           UPDATED:
 *           sec_acl_p->sec_acl_entries[sec_acl_p->num_entries]:
 *           One particular ACL entry that will be changed.
 *
 * OUTPUTS:
 * perms_changed_p:
 *           Permissions that are added due to performing ACL
 *           additions or ACL changes that may increase permissions.
 *           This is only associated with ACL entries in the file
 *           group class only.
 *           For modification types NORMAL_MOD and DECREASE_MOD,
 *           this field is ignored.
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t
dcp__acl_do_change_entries(sec_acl_entry_t *sec_acl_entry_p,
			    dcp_e_acl_mod_edit_type_t mod_type,
			    dcp_acl_info_p_t dcp_acl_info_p,
			    sec_acl_permset_t *perms_changed_p)
{
    int i, j;
    error_status_t  status;
    boolean32 entry_found = FALSE;

    CLEAR_STATUS(&status);

    /*
     * It is this function's responsibility to always
     * initialize perms_changed_p to 0 zero the beginning.
     */
    *perms_changed_p = (sec_acl_permset_t) 0;

    /* 
     * We have existing ACL, now check for no ACL entries
     */
    if ((DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) &&
	(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries == NULL)){
	SET_STATUS(&status, sec_acl_no_acl_found);
	return(status);
    }

    /* 
     * If a specified entry exists, change it.
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	if (dcp__acl_entry_compare(
			     &(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i]),
			     sec_acl_entry_p, 
			     FALSE)) {
	    entry_found = TRUE;
	    if (mod_type == INCREASE_MOD){
		/* 
	         * Add permissions. Keep track of which new
	         * ones were granted for the entry in the file
		 * group class only.
	         */
		for (j = 0; 
		     j < DCP_C_ACL_MGR.sec_acl_num_printstrings; 
		     j++) {
		    if ((! FLAG_SET(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms, DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions)) &&
			    (FLAG_SET(sec_acl_entry_p->perms, 
			DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions))){

			SET(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms, DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions);
			if (dcp__acl_is_file_group_class(&DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i])){
			    SET(*perms_changed_p, DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions);
		        } /* End of update granted perms */
		    } /* End of chg permset in entry: increase it*/
		 } /* End of loop thru printstrings */
	    } /* End of INCREASE_MOD processing */
	    else if (mod_type == DECREASE_MOD){
		    /* 
		     * Decrease entry's permissions and ignore the
		     * perms_changed_p on return.
		     */
		    for (j = 0; 
			 j < DCP_C_ACL_MGR.sec_acl_num_printstrings; 
			 j++) {
		        if ((FLAG_SET(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms, DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions)) &&
			    (! FLAG_SET(sec_acl_entry_p->perms, 
			DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions))){
			    UNSET(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms, DCP_C_ACL_MGR.sec_acl_printstrings[j].permissions);
			} /* End of chg permset in entry: decrease it*/
		    } /* End of loop thru printstrings */
	    } /* End of DECREASE_MOD processing */
	} /* End of process entry found */
    } /* End of loop thru sec entries */


    /*
     * The specified ACL entry to be changed does not exist. 
     */
    if (! (entry_found)){
	SET_STATUS(&status, dcp_s_acl_entry_not_found);
    } 

    return(status);

} /* End of dcp__acl_do_change_entries() routine */


/*
 * FUNCTION:
 * dcp__acl_calc_union_permissions()
 *
 * OVERVIEW:
 * This routine returns a permissions set that reflects the 
 * union of all "actual" or "effective" permissions of the remaining
 * entries for the file group class of the specified object.
 * If no "mask_obj" entry exists in the current ACL entry list and
 * the union of "effective" permissions was requested, then the 
 * "perms_p" field will be set to 0.
 *
 * INPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           sec_acl_p->num_entries:
 *              Number of ACL entries for the specified object.
 *           sec_acl_p->sec_acl_entries[i]:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *           sec_acl_entries[i].entry_info.entry_type:
 *              ACL type for this ACL entry.
 *           sec_acl_entries[i].perms:
 *              Permissions granted for this ACL entry.
 *
 * masked:
 *           A flag to indicate whether to calculate the union
 *           of all "actual" or "effective" permissions.
 *           If TRUE, calculate "effective" permissions.
 *           If FALSE, calculate "actual" permissions.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * perms_p:
 * 	    Permissions set of the union of all "actual" or "effective" 
 *          permissions of the remaining entries for the file group class 
 *          of the specified object.
 *
 * RETURNS:
 * None
 */
void
dcp__acl_calc_union_permissions(dcp_acl_info_p_t dcp_acl_info_p,
				boolean32 masked,
				sec_acl_permset_t *perms_p)
{
    int mask_obj_entry_in_list = -1;
    int i;

    *perms_p = (sec_acl_permset_t) 0;

    /*
     * Check for existence of the "mask_obj" ACL entry type
     */
    if (masked){
        for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
            if (DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].entry_info.entry_type == sec_acl_e_type_mask_obj){
    	        mask_obj_entry_in_list = i;
		break;
            }
         } /* End of loop thru ACL entries for mask_obj type */
    } /* End of if masked is true */

    /* 
     * Calculate the union of all permissions of the remaining 
     * entries for the file group class of the specified object.
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	if (dcp__acl_is_file_group_class(&DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i])){
	    if (masked && (mask_obj_entry_in_list != -1)) {   
		*perms_p = *perms_p |
		     (DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms &
   		      DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[mask_obj_entry_in_list].perms);
	    } /* End of calc union of effective perms */
	    else {                        
		*perms_p = *perms_p | 
		      DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms;
	    } /* End of calc union of actual permissions */
	} /* End of ACL entry is part of file group class */

    } /* End of for loop thru all ACL entries */

} /* End of dcp__acl_calc_union_permissions() */


/*
 * FUNCTION:
 * dcp__acl_do_purge_ineffective()
 *
 * OVERVIEW:
 * This routine will purge the ineffective permissions if
 * the ACL entry is part of the file group class.
 * If no "mask_obj" entry exists in the current ACL entry list,
 * the -purge switch is ignored.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * dcp_acl_info_p:
 *           A pointer to all ACL information related to the
 *           specified object.
 *           REFERENCED:
 *           sec_acl_p->num_entries:
 *              Number of ACL entries for the specified object.
 *           sec_acl_p->sec_acl_entries[i]:
 *              Entire ACL entry containing the ACL type, ACL
 *              key if applicable, and ACL permissions.
 *           sec_acl_entries[i].entry_info.entry_type:
 *              ACL type for this ACL entry.
 *           UPDATED:
 *           sec_acl_entries[i].perms:
 *              Permissions granted for this ACL entry.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
error_status_t
dcp__acl_do_purge_ineffective(dcp_acl_info_p_t dcp_acl_info_p)
{
    error_status_t status;
    int mask_obj_entry_in_list = -1;
    int i;

    CLEAR_STATUS(&status);

    /* 
     * We have existing ACL, now check for no ACL entries
     */
    if ((DCP_C_ACL_MGR.sec_acl_p->num_entries > 0) &&
	(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries == NULL)){
	SET_STATUS(&status, sec_acl_no_acl_found);
	return(status);
    }

    /*
     * Check for existence of the "mask_obj" ACL entry type
     */
    for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
        if (DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].entry_info.entry_type == sec_acl_e_type_mask_obj){
            mask_obj_entry_in_list = i;
	    break;
        }
     } /* End of loop thru ACL entries for mask_obj type */


    if (mask_obj_entry_in_list != -1){
        for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	    if (dcp__acl_is_file_group_class(&DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i])){
	        DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i].perms &= DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[mask_obj_entry_in_list].perms;
	    } /* End of ACL entry is part of file group class */

        } /* End of loop thru all ACL entries */
    } /* End of purge ineffective permissions */

    return(status);

} /* End of dcp__acl_do_purge_ineffective() */


/*
 * FUNCTION:
 * dcp__acl_entry_compare()
 *
 * OVERVIEW:
 * This routine compares the ACL entry specified by the user for 
 * modification to an ACL entry in the current ACL entry list for
 * the specified object.
 *
 * INPUTS:
 * e1:
 *           A pointer to one particular ACL entry in the current
 *           ACL entry list of the specified object.
 *           REFERENCED:
 *           entry_info.entry_type:
 *               ACL type of this ACL entry.
 *           entry_info.tagged_union.id.uuid:
 *               UUID of the principal or group with type sec_id_t.
 *           entry_info.tagged_union.foreign_id.id.uuid:
 *               UUID of the principal or group with type sec_foreign_id_t.
 * e2:
 *           A pointer to a specified ACL entry for modification.
 *           REFERENCED:
 *           entry_info.tagged_union.id.uuid:
 *               UUID of the principal or group with type sec_id_t.
 *           entry_info.tagged_union.foreign_id.id.uuid:
 *               UUID of the principal or group with type sec_foreign_id_t.
 *
 * permset_filled:
 * 	    Enables comparison of the permissions. 
 *          If set to FALSE, compare only the entry_type and
 * 	    the id (if the type has a corresponding id key).  
 *          When set to TRUE, then also compare the permsets.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TRUE if the ACL entries are equal
 * FALSE if the ACL entries are NOT equal
 */
boolean32
dcp__acl_entry_compare(sec_acl_entry_t *e1,
		       sec_acl_entry_t *e2,
		       boolean32 permset_filled)
{
    boolean32 entry_equal = FALSE;
    error_status_t status;

    /*
     * Compare ACL entry types
     */
    if (e1->entry_info.entry_type == e2->entry_info.entry_type)
	entry_equal = TRUE;

    /*
     * Compare ACL entry keys if applicable
     */
    if (entry_equal){
        switch (e1->entry_info.entry_type){
	    case sec_acl_e_type_user:
	    case sec_acl_e_type_group:
	    case sec_acl_e_type_foreign_other:
	    case sec_acl_e_type_user_deleg:
	    case sec_acl_e_type_group_deleg:
	    case sec_acl_e_type_for_other_deleg:
		if (! uuid_equal(&e1->entry_info.tagged_union.id.uuid,
		                 &e2->entry_info.tagged_union.id.uuid, 
				 &status))
		    entry_equal = FALSE;
	        break;
	    case sec_acl_e_type_foreign_user:
	    case sec_acl_e_type_foreign_group:
	    case sec_acl_e_type_for_user_deleg:
	    case sec_acl_e_type_for_group_deleg:
	      if (! uuid_equal(&e1->entry_info.tagged_union.foreign_id.id.uuid,
		               &e2->entry_info.tagged_union.foreign_id.id.uuid, 			      &status))
		    entry_equal = FALSE;
	        break;
	    case sec_acl_e_type_extended:
	    /* extended has a key but does not need to be compared */
	        break;
	    /*
	     * These ACL entries have no keys
	     */
	    case sec_acl_e_type_mask_obj:
	    case sec_acl_e_type_user_obj:
	    case sec_acl_e_type_group_obj:
	    case sec_acl_e_type_user_obj_deleg:
	    case sec_acl_e_type_group_obj_deleg:
	    case sec_acl_e_type_other_obj:
	    case sec_acl_e_type_other_obj_deleg:
	    case sec_acl_e_type_unauthenticated:
	    case sec_acl_e_type_any_other:
	    case sec_acl_e_type_any_other_deleg:
	    default:
	        break;
        } /* End of switch */
    } /* End of compare on ACL keys */

    /*
     * Compare ACL permsets 
     */
    if (entry_equal && permset_filled) {
	if (e1->perms != e2->perms) {
	    entry_equal = FALSE;
	}
    } /* End of compare on ACL permsets */

    return(entry_equal);

} /* End of dcp__acl_entry_compare() */


/*
 * FUNCTION:
 * dcp__acl_is_file_group_class()
 *
 * OVERVIEW:
 * This routine determines if the ACL type is a part of the
 * file group class as defined by POSIX 1003.6 Draft 12.
 *
 * The "mask_obj" ACL entry denotes maximum permissions allowed on an
 * entry in the file group class.
 * Note that file group class includes all ACL entry types except
 * mask_obj, user_obj, other_obj, and unauthenticated.
 *
 * INPUTS:
 * sec_acl_entry_p:
 *           A pointer to one particular ACL entry of the
 *           specified object.
 *           REFERENCED:
 *           entry_info.entry_type:
 *           ACL type of this ACL entry.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TRUE if the ACL entry is a part of the file group class.
 * FALSE if the ACL entry is NOT a part of the file group class.
 */
boolean32
dcp__acl_is_file_group_class(sec_acl_entry_t *sec_acl_entry_p)
{
    switch (sec_acl_entry_p->entry_info.entry_type){
	case sec_acl_e_type_user_obj:
	case sec_acl_e_type_unauthenticated:
	case sec_acl_e_type_mask_obj:
	case sec_acl_e_type_other_obj:
	case sec_acl_e_type_user_obj_deleg:
	case sec_acl_e_type_other_obj_deleg:
	    return(FALSE);
	case sec_acl_e_type_group_obj:
	case sec_acl_e_type_any_other:
	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:
	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:
	case sec_acl_e_type_group_obj_deleg:
	case sec_acl_e_type_any_other_deleg:
	case sec_acl_e_type_user_deleg:
	case sec_acl_e_type_group_deleg:
	case sec_acl_e_type_for_other_deleg:
	case sec_acl_e_type_for_user_deleg:
	case sec_acl_e_type_for_group_deleg:
	case sec_acl_e_type_extended:
	default:
	    return(TRUE);
    } /* End of switch on ACL entry type */

} /* End of dcp__acl_is_file_group_class() */


/*
 * FUNCTION:
 * dcp__acl_is_2_key_entry()
 *
 * OVERVIEW:
 * This routine determines if the ACL entry type is "foreign_user" 
 * or "foreign_group" to indicate that the key has 2 names
 * associated with it. Either cell_name/principal_name or
 * cell_name/group_name respectively.
 *
 * INPUTS:
 * sec_acl_entry_p:
 *           A pointer to one particular ACL entry of the
 *           specified object.
 *           REFERENCED:
 *           entry_info.entry_type:
 *           ACL type of this ACL entry.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TRUE if the ACL entry has a 2 name key.
 * FALSE if the ACL entry does NOT have a 2 name key.
 */
boolean32
dcp__acl_is_2_key_entry(sec_acl_entry_t *sec_acl_entry_p)
{
    switch (sec_acl_entry_p->entry_info.entry_type){
	case sec_acl_e_type_user_obj:
	case sec_acl_e_type_unauthenticated:
	case sec_acl_e_type_mask_obj:
	case sec_acl_e_type_other_obj:
	case sec_acl_e_type_group_obj:
	case sec_acl_e_type_any_other:
	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:
	case sec_acl_e_type_extended:
	case sec_acl_e_type_user_obj_deleg:
	case sec_acl_e_type_other_obj_deleg:
	case sec_acl_e_type_for_other_deleg:
	case sec_acl_e_type_group_obj_deleg:
	case sec_acl_e_type_any_other_deleg:
	case sec_acl_e_type_user_deleg:
	case sec_acl_e_type_group_deleg:
	    return(FALSE);
	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:
	case sec_acl_e_type_for_user_deleg:
	case sec_acl_e_type_for_group_deleg:
	default:
	    return(TRUE);
    } /* End of switch on ACL entry type */

} /* End of dcp__acl_is_2_key_entry() */


/*
 * FUNCTION:
 * dcp__acl_free_existing_entry_keys()
 *
 * OVERVIEW:
 * This routine frees allocated memory for ACL entry keys.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * dcp_acl_info_p:
 *           A pointer to a specified ACL entry for modification.
 *           REFERENCED:
 *           sec_acl_entry_p->entry_info.entry_type:
 *               ACL type for the ACL entry.
 *           sec_acl_p->num_entries:
 *               Number of sec_acl entries.
 *           sec_acl_p->sec_acl_entries[i]:
 *               Entire ACL entry
 *           UPDATED:
 *           sec_acl_entry_p->entry_info.tagged_union.id.name:
 *               Name of the principal or group with type sec_id_t.
 *           sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.name:
 *               Name of the cell with type sec_foreign_id_t.
 *           sec_acl_entry_p->entry_info.tagged_union.extended_info:
 *               Extended info with type sec_acl_extend_info_t.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp__acl_free_existing_entry_keys(dcp_acl_info_p_t dcp_acl_info_p)
{
    int i;
    sec_acl_entry_t *entry_p;

    if ((DCP_C_ACL_MGR.sec_acl_p != NULL) && 
	(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries != NULL)) {
	for (i = 0; i < DCP_C_ACL_MGR.sec_acl_p->num_entries; i++) {
	    entry_p = &(DCP_C_ACL_MGR.sec_acl_p->sec_acl_entries[i]);
	    if (entry_p) {
		switch (entry_p->entry_info.entry_type)  {
		    case sec_acl_e_type_user:
		    case sec_acl_e_type_group:
		    case sec_acl_e_type_foreign_other:
		    case sec_acl_e_type_user_deleg:
		    case sec_acl_e_type_group_deleg:
		    case sec_acl_e_type_for_other_deleg:
		      if (entry_p->entry_info.tagged_union.id.name)
			 free((void *)entry_p->entry_info.tagged_union.id.name);
		       break;
		    case sec_acl_e_type_foreign_user:
		    case sec_acl_e_type_foreign_group:
		    case sec_acl_e_type_for_user_deleg:
		    case sec_acl_e_type_for_group_deleg:
		      if (entry_p->entry_info.tagged_union.foreign_id.id.name)
			 free((void *)entry_p->entry_info.tagged_union.foreign_id.id.name);
	       	      break;
		    case sec_acl_e_type_extended:
		      if (entry_p->entry_info.tagged_union.extended_info)
			 free((void *)entry_p->entry_info.tagged_union.extended_info);
		      break;
		    default:
		      break;
		} /* End of switch on entry type */
	    } /* End of if (entry_p) */
	} /* End of loop thru entries */
    } /* if we have an acl and entries */

} /* End of dcp__acl_free_existing_entry_keys() routine */


/*
 * FUNCTION:
 * dcp__acl_free_specified_entries()
 *
 * OVERVIEW:
 * This routine frees allocated memory used during the manipulation 
 * of lists of entries to modify.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * remove_entry_argv:
 *	    A pointer to a list of ACL entries specified by the user
 *          to be removed. 
 * remove_acl_entries:
 *	    A pointer to a list of ACL entries to remove in
 *          sec_acl_entry_t format. 
 * change_entry_argv:
 *	    A pointer to a list of ACL entries specified by the user
 *          to be changed. 
 * change_acl_entries:
 *	    A pointer to a list of ACL entries to change in
 *          sec_acl_entry_t format. 
 * add_entry_argv:
 *	    A pointer to a list of ACL entries specified by the user
 *          to be added. 
 * add_acl_entries:
 *	    A pointer to a list of ACL entries to add in
 *          sec_acl_entry_t format. 
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void
dcp__acl_free_specified_entries(char **remove_entry_argv,
    			        sec_acl_entry_t *remove_acl_entries,
				char **change_entry_argv,
    			        sec_acl_entry_t *change_acl_entries,
				char **add_entry_argv,
    			        sec_acl_entry_t *add_acl_entries)
{
    /*
     * Free list of entries specified to be removed
     */
    if (remove_entry_argv != NULL){
        free((char *) remove_entry_argv);
	remove_entry_argv = NULL;
    }

    if (remove_acl_entries != NULL){
        free((void *)remove_acl_entries);
	remove_acl_entries = NULL;
    }

    /*
     * Free list of entries specified to be changed
     */
    if (change_entry_argv != NULL){
        free((char *) change_entry_argv);
	change_entry_argv = NULL;
    }
    if (change_acl_entries != NULL){
        free((void *)change_acl_entries);
	change_acl_entries = NULL;
    }

    /*
     * Free list of entries specified to be added
     */
    if (add_entry_argv != NULL){
        free((char *) add_entry_argv);
	add_entry_argv = NULL;
    }
    if (add_acl_entries != NULL){
        free((void *)add_acl_entries);
	add_acl_entries = NULL;
    }

} /* End of dcp__acl_free_specified_entries() routine */

    
void
dcp__acl_make_lowercase(char *buffer)
{

    while (*buffer != '\0') {
	if (isupper(*buffer)) {
	    *buffer = tolower(*buffer);
	}
	buffer++;
    }

} /* End of dcp__acl_make_lowercase() routine */


/*
 * FUNCTION:
 * dcp__acl_list_entries()
 *
 * OVERVIEW:
 * This routine will ensure that list of entries are split up
 * properly.
 * Lists of ACL entries can look like the following:
 * -r {{user a_prin} {any_other} group:a_group unauthenticated}
 * -r {any_other}
 * -r {any_other}
 * -a {{user a_prin r} {any_other r} group:a_group:r unauthenticated:r}
 * -ch {user a_prin rw} 
 *
 * INPUTS:
 * dcp_acl_info_p:   Pointer to all ACL info related to the specified
 *                   object.
 *
 * permset_filled:
 * 	    Enables validation and translation of the permissions. 
 *          It is set to FALSE for the -remove switch, which needs
 * 	    to do a lookup by type{:key} or type{ key} only.  It is set 
 *          to TRUE whenever a full sec_acl_entry is necessary.
 *
 * entries_tcllist:  Pointer to string with list structure.
 *
 * INPUT/OUTPUTS:
 * interp:      Interpreter to use for error reporting.  
 * entry_argc:  Pointer to location to fill in with the number of 
 *              elements in the list. 
 * entry_argv:  Pointer to store pointer to array of pointers 
 *              to list elements. 
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK
 * TCL_ERROR
 */
int
dcp__acl_list_entries(Tcl_Interp *interp, 
		     dcp_acl_info_p_t dcp_acl_info_p,
		     boolean32 permset_filled,
		     char *entries_tcllist, 
		     int  *entry_argc, 
		     char ***entry_argv)
{
    static sec_acl_entry_t temp_acl_entry;
    char *input_acl_entry;
    error_status_t status;
    int rc = TCL_OK;

    if (Tcl_SplitList(interp, entries_tcllist, 
			  entry_argc, entry_argv) != TCL_OK){
	return(TCL_ERROR);    
    }

    /*
     * Determine if there is 1 ACL entry in TCL syntax.
     */
    input_acl_entry = dce_strdup((char *)*entry_argv[0]);
    status = dcp__acl_validate_entry(dcp_acl_info_p,
				     input_acl_entry,
				     permset_filled,
				     &temp_acl_entry);
    if (BAD_STATUS(&status)){
	free((char *) *entry_argv); 
	rc = dcp_general_allocate_argv_for_one(interp,
					       (char *)entries_tcllist,
					       entry_argc,
					       entry_argv);
    } /* End of only 1 ACL entry specified in TCL syntax */

    return(rc);

} /* End of dcp__acl_list_entries() routine */


/*
 * FUNCTION:
 * dcp_acl_list_objects()
 *
 * OVERVIEW:
 * This routine will ensure that list of ACL objects are split up
 * properly. 
 *
 * Possible string binding inputs:
 *
 * In string syntax:
 *	   {<$OBJECT_UUID>@<$PROTSEQ>:<$NETADDR> <RESIDUAL_NAME>}
 *	   {<$OBJECT_UUID>@<$PROTSEQ>:<$NETADDR>\[<$ENDPOINT>\] <RESIDUAL_NAME>}
 * In TCL syntax:
 *	   {<$OBJECT_UUID> <$PROTSEQ> <$NETADDR> <RESIDUAL_NAME>}
 *         {<$OBJECT_UUID> <$PROTSEQ> <$NETADDR> <$ENDPOINT> <RESIDUAL_NAME>}
 *
 * This routine must also handle 1 or more ACL object names or bindings, like:
 * {/.:/sec/principal/jas}
 * {/.:/hosts /.:/sec/principal/jas}
 * {{<$OBJECT_UUID>@<$PROTSEQ>:<$NETADDR> <RESIDUAL_NAME>} 
 *  {<$OBJECT_UUID>@<$PROTSEQ>:<$NETADDR> <RESIDUAL_NAME>}}
 *
 * And mixes of names and string bindings like:
 * {/.:/hosts {<$OBJECT_UUID>@<$PROTSEQ>:<$NETADDR> <RESIDUAL_NAME>}}
 *
 * INPUTS:
 * name_tcllist:
 *           Pointer to string with list structure.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * name_argc:
 *           Pointer to location to fill in with the number of
 *           elements in the list.
 *
 * name_argv:
 *           Pointer to store pointer to array of pointers
 *           to list elements.
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int
dcp_acl_list_objects(Tcl_Interp *interp,
		     char *name_tcllist,
		     int *name_argc,
                     char ***name_argv)
{
    int                         rc = TCL_OK;
    error_status_t              status;
    error_status_t              st1;
    char                        *sbind;
    rpc_binding_handle_t        server_handle;

    CLEAR_STATUS(&status);
    CLEAR_STATUS(&st1);

    /*
     * Determine if there is 1 string binding specification
     */
    if (Tcl_SplitList(interp, name_tcllist,
				  name_argc, name_argv) != TCL_OK){
        return(TCL_ERROR);
    }

    /*
     * Remove residual from string binding 
     *
     * If 1 string binding specification, allocate argv for one ACL 
     * object with both binding and residual
     */
    (*name_argc)--;  

    if (*name_argc == 1) {
	rpc_binding_from_string_binding((unsigned_char_p_t)*name_argv[0],
					&server_handle, 
					&status);
        if (GOOD_STATUS(&status)) {
	    free((char *) *name_argv);
	    rc = dcp_general_allocate_argv_for_one(interp,
						   name_tcllist,
						   name_argc,
						   name_argv);
        } 
    } /* End of determine if 1 string binding in string syntax */
    else if (*name_argc > 1) {
	sbind = dcp_binding_tcl_to_string(*name_argc, *name_argv, &status);
	if (sbind != NULL) {
	    rpc_binding_from_string_binding((unsigned_char_p_t)sbind,
					    &server_handle, 
					    &status);
            if (GOOD_STATUS(&status)) {
		free((char *) *name_argv);
		rc = dcp_general_allocate_argv_for_one(interp,
						       name_tcllist,
						       name_argc,
						       name_argv);
            } /* End of only 1 binding specified in TCL syntax */
	    rpc_string_free((unsigned char **)&sbind, &st1);
	} /* End of process potential string binding in TCL syntax */
    } /* End of determine if 1 string binding in TCL syntax */

    if ((*name_argc == 0) || (BAD_STATUS(&status))) {
        (*name_argc)++;  
    } /* End of 1 ACL object name or no sting binding specification */

    return(rc);

} /* End of dcp_acl_list_objects() routine */
