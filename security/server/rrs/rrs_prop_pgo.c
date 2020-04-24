/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_pgo.c,v $
 * Revision 1.1.12.1  1996/05/10  13:19:48  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	 	HP revision /main/hanfei_dce1.2_b1/2  1996/03/14  19:15 UTC  hanfei
 * 	rrs_prop_pgo_add_member(): add check for migration switch for DCE 1.2.2.
 *
 * 	 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/29  16:33 UTC  hanfei
 * 	add uuids as arguments to rrs_prop_pgo_add_members().
 * 	work for rrs_prop_pgo_mem_init() to get uuids and pass
 * 	them to rrs_prop_pgo_add_members().
 * 	[1996/05/09  21:36:46  arvind]
 *
 * Revision 1.1.8.1  1994/08/23  20:18:38  max
 * 	Fix for CR 11135 .  64bit problem. In rrs_prop_pgo_mem_init
 * 	rsdb_pgo_get_members requires signed32* parameters not long*
 * 	[1994/08/23  20:05:25  max]
 * 
 * Revision 1.1.6.1  1993/10/15  20:46:33  ahop
 * 	OT7832: malloc/free bulk transfer structures instead of using
 * 	automatic variables that gobble dangerous amounts of stack.
 * 	[1993/10/15  19:26:41  ahop]
 * 
 * Revision 1.1.3.4  1992/12/29  16:33:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:48  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:11:18  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:47:57  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:31:53  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:30:30  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:29  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:54:29  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - PGO Items
 */
    
#include <rs_server.h>
#include <rs_util.h>
#include <rs_prop_pgo.h>
#include <rs_ver_pvt.h>
#include <rrs_prop_pgo.h>
#include <rrs_prop_plcy.h>
#include <rrs_prop_util.h>
#include <rsdb_pgo.h>
#include <rsdb_policy.h>

/* the maximum number of items that 
 * can be propagated in a single call 
 */
/*XXX figure out what this should really be */
#define RS_PROP_PGO_ADD_MAX 25
#define RS_PROP_PGO_ADD_MEM_MAX 25

/* 
 * Private routines
 */


/* rrs_prop_pgo_mem_init
 *
 * Propagate all members in a group or org to a remote rgy
 */
PRIVATE void rrs_prop_pgo_mem_init
#ifndef __STDC__
    ( rep_binding, domain, name, buf_size, mem_buf, 
      master_info, propq_only, status )
    rs_rep_binding_t            *rep_binding;   /* [in] */
    sec_rgy_domain_t            domain;         /* [in] */
    sec_rgy_name_t              name;           /* [in] */
    unsigned32                  buf_size;       /* [in] */
    sec_rgy_member_t            mem_buf[];      /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in] */
    boolean32                   propq_only;     /* [in] */
    error_status_t              *status;        /* [out] */
#else
  (
    rs_rep_binding_t            *rep_binding,
    sec_rgy_domain_t            domain,         
    sec_rgy_name_t              name,           
    unsigned32                  buf_size,       
    sec_rgy_member_t            mem_buf[],      
    rs_replica_master_info_t    *master_info,   
    boolean32                   propq_only,     
    error_status_t              *status
  )
