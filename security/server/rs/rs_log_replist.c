/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_log_replist.c,v $
 * Revision 1.1.10.3  1996/02/18  00:18:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:00  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:52:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:06 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:49 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Fix "unsatisfied symbol" link errors.
 * 
 * 	[1995/01/23  01:38 UTC  greg  /main/greg_migration2/2]
 * 
 * 	add log and replay functions for
 * 	bad_sw_rev and sw_rev_ok operations.
 * 	[1995/01/21  20:38 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.3  1994/08/25  20:47:34  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:25:12  sekhar]
 * 
 * Revision 1.1.6.2  1994/08/04  16:13:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:26  mdf]
 * 
 * Revision 1.1.6.1  1994/06/02  21:25:19  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:39  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:54  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  21:15 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/03  20:40 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.4.2  1993/12/16  19:11:06  hanfei
 * 	fill in rs_log_replist_master, prop_master
 * 	[1993/12/08  04:28:57  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	[1992/10/06  17:00:53  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:11  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.7  1993/02/26  23:50:42  emartin
 * 	em_bl6f: log_init_replica - invoke rs_prop_tasks_wakeup
 * 	[1993/02/11  20:16:01  emartin]
 * 
 * Revision 1.1.2.6  1993/01/26  23:06:40  emartin
 * 	rrs_prop_replist_delete_replica => rrs_prop_replist_del_replica
 * 	[1993/01/13  16:28:38  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  16:35:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:06  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:11:59  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:49:19  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:34:34  emartin
 * 	bl6c: do delete_rep_done
 * 
 * Revision 1.1.2.3  1992/10/07  20:33:45  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:03  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:24:36  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:53:20  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */

/*    
 *      Registry Server - Replist Log operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_log_replist.c,v 1.1.10.3 1996/02/18 00:18:13 marty Exp $";
#endif


#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_rep_pvt.h>
#include <rs_rep_util.h>
#include <rs_replist2.h>

#include <rs_log.h>
#include <rs_log_replist.h>
#include <rrs_prop_replist.h>

#include <sec_svc.h>

/*
 * Local types
 */
typedef struct rs_log_replist_add_rep_t {
    rs_log_hdr_t                upd_hdr;
    uuid_t                      rep_id;
    uuid_t                      init_id;
    unsigned32                  rep_buffers_len;
    rs_replica_packed_buff_t    rep_buffers;
} rs_log_replist_add_rep_t;

typedef struct rs_log_replist_init_rep_t {
    rs_log_hdr_t                upd_hdr;
    uuid_t                      rep_id;
    uuid_t                      init_id;
} rs_log_replist_init_rep_t;

typedef struct rs_log_replist_delete_rep_t {
    rs_log_hdr_t                upd_hdr;
    uuid_t                      rep_id;
    boolean32                   force_delete;
} rs_log_replist_delete_rep_t;

typedef struct rs_log_replist_master_t {
    rs_log_hdr_t                upd_hdr;
    uuid_t                      master_id;
    rs_update_seqno_t           master_seqno;
} rs_log_replist_master_t;

typedef struct rs_log_replist_bad_sw_rev_t {
    rs_log_hdr_t                upd_hdr;
    uuid_t                      rep_id;
    rs_replica_prop_t           prev_prop_type;
} rs_log_replist_bad_sw_rev_t;

typedef struct rs_log_replist_sw_rev_ok_t {
    rs_log_hdr_t                upd_hdr;
    uuid_t                      rep_id;
    rs_replica_prop_t           prop_type;
} rs_log_replist_sw_rev_ok_t;

/*
 * PRIVATE local routines
 */
PRIVATE PROP_FUNCTION(rs_log_replist_prop_add_rep);
PRIVATE PROP_FUNCTION(rs_log_replist_prop_delete_rep);
PRIVATE PROP_FUNCTION(rs_log_replist_prop_master);

PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_add_rep);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_replace_rep);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_init_rep);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_init_rep_done);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_delete_rep);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_delete_rep_done);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_master);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_bad_sw_rev);
PRIVATE REPLAY_FUNCTION(rs_log_replist_replay_sw_rev_ok);

