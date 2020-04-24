/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_acl.c,v $
 * Revision 1.1.14.3  1996/02/18  00:16:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:46  marty]
 *
 * Revision 1.1.14.2  1995/12/08  17:48:00  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:04 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  19:53 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:18 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	Fix bug in xattrschema acl propagation.
 * 	[1995/04/05  15:05 UTC  greg  /main/SEC_migrate/greg_sec_migrate_acls/1]
 * 
 * 	propagate xattrschema acl during initialization,
 * 	but only if the current software version supports
 * 	xattrs.
 * 	[1995/04/03  18:40 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.10.2  1994/08/04  16:13:17  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:45  mdf]
 * 
 * Revision 1.1.10.1  1994/04/22  19:13:56  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:10:31  rps]
 * 
 * Revision 1.1.8.2  1994/03/29  23:44:28  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.7.1  1993/10/15  20:46:30  ahop
 * 	OT7832: malloc/free bulk transfer structures instead of using
 * 	automatic variables that gobble dangerous amounts of stack
 * 	[1993/10/15  19:26:29  ahop]
 * 
 * Revision 1.1.3.7  1993/03/10  15:18:26  emartin
 * 	OT 7279 rrs_prop_acl_replace_all - propagate top level
 * 	directories' acls
 * 	[1993/03/09  22:16:12  emartin]
 * 
 * Revision 1.1.3.6  1993/01/26  23:02:23  emartin
 * 	bl6d: fix bug copying directory component names in domain_init
 * 	[1993/01/07  18:44:37  emartin]
 * 
 * Revision 1.1.3.5  1992/12/29  16:33:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:21:44  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:11:14  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:47:51  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:37:52  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.3  1992/11/04  19:34:11  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:26:36  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:30:20  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:24  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:54:10  burati
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

#include <dce/dce.h>    
#include <dce/sec_daclmgr.h>
#include <rs_server.h>
#include <rs_util.h>
#include <rs_prop_acl.h>
#include <rrs_prop_acl.h>
#include <rrs_prop_util.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_policy.h>
#include <sec_svc.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>

/* the maximum number of ACLS that 
 * can be propagated in a single call
 */
/* note: the value for init_bulk_max was arbitrarily
 * assigned; it could perhaps be improved.
 */
#define RS_PROP_ACL_INIT_BULK_MAX 10

PRIVATE void    free_sec_acl_list
#ifndef __STDC__
    ( sec_acl_list )
    sec_acl_list_t              **sec_acl_list; /* [in, out] */
#else
    (
    sec_acl_list_t              **sec_acl_list
    )
#endif
{
    unsigned32          i;
                                               
    if(*sec_acl_list != (sec_acl_list_t *)NULL) {
        for(i = 0; i < (*sec_acl_list)->num_acls; i++) {
            if((*sec_acl_list)->sec_acls[i] != NULL) 
                free((char *)(*sec_acl_list)->sec_acls[i]);
        }
        free((char *)(*sec_acl_list));
        *sec_acl_list = NULL;
    }
}


static void    rrs_prop_acl_replace_attr_schema_init (
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st
)
{
    rsdb_acl_ids_t              key_ids;
    rs_prop_acl_data_t          acls[1];
    unsigned32                  num_acls;
    boolean32                   propq_only = false;

    CLEAR_STATUS(st);

    key_ids.acl_id = rsdb_get_attr_schema_acl_id();
    key_ids.obj_id = RSDB_ATTR_SCHEMA_OBJ_ID;

    num_acls = 1;
    acls[0].component_name = (sec_acl_component_name_t)SEC_ATTR_SCHEMA_OBJ_NAME;
    acls[0].manager_type = rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_attr_schema];
    acls[0].acl_type = sec_acl_type_object;

    sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL, (sec_acl_key_t)&key_ids,
                       &acls[0].manager_type, acls[0].acl_type,
                       &acls[0].acl_list, st);
    if(BAD_STATUS(st))
        return; 

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {
        rrs_prop_acl_replace(rep_binding->rep_handle, num_acls, 
                             acls, master_info, propq_only, st);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st)
    } PROP_END_RETRY

    free_sec_acl_list(&acls[0].acl_list);
}


