/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_replist2.c,v $
 * Revision 1.1.10.3  1996/02/18  00:19:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:04  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:56:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:09 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add compat parameter to rs_replist2_read_full()
 * 	so we know whther or not we're constrained to
 * 	remain compatible with the pre-1.1 sematics of
 * 	the read_full operation.
 * 	[1995/01/30  00:50 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	prev_prop_type doesn't need to be a pointer in
 * 	rsdb_replica_set_prop_info().
 * 	[1995/01/29  14:32 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:48 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add prev_prop_type to stable replist management info.
 * 
 * 	[1995/01/22  18:03 UTC  greg  /main/greg_migration2/2]
 * 
 * 	add rs_replist2_bad_sw_rev().
 * 	add rs_replist2_sw_rev_ok().
 * 	Deal with new prev_prop_type
 * 	field of stable prop mgmt info
 * 	(still have a ways to go in this one:
 * 	need to sort out how the prev_prop_type
 * 	fits into the rs_rep_util_rep_item_pack()
 * 	routine).
 * 	[1995/01/21  22:53 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.2  1994/08/04  16:14:01  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:23  mdf]
 * 
 * Revision 1.1.6.1  1994/06/02  21:23:28  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:01  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:01  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  21:16 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/03  20:41 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.4.3  1993/12/28  21:17:26  hanfei
 * 	rs_replist2_slave_delete_rep - map sec_rgy_object_not_found
 * 	to status_ok.
 * 	[1993/12/22  21:54:23  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  19:11:56  hanfei
 * 	do rs_replist2_purge_deleted_reps
 * 	[1993/12/13  21:50:03  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	read_replica - use rs_master_info_is_master
 * 	[1992/10/06  17:27:09  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:51:03  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.7  1993/03/03  22:31:34  emartin
 * 	master_add_rep, master_replace_rep - don't allow dup rep_name
 * 	[1993/03/03  22:27:37  emartin]
 * 
 * Revision 1.1.2.6  1992/12/29  16:37:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:26  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:13:00  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:49  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:53:56  emartin
 * 	bl6c: do init_replica, init_replica_done
 * 
 * Revision 1.1.2.4  1992/11/04  19:40:00  ahop
 * 	Third replication drop: bl6a
 * 	name changes (eg. master_add_replica, add_replica, slave_add_replica)
 * 	[1992/11/04  19:09:57  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:36:59  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:17  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:26:37  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:55:52  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry server replica list update and lookup "worker"
 *          routines
 */
    
#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_replist2.c,v 1.1.10.3 1996/02/18 00:19:51 marty Exp $";
#endif

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/uuid.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_rep_pvt.h>
#include <rs_lock.h>

#include <rs_replist2.h>
#include <rs_master_replist.h>
#include <rs_rep_util.h>

#include <rsdb_replica.h>
#include <sec_svc.h>

/*
 * Local routine prototypes
 */
PRIVATE void chk_dupe_rep_name (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    error_status_t          *st         /* [out] */
);

PRIVATE void read_replica (
    uuid_t                      *marker,
    rs_replica_item_t           *c_rp,
    rs_rep_prop_mgt_handle_t    *prop_mgt_h, 
    error_status_t              *st
);


/*
 * Add a replica to the replist.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when client add_replica
 * update arrives.
 */
PUBLIC void rs_replist2_master_add_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
  )
{
    rs_replica_mgt_item_t       rep_item;
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      tmp_init_id;

    /* check that there are some towers to
     * communicate with the replica
     */
    if ((rep_twrs == NULL) ||
        (rep_twrs->num_towers == 0)) {
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    /* check whether replica is already on replist
     */
    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type,
			       &tmp_init_id, st);
    if (BAD_STATUS(st)) {
        if (! STATUS_EQUAL(st, sec_rgy_object_not_found))
            return;
    }
    else {
        SET_STATUS(st, sec_rgy_rep_doppelganger);
        return;
    }

    CLEAR_STATUS(st);

    /* check whether this name is already on the replist
     */
    chk_dupe_rep_name(rep_id, rep_name, st);
    if (BAD_STATUS(st)) return;

    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        rs_c_replica_prop_init, rs_c_replica_prop_unknown,
        init_id, NULL, &rep_item);

    /* if maintaining master's volatile mgmt 
     * replist, add this item to the volatile
     * replist info and get a handle to the item.
     * This routine will check that the replica
     * has at least on protocol tower in common
     * with the master.
     */
    rs_m_replist_add_replica(&rep_item, &rep_item.prop_mgt, st);
    if (BAD_STATUS(st)) return;

    rsdb_replica_add(&rep_item, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Invoked at master restart time to replay
 * an add replica update from the log
 */
PUBLIC void rs_replist2_add_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
  )
{
    rs_replica_mgt_item_t       rep_item;

    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        rs_c_replica_prop_init, rs_c_replica_prop_unknown,
        init_id, NULL, &rep_item);

    rsdb_replica_replace(&rep_item, st);
    if (BAD_STATUS(st)) return;
}

/* Add a replica to the replist.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at a slave when a propagated 
 * update is received or when the log
 * is replayed at startup time.
 */
PUBLIC void rs_replist2_slave_add_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
  )
{
    rs_replica_mgt_item_t       rep_item;

    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        rs_c_replica_prop_update, rs_c_replica_prop_unknown,
        NULL, NULL, &rep_item);

    rsdb_replica_replace(&rep_item, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Replace information about replica "rep_id"
 * on the replist.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when a client replace_replica
 * update arrives or a slave i_am_slave message
 * indicates that the slave's tower set has changed.
 */
PUBLIC void rs_replist2_master_replace_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
  )
{
    rs_replica_mgt_item_t       rep_item;
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      init_id;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    error_status_t              lst;

    /* check that there are some towers to
     * communicate with the replica
     */
    if ((rep_twrs == NULL) ||
        (rep_twrs->num_towers == 0)) {
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    /*
     * Make sure this replica is already on the replist.
     * Get its init_id, prop_type, and prop_mgt handle.
     */
    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type, 
			       &init_id, st);
    if (BAD_STATUS(st)) return;

    rsdb_replica_get_prop_mgt_handle(rep_id, &prop_mgt_h, st);
    if (BAD_STATUS(st)) return;

    /* check whether this name is already on the replist
     * and is associated with another replica.
     */
    chk_dupe_rep_name(rep_id, rep_name, st);
    if (BAD_STATUS(st)) return;

    /* 
     * replace replica's name and tower set only
     */
    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        prop_type, prev_prop_type, &init_id, prop_mgt_h, &rep_item);

    /* if maintaining master's volatile mgmt 
     * replist, replace this replica's tower set
     */
    rs_m_replist_replace_rep_twrs(prop_mgt_h, 
        rep_id, rep_twrs, st);
    if (BAD_STATUS(st)) return;

    rsdb_replica_replace(&rep_item, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Invoked at restart time to replay
 * a replace replica update from the log.
 * Also invoked by the master to change
 * its replist entry. 
 */
PUBLIC void rs_replist2_replace_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
  )
{
    rs_replica_mgt_item_t       rep_item;
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      init_id;
    error_status_t              lst;

    /*
     * Get replica's init_id and prop_type
     */
    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type,
			       &init_id, st);
    if (BAD_STATUS(st)) return;

    /* 
     * replace replica's name and tower set only
     */
    rs_rep_util_rep_item_pack(rep_id, rep_name, rep_twrs,
        prop_type, prev_prop_type, &init_id, NULL, &rep_item);

    rsdb_replica_replace(&rep_item, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Initiate removal of replica "rep_id" from the 
 * replica list.  If not "force_delete" get organized
 * to send the secd server identified by "rep_id" 
 * a delete of itself.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when client update arrives
 */
PUBLIC void rs_replist2_master_delete_rep
  (
    uuid_p_t                rep_id,        /* [in] */
    boolean32               force_delete,  /* [in] */
    error_status_t          *st            /* [out] */
  )
{
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      init_id;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    error_status_t              lst;

    if (uuid_equal(&rs_state.rep_id, rep_id, &lst)) {
        SET_STATUS(st, sec_rgy_rep_master);
        return;
    }

    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type, 
			       &init_id, st);
    if (BAD_STATUS(st)) return;

    rsdb_replica_get_prop_mgt_handle(rep_id, &prop_mgt_h, st);
    if (BAD_STATUS(st)) return;

    if (force_delete) {
        rs_m_replist_purge_replica(prop_mgt_h, rep_id, st);
        if (BAD_STATUS(st)) return;
    
        rsdb_replica_delete(rep_id, st);
        if (BAD_STATUS(st)) return;
    }
    else {
        rs_m_replist_delete_replica(prop_mgt_h, rep_id, st);
        if (BAD_STATUS(st)) return;
    
        rsdb_replica_set_prop_info(rep_id, rs_c_replica_prop_delete, 
            rs_c_replica_prop_unknown, &init_id, st);
        if (BAD_STATUS(st)) return;
    }
}

/*
 * Replay delete replica update at master startup time
 */
PUBLIC void rs_replist2_delete_rep
  (
    uuid_p_t                rep_id,        /* [in] */
    boolean32               force_delete,  /* [in] */
    error_status_t          *st            /* [out] */
  )
{
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      init_id;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    error_status_t              lst;

    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type, 
			       &init_id, st);
    if (BAD_STATUS(st)) return;

    if (force_delete) {
        rsdb_replica_delete(rep_id, st);
        if (BAD_STATUS(st)) return;
    }
    else {
        rsdb_replica_set_prop_info(rep_id, rs_c_replica_prop_delete, 
           rs_c_replica_prop_unknown, &init_id, st);
        if (BAD_STATUS(st)) return;
    }
}

/* Remove a replica from the replist.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at a slave when a propagated 
 * update is received or when the log
 * is replayed at startup time.
 */
PUBLIC void rs_replist2_slave_delete_rep
  (
    uuid_p_t                rep_id,        /* [in] */
    error_status_t          *st            /* [out] */
  )
{
    rsdb_replica_delete(rep_id, st);
    if (STATUS_EQUAL(st, sec_rgy_object_not_found)) {
        CLEAR_STATUS(st);
    }
}

/* Remove replicas whose prop_type is deleted
 * from the replist.
 *
 * Invoked when the master becomes a slave and when
 * a slave boots.
 */
PUBLIC void rs_replist2_purge_deleted_reps
  (
    error_status_t          *st            /* [out] */
  )
{
    uuid_t                      marker;
    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_p_t  rep_buff;
    unsigned32                  rep_bufflen;
    error_status_t              lst;

    CLEAR_STATUS(st);

    uuid_create_nil(&marker, st);
    if (BAD_STATUS(st)) return;

    while (true) {
        rsdb_replica_lookup_by_ref(&marker, 
            &rp, &rep_buff, &rep_bufflen, st);
        if (BAD_STATUS(st)) return;

        if (rp->prop_type == rs_c_replica_prop_delete) {
            rsdb_replica_delete(&rp->rep_id, st);
        }

        /* marker == uuid_nil means no more on list
         */
        if (uuid_is_nil(&marker, &lst)) break;
    }
}

/*
 * Mark a replica for initialization.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when a client update arrives
 * and when the master decides a replica should
 * be re-inited (usually because its database
 * is very out of date because it has been 
 * incommunicado for a long time and the propq
 * was truncated).
 */
PUBLIC void rs_replist2_master_init_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
  )
{
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      tmp_init_id;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    error_status_t              lst;

    if (uuid_equal(&rs_state.rep_id, rep_id, &lst)) {
        SET_STATUS(st, sec_rgy_rep_master);
        return;
    }

    /*
     * Make sure this replica is already on the replist.
     * Get its prop_type and prop_mgt handle.
     */
    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type,
			       &tmp_init_id, st);
    if (BAD_STATUS(st)) return;

    rsdb_replica_get_prop_mgt_handle(rep_id, &prop_mgt_h, st);
    if (BAD_STATUS(st)) return;

    /*
     * Check replica's current prop type.
     * Mark it for init if its prop type is update.
     * Otherwise return an error.
     */
    switch(prop_type) {
        case rs_c_replica_prop_init:
            SET_STATUS(st, sec_rgy_rep_marked_for_init);
            break;

        case rs_c_replica_prop_update:
            rs_m_replist_init_replica(prop_mgt_h, rep_id, init_id, st);
            if (GOOD_STATUS(st))
                rsdb_replica_set_prop_info(rep_id, rs_c_replica_prop_init, 
                    rs_c_replica_prop_unknown, init_id, st);
            break;

        case rs_c_replica_prop_delete:
        case rs_c_replica_prop_bad_sw_rev:
            SET_STATUS(st, sec_rgy_rep_bad_state);
            break; 

        default:
            SET_STATUS(st, sec_rgy_rep_pgmerr);
            break;
    }
}

