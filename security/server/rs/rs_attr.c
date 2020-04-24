/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_attr.c,v $
 * Revision 1.1.10.1  1996/08/09  12:09:04  arvind
 * 	Modify rs_attr_lookup_by_id() and rs_attr_lookup_no_expand() to
 * 	return all entries that the caller is authorized to get. Do not
 * 	fail if entries are found that the caller is not auth'ed to get.
 * 	Return success even if 0 entries are returned.
 * 	[1996/05/17  17:36 UTC  jrr  /main/HPDCE02/jrr_davis_3/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/16  15:20 UTC  aha  /main/aha_pk6/4]
 *
 * 	Include sec_pk.h for sec_pk_attr_write_setup()
 * 	[1996/06/19  20:33 UTC  aha  /main/aha_pk6/3]
 *
 * 	Changes for Public Key Login; fix rs_attr_update_verify_under_RIW()
 * 	[1996/02/18  00:17:27  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/02/18  00:17:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:28  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:50:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/16  15:46 UTC  mullan_s
 * 	Initial HPDCE02 Branch.
 * 	Merge fix for CHFts16628.
 * 
 * 	HP revision /main/mullan_chfts16628/1  1995/10/16  15:37 UTC  mullan_s
 * 	CHFts16628: rs_attr_delete calling rs_log_attr_update not
 * 	rs_log_attr_delete
 * 	[1995/12/08  17:20:13  root]
 * 
 * Revision 1.1.2.9  1994/08/12  17:37:43  mdf
 * 	Hewlett Packard Security Drop
 * 	[1994/08/12  14:15:37  mdf]
 * 
 * Revision 1.1.2.8  1994/08/08  19:59:34  mdf
 * 	CR11600, fix inconsistant calling of rsdb_attr_schema_lookup_by_name.
 * 	Changed NULL to rpc_ss_allocate, and removed the free_sch_entry_ptrs.
 * 	[1994/08/08  19:45:46  mdf]
 * 
 * Revision 1.1.2.7  1994/08/08  14:23:31  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:23:43  mdf]
 * 
 * Revision 1.1.2.5  1994/06/17  18:42:45  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:15:36  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/02  21:24:54  mdf
 * 	hp_sec_to_osf_3 drop, fix merge problems.
 * 	[1994/05/26  13:50:02  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:04  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.2  1994/04/22  19:13:59  rps
 * 	Add named_item_type differentiator to rsdb_util_get_obj_info_by_name.
 * 	Currently it's always pgo; dirs may support attributes in the future.
 * 	[1994/04/21  00:11:11  rps]
 * 
 * Revision 1.1.2.1  1994/03/02  17:55:38  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:37:09  hanfei]
 * 
 * $EndLog$
 */
/*
 * @OSF_COPYRIGHT@
 */
#include <dce/sec_attr_util.h>

#include <rs_attr.h>
#include <rs_ch_lock.h>
#include <rs_auth.h>
#include <rs_util.h>
#include <rs_attr_pvt.h>

#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_create.h>
#include <rsdb_attr.h>
#include <rsdb_attr_schema.h>

#include <sec_pk.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/* PRIVATE FUNCTIONS
 */

/* PUBLIC FUNCTIONS
 */

/* r s _ a t t r _ c u r s o r _ i n i t 
 *
 * Access Control: caller must have any (at least one, doesn't
 * matter which) permission on object named component_name.
 */
void rs_attr_cursor_init
(
    handle_t			h,		/* [in] */
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			*cur_num_attrs,	/* [out] */
    rs_attr_cursor_t		*cursor,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    sec_acl_permset_t		requested_perms = NULL;

    BEGIN_READ(lock_db, st_p) {
        rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type, 
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);
        if(STATUS_OK(st_p)) {
	    if(rs_attr_util_is_auth(h, &rgy_obj_info, requested_perms, 
	        component_name, st_p)) {
	        rsdb_attr_cursor_init(&rgy_obj_info, cur_num_attrs, 
		    cursor, st_p);
	    }
        }
    } END_READ

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


