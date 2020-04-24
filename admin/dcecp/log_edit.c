/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: log_edit.c,v $
 * Revision 1.1.4.2  1996/02/18  19:13:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:21:31  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/07  22:37 UTC  dat
 * 	Fix error caused by incompatible types in comparison caught by AIX compiler
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/22  20:38 UTC  truitt
 * 	Merge log list work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts45/1  1995/08/22  20:37 UTC  truitt
 * 	CHFts16101: Format log list output correctly.
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/22  14:28 UTC  jrr
 * 	Merge fix for CHFts15310.
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts15310/1  1995/05/19  20:08 UTC  jrr
 * 	Fix core dump in bind_named_object().
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/29  16:48 UTC  jrr
 * 	Remove extra {} from log show output (CR11767).
 * 	[1995/12/08  17:56:07  root]
 * 
 * Revision 1.1.2.6  1994/09/08  14:00:44  salamone
 * 	CR11968-Setting authentication on binding handles.
 * 	[1994/09/08  13:59:16  salamone]
 * 
 * Revision 1.1.2.5  1994/08/26  18:31:41  kevins
 * 	OT 11888 replace obsolete constants
 * 	[1994/08/26  18:22:30  kevins]
 * 
 * Revision 1.1.2.4  1994/08/19  18:04:13  salamone
 * 	CR10683 - Place newline bewteen objects where applicable.
 * 	[1994/08/19  17:52:46  salamone]
 * 
 * Revision 1.1.2.3  1994/08/18  21:39:21  salamone
 * 	CR10460 - Handle unnecessary "where" field.
 * 	CR10865 - Handle modifying and displaying routing specifications
 * 		  with multiple routes in TCL and string syntax.
 * 	[1994/08/18  21:38:04  salamone]
 * 
 * Revision 1.1.2.2  1994/05/18  20:41:03  salamone
 * 	CR10636 - "log show" and "log list" should display the appropriate
 * 		   info if unauthenticated.
 * 	[1994/05/13  21:33:13  salamone]
 * 
 * Revision 1.1.2.1  1994/04/26  15:32:35  salamone
 * 	Free argv properly.
 * 	[1994/04/26  15:31:37  salamone]
 * 
 * 	Initial version of log_edit.c
 * 	[1994/04/25  15:18:31  salamone]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * log_edit.c
 *
 * DESCRIPTION:
 * This module contains the functions to modify, show, or list 
 * the current state of routing for DCE serviceability messages
 * of a process.
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
#include <dce/sec_login.h>
#include <dce/binding.h>
#include <dce/service.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dce_error.h>

#include <dce_svc.h>
#include <dce/tcl.h>
#include <dcecp.h>
#include <log_edit.h>
#include <util_binding.h>
#include <util_general.h>
#include <util_login.h>

/*
 * Current global login context. This is defined in util_login.h
 */
extern dcp_login_context_p_t global_context_list;

/*
 * INTERNAL VARIABLES 
 */


/*
 * PROTOTYPES 
 */
int dcp__log_list_routings(Tcl_Interp *, dcp_log_info_p_t, char *, int *, char ***);
error_status_t dcp__log_validate_routing(dcp_log_info_p_t, char *, Tcl_Interp *, boolean32 *, char *);
int dcp__log_routing_tcl_to_string(char *, boolean32 *, Tcl_Interp *, char *); 
int dcp__log_str_to_tcl_debug_flags(char *, Tcl_Interp *, char *); 
void dcp__log_process_ignored_rfields(char *, Tcl_Interp *, char *);
void dcp__log_str_to_tcl_routing(char *, char *);
int dcp__log_str_to_tcl_multi_routing(boolean32, char *, Tcl_Interp *, char *);


