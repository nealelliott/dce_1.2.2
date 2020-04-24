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
 * $Log: rgy_sa_replist.c,v $
 * Revision 1.1.9.1  1996/08/09  11:49:50  arvind
 * 	Merge changes.
 * 	[1996/06/12  13:31 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/15  18:49 UTC  truitt  /main/HPDCE02/6]
 *
 * 	Merge replist changes.
 * 	[1996/02/15  18:47 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts16871 (OT13316): Have the registry show operation support
 * 	return of slave replica list information.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  13:15 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.3  1996/02/18  19:14:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:00:47  marty]
 * 
 * Revision 1.1.6.2  1995/12/08  21:22:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/09/11  14:20 UTC  truitt
 * 	Merge registry changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts46/1  1995/09/11  14:18 UTC  truitt
 * 	CHFts16100: Comprehensive clean-up of registry commands.
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/05  18:52 UTC  truitt
 * 	Merge cell alias changes.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts33/1  1995/06/05  18:48 UTC  truitt
 * 	CHFts15405: Minor changes to some of the registry functions
 * 	so that cell alias names will be recognized.
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/23  19:18 UTC  truitt
 * 	Merge header work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  19:17 UTC  truitt
 * 	CHFts14679: Move field headers to sams file.
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  17:03 UTC  truitt
 * 	Merge warranty patch and message change work.
 * 
 * 	HP revision /main/HPDCE02/truitt_dcecp_chfts29/1  1995/05/15  17:01 UTC  truitt
 * 	CHFts15237: Minor message cleanup.
 * 
 * 	HP revision /main/HPDCE02/SEC_migrate_merge/1  1995/04/25  18:44 UTC  truitt
 * 	Branch for migration merge.
 * 
 * 	HP revision /main/SEC_migrate/1  1995/04/05  15:28 UTC  truitt
 * 	Merge warranty patch work.
 * 
 * 	HP revision /main/truitt_rgy_wp/1  1995/03/29  14:38 UTC  truitt
 * 	Warranty patch work: have the registry 'show' command
 * 	return the correct version number information.
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/14  16:28 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts13/1  1995/02/14  16:26 UTC  truitt
 * 	CHFts14285: Call the correct macros to set an
 * 	error condition, so the _e variable gets set.
 * 	[1995/12/08  17:56:52  root]
 * 
 * Revision 1.1.2.26  1994/10/05  21:59:26  salamone
 * 	CR12447 - set _b(sec) to fullname
 * 	[1994/10/05  21:21:44  salamone]
 * 
 * Revision 1.1.2.25  1994/09/28  17:54:37  kevins
 * 	OT 12376 modified registry unavailable message delivery
 * 	[1994/09/28  17:50:58  kevins]
 * 
 * Revision 1.1.2.24  1994/09/22  16:29:56  salamone
 * 	dcp_rgy_sa_list_replicas() returns pointer to local storage.
 * 	[1994/09/22  16:28:48  salamone]
 * 
 * Revision 1.1.2.23  1994/09/15  21:28:58  salamone
 * 	CR12169 - If last communication status is bad, display error msg in braces.
 * 	[1994/09/15  21:28:14  salamone]
 * 
 * Revision 1.1.2.22  1994/09/13  19:12:41  kevins
 * 	OT 11641 list syntax not supported in catalog commands.
 * 	[1994/09/13  18:46:44  kevins]
 * 
 * Revision 1.1.2.21  1994/08/31  23:04:12  salamone
 * 	CR11916 - Add _b(sec) support.
 * 	[1994/08/31  23:02:40  salamone]
 * 
 * Revision 1.1.2.20  1994/08/25  18:59:31  salamone
 * 	CR11393 - String binding is displayed in TCL syntax.
 * 	[1994/08/25  18:45:56  salamone]
 * 
 * Revision 1.1.2.19  1994/08/24  21:52:50  salamone
 * 	CR11453 - Display tower in binary format first. If not, then display
 * 		  untranslatable msg.
 * 	[1994/08/24  21:52:16  salamone]
 * 
 * Revision 1.1.2.18  1994/08/24  15:04:43  salamone
 * 	CR11838 - Add support for "registry set" command.
 * 	[1994/08/23  21:25:00  salamone]
 * 
 * Revision 1.1.2.17  1994/08/19  18:04:16  salamone
 * 	CR10683 - Use generic routines to handle 1 sbind in TCL.
 * 	[1994/08/19  17:53:48  salamone]
 * 
 * Revision 1.1.2.16  1994/08/16  21:16:20  salamone
 * 	CR11723 - Bypass master replica for registry show -master.
 * 	[1994/08/16  20:43:41  salamone]
 * 
 * Revision 1.1.2.15  1994/08/15  17:28:40  salamone
 * 	CR11636: Allow registry catalog to take an argument.
 * 	CR11651: Generate errors with bad option or bad argument.
 * 	[1994/08/15  17:24:08  salamone]
 * 
 * Revision 1.1.2.14  1994/08/08  18:51:55  melman
 * 	change strdup to dce_strdup
 * 	[1994/08/08  17:52:28  melman]
 * 
 * Revision 1.1.2.13  1994/08/05  18:30:13  salamone
 * 	CR11453 - Display msg if tower is untranslatable.
 * 	[1994/08/05  18:06:14  salamone]
 * 
 * Revision 1.1.2.12  1994/07/22  18:03:36  salamone
 * 	CR11393 - Accept setting _s(sec) with a TCL syntax string binding
 * 	[1994/07/22  17:57:13  salamone]
 * 
 * Revision 1.1.2.11  1994/07/20  19:28:03  salamone
 * 	Always display replica name instead of string binding.
 * 	[1994/07/20  19:23:01  salamone]
 * 
 * Revision 1.1.2.10  1994/06/30  17:52:01  salamone
 * 	CR11134 - Handle TCL syntax string bindings properly.
 * 	[1994/06/30  17:16:33  salamone]
 * 
 * Revision 1.1.2.9  1994/06/29  17:44:43  salamone
 * 	Add brace for displaying addresses
 * 	[1994/06/29  17:44:26  salamone]
 * 
 * Revision 1.1.2.8  1994/06/24  20:02:02  salamone
 * 	CR11056 - Slave replica specification not accepted.
 * 	[1994/06/24  20:00:15  salamone]
 * 
 * Revision 1.1.2.7  1994/06/20  21:03:03  salamone
 * 	Merged with changes from 1.1.2.6
 * 	[1994/06/20  21:02:46  salamone]
 * 
 * 	Fix indentation for showing addresses.
 * 	[1994/06/20  20:59:32  salamone]
 * 
 * 	Added "registry show" and "registry dump" support.
 * 	[1994/06/20  19:44:41  salamone]
 * 
 * Revision 1.1.2.6  1994/06/09  16:02:20  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:54  devsrc]
 * 
 * Revision 1.1.2.5  1994/06/06  16:50:56  salamone
 * 	Enhanced registry modify support.
 * 	[1994/06/06  16:48:55  salamone]
 * 
 * Revision 1.1.2.4  1994/05/31  21:18:03  salamone
 * 	For registry commands related to "sec_admin" functionality, invoke
 * 	dcp__rgy_sa_open_site() instead of open_rgy_site().
 * 	[1994/05/31  20:45:56  salamone]
 * 
 * Revision 1.1.2.3  1994/05/26  17:56:33  salamone
 * 	Ensure binding to proper site for all regi cmds.  Added another
 * 	flag to open_rgy_site() to bind to a specific registry site
 * 	for update.
 * 	[1994/05/26  16:39:06  salamone]
 * 
 * Revision 1.1.2.2  1994/05/24  19:31:24  salamone
 * 	Added registry synchronize and delete support.
 * 	[1994/05/24  19:29:44  salamone]
 * 
 * Revision 1.1.2.1  1994/05/12  18:29:42  salamone
 * 	Initial version of rgy_sa_replist.c
 * 	[1994/05/12  18:28:27  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * rgy_sa_replist.c
 *
 * DESCRIPTION:
 * This module contains the functions to display and manipulate 
 * replication information.
 */

/*
 * INCLUDE FILES
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dce/macros.h>
#include <dce/utc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dcesadmsg.h>
#include <dce/dce_error.h>

#include <dce/tcl.h>
#include <dcecp.h>
#include <util_binding.h>
#include <util_general.h>
#include <util_var.h>
#include <rgy_sa_replist.h>

/*
 * DECLARATIONS
 */
#define DCP_C_RGY_SA_NAME               "name"
#define DCP_C_RGY_SA_CELL               "cell"
#define DCP_C_RGY_SA_UUID               "uuid"
#define DCP_C_RGY_SA_TYPE               "type"
#define DCP_C_RGY_SA_STATUS             "status"
#define DCP_C_RGY_SA_PROP_STATUS        "propstatus"
#define DCP_C_RGY_SA_LAST_UPD_TIME      "lastupdtime"
#define DCP_C_RGY_SA_LAST_UPD_SEQ       "lastupdseq"
#define DCP_C_RGY_SA_LAST_UPD_SEQ_SENT  "lastupdseqsent"
#define DCP_C_RGY_SA_ADDRS              "addresses"
#define DCP_C_RGY_SA_MASTER_ADDRS       "masteraddrs"
#define DCP_C_RGY_SA_MASTER_SEQ         "masterseqnum"
#define DCP_C_RGY_SA_MASTER_UUID        "masteruuid"
#define DCP_C_RGY_SA_VERSION            "version"
#define DCP_C_RGY_SA_SUPPORTEDVERSIONS  "supportedversions"
#define DCP_C_RGY_SA_MASTER_QUEUE       "updseqqueue"
#define DCP_C_RGY_SA_NUM_UPD_TO_GO      "numupdtogo"
#define DCP_C_RGY_SA_COMM_STATE         "commstate"
#define DCP_C_RGY_SA_LAST_COMM_STAT     "lastcommstatus"

/*
 * Prop state values
 */
#define DCP_RS_C_REPLICA_PROP_UNKNOWN  "unavailable"
#define DCP_RS_C_REPLICA_PROP_INIT     "initmarked"
#define DCP_RS_C_REPLICA_PROP_INITING  "initing"
#define DCP_RS_C_REPLICA_PROP_UPDATE   "update"
#define DCP_RS_C_REPLICA_PROP_DELETE   "delete"
#define DCP_RS_C_REPLICA_PROP_BAD_SW   "badversion"
#define DCP_RS_C_REPLICA_PROP_DEFAULT  "unknown"

/*
 * Comm state values
 */
#define DCP_RS_C_REPLICA_COMM_OK          "ok"
#define DCP_RS_C_REPLICA_COMM_SHORT_FAIL  "shortfail"
#define DCP_RS_C_REPLICA_COMM_LONG_FAIL   "longfail"

/*
 * Registry state values
 */
#define DCP_RS_C_STATE_UNKNOWN_TO_MASTER  "unknown"
#define DCP_RS_C_STATE_UNINITIALIZED      "uninitialized"
#define DCP_RS_C_STATE_INITIALIZING       "initializing"
#define DCP_RS_C_STATE_IN_SERVICE         "enabled"
#define DCP_RS_C_STATE_RENAMING           "savingdb"
#define DCP_RS_C_STATE_COPYING_DBASE      "copyingdb"
#define DCP_RS_C_STATE_IN_MAINTENANCE     "disabled"
#define DCP_RS_C_STATE_MKEY_CHANGING      "changingkey"
#define DCP_RS_C_STATE_BECOMING_MASTER    "becomingmaster"
#define DCP_RS_C_STATE_CLOSED             "closed"
#define DCP_RS_C_STATE_DELETED            "deleted"
#define DCP_RS_C_STATE_BECOMING_SLAVE     "becomingslave"
#define DCP_RS_C_STATE_DUP_MASTER         "dupmaster"
#define DCP_RS_C_STATE_DEFAULT            "unavailable"

/*
 * INTERNAL VARIABLES
 */
static rs_replica_item_full_t  *freplist = NULL;
static rs_replica_item_t       *replist = NULL;
static unsigned32              num_reps = 0;
static unsigned32              num_freps = 0;

/*
 * PROTOTYPES
 */
error_status_t dcp__rgy_sa_obtain_full_replist(void);
boolean32 dcp__rgy_sa_find_rep_item_by_name(unsigned_char_p_t, rs_replica_item_t **);
boolean32 dcp__rgy_sa_find_master_by_name(unsigned_char_p_t);
signed32 dcp__rgy_sa_seqno_cmp(rs_update_seqno_t *, rs_update_seqno_t *);
void dcp__rgy_sa_print_propagation_info(Tcl_Interp *, int);
void dcp__rgy_sa_print_towers(Tcl_Interp *, char *, rs_replica_twr_vec_p_t);
boolean32 dcp__rgy_sa_state_ok(unsigned32);


/*
 * FUNCTION:
 * dcp__rgy_sa_obtain_replist()
 *
 * OVERVIEW:
 * This routine retrieves a copy of the replica list.
 * The registry server handle can be any registry.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status: A pointer to the completion status. On successful completion,
 *         the routine returns error_status_ok. Otherwise, it returns
 *         an error.
 */
error_status_t dcp__rgy_sa_obtain_replist()
{
    uuid_t          marker;
    unsigned32	    tmp_nreps = 50;		/* Try 50 first */
    error_status_t  status;

    CLEAR_STATUS(&status);

    /*
     * Clean up previous replist if it exists and allocate a
     * new one.
     */
    if (replist != NULL) {
        free(replist);
    }
    replist = (rs_replica_item_p_t)malloc(tmp_nreps * sizeof(rs_replica_item_t));
    if (replist == NULL) {
        SET_STATUS(&status, dcp_s_no_memory);
        return status;
    }

    /*
     * To start reading at the beginning of the replica list,
     * set marker to uuid_nil.
     */
    uuid_create_nil(&marker, &status);
    if (BAD_STATUS(&status)) {
        return status;
    }

    sec_rgy_replist_read(rgy_context,
                         &marker,
                         tmp_nreps,
                         &num_reps,
                         replist,
                         &status);
    if (BAD_STATUS(&status)) {
        free(replist);
        replist = NULL;
        num_reps = 0;
        return status;
    }

    /*
     * Determine if more replica information needs to be obtained.
     */
    if (num_reps > tmp_nreps) {
        free(replist);
        tmp_nreps = num_reps;
        replist = (rs_replica_item_p_t)malloc(tmp_nreps * sizeof(rs_replica_item_t));
        if (replist == NULL) {
            SET_STATUS(&status, dcp_s_no_memory);
            num_reps = 0;
            return status;
        }
        sec_rgy_replist_read(rgy_context, 
                             &marker, 
                             tmp_nreps, 
                             &num_reps,
                             replist, 
                             &status);
        if (BAD_STATUS(&status)) {
            free(replist);
            replist = NULL;
            num_reps = 0;
        }
    }

    return status;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_obtain_full_replist()
 *
 * OVERVIEW:
 * This routine retrieves a copy of the full replica list.
 * The registry server handle should be from the master registry.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status: A pointer to the completion status. On successful completion,
 *         the routine returns error_status_ok. Otherwise, it returns
 *         an error.
 */
error_status_t dcp__rgy_sa_obtain_full_replist()
{
    uuid_t          marker;
    unsigned32	    tmp_nreps = 50;		/* Try 50 first */
    error_status_t  status;

    CLEAR_STATUS(&status);

    /*
     * Clean up previous freplist if it exists and allocate a
     * new one.
     */
    if (freplist != NULL) {
        free(freplist);
    }
    freplist = (rs_replica_item_full_p_t)malloc(tmp_nreps * sizeof(rs_replica_item_full_t));
    if (freplist == NULL) {
        SET_STATUS(&status, dcp_s_no_memory);
        return status;
    }

    /*
     * To start reading at the beginning of the replica list,
     * set marker to uuid_nil.
     */
    uuid_create_nil(&marker, &status);
    if (BAD_STATUS(&status)) {
        return status;
    }

    /*
     * If registry server is NOT the master, then this call will fail.
     */
    sec_rgy_replist_read_full(rgy_context,
                              &marker,
                              tmp_nreps,
                              &num_freps,
                              freplist,
                              &status);
    if (BAD_STATUS(&status)) {
        free(freplist);
        freplist = NULL;
        num_freps = 0;
        return status;
    }

    /*
     * Determine if more replica information needs to be obtained.
     */
    if (num_freps > tmp_nreps) {
        free(freplist);
        tmp_nreps = num_freps;
        freplist = (rs_replica_item_full_p_t)malloc(
                       tmp_nreps * sizeof(rs_replica_item_full_t));
        if (freplist == NULL) {
            SET_STATUS(&status, dcp_s_no_memory);
            num_freps = 0;
            return status;
        }
        sec_rgy_replist_read_full(rgy_context, 
                                  &marker, 
                                  tmp_nreps, 
                                  &num_freps,
                                  freplist, 
                                  &status);
        if (BAD_STATUS(&status)) {
            free(freplist);
            freplist = NULL;
            num_freps = 0;
        }
    }

    return status;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_do_catalog()
 *
 * OVERVIEW:
 * This routine returns a list of fully-qualified names of the 
 * security servers running in the cell.
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp: Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convenience variables.
 *
 * master_option: To list the master replica only.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void dcp_rgy_sa_do_catalog(Tcl_Interp *interp,
                           boolean32 master_option)
{
    int   i;
    char  output_string[BUFSIZ];

    for (i = 0; i < num_reps; i++) {
        if (((replist[i].master) && (master_option)) || (!master_option)) {
            sprintf(output_string,
                    "%s/%s",
                    (char *)site_cache[realtime_cache].cell_name,
                    (char *)replist[i].rep_name);
            Tcl_AppendResult(interp, output_string, (char *)NULL);
        }
        if ((i < (num_reps - 1)) && (!master_option)) {
            Tcl_AppendResult(interp, "\n", (char *)NULL);
        }
    }
}


/*
 * FUNCTION:
 * dcp_rgy_sa_do_show_replica_info()
 *
 * OVERVIEW:
 * This routine returns the replica information that is kept
 * by the replica specified in the argument or in _s(sec).
 *
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp: Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int dcp_rgy_sa_do_show_replica_info(Tcl_Interp *interp)
{
    char                       output_string[BUFSIZ];
    char                       time_string[UTC_MAX_STR_LEN];
    char                       *type_svc;
    char                       *unknown_svc;
    unsigned_char_p_t          name1;
    rs_replica_info_t          rgy_info;
    rs_replica_item_t          *item_p;
    rs_replica_twr_vec_p_t     rtwrs = NULL;
    rs_replica_twr_vec_p_t     mtwrs = NULL;
    rs_replica_sw_vers_info_t  *version_info;
    unsigned32                 number_of_versions = 0;
    unsigned32                 current_version = 0;
    int                        i;
    error_status_t             status;

    /*
     * Initialization.
     */
    CLEAR_STATUS(&status);

    sec_rgy_rep_admin_info_vers(rgy_context,
                                &rgy_info,
                                &rtwrs,
                                &mtwrs,
                                &number_of_versions,
                                &version_info,
                                &current_version,
                                &status);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * Display fully qualified CDS name of replica.
     */
    if (strchr((char *)site_cache[realtime_cache].site, ':')) {
        dcp__rgy_sa_find_rep_item_by_uuid(&rgy_info.rep_id, &item_p);
        sprintf(output_string, "{%s %s/%s}", DCP_C_RGY_SA_NAME,
                (char *)site_cache[realtime_cache].cell_name,
                (char *)item_p->rep_name);
    }
    else {
        sprintf(output_string, "{%s %s}", DCP_C_RGY_SA_NAME,
                (char *)site_cache[realtime_cache].site);
    }
    Tcl_AppendResult(interp, output_string, (char *)NULL);

    /*
     * Display type if replica is a "master" or "slave".
     */
    if (!rgy_info.master) {  
        type_svc = (char *)dce_sprintf(dcp_t_rgy_slave);
    }
    else {
        type_svc = (char *)dce_sprintf(dcp_t_rgy_master);
    }
    if (type_svc == NULL) {
        DCP_SET_MISC_ERROR(dcp_s_no_memory);
    }
    sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_TYPE, type_svc);
    Tcl_AppendResult(interp, output_string, (char *)NULL);
    free(type_svc);

    /*
     * Display name of cell that the replica is in.
     */
    sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_CELL,
            (char *)site_cache[realtime_cache].cell_name);
    Tcl_AppendResult(interp, output_string, (char *)NULL);

    /*
     * Display UUID of the replica.
     */
    uuid_to_string(&rgy_info.rep_id, &name1, &status);
    sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_UUID, (char *)name1);
    Tcl_AppendResult(interp, output_string, (char *)NULL);

    /*
     * Display state of the replica.
     */
    Tcl_AppendResult(interp, "\n{", DCP_C_RGY_SA_STATUS, " ", NULL);
    switch (rgy_info.rep_state) {
        case rs_c_state_unknown_to_master:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_UNKNOWN_TO_MASTER, "}", NULL);
            break;
        case rs_c_state_uninitialized:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_UNINITIALIZED, "}", NULL);
            break;
        case rs_c_state_initializing:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_INITIALIZING, "}", NULL);
            break;
        case rs_c_state_in_service:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_IN_SERVICE, "}", NULL);
            break;
        case rs_c_state_renaming:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_RENAMING, "}", NULL);
            break;
        case rs_c_state_copying_dbase:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_COPYING_DBASE, "}", NULL);
            break;
        case rs_c_state_in_maintenance:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_IN_MAINTENANCE, "}", NULL);
            break;
        case rs_c_state_mkey_changing:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_MKEY_CHANGING, "}", NULL);
            break;
        case rs_c_state_becoming_master:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_BECOMING_MASTER, "}", NULL);
            break;
        case rs_c_state_closed:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_CLOSED, "}", NULL);
            break;
        case rs_c_state_deleted:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_DELETED, "}", NULL);
            break;
        case rs_c_state_becoming_slave:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_BECOMING_SLAVE, "}", NULL);
            break;
        case rs_c_state_dup_master:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_DUP_MASTER, "}", NULL);
            break;
        default:
            Tcl_AppendResult(interp, DCP_RS_C_STATE_DEFAULT, "}", NULL);
            break;
    }

    /*
     * If state OK, then replica info is meaningful.
     */
    if (dcp__rgy_sa_state_ok(rgy_info.rep_state)) {
        /*
         * The localized date/time the replica was last updated.
         */
        sec_to_date_string(rgy_info.last_upd_ts.sec, time_string);
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_LAST_UPD_TIME, time_string);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

        /*
         * The sequence number of last update the replica received.
         */
        sprintf(output_string, "\n{%s %d.%d}", DCP_C_RGY_SA_LAST_UPD_SEQ,
                rgy_info.last_upd_seqno.high, rgy_info.last_upd_seqno.low);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
    }

    /*
     * List of network addresses of the replica.
     */
    if (rtwrs) {
        dcp__rgy_sa_print_towers(interp, DCP_C_RGY_SA_ADDRS, rtwrs);
    } else {
        unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
        if (unknown_svc == NULL) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_ADDRS, unknown_svc);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
        free(unknown_svc);
    }

    /*
     * List of network addresses of the master registry in the cell.
     */
    if (mtwrs) {
        dcp__rgy_sa_print_towers(interp, DCP_C_RGY_SA_MASTER_ADDRS, mtwrs);

        /*
         * The master sequence number, which is the sequence number of 
         * the event that made the replica the master.
         */
        sprintf(output_string, "\n{%s %d.%d}", DCP_C_RGY_SA_MASTER_SEQ,
                rgy_info.master_seqno.high, rgy_info.master_seqno.low);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

        /*
         * The UUID of the master registry of the cell.
         */
        uuid_to_string(&rgy_info.master_id, &name1, &status);
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_MASTER_UUID, (char *)name1);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
    }
    else {
        unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
        if (unknown_svc == NULL) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_MASTER_ADDRS, unknown_svc);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
        free(unknown_svc);
    }

    /*
     * Show the several sets of security server software
     * that this security server supports.
     */
    Tcl_AppendResult(interp, "\n{", DCP_C_RGY_SA_SUPPORTEDVERSIONS, NULL);
    if (number_of_versions > 1) {
        Tcl_AppendResult(interp, "\n", NULL);
    }
    for (i = 0; i < number_of_versions; i++) {
        Tcl_AppendResult(interp, " ", version_info[i].sw_vers_printstring, NULL);
        if (i < (number_of_versions - 1)) {
            Tcl_AppendResult(interp, "\n", NULL);
        }
    }
    if (number_of_versions == 0) {
        unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
        if (unknown_svc == NULL) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        Tcl_AppendResult(interp, " ", unknown_svc, "}", NULL);
        free(unknown_svc);
    }
    else {
        Tcl_AppendResult(interp, "}", NULL);
        free((rs_replica_sw_vers_info_t *)version_info);
    }

    /*
     * The update sequence numbers that are still in the propagation
     * queue and have yet to be propagated.
     */
    if (rgy_info.master) {  
        sprintf(output_string,
                "\n{%s {%d.%d %d.%d}}", 
                DCP_C_RGY_SA_MASTER_QUEUE,
                rgy_info.base_propq_seqno.high,
                rgy_info.base_propq_seqno.low,
                rgy_info.last_upd_seqno.high,
                rgy_info.last_upd_seqno.low);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
    }

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_print_towers()
 *
 * OVERVIEW:
 * This routine displays the network addresses in string 
 * representation of the specified replica.
 *
 * INPUTS:
 * prop_info_item:
 *           The propagation information item to be displayed.
 *
 * twrs: 
 *           The network address(es) that need to be converted
 *           to a string representation of the binding handle.
 *           
 * INPUT/OUTPUTS:
 * interp: Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * None
 */
