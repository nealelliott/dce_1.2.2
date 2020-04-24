/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: asops.c,v $
 * Revision 1.1.4.1  1996/11/13  17:55:45  arvind
 * 	fix dced acl code returns wrong error on unauthorized
 * 	[1996/10/09  19:39 UTC  bartf  /main/bartf_pk_fix/1]
 *
 * 	fix incorrect invalid_permission error to be not_authorized
 * 	[1996/02/18  19:16:04  marty  1.1.2.2]
 *
 * Revision 1.1.2.2  1996/02/18  19:16:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:03  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:53:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:48:50  root]
 * 
 * Revision 1.1.2.17  1994/09/27  17:14:59  pwang
 * 	Validated fields in 'update" operation and validate attr sch
 * 	obj name in all operations [OT#12321, 12322, 12324]
 * 	[1994/09/27  16:51:35  pwang]
 * 
 * Revision 1.1.2.16  1994/09/16  20:51:09  bowe
 * 	SVC work - cast error_status_t to (long) for dce_svc_printf [CR 11725]
 * 	[1994/09/16  20:50:35  bowe]
 * 
 * Revision 1.1.2.15  1994/09/14  20:04:15  bowe
 * 	SVC work [CR 11725]
 * 	Delete ACL when deleting the object [CR 12030]
 * 	[1994/09/14  20:03:36  bowe]
 * 
 * Revision 1.1.2.14  1994/09/08  21:52:46  pwang
 * 	Don't check the acl_mgr_set if it's created by dced
 * 	itself [OT#11840]
 * 	[1994/09/08  21:52:27  pwang]
 * 
 * Revision 1.1.2.13  1994/09/08  20:08:13  pwang
 * 	Validated the fields of schema entry in
 * 	rs_attr_schema_create_entry() [OT#11840]
 * 	[1994/09/08  20:06:36  pwang]
 * 
 * Revision 1.1.2.12  1994/08/04  16:12:12  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/01  15:11:31  mdf]
 * 
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/01  15:02:07  mdf]
 * 
 * Revision 1.1.2.11  1994/07/15  14:58:10  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  21:01:49  mdf]
 * 
 * Revision 1.1.2.10  1994/06/13  20:44:36  bowe
 * 	Check that name does not exist on create. [CR 10702]
 * 	[1994/06/13  20:14:53  bowe]
 * 
 * Revision 1.1.2.9  1994/05/26  19:42:06  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:35  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/12  19:30:25  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:24  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/11  15:32:59  bowe
 * 	Correct args to dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:31:25  bowe]
 * 
 * Revision 1.1.2.6  1994/05/10  20:15:07  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:36  rsalz]
 * 
 * Revision 1.1.2.5  1994/04/26  19:25:49  bowe
 * 	New params to dce_acl_is_client_authorized().
 * 	Set prin (owner) when creating a new entry.
 * 	[1994/04/26  19:22:41  bowe]
 * 
 * Revision 1.1.2.4  1994/04/18  15:48:13  bowe
 * 	Added a missing '}'.
 * 	[1994/04/18  15:47:55  bowe]
 * 
 * Revision 1.1.2.3  1994/04/18  15:05:39  bowe
 * 	When creating, first check if object exists (and report error).
 * 	In all ops, if handle is NULL, skip ACL check.
 * 	[1994/04/18  15:04:49  bowe]
 * 
 * Revision 1.1.2.2  1994/04/06  21:48:08  bowe
 * 	Implemented the attr_schema operations
 * 	[1994/04/06  21:47:49  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:42  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:16  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>
#include <asimpl.h>

/* ______________________________________________________________________ */

static rs_cache_data_t		s_null_cache_data;
static sec_attr_schema_entry_t	s_null_attr_schema_entry;
static rs_attr_cursor_t		s_null_cursor;
static sec_attr_twr_set_p_t	s_null_attr_twr_set;

static void
init_schema_entry(
    sec_attr_schema_entry_t	*se
)
{
    *se = s_null_attr_schema_entry;
    se->attr_encoding	 = sec_attr_enc_printstring_array;
    se->intercell_action = sec_attr_intercell_act_reject;
    se->trig_types	 = sec_attr_trig_type_none;
}

/* ______________________________________________________________________ */

void
rs_attr_schema_create_entry(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    sec_attr_schema_entry_t	*schema_entry,
  /* out */
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    uuid_t			au;
    sec_acl_t			i_acl;
    attr_schema_data_t		schema_d;
    sec_attr_acl_mgr_info_set_t	*acl_mgr_set_p; 
    sec_acl_permset_t		perms;
    uuid_t			Prin;
    int				i;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_create_entry: %s", schema_name));

    if (h != NULL && (dced_is_bootstrap || dced_is_remote_disabled)) {
	*st = sec_acl_not_authorized;
	return;
    }

    *st = error_status_ok;
    *cache_info = s_null_cache_data;

    if (!dced_authorized(h, &as_cont_type, &as_acl, &Prin, dced_perm_insert, 
                         st))
	return;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }
     
    /* See if this sch entry exists first. */
    dce_db_fetch_by_uuid(as_db, &schema_entry->attr_id, &schema_d, st);
    if (*st == error_status_ok ||
	    name_cache_find_uuid(as_name_cache, (char *)schema_entry->attr_name,
		                 &au) ) {
	*st = dced_s_already_exists;
	return;
    }

    /* First perform checks of fields of the schema_entry to be created */

    /* Determine if name is legal */
    if (!VALID_SCH_ATTR_NAME(schema_entry->attr_name)) {
        *st = dced_s_attr_bad_name; 
        return;
    }

    /* Determine if encoding type is valid */
    if (!VALID_SCH_ENCODE_TYPE(schema_entry->attr_encoding)) {
        *st = dced_s_attr_bad_encoding_type;
        return;
    }

    /* Determine if contents of acl_mgr_set is valid */ 

    /* Make sure an acl_mgr_set was passed in before dereferencing */ 
    if (!(acl_mgr_set_p = schema_entry->acl_mgr_set)) { 
        *st = dced_s_attr_bad_acl_mgr_set; 
        return; 
    } 

    for (i=0; i<acl_mgr_set_p->num_acl_mgrs; i++) { 
        /* Make sure acl mgr_info is present before dereferencing */
        if (!(acl_mgr_set_p->mgr_info[i])) {
            *st = dced_s_attr_bad_acl_mgr_set;
            return;
        } 
        /* user other than dced itself is allowed to create scehma entry 
           only for srvrconf */
        if (h) {
            /* Collect all permset info */
            perms = (acl_mgr_set_p->mgr_info[i]->query_permset |
                     acl_mgr_set_p->mgr_info[i]->update_permset |
                     acl_mgr_set_p->mgr_info[i]->test_permset |
                     acl_mgr_set_p->mgr_info[i]->delete_permset);
            if (!uuid_equal(&acl_mgr_set_p->mgr_info[i]->acl_mgr_type,
                            &sc_obj_type, st)) { 
                *st = dced_s_attr_invalid_manager_type;
                return;
            }
            if (perms & ~(dced_sc_all_perms)) {
                *st = dced_s_attr_acl_bad_permset;
                return;
            }
        }
    }

    /* Determine if schema entry flag specified is valid */
    if (!VALID_SCH_FLAG(schema_entry->schema_entry_flags)) { 
        *st = dced_s_attr_bad_entry_flag;
        return;
    }

    /* Determine if intercell action specified is valid */
    if (!VALID_SCH_ACTION(schema_entry->intercell_action)) { 
        *st = dced_s_attr_bad_intercell_action;
        return;
    }

    /* Determine if trig type specified is valid */
    if (!VALID_SCH_TRIG_TYPES(schema_entry->trig_types)) { 
        *st = dced_s_attr_bad_trig_types;
        return;
    }

    /* If entry unique and query trigger, don't allow intercell action accept */
    if ((schema_entry->schema_entry_flags == sec_attr_sch_entry_unique) &&
        (schema_entry->trig_types == sec_attr_trig_type_query) &&
        (schema_entry->intercell_action == sec_attr_intercell_act_accept)) {
        *st = dced_s_attr_bad_uniq_query_accept;
        return;
    }
    
    /* Check to see if we have valid trigger bindings */
    if ((schema_entry->trig_types & sec_attr_trig_type_query) ||
        (schema_entry->trig_types & sec_attr_trig_type_update) ||
        (schema_entry->intercell_action == sec_attr_intercell_act_evaluate)) { 

        if (!VALID_SCH_TRIG_BINDING(schema_entry->trig_binding)) {
            *st = dced_s_attr_bad_trig_binding;
            return;
        }
    }

    /* A nil uuid indicates that the caller wants the server to generate 
      an attr type uuid */
    if (uuid_is_nil(&schema_entry->attr_id, st)) {  
         uuid_create(&schema_entry->attr_id, st);
         if (*st != error_status_ok) {
             return;
         }
    }

    /* Create an ACL for the object. */
    uuid_create(&au, st);
    CHECK_STAT_RET(*st);
    /* get initial obj acl */
    dce_db_fetch_by_uuid(acl_db, &as_do_acl, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_FETCH_IACL_MSG,
		"rs_attr_schema_create_entry", (long)*st);
	return;
    }
    dce_db_store_by_uuid(acl_db, &au, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG,
		"rs_attr_schema_create_entry", (long)*st);
	return;
    }

    dce_db_std_header_init(as_db, &schema_d.h, &schema_entry->attr_id, &au,
	                   NULL, NULL, 0, st);
    CHECK_STAT_RET(*st);
    schema_d.h.tagged_union.h.owner_id = Prin;
    schema_d.data = *schema_entry;
    dce_db_store_by_uuid(as_db, &schema_entry->attr_id, &schema_d, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG,
		"rs_attr_schema_create_entry", (long)*st);
	return;
    }

    /* Last, add to cached name->uuid mapping. */
    name_cache_add(as_name_cache, (char *)schema_entry->attr_name,
	           &schema_entry->attr_id);
}