/*
 * FUNCTION:
 * dcp_log_bind_named_object()
 *
 * OVERVIEW:
 * This routine resolves a partially bound server binding handle
 * into a fully bound server binding handle. And then sets authenication
 * and authorization information with the server.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     named_object: 
 *              The name of the target object. 
 *           UPDATED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
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
dcp_log_bind_named_object(Tcl_Interp *interp,
			  dcp_log_info_p_t dcp_log_info_p)
{
    error_status_t  	    	status;
    error_status_t  	    	temp_status;
    rpc_ns_handle_t             context;
    boolean32                   string_binding_flag = FALSE;
    char                        **largv;
    int                         largc;
    char                        *sbind;

    CLEAR_STATUS(&status);
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_default, 
				(unsigned_char_t *)dcp_log_info_p->named_object,
		                NULL, 
				NULL, 
				&context, 
				&status);

    if (BAD_STATUS(&status)) {
        CLEAR_STATUS(&status);
	string_binding_flag = TRUE;
	if (!strchr(dcp_log_info_p->named_object, ' ')) {
	    /* we have a single string syntax binding */
	    rpc_binding_from_string_binding(
			       (unsigned_char_t *)dcp_log_info_p->named_object, 
				&dcp_log_info_p->server_handle, 
				&status);
	    DCP_CHECK_RPC_ERROR(status);
        } else {
	    /* we have a single TCL syntax binding */
	    if (Tcl_SplitList(interp, dcp_log_info_p->named_object, 
			  &largc, &largv) != TCL_OK)
		return(TCL_ERROR);
	    sbind = dcp_binding_tcl_to_string(largc, largv, &status);
	    rpc_binding_from_string_binding(
			       (unsigned_char_t *)sbind, 
				&dcp_log_info_p->server_handle, 
				&status);
	    rpc_string_free((unsigned char **)&sbind, &temp_status);
	    free((char *) largv);
	    DCP_CHECK_RPC_ERROR(status);
	} /* End of get binding handle */
	rpc_binding_set_object(dcp_log_info_p->server_handle, 
			       NULL, 
			       &status);
	DCP_CHECK_RPC_ERROR(status);
    } /* End of if (BAD_STATUS) */

    for ( ;string_binding_flag == FALSE; ) {
	rpc_ns_binding_import_next(context, 
				   &dcp_log_info_p->server_handle, 
				   &status);
        if (BAD_STATUS(&status)) {
	    rpc_ns_binding_import_done(&context, &temp_status);
            DCP_CHECK_RPC_ERROR(status);
        }

	rpc_ep_resolve_binding(dcp_log_info_p->server_handle, 
			       serviceability_v1_0_c_ifspec, 
			       &status);
	if (GOOD_STATUS(&status)) {
	    rpc_ns_binding_import_done(&context, &status);
	    break;
	}
	/* 
	 * Free binding handle resource NOT used as a fully
	 * bound server handle.
	 */
	 rpc_binding_free(&dcp_log_info_p->server_handle,
			  &status);

    } /* End of loop until fully bound server handle is obtained */

    /* 
     * Determine the authentication level to use
     */
    if (GOOD_STATUS(&status)) {
        if (dcp_is_logged_in()) {
            rpc_binding_set_auth_info(dcp_log_info_p->server_handle,
                                      NULL, 
                                      rpc_c_protect_level_default, 
                                      rpc_c_authn_default,
                                      (rpc_auth_identity_handle_t)global_context_list->context,
                                      rpc_c_authz_dce,
                                      &status);
	}  /* End of authenticated */
	else {
            rpc_binding_set_auth_info(dcp_log_info_p->server_handle,
                                      NULL, 
                                      rpc_c_protect_level_none, 
                                      rpc_c_authn_none,
                                      (rpc_auth_identity_handle_t)NULL,
                                      rpc_c_authz_none,
                                      &status);
	} /* End of unauthenticated */
    } /* End of GOOD_STATUS */

    DCP_CHECK_RPC_ERROR(status);
    return(TCL_OK);

} /* End of dcp_log_bind_named_object() */


/*
 * FUNCTION:
 * dcp_log_do_list_comp()
 *
 * OVERVIEW:
 * This routine returns a list of serviceability components 
 * registered by the server.
 *
 * INPUTS:
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
 *
 * OUTPUTS:
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
dcp_log_do_list_comp(dcp_log_info_p_t dcp_log_info_p,
                     Tcl_Interp *interp)
{
    error_status_t         status;
    int                    i;
    char                   output_string[BUFSIZ];
    dce_svc_stringarray_t  components;

    CLEAR_STATUS(&status);
    dce_svc_inq_components(dcp_log_info_p->server_handle,
                           &components,
                           &status);
    if (BAD_STATUS(&status)) {
        DCP_SET_MISC_ERROR(status);
    }

    for (i = 0; i < components.tab_size; i++) {
        sprintf(output_string, "%s", components.table[i]);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
        if (i != (components.tab_size - 1))
            Tcl_AppendResult(interp, " ", (char *)NULL);
    }

    return(TCL_OK);
}


/*
 * FUNCTION:
 * dcp_log_do_list_subcomp()
 *
 * OVERVIEW:
 * This routine returns a list of sub-components of the specified 
 * component name(s), where each element of the list is a list of
 * 3 elements: the sub-component, the level, and the long description
 * of the sub-component.
 *
 * INPUTS:
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
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
dcp_log_do_list_subcomp(char *component_name_list,
                        dcp_log_info_p_t dcp_log_info_p,
                        Tcl_Interp *interp)
{
    error_status_t          status;
    int                     i;
    int                     j;
    char                    output_string[BUFSIZ];
    dce_svc_subcomparray_t  table;
    dce_svc_subcomp_t       *tp;
    char                    **comp_argv;
    int                     comp_argc;

    CLEAR_STATUS(&status);

    if (Tcl_SplitList(interp, component_name_list, &comp_argc, &comp_argv) != TCL_OK)
        return TCL_ERROR;

    /*
     * Cycle through all components specified.
     */
    for (i = 0; i < comp_argc; i++) {
        dce_svc_inq_table(dcp_log_info_p->server_handle,
                          (dce_svc_string_t)comp_argv[i],
                          &table,
                          &status);

        if (BAD_STATUS(&status)) {
            free((char *)comp_argv);
            DCP_SET_MISC_ERROR(status);
        }

        for (j = 0, tp = table.table; j < table.tab_size; j++, tp++) {
            if (strlen((char *)tp->sc_name) > 16) {
                sprintf(output_string, "{%s %-2ld {%s}",
                                       tp->sc_name,
                                       (long)tp->sc_level,
                                       tp->sc_descr);
            } else {
                sprintf(output_string, "{%-16s %-2ld {%s}",
                                       tp->sc_name,
                                       (long)tp->sc_level,
                                       tp->sc_descr);
            }
            Tcl_AppendResult(interp, output_string, (char *)NULL);

            /*
             * Enclose entire subcomponent and begin on new line.
             */
            Tcl_AppendResult(interp, "}", (char *)NULL);
            if (j != (table.tab_size - 1))
                Tcl_AppendResult(interp, "\n", (char *)NULL);
        }

        if (i != (comp_argc - 1))
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
    }

    free((char *)comp_argv);
    return(TCL_OK);
}