void dcp__rgy_sa_print_towers(Tcl_Interp *interp,
                             char *prop_info_item,
                             rs_replica_twr_vec_p_t twrs)
{
    int                   i;
    char                  output_string[BUFSIZ];
    rpc_binding_handle_t  binding;
    unsigned_char_p_t     bstr;
    char                  *tcl_bstr;
    char                  *msg_p;
    error_status_t        status;

    CLEAR_STATUS(&status);

    sprintf(output_string, "\n{%s", prop_info_item);
    Tcl_AppendResult(interp, output_string, (char *)NULL);

    for (i = 0; (i < twrs->num_towers); i++) {
        rpc_tower_to_binding(twrs->towers[i]->tower_octet_string,
                             &binding,
                             &status);
        if (GOOD_STATUS(&status)) {
            rpc_binding_to_string_binding(binding, &bstr, &status);
            if (GOOD_STATUS(&status)) {
                tcl_bstr = dcp_binding_string_to_tcl((char *)bstr, &status);
                sprintf(output_string, "\n {%s}", tcl_bstr);
                Tcl_AppendResult(interp, output_string, (char *)NULL);
                rpc_string_free(&bstr, &status);
            }
            rpc_binding_free(&binding, &status);
        }

        if (BAD_STATUS(&status)) {
            msg_p = dcp_binding_to_bytes(twrs->towers[i], " ", &status);
            if (GOOD_STATUS(&status)) {
                Tcl_AppendResult(interp, "\n", (char *)NULL);
                Tcl_AppendResult(interp, msg_p, (char *)NULL);
            }
            else {
                msg_p = (char *)dce_sprintf(dcp_t_rpc_binding_untranslated);
                sprintf(output_string, "\n {%s}", msg_p);
                Tcl_AppendResult(interp, output_string, (char *)NULL);
            }
            free((char *) msg_p);
        }
    }

    Tcl_AppendResult(interp, "}", (char *)NULL);
}