/* ______________________________________________________________________ */

void
rs_attr_schema_delete_entry(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    uuid_t			*attr_id,
  /* out */
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    attr_schema_data_t		schema_d;
    uuid_t			Prin;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_delete_entry: %s", schema_name));

    if (h != NULL && (dced_is_bootstrap || dced_is_remote_disabled)) {
	*st = sec_acl_not_authorized;
	return;
    }

    *st = error_status_ok;
    *cache_info = s_null_cache_data;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

    /* find entry */
    dce_db_fetch_by_uuid(as_db, attr_id, &schema_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_authorized(h, &as_obj_type,
	    &schema_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_delete, st))
	return;
    /* delete acl and entry */
    dce_db_delete_by_uuid(acl_db, &schema_d.h.tagged_union.h.acl_uuid, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_DELETE_ACL_MSG,
		"rs_attr_schema_delete_entry", (long)*st);
	return;
    }
    dce_db_delete_by_uuid(as_db, attr_id, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_DELETE_OBJECT_MSG,
		"rs_attr_schema_delete_entry", (long)*st);
	return;
    }

    /* Last, delete cached name->uuid mapping. */
    name_cache_del(as_name_cache, (char *)schema_d.data.attr_name);
}

/* ______________________________________________________________________ */
/* Update the modifiable fields of a schema entry.
 */

