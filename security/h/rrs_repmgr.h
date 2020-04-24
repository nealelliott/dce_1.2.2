/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_repmgr.h,v $
 * Revision 1.1.8.2  1996/02/18  22:58:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:17  marty]
 *
 * Revision 1.1.8.1  1995/12/13  16:24:53  root
 * 	Submit
 * 	[1995/12/11  15:14:32  root]
 * 
 * Revision 1.1.6.1  1994/07/15  14:59:07  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:54  mdf]
 * 
 * Revision 1.1.3.6  1993/02/26  23:47:35  emartin
 * 	mem_bl6f: decl rrs_rep_mgr_stop_til_compat_sw
 * 	[1993/02/22  22:51:48  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:51  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:32  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:05:48  emartin
 * 	bl6c: arg changes for init_done, copy_all
 * 
 * Revision 1.1.3.3  1992/11/04  19:31:20  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:09:26  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:25:18  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:03  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:33:35  burati
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

#ifndef __rrs_repmgr_h__included__
#define __rrs_repmgr_h__included__

#include <dce/rsbase.h>                                        


/*
 *  rrs_rep_mgr_get_info_and_creds
 *
 *  get a replica's basic state information
 *  and credentials to authenticate to it
 */
void  rrs_rep_mgr_get_info_and_creds (
#ifdef __STDC__
    handle_t             h,
    rs_replica_info_t    *rep_info,
    rs_replica_auth_p_t  *rep_auth_info,
    error_status_t       *st
#endif
);

 
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
void  rrs_rep_mgr_init (
#ifdef __STDC__
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
#endif
);


/*
 *  rrs_rep_mgr_init_done
 *
 *  slave tells master that it is finished initializing
 *  itself from "from_rep_id"
 */
void  rrs_rep_mgr_init_done (
#ifdef __STDC__
    handle_t                    h,               /* [in] */
    uuid_p_t                    rep_id,          /* [in] */
    uuid_p_t                    init_id,         /* [in] */
    uuid_p_t                    from_rep_id,     /* [in] */
    rs_update_seqno_t           *last_upd_seqno, /* [in] */
    sec_timeval_t               *last_upd_ts,    /* [in] */
    error_status_t              *init_st,        /* [in] */
    error_status_t              *st
#endif
);


/*
 *  rrs_rep_mgr_i_am_slave
 *
 *  message sent from a slave to the master when 
 *  the slave restarts.  The slave sends the master 
 *  its current tower set and software compatability
 *  version.
 */
void  rrs_rep_mgr_i_am_slave (
#ifdef __STDC__
    handle_t                    h,              /* [in] */
    uuid_p_t                    rep_id,         /* [in] */
    unsigned32                  compat_sw_rev,  /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs,       /* [in] */
    error_status_t              *st
#endif
);


/*
 *  rrs_rep_mgr_i_am_master
 *
 *  New master tells a slave that it is now
 *  the master.
 *
 */
void  rrs_rep_mgr_i_am_master (
#ifdef __STDC__
    handle_t                    h,            /* [in] */
    rs_replica_master_info_p_t  master_info,  /* [in] */
    error_status_t              *st
#endif
);


/*
 *  rrs_rep_mgr_become_master
 *
 *  The master tells a slave to become the new master.
 *  The base_prop_seqno is the sequence number of
 *  the earliest update currently on the prop queue.
 */
void rrs_rep_mgr_become_master (
#ifdef __STDC__
    handle_t                    h,                 /* [in] */
    rs_update_seqno_t           base_prop_seqno,   /* [in] */
    rs_replica_master_info_p_t  old_master_info,   /* [in] */
    rs_replica_master_info_t    *new_master_info,  /* [out] */
    error_status_t              *st
#endif
);


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
void  rrs_rep_mgr_copy_all (
#ifdef __STDC__
    handle_t                    h,                 /* [in] */
    rs_replica_master_info_p_t  copy_master_info,  /* [in] */
    rs_replica_auth_p_t         rep_auth_info,     /* [in] */
    rs_acct_key_transmit_t      *encryption_key,   /* [in] */
    rs_update_seqno_t           *last_upd_seqno,   /* [out] */
    sec_timeval_t               *last_upd_ts,      /* [out] */
    error_status_t              *st
#endif
);


/*
 *  rrs_rep_mgr_copy_propq
 *
 *  request from a slave, which is becoming the master,
 *  to the old master to send the new master the
 *  propagation queue
 */
void rrs_rep_mgr_copy_propq (
#ifdef __STDC__
    handle_t                    h,    /* [in] */
    error_status_t              *st
#endif
); 

PUBLIC void rrs_rep_mgr_stop_til_compat_sw (
#ifdef __STDC__
    handle_t                    h,    /* [in] */
    unsigned32                  compat_sw_rev,
    rs_replica_master_info_p_t  master_info,
    error_status_t              *st
#endif
);

#endif  /* __rrs_repmgr_h__included__ */