/*
 * FUNCTION:
 * dcp__rgy_sa_state_ok()
 *
 * OVERVIEW:
 * This routine determines if the replica is in a state 
 * where the propagation information will be meaningful.
 *
 * INPUTS:
 * state:
 *           
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TRUE if replica information is meaningful
 * FALSE if replica information is NOT meaningful
 */
boolean32 dcp__rgy_sa_state_ok(unsigned32 state) 
{
    switch ((int)state) {
        case rs_c_state_in_service:
        case rs_c_state_renaming:
        case rs_c_state_copying_dbase:
        case rs_c_state_in_maintenance:
        case rs_c_state_mkey_changing:
        case rs_c_state_becoming_master:
        case rs_c_state_becoming_slave:
            return TRUE;
        default:
            return FALSE;
    }
}


/*
 * FUNCTION:
 * dcp_rgy_sa_do_show_master_info()
 *
 * OVERVIEW:
 * This routine returns the propagation information that the
 * master keeps for itself and each slave. If the argument is
 * present, it must be the master.
 *
 * INPUTS:
 * itemP:
 *         Contains replica information including the replica's UUID
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int dcp_rgy_sa_do_show_master_info(Tcl_Interp *interp)
{
    char               output_string[BUFSIZ];
    char               time_string[UTC_MAX_STR_LEN];
    char               *type_svc;
    char               *unknown_svc;
    unsigned_char_p_t  name1;
    int                i;
    char               *msg_p;
    error_status_t     status;

    /*
     * Initialization.
     */
    CLEAR_STATUS(&status);

    /*
     * Obtain current full replica list.  This call will
     * fail if the master replica was not bound to.
     */
    status = dcp__rgy_sa_obtain_full_replist();
    if (BAD_STATUS(&status)) {
        DCP_SET_MISC_ERROR(status);
    }

    /*
     * Cycle through all replicas in the replica list.
     */
    for (i = 0; ((i < num_freps) && (freplist != NULL)); i++) {
        if (i > 0) {
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
        }

        /*
         * Display fully qualified CDS name of replica.
         */
        sprintf(output_string,
                "{%s %s/%s}",
                DCP_C_RGY_SA_NAME,
                (char *)site_cache[realtime_cache].cell_name,
                (char *)freplist[i].rep_name);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

        /*
         * Display UUID of the replica.
         */
        uuid_to_string(&freplist[i].rep_id, &name1, &status);
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_UUID, (char *)name1);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

        /*
         * Display type if replica is a "master" or "slave".
         */
        if (freplist[i].master) {
            type_svc = (char *)dce_sprintf(dcp_t_rgy_master);
        }
        else {
            type_svc = (char *)dce_sprintf(dcp_t_rgy_slave);
        }
        if (type_svc == NULL) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_TYPE, type_svc);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
        free(type_svc);

        /*
         * List of network addresses of the replica.
         */
        if (freplist[i].rep_twrs) {
            dcp__rgy_sa_print_towers(interp, DCP_C_RGY_SA_ADDRS, freplist[i].rep_twrs);
        } else {
            unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
            if (unknown_svc == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_ADDRS, unknown_svc);
            Tcl_AppendResult(interp, output_string, (char *)NULL);
            free(unknown_svc);
        }

        /*
         * If master replica, then this info is meaningless.
         */
        if (!freplist[i].master) {  
            /*
             * Display state of the replica.
             */
            Tcl_AppendResult(interp, "\n{", DCP_C_RGY_SA_PROP_STATUS, " ", NULL);
            switch (freplist[i].prop_info.prop_type) {
                case rs_c_replica_prop_unknown:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_UNKNOWN, "}", NULL);
                    break;
                case rs_c_replica_prop_init:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_INIT, "}", NULL);
                    break;
                case rs_c_replica_prop_initing:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_INITING, "}", NULL);
                    break;
                case rs_c_replica_prop_update:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_UPDATE, "}", NULL);
                    break;
                case rs_c_replica_prop_delete:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_DELETE, "}", NULL);
                    break;
                case rs_c_replica_prop_bad_sw_rev:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_BAD_SW, "}", NULL);
                    break;
                default:
                    Tcl_AppendResult(interp, DCP_RS_C_REPLICA_PROP_DEFAULT, "}", NULL);
                    break;
            }

            /*
             * Determine if replica propagation info is meaningful
             * The master replica verifies that this replica as OK.
             */
            if ((freplist[i].prop_info.prop_type == rs_c_replica_prop_update) &&
                (freplist[i].prop_info.last_upd_inited)) {

                /*
                 * The localized date/time the last update was sent to 
                 * the replica.
                 */
                sec_to_date_string(freplist[i].prop_info.last_upd_ts.sec, time_string);
                sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_LAST_UPD_TIME, time_string);
                Tcl_AppendResult(interp, output_string, (char *)NULL);

                /*
                 * The sequence number of last update the replica received.
                 */
                sprintf(output_string,
                        "\n{%s %d.%d}", 
                        DCP_C_RGY_SA_LAST_UPD_SEQ_SENT,
                        freplist[i].prop_info.last_upd_seqno.high,
                        freplist[i].prop_info.last_upd_seqno.low);
                Tcl_AppendResult(interp, output_string, (char *)NULL);

                /*
                 * The number of outstanding updates.
                 */
                sprintf(output_string,
                        "\n{%s %d}", 
                        DCP_C_RGY_SA_NUM_UPD_TO_GO,
                        freplist[i].prop_info.num_updates);
                Tcl_AppendResult(interp, output_string, (char *)NULL);

                /*
                 * The communications state.
                 */
                Tcl_AppendResult(interp, "\n{", DCP_C_RGY_SA_COMM_STATE, " ", NULL);
                switch (freplist[i].comm_info.comm_state) {
                    case rs_c_replica_comm_short_failure:
                        Tcl_AppendResult(interp, DCP_RS_C_REPLICA_COMM_SHORT_FAIL, "}", NULL);
                        break;
                    case rs_c_replica_comm_long_failure:
                        Tcl_AppendResult(interp, DCP_RS_C_REPLICA_COMM_LONG_FAIL, "}", NULL);
                        break;
                    case rs_c_replica_comm_ok:
                    default:
                        Tcl_AppendResult(interp, DCP_RS_C_REPLICA_COMM_OK, "}", NULL);
                        break;
                }

                /*
                 * The status of the last communication with the replica.
                 */
                if (GOOD_STATUS(&freplist[i].comm_info.last_status)) {
                    msg_p = (char *)dce_sprintf(dcp_s_ok);
                }
                else {
                    msg_p = (char *)dce_sprintf(freplist[i].comm_info.last_status);
                }
                sprintf(output_string, "\n{%s {%s}}", DCP_C_RGY_SA_LAST_COMM_STAT, msg_p);
                free(msg_p);
                Tcl_AppendResult(interp, output_string, (char *)NULL);
            }
        }
    }

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_do_show_replist_info()
 *
 * OVERVIEW:
 * This routine returns the propagation information that the
 * specified replica keeps for itself and the other replicas.
 * The argument can be a master or a slave replica.
 *
 * INPUTS:
 * itemP:
 *         Contains replica information including the replica's UUID
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int dcp_rgy_sa_do_show_replist_info(Tcl_Interp *interp)
{
    char               output_string[BUFSIZ];
    char               *type_svc;
    char               *unknown_svc;
    unsigned_char_p_t  name1;
    int                i;
    error_status_t     status;

    /*
     * Initialization.
     */
    CLEAR_STATUS(&status);

    /*
     * Cycle through all replicas in the replica list.
     */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        if (i > 0) {
            Tcl_AppendResult(interp, "\n\n", (char *)NULL);
        }

        /*
         * Display fully qualified CDS name of replica.
         */
        sprintf(output_string,
                "{%s %s/%s}",
                DCP_C_RGY_SA_NAME,
                (char *)site_cache[realtime_cache].cell_name,
                (char *)replist[i].rep_name);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

        /*
         * Display UUID of the replica.
         */
        uuid_to_string(&replist[i].rep_id, &name1, &status);
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_UUID, (char *)name1);
        Tcl_AppendResult(interp, output_string, (char *)NULL);

        /*
         * Display type if replica is a "master" or "slave".
         */
        if (replist[i].master) {
            type_svc = (char *)dce_sprintf(dcp_t_rgy_master);
        }
        else {
            type_svc = (char *)dce_sprintf(dcp_t_rgy_slave);
        }
        if (type_svc == NULL) {
            DCP_SET_MISC_ERROR(dcp_s_no_memory);
        }
        sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_TYPE, type_svc);
        Tcl_AppendResult(interp, output_string, (char *)NULL);
        free(type_svc);

        /*
         * List of network addresses of the replica.
         */
        if (replist[i].rep_twrs) {
            dcp__rgy_sa_print_towers(interp, DCP_C_RGY_SA_ADDRS, replist[i].rep_twrs);
        } else {
            unknown_svc = (char *)dce_sprintf(dcp_t_rgy_unknown);
            if (unknown_svc == NULL) {
                DCP_SET_MISC_ERROR(dcp_s_no_memory);
            }
            sprintf(output_string, "\n{%s %s}", DCP_C_RGY_SA_ADDRS, unknown_svc);
            Tcl_AppendResult(interp, output_string, (char *)NULL);
            free(unknown_svc);
        }
    }

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_do_modify_key()
 *
 * OVERVIEW:
 * This routine will generate a new master key for the specified 
 * replica. If the specified replica is not in the master replica
 * list, then an error will be displayed.
 *
 * INPUTS:
 * name:
 *           Pointer to string with list structure.
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * status: A pointer to the completion status. On successful completion,
 *         the routine returns error_status_ok. Otherwise, it returns
 *         an error.
 */