/*
* Define Module opcodes and EPV
*/
#define rs_log_replist_add_rep_op          0
#define rs_log_replist_replace_rep_op      1
#define rs_log_replist_init_rep_op         2
#define rs_log_replist_init_rep_done_op    3
#define rs_log_replist_delete_rep_op       4
#define rs_log_replist_delete_rep_done_op  5
#define rs_log_replist_master_op           6
#define rs_log_replist_bad_sw_rev_op       7
#define rs_log_replist_sw_rev_ok_op        8

#define rs_log_replist_last    9    /* Must equal the number of operations */

PRIVATE rs_log_ops_t rs_log_replist_epv[rs_log_replist_last] = {
    { GLOBAL_UPDATE, rs_log_replist_prop_add_rep,
      rs_log_replist_replay_add_rep, "add_rep" } ,
    { GLOBAL_UPDATE, rs_log_replist_prop_add_rep,
      rs_log_replist_replay_replace_rep, "replace_rep" } ,
    { LOCAL_UPDATE,  rs_log_prop_nop,
      rs_log_replist_replay_init_rep, "init_rep" } ,
    { LOCAL_UPDATE,  rs_log_prop_nop,
      rs_log_replist_replay_init_rep_done, "init_rep_done" } ,
    { GLOBAL_UPDATE, rs_log_replist_prop_delete_rep,
      rs_log_replist_replay_delete_rep, "delete_rep" } ,
    { LOCAL_UPDATE,  rs_log_prop_nop,
      rs_log_replist_replay_delete_rep_done, "delete_rep_done" } ,
    { GLOBAL_UPDATE, rs_log_replist_prop_master,
      rs_log_replist_replay_master, "new master" },
    { LOCAL_UPDATE,  rs_log_prop_nop,
      rs_log_replist_replay_bad_sw_rev, "bad_sw_rev" } ,
    { LOCAL_UPDATE,  rs_log_prop_nop,
      rs_log_replist_replay_sw_rev_ok, "sw_rev_ok" }
};

/*
 * Initialize the replist logging module
 */
PUBLIC void rs_log_replist_init
  (
    error_status_t   *st
  )
{
    int         num_ops;

    num_ops = rs_log_replist_last;
    rs_log_init_module(RS_LOG_MODULE_REPLIST, num_ops,
               rs_log_replist_epv, st);
}


PUBLIC void rs_log_replist_add_replica
  (
    uuid_p_t                    rep_id,
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    uuid_p_t                    init_id,
    rs_replica_master_info_t    *master_info
  )
{
    rs_log_replist_add_rep_t    log_rep_item,
                                *lrp;
    unsigned32                  prefix_len;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_bufflen;
    error_status_t              status;

    prefix_len = ((char *) &log_rep_item.rep_buffers) -
                    ((char *) &log_rep_item);
    rs_rep_util_rep_buff_pack(rep_name, rep_twrs, prefix_len,
        (void *) &lrp, &rep_buff, &rep_bufflen, &status);
    if (BAD_STATUS(&status))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_PACK_ENTRY_MSG, status);

    lrp->rep_buffers_len = rep_bufflen;

    lrp->rep_id = *rep_id;
    if (init_id != NULL) {
        lrp->init_id = *init_id;
    }
    else {
        uuid_create_nil(&lrp->init_id, &status);
    }

    rs_log_setup_log_header(&lrp->upd_hdr, prefix_len + rep_bufflen, 
        RS_LOG_MODULE_REPLIST, rs_log_replist_add_rep_op);

    if (master_info != NULL) {
        rs_log_write(&lrp->upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&lrp->upd_hdr, NULL, NULL);
    }

    rs_rep_util_rep_buff_free((void **) &lrp, &status);
} 

PRIVATE void rs_log_replist_prop_add_rep
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_replist_add_rep_t    *lrp;
    rs_replica_name_p_t         rep_name;
    rs_replica_twr_vec_p_t      rep_twrs;

    lrp = (rs_log_replist_add_rep_t *) info;

    rs_rep_util_rep_buff_unpack(&lrp->rep_buffers, 
        &rep_name, &rep_twrs, st);
    if (BAD_STATUS(st)) return;

    rrs_prop_replist_add_replica(handle, &lrp->rep_id, rep_name, rep_twrs,
                             master_info, propq_only, st);

}