PRIVATE void    rrs_prop_acl_replace_plcy_init
#ifndef __STDC__
    ( rep_binding, master_info, st ) 
    rs_rep_binding_t            *rep_binding;   /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in]  */
    error_status_t              *st;            /* [out] */
#else
  (
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st
  )
#endif
{
    rsdb_acl_ids_t              key_ids;
    rs_prop_acl_data_t          acls[1];
    unsigned32                  num_acls;
    boolean32                   propq_only = false;

    CLEAR_STATUS(st);

    key_ids.acl_id = rsdb_get_rgy_acl_id();
    key_ids.obj_id = RSDB_POLICY_OBJ_ID;

    num_acls = 1;
    acls[0].component_name = (sec_acl_component_name_t)SEC_POLICY_OBJ_NAME;
    acls[0].manager_type = rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_rgy];
    acls[0].acl_type = sec_acl_type_object;

    sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL, (sec_acl_key_t)&key_ids,
                       &acls[0].manager_type, acls[0].acl_type,
                       &acls[0].acl_list, st);
    if(BAD_STATUS(st))
        return; 

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {
        rrs_prop_acl_replace(rep_binding->rep_handle, num_acls, 
                             acls, master_info, propq_only, st);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st)
    } PROP_END_RETRY

    free_sec_acl_list(&acls[0].acl_list);
}


PRIVATE void    rrs_prop_acl_replace_rplst_init
#ifndef __STDC__
    ( rep_binding, master_info, st ) 
    rs_rep_binding_t            *rep_binding;   /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in]  */
    error_status_t              *st;            /* [out] */
#else
  (
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st
  )
#endif
{
    rsdb_acl_ids_t              key_ids;
    rs_prop_acl_data_t          acls[1];
    unsigned32                  num_acls;
    boolean32                   propq_only = false;

    CLEAR_STATUS(st);

    key_ids.acl_id = rsdb_get_replist_acl_id();
    key_ids.obj_id = RSDB_REPLIST_OBJ_ID;

    num_acls = 1;
    acls[0].component_name = (sec_acl_component_name_t)SEC_REPLIST_OBJ_NAME;
    acls[0].manager_type = rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_replist];
    acls[0].acl_type = sec_acl_type_object;

    sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL, (sec_acl_key_t)&key_ids,
                       &acls[0].manager_type, acls[0].acl_type,
                       &acls[0].acl_list, st);
    if(BAD_STATUS(st))
        return; 
    
    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {
        rrs_prop_acl_replace(rep_binding->rep_handle, num_acls, 
                             acls, master_info, propq_only, st);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st)
    } PROP_END_RETRY
    free_sec_acl_list(&acls[0].acl_list);
}

PRIVATE void fill_acl
#ifndef __STDC__
    (domain, name, item, num_acls, acls, st)
    sec_rgy_domain_t            domain;         /* [in] */
    sec_rgy_name_t              name;           /* [in] */
    rsdb_named_item_t           *item;          /* [in] */
    unsigned32                  *num_acls;      /* [in, out] */
    rs_prop_acl_data_t          acls[];         /* [out] */
    error_status_t              *st;            /* [out] */
#else
  (
    sec_rgy_domain_t            domain,         /* [in] */
    sec_rgy_name_t              name,           /* [in] */
    rsdb_named_item_t           *item,          /* [in] */
    unsigned32                  *num_acls,      /* [in, out] */
    rs_prop_acl_data_t          acls[],         /* [out] */
    error_status_t              *st             /* [out] */
  )
