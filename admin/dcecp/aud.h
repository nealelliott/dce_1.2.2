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
 * $Log: aud.h,v $
 * Revision 1.1.6.2  1996/02/18  23:31:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:33  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:19:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/14  15:41 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts13/1  1995/02/14  15:38 UTC  truitt
 * 	CHFts14285: Have the audit code call the correct
 * 	macros when setting an error condition.
 * 	[1995/12/08  17:54:05  root]
 * 
 * Revision 1.1.2.15  1994/10/05  17:57:54  rousseau
 * 	Fix multi-guide logic (CR 12502).
 * 	[1994/10/05  17:57:29  rousseau]
 * 
 * Revision 1.1.2.14  1994/09/23  20:40:05  rousseau
 * 	Secure binding changes (CR 12214).
 * 	[1994/09/23  20:36:45  rousseau]
 * 
 * Revision 1.1.2.13  1994/09/01  14:16:42  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:15:59  rousseau]
 * 
 * Revision 1.1.2.12  1994/08/19  22:15:29  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:14:39  rousseau]
 * 
 * Revision 1.1.2.11  1994/08/17  19:16:23  rousseau
 * 	Added new condition pending (CR 11679).
 * 	[1994/08/17  19:15:47  rousseau]
 * 
 * Revision 1.1.2.10  1994/07/28  20:39:09  rousseau
 * 	64 bit changes (CR 11451).
 * 	Fix reading of audit daemon bindings (CR 11466).
 * 	[1994/07/28  20:36:42  rousseau]
 * 
 * Revision 1.1.2.9  1994/07/19  18:06:24  rousseau
 * 	Implemented reomte control and fixed ~ in paths (CR 11178 11179 10221).
 * 	[1994/07/19  18:05:35  rousseau]
 * 
 * Revision 1.1.2.8  1994/06/09  16:01:32  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:46:53  devsrc]
 * 
 * Revision 1.1.2.7  1994/06/01  17:26:15  rousseau
 * 	Added aud_rewind.
 * 	[1994/06/01  17:25:19  rousseau]
 * 
 * Revision 1.1.2.6  1994/05/18  21:17:54  rousseau
 * 	Initialize guides to NULL before every RPC in audfilter create and modify (CR 10373).
 * 	[1994/05/18  21:17:32  rousseau]
 * 
 * Revision 1.1.2.5  1994/05/09  19:25:59  rousseau
 * 	Fixes for:
 * 	10394   Need better error messages for list problems.
 * 	10393   core dump in dcp_aud_check_multi_guides()
 * 	10391   audf create doesn't catch invalid condition.
 * 	10383   audfilter create dies w/o -attribute option
 * 	10293   Memory allocated by dce_sprinf is not released
 * 	9869    the dcecp audit component is not complete
 * 	[1994/05/09  19:22:07  rousseau]
 * 
 * Revision 1.1.2.4  1994/04/18  16:58:37  rousseau
 * 	Second drop of audit functionality (CR 9691).
 * 	[1994/04/18  16:55:13  rousseau]
 * 
 * Revision 1.1.2.3  1994/03/31  18:39:45  rousseau
 * 	Changes to all audfilter commands (CR 9869).
 * 	[1994/03/31  18:39:27  rousseau]
 * 
 * Revision 1.1.2.2  1994/02/02  20:00:34  rousseau
 * 	Initial implementation of audfilter, audtrail and audevents. Includes help functions.
 * 	[1994/02/02  19:57:01  rousseau]
 * 
 * Revision 1.1.2.1  1994/01/06  16:50:58  rousseau
 * 	Initial version.
 * 	[1994/01/06  16:48:41  rousseau]
 * 
 * $EndLog$
 */
 
#ifndef _AUD_H
#define _AUD_H

/*
 * MODULE: aud.h
 *
 * DESCRIPTION:
 *   This module contains prototypes and definitions used in the aud
 * component of dcecp.
 *
 */

/* INCLUDE FILES  */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/audit_control.h>
#include <dce/cma.h>
#include <util_help.h>
#include <util_login.h>
#include <dcecp.h>

/* DECLARATIONS */

#define HASH_TABLE_SIZE         100
#define AUD_MAX_ARGUMENTS       128
#define AUD_MAX_STRING_LENGTH   1024
#define AUD_MAX_EVENT_CLASSES   1024
#define AUD_MAX_CONDITIONS      5
#define AUD_MAX_ACTIONS         4
#define AUD_SERVER_INTERFACE    audit_control_v1_0_c_ifspec
#define BINDING_HANDLE_STR_LEN 	128