void
rs_attr_schema_update_entry(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    sec_attr_schema_entry_parts_t	modify_parts,
    sec_attr_schema_entry_t	*schema_entry,
  /* out */
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    attr_schema_data_t		schema_d;
    char			*cp;
    uuid_t			Prin;
    sec_attr_acl_mgr_info_set_t	*acl_mgr_set_p; 
    sec_acl_permset_t           perms;
    int				i;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_update_entry: %s", schema_name));

    if (h != NULL && (dced_is_bootstrap || dced_is_remote_disabled)) {
	*st = sec_acl_not_authorized;
	return;
    }

    *st = error_status_ok;
    *cache_info = s_null_cache_data;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

    /* Fetch schema entry from database */
    dce_db_fetch_by_uuid(as_db, &schema_entry->attr_id,  &schema_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_authorized(h, &as_obj_type,
	    &schema_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_write, st))
	return;

    /* There is a bit saying if we can update each of these fields. */
    if (modify_parts & sec_attr_schema_part_name) {
        /* Determine if name is legal */
        if (!VALID_SCH_ATTR_NAME(schema_entry->attr_name)) {
            *st = dced_s_attr_bad_name; 
            return;
        }
	cp = (char *)schema_d.data.attr_name;
	schema_d.data.attr_name = schema_entry->attr_name;
    }

    if (modify_parts & sec_attr_schema_part_acl_mgrs) {
        /* Make sure an acl_mgr_set was passed in before dereferencing */ 
        if (!(acl_mgr_set_p = schema_entry->acl_mgr_set)) { 
            *st = dced_s_attr_bad_acl_mgr_set; 
            return; 
        } 

        for (i=0; i<acl_mgr_set_p->num_acl_mgrs; i++) { 
            /* Make sure acl mgr_info is present before dereferencing */
            if (!(acl_mgr_set_p->mgr_info[i])) {
                *st = dced_s_attr_bad_acl_mgr_set;
                return;
            } 
            /* user other than dced itself is allowed to create scehma entry 
               only for srvrconf */
            if (h) {
                /* Collect all permset info */
                perms = (acl_mgr_set_p->mgr_info[i]->query_permset |
                         acl_mgr_set_p->mgr_info[i]->update_permset |
                         acl_mgr_set_p->mgr_info[i]->test_permset |
                         acl_mgr_set_p->mgr_info[i]->delete_permset);
                if (!uuid_equal(&acl_mgr_set_p->mgr_info[i]->acl_mgr_type,
                                &sc_obj_type, st)) { 
                    *st = dced_s_attr_invalid_manager_type;
                    return;
                }
                if (perms & ~(dced_sc_all_perms)) {
                    *st = dced_s_attr_acl_bad_permset;
                    return;
                }
            }
        }

	schema_d.data.acl_mgr_set = schema_entry->acl_mgr_set;
    }

    if (modify_parts & sec_attr_schema_part_unique) {
        *st = dced_s_attr_not_supported_for_update;
        return;
    }
    if ((modify_parts & sec_attr_schema_part_reserved) ||
	(modify_parts & sec_attr_schema_part_defaults)) { 
         /* Determine if schema entry flag specified is valid */
         if (!VALID_SCH_FLAG(schema_entry->schema_entry_flags)) { 
             *st = dced_s_attr_bad_entry_flag;
             return;
         }

        schema_d.data.schema_entry_flags = schema_entry->schema_entry_flags;
    }

    if (modify_parts & sec_attr_schema_part_intercell) {
        /* Determine if intercell action specified is valid */
        if (!VALID_SCH_ACTION(schema_entry->intercell_action)) { 
            *st = dced_s_attr_bad_intercell_action;
            return;
        }
        
	schema_d.data.intercell_action = schema_entry->intercell_action;
    }

    /* If entry unique and query trigger, don't allow intercell action accept */
    if ((schema_d.data.schema_entry_flags == sec_attr_sch_entry_unique) &&
        (schema_d.data.trig_types == sec_attr_trig_type_query) &&
        (schema_d.data.intercell_action == sec_attr_intercell_act_accept)) {
        *st = dced_s_attr_bad_uniq_query_accept;
        return;
    }

    if (modify_parts & sec_attr_schema_part_trig_types) {
        *st = dced_s_attr_not_supported_for_update;
        return;
    }

    if (modify_parts & sec_attr_schema_part_trig_bind) {
        /* Check to see if we have valid trigger bindings */
        if ((schema_d.data.trig_types & sec_attr_trig_type_query) ||
            (schema_d.data.trig_types & sec_attr_trig_type_update) ||
            (schema_d.data.intercell_action == sec_attr_intercell_act_evaluate))
        { 

            if (!VALID_SCH_TRIG_BINDING(schema_entry->trig_binding)) {
                *st = dced_s_attr_bad_trig_binding;
                return;
            }
        }

	schema_d.data.trig_binding = schema_entry->trig_binding;
    }

    if (modify_parts & sec_attr_schema_part_comment) {
	schema_d.data.comment = schema_entry->comment;
    }

    dce_db_store_by_uuid(as_db, &schema_entry->attr_id, &schema_d, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG,
		"rs_attr_schema_update_entry", (long)*st);
	return;
    }

    /* Remove old name from cache, and add new name. */
    if (modify_parts & sec_attr_schema_part_name) {
	name_cache_del(as_name_cache, cp);
	name_cache_add(as_name_cache, (char *)schema_entry->attr_name,
		&schema_entry->attr_id);
    }
}

