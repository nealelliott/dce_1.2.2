/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_attr_schema.c,v $
 * Revision 1.1.8.3  1996/02/18  00:16:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:50  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:48:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:35  root]
 * 
 * Revision 1.1.4.1  1994/10/25  21:13:59  burati
 * 	CR12713 Attribute schema updates not logged or propagated
 * 	[1994/10/25  19:55:26  burati]
 * 
 * Revision 1.1.2.2  1994/06/10  17:13:24  annie
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:11:30  annie]
 * 
 * Revision 1.1.2.1  1994/06/02  21:26:56  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:15  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	 /main/ODESSA_2/2  1994/05/06  19:56 UTC  mdf
 * 	 Fixes for Inital ERA progagation drop
 * 
 * 	 /main/ODESSA_2/mdf_odessa_era_bl36/1  1994/05/06  19:47 UTC  mdf
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
#include <rs_server.h>
#include <rs_util.h>
#include <rrs_prop_util.h>
#include <rs_prop_attr_schema.h>
#include <rrs_prop_attr_schema.h>
#include <rs_attr_util.h>
#include <rsdb_attr_pvt.h>              /* macros */
#include <rsdb_attr_schema.h>           /* prototypes */
#include <sec_attr_tools.h>

/* 
 * note: this value was arbitrarily assigned;
 * it could perhaps be improved.
 */
#define RS_PROP_ATTR_SCH_MAX 10	
PUBLIC void rrs_prop_attr_schema_create
(
    handle_t				h,
    unsigned32				number,
    rs_prop_attr_sch_create_data_t	entries[],
    rs_replica_master_info_t		*master_info,
    boolean32				propq_only,
    error_status_t			*st_p
)
{
    (*rs_prop_attr_sch_v1_0_c_epv.rs_prop_attr_schema_create)
	(h, number, entries, master_info, propq_only, st_p);
}

PRIVATE void rrs_prop_attr_schema_create_init
(
    rs_rep_binding_t			*rep_binding,
    unsigned32				number,
    rs_prop_attr_sch_create_data_t	schemas[],
    rs_replica_master_info_t		*master_info,
    error_status_t			*st_p
)
{
    CLEAR_STATUS(st_p);

    master_info->previous_update_seqno = master_info->update_seqno;
    rs_util_seqno_next(&master_info->update_seqno);

    PROP_SETUP_RETRY {

        rrs_prop_attr_schema_create(rep_binding->rep_handle,
            number, schemas, master_info, false, st_p);

        PROP_CHECK_RETRY(rrs_prop_util_retry_handler, rep_binding, st_p)

    } PROP_END_RETRY
}

PUBLIC void rrs_prop_attr_schema_create_all
(
    rs_rep_binding_t          *rep_binding,
    rs_replica_master_info_t  *master_info,
    error_status_t            *st_p
)
{
    rs_prop_attr_sch_create_data_t	schema[RS_PROP_ATTR_SCH_MAX];
    rs_attr_cursor_t			cursor;
    unsigned32				num_entries; 		/* ignored */
    sec_attr_component_name_t		schema_name = '\0';	/* not useful */
    unsigned32				i, index;
    boolean32				done = false;

    CLEAR_STATUS(st_p);
	
    rsdb_attr_sch_cursor_init(schema_name, &num_entries, &cursor, st_p);
    while (GOOD_STATUS(st_p) && !done) {
	index = 0;
    	while (GOOD_STATUS(st_p) && index < RS_PROP_ATTR_SCH_MAX ) {
	    rsdb_attr_sch_get_next(schema_name, NULL, &cursor, 
		&schema[index].schema_entry, &num_entries, st_p);
	    if (GOOD_STATUS(st_p)) 
	    schema[index].schema_name = schema_name; 	/* not used */
	    if (GOOD_STATUS(st_p)) index++;
	}
	done = STATUS_EQUAL(st_p, sec_attr_no_more_entries);
	if (GOOD_STATUS(st_p) || done) {
	    if (index) {
		rrs_prop_attr_schema_create_init(rep_binding, index, schema, 
			master_info, st_p);
		for(i = 0; i < index - 1  && GOOD_STATUS(st_p); i++) {
		    sec_attr_util_sch_ent_free_ptrs(&schema[i].schema_entry);
		}
	    }
	}
    }
    if (STATUS_EQUAL(st_p, sec_attr_no_more_entries)) {
	CLEAR_STATUS(st_p);
    }
}


PUBLIC void rrs_prop_attr_schema_delete
(
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    uuid_t			*attr_id,
    rs_replica_master_info_t	*master_info,
    boolean32			propq_only,
    error_status_t		*st_p
)
{
    (*rs_prop_attr_sch_v1_0_c_epv.rs_prop_attr_schema_delete)
	(h, schema_name, attr_id, master_info, propq_only, st_p);
}


PUBLIC void rrs_prop_attr_schema_update
(
    handle_t                        h,
    rs_prop_attr_sch_update_data_t  *schema_p,
    rs_replica_master_info_t        *master_info,
    boolean32                       propq_only,
    error_status_t                  *st_p
)
{
    (*rs_prop_attr_sch_v1_0_c_epv.rs_prop_attr_schema_update)
	(h, schema_p, master_info, propq_only, st_p);
}
