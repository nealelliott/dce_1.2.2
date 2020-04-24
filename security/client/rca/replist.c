/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: replist.c,v $
 * Revision 1.1.9.3  1996/02/18  00:07:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:30  marty]
 *
 * Revision 1.1.9.2  1995/12/08  18:07:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/02  20:04 UTC  aha
 * 	CHFts15825: dced pe_site update thread does not rebind
 * 
 * 	HP revision /main/HPDCE02/aha_mothra8/1  1995/08/02  18:42 UTC  aha
 * 	CHFts15825: dced pe_site update thread doesn't rebind;
 * 	 add new APIs  sec_rgy_replist_read_any() and sec_rgy_replist_read_full_any()
 * 	 that can bind to any working replica.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:51 UTC  greg  /main/SEC_migrate_merge/2]
 * 
 * 	sec_bind_util_binding_resolve() --> sec_bind_util_resolve_binding().
 * 	[1995/05/03  21:55 UTC  greg  /main/SEC_migrate_merge/greg_sec_migrate2/2]
 * 
 * 	Move replication retry macros out into a common header file.
 * 	[1995/05/02  20:30 UTC  greg  /main/SEC_migrate_merge/greg_sec_migrate2/1]
 * 
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/20  21:03 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
 * 	[1995/02/11  22:12 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Use new 2.0 replist interface in all calls, even though only the
 * 	sematics of read_replist_full()  have changed.   This will allow us
 * 	to drop the old interface registrations in the server as soon as all
 * 	replicas are running at the 1.1.
 * 	[1995/02/02  21:12 UTC  greg  /main/SEC_migrate/greg_sec_migrate/1]
 * 
 * 	Add support for new replist interface.
 * 	[1995/01/30  05:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.5.1  1994/06/17  18:41:37  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:17  devsrc]
 * 
 * Revision 1.1.2.3  1993/03/02  22:27:17  burati
 * 	CR7400 Retry with reset handle on failure
 * 	[1993/03/02  22:14:50  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:50:47  burati
 * 	Initial version
 * 	[1993/01/14  23:15:33  burati]
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$ID: $";
#endif

#include <rca_pvt.h>
#include <rgymacro.h>
#include <replist.h>
#include <rs_replist.h>
#include <rs_replist_1_0.h>
#include <sec_bind_util.h>

#define COMPAT_REPLIST_OP(op, args, handle, ST)\
    if (sec_bind_util_resolve_binding((handle), rs_replist_v2_0_c_ifspec, NULL, (ST))) { \
        (*rs_replist_v2_0_c_epv.##op)##args;\
    } \
    if (STATUS_EQUAL((ST), rpc_s_unknown_if)\
    || STATUS_EQUAL((ST), ept_s_not_registered)) {\
        (*rs_replist_v1_0_c_epv.##op)##args;\
    }

#define Crs_replist_add_replica(CH, b, c, d, ST)\
    COMPAT_REPLIST_OP(rs_replist_add_replica, (HANDLE(CH), b, c, d, ST), HANDLE(CH), ST)

#define Crs_replist_replace_replica(CH, b, c, d, ST)\
    COMPAT_REPLIST_OP(rs_replist_replace_replica, (HANDLE(CH), b, c, d, ST), HANDLE(CH), ST)

#define Crs_replist_delete_replica(CH, b, c, ST)\
    COMPAT_REPLIST_OP(rs_replist_delete_replica, (HANDLE(CH), b, c, ST), HANDLE(CH), ST)

#define Crs_replist_read(CH, b, c, d, e, ST)\
    COMPAT_REPLIST_OP(rs_replist_read, (HANDLE(CH), b, c, d, e, ST), HANDLE(CH), ST)

#define Crs_replist_read_full(CH, b, c, d, e, ST)\
    COMPAT_REPLIST_OP(rs_replist_read_full, (HANDLE(CH), b, c, d, e, ST), HANDLE(CH), ST)

/*
 *  sec_rgy_replist_add_replica
 *
 *  add a replica to the replica list.
 *
 *  master-only operation.
 */
PUBLIC void sec_rgy_replist_add_replica
#ifndef __STDC__
    (context, rep_id, rep_name, rep_twrs, status)
    sec_rgy_handle_t        context;
    uuid_p_t                rep_id;
    rs_replica_name_p_t     rep_name;
    rs_replica_twr_vec_p_t  rep_twrs;
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    uuid_p_t                rep_id,
    rs_replica_name_p_t     rep_name,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *status
  )
#endif
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_replist_add_replica(context, rep_id, rep_name, rep_twrs, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  sec_rgy_replist_replace_replica
 *
 *  replace information about replica "rep_id" on the 
 *  replica list.  
 *
 *  master-only operation.
 */
PUBLIC void sec_rgy_replist_replace_replica
#ifndef __STDC__
    (context, rep_id, rep_name, rep_twrs, status)
    sec_rgy_handle_t        context;
    uuid_p_t                rep_id;
    rs_replica_name_p_t     rep_name;
    rs_replica_twr_vec_p_t  rep_twrs;
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    uuid_p_t                rep_id,
    rs_replica_name_p_t     rep_name,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *status
  )
#endif
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_replist_replace_replica(context, rep_id, rep_name, rep_twrs, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  sec_rgy_replist_delete_replica
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
PUBLIC void sec_rgy_replist_delete_replica
#ifndef __STDC__
    (context, rep_id, force_delete, status)
    sec_rgy_handle_t        context;
    uuid_p_t                rep_id;
    boolean32               force_delete;
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    uuid_p_t                rep_id,
    boolean32               force_delete,
    error_status_t          *status
  )
#endif
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_replist_delete_replica(context, rep_id, force_delete, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  sec_rgy_replist_read 
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
 */
PUBLIC void sec_rgy_replist_read
#ifndef __STDC__
    (context, marker, max_ents, n_ents, replist, status)
    sec_rgy_handle_t    context;
    uuid_t              *marker;
    unsigned32          max_ents;
    unsigned32          *n_ents;
    rs_replica_item_t   replist[];
    error_status_t      *status;
#else
  (
    sec_rgy_handle_t    context,
    uuid_t              *marker,
    unsigned32          max_ents,
    unsigned32          *n_ents,
    rs_replica_item_t   replist[],
    error_status_t      *status
  )
#endif
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_replist_read(context, marker, max_ents, n_ents, replist, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  sec_rgy_replist_read_any
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
 *  Unlike sec_rgy_replist_read(), bind to any working
 *  replica.
 */
PUBLIC void sec_rgy_replist_read_any
#ifndef __STDC__
    (context, marker, max_ents, n_ents, replist, status)
    sec_rgy_handle_t    context;
    uuid_t              *marker;
    unsigned32          max_ents;
    unsigned32          *n_ents;
    rs_replica_item_t   replist[];
    error_status_t      *status;
#else
  (
    sec_rgy_handle_t    context,
    uuid_t              *marker,
    unsigned32          max_ents,
    unsigned32          *n_ents,
    rs_replica_item_t   replist[],
    error_status_t      *status
  )
#endif
{
    SETUP_RETRY(rca_op_read, status) {
        Crs_replist_read(context, marker, max_ents, n_ents, replist, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    RETRY_END;
}

/*
 *  sec_rgy_replist_read_full
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
 */
PUBLIC void sec_rgy_replist_read_full
#ifndef __STDC__
    (context, marker, max_ents, n_ents, replist, status)
    sec_rgy_handle_t        context;
    uuid_t                  *marker;
    unsigned32              max_ents;
    unsigned32              *n_ents;
    rs_replica_item_full_t  replist[];
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    uuid_t                  *marker,
    unsigned32              max_ents,
    unsigned32              *n_ents,
    rs_replica_item_full_t  replist[],
    error_status_t          *status
  )
#endif
{
    REP_SETUP_RETRY(rca_op_read, status) {
        Crs_replist_read_full(context, marker, max_ents, n_ents, replist, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    REP_RETRY_END;
}

/*
 *  sec_rgy_replist_read_full_any
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
 *  Unlike sec_rgy_replist_read_full(), bind to any working
 *  replica.
 */
PUBLIC void sec_rgy_replist_read_full_any
#ifndef __STDC__
    (context, marker, max_ents, n_ents, replist, status)
    sec_rgy_handle_t        context;
    uuid_t                  *marker;
    unsigned32              max_ents;
    unsigned32              *n_ents;
    rs_replica_item_full_t  replist[];
    error_status_t          *status;
#else
  (
    sec_rgy_handle_t        context,
    uuid_t                  *marker,
    unsigned32              max_ents,
    unsigned32              *n_ents,
    rs_replica_item_full_t  replist[],
    error_status_t          *status
  )
#endif
{
    SETUP_RETRY(rca_op_read, status) {
        Crs_replist_read_full(context, marker, max_ents, n_ents, replist, status);
	NORMAL_CASE;
	RETRY_CASE;
    }
    RETRY_END;
}