#endif
{
    unsigned32                  nacls;
    rsdb_acl_ids_t              key_ids;
    rsdb_credentials_t          credentials;

    nacls = *num_acls;

    key_ids.domain = domain;
    key_ids.obj_id = item->body.threads.id;
    if (RSDB_IS_PGO(item)) {
        if(!rsdb_get_credentials_by_unix_id(domain_info[domain].db,
                RSDB_PGO(item).unix_id, &credentials)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_PGO_CREDS_MSG);
        }
        key_ids.acl_id = credentials.acl_id;
        /* Copy a pointer to component name into acls[].component_name */
        acls[nacls].component_name = name;
        acls[nacls].acl_type = sec_acl_type_object;
        acls[nacls].manager_type = 
                rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(domain)];
        sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL, (sec_acl_key_t)&key_ids,
                &acls[nacls].manager_type, acls[nacls].acl_type,
                &acls[nacls].acl_list, st);
        if(GOOD_STATUS(st))
            nacls++;
    }
    if (RSDB_IS_DIR(item)) {		/* must look up three acls */
        /* Get directory's object acl */
        key_ids.acl_id = GET_DIR_ACL_ID(item, sec_acl_type_object);
        /* Copy a pointer to component name into acls[].component_name */
        acls[nacls].component_name = name;
        acls[nacls].acl_type = sec_acl_type_object;
        acls[nacls].manager_type = 
                rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_dir];
        sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL, 
	        (sec_acl_key_t)&key_ids,
                &acls[nacls].manager_type, acls[nacls].acl_type,
                &acls[nacls].acl_list, st);
        if(GOOD_STATUS(st)) {
            nacls++;
        }
    
        /* Get directory's default object acl */
        key_ids.acl_id = 
                GET_DIR_ACL_ID(item, sec_acl_type_default_object);
        /* Component name is the same as above, so copy pointer */
        acls[nacls].component_name = name;
        acls[nacls].acl_type = sec_acl_type_default_object;
        acls[nacls].manager_type = 
                rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(domain)];
        sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL,
	        (sec_acl_key_t)&key_ids,
                &acls[nacls].manager_type, acls[nacls].acl_type,
                &acls[nacls].acl_list, st);
        if(GOOD_STATUS(st)) {
            nacls++;
        }
        /* Get directory's default container acl */
        key_ids.acl_id = 
                GET_DIR_ACL_ID(item, sec_acl_type_default_container);
        /* Component name is the same as above, so copy pointer */
        acls[nacls].component_name = name;
        acls[nacls].acl_type = sec_acl_type_default_container;
        acls[nacls].manager_type = 
                rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_dir];
        sec_acl_mgr_lookup((sec_acl_mgr_handle_t) NULL,
	        (sec_acl_key_t)&key_ids,
                &acls[nacls].manager_type, acls[nacls].acl_type,
                &acls[nacls].acl_list, st);
        if(GOOD_STATUS(st)) {
            nacls++;
        }
    }

    *num_acls = nacls;
}

/*
 * Propagate the acls for the domain's top level directory.
 * This directory's item lookup is done differently
 * from the lower level names - ergo the special routine
 */
PRIVATE void rrs_prop_acl_replace_domain_topd
#ifndef __STDC__
    ( rep_binding, domain, master_info, st ) 
    rs_rep_binding_t            *rep_binding;   /* [in] */
    sec_rgy_domain_t            domain;         /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in]  */
    error_status_t              *st;            /* [out] */
#else
  (
    rs_rep_binding_t            *rep_binding,
    sec_rgy_domain_t            domain,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st
  )
#endif
{
    sec_rgy_name_t              component_name;
    char                        *p1;
    rsdb_named_item_t           item;
    rs_prop_acl_data_t          acls[RSDB_ITEM_MAX_ACLS];
    unsigned32                  num_acls;
    boolean32                   propq_only = false;
    unsigned32                  i;

    CLEAR_STATUS(st);

    /* get domain name
     */
    rsdb_name_util_complete_name(domain, "dummy", component_name, st);
    p1 = (char *)index((char *)component_name, '/');
    *p1 = '\0';

    /* get this entry's item
     */
    if (!rsdb_util_get_item_by_id(domain, START_OF_LIST, &item)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
    }

    num_acls = 0;
    fill_acl(domain, component_name, &item,
        &num_acls, acls, st);
    if (BAD_STATUS(st)) return;

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {
        rrs_prop_acl_replace(rep_binding->rep_handle, num_acls, 
                             acls, master_info, propq_only, st);
        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                         rep_binding, st)
    } PROP_END_RETRY

    for(i = 0; i < num_acls; i++)
        free_sec_acl_list(&acls[i].acl_list);
}

