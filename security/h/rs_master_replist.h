/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master_replist.h,v $
 * Revision 1.1.11.3  1996/02/18  22:59:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:45  marty]
 *
 * Revision 1.1.11.2  1995/12/13  16:25:25  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:01 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add prev_prop_type to signature of
 * 	rs_m_replist_get_rep_prop_info().
 * 	[1995/01/30  00:52 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.7.2  1994/09/20  20:14:36  rps
 * 	[OT11944] add rs_m_replist_check_prop_seqno
 * 	[1994/09/20  20:08:17  rps]
 * 
 * Revision 1.1.7.1  1994/07/15  14:59:27  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:43  mdf]
 * 
 * Revision 1.1.4.2  1993/10/29  17:43:42  hanfei
 * 	add rs_m_replist_construct() header.
 * 	[1993/10/29  17:43:00  hanfei]
 * 
 * 	 Second replication code drop: bl5
 * 	[1992/10/06  15:38:41  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:33  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.7  1993/02/03  22:06:08  ahop
 * 	bl6e: do rs_m_replist_mark_rep_purged
 * 	[1993/01/29  02:46:08  ahop]
 * 
 * Revision 1.1.2.6  1992/12/29  13:08:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:41  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:10:22  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:05  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:09:09  emartin
 * 	bl6c: decl rs_m_replist_init_replica,
 * 	rs_m_replist_init_replica_done,
 * 	rs_m_replist_get_num_prop_to_replicas,
 * 	rs_m_replist_get/free_init_from_reps
 * 
 * Revision 1.1.2.4  1992/11/04  19:32:02  ahop
 * 	Third replication drop: bl6a
 * 	rs_m_replist_replace_replica => rs_m_replist_replace_rep_twrs,some arg changes
 * 	[1992/11/03  18:11:14  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:26:21  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:46  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:38:41  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:18:19  sekhar
 * 	First replication code drop
 * 	[1992/08/31  16:43:24  sekhar]
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Master's routines to manage a copy of the
 *          replist in volatile memory for bookkeeping about 
 *          propagation to the replicas
 */

#ifndef rs_master_replist_h__included
#define rs_master_replist_h__included

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>

void rs_m_replist_init (
    error_status_t              *st          
);

void rs_m_replist_free (
    void
);

void rs_m_replist_add_replica (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_handle_t    *prop_mgt_h,
    error_status_t              *st          
);

void rs_m_replist_replace_rep_twrs (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    rs_replica_twr_vec_p_t      rep_twrs,
    error_status_t              *st          
);

void rs_m_replist_delete_replica (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st          
);

void rs_m_replist_purge_replica (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st          
);

/*
 * Mark a replica as purged
 */
void rs_m_replist_mark_rep_purged (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st          
);

/*
 * Mark a replica for initialization
 */
void rs_m_replist_init_replica (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id,
    error_status_t              *st          
);

/*
 * Mark a replica as ready to receive propagated 
 * updates
 */
void rs_m_replist_init_replica_done (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id,
    uuid_p_t                    init_from_rep_id,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *init_st,
    error_status_t              *st
);

void rs_m_replist_get_rep_prop_info (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    rs_replica_prop_info_t      *prop_info,
    rs_replica_prop_t           *prev_prop_type,
    error_status_t              *st          
);

void rs_m_replist_get_rep_comm_info (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    rs_replica_comm_info_t      *comm_info,
    error_status_t              *st          
);

void rs_m_replist_get_base_prop_seqn (
    rs_update_seqno_t           *base_prop_seqno,
    rs_update_seqno_t           *last_upd_seqno,
    rs_update_seqno_t           *new_base_prop_seqno
);

boolean32 rs_m_replist_check_prop_seqno (
    boolean32			*retry,
    rs_update_seqno_t		*seqno,
    error_status_t		*status
);

/*
 * Return a count of the number of replicas on the
 * replist minus the master.
 *
 * Caller must have the replist locked for read.
 */
void rs_m_replist_get_nprop_to_reps (
    unsigned32  *nprop_reps
);

void rs_m_replist_get_init_frm_reps (
    uuid_p_t                init_from_rep_id,
    unsigned32              *nreps,
    rs_replica_mgt_item_p_t **init_from_reps,
    uuid_p_t                **init_from_rep_ids,
    rs_replica_twr_vec_p_t  **init_from_rep_twrs,
    error_status_t          *st
);

void rs_m_replist_free_init_frm_reps (
    unsigned32              nreps,
    rs_replica_mgt_item_p_t **init_from_reps,
    uuid_p_t                **init_from_rep_ids,
    rs_replica_twr_vec_p_t  **init_from_rep_twrs
);

void rs_m_replist_stop_til_sw_compat (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st
);

void rs_m_replist_bad_sw_rev (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st
);

#endif