/*
 * FUNCTION:
 * dcp_log_do_show_svc_routings()
 *
 * OVERVIEW:
 * This routine returns a list describing the current serviceability
 * routing settings for a server. If -debug option specified, the 
 * debug routing settings for a server are returned.
 *
 * INPUTS:
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
 *
 * debug_values:
 *           The debug_values is a boolean that if false says "give me 
 *           the production routings" if true says "give me the debug 
 *           routings".
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
dcp_log_do_show_svc_routings(boolean32 debug_values,
                             dcp_log_info_p_t dcp_log_info_p,
                             Tcl_Interp *interp)
{
    error_status_t  	    	status;
    int                         i;
    char 			output_string[BUFSIZ] = "";
    static char 		tcl_debug_routing[BUFSIZ] = "";
    static char 		str_routing[BUFSIZ] = "";
    static char 		tcl_single_routing[BUFSIZ] = "";
    static char 		tcl_routing[BUFSIZ] = "";
    char 			*tcl_debug_routing_p;
    char 			*str_routing_p;
    char 			*tcl_single_routing_p;
    char 			*tcl_routing_p;
    dce_svc_stringarray_t       routings;
    int                         rc = 0;

    CLEAR_STATUS(&status);
    str_routing_p = (char *)&str_routing;
    tcl_routing_p = (char *)&tcl_routing;
    tcl_single_routing_p = (char *)&tcl_single_routing;
    tcl_debug_routing_p = (char *)&tcl_debug_routing;

#if !defined(DCE_DEBUG)
    if (debug_values) {
	SET_STATUS(&status, dcp_s_log_no_debug);
	DCP_SET_MISC_ERROR(status);
    }
#endif /* if !defined(DCE_DEBUG) */

    dce_svc_inq_routings(dcp_log_info_p->server_handle,
			 debug_values, 
			 &routings,
			 &status);

    if (BAD_STATUS(&status)) {
	DCP_SET_MISC_ERROR(status);
    }

    /*
     * Process each routing specification
     */
    for (i = 0; i < routings.tab_size; i++) {

        /*
         * Need to find ignored fields and display "{}".
         */
	dcp__log_process_ignored_rfields((char *)routings.table[i],
					 interp,
					 str_routing_p);
        /*
         * Routing is still in string syntax, need to replace
         * colons with spaces.
         */
	dcp__log_str_to_tcl_routing(str_routing_p,
				    tcl_single_routing_p);

        /*
         * A routing specification may have multiple routes.
         */
	dcp__log_str_to_tcl_multi_routing(debug_values,
					  tcl_single_routing_p,
					  interp,
				          tcl_routing_p);

        /*
         * If debug routing specification, need to
         * place "flags" field in a TCL list.
         */
	if (! debug_values) {
	    sprintf(output_string, "{%s}", tcl_routing_p);
	} else {
	    rc = dcp__log_str_to_tcl_debug_flags(tcl_routing_p,
					         interp,
					         tcl_debug_routing_p);
	    if (rc != TCL_OK)
	        return(rc);
	    sprintf(output_string, "{%s}", tcl_debug_routing_p);
	}

	Tcl_AppendResult(interp, output_string, (char *)NULL);

	if (i != (routings.tab_size - 1))
	    Tcl_AppendResult(interp, "\n", (char *)NULL);

    } /* End of loop thru all routings */

    return(TCL_OK);

} /* End of dcp_log_do_show_svc_routings() routine */


