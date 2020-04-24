/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_replist.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:15  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:51  root
 * 	Submit
 * 	[1995/12/11  15:14:31  root]
 * 
 * Revision 1.1.2.6  1993/01/26  23:00:10  emartin
 * 	delete_replica => del_replica
 * 	[1993/01/13  16:25:10  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  13:07:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:45  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:04  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:22  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:05:14  emartin
 * 	bl6c: decl rrs_prop_replist_add_all
 * 
 * Revision 1.1.2.3  1992/10/07  20:25:05  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:54  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:32:51  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:00  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:03:05  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Replica list
 */   
#ifndef __rrs_prop_replist_h__included__
#define __rrs_prop_replist_h__included__

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

/* rrs_prop_replist_add_all
 *
 * Propagate all replist entries to an initializing slave.  
 */
void  rrs_prop_replist_add_all (
#ifdef __STDC__ 
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
#endif
);

void rrs_prop_replist_add_replica (
#ifdef __STDC__
    handle_t                    h,              /* [in] */
    uuid_p_t                    rep_id,         /* [in] */
    rs_replica_name_p_t         rep_name,       /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs,       /* [in] */
    rs_replica_master_info_t    *master_info,   /* [in] */
    boolean32                   propq_only,     /* [in] */
    error_status_t              *status
#endif
);

void rrs_prop_replist_del_replica (
#ifdef __STDC__
    handle_t                    h,              /* [in] */
    uuid_p_t                    rep_id,         /* [in] */
    rs_replica_master_info_t    *master_info,   /* [in] */
    boolean32                   propq_only,     /* [in] */
    error_status_t              *status
#endif
);

#endif
