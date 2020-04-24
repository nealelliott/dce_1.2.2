/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master_replist.c,v $
 * Revision 1.1.71.3  1996/02/18  00:18:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:12  marty]
 *
 * Revision 1.1.71.2  1995/12/08  17:53:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:07 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:58 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/06  23:41 UTC  rps
 * 	Fix return value typo (affects only error returns)
 * 	[1995/12/08  17:20:49  root]
 * 
 * Revision 1.1.67.5  1994/09/20  20:14:40  rps
 * 	[OT11944] add rs_m_replist_check_prop_seqno
 * 	[1994/09/20  20:09:32  rps]
 * 
 * Revision 1.1.67.4  1994/08/25  20:14:26  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:11:40  sekhar]
 * 
 * Revision 1.1.67.3  1994/08/04  16:13:38  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:39  mdf]
 * 
 * Revision 1.1.67.1  1994/06/02  21:25:33  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:06  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  18:54 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/04/20  15:11 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/12  14:40 UTC  emartin
 * 	rs_m_replist_construct moved to rs_master_change.c
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:03  mdf
 * 	merge
 * 
 * Revision 1.1.4.3  1993/12/16  19:11:13  hanfei
 * 	rework for rs_m_replist_construct.
 * 	[1993/12/11  18:48:16  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  16:51:32  hanfei
 * 	work for change master : add  rs_m_replist_construct().
 * 	[1993/10/29  16:50:44  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	[1992/10/06  17:19:17  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:29  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.9  1993/02/26  23:51:34  emartin
 * 	em_bl6f: rs_m_replist_get_rep_prop_info - if entry is the
 * 	 master's, don't return number of updates
 * 	[1993/02/22  23:04:36  emartin]
 * 
 * 	em_bl6f: rs_m_replist_get_init_frm_reps - don't give out
 * 	a replica whose last_upd_inited is false.  master hasn't
 * 	verified that this guy is ok (sw rev, seqnos, etc)
 * 	[1993/02/11  20:21:01  emartin]
 * 
 * Revision 1.1.2.8  1993/02/03  22:07:57  ahop
 * 	bl6e: do rs_m_replist_mark_rep_purged
 * 	[1993/01/29  02:46:48  ahop]
 * 
 * Revision 1.1.2.7  1993/01/26  23:08:06  emartin
 * 	fix rs_m_replist_get_init_frm_reps to return corrects nreps
 * 	[1993/01/13  16:29:36  emartin]
 * 
 * Revision 1.1.2.6  1992/12/29  16:35:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:30  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:12:10  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:50:00  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:43:28  emartin
 * 	bl6c: move types to rs_master_replist_pvt.h
 * 	new routines for init replica
 * 
 * Revision 1.1.2.4  1992/11/04  19:36:31  ahop
 * 	Third replication drop: bl6a
 * 	fill in STUB routines
 * 	[1992/11/03  18:37:17  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:34:10  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:28  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:25:08  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:53:55  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */

/*    
 *      Registry server master's volatile replica list management
 */
    
#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_master_replist.c,v 1.1.71.3 1996/02/18 00:18:30 marty Exp $";
#endif

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/rpc.h>
#include <dce/uuid.h>

#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>

#include <rs_master_replist_pvt.h>
#include <rs_master_replist.h>
#include <rs_state.h>
#include <rs_log.h>
#include <rs_util.h>
#include <rs_rep_util.h>
#include <rs_rep_bind.h>
#include <rs.h>
#include <rs_prop_tasks.h>

#include <rsdb_replica.h>

#include <sec_svc.h>

/*
 * Public variables
 */

PUBLIC rs_list_t   rs_master_replistq;

/*
 * Local variables
 */

PRIVATE boolean32   rs_m_replist_inited = false;

/*
 * Local routines
 */

PRIVATE boolean32 inited_and_master (
    error_status_t              *st          
);

PRIVATE boolean32 prop_mgt_handle_ok (
    rs_rep_prop_mgt_item_p_t    prp,
    uuid_p_t                    rep_id,
    error_status_t              *st          
);

PRIVATE void fill_rep_prop_mgt_item_for_init (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_item_p_t    *o_prp,
    error_status_t              *st
);

PRIVATE void fill_rep_prop_mgt_item_for_addr (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_item_p_t    *o_prp,
    error_status_t              *st
);

PRIVATE void fill_rep_prop_mgt_item (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_item_p_t    *o_prp,
    error_status_t              *st
);



/*
 * Initialize the master's volatile copy
 * of the replica list.
 */
PUBLIC void rs_m_replist_init 
  (
    error_status_t              *st          
  )
{
    uuid_t                      nil_uuid,
                                cursor;
    uuid_t                      rep_id;
    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_buff_len;
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    CLEAR_STATUS(st);

    if (rs_m_replist_inited) return;

    rs_util_list_init(&rs_master_replistq);
    uuid_create_nil(&nil_uuid, st);
    if (BAD_STATUS(st)) return;

    cursor = nil_uuid;

    while (true) {
        rsdb_replica_lookup_by_ref(&cursor, &rp, 
            &rep_buff, &rep_buff_len, st);
        if (BAD_STATUS(st)) return;

        fill_rep_prop_mgt_item_for_init(rp, &prp, st);
        if (BAD_STATUS(st)) return;

        rs_util_list_add(&rs_master_replistq, &prp->list);

        /* 
         * NB: rp points to the actual rsdb data for replica,
         * not a copy of the data.  This data will be replaced
         * by rsdb_replica_set_prop_mgt_handle so don't use
         * rp after that call
         */

        rep_id = rp->rep_id;
        rsdb_replica_set_prop_mgt_handle(
            &rep_id, (rs_rep_prop_mgt_handle_t) prp, st);
        if (BAD_STATUS(st)) return;

        /* check for end of replist
         */
        if (uuid_is_nil(&cursor, &lst))
            break;
    }

    rs_m_replist_inited = true;
}

/*
 * Free the master's volatile copy of the
 * replica list
 */
PUBLIC void rs_m_replist_free (
    void
)
{
    rs_rep_prop_mgt_item_p_t    prp,
                                next_prp;
    error_status_t              lst;

    if (!rs_m_replist_inited) return;

    for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
         prp != NULL;
         prp = next_prp) {
 
        /*
         * will remove this entry from the list and free its
         * storage so get a ptr to the next entry on the list
         * now
         */
        next_prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd;

        if (prp->prop_in_progress) {
	    dce_svc_printf(SEC_REP_PROP_IN_PROGRESS_MSG);
        }

        /*
         * remove this item from rs_master_replistq and
         * free all its memory
         */
        rs_m_replist_purge_replica((rs_rep_prop_mgt_handle_t) prp,
            &prp->rep_id, &lst);
    }

    rs_m_replist_inited = false;
}

/*
 * Add a replica to the master's volatile
 * copy of the replica list
 */
PUBLIC void rs_m_replist_add_replica 
  (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_handle_t    *prop_mgt_h,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;

    *prop_mgt_h = NULL;
    
    if (!inited_and_master(st)) return;

    fill_rep_prop_mgt_item_for_addr(rp, &prp, st);
    if (BAD_STATUS(st)) return;

    rs_util_list_add(&rs_master_replistq, &prp->list);

    *prop_mgt_h = (rs_rep_prop_mgt_handle_t) prp;
    CLEAR_STATUS(st);
}

/*
 * Replace a replica's towers and put it
 * into the healthy communication state.
 */
PUBLIC void rs_m_replist_replace_rep_twrs
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    rs_replica_twr_vec_p_t      rep_twrs,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    rs_replica_twr_vec_p_t      tmp_rep_twrs;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    if (!prp->master) {
        rs_rep_util_copy_compat_twrs(
            rep_twrs, &tmp_rep_twrs, st);
        if (BAD_STATUS(st)) return;

        rs_rep_util_free_twrs(&prp->rep_binding.rep_twrs, &lst);

        prp->rep_binding.twr_offset = -1;
        prp->rep_binding.rep_twrs = tmp_rep_twrs;
        prp->rep_binding.valid_handle = false;

        prp->comm_info.comm_state = rs_c_replica_comm_ok;
        prp->comm_info.nprop_tries = 0;
        prp->comm_info.last_status = error_status_ok;
    }

    CLEAR_STATUS(st);
}

/*
 * Set a replica's prop_state to delete
 */
PUBLIC void rs_m_replist_delete_replica
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    prp->prop_info.prop_type = rs_c_replica_prop_delete;

    CLEAR_STATUS(st);
}

/*
 * Remove a replica from the master's
 * volatile copy of the replist.
 * If a propagation is in progress,
 * just mark this replica as purged;
 * it's memory will be freed when
 * the propagation has completed.
 */
PUBLIC void rs_m_replist_purge_replica
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    if (!prp->prop_in_progress) {
        rs_util_list_remove(&rs_master_replistq, &prp->list);

        rs_rep_bind_free_binding(&prp->rep_binding);

        free(prp);
    }
    else {
        prp->purged = true;
    }

    CLEAR_STATUS(st);
}