/*
 * FUNCTION:
 * dcp_log_do_modify_routings()
 *
 * OVERVIEW:
 * This routine changes the routing specifications of the specified
 * servers. Note that the SVC API expects the routing specification
 * in string syntax.
 *
 * INPUTS:
 * change_routings_tcllist:
 *           A list of serviceability and/or debug routing
 *           specifications in TCL and/or string syntax.
 *
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
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
dcp_log_do_modify_routings(char *change_routings_tcllist,
                           dcp_log_info_p_t dcp_log_info_p,
                           Tcl_Interp *interp)
{
    int                         i;
    int                         change_routing_argc;
    char                        **change_routing_argv = (char **)NULL;
    static char                 string_routing[BUFSIZ] = "";
    char                        *string_routing_p;
    error_status_t  	    	status;
    boolean32                   debug_flag;

    /*
     * Initialization
     */
    CLEAR_STATUS(&status);
    string_routing_p = (char *)&string_routing;

    /*
     * Split list of routings properly.
     */
    if (dcp__log_list_routings(interp, 
			       dcp_log_info_p,
		               change_routings_tcllist,
		               &change_routing_argc, 
			       &change_routing_argv) != TCL_OK){
            free((char *) change_routing_argv); 
	    return(TCL_ERROR);
    }

    /*
     * Validate all LOG routings specified to be modified
     */
    for (i = 0; (i < change_routing_argc && (GOOD_STATUS(&status))); i++){
	debug_flag = FALSE;
	status = dcp__log_validate_routing(dcp_log_info_p,
					   (char *)change_routing_argv[i],
					   interp,
					   &debug_flag,
					   string_routing_p);
        if (BAD_STATUS(&status)) {
            free((char *) change_routing_argv); 
	    DCP_SET_MISC_ERROR(status);
        }

	if (debug_flag) {
#if !defined(DCE_DEBUG)
	    SET_STATUS(&status, dcp_s_log_inv_routing); 
#endif /* if !defined(DCE_DEBUG) */
            if (GOOD_STATUS(&status)){
    	        dce_svc_set_dbg_route(dcp_log_info_p->server_handle,
			              (idl_byte *)string_routing_p,
			              &status);
	    }
	} else {
    	    dce_svc_set_route(dcp_log_info_p->server_handle,
			      (idl_byte *)string_routing_p,
			      &status);
	}
    } /* End of loop thru all routings */

    /*
     * Clean-up and return
     */
     free((char *) change_routing_argv);

    if (BAD_STATUS(&status)) {
	DCP_SET_MISC_ERROR(status);
    }

    return(TCL_OK);

} /* End of dcp_log_do_modify_routings() routine */


/*
 * FUNCTION:
 * dcp__log_list_routings()
 *
 * OVERVIEW:
 * This routine will ensure that list of routings are split up
 * properly.
 *
 * INPUTS:
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
 *
 * routings_tcllist:  
 *           Pointer to string with list structure.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * routing_argc:  
 *           Pointer to location to fill in with the number of 
 *           elements in the list. 
 *
 * routing_argv:  
 *           Pointer to store pointer to array of pointers 
 *           to list elements. 
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK
 * TCL_ERROR
 */
int
dcp__log_list_routings(Tcl_Interp *interp, 
		     dcp_log_info_p_t dcp_acl_info_p,
		     char *routings_tcllist, 
		     int  *routing_argc, 
		     char ***routing_argv)
{
    char **temp_argv = (char **)NULL; 
    char *routing_ptr;

    if (Tcl_SplitList(interp, routings_tcllist, 
			  routing_argc, routing_argv) != TCL_OK){
	return(TCL_ERROR);    
    }

    /*
     * Determine if there is 1 LOG routing in TCL syntax.
     */
    routing_ptr = strchr((char *)*routing_argv[0], ' ');
    if (routing_ptr == NULL){
	routing_ptr = strchr((char *)*routing_argv[0], DCP_C_LOG_SEP_CHAR);
	if (routing_ptr == NULL) {
	    free((char *) *routing_argv);  
	    temp_argv = (char **)malloc(2 * (sizeof(char *)));
	    if (temp_argv == NULL){
	        DCP_SET_MISC_ERROR(dcp_s_no_memory);
	    }
            temp_argv[0] = routings_tcllist;
            temp_argv[1] = NULL;
            *routing_argc = 1;
            *routing_argv = temp_argv;
	}
    } /* End of only 1 LOG routing specified in TCL syntax */

    return TCL_OK;

} /* End of dcp__log_list_routings() routine */


/*
 * FUNCTION:
 * dcp__log_validate_routing()
 *
 * OVERVIEW:
 * This routine parses input routings which can have the following 4 forms:
 * And builds the applicable routing in string syntax.
 *
 * For svc routing format:
 * severity:how:where{:private}
 * severity how where{ private}
 *
 * For dbg routing format:
 * component:flags:how:where{:private}
 * component flags how where{ private}
 *
 * For dbg routing flags format:
 * sub-component.level
 * {sub-component level}
 * sub-component.level,sub-component.level
 * {{sub-component level} {sub-component level}}
 *
 * For routings with multiple routes format:
 * how:where:private;how:where;how:
 * {{how where private} {how where} {how}}
 *
 * INPUTS:
 * dcp_log_info_p:
 *           A pointer to all LOG information related to the
 *           specified object.
 *           REFERENCED:
 * 	     handle:
 *               A handle_t pointer to receive the fully bound server
 *               binding handle.  The other log routines use this handle 
 *               to refer to the named_object.
 *
 * routing:
 *           One serviceability or debug routing specification in 
 *           TCL or string syntax.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * debug_flag:
 *           Indicates whether the routing is a serviceability or
 *           debug routing specification.
 *
 * string_routing:
 *           One serviceability or debug routing specification in 
 *           string syntax to give to SVC API.
 *
 * RETURNS:
 * status:   A pointer to the completion status. On successful completion, 
 *	     the routine returns error_status_ok. Otherwise, it returns 
 *           an error. 
 */
