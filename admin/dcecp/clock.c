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
 * $Log: clock.c,v $
 * Revision 1.1.8.1  1996/08/09  11:46:05  arvind
 * 	Merge changes.
 * 	[1996/06/10  18:07 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/27  14:02 UTC  truitt  /main/HPDCE02/7]
 *
 * 	Merge protocol changes.
 * 	[1996/02/27  13:49 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf64/1]
 *
 * 	CHFts17590: Remove hard-coded DG protocol in rpc call in clock_show.
 * 	[1995/02/26  16:30 UTC  gilda  /main/HPDCE02/6]
 *
 * 	Merge changes.
 *
 * 	Revision /main/HPDCE02/gilda_dcecp_bug1  1996/02/21  18:08 UTC gilda
 * 	[1995/02/26  16:30 UTC  gilda  /main/HPDCE02/6]
 *
 * 	Revision /main/HPDCE02/gilda_dcecp_bug1  1996/02/21  18:08 UTC gilda
 * 	CHFts16998: Change the 'clock synchronize' code so that the
 * 	'-abruptly' option will work if the '-dtsd' option is specified.
 * 	[1996/02/21  18:08 UTC  gilda  /main/HPDCE02/gilda_dcecp_bug1/2]
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/10  14:59 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.2  1996/02/18  19:13:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:59:51  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  21:20:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/10/20  21:21 UTC  jrr
 * 	Add a (nop) -dtsd option to the clock show and clock synchronize
 * 	commands for consistency.
 * 	[1995/10/20  21:10 UTC  jrr  /main/HPDCE02/jrr_dcecp_01/1]
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/06  13:55 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:32 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/27  18:30 UTC  truitt
 * 	Merge clock show changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts16/1  1995/02/27  18:29 UTC  truitt
 * 	Have clock show figure out the hostname if it not present.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/14  15:59 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts13/1  1995/02/14  15:58 UTC  truitt
 * 	CHFts14285: When setting error codes, use the right
 * 	macros, to ensure the _e var sets set correctly.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/07  18:17 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_01/1  1994/12/07  18:15 UTC  truitt
 * 	Add capability to retrieve system time via dced and
 * 	inetd, thus bypassing dtsd.
 * 	[1995/12/08  17:54:55  root]
 * 
 * Revision 1.1.4.15  1994/09/26  20:25:41  kevins
 * 	OT 11780 modified clock compare and dts catalog commands.
 * 	[1994/09/26  20:19:45  kevins]
 * 
 * Revision 1.1.4.14  1994/09/23  23:18:46  rousseau
 * 	Internal rebinding changes (CR 12282).
 * 	[1994/09/23  23:18:21  rousseau]
 * 
 * Revision 1.1.4.13  1994/09/01  14:16:46  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:05  rousseau]
 * 
 * Revision 1.1.4.12  1994/08/12  16:04:38  rousseau
 * 	Call the correct RPC for epoch changes (CR 11555).
 * 	[1994/08/12  16:04:30  rousseau]
 * 
 * Revision 1.1.4.11  1994/08/03  21:48:12  rousseau
 * 	Allow epoch to be changed w/o setting time (CR 11390).
 * 	[1994/08/03  21:47:43  rousseau]
 * 
 * Revision 1.1.4.10  1994/07/28  20:34:45  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:16  rousseau]
 * 
 * Revision 1.1.4.9  1994/07/19  18:09:50  rousseau
 * 	Added 'dts catalog' and 'clock compare' (CR 11249 11251).
 * 	[1994/07/19  18:08:49  rousseau]
 * 
 * Revision 1.1.4.8  1994/06/09  16:01:47  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:12  devsrc]
 * 
 * Revision 1.1.4.7  1994/05/20  20:41:43  rousseau
 * 	Make clock show and clock set talk to remote daemons.
 * 	[1994/05/20  20:37:07  rousseau]
 * 
 * Revision 1.1.4.6  1994/05/16  20:47:39  rousseau
 * 	Fix build problem (CR 10642).
 * 	[1994/05/16  20:39:13  rousseau]
 * 
 * Revision 1.1.4.5  1994/05/13  22:22:39  melman
 * 	renamed dcecp's dts.h to dcpdts.h
 * 	[1994/05/13  22:18:16  melman]
 * 
 * Revision 1.1.4.4  1994/05/09  20:42:54  rousseau
 * 	Updated to new control interface. Enabled talking to remote daemons (CR 10434).
 * 	[1994/05/09  20:42:47  rousseau]
 * 
 * Revision 1.1.4.3  1994/03/17  21:34:57  rousseau
 * 	Removed overly restrictive check for authentication (CR 10144).
 * 	[1994/03/17  21:34:39  rousseau]
 * 
 * Revision 1.1.4.2  1994/02/10  21:47:02  rousseau
 * 	Fixes for help facility problems. CR's 9917 9918 9919 9920
 * 	[1994/02/10  21:43:46  rousseau]
 * 
 * Revision 1.1.4.1  1994/02/02  19:43:47  rousseau
 * 	Changed to support help functions.
 * 	[1994/02/02  19:42:15  rousseau]
 * 
 * Revision 1.1.2.3  1993/12/02  03:30:37  rousseau
 * 	Changed TCL_DYNAMIC to TCL_VOLATILE in setting the result of show.
 * 	[1993/12/02  03:30:24  rousseau]
 * 
 * Revision 1.1.2.2  1993/11/29  17:57:22  rousseau
 * 	Tightened up error checking on user input.
 * 	[1993/11/29  17:57:16  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/15  00:23:49  rousseau
 * 	Initial version.
 * 	[1993/11/15  00:23:44  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: clock.c
 *
 * DESCRIPTION:
 *   This module contains all of the functions that implement the
 *   dcecp clock commands. Helper functions are found in dts_util.c.
 *
 */

/* INCLUDE FILES  */

#include <dce/dtss_ctrl.h>
#include <dce/dtss_includes.h>
#include <dce/rutc.h>
#include <dcecp.h>
#include <dcedcpmsg.h>
#include <util_help.h>
#include <dcp_dts.h>
#include <dts_proto.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

/* PROTOTYPES */


/*
 * FUNCTION: clock_set
 *
 * OVERVIEW: Gradually adjusts the system clock to the time specified by
 *    the supplied time.  The supplied time must be in the form of a DTS
 *    style timestamp.  This command takes two switches, -abruptly specifies
 *    that the system clock is to be set immediately to the specified time.
 *    If the -abruptly switch is specified, the user must also specify a new
 *    epoch with the -epoch switch.  Valid values for -epoch are 0...255.
 *    The epoch can be changed without changing the time.  When doing this,
 *    the abuptly switch is not allowed (it makes no sense), so the command
 *    "clock set -epoch 2" is a valid command.
 *    The -bypass switch will be used to allow the user to set the system
 *    clock on the machine without using dtsd.
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
clock_set (ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    boolean32       abrupt_flag = FALSE;
    boolean32       bypass_flag = FALSE;
    boolean32       epoch_change = FALSE;
    int             epoch = -1;
    int             ret;
    char            *to_time = NULL;
    char            *hostname;
    utc_t           time_value;
    time_t          new_time = 0L;
    timespec_t      time_spec;
    Change_t        change_dir;
    int             retry;
    StatusBlk_t     st_block;
    error_status_t  comm_st;

    dcp_ArgvInfo arg_table[] = {
        {"-abruptly", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_clock_set_abruptly_help},
        {"-bypass", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_clock_set_bypass_help},
        {"-epoch", DCP_ARGV_INT, NULL, NULL, 
             dcp_t_clock_set_epoch_help},
        {"-to", DCP_ARGV_STRING, NULL, NULL, 
             dcp_t_clock_set_to_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&abrupt_flag;
    arg_table[1].dst = (char *)&bypass_flag;
    arg_table[2].dst = (char *)&epoch;
    arg_table[3].dst = (char *)&to_time;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Did the user specify the bypass switch along with others?
     */
    if ((bypass_flag == TRUE) && ((epoch != -1) || (abrupt_flag == TRUE))) {
        DCP_SET_RESULT_CODE(dcp_s_clock_set_bypass_and_others);
        return TCL_ERROR;
    }

    /*
     * Did the user not specify a time, and isn't just changing the epoch?
     */
    if ((to_time == NULL) && (epoch == -1)) {
        DCP_SET_RESULT_CODE(dcp_s_clock_set_need_time);
        return TCL_ERROR;
    }

    if ((to_time == NULL) && (epoch > -1)) {
        epoch_change = TRUE;
    }

    if (epoch_change) {
        /*
         * Did the user specify the abrupt flag with no time?
         */
        if (abrupt_flag) {
            DCP_SET_RESULT_CODE(dcp_s_clock_set_epoch_no_abrupt);
            return TCL_ERROR;
        }
    } else {
        /*
         * Did the user specify one switch without the other?
         */
        if (abrupt_flag && (epoch == -1)) {
            DCP_SET_RESULT_CODE(dcp_s_clock_set_need_epoch);
            return TCL_ERROR;
        }

        if (!abrupt_flag && (epoch != -1)) {
            DCP_SET_RESULT_CODE(dcp_s_clock_set_need_abruptly);
            return TCL_ERROR;
        }
    }

    /*
     * Check for extraneous arguments
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Did the user specify a valid epoch?
     */
    if (abrupt_flag && ((epoch < 0) || (epoch > 255))) {
        DCP_SET_RESULT_CODE(dcp_s_clock_set_invalid_epoch);
        return TCL_ERROR;
    }   

    /*
     * Did the user enter a valid timestamp?
     */
    if (!epoch_change) {
        if (utc_mkasctime(&time_value, to_time) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_clock_time_conversion_fail);
            return TCL_ERROR;
        }
    }

    /*
     * Check if the user specified a server to talk to.
     */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /*
     * If the bypass switch is used, then no server should be specified.
     */
    if ((bypass_flag == TRUE) && (hostname != NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_clock_set_bypass_and_hostname);
        return TCL_ERROR;
    }

    if (bypass_flag == FALSE) {
        /*
         * Initialize rpc communications with server.
         */
        if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
            return TCL_ERROR;
        }

        /*
         * Initialize the status block.
         */
        st_block.status = 0;
        st_block.sysStatus = 0;
        retry = 0;

        /*
         * Setup parameters, and call the correct RPC.
         */
        if (abrupt_flag) {
            /*
             * Set the time and epoch abruptly.
             */
            change_dir.newEpoch = epoch;
            change_dir.newTimePresent = TRUE;
            change_dir.newTime = time_value;
            for (;;) {
                ChangeCmd(dcp_dts_binding_h, &change_dir, &st_block, &comm_st);
                if (comm_st != rpc_s_ok) {
                    if (retry >= DCP_DTS_MAX_RETRIES) {
                        dts_check_status(interp, st_block, comm_st);
                        return TCL_ERROR;
                    }
                    else {
                        /* 
                         * Set up for another pass at the RPC, nuke the previous
                         * result and force a rebind.
                         */
                        retry++;
                        Tcl_ResetResult(interp);
                        dcp_dts_valid_binding = FALSE;
                        if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                            return TCL_ERROR;
                        }
                    }
                } 
                else {
                    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                        return TCL_ERROR;
                    }
                    break;
                }
            }
        }
        else if (epoch_change) {
            /*
             * Set the epoch.
             */
            change_dir.newEpoch = epoch;
            change_dir.newTimePresent = FALSE;
            for (;;) {
                ChangeCmd(dcp_dts_binding_h, &change_dir, &st_block, &comm_st);
                if (comm_st != rpc_s_ok) {
                    if (retry >= DCP_DTS_MAX_RETRIES) {
                        dts_check_status(interp, st_block, comm_st);
                        return TCL_ERROR;
                    }
                    else {
                        /* 
                         * Set up for another pass at the RPC, nuke the previous
                         * result and force a rebind.
                         */
                        retry++;
                        Tcl_ResetResult(interp);
                        dcp_dts_valid_binding = FALSE;
                        if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                            return TCL_ERROR;
                        }
                    }
                }
                else {
                    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                        return TCL_ERROR;
                    }
                    break;
                }
            }
        }
        else {
            /*
             * Set the time.
             */
            for (;;) {
                UpdateCmd(dcp_dts_binding_h, &time_value, &st_block, &comm_st);
                if (comm_st != rpc_s_ok) {
                    if (retry >= DCP_DTS_MAX_RETRIES) {
                        dts_check_status(interp, st_block, comm_st);
                        return TCL_ERROR;
                    }
                    else {
                        /* 
                         * Set up for another pass at the RPC, nuke the previous
                         * result and force a rebind.
                         */
                        retry++;
                        Tcl_ResetResult(interp);
                        dcp_dts_valid_binding = FALSE;
                        if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                            return TCL_ERROR;
                        }
                    }
                } 
                else {
                    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                        return TCL_ERROR;
                    }
                    break;
                }
            }
        }
    }
    else {
        /*
         * Bypass dtsd and hit the system clock directly.
         */
        if ((utc_bintime(&time_spec, (timespec_t *)0, (long *)0, &time_value)) < 0) {
            /*
             * Problem converting utc time to binary time.
             */
            DCP_SET_RESULT_CODE(dcp_s_utc_convert_failed);
            return TCL_ERROR;
        }
        new_time = time_spec.tv_sec;
        ret = stime(&new_time);
        if (ret != 0) {
            /*
             * Problem setting the system time.
             */
            DCP_SET_RESULT_CODE(dcp_s_clock_set_failed);
            return TCL_ERROR;
        }
    }
    return TCL_OK;
}