/* r s _ a t t r _ l o o k u p _ b y _ i d
 *
 * INPUT
 *      num_attr_keys - number of attribute keys in attr_keys[]
 *      space_avail - size of the output attrs[] array.
 *      attr_keys - array of sec_attr_t attribute keys containing,
 *              in the attr_id field, the attribute type uuid of the
 *              attribute instance(s) to be retrieved.  The attribute
 *              encoding type in the attr_value field should normally
 *              be set to sec_attr_enc_void.
 *              Query Trigger: if the requested attribute type is
 *              associated with a query trigger, the attr_value field
 *              may be used to pass in optional information if
 *              required by the trigger.  In this case, the encoding
 *              type should be set to reflect the type of data passed in.
 * INPUT/OUTPUT:
 *      cursor - [in] identifies the next attribute instance(s)
 *              on this object to return in attrs[].
 *              [out] points to the next instance(s) to be returned
 *              in a subsequent call.
 * OUTPUT:
 *      num_returned - specifies the number of attribute instances
 *              returned in the attrs[] array.
 *      attrs - array of attribute instances retrieved by id.
 *      num_left - hints at the number of attributes matching the
 *              input attr_keys that could not be returned due
 *              to space constraints in the attrs buffer.
 *
 * Access Control: caller must have, for each attr type in
 * attr_keys, the query_permset permissions on the object
 * named by component_name.
 */
void rs_attr_lookup_by_id
(
    handle_t			h,
    sec_attr_component_name_t	component_name,	/* [in] */
    rs_attr_cursor_t		*cursor,	/* [in, out] */
    unsigned32			num_attr_keys,	/* [in] */
    unsigned32			space_avail,	/* [in] */
    sec_attr_t			attr_keys[],	/* [in] */
    unsigned32			*num_returned,	/* [out] */
    sec_attr_t			attrs[],	/* [out] */
    unsigned32			*num_left,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    sec_acl_permset_t		requested_perms;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32			*xref = NULL;
    signed32			failure_index;
    signed32			i;

#if defined(DCE_AUD_POINTS)
    unsigned32                    aud_count;
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);
    *num_returned = 0;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_LookupById, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = num_attr_keys;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        /* only record each attribute's uuid type */
        for (aud_count = 0; aud_count < num_attr_keys; aud_count++) {
            ev_info.format = aud_c_evt_info_uuid;
            ev_info.data.uuid = attr_keys[aud_count].attr_id;
            dce_aud_put_ev_info(ard, ev_info, &aud_status);
        }

    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, st_p) {
	/* translate the object name into rgy_obj_info for use
	 * in following rsdb_attr calls
	 */
	rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type,
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);

	/* make sure the cursor is valid, initializing it if necessary */
	if(STATUS_OK(st_p)) {
	    rsdb_attr_cursor_check(&rgy_obj_info, cursor, st_p);
	}
	
	if(STATUS_OK(st_p)) {
	    rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
		num_attr_keys, attr_keys, &rgy_obj_info, &sch_list,
		&xref, &failure_index, st_p);
	}
	/* Check if caller is authorized for queries of each attr type */
	for(i = 0; STATUS_OK(st_p) && i < sch_list->count; i++) {
	    rs_attr_util_get_permset(&rgy_obj_info,
		&(sch_list->entries[i].sch_entry),
		rs_attr_op_query, &requested_perms, st_p);
	    if(STATUS_OK(st_p)) {
		rs_attr_util_is_auth(h, &rgy_obj_info, 
		    requested_perms, component_name, st_p);
	    }

            /* 
             * If the caller is not authorized to lookup this entry,
             * just mark it as deleted and clear the status.
             */
            if (*st_p == sec_attr_unauthorized ) {
                sch_list->entries[i].flags.deleted = 1;
                CLEAR_STATUS(st_p);
            }                     
        }

	if(STATUS_OK(st_p)) {
	    rsdb_attr_lookup_by_uuid(&rgy_obj_info, cursor, 
		space_avail, sch_list,
		rpc_ss_allocate, num_returned, attrs, 
		num_left, st_p);
	}
    } END_READ

    if(sch_list) {
	/* free internal ptrs of each schema_entry allocated */
	for(i = 0; i < sch_list->count; i++) {
	    sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
	}
	free(sch_list);
    }
    if(xref)
	free(xref);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                           1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
    return;
}