int dcp_rgy_sa_do_modify_key(Tcl_Interp *interp, char *name)
{
    uuid_t          rep_id;
    int             rc = TCL_OK;
    error_status_t  status;

    CLEAR_STATUS(&status);

    rc = dcp_rgy_sa_bind_named_object(interp, (char *)name, site_specific);
    if (rc == TCL_OK) {
        /*
         * Determine if specified replica is in master replica list.
         */
        status = dcp__rgy_sa_get_rep_id_from_name((unsigned_char_p_t)name,
                                                  FALSE,
                                                  &rep_id);
        if (status == sec_rgy_server_unavailable) {
            DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_server_unavailable,name);
            return TCL_ERROR;
        }
        
        if (GOOD_STATUS(&status)) {
            sec_rgy_rep_admin_mkey(rgy_context, &status);
        }
    }

    DCP_CHECK_SEC_ERROR(status);
    return rc;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_get_rep_id_from_name()
 *
 * OVERVIEW:
 * This routine returns a replica info given a specified replica name
 * either as a string binding, local/global name service name, or
 * local/global cell name.
 *
 * INPUTS:
 * rep_name:     
 *          Name of replica as specified by the user.
 *               /.:
 *               /.../<cell_name>
 *               /.../<cell_name>/<replica_name>
 *               /.:/<replica_name>
 *               <replica_name>
 *               <string binding of replica in string syntax>
 *               <string binding of replica in TCL syntax>
 *
 * master_only:
 *          Replica search indicator. If TRUE, determine if specified
 *          replica is the master replica. If FALSE, determine if
 *          specified replica is a member of the master replica list.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * rep_id_p:
 *         Contains the replica's UUID
 *
 * RETURNS:
 * status: A pointer to the completion status. On successful completion,
 *         the routine returns error_status_ok. Otherwise, it returns
 *         an error.
 */
