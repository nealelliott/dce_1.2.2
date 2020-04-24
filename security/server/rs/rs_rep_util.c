/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_util.c,v $
 * Revision 1.1.56.1  1996/10/15  20:55:35  arvind
 * 	Replace rs_rep_util_copy_twrs with a much simpler version.
 * 	[1996/06/12  17:47 UTC  jrr  /main/HPDCE02/jrr_davis_4/3]
 *
 * 	Add code review changes.
 * 	[1996/06/11  21:18 UTC  jrr  /main/HPDCE02/jrr_davis_4/2]
 *
 * 	Create rs_rep_util_rep_item_copy() and rs_rep_util_copy_twrs().
 * 	[1996/06/11  17:17 UTC  jrr  /main/HPDCE02/jrr_davis_4/1]
 *
 * Revision 1.1.53.3  1996/02/18  00:19:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:55  marty]
 * 
 * Revision 1.1.53.2  1995/12/08  17:55:37  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:08 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:48 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add prev_prop_type to rs_rep_util_rep_item_pack() scheme.
 * 	[1995/01/22  18:20 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.49.3  1994/08/04  16:13:57  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:18  mdf]
 * 
 * Revision 1.1.49.1  1994/06/02  21:23:22  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:52  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:56  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/2  1994/05/03  21:15 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/03  20:41 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.4.2  1993/10/29  17:13:03  hanfei
 * 	change master work : add rs_rep_util_ts_equal()
 * 	[1993/10/29  17:12:21  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	move rs_rep_util_cmp_seqno to rs_util_seqno_cmp
 * 	rs_rep_util_enable_master_write => rs_rep_util_enable_master_riw
 * 	do rs_rep_util_enable_mkey_global_riw
 * 	[1992/10/06  17:25:25  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:57  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.6  1993/01/26  23:12:36  emartin
 * 	support rs_bypass_auth
 * 	[1993/01/13  16:32:15  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  16:36:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:07  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/04  19:39:14  ahop
 * 	Third replication drop: bl6a
 * 	do copy_compat_twrs, free_twrs, twrs_equal
 * 	[1992/11/03  18:46:49  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:36:26  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:54  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:26:07  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:54:55  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Replication support utilities
 */
    
#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_rep_util.c,v 1.1.56.1 1996/10/15 20:55:35 arvind Exp $";
#endif

#include <dce/dce.h>
#include <rs_server.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_rep_pvt.h>
#include <rs_rep_util.h>
#include <rs_auth.h>
#include <rs_master.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>

#include <sec_svc.h>

/* 
 * Local macros
 */
#define ROUND_UP(ru_number, ru_boundary) \
    (   (( (ru_number) % (ru_boundary) ) != 0) ?  \
        ( (ru_number) + ((ru_boundary) - ( (ru_number) % (ru_boundary) ))) : \
        (ru_number) )


/*
 * Local data types
 */

/*
 *  Packed buffer of replica info
 *
 * NB: beginning of various data structs (eg. twr_offsets array, 
 * twr_vec_buff, each twr_t) should be quad word aligned.
 */

typedef struct {
    /* magic number to check that dealing with a
     * replica_packed_item
     */
    unsigned32                  cookie;
    unsigned32                  version;

    /* 
     * Size of this rs_replica_packed_item_t
     */
    unsigned32                  buffsize;           

    /*
     * Buffer sizes
     */
    unsigned32                  name_bufflen;
    unsigned32                  num_towers;

    /*
     * Variable length buffers
     *
     * unsigned char            name_buff[];        (size = name_bufflen)
     * 
     * Offsets from beginning of rs_replica_packed_item_t to twr_t buffers
     *
     * unsigned32               twr_offsets[];      (size = num_towers)
     * rs_replica_twr_vec_t     twr_vec_buff;       (tower vector)
     * twr_t                    towers[];        
     */

    unsigned_char_t              buff[1];
} rs_replica_packed_item_t, *rs_replica_packed_item_p_t;


/*
 * Local constants
 */

#define rs_c_replica_packed_buff_cookie     0x12340ace
#define rs_c_replica_packed_buff_version    1


/* rs_rep_util_check_caller
 *
 * Check that the caller of a client operation
 * on the replist or a basic replica mgmt
 * operation has "requested_perms" to do
 * the operation. 
 * Calling routine should have read lock_db.
 */
PUBLIC void rs_rep_util_check_caller
  (
    handle_t            handle,
    sec_acl_permset_t   requested_perms,
    error_status_t      *st
  )
{
    sec_acl_permset_t       parent_perms;
    sec_rgy_name_t          *replist_obj_name;
    uuid_t                  *replist_mgr_type;
    sec_rgy_foreign_id_t    dummy_ids;

    CLEAR_STATUS(st);

#ifdef RS_REP_NAUTH
    if (rs_bypass_auth) return;
#endif

    parent_perms = (sec_acl_permset_t)NULL;
    replist_obj_name = (sec_rgy_name_t *)SEC_REPLIST_OBJ_NAME;
    replist_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_replist]);

    rs_is_authorized(handle, requested_perms, parent_perms,
                            *replist_obj_name,
                            replist_mgr_type, false, &dummy_ids, st);
}    

/*
 * Invoke rs_state_check to see that replica is in correct
 * state for this event.  If this replica thinks it is
 * the master, return good status.  The post lock check
 * will check whether this is a duplicate master.
 */
PRIVATE void pre_lock_state_check
  (
    rs_state_event_t           event,          /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    boolean32                  lock_taken,     /* [out] */
    error_status_t             *st             /* [out] */
  )
{
    CLEAR_STATUS(st);

    rs_state_check(event, lock_taken, st);
    if (BAD_STATUS(st)) {
        if (STATUS_EQUAL(st, sec_rgy_rep_master) &&
            (master_info != NULL))
            CLEAR_STATUS(st);
    }
}

/*
 * Invoke rs_state_check to see that replica is in correct
 * state for this event.  If this replica thinks it is
 * the master and it shouldn't be for this event, check 
 * whether this is a duplicate master.  If it is a dupe 
 * master start task to go into dupe master state and 
 * process exit.
 */
PRIVATE void post_lock_state_check
  (
    rs_state_event_t           event,          /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    boolean32                  lock_taken,    /* [out] */
    error_status_t             *st             /* [out] */
  )
{
    CLEAR_STATUS(st);

    rs_state_check(event, lock_taken, st);
    if (BAD_STATUS(st)) {

        if (STATUS_EQUAL(st, sec_rgy_rep_master) &&
            (master_info != NULL) &&
            rs_master_info_later_master(
                &master_info->master_id, &master_info->master_seqno)) {
                rs_master_strt_obs_master_tsk();
        }       

    }
}

/* rs_rep_util_enable_event
 *
 * Check that the state is ok for this event.
 * If it is ok, take the specified lock.
 */
PUBLIC void rs_rep_util_enable_event
  (
    rs_state_event_t           event,          /* [in] */
    lock_type_t                lock_type,      /* [in] */
    lock_mode_t                lock_mode,      /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    boolean32                  *lock_taken,    /* [out] */
    error_status_t             *st             /* [out] */
  )
{
    CLEAR_STATUS(st);
    *lock_taken = false;

    pre_lock_state_check(event, master_info, *lock_taken, st);
    if (BAD_STATUS(st)) return;

    lock(lock_type, lock_mode);
    *lock_taken = true;

    post_lock_state_check(event, master_info, *lock_taken, st);
    if (BAD_STATUS(st)) {
        unlock(lock_type, lock_mode);
        *lock_taken = false;
        return;
    }
}

/* rs_rep_util_enable_global_event
 *
 * Check that the state is ok for this event.
 * If it is ok, take the global lock in the specified
 * mode.
 */
PUBLIC void  rs_rep_util_enable_global_event
  (
    rs_state_event_t           event,          /* [in] */
    lock_mode_t                lock_mode,      /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    boolean32                  *lock_taken,    /* [out] */
    error_status_t             *st             /* [out] */
  )
{
    CLEAR_STATUS(st);
    *lock_taken = false;

    pre_lock_state_check(event, master_info, *lock_taken, st);
    if (BAD_STATUS(st)) return;

    lock_global(lock_mode);
    *lock_taken = true;

    post_lock_state_check(event, master_info, *lock_taken, st);
    if (BAD_STATUS(st)) {
        unlock_global(lock_mode);
        *lock_taken = false;
        return;
    }
}

/*
 * rs_rep_util_rep_item_pack
 *
 * fill fields of an rs_replica_mgt_item_t
 */
PUBLIC void rs_rep_util_rep_item_pack
  (
    uuid_p_t                    rep_id,
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    rs_replica_prop_t           prop_type,
    rs_replica_prop_t           prev_prop_type,
    uuid_p_t                    init_id,
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    rs_replica_mgt_item_t       *rp
  )
{
    error_status_t  lst;

    rp->rep_id = *rep_id;
    rp->rep_name = rep_name;
    rp->rep_twrs = rep_twrs;
    rp->prop_type = prop_type;
    rp->prev_prop_type = prev_prop_type;
    if (init_id == NULL)
        uuid_create_nil(&rp->init_id, &lst);
    else
        rp->init_id = *init_id;
    rp->alloc_type = rs_e_rep_mgt_item_alloc_other;
    rp->prop_mgt = prop_mgt_h;
}

/*
 * rs_rep_util_rep_item_copy
 *
 * Allocate a new rs_replica_mgt_item_t and fill in
 * the fields.
 */
PUBLIC void rs_rep_util_rep_item_copy
  (
    uuid_p_t                    rep_id,
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    rs_replica_prop_t           prop_type,
    rs_replica_prop_t           prev_prop_type,
    uuid_p_t                    init_id,
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    rs_replica_mgt_item_t       **rep_item,
    error_status_t              *st
  )
{
    rs_replica_mgt_item_t       *rp;

    CLEAR_STATUS(st);

    rp = (rs_replica_mgt_item_t *)malloc(sizeof(rs_replica_mgt_item_t));
    if (rp == NULL) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        return;
    }

    rp->rep_id = *rep_id;
    rp->rep_name = (unsigned char *)dce_strdup(rep_name);
    if (rp->rep_name == NULL) {
        free(rp);
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        return;
    }

    rs_rep_util_copy_twrs(rep_twrs, &rp->rep_twrs, st);
    if (BAD_STATUS(st)) {
        free(rp->rep_name);
        free(rp);
        return;
    }
    
    rp->prop_type = prop_type;
    rp->prev_prop_type = prev_prop_type;
    if (init_id == NULL) {
        uuid_create_nil(&rp->init_id, st);
        if (BAD_STATUS(st)) {
            free(rp->rep_name);
            TOWER_VEC_FREE(free, rp->rep_twrs);
            free(rp);
            return;
        }
    }
    else {
        rp->init_id = *init_id;
    }
    rp->alloc_type = rs_e_rep_mgt_item_alloc_other;
    rp->prop_mgt = prop_mgt_h;
    
    *rep_item = rp;
}

/*
 *  rs_rep_util_rep_buff_pack 
 *
 *  pack a replica's name and tower set (ie. its
 *  variable length fields) into a contiguous buffer
 *  with a prefix length of prefix_len.  prefix_len
 *  must be divisible by rs_c_align_size (eg. 4 ???) 
 *  so that rep_buff is naturally aligned.
 *
 *  return pointers to the full buffer in "buff"
 *  (ie. prefix followed by packed replica info)
 *  and to the packed replica info in "rep_buff".
 *  Also return rep_buff's length in "rep_buff_len".
 *  The buffer's full length is prefix_len + 
 *  rep_buff_len.
 */
PUBLIC void rs_rep_util_rep_buff_pack 
  (
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    unsigned32                  prefix_len,
    void                        **buff,         /* [out] */
    rs_replica_packed_buff_t    **rep_buff,     /* [out] */
    unsigned32                  *rep_buff_len,  /* [out] */
    error_status_t              *st             /* [out] */
  )
{
    unsigned32                  packed_buff_len,
                                packed_buff_hlen;
    rs_replica_packed_item_t    packed_buff,
                                *p_bp;

    unsigned_char_p_t           bp,
                                begin_bp,
                                begin_p_bp,
                                end_p_bp;

    unsigned32                  name_blen;

    unsigned32                  num_towers;

    unsigned32                  twr_offs_blen;
    unsigned32                  *twr_offp;

    unsigned32                  twr_vec_blen;
    rs_replica_twr_vec_t        twr_vec,
                                *twr_vecp;

    unsigned32                  twr_hlen,
                                tlen,
                                twrs_blen;
    twr_t                       tower,
                                *twrp;
    unsigned32                  i;

    *buff = NULL;
    *rep_buff = NULL;

    if ((prefix_len % rs_c_align_size) != 0) {
         dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                        sec_s_rs_rep,
                        svc_c_sev_error,
                        sec_rgy_rep_pgmerr);
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        return;
    } 

    if ((rep_twrs == NULL) ||
        (rep_twrs->num_towers == 0)) {
         dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                        sec_s_rs_rep,
                        svc_c_sev_error,
                        sec_rgy_rep_bad_binding);
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    } 


    packed_buff_hlen = ((unsigned_char_p_t) packed_buff.buff) - 
                ((unsigned_char_p_t) &packed_buff);
    packed_buff_hlen = ROUND_UP(packed_buff_hlen, rs_c_align_size);

    if (rep_name == NULL) {
        name_blen = rs_c_align_size;
    }
    else {
        name_blen = strlen((char *) rep_name) + 1;
        name_blen = ROUND_UP(name_blen, rs_c_align_size);
    }

    num_towers = rep_twrs->num_towers;

    /* get length of buffer for twr_offsets array
     */
    twr_offs_blen = sizeof(*twr_offp) * num_towers;
    twr_offs_blen = ROUND_UP(twr_offs_blen, rs_c_align_size);

    /* get length of buffer for tower vector
     */
    twr_vec_blen = sizeof(twr_vec) + 
                    (sizeof(twr_vec.towers[0]) * (num_towers - 1));
    twr_vec_blen = ROUND_UP(twr_vec_blen, rs_c_align_size);


    /* get length of buffer for all the towers.
     * align each tower on "rs_c_align_size" word boundaries
     */
    twr_hlen = ((unsigned_char_p_t) tower.tower_octet_string) - 
                ((unsigned_char_p_t) &tower);

    twrs_blen = 0;
    for (i = 0; i < num_towers; i++) {
        tlen = twr_hlen + rep_twrs->towers[i]->tower_length;
        tlen = ROUND_UP(tlen, rs_c_align_size);
        twrs_blen += tlen;
    }

    /* get length of total replica packed item
     */
    packed_buff_len = packed_buff_hlen + name_blen + 
              twr_offs_blen + twr_vec_blen + twrs_blen;
                
    /* allocate replica packed item buffer and some
     * prefix space requested by caller
     */
    bp = (unsigned_char_t *) malloc(prefix_len + packed_buff_len);
    begin_bp = bp;                                   

    bp += prefix_len;
    begin_p_bp = bp;
    end_p_bp = bp + packed_buff_len;

    /* fill replica packed item header
     */
    p_bp = (rs_replica_packed_item_p_t) bp;
    p_bp->cookie = rs_c_replica_packed_buff_cookie;
    p_bp->version = rs_c_replica_packed_buff_version;
    p_bp->buffsize = packed_buff_len;
    p_bp->name_bufflen = name_blen;
    p_bp->num_towers = num_towers;

    bp += packed_buff_hlen;

    /* fill name buffer
     */
    if (rep_name != NULL) {
        strcpy((char *) bp, (char *) rep_name);
    }
    else {
        strcpy((char *) bp, "");
    }

    bp += name_blen;

    /* point to beginning of tower offsets
     */
    twr_offp = (unsigned32 *) bp;

    /* point to beginning of tower vector
     */
    twr_vecp = (rs_replica_twr_vec_p_t) 
                (bp + twr_offs_blen);
    twr_vecp->num_towers = num_towers;

    /* point to beginning of tower buffers
     */
    twrp = (twr_p_t) (bp + twr_offs_blen + twr_vec_blen);

    /* For each tower in rep_twrs,
     * copy it into the packed item buffer, and
     * save its offset in the packed item buffer in
     * the tower offsets array and save a pointer to 
     * the tower in the tower vector.  The tower offset
     * array and the tower vector are both in the
     * replica packed item buffer.
     */
    for (i = 0; i < num_towers; i++, twr_offp++) {
        /* again get length of this tower's buffer
         */
        tlen = twr_hlen + rep_twrs->towers[i]->tower_length;
        tlen = ROUND_UP(tlen, rs_c_align_size);

        if ( ( ((unsigned_char_p_t) twrp) + tlen - 1) >= end_p_bp) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rep,
                           svc_c_sev_error,
                           sec_rgy_rep_pgmerr);
            SET_STATUS(st, sec_rgy_rep_pgmerr);
            free(begin_bp);
            return;
        }

        *twr_offp = ((unsigned_char_p_t) twrp) - begin_p_bp;
        twr_vecp->towers[i] = twrp;

        twrp->tower_length = rep_twrs->towers[i]->tower_length;
        memcpy((char *) twrp->tower_octet_string, 
               (char *) rep_twrs->towers[i]->tower_octet_string,
               twrp->tower_length);

        twrp = (twr_p_t) ( ((unsigned_char_p_t) twrp) + tlen);
    }

    *buff = begin_bp;
    *rep_buff = (rs_replica_packed_buff_t *) begin_p_bp;
    *rep_buff_len = packed_buff_len;

    CLEAR_STATUS(st);
}

