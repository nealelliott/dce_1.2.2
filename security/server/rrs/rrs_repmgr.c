/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_repmgr.c,v $
 * Revision 1.1.8.2  1996/02/18  00:17:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:04  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:48:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:43  root]
 * 
 * Revision 1.1.6.1  1994/06/02  21:26:59  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:21  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:46:51  mdf]
 * 
 * Revision 1.1.4.3  1993/12/16  18:27:15  hanfei
 * 	take out new_master_info argument for rs_rep_mgr_copy_propq
 * 	[1993/12/15  21:52:23  hanfei]
 * 
 * Revision 1.1.4.2  1993/11/03  18:38:19  hanfei
 * 	add master_info as argument to rs_rep_mgr_copy_propq.
 * 	[1993/11/03  18:37:32  hanfei]
 * 
 * 	em_bl6f: repmgr_v2 epv
 * 	[1993/02/24  22:17:47  emartin]
 * 
 * 	em_bl6f: do rrs_rep_mgr_stop_til_compat_sw
 * 	 the code doesn't use it yet
 * 	[1993/02/22  22:25:02  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:11  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:38  sekhar]
 * 
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:27:16  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:55  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:49:27  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:39:48  emartin
 * 	bl6c: replication updates
 * 
 * Revision 1.1.1.2  1992/10/06  15:57:48  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Management Client
 */
    
#include <rs_server.h>
#include <rs_repmgr.h>
#include <rrs_repmgr.h>



/*
 *  rrs_rep_mgr_get_info_and_creds
 *
 *  get a replica's basic state information
 *  and credentials to authenticate to it
 */
PUBLIC  void  rrs_rep_mgr_get_info_and_creds
    (
    handle_t             h,
    rs_replica_info_t    *rep_info,
    rs_replica_auth_p_t  *rep_auth_info,
    error_status_t       *st
    )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_get_info_and_creds)(h, rep_info, 
                                                               rep_auth_info, st);
}
 
/*
 *  rrs_rep_mgr_init 
 *
 *  master tells slave to initialize itself from
 *  one of the "init_from_reps".  "init_id" identifies
 *  the initialize event and prevents redundant
 *  initializations.
 *
 *  The slave returns the id of the replica it will 
 *  init from and the last update sequence number and 
 *  timestamp of the "init_from_rep". 
 */
PUBLIC  void  rrs_rep_mgr_init
  (
    handle_t                    h,                     /* [in] */
    uuid_p_t                    init_id,               /* [in] */
    unsigned32                  nreps,                 /* [in] */
    uuid_p_t                    init_from_rep_ids[],   /* [in, size_is(nreps)] */
    rs_replica_twr_vec_p_t      init_from_rep_twrs[],  /* [in, size_is(nreps)] */
    rs_replica_master_info_p_t  master_info,           /* [in] */
    uuid_t                      *from_rep_id,          /* [out] */
    rs_update_seqno_t           *last_upd_seqno,       /* [out] */
    sec_timeval_t               *last_upd_ts,          /* [out] */
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_init)(h, init_id, nreps,
            init_from_rep_ids, init_from_rep_twrs, master_info, 
            from_rep_id, last_upd_seqno, last_upd_ts, st);
}


/*
 *  rrs_rep_mgr_init_done
 *
 *  slave tells master that it is finished initializing
 *  itself from "from_rep_id"
 */
PUBLIC  void  rrs_rep_mgr_init_done
  (
    handle_t                    h,               /* [in] */
    uuid_p_t                    rep_id,          /* [in] */
    uuid_p_t                    init_id,         /* [in] */
    uuid_p_t                    from_rep_id,     /* [in] */
    rs_update_seqno_t           *last_upd_seqno, /* [in] */
    sec_timeval_t               *last_upd_ts,    /* [in] */
    error_status_t              *init_st,        /* [in] */
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_init_done)(h, rep_id, init_id,
            from_rep_id, last_upd_seqno, last_upd_ts, init_st, st);
}

/*
 *  rrs_rep_mgr_i_am_slave
 *
 *  message sent from a slave to the master when 
 *  the slave restarts.  The slave sends the master 
 *  its current tower set and software version.
 */
PUBLIC  void  rrs_rep_mgr_i_am_slave
  (
    handle_t                    h,         /* [in] */
    uuid_p_t                    rep_id,    /* [in] */
    unsigned32                  sw_rev,    /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs,  /* [in] */
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_i_am_slave)(h, rep_id,
            sw_rev, rep_twrs, st);
}

/*
 *  rrs_rep_mgr_i_am_master
 *
 *  New master tells a slave that it is now
 *  the master.
 *
 */
PUBLIC  void  rrs_rep_mgr_i_am_master
  (
    handle_t                    h,            /* [in] */
    rs_replica_master_info_p_t  master_info,  /* [in] */
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_i_am_master)(h, 
            master_info, st);
}


/*
 *  rrs_rep_mgr_become_master
 *
 *  The master tells a slave to become the new master.
 *  The base_prop_seqno is the sequence number of
 *  the earliest update currently on the prop queue.
 */
PUBLIC void rrs_rep_mgr_become_master
  (
    handle_t                    h,                 /* [in] */
    rs_update_seqno_t           base_prop_seqno,   /* [in] */
    rs_replica_master_info_p_t  old_master_info,   /* [in] */
    rs_replica_master_info_t    *new_master_info,  /* [out] */
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_become_master)(h, 
            base_prop_seqno, old_master_info, new_master_info, st);
}


/*
 *  rrs_rep_mgr_copy_all
 *
 *  A request sent from one replica to another asking 
 *  the replica to push its entire database to it.
 *
 *  The replica which is pushing its database 
 *  uses "copy_master_info" as the master_info
 *  which accompanies the updates it sends.
 *  "rep_auth_info" includes a session key which
 *  is used by the two replicas to authenticate 
 *  each other.   "encryption_key" is a key (pickled and
 *  encrypted with the session key) that B will use
 *  to encrypt the account auth_keys during propagation.
 */
PUBLIC  void  rrs_rep_mgr_copy_all
  (
    handle_t                    h,              
    rs_replica_master_info_p_t  copy_master_info,
    rs_replica_auth_p_t         rep_auth_info,   
    rs_acct_key_transmit_t      *encryption_key, 
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_copy_all)(h, 
            copy_master_info, rep_auth_info, encryption_key, 
            last_upd_seqno, last_upd_ts, st);
}



/*
 *  rrs_rep_mgr_copy_propq
 *
 *  request from a slave, which is becoming the master,
 *  to the old master to send the new master the
 *  propagation queue
 */
PUBLIC void rrs_rep_mgr_copy_propq
  (
    handle_t                    h,    /* [in] */
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_copy_propq)(h, st);
}

PUBLIC void rrs_rep_mgr_stop_til_compat_sw
  (
    handle_t                    h,    /* [in] */
    unsigned32                  compat_sw_rev,
    rs_replica_master_info_p_t  master_info,
    error_status_t              *st
  )
{
    (*rs_repmgr_v2_0_c_epv.rs_rep_mgr_stop_until_compat_sw)(h, 
        compat_sw_rev, master_info, st);
}


