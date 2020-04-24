/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_rep_init.h,v $
 * Revision 1.1.9.3  1996/02/18  22:59:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:00  marty]
 *
 * Revision 1.1.9.2  1995/12/13  16:25:43  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:01 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add prototype for rs_rep_init_master_sw_rev().
 * 	[1995/01/29  21:36 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.5.3  1994/06/02  21:09:46  mdf
 * 	Merged with changes from 1.1.5.2
 * 	[1994/06/02  21:09:36  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:10  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	add header for rs_rep_init_copy_all_to_new_master().
 * 	[1993/10/29  17:46:02  hanfei]
 * 
 * 	bl6c: rs_rep_init_recover_db, reinit_db now PRIVATE
 * 	some new routine decls
 * 	[1992/12/11  21:34:09  emartin]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:45  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.5.2  1994/06/02  20:21:55  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:10  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	add header for rs_rep_init_copy_all_to_new_master().
 * 	[1993/10/29  17:46:02  hanfei]
 * 
 * 	bl6c: rs_rep_init_recover_db, reinit_db now PRIVATE
 * 	some new routine decls
 * 	[1992/12/11  21:34:09  emartin]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:45  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.4  1992/12/29  13:25:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:15  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/18  22:10:44  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:44  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/11  21:34:09  emartin
 * 	bl6c: rs_rep_init_recover_db, reinit_db now PRIVATE
 * 	some new routine decls
 * 
 * Revision 1.1.2.2  1992/11/20  15:50:50  ahop
 * 	Remove client_ids from rs_rep_init_db signature
 * 	[1992/11/16  16:34:39  ahop]
 * 
 * 	Initial revision
 * 	[1992/11/10  23:03:00  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Replica initialization routines
 */

#ifndef RS_REP_INIT_H 
#define RS_REP_INIT_H 

#include <dce/nbase.h>
#include <dce/id_base.h>

#include <dce/rplbase.h>

/*
 * Copy this replica's database to another replica
 */
extern void rs_rep_init_copy_all_from_me (
    handle_t                    h,  
    rs_replica_master_info_p_t  copy_master_info, 
    rs_replica_auth_p_t         rep_auth_info,   
    rs_acct_key_transmit_t      *encryption_key,
    rs_update_seqno_t           *last_upd_seqno,
    sec_timeval_t               *last_upd_ts,
    error_status_t              *st
);

/*
 * Initialize this replica
 */
extern void rs_rep_init (
    uuid_p_t                    init_id,               /* [in] */
    unsigned32                  nreps,                 /* [in] */
    uuid_p_t                    init_from_rep_ids[],   /* [in, size_is(nreps)] */
    rs_replica_twr_vec_p_t      init_from_rep_twrs[],  /* [in, size_is(nreps)] */
    uuid_t                      *from_rep_id,          /* [out] */
    rs_update_seqno_t           *last_upd_seqno,       /* [out] */
    sec_timeval_t               *last_upd_ts,          /* [out] */
    error_status_t              *st
);

extern void rs_rep_init_chk_update_seqno (
    rs_replica_master_info_t    *master_info,
    boolean32                   *apply_update,
    error_status_t              *st
);

extern void rs_rep_init_copy_all_to_new_master (
    uuid_t                      *copy_from_rep_id,
    rs_update_seqno_t           *copy_from_upd_seqno,
    error_status_t              *st
);

extern void  rs_rep_init_chk_master_sw_rev  (
    unsigned32      master_compat_sw_rev, 
    error_status_t  *st
);

#endif /* RS_REP_INIT_H */