/*STUB
 *  rs_rep_util_rep_buff_unpack 
 *
 *  return pointers to the replica's
 *  name and tower vector which are
 *  in rep_buff.  
 *  Also fill in the tower vector so 
 *  its entries point to the twr_t's 
 *  in rep_buff.
 *
 *  used when copying a replica entry
 *  or when reading it from disk into
 *  memory.
 */

PUBLIC void rs_rep_util_rep_buff_unpack
  (
    rs_replica_packed_buff_t    *rep_buff,  /* [in, out] */
    rs_replica_name_p_t         *rep_name,  /* [out] */
    rs_replica_twr_vec_p_t      *rep_twrs,  /* [out] */
    error_status_t              *st         /* [out] */
  )
{
    rs_replica_packed_item_t    *p_bp;
    unsigned_char_p_t           bp,
                                begin_p_bp;
    unsigned32                  num_towers;
    unsigned32                  *twr_offp;
    unsigned32                  twr_offs_blen;
    rs_replica_twr_vec_t        *twr_vecp;
    unsigned32                  i;
    unsigned32                  packed_buff_hlen;

    p_bp =  (rs_replica_packed_item_t *) rep_buff;

    if ((p_bp->cookie != rs_c_replica_packed_buff_cookie) ||
        (p_bp->version != rs_c_replica_packed_buff_version)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_invalid_entry);
        SET_STATUS(st, sec_rgy_rep_invalid_entry);
        return;
    }

    begin_p_bp = (unsigned_char_p_t) p_bp;

    num_towers = p_bp->num_towers;

    packed_buff_hlen = ((unsigned_char_p_t)&p_bp->buff) -
                ((unsigned_char_p_t)p_bp);
    packed_buff_hlen = ROUND_UP(packed_buff_hlen, rs_c_align_size);

    bp = (unsigned_char_p_t)p_bp + packed_buff_hlen;

    /* point to beginning of name buffer
     */
    *rep_name = bp;

    bp = (unsigned_char_p_t)bp + p_bp->name_bufflen;

    /* point to beginning of tower offsets
     */
    twr_offp = (unsigned32 *) bp;
    twr_offs_blen = sizeof(*twr_offp) * num_towers;
    twr_offs_blen = ROUND_UP(twr_offs_blen, rs_c_align_size);

    /* point to beginning of tower vector
     */
    twr_vecp = (rs_replica_twr_vec_p_t)
                (bp + twr_offs_blen);

    /* fill in the tower vector.
     * each tower pointer = pointer to begin of packed buffer +
     *  offset to tower (saved in tower offset array)
     */
    twr_vecp->num_towers = num_towers;

    for (i = 0; i < num_towers; i++, twr_offp++) {
        twr_vecp->towers[i] = (twr_p_t) (begin_p_bp + (*twr_offp));
    }

    *rep_twrs = twr_vecp;

    CLEAR_STATUS(st);
}

