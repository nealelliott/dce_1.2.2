/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: log_tclif.c,v $
 * Revision 1.1.4.2  1996/02/18  19:13:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:21:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/22  20:37 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chfts45/1  1995/08/22  20:34 UTC  truitt
 * 	CHFts16101: Format log list output correctly.
 * 	[1995/12/08  17:56:12  root]
 * 
 * Revision 1.1.2.5  1994/09/08  14:00:46  salamone
 * 	CR11968-Setting authentication on binding handles.
 * 	[1994/09/08  13:59:18  salamone]
 * 
 * Revision 1.1.2.4  1994/08/19  18:04:14  salamone
 * 	CR10683 - Handle 1 string binding in TCL syntax.
 * 	[1994/08/19  17:52:10  salamone]
 * 
 * Revision 1.1.2.3  1994/08/08  18:51:50  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:20  melman]
 * 
 * Revision 1.1.2.2  1994/05/18  20:41:04  salamone
 * 	CR10636 - "log show" and "log list" should display the appropriate
 * 		   info if unauthenticated.
 * 	[1994/05/13  21:33:32  salamone]
 * 
 * Revision 1.1.2.1  1994/04/26  15:32:37  salamone
 * 	Free argv properly.
 * 	[1994/04/26  15:31:01  salamone]
 * 
 * 	Initial version of log_tclif.c
 * 	[1994/04/25  13:38:32  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * log_tclif.c
 *
 * DESCRIPTION:
 * This module contains all the "log" operations to show, modify, or 
 * list the current state of routing for DCE serviceability messages
 * of a process thru dcecp.
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

#include <log_tclif.h>
#include <log_edit.h>

/*
 * DECLARATIONS 
 */

/*
 * INTERNAL VARIABLES
 */
static dcp_log_info_t dcp_log_info, *dcp_log_info_p;

/*
 * PROTOTYPES 
 */


/*
 * FUNCTION:
 * dcp_log_list()
 *
 * OVERVIEW:
 * This routine returns a list of serviceability components registered
 * by the server. In addition, the -comp option, which is used to
 * specify a component name, returns a list of subcomponent names of
 * the specified component, its active level, and its text of the long 
 * description of the subcomponent name.
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
dcp_log_list(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int   i;
    char  **largv;
    int   largc;
    int   rc;
    char  *component_name_list = NULL;

    dcp_ArgvInfo arg_table[] = {
        {"-comp", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_log_list_comp_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&component_name_list;

    dcp_log_info_p = &dcp_log_info;

    /*
     * Process command line arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_log_no_server); 
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);   

    /* 
     * Cycle through all servers that were specified to be listed.
     */
    if (dcp_general_split_list(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_log_info.named_object = dce_strdup((char *)largv[i]);

        rc = dcp_log_bind_named_object(interp, dcp_log_info_p);
        if (rc != TCL_OK) {
            break;
        }

        if (component_name_list == NULL) {
            rc = dcp_log_do_list_comp(dcp_log_info_p, interp);
        } else {
            rc = dcp_log_do_list_subcomp(component_name_list,
                                         dcp_log_info_p,
                                         interp);
        }
        if (rc != TCL_OK) {
            break;
        }

        if (i != (largc - 1)) {
            Tcl_AppendResult(interp, "\n", (char *)NULL);
        }
        if ((component_name_list != NULL) && (i != (largc - 1))) {
            Tcl_AppendResult(interp, "\n", (char *)NULL);
        }
    }

    /*
     * List of elements are no longer needed.
     */
    free((char *)largv);
    return(rc);
}


/*
 * FUNCTION:
 * dcp_log_modify()
 *
 * OVERVIEW:
 * This routine is used to change the routing specifications of the 
 * specified servers. The -change option takes a list of either 
 * serviceability or debug routing specifications.
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
dcp_log_modify(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int   i;
    char  **largv;
    int   largc;
    int   rc;
    char  *change_routing_list = NULL;

    dcp_ArgvInfo arg_table[] = {
        {"-change", DCP_ARGV_STRING, (char *)NULL, (char *)NULL,
            dcp_t_log_modify_change_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&change_routing_list;

    dcp_log_info_p = &dcp_log_info;

    /*
     * Process command line arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_log_no_server); 
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /*
     * The -change option must be specified.
     */
    if (!change_routing_list) {
        DCP_SET_MISC_ERROR(dcp_s_log_no_modify_op); 
    }

    /*
     * Cycle through all servers that were specified to be modified.
     */
    if (dcp_general_split_list(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_log_info.named_object = dce_strdup((char *)largv[i]);

        rc = dcp_log_bind_named_object(interp, dcp_log_info_p);
        if (rc != TCL_OK) {
            break;
        }

        rc = dcp_log_do_modify_routings(change_routing_list,
                                        dcp_log_info_p,
                                        interp);
        if (rc != TCL_OK) {
            break;
        }
    }

    /*
     * List of elements are no longer needed.
     */
    free((char *)largv);
    return(rc);
}


/*
 * FUNCTION:
 * dcp_log_show()
 *
 * OVERVIEW:
 * This routine returns a list describing the current serviceability
 * routing settings for a server. If more than 1 server is specified,
 * the output is concatenated together.  In addition, the optional
 * -debug option indicates that debug routing settings are returned
 * instead of normal serviceability routing settings.
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
dcp_log_show(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    int   i;
    char  **largv;
    int   largc;
    int   rc;
    int   debug_values = 0;

    dcp_ArgvInfo arg_table[] = {
        {"-debug", DCP_ARGV_CONSTANT, (char *)1, (char *)NULL,
            dcp_t_log_debug_help}, 
        {(char *)NULL, DCP_ARGV_END, (char *)NULL, (char *)NULL, 0}
    };

    arg_table[0].dst = (char *)&debug_values;

    dcp_log_info_p = &dcp_log_info;

    /*
     * Process command line arguments.
     */
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) == TCL_ERROR)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for syntax errors.
     */
    if (argc < 2) {
        DCP_SET_MISC_ERROR(dcp_s_log_no_server); 
    }
    else if (argc > 2) {
        DCP_SET_MISC_ERROR(dcp_s_too_many_args);
    }

    /*
     * Set convenience variables.
     */
    DCP_GENERAL_SET_NAMELIST(argv[1]);

    /* 
     * Cycle through all servers that were specified to be listed.
     */
    if (dcp_general_split_list(interp, argv[1], &largc, &largv) != TCL_OK) {
        return TCL_ERROR;
    }

    rc = TCL_OK;
    for (i = 0; i < largc; i++) {
        dcp_log_info.named_object = dce_strdup((char *)largv[i]);

        rc = dcp_log_bind_named_object(interp, dcp_log_info_p);
        if (rc != TCL_OK) {
            break;
        }

        rc = dcp_log_do_show_svc_routings(debug_values,
                                          dcp_log_info_p, 
                                          interp);
        if (rc != TCL_OK) {
            break;
        }

        if (i != (largc - 1)) {
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
        }
    }

    /*
     * List of elements are no longer needed.
     */
    free((char *)largv);
    return(rc);
}