/*
 * Mark a replica as purged
 */
PUBLIC void rs_m_replist_mark_rep_purged
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    prp->purged = true;

    CLEAR_STATUS(st);
}

/*
 * Mark a replica for initialization
 */
PUBLIC void rs_m_replist_init_replica
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    prp->prop_info.prop_type = rs_c_replica_prop_init;
    prp->prop_info.last_upd_inited = false;

    prp->init_id = *init_id;
    uuid_create_nil(&prp->init_from_rep_id, &lst);

    CLEAR_STATUS(st);
}

PUBLIC void rs_m_replist_init_replica_done
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id,
    uuid_p_t                    init_from_rep_id,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *init_st,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    if ((prp->prop_info.prop_type != rs_c_replica_prop_init) &&
        (prp->prop_info.prop_type != rs_c_replica_prop_initing)) {
	dce_svc_printf(SEC_REP_PROP_TYPE_NOT_INIT_MSG);
    }

    if (GOOD_STATUS(init_st)) { 
        /* Slave reports that init succeeded
         */
        prp->prop_info.prop_type = rs_c_replica_prop_update;
        prp->prop_info.last_upd_inited = true;
        prp->prop_info.last_upd_seqno = *last_upd_seqno;
        prp->prop_info.last_upd_ts = *last_upd_ts;
    
        prp->init_id = *init_id;
        uuid_create_nil(&prp->init_from_rep_id, &lst);
    }
    else {
        /* Slave reports that init did not succeed
         */
        prp->prop_info.prop_type = rs_c_replica_prop_init;
        prp->prop_info.last_upd_inited = false;
    
        prp->init_id = *init_id;
        prp->init_from_rep_id = *init_from_rep_id;
    }

    rs_prop_tasks_status_comm_good((rs_rep_prop_mgt_handle_t) prp, st);
}
            
