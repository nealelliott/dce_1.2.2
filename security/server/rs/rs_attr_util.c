/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_attr_util.c,v $
 * Revision 1.1.4.2  1996/02/18  00:17:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:17  root]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:48  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:15:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/02  21:24:58  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:11  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:31  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 * rs_attr_util.c: utilities useful for rs_attr_*.c
 */

#include <dce/sec_attr_tools.h>

#include <rs_auth.h>
#include <rs_attr_util.h>

#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>


/* r s _ a t t r _ u t i l _ m a p _ s t a t u s
 * 
 * Translate the input general sec status code to
 * a sec_attr status code where appropriate.
 */
PUBLIC void rs_attr_util_map_status
(
    error_status_t	*st_p	/* [in, out] */
)
{
    if(STATUS_OK(st_p))
	return;

    switch(*st_p) {
    case sec_rgy_object_not_found:
	SET_STATUS(st_p, sec_attr_rgy_obj_not_found);
	break;
    case sec_rgy_not_authorized:
	SET_STATUS(st_p, sec_attr_unauthorized);
	break;
    case sec_acl_object_not_found:
	SET_STATUS(st_p, sec_attr_rgy_obj_not_found);
	break;
    case sec_acl_unknown_manager_type:
	SET_STATUS(st_p, sec_attr_bad_acl_mgr_type);
	break;
    case sec_acl_invalid_manager_type:
	SET_STATUS(st_p, sec_attr_bad_acl_mgr_type);
	break;
    case sec_acl_not_authorized:
	SET_STATUS(st_p, sec_attr_unauthorized);
	break;
    case sec_acl_bad_permset:
	SET_STATUS(st_p, sec_attr_bad_permset);
	break;
    }
}


/* r s _ a t t r _ u t i l _ i s _ a u t h
 *
 */
PUBLIC boolean32 rs_attr_util_is_auth
(
    handle_t                    h,			/* [in] */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,		/* [in] */
    sec_acl_permset_t           requested_perms,	/* [in] */
    sec_attr_component_name_t   object_name,		/* [in] */
    error_status_t              *st_p
)
{
    uuid_t                      mgr_type_uuid;
    sec_acl_permset_t           parent_perms = NULL;
    sec_rgy_foreign_id_t        client_ids;
    sec_rgy_name_t              *object_name_p;

    object_name_p = (sec_rgy_name_t *)object_name;

    if(rsdb_acl_get_mgr_uuid_from_type(rgy_obj_info->acl_mgr_type,
        &mgr_type_uuid, st_p)) {
	rs_is_authorized(h, requested_perms, parent_perms,
            *object_name_p, &mgr_type_uuid, false, &client_ids, st_p);
    }
    if(BAD_STATUS(st_p)) {
	rs_attr_util_map_status(st_p);
	return false;
    }
    return true;
}


/* r s _ a t t r _ u t i l _ g e t _ p e r m s e t
 *
 * Search the input schema entry for the permset corresponding
 * to the input operation and acl_mgr_type.
 * If there is no acl_mgr_type in the schema_entry that
 * matches the acl_mgr_type for the object, then return bad_object_type.
 * 
 *
 * Status codes:
 *	sec_attr_bad_acl_mgr_set
 *	sec_attr_bad_acl_mgr_type
 *	sec_attr_bad_object_type
 */
PUBLIC void rs_attr_util_get_permset
(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,		/* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,	/* [in] */
    rs_attr_operation_t		operation,		/* [in] */
    sec_acl_permset_t           *permset,		/* [out] */
    error_status_t              *st_p
)
{
    uuid_t                      mgr_type_uuid;
    int				i;

    CLEAR_STATUS(st_p);

    if(!rsdb_acl_get_mgr_uuid_from_type(rgy_obj_info->acl_mgr_type,
        &mgr_type_uuid, st_p)) {
	rs_attr_util_map_status(st_p);
	return;
    }
    if(!(SA_ACL_MGR_SET_P(sec_sch_entry_p))) {
        SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
        return;
    }
    for(i = 0; i < SA_ACL_MGR_NUM(sec_sch_entry_p); i++) {
	if(uuid_equal(&mgr_type_uuid, 
	    &(SA_ACL_MGR_TYPE(sec_sch_entry_p, i)), st_p)) {
	    switch(operation) {
	    case rs_attr_op_query:
		*permset = SA_ACL_MGR_PERMS_QUERY(sec_sch_entry_p, i);
		break;
	    case rs_attr_op_update:
		*permset = SA_ACL_MGR_PERMS_UPDATE(sec_sch_entry_p, i);
		break;
	    case rs_attr_op_test:
		*permset = SA_ACL_MGR_PERMS_TEST(sec_sch_entry_p, i);
		break;
	    case rs_attr_op_delete:
		*permset = SA_ACL_MGR_PERMS_DELETE(sec_sch_entry_p, i);
		break;
	    default:
		SET_STATUS(st_p, sec_attr_not_implemented);
	    }
	    return;
	}
    }
    /* If there were no acl_mgr_type in the schema_entry that
     * matched the acl_mgr_type for the object, then attributes
     * of this type are not supported on this object type.
     */
    SET_STATUS(st_p, sec_attr_bad_object_type);
    return;
}

PUBLIC void rs_attr_util_pre_update
(
    handle_t			h,			/* in */
    sec_attr_component_name_t	component_name,		/* in */
    unsigned32			num_to_write,		/* in */
    sec_attr_t			in_attrs[],		/* in */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info, 		/* out */
    rsdb_attr_sch_list_t        **sch_list, 		/* out */
    signed32			**xref,	 		/* out */
    signed32			*failure_index,	 	/* out */
    error_status_t		*st_p 			/* out */
)
{
    uuid_t			obj_uuid;
    sec_acl_permset_t		requested_perms;
    signed32 			i, j;

    *xref = NULL;
    *sch_list = NULL;
    rsdb_util_get_obj_info_by_name(component_name, 
	    rsdb_pgo_type, sec_acl_type_object, rgy_obj_info, &obj_uuid, st_p);
    if(STATUS_OK(st_p)) {
	    rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
		num_to_write, in_attrs, rgy_obj_info, &(*sch_list), &(*xref),
		failure_index, st_p);
    }
    /* Check if caller is authorized for updates of each attr type */
    for(i = 0; STATUS_OK(st_p) && i < (*sch_list)->count; i++) {
        rs_attr_util_get_permset(rgy_obj_info,
		&((*sch_list)->entries[i].sch_entry),
		rs_attr_op_update, &requested_perms, st_p);
	if(STATUS_OK(st_p)) {
	    if(!(rs_attr_util_is_auth(h, rgy_obj_info, 
	          requested_perms, component_name, st_p))) {
		/* get failure index, first attr of this type */
		for(j = 0; j < num_to_write; j++) {
		    if(*((*xref) + j) == i) {
		    	*failure_index = j;
			break;
		    }
		}
	    } /* end is_auth */
	}
    }
}