/*
 * rs_rep_util_rep_buff_free
 *
 * free a buffer that was allocated by 
 * rs_rep_util_rep_buff_pack
 */
PUBLIC void rs_rep_util_rep_buff_free
  (
    void                        **buff,         /* [in, out] */
    error_status_t              *st             /* [out] */
  )
{
    free(*buff);

    *buff = NULL;
    CLEAR_STATUS(st);
}

/* rs_rep_util_copy_twrs
 *
 * Copy towers from a src twr_vec_t to a dest twr_vec_t
 */
PUBLIC void rs_rep_util_copy_twrs
  (
    rs_replica_twr_vec_p_t      src,    /* [in] */
    rs_replica_twr_vec_p_t      *dest,  /* [out] */
    error_status_t              *st     /* [out] */
  )
{
    unsigned32 i;
    rs_replica_twr_vec_p_t	new_towerv;

    CLEAR_STATUS(st);
    *dest = NULL;

    new_towerv = malloc(sizeof(*src) +
			sizeof(*src->towers) * (src->num_towers-1) );
    if (new_towerv == NULL) {
	SET_STATUS(st, sec_rgy_cant_allocate_memory);
	return;
    }
    new_towerv->num_towers = src->num_towers;

    for (i = 0; i < src->num_towers; i++) {
	twr_p_t ts = src->towers[i];
	twr_p_t td = malloc(sizeof(*ts) + ts->tower_length-1);
	if (td == NULL) {
	    new_towerv->num_towers = i;
	    TOWER_VEC_FREE(free, new_towerv);
	    SET_STATUS(st, sec_rgy_cant_allocate_memory);
	    return;
	}
	new_towerv->towers[i] = td;
	td->tower_length = ts->tower_length;
	memcpy(td->tower_octet_string,
			ts->tower_octet_string, ts->tower_length);
    }

    *dest = new_towerv;
}