PUBLIC void rs_m_replist_get_rep_prop_info
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    rs_replica_prop_info_t      *prop_info,
    rs_replica_prop_t           *prev_prop_type,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    rs_update_seqno_t           last_upd_seqno;
    sec_timeval_t               last_upd_ts;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;

    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    *prop_info = prp->prop_info;
    *prev_prop_type = prp->prev_prop_type;

    if ((!prp->master) &&
        prop_info->last_upd_inited) {
        rs_log_get_update_info(&last_upd_seqno,
            &last_upd_ts, st);
        if (BAD_STATUS(st)) return;

        rs_util_seqno_sub(&last_upd_seqno, 
            &prop_info->last_upd_seqno, 
            &prop_info->num_updates, st);
        if (BAD_STATUS(st)) return;
    }

    CLEAR_STATUS(st);
}

PUBLIC void rs_m_replist_get_rep_comm_info
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    rs_replica_comm_info_t      *comm_info,
    error_status_t              *st          
  )
{
    rs_rep_prop_mgt_item_p_t    prp;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;

    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    comm_info->comm_state = prp->comm_info.comm_state;
    comm_info->last_status = prp->comm_info.last_status;
    comm_info->twr_offset = prp->rep_binding.twr_offset;

    CLEAR_STATUS(st);
}

/*
 * Get the current base_prop_seqno of the propq
 *
 * Find the earliest "last update seqno" of the 
 * replicas that are having updates propagated to 
 * them.  Replicas that are marked for init or delete
 * are not being sent updates from the propq.
 */
PUBLIC void rs_m_replist_get_base_prop_seqn
  (
    rs_update_seqno_t           *base_prop_seqno,
    rs_update_seqno_t           *last_upd_seqno,
    rs_update_seqno_t           *new_base_prop_seqno
  )
{
    rs_update_seqno_t           seqno;
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    seqno = *last_upd_seqno;

    if (!inited_and_master(&lst)) return;

    for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
         prp != NULL;
         prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd) {

        if (prp->master) continue;

        if (prp->prop_info.prop_type == rs_c_replica_prop_update) {
            /* this replica is being sent updates on the propq
             */
            if (!prp->prop_info.last_upd_inited) {
                /* this replica's last_upd_seqno is
                 * not known so assume it hasn't received
                 * any updates on the current propq
                 */
                seqno = *base_prop_seqno;
                break;
            }
            else {
                /* is this replica's last update earlier than
                 * seqno?
                 */
                if (rs_util_seqno_cmp(
                        &prp->prop_info.last_upd_seqno, &seqno) < 0) {
                    seqno = prp->prop_info.last_upd_seqno;
                }
            }
        }
        else
        if (prp->prop_info.prop_type == rs_c_replica_prop_initing) {
            /* this replica is being initialized.
             * is the last update seqno that it will receive
             * earlier than seqno?
             */
            if (rs_util_seqno_cmp(
                    &prp->prop_info.last_upd_seqno, &seqno) < 0) {
                seqno = prp->prop_info.last_upd_seqno;
            }
        }
    }

    *new_base_prop_seqno = seqno;
}