#endif
{
    sec_rgy_cursor_t      cursor;      /* current cursor in domain */
    sec_rgy_cursor_t      mbr_cursor;  /* cursor within member list */
    signed32              num_returned;/* number of members returned in
                                        * a single call
                                        */
    signed32              num_members; /* number of members total */
    rsdb_named_item_t     pgo;
    uuid_p_t		  *cell_uuids, *current_cell_p;
    uuid_p_t		  *princ_uuids, *current_princ_p;
    int                   i;
    error_status_t        lst;
    
    rs_util_clear_cursor(&mbr_cursor);
    CLEAR_STATUS(&lst);
    CLEAR_STATUS(status);

    while(GOOD_STATUS(&lst) && GOOD_STATUS(status)) {
        rsdb_pgo_get_members(false, domain, name, &mbr_cursor, 
                    buf_size, mem_buf, &num_returned,  
                    &num_members, &lst);
                             
        if ( GOOD_STATUS(&lst) && num_returned > 0 ) {
            master_info->previous_update_seqno = 
                        master_info->update_seqno;
            rs_util_seqno_next(&master_info->update_seqno);

	/*
	 * allocate space for cell_uuids and princ_uuids
	 * if domain is for group and the member is a foreign 
	 * principal syntax then the uuids entry for that member
	 * is updated with itse cell uuid and principal uuid
 	 * else the cell_uuid entry for that member is set to
	 * be NULL_PTR.
	 */

	if ( ! ( cell_uuids = malloc(num_returned*sizeof(uuid_p_t))) ) {
            SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return;
	}
	if ( ! ( princ_uuids = malloc(num_returned*sizeof(uuid_p_t))) ) {
            SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return;
	}

	current_cell_p = cell_uuids;
	current_princ_p = princ_uuids;

	for ( i = 0; i < num_returned && GOOD_STATUS(status) ; i++ ) {
	    /* Filling in cell and princ uuids. */
	    if ( rsdb_name_util_is_foreign_pgo(&mem_buf[i]) && 
		 domain == sec_rgy_domain_group ) {
		    if ( ! ( *current_cell_p = (uuid_p_t)malloc(sizeof(uuid_t))) ) {
            		SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    		return;
		    }
		    if ( ! ( *current_princ_p = (uuid_p_t)malloc(sizeof(uuid_t))) ) {
            		SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    		return;
		    }

		 if ( rsdb_pgo_util_get_by_name(sec_rgy_domain_foreign_person,
			mem_buf[i], strlen((char *)&mem_buf[i]), &pgo) ) {
		    rsdb_get_credentials_uuids_by_pvt_id(
			domain_info[sec_rgy_domain_foreign_person].db,
			RSDB_PGO(&pgo).threads.id, *current_cell_p, 
			*current_princ_p);
		} else {
		    SET_STATUS(status, sec_rgy_object_not_found);
		}
	    } else {
		*current_cell_p = (uuid_p_t)NULL_PTR;
		*current_princ_p = (uuid_p_t)NULL_PTR;
	    }
	    current_cell_p++; current_princ_p++;
	}

            PROP_SETUP_RETRY {
                rrs_prop_pgo_add_member(rep_binding->rep_handle, 
                            domain, name, num_returned, mem_buf,
			    cell_uuids, princ_uuids,
                            master_info, propq_only, status);
                PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                                 rep_binding, status)
            } PROP_END_RETRY

	current_cell_p = cell_uuids;
	current_princ_p = princ_uuids;
	for ( i = 0; i < num_returned && GOOD_STATUS(status) ; i++ ) {
	    if ( *current_cell_p != (uuid_p_t)NULL_PTR ) {
		free(*current_cell_p);
	    }
	    if ( *current_princ_p != (uuid_p_t)NULL_PTR ) {
		free(*current_princ_p);
	    }
	}
	free(cell_uuids);
	free(princ_uuids);

        }
        /*
         * work around get_members anomaly - cursor is reset when
         * read last member 
         */
        if (!mbr_cursor.valid) break; 
    } /* end loop - all members in org or group */
          
     /* propagate malignant database lookup errors back to the caller */
    if (BAD_STATUS(&lst) && !STATUS_EQUAL(&lst, sec_rgy_no_more_entries)) {
        COPY_STATUS(&lst, status);
    }
}


/* rrs_prop_pgo_add_domain_init
 *
 * propagate all pgo items in a domain to an initializing slave
 */
PRIVATE void rrs_prop_pgo_add_domain_init
#ifndef __STDC__
    ( rep_binding, domain, master_info, status ) 
    rs_rep_binding_t           *rep_binding;   /* [in] */
    sec_rgy_domain_t           domain;         /* [in] */
    rs_replica_master_info_t   *master_info;   /* [in] */
    error_status_t             *status;        /* [out] */
#else
  (
    rs_rep_binding_t           *rep_binding,
    sec_rgy_domain_t           domain,         /* [in] */
    rs_replica_master_info_t   *master_info,   /* [in] */
    error_status_t             *status
  )
