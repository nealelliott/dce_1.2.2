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
 * $Log: aud_ops.c,v $
 * Revision 1.1.10.3  1996/02/18  19:11:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:20  marty]
 *
 * Revision 1.1.10.2  1995/12/08  21:19:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/30  15:45 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts32/1  1995/05/30  15:44 UTC  truitt
 * 	CHFts15357: Fix error messages coming out of the
 * 	audtrail show command.
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/23  19:04 UTC  truitt
 * 	Merge field header work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  19:03 UTC  truitt
 * 	CHFts14679: Clean up show headers.
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/14  15:43 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:42 UTC  truitt
 * 	CHFts14285: Have the audit code call the correct
 * 	macros when setting an error condition.
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/18  14:42 UTC  truitt
 * 	Merge unintegrated
 * 	[1995/12/08  17:54:08  root]
 * 
 * Revision 1.1.6.1  1994/10/19  17:31:11  rousseau
 * 	Fix off-by-one problem in filter type check (CR 12665).
 * 	[1994/10/19  17:27:40  rousseau]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/03  19:43 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/11/03  19:41 UTC  truitt
 * 	Fix aud command switches.
 * 
 * Revision 1.1.2.27  1994/10/05  17:57:59  rousseau
 * 	Fix multi-guide logic (CR 12502).
 * 	[1994/10/05  17:57:34  rousseau]
 * 
 * Revision 1.1.2.26  1994/10/01  01:33:52  rousseau
 * 	Remove server binding code from audevents commands.
 * 	[1994/10/01  01:33:37  rousseau]
 * 
 * Revision 1.1.2.25  1994/09/27  13:45:40  rousseau
 * 	Moved dce_aud_close calls in audtrail_show (CR 12332).
 * 	[1994/09/27  13:45:32  rousseau]
 * 
 * Revision 1.1.2.24  1994/09/23  02:17:00  rousseau
 * 	Change stop stostrategy to save (CR 12312).
 * 	[1994/09/23  02:16:40  rousseau]
 * 
 * Revision 1.1.2.23  1994/09/19  17:27:18  rousseau
 * 	Close audit trail (CR 12233).
 * 	[1994/09/19  17:27:07  rousseau]
 * 
 * Revision 1.1.2.22  1994/09/16  15:11:00  rousseau
 * 	Remove #ifdefs in audttrail show.
 * 	[1994/09/16  15:10:42  rousseau]
 * 
 * Revision 1.1.2.21  1994/09/02  19:47:21  rousseau
 * 	Fix core dump in audf create (CR 12033).
 * 	[1994/09/02  19:46:49  rousseau]
 * 
 * Revision 1.1.2.20  1994/09/01  14:16:44  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:01  rousseau]
 * 
 * Revision 1.1.2.19  1994/08/24  15:32:05  rousseau
 * 	Fixed handling of null arg (CR 11835).
 * 	[1994/08/24  15:31:49  rousseau]
 * 
 * Revision 1.1.2.18  1994/08/19  22:15:31  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:14:41  rousseau]
 * 
 * Revision 1.1.2.17  1994/08/18  21:09:21  rousseau
 * 	Handle error return from dce_aud_print() (CR 11703).
 * 	[1994/08/18  21:09:04  rousseau]
 * 
 * Revision 1.1.2.16  1994/08/08  21:04:04  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:01:36  melman]
 * 
 * Revision 1.1.2.15  1994/08/08  18:24:48  rousseau
 * 	Check args in audevents show (CR 11579).
 * 	[1994/08/08  18:24:38  rousseau]
 * 
 * Revision 1.1.2.14  1994/07/28  20:39:11  rousseau
 * 	64 bit changes (CR 11451).
 * 	Fix reading of audit daemon bindings (CR 11466).
 * 	[1994/07/28  20:37:49  rousseau]
 * 
 * Revision 1.1.2.13  1994/07/19  18:06:25  rousseau
 * 	Implemented reomte control and fixed ~ in paths (CR 11178 11179 10221).
 * 	[1994/07/19  18:05:38  rousseau]
 * 
 * Revision 1.1.2.12  1994/06/27  20:04:29  rousseau
 * 	Allow audfilter create and modify to batch guides (CR 10647).
 * 	[1994/06/27  20:04:11  rousseau]
 * 
 * Revision 1.1.2.11  1994/06/24  15:34:07  rousseau
 * 	Fix audtrail show problem (CR 10827).
 * 	[1994/06/24  15:33:06  rousseau]
 * 
 * Revision 1.1.2.10  1994/06/21  18:22:03  rousseau
 * 	Fixes for aud modify problems (CR 10520 10915 10920).
 * 	[1994/06/21  18:21:37  rousseau]
 * 
 * Revision 1.1.2.9  1994/06/09  16:01:33  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:46:54  devsrc]
 * 
 * Revision 1.1.2.8  1994/06/01  17:26:16  rousseau
 * 	Added aud_rewind.
 * 	[1994/06/01  17:25:20  rousseau]
 * 
 * Revision 1.1.2.7  1994/05/18  21:17:56  rousseau
 * 	Initialize guides to NULL before every RPC in audfilter create and modify (CR 10373).
 * 	[1994/05/18  21:17:34  rousseau]
 * 
 * Revision 1.1.2.6  1994/05/09  19:26:01  rousseau
 * 	Fixes for:
 * 	10394   Need better error messages for list problems.
 * 	10393   core dump in dcp_aud_check_multi_guides()
 * 	10391   audf create doesn't catch invalid condition.
 * 	10383   audfilter create dies w/o -attribute option
 * 	10293   Memory allocated by dce_sprinf is not released
 * 	9869    the dcecp audit component is not complete
 * 	[1994/05/09  19:22:45  rousseau]
 * 
 * Revision 1.1.2.5  1994/04/18  16:58:40  rousseau
 * 	Merged with changes from 1.1.2.4
 * 	[1994/04/18  16:58:29  rousseau]
 * 
 * 	Second drop of audit functionality (CR 9691).
 * 	[1994/04/18  16:55:15  rousseau]
 * 
 * Revision 1.1.2.4  1994/04/07  10:22:04  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:44  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component dependency
 * 	     build issues
 * 	[1994/04/07  10:10:29  kevins]
 * 
 * Revision 1.1.2.3  1994/03/31  18:39:47  rousseau
 * 	Changes to all audfilter commands (CR 9869).
 * 	[1994/03/31  18:39:28  rousseau]
 * 
 * Revision 1.1.2.2  1994/02/02  20:00:37  rousseau
 * 	Initial implementation of audfilter, audtrail and audevents.
 * 	     Includes help functions.
 * 	[1994/02/02  19:57:03  rousseau]
 * 
 * Revision 1.1.2.1  1994/01/06  16:50:59  rousseau
 * 	Initial version.
 * 	[1994/01/06  16:49:23  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: aud_ops.c
 *
 * DESCRIPTION:
 *   This module contains all of the functions that implement the dcecp
 * audit (aud, audfilter, audevents, audtrail) commands. Helper functions
 * are found in aud_util.c.
 *
 */

/* INCLUDE FILES  */

#include <stdlib.h>
#include <fcntl.h>
#include <dce/eslp.h>
#include <aud.h>
#include <dce/dceaudmsg.h>
#include <util_general.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

extern char *Type[];
extern aud_index_entry_t aud_g_cp_condition[];
extern aud_index_entry_t aud_g_cp_action[];

char *aud_attributes[NUM_AUD_ATTRIBUTES] = {
    "stostrategy",
    "state"
};

extern boolean32                       dcp_aud_init;

/* PROTOTYPES */

extern void aud_esl_free_guides(aud_esl_guides_p_t);
extern void aud_esl_purge_entries(aud_esl_type_t, void *);
extern boolean32 get_ec_number_from_name(char *, unsigned32 *);
extern boolean32 get_ec_names(int *, char ***);
extern boolean32 get_ec_events(char *, unsigned32 *, int *, unsigned32 **);


/** 
 ** 
 **   DCECP aud operations
 ** 
 **/


/*
 * FUNCTION:    aud_enable
 *
 * OVERVIEW:    Enables an audit daemon. The state attribute is changed to 
 *              "enabled".
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
aud_enable(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                                *site;
    error_status_t                      st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        site = argv[1];
    }
    else {
        site = NULL;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, site) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Make the RPC to change the server's state */
    audit_control_modify_state(dcp_aud_binding_h, 
                               aud_c_dmn_state_enabled, 
                               &st);
    DCP_CHECK_AUD_ERROR(st);

    return TCL_OK;
}


