/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_replist.c,v $
 * Revision 1.1.6.2  1996/02/18  00:16:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:59  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:39  root]
 * 
 * Revision 1.1.2.6  1993/01/26  23:02:48  emartin
 * 	delete_replica => del_replica
 * 	[1993/01/13  16:26:52  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  16:33:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:57  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:11:23  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:06  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:38:33  emartin
 * 	bl6c: add add_replica_init, add_all
 * 
 * Revision 1.1.2.3  1992/10/07  20:30:41  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:38  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:55:08  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:59:31  sekhar
 * 	Fixing history header.
 * 	[1992/08/31  18:59:12  sekhar]
 * 
 * 	First replication code drop.
 * 	[1992/08/31  17:50:45  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry server-to-server client calls to propagate 
 *      replist updates
 */
    
#include <dce/nbase.h>
#include <dce/rplbase.h>

#include <rs_repadm.h>
#include <rs_repmgr.h>
#include <rs_prop_replist.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>

#include <rsdb_replica.h>

#include <rrs_prop_replist.h>
#include <rrs_prop_util.h>


PRIVATE void rrs_prop_replist_add_replica_init
#ifndef __STDC__ 
    (rep_binding, rp, master_info, st)
    rs_rep_binding_t            *rep_binding;
    rs_replica_mgt_item_p_t     rp;
    rs_replica_master_info_t    *master_info;
    error_status_t              *st;
#else
  (
    rs_rep_binding_t            *rep_binding,
    rs_replica_mgt_item_p_t     rp,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
  )
#endif
{
    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {

        rrs_prop_replist_add_replica(
            rep_binding->rep_handle, &rp->rep_id, 
            rp->rep_name, rp->rep_twrs, master_info, 
            false, st);

        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st)

    } PROP_END_RETRY
}

/*  rrs_prop_replist_add_all
 *
 * Propagate all replist entries to an initializing slave.  
 */
PUBLIC void  rrs_prop_replist_add_all
#ifndef __STDC__ 
    (rep_binding, master_info, st)
    rs_rep_binding_t            *rep_binding;
    rs_replica_master_info_t    *master_info;
    error_status_t              *st;
#else
  (
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
  )
#endif
{
    uuid_t                      cursor;
    rs_replica_mgt_item_p_t     rp;
    rs_replica_packed_buff_t    *rep_buff;
    unsigned32                  rep_buff_len;
    error_status_t              lst;

    uuid_create_nil(&cursor, st);
    if (BAD_STATUS(st)) return;

    while (true) {
        rsdb_replica_lookup_by_ref(&cursor, &rp, 
            &rep_buff, &rep_buff_len, st);
        if (BAD_STATUS(st)) break;

        /* don't send a replica that is marked as deleted
         * on the master replist
         */
        if (rp->prop_type != rs_c_replica_prop_delete) {
            rrs_prop_replist_add_replica_init(
                rep_binding, rp, master_info, st);
            if (BAD_STATUS(st)) break;
        }

        /* check for end of replist
         */
        if (uuid_is_nil(&cursor, &lst))
            break;
    }
}


PUBLIC void rrs_prop_replist_add_replica 
#ifndef __STDC__
    (h, rep_id, rep_name, rep_twrs, master_info,
        propq_only, st)
    handle_t                    h;              /* [in] */
    uuid_p_t                    rep_id;         /* [in] */
    rs_replica_name_p_t         rep_name;       /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs;       /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in] */
    boolean32                   propq_only;     /* [in] */
    error_status_t              *st;
#else
  (
    handle_t                    h,              /* [in] */
    uuid_p_t                    rep_id,         /* [in] */
    rs_replica_name_p_t         rep_name,       /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs,       /* [in] */
    rs_replica_master_info_t    *master_info,   /* [in] */
    boolean32                   propq_only,     /* [in] */
    error_status_t              *st
  )
#endif
{
        (*rs_prop_replist_v1_0_c_epv.rs_prop_replist_add_replica)
            (h, rep_id, rep_name, rep_twrs, master_info, 
             propq_only, st);
}

PUBLIC void rrs_prop_replist_del_replica 
#ifndef __STDC__
    (h, rep_id, master_info, propq_only, st)
    handle_t                    h;              /* [in] */
    uuid_p_t                    rep_id;         /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in] */
    boolean32                   propq_only;     /* [in] */
    error_status_t              *st;
#else
  (
    handle_t                    h,     
    uuid_p_t                    rep_id,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,  
    error_status_t              *st
  )
#endif
{
        (*rs_prop_replist_v1_0_c_epv.rs_prop_replist_del_replica)
            (h, rep_id, master_info, propq_only, st);
}