#endif
{
    sec_rgy_cursor_t        cursor;     /* current cursor in domain */
    sec_rgy_name_t          name;       /* current item name */
    sec_rgy_pgo_item_t      item;       /* current item data */
    sec_rgy_plcy_t          policy;     /* policy data for this org */
    boolean32               propq_only = false;
    rs_prop_pgo_add_data_t  *bulk;
    unsigned32              num_items = 0;
    sec_rgy_member_t        *mem_buf;
    unsigned32              i;
    error_status_t          lst;

    rs_util_clear_cursor(&cursor);
    CLEAR_STATUS(&lst);
    CLEAR_STATUS(status);

    if(!(bulk = 
        (rs_prop_pgo_add_data_t *)malloc(RS_PROP_PGO_ADD_MAX * 
                                  sizeof(rs_prop_pgo_add_data_t)))) {
        SET_STATUS(status, sec_rgy_cant_allocate_memory);
        return;
    }
    if(!(mem_buf = 
        (sec_rgy_member_t *)malloc(RS_PROP_PGO_ADD_MEM_MAX * 
                                  sizeof(sec_rgy_member_t)))) {
        SET_STATUS(status, sec_rgy_cant_allocate_memory);
        free(bulk);
        return;
    }

    while(GOOD_STATUS(&lst) && GOOD_STATUS(status)) {
        /* 
         * Pack "bulk" with pgo items from the database
         */
        num_items = 0;
        for(i = 0; i < RS_PROP_PGO_ADD_MAX && GOOD_STATUS(&lst); i++) {
            rsdb_pgo_get_next(domain, &cursor, (bulk + i)->name, 
                              &((bulk + i)->item), &lst);
            if (GOOD_STATUS(&lst)) {
                num_items += 1;
            } 
        } 

        if (GOOD_STATUS(&lst) || 
            STATUS_EQUAL(&lst, sec_rgy_no_more_entries) ||
            STATUS_EQUAL(&lst, sec_rgy_object_not_found)) {

            if (num_items > 0) {
                master_info->previous_update_seqno = 
                        master_info->update_seqno;
                rs_util_seqno_next(&master_info->update_seqno);

                PROP_SETUP_RETRY {
                    rrs_prop_pgo_add(rep_binding->rep_handle, 
                            domain, num_items, bulk, master_info, 
                            propq_only, status);
                            
                    PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                                     rep_binding, status)
                } PROP_END_RETRY
                /* 
                 * propagate group/org memberships. (We can do this here
                 * because we propagate all principals before groups and
                 * orgs, thus ensuring that all potential members exist
                 * at the slave.  By doing it here we avoid iterating
                 * over the group and org domains twice.  Propagate
                 * org policy.
                 */
                if(domain != sec_rgy_domain_person) {
                    for(i = 0; i<num_items && GOOD_STATUS(status); 
                        i++) {
                        /* only organizations have policy 
                         * records to propagate 
                         */
                        if (domain == sec_rgy_domain_org) {
                            rsdb_policy_get_info((bulk + i)->name, 
                                    &policy, status);
                            if (STATUS_OK(status)) {
                                master_info->previous_update_seqno =
                                        master_info->update_seqno;
                                rs_util_seqno_next(
                                        &master_info->update_seqno);
                                PROP_SETUP_RETRY {
                                    rrs_prop_plcy_set_info(
                                        rep_binding->rep_handle, 
                                        (bulk + i)->name, 
                                        &policy, master_info, 
                                        propq_only, status);
                                    PROP_CHECK_RETRY(
                                        rrs_prop_util_retry_handler,
                                        rep_binding, status)
                                } PROP_END_RETRY
                            } else if (STATUS_EQUAL(status, 
                                       sec_rgy_object_not_found)) {
                                CLEAR_STATUS(status);
                            }
                        }
                        rrs_prop_pgo_mem_init(rep_binding,
                                domain, (bulk + i)->name,
                                RS_PROP_PGO_ADD_MEM_MAX, 
                                mem_buf, master_info, 
                                propq_only, status);
                    }
                }
            }
        } else {
            /* propagate malignant database lookup 
             * errors back to the caller 
             */
            COPY_STATUS(&lst, status);  
        }

    } /* end loop - all domain entries */

    free(bulk);
    free(mem_buf);
}