/*
 * FUNCTION:    aud_disable
 *
 * OVERVIEW:    Disables an audit daemon. The state attribute is changed to 
 *              "disabled".
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
aud_disable(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *site;
    error_status_t              st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        site = argv[1];
    }
    else {
        site = NULL;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, site) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Make the RPC to change the server's state */
    audit_control_modify_state(dcp_aud_binding_h, 
                               aud_c_dmn_state_disabled, 
                               &st);
    DCP_CHECK_AUD_ERROR(st);

    return TCL_OK;
}


/*
 * FUNCTION:    aud_modify
 *
 * OVERVIEW:    Allows modification of the stostrtegy and state attributes. 
 *              Accepts either the -changeattr or the -attribute options with 
 *              an attribute list. Also accepts the -stostrategy and -state 
 *              options to change the individual attributes.
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
aud_modify(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *site;
    char                        *stostrategy = NULL;
    char                        *state = NULL;
    char                        *change_list = NULL;
    int                         i, index;
    int                         change_listc, attrc;
    char                        **change_listv, **attrv, **tmp;
    error_status_t              st;

    dcp_ArgvInfo arg_table[] = {
        {"-change", DCP_ARGV_STRING, NULL, NULL, 
             dcp_t_aud_modify_change_help},
        {"-state", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_aud_modify_state_help},
        {"-stostrategy", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_aud_modify_stostrategy_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&change_list;
    arg_table[1].dst = (char *)&state;
    arg_table[2].dst = (char *)&stostrategy;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        site = argv[1];
    }
    else {
        site = NULL;
    }

    /* 
     * The -change flag is mutually exclusive with the
     * individual attribute options.
     */
    if ((change_list != NULL) && (stostrategy != NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        return TCL_ERROR;
    }

    if ((change_list != NULL) && (state != NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        return TCL_ERROR;
    }

    /* Make sure that the user specified at least one option */
    if ((change_list == NULL) && (stostrategy == NULL) && (state == NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_aud_modify_need_opt);
        return TCL_ERROR;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, site) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Work on the list passed to -change */
    if (change_list != NULL) {
        Tcl_SplitList(interp, change_list, &change_listc, &change_listv);
        for (i = 0; i < change_listc; i++) {
            Tcl_SplitList(interp, change_listv[i], &attrc, &attrv);

            /* 
             * If we are passed a single two-element list, the second 
             * SplitList was a no-op. We need to check for this case
             * and modify our working variables accordingly.
             * Just swap the values for argv around so we can hold on to  
             * the pointers for later free's.
             */
            if (strcmp(attrv[0], change_listv[0]) == 0) {
                attrc = change_listc;
                tmp = attrv;
                attrv = change_listv;
                change_listv = tmp;
                /* Change our loop index so we don't run through here again. */
                i = change_listc;
            }

            if (attrc < 2) {
                DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_short, attrc, attrv);
                free((char *)change_listv);
                return TCL_ERROR;
            }

            if (attrc > 2) {
                DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, attrc, attrv);
                free((char *)change_listv);
                return TCL_ERROR;
            }

            /* 
             * Pull the information out of the -change list and place
             * it into the individual attribute variables.
             */
            if (dcp_general_find_attr_index(interp, 
                                            attrv[0], 
                                            NUM_AUD_ATTRIBUTES,
                                            aud_attributes, 
                                            &index) != TCL_OK) {
                free((char *)attrv);
                free((char *)change_listv);
                return TCL_ERROR;
            }

            switch(index) {
              case aud_stostrategy:
                stostrategy = dce_strdup(attrv[1]);
                free((char *)attrv);
                break;
              case aud_state:
                state = dce_strdup(attrv[1]);
                free((char *)attrv);
                break;
            }                
        } /* for (i < change_listc) */
        free((char *)change_listv);
    } /* if (change_list != NULL) */

    /* Make the RPC to change the storage strategy */
    if (stostrategy != NULL) {
        if (strncmp(stostrategy, "wrap", strlen(stostrategy)) == 0) {
            audit_control_modify_sstrategy(dcp_aud_binding_h, 
                                           aud_c_trl_ss_wrap, 
                                           &st);
            DCP_CHECK_AUD_ERROR(st);
        } 
        else if (strncmp(stostrategy, "save", strlen(stostrategy)) == 0) {
            audit_control_modify_sstrategy(dcp_aud_binding_h, 
                                           aud_c_trl_ss_save, 
                                           &st);
            DCP_CHECK_AUD_ERROR(st);
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_modify_bad_stostrategy, stostrategy);
            return TCL_ERROR;
        }
    }

    /* Make the RPC to change the state */
    if (state != NULL) {
        if (strncmp(state, "enabled", strlen(state)) == 0) {
            audit_control_modify_state(dcp_aud_binding_h, 
                                       aud_c_dmn_state_enabled, 
                                       &st);
            DCP_CHECK_AUD_ERROR(st);
        } 
        else if (strncmp(state, "disabled", strlen(state)) == 0) {
            audit_control_modify_state(dcp_aud_binding_h, 
                                       aud_c_dmn_state_disabled, 
                                       &st);
            DCP_CHECK_AUD_ERROR(st);
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_modify_bad_state, state);
            return TCL_ERROR;
        }
    }

    return TCL_OK;
}