/* rs_rep_util_copy_compat_twrs
 *
 * Copy towers in rep_twrs from which a binding handle can 
 * be derived (eg. towers with a locally supported protseq)
 * into compat_rep_twrs.  Keep tower offset in rep_twrs and
 * compat_rep_twrs the same by filling incompatible tower
 * slots in the compat_rep_twrs vector with NULL.
 */
PUBLIC void rs_rep_util_copy_compat_twrs
  (
    rs_replica_twr_vec_p_t      rep_twrs,           /* [in] */
    rs_replica_twr_vec_p_t      *compat_rep_twrs,   /* [out] */
    error_status_t              *st                 /* [out] */
  )
{
    unsigned32                  num_towers;

    rpc_binding_handle_t        binding_h;
    error_status_t              lst;

    unsigned32                  twr_vec_blen;
    rs_replica_twr_vec_t        twr_vec,
                                *tmp_twr_vecp,
                                *twr_vecp;

    unsigned32                  twr_hlen,
                                tlen,
                                twrs_blen;
    twr_t                       tower,
                                *twrp;
    unsigned32                  i;

    unsigned_char_p_t           bp,
                                end_bp;

    *compat_rep_twrs = NULL;

    num_towers = rep_twrs->num_towers;
    if (num_towers <= 0) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_bad_binding);
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    /* get length of buffer for tower vector
     */
    twr_vec_blen = sizeof(twr_vec) + 
                    (sizeof(twr_vec.towers[0]) * (num_towers - 1));
    twr_vec_blen = ROUND_UP(twr_vec_blen, rs_c_align_size);
    tmp_twr_vecp = (rs_replica_twr_vec_p_t) malloc(twr_vec_blen);

    /* get length of buffer for all the legit towers.
     * align each tower on "rs_c_align_size" word boundaries
     */
    twr_hlen = ((unsigned_char_p_t) tower.tower_octet_string) - 
                ((unsigned_char_p_t) &tower);

    twrs_blen = 0;
    for (i = 0; i < num_towers; i++) {
        rpc_tower_to_binding(
            rep_twrs->towers[i]->tower_octet_string, 
            &binding_h, st);
        if (BAD_STATUS(st)) {
            tmp_twr_vecp->towers[i] = NULL;
        }
        else {
            tmp_twr_vecp->towers[i] = rep_twrs->towers[i];

            rpc_binding_free(&binding_h, &lst);

            tlen = twr_hlen + rep_twrs->towers[i]->tower_length;
            tlen = ROUND_UP(tlen, rs_c_align_size);
            twrs_blen += tlen;
        }
    }

    if (twrs_blen == 0) {
        free(tmp_twr_vecp);
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rep,
                       svc_c_sev_error,
                       sec_rgy_rep_bad_binding);
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    bp = (unsigned_char_p_t) malloc(twr_vec_blen + twrs_blen);
    end_bp = bp + twr_vec_blen + twrs_blen;

    twr_vecp = (rs_replica_twr_vec_p_t) bp;
    bp += twr_vec_blen;
    twrp = (twr_p_t) bp;
                        
    twr_vecp->num_towers = num_towers;

    for (i = 0; i < num_towers; i++) {
        if (tmp_twr_vecp->towers[i] != NULL) {
            twr_vecp->towers[i] = twrp;

            /* again get length of this tower's buffer
             */
            tlen = twr_hlen + rep_twrs->towers[i]->tower_length;
            tlen = ROUND_UP(tlen, rs_c_align_size);

            if ( ( ((unsigned_char_p_t) twrp) + tlen - 1) >= end_bp) {
                /*** SVC_OPEN  : Turn this into debug statment 
                 *
                 *   error("rs_rep_util_copy_compat_twrs - write past end of buffer");
                 * 
                 */
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rep,
                               svc_c_sev_error,
                               sec_rgy_rep_pgmerr);
                SET_STATUS(st, sec_rgy_rep_pgmerr);
                free(twr_vecp);
                free(tmp_twr_vecp);
                return;
            }
    
            twrp->tower_length = rep_twrs->towers[i]->tower_length;
            memcpy((char *) twrp->tower_octet_string, 
                   (char *) rep_twrs->towers[i]->tower_octet_string,
                   twrp->tower_length);
    
            twrp = (twr_p_t) ( ((unsigned_char_p_t) twrp) + tlen);
        }
        else {
            twr_vecp->towers[i] = NULL;
        }
    }

    free(tmp_twr_vecp);
    *compat_rep_twrs = twr_vecp;
    CLEAR_STATUS(st);
}

