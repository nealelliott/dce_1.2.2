/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_attr.c,v $
 * Revision 1.1.4.2  1996/02/18  00:18:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:54:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:07  root]
 * 
 * Revision 1.1.2.4  1994/09/02  17:47:29  mdf
 * 	fix bogus check ins
 * 	[1994/08/31  19:17:12  mdf]
 * 
 * 	Botched frist check in.
 * 	[1994/08/31  13:21:08  mdf]
 * 
 * 	CR 11868  Initial checkin of missing functionality that propagates removal of attributes
 * 	[1994/08/30  13:36:16  mdf]
 * 
 * Revision 1.1.2.3  1994/07/15  15:01:54  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:35  mdf]
 * 
 * Revision 1.1.2.2  1994/06/09  19:32:07  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:31:23  annie]
 * 
 * Revision 1.1.2.1  1994/06/02  21:22:52  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:15  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:23  mdf]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
#include <rs_log.h>
#include <rsdb_attr_pvt.h>
#include <rsdb_attr.h>
#include <rs_master.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rs_ch_lock.h>
#include <rs_attr_pvt.h>
#include <dce/sec_attr_tools.h>
#include <rrs_prop_attr.h>


/*
 * rs_prop_attr_update
 */
PUBLIC void rs_prop_attr_update
(
    handle_t			h,
    unsigned32			num_to_write,
    rs_prop_attr_data_t		prop_attrs[],
    rs_replica_master_info_t	*master_info,
    boolean32			propq_only,
    error_status_t		*st_p
)
{
    boolean32                   apply_update;
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    uuid_t                      obj_uuid;
    sec_acl_permset_t           requested_perms;
    char                        buf[sizeof(sec_rgy_name_t) + 80];
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32                    *xref = NULL;
    signed32			failure_index;
    signed32                    ii, i, j;

    CLEAR_STATUS(st_p);

    rs_rep_auth_check(h, st_p);
    if (BAD_STATUS(st_p)) {
    	return;
    }
	
    if (! propq_only) {
	BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, 
            master_info, st_p) {
	    rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		for (ii = 0; ii < num_to_write && GOOD_STATUS(st_p); ii++) {
		    rsdb_util_get_obj_info_by_name(prop_attrs[ii].component_name,
			rsdb_pgo_type, sec_acl_type_object, &rgy_obj_info, 
			&obj_uuid, st_p);
		    if(BAD_STATUS(st_p)) return;
		    rsdb_attr_sch_get_entries(prop_attrs[ii].component_name,
			prop_attrs[ii].attr_list->num_attrs,
			prop_attrs[ii].attr_list->attrs, 
			&rgy_obj_info, &sch_list, &xref,
			&failure_index, st_p);
		    if (STATUS_EQUAL(st_p, sec_attr_no_more_entries)) {
			CLEAR_STATUS(st_p);
			continue;
		    }
		    if(BAD_STATUS(st_p)) return;
		    rsdb_attr_update(&rgy_obj_info,
			prop_attrs[ii].attr_list->num_attrs,
			prop_attrs[ii].attr_list->attrs, sch_list, xref,
			&failure_index,
			st_p);
		    if (GOOD_STATUS(st_p) && 
			(rs_state.rep_state != rs_c_state_initializing)) {
			    rs_log_attr_update(prop_attrs[ii].component_name, 
				prop_attrs[ii].attr_list->num_attrs,
				prop_attrs[ii].attr_list->attrs, 
				master_info);
	    	    }
		}
	    }
	} END_SLAVE_WRITE_EVENT;
    } else {
        /* 
         * Just add update to propq only 
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, st_p) {
	    rs_rep_mgmt_slave_chk_propq_upd(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		for (ii = 0; ii < num_to_write && GOOD_STATUS(st_p); ii++) {
		    rs_log_attr_update(prop_attrs[ii].component_name,
			prop_attrs[ii].attr_list->num_attrs,
			prop_attrs[ii].attr_list->attrs, 
			master_info);
		}
	    }
	} END_RIW_EVENT;
    }
}

/*
 * rs_prop_attr_delete
 */
PUBLIC void rs_prop_attr_delete
(
    handle_t			h,
    unsigned32			num_to_delete,
    rs_prop_attr_data_t		prop_attrs[],
    rs_replica_master_info_t	*master_info,
    boolean32			propq_only,
    error_status_t		*st_p
)
{
    boolean32                   apply_update;
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    uuid_t                      obj_uuid;
    sec_acl_permset_t           requested_perms;
    char                        buf[sizeof(sec_rgy_name_t) + 80];
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32                    *xref = NULL;
    signed32			failure_index;
    signed32                    ii, i, j;

    CLEAR_STATUS(st_p);

    rs_rep_auth_check(h, st_p);
    if (BAD_STATUS(st_p)) {
    	return;
    }
	
    if (! propq_only) {
	BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, 
            master_info, st_p) {
	    rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		for (ii = 0; ii < num_to_delete && GOOD_STATUS(st_p); ii++) {
		    rsdb_util_get_obj_info_by_name(prop_attrs[ii].component_name,
			rsdb_pgo_type, sec_acl_type_object, &rgy_obj_info, 
			&obj_uuid, st_p);
		    if(BAD_STATUS(st_p)) return;
		    rsdb_attr_sch_get_entries(prop_attrs[ii].component_name,
			prop_attrs[ii].attr_list->num_attrs,
			prop_attrs[ii].attr_list->attrs, 
			&rgy_obj_info, &sch_list, &xref,
			&failure_index, st_p);
		    if (STATUS_EQUAL(st_p, sec_attr_no_more_entries)) {
			CLEAR_STATUS(st_p);
			continue;
		    }
		    if(BAD_STATUS(st_p)) return;
		    rsdb_attr_delete(&rgy_obj_info,
			prop_attrs[ii].attr_list->num_attrs,
			prop_attrs[ii].attr_list->attrs, sch_list, xref,
			&failure_index,
			st_p);
		    if (GOOD_STATUS(st_p) && 
			(rs_state.rep_state != rs_c_state_initializing)) {
			    rs_log_attr_delete(prop_attrs[ii].component_name, 
				prop_attrs[ii].attr_list->num_attrs,
				prop_attrs[ii].attr_list->attrs, 
				master_info);
	    	    }
		}
	    }
	} END_SLAVE_WRITE_EVENT;
    } else {
        /* 
         * Just add update to propq only 
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, st_p) {
	    rs_rep_mgmt_slave_chk_propq_upd(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		for (ii = 0; ii < num_to_delete && GOOD_STATUS(st_p); ii++) {
		    rs_log_attr_delete(prop_attrs[ii].component_name,
			prop_attrs[ii].attr_list->num_attrs,
			prop_attrs[ii].attr_list->attrs, 
			master_info);
		}
	    }
	} END_RIW_EVENT;
    }
}
