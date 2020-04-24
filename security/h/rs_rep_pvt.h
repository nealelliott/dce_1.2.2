/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_pvt.h,v $
 * Revision 1.1.12.1  1996/10/15  20:49:33  arvind
 * 	Create rs_replica_mgt_item_list_t.
 * 	[1996/06/11  17:19 UTC  jrr  /main/HPDCE02/jrr_davis_4/1]
 *
 * Revision 1.1.10.3  1996/02/18  22:59:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:03  marty]
 * 
 * Revision 1.1.10.2  1995/12/13  16:25:46  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:01 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add prev_prop_type to stable replist management info.
 * 	[1995/01/22  20:42 UTC  greg  /main/greg_migration2/2]
 * 
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.1  1993/10/13  20:37:42  mccann
 * 	CR8651 64bit porting
 * 	[1993/10/13  20:37:15  mccann]
 * 
 * Revision 1.1.2.5  1992/12/29  13:25:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:22  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:49  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:53  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:34:46  emartin
 * 	bl6c: decl rs_rep_binding_t, rs_rep_auth_handle_t
 * 
 * Revision 1.1.2.3  1992/10/07  20:27:04  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:14  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:40:02  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:02  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:47:13  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Server - Replist Log operations
 */

#ifndef rs_rep_pvt__included
#define rs_rep_pvt__included

#include <dce/nbase.h>
#include <dce/rpc.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

/* 
 * Opaque pointer for arguments which are pointers to
 * internal structures.
 */
typedef struct
{
    unsigned32  dummy;
} *rs_rep_opaque_ptr_t;


/* 
 * Opaque handle on deciphered auth info 
 */
typedef rs_rep_opaque_ptr_t     rs_rep_auth_handle_t;

/*
 * Binding info to a replica
 */
typedef struct {
    boolean32               valid_handle;
    rpc_binding_handle_t    rep_handle;
    boolean32               valid_auth_handle;
    rs_rep_auth_handle_t    auth_handle;
    /* offset to current comm. tower in rep_twrs
     * -1 = no comm. tower
     */
    signed32                twr_offset;     
    rs_replica_twr_vec_p_t  rep_twrs;
} rs_rep_binding_t;

/*
 *  Opaque handle to the volatile information that
 *  the master maintains about a replica
 */
typedef rs_rep_opaque_ptr_t     rs_rep_prop_mgt_handle_t;

/*               
 *  memory for a rs_replica_mgt_item's name, tower vector,
 *  and towers allocated by rsdb or other means
 */
typedef enum rs_rep_mgt_item_alloc_t {   
    rs_e_rep_mgt_item_alloc_other, 
    rs_e_rep_mgt_item_alloc_rsdb 
} rs_rep_mgt_item_alloc_t;

/*  
 *  Replica information that is stablely stored
 *  by rsdb
 */
typedef struct {
    uuid_t                      rep_id;          /* instance uuid */
    rs_replica_name_p_t         rep_name;        /* (global) name service name */
    rs_replica_twr_vec_p_t      rep_twrs;        /* protocol towers for comm. w. replica */
    rs_replica_prop_t           prop_type;       /* prop type (init, update, bad_sw_rev, or delete) */
    rs_replica_prop_t           prev_prop_type;  /* prev prop type (only if prop type is bad_sw_rev) */
    uuid_t                      init_id;
    rs_rep_mgt_item_alloc_t     alloc_type; 
    rs_rep_prop_mgt_handle_t    prop_mgt;   /* replica's prop mgt info (volatile) */
} rs_replica_mgt_item_t, *rs_replica_mgt_item_p_t;

/* Structure for moving lists of rs_replica_mgt_item_t's around */
typedef struct rs_replica_mgt_item_list_s_t {
    rs_replica_mgt_item_p_t             item;
    struct rs_replica_mgt_item_list_s_t *next;
} rs_replica_mgt_item_list_t;


/*
 *  opaque packed replica information which
 *  includes the replica's name, tower vector,
 *  and towers in a contiguous buffer.
 *
 *  The buffer's real layout is in rs_replica_packed_item_t.
 *  it is manipulated by rs_rep_util_buff_pack/unpack operations
 *  in rs_reputil.c
 *  
 *  The code assumes the alignment of this buffer is rs_c_align_size,
 *  so force to to be so.  Need long for non-32 bit machines.
 */
typedef long rs_replica_packed_buff_t,
                        *rs_replica_packed_buff_p_t;



#endif