error_status_t
dcp__log_validate_routing(dcp_log_info_p_t dcp_log_info_p,
                          char *routing,
                          Tcl_Interp *interp, 
                          boolean32 *debug_flag,
			  char *string_routing)
{
    error_status_t  	    	status;
    int                         i;
    dce_svc_stringarray_t       components;
    char                        *routing_ptr;
    char                        *temp_string_routing_ptr;
    static char 		temp_string_routing[BUFSIZ] = "";
    int                         rc;

    /*
     * Initialization
     */
    CLEAR_STATUS(&status);
    temp_string_routing_ptr = (char *)&temp_string_routing;

    /*
     * Parse the input LOG routing
     */
    routing_ptr = strchr(routing, ' ');
    if (routing_ptr == NULL){
	routing_ptr = strchr(routing, DCP_C_LOG_SEP_CHAR);
	if (routing_ptr == NULL) {
	    SET_STATUS(&status, dcp_s_log_inv_routing); 
	    return(status);
	}
    }

    dce_svc_inq_components(dcp_log_info_p->server_handle,
    			   &components,
			   &status);
    if (BAD_STATUS(&status)) {
    	 return(status);
    }

    for (i = 0; (i < components.tab_size) && (! *debug_flag); i++) {
	if (strncmp((char *)components.table[i], 
		    routing,
		    strlen((char *)components.table[i])) == 0) {
	    *debug_flag = TRUE;
	}
    } /* End of loop thru all components */


    /*
     * If svc or debug "routing" is in string syntax, it's OK to 
     * pass to the serviceability API.  
     *
     * If svc "routing" is in TCL syntax, need to replace spaces 
     * with colons.
     * If debug "routing" is in TCL syntax, need to convert to
     * string syntax, also. But "flags" field is tricky.
     *
     * Also, must handle multiple routes in a single routing
     * specification.
     */
    routing_ptr = routing; 
    routing_ptr = strchr(routing, DCP_C_LOG_SEP_CHAR);
    if (routing_ptr != NULL){
        strcpy(string_routing, routing);
    } /* End of routing in string syntax */
    else {
	rc = dcp__log_routing_tcl_to_string(routing, 
	                                    debug_flag,
	                                    interp,
	                                    temp_string_routing_ptr);
	if (rc != TCL_OK) {
	    SET_STATUS(&status, dcp_s_log_inv_routing); 
	    return(status);
	}
        strcpy(string_routing, temp_string_routing_ptr);
    } /* End of routing in TCL syntax */

    return(status);

} /* End of dcp__log_validate_routing() routine */


