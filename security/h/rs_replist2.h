/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_replist2.h,v $
 * Revision 1.1.9.3  1996/02/18  22:59:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:08  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:27:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:02 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add "compat" parameter to rs_replist2_read_full()
 * 	so we know whether we were called by an old client
 * 	with whom we must preserve compatibility.
 * 	[1995/01/30  00:52 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.5.3  1994/06/02  21:12:26  mdf
 * 	Merged with changes from 1.1.5.2
 * 	[1994/06/02  21:12:15  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:16  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	add decl for rs_replist2_purge_deleted_reps
 * 	[1993/12/13  21:50:50  hanfei]
 * 
 * 	bl6c: some new routines
 * 	[1992/12/11  21:35:05  emartin]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:49  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.5.2  1994/06/02  20:22:01  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:16  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	add decl for rs_replist2_purge_deleted_reps
 * 	[1993/12/13  21:50:50  hanfei]
 * 
 * 	bl6c: some new routines
 * 	[1992/12/11  21:35:05  emartin]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:49  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.5  1992/12/29  13:25:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:35  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:51  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:58  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/11  21:35:05  emartin
 * 	bl6c: some new routines
 * 
 * Revision 1.1.2.3  1992/11/04  19:32:47  ahop
 * 	Third replication drop: bl6a
 * 	change names of some routines
 * 	[1992/11/03  18:16:01  ahop]
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:17  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:47:23  sekhar]
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Replica list update and read
 *          "worker" routines
 */


#ifndef rs_replist2_h__included
#define rs_replist2_h__included


#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_base.h>
#include <rs_rep_pvt.h>

/*
 * Initial add replica operation which is
 * done at the master site
 */
void rs_replist2_master_add_rep (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
);

/*
 * More primitive add replica operation done
 * when the master replays its log at restart time
 */
void rs_replist2_add_rep (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
);

/* 
 * Add replica operation done at a slave when
 * an add replica update is propagated to
 * a slave or when a slave replays its log
 * at restart time
 */
void rs_replist2_slave_add_rep (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
);

/*
 * Initial replace replica operation which is
 * done at the master site
 */
void rs_replist2_master_replace_rep (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
);

/*
 * More primitive replace replica operation
 * which is done when the master replays it
 * log at restart time or when the master
 * changes its own entry in the replica list.
 */
void rs_replist2_replace_rep (
    uuid_p_t                rep_id,     /* [in] */
    rs_replica_name_p_t     rep_name,   /* [in] */
    rs_replica_twr_vec_p_t  rep_twrs,   /* [in] */
    error_status_t          *st
);

/*
 * Initial delete replica operation which is
 * done at the master site
 */
void rs_replist2_master_delete_rep (
    uuid_p_t                rep_id,        /* [in] */
    boolean32               force_delete,  /* [in] */
    error_status_t          *st            /* [out] */
);

/*
 * More primitive delete replica operation
 * which is done when the master replays its
 * log at restart time.
 */
void rs_replist2_delete_rep (
    uuid_p_t                rep_id,        /* [in] */
    boolean32               force_delete,  /* [in] */
    error_status_t          *st            /* [out] */
);

/* 
 * Delete replica operation done at a slave when
 * a delete replica update is propagated to
 * a slave or when a slave replays its log
 * at restart time
 */
void rs_replist2_slave_delete_rep (
    uuid_p_t                rep_id,        /* [in] */
    error_status_t          *st            /* [out] */
);

/* Remove replicas whose prop_type is deleted
 * from the replist.
 *
 * Invoked when the master becomes a slave and when
 * a slave boots.
 */
void rs_replist2_purge_deleted_reps (
    error_status_t          *st            /* [out] */
);

/*
 * Mark a replica for initialization
 */
void rs_replist2_master_init_rep (
    uuid_p_t                rep_id,     /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
);

/*
 * Replay mark a replica for initialization
 */
void rs_replist2_init_rep (
    uuid_p_t                rep_id,     /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
);

/*
 * Mark a replica for updates.
 */
void rs_replist2_master_init_rep_don (
    uuid_p_t            rep_id,
    uuid_p_t            init_id,
    uuid_p_t            init_from_rep_id,
    rs_update_seqno_t   *last_upd_seqno,
    sec_timeval_t       *last_upd_ts,
    error_status_t      *init_st,
    error_status_t      *st
);

/*
 * Replay mark a replica for updates
 */
void rs_replist2_init_rep_done (
    uuid_p_t                rep_id,     /* [in] */
    uuid_p_t                init_id,    /* [in] */
    error_status_t          *st         /* [out] */
);

void rs_replist2_read (
    uuid_t              *marker,    /* [in, out] */
    unsigned32          max_ents,   /* [in] */
    unsigned32          *n_ents,    /* [out] */
    rs_replica_item_t   replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    error_status_t      *st
);

void rs_replist2_read_full (
    uuid_t                  *marker,    /* [in, out] */
    unsigned32              max_ents,   /* [in] */
    unsigned32              *n_ents,    /* [out] */
    rs_replica_item_full_t  replist[],  /* [out, length_is(*n_ents), size_is(max_ents)] */
    boolean32               compat,
    error_status_t          *st
);

/*
 * Mark a replica as having been shutdown
 * due to a bad software rev..
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when the log is replayed 
 * at restart time.
 */
void rs_replist2_bad_sw_rev (
    uuid_p_t                rep_id,         /* [in] */
    rs_replica_prop_t       prev_prop_type, /* [in] */
    error_status_t          *st             /* [out] */
);

/*
 * Mark a replica as having the correct
 * compat sw rev.
 *
 * Must be invoked with replist write lock.
 *
 * Invoked at master when the log is replayed 
 * at restart time.
 */
void rs_replist2_sw_rev_ok (
    uuid_p_t                rep_id,    /* [in] */
    rs_replica_prop_t       prop_type, /* [in] */
    error_status_t          *st        /* [out] */
);

#endif

