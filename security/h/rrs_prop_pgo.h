/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_pgo.h,v $
 * Revision 1.1.8.1  1996/05/10  13:15:24  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/29  16:19 UTC  hanfei
 * 	add uuids as args to rrs_prop_pgo_add_member().
 * 	[1996/05/09  20:49:19  arvind]
 *
 * Revision 1.1.3.4  1992/12/29  13:07:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:39  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:09:59  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:12  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:04:11  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:24:46  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:43  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:30:26  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Pgo Items
 */

#ifndef __rrs_prop_pgo_h__included__
#define __rrs_prop_pgo_h__included__

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_prop_pgo.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

/* rrs_prop_pgo_add_all
 *
 * propgate all pgo items to an initializing slave
 */
void  rrs_prop_pgo_add_all (
#ifdef __STDC__
    rs_rep_binding_t          * rep_binding,  /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    error_status_t            * status        /* [out] */
#endif
);


/*
 * rrs_prop_pgo_add
 */
void  rrs_prop_pgo_add (
#ifdef __STDC__ 
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    unsigned32                num_pgo_items,  /* [in] */
    rs_prop_pgo_add_data_t    pgo_items[],    /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status        /* [out] */
#endif
);


/* rrs_prop_pgo_delete 
 * 
 */
void rrs_prop_pgo_delete (
#ifdef __STDC__ 
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            name,           /* [in, ref] */
    sec_timeval_sec_t         cache_info,     /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status        /* [out] */
#endif
);


/* rrs_prop_pgo_rename 
 * 
 */
void  rrs_prop_pgo_rename (
#ifdef __STDC__ 
    handle_t                   h,               /* [in] */
    sec_rgy_domain_t           domain,          /* [in] */
    sec_rgy_name_t             old_name,        /* [in, ref] */
    sec_rgy_name_t             new_name,        /* [in, ref] */
    sec_timeval_sec_t          cache_info,      /* [in] */
    rs_replica_master_info_t   * master_info,   /* [in, ref] */
    boolean32                  propq_only,      /* [in] */
    error_status_t             * status         /* [out] */
#endif
);


/* rrs_prop_pgo_replace 
 * 
 */
void  rrs_prop_pgo_replace (
#ifdef __STDC__ 
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            name,           /* [in, ref] */
    sec_rgy_pgo_item_t        * item,         /* [in, ref] */
    sec_timeval_sec_t         cache_info,     /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status        /* [out] */
#endif
);


/* rrs_prop_pgo_add_member
 * 
 */
void  rrs_prop_pgo_add_member (
#ifdef __STDC__ 
    handle_t                      h,               /* [in] */
    sec_rgy_domain_t              domain,          /* [in] */
    sec_rgy_name_t                go_name,         /* [in] */
    unsigned32                    num_members,     /* [in] */
    sec_rgy_member_t              members[],       /* [in, size_is(num_members)]  */
    uuid_p_t                      cell_uuids[],    /* [in] */
    uuid_p_t                      princ_uuids[],   /* [in] */
    rs_replica_master_info_t      * master_info,   /* [in, ref] */
    boolean32                     propq_only,      /* [in] */
    error_status_t                * status         /* [out] */
#endif
);


/* rrs_prop_pgo_delete_member
 * 
 */
void  rrs_prop_pgo_delete_member (
#ifdef __STDC__ 
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            go_name,        /* [in, ref] */
    sec_rgy_name_t            person_name,    /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status        /* [out] */
#endif
);


#endif /* __rrs_prop_pgo_h__included__ */

