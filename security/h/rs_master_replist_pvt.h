/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master_replist_pvt.h,v $
 * Revision 1.1.7.3  1996/02/18  22:59:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:47  marty]
 *
 * Revision 1.1.7.2  1995/12/13  16:25:27  root
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
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.3.3  1992/12/29  13:08:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:44  zeliff]
 * 
 * Revision 1.1.3.2  1992/12/18  22:10:28  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:10  sekhar]
 * 
 * Revision 1.1.1.2  1992/12/11  21:09:25  emartin
 * 	bl6c: initial version
 * 
 * $EndLog$
 */
/*  
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Internal master replist types and externs
 *
 */

#ifndef rs_master_replist_pvt_h__included
#define rs_master_replist_pvt_h__included

#include <dce/uuid.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <rs_rep_pvt.h>
#include <rs_util.h>

/*
 *  Summary of comm state between the master and
 *  a replica.
 */
typedef struct {
    rs_replica_comm_t       comm_state; 
    unsigned32              nprop_tries;           
    error_status_t          last_status;
} rs_rep_comm_info_mgt_t, *rs_rep_comm_info_mgt_p_t;



/*  
 *  Volatile information managed by the master to
 *  guide its propagation to a replica.
 *
 *  Entries are accessed via a linked list.
 */
typedef struct {
    rs_list_t                   list;
    uuid_t                      rep_id;
    boolean32                   purged;
    boolean32                   master;
    rs_rep_binding_t            rep_binding;
    boolean32                   prop_in_progress;
    uuid_t                      init_id;
    uuid_t                      init_from_rep_id;   /* nil if not initing */
    rs_replica_prop_info_t      prop_info;
    rs_rep_comm_info_mgt_t      comm_info;
    unsigned32                  compat_sw_rev;      /* 0xffffffff if not known */
   rs_replica_prop_t            prev_prop_type;     
} rs_rep_prop_mgt_item_t, *rs_rep_prop_mgt_item_p_t;

#define PROP_MGT_SW_REV_UNKNOWN  0xffffffffUL
#define PROP_MGT_SW_REV_INITED(prp) \
    ((prp)->compat_sw_rev != PROP_MGT_SW_REV_UNKNOWN)

extern rs_list_t    rs_master_replistq;

#endif
