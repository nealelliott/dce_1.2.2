/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_acl.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:10  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:47  root
 * 	Submit
 * 	[1995/12/11  15:14:28  root]
 * 
 * Revision 1.1.3.4  1992/12/29  13:07:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:36  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:09:56  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:07  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:03:54  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:24:39  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:39  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:29:56  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - ACLs
 */
#ifndef __rrs_prop_acl_h__included__
#define __rrs_prop_acl_h__included__

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_prop_acl.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

/*  rrs_prop_acl_replace_all
 *
 * Walk the entire security service name space, propagating all ACLs
 * to an initializing slave.  Must not be called until the entire name
 * space exists at the slave i.e., until after calling rrs_prop_pgo_add_all
 */
void  rrs_prop_acl_replace_all (
#ifdef __STDC__
    rs_rep_binding_t          * rep_binding,  /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in]  */
    error_status_t            * st            /* [out] */
#endif
);


/* rrs_prop_acl_replace
 * 
 */
void  rrs_prop_acl_replace (
#ifdef __STDC__
    handle_t                  h,              /* [in] */
    unsigned32                num_acls,       /* [in] */
    rs_prop_acl_data_t        acls[],         /* [in, size_is(num_acls)] */   
    rs_replica_master_info_t  * master_info,  /* [in] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status        /* [out] */
#endif
);

#endif  /* __rrs_prop_acl_h__included__ */

