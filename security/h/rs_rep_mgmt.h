/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_mgmt.h,v $
 * Revision 1.1.10.3  1996/02/18  22:59:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:02  marty]
 *
 * Revision 1.1.10.2  1995/12/13  16:25:44  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:01 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/27  19:32 UTC  cuti
 * 	CHFts14111 bug fix
 * 
 * 	HP revision /main/cuti_mothra_bug/1  1995/03/23  18:47 UTC  cuti
 * 	Change signature name of rs_rep_mgmt_shutdown
 * 	[1995/12/11  15:15:03  root]
 * 
 * Revision 1.1.6.3  1994/06/02  21:11:00  mdf
 * 	Merged with changes from 1.1.6.2
 * 	[1994/06/02  21:10:45  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:14  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.5  1993/12/20  22:27:47  hanfei
 * 	kk start_master => start_new_master
 * 	reinstate gamera start_master decl.
 * 	[1993/12/20  22:25:11  hanfei]
 * 
 * Revision 1.1.4.4  1993/12/17  22:28:28  hanfei
 * 	add rs_rep_mgmt_mark_reps_for_init
 * 	[1993/12/17  22:07:43  hanfei]
 * 
 * Revision 1.1.4.3  1993/12/16  18:44:52  hanfei
 * 	add decl for rs_rep_mgmt_become_slave
 * 	[1993/12/16  17:16:18  hanfei]
 * 
 * 	delete decl rs_rep_mgmt_start_new_master
 * 	[1993/12/11  18:54:43  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:56:05  hanfei
 * 	added rs_rep_mgmt_start_new_master,rs_rep_mgr_change_master
 * 	and rs_rep_mgr_become_slave.
 * 	[1993/10/29  17:55:12  hanfei]
 * 
 * 	 Second replication code drop: bl5
 * 	[1992/10/06  15:39:48  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:46  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.6.2  1994/06/02  20:21:58  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:14  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.5  1993/12/20  22:27:47  hanfei
 * 	kk start_master => start_new_master
 * 	reinstate gamera start_master decl.
 * 	[1993/12/20  22:25:11  hanfei]
 * 
 * Revision 1.1.4.4  1993/12/17  22:28:28  hanfei
 * 	add rs_rep_mgmt_mark_reps_for_init
 * 	[1993/12/17  22:07:43  hanfei]
 * 
 * Revision 1.1.4.3  1993/12/16  18:44:52  hanfei
 * 	add decl for rs_rep_mgmt_become_slave
 * 	[1993/12/16  17:16:18  hanfei]
 * 
 * 	delete decl rs_rep_mgmt_start_new_master
 * 	[1993/12/11  18:54:43  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:56:05  hanfei
 * 	added rs_rep_mgmt_start_new_master,rs_rep_mgr_change_master
 * 	and rs_rep_mgr_become_slave.
 * 	[1993/10/29  17:55:12  hanfei]
 * 
 * 	 Second replication code drop: bl5
 * 	[1992/10/06  15:39:48  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:46  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.7  1993/02/26  23:48:00  emartin
 * 	em_bl6f: try to fix bsubmit problem - no change
 * 	[1993/02/26  23:02:57  emartin]
 * 
 * 	em_bl6f: nuke rs_rep_mgmt_slave_incompat_sw
 * 	 decl rs_rep_mgmt_stop_til_sw_ok_task,
 * 	      rs_rep_mgmt_stop_til_sw_ok
 * 	[1993/02/22  22:53:26  emartin]
 * 
 * Revision 1.1.2.6  1992/12/29  13:25:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:19  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:10:46  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:49  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:34:30  emartin
 * 	bl6c: decl rs_rep_mgmt_slave_chk... routines
 * 
 * Revision 1.1.2.4  1992/11/04  19:32:29  ahop
 * 	Third replication drop: bl6a
 * 	add start_master, remove delete_replica, add get_info
 * 	[1992/11/03  18:13:25  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:26:59  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:09  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:39:48  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:18:54  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:47:08  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - replica management mainly
 *          for startup and delete/stop of slave and 
 *          master.
 */

#ifndef rs_rep_mgmt_h__included
#define rs_rep_mgmt_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <dce/rplbase.h>

#include <macros.h>

void rs_rep_mgmt_init (
    rs_replica_twr_vec_p_t  rep_twrs,
    boolean32               restore_masterf,
    rs_update_seqno_t       *master_seqno,
    error_status_t          *st
);

/*
 * Init master replist and start prop tasks
 * after master has been running for a while.
 * Invoked when master comes out of maintenance.
 */
void rs_rep_mgmt_start_master (
    error_status_t          *st
);

/*
 * Invoked when a slave becomes the master.
 * All replicas are marked for init when a
 * force become master is done.
 */
void rs_rep_mgmt_start_new_master (
    boolean32                   force_become_master,
    error_status_t              *st
);

void rs_rep_mgmt_mark_reps_for_init (
    unsigned32              dont_init_deleted_reps,
    error_status_t          *st
);

/*
 * Tell the master that a slave process has just 
 * restarted.  
 * The slave sends its tower set to the master.
 * If the slave's tower set has changed, the master will
 * update its copy of the replica list and propagate the
 * update to the other replicas.
 */
void rs_rep_mgmt_i_am_slave (
    void
);

/*
 * Invoked by the master when it receives an i_am_slave
 * message from a replica.
 * Check the slave's compat software version.
 * If the slave's tower set has changed, replace the
 * slave's info on the replist and log the change.
 *
 * Caller should have replist RIW lock.
 */
void rs_rep_mgmt_master_chk_i_am_slv (
    uuid_p_t                rep_id,
    unsigned32              compat_sw_rev,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st     
);

/*
 * Master's check that the slave's software
 * version is ok and that its software version is 
 * ok.  Master will crash if its sw version is
 * bad.
 */
void rs_rep_mgmt_master_ck_sw_compat (
    unsigned32              slave_compat_sw_rev,
    error_status_t          *st     
);

/*
 * Slave's check that the update it has just received
 * is from the master and is in correct order.
 * The slave also checks that its software version
 * matches the master's; the slave invokes a task
 * to kill itself if its sw rev is incorrect
 */
void rs_rep_mgmt_slave_chk_update (
    rs_replica_master_info_t    *master_info,
    boolean32                   *apply_update,
    error_status_t              *st
);

/*
 * Slave's check that the update is from the
 * master and that its software version is
 * compatible with the master's.
 */
void rs_rep_mgmt_slave_chk_master (
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
);

void rs_rep_mgmt_slave_chk_upd_seqno (
    rs_replica_master_info_t    *master_info,
    rs_update_seqno_t           *last_upd_seqno,
    boolean32                   *apply_update,
    error_status_t              *st
);

/*
 * Check that an update which is directed to the
 * propq only is ok; ie. that the update is from 
 * the master and that it is in correct order.
 * The slave's state has already been verified
 * to be rs_c_state_becoming_master.
 */
void rs_rep_mgmt_slave_chk_propq_upd (
    rs_replica_master_info_t    *master_info,
    boolean32                   *apply_update,
    error_status_t              *st
);

/*
 * copy propq has finished.
 * clear the last_upd_seqno_inited flag in
 * case another propq is done.
 * check that the last update that went on the
 * propq has the same seqno as the last
 * update that the replica received during
 * normal propagation.
 */
void rs_rep_mgmt_copy_propq_done (
    error_status_t              *st
);

void rs_rep_mgmt_get_info (
    rs_replica_info_t   *rep_info,
    error_status_t      *st
);

void rs_rep_mgmt_get_info_full (
    rs_replica_info_t       *rep_info,
    rs_replica_twr_vec_p_t  *rep_twrs,
    rs_replica_twr_vec_p_t  *master_twrs,
    error_status_t          *st
);

/*
 * STUB routine
 */
void rs_rep_mgmt_change_myname (
    unsigned_char_p_t       new_rep_name,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
);

/*
 * start a task to stop the process
 */
void rs_rep_mgmt_stop_til_sw_ok_task (
    void
);

void rs_rep_mgmt_stop_til_sw_ok (
    error_status_t  *st
);

void rs_rep_mgmt_shutdown (
    boolean32               checkpoint,
    error_status_t          *st
);

void rs_rep_mgmt_delete (
    error_status_t          *st
);

boolean rs_rep_mgmt_set_sw_rev (
    unsigned32      new_sw_rev,
    handle_t        auth_rpc_handle, 
    error_status_t  *st_p
);

void rs_rep_mgmt_slave_chk_stop_til_compat (
    unsigned32      compat_sw_rev, 
    error_status_t  *st_p
);

#endif
