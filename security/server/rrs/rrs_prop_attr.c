/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_attr.c,v $
 * Revision 1.1.6.2  1996/02/18  00:16:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:49  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:33  root]
 * 
 * Revision 1.1.2.5  1994/09/02  17:47:27  mdf
 * 	CR 11868  Initial checkin of missing functionality that propagates removal of attributes
 * 	[1994/08/30  13:36:11  mdf]
 * 
 * Revision 1.1.2.4  1994/07/15  15:01:02  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:18:39  mdf]
 * 
 * Revision 1.1.2.3  1994/06/16  19:40:27  desai
 * 	fix unsigned char * strncpy type mismatches.
 * 	[1994/06/16  18:25:29  desai]
 * 
 * 	Fix OT# 10950: include rsdb_attr.h to get prototypes and fix
 * 	               fix prototype mismatch errors.
 * 	[1994/06/16  18:07:09  desai]
 * 
 * Revision 1.1.2.2  1994/06/10  17:13:23  annie
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:11:29  annie]
 * 
 * Revision 1.1.2.1  1994/06/02  21:26:54  mdf
 * 	hp_sec_to_osf_3 drop, disable propagation of directories.
 * 	[1994/05/26  19:54:17  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:12  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	 /main/ODESSA_2/2  1994/05/06  19:56 UTC  mdf
 * 	 Fixes for Inital ERA progagation drop
 * 
 * 	 /main/ODESSA_2/mdf_odessa_era_bl36/1  1994/05/06  19:46 UTC  mdf
 * 	fix problems with multiple attr updates.
 * 
 * 	 /main/ODESSA_2/1  1994/04/26  19:54 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * 	 /main/mdf_odessa_era_bl33/1  1994/04/26  19:38 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * $EndLog$
 */

#include <rs_log.h>
#include <rs_prop_attr.h>
#include <rrs_prop_util.h>
#include <rsdb_attr_pvt.h>              /* macros */
#include <rsdb_attr.h>         
#include <rsdb_attr_schema.h>           /* prototypes */
#include <sec_attr_tools.h>

# define RS_PROP_ATTR_MAX 10

PUBLIC void rrs_prop_attr_update
(
    handle_t			h,
    unsigned32			num_to_write,
    rs_prop_attr_data_t		prop_attrs[],
    rs_replica_master_info_t	*master_info,
    boolean32			propq_only,
    error_status_t		*st_p
)
{
    (*rs_prop_attr_v1_0_c_epv.rs_prop_attr_update)
	(h, num_to_write, prop_attrs, master_info, propq_only, st_p);
}
PRIVATE void rrs_prop_attr_update_domain_topd
(
    rs_rep_binding_t            *rep_binding,
    sec_rgy_domain_t            domain,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st_p
)
{
    sec_rgy_name_t              component_name;
    char                        *p1;
    unsigned32                  num_returned;
    rsdb_named_item_t           item;
    rs_prop_attr_data_t         prop_attrs[1];
    sec_attr_t			attr_keys[1], *sec_attr_p;
    unsigned32                  num_attrs=0;
    boolean32                   propq_only = false;
    unsigned32                  i, space_avail;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32                    *xref = NULL;
    signed32                    failure_index;
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    unsigned32			cur_num_attrs;
    rs_attr_cursor_t		cursor;
    unsigned32			ii, num_left;
    uuid_t			obj_uuid;

    CLEAR_STATUS(st_p);

    /* get domain name
     */
    rsdb_name_util_complete_name(domain, "dummy", component_name, st_p);
    p1 = (char *)index((char *)component_name, '/');
    *p1 = '\0';

    rsdb_util_get_obj_info_by_name(component_name, rsdb_dir_type,
	sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);
    if(BAD_STATUS(st_p)) {
	return;
    }

    rsdb_attr_cursor_init(&rgy_obj_info, &cur_num_attrs, &cursor, st_p);
    if(BAD_STATUS(st_p)) {
	return;
    }

    /* 
     * Allocate a (rs_prop_attr_list_t *) with enough contiguous 
     * memory for a sec_attr_t array of size cur_num_attrs.
     */
    if (!(prop_attrs[0].attr_list = (rs_prop_attr_list_t *)
                rpc_ss_allocate(sizeof(rs_prop_attr_list_t)
                    + (cur_num_attrs - 1) * sizeof(sec_attr_t))))  {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
    }

    rsdb_attr_sch_get_entries(component_name,
	num_attrs, attr_keys, &rgy_obj_info, &sch_list,
	&xref, &failure_index, st_p);
    if (STATUS_EQUAL(st_p, sec_attr_no_more_entries)) {
	CLEAR_STATUS(st_p);
	return;
    }

    if(GOOD_STATUS(st_p)) {
        u_strncpy(prop_attrs[0].component_name, component_name, 
                sizeof(sec_rgy_name_t));
        prop_attrs[0].component_name[sizeof(sec_rgy_name_t) -1] = '\0';

        rsdb_attr_lookup_by_uuid(&rgy_obj_info, &cursor,
            cur_num_attrs, sch_list, rpc_ss_allocate, 
            &prop_attrs[0].attr_list->num_attrs, 
            prop_attrs[0].attr_list->attrs, &num_left, st_p);
    } 

    if(GOOD_STATUS(st_p)) {
        master_info->previous_update_seqno = master_info->update_seqno;
        rs_util_seqno_next(&master_info->update_seqno);
        PROP_SETUP_RETRY {
	    rrs_prop_attr_update(rep_binding->rep_handle, 1, 
	        prop_attrs, master_info, propq_only, st_p);
	    PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st_p)
        } PROP_END_RETRY
    }
}