/*
 * FUNCTION:
 * dcp__log_routing_tcl_to_string()
 *
 * OVERVIEW:
 * This routine converts input routings in TCL sytax to string
 * syntax.
 *
 * For svc routing format:
 * severity how where{ private}
 *
 * For dbg routing format:
 *           <flags>
 * component sub-component level how where{ private}
 * component {sub-component level} how where{ private}
 * component {{sub-component level} {sub-component level}} how where{ private}
 *
 * For routings with multiple routes format:
 * {{how where private} {how where} {how}}
 *
 * INPUTS:
 * routing:
 *           One serviceability or debug routing specification in 
 *           TCL syntax.
 *
 * debug_flag:
 *           Indicates whether the routing is a serviceability or
 *           debug routing specification.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * outstring_routing:
 *           One serviceability or debug routing specification in 
 *           string syntax.
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp__log_routing_tcl_to_string(char *routing,
                               boolean32 *debug_flag,
                               Tcl_Interp *interp,
                               char *outstring_routing)
{
    int                         j;
    int                         routing_argc;
    char                        **routing_argv;
    int                         flags_argc;
    char                        **flags_argv;
    char                        *flags_field_ptr;
    int                         multi_route_argc;
    char                        **multi_route_argv;
    char                        *multi_route_ptr;
    int                         element_nbr = 0;
    char                        str_routing[BUFSIZ] = "";
    char                        *str_routing_ptr; 

    if (Tcl_SplitList(interp, routing, 
			  &routing_argc, &routing_argv) != TCL_OK){
	return(TCL_ERROR);    
    }

    /*
     * svc routing "severity" field is appended OR
     * debug routing "component" field is appended
     */
    str_routing_ptr = strcat(str_routing, routing_argv[element_nbr++]);
    str_routing_ptr = strcat(str_routing, ":");

    /*
     * If debug routing, "flags" field is appended
     */
    if (*debug_flag) {
        if (isllist(routing_argv[element_nbr])) {
	    /* 
	     * To satisfy multiple flag fields, ex: 
	     * {dts {{msgs 1} {states 8}} TEXTFILE /tmp/jas-dmsgs}
	     */
    	    if (Tcl_SplitList(interp, routing_argv[element_nbr], 
			  &flags_argc, &flags_argv) != TCL_OK){
                free((char *) routing_argv);
		return(TCL_ERROR);    
    	    }
	    for (j = 0; j < flags_argc; j++) {
	        flags_field_ptr = strchr(flags_argv[j], ' ');	
	        if (flags_field_ptr != NULL) {
	            *flags_field_ptr = DCP_C_LOG_LEVELSEP_CHAR;
                    str_routing_ptr = strcat(str_routing, flags_argv[j]);
	        } else {
                    free((char *) routing_argv);
                    free((char *) flags_argv);
		    return(TCL_ERROR);    
		}
		if (j != (flags_argc - 1))
                    str_routing_ptr = strcat(str_routing, ",");
	    } /* End of loop thru each flags specification */
            free((char *) flags_argv);
        } else {
	    flags_field_ptr = strchr(routing_argv[element_nbr], ' ');	
	    if (flags_field_ptr != NULL) {
	        /* 
	         * To satisfy flag fields specified as follows: 
	         * {dts {msgs 1} TEXTFILE /tmp/jas-dmsgs}
	         */
	        *flags_field_ptr = DCP_C_LOG_LEVELSEP_CHAR;
                str_routing_ptr = strcat(str_routing, 
					 routing_argv[element_nbr]);
	    } else {
	        /* 
	         * illegal routing specifications:
		 * {dts msgs 1 TEXTFILE /tmp/jas-dmsgs} 
		 * {dts msgs.1 TEXTFILE /tmp/jas-dmsgs} 
		 */
                free((char *) routing_argv);
		return(TCL_ERROR);    
	    }
        } /* End of 1 flags field  specified */

        str_routing_ptr = strcat(str_routing, ":");
	element_nbr++;   

    } /* End of debug routing "flags" field */


    /*
     * Finish rest of routing specification
     * Must check if multiple routes were specified
     */
    for ( ; element_nbr < routing_argc; element_nbr++) {

        multi_route_ptr = strchr(routing_argv[element_nbr], ' ');	
        if (multi_route_ptr != NULL) {
    	    if (Tcl_SplitList(interp, routing_argv[element_nbr], 
			  &multi_route_argc, &multi_route_argv) != TCL_OK){
                free((char *) routing_argv);
		return(TCL_ERROR);    
    	    }
	    for (j = 0; j < multi_route_argc; j++) {
		for (multi_route_ptr = multi_route_argv[j]; 
		     multi_route_ptr != NULL; ) {
	            multi_route_ptr = strchr(multi_route_argv[j], ' ');	
	            if (multi_route_ptr != NULL) {
	                *multi_route_ptr = DCP_C_LOG_SEP_CHAR;
	            } 
	        } 
                str_routing_ptr = strcat(str_routing, multi_route_argv[j]);
                str_routing_ptr = strcat(str_routing, ":");
		if (j != (multi_route_argc - 1))
                    str_routing_ptr = strcat(str_routing, ";");
	    } /* End of loop thru each mulitple route specification */
            free((char *) multi_route_argv);
        } else {
            str_routing_ptr = strcat(str_routing, routing_argv[element_nbr]);
            str_routing_ptr = strcat(str_routing, ":");
	}

    } /* End of loop thru each element of routing specification */

    /*
     * Store new routing format for return
     */
     strcpy(outstring_routing, str_routing_ptr);

    /*
     * Free allocated memory
     */
    free((char *) routing_argv);

    return(TCL_OK);

} /* End of dcp__log_routing_tcl_to_string() routine */