PRIVATE void    rrs_prop_acl_replace_domain_init
#ifndef __STDC__
    ( rep_binding, domain, master_info, st ) 
    rs_rep_binding_t            *rep_binding;   /* [in] */
    sec_rgy_domain_t            domain;         /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in]  */
    error_status_t              *st;            /* [out] */
#else
  (
    rs_rep_binding_t            *rep_binding,
    sec_rgy_domain_t            domain,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st
  )
#endif
{
    rsdb_pvt_id_t               parent_id;
    sec_rgy_name_t              name;
    unsigned32                  name_len;
    rsdb_name_key_t             next;
    rsdb_named_item_t           item;
    signed32                    domain_prefix_len;
    sec_rgy_name_t              *component_names;
    long                        component_name_len;
    rs_prop_acl_data_t          acls[RS_PROP_ACL_INIT_BULK_MAX];
    unsigned32                  num_acls;
    boolean32                   propq_only = false;
    unsigned32                  i;
    boolean32                   done;
    char                        *p1, *p2;

    CLEAR_STATUS(st);

    if(!(component_names =
        (sec_rgy_name_t *)malloc(RS_PROP_ACL_INIT_BULK_MAX *
                                 sizeof(sec_rgy_name_t)))) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        return;
    }

    memset(acls, 0, RS_PROP_ACL_INIT_BULK_MAX * sizeof(rs_prop_acl_data_t));

    /* Initialize each name in component_names with the 
     * domain name prefix followed by a slash
     */
    for(i = 0; i < RS_PROP_ACL_INIT_BULK_MAX; i++) {
        rsdb_name_util_complete_name(domain, "dummy", *(component_names + i), st);
    }
    p1 = (char *)index((char *)component_names, '/');
    p2 = (char *)component_names;
    /* prefix length includes '/' */
    domain_prefix_len = (p1 - p2) + 1;

    /* Retrieve every named_item (pgo or directory) in the domain.
     * Use get_next_by_name_key starting with the lexicographically
     * least name_key (parent_id = 0, name_len = 0).  This will
     * retrieve the first named_item in the tree.  Subsequent
     * calls will return the rest until no more named items are found.
     * Use the item id with path_contruct to create the component name.
     * PGOs require a credentials lookup to get the acl_id.
     * Directory items contain three acl_ids, or four if also a pgo.
     */
    num_acls = 0;
    done = false;
    parent_id = 0;
    name_len = 0;
    while(GOOD_STATUS(st) && !done) {
        rsdb_util_get_next_by_name_key(domain, parent_id, name,
                                       name_len, &next, &item, st);
        if(BAD_STATUS(st)) {
            if(STATUS_EQUAL(st, sec_rgy_object_not_found) ||
               STATUS_EQUAL(st, sec_rgy_no_more_entries)) {
                done = true;
                CLEAR_STATUS(st);
            }
        }

        /* If there's no room for the acls associated with the
         * current item, propagate what we have so far and 
         * free the acl_list memory.
         */
        if( GOOD_STATUS(st) && 
            (done || num_acls == RS_PROP_ACL_INIT_BULK_MAX ||
            (RSDB_IS_DIR(&item) &&
             num_acls > RS_PROP_ACL_INIT_BULK_MAX - RSDB_ITEM_NUM_ACLS(&item))) ) {

            master_info->previous_update_seqno = master_info->update_seqno;
            rs_util_seqno_next(&master_info->update_seqno);

            PROP_SETUP_RETRY {
                rrs_prop_acl_replace(rep_binding->rep_handle, num_acls, 
                                     acls, master_info, propq_only, st);
                PROP_CHECK_RETRY(rrs_prop_util_retry_handler, 
                                 rep_binding, st)
            } PROP_END_RETRY

            for(i = 0; i < num_acls; i++)
                free_sec_acl_list(&acls[i].acl_list);
            num_acls = 0;
        }

        if(GOOD_STATUS(st) && !done) {
            /* Construct the full name of the item */
            rsdb_util_path_construct(domain, item.body.threads.id, 
                &(*(component_names + num_acls))[domain_prefix_len],
                &component_name_len, st);
        }

        if(GOOD_STATUS(st) && !done) {
            /* fill component's acls from component_name and item
             */
            fill_acl(domain, *(component_names + num_acls), &item,
                &num_acls, acls, st);
        } 

        if(GOOD_STATUS(st) && !done) {
            /* Set up parent_id and name for get_next_by_name_key call */
            parent_id = next.parent_id;
            u_strncpy(name, next.name, next.name_len);
            name_len = next.name_len;
        }
    }

    if(BAD_STATUS(st)) {
        for(i = 0; i < num_acls; i++)
            free_sec_acl_list(&acls[i].acl_list);
    }
    free(component_names);
}