/*
 * FUNCTION:    aud_rewind
 *
 * OVERVIEW:    Rewind the specified audit trail file.
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
aud_rewind(ClientData clientData, Tcl_Interp *interp, 
           int argc, char **argv)
{
    char                        *site;
    error_status_t              st;

    dcp_ArgvInfo arg_table[] = {
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    if (dcp_ParseArgv(interp, 
                      &argc, 
                      argv,
                      arg_table, 
                      DCP_ARGV_NO_OPT_LEFTOVERS) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        site = argv[1];
    }
    else {
        site = NULL;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, site) == TCL_ERROR) {
        return TCL_ERROR;
    }

    audit_control_rewind(dcp_aud_binding_h, &st);
    DCP_CHECK_AUD_ERROR(st);

    return TCL_OK;
}


/*
 * FUNCTION:    aud_show
 *
 * OVERVIEW:    Returns an attribute list for the audit daemon. The 
 *              -attributes option is a no-op, but is included for 
 *              consistency between commands.
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
aud_show(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *site;
    char                        *state_svc;
    char                        *stostr_svc;
    boolean32                   attr_flag = FALSE;
    unsigned32                  state_value, stostrategy_value;
    error_status_t              st;

    dcp_ArgvInfo arg_table[] = {
        {"-attributes", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_aud_show_attributes_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&attr_flag;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        site = argv[1];
    }
    else {
        site = NULL;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, site) == TCL_ERROR) {
        return TCL_ERROR;
    }

    audit_control_show_state(dcp_aud_binding_h, &state_value, &st);
    DCP_CHECK_AUD_ERROR(st);
    audit_control_show_sstrategy(dcp_aud_binding_h, &stostrategy_value, &st);
    DCP_CHECK_AUD_ERROR(st);

    /* Output the returned state */
    Tcl_AppendResult(interp, "{", aud_attributes[1], " ", (char *)NULL);
    switch((int)state_value) {
        case aud_c_dmn_state_enabled:
            state_svc = (char *)dce_sprintf(dcp_t_aud_state_enabled);
            if (state_svc == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_no_memory);
                return TCL_ERROR;
            }
            Tcl_AppendResult(interp, state_svc, "}\n", (char *)NULL);
            free(state_svc);
            break;
        case aud_c_dmn_state_disabled:
            state_svc = (char *)dce_sprintf(dcp_t_aud_state_disabled);
            if (state_svc == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_no_memory);
                return TCL_ERROR;
            }
            Tcl_AppendResult(interp, state_svc, "}\n", (char *)NULL);
            free(state_svc);
            break;
        default:
            DCP_SET_RESULT_CODE(dcp_s_aud_show_state_invalid);
            return TCL_ERROR;
    }

    /* Output the returned storage strategy */
    Tcl_AppendResult(interp, "{", aud_attributes[0], " ", (char *)NULL);
    switch((int)stostrategy_value) {
        case aud_c_trl_ss_wrap:
            stostr_svc = (char *)dce_sprintf(dcp_t_aud_sto_wrap);
            if (stostr_svc == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_no_memory);
                return TCL_ERROR;
            }
            Tcl_AppendResult(interp, stostr_svc, "}", (char *)NULL);
            free(stostr_svc);
            break;
        case aud_c_trl_ss_save:
            stostr_svc = (char *)dce_sprintf(dcp_t_aud_sto_save);
            if (stostr_svc == NULL) {
                DCP_SET_RESULT_CODE(dcp_s_no_memory);
                return TCL_ERROR;
            }
            Tcl_AppendResult(interp, stostr_svc, "}", (char *)NULL);
            free(stostr_svc);
            break;
        default:
            DCP_SET_RESULT_CODE(dcp_s_aud_show_stostrategy_invalid);
            return TCL_ERROR;
    }
    return TCL_OK;
}


/*
 * FUNCTION:    aud_stop
 *
 * OVERVIEW:    Stops the audit daemon process.
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
aud_stop(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                        *site;
    error_status_t              st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check if the user specified a server to talk to */
    if (argc == 2) {
        site = argv[1];
    }
    else {
        site = NULL;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, site) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Stop the server */
    audit_control_stop(dcp_aud_binding_h, &st);
    DCP_CHECK_AUD_ERROR(st);
    
    return TCL_OK;
}




/** 
 ** 
 **   DCECP audfilter operations
 ** 
 **/