/* rs_rep_util_free_twrs
 *
 * Free a tower vector which has been allocated by
 * rs_rep_util_copy_compat_twrs
 */
PUBLIC void rs_rep_util_free_twrs
  (
    rs_replica_twr_vec_p_t      *rep_twrs,          /* [in, out] */
    error_status_t              *st                 /* [out] */
  )
{
    free(*rep_twrs);

    *rep_twrs = NULL;
    CLEAR_STATUS(st);
} 

/* rs_rep_util_twrs_equal
 *
 * Return true if the towers in twr-vec1 and twr_vec2 are 
 * the same; otherwise return false.
 */
PUBLIC boolean32 rs_rep_util_twrs_equal
  (
    rs_replica_twr_vec_p_t      twr_vec1,           /* [in] */
    rs_replica_twr_vec_p_t      twr_vec2            /* [in] */
  )
{
    boolean32   match_found;
    unsigned32  i,
                j;

    if (twr_vec1->num_towers != twr_vec2->num_towers) {
        return false;
    }

    for (i = 0; i < twr_vec1->num_towers; i++) {
 
        match_found = false;

        for (j = 0; j < twr_vec2->num_towers; j++) {
            if ((twr_vec1->towers[i]->tower_length ==
                 twr_vec2->towers[j]->tower_length) &&
                (bcmp(twr_vec1->towers[i]->tower_octet_string,
                      twr_vec2->towers[j]->tower_octet_string,
                      twr_vec1->towers[i]->tower_length) == 0) ) { 
                match_found = true;
                break;
            }
        }

        if (!match_found) 
            return false;
    } 

    return true;
}


/* rs_rep_util_ts_equal
 *
 * Return true if the time stamps passed in are
 * the same; otherwise return false.
 */
PUBLIC boolean32 rs_rep_util_ts_equal
  (
    sec_timeval_t     ts1,
    sec_timeval_t     ts2
  )
{
    if ( ( ts1.sec == ts2.sec ) && ( ts1.usec == ts2.usec ) )
	return true;
    else
	return false;
}