/*
 * Mark a replica for initialization.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when the log is replayed 
 * at restart time.
 */
PUBLIC void rs_replist2_init_rep
  (
    uuid_p_t                rep_id,     /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
  )
{
    rsdb_replica_set_prop_info(rep_id, rs_c_replica_prop_init, 
        rs_c_replica_prop_unknown, init_id, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Initialization of replica "rep_id" is
 * "reported" to be done.  Check things out
 * and change replica's prop state.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when client update arrives 
 * indicating that an init session has completed
 * or when the master's prop task detects that
 * the init session has completed.  The init_st
 * argument indicates whether or not the init
 * actually succeeded.
 */
PUBLIC void rs_replist2_master_init_rep_don
  (
    uuid_p_t            rep_id,
    uuid_p_t            init_id,
    uuid_p_t            init_from_rep_id,
    rs_update_seqno_t   *last_upd_seqno,
    sec_timeval_t       *last_upd_ts,
    error_status_t      *init_st,
    error_status_t      *st
  )
{
    rs_replica_prop_t           prop_type, prev_prop_type;
    uuid_t                      current_init_id;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    error_status_t              lst;

    /*
     * Make sure this replica is already on the replist.
     * Get its current prop_type, init_id, and prop_mgt handle.
     */
    rsdb_replica_get_prop_info(rep_id, &prop_type, &prev_prop_type,
			       &current_init_id, st);
    if (BAD_STATUS(st)) return;

    rsdb_replica_get_prop_mgt_handle(rep_id, &prop_mgt_h, st);
    if (BAD_STATUS(st)) return;

    if (uuid_equal(init_id, &current_init_id, &lst)) {
        switch(prop_type) {
            case rs_c_replica_prop_init:
                rs_m_replist_init_replica_done(prop_mgt_h, rep_id,
                    init_id, init_from_rep_id, last_upd_seqno, last_upd_ts, 
                    init_st, st);
            
                if (GOOD_STATUS(init_st) && GOOD_STATUS(st)) 
                    rsdb_replica_set_prop_info(rep_id, 
                        rs_c_replica_prop_update, 
		        rs_c_replica_prop_unknown, init_id, st);
                break;

            case rs_c_replica_prop_update:
                if (BAD_STATUS(init_st)) {
                    /*** SVC_OPEN : Log a debug message 
                     * 
                     *   error("rs_replist2_master_init_replica_done");
                     *   error("received bad init status when in prop update state");
                     *
                     *  -- Sekhar
                     */
                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                   sec_s_rs_rep,
                                   svc_c_sev_error,
                                   sec_rgy_rep_pgmerr);
                    SET_STATUS(st, sec_rgy_rep_pgmerr);
                }
                break;

            case rs_c_replica_prop_bad_sw_rev:
            case rs_c_replica_prop_delete:
                SET_STATUS(st, sec_rgy_rep_bad_state);
                break; 

            default:
                SET_STATUS(st, sec_rgy_rep_pgmerr);
                break;
        }
    }
    else { 
        SET_STATUS(st, sec_rgy_rep_bad_init_id);
    }
}

/*
 * Mark a replica that init is done.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when the log is replayed 
 * at restart time.
 */
PUBLIC void rs_replist2_init_rep_done
#ifndef __STDC__
    ( rep_id, init_id, st ) 
    uuid_p_t                rep_id;     /* [in] */
    uuid_p_t                init_id;    /* [in] */
    error_status_t          *st;        /* [out] */
#else
  (
    uuid_p_t                rep_id,     /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
  )
#endif
{
    rsdb_replica_set_prop_info(rep_id, rs_c_replica_prop_update, 
        rs_c_replica_prop_unknown, init_id, st);
    if (BAD_STATUS(st)) return;
}

PUBLIC void rs_replist2_read
  (
    uuid_t              *marker,    /* [in, out] */
    unsigned32          max_ents,   /* [in] */
    unsigned32          *n_ents,    /* [out] */
    rs_replica_item_t   replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    error_status_t      *st
  )
{
    rs_replica_item_p_t         c_rp;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    unsigned32                  ln_ents;
    unsigned32                  i;
    error_status_t              lst;

    CLEAR_STATUS(st);
    *n_ents = 0;

    ln_ents = 0;

    for (i = 0, c_rp = &replist[0]; i < max_ents; i++, c_rp++) {
        read_replica(marker, c_rp, &prop_mgt_h, st);
        if (BAD_STATUS(st)) {
            if ( STATUS_EQUAL(st, sec_rgy_object_not_found) &&
                 (i > 0) ) {
                CLEAR_STATUS(st);
            }
            break;
        }

        ln_ents++;

        /* marker == uuid_nil means no more on list
         */
        if (uuid_is_nil(marker, &lst)) break;
    } 

    if (GOOD_STATUS(st)) {
        *n_ents = ln_ents;
    }
}

PUBLIC void rs_replist2_read_full
  (
    uuid_t                  *marker,    /* [in, out] */
    unsigned32              max_ents,   /* [in] */
    unsigned32              *n_ents,    /* [out] */
    rs_replica_item_full_t  replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    boolean32               compat,
    error_status_t          *st
  )
{
    rs_replica_item_t           client_rep_item,
                                *c_rp;
    rs_rep_prop_mgt_handle_t    prop_mgt_h; 
    rs_replica_prop_t           prev_prop_type;
    rs_replica_item_full_p_t    c_rfp;
    unsigned32                  ln_ents;
    unsigned32                  i;
    error_status_t              lst;

    CLEAR_STATUS(st);
    *n_ents = 0;

    ln_ents = 0;
    c_rp = &client_rep_item;

    for (i = 0; i < max_ents; i++) {
        read_replica(marker, c_rp, &prop_mgt_h, st);
        if (BAD_STATUS(st)) {
            if ( STATUS_EQUAL(st, sec_rgy_object_not_found) &&
                 (i > 0) ) {
                CLEAR_STATUS(st);
            }
            break;
        }

        c_rfp = &replist[i];

        c_rfp->rep_id = c_rp->rep_id;
        c_rfp->rep_name = c_rp->rep_name;
        c_rfp->master = c_rp->master;
        c_rfp->deleted = c_rp->deleted;
        c_rfp->rep_twrs = c_rp->rep_twrs;

        rs_m_replist_get_rep_prop_info(prop_mgt_h, &c_rp->rep_id,
            &c_rfp->prop_info, &prev_prop_type, st);
        if (BAD_STATUS(st)) break;

	/*
	 * This is a silly one.  The prop_type is an unsigned32 value, but the
	 * existing admin tools use the return value to index directly into an
	 * static array of printstrings, which means we can't just return 
	 * the new value (5) or old admin tools will crash.  
	 * We do the best we can but there's no way we can avoid misleading users 
         * of the old tools in certain circumstances.
	 */
	if (compat) {
	    if (c_rfp->prop_info.prop_type == rs_c_replica_prop_unknown) {
		c_rfp->prop_info.prop_type = rs_c_replica_prop_init;
	    } else if (c_rfp->prop_info.prop_type == rs_c_replica_prop_bad_sw_rev) {
		if (prev_prop_type == rs_c_replica_prop_unknown) {
		    c_rfp->prop_info.prop_type = rs_c_replica_prop_init;
		} else {
		    c_rfp->prop_info.prop_type = prev_prop_type;
		}
	    }
	}

        rs_m_replist_get_rep_comm_info(prop_mgt_h, &c_rp->rep_id,
            &c_rfp->comm_info, st);
        if (BAD_STATUS(st)) break;

        ln_ents++;

        /* marker == uuid_nil means no more on list
         */
        if (uuid_is_nil(marker, &lst)) break;
    } 

    if (GOOD_STATUS(st)) {
        *n_ents = ln_ents;
    }
}

PRIVATE void read_replica
  (
    uuid_t                      *marker,
    rs_replica_item_t           *c_rp,
    rs_rep_prop_mgt_handle_t    *prop_mgt_h, 
    error_status_t              *st
  )
{
    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_p_t  rep_buff,
                                c_rep_buff;
    unsigned32                  rep_bufflen;
    error_status_t              lst;

    rsdb_replica_lookup_by_ref(marker, &rp, &rep_buff, &rep_bufflen, st);
    if (BAD_STATUS(st)) return;

    c_rp->rep_id = rp->rep_id;
    c_rp->master = rs_master_info_current_master(&rp->rep_id, NULL);
    c_rp->deleted = (rp->prop_type == rs_c_replica_prop_delete) ?
                    true : false;

    *prop_mgt_h = rp->prop_mgt;

    if ( (c_rep_buff = (rs_replica_packed_buff_p_t) 
                            rpc_ss_allocate(rep_bufflen)) == NULL) {
        SET_STATUS(st, sec_s_no_memory);
        return;
    }

    memcpy((void *) c_rep_buff, (void *) rep_buff, rep_bufflen);

    rs_rep_util_rep_buff_unpack(c_rep_buff, &c_rp->rep_name, 
                                &c_rp->rep_twrs, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Search replist to see if rep_name matches a name
 * that is already on the replist.  It's ok to have
 * an entry which both rep_id and rep_name match.
 */
PRIVATE void chk_dupe_rep_name
  (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    error_status_t          *st         /* [out] */
  )
{
    uuid_t                      cursor;
    unsigned32                  nlen;
    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_buff_len;
    error_status_t              lst;

    nlen = strlen((char *) rep_name);

    /* if rep_id is already on the replist and
     * its entry name matches rep_name, rep_name 
     * has already been checked.
     */
    cursor = *rep_id;
    rsdb_replica_lookup_by_ref(&cursor, &rp, 
        &rep_buff, &rep_buff_len, st);

    if (GOOD_STATUS(st)) {
        if ((nlen == strlen((char *) rp->rep_name)) &&
            (memcmp(rp->rep_name, rep_name, nlen) == 0)) {
            return;
        }
    }

    uuid_create_nil(&cursor, st);
    if (BAD_STATUS(st)) return;

    while (true) {
        rsdb_replica_lookup_by_ref(&cursor, &rp, 
            &rep_buff, &rep_buff_len, st);
        if (BAD_STATUS(st)) return;

        if ((nlen == strlen((char *) rp->rep_name)) &&
            (memcmp(rp->rep_name, rep_name, nlen) == 0) ) {
            SET_STATUS(st, sec_rgy_rep_doppelganger);
            return;
        }

        /* check for end of replist
         */
        if (uuid_is_nil(&cursor, &lst))
            break;
    }
}

/*
 * Mark a replica as having been shutdown
 * due to a bad software rev..
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when the log is replayed 
 * at restart time.
 */
void rs_replist2_bad_sw_rev
  (
    uuid_p_t                rep_id,         /* [in] */
    rs_replica_prop_t       prev_prop_type, /* [in] */
    error_status_t          *st             /* [out] */
  )
{
    rsdb_replica_set_prop_info(rep_id, rs_c_replica_prop_bad_sw_rev, 
        prev_prop_type, NULL, st);
    if (BAD_STATUS(st)) return;
}

/*
 * Mark a replica as having the correct
 * compat sw rev.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when the log is replayed 
 * at restart time.
 */
void rs_replist2_sw_rev_ok
  (
    uuid_p_t                rep_id,    /* [in] */
    rs_replica_prop_t       prop_type, /* [in] */
    error_status_t          *st        /* [out] */
  )
{
    rsdb_replica_set_prop_info(rep_id, prop_type, 
        rs_c_replica_prop_unknown, NULL, st);
    if (BAD_STATUS(st)) return;
}








