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
 * $Log: util_help.c,v $
 * Revision 1.1.6.3  1996/02/18  19:15:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:26  marty]
 *
 * Revision 1.1.6.2  1995/12/08  21:25:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/14  16:34 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  16:33 UTC  truitt
 * 	CHFts14285: Replace direct set of error with call
 * 	to correct macro.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/10  19:01 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts11/1  1995/02/10  18:59 UTC  truitt
 * 	CHFts14285 partial: Add support to pass the error
 * 	code back to main, instead of just the error message.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  17:24 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/12/05  17:23 UTC  truitt
 * 	OT12675: Fix bad pointer in help messages when
 * 	the action is ambiguous.
 * 	[1995/12/08  17:58:19  root]
 * 
 * Revision 1.1.2.9  1994/09/30  16:54:04  kevins
 * 	OT 12408 removed verbose help processing
 * 	[1994/09/30  16:53:43  kevins]
 * 
 * Revision 1.1.2.8  1994/09/28  15:15:58  rousseau
 * 	Fix <command> help operations (CR 12386).
 * 	[1994/09/28  15:15:28  rousseau]
 * 
 * Revision 1.1.2.7  1994/09/21  20:46:27  kevins
 * 	OT 12256 added help help capability
 * 	[1994/09/21  20:35:39  kevins]
 * 
 * Revision 1.1.2.6  1994/08/21  19:45:33  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:50  melman]
 * 
 * Revision 1.1.2.5  1994/08/18  17:43:53  rousseau
 * 	Implemented help -verbose (CR 11559).
 * 	[1994/08/18  17:42:40  rousseau]
 * 
 * Revision 1.1.2.4  1994/07/28  20:34:53  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:30  rousseau]
 * 
 * Revision 1.1.2.3  1994/06/09  16:02:36  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:13  devsrc]
 * 
 * Revision 1.1.2.2  1994/02/10  21:46:58  rousseau
 * 	Fixes for help facility problems. CR's 9917 9918 9919 9920
 * 	[1994/02/10  21:43:41  rousseau]
 * 
 * Revision 1.1.2.1  1994/02/02  19:43:58  rousseau
 * 	Initial version.
 * 	[1994/02/02  19:39:20  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: util_help.c
 *
 * DESCRIPTION:
 *   This module contains the functions that implement the various dcecp
 * help utilities.
 *
 */

/* INCLUDE FILES  */

#include <stdlib.h>
#include <string.h>
#include <util_help.h>
#include <util_var.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

/* PROTOTYPES */


/*
 * FUNCTION:    dcp_object_help
 *
 * OVERVIEW:    This function will either display a help message for each
 *              verb in an object (OBJ help), or set up for and call the
 *              individial command for option help (OBJ help VERB).
 * 
 *
 * INPUTS:
 *  clientData
 *    Client specific data.
 *
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 *  arg_table
 *    Array of dcp_ArgvInfo entries for command line parsing.
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
dcp_object_help(ClientData clientData,
                Tcl_Interp *interp,
                int argc,
                char **argv,
                dcp_ArgvInfo arg_table[],
                unsigned32 verbose_tag)
{
    int                         i = 0;
    int                         match, match_loc;
    char                        tmp[MAX_HELP_MESSAGE_SIZE];
    char                        *msg;
    boolean32                   help_flag = FALSE;
    boolean32                   verbose_flag = FALSE;
    boolean32                   operations_flag = FALSE;

    static int (*func)(ClientData, Tcl_Interp *, int, char **) = NULL;
    dcp_ArgvInfo local_arg_table[] = {
        {"-verbose", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_util_help_verbose_help},
        {"help", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, (int )NULL},
        {"operations", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, (int )NULL},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    local_arg_table[0].dst = (char *)&verbose_flag;
    local_arg_table[1].dst = (char *)&help_flag;
    local_arg_table[2].dst = (char *)&operations_flag;

    if (dcp_ParseArgv(interp, &argc, argv, local_arg_table, 0) != TCL_OK) {
        return TCL_ERROR;
    }

    /* 
     * Check to see if the user entered '<object> help help' or 
     * 'help <object> help' as a command.
     */

    if (help_flag) {
        sprintf(tmp,"%-20s%s", local_arg_table[0].key,
                    (char *)dce_sprintf(local_arg_table[0].helpcode));
        DCP_APPEND_RESULT(tmp);
        return TCL_OK;
    }

    /* Catch the "operations" special case */
    if (operations_flag) {
        return TCL_OK;
    }

    /* 
     * The command takes the -verbose switch, an argument specifying
     * an operation to get help for, or no argument. -verbose can not
     * be specified with an argument.
     */

    if (verbose_flag) {
        if (argc != 1) {
            DCP_SET_RESULT_CODE(dcp_s_help_no_verbose);
            return TCL_ERROR;
        }
    } 
    else {
        /* Check for extraneous arguments */
        DCP_CHECK_EXTRA_ARGS();
    } /* if (verbose_flag) */

    /* If anything is left in argv, then we are doing option help.
     * If we are doing option help, set clientData and call the command.
     * The command will generate it's own help info and return it to us
     * in interp.
     * If we are not doing option help, generate the help message from
     * the arg_table passed here, and return it in interp.
     * If the verbose flag is set, call the verbose help routine.
     */

    if (argc < 2) {
        if (verbose_flag) {
            msg = (char *)dce_sprintf(verbose_tag);
            DCP_APPEND_RESULT(msg);
            free(msg);
            return TCL_OK;
        }

        i = 0;
        while (arg_table[i].type != DCP_ARGV_END) {
            msg =  (char *)dce_sprintf(arg_table[i].helpcode);
            sprintf(tmp, "%-20s%s", arg_table[i].key,msg);
            free(msg);
            DCP_APPEND_RESULT(tmp);
            i++;
        }
        msg = (char *)dce_sprintf(dcp_t_operations_help);
        sprintf(tmp, "%-20s%s", "operations",msg);
        free(msg);
        DCP_APPEND_RESULT(tmp);
        return TCL_OK;
    }
    else {
        clientData = (char *)HELP_TOKEN;
        i = 0;
        match = 0;
        match_loc = 0;
        while (arg_table[i].type != DCP_ARGV_END) {
            if (arg_table[i + 1].type == DCP_ARGV_END) {
                break;
            }
            if (strncmp(arg_table[i].key, argv[1], strlen(argv[1])) == 0) {
                match++;
                match_loc = i;
            }
            i++;
        } /* while (!DCP_ARGV_END) */

        if (match == 1) {
            func = arg_table[match_loc].src;
            return (*func)(clientData, interp, argc, argv);
        }

        /* If we get here, the command passed was bad. */
        if (argv[1][0] == '-') {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_opt, argv[1]);
        }
        else if (match > 1) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_ambiguous_arg, argv[1]);
            i = 0;
            while (arg_table[i].type != DCP_ARGV_END) {
                if (arg_table[i+1].type == DCP_ARGV_END) {
                    break;
                }
                if (strncmp(arg_table[i].key, argv[1], strlen(argv[1])) == 0) {
                    Tcl_AppendResult(interp, " ", arg_table[i].key, (char *)NULL);
                }
                i++;
            }
        }
        else {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_bad_arg, argv[1]);
        }
        return TCL_ERROR;
    } /* if (argc < 2) */
}