/*
 * FUNCTION:    audfilter_catalog
 *
 * OVERVIEW:    Returns a list of names of all filters in the audit daemon.
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
audfilter_catalog(ClientData clientData, Tcl_Interp *interp, 
                  int argc, char **argv)
{
    unsigned16                  cursor;
    aud_esl_type_t              esl_type;
    sec_rgy_name_t              subject_name;
    aud_esl_guides_p_t          guides;
    aud_esl_entry_p_t           entry;
    aud_esl_foreign_entry_p_t   foreign_entry;
    error_status_t              st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* 
     * This command accepts no options. If there are any arguments, fail.
     */
    if (argc != 1) {
        if (argv[1][0] == '-') {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_opt, argv[1]);
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_arg, argv[1]);
        }
        return TCL_ERROR;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, NULL) == TCL_ERROR) {
        return TCL_ERROR;
    }

    for (esl_type = aud_e_esl_princ;
         esl_type <= aud_e_esl_world_overridable;
         esl_type++) {
        if (WORLD_TYPE(esl_type)) {
            audit_control_show_filter(dcp_aud_binding_h, esl_type, 
                                      subject_name, &guides, &st);
            DCP_CHECK_AUD_ERROR(st);

            if (guides != NULL) {
                if (strcmp(interp->result, "") != 0) {
                    Tcl_AppendResult(interp, "\n", (char *)NULL);
                }
                Tcl_AppendResult(interp, Type[esl_type], (char *)NULL);
                aud_esl_free_guides(guides);
            }
        }
        else {
            cursor = 0;
            while (cursor < HASH_TABLE_SIZE) {
                audit_control_list_filter(dcp_aud_binding_h, 
                                          esl_type, 
                                          &cursor,
                                          &entry, 
                                          &foreign_entry, 
                                          &st);
                DCP_CHECK_AUD_ERROR(st);
                
                if (FOREIGN_TYPE(esl_type)) {
                    if (foreign_entry != NULL) {
                        aud_list_entries(interp, esl_type, foreign_entry);
                        aud_esl_purge_entries(esl_type, foreign_entry);
                    }
                }
                else {
                    if (entry != NULL) {
                        aud_list_entries(interp, esl_type, entry);
                        aud_esl_purge_entries(esl_type, entry);
                    }
                } /* if (FOREIGN_TYPE) */
            } /* while (cursor < HASH_TABLE_SIZE) */
        } /* if (WORLD_TYPE) */
    } /* for (esl_type <= aud_e_esl_world_overwritable) */

    return TCL_OK;
}


/*
 * FUNCTION:    audfilter_create
 *
 * OVERVIEW:    Creates a new audit filter with the guides specified by
 *              -attributes.
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
audfilter_create(ClientData clientData, Tcl_Interp *interp, 
                 int argc, char **argv)
{
    aud_esl_guides_p_t          guide;
    aud_esl_guides_p_t          prev_guides;
    int                         i, j, filterc, filter_listc;
    int                         k, l, guidec, guide_listc;
    int                         m, condc, actionc, eclistc;
    char                        **filterv, **filter_listv;
    char                        **guidev, **guide_listv, **tmp;
    char                        **condv, **actionv, **eclistv;
    char                        *subject_name, *attr_list = NULL; 
    unsigned32                  evt_class;
    boolean32                   multi_guides;
    aud_esl_evt_classes_p_t     ec_ptr;
    error_status_t              st;

    dcp_ArgvInfo arg_table[] = {
        {"-attribute", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_audfilter_create_attribute_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&attr_list;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Did the user not specify an argument? */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    } 
    
    /* Did the user not specify the -attribute switch */
    if (attr_list == NULL) {
        DCP_SET_RESULT_CODE(dcp_s_audfilter_create_need_attr);
        return TCL_ERROR;
    }

    /* Check for empty argument */
    if (strlen(argv[1]) == 0) {
        DCP_SET_RESULT_CODE(dcp_s_null_arg);
        return TCL_ERROR;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, NULL) == TCL_ERROR) {
        return TCL_ERROR;
    }

    Tcl_SplitList(interp, argv[1], &filter_listc, &filter_listv);
    for (i = 0; i < filter_listc; i++) {
        Tcl_SplitList(interp, filter_listv[i], &filterc, &filterv);
        
        /* 
         * If we are passed a single two-element list, the second 
         * SplitList was a no-op. We need to check for this case
         * and modify our working variables accordingly.
         */
        if (strcmp(filterv[0], filter_listv[0]) == 0) {
            filterc = filter_listc;
            tmp = filterv;
            filterv = filter_listv;
            filter_listv = tmp;
            /* Change our loop index so we don't run through here again. */
            i = filter_listc;
        }

        if (filterc > 2) {
            DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, filterc, filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* Check if the supplied filter type is valid */
        for (j = 0; j < aud_e_esl_max; j++) {
            if (strcmp((char *)Type[j], filterv[0]) == 0) {
                break;
            }
            if (j == (aud_e_esl_max - 1)) {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_type, filterv[0]);
                free((char *)filterv);
                free((char *)filter_listv);
                return TCL_ERROR;
            }
        }

        /* See if the user didn't supply a key where appropriate */
        if ((filterv[1] == NULL) && (!(WORLD_TYPE(j)))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_need_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* See if the user supplied an extra key. */
        if ((filterv[1] != NULL) && (WORLD_TYPE(j))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_extra_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }
        
        /* Fill in the subject name with the event type on world types */
        if (WORLD_TYPE(j)) {
            subject_name = dce_strdup(Type[j]);
        }
        else {
            subject_name = dce_strdup(filterv[1]);
        }

        free((char *)filterv);
        
        /* Initialize the guides list */
        guide = NULL;
        prev_guides = NULL;

        /* 
         * This is the tricky part. We need to figure out what kind
         * of list we were passed. We will do multiple split lists until
         * we can determine if we have one guide or multiple ones.
         */
        multi_guides = dcp_aud_check_multi_guides(interp, attr_list);

        Tcl_SplitList(interp, attr_list, &guide_listc, &guide_listv);
        for (k = 0; k < guide_listc; k++) {
            Tcl_SplitList(interp, guide_listv[k], &guidec, &guidev);
            
            /* Create a new guide on the guides list */
            guide = (aud_esl_guides_p_t )malloc(sizeof(aud_esl_guides_t));
            guide->next = prev_guides;
            guide->ec_list = NULL;
            prev_guides = guide;

            /* 
             * If we are passed a single guide as a list, we need to
             * only do two split lists. We will undo the effects of the
             * second split list, and do the third (really second) split
             * later.
             */
            if (!multi_guides) {
                guidec = guide_listc;
                tmp = guidev;
                guidev = guide_listv;
                guide_listv = tmp;
                /* Change our loop index so we don't run through here again. */
                k = guide_listc;
            }
            
            if (guidec > 3) {
                DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, guidec, guidev);
                free((char *)filter_listv);
                free((char *)guide_listv);
                free(subject_name);
                aud_esl_free_guides(guide);
                return TCL_ERROR;
            }
            
            if (guidec < 3) {
                DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_short, guidec, guidev);
                free((char *)filter_listv);
                free((char *)guide_listv);
                free(subject_name);
                aud_esl_free_guides(guide);
                return TCL_ERROR;
            }
            
            /* Verify that the event class list is valid */
            guide->ec_list = NULL;
            Tcl_SplitList(interp, guidev[0], &eclistc, &eclistv);
            for (l = 0; l < eclistc; l++) {
                if (get_ec_number_from_name(eclistv[l], &evt_class) == TRUE) {
                    ec_ptr = (aud_esl_evt_classes_p_t )
                        malloc(sizeof(aud_esl_evt_classes_t));
                    ec_ptr->evt_class = evt_class;
                    ec_ptr->next = guide->ec_list;
                    guide->ec_list = ec_ptr;
                }
                else {
                    DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_ec, eclistv[l]);
                    free((char *)filter_listv);
                    free((char *)guidev);
                    free((char *)guide_listv);
                    free((char *)eclistv);
                    free(subject_name);
                    aud_esl_free_guides(guide);
                    return TCL_ERROR;
                }
            }
            free((char *)eclistv);

            /* Verify that the condition is valid */
            guide->audit_condition = 0;
            Tcl_SplitList(interp, guidev[1], &condc, &condv);
            for (l = 0; l < condc; l++) {
                for (m = 0; m < AUD_MAX_CONDITIONS; m++) {
                    if (strcmp(condv[l], 
                               (char *)aud_g_cp_condition[m].name) == 0) {
                        guide->audit_condition |= aud_g_cp_condition[m].number;
                        break;
                    }
                    if (m == AUD_MAX_CONDITIONS - 1) {
                        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_condition,
                                                    condv[l]);
                        free((char *)filter_listv);
                        free((char *)guidev);
                        free((char *)guide_listv);
                        free((char *)condv);
                        free(subject_name);
                        aud_esl_free_guides(guide);
                        return TCL_ERROR;
                    }
                }
            }
            free((char *)condv);

            /* Verify that the action is valid */
            guide->audit_action = 0;
            Tcl_SplitList(interp, guidev[2], &actionc, &actionv);
            for (l = 0; l < actionc; l++) {
                for (m = 0; m < AUD_MAX_ACTIONS; m++) {
                    if (strcmp(actionv[l], 
                               (char *)aud_g_cp_action[m].name) == 0) {
                        guide->audit_action |= aud_g_cp_action[m].number;
                        break;
                    }
                    if (m == AUD_MAX_ACTIONS - 1) {
                        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_action, actionv[l]);
                        free((char *)filter_listv);
                        free((char *)guidev);
                        free((char *)guide_listv);
                        free((char *)actionv);
                        free(subject_name);
                        aud_esl_free_guides(guide);
                        return TCL_ERROR;
                    }
                }
            }
            free((char *)actionv);

        } /* for (k < guide_listc) */

        /* Make the RPC to add the filter */
        audit_control_add_filter(dcp_aud_binding_h, 
                                 j, 
                                 (unsigned char *)subject_name,
                                 &guide, 
                                 &st);
        DCP_CHECK_AUD_ERROR_FREE_FOUR(st, 
                                      filter_listv, 
                                      guidev,
                                      guide_listv,
                                      &subject_name);
        
        aud_esl_free_guides(guide);
    } /* for (i < filter_listc) */

    free((char *)filter_listv);
    free((char *)guidev);
    free((char *)guide_listv);
    free(subject_name);
    return TCL_OK;
}