PRIVATE void rs_log_replist_replay_add_rep
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_add_rep_t    *lrp;
    rs_replica_name_p_t         rep_name;
    rs_replica_twr_vec_p_t      rep_twrs;

    lrp = (rs_log_replist_add_rep_t *) info;

    rs_rep_util_rep_buff_unpack(&lrp->rep_buffers, 
        &rep_name, &rep_twrs, st);
    if (BAD_STATUS(st)) return;

    if (rs_state_master()) {
        rs_replist2_add_rep(&lrp->rep_id, rep_name, rep_twrs, 
                            &lrp->init_id, st);
    }
    else {
        rs_replist2_slave_add_rep(&lrp->rep_id, rep_name, rep_twrs, st);
    }                 
}

PUBLIC void rs_log_replist_replace_replica
  (
    uuid_p_t                    rep_id,
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    rs_replica_master_info_t    *master_info
  )
{
    rs_log_replist_add_rep_t    log_rep_item,
                                *lrp;
    unsigned32                  prefix_len;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_bufflen;
    error_status_t              status;

    prefix_len = ((char *) &log_rep_item.rep_buffers) -
                    ((char *) &log_rep_item);
    rs_rep_util_rep_buff_pack(rep_name, rep_twrs, prefix_len,
        (void *) &lrp, &rep_buff, &rep_bufflen, &status);
    if (BAD_STATUS(&status))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_PACK_ENTRY_MSG, status);

    lrp->rep_buffers_len = rep_bufflen;

    lrp->rep_id = *rep_id;

    rs_log_setup_log_header(&lrp->upd_hdr, prefix_len + rep_bufflen, 
        RS_LOG_MODULE_REPLIST, rs_log_replist_replace_rep_op);

    if (master_info != NULL) {
        rs_log_write(&lrp->upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&lrp->upd_hdr, NULL, NULL);
    }

    rs_rep_util_rep_buff_free((void **) &lrp, &status);
} 

PRIVATE void rs_log_replist_replay_replace_rep
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_add_rep_t    *lrp;
    rs_replica_name_p_t         rep_name;
    rs_replica_twr_vec_p_t      rep_twrs;

    lrp = (rs_log_replist_add_rep_t *) info;

    rs_rep_util_rep_buff_unpack(&lrp->rep_buffers, 
        &rep_name, &rep_twrs, st);
    if (BAD_STATUS(st)) return;

    if (rs_state_master()) {
        rs_replist2_replace_rep(&lrp->rep_id, rep_name, rep_twrs, st);
    }
    else {
        rs_replist2_slave_add_rep(&lrp->rep_id, rep_name, rep_twrs, st);
    }                 
}

PUBLIC void rs_log_replist_init_replica
  (
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id
  )
{
    rs_log_replist_init_rep_t   log_rep_item,
                                *lrp;
    rs_update_seqno_t           update_seqno;
    sec_timeval_t               update_ts;
    error_status_t              lst;

    lrp = &log_rep_item;

    lrp->rep_id = *rep_id;
    lrp->init_id = *init_id;
                                
    rs_log_setup_log_header(&lrp->upd_hdr, sizeof(log_rep_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_init_rep_op);

    rs_log_get_update_info(&update_seqno, &update_ts, &lst);

    rs_log_write(&lrp->upd_hdr, &update_seqno, &update_ts);

    /* 
     * This isn't a "global" update so
     * rs_log_write won't wakeup the prop tasks
     */
    rs_prop_tasks_wakeup();
}

PRIVATE void rs_log_replist_replay_init_rep
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_init_rep_t *lrp;

    CLEAR_STATUS(st);

    lrp = (rs_log_replist_init_rep_t *) info;

    if (rs_state_master()) {
        rs_replist2_init_rep(&lrp->rep_id, &lrp->init_id, st);
    }
}