error_status_t dcp__rgy_sa_get_rep_id_from_name(unsigned_char_p_t rep_name,
                                                boolean32 master_only,
                                                uuid_p_t rep_id_p)
{
    boolean32                 string_binding_name = FALSE;
    rs_replica_info_t         rgy_info;
    rs_replica_item_t         *item_p;
    sec_rgy_handle_t          rep_name_context;
    sec_rgy_bind_auth_info_t  auth_info;
    error_status_t 	      status;
    error_status_t 	      tmp_status;
    sec_rgy_name_t            cell_name;
    sec_rgy_name_t            residual_name;

    CLEAR_STATUS(&status);

    /*
     * Determine if specified replica name is prefixed with local or
     * global cell name, or if the specified replica is a string binding.
     */
    if (strstr((char *)rep_name, "/.:/")) {
        rep_name += (strlen("/.:/"));
    }
    else if (*rep_name == '/') {
        /*
         * Check for presence of full cell name or just local cell.
         */
        if (strlen((char *)site_cache[realtime_cache].cell_name) < strlen((char *)rep_name)) {
            /*
             * Looks like the full cell name has been specified.
             * Remove the full cell name, leaving only the replica info.
             */
            sec_id_parse_name(rgy_default_context,
                              (char *)rep_name,
                              cell_name,
                              NULL,
                              residual_name,
                              NULL,
                              &status);
            if (BAD_STATUS(&status))
                return status;
            rep_name = residual_name;
        } else {
            /*
             * Looks like the local cell name.  If checking for the
             * master, there is no need to check the replica list.
             */
            if (master_only) {
                CLEAR_STATUS(&status);
                return status;
            }
        } 
    }
    else if (strchr((char *)rep_name, ':')) {
        string_binding_name = TRUE;

        /*
         * rep_name may not be the same as the "rgy_context"
         * which was bound to to perform the operation.
         */
        auth_info.info_type = sec_rgy_bind_auth_none;
        auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
        auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
        auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
        auth_info.tagged_union.dce_info.identity  = NULL;

        sec_rgy_site_bind_query(rep_name, &auth_info, &rep_name_context, &status);
        if (GOOD_STATUS(&status)) {
            sec_rgy_rep_admin_info(rep_name_context, &rgy_info, &status);
        } 
        if (BAD_STATUS(&status))
            return status;
    }

    /*
     * Determine if specified replica found in replica list or
     * if specified replica is the master if 'master_only'.
     */
    if (master_only) {
        if (string_binding_name) {
            if (!dcp__rgy_sa_find_master_by_uuid(&rgy_info.rep_id)) {
                SET_STATUS(&status, dcp_s_rgy_sa_replica_not_master); 
            } 
        }
        else {
            if (! dcp__rgy_sa_find_master_by_name(rep_name)) {
                SET_STATUS(&status, dcp_s_rgy_sa_replica_not_master); 
            }  
        }
    }
    else {
        if (string_binding_name) {
            if (dcp__rgy_sa_find_rep_item_by_uuid(&rgy_info.rep_id, &item_p)) {
                *rep_id_p = item_p->rep_id;
            }
            else {
                SET_STATUS(&status, sec_rgy_server_unavailable);
            }
            sec_rgy_site_close(rep_name_context, &tmp_status);
        }
        else if (dcp__rgy_sa_find_rep_item_by_name(rep_name, &item_p)) {
            *rep_id_p = item_p->rep_id;
        }
        else {
            SET_STATUS(&status, sec_rgy_server_unavailable);
        }
    }

    return status;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_find_rep_item_by_name()
 *
 * OVERVIEW:
 * This routine returns replica info if found in the cell's
 * replica list.  Returns "FALSE" if the specified replica is not
 * found in the replica list.
 *
 * INPUTS:
 * rep_name:     
 *          Name service name of replica (not global)
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * itemP:
 *         Contains replica information including the replica's UUID
 *
 * RETURNS:
 * TRUE:  indicates that the replica was found in the replica list
 * FALSE: indicates that the replica was NOT found in the replica list
 */
boolean32 dcp__rgy_sa_find_rep_item_by_name(unsigned_char_p_t rep_name, 
                                            rs_replica_item_t **itemP)
{
    int  i;

    /*
     * Determine if specified replica is found in the replica list.
     */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        if (strcmp((char *)rep_name, (char *)replist[i].rep_name) == 0) {
            *itemP = &replist[i];
            return TRUE;
        } 
    }

    return FALSE;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_find_master_by_name()
 *
 * OVERVIEW:
 * This routine, given a replica name, determines if the specified 
 * replica is the master replica.
 *
 * INPUTS:
 * rep_name:     
 *          Name service name of replica (not global)
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TRUE:  indicates that the replica is a master replica
 * FALSE: indicates that the replica is NOT a master replica
 */
boolean32 dcp__rgy_sa_find_master_by_name(unsigned_char_p_t rep_name)
{
    int  i;

    /*
     * Determine if specified replica is the master replica.
     */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        if (strcmp((char *)rep_name, (char *)replist[i].rep_name) == 0) {
            if (replist[i].master)
                return TRUE;
        }
    }

    return FALSE;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_find_master_by_uuid()
 *
 * OVERVIEW:
 * This routine, given a replica UUID, determines if the specified 
 * replica is the master replica.
 *
 * INPUTS:
 * rep_id:
 *          Replica UUID to be compared to UUID's in the replica list
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TRUE:  indicates that the replica is the master replica.
 * FALSE: indicates that the replica is NOT the master replica.
 */
boolean32 dcp__rgy_sa_find_master_by_uuid(uuid_p_t rep_id)
{
    int             i;
    error_status_t  status;

    CLEAR_STATUS(&status);

    /*
     * Determine if specified replica is the master replica.
     */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        if (uuid_equal(rep_id, &replist[i].rep_id, &status)) {
            if (replist[i].master)
                return TRUE;
        }
    }

    return FALSE;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_find_rep_item_by_uuid()
 *
 * OVERVIEW:
 * This routine returns the replica's UUID if found in the cell's
 * replica list.  Returns "FALSE" if the specified replica is not
 * found in the replica list.
 *
 * INPUTS:
 * rep_id:
 *          Replica UUID to be compared to UUID's in the replica list
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * itemP:
 *         Contains replica information including the replica's UUID
 *
 * RETURNS:
 * TRUE:  indicates that the replica was found in the replica list
 * FALSE: indicates that the replica was NOT found in the replica list
 */
boolean32 dcp__rgy_sa_find_rep_item_by_uuid(uuid_p_t rep_id, 
                                            rs_replica_item_t **itemP)
{
    int             i;
    error_status_t  status;

    CLEAR_STATUS(&status);

    /*
     * Determine if specified replica is found in the replica list.
     */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        if (uuid_equal(rep_id, &replist[i].rep_id, &status)) {
            *itemP = &replist[i];
            return TRUE;
	} 
    }

    return FALSE;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_check_last_upd_seqno()
 *
 * OVERVIEW:
 * This routine checks if there is a replica in the replica list that
 * has a more recent last update sequence number than the specified
 * replica.
 * 
 * INPUTS:
 * None
 *
 * INPUT/OUTPUTS:
 * interp: Structure associated with the interpreter. This will hold
 *         the error message (if any) generated from the command. This
 *         is also the location of any convenience variables.
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 * TCL_OK    if no error occurs.
 * TCL_ERROR if error occurs.
 */
int 
dcp_rgy_sa_check_last_upd_seqno(Tcl_Interp *interp)
{
    int                       i;
    rs_replica_info_t         specified_rgy_info;
    rs_replica_info_t         replist_rgy_info;
    sec_rgy_handle_t          rgyh;
    sec_rgy_bind_auth_info_t  auth_info;
    error_status_t            status;

    CLEAR_STATUS(&status);
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    /*
     * Obtain last_upd_seqno for the specified replica.
     */
    sec_rgy_rep_admin_info(rgy_context, &specified_rgy_info, &status);
    DCP_CHECK_SEC_ERROR(status);

    /*
     * For each replica in the replica list, determine if it's 
     * last_upd_seqno is greater than the one specified.
     */
    for (i = 0; ((i < num_reps) && (replist != NULL)); i++) {
        if (! replist[i].deleted) {
            /*
             * Obtain handle for this registry server in 
             * order to obtain its last_upd_seqno.
             */
            sec_rgy_site_bind_from_towerv(
                (rpc_tower_vector_p_t)replist[i].rep_twrs,
                (unsigned_char_p_t)site_cache[realtime_cache].cell_name, 
                replist[i].rep_name, 
                &auth_info,
                &rgyh, 
                &status);
            if (GOOD_STATUS(&status)) {
                sec_rgy_rep_admin_info(rgyh, &replist_rgy_info, &status);
                if (GOOD_STATUS(&status)) {
                    if (dcp__rgy_sa_seqno_cmp(&replist_rgy_info.last_upd_seqno,
                                              &specified_rgy_info.last_upd_seqno)) {
                        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_rgy_sa_seqno_high,
                                                    (char *)replist[i].rep_name);
                        sec_rgy_site_close(rgyh, &status);
                        return TCL_ERROR;
		    }
		}
                sec_rgy_site_close(rgyh, &status);
	    }
	}
    }

    return TCL_OK;
}