/*
 * FUNCTION:    audfilter_delete
 *
 * OVERVIEW:    Deletes the filter including all filter guides.
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
audfilter_delete(ClientData clientData, Tcl_Interp *interp, 
                 int argc, char **argv)
{
    int                         i, j, filter_listc, filterc;
    char                        **filter_listv, **filterv, **tmp;
    char                        *subject_name;
    error_status_t              st;


    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Did the user not specify an argument? */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    } 
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check for empty argument */
    if (strlen(argv[1]) == 0) {
        DCP_SET_RESULT_CODE(dcp_s_null_arg);
        return TCL_ERROR;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, NULL) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Split the input list */
    Tcl_SplitList(interp, argv[1], &filter_listc, &filter_listv);
    for (i = 0; i < filter_listc; i++) {
        Tcl_SplitList(interp, filter_listv[i], &filterc, &filterv);
        
        /* 
         * If we are passed a single two-element list, the second 
         * SplitList was a no-op. We need to check for this case
         * and modify our working variables accordingly.
         */
        if (strcmp(filterv[0], filter_listv[0]) == 0) {
            filterc = filter_listc;
            tmp = filterv;
            filterv = filter_listv;
            filter_listv = tmp;
            /* Change our loop index so we don't run through here again. */
            i = filter_listc;
        }

        if (filterc > 2) {
            DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, filterc, filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* Check if the supplied filter type is valid */
        for (j = 0; j < aud_e_esl_max; j++) {
            if (strcmp((char *)Type[j], filterv[0]) == 0) {
                break;
            }
            if (j == (aud_e_esl_max - 1)) {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_type, filterv[0]);
                free((char *)filterv);
                free((char *)filter_listv);
                return TCL_ERROR;
            }
        }

        /* See if the user didn't supply a key where appropriate */
        if ((filterv[1] == NULL) && (!(WORLD_TYPE(j)))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_need_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* See if the user supplied an extra key. */
        if ((filterv[1] != NULL) && (WORLD_TYPE(j))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_extra_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }
        
        /* Fill in the subject name with the event type on world types */
        if (WORLD_TYPE(j)) {
            subject_name = dce_strdup(Type[j]);
        }
        else {
            subject_name = dce_strdup(filterv[1]);
        }

        free((char *)filterv);

        audit_control_delete_filter(dcp_aud_binding_h, 
                                    j, 
                                    (unsigned char *)subject_name, 
                                    &st);
        DCP_CHECK_AUD_ERROR_FREE_TWO(st, filter_listv, &subject_name);
    }
    
    free((char *)filter_listv);
    free(subject_name);
    return TCL_OK;
}