PRIVATE void
rrs_prop_attr_update_init
(
    rs_rep_binding_t                    *rep_binding,
    unsigned32                          number,
    rs_prop_attr_data_t      		attrs[],
    rs_replica_master_info_t            *master_info,
    error_status_t                      *st_p
)
{
    boolean32                   	propq_only = false;

    CLEAR_STATUS(st_p);
    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {

	rrs_prop_attr_update(rep_binding->rep_handle, number, attrs,
		master_info, propq_only, st_p);

	PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st_p)

    } PROP_END_RETRY
}

PRIVATE void    rrs_prop_attr_update_domain_init
(
    rs_rep_binding_t            *rep_binding,
    sec_rgy_domain_t            domain,
    rs_replica_master_info_t    *master_info, 
    error_status_t              *st_p
)
{
    rsdb_pvt_id_t               parent_id;
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    rs_attr_cursor_t		cursor;
    unsigned32			num_entries, ii, i, j;

    sec_rgy_name_t              name, component_name;
    unsigned32                  name_len;
    unsigned32                  space_avail;
    long			component_name_len;
    rsdb_name_key_t             next;
    rsdb_named_item_t           item;
    signed32                    domain_prefix_len;
    rs_prop_attr_data_t         prop_attrs[RS_PROP_ATTR_MAX];
    boolean32                   propq_only = false;
    boolean32                   done;
    char                        *p1, *p2;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32                    *xref = NULL;
    signed32                    failure_index;
    sec_attr_t			attr_keys[1], *sec_attr_p;
    unsigned32                  num_attrs=0;
    unsigned32                  num_returned;
    unsigned32			num_left;
    unsigned32			cur_num_attrs;

    CLEAR_STATUS(st_p);

    for (i=0; i < RS_PROP_ATTR_MAX; i++) {
	rsdb_name_util_complete_name(domain, "dummy",
		prop_attrs[i].component_name, st_p);
    }
    p1 = (char *)index((char *)prop_attrs[0].component_name, '/');
    p2 = (char *)prop_attrs[0].component_name;
    /* prefix length includes '/' */
    domain_prefix_len = (p1 - p2) + 1;
    
    /* Retrieve every named_item (pgo or directory) in the domain.
     * Use get_next_by_name_key starting with the lexicographically
     * least name_key (parent_id = 0, name_len = 0).  This will
     * retrieve the first named_item in the tree.  Subsequent
     * calls will return the rest until no more named items are found.
     * Use the item id with path_contruct to create the component name.
     * PGOs require a credentials lookup to get the acl_id.
     */
    ii = 0;
    done = false;
    parent_id = 0;
    name_len = 0;


    while(GOOD_STATUS(st_p) && !done) {
        rsdb_util_get_next_by_name_key(domain, parent_id, name,
		name_len, &next, &item, st_p);

        if(BAD_STATUS(st_p)) {
            if(STATUS_EQUAL(st_p, sec_rgy_object_not_found) ||
               STATUS_EQUAL(st_p, sec_rgy_no_more_entries)) {
                done = true;
                CLEAR_STATUS(st_p);
            }
        }

	/* 
	 * If we have a good status, and were done, or the index
	 * is equal to max then update the attributes. 
	 */

        if (GOOD_STATUS(st_p) && (done || ii == RS_PROP_ATTR_MAX)) {
	    if (ii) {
		rrs_prop_attr_update_init(rep_binding, ii, prop_attrs, 
			master_info, st_p);
                ii = 0;
	    }
        }

	/*
	 * If were are finsihed, then just continue, so that
         * the while loop will catch that were done.
	 */
	if (done)
		continue;

        if(GOOD_STATUS(st_p)) {
            /* Construct the full name of the item */
            rsdb_util_path_construct(domain, item.body.threads.id, 
		&prop_attrs[ii].component_name[domain_prefix_len],
		&component_name_len, st_p);
        }

        if(GOOD_STATUS(st_p)) {
	    rsdb_util_get_obj_info_by_name(prop_attrs[ii].component_name,
		rsdb_pgo_type, sec_acl_type_object, &rgy_obj_info,
		&obj_uuid, st_p);
	}

	if(GOOD_STATUS(st_p)) {
    	    rsdb_attr_cursor_init(&rgy_obj_info, &cur_num_attrs, &cursor, st_p);
	    if (BAD_STATUS(st_p)) {
                return;
	    }
	}

        if (!(prop_attrs[ii].attr_list = (rs_prop_attr_list_t *)
                rpc_ss_allocate(sizeof(rs_prop_attr_list_t)
                    + (cur_num_attrs - 1) * sizeof(sec_attr_t))))  {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
        }

	if(GOOD_STATUS(st_p)) {
            rsdb_attr_sch_get_entries( prop_attrs[ii].component_name,
                num_attrs, attr_keys, &rgy_obj_info, &sch_list,
                &xref, &failure_index, st_p);
        }

        if(GOOD_STATUS(st_p)) {
	    rsdb_attr_lookup_by_uuid(&rgy_obj_info, &cursor,
		cur_num_attrs, sch_list, rpc_ss_allocate, 
		&num_returned, prop_attrs[ii].attr_list->attrs, &num_left, st_p);
            prop_attrs[ii].attr_list->num_attrs = num_returned;
        } 

	/*
	 * Ok, the above call to rsdb_attr_lookup_by_uuid, will
	 * more than likely return sec_attr_no_more_entries, 
	 * which is OK, just don't increment the index 'ii'.
 	 */

        if (STATUS_EQUAL(st_p, sec_attr_no_more_entries)) {
	    CLEAR_STATUS(st_p);
	} else if (GOOD_STATUS(st_p)) {
	    ii++;
	}

	/* 
	 * As long as we have a good status then set up parent_id 
	 * and name for the get_next_by_name_key call.
	 */
        if(GOOD_STATUS(st_p)) {
            parent_id = next.parent_id;
            u_strncpy(name, next.name, next.name_len);
            name_len = next.name_len;
        }
    }
}