/*
 * Check the prop sequences of replicas against base
 *
 * Check that all replicas are consistent with master.
 * Replicas that are marked for init or delete
 * are not being sent updates from the propq.
 *
 * Caller must have the replist locked for read.
 */
PUBLIC boolean32 rs_m_replist_check_prop_seqno
  (
    boolean32			*retry,
    rs_update_seqno_t		*seqno,
    error_status_t		*status
  )
{
    boolean32			result;
    rs_rep_prop_mgt_item_p_t    prp;

    *retry = false;
    CLEAR_STATUS(status);
    result = false;

    if (!inited_and_master(status) ||
	    (rs_state.rep_state != rs_c_state_in_service) ) {
	SET_STATUS(status, sec_rgy_read_only);
	return result;
    }

    if (!seqno->high && !seqno->low) {
	sec_timeval_t	ts;

	rs_log_get_update_info(seqno, &ts, status);
	if (BAD_STATUS(status))
	    return result;
    }

    result = true;

    for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
	 prp != NULL;
	 prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd) {

	if (prp->master)
	    continue;

	if (prp->prop_info.prop_type == rs_c_replica_prop_update) {
	    /* this replica is being sent updates on the propq
	     */
	    if (!prp->prop_info.last_upd_inited) {
		/* this replica's last_upd_seqno is
		 * not known so assume it hasn't received
		 * any updates on the current propq
		 */
		result = false;
		continue;
	    }

	    /* is this replica's last update earlier than
	     * seqno?
	     */
	    if (rs_util_seqno_cmp(
			&prp->prop_info.last_upd_seqno, seqno) < 0) {

		/* is the replica a zombie? */
		if (BAD_STATUS(&prp->comm_info.last_status)) {
		    result = false;
		    continue;
		}

		*retry = true;
		return result;
	    }
	}
/*	else prop initing or deleting; ignore */
    }

    return result;
}

/*
 * Return a count of the number of replicas on the
 * replist minus the master.
 *
 * Caller must have the replist locked for read.
 */
PUBLIC void rs_m_replist_get_nprop_to_reps
  (
    unsigned32  *nprop_reps
  )
{
    unsigned32                  nreps;
    rs_rep_prop_mgt_item_p_t    prp;

    nreps = 0;
    for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
         prp != NULL;
         prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd) {

        if (!prp->master) nreps++;
    }

    *nprop_reps = nreps;
}

/*
 * Return a list of replicas that are candidates
 * to initialize another replica (ie. replicas
 * that are themselves initialized and are communicado).
 * Don't include the "init_from_rep_id" in the list;
 * a previous init session with it failed.
 *
 * The routine mallocs and fills 3 arrays of ptrs
 * for the calling routine.  The first array is
 * an array of ptrs to rs_replica_mgt_items (which
 * are malloced and filled by rsdb_replica_lookup).  
 * The second array is an array of ptrs to replica 
 * uuids; these uuids are in the rs_replica_mgt_items.
 * The third array is an array of ptrs to tower
 * vectors; these ptrs are also in the rs_replica_mgt_items
 * and their storage is allocated by rsdb_replica_lookup.
 */