/*
 * FUNCTION:    audfilter_modify
 *
 * OVERVIEW:    Add or remove one or more guides of a filter. The argument is 
 *              a list of filter names. Takes the switches -add or -remove
 *              to preform the requestio operation. 
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
audfilter_modify(ClientData clientData, Tcl_Interp *interp, 
                 int argc, char **argv)
{
    char                        *add_list = NULL;
    char                        *remove_list = NULL;
    char                        *attr_list = NULL;
    aud_esl_guides_p_t          guide = NULL;
    aud_esl_guides_p_t          prev_guides = NULL;
    int                         i, j, filter_listc, filterc, pass;
    int                         k, l, guidec, guide_listc;
    int                         m, condc, actionc, eclistc;
    char                        **filter_listv, **filterv, **tmp;
    char                        **guidev, **guide_listv;
    char                        **condv, **actionv, **eclistv;
    char                        *subject_name;
    unsigned32                  evt_class;
    boolean32                   multi_guides;
    aud_esl_evt_classes_p_t     ec_ptr;
    error_status_t              st;

    dcp_ArgvInfo arg_table[] = {
        {"-add", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_audfilter_modify_add_help},
        {"-remove", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_audfilter_modify_remove_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&add_list;
    arg_table[1].dst = (char *)&remove_list;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Did the user not specify a filter? */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    } 
    
    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Make sure that the user specified at least one option */
    if ((add_list == NULL) && (remove_list == NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_audfilter_modify_need_option);
        return TCL_ERROR;
    }

    /* Bind to the server */
    if (aud_bind_to_server(interp, NULL) == TCL_ERROR) {
        return TCL_ERROR;
    }

    Tcl_SplitList(interp, argv[1], &filter_listc, &filter_listv);
    for (i = 0; i < filter_listc; i++) {
        Tcl_SplitList(interp, filter_listv[i], &filterc, &filterv);
        
        /* 
         * If we are passed a single two-element list, the second 
         * SplitList was a no-op. We need to check for this case
         * and modify our working variables accordingly.
         */
        if (strcmp(filterv[0], filter_listv[0]) == 0) {
            filterc = filter_listc;
            tmp = filterv;
            filterv = filter_listv;
            filter_listv = tmp;
            /* Change our loop index so we don't run through here again. */
            i = filter_listc;
        }

        if (filterc > 2) {
            DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, filterc, filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* Check if the supplied filter type is valid */
        for (j = 0; j < aud_e_esl_max; j++) {
            if (strcmp((char *)Type[j], filterv[0]) == 0) {
                break;
            }
            if (j == (aud_e_esl_max - 1)) {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_type, filterv[0]);
                free((char *)filterv);
                free((char *)filter_listv);
                return TCL_ERROR;
            }
        }

        /* See if the user didn't supply a key where appropriate */
        if ((filterv[1] == NULL) && (!(WORLD_TYPE(j)))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_need_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* See if the user supplied an extra key. */
        if ((filterv[1] != NULL) && (WORLD_TYPE(j))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_extra_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }
        
        /* Fill in the subject name with the event type on world types */
        if (WORLD_TYPE(j)) {
            subject_name = dce_strdup(Type[j]);
        }
        else {
            subject_name = dce_strdup(filterv[1]);
        }

        free((char *)filterv);
        
        /* Run the following code for -add and -remove} */
        /* pass == 0 is the add pass, pass == 1 is the remove pass */
        for (pass = 0; pass < 2; pass++) {
            if (pass == 0) {
                attr_list = add_list;
            }
            else {
                attr_list = remove_list;
            }

            if (attr_list == NULL) {
                continue;
            }

            /* Initialize the guides list */
            guide = NULL;
            prev_guides = NULL;

            /* 
             * This is the tricky part. We need to figure out what kind
             * of list we were passed. We will do multiple split lists until
             * we can determine if we have one guide or multiple ones.
             */
            multi_guides = dcp_aud_check_multi_guides(interp, attr_list);
            
            Tcl_SplitList(interp, attr_list, &guide_listc, &guide_listv);
            for (k = 0; k < guide_listc; k++) {
                Tcl_SplitList(interp, guide_listv[k], &guidec, &guidev);
                
                /* Create a new guide on the guides list */
                guide = (aud_esl_guides_p_t )malloc(sizeof(aud_esl_guides_t));
                guide->next = prev_guides;
                guide->ec_list = NULL;
                prev_guides = guide;
                
                /* 
                 * If we are passed a single guide as a list, we need to
                 * only do two split lists. We will undo the effects of the
                 * second split list, and do the third (really second) split
                 * later.
                 */
                if (!multi_guides) {
                    guidec = guide_listc;
                    tmp = guidev;
                    guidev = guide_listv;
                    guide_listv = tmp;
                    /* 
                     * Change our loop index so we don't run through 
                     * here again. 
                     */
                    k = guide_listc;
                }
                
                if (guidec > 3) {
                    DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, guidec, guidev);
                    free((char *)filter_listv);
                    free((char *)guide_listv);
                    free(subject_name);
                    aud_esl_free_guides(guide);
                    return TCL_ERROR;
                }
                
                if (guidec < 3) {
                    DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_short, guidec, guidev);
                    free((char *)filter_listv);
                    free((char *)guide_listv);
                    free(subject_name);
                    aud_esl_free_guides(guide);
                    return TCL_ERROR;
                }
                
                /* Verify that the event class list is valid */
                guide->ec_list = NULL;
                Tcl_SplitList(interp, guidev[0], &eclistc, &eclistv);
                for (l = 0; l < eclistc; l++) {
                    if (get_ec_number_from_name(eclistv[l], 
                                                &evt_class) == TRUE) {
                        ec_ptr = (aud_esl_evt_classes_p_t )
                            malloc(sizeof(aud_esl_evt_classes_t));
                        ec_ptr->evt_class = evt_class;
                        ec_ptr->next = guide->ec_list;
                        guide->ec_list = ec_ptr;
                    }
                    else {
                        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_ec, eclistv[l]);
                        free((char *)filter_listv);
                        free((char *)guidev);
                        free((char *)guide_listv);
                        free((char *)eclistv);
                        free(subject_name);
                        aud_esl_free_guides(guide);
                        return TCL_ERROR;
                    }
                }
                free((char *)eclistv);

                /* Verify that the condition is valid */
                guide->audit_condition = 0;
                Tcl_SplitList(interp, guidev[1], &condc, &condv);
                for (l = 0; l < condc; l++) {
                    for (m = 0; m < AUD_MAX_CONDITIONS; m++) {
                        if (strcmp(condv[l], 
                                   (char *)aud_g_cp_condition[m].name) == 0) {
                            guide->audit_condition 
                                |= aud_g_cp_condition[m].number;
                            break;
                        }
                        if (m == AUD_MAX_CONDITIONS - 1) {
                            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_condition,
                                                        condv[l]);
                            free((char *)filter_listv);
                            free((char *)guidev);
                            free((char *)guide_listv);
                            free((char *)condv);
                            free(subject_name);
                            aud_esl_free_guides(guide);
                            return TCL_ERROR;
                        }
                    }
                }
                free((char *)condv);
                
                /* Verify that the action is valid */
                guide->audit_action = 0;
                Tcl_SplitList(interp, guidev[2], &actionc, &actionv);
                for (l = 0; l < actionc; l++) {
                    for (m = 0; m < AUD_MAX_ACTIONS; m++) {
                        if (strcmp(actionv[l], 
                                   (char *)aud_g_cp_action[m].name) == 0) {
                            guide->audit_action |= aud_g_cp_action[m].number;
                            break;
                        }
                        if (m == AUD_MAX_ACTIONS - 1) {
                            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_action,
                                                        actionv[l]);
                            free((char *)filter_listv);
                            free((char *)guidev);
                            free((char *)guide_listv);
                            free((char *)actionv);
                            free(subject_name);
                            aud_esl_free_guides(guide);
                            return TCL_ERROR;
                        }
                    }
                }
                free((char *)actionv);
                
            } /* for (k < guide_listc) */

            if (pass == 0) {                /* Add pass */
                /* Make the RPC to add the filter */
                audit_control_add_filter(dcp_aud_binding_h, 
                                         j, 
                                         (unsigned char *)subject_name,
                                         &guide, 
                                         &st);
                DCP_CHECK_AUD_ERROR_FREE_FOUR(st,
                                              filter_listv,
                                              guidev,
                                              guide_listv,
                                              &subject_name);
            }
            else {                          /* Remove pass */
                /* Make the RPC to remove the filter */
                audit_control_remove_filter(dcp_aud_binding_h, 
                                            j, 
                                            (unsigned char *)subject_name,
                                            &guide, 
                                            &st);
                DCP_CHECK_AUD_ERROR_FREE_FOUR(st,
                                              filter_listv,
                                              guidev,
                                              guide_listv,
                                              &subject_name);
            }
            
            aud_esl_free_guides(guide);
        } /* for (i < filter_listc) */
    } /* for (pass < 2) */
    
    free((char *)filter_listv);
    free((char *)guidev);
    free((char *)guide_listv);
    free(subject_name);
    return TCL_OK;
}