/* r s _ a t t r _ l o o k u p _ n o _ e x p a n d
 *
 * Access Control: caller must have, for each attr type in
 * attr_keys, the query_permset permissions on the object
 * named by component_name.
 */
void rs_attr_lookup_no_expand
(
    handle_t			h,
    sec_attr_component_name_t	component_name,	/* [in] */
    rs_attr_cursor_t		*cursor,	/* [in, out] */
    unsigned32			num_attr_keys,	/* [in] */
    unsigned32			space_avail,	/* [in] */
    sec_attr_t			attr_keys[],	/* [in] */
    unsigned32			*num_returned,	/* [out] */
    sec_attr_t			attr_sets[],	/* [out] */
    unsigned32			*num_left,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    sec_acl_permset_t		requested_perms;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32			*xref = NULL;
    signed32			failure_index;
    signed32			i;

#if defined(DCE_AUD_POINTS)
    signed32                    aud_count;
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);
    *num_returned = 0;

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_LookupNoExpand, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = num_attr_keys;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        /* only record each attribute's uuid type */
        for (aud_count = 0; aud_count < num_attr_keys; aud_count++) {
            ev_info.format = aud_c_evt_info_uuid;
            ev_info.data.uuid = attr_keys[aud_count].attr_id;
            dce_aud_put_ev_info(ard, ev_info, &aud_status);
        }

    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, st_p) {
	/* translate the object name into rgy_obj_info for use
	 * in following rsdb_attr calls
	 */
	rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type,
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);

	/* make sure the cursor is valid, initializing it if necessary */
	if(STATUS_OK(st_p)) {
	    rsdb_attr_cursor_check(&rgy_obj_info, cursor, st_p);
	}
	
	if(STATUS_OK(st_p)) {
	    rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
		num_attr_keys, attr_keys, &rgy_obj_info, &sch_list, &xref, 
		&failure_index, st_p);
	}
	/* Check if caller is authorized for queries of each attr type */
	for(i = 0; STATUS_OK(st_p) && i < sch_list->count ; i++) {
	    rs_attr_util_get_permset(&rgy_obj_info,
		&(sch_list->entries[i].sch_entry),
		rs_attr_op_query, &requested_perms, st_p);
	    if(STATUS_OK(st_p)) {
		rs_attr_util_is_auth(h, &rgy_obj_info, 
		    requested_perms, component_name, st_p);
	    }

            /* 
             * If the caller is not authorized to lookup this entry,
             * just mark it as deleted and clear the status.
             */
            if (*st_p == sec_attr_unauthorized ) {
                sch_list->entries[i].flags.deleted = 1;
                CLEAR_STATUS(st_p);
            }                     
        }

	if(STATUS_OK(st_p)) {
	    rsdb_attr_lookup_by_uuid(&rgy_obj_info, cursor, 
		space_avail, sch_list,
		rpc_ss_allocate, num_returned, attr_sets, 
		num_left, st_p);
	}
    } END_READ

    if(sch_list) {
	/* free internal ptrs of each schema_entry allocated */
	for(i = 0; i < sch_list->count; i++) {
	    sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
	}
	free(sch_list);
    }
    if(xref)
	free(xref);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                           1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
    return;
}


/* r s _ a t t r _ l o o k u p _ b y _ n a m e
 *
 * Access Control: caller must have, for the attr type named
 * by attr_name, the query_permset permissions on the object
 * named by component_name.
 */
void rs_attr_lookup_by_name
(
    handle_t			h,
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned_char_t		*attr_name,	/* [in] */
    sec_attr_t			*attr_p,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    sec_attr_schema_entry_t	sch_entry;
    sec_acl_permset_t		requested_perms;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_LookupByName, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = attr_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_READ(lock_db, st_p) {
	rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type,
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);
	if(STATUS_OK(st_p)) {
	    rsdb_attr_sch_lookup_by_name((sec_attr_component_name_t)"", 
		(char *)attr_name, rpc_ss_allocate, &sch_entry, st_p);
	}
	if(STATUS_OK(st_p)) {
	    rs_attr_util_get_permset(&rgy_obj_info, &sch_entry,
		rs_attr_op_query, &requested_perms, st_p);
	}
	if(STATUS_OK(st_p)) {
	    if(rs_attr_util_is_auth(h, &rgy_obj_info, requested_perms, 
		component_name, st_p)) {
		rsdb_attr_lookup_by_name(&rgy_obj_info, (char *)attr_name,
		    rpc_ss_allocate, attr_p, st_p);
	    }
	}
    } END_READ


