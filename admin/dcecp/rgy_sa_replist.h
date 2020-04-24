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
 * $Log: rgy_sa_replist.h,v $
 * Revision 1.1.7.1  1996/08/09  11:50:08  arvind
 * 	Merge changes.
 * 	[1996/06/12  13:35 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/15  18:45 UTC  truitt  /main/HPDCE02/2]
 *
 * 	Merge replist changes.
 * 	[1996/02/15  18:44 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf62/1]
 *
 * 	CHFts16871 (OT13316): Have the registry show operation support
 * 	return of slave replica list information.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  13:17 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.5.2  1996/02/18  23:31:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:53  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  21:22:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/11  14:16 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chfts46/1  1995/09/11  14:14 UTC  truitt
 * 	CHFts16100: Comprehensive clean-up of registry commands.
 * 	[1995/12/08  17:56:55  root]
 * 
 * Revision 1.1.2.9  1994/10/05  21:59:27  salamone
 * 	CR12447 - set _b(sec) to fullname
 * 	[1994/10/05  21:21:47  salamone]
 * 
 * Revision 1.1.2.8  1994/08/24  15:04:41  salamone
 * 	CR11838 - Add support for "registry set" command.
 * 	[1994/08/23  21:25:24  salamone]
 * 
 * Revision 1.1.2.7  1994/08/19  18:04:15  salamone
 * 	CR10683 - Use generic routines to handle 1 sbind in TCL.
 * 	[1994/08/19  17:53:47  salamone]
 * 
 * Revision 1.1.2.6  1994/06/24  20:02:01  salamone
 * 	CR11056 - Slave replica specification not accepted.
 * 	[1994/06/24  19:24:06  salamone]
 * 
 * Revision 1.1.2.5  1994/06/20  21:02:59  salamone
 * 	Merged with changes from 1.1.2.4
 * 	[1994/06/20  21:02:39  salamone]
 * 
 * 	Added "registry show" and "registry dump" support.
 * 	[1994/06/20  19:43:22  salamone]
 * 
 * Revision 1.1.2.4  1994/06/09  16:02:21  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:47:55  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/06  16:50:53  salamone
 * 	Enhanced registry modify support.
 * 	[1994/06/06  16:49:17  salamone]
 * 
 * Revision 1.1.2.2  1994/05/24  19:31:26  salamone
 * 	Added registry synchronize and delete support.
 * 	[1994/05/24  19:30:10  salamone]
 * 
 * Revision 1.1.2.1  1994/05/12  18:29:43  salamone
 * 	Initial version of rgy_sa_replist.h
 * 	[1994/05/12  18:27:56  salamone]
 * 
 * $EndLog$
 */

/*
 * MODULE:
 * rgy_sa_replist.h
 *
 * DESCRIPTION:
 * External declarations for "registry" replication information.
 *
 */

#ifndef _RGY_SA_REPLIST_H
#define _RGY_SA_REPLIST_H

/*
 * DECLARATIONS 
 */
#include <dce/replist.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <dce/repadm.h>

#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/binding.h>
#include <rgy.h>
#include <rgy_sa_tclif.h>

/*
 * INTERNAL VARIABLES
 */
extern sec_rgy_handle_t rgy_context;

/*
 * PROTOTYPES 
 */
void dcp_rgy_sa_do_catalog(Tcl_Interp *, boolean32);
int dcp_rgy_sa_do_modify_key(Tcl_Interp *, char *);
int dcp_rgy_sa_do_show_master_info(Tcl_Interp *);
int dcp_rgy_sa_do_show_replist_info(Tcl_Interp *);
int dcp_rgy_sa_do_show_replica_info(Tcl_Interp *);
int dcp_rgy_sa_check_last_upd_seqno(Tcl_Interp *);
int dcp_rgy_sa_list_replicas(Tcl_Interp *, char *, int *, char ***);
int dcp_rgy_sa_bind_named_object(Tcl_Interp *, char *, site_access_t);
error_status_t dcp__rgy_sa_obtain_replist(void);
error_status_t dcp__rgy_sa_get_rep_id_from_name(unsigned_char_p_t, boolean32, uuid_p_t);
boolean32 dcp__rgy_sa_find_master_by_uuid(uuid_p_t);
boolean32 dcp__rgy_sa_find_rep_item_by_uuid(uuid_p_t, rs_replica_item_t **);

#endif   /* _RGY_SA_REPLIST_H */