/*
 * FUNCTION:    audfilter_show
 *
 * OVERVIEW:    Returns a list of guides in a specified filter.
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
audfilter_show(ClientData clientData, Tcl_Interp *interp, 
               int argc, char **argv)
{
    int                         i, j, filter_listc, filterc;
    char                        **filter_listv, **filterv, **tmp;
    char                        *subject_name;
    aud_esl_guides_p_t          guides;
    error_status_t              st;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Did the user not specify a filter name? */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    } 
    
    /* Bind to the server */
    if (aud_bind_to_server(interp, NULL) == TCL_ERROR) {
        return TCL_ERROR;
    }

    /* Check for empty argument */
    if (strlen(argv[1]) == 0) {
        DCP_SET_RESULT_CODE(dcp_s_null_arg);
        return TCL_ERROR;
    }

    Tcl_SplitList(interp, argv[1], &filter_listc, &filter_listv);
    for (i = 0; i < filter_listc; i++) {
        Tcl_SplitList(interp, filter_listv[i], &filterc, &filterv);
        
        /* 
         * If we are passed a single two-element list, the second 
         * SplitList was a no-op. We need to check for this case
         * and modify our working variables accordingly.
         */
        if (strcmp(filterv[0], filter_listv[0]) == 0) {
            filterc = filter_listc;
            tmp = filterv;
            filterv = filter_listv;
            filter_listv = tmp;
            /* Change our loop index so we don't run through here again. */
            i = filter_listc;
        }

        if (filterc > 2) {
            DCP_SET_AUD_ERROR_WITH_MERGE(dcp_s_list_too_long, filterc, filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* Check if the supplied filter type is valid */
        for (j = 0; j < aud_e_esl_max; j++) {
            if (strcmp((char *)Type[j], filterv[0]) == 0) {
                break;
            }
            if (j == (aud_e_esl_max - 1)) {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_invalid_type, filterv[0]);
                free((char *)filterv);
                free((char *)filter_listv);
                return TCL_ERROR;
            }
        }

        /* See if the user didn't supply a key where appropriate */
        if ((filterv[1] == NULL) && (!(WORLD_TYPE(j)))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_need_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }

        /* See if the user supplied an extra key. */
        if ((filterv[1] != NULL) && (WORLD_TYPE(j))) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audfilter_extra_key, filterv[0]);
            free((char *)filterv);
            free((char *)filter_listv);
            return TCL_ERROR;
        }
        
        /* Fill in the subject name with the event type on world types */
        if (WORLD_TYPE(j)) {
            subject_name = dce_strdup(Type[j]);
        }
        else {
            subject_name = dce_strdup(filterv[1]);
        }

        free((char *)filterv);

        guides = NULL;
        audit_control_show_filter(dcp_aud_binding_h, 
                                  j, 
                                  (unsigned char *)subject_name, 
                                  &guides, 
                                  &st);
        DCP_CHECK_AUD_ERROR_FREE_TWO(st, filter_listv, &subject_name);
        
        if (guides != NULL) {
            aud_list_guides(interp, guides);
            aud_esl_free_guides(guides);
        }
        else {
            DCP_SET_RESULT_CODE(dcp_s_audfilter_no_filter);
            free((char *)filter_listv);
            free(subject_name);
            return TCL_ERROR;
        }

        if ((filter_listc > 1) && (i < (filter_listc - 1))) {
            Tcl_AppendResult(interp, "\n", (char *)NULL);
        }

    } /* for (i < filter_listc) */

    free((char *)filter_listv);
    free(subject_name);
    return TCL_OK;
}




/** 
 ** 
 **   DCECP audevents operations
 ** 
 **/


/*
 * FUNCTION:    audevents_catalog
 *
 * OVERVIEW:    Returns a list of the names of all event classes.
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
audevents_catalog(ClientData clientData, Tcl_Interp *interp, 
                  int argc, char **argv)
{
    int                         i, ec_count;
    char                        **ec_names;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* This command accepts no arguments. If there are any arguments, fail. */
    if (argc != 1) {
        if (argv[1][0] == '-') {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_opt, argv[1]);
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_arg, argv[1]);
        }
        return TCL_ERROR;
    }

    if (dcp_aud_init == FALSE) {
        /* Init svc */
        dce_aud_init_svc();
        
        /* Init ec list */
        input_ec_table();
        
        dcp_aud_init = TRUE;
    }

    /* Return the list of event classes */
    if (get_ec_names(&ec_count, &ec_names) == TRUE) {
        for (i = 0; i < ec_count; i++) {
            DCP_APPEND_RESULT(ec_names[i]);
        }
    }

    return TCL_OK;
}