#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                           1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}

/*
 * r s _ a t t r _ u p d a t e _ v e r i f y _ u n d e r _ R I W
 *
 * For use only by rs_attr_update() and rs_acct_replace()
 * Must be called under db RIW lock
 */
PUBLIC void rs_attr_update_verify_under_RIW(
    handle_t			h,
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			num_to_write,	/* [in] */
    sec_attr_t			in_attrs[],	/* [in] */
    rsdb_util_rgy_obj_info_t	*rgy_obj_info_p,/* [in] */
    signed32			*failure_index,	/* [out] */
    rsdb_attr_sch_list_t        **sch_list_p_p, /* [out] */
    signed32                    **xref_p_p,     /* [out] */
    error_status_t		*st_p
)
{
    uuid_t			obj_uuid;
    sec_acl_permset_t		requested_perms;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32			*xref = NULL;
    signed32			i, j;

    rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type,
            sec_acl_type_object, rgy_obj_info_p, &obj_uuid, st_p);
    if(STATUS_OK(st_p)) {
        rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
            num_to_write, in_attrs, rgy_obj_info_p, &sch_list, &xref,
            failure_index, st_p);
    }
    /* Check if caller is authorized for updates of each attr type */
    for(i = 0; STATUS_OK(st_p) && i < sch_list->count; i++) {
        rs_attr_util_get_permset(rgy_obj_info_p,
            &(sch_list->entries[i].sch_entry),
            rs_attr_op_update, &requested_perms, st_p);
        if(STATUS_OK(st_p)) {
            if(!(rs_attr_util_is_auth(h, rgy_obj_info_p, 
                requested_perms, component_name, st_p))) {
                /* get failure index, first attr of this type */
                for(j = 0; j < num_to_write; j++) {
                    if(*(xref + j) == i) {
                        *failure_index = j;
                        break;
                    }
                }
            } /* end is_auth */
        }
    }
    /* Perform sematic check of input attributes */
    if(STATUS_OK(st_p)) {
        rsdb_attr_semantic_check(rgy_obj_info_p, num_to_write,
            in_attrs, sch_list, xref, failure_index, st_p);
    }
    *sch_list_p_p=sch_list;
    *xref_p_p=xref;
}

/*
 * r s _ a t t r _ u p d a t e 
 *
 * Access Control: caller must have, for each attr type in
 * attr_keys, the query_permset permissions on the object
 * named by component_name.
 */
