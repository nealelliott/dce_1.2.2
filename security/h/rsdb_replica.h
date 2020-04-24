/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_replica.h,v $
 * Revision 1.1.9.3  1996/02/18  23:00:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:01  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:28:27  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:03 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	prev_prop_type doesn't need to be a pointer in
 * 	rsdb_replica_set_prop_info().
 * 	[1995/01/29  14:31 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.5.1  1994/07/15  14:59:53  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:09  mdf]
 * 
 * Revision 1.1.2.4  1993/02/25  16:45:35  ahop
 * 	7184: add rsdb_replica_get_name_by_uuid
 * 	[1993/02/25  03:15:35  ahop]
 * 
 * Revision 1.1.2.3  1992/12/29  13:26:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:50  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  18:20:35  sekhar
 * 	First security replication code drop.
 * 	[1992/08/31  15:08:41  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - replica management backend interface
 *
 */

#ifndef rsdb_replica_h__included
#define rsdb_replica_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <dce/rplbase.h>
#include <rs_rep_pvt.h>

#include <rsdb_pvt.h>


/*  
 *  Add a replica to the database.
 */
void rsdb_replica_add (
    rs_replica_mgt_item_p_t rp,
    error_status_t          *st
);

/*
 *  Replace information about a replica
 */
void rsdb_replica_replace (
    rs_replica_mgt_item_p_t rp,
    error_status_t          *st
);

/*
 *  Delete a replica from the database
 *
 */
void rsdb_replica_delete (
    uuid_p_t                rep_id,
    error_status_t          *st
);

void rsdb_replica_set_prop_info (
    uuid_p_t                rep_id,
    rs_replica_prop_t       prop_type,
    rs_replica_prop_t       prev_prop_type,
    uuid_p_t                init_id,
    error_status_t          *st
);

void rsdb_replica_get_prop_info (
    uuid_p_t                rep_id,
    rs_replica_prop_t       *prop_type,
    rs_replica_prop_t       *prev_prop_type,
    uuid_t                  *init_id,
    error_status_t          *st
);

void rsdb_replica_set_prop_mgt_handle (
    uuid_p_t                rep_id,
    rs_rep_prop_mgt_handle_t    
                            prop_mgt_h,
    error_status_t          *st
);

void rsdb_replica_get_prop_mgt_handle (
    uuid_p_t                rep_id,
    rs_rep_prop_mgt_handle_t    
                            *prop_mgt_h,
    error_status_t          *st
);

/*
 *  return a pointer to a copy of the
 *  next replica's information.
 *
 *  If cursor is uuid_nil, return a
 *  pointer to the first replica's
 *  information.
 */
void rsdb_replica_lookup (
    uuid_t                  *cursor,
    rs_replica_mgt_item_t   **rp,
    error_status_t          *st
);

/* 
 *  return a pointer to the next 
 *  replica's information.  
 *
 *  If cursor is uuid_nil, return a
 *  pointer to the first replica's
 *  information.
 */
void rsdb_replica_lookup_by_ref (
    uuid_t                      *cursor,        /* [in, out] */
    rs_replica_mgt_item_t       **rp,
    rs_replica_packed_buff_t    **rep_buff,
    unsigned32                  *rep_buff_len,  /* [out] */
    error_status_t              *st
);

/*
 *  free replica info that has been allocated
 *  by rsdb
 */
void rsdb_replica_item_free (
    rs_replica_mgt_item_t   **rp,
    error_status_t          *st
);

void rsdb_replica_get_name_by_uuid (
    uuid_t                  *rep_id,    /* [in] */
    rs_replica_name_p_t     *rep_name,  /* [out] */
    error_status_t          *st
);

#endif
