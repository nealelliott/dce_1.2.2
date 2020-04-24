/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_replica_pvt.h,v $
 * Revision 1.1.6.2  1996/02/18  22:59:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:07  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:27:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:30  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:59:37  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:51  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  13:25:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_replica_pvt.h V=10 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - Replica management
 * 
 */

#ifndef rs_replica_pvt_h__included
#define rs_replica_pvt_h__included
 
#include <pbase.h>
 
#include <dce/rplbase.h>
#include <dce/rgynbase.h>

#define TASK_STACK_SIZE     64000
#define TASK_PRIORITY       1           /* lowest priority, same as dist. task */


/* defined in rs_replica2.c */
extern ec2_$eventcount_t shut_prop_rec;
extern ec2_$ptr_t        shut_prop_ptr[1];
extern long              shut_prop_val[1];


/*  Interfaces currently defined in rs_replica3.c */

void rep_copy_to_me (
#ifdef __STDC__
    uuid_t         *from_id,
    rs_replica_t   *from_addr,
    sec_timeval_t      *update_ts,
    error_status_t  *st
#endif
);

void rep_svr_addrep(
#ifdef __STDC__
    uuid_t          *rs_id,
    long            num_addrs,
    rs_addresses_t  addrs,
    sec_timeval_t       update_ts,
    error_status_t  *st
#endif
);

void rep_initrep(
#ifdef __STDC__
    uuid_t          *rs_id,
    error_status_t  *st
#endif
);

void rep_replay_initrep(
#ifdef __STDC__
    uuid_t          *rs_id,
    sec_timeval_t       ts,
    error_status_t  *st
#endif
);

void rep_replay_initrep_done(
#ifdef __STDC__
    uuid_t          *rs_id,
    error_status_t  *st
#endif
);

void rep_svr_initrep(
#ifdef __STDC__
    long            num_addrs,
    rs_addresses_t  addrs,
    rs_ids_t        ids,
    sec_timeval_t       *(update_ts),
    error_status_t  *st
#endif
);

/* XXX reprep is in rsdb_replica.c .  This is probably where this should go too */
rep_replay_reprep(
#ifdef __STDC__
    uuid_t          *rs_id,
    long            num_addr,
    rs_addresses_t  addrs,
    error_status_t   *st
#endif
);

void rep_delrep(
#ifdef __STDC__
    uuid_t         *rs_id,
    error_status_t  *st
#endif
);

void rep_purgerep(
#ifdef __STDC__
    uuid_t         *rs_id,
    error_status_t  *st
#endif
);

void rep_listrep(
#ifdef __STDC__
    uuid_t             *marker,
    long               max_ents,
    long               *n_ents,
    rs_replist_t       replist,
    error_status_t      *st
#endif
);

void rep_listrep_full(
#ifdef __STDC__
    uuid_t             *marker,
    long               max_ents,
    long               *n_ents,
    rs_replist_full_t  replist,    /* List o' rsdb_replica_item_t */
    error_status_t      *st
#endif
);

void rep_copy_from_me(
#ifdef __STDC__
    rs_replica_t  *(to_rep),
    sec_timeval_t     *(copy_master_ts),
    error_status_t *st
#endif
);



/*  Interfaces currently defined in rs_replica_mc.c */
void rep_stop_prop_tasks(
#ifdef __STDC__
    void
#endif
);

void rep_change_master(
#ifdef __STDC__
    uuid_t          *new_master_id,
    error_status_t  *st
#endif
);

void rs_check_master(
#ifdef __STDC__
    rs_replica_master_info_t   *master_info,
    error_status_t              *st
#endif
);

void rep_svr_become_master(
#ifdef __STDC__
    handle_t      handle,
    sec_timeval_t     base_prop_ts,
    boolean       replist_readonly,
    error_status_t *st
#endif
);

void rep_become_master (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rep_become_slave(
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rep_restore_master(
#ifdef __STDC__
    error_status_t   *st
#endif
);

/*      Declarations of procedures defined in rs_replica2.c */
void rep_initialize(
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rep_start_task(
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rep_register(
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rep_unregister(
#ifdef __STDC__
    void
#endif
);

void rep_read_master_replist(
#ifdef __STDC__
    error_status_t   *st
#endif
);


void rep_find_earliest_rep(
#ifdef __STDC__
    sec_timeval_t   *min_ts
#endif
);


boolean rep_many_reps(
#ifdef __STDC__
    void
#endif
);


void rep_bring_up_to_date(
#ifdef __STDC__
    uuid_t          *rs_id,
    error_status_t  *st
#endif
);
/*      Declarations of procedures defined in rs_propagate.c */

void rep_wakeup_prop_task(
#ifdef __STDC__
    void
#endif
);

void wait_for_prop_shutdown (
#ifdef __STDC__
    void
#endif
);

void prop_task_is_shutdown (
#ifdef __STDC__
    void
#endif
);


void rep_propagate_task(
#ifdef __STDC__
    char    *argv[],
    int     argc
#endif
);

void ZombieMgr_task(
#ifdef __STDC__
    char    *argv[],
    int     argc
#endif
);

#endif rs_replica_pvt_h__included

