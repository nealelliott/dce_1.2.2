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
 * $Log: dts_util.c,v $
 * Revision 1.1.8.1  1996/08/09  11:48:08  arvind
 * 	Merge changes.
 * 	[1996/06/11  15:14 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/27  14:21 UTC  truitt  /main/HPDCE02/15]
 *
 * 	Fix merge problem.
 * 	[1996/02/27  14:19 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf65/1]
 *
 * 	Fix merge problem.
 * 	[1996/02/26  15:34 UTC  gilda  /main/HPDCE02/14]
 *
 * 	re-merge
 * 	[1996/02/26  14:52 UTC  gilda  /main/HPDCE02/13]
 *
 * 	merge
 * 	[1996/02/12  16:17 UTC  gilda  /main/HPDCE02/gilda_dcecp_bug1]
 *
 * 	CHFts17449:  The 'badtimerep' attribute in dts should be plural.
 * 	[1996/02/12  16:17 UTC  gilda  /main/HPDCE02/gilda_dcecp_bug1/2]
 *
 * 	Merge changes.
 * 	[1995/11/17  18:47 UTC  truitt  /main/HPDCE02/truitt_dcecp_chfts53/1]
 *
 * 	CHFts16869: Set the attribute type correctly for uuid's being shown.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/11  14:45 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.2  1996/02/18  19:13:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:17  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  21:21:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/11  1995/06/16  18:21 UTC  truitt
 * 	Merge build corrections.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts35/1  1995/06/16  18:20 UTC  truitt
 * 	CHFts15504: Correct build warnings.
 * 
 * 	HP revision /main/HPDCE02/10  1995/05/24  16:32 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts31/1  1995/05/24  16:31 UTC  truitt
 * 	CHFts14679: Last of string info to move.
 * 
 * 	HP revision /main/HPDCE02/9  1995/05/23  19:20 UTC  truitt
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  19:19 UTC  truitt
 * 	CHFts14679: Move field headers.
 * 
 * 	HP revision /main/HPDCE02/8  1995/05/04  17:39 UTC  jrr
 * 	Merge fix for CHFts15009.
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_chfts15009/1  1995/05/04  17:37 UTC  jrr
 * 	Fix no_more_bindings message (CHFts15009).
 * 
 * 	HP revision /main/HPDCE02/7  1995/04/06  13:55 UTC  jrr
 * 	Fix various memory leaks (CHFts14834).
 * 	[1995/04/05  14:32 UTC  jrr  /main/HPDCE02/jrr_dcecp_chfts14834/1]
 * 
 * 	HP revision /main/HPDCE02/6  1995/03/14  14:11 UTC  truitt
 * 	Merge in message changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts19/1  1995/03/14  13:31 UTC  truitt
 * 	CHFts14472: When unable to contact a dts server
 * 	due to an rpc problem, trap the rpc errors and
 * 	return more comprehensive error messages.
 * 
 * 	HP revision /main/HPDCE02/5  1994/10/11  13:36 UTC  jane
 * 	Merge Hpdce02_04 to HPDCE02 branch
 * 	[1995/12/08  17:56:03  root]
 * 
 * Revision 1.1.4.31  1994/10/02  19:13:30  rousseau
 * 	Fix up a few more free's
 * 	[1994/10/02  19:13:07  rousseau]
 * 
 * 	HP revision /main/HPDCE02/4  1994/10/03  18:35 UTC  jane
 * 	merge from Hpdce02_03
 * 
 * Revision 1.1.4.30  1994/09/27  22:18:03  rousseau
 * 	Fix bad memory use (CR 12375).
 * 	[1994/09/27  22:17:51  rousseau]
 * 
 * Revision 1.1.4.29  1994/09/27  20:54:54  rousseau
 * 	Fix lastobskew test (CR 12371).
 * 	[1994/09/27  20:54:45  rousseau]
 * 
 * Revision 1.1.4.28  1994/09/23  23:18:50  rousseau
 * 	Internal rebinding changes (CR 12282).
 * 	[1994/09/23  23:18:26  rousseau]
 * 
 * Revision 1.1.4.27  1994/09/23  19:59:50  tom
 * 	Back out previous submission - leave change to use HYPER_TO_U64.
 * 	[1994/09/23  19:26:38  tom]
 * 
 * Revision 1.1.4.25  1994/09/15  23:24:24  rousseau
 * 	Fix bad free.
 * 	[1994/09/15  23:13:03  rousseau]
 * 
 * Revision 1.1.4.24  1994/09/15  22:24:14  rousseau
 * 	Cache binding handles correctly (CR 12213).
 * 	[1994/09/15  22:23:56  rousseau]
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/22  15:56 UTC  jane
 * 	merge from Hpdce02_02
 * 
 * Revision 1.1.4.23  1994/09/01  14:16:54  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:15  rousseau]
 * 
 * Revision 1.1.4.22  1994/08/26  18:31:40  kevins
 * 	OT 11888 replace  obsolete constants
 * 	[1994/08/26  18:22:27  kevins]
 * 
 * Revision 1.1.4.21  1994/08/25  15:51:11  rousseau
 * 	Change _POSIX_PATH_MAX to DCECP_PATH_MAX (CR 11866).
 * 	[1994/08/25  15:50:49  rousseau]
 * 
 * Revision 1.1.4.20  1994/08/23  20:31:40  cbrooks
 * 	Code Cleanup
 * 	[1994/08/23  18:59:41  cbrooks]
 * 
 * Revision 1.1.4.19  1994/08/21  19:45:30  melman
 * 	fix various compiler warnings
 * 	[1994/08/21  19:29:47  melman]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/19  21:21 UTC  pulsinelli
 * 	Merge from Hpdce02_01
 * 
 * Revision 1.1.4.18  1994/08/08  21:04:28  melman
 * 	put include dce_utils.h in dcecp.h
 * 	[1994/08/08  21:01:49  melman]
 * 
 * Revision 1.1.4.17  1994/08/01  15:14:32  rousseau
 * 	Fix typo.
 * 	[1994/08/01  15:14:17  rousseau]
 * 
 * Revision 1.1.4.16  1994/07/28  20:34:48  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:22  rousseau]
 * 
 * Revision 1.1.4.15  1994/07/01  14:01:38  rousseau
 * 	Added error message when binding file open fails (CR 11151).
 * 	[1994/07/01  14:01:19  rousseau]
 * 
 * Revision 1.1.4.14  1994/06/29  21:00:13  ohara
 * 	include <limits.h>
 * 	[1994/06/27  19:43:29  ohara]
 * 
 * Revision 1.1.4.13  1994/06/24  22:07:30  rousseau
 * 	Do a deactivate before stop (CR 10658) and fix RIOS counters problem.
 * 	[1994/06/24  21:53:10  rousseau]
 * 
 * Revision 1.1.4.12  1994/06/24  17:49:17  rousseau
 * 	Merged with changes from 1.1.4.11
 * 	[1994/06/24  17:49:08  rousseau]
 * 
 * 	Change show output indent (CR 11075).
 * 	[1994/06/24  17:48:30  rousseau]
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/26  10:54 UTC  mothra
 * 	change _POSIX_PATH_MAX to MAXPATHLEN
 * 
 * 	HP revision /main/pulsinelli_dcecp_build/1  1994/07/25  23:14 UTC  pulsinelli
 * 	Correct bogus path name so we can build.
 * 
 * Revision 1.1.4.11  1994/06/09  16:02:12  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:43  devsrc]
 * 
 * Revision 1.1.4.10  1994/05/20  20:41:46  rousseau
 * 	Set _n. Make cosmetic output changes. Fix use of _s(dts).
 * 	[1994/05/20  20:38:49  rousseau]
 * 
 * Revision 1.1.4.9  1994/05/16  20:47:45  rousseau
 * 	Merged with changes from 1.1.4.8
 * 	[1994/05/16  20:47:31  rousseau]
 * 
 * 	Fix build problem (CR 10642).
 * 	[1994/05/16  20:39:12  rousseau]
 * 
 * Revision 1.1.4.8  1994/05/13  22:22:43  melman
 * 	renamed dcecp's dts.h to dcpdts.h
 * 	[1994/05/13  22:18:20  melman]
 * 
 * Revision 1.1.4.7  1994/05/11  15:39:48  rousseau
 * 	Use conv. var macros. Clean up localserver and globalservers output (CR 10595).
 * 	[1994/05/11  15:39:34  rousseau]
 * 
 * Revision 1.1.4.6  1994/05/09  20:41:37  rousseau
 * 	Updated to new control interface. Enabled talking to remote daemons (CR 10434).
 * 	[1994/05/09  20:41:21  rousseau]
 * 
 * Revision 1.1.4.4  1994/03/01  16:03:58  rousseau
 * 	Special case check for dtsd not running (CR 10029).
 * 	Removed free of login context in dts_init_rpc() (CR 9609).
 * 	[1994/03/01  16:03:11  rousseau]
 * 
 * Revision 1.1.4.3  1994/02/10  20:57:50  rousseau
 * 	Fix extra newline in show for CR 9892.
 * 	[1994/02/10  20:57:42  rousseau]
 * 
 * Revision 1.1.4.2  1994/02/02  19:43:55  rousseau
 * 	Changed to support help functions.
 * 	[1994/02/02  19:40:18  rousseau]
 * 
 * Revision 1.1.4.1  1993/12/22  15:00:00  rousseau
 * 	Style guide changes.
 * 	[1993/12/22  14:59:54  rousseau]
 * 
 * Revision 1.1.2.4  1993/11/19  22:47:24  rousseau
 * 	Added internal functions for show command.
 * 	[1993/11/19  22:47:17  rousseau]
 * 
 * Revision 1.1.2.3  1993/11/11  13:56:20  rousseau
 * 	Changed output formatting for 'show'.
 * 	[1993/11/11  13:56:15  rousseau]
 * 
 * Revision 1.1.2.2  1993/11/09  18:22:00  rousseau
 * 	Added routines for dts show commands.
 * 	[1993/11/09  18:21:55  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/05  15:34:44  rousseau
 * 	Initial version.
 * 	[1993/11/05  15:34:17  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: dts_util.c
 *
 * DESCRIPTION:
 *   This module contains internal functions used to implement the
 * DTS operations in dcecp.
 *
 */

/* INCLUDE FILES  */

#include <limits.h>
#include <dcp_dts.h>
#include <util_binding.h>
#include <util_general.h>

/* DECLARATIONS */

#define BINDING_HANDLE_STR_LEN 	128

/* INTERNAL VARIABLES */

boolean32                       dcp_dts_valid_binding = FALSE;
rpc_binding_handle_t            dcp_dts_binding_h = NULL;
char                            *dcp_dts_binding_site = NULL;

/*
 * Field headers
 */
#define DCP_T_DTS_CHECK_INTERVAL_HEADER         "checkinterval"
#define DCP_T_DTS_EPOCH_NUMBER_HEADER           "epoch"
#define DCP_T_DTS_ERROR_TOLERANCE_HEADER        "tolerance"
#define DCP_T_DTS_TIME_DIFF_HEADER              "tdf"
#define DCP_T_DTS_MAXIMUM_INACCURACY_HEADER     "maxinaccuracy"
#define DCP_T_DTS_SERVERS_REQUIRED_HEADER       "minservers"
#define DCP_T_DTS_QUERY_ATTEMPTS_HEADER         "queryattempts"
#define DCP_T_DTS_LAN_TIMEOUT_HEADER            "localtimeout"
#define DCP_T_DTS_WAN_TIMEOUT_HEADER            "globaltimeout"
#define DCP_T_DTS_SYNCH_HEADER                  "syncinterval"
#define DCP_T_DTS_VARIANT_HEADER                "type"
#define DCP_T_DTS_COURIER_ROLE_HEADER           "courierrole"
#define DCP_T_DTS_BACKUP_COURIER_ROLE_HEADER    "actcourierrole"
#define DCP_T_DTS_CLOCK_ADJUSTMENT_HEADER       "clockadjrate"
#define DCP_T_DTS_CLOCK_DRIFT_HEADER            "maxdriftrate"
#define DCP_T_DTS_DECDTS_VERSION_HEADER         "version"
#define DCP_T_DTS_TIME_REP_HEADER               "timerep"
#define DCP_T_DTS_TP_PRESENT_HEADER             "provider"
#define DCP_T_DTS_AUTO_TDF_HEADER               "autotdfchange"
#define DCP_T_DTS_NEXT_TDF_HEADER               "nexttdfchange"
#define DCP_T_DTS_CLOCK_RESOLUTION_HEADER       "clockresolution"
#define DCP_T_DTS_SERVER_PRINCIPAL_HEADER       "serverprincipal"
#define DCP_T_DTS_SERVER_ENTRY_HEADER           "serverentry"
#define DCP_T_DTS_SERVER_GROUP_HEADER           "servergroup"
#define DCP_T_DTS_UID_HEADER                    "uuid"
#define DCP_T_DTS_LAST_SYNCH_HEADER             "lastsync"
#define DCP_T_DTS_STATE_HEADER                  "status"
#define DCP_T_DTS_CREATION_TIME_HEADER          "creationtime"
#define DCP_T_DTS_LOCAL_FAULT_HEADER            "nolocalintersections"
#define DCP_T_DTS_FAULTY_SERVER_HEADER          "noserverintersections"
#define DCP_T_DTS_DIFFERENT_EPOCH_HEADER        "diffepochs"
#define DCP_T_DTS_TOO_FEW_HEADER                "toofewservers"
#define DCP_T_DTS_PROTOCOL_VERSION_HEADER       "badprotocols"
#define DCP_T_DTS_TIME_REPS_HEADER              "badtimereps"
#define DCP_T_DTS_NO_GLOBAL_HEADER              "noglobals"
#define DCP_T_DTS_NOT_RESPONDING_HEADER         "noresponses"
#define DCP_T_DTS_CLOCK_SET_HEADER              "abrupts"
#define DCP_T_DTS_EPOCH_DECLARATION_HEADER      "epochchanges"
#define DCP_T_DTS_SYSTEM_ERROR_HEADER           "syserrors"
#define DCP_T_DTS_SYNCH_COMPLETED_HEADER        "syncs"
#define DCP_T_DTS_UPDATE_COMPLETED_HEADER       "updates"
#define DCP_T_DTS_STARTUP_COMPLETED_HEADER      "enables"
#define DCP_T_DTS_SHUTDOWN_COMPLETED_HEADER     "disables"
#define DCP_T_DTS_INSUFFICIENT_RESOURCE_HEADER  "nomemories"
#define DCP_T_DTS_TIME_PROVIDER_HEADER          "providerfailures"
#define DCP_T_DTS_TIME_PROVIDER_TIMEOUT_HEADER  "providertimeouts"
#define DCP_T_DTS_LOC_SERV_NOT_TSG_HEADER       "badlocalservers"
#define DCP_T_DTS_SERV_NOT_TSG_HEADER           "badservers"
#define DCP_T_DTS_TIME_LAST_POLLED_HEADER       "timelastpolled"
#define DCP_T_DTS_LAST_OBS_TIME_HEADER          "lastobstime"
#define DCP_T_DTS_LAST_OBS_SKEW_HEADER          "lastobsskew"
#define DCP_T_DTS_IN_LAST_SYNC_HEADER           "inlastsync"
#define DCP_T_DTS_TRANSPORT_HEADER              "transport"
#define DCP_T_DTS_GLOBAL_SERVERS_HEADER         "globalservers"
#define DCP_T_DTS_LOCAL_SERVERS_HEADER          "localservers"
#define DCP_T_DTS_NAME_HEADER                   "name"

/*
 * Courier roles
 */
#define DCP_T_DTS_COURIER     "courier"
#define DCP_T_DTS_NONCOURIER  "noncourier"
#define DCP_T_DTS_BACKUP      "backup"

/*
 * States
 */
#define DCP_T_DTS_DISABLED     "disabled"
#define DCP_T_DTS_ENABLED      "enabled"
#define DCP_T_DTS_SYNCING      "syncing"
#define DCP_T_DTS_UPDATING     "updating"
#define DCP_T_DTS_NOT_CREATED  "{not created}"

/*
 * Types
 */
#define DCP_T_DTS_SERVER  "server"
#define DCP_T_DTS_CLERK   "clerk"

/*
 * Transport types
 */
#define DCP_T_DTS_IEEE     "{IEEE 802.3}"
#define DCP_T_DTS_DECNET   "DECnet"
#define DCP_T_DTS_UDP_IP   "UDP/IP"
#define DCP_T_DTS_TCP_IP   "TCP/IP"
#define DCP_T_DTS_RPC      "RPC"
#define DCP_T_DTS_UNKNOWN  "unknown"

/*
 * Miscellaneous
 */
#define DCP_T_DTS_YES             "yes"
#define DCP_T_DTS_NO              "no"
#define DCP_T_DTS_NEVER           "never"
#define DCP_T_DTS_FALSE           "FALSE"
#define DCP_T_DTS_TRUE            "TRUE"
#define DCP_T_DTS_NSEC            "nsec"
#define DCP_T_DTS_NSEC_SEC        "nsec/sec"
#define DCP_T_DTS_INTERNAL_ERROR  "{internal error}"

/* PROTOTYPES */

int dcp_dts_read_bindings(Tcl_Interp *, rpc_binding_vector_t **);
void emul(long int, long int, Bits64 *);
void uemul(unsigned long, unsigned long, Bits64 *);
int uediv(Bits64 *, unsigned long, unsigned long *, unsigned long *);



/*
 * FUNCTION:    dcp_dts_bind_to_server
 *
 * OVERVIEW:    Initialize communication with the specified dtsd.
 * 
 *
 * INPUTS:
 *  user_site
 *    A namespace entry or a string binding used to find the correct server.
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
dcp_dts_bind_to_server(Tcl_Interp *interp, char *user_site)
{
    char                  *s_var = NULL;
    char                  *site = NULL;
    char                  *tmp = NULL;
    unsigned_char_t       *dts_princ_name;
    rpc_ns_handle_t       import_context;
    char                  **sbv;
    char                  *str_bind;
    rpc_binding_vector_t  *bind_vec = 0;
    int                   sbc;
    error_status_t        st, st2;
    
    /*
     * Check for and cannonicalize any passed in site name or _s(dts).
     */
    if (user_site != NULL) {
        tmp = expand_cell_name(user_site, &st);
        if ((tmp != NULL) && (st == rpc_s_ok)) {
            site = tmp;
        }
        else {
            site = dce_strdup(user_site);
        }

        /*
         * Set $_n to the site passed in.
         */
        DCP_GENERAL_SET_NAMELIST(site);
    }
    else {
        /*
         * Get the value of _s(dts).
         */
        s_var = DCP_GENERAL_GET_S_VAR("dts");

        /*
         * If someone has unset _s(dts), squash the result.
         */
        if (s_var == NULL) {
            Tcl_ResetResult(interp);
        }
        else if (strlen(s_var) > 0) {
            tmp = expand_cell_name(s_var, &st);    
            if ((tmp != NULL) && (st == rpc_s_ok)) {
                site = tmp;
            }
            else {
                site = dce_strdup(s_var);
            }
        }
    }

    /*
     * Preload dce_dts_binding_site if it has never been used.
     */
    if (dcp_dts_binding_site == NULL) {
        dcp_dts_binding_site = dce_strdup("NoSite");
    }

    if (dcp_dts_valid_binding) {
        if (site && (strcmp(site, dcp_dts_binding_site) == 0)) {
            free(site);
            return TCL_OK;
        }
        else if ((site == NULL) && (strcmp(dcp_dts_binding_site, "local") == 0)) {
            return TCL_OK;
        }
    }

    /*
     * We've decided that we need a new binding. Invalidate the cache
     * and free the old binding if needed. 
     */
    dcp_dts_valid_binding = FALSE;
    if (dcp_dts_binding_h != NULL) {
        rpc_binding_free(&dcp_dts_binding_h, &st);
        dcp_dts_binding_h = NULL;
        DCP_CHECK_RPC_ERROR(st);
    }

    /*
     * Set $_n to the site passed in.
     */
    if (site != NULL) {
        DCP_GENERAL_SET_NAMELIST(site);
    }

    if (site == NULL) { 
        /*
         * Set cache to the local site.
         */
        free(dcp_dts_binding_site);
        dcp_dts_binding_site = dce_strdup("local");

        /*
         * Read the local bindings file, and set the global dts
         * binding handle upon success.
         */
        if (dcp_dts_read_bindings(interp, &bind_vec) != TCL_OK) {
            return TCL_ERROR;
        }

        /* If we have one or more bindings, just pick the first one. */
        if (bind_vec->count > 0) {
            rpc_binding_copy(bind_vec->binding_h[0], &dcp_dts_binding_h, &st);
            DCP_CHECK_RPC_ERROR(st);
        }

        /* Free bind_vec */
        rpc_binding_vector_free(&bind_vec, &st);
        DCP_CHECK_RPC_ERROR(st);
    }
    else {
        /*
         * Cache the site we are binding to.
         */
        free(dcp_dts_binding_site);
        dcp_dts_binding_site = dce_strdup(site);

	if (!strchr(site, ' ')) {
	    /*
             * See if we have a single string syntax binding.
             */
	    rpc_binding_from_string_binding((unsigned_char_t *)site,
                                            &dcp_dts_binding_h,
                                            &st);
            if (st != rpc_s_ok) {
                /*
                 * See if we have a namespace entry.
                 */
                rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
                                            (unsigned_char_t *)site,
                                            DTS_SERVER_INTERFACE,
                                            NULL,
                                            &import_context,
                                            &st);
                /* 
                 * If it's not a string binding or a namespace entry,
                 * give up.
                 */
                DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
                
                rpc_ns_binding_import_next(import_context, 
                                           &dcp_dts_binding_h, 
                                           &st);
                if (st != rpc_s_ok) {
                    if (st == rpc_s_no_more_bindings) {
                        /*
                         * The specified dts server either did not export its
                         * bindings, or it is the wrong (pre-1.1) version.
                         */
                        rpc_ns_binding_import_done(&import_context, &st2);
                        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_dts_no_more_bindings, 
                                                    site);
                        free(site);
                        return TCL_ERROR;
                    }
                    else {
                        free(site);
                        rpc_ns_binding_import_done(&import_context, &st2);
                        DCP_SET_MISC_ERROR(st);
                    }
                }

                rpc_ns_binding_import_done(&import_context, &st);
                DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
                
            }
        } else {
	    /*
             * We have a single TCL syntax binding.
             */
	    if (Tcl_SplitList(interp, site, &sbc, &sbv) != TCL_OK) {
                free(site);
		return(TCL_ERROR);
            }

	    str_bind = dcp_binding_tcl_to_string(sbc, sbv, &st);
            if (str_bind == NULL) {
                free(site);
                free((char *)sbv);
                return TCL_ERROR;
            }

	    rpc_binding_from_string_binding((unsigned_char_t *)str_bind, 
                                            &dcp_dts_binding_h, 
                                            &st);
	    rpc_string_free((unsigned char **)&str_bind, &st);
	    free((char *)sbv);
	    DCP_CHECK_RPC_ERROR_FREE_ONE(st, site);
	}
        free(site);
    }

    /* 
     * Get the time service principal by making an rpc_mgmt call
     * to the server.
     */
    rpc_mgmt_inq_server_princ_name(dcp_dts_binding_h,
                                   rpc_c_authn_dce_secret,
                                   &dts_princ_name,
                                   &st);
    if (st != rpc_s_ok) {
        if (st == rpc_s_comm_failure) {
            /*
             * The specified dts server is valid, but
             * cannot currently be reached.
             */
            DCP_SET_MISC_ERROR(dcp_s_dts_comm_failure);
        }
        else {
            DCP_SET_MISC_ERROR(st);
        }
    }

    if (dcp_is_logged_in()) {
        rpc_binding_set_auth_info(dcp_dts_binding_h,
                                  dts_princ_name,
                                  rpc_c_protect_level_default,
                                  rpc_c_authn_default,
                                  NULL,
                                  rpc_c_authz_dce,
                                  &st);
    }
    else {
        rpc_binding_set_auth_info(dcp_dts_binding_h,
                                  dts_princ_name,
                                  rpc_c_protect_level_none,
                                  rpc_c_authn_none,
                                  NULL,
                                  rpc_c_authz_dce,
                                  &st);
    }
    rpc_string_free(&dts_princ_name, &st2);
    DCP_CHECK_RPC_ERROR(st);

    dcp_dts_valid_binding = TRUE;
    return TCL_OK;
}