/*
 * FUNCTION:
 * dcp__log_str_to_tcl_debug_flags()
 *
 * OVERVIEW:
 * This routine converts input debug routings in string sytax to TCL
 * syntax.
 *
 * For input dbg routing format:
 *           <flags>
 * component sub-component.level how where{ private}
 * component sub-component.level,sub-component.level how where{ private}
 *
 * INPUTS:
 * routing:
 *           One debug routing specification with the "flags"
 *           field still in string syntax.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * tcl_debug_routing:
 *           One debug routing specification in TCL syntax.
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp__log_str_to_tcl_debug_flags(char *routing,
                                Tcl_Interp *interp,
			        char *tcl_debug_routing)
{
    int                         i;
    char                        *flags_field_ptr = (char *)NULL;
    char                        *level_field_ptr = (char *)NULL;
    int                         routing_argc;
    char                        **routing_argv = (char **)NULL;
    int                         flags_argc;
    char                        **flags_argv = (char **)NULL;
    int                         element_nbr = 0;
    Tcl_DString                 dstring;
    Tcl_DString                 dstr_flags;
    char                        *tcl_debug_routing_ptr = (char *)NULL;
    char                        *tcl_flags_ptr = (char *)NULL;


    if (Tcl_SplitList(interp, routing, 
			  &routing_argc, &routing_argv) != TCL_OK){
	return(TCL_ERROR);    
    }

    Tcl_DStringInit(&dstring);

    /*
     * debug routing "component" field is appended
     */
    tcl_debug_routing_ptr = Tcl_DStringAppendElement(&dstring, routing_argv[0]);

    /*
     * debug routing "flags" field is appended
     */
    flags_field_ptr = strchr((char *)routing_argv[1], DCP_C_LOG_FLAGSEP_CHAR);
    if (flags_field_ptr == NULL){
        /* 
         * To satisfy one flag field specification:
         * msgs.1 
         */
        level_field_ptr = strchr((char *)routing_argv[1],
	                         DCP_C_LOG_LEVELSEP_CHAR);
	if (level_field_ptr != NULL)
            *level_field_ptr = ' ';
        tcl_debug_routing_ptr = Tcl_DStringAppendElement(&dstring, 
                                                         routing_argv[1]);
    } else {
        /* 
         * To satisfy multiple flag fields, ex: 
         * msgs.1,states.8,general.5
	 */
	*flags_field_ptr = ' ';
        for ( ; flags_field_ptr != NULL; ) {
            flags_field_ptr = strchr((char *)routing_argv[1], 
				     DCP_C_LOG_FLAGSEP_CHAR);
            if (flags_field_ptr != NULL)
	        *flags_field_ptr = ' ';
	} /* End of loop thru all flags specifications */

	/* 
	 * Manipulate:   msgs.1 states.8 general.5 
	 */
    	if (Tcl_SplitList(interp, routing_argv[1], 
			      &flags_argc, &flags_argv) != TCL_OK){
                    free((char *) routing_argv);
                    Tcl_DStringFree(&dstring);
		    return(TCL_ERROR);    
    	}
        Tcl_DStringInit(&dstr_flags);
	for (i = 0; i < flags_argc; i++) {
	    flags_field_ptr = strchr(flags_argv[i], 
				     DCP_C_LOG_LEVELSEP_CHAR);
	    if (flags_field_ptr != NULL) {
	        *flags_field_ptr = ' ';
	    } 
            tcl_flags_ptr = Tcl_DStringAppendElement(&dstr_flags,
		    			             flags_argv[i]);
	} /* End of loop thru each flags specification */
        tcl_debug_routing_ptr = Tcl_DStringAppendElement(&dstring, 
                                                         tcl_flags_ptr);
        Tcl_DStringFree(&dstr_flags);
        free((char *) flags_argv);
    } /* End of manipulate flags field */

    /*
     * Finish rest of routing specification
     */
    for (element_nbr = 2; element_nbr < routing_argc; element_nbr++) {
        tcl_debug_routing_ptr = Tcl_DStringAppendElement(&dstring, 
						 routing_argv[element_nbr]);
    }

    /*
     * Store new routing format for return
     */
    strcpy(tcl_debug_routing, tcl_debug_routing_ptr);

    /*
     * Free allocated memory
     */
    Tcl_DStringFree(&dstring);
    free((char *) routing_argv);

    return(TCL_OK);

} /* End of dcp__log_str_to_tcl_debug_flags() routine */


/*
 * FUNCTION:
 * dcp__log_process_ignored_rfields()
 *
 * OVERVIEW:
 * This routine converts input routings in string sytax to handle
 * fields that are ignored. Replacing the ignored field with a
 * {} string.
 * The SVC API ignores the "where" field for all DISCARD, STDERR, 
 * and STDOUT "how" fields. Examples:
 *
 *      ERROR:DISCARD:
 *      ERROR:DISCARD::private_data
 *      ERROR:DISCARD:-
 *      ERROR:DISCARD:-:private_data
 * or
 *      dts:general.1:STDERR:
 *      dts:general.1:STDERR::private_data
 *      dts:general.1:STDERR:-
 *      dts:general.1:STDERR:-:private_data
 *
 * INPUTS:
 * routing:
 *           One production routing specification with the "where"
 *           field but a "private" field was specified.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * tcl_prod_routing:
 *           One production routing specification in TCL syntax.
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
void 
dcp__log_process_ignored_rfields(char *routing_p,
                               Tcl_Interp *interp,
			       char *full_str_routing)
{
    char                        *full_str_routing_ptr;
    char                        tmp_str_routing[BUFSIZ] = "";

    /*
     * To handle fields that do not require any
     * specification and place a "-" there if we see two
     * colons in a row. 
     */
    for ( ; *routing_p != '\0'; routing_p++) {
        full_str_routing_ptr = strncat(tmp_str_routing, routing_p, (size_t)1);
        if ((*(routing_p) == ':') && (*(routing_p + 1) == ':')) {
            full_str_routing_ptr = strcat(tmp_str_routing, "{}");
         } 
    } /* End of process routing */

    /*
     * Store new routing format for return
     */
    strcpy(full_str_routing, full_str_routing_ptr);

} /* End of dcp__log_process_ignored_rfields() routine */


/*
 * FUNCTION:
 * dcp__log_str_to_tcl_routing()
 *
 * OVERVIEW:
 * This routine converts input routings in string sytax to TCL
 * syntax. Note that debug routing specifications will have to
 * go thru another parsing routine to handle the "flags" field 
 * containing subcomponents and debug levels.
 *
 * INPUTS:
 * routing:
 *           One routing specification in string syntax
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * tcl_routing:
 *           One routing specification in TCL syntax.
 *
 * RETURNS:
 * None
 */
void 
dcp__log_str_to_tcl_routing(char *str_routing,
			    char *tcl_routing)
{
    char                        *routing_ptr;

    for (routing_ptr = str_routing; routing_ptr != NULL; ) {
        routing_ptr = strchr(str_routing, DCP_C_LOG_SEP_CHAR);
        if (routing_ptr != NULL){
            /* To prevent last space: {ERROR DISCARD } */
            if (*(routing_ptr + 1) == '\0') {
                *routing_ptr = '\0';
            } else {
                *routing_ptr = ' ';
            }
        }
    } /* End of manipulate routing syntax loop */

    /*
     * Store new routing format for return
     */
    strcpy(tcl_routing, str_routing);

} /* End of dcp__log_str_to_tcl_routing() routine */