/*
 * PUBLIC routines
 */

/* rrs_prop_pgo_add_all
 *
 * propgate all pgo items to an initializing slave
 */
PUBLIC  void  rrs_prop_pgo_add_all
#ifndef __STDC__
    ( rep_binding, master_info, status )  
    rs_rep_binding_t          * rep_binding;  /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    error_status_t            * status;       /* [out] */
#else
  (
    rs_rep_binding_t          * rep_binding,
    rs_replica_master_info_t  * master_info,
    error_status_t            * status
  )
#endif
{
    /*
     * MUST propagate principals before group 
     * and orgs so we can propagate
     * group and org memberships from 
     * within rrs_prop_pgo_add_domain_init.
     */
    rrs_prop_pgo_add_domain_init(rep_binding, sec_rgy_domain_person, 
                            master_info, status);
    if (BAD_STATUS(status)) return;

    rrs_prop_pgo_add_domain_init(rep_binding, sec_rgy_domain_group, 
                            master_info, status);
    if (BAD_STATUS(status)) return;

    rrs_prop_pgo_add_domain_init(rep_binding, sec_rgy_domain_org, 
                            master_info, status);
    if (BAD_STATUS(status)) return;
}



/*
 * rrs_prop_pgo_add
 */
PUBLIC  void  rrs_prop_pgo_add
#ifndef __STDC__
    ( h, domain, num_pgo_items, pgo_items, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_domain_t          domain;         /* [in] */
    unsigned32                num_pgo_items;  /* [in] */
    rs_prop_pgo_add_data_t    pgo_items[];    /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,             
    sec_rgy_domain_t          domain,       
    unsigned32                num_pgo_items,  
    rs_prop_pgo_add_data_t    pgo_items[],   
    rs_replica_master_info_t  * master_info,
    boolean32                 propq_only,  
    error_status_t            * status
  )
#endif
{
        (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_add) (h, domain, 
                    num_pgo_items, pgo_items, master_info, 
                    propq_only, status);
}


/* rrs_prop_pgo_delete 
 * 
 */
