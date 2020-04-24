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
 * $Log: dcp_dts.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:20:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:55:31  root]
 * 
 * Revision 1.1.2.6  1994/09/23  23:18:47  rousseau
 * 	Internal rebinding changes (CR 12282).
 * 	[1994/09/23  23:18:23  rousseau]
 * 
 * Revision 1.1.2.5  1994/09/01  14:16:51  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:11  rousseau]
 * 
 * Revision 1.1.2.4  1994/08/19  22:15:45  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:14:58  rousseau]
 * 
 * Revision 1.1.2.3  1994/06/26  14:14:45  kevins
 * 	OT 11083 modified prototype for dts_show_all_counters
 * 	[1994/06/26  14:10:53  kevins]
 * 
 * 	Updated to new control interface. Enabled talking to remote daemons (CR 10434).
 * 	[1994/05/09  20:41:18  rousseau]
 * 
 * 	Changes so we can build with new dts ctrl interface (CR 10434).
 * 	[1994/04/22  21:14:37  rousseau]
 * 
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:17:58  kevins]
 * 
 * 	CR 10304 Modifications to help remove cross component dependency build issues
 * 	[1994/04/07  10:10:40  kevins]
 * 
 * 	Changed to support help functions.
 * 	[1994/02/02  19:40:14  rousseau]
 * 
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:58:04  rousseau]
 * 
 * 	Changed DTS_NUM_RW_OPTS from 13 to 12.
 * 	[1993/12/27  19:14:02  rousseau]
 * 
 * 	Style guide changes.
 * 	[1993/12/21  19:27:43  rousseau]
 * 
 * Revision 1.1.2.2  1994/06/09  16:01:59  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:28  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/16  20:47:42  rousseau
 * 	Fix build problem (CR 10642).
 * 	[1994/05/16  20:39:08  rousseau]
 * 
 * Revision 1.1.2.5  1993/12/01  16:57:57  rousseau
 * 	Cleaned up attribute lists and added attributes structure.
 * 	[1993/12/01  16:57:01  rousseau]
 * 
 * Revision 1.1.2.4  1993/11/19  22:45:42  rousseau
 * 	Added prototypes for show internal functions.
 * 	[1993/11/19  22:45:38  rousseau]
 * 
 * Revision 1.1.2.3  1993/11/11  02:35:31  rousseau
 * 	Removed external global_context_list declaration.
 * 	[1993/11/11  02:35:24  rousseau]
 * 
 * Revision 1.1.2.2  1993/11/09  18:19:21  rousseau
 * 	Changes for dts show commands.
 * 	[1993/11/09  18:19:16  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/05  15:34:37  rousseau
 * 	Initial version.
 * 	[1993/11/05  15:31:37  rousseau]
 * 
 * $EndLog$
 */

#ifndef _DTS_H
#define _DTS_H

/*
 * MODULE: dcp_dts.h
 *
 * DESCRIPTION:
 *   This module contains prototypes and definitions used in the
 * DTS component of dcecp.
 *
 */

/* INCLUDE FILES  */

#include <stdlib.h>
#include <dce/dce.h>
#include <dce/sec_login.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dtss_includes.h>
#include <dce/tcl.h>
#include <dcecp.h>
#include <util_help.h>
#include <util_login.h>

/* DECLARATIONS */

#define DTS_NUM_RW_ATTR         11
#define LOCAL_HOST              "127.0.0.1"  /* inet address of local host */

#define DTS_DEF_STR_BINDING     "ncadg_ip_udp:127.0.0.1"
#define DTS_SERVER_INTERFACE    time_control_v2_0_c_ifspec
#define DCP_DTS_MAX_RETRIES     1

#define UTCtoBinAbs(binTimePtr, UTCptr)                                 \
    *(binTimePtr) = *(UTCptr)

/* INTERNAL VARIABLES */
/* Structure for list of modifiable attributes. Used only in dts_modify. */
typedef struct dcp_dts_attributes_s_t {
    char                        *attribute;
    int                         value;
    int                         type;
} dcp_dts_attributes_t;

extern boolean32                dcp_dts_valid_binding;
extern rpc_binding_handle_t     dcp_dts_binding_h;


/* PROTOTYPES */
int dcp_dts_bind_to_server      (Tcl_Interp *, 
                                 char *);
int dts_check_status            (Tcl_Interp *, 
                                 StatusBlk_t, 
                                 error_status_t);
int dts_print_attr_text         (Tcl_Interp *, 
                                 int );
int dts_print_attr_value        (Tcl_Interp *, 
                                 int, 
                                 int,
                                 VOID *);
int dts_show_all_attr           (Tcl_Interp *, 
                                 CtlShowAllChar *);
int dts_show_all_counters       (Tcl_Interp *, 
                                 CtlShowAllCntrs *);
int dts_init_counters           (char []);
int dts_show_status             (Tcl_Interp *);
int dts_show_uuid               (Tcl_Interp *);
int dts_show_local_servers      (Tcl_Interp *);
int dts_show_global_servers     (Tcl_Interp *);
int dts_get_attr_type           (int, 
                                 SetShow_t *);
int dts_show_servers            (Tcl_Interp *,
                                 long, 
                                 ServerID []);


#endif