/*
 * FUNCTION:
 * dcp__log_str_to_tcl_multi_routing()
 *
 * OVERVIEW:
 * This routine converts input routing with multiple routes in string 
 * sytax to TCL syntax.  Note that all colons have been replaced with
 * spaces. Possible inputs:
 *  ERROR STDERR ;STDOUT ;FILE /tmp/jas
 * or
 *  dts msgs.2,general.7 FILE /tmp/jas;STDERR ;STDOUT
 *  
 *
 * INPUTS:
 * debug_flag:
 *           Indicates whether the routing is a serviceability or
 *           debug routing specification.
 *
 * tcl_single_routing:
 *           One routing specification with multiple routes in string syntax
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold 
 *	     the error message (if any) generated from the command. This 
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * tcl_routing:
 *           One routing specification with multiple routes in TCL syntax.
 *
 * RETURNS:
 * TCL_OK     Notify the interpreter of successful completion
 * TCL_ERROR  Notify the interpreter of command failure
 */
int 
dcp__log_str_to_tcl_multi_routing(boolean32 debug_flag,
				  char *tcl_single_routing,
				  Tcl_Interp *interp,
			          char *tcl_routing)
{
    char                        *routing_ptr;
    char                        routing[BUFSIZ] = "";
    char                        tmp[BUFSIZ] = "";
    char                        c, nextc;
    int                         i = 0;
    int                         routing_len;
    int                         tmp_ptr = 0;

    /*
     * Determine if there are multiple routes specified.
     * If not, return.
     */
    routing_ptr = strchr(tcl_single_routing, DCP_C_LOG_ROUTESEP_CHAR);
    if (routing_ptr == NULL){
        strcpy(tcl_routing, tcl_single_routing);
	return(TCL_OK);
    }

    routing_ptr = (char *)NULL;

    /*
     * If debug, handle "component" and "sub-component" fields.
     * Otherwise, handle "severity" field only.
     */
    while ((tcl_single_routing != NULL) && (*tcl_single_routing != ' ')) {
        routing[i] = *tcl_single_routing;
        i++; tcl_single_routing++;
    }
    routing[i++] = ' ';

    if (debug_flag) {
        while ((tcl_single_routing != NULL) && (*tcl_single_routing != ' ')) {
            routing[i] = *tcl_single_routing;
            i++; tcl_single_routing++;
        }
        routing[i++] = ' ';
    } 

    /* Terminat ethe routing string and eat the first space */
    routing[i++] = '\0';
    tcl_single_routing++;

    /*
     * Handle all "how:where:private" field combinations.
     * For example,     
     *   how:
     *   how:where
     *   how:where:private
     */
    routing_ptr = strcat(routing, "{");

    /* Scan through the remaining string character by character. Grouping
     * words between semicolons and putting braces around these groups 
     * as required. */
    routing_len = strlen(tcl_single_routing);
    for (i = 0; i <= routing_len; i++) {
        c = *(tcl_single_routing + i);
        nextc = *(tcl_single_routing + i + 1);
        if (c == '\0') {
            /* Copy down the completed routing element */
            /* Copy down the completed routing element */
            if (routing[strlen(routing)] != '{') {
                routing_ptr = strcat(routing, " ");
            }
            if (strrchr(tmp, ' ') != NULL) {
                routing_ptr = strcat(routing, "{");
                routing_ptr = strcat(routing, tmp);
                routing_ptr = strcat(routing, "}");
            }
            else {
                routing_ptr = strcat(routing, tmp);
            }

            break;
        }

        if (((c == ' ') && (nextc == DCP_C_LOG_ROUTESEP_CHAR)) ||
            (c == DCP_C_LOG_ROUTESEP_CHAR)) {
            /* Copy down the completed routing element */
            if (routing[strlen(routing) - 1] != '{') {
                routing_ptr = strcat(routing, " ");
            }
            if (strrchr(tmp, ' ') != NULL) {
                routing_ptr = strcat(routing, "{");
                routing_ptr = strcat(routing, tmp);
                routing_ptr = strcat(routing, "}");
            }
            else {
                routing_ptr = strcat(routing, tmp);
            }

            tmp[0] = '\0';
            tmp_ptr = 0;

            /* If the NEXT char is a semi, skip it */
            if (nextc == DCP_C_LOG_ROUTESEP_CHAR) {
                i++;
            }
            continue;
        }
        
        /*
         * c should be a space, not followed by a semi, or a regular char.
         * Copy it down and keep going. 
         */
        tmp[tmp_ptr] = c;
        tmp[++tmp_ptr] = '\0';
    }
    routing_ptr = strcat(routing, "}");

    /*
     * Store new routing format for return
     */
    strcpy(tcl_routing, routing_ptr);

    return(TCL_OK);

} /* End of dcp__log_str_to_tcl_multi_routing() routine */