/*
 * FUNCTION:    dcp_dts_read_bindings
 *
 * OVERVIEW:    Read and return the bindings for the local dts entity 
 *              from the file on the local system.
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  bv
 *    A pointer to a list of binding vectors.
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
dcp_dts_read_bindings(Tcl_Interp *interp, rpc_binding_vector_t **bv)
{
    static char                 dts_binding_file_name[DCECP_PATH_MAX] ;
    FILE                        *fp = 0; 
    error_status_t              st;

    /* Since name is static, it will be 0. Use this to check for init */
    if (dts_binding_file_name[0] == '\0') {
        sprintf(dts_binding_file_name,
                "%s/var/adm/time/dtsd.binding",
                dcelocal_path);
    }

    if ((fp = fopen(dts_binding_file_name, "r")) == NULL) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_dts_bind_file_open_fail, 
                                    dts_binding_file_name);
        return TCL_ERROR;
    }

    dce_bindings_from_file(fp, bv, &st);
    DCP_CHECK_RPC_ERROR(st);
        
    (void)fclose(fp);

    return TCL_OK;
}


/*
 * FUNCTION:    dts_check_status
 *
 * OVERVIEW:    Check ths status block returned from the RPC for errors.
 * 
 *
 * INPUTS:
 *  st_block
 *    The status block returned from the RPC.
 *
 *  comm_st
 *    A status code containing any communications error that occured 
 *    during the RPC.
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
dts_check_status(Tcl_Interp *interp, 
                 StatusBlk_t st_block,
                 error_status_t comm_st)
{
    if (comm_st != rpc_s_ok) {
        /* 
         * This will only happen in rare cases in which a dts_util.c RPC
         * fails with a communications error. All other RPC errors are
         * now trapped in their calling routine.
         */
        dcp_dts_valid_binding = FALSE;

        DCP_SET_RESULT_CODE(comm_st);
        return TCL_ERROR;
    }
    
    /* Check the status block returned from dtsd */
    if (st_block.status != DTSS_SUCCESS) {
        DCP_SET_RESULT_CODE(st_block.sysStatus);
        return TCL_ERROR;
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_print_attr_text
 *
 * OVERVIEW:    Print out the Lexical form of the passed attribute number.
 * 
 *
 * INPUTS:
 *  attr_num
 *    The attribute number of the attribute to be looked up.
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
dts_print_attr_text(Tcl_Interp *interp, int attr_num)
{
    char  buf[25], tmp[25];

    switch(attr_num) {

      case K_CHECK_INTERVAL_ATTR:
        sprintf(buf, DCP_T_DTS_CHECK_INTERVAL_HEADER);
        break;
        
      case K_EPOCH_NUMBER_ATTR:
        sprintf(buf, DCP_T_DTS_EPOCH_NUMBER_HEADER);
        break;
        
      case K_ERROR_TOLLERANCE_ATTR:
        sprintf(buf, DCP_T_DTS_ERROR_TOLERANCE_HEADER);
        break;
        
      case K_TIME_DIFF_ATTR:
        sprintf(buf, DCP_T_DTS_TIME_DIFF_HEADER);
        break;
        
      case K_MAXIMUM_INACCURACY_ATTR:
        sprintf(buf, DCP_T_DTS_MAXIMUM_INACCURACY_HEADER);
        break;
        
      case K_SERVERS_REQUIRED_ATTR:
        sprintf(buf, DCP_T_DTS_SERVERS_REQUIRED_HEADER);
        break;
        
      case K_QUERY_ATTEMPTS_ATTR:
        sprintf(buf, DCP_T_DTS_QUERY_ATTEMPTS_HEADER);
        break;
        
      case K_LAN_TIMEOUT_ATTR:
        sprintf(buf, DCP_T_DTS_LAN_TIMEOUT_HEADER);
        break;
        
      case K_WAN_TIMEOUT_ATTR:
        sprintf(buf, DCP_T_DTS_WAN_TIMEOUT_HEADER);
        break;
        
      case K_SYNCH_ATTR:
        sprintf(buf, DCP_T_DTS_SYNCH_HEADER);
        break;
        
      case K_VARIANT_ATTR:
        sprintf(buf, DCP_T_DTS_VARIANT_HEADER);
        break;
        
      case K_COURIER_ROLE_ATTR:
        sprintf(buf, DCP_T_DTS_COURIER_ROLE_HEADER);
        break;
        
      case K_BACKUP_COURIER_ROLE_ATTR:
        sprintf(buf, DCP_T_DTS_BACKUP_COURIER_ROLE_HEADER);
        break;
        
      case K_CLOCK_ADJUSTMENT_ATTR:
        sprintf(buf, DCP_T_DTS_CLOCK_ADJUSTMENT_HEADER);
        break;
        
      case K_CLOCK_DRIFT_ATTR:
        sprintf(buf, DCP_T_DTS_CLOCK_DRIFT_HEADER);
        break;
        
      case K_DECDTS_VERSION_ATTR:
        sprintf(buf, DCP_T_DTS_DECDTS_VERSION_HEADER);
        break;
        
      case K_TIME_REP_ATTR:
        sprintf(buf, DCP_T_DTS_TIME_REP_HEADER);
        break;
        
      case K_TP_PRESENT_ATTR:
        sprintf(buf, DCP_T_DTS_TP_PRESENT_HEADER);
        break;
        
      case K_AUTO_TDF_ATTR:
        sprintf(buf, DCP_T_DTS_AUTO_TDF_HEADER);
        break;
        
      case K_NEXT_TDF_ATTR:
        sprintf(buf, DCP_T_DTS_NEXT_TDF_HEADER);
        break;
        
      case K_CLOCK_RESOLUTION_ATTR:
        sprintf(buf, DCP_T_DTS_CLOCK_RESOLUTION_HEADER);
        break;
        
      case K_SERVER_PRINCIPAL_NAME:
        sprintf(buf, DCP_T_DTS_SERVER_PRINCIPAL_HEADER);
        break;
        
      case K_SERVER_ENTRY_NAME:
        sprintf(buf, DCP_T_DTS_SERVER_ENTRY_HEADER);
        break;
        
      case K_SERVER_GROUP_NAME:
        sprintf(buf, DCP_T_DTS_SERVER_GROUP_HEADER);
        break;
        
      case K_UID_ATTR:
        sprintf(buf, DCP_T_DTS_UID_HEADER);
        break;
        
      case K_LAST_SYNCH_ATTR:
        sprintf(buf, DCP_T_DTS_LAST_SYNCH_HEADER);
        break;
        
      case K_STATE_ATTR:
        sprintf(buf, DCP_T_DTS_STATE_HEADER);
        break;
        
      case K_CREATION_TIME_ATTR:
        sprintf(buf, DCP_T_DTS_CREATION_TIME_HEADER);
        break;
        
      case K_LOCAL_FAULT_ATTR:
        sprintf(buf, DCP_T_DTS_LOCAL_FAULT_HEADER);
        break;
        
      case K_FAULTY_SERVER_ATTR:
        sprintf(buf, DCP_T_DTS_FAULTY_SERVER_HEADER);
        break;
        
      case K_DIFFERENT_EPOCH_ATTR:
        sprintf(buf, DCP_T_DTS_DIFFERENT_EPOCH_HEADER);
        break;
        
      case K_TOO_FEW_ATTR:
        sprintf(buf, DCP_T_DTS_TOO_FEW_HEADER);
        break;
        
      case K_PROTOCOL_VERSION_ATTR:
        sprintf(buf, DCP_T_DTS_PROTOCOL_VERSION_HEADER);
        break;
        
      case K_TIME_REPS_ATTR:
        sprintf(buf, DCP_T_DTS_TIME_REPS_HEADER);
        break;
        
      case K_NO_GLOBAL_ATTR:
        sprintf(buf, DCP_T_DTS_NO_GLOBAL_HEADER);
        break;
        
      case K_NOT_RESPONDING_ATTR:
        sprintf(buf, DCP_T_DTS_NOT_RESPONDING_HEADER);
        break;
        
      case K_CLOCK_SET_ATTR:
        sprintf(buf, DCP_T_DTS_CLOCK_SET_HEADER);
        break;
        
      case K_EPOCH_DECLARATION_ATTR:
        sprintf(buf, DCP_T_DTS_EPOCH_DECLARATION_HEADER);
        break;
        
      case K_SYSTEM_ERROR_ATTR:
        sprintf(buf, DCP_T_DTS_SYSTEM_ERROR_HEADER);
        break;
        
      case K_SYNCH_COMPLETED_ATTR:
        sprintf(buf, DCP_T_DTS_SYNCH_COMPLETED_HEADER);
        break;
        
      case K_UPDATE_COMPLETED_ATTR:
        sprintf(buf, DCP_T_DTS_UPDATE_COMPLETED_HEADER);
        break;
        
      case K_STARTUP_COMPLETED_ATTR:
        sprintf(buf, DCP_T_DTS_STARTUP_COMPLETED_HEADER);
        break;
        
      case K_SHUTDOWN_COMPLETED_ATTR:
        sprintf(buf, DCP_T_DTS_SHUTDOWN_COMPLETED_HEADER);
        break;
        
      case K_INSUFFICIENT_RESOURCE_ATTR:
        sprintf(buf, DCP_T_DTS_INSUFFICIENT_RESOURCE_HEADER);
        break;
        
      case K_TIME_PROVIDER_ATTR:
        sprintf(buf, DCP_T_DTS_TIME_PROVIDER_HEADER);
        break;
        
      case K_TIME_PROVIDER_TIMEOUT_ATTR:
        sprintf(buf, DCP_T_DTS_TIME_PROVIDER_TIMEOUT_HEADER);
        break;
        
      case K_LOC_SERV_NOT_TSG_MEMBER_ATTR:
        sprintf(buf, DCP_T_DTS_LOC_SERV_NOT_TSG_HEADER);
        break;
        
      case K_SERV_NOT_TSG_MEMBER_ATTR:
        sprintf(buf, DCP_T_DTS_SERV_NOT_TSG_HEADER);
        break;
        
      default:
        sprintf(buf, DCP_T_DTS_INTERNAL_ERROR);
        return TCL_ERROR;
    }
    
    sprintf(tmp, "{%s", buf);
    DCP_APPEND_RESULT(tmp);
    return TCL_OK;
}


/*
 * FUNCTION:    dts_print_attr_value
 *
 * OVERVIEW:    Print the attribute value.  
 * 
 *
 * INPUTS:
 *  attribute
 *    The number of the attribute to be displayed.
 *
 *  attrType
 *    The type of the attribute to be displayed.
 *
 *  attrValue
 *    A pointer to the value of the attribute.
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
dts_print_attr_value(Tcl_Interp *interp, int attribute, 
                     int attrType, VOID *attrValue)
{
    char printStr[1024];
    char temp_str[1024];
    size_t printStrLen = 1024;
    
#if CONFIG_DECNET
    int status;
#endif
    int letter, v_major, v_minor, v_level;
    String_t *string;
    
    switch ( attrType ) {
      case K_TYPE_BINRELTIME:
        if (utc_ascreltime(printStr, printStrLen, (utc_t *)attrValue))
            sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
        break;
        
      case K_TYPE_UNSIGNED32:
        sprintf(printStr, "%u", *((unsigned *)attrValue));
        break;
        
      case K_TYPE_INTEGER32:
        sprintf(printStr,"%d", *((int *)attrValue));
        break;
        
      case K_TYPE_COURIER_ROLE:
        switch (*((int *)attrValue)) {
          case 0: sprintf(printStr, DCP_T_DTS_COURIER);
            break;
          case 1: sprintf(printStr, DCP_T_DTS_NONCOURIER);
            break;
          case 2: sprintf(printStr, DCP_T_DTS_BACKUP);
            break;
          default: sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
            break;
        }
        break;
        
      case K_TYPE_VERSION:
        letter= ((char *)attrValue)[K_VER_LET];
        v_major = ((char *)attrValue)[K_VER_MAJ];
        v_minor = ((char *)attrValue)[K_VER_MIN];
        v_level = ((char *)attrValue)[K_VER_SUB];
        sprintf(printStr,"%c%1d.%1d.%1d",
                letter,v_major,v_minor,v_level);
        break;
        
      case K_TYPE_STATE:
        switch ( *((int *)attrValue) ) {
          case 0: sprintf(printStr, "%s", DCP_T_DTS_DISABLED);
            break;
          case 1: sprintf(printStr, "%s", DCP_T_DTS_ENABLED);
            break;
          case 2: 
          case 4: sprintf(printStr, "%s", DCP_T_DTS_SYNCING);
            break;
          case 3: sprintf(printStr, "%s", DCP_T_DTS_UPDATING);
            break;
          case 5: sprintf(printStr, "%s", DCP_T_DTS_NOT_CREATED);
            break;
          default: sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
            break;
        }
        break;

      case K_TYPE_VARIANT:
        switch ( *((int *)attrValue) ) {
          case 0: sprintf(printStr, "%s", DCP_T_DTS_SERVER);
            break;
          case 1: sprintf(printStr, "%s", DCP_T_DTS_CLERK);
            break;
          default:
            sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
            break;
        }
        break;
        
      case K_TYPE_BOOLEAN:
        switch( *((int *)attrValue) ) {
          case 0: sprintf(printStr, DCP_T_DTS_NO);
            break;
          case 1: sprintf(printStr, DCP_T_DTS_YES);
            break;
          default: sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
            break;
        }
        break;
        
      case K_TYPE_SIMPLENAME:
        {
            
#if CONFIG_DECNET
            DnsSimpleName_t *Name;
            
            Name = (DnsSimpleName_t *)attrValue;
            status = dnsCvtOpqSimpleToStr(
                                          (dns_opq_sname *)Name->Sname,
                                          (unsigned char *)printStr,
                                          &printStrLen);
            if (status != DNS_SUCCESS) {
                sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
            }
#endif
            break;
        }

      case K_TYPE_FULLNAME:
        {
            break;
        }
        
      case K_TYPE_STRING:
        string = (String_t *)attrValue;
        strcpy(printStr, (char *)string->buffer );
        break;
        
      case K_TYPE_BINABSTIME:
        if (utc_asclocaltime(printStr, printStrLen, (utc_t *)attrValue))
            sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
        break;
        
      case K_TYPE_UID:
        {
            uuid_t *uuid;
            unsigned_char_p_t string_uuid;
            unsigned32 status;
            
            uuid = (uuid_t *)attrValue;
            uuid_to_string (uuid, &string_uuid, &status);
            sprintf(printStr, "%s", string_uuid);
            rpc_string_free (&string_uuid, &status);
            
            break;
        }
        
      case K_TYPE_COUNTER64:
        {
            char *ptr, ch;
            int  leadingZeros;
            unsigned long int quot, remd, dividend;
            unsigned long int divisor = 1000000000;  
            /* 10E9 equals the magnitude of 32 bit int. */
            /* Should be O.K. for larger size ints since*/
            /* counter type guaranteed to be 64 bits.   */
            
            leadingZeros = TRUE;
            printStr[0] = '\0';
            uediv((Bits64 *)attrValue, divisor, &quot, &remd);
            
            /*
             * Check for zero && for > size int
             */
            if (quot == 0 && remd == 0) {
                printStr[0] = '0';
                printStr[1] = '\0';
            }
            else if (quot != 0) {
                leadingZeros = FALSE;
                sprintf(printStr, "%u", quot);
            }
            
            /*
             * if there are trailing zeros to print, or if
             * there is a number less than 10E9, generate the output.
             */
            if (quot != 0 || remd != 0) {
                ptr = &(printStr[strlen(printStr)]);
                do {
                    divisor = divisor/10;  
                    dividend = remd;
                    quot = dividend/divisor;
                    ch = quot + '0';
                    if (ch != '0' || !leadingZeros) {
                        *ptr = ch;
                        ptr++;
                        leadingZeros = FALSE;
                    }
                    remd = dividend % divisor;
                } while (divisor > 1);
                
                *ptr = '\0';
            }
            
            break;
        }

      case K_TYPE_ID802:
      case K_TYPE_TOWERSET:
      case K_TYPE_NSCTS:
      case K_TYPE_SET:
      case K_TYPE_ENUMERATION:
        sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
        break;

      default:
        sprintf(printStr, DCP_T_DTS_INTERNAL_ERROR);
    }

    switch (attribute) {
      case K_CLOCK_ADJUSTMENT_ATTR:
      case K_CLOCK_DRIFT_ATTR:
        sprintf(temp_str, " %s %s}", printStr, DCP_T_DTS_NSEC_SEC);
        Tcl_AppendResult(interp, temp_str, (char *)NULL);
        break;

      case K_CLOCK_RESOLUTION_ATTR:
        sprintf(temp_str, " %s %s}", printStr, DCP_T_DTS_NSEC);
        Tcl_AppendResult(interp, temp_str, (char *)NULL);
        break;

      default:
        sprintf(temp_str, " %s}", printStr);
        Tcl_AppendResult(interp, temp_str, (char *)NULL);
    }

    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_all_attr
 *
 * OVERVIEW:    Print out the values retuned by the show characteristics 
 *              directive.
 * 
 *
 * INPUTS:
 *  charBuf
 *    A structure containing data returned from the show characteristics
 *    RPC.
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
dts_show_all_attr(Tcl_Interp *interp, CtlShowAllChar *charBuf)
{
    utc_t                       tempBinTime;
    int                         tempInteger32;
    long                        tdfvalue;
    reltimespec_t               reltime;
    
    if ( charBuf->variant == (long int)VTserver ) {
	
	dts_print_attr_text(interp, K_CHECK_INTERVAL_ATTR);
	UTCtoBinAbs(&tempBinTime, &charBuf->checkInt);
	dts_print_attr_value(interp, K_CHECK_INTERVAL_ATTR,
                             K_TYPE_BINRELTIME,
                             (VOID *)&tempBinTime);
        
	dts_print_attr_text(interp, K_EPOCH_NUMBER_ATTR);
	dts_print_attr_value(interp, K_EPOCH_NUMBER_ATTR,
                             K_TYPE_INTEGER32, 
                             (VOID *)&charBuf->epochNumber);
    }    
    
    dts_print_attr_text(interp, K_ERROR_TOLLERANCE_ATTR);
    UTCtoBinAbs(&tempBinTime, &charBuf->errorTolerance);
    dts_print_attr_value(interp, K_ERROR_TOLLERANCE_ATTR,
                         K_TYPE_BINRELTIME,
                         (VOID *)&tempBinTime);
    
    dts_print_attr_text(interp, K_TIME_DIFF_ATTR);
    tdfvalue = charBuf->serverTDF;
    reltime.tv_sec = tdfvalue ;
    reltime.tv_nsec = 0L ;
    (void)utc_mkbinreltime(&tempBinTime, &reltime, (timespec_t*)0);
    dts_print_attr_value(interp, K_TIME_DIFF_ATTR,
                         K_TYPE_BINRELTIME,
                         (VOID *)&tempBinTime);
    
    dts_print_attr_text(interp, K_MAXIMUM_INACCURACY_ATTR);
    UTCtoBinAbs(&tempBinTime, &charBuf->maxInacc);
    dts_print_attr_value(interp, K_MAXIMUM_INACCURACY_ATTR,
                         K_TYPE_BINRELTIME,
                         (VOID *)&tempBinTime);
    
    dts_print_attr_text(interp, K_SERVERS_REQUIRED_ATTR);
    dts_print_attr_value(interp, K_SERVERS_REQUIRED_ATTR,
                         K_TYPE_INTEGER32, 
                         (VOID *)&charBuf->minServers);
    
    dts_print_attr_text(interp, K_QUERY_ATTEMPTS_ATTR);
    dts_print_attr_value(interp, K_QUERY_ATTEMPTS_ATTR,
                         K_TYPE_INTEGER32, 
                         (VOID *)&charBuf->queryAttempts);
    
    dts_print_attr_text(interp, K_LAN_TIMEOUT_ATTR);
    UTCtoBinAbs(&tempBinTime, &charBuf->LANtimeOut);
    dts_print_attr_value(interp, K_LAN_TIMEOUT_ATTR,
                         K_TYPE_BINRELTIME,
                         (VOID *)&tempBinTime);
    
    dts_print_attr_text(interp, K_WAN_TIMEOUT_ATTR);
    UTCtoBinAbs(&tempBinTime, &charBuf->WANtimeOut);
    dts_print_attr_value(interp, K_WAN_TIMEOUT_ATTR,
                         K_TYPE_BINRELTIME,
                         (VOID *)&tempBinTime);
    
    dts_print_attr_text(interp, K_SYNCH_ATTR);
    UTCtoBinAbs(&tempBinTime, &charBuf->syncHold);
    dts_print_attr_value(interp, K_SYNCH_ATTR,
                         K_TYPE_BINRELTIME,
                         (VOID *)&tempBinTime);
    
    dts_print_attr_text(interp, K_VARIANT_ATTR);
    tempInteger32 = (charBuf->variant==(long int)VTserver) ? 0 : 1;
    dts_print_attr_value(interp, K_VARIANT_ATTR,
                         K_TYPE_VARIANT,
                         (VOID *)&tempInteger32);
    
    if ( charBuf->variant == (long int)VTserver ) {
        
	dts_print_attr_text(interp, K_COURIER_ROLE_ATTR);
	dts_print_attr_value(interp, K_COURIER_ROLE_ATTR,
                             K_TYPE_COURIER_ROLE, 
                             (VOID *)&charBuf->courierRole);
        
	dts_print_attr_text(interp, K_BACKUP_COURIER_ROLE_ATTR);
	dts_print_attr_value(interp, K_BACKUP_COURIER_ROLE_ATTR,
                             K_TYPE_COURIER_ROLE, 
                             (VOID *)&charBuf->actingCourierRole);
    }
    
    dts_print_attr_text(interp, K_CLOCK_ADJUSTMENT_ATTR);
    dts_print_attr_value(interp, K_CLOCK_ADJUSTMENT_ATTR,
                         K_TYPE_UNSIGNED32, 
                         (VOID *)&charBuf->clockAdjustment);
    
    dts_print_attr_text(interp, K_CLOCK_DRIFT_ATTR);
    dts_print_attr_value(interp, K_CLOCK_DRIFT_ATTR,
                         K_TYPE_UNSIGNED32,
                         (VOID *)&charBuf->driftRate);
    
    dts_print_attr_text(interp, K_CLOCK_RESOLUTION_ATTR);
    dts_print_attr_value(interp, K_CLOCK_RESOLUTION_ATTR,
                         K_TYPE_UNSIGNED32, 
                         (VOID *)&charBuf->clockResolution);
    
#if CONFIG_DECNET
    dts_print_attr_text(interp, K_DNA_VERSION_ATTR);
    dts_print_attr_value(interp, K_DNA_VERSION_ATTR,
                         K_TYPE_VERSION,
                         (VOID *)charBuf->DNAversion);
#endif
    
    dts_print_attr_text(interp, K_DECDTS_VERSION_ATTR);
    dts_print_attr_value(interp, K_DECDTS_VERSION_ATTR,
                         K_TYPE_VERSION,
                         (VOID *)charBuf->DECdts);
    
    dts_print_attr_text(interp, K_TIME_REP_ATTR);
    dts_print_attr_value(interp, K_TIME_REP_ATTR,
                         K_TYPE_VERSION,
                         (VOID *)charBuf->timeVersion);
    
    
    if ( charBuf->variant == (long int)VTserver ) {
        
	dts_print_attr_text(interp, K_TP_PRESENT_ATTR);
	dts_print_attr_value(interp, K_TP_PRESENT_ATTR,
                             K_TYPE_BOOLEAN,
                             (VOID *)&charBuf->hasProvider);
    }
    
    dts_print_attr_text(interp, K_AUTO_TDF_ATTR);
    dts_print_attr_value(interp, K_AUTO_TDF_ATTR,
                         K_TYPE_BOOLEAN,
                         (VOID *)&charBuf->autoTDF);
    
    dts_print_attr_text(interp, K_NEXT_TDF_ATTR);
    dts_print_attr_value(interp, K_NEXT_TDF_ATTR,
                         K_TYPE_BINABSTIME,
                         (VOID *)&charBuf->nextTDFchange);
    
    
    if ( charBuf->variant == (long int)VTserver ) 
    {
        
	dts_print_attr_text(interp, K_SERVER_PRINCIPAL_NAME);
	dts_print_attr_value(interp, K_SERVER_PRINCIPAL_NAME,
                             K_TYPE_STRING, 
                             (VOID *)&charBuf->serverPrincName);
	
	dts_print_attr_text(interp, K_SERVER_ENTRY_NAME);
	dts_print_attr_value(interp, K_SERVER_ENTRY_NAME,
                             K_TYPE_STRING, 
                             (VOID *)&charBuf->serverEntryName);
	
	dts_print_attr_text(interp, K_SERVER_GROUP_NAME);
	dts_print_attr_value(interp, K_SERVER_GROUP_NAME,
                             K_TYPE_STRING, 
                             (VOID *)&charBuf->TSgroupName);
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_all_counters
 *
 * OVERVIEW:    Show all of the counters returned from the DTS entity.
 * 
 *
 * INPUTS:
 *  all_counters
 *    A structure containing data returned from the show counters RPC. 
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
dts_show_all_counters(Tcl_Interp *interp, CtlShowAllCntrs *all_counters)
{
    int                         i, attr;
    Bits64                      a_counter;
    char                        clerk_counter[K_NUM_COUNTERS]; 
                                       /* array of booleans, true if event  *
                                        * is a clerk event, false otherwise *
                                        * Add one, since events are numb'd  *
                                        * starting at 1.                    */
    
    dts_init_counters(clerk_counter);
    
    dts_print_attr_text(interp, K_CREATION_TIME_ATTR);
    dts_print_attr_value(interp, 
                         K_CREATION_TIME_ATTR,
                         K_TYPE_BINABSTIME, 
                         &all_counters->creationTime);
    
    for (i = 0; i < K_NUM_COUNTERS; i++) {
        if ((all_counters->variant == (long int )VTserver) || clerk_counter[i]) {
            attr = i + K_FIRST_COUNTER;

            if ((attr != K_TIME_DIFFS_ATTR) 
                && (attr != K_INVALID_MESSAGE_ATTR)) {
                dts_print_attr_text(interp, i + K_FIRST_COUNTER );
                HYPER_TO_U64(a_counter, all_counters->counters[i]);
                fflush(stdout);
                dts_print_attr_value(interp,
                                     (i + K_FIRST_COUNTER),
                                     K_TYPE_COUNTER64,
                                     &a_counter);
            }
        }
    }
    
    return TCL_OK;
}     


/*
 * FUNCTION:    dts_init_counters
 *
 * OVERVIEW:    Initialize the DTS counter variables.
 * 
 *
 * INPUTS:
 *  None
 *
 * INPUT/OUTPUTS:
 *  counter
 *    An array of counters to initialize.
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  Always TCL_OK.
 */
int
dts_init_counters(char counter[K_NUM_COUNTERS])
{
    int i;
    
    for (i = 0; i < K_NUM_COUNTERS; i++)
        counter[i] = 1;
    counter[K_FAULTY_SERVER_CNTR] = 0;
    counter[K_DIFFERENT_EPOCH_CNTR] = 0;
    counter[K_NO_GLOBAL_CNTR] = 0;
    counter[K_EPOCH_DECLARATION_CNTR] = 0;
    counter[K_UPDATE_COMPLETED_CNTR] = 0;
    counter[K_TIME_PROVIDER_CNTR] = 0;
    counter[K_NOT_RESPONDING_CNTR] = 0;
    counter[K_SERV_NOT_TSG_MEMBER_CNTR] = 0;
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_status
 *
 * OVERVIEW:    Sho the current status of the time server.
 * 
 *
 * INPUTS:
 *  None
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
dts_show_status(Tcl_Interp *interp)
{
    AttrVal_t                     attribute;
    signed32                      show_attr;
    StatusBlk_t                   st_block;
    error_status_t                comm_st;
    
    /* Initialize the status block */
    st_block.status = 0;
    st_block.sysStatus = 0;
    
    /* Get the status attribute */
    attribute.Data.type = VT_Decimal;
    show_attr = K_STATE_ATTR;
    ShowAttrCmd(dcp_dts_binding_h, 
                &show_attr, 
                &attribute, 
                &st_block, 
                &comm_st);
    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
        return TCL_ERROR;
    } 
    
    dts_print_attr_text(interp, K_STATE_ATTR);
    dts_print_attr_value(interp, 
                         K_STATE_ATTR,
                         K_TYPE_STATE,
                         &attribute.Data.setData.userDecimal);
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_uuid
 *
 * OVERVIEW:    Show the uuid of the dts entity.
 * 
 *
 * INPUTS:
 *  None
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
dts_show_uuid(Tcl_Interp *interp)
{
    AttrVal_t                     attribute;
    signed32                      show_attr;
    StatusBlk_t                   st_block;
    error_status_t                comm_st;

    /* Get the uid attribute */
    attribute.Data.type = VT_UUID;
    show_attr = K_UID_ATTR;
    ShowAttrCmd(dcp_dts_binding_h, 
                &show_attr, 
                &attribute, 
                &st_block, 
                &comm_st);
    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
        return TCL_ERROR;
    } 

    dts_print_attr_text(interp, K_UID_ATTR);
    dts_print_attr_value(interp, 
                         K_UID_ATTR,
                         K_TYPE_UID,
                         &attribute.Data.setData.UUID);

    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_local_servers
 *
 * OVERVIEW:    Show the list of known local servers.
 * 
 *
 * INPUTS:
 *  None
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
dts_show_local_servers(Tcl_Interp *interp)
{
    ServerID                      local_servers[MAX_TSERVERS];
    signed32                      max_servers = MAX_TSERVERS;
    signed32                      server_count;
    StatusBlk_t                   st_block;
    error_status_t                comm_st;
    
    ShowLocServersCmd(dcp_dts_binding_h,
                      &max_servers, 
                      local_servers, 
                      &server_count,
                      CtlShowLservers,
                      &st_block,
                      &comm_st);
    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
        return TCL_ERROR;
    } 
    
    if (dts_show_servers(interp, server_count, local_servers) != TCL_OK) {
        return TCL_ERROR;
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_global_servers
 *
 * OVERVIEW:    Show the list of known global servers.
 * 
 *
 * INPUTS:
 *  None
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
dts_show_global_servers(Tcl_Interp *interp)
{
    ServerID                      global_servers[MAX_TSERVERS];
    signed32                      max_servers = MAX_TSERVERS;
    signed32                      server_count;
    StatusBlk_t                   st_block;
    error_status_t                comm_st;
    
    ShowGblServersCmd(dcp_dts_binding_h,
                      &max_servers, 
                      global_servers, 
                      &server_count,
                      CtlShowGservers,
                      &st_block,
                      &comm_st);
    if (dts_check_status(interp, st_block, comm_st) != TCL_OK) {
        return TCL_ERROR;
    } 
    
    if (dts_show_servers(interp, server_count, global_servers) != TCL_OK) {
        return TCL_ERROR;
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_get_attr_type
 *
 * OVERVIEW:    Return the data type of the attribute.
 * 
 *
 * INPUTS:
 *  attribute
 *    The number of the attribute to be looked up.
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  dataType
 *    A pointer to the data type of the attribute.
 *
 * RETURNS:
 *  Always TCL_OK.
 */
int
dts_get_attr_type(int attribute, SetShow_t *dataType)
{
    *dataType = VT_Decimal;   /* quiet compiler warnings */
    
    switch (attribute) {
      case K_CURRENT_TIME_ATTR:
      case K_LAST_SYNCH_ATTR:
      case K_CREATION_TIME_ATTR:
      case K_NEXT_TDF_ATTR:
        *dataType = VT_Bintime;
        break;
        
      case K_CHECK_INTERVAL_ATTR:
      case K_ERROR_TOLLERANCE_ATTR:
      case K_TIME_DIFF_ATTR:
      case K_MAXIMUM_INACCURACY_ATTR:
      case K_LAN_TIMEOUT_ATTR:
      case K_WAN_TIMEOUT_ATTR:
      case K_SYNCH_ATTR:            
        *dataType = VT_Reltime;
        break;
        
      case K_EPOCH_NUMBER_ATTR:
      case K_SERVERS_REQUIRED_ATTR:
      case K_QUERY_ATTEMPTS_ATTR:
      case K_CLOCK_ADJUSTMENT_ATTR:
      case K_CLOCK_DRIFT_ATTR:
      case K_STATE_ATTR:
      case K_VARIANT_ATTR:
      case K_LOCAL_FAULT_ATTR:
      case K_FAULTY_SERVER_ATTR:
      case K_DIFFERENT_EPOCH_ATTR:
      case K_TOO_FEW_ATTR:
      case K_PROTOCOL_VERSION_ATTR:
      case K_TIME_REPS_ATTR:
      case K_NO_GLOBAL_ATTR:
      case K_NOT_RESPONDING_ATTR:
      case K_CLOCK_SET_ATTR:
      case K_EPOCH_DECLARATION_ATTR:
      case K_TIME_DIFFS_ATTR:
      case K_SYSTEM_ERROR_ATTR:
      case K_SYNCH_COMPLETED_ATTR:
      case K_UPDATE_COMPLETED_ATTR:
      case K_STARTUP_COMPLETED_ATTR:
      case K_SHUTDOWN_COMPLETED_ATTR:
      case K_INSUFFICIENT_RESOURCE_ATTR:
      case K_TIME_PROVIDER_ATTR:
      case K_TIME_PROVIDER_TIMEOUT_ATTR:
      case K_SERV_NOT_TSG_MEMBER_ATTR:
      case K_LOC_SERV_NOT_TSG_MEMBER_ATTR:
      case K_CLOCK_RESOLUTION_ATTR:
        *dataType = VT_Decimal;
        break;
        
      case K_DECDTS_VERSION_ATTR:
      case K_TIME_REP_ATTR:
        *dataType = VT_String;
        break;

      case K_SERVICE_TRACE_ATTR:
      case K_COMM_TRACE_ATTR:
      case K_SYNCH_TRACE_ATTR:
      case K_ARITH_TRACE_ATTR:
      case K_TP_PRESENT_ATTR:
      case K_AUTO_TDF_ATTR:
        *dataType = VT_Boolean;
        break;
        
      case K_BACKUP_COURIER_ROLE_ATTR:
      case K_COURIER_ROLE_ATTR:
        *dataType = VT_CourierRole;
        break;
#if CONFIG_RPC
      case K_SERVER_PRINCIPAL_NAME:
        *dataType = VT_String;
        break;
        
      case K_SERVER_ENTRY_NAME:
        *dataType = VT_String;
        break;
        
      case K_SERVER_GROUP_NAME:
        *dataType = VT_String;
        break;
        
#endif
      case K_UID_ATTR:
        *dataType = VT_UUID;
        break;
      default:
        *dataType = VT_String;
        break;
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    dts_show_servers
 *
 * OVERVIEW:    Print either the local or global server set contained in the 
 *              service response message.
 * 
 *
 * INPUTS:
 *  Count
 *    A count of the servers returned from DTS entity.
 *
 *  serverSet
 *    An array listing the servers returned from the DTS entity.
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
dts_show_servers(Tcl_Interp *interp, long Count, ServerID serverSet[])
{
    int i;
    ServerID   *serverID;
    utc_t utcDiff;
    utc_t SyncTime;
    utc_t ServTime;
	UTCValue tvalue = { 0 }, inacc = { 0 };
    
    for (i=0; i < Count; i++) {
        serverID = &serverSet[i];

        Tcl_AppendResult(interp,
                         "\n{",
                         serverID->isGlobal ?
                         DCP_T_DTS_GLOBAL_SERVERS_HEADER : DCP_T_DTS_LOCAL_SERVERS_HEADER,
                         "\n {",
                         DCP_T_DTS_NAME_HEADER,
                         " ",
                         serverID->nodeName.buffer,
                         "}",
                         (char *)NULL);

        /* 
         * print the last use of server - if this server has been used before,
         * convert the UTC to local time by calling the API and print the times
         * in ISO format.  
         */
        
        SyncTime = serverID->sync;
        (void)utc_comptime(&tvalue, &inacc, (long *)0, &SyncTime);
        Tcl_AppendResult(interp, "\n {", DCP_T_DTS_TIME_LAST_POLLED_HEADER, (char *)NULL);

        if (U64FITSIN32(tvalue)) {
            Tcl_AppendResult(interp, " ", DCP_T_DTS_NEVER, "}", (char *)NULL);
        }
        else {
            dts_print_attr_value(interp, 0, K_TYPE_BINABSTIME, 
                                 (VOID *)&SyncTime );
        }
        
        /* print the reported time */
        ServTime = serverID->serv;
        (void) utc_comptime(&tvalue, &inacc, (long *)0, &ServTime);
        Tcl_AppendResult(interp, "\n {", DCP_T_DTS_LAST_OBS_TIME_HEADER, (char *)NULL);

        if (U64FITSIN32(tvalue)) {
            Tcl_AppendResult(interp, " ", DCP_T_DTS_NEVER, "}", (char *)NULL);
        }
        else {
            dts_print_attr_value(interp, 0, K_TYPE_BINABSTIME, 
                                 (VOID *)&ServTime);
        }

        Tcl_AppendResult(interp, "\n {", DCP_T_DTS_LAST_OBS_SKEW_HEADER, (char *)NULL);
        utc_comptime(&tvalue, &inacc, (long *)0, &ServTime);

        if (!U64FITSIN32(tvalue)) {
            (void) utc_subtime(&utcDiff, &ServTime, &SyncTime);
            (void) utc_abstime(&utcDiff, &utcDiff);
            dts_print_attr_value(interp, 
                                 0, 
                                 K_TYPE_BINRELTIME, 
                                 (VOID *)&utcDiff);
        }
        else {
            Tcl_AppendResult(interp, " ", DCP_T_DTS_NEVER, "}", (char *)NULL);
        }
        
        Tcl_AppendResult(interp, "\n {", DCP_T_DTS_IN_LAST_SYNC_HEADER, (char *)NULL);
        if (serverID->lastUsed) {
            Tcl_AppendResult(interp, " ", DCP_T_DTS_TRUE, "}", (char *)NULL);
        } 
        else {
            Tcl_AppendResult(interp, " ", DCP_T_DTS_FALSE, "}", (char *)NULL);
        }    

        Tcl_AppendResult(interp, "\n {", DCP_T_DTS_TRANSPORT_HEADER, (char *)NULL);
        switch((int )serverID->transport) {
          case K_PROT_IEEE802: 
            Tcl_AppendResult(interp, " ", DCP_T_DTS_IEEE, "}", (char *)NULL);
            break;
          case K_PROT_DECNET:  
            Tcl_AppendResult(interp, " ", DCP_T_DTS_DECNET, "}", (char *)NULL);
            break;
          case K_PROT_UDP_IP:  
            Tcl_AppendResult(interp, " ", DCP_T_DTS_UDP_IP, "}", (char *)NULL);
            break;
          case K_PROT_TCP_IP:  
            Tcl_AppendResult(interp, " ", DCP_T_DTS_TCP_IP, "}", (char *)NULL);
            break;
          case K_PROT_RPC:  
            Tcl_AppendResult(interp, " ", DCP_T_DTS_RPC, "}", (char *)NULL);
            break;
          default: 
            Tcl_AppendResult(interp, " ", DCP_T_DTS_UNKNOWN, "}", (char *)NULL);
            break;
        }

        Tcl_AppendResult(interp, "}", (char *)NULL);
    }
    
    return TCL_OK;
}


/*
 * FUNCTION:    emul
 *
 * OVERVIEW:    32-bit signed quantity * 32-bit signed quantity producing 
 *              64-bit signed result. This routine assumes long's contain 
 *              at least 32 bits. It makes no assumptions about byte 
 *              orderings.
 *
 * INPUTS:
 *  u, v       
 *    Are the numbers to be multiplied passed by value.
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  prodPtr
 *    Is a pointer to the 64-bit result.
 *
 * RETURNS:
 *  void
 */
void
emul(long int u, long int v, Bits64 *prodPtr)
{
#if LONG_BIT >= 64
    *prodPtr = u * v;
#else
    uemul((unsigned long)u, (unsigned long)v, prodPtr);
    
    /* Correct for signed multiply */
    if (u < 0)
        prodPtr->hi -= v;
    if (v < 0)
        prodPtr->hi -= u;
#endif
}


/*
 * FUNCTION:    uediv
 *
 * OVERVIEW:    64-bit unsigned quantity / 32-bit unsigned quantity producing 
 *              32-bit unsigned quotient and 32-bit unsigned remainder. This 
 *              routine assumes long's contain at least 32 bits. It makes no 
 *              assumptions about byte orderings.
 * 
 *
 * INPUTS:
 *  DividendPtr
 *    Pointer to number to be divided.
 *  Divisor
 *    Divisor
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  QuotPtr
 *    Pointer to quotient
 *  RemdPtr
 *    Pointer to remainder
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int 
uediv(Bits64 *dividendPtr, 
      unsigned long divisor, 
      unsigned long *quotPtr, 
      unsigned long *remdPtr)
{
    /* Following the notation in Knuth, Vol. 2 */

    unsigned long int u1, u2, u3, u4, v1, v2, temp, trial;
    unsigned long int q1, q2;
    unsigned long int v, uhigh, ulow;

    /* D1 */
    if (!(v = divisor))
        return(-1);

#if LONG_BIT >= 64
    if (remdPtr) {
        *remdPtr = *dividendPtr % divisor;
    }
    *quotPtr = *dividendPtr / divisor;
    return TCL_OK;
#else
    uhigh = dividendPtr->hi;
    ulow = dividendPtr->lo;
    while (!(v & 0x80000000UL)) {
        v = v << 1;
        if (uhigh & 0x80000000UL)
            return(-1);

        uhigh = (uhigh << 1) | ulow >> 31;
        ulow = ulow << 1;
    }        

    v1 = v >> 16;
    v2 = v & 0xffff;

    u1 = uhigh >> 16;
    u2 = uhigh & 0xffff;
    u3 = ulow >> 16;
    u4 = ulow & 0xffff;

    if (v <= uhigh)
        return(-1);

    /* D3                        Calculate Trial divisor */
    if (u1 == v1)
        trial = 0xffff;
    else
        trial = uhigh / v1;

    while (!((uhigh - v1 * trial) & 0xffff0000UL) &&
           ((v2 * trial) > (((uhigh - v1 * trial) << 16) + u3)))
        trial--;

    /* D4                        Multiply and subtract      */
    temp = v2 * trial;        /* Replace (u1,u2,u3) by      */
    u3 -= temp & 0xffff;      /* (u1,u2,u3) - trial*(v1,v2) */
    if (u3 & 0xffff0000UL) {
        u2--;
        u3 &= 0xffff;
    }
    u2 -= (v1 * trial + (temp >> 16));
    u2 &= 0xffff;

    /* D5 */
    q1 = trial;

    /* The complication of step D6 can not occur, since n = 2! */

    /* D3                        Calculate Trial divisor */
    uhigh = (u2 << 16) + u3;
    if (u2 == v1)
        trial = 0xffff;
    else
        trial = uhigh / v1;

    while (!((uhigh - v1 * trial) & 0xffff0000UL) &&
           ((v2 * trial) > (((uhigh - v1 * trial) << 16) + u4)))
        trial--;

    /* D4                        Multiply and subtract      */
    temp = v2 * trial;        /* Replace (u2,u3,u4) by      */
    u4 -= temp & 0xffff;      /* (u2,u3,u4) - trial*(v1,v2) */
    if (u4 & 0xffff0000UL) {
        u3--;
        u4 &= 0xffff;
    }
    u3 -= (v1 * trial + (temp >> 16));
    u3 &= 0xffff;

    /* D5 */
    q2 = trial;

    /* The complication of step D6 can not occur, since n = 2! */

    /* D8                        Unnormalize */
    *quotPtr = (q1 << 16) + q2;

    if (remdPtr == NULL)
        return(0);

    v = divisor;
    uhigh = (u3 << 16) + u4;

    while (!(v & 0x80000000UL)) {
        v = v << 1;
        uhigh = uhigh >> 1;
    }        
    *remdPtr = uhigh;
    return(0);
#endif
}


/*
 * FUNCTION:    uemul
 *
 * OVERVIEW:    32-bit unsigned quantity * 32-bit unsigned quantity
 *              producing 64-bit unsigned result. This routine assumes
 *              long's contain at least 32 bits. It makes no assumptions
 *              about byte orderings.
 * 
 *
 * INPUTS:
 *  u, v       
 *    Are the numbers to be multiplied passed by value.
 *
 * INPUT/OUTPUTS:
 *  None
 *
 * OUTPUTS:
 *  prodPtr
 *    Is a pointer to the 64-bit result.
 *  None
 *
 * RETURNS:
 *  void
 */
void
uemul(unsigned long u, unsigned long v, Bits64 *prodPtr)
{
    /* Following the notation in Knuth, Vol. 2 */

#if LONG_BIT >= 64
    *prodPtr = u * v;
#else
    unsigned long int   u1, u2, v1, v2, temp;

    u1 = u >> 16;
    u2 = u & 0xffff;
    v1 = v >> 16;
    v2 = v & 0xffff;

    temp = u2 * v2;
    prodPtr->lo = temp & 0xffff;
    temp = u1 * v2 + (temp >> 16);
    prodPtr->hi = temp >> 16;
    temp = u2 * v1 + (temp & 0xffff);
    prodPtr->lo += (temp & 0xffff) << 16;
    prodPtr->hi += u1 * v1 + (temp >> 16);
#endif
}