/*  rrs_prop_acl_replace_all
 *
 * Walk the entire security service name space, propagating all ACLs
 * to an initializing slave.  Must not be called until the entire name
 * space exists at the slave i.e., until after calling rrs_prop_pgo_add_all
 */
PUBLIC  void  rrs_prop_acl_replace_all
#ifndef __STDC__
    ( rep_binding, master_info, st ) 
    rs_rep_binding_t            *rep_binding;   /* [in] */
    rs_replica_master_info_t    *master_info;   /* [in, ref]  */
    error_status_t              *st;            /* [out] */
#else
  (
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st
  )
#endif
{
    error_status_t  ignore_st;

    CLEAR_STATUS(st);

    /* propagate the registry POLICY node ACL */
    rrs_prop_acl_replace_plcy_init(rep_binding, master_info, st);
    if(BAD_STATUS(st)) return;

    /* propagate the REPLICA list node acl */
    rrs_prop_acl_replace_rplst_init(rep_binding, master_info, st);
    if(BAD_STATUS(st)) return;

    if (rs_sw_feature_test_support(rs_c_sw_feature_era, &ignore_st)) {
	/* propagate the XATTRSCHEMA list node acl */
	rrs_prop_acl_replace_attr_schema_init(rep_binding, master_info, st);
	if(BAD_STATUS(st)) return;
    }

    /* 
     * walk the principal, group, and org subtrees, and propagate the
     * ACL for each node encountered along the way.  
     * Propagate the top level directory for each domain in a special 
     * routine.
     */

    rrs_prop_acl_replace_domain_topd(rep_binding, 
            sec_rgy_domain_person, master_info, st);
    if(BAD_STATUS(st)) return;

    rrs_prop_acl_replace_domain_init(rep_binding, 
            sec_rgy_domain_person, master_info, st);
    if(BAD_STATUS(st)) return;

    rrs_prop_acl_replace_domain_topd(rep_binding, 
            sec_rgy_domain_group, master_info, st);
    if(BAD_STATUS(st)) return;

    rrs_prop_acl_replace_domain_init(rep_binding, 
            sec_rgy_domain_group, master_info, st);
    if(BAD_STATUS(st)) return;

    rrs_prop_acl_replace_domain_topd(rep_binding, 
            sec_rgy_domain_org, master_info, st);
    if(BAD_STATUS(st)) return;

    rrs_prop_acl_replace_domain_init(rep_binding, 
            sec_rgy_domain_org, master_info, st);
    if(BAD_STATUS(st)) return;
}


/* rrs_prop_acl_replace
 * 
 */
PUBLIC  void  rrs_prop_acl_replace
#ifndef __STDC__
    ( h, num_acls, acls, master_info, propq_only, status )
    handle_t                  h;              /* [in] */
    unsigned32                num_acls;       /* [in] */
    rs_prop_acl_data_t        acls[];         /* [in, size_is(num_acls)] */
    rs_replica_master_info_t  *master_info;   /* [in] */
    boolean32                 propq_only;     /* [in] */
    error_status_t            *status;        /* [out] */
#else
  (
    handle_t                  h,              
    unsigned32                num_acls,       
    rs_prop_acl_data_t        acls[],         
    rs_replica_master_info_t  *master_info,   
    boolean32                 propq_only,     
    error_status_t            *status
  )
#endif
{
        (*rs_prop_acl_v1_0_c_epv.rs_prop_acl_replace) (h, num_acls, acls,
                                                       master_info, propq_only, 
                                                       status);
}

