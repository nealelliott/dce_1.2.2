/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_rep_bind.c,v $
 * Revision 1.1.7.2  1996/02/18  00:19:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:49  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:55:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/03  15:52 UTC  emartin
 * 	merge in some replication fixes
 * 
 * 	HP revision /main/emartin_mothra2/1  1995/01/30  22:20 UTC  emartin
 * 	nuke rs_rep_bind_auth_to_slave
 * 	[1995/12/08  17:21:21  root]
 * 
 * Revision 1.1.5.1  1994/06/02  21:23:10  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:39  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, AGAIN!
 * 	[1994/05/20  10:58:28  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, removed __STDC__ stuff.
 * 	[1994/05/19  19:59:58  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:45  mdf]
 * 
 * 	add function rs_rep_bind_init_auth_to_master
 * 	[1993/12/15  22:15:41  hanfei]
 * 
 * 	remove ifdef NAUTH from around rs_rep_bind_init_from_handle_na
 * 	[1993/12/11  18:48:36  hanfei]
 * 
 * 	bl6c: many new routines
 * 	[1992/12/11  21:52:21  emartin]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  18:54 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/04/20  15:18 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/12  14:37 UTC  emartin
 * 	do rs_rep_bind_init_auth_to_slave
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:04  mdf
 * 	merge
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:53  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.6  1993/02/10  20:35:21  emartin
 * 	bl6e: nuke  rs_rep_bind_to_master
 * 	[1993/02/04  23:27:55  emartin]
 * 
 * Revision 1.1.2.5  1993/01/26  23:11:22  emartin
 * 	fill rpc_handle with object uuid
 * 	some fixes to rs_rep_bind
 * 	do rs_rep_bind_init_from_handle_na
 * 	[1993/01/13  16:31:16  emartin]
 * 
 * Revision 1.1.2.4  1992/12/29  16:36:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:51  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/18  22:12:43  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:21  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:25:47  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:54:29  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */

/*    
 *      Registry server routines to bind to another replica
 */
    
#include <dce/nbase.h>
#include <dce/rpc.h>
#include <dce/rpcpvt.h>
#include <dce/rgynbase.h>
#include <dce/rsbase.h>
#include <dce/rplbase.h>

#include <rs_update.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>

#include <rs_ns.h>
#include <rs_rep_util.h>
#include <rs_rep_bind.h>
#include <rs_rep_auth.h>
#include <rs_login.h>
#include <rs_master.h>

#include <rsdb_replica.h>

#include <rrs_rep_remote.h>
#include <rrs_repmgr.h>

PUBLIC void rs_rep_bind_from_twrs
  (
    rs_replica_twr_vec_p_t  twrs,
    rpc_binding_handle_t    *ohandle,
    error_status_t          *st
  )
{
    unsigned32  i;

    *ohandle = NULL;
    SET_STATUS(st, sec_rgy_rep_bad_binding);

    for (i = 0; i < twrs->num_towers; i++) {
        if (twrs->towers[i] == NULL) continue;

        rpc_tower_to_binding(twrs->towers[i]->tower_octet_string,
            ohandle, st);
        if (GOOD_STATUS(st)) {
            break;
        }
    }

    /*
     * st will contain bad status if no binding
     * could be created from one of the towers
     */

    if (GOOD_STATUS(st)) {
        rpc_binding_set_object(*ohandle, 
            &rs_state.cell_sec_id, st);
    }
}

/*
 * Initialize a rep_binding.  Copy
 * towers with compatible protocols
 * into a tower vector for this 
 * rep_binding.
 * NB: the rep_handle is not filled by this
 * call.  It is filled by later calls to
 * the various flavors of rs_rep_bind...
 */
PUBLIC void rs_rep_bind_init_binding
  (
    rs_replica_twr_vec_p_t  rep_twrs,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    rep_binding->valid_handle = false;
    rep_binding->rep_handle = NULL;
    rep_binding->valid_auth_handle = false;
    rep_binding->auth_handle = NULL;
    rep_binding->twr_offset = -1;
    rep_binding->rep_twrs = NULL;

    if (rep_twrs != NULL) {
        rs_rep_util_copy_compat_twrs(
            rep_twrs, &rep_binding->rep_twrs, st);
        if (BAD_STATUS(st)) return;
    }
}

PUBLIC void rs_rep_bind_init_from_handle
  (
    rpc_binding_handle_t    rpc_handle,
    rs_replica_auth_p_t     auth_info,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    rep_binding->valid_handle = false;
    rep_binding->rep_handle = rpc_handle;
    rep_binding->valid_auth_handle = false;
    rep_binding->auth_handle = NULL;
    rep_binding->twr_offset = -1;
    rep_binding->rep_twrs = NULL;

    rpc_binding_set_object(rep_binding->rep_handle, 
        &rs_state.cell_sec_id, st);
    if (BAD_STATUS(st)) return;

    rs_rep_auth_get_handle(auth_info, &rep_binding->auth_handle, st);
    if (BAD_STATUS(st)) return;

    rs_rep_auth_binding_set_auth(rep_binding->auth_handle, 
        rep_binding->rep_handle, st);
    if (BAD_STATUS(st)) {
        rs_rep_auth_dispose_handle(&rep_binding->auth_handle);
        return;
    }

    rep_binding->valid_auth_handle = true;
    rep_binding->valid_handle = true;
}

/*
 * Gen up an unauthenticated rep_binding
 * from rpc_handle
 */
PUBLIC void rs_rep_bind_init_from_handle_na
  (
    rpc_binding_handle_t    rpc_handle,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    CLEAR_STATUS(st);

    rep_binding->valid_handle = false;
    rep_binding->rep_handle = rpc_handle;
    rep_binding->valid_auth_handle = false;
    rep_binding->auth_handle = NULL;
    rep_binding->twr_offset = -1;
    rep_binding->rep_twrs = NULL;

    rpc_binding_set_object(rep_binding->rep_handle, 
        &rs_state.cell_sec_id, st);
    if (BAD_STATUS(st)) return;

    rep_binding->valid_handle = true;
}

/*
 * Get an rpc binding handle from the "next" good tower
 */
PUBLIC void rs_rep_bind
  (
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    signed32            twr_offset,
                        i;
    boolean32           twr_found;
    error_status_t      lst;

    CLEAR_STATUS(st);

    /* Free current rpc binding handle if there is one
     */
    if (rep_binding->rep_handle != NULL) {
        rpc_binding_free (&rep_binding->rep_handle, &lst);
    }

    if (rep_binding->rep_twrs == NULL) {
        rep_binding->valid_handle = false;
        SET_STATUS(st, sec_rgy_rep_bad_binding);
        return;
    }

    if (rep_binding->twr_offset == -1) {
        twr_offset = 0;
    }
    else {
        twr_offset = rep_binding->twr_offset + 1;
    }

    twr_found = false;

    for (i = twr_offset; i < rep_binding->rep_twrs->num_towers; i++) {
        if (rep_binding->rep_twrs->towers[i] == NULL) continue;

        rpc_tower_to_binding(rep_binding->rep_twrs->towers[i]->tower_octet_string,
            &rep_binding->rep_handle, st);
        if (GOOD_STATUS(st)) {
            twr_found = true;
            rep_binding->twr_offset = i;
            break;
        }
    }

    if (!twr_found) {
        for (i = 0; i < twr_offset; i++) {
            if (rep_binding->rep_twrs->towers[i] == NULL) continue;

            rpc_tower_to_binding(rep_binding->rep_twrs->towers[i]->tower_octet_string,
                &rep_binding->rep_handle, st);
            if (GOOD_STATUS(st)) {
                twr_found = true;
                rep_binding->twr_offset = i;
                break;
            }
        }
    }

    if (twr_found) {
        rpc_binding_set_object(rep_binding->rep_handle, 
            &rs_state.cell_sec_id, st);
    }

    if (twr_found && GOOD_STATUS(st)) {
        rep_binding->valid_handle = true;
    }
    else {
        rep_binding->valid_handle = false;
        rep_binding->twr_offset = -1;
        if (GOOD_STATUS(st)) {
            SET_STATUS(st, sec_rgy_rep_bad_binding);
        }
    }
}

/*
 * Free storage associated with rep_binding
 */
PUBLIC void rs_rep_bind_free_binding
  (
    rs_rep_binding_t        *rep_binding
  )
{
    error_status_t  lst;

    if (rep_binding->auth_handle != NULL)
        rs_rep_auth_dispose_handle(&rep_binding->auth_handle);
    if (rep_binding->rep_twrs != NULL)
        rs_rep_util_free_twrs(&rep_binding->rep_twrs, &lst);
    if (rep_binding->rep_handle != NULL)
        rpc_binding_free(&rep_binding->rep_handle, &lst);
}

/*
 * Refresh or create an auth handle by pinging a replica for
 * its replica auth info and refreshing or creating  the 
 * virtual memory credential cache for that replica.  
 * Then associate this auth handle with the rpc handle.
 * We assume that the rpc_handle is ok.
 *
 * Since this routine makes rs_rep_auth calls, the
 * caller must hold the database read lock.
 */
PUBLIC void rs_rep_bind_auth_to_slave_rfrsh
  (
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    rs_replica_info_t     rep_info;
    rs_replica_auth_p_t   rep_auth_info;

    rep_binding->valid_handle = false;
    rep_binding->valid_auth_handle = false;

    rrs_rep_mgr_get_info_and_creds(rep_binding->rep_handle, 
        &rep_info, &rep_auth_info, st);
    if (BAD_STATUS(st)) return;

    if (rep_binding->auth_handle == NULL) {
        rs_rep_auth_get_handle(rep_auth_info, 
            &rep_binding->auth_handle, st);
    }
    else {
        rs_rep_auth_refresh_handle(rep_auth_info, 
            rep_binding->auth_handle, st);
    }

    rpc_ss_client_free(rep_auth_info);

    if (BAD_STATUS(st)) return;

    rs_rep_auth_binding_set_auth(rep_binding->auth_handle, 
        rep_binding->rep_handle, st);
    if (BAD_STATUS(st)) return;

    rep_binding->valid_handle = true;
    rep_binding->valid_auth_handle = true;
}

PUBLIC void rs_rep_bind_init_auth_to_slave
  (
    rs_replica_twr_vec_p_t  rep_twrs,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    error_status_t  lst;

    rs_rep_bind_init_binding(rep_twrs, rep_binding, st);
    if (BAD_STATUS(st)) return;

    rs_rep_bind(rep_binding, st);

    if (GOOD_STATUS(st)) {
        rs_rep_bind_auth_to_slave_rfrsh(rep_binding, st);
    }

    if (BAD_STATUS(st)) {
        rs_rep_bind_free_binding(rep_binding);
    }
}

/* 
 * Get an authenticated rpc handle for slave to master
 * operations.  The slave uses its host machine identity
 * when doing operations with the (surrogate) master;
 * the host login context should have already been
 * setup.
 */
PUBLIC void rs_rep_bind_auth_to_master
  (
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    rs_rep_bind(rep_binding, st);
    if (BAD_STATUS(st)) return;

    rs_login_host_set_auth_info(rep_binding->rep_handle, 
        (unsigned_char_p_t) SEC_RGY_SERVER_NAME, st);
    if (BAD_STATUS(st)) {
        rep_binding->valid_handle = false;
    }
}

/*
 * Init a binding to the master as remembered
 * in rs_master_info
 *
 * Must be invoked with replist read lock.
 */
PUBLIC void  rs_rep_bind_init_to_master
  (
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
{
    boolean32                   master_known;
    uuid_t                      master_id;
    rs_update_seqno_t           master_seqno;
    uuid_t                      cursor;

    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_p_t  rep_buff,
                                c_rep_buff;
    unsigned32                  rep_bufflen;

    rs_master_info_get(&master_known,
        &master_id, &master_seqno, st);
    if (!master_known) {
        SET_STATUS(st, sec_rgy_rep_master_not_found);
    }
    if (BAD_STATUS(st)) return;

    cursor = master_id;
    rsdb_replica_lookup_by_ref(&cursor, &rp, 
        &rep_buff, &rep_bufflen, st);
    if (BAD_STATUS(st)) return;

    rs_rep_bind_init_binding(rp->rep_twrs, rep_binding, st);
    if (BAD_STATUS(st)) return;
}

/*
 *  Bind new master with authentication to old master.
 */
PUBLIC  void rs_rep_bind_init_auth_to_master
  (
    rs_rep_binding_t    *rep_binding,   /* [out] */
    error_status_t      *st             /* [out] */
  )
{

    CLEAR_STATUS(st);

    rs_rep_bind_init_to_master(rep_binding, st);

    if (GOOD_STATUS(st)) {
        rs_rep_bind_auth_to_master(rep_binding, st);
    }
}