PUBLIC void rs_m_replist_get_init_frm_reps
  (
    uuid_p_t                init_from_rep_id,
    unsigned32              *nreps,
    rs_replica_mgt_item_p_t **init_from_reps,
    uuid_p_t                **init_from_rep_ids,
    rs_replica_twr_vec_p_t  **init_from_rep_twrs,
    error_status_t          *st
  )
{
#define rs_c_max_init_from_reps 10

    unsigned32                  l_nreps;
    signed32                    nr,
                                nr2;
    rs_rep_prop_mgt_item_p_t    prp;
    uuid_t                      cursor;
    rs_replica_mgt_item_p_t     *rpp;
    uuid_p_t                    *rid_p;
    rs_replica_twr_vec_p_t      *rtwr_p;
    error_status_t              lst;

    *nreps = 0;
    l_nreps = 0;

    /*
     * calculate max number of replicas that may be
     * returned
     */
    for (nr = 0, prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
         nr < rs_c_max_init_from_reps, prp != NULL;
         nr++, prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd)  ;


    *init_from_reps = 
	(rs_replica_mgt_item_p_t *)malloc(sizeof(**init_from_reps) * nr);
    *init_from_rep_ids = 
	(uuid_p_t *)malloc(sizeof(**init_from_rep_ids) * nr);
    *init_from_rep_twrs = 
	(rs_replica_twr_vec_p_t *)malloc(sizeof(**init_from_rep_twrs) * nr);
    rpp = *init_from_reps;
    rid_p = *init_from_rep_ids;
    rtwr_p = *init_from_rep_twrs;

    for (nr2 = 0, prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
         nr2 < (nr - 1), prp != NULL;
         nr2++, prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd) {

        if (prp->master || prp->purged) continue;
        if (uuid_equal(init_from_rep_id, &prp->rep_id, &lst)) continue;

        if ((prp->prop_info.prop_type == rs_c_replica_prop_update) &&
            (prp->prop_info.last_upd_inited) &&
            (prp->comm_info.comm_state == rs_c_replica_comm_ok)) {

            cursor = prp->rep_id;
            rsdb_replica_lookup(&cursor, rpp, st);
            if (BAD_STATUS(st)) {
                SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_ENTRY_NOT_FOUND_MSG, *st);
            }
            *rid_p = &((*rpp)->rep_id);
            *rtwr_p = (*rpp)->rep_twrs;

            rpp++;
            rid_p++;
            rtwr_p++;

            l_nreps++;
        }
    }

    /* put master at end of list
     */
    cursor = rs_state.rep_id;
    rsdb_replica_lookup(&cursor, rpp, &lst);
    *rid_p = &((*rpp)->rep_id);
    *rtwr_p = (*rpp)->rep_twrs;

    l_nreps++;

    *nreps = l_nreps;
}

/*
 * Free arrays, etc. that were allocated by 
 * rs_m_replist_get_init_from_reps
 */
PUBLIC void rs_m_replist_free_init_frm_reps
  (
    unsigned32              nreps,
    rs_replica_mgt_item_p_t **init_from_reps,
    uuid_p_t                **init_from_rep_ids,
    rs_replica_twr_vec_p_t  **init_from_rep_twrs
  )
{
    rs_replica_mgt_item_t   **rpp;
    unsigned32              nr;
    error_status_t          lst;

    for (nr = 0, rpp = *init_from_reps;
         nr < nreps; 
         nr++, rpp++) {
        rsdb_replica_item_free(rpp, &lst);
    }

    free(*init_from_reps);
    free(*init_from_rep_ids);
    free(*init_from_rep_twrs);

    *init_from_reps = NULL;
    *init_from_rep_ids = NULL;
    *init_from_rep_twrs = NULL;
}

PRIVATE boolean32 inited_and_master
  (
    error_status_t              *st          
  )
{
    if ((!rs_m_replist_inited) ||
        (!rs_state_master()) ) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_pgmerr);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        return false;
    }
    else {
        CLEAR_STATUS(st);
        return true;
    }
}

PRIVATE boolean32 prop_mgt_handle_ok 
  (
    rs_rep_prop_mgt_item_p_t    prp,
    uuid_p_t                    rep_id,
    error_status_t              *st          
  )
{
    error_status_t  lst;

    if ((prp == NULL) ||
        (!uuid_equal(rep_id, &prp->rep_id, &lst)) ) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_pgmerr);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        return false;
    }
    else {
        CLEAR_STATUS(st);
        return true;
    }
}

/*
 * Invoked during master replist initialization
 * to fill a prop_mgt_item.  
 * If the replica has no compatible towers just 
 * set its rep_twrs field to NULL, report an error, 
 * and continue.
 */