PUBLIC void rs_attr_update
(
    handle_t			h,
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			num_to_write,	/* [in] */
    sec_attr_t			in_attrs[],	/* [in] */
    signed32			*failure_index,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    char			buf[sizeof(sec_rgy_name_t) + 80];
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32			*xref = NULL;
    signed32			i;

#if defined(DCE_AUD_POINTS)
    signed32                    aud_count;
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

    if (num_to_write == 0) {
	return;
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_Update, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = num_to_write;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        /* only record each attribute's uuid type */
        for (aud_count = 0; aud_count < num_to_write; aud_count++) {
            ev_info.format = aud_c_evt_info_uuid;
            ev_info.data.uuid = in_attrs[aud_count].attr_id;
            dce_aud_put_ev_info(ard, ev_info, &aud_status);
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

	
    BEGIN_MASTER_RIW(lock_db, st_p) {
        rs_attr_update_verify_under_RIW(h,
                                        component_name,
                                        num_to_write,
                                        in_attrs,
                                        &rgy_obj_info,
                                        failure_index,
                                        &sch_list,
                                        &xref,
                                        st_p);
        if(STATUS_OK(st_p)) {
            sprintf(buf, "Attempting attribute updates on object %s", 
                (char *)component_name);
            rs_audit_trail_log(h, buf, (char *)component_name);
            RIW_PROMOTE(lock_db);
            rsdb_attr_update(&rgy_obj_info, num_to_write, in_attrs,
                sch_list, xref, failure_index, st_p);
            if(STATUS_OK(st_p)) {
                sprintf(buf, "Updated attributes on object %s",
                    (char *)component_name);
                rs_audit_trail_log(h, buf, (char *)component_name);
                rs_log_attr_update(component_name, num_to_write, in_attrs, NULL_PTR);
            }
        }
    } END_MASTER_RIW

    if(sch_list) {
	/* free internal ptrs of each schema_entry allocated */
	for(i = 0; i < sch_list->count; i++) {
	    sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
	}
	free(sch_list);
    }
    if(xref)
	free(xref);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                           1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);

    return;
}


void rs_attr_test_and_update
(
    handle_t			h,		/* [in] */
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			num_to_test,	/* [in] */
    sec_attr_t			test_attrs[],	/* [in] */
    unsigned32			num_to_write,	/* [in] */
    sec_attr_t			update_attrs[],	/* [in] */
    signed32			*failure_index,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p		/* [out] */
)
{

/* An audit point should be included in this routine when it is
 * implemented.  The audit point ERA_TestUpdate is defined
 * for this code.  
 * Event specific information for this audit point are:
 *    component_name                        (aud_c_evt_info_char_string)
 *    num_to_test                           (aud_c_evt_info_ulong_int)
 *    num_to_test * test_attrs[].attr_id    (aud_c_evt_info_uuid)
 *    num_to_write                          (aud_c_evt_info_ulong_int) 
 *    num_to_write * update_attrs[].attr_id (aud_c_evt_info_uuid) 
 */

	SET_STATUS(st_p, sec_rgy_not_implemented);

}
 

void rs_attr_delete
(
    handle_t			h,		/* [in] */
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			num_to_delete,	/* [in] */
    sec_attr_t			attrs[],	/* [in] */
    signed32			*failure_index,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p		/* [out] */
)
{
    rsdb_util_rgy_obj_info_t	rgy_obj_info;
    uuid_t			obj_uuid;
    sec_acl_permset_t		requested_perms;
    char			buf[sizeof(sec_rgy_name_t) + 80];
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32			*xref = NULL;
    signed32			i, j;

#if defined(DCE_AUD_POINTS)
    signed32                    aud_count;
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);
    if (num_to_delete == 0) {
	return;
    }
	
#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_Delete, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = component_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = num_to_delete;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        /* only record each attribute's uuid type */
        for (aud_count = 0; aud_count < num_to_delete; aud_count++) {
            ev_info.format = aud_c_evt_info_uuid;
            ev_info.data.uuid = attrs[aud_count].attr_id;
            dce_aud_put_ev_info(ard, ev_info, &aud_status);
        }

    }
#endif	/* defined(DCE_AUD_POINTS) */

    BEGIN_MASTER_RIW(lock_db, st_p) {
	rsdb_util_get_obj_info_by_name(component_name, rsdb_pgo_type,
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);
	if(STATUS_OK(st_p)) {
	    rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
		num_to_delete, attrs, &rgy_obj_info, &sch_list, &xref,
		failure_index, st_p);
	}
	/* Check if caller is authorized for deletes of each attr type */
	for(i = 0; STATUS_OK(st_p) && i < sch_list->count; i++) {
	    rs_attr_util_get_permset(&rgy_obj_info,
		&(sch_list->entries[i].sch_entry),
		rs_attr_op_delete, &requested_perms, st_p);
	    if(STATUS_OK(st_p)) {
		if(!(rs_attr_util_is_auth(h, &rgy_obj_info, 
		    requested_perms, component_name, st_p))) {
		    /* get failure index, first attr of this type */
		    for(j = 0; j < num_to_delete; j++) {
			if(*(xref + j) == i) {
			    *failure_index = j;
			    break;
			}
		    }
		} /* end is_auth */
	    }
	}
	if(STATUS_OK(st_p)) {
            sprintf(buf, "Attempting attribute deletes on object %s", 
		(char *)component_name);
            rs_audit_trail_log(h, buf, (char *)component_name);
            RIW_PROMOTE(lock_db);
	    rsdb_attr_delete(&rgy_obj_info, num_to_delete, attrs,
		sch_list, xref, failure_index, st_p);
            if(STATUS_OK(st_p)) {
                sprintf(buf, "Deleted attributes on object %s",
		    (char *)component_name);
                rs_audit_trail_log(h, buf, (char *)component_name);
                rs_log_attr_delete(component_name, num_to_delete, attrs, 
		    NULL_PTR);
	    }
	}
    } END_MASTER_RIW

    if(sch_list) {
	/* free internal ptrs of each schema_entry allocated */
	for(i = 0; i < sch_list->count; i++) {
	    sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
	}
	free(sch_list);
    }
    if(xref)
	free(xref);

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                           1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
    return;
}


