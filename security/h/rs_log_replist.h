/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_replist.h,v $
 * Revision 1.1.10.3  1996/02/18  22:58:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:36  marty]
 *
 * Revision 1.1.10.2  1995/12/13  16:25:21  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.1  1994/07/15  14:59:25  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:40  mdf]
 * 
 * Revision 1.1.2.5  1992/12/29  13:08:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:28  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:17  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:53  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:07:01  emartin
 * 	bl6c: delete_replica_done
 * 	init_replica, init_replica_done - remove master_info
 * 
 * Revision 1.1.2.3  1992/10/07  20:26:02  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:31  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:35:50  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:56  sekhar
 * 	First replicaiton code drop.
 * 	[1992/08/31  16:41:30  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Server - Replist Log operations
 */


#ifndef rs_log_replist__included
#define rs_log_replist__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

void rs_log_replist_init (
    error_status_t   *st
);

void rs_log_replist_add_replica (
    uuid_p_t                    rep_id,  
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    uuid_p_t                    init_id,    
    rs_replica_master_info_t    *master_info
);

void rs_log_replist_replace_replica (
    uuid_p_t                    rep_id,  
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    rs_replica_master_info_t    *master_info
);

void rs_log_replist_init_replica (
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id
);

void rs_log_replist_init_rep_done (
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id
);

void rs_log_replist_delete_replica (
    uuid_p_t                    rep_id,
    boolean32                   force_delete,
    rs_replica_master_info_t    *master_info
);

void rs_log_replist_delete_rep_done (
    uuid_p_t                    rep_id
);

void rs_log_replist_master (
    uuid_p_t                    master_id,
    rs_update_seqno_t           *master_seqno,
    rs_replica_master_info_t    *master_info
);

void rs_log_replist_bad_sw_rev (
    uuid_p_t                    rep_id,
    rs_replica_prop_t           *prev_prop_type
);

void rs_log_replist_sw_rev_ok (
    uuid_p_t                    rep_id,
    rs_replica_prop_t           *prop_type
);
#endif