/* ______________________________________________________________________ */

void
rs_attr_schema_cursor_init(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
  /* out */
    unsigned32			*cur_num_entries,
    rs_attr_cursor_t		*cursor,
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_cursor_init: %s", schema_name));
    *st = error_status_ok;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }
    *cache_info = s_null_cache_data;
    *cursor     = s_null_cursor;
    cursor->source = dced_g_info.thishost_prin_uuid;
    dce_db_inq_count(as_db, cur_num_entries, st);
    cursor->num_entries_left = *cur_num_entries;
    cursor->valid = TRUE;
}

/* ______________________________________________________________________ */

void
rs_attr_schema_scan(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    rs_attr_cursor_t		*cursor,
    unsigned32			num_to_read,
  /* out */
    unsigned32			*num_read,
    sec_attr_schema_entry_t	schema_entries[],
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    uuid_t			*up;
    error_status_t		st2;
    attr_schema_data_t		schema_d;
    unsigned32			n;
    unsigned32			i;
    uuid_t			Prin;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_scan: %s, N=%d", schema_name, num_to_read));

    *st = error_status_ok;
    *cache_info = s_null_cache_data;
    *num_read = 0;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

    if (!dced_authorized(h, &as_cont_type, &as_acl, &Prin, dced_perm_read, st))
	return;

    dce_db_inq_count(as_db, &n, st);
    CHECK_STAT_RET(*st);

    dce_db_iter_start(as_db, st);
    CHECK_STAT_RET(*st);
    for (i=0; ; i++) {
	if (*num_read >= num_to_read) break;
	dce_db_iter_next_by_uuid(as_db, &up, st);
	if (*st == db_s_no_more) {
	    *st = error_status_ok;
	    break;
	}
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_ITER_FAIL_MSG, "rs_attr_schema_scan",
		(long)*st);
	    dce_db_iter_done(as_db, &st2);
	    return;
	}
	/* see if we're skipping any entries */
	if (cursor->entry > i) continue;
	dce_db_fetch_by_uuid(as_db, up, &schema_d, st);
	if (*st != error_status_ok) {
	    dce_db_iter_done(as_db, &st2);
	    return;
	}
	schema_entries[*num_read] = schema_d.data;
	(*num_read)++;
    }
    dce_db_iter_done(as_db, st);
    cursor->num_entries_left = n - i;
}

