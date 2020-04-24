/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_attr_schema.c,v $
 * Revision 1.1.8.3  1996/02/18  00:18:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:32  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:54:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:08  root]
 * 
 * Revision 1.1.4.1  1994/10/25  21:14:06  burati
 * 	CR12713 Attribute schema updates not logged or propagated
 * 	[1994/10/25  19:55:49  burati]
 * 
 * Revision 1.1.2.3  1994/07/15  15:01:55  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:39  mdf]
 * 
 * Revision 1.1.2.2  1994/06/10  17:13:28  annie
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:12:37  annie]
 * 
 * Revision 1.1.2.1  1994/06/02  21:22:54  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:18  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	 /main/ODESSA_2/1  1994/04/26  19:58 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * 	 /main/mdf_odessa_era_bl33/2  1994/04/26  19:35 UTC  mdf
 * 	propagation support.
 * 
 * 	 /main/mdf_odessa_era_bl33/1  1994/04/13  13:07 UTC  mdf
 * 	Initial support.
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
#include <rs_log.h>
#include <rsdb_attr_pvt.h>
#include <rsdb_attr_schema.h>
#include <rs_master.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rs_ch_lock.h>
#include <sec_attr_tools.h>
#include <rrs_prop_attr_schema.h>



/*
 * rs_prop_attr_schema_create
 */
PUBLIC void rs_prop_attr_schema_create
(
    handle_t				h,
    unsigned32				number,
    rs_prop_attr_sch_create_data_t	schema[],
    rs_replica_master_info_t		*master_info,
    boolean32				propq_only,
    error_status_t			*st_p
)
{
    boolean32                   apply_update;
    int				i;

    CLEAR_STATUS(st_p);

    rs_rep_auth_check(h, st_p);
    if (BAD_STATUS(st_p)) {
    	return;
    }
	
    if (! propq_only) {
	BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, 
            master_info, st_p) 
        {
	    rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		for ( i = 0; i < number && GOOD_STATUS(st_p); i++) {
		    rsdb_attr_sch_add(schema[i].schema_name, 
			&schema[i].schema_entry, st_p);
		    if (GOOD_STATUS(st_p) && 
		    	(rs_state.rep_state != rs_c_state_initializing)) {
		    	rs_log_attr_schema_create(schema[i].schema_name,
			    &schema[i].schema_entry, master_info);
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
		for ( i = 0; i < number && GOOD_STATUS(st_p); i++) {
		    rs_log_attr_schema_create(schema[i].schema_name,
			&schema[i].schema_entry, master_info);
		}
	    }
	} END_RIW_EVENT;
    }
}

/*
 * rs_prop_attr_schema_delete
 */
PUBLIC void rs_prop_attr_schema_delete
(
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    uuid_t			*attr_id,
    rs_replica_master_info_t	*master_info,
    boolean32			propq_only,
    error_status_t		*st_p
)
{
    boolean32                   apply_update;
    CLEAR_STATUS(st_p);

    rs_rep_auth_check(h, st_p);
    if (BAD_STATUS(st_p)) {
    	return;
    }
	
    if (! propq_only) {
	BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, 
            master_info, st_p) 
        {
	    rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		rsdb_attr_sch_delete(schema_name, attr_id, st_p);
		if (GOOD_STATUS(st_p) && 
		    (rs_state.rep_state != rs_c_state_initializing)) {
		    rs_log_attr_schema_delete(schema_name, attr_id,
			master_info);
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
		rs_log_attr_schema_delete(schema_name, attr_id, master_info);
	    }
	} END_RIW_EVENT;
    }
}


/*
 * rs_prop_attr_schema_update
 */
PUBLIC void rs_prop_attr_schema_update
(
    handle_t			    h,
    rs_prop_attr_sch_update_data_t  *schema_p,
    rs_replica_master_info_t        *master_info,
    boolean32                       propq_only,
    error_status_t                  *st_p
)
{
    boolean32                   apply_update;
    CLEAR_STATUS(st_p);

    rs_rep_auth_check(h, st_p);
    if (BAD_STATUS(st_p)) {
    	return;
    }

    if (! propq_only) {
	BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db,
            master_info, st_p)
        {
	    rs_rep_mgmt_slave_chk_update(master_info, &apply_update, st_p);
	    if (apply_update && GOOD_STATUS(st_p)) {
		rsdb_attr_sch_replace(schema_p->schema_name,
		    &schema_p->schema_entry, schema_p->modify_parts, st_p);
		if (GOOD_STATUS(st_p) &&
		    (rs_state.rep_state != rs_c_state_initializing)) {
		    rs_log_attr_schema_update(schema_p->schema_name,
			&schema_p->schema_entry, schema_p->modify_parts,
			master_info);
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
		rs_log_attr_schema_update(schema_p->schema_name,
		    &schema_p->schema_entry, schema_p->modify_parts,
		    master_info);
	    }
	} END_RIW_EVENT;
    }
}