#define DCP_IS_COMM_ERROR(x) ((x == rpc_s_comm_failure)                 \
                              || (x == rpc_s_connect_closed_by_rem)     \
                              || (x == rpc_s_connect_no_resources)      \
                              || (x == rpc_s_connect_rejected)          \
                              || (x == rpc_s_connect_timed_out)         \
                              || (x == rpc_s_connection_closed))

#define DCP_CHECK_AUD_ERROR(code) {                                     \
  if (code != aud_s_ok) {                                               \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    if (DCP_IS_COMM_ERROR(code))                                        \
        dcp_aud_valid_binding = FALSE;                                  \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_AUD_ERROR_FREE_ONE(code, arg1) {                      \
  if (code != aud_s_ok) {                                               \
    free((char *)arg1);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    if (DCP_IS_COMM_ERROR(code)) {                                      \
        dcp_aud_valid_binding = FALSE;                                  \
    }                                                                   \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_AUD_ERROR_FREE_TWO(code, arg1, arg2) {                \
  if (code != aud_s_ok) {                                               \
    free((char *)arg1);                                                 \
    free((char *)arg2);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    if (DCP_IS_COMM_ERROR(code)) {                                      \
        dcp_aud_valid_binding = FALSE;                                  \
    }                                                                   \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_CHECK_AUD_ERROR_FREE_FOUR(code, arg1, arg2, arg3, arg4) {   \
  if (code != aud_s_ok) {                                               \
    free((char *)arg1);                                                 \
    free((char *)arg2);                                                 \
    free((char *)arg3);                                                 \
    free((char *)arg4);                                                 \
    Tcl_SetResult(interp, (char *)dce_sprintf(code), TCL_DYNAMIC);      \
    DCP_GENERAL_SET_E_VAR(code);                                        \
    if (DCP_IS_COMM_ERROR(code)) {                                      \
        dcp_aud_valid_binding = FALSE;                                  \
    }                                                                   \
    return TCL_ERROR;                                                   \
  }                                                                     \
}

#define DCP_SET_AUD_ERROR_WITH_MERGE(code, arg1, arg2) {                                  \
    Tcl_SetResult(interp, (char *)dce_sprintf(code, Tcl_Merge(arg1, arg2)), TCL_DYNAMIC); \
    DCP_GENERAL_SET_E_VAR(code);                                                          \
    free((char *)arg2);                                                                   \
}

/* INTERNAL VARIABLES */

typedef struct aud_index_entry_s_t {
    unsigned char   *name;
    unsigned32      number;
} aud_index_entry_t;

extern rpc_binding_handle_t             dcp_aud_binding_h;
extern boolean32                        dcp_aud_valid_binding;


/* Audit attributes */
#define NUM_AUD_ATTRIBUTES      2
#define aud_stostrategy         0
#define aud_state               1


/* PROTOTYPES */

int aud_disable         (ClientData, Tcl_Interp *, int, char **);
int aud_enable          (ClientData, Tcl_Interp *, int, char **);
int aud_modify          (ClientData, Tcl_Interp *, int, char **);
int aud_rewind          (ClientData, Tcl_Interp *, int, char **);
int aud_show            (ClientData, Tcl_Interp *, int, char **);
int aud_stop            (ClientData, Tcl_Interp *, int, char **);

int audfilter_catalog   (ClientData, Tcl_Interp *, int, char **);
int audfilter_create    (ClientData, Tcl_Interp *, int, char **);
int audfilter_delete    (ClientData, Tcl_Interp *, int, char **);
int audfilter_modify    (ClientData, Tcl_Interp *, int, char **);
int audfilter_show      (ClientData, Tcl_Interp *, int, char **);

int audevents_catalog   (ClientData, Tcl_Interp *, int, char **);
int audevents_show      (ClientData, Tcl_Interp *, int, char **);

int audtrail_show       (ClientData, Tcl_Interp *, int, char **);


int aud_bind_to_server  (Tcl_Interp *, char *);
int aud_list_entries    (Tcl_Interp *, aud_esl_type_t, void *);
int aud_get_subject_name(Tcl_Interp *, aud_esl_type_t, uuid_t, 
                         uuid_t, sec_rgy_name_t);
int aud_list_guides     (Tcl_Interp *, aud_esl_guides_p_t);
int dcp_aud_get_ec_list (aud_index_entry_t [], int *);
int dcp_aud_check_multi_guides(Tcl_Interp *, char *);

extern void dce_aud_init_svc(void);
extern void input_ec_table(void);

#endif 