PRIVATE void fill_rep_prop_mgt_item_for_init
  (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_item_p_t    *o_prp,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp; 
    unsigned_char_p_t           rep_str;
    char                        error_msg[128];
    error_status_t              lst;

    *o_prp = NULL;

    fill_rep_prop_mgt_item(rp, &prp, st);
    if (BAD_STATUS(st)) return;

    if (!prp->master) {
        rs_rep_bind_init_binding(rp->rep_twrs, 
            &prp->rep_binding, st);
    }
    else {
        rs_rep_bind_init_binding(NULL,
            &prp->rep_binding, st);
    }
    if (BAD_STATUS(st)) {
        if (STATUS_EQUAL(st, sec_rgy_rep_bad_binding)) {
            uuid_to_string(&rp->rep_id, &rep_str, &lst);
            /**** SVC_OPEN : Turn this into a debug statement. Status cleared ***/
            sprintf(error_msg, 
                "rs_m_replist_init no compatible towers with replica %s",
                (char *) rep_str);
            error(error_msg);
            rpc_string_free(&rep_str, &lst);
            CLEAR_STATUS(st);
        }
        else {
            free(prp);
            return;
        }
    }

    *o_prp = prp;
}

/*
 * Invoked when a new replica is being added to
 * the master's replist to fill its prop_mgt_item.   
 * 
 * If the replica has no compatible towers 
 * return an error.
 */
PRIVATE void fill_rep_prop_mgt_item_for_addr
  (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_item_p_t    *o_prp,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp; 
    error_status_t              lst;

    *o_prp = NULL;

    fill_rep_prop_mgt_item(rp, &prp, st);
    if (BAD_STATUS(st)) return;

    if (!prp->master) {
        rs_rep_bind_init_binding(rp->rep_twrs, 
            &prp->rep_binding, st);
    }
    else {
        rs_rep_bind_init_binding(NULL,
            &prp->rep_binding, st);
    }
    if (BAD_STATUS(st)) {
        free(prp);
        return;
    }

    *o_prp = prp;
}

/*
 * Allocate a rs_rep_prop_mgt_item and
 * fill its fields.  Its rep_binding fields
 * will be filled later.
 */
PRIVATE void fill_rep_prop_mgt_item
  (
    rs_replica_mgt_item_p_t     rp,
    rs_rep_prop_mgt_item_p_t    *o_prp,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    *o_prp = NULL;

    prp = (rs_rep_prop_mgt_item_p_t) malloc(sizeof(*prp));
    if (prp == NULL) {
        SET_STATUS(st, sec_s_no_memory);
        return;
    }

    prp->rep_id = rp->rep_id;
    prp->purged = false;
    prp->master = uuid_equal(&rp->rep_id, &rs_state.rep_id, &lst);

    prp->prop_in_progress = false;
    prp->init_id = rp->init_id;
    uuid_create_nil(&prp->init_from_rep_id, &lst);

    prp->prop_info.prop_type = rp->prop_type;
    prp->prop_info.last_upd_inited = false;

    prp->comm_info.comm_state = rs_c_replica_comm_ok;
    prp->comm_info.nprop_tries = 0;
    prp->comm_info.last_status = error_status_ok;

    prp->compat_sw_rev =  PROP_MGT_SW_REV_UNKNOWN;
    prp->prev_prop_type =  rp->prev_prop_type;

    *o_prp = prp;
    CLEAR_STATUS(st);
}


/*
 *  
 */
PUBLIC void rs_m_replist_bad_sw_rev
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    CLEAR_STATUS(st);
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    if (prp->prop_info.prop_type == rs_c_replica_prop_bad_sw_rev) {
	return;
    }

    /* 
     * Save current prop type so we can restore it should the
     * replica come back on line at the correct sw rev.  
     */
    prp->prev_prop_type = prp->prop_info.prop_type;
    prp->prop_info.prop_type = rs_c_replica_prop_bad_sw_rev;

}

PUBLIC void rs_m_replist_sw_rev_ok
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    uuid_p_t                    rep_id,
    unsigned32                  master_compat_sw_rev,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;
    CLEAR_STATUS(st);
    
    if ( (!inited_and_master(st)) ||
         (!prop_mgt_handle_ok(prp, rep_id, st)) ) 
        return;

    if (prp->prop_info.prop_type == rs_c_replica_prop_bad_sw_rev) {
	/* 
	 * Restore previous prop_type that was saved when the replica was
	 * transistioned to the rs_c_replica_prop_bad_sw_rev prop type
	 */
	if (prp->prev_prop_type == rs_c_replica_prop_unknown) {
	    /* 
	     * We dont' know what state the replica was in when it got
	     * shut down so we have no choice but to schedule it for init.
	     */
	    prp->prop_info.prop_type = rs_c_replica_prop_init;
	} else {
	    prp->prop_info.prop_type = prp->prev_prop_type;
	}
	prp->prev_prop_type = rs_c_replica_prop_unknown;
    }

    prp->compat_sw_rev = master_compat_sw_rev;
}