/* ______________________________________________________________________ */

void
rs_attr_schema_lookup_by_name(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    idl_char			*attr_name,
  /* out */
    sec_attr_schema_entry_t	*schema_entry,
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    attr_schema_data_t		schema_d;
    uuid_t			sch_uuid;
    uuid_t			Prin;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_lookup_by_name: %s / %s", schema_name,attr_name));
    *st = error_status_ok;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

    *cache_info = s_null_cache_data;
    init_schema_entry(schema_entry);

    /* Fetch schema entry from database */
    if (name_cache_find_uuid(as_name_cache, (char *)attr_name, &sch_uuid))
	dce_db_fetch_by_uuid(as_db, &sch_uuid, &schema_d, st);
    else
	*st = db_s_key_not_found;
    CHECK_STAT_RET(*st);

    if (!dced_authorized(h, &as_obj_type,
	    &schema_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_read, st))
	return;
    *schema_entry = schema_d.data;
}

/* ______________________________________________________________________ */

void
rs_attr_schema_lookup_by_id(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    uuid_t			*attr_id,
  /* out */
    sec_attr_schema_entry_t	*schema_entry,
    rs_cache_data_t		*cache_info,
    error_status_t		*st
)
{
    attr_schema_data_t		schema_d;
    uuid_t			Prin;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_lookup_by_id: %s", schema_name));
    *st = error_status_ok;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

    *cache_info = s_null_cache_data;
    init_schema_entry(schema_entry);

    dce_db_fetch_by_uuid(as_db, attr_id, &schema_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_authorized(h, &as_obj_type,
	    &schema_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_read, st))
	return;

    *schema_entry = schema_d.data;
}