/*
 * FUNCTION: clock_show
 *
 * OVERVIEW: Returns a DTS style timestamp with the TDF indicated.
 *    Also, by using the -inetd and -dced switches along with a
 *    hostname, you can get the time on a node without using dtsd.
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
clock_show (ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char                  local_time[UTC_MAX_STR_LEN];
    char                  *hostname;
    boolean32             dced_flag = FALSE;
    boolean32             dtsd_flag = FALSE;
    boolean32             inetd_flag = FALSE;
    boolean32             got_binding;
    signed32              show_attr;
    AttrVal_t             attribute;
    int                   i, j, retry;
    utc_t                 utc;
    StatusBlk_t           st_block;
    error_status_t        tmp_st, comm_st;
    long int              ret;
    char                  *h_var = NULL;
    rpc_binding_handle_t  h;
    rpc_protseq_vector_t  *protseq_vector = NULL;
    ndr_char              *string_binding;
    rpc_if_id_t           if_id;
    rpc_ep_inq_handle_t   inquiry_context;
    rpc_binding_handle_t  out_binding;
    uuid_t                object_uuid;
    unsigned_char_p_t     annot;

    dcp_ArgvInfo arg_table[] = {
        {"-dced", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
            dcp_t_clock_show_dced_help},
        {"-dtsd", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
            dcp_t_clock_show_dtsd_help},
        {"-inetd", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
            dcp_t_clock_show_inetd_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&dced_flag;
    arg_table[1].dst = (char *)&dtsd_flag;
    arg_table[2].dst = (char *)&inetd_flag;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check for extraneous arguments.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Check if the user specified a server to talk to.
     */
    if (argc == 2) {
        hostname = dce_strdup(argv[1]);
    }
    else {
        hostname = NULL;
    }

    /*
     * Check that if the user wants to bypass dtsd and use inetd or dced
     * directly, then only one of the options can be selected at a time.
     */
    if ((dced_flag && inetd_flag) || 
        (dtsd_flag && inetd_flag) ||
        (dced_flag && dtsd_flag)) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        if (hostname) {
            free(hostname);
        }
        return TCL_ERROR;
    }

    /*
     * Check that if the user specified one of the -dced or the
     * -inetd switches, and the hostname is not present, try to
     * get local hostname from the OS.
     */
    if (((dced_flag == TRUE) || (inetd_flag == TRUE)) && (hostname == NULL)) {
        hostname = (char *)malloc(128);
        if (gethostname(hostname, 127) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_clock_missing_hostname);
            return TCL_ERROR;
        }
    }

    /*
     * Now show the time on the requested system based on
     * the method chosen; dced, inetd, or dtsd.
     */
    if (dced_flag == TRUE) {
        /*
         * Use dced to determine the system time.  First get a vector
         * of the supported protocols.
         */
        rpc_network_inq_protseqs(&protseq_vector, &comm_st);
        if ((protseq_vector == NULL) || (comm_st != rpc_s_ok))
            return TCL_ERROR;

        /*
         * Cycle through all of the available protocol sequences until 
         * one successfully binds. 
         */
        got_binding = FALSE;
        for (i = 0; i < protseq_vector->count; ++i) {
            rpc_string_binding_compose(NULL,
                                       protseq_vector->protseq[i],
                                       (unsigned_char_t *)hostname,
                                       NULL,
                                       NULL,
                                       &string_binding,
                                       &comm_st);
            if (comm_st == rpc_s_ok) {
                /*
                 * Get the rpc binding from the string binding.
                 */
                rpc_binding_from_string_binding(string_binding, &h, &comm_st);
                if (comm_st == rpc_s_ok) {
                    /*
                     * Got the rpc binding, free the string binding.
                     */
                    got_binding = TRUE;
                    rpc_string_free(&string_binding, &tmp_st);
                    rpc_protseq_vector_free(&protseq_vector, &tmp_st);
                    break;
                }
            }
        }
        if (!got_binding) {
            /*
             * Never did get a binding.  Free all the necessary
             * stuff, then check if an error was encountered.
             */
            if (hostname) {
                free(hostname);
            }
            if (string_binding) {
                rpc_string_free(&string_binding, &tmp_st);
            }
            rpc_protseq_vector_free(&protseq_vector, &tmp_st);
            DCP_CHECK_RPC_ERROR(comm_st);
        }

        /*
         * At this point, we should have a valid rpc binding, but
         * we might need more information about the rutc service
         * before we can use it, depending on the protocol being
         * used.  Get the interface id of the dced rutc service.
         */
        rpc_if_inq_id(rutc_v0_0_c_ifspec, &if_id, &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Get a list of the available endpoints using the rutc
         * interface id.
         */
        rpc_mgmt_ep_elt_inq_begin(h,
                                  rpc_c_ep_match_by_if,
                                  &if_id,
                                  rpc_c_vers_all,
                                  NULL,
                                  &inquiry_context,
                                  &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * See if we've found a valid match for the rutc interface
         * id.  If so, we'll get the object uuid we need.
         */
        rpc_mgmt_ep_elt_inq_next(inquiry_context,
                                 &if_id,
                                 &out_binding,
                                 &object_uuid,
                                 &annot,
                                 &comm_st);
        if (comm_st == rpc_s_no_more_elements) {
            DCP_SET_RESULT_CODE(dcp_s_clock_get_dced_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Clean up the binding and annotation variables,
         * and the inquiry context.
         */
        rpc_string_free(&annot, &tmp_st);
        rpc_binding_free(&out_binding, &tmp_st);
        rpc_mgmt_ep_elt_inq_done(&inquiry_context, &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Set the object uuid so the following call to the
         * rutc service will find the correct entry.
         */
        rpc_binding_set_object(h, &object_uuid, &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Make the call to the daemon.
         */
        ret = rutc_gettime(h, &utc, &comm_st);
        rpc_binding_free(&h, &tmp_st);
        if (comm_st != rpc_s_ok) {
            DCP_SET_RESULT_CODE(dcp_s_clock_get_dced_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }
        if (ret != 0) {
            DCP_SET_RESULT_CODE(dcp_s_clock_dced_server_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }

        if ((utc_asclocaltime(local_time, UTC_MAX_STR_LEN, &utc)) < 0) {
            /*
             * Problem converting utc time to ascii time.
             */
            DCP_SET_RESULT_CODE(dcp_s_clock_get_local_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }
        else {
            Tcl_SetResult(interp, local_time, TCL_VOLATILE);
        }
        if (hostname) {
            free(hostname);
            hostname = NULL;
        }
    }
    else if (inetd_flag == TRUE) {
        /*
         * Use inetd to determine the system time.
         */
        if ((utc_getTCPtime(hostname, &utc)) < 0) {
            /*
             * Getting the time via this call failed.
             */
            DCP_SET_RESULT_CODE(dcp_s_clock_get_inetd_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }
        else {
            /*
             * We successfully got the time in utc format.
             */
            if ((utc_asclocaltime(local_time, UTC_MAX_STR_LEN, &utc)) < 0) {
                /*
                 * Problem converting utc time to ascii time.
                 */
                DCP_SET_RESULT_CODE(dcp_s_clock_get_local_time_fail);
                if (hostname) {
                    free(hostname);
                }
                return TCL_ERROR;
            }
            else {
                Tcl_SetResult(interp, local_time, TCL_VOLATILE);
            }
        }
        if (hostname) {
            free(hostname);
            hostname = NULL;
        }
    }
    else {
        /*
         * Use dtsd to determine the system time.
         * Initialize rpc communications with server.
         */
        if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
            if (hostname) {
                return TCL_ERROR;
            }
        }

        /*
         * Initialize the status block.
         */
        st_block.status = 0;
        st_block.sysStatus = 0;
        retry = 0;

        attribute.Data.type = VT_Bintime;
        show_attr = K_CURRENT_TIME_ATTR;
        for (;;) {
            ShowAttrCmd(dcp_dts_binding_h, 
                        &show_attr,
                        &attribute,
                        &st_block,
                        &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    if (hostname) {
                        free(hostname);
                    }
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                        if (hostname) {
                            free(hostname);
                        }
                        return TCL_ERROR;
                    }
                }
            } 
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    if (hostname) {
                        free(hostname);
                    }
                    return TCL_ERROR;
                }
                break;
            }
        }

        if (utc_asclocaltime(local_time, UTC_MAX_STR_LEN,
                             (utc_t *)&attribute.Data.setData.userBinTime)) {
            DCP_SET_RESULT_CODE(dcp_s_clock_get_local_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }
        Tcl_SetResult(interp, local_time, TCL_VOLATILE);
    }
    if (hostname) {
        free(hostname);
    }
    return TCL_OK;
}


/*
 * FUNCTION: clock_compare
 *
 * OVERVIEW: A wrapper around a dcecp script the returns the difference
 *    between the clocks on the local machine and a time provider in the
 *    cell. If a time provider is not running, the command will pick the
 *    last responding server returned by 'dts catalog'.  Optionally takes
 *    an argument to specify a dts daemon to compare instead of the local
 *    host. Also takes an optional -server switch to specify a specific
 *    server to compare against.
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
clock_compare(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    char  cmd[1024];

    dcp_ArgvInfo arg_table[] = {
        {"-server", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
             dcp_t_clock_compare_server_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    sprintf(cmd, "_dcp_clock_compare %s", Tcl_Merge(argc, argv));
    return (Tcl_Eval(interp, cmd));
}


/*
 * FUNCTION: clock_synchronize
 *
 * OVERVIEW: Causes the current DTS entity to synchronize with DTS servers.
 *    servers.  The machine's clock is adjusted accordingly.  By default,
 *    the time will be adjusted gradually.  Specifying -abruptly will cause
 *    the time to be synchronized immediately.  Choosing one of the other
 *    options (-dced or -inetd) will cause the clock synchronization to be
 *    done using either of those two services, bypassing dtsd.
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
clock_synchronize(ClientData clientData, Tcl_Interp *interp, 
                  int argc, char **argv)
{
    utc_t                 utc;
    time_t                new_time = 0L;
    timespec_t            time_spec;
    boolean32             abrupt_flag = FALSE;
    boolean32             dced_flag = FALSE;
    boolean32             inetd_flag = FALSE;
    boolean32             dtsd_flag = FALSE;
    boolean32             got_binding;
    char                  *hostname;
    int                   i, retry;
    StatusBlk_t           st_block;
    error_status_t        tmp_st, comm_st;
    long int              ret;
    ndr_char              *string_binding;
    rpc_binding_handle_t  h;
    rpc_protseq_vector_t  *protseq_vector = NULL;
    rpc_if_id_t           if_id;
    rpc_ep_inq_handle_t   inquiry_context;
    rpc_binding_handle_t  out_binding;
    uuid_t                object_uuid;
    unsigned_char_p_t     annot;

    dcp_ArgvInfo arg_table[] = {
        {"-abruptly", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_dts_sync_abruptly_help},
        {"-dced", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
            dcp_t_clock_synch_dced_help},
        {"-dtsd", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
            dcp_t_clock_synch_dtsd_help},
        {"-inetd", DCP_ARGV_CONSTANT, (char *)TRUE, NULL, 
            dcp_t_clock_synch_inetd_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };

    arg_table[0].dst = (char *)&abrupt_flag;
    arg_table[1].dst = (char *)&dced_flag;
    arg_table[2].dst = (char *)&dtsd_flag;
    arg_table[3].dst = (char *)&inetd_flag;

    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;

    /*
     * Check for extraneous arguments.
     */
    DCP_CHECK_EXTRA_ARGS();

    /*
     * Check to see if the user just wants help.
     */
    DCP_CHECK_COMMAND_HELP();

    /*
     * Check if the user specified a server to talk to.
     */
    if (argc == 2) {
        hostname = argv[1];
    }
    else {
        hostname = NULL;
    }

    /*
     * Check that if the user specified one of the -dced or -inetd
     * options, the -abruptly option must not be present.
     */
    if ((dced_flag || inetd_flag) && abrupt_flag) {
        DCP_SET_RESULT_CODE(dcp_s_clock_abrupt_present);
        return TCL_ERROR;
    }

    /*
     * Check that if the user specified one of the -dced or the
     * -inetd switches, the hostname must also be present.
     */
    if (((dced_flag == TRUE) || (inetd_flag == TRUE)) && (hostname == NULL)) {
        DCP_SET_RESULT_CODE(dcp_s_clock_missing_hostname);
        return TCL_ERROR;
    }

    /* Make sure that -dtsd is not specified with either of the -dced or 
     * -inetd options.
     */
    if ((dtsd_flag && inetd_flag) || (dtsd_flag && dced_flag)) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        return TCL_ERROR;
    }

    /*
     * Check that if the user wants to bypass dtsd and use inetd or dced
     * directly, then only one of the options can be selected at a time.
     */
    if (dced_flag && inetd_flag) {
        DCP_SET_RESULT_CODE(dcp_s_mut_excl_opts);
        return TCL_ERROR;
    }

    /*
     * Now synchronize the time with the requested system
     * based on the method chosen; dced, inetd, or dtsd.
     */
    if (dced_flag == TRUE) {
        /*
         * Use dced to synchronize the time.  First get a vector
         * of the supported protocols.
         */
        rpc_network_inq_protseqs(&protseq_vector, &comm_st);
        if ((protseq_vector == NULL) || (comm_st != rpc_s_ok))
            return TCL_ERROR;

        /*
         * Cycle through all of the available protocol sequences until 
         * one successfully binds. 
         */
        got_binding = FALSE;
        for (i = 0; i < protseq_vector->count; ++i) {
            rpc_string_binding_compose(NULL,
                                       protseq_vector->protseq[i],
                                       (unsigned_char_t *)hostname,
                                       NULL,
                                       NULL,
                                       &string_binding,
                                       &comm_st);
            if (comm_st == rpc_s_ok) {
                /*
                 * Get the rpc binding from the string binding.
                 */
                rpc_binding_from_string_binding(string_binding, &h, &comm_st);
                if (comm_st == rpc_s_ok) {
                    /*
                     * Got the rpc binding, free the string binding.
                     */
                    got_binding = TRUE;
                    rpc_string_free(&string_binding, &tmp_st);
                    rpc_protseq_vector_free(&protseq_vector, &tmp_st);
                    break;
                }
            }
        }
        if (!got_binding) {
            /*
             * Never did get a binding.  Free all the necessary
             * stuff, then check if an error was encountered.
             */
            if (string_binding) {
                rpc_string_free(&string_binding, &tmp_st);
            }
            rpc_protseq_vector_free(&protseq_vector, &tmp_st);
            DCP_CHECK_RPC_ERROR(comm_st);
        }

        /*
         * At this point, we should have a valid rpc binding, but
         * we might need more information about the rutc service
         * before we can use it, depending on the protocol being
         * used.  Get the interface id of the dced rutc service.
         */
        rpc_if_inq_id(rutc_v0_0_c_ifspec, &if_id, &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Get a list of the available endpoints using the rutc
         * interface id.
         */
        rpc_mgmt_ep_elt_inq_begin(h,
                                  rpc_c_ep_match_by_if,
                                  &if_id,
                                  rpc_c_vers_all,
                                  NULL,
                                  &inquiry_context,
                                  &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * See if we've found a valid match for the rutc interface
         * id.  If so, we'll get the object uuid we need.
         */
        rpc_mgmt_ep_elt_inq_next(inquiry_context,
                                 &if_id,
                                 &out_binding,
                                 &object_uuid,
                                 &annot,
                                 &comm_st);
        if (comm_st == rpc_s_no_more_elements) {
            DCP_SET_RESULT_CODE(dcp_s_clock_get_dced_time_fail);
            if (hostname) {
                free(hostname);
            }
            return TCL_ERROR;
        }
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Clean up the binding and annotation variables,
         * and the inquiry context.
         */
        rpc_string_free(&annot, &tmp_st);
        rpc_binding_free(&out_binding, &tmp_st);
        rpc_mgmt_ep_elt_inq_done(&inquiry_context, &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Set the object uuid so the following call to the
         * rutc service will find the correct entry.
         */
        rpc_binding_set_object(h, &object_uuid, &comm_st);
        DCP_CHECK_RPC_ERROR(comm_st);

        /*
         * Make the call to the daemon.
         */
        ret = rutc_gettime(h, &utc, &comm_st);
        if (comm_st != rpc_s_ok) {
            DCP_SET_RESULT_CODE(dcp_s_clock_get_dced_time_fail);
            return TCL_ERROR;
        }
        if (ret != 0) {
            DCP_SET_RESULT_CODE(dcp_s_clock_dced_server_time_fail);
            return TCL_ERROR;
        }

        /*
         * We successfully got the time in utc format.  Now
         * bypass dtsd and hit the system clock directly.
         */
        if ((utc_bintime(&time_spec, (timespec_t *)0, (long *)0, &utc)) < 0) {
            /*
             * Problem converting utc time to binary time.
             */
            DCP_SET_RESULT_CODE(dcp_s_utc_convert_failed);
            return TCL_ERROR;
        }
        new_time = time_spec.tv_sec;
        ret = stime(&new_time);
        if (ret != 0) {
            /*
             * Problem setting the system time.
             */
            DCP_SET_RESULT_CODE(dcp_s_clock_set_failed);
            return TCL_ERROR;
        }
    }
    else if (inetd_flag == TRUE) {
        /*
         * Use inetd to synchronize the time.
         */
        if ((utc_getTCPtime(hostname, &utc)) < 0) {
            /*
             * Getting the time via this call failed.
             */
            DCP_SET_RESULT_CODE(dcp_s_clock_get_inetd_time_fail);
            return TCL_ERROR;
        }
        else {
            /*
             * We successfully got the time in utc format.  Now
             * bypass dtsd and hit the system clock directly.
             */
            if ((utc_bintime(&time_spec, (timespec_t *)0, (long *)0, &utc)) < 0) {
                /*
                 * Problem converting utc time to binary time.
                 */
                DCP_SET_RESULT_CODE(dcp_s_utc_convert_failed);
                return TCL_ERROR;
            }
            new_time = time_spec.tv_sec;
            ret = stime(&new_time);
            if (ret != 0) {
                /*
                 * Problem setting the system time.
                 */
                DCP_SET_RESULT_CODE(dcp_s_clock_set_failed);
                return TCL_ERROR;
            }
        }
    }
    else {
        /*
         * Use dtsd to synchronize the time.
         * Initialize rpc communications with server.
         */
        if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
            return TCL_ERROR;
        }

        /*
         * Initialize the status block.
         */
        st_block.status = 0;
        st_block.sysStatus = 0;
        retry = 0;

        /*
         * Send the RPC to have the server synchronize.
         */
        for (;;) {
            SynchronizeCmd(dcp_dts_binding_h, 
                           (signed32 *)&abrupt_flag, 
                           &st_block, 
                           &comm_st);
            if (comm_st != rpc_s_ok) {
                if (retry >= DCP_DTS_MAX_RETRIES) {
                    dts_check_status(interp, st_block, comm_st);
                    return TCL_ERROR;
                }
                else {
                    /* 
                     * Set up for another pass at the RPC, nuke the previous
                     * result and force a rebind.
                     */
                    retry++;
                    Tcl_ResetResult(interp);
                    dcp_dts_valid_binding = FALSE;
                    if (dcp_dts_bind_to_server(interp, hostname) == TCL_ERROR) {
                        return TCL_ERROR;
                    }
                }
            }
            else {
                if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
                    return TCL_ERROR;
                }
                break;
            }
        }
    }
    return TCL_OK;
}
