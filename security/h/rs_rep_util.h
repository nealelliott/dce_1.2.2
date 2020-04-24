/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_util.h,v $
 * Revision 1.1.12.1  1996/10/15  20:49:47  arvind
 * 	Add TOWER_VEC_FREE macro.
 * 	[1996/06/12  17:46 UTC  jrr  /main/HPDCE02/jrr_davis_4/3]
 *
 * 	Add code review changes.
 * 	[1996/06/11  21:19 UTC  jrr  /main/HPDCE02/jrr_davis_4/2]
 *
 * 	Add prototype for rs_rep_util_copy_twrs() and rs_rep_util_rep_item_copy().
 * 	[1996/06/11  17:19 UTC  jrr  /main/HPDCE02/jrr_davis_4/1]
 *
 * Revision 1.1.10.3  1996/02/18  22:59:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:05  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  17:27:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:02 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add prev_prop_type to rs_rep_util_rep_item_pack()
 * 	signature.
 * 	[1995/01/22  20:42 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.5.1  1994/07/15  14:59:35  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:48  mdf]
 * 
 * Revision 1.1.2.5  1992/12/29  13:25:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:27  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/04  19:32:37  ahop
 * 	Third replication drop: bl6a
 * 	do copy_compat_twrs, free_twrs, twrs_equal,
 * 	enable_master_write => enable_master_riw, decl enable_mkey_global_riw
 * 	[1992/11/03  18:14:31  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:27:15  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:18  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:40:17  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:08  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:47:17  sekhar]
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Replication utilities
 */


#ifndef rs_rep_util_h__included
#define rs_rep_util_h__included


#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <dce/aclbase.h>
#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_lock.h>
#include <rs_rep_pvt.h>


/* 
 * Local macros
 */
#define TOWER_VEC_FREE(routine, towerv) \
	do {	unsigned32 i; \
		for (i=0; i<towerv->num_towers; i++) \
			routine(towerv->towers[i]); \
		routine(towerv); \
	} while (0)


/* rs_rep_util_check_caller
 *
 * Check that the caller of a client operation
 * on the replist or a basic replica mgmt
 * operation has "requested_perms" to do
 * the operation.
 */
void rs_rep_util_check_caller (
    handle_t            handle,
    sec_acl_permset_t   requested_perms,
    error_status_t      *st
);

/*
 * Check that the state is ok for this event.
 * If it is ok, take the specified lock.
 */
void rs_rep_util_enable_event (
    rs_state_event_t           event,          /* [in] */
    lock_type_t                lock_type,      /* [in] */
    lock_mode_t                lock_mode,      /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    boolean32                  *lock_taken,    /* [out] */
    error_status_t             *st             /* [out] */
);

/* rs_rep_util_enable_global_event
 *
 * Check that the state is ok for this event.
 * If it is ok, take the global lock in the specified
 * mode.
 */
void  rs_rep_util_enable_global_event (
    rs_state_event_t           event,          /* [in] */
    lock_mode_t                lock_mode,      /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    boolean32                  *lock_taken,    /* [out] */
    error_status_t             *st             /* [out] */
);

/*
 * rs_rep_util_rep_item_pack
 *
 * fill fields of an rs_replica_mgt_item_t
 */
void rs_rep_util_rep_item_pack (
    uuid_p_t                    rep_id,
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    rs_replica_prop_t           prop_type,
    rs_replica_prop_t           prev_prop_type,
    uuid_p_t                    init_id,
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    rs_replica_mgt_item_t       *rp
);

/*
 * rs_rep_util_rep_item_copy
 *
 * Copy a src rep_item_t into a dest rep_item_t
 */
void rs_rep_util_rep_item_copy (
    uuid_p_t                    rep_id,
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    rs_replica_prop_t           prop_type,
    rs_replica_prop_t           prev_prop_type,
    uuid_p_t                    init_id,
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    rs_replica_mgt_item_t       **rp,
    error_status_t              *st
);

/*
 *  rs_rep_util_rep_buff_pack 
 *
 *  Pack a replica's name and tower set (ie. its
 *  variable length fields into a contiguous buffer
 *  with a prefix length of prefix_len.  prefix_len
 *  must be divisible by rs_c_align_size (eg. 4 ???) 
 *  so that rep_buff is naturally aligned.
 *
 *  Return pointers to the full buffer in "buff"
 *  (ie. prefix followed by packed replica info)
 *  and to the packed replica info in "rep_buff".
 *  Also return rep_buff's length in "rep_buff_len".
 *  The buffer's full length is prefix_len + 
 *  rep_buff_len.
 */
void rs_rep_util_rep_buff_pack (
    rs_replica_name_p_t         rep_name,
    rs_replica_twr_vec_p_t      rep_twrs,
    unsigned32                  prefix_len,
    void                        **buff,         /* [out] */
    rs_replica_packed_buff_t    **rep_buff,     /* [out] */
    unsigned32                  *rep_buff_len,  /* [out] */
    error_status_t              *st             /* [out] */
);

/*
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
void rs_rep_util_rep_buff_unpack ( 
    rs_replica_packed_buff_t    *rep_buff,  /* [in, out] */
    rs_replica_name_p_t         *rep_name,  /* [out] */
    rs_replica_twr_vec_p_t      *rep_twrs,  /* [out] */
    error_status_t              *st         /* [out] */
);

/*
 * rs_rep_util_rep_buff_free
 *
 * free a buffer that was allocated by 
 * rs_rep_util_rep_buff_pack
 */
void rs_rep_util_rep_buff_free (
    void                        **buff,         /* [in, out] */
    error_status_t              *st             /* [out] */
);

/*
 * Copy towers from a src twr_vec_t to a dest twr_vec_t
 */
void rs_rep_util_copy_twrs (
    rs_replica_twr_vec_p_t      src,    /* [in] */
    rs_replica_twr_vec_p_t      *dest,  /* [out] */
    error_status_t              *st     /* [out] */
);

/*
 * Copy towers in rep_twrs from which a binding handle can 
 * be derived (eg. towers with a locally supported protseq)
 * into compat_rep_twrs.  Keep tower offset in rep_twrs and
 * compat_rep_twrs the same by filling incompatible tower
 * slots in the compat_rep_twrs vector with NULL.
 */
void rs_rep_util_copy_compat_twrs (
    rs_replica_twr_vec_p_t      rep_twrs,           /* [in] */
    rs_replica_twr_vec_p_t      *compat_rep_twrs,   /* [out] */
    error_status_t              *st                 /* [out] */
);

/*
 * Free a tower vector which has been allocated by
 * rs_rep_util_copy_compat_twrs
 */
void rs_rep_util_free_twrs (
    rs_replica_twr_vec_p_t      *rep_twrs,          /* [in, out] */
    error_status_t              *st                 /* [out] */
);

/*
 * Return true if the towers in twr_vec1 and twr_vec2 are 
 * the same; otherwise return false.
 */
boolean32 rs_rep_util_twrs_equal (
    rs_replica_twr_vec_p_t      twr_vec1,           /* [in] */
    rs_replica_twr_vec_p_t      twr_vec2            /* [in] */
);


#endif /* rs_rep_util_h__included */

