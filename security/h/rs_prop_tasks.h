/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_tasks.h,v $
 * Revision 1.1.10.3  1996/02/18  22:59:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:52  marty]
 *
 * Revision 1.1.10.2  1995/12/13  16:25:34  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:01 UTC  greg
 * 	Fix botched merge,
 * 	[1995/04/24  17:36 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	Migration fixes.  Remove
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/03  15:54 UTC  emartin
 * 	merge in some replication fixes
 * 
 * 	HP revision /main/emartin_mothra2/1  1995/01/30  22:21 UTC  emartin
 * 	add rs_prop_tasks_slave_alive
 * 	remove rs_prop_tasks_refrsh_slv_creds
 * 	[1995/12/11  15:14:57  root]
 * 
 * Revision 1.1.6.3  1994/06/02  21:05:19  mdf
 * 	Merged with changes from 1.1.6.2
 * 	[1994/06/02  21:05:05  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:03  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/16  18:44:39  hanfei
 * 	add decl for rs_prop_copy_propq,
 * 		rs_prop_update_rep_binding
 * 	replace rs_prop_updates_to_new_master
 * 	[1993/12/11  18:53:49  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:51:52  hanfei
 * 	added rs_prop_update_to_new_master, rs_prop_get_slave_seqno
 * 	and rs_prop_clear_replistq
 * 	[1993/10/29  17:50:58  hanfei]
 * 
 * 	CR 7521.  decl. rs_prop_tasks_refrsh_slv_creds
 * 	[1993/03/24  20:38:53  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:56  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:25  sekhar]
 * 
 * 	Third replication drop: bl6a
 * 	add rs_prop_tasks_start
 * 	[1992/11/03  18:11:53  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:57  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:39  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:10:28  emartin
 * 	bl6c: decl rs_prop_tasks_status_comm_good
 * 
 * Revision 1.1.1.2  1992/10/06  15:39:13  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.6.2  1994/06/02  20:21:48  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:03  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/16  18:44:39  hanfei
 * 	add decl for rs_prop_copy_propq,
 * 		rs_prop_update_rep_binding
 * 	replace rs_prop_updates_to_new_master
 * 	[1993/12/11  18:53:49  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:51:52  hanfei
 * 	added rs_prop_update_to_new_master, rs_prop_get_slave_seqno
 * 	and rs_prop_clear_replistq
 * 	[1993/10/29  17:50:58  hanfei]
 * 
 * 	CR 7521.  decl. rs_prop_tasks_refrsh_slv_creds
 * 	[1993/03/24  20:38:53  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:56  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:25  sekhar]
 * 
 * 	Third replication drop: bl6a
 * 	add rs_prop_tasks_start
 * 	[1992/11/03  18:11:53  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:57  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:39  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:10:28  emartin
 * 	bl6c: decl rs_prop_tasks_status_comm_good
 * 
 * Revision 1.1.1.2  1992/10/06  15:39:13  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.3.6  1993/03/25  16:18:30  emartin
 * 	CR 7521.  decl. rs_prop_tasks_refrsh_slv_creds
 * 	[1993/03/24  20:38:53  emartin]
 * 
 * Revision 1.1.3.5  1992/12/29  13:08:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:56  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:10:34  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:25  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:10:28  emartin
 * 	bl6c: decl rs_prop_tasks_status_comm_good
 * 
 * Revision 1.1.3.3  1992/11/04  19:32:10  ahop
 * 	Third replication drop: bl6a
 * 	add rs_prop_tasks_start
 * 	[1992/11/03  18:11:53  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:26:42  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:57  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:39:13  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */

/*  
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Propagation task management
 */

#ifndef rs_prop_tasks__included
#define rs_prop_tasks__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_base.h> 
#include <rs_rep_pvt.h> 
#include <rs_master_replist_pvt.h>

void rs_prop_tasks_start (
    error_status_t          *st
);

void rs_prop_tasks_stop (
    error_status_t          *st
);

void rs_prop_tasks_wakeup (
    void
);

void rs_prop_updates_to_new_master (
    uuid_p_t                new_master_id,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
);
    
void rs_prop_copy_propq (
    handle_t                    h,
    error_status_t              *st
);

void rs_prop_tasks_slave_alive (
    rs_rep_prop_mgt_handle_t    prop_mgt_h
);

void rs_prop_tasks_get_master_info (
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
);

/* Mark this replica as being in a good
 * comm state
 */
void rs_prop_tasks_status_comm_good (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    error_status_t              *st
);

void rs_prop_tasks_handle_i_am_slave (
    rs_rep_prop_mgt_handle_t  prop_mgt_h,
    unsigned32                slave_compat_sw_rev,
    lock_mode_t               lock_mode,
    error_status_t            *st_p
);

#endif