/*
 * FUNCTION:
 * dcp__rgy_sa_seqno_cmp()
 *
 * OVERVIEW:
 * This routine compares 2 sequence numbers. Every event at the master 
 * is assigned a monotonically increasing sequence number. Most
 * of these events are propagated as updates to the slaves. But
 * sometimes a slave may have gaps in the seqno's of its updates.
 *
 * INPUTS:
 * seqno1: First sequence number as defined by rs_update_seqno_t.
 *
 * seqno2: Second sequence number as defined by rs_update_seqno_t.
 *
 * INPUT/OUTPUTS:
 * None
 *
 * OUTPUTS:
 * None
 *
 * RETURNS:
 *  Return 0 if the sequence numbers are equal.
 *  Return 1 if seqno1 > seqno2
 *  Return -1 if seqno1 < seqno2 
 */
signed32 dcp__rgy_sa_seqno_cmp(rs_update_seqno_t *seqno1, rs_update_seqno_t *seqno2)
{
    signed32  retval;

    if (seqno1->high == seqno2->high) {
        if (seqno1->low == seqno2->low) {
            retval = 0;
        }
        else {
            retval = (seqno1->low > seqno2->low) ? 1:-1;
        }
    }
    else {
        retval = (seqno1->high > seqno2->high) ? 1:-1;
    }

    return retval;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_list_replicas()
 *
 * OVERVIEW:
 * This routine will ensure that list of replicas are split up
 * properly.
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
int dcp_rgy_sa_list_replicas(Tcl_Interp *interp,
                             char *name_tcllist,
                             int *name_argc,
                             char ***name_argv)
{
    int             rc = TCL_OK;
    error_status_t  status;

    CLEAR_STATUS(&status);

    /*
     * If no site name was passed in, allocate argv for one.
     */
    if (name_tcllist == NULL) {
        rc = dcp_general_allocate_argv_for_one(interp, (char *)"", name_argc, name_argv);
        return rc;
    }

    /*
     * Determine if there is 1 string binding in TCL syntax.  
     */
    rc = dcp_general_split_list(interp, name_tcllist, name_argc, name_argv);
    return rc;
}


/*
 * FUNCTION:
 * dcp_rgy_sa_bind_named_object()
 *
 * OVERVIEW:
 * This routine binds to the proper security replica and if all goes
 * well, sets the SITE_VAR convenience variable. If a specific
 * security replica is required, then bind to the site that was 
 * passed in. If not, then obtain the current value of the SITE_VAR
 * convenience variable.
 *
 * INPUTS:
 * access_type:
 *           Determines if we can communicate with the registry
 *           server unauthenticated or not. Valid values:
 *               site_update
 *               site_specific
 *               site_query
 *               site_master_query
 *
 * INPUT/OUTPUTS:
 * interp    Structure associated with the interpreter. This will hold
 *           the error message (if any) generated from the command. This
 *           is also the location of any convenience variables.
 *
 * named_object:
 *           The name of the target object. 
 *
 * OUTPUTS:
 * site:
 *           Cache to hold the current site name.
 *
 * _b(sec): 
 *           Convenience variable for the name of the server bound to.
 *
 * RETURNS:
 * TCL_OK
 * TCL_ERROR
 */
int dcp_rgy_sa_bind_named_object(Tcl_Interp *interp,
                                 char *named_object,
                                 site_access_t access_type)
{
    error_status_t  status;
    sec_rgy_name_t  lsite;

    CLEAR_STATUS(&status);
    strcpy((char *)lsite, named_object);

    /*
     * Verify that the site is open with the appropriate access capabilities. 
     * If successful, set the _b(sec) convenience variable.  
     * _b(sec) holds the name of the server bound to for the last
     * command.  This variable is only set by dcecp, not the user.
     */
    if (open_rgy_site(interp, lsite, access_type) != TCL_OK)
        return TCL_ERROR;

    /*
     * Obtain current replica list now that we have a registry context.
     */
    status = dcp__rgy_sa_obtain_replist();
    if (BAD_STATUS(&status)) {
        DCP_SET_MISC_ERROR(status);
    }

    return TCL_OK;
}