PUBLIC void rs_log_replist_init_rep_done
  (
    uuid_p_t                    rep_id,
    uuid_p_t                    init_id
  )
{
    rs_log_replist_init_rep_t   log_rep_item,
                                *lrp;
    rs_update_seqno_t           update_seqno;
    sec_timeval_t               update_ts;
    error_status_t              lst;

    lrp = &log_rep_item;

    lrp->rep_id = *rep_id;
    lrp->init_id = *init_id;
                                
    rs_log_setup_log_header(&lrp->upd_hdr, sizeof(log_rep_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_init_rep_done_op);

    rs_log_get_update_info(&update_seqno, &update_ts, &lst);

    rs_log_write(&lrp->upd_hdr, &update_seqno, &update_ts);
}

PRIVATE void rs_log_replist_replay_init_rep_done
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_init_rep_t *lrp;

    CLEAR_STATUS(st);

    lrp = (rs_log_replist_init_rep_t *) info;

    if (rs_state_master()) {
        rs_replist2_init_rep_done(&lrp->rep_id, &lrp->init_id, st);
    }
}

PUBLIC void rs_log_replist_delete_replica
  (
    uuid_p_t                    rep_id,
    boolean32                   force_delete,
    rs_replica_master_info_t    *master_info
  )
{
    rs_log_replist_delete_rep_t    log_rep_item,
                                   *lrp;

    lrp = &log_rep_item;

    lrp->rep_id = *rep_id;
    lrp->force_delete = force_delete;

    rs_log_setup_log_header(&lrp->upd_hdr, sizeof(log_rep_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_delete_rep_op);

    if (master_info != NULL) {
        rs_log_write(&lrp->upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&lrp->upd_hdr, NULL, NULL);
    }
} 

PRIVATE void rs_log_replist_prop_delete_rep
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_replist_delete_rep_t *lrp;

    lrp = (rs_log_replist_delete_rep_t *) info;

    rrs_prop_replist_del_replica(handle, &lrp->rep_id, 
                             master_info, propq_only, st);
}

/*
 * Replay delete replica.  
 * 
 * The rep_id of the replica being deleted should never 
 * be this replica's uuid.  
 * The del_rep operation should never be logged by a 
 * replica which is being deleted. When a replica is 
 * deleted it just deletes its database and exits without
 * logging this update.
 */
PRIVATE void rs_log_replist_replay_delete_rep
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_delete_rep_t *lrp;
    error_status_t              lst;

    lrp = (rs_log_replist_delete_rep_t *) info;

    if (uuid_equal(&rs_state.rep_id, &lrp->rep_id, &lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       lst);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        return;
    }

    if (rs_state_master()) {
        rs_replist2_delete_rep(&lrp->rep_id, lrp->force_delete, st);
    }
    else {
        rs_replist2_slave_delete_rep(&lrp->rep_id, st);
    }                 
}

PUBLIC void rs_log_replist_delete_rep_done
  (
    uuid_p_t                    rep_id
  )
{
    rs_log_replist_delete_rep_t log_rep_item,
                                *lrp;
    rs_update_seqno_t           update_seqno;
    sec_timeval_t               update_ts;
    error_status_t              lst;

    lrp = &log_rep_item;

    lrp->rep_id = *rep_id;
                                
    rs_log_setup_log_header(&lrp->upd_hdr, sizeof(log_rep_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_delete_rep_done_op);

    rs_log_get_update_info(&update_seqno, &update_ts, &lst);

    rs_log_write(&lrp->upd_hdr, &update_seqno, &update_ts);
}

/*
 * Replay delete replica done
 *
 * Normally this update is replayed at the master
 * and indicates that the master has successfully
 * sent the delete replica udpte to the slave which
 * is being deleted.  If the master changed after 
 * this update was written to the log, the slave
 * may have already deleted this replica from its 
 * replist.
 * 
 * The rep_id of the replica being deleted should never 
 * be this replica's uuid.  
 * The del_rep operation should never be logged by a 
 * replica which is being deleted. When a replica is 
 * deleted it just deletes its database and exits without
 * logging this update.
 */
PRIVATE void rs_log_replist_replay_delete_rep_done
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_delete_rep_t *lrp;
    error_status_t              lst;

    lrp = (rs_log_replist_delete_rep_t *) info;

    if (uuid_equal(&rs_state.rep_id, &lrp->rep_id, &lst)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       lst);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        return;
    }

    /* same as a slave delete - just remove the replica from
     * the replist.
     */
    rs_replist2_slave_delete_rep(&lrp->rep_id, st);
}