/*
 * FUNCTION:    audevents_show
 *
 * OVERVIEW:    Returns the contents of an event class.
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
audevents_show(ClientData clientData, Tcl_Interp *interp, 
               int argc, char **argv)
{
    int                         i, j, ev_count, listc;
    unsigned32                  ec_number, *events;
    char                        tmp[32], **listv;

    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for insufficient arguments */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    }

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Check for empty argument */
    if (strlen(argv[1]) == 0) {
        DCP_SET_RESULT_CODE(dcp_s_null_arg);
        return TCL_ERROR;
    }

    if (dcp_aud_init == FALSE) {
        /* Init svc */
        dce_aud_init_svc();
        
        /* Init ec list */
        input_ec_table();
        
        dcp_aud_init = TRUE;
    }

    Tcl_SplitList(interp, argv[1], &listc, &listv);
    for (i = 0; i < listc; i++) {
        if (get_ec_events(listv[i], &ec_number, &ev_count, &events) == TRUE) {
            if (i != 0) {
                Tcl_AppendResult(interp, "\n", (char *)NULL);
            }
            sprintf(tmp, "{%s ", listv[i]);
            Tcl_AppendResult(interp, tmp, (char *)NULL);
            for (j = 0; j < ev_count; j++) {
                sprintf(tmp, "0x%lx", events[j]);
                if (j != 0) {
                    Tcl_AppendResult(interp, " ", (char *)NULL);
                }
                Tcl_AppendResult(interp, tmp, (char *)NULL);
            }
            Tcl_AppendResult(interp, "}", (char *)NULL);
            free(events);
        }
        else {
            DCP_SET_RESULT_CODE(dcp_s_audevents_get_events_fail);
            free((char *)listv);
            return TCL_ERROR;
        }
    } /* for (i < listc) */

    free((char *)listv);
    return TCL_OK;
}



/** 
 ** 
 **   DCECP audtrail operations
 ** 
 **/


/*
 * FUNCTION:    audtrail_show
 *
 * OVERVIEW:    Convert the audit trail into a human readable format.
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
audtrail_show(ClientData clientData, Tcl_Interp *interp, 
              int argc, char **argv)
{
    char                        *to_file = NULL;
    char                        *expand_to_file = NULL;
    char                        *expand_trail_file = NULL;
    Tcl_DString                 buffer, outbuff;
    dce_aud_trail_t             trail;
    dce_aud_rec_t               ard;
    unsigned_char_t             *buff;
    char                        **listv;
    int                         fd, i, listc;
    boolean32                   done;
    error_status_t              st, st2;

    dcp_ArgvInfo arg_table[] = {
        {"-to", DCP_ARGV_STRING, NULL, NULL,
             dcp_t_audtrail_show_to_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&to_file;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Did the user not specify a trail file? */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    } 

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* If an output file was specified */
    if (to_file) {
        expand_to_file = Tcl_TildeSubst(interp, to_file, &buffer);
        if (expand_to_file == NULL) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audtrail_outfile_open_fail, to_file);
            return TCL_ERROR;
        }
        fd = open(expand_to_file, O_CREAT | O_RDWR, 0400);
        if (fd == -1) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_audtrail_outfile_open_fail, to_file);
            return TCL_ERROR;
        }
        Tcl_DStringFree(&buffer);
    }

    /* Initialize the output buffer string */
    if (!to_file) {
        Tcl_DStringInit(&outbuff);
    }

    /* Cycle through the trail files specified */
    Tcl_SplitList(interp, argv[1], &listc, &listv);
    for (i = 0; i < listc; i++) {
        expand_trail_file = Tcl_TildeSubst(interp, listv[i], &buffer);
        dce_aud_open(aud_c_trl_open_read, expand_trail_file, 
                     0, 0, &trail, &st);
        if (st != aud_s_ok) {
            if (to_file) {
                close(fd);
            }
            if (st == aud_s_cannot_open_trail_file_rc) {
                DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_cannot_open_trail_file, listv[i]);
                free((char *)listv);
                return TCL_ERROR;
            }
            else {
                DCP_CHECK_AUD_ERROR_FREE_ONE(st, listv);
            }
        }

        done = FALSE;
        while (!done) {
            dce_aud_next(trail, NULL, 0, &ard, &st);
            if (st != aud_s_ok) {
                if (to_file) {
                    close(fd);
                }
                dce_aud_close(trail, &st2);
                if (st == aud_s_trail_file_corrupted) {
                    DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_aud_trail_file_corrupt, listv[i]);
                    free((char *)listv);
                    return TCL_ERROR;
                }
                else {
                    DCP_CHECK_AUD_ERROR_FREE_ONE(st, listv);
                }
            }

            if (ard == NULL) {
                done = TRUE;
                break;
            }

            dce_aud_print(ard, aud_c_evt_all_info, &buff, &st);
            if (st != aud_s_ok) {
                if (to_file) {
                    close(fd);
                }
                dce_aud_close(trail, &st2);
                DCP_CHECK_AUD_ERROR_FREE_ONE(st, listv);
            }

            if (to_file) {
                write(fd, buff, strlen((char *)buff));
            }
            else {
                Tcl_DStringAppend(&outbuff, (char *)buff, -1);
            }
            free(buff);

            dce_aud_discard(ard, &st);
            if (st != aud_s_ok) {
                if (to_file) {
                    close(fd);
                }
                dce_aud_close(trail, &st2);
                DCP_CHECK_AUD_ERROR_FREE_ONE(st, listv);
            }
        } /* while (!done) */

        Tcl_DStringFree(&buffer);
        dce_aud_close(trail, &st);
        if (st != aud_s_ok) {
            if (to_file) {
                close(fd);
            }
            DCP_CHECK_AUD_ERROR_FREE_ONE(st, listv);
        }
    } /* if (i < listc) */

    /* Clean up */
    if (to_file) {
        close(fd);
    }
    else {
        Tcl_DStringResult(interp, &outbuff);
    }

    free((char *)listv);

    return TCL_OK;
}