/* ______________________________________________________________________ */

void
rs_attr_schema_get_referral(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    uuid_t			*attr_id,
  /* out */
    sec_attr_twr_set_p_t	*towers,
    error_status_t		*st
)
{
    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_get_referral: %s", schema_name));
    *towers = s_null_attr_twr_set;
    *st = error_status_ok;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

}

void
rs_attr_schema_get_acl_mgrs(
  /* in */
    handle_t			h,
    sec_attr_component_name_t	schema_name,
    unsigned32			size_avail,
  /* out */
    unsigned32                  *size_used, 
    unsigned32                  *num_acl_mgr_types,
    uuid_t                      acl_mgr_types[],
    rs_cache_data_t             *cache_info,
    error_status_t              *st
)
{
    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_get_acl_mgrs: %s", schema_name));
    *size_used = 0;
    *st = dced_s_no_support;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

}

void
rs_attr_schema_aclmgr_strings(
  /* in */
    handle_t                    h,
    sec_attr_component_name_t   schema_name,
    uuid_t                      *acl_mgr_type,
    unsigned32                  size_avail,
  /* out */
    uuid_t                      *acl_mgr_type_chain,
    sec_acl_printstring_t       *acl_mgr_info,
    boolean32                   *tokenize,
    unsigned32                  *total_num_printstrings,
    unsigned32                  *size_used,
    sec_acl_printstring_t       permstrings[],
    rs_cache_data_t             *cache_info,
    error_status_t              *st
)
{
    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_xattrschema, svc_c_debug1,
	"ENTER rs_attr_schema_aclmgr_strings: %s", schema_name));
    *size_used = 0;
    *st = dced_s_no_support;

    /* See if this req. schema (object) name is supported. */
    if ((h) && (!VALID_ATTR_SCH_NAME(schema_name))) {
        *st = dced_s_attr_bad_sch_name; 
        return;
    }

}

rs_attr_schema_v0_0_epv_t dced_rs_attr_schema_v0_0_epv = {
    rs_attr_schema_create_entry,
    rs_attr_schema_delete_entry,
    rs_attr_schema_update_entry,
    rs_attr_schema_cursor_init,
    rs_attr_schema_scan,
    rs_attr_schema_lookup_by_name,
    rs_attr_schema_lookup_by_id,
    rs_attr_schema_get_referral,
    rs_attr_schema_get_acl_mgrs,
    rs_attr_schema_aclmgr_strings
};