PUBLIC void rrs_prop_attr_update_all
(
    rs_rep_binding_t            *rep_binding,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st_p
)
{
    /*
     * walk the principal, group, and org subtrees, and propagate the
     * ACL for each node encountered along the way.
     * Propagate the top level directory for each domain in a special
     * routine.
     */

#ifdef NOT_YET
    rrs_prop_attr_update_domain_topd(rep_binding,
            sec_rgy_domain_person, master_info, st_p);
    if(BAD_STATUS(st_p)) return;
#endif

    rrs_prop_attr_update_domain_init(rep_binding,
            sec_rgy_domain_person, master_info, st_p);
    if(BAD_STATUS(st_p)) return;

#ifdef NOT_YET
    rrs_prop_attr_update_domain_topd(rep_binding,
            sec_rgy_domain_group, master_info, st_p);
    if(BAD_STATUS(st_p)) return;
#endif

    rrs_prop_attr_update_domain_init(rep_binding,
            sec_rgy_domain_group, master_info, st_p);
    if(BAD_STATUS(st_p)) return;

#ifdef NOT_YET
    rrs_prop_attr_update_domain_topd(rep_binding,
            sec_rgy_domain_org, master_info, st_p);
    if(BAD_STATUS(st_p)) return;
#endif

    rrs_prop_attr_update_domain_init(rep_binding,
            sec_rgy_domain_org, master_info, st_p);
    if(BAD_STATUS(st_p)) return;

}

PUBLIC void rrs_prop_attr_delete
(
    handle_t			h,
    unsigned32			number,
    rs_prop_attr_data_t		prop_attrs[],
    rs_replica_master_info_t	*master_info,
    boolean32			propq_only,
    error_status_t		*st_p
)
{
    (*rs_prop_attr_v1_0_c_epv.rs_prop_attr_delete)
	(h, number, prop_attrs, master_info, propq_only, st_p);
}