void rs_attr_get_referral
(
	/* [in] */ handle_t h,
	/* [in] */ sec_attr_component_name_t component_name,
	/* [in] */ uuid_t *attr_id,
	/* [out] */ sec_attr_twr_set_p_t *towers,
    		rs_cache_data_t      *cache_info,  /* [out] */
	/* [out] */ error_status_t *st_p
)
{
	SET_STATUS(st_p, sec_rgy_not_implemented);
}

/* r s _ a t t r _ g e t _ e f f e c t i v e
 *
 * Read the "effective" attributes by id.  The server
 * allocates a buffer large enough to return ALL the
 * requested attributes so that subsequent calls are
 * not necessary.
 *
 * By "effective," the following "inheritance" mechanism
 * is implied:
 * A. If the object identified by "component_name" possesses instances
 * of the requested attribute type, return them; else, do B.
 * B. If the schema_entry for the requested attribute type
 * does not have the "apply_defaults" flag set, no 
 * attribute inheritance should take place, so return with no
 * attributes of this type.  If the "apply_defaults"
 * flag *is* set, do C.
 * C. If the object is a principal associated with an account,
 * do D; otherwise, do E.
 * D. If the org associated with the principal's account
 * possesses instances of the requested attribute type, return
 * them; otherwise do E.
 * E. If the policy object possesses instances of the requested
 * attribute type, return them; otherwise, no instances of
 * this type will be returned.
 *
 * If the number of query attribute keys (num_attr_keys)
 * is 0, this function will return all attributes that
 * the caller is authorized to see.
 *
 * Multi-instanced attributes:
 * Multi-instanced attributes are returned as independent attribute
 * instances sharing the same attribute type uuid.
 * To learn the exact number of instances in a given multi-instanced
 * attribute, call this operation with only a single query id.
 * The num_returned plus the num_left will specify the total
 * number of instances for that attribute type.
 *
 * Attribute Sets:
 * Reads of an attribute set return all instances of members 
 * of that set.  The attribute set instance will not be returned.
 *
 * Triggers:
 * If the requested attribute type is associated with a query
 * trigger, the value returned for the attribute will be the
 * binding (as set in the schema entry) of the trigger server.
 * The caller must bind to the trigger server and pass the
 * original input query attribute to the sec_attr_trig_query call
 * in order to retrieve the attribute value.
 *
 *    In Parameters:
 *        component_name - identifies the registry object on which
 *            to perform this operation.
 *        num_attr_keys - specifies the number of elements in the
 *            input attr_keys array.
 *        attr_keys - in the attr_id field of each element, contains
 *            the attribute type id of the attribute instance(s)
 *            requested by this lookup. If the requested attribute
 *            type is associated with a query trigger, the attr_val
 *            field may be used to pass in optional information 
 *            required by the trigger query. If no information is
 *            to be passed in the attr_val field (whether the type
 *            indicates a trigger query or not), the attr_val
 *            encoding type should be set to
 *            sec_attr_enc_void.
 *            
 *    Out Parameters:
 *        attr_list - an attribute vector allocated by the server
 *		  containing all of the attributes matching the
 *		  search criteria.
 *
 */
void rs_attr_get_effective
(
    handle_t			h,		/* [in] */
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			num_attr_keys,	/* [in] */
    sec_attr_t			attr_keys[],	/* [in] */
    sec_attr_vec_t		*attr_list,	/* [out] */
    rs_cache_data_t		*cache_info,	/* [out] */
    error_status_t		*st_p		/* [out] */
)
{
    /* initialize attr_list array to have zero elements so that,
     * in case of error, the stub doesn't try to marshall bogus
     * data in attr_list.
     */
    attr_list->num_attrs = 0;

    SET_STATUS(st_p, sec_rgy_not_implemented);

}