PUBLIC  void rrs_prop_pgo_delete
#ifndef __STDC__
    ( h, domain, name, cache_info, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_domain_t          domain;         /* [in] */
    sec_rgy_name_t            name;           /* [in, ref] */
    sec_timeval_sec_t         cache_info;     /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            name,           /* [in, ref] */
    sec_timeval_sec_t         cache_info,     /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
#endif
{
        (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_delete) (h, domain, 
                    name, cache_info, master_info, propq_only, 
                    status);
}


/* rrs_prop_pgo_rename 
 * 
 */
PUBLIC  void  rrs_prop_pgo_rename
#ifndef __STDC__
    ( h, domain, old_name, new_name, cache_info, master_info, 
         propq_only, status )
    handle_t                   h;               /* [in] */
    sec_rgy_domain_t           domain;          /* [in] */
    sec_rgy_name_t             old_name;        /* [in, ref] */
    sec_rgy_name_t             new_name;        /* [in, ref] */
    sec_timeval_sec_t          cache_info;      /* [in] */
    rs_replica_master_info_t   * master_info;   /* [in, ref] */
    boolean32                  propq_only;      /* [in] */
    error_status_t             * status;        /* [out] */
#else
  (
    handle_t                   h,               /* [in] */
    sec_rgy_domain_t           domain,          /* [in] */
    sec_rgy_name_t             old_name,        /* [in, ref] */
    sec_rgy_name_t             new_name,        /* [in, ref] */
    sec_timeval_sec_t          cache_info,      /* [in] */
    rs_replica_master_info_t   * master_info,   /* [in, ref] */
    boolean32                  propq_only,      /* [in] */
    error_status_t             * status
  )
#endif
{
        (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_rename) (h, domain, 
                    old_name, new_name, cache_info, master_info, 
                    propq_only, status);
}


/* rrs_prop_pgo_replace 
 * 
 */
PUBLIC  void  rrs_prop_pgo_replace
#ifndef __STDC__
    ( h, domain, name, item, cache_info, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    sec_rgy_domain_t          domain;         /* [in] */
    sec_rgy_name_t            name;           /* [in, ref] */
    sec_rgy_pgo_item_t        * item;         /* [in, ref] */
    sec_timeval_sec_t         cache_info;     /* [in] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            name,           /* [in, ref] */
    sec_rgy_pgo_item_t        * item,         /* [in, ref] */
    sec_timeval_sec_t         cache_info,     /* [in] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
#endif
{
        (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_replace) (h, domain, 
                    name, item, cache_info, master_info, 
                    propq_only, status);
}


/* rrs_prop_pgo_add_member
 * 
 */
PUBLIC  void  rrs_prop_pgo_add_member
#ifndef __STDC__
    ( h, domain, go_name, num_members, members, person_cell_uuids,
	 person_princ_uuids, master_info, propq_only, status )
    handle_t                      h;               /* [in] */
    sec_rgy_domain_t              domain;          /* [in] */
    sec_rgy_name_t                go_name;         /* [in] */
    unsigned32                    num_members;     /* [in] */
    sec_rgy_member_t              members[];       /* [in] */
    uuid_p_t			  cell_uuids[];    /* [in} */
    uuid_p_t			  princ_uuids[];   /* [in] */
    rs_replica_master_info_t      * master_info;   /* [in, ref] */
    boolean32                     propq_only;      /* [in] */
    error_status_t                * status;        /* [out] */
#else
  (
    handle_t                      h,               
    sec_rgy_domain_t              domain,         
    sec_rgy_name_t                go_name,       
    unsigned32                    num_members,     
    sec_rgy_member_t              members[],      
    uuid_p_t			  cell_uuids[],
    uuid_p_t			  princ_uuids[],
    rs_replica_master_info_t      * master_info, 
    boolean32                     propq_only,   
    error_status_t                * status
  )
#endif
{
    	static boolean32	support_global_group = false;
	error_status_t		lst;

    	/* 
	 * Global group is introduced in DCE 1.2.2 to allow global
	 * principals to be added as group member.  To ensure
	 * successful propagation, we need all servers running new
	 * bits for this functionality, hence we need check software
	 * version to decide if we can call the new function -
	 * rs_prop_pgo_add_member_global().
	 */
	if ( ! support_global_group ) {
	    /* can ignore lst, we just use the older version */
            support_global_group = rs_sw_feature_test_support
		(rs_c_sw_feature_global_group, &lst);
	}

	if ( support_global_group ) {
            (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_add_member_global) (h, 
                    domain, go_name, num_members, members,
		    cell_uuids, princ_uuids,
                    master_info, propq_only, status);
	} else {
            (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_add_member) (h, 
                    domain, go_name, num_members, members,
                    master_info, propq_only, status);
	}
}


/* rrs_prop_pgo_delete_member
 * 
 */
PUBLIC  void  rrs_prop_pgo_delete_member
#ifndef __STDC__
    ( h, domain, go_name, person_name, master_info, 
      propq_only, status ) 
    handle_t                  h;              /* [in] */
    sec_rgy_domain_t          domain;         /* [in] */
    sec_rgy_name_t            go_name;        /* [in, ref] */
    sec_rgy_name_t            person_name;    /* [in, ref] */
    rs_replica_master_info_t  * master_info;  /* [in, ref] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            * status;       /* [out] */
#else
  (
    handle_t                  h,              /* [in] */
    sec_rgy_domain_t          domain,         /* [in] */
    sec_rgy_name_t            go_name,        /* [in, ref] */
    sec_rgy_name_t            person_name,    /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status
  )
#endif
{
        (*rs_prop_pgo_v1_0_c_epv.rs_prop_pgo_delete_member) (h, 
                    domain, go_name, person_name, master_info, 
                    propq_only, status);
}     