PUBLIC void rs_log_replist_master
  (
    uuid_p_t                    master_id,
    rs_update_seqno_t           *master_seqno,
    rs_replica_master_info_t    *master_info
  )
{
    rs_log_replist_master_t     log_item,
                                *lp;

    lp = &log_item;

    lp->master_id = *master_id;
    lp->master_seqno = *master_seqno;

    rs_log_setup_log_header(&lp->upd_hdr, sizeof(log_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_master_op);

    if (master_info != NULL) {
        rs_log_write(&lp->upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&lp->upd_hdr, NULL, NULL);
    }
} 

PRIVATE void rs_log_replist_prop_master
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_replist_master_t     *lrp;

    rrs_rep_mgr_i_am_master(handle, master_info, st);
}

PRIVATE void rs_log_replist_replay_master
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    CLEAR_STATUS(st);
}

PUBLIC void rs_log_replist_bad_sw_rev
  (
    uuid_p_t                    rep_id,
    rs_replica_prop_t           *prev_prop_type
  )
{
    rs_log_replist_bad_sw_rev_t   log_item,
                                  *lrp;
    rs_update_seqno_t             update_seqno;
    sec_timeval_t                 update_ts;
    error_status_t                lst;

    lrp = &log_item;

    lrp->rep_id = *rep_id;
    lrp->prev_prop_type = *prev_prop_type;
                                
    rs_log_setup_log_header(&lrp->upd_hdr, sizeof(log_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_bad_sw_rev_op);

    rs_log_get_update_info(&update_seqno, &update_ts, &lst);

    rs_log_write(&lrp->upd_hdr, &update_seqno, &update_ts);

    /*
     * No need to wake up prop tasks, since a replica in the
     * "bad_sw_ver" state is handled by the "long incommunicado"
     * prop task, which is on a timer.
     */
}

PRIVATE void rs_log_replist_replay_bad_sw_rev
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_bad_sw_rev_t *lrp;

    CLEAR_STATUS(st);

    lrp = (rs_log_replist_bad_sw_rev_t *) info;

    if (rs_state_master()) {
        rs_replist2_bad_sw_rev(&lrp->rep_id, lrp->prev_prop_type, st);
    }
}

PUBLIC void rs_log_replist_sw_rev_ok
  (
    uuid_p_t                    rep_id,
    rs_replica_prop_t           *prop_type
  )
{
    rs_log_replist_sw_rev_ok_t    log_item,
                                  *lrp;
    rs_update_seqno_t             update_seqno;
    sec_timeval_t                 update_ts;
    error_status_t                lst;

    lrp = &log_item;

    lrp->rep_id = *rep_id;
    lrp->prop_type = *prop_type;
                                
    rs_log_setup_log_header(&lrp->upd_hdr, sizeof(log_item),
        RS_LOG_MODULE_REPLIST, rs_log_replist_sw_rev_ok_op);

    rs_log_get_update_info(&update_seqno, &update_ts, &lst);

    rs_log_write(&lrp->upd_hdr, &update_seqno, &update_ts);

    /* LOCAL update so we need to wake the prop tasks up here. */
    rs_prop_tasks_wakeup();
}

PRIVATE void rs_log_replist_replay_sw_rev_ok
  (
    rs_log_rec_t                *info,
    error_status_t              *st
  )
{
    rs_log_replist_sw_rev_ok_t *lrp;

    CLEAR_STATUS(st);

    lrp = (rs_log_replist_sw_rev_ok_t *) info;

    if (rs_state_master()) {
        rs_replist2_sw_rev_ok(&lrp->rep_id, lrp->prop_type, st);
    }
}
