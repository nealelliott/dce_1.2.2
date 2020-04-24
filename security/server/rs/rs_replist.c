/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_replist.c,v $
 * Revision 1.1.10.3  1996/02/18  00:19:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:03  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:55:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:08 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add rs_replist manager EPVs.  We support
 * 	two majorly different versions of the
 * 	interface.
 * 
 * 	[1995/01/30  01:53 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Add rs_replist_read_full_compat() It will be used
 * 	in the mgr_epv registered with the old rs_replist
 * 	interface.
 * 	[1995/01/30  00:52 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.6.1  1994/07/15  15:02:15  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:21:58  mdf]
 * 
 * Revision 1.1.2.7  1993/03/02  20:31:04  burati
 * 	CR7393 rs_replist_read_full needs to initialize *n_ents to 0
 * 	[1993/03/02  19:48:31  burati]
 * 
 * Revision 1.1.2.6  1992/12/29  16:37:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:21  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:12:57  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:44  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:39:06  emartin
 * 	bl6c replication drop
 * 
 * Revision 1.1.2.4  1992/11/04  19:39:46  ahop
 * 	Third replication drop: bl6a
 * 	replist2 routines renamed, read_full - error if this isn't the master
 * 	[1992/11/04  19:08:05  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:36:55  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:12  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  17:26:48  burati
 * 	Second replication drop: bl5
 * 	Use BEGIN/END_MASTER_RIW, RIW_PROMOTE
 * 
 * Revision 1.1.2.2  1992/08/31  18:26:25  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:55:31  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry server replica list update and lookup operations
 */
    
#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_replist.c,v 1.1.10.3 1996/02/18 00:19:47 marty Exp $";
#endif

#include <dce/rplbase.h>
#include <rs_replist.h>
#include <rs_replist_1_0.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>

#include <rs_replist2.h>
#include <rs_rep_util.h>
#include <rs_log_replist.h>
#include <rs_audit_trail.h>

#include <dce/aclbase.h>
#include <rsdb_acl.h>

void  rs_replist_v1_0_read_full (
    handle_t                h,          /* [in] */
    uuid_t                  *marker,    /* [in, out] */
    unsigned32              max_ents,   /* [in] */
    unsigned32              *n_ents,    /* [out] */
    rs_replica_item_full_t  replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    error_status_t          *st
);

globaldef rs_replist_v1_0_epv_t rs_replist_v1_0_manager_epv = {
    rs_replist_add_replica,
    rs_replist_replace_replica,
    rs_replist_delete_replica,
    rs_replist_read,
    rs_replist_v1_0_read_full
};

globaldef rs_replist_v2_0_epv_t rs_replist_v2_0_manager_epv = {
    rs_replist_add_replica,
    rs_replist_replace_replica,
    rs_replist_delete_replica,
    rs_replist_read,
    rs_replist_read_full
};

/*
 *  rs_replist_add_replica
 *
 *  add a replica to the replica list.
 *
 *  master-only operation.
 */
void  rs_replist_add_replica
  (
    handle_t                h,          /* [in] */
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
  )
{
    uuid_t              init_id;
    unsigned_char_p_t   rep_id_str;
    error_status_t      lst;

    uuid_create(&init_id, st);
    if (BAD_STATUS(st)) return;

    rep_id_str = NULL;
    uuid_to_string(rep_id, &rep_id_str, &lst);

    BEGIN_MASTER_RIW(lock_replist, st) {

        READ_LOCK(lock_db) {
            rs_audit_trail_log(h, "Attempt add replica", 
                (char *) rep_id_str);
            rs_rep_util_check_caller(h, sec_acl_perm_insert, st);

            if (GOOD_STATUS(st)) {
                RIW_PROMOTE(lock_replist);
                rs_replist2_master_add_rep(
                    rep_id, rep_name, rep_twrs, &init_id, st);
    
                if (GOOD_STATUS(st)) { 
                    rs_audit_trail_log(h, "Add replica succeeded", 
                        (char *) rep_id_str);
                    rs_log_replist_add_replica(rep_id, rep_name, rep_twrs, 
                        &init_id, NULL);
                }
            }

        } END_READ_LOCK;

    } END_MASTER_RIW

    if (rep_id_str != NULL)
        rpc_string_free(&rep_id_str, &lst);
}

/*
 *  rs_replist_replace_replica
 *
 *  replace information about replica "rep_id" on the 
 *  replica list.  
 *
 *  master-only operation.
 */
void  rs_replist_replace_replica
  (
    handle_t                h,         /* [in] */
    uuid_p_t                rep_id,    /* [in] */
    rs_replica_name_p_t     rep_name,  /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,  /* [in] */
    error_status_t          *st
  )
{
    unsigned_char_p_t   rep_id_str;
    error_status_t      lst;

    rep_id_str = NULL;
    uuid_to_string(rep_id, &rep_id_str, &lst);

    BEGIN_MASTER_RIW(lock_replist, st) {

        /* Can't change master's info via client
         * interface
         */
        if (uuid_equal(&rs_state.rep_id, rep_id, &lst)) {
            SET_STATUS(st, sec_rgy_rep_master);
        }

        if (GOOD_STATUS(st)) {

            READ_LOCK(lock_db) {
                rs_audit_trail_log(h, "Attempt replace replica", 
                    (char *) rep_id_str);
                rs_rep_util_check_caller(h, sec_acl_perm_mgmt_info, st);

                if (GOOD_STATUS(st)) {
                    RIW_PROMOTE(lock_replist);
                    rs_replist2_master_replace_rep(
                        rep_id, rep_name, rep_twrs, st);
            
                    if (GOOD_STATUS(st)) { 
                        rs_audit_trail_log(h, "Replace replica succeeded", 
                                       (char *) rep_id_str);
                        rs_log_replist_replace_replica(rep_id, rep_name, 
                                       rep_twrs, NULL);
                    }
                }

            } END_READ_LOCK;
        }

    } END_MASTER_RIW

    if (rep_id_str != NULL)
        rpc_string_free(&rep_id_str, &lst);
}

/*
 *  rs_replist_delete_replica
 *
 *  delete the replica identified by "rep_id".
 *  If "force_delete" is false, send the delete
 *  to the replica identified by "rep_id" as
 *  well as the other replicas.
 *  If "force_delete" is true, do not send the
 *  delete to the replica identified by "rep_id";
 *  it has been killed off some other way.
 *
 *  the master may NOT be deleted with this operation
 *
 *  master-only operation.
 */
void  rs_replist_delete_replica
  (
    handle_t                h,             /* [in] */
    uuid_p_t                rep_id,        /* [in] */
    boolean32               force_delete,  /* [in] */
    error_status_t          *st
  )
{
    unsigned_char_p_t   rep_id_str;
    error_status_t      lst;

    rep_id_str = NULL;
    uuid_to_string(rep_id, &rep_id_str, &lst);

    BEGIN_MASTER_RIW(lock_replist, st) {

        READ_LOCK(lock_db) {
            rs_audit_trail_log(h, "Attempt delete replica", 
                  (char *) rep_id_str);
            rs_rep_util_check_caller(h, sec_acl_perm_delete, st);

            if (GOOD_STATUS(st)) {
                RIW_PROMOTE(lock_replist);
                rs_replist2_master_delete_rep(
                    rep_id, force_delete, st);
        
                if (GOOD_STATUS(st)) {
                    rs_audit_trail_log(h, "Delete replica succeeded", 
                              (char *) rep_id_str);
                    rs_log_replist_delete_replica(rep_id, force_delete, NULL);
                }
            }

        } END_READ_LOCK;

    } END_MASTER_RIW

    if (rep_id_str != NULL)
        rpc_string_free(&rep_id_str, &lst);
}

/*
 *  rs_replist_read 
 *
 *  read the replica list
 *
 *  To start reading at the beginning of the replica
 *  list, set marker to uuid_nil.
 *  To read information about a specific replica, set
 *  marker to its uuid and max_ents to 1.
 *
 *  The returned marker contains the uuid of the next
 *  replica on the list.  Marker contains uuid_nil
 *  when there are no more replicas on the list.
 *
 *  This call is unauthenticated.
 */
void  rs_replist_read
  (
    handle_t            h,          /* [in] */
    uuid_t              *marker,    /* [in, out] */
    unsigned32          max_ents,   /* [in] */
    unsigned32          *n_ents,    /* [out] */
    rs_replica_item_t   replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    error_status_t      *st
  )
{

    BEGIN_READ(lock_replist, st) {

        rs_replist2_read(marker, max_ents, n_ents, replist, st);
    
    } END_READ

}

/*
 *  rs_replist_read_full
 *
 *  read the replica list getting additional
 *  propagation information about each replica.
 *
 *  To start reading at the beginning of the replica
 *  list, set marker to uuid_nil.
 *  To read information about a specific replica, set
 *  marker to its uuid and max_ents to 1.
 *
 *  The returned marker contains the uuid of the next
 *  replica on the list.  Marker contains uuid_nil
 *  when there are no more replicas on the list.
 *
 *  This call is unauthenticated.
 *
 *  master-only operation.
 */
void  rs_replist_read_full
  (
    handle_t                h,          /* [in] */
    uuid_t                  *marker,    /* [in, out] */
    unsigned32              max_ents,   /* [in] */
    unsigned32              *n_ents,    /* [out] */
    rs_replica_item_full_t  replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    error_status_t          *st
  )
{
    boolean32  compat = false;

    *n_ents = 0;
    if (!rs_state_master()) {
        SET_STATUS(st, sec_rgy_rep_not_master);
        return;
    }

    BEGIN_READ(lock_replist, st) {

        rs_replist2_read_full(marker, max_ents, n_ents, 
			      replist, compat, st);
    
    } END_READ

}

    
/* Compat read_replist_full routine for compatibility with
 * pre-1.1 clients.
 */
void  rs_replist_v1_0_read_full
  (
    handle_t                h,          /* [in] */
    uuid_t                  *marker,    /* [in, out] */
    unsigned32              max_ents,   /* [in] */
    unsigned32              *n_ents,    /* [out] */
    rs_replica_item_full_t  replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    error_status_t          *st
  )
{
    boolean32  compat = true;
    *n_ents = 0;
    
    if (!rs_state_master()) {
        SET_STATUS(st, sec_rgy_rep_not_master);
        return;
    }

    BEGIN_READ(lock_replist, st) {

        rs_replist2_read_full(marker, max_ents, n_ents, 
				     replist, compat, st);
    
    } END_READ
}
