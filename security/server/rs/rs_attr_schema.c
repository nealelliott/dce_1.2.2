/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_attr_schema.c,v $
 * Revision 1.1.13.1  1996/10/15  20:54:50  arvind
 * 	DCE_1.2.2 branch
 *
 * 	Change to allow acl_mgr permset change
 * 	[1996/10/09  18:24 UTC  cuti  /main/cuti_pk_fix/1]
 *
 * Revision 1.1.11.3  1996/02/18  00:17:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:30  marty]
 * 
 * Revision 1.1.11.2  1995/12/08  17:50:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/11  19:38 UTC  cuti
 * 	"CHFts15650"
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra_fix/2  1995/07/11  19:26 UTC  cuti
 * 	CHFts15650: Initialize acl_mgr_info
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra_fix/1  1995/06/20  16:00 UTC  cuti
 * 	Initialize the return param to 0 or NULL to avoid coredump while transmitting
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/01  18:55 UTC  mdf
 * 	Resolve CHFts15067
 * 	[1995/04/28  13:22 UTC  mdf  /main/mdf_mothra_defects_3/1]
 * 
 * Revision 1.1.7.2  1994/10/25  21:14:01  burati
 * 	CR12713 Attribute schema updates not logged or propagated
 * 	[1994/10/25  19:55:36  burati]
 * 
 * Revision 1.1.7.1  1994/10/17  18:18:19  cuti
 * 	implement update_entry, modularize and improve semantic checking
 * 	[1994/10/14  16:16:12  cuti]
 * 
 * Revision 1.1.2.10  1994/08/16  10:36:48  mdf
 * 	Bug fixes from Hewlett Packard
 * 	[1994/08/15  19:51:29  mdf]
 * 
 * Revision 1.1.2.8  1994/08/08  14:23:32  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:23:47  mdf]
 * 
 * Revision 1.1.2.5  1994/06/17  18:42:46  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:15:37  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/02  21:24:56  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:08  mdf]
 * 
 * Revision 1.1.2.3  1994/05/11  19:29:18  ahop
 * 	hp_sec_to_osf_2 drop
 * 	 Logging support of the create and delete ERA functions
 * 	[1994/04/29  21:42:32  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1993, 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 *	Registry Server - Schema Management
 */

#include <dce/rpc.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>

#include <rs_attr_schema.h>
#include <rs_attr_util.h>
#include <rs_ch_lock.h>
#include <rs_auth.h>

#include <rsdb_attr_schema.h>
#include <rsdb_attr_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_misc.h>
#include <rsdb_name_util.h>

#include <macros.h>
#include <u_str.h>
#include <Pointer.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */


PRIVATE uuid_t  *attr_schema_mgr_type = &(rsdb_acl_mgr_uuids
                                              [rsdb_acl_mgr_type_attr_schema]);

PRIVATE sec_rgy_name_t *object_name_p = 
			(sec_rgy_name_t *)SEC_ATTR_SCHEMA_OBJ_NAME;

/* 
 * PRIVATE OPERATIONS
 */

/*
 * r s _ a t t r _ s c h e m a _ b i n d i n g _ c h e c k
 *
 * Check the trig_binding field of the input schema_entry for correctness.
 * This routine should be called only if the input schema entry should
 * contain a trig_binding for update.  Return true if the trig_binding 
 * is valid for update.  Return false if bad status is returned.
 *
 * In Parameters:
 *	schema_entry - a schema entry whose fields are being checked
 *		for semantic correctness before an update.
 * 		       
 * 		       
 */
PRIVATE boolean32 rs_attr_schema_binding_check
(
    sec_attr_schema_entry_t         *schema_entry,	/* [in] */ 
    error_status_t                  *st_p		/* [out] */ 
)
{
    signed32			i, j;

    /* determine if bind_info is present before dereferencing
     * bind_info ptr.
     */
    if(!(SA_TRG_BND_INFO_P(schema_entry))) {
	SET_STATUS(st_p, sec_attr_trig_bind_info_missing);
	return false;
    }
    if( SA_TRG_BND_NUM(schema_entry) < 1) {
	SET_STATUS(st_p, sec_attr_trig_bind_info_missing);
	return false;
    }

    /* If DCE authentication is being used, check auth_info parameters */
    if( SA_TRG_BND_AUTH_INFO_TYPE(schema_entry) == sec_attr_bind_auth_dce) {
        /* Validate server principal name */                        
        if(!(SA_SCH_STRING_VALID(SA_TRG_BND_AUTH_SVR_PNAME_P(schema_entry),
	     RS_SCH_STRING_MAX_LEN))) {
            SET_STATUS(st_p, sec_attr_bad_bind_svr_name);
	    return false;
	}
    
        /* verify prot_level */
        if(!(SA_BND_AUTH_PROT_LEV_VALID(SA_TRG_BND_INFO_P(schema_entry)))) {
            SET_STATUS(st_p, sec_attr_bad_bind_prot_level);
	    return false;
	}
    
        /* verify authn_svc */
        if(!(SA_BND_AUTH_AUTHN_SVC_VALID(SA_TRG_BND_INFO_P(schema_entry)))) {
            SET_STATUS(st_p, sec_attr_bad_bind_authn_svc);
	    return false;
	}

        /* verify authz_svc */
        if(!(SA_BND_AUTH_AUTHZ_SVC_VALID(SA_TRG_BND_INFO_P(schema_entry)))) {
            SET_STATUS(st_p, sec_attr_bad_bind_authz_svc);
	    return false;
	}
    } /* End binding auth_info validation */

    /* Validate the binding information */
    /* Currently, the only check is to verify that pointers
     * are not null.  The binding info referenced by the pointers
     * is not examined.
     */
    for(i = 0; i < SA_TRG_BND_NUM(schema_entry); i++) {
        switch (SA_TRG_BND_TYPE(schema_entry, i)) {
            case sec_attr_bind_type_string:
                /* verify string */
		if(!SA_TRG_BND_STRING_P(schema_entry, i)) {
                    SET_STATUS(st_p, sec_attr_bad_bind_info);
		    return false;
		}
                break;

            case sec_attr_bind_type_twrs:
                /* verify towers */
		if(!SA_TRG_BND_TWRSET_P(schema_entry, i)) {
                    SET_STATUS(st_p, sec_attr_bad_bind_info);
		    return false;
		}
                for(j = 0; j < SA_TRG_BND_TWRSET_COUNT(schema_entry, i);
                    j++) {
		    if(!SA_TRG_BND_TWR_P(schema_entry, i, j)) {
                        SET_STATUS(st_p, sec_attr_bad_bind_info);
		        return false;
		    }
		}
                break;

            case sec_attr_bind_type_svrname:
                /* verify svrname */
		if(!SA_TRG_BND_SVRNAME_P(schema_entry, i)) {
                    SET_STATUS(st_p, sec_attr_bad_bind_info);
		    return false;
		}
		if(!SA_TRG_BND_SVRNAME_NAME_P(schema_entry, i)) {
                    SET_STATUS(st_p, sec_attr_bad_bind_info);
		    return false;
		}
                break;

            default:
                SET_STATUS(st_p, sec_attr_bad_bind_info);
		return false;
        } /* end switch */
    } /* end for each binding */

    /* all checks succeeded */
    return true;
}


/*
 * r s _ a t t r _ s c h e m a _ a c l _ m g r _ c h e c k
 *
 * Check the acl_mgr_set field of the input schema_entry for correctness.
 * This routine should be called only if the input schema entry contains
 * an acl_mgr_set for update.  Return true if the acl_mgr_set is valid
 * for update.  Return false if bad status is returned.
 *
 * In Parameters:
 *	schema_entry - a schema entry whose fields are being checked
 *		for semantic correctness before an update.
 * 		       
 *	old_schema_entry - the old version of the schema entry whose 
 *		acl_mgr_set is required for comparison checking with
 *		the new schema_entry.  If comparison_required is false,
 *		this parameter should be NULL and won't be accessed.
 *
 *	comparison_required - if true, compare the new acl_mgr_set
 *		in schema_entry with the existing acl_mgr_set in
 *		old_schema_entry for restricted modifications
 * 		       
 */
PRIVATE boolean32 rs_attr_schema_acl_mgr_check
(
    sec_attr_schema_entry_t         *schema_entry,	/* [in] */ 
    sec_attr_schema_entry_t         *old_schema_entry,	/* [in] */ 
    boolean32			    comparison_required,/* [in] */
    error_status_t                  *st_p		/* [out] */ 
)
{
    signed32			i, j;
    sec_acl_permset_t		perms;
    boolean32			*acl_mgr_checked = NULL;


    CLEAR_STATUS(st_p);

    /* Make sure an acl_mgr_set was passed in before dereferencing */
    if(!(SA_ACL_MGR_SET_P(schema_entry))) {
        SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
        return false;
    }

    /* Setup acl_mgr_checked array for comparison checking
     * with old schema entry if necessary.
     */
    if(comparison_required) {
        if(!(acl_mgr_checked = RSDB_ATTR_ALLOCATE(rpc_ss_allocate,
	    SA_ACL_MGR_NUM(old_schema_entry) * sizeof(boolean32)))) {
	    SET_STATUS(st_p, sec_attr_no_memory);
	    return false;
        }
        memset(acl_mgr_checked, NULL, 
	    SA_ACL_MGR_NUM(old_schema_entry) * sizeof(boolean32));
    }

    for(i = 0; i < SA_ACL_MGR_NUM(schema_entry); i++) {
        /* Make sure acl mgr_info is present before dereferencing */
        if(!(SA_ACL_MGR_INFO_P(schema_entry, i))) {
            SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
            return false;
        } else {
            /* Collect all permset info */
            perms = ( SA_ACL_MGR_PERMS_QUERY(schema_entry, i) |
                      SA_ACL_MGR_PERMS_UPDATE(schema_entry, i) |
                      SA_ACL_MGR_PERMS_TEST(schema_entry, i) |
                      SA_ACL_MGR_PERMS_DELETE(schema_entry, i) );
    
	    /* Verify that the acl_mgr_type is valid and that
	     * the perms are valid for that acl_mgr_type.
	     */
            if(!rsdb_acl_mgr_perms_valid(
	        &(SA_ACL_MGR_TYPE(schema_entry, i)), perms, st_p)) {
	        return false;
	    }

	    /* If this is a modification of an existing acl_mgr_set field,
	     * and if this incoming acl_mgr_type is the same as
	     * an existing acl_mgr_type, make sure the permsets
	     * haven't changed.  Remember (in acl_mgr_checked)
	     * which existing acl_mgr_types have been input correctly.
	     */
	    if(comparison_required) {
	        for(j = 0; j < SA_ACL_MGR_NUM(old_schema_entry); j++) {
		    if(!(uuid_compare(&(SA_ACL_MGR_TYPE(schema_entry, i)),
		        &(SA_ACL_MGR_TYPE(old_schema_entry, j)), 
		        st_p))) {
		        /* incoming acl_mgr_type matches existing */
		        if(*(acl_mgr_checked + j) == true) {
			    /* This is a duplicate input acl_mgr_type */
			    SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
			    return false; 
			}
			/* Ensure permsets haven't changed.  This is no longer
                           hold as DCE1.2.2: we want to let admin change the permset
                           of PK ERA, so remove them out /
                        */
			*(acl_mgr_checked + j) = true;
			break;
		    }
		}
            }
        }
    } /* end for each incoming acl_mgr_type */

    if(comparison_required) {
        /* Now that the input acl_mgr_types have been verified, 
         * make sure that all of the existing acl_mgr_types are
         * represented by the input acl_mgr_types.
         */
	for(j = 0; j < SA_ACL_MGR_NUM(old_schema_entry); j++) {
	    if(*(acl_mgr_checked + j) == false) {
	        /* This existing acl_mgr_type was not represented
	         * in the incoming acl_mgr_set.  Return error.
	         */
	        SET_STATUS(st_p, sec_attr_bad_acl_mgr_set);
	        return false;
	    }
	}
    }

    /* all checks passed */
    return true;
}


/*
 * r s _ a t t r _ s c h e m a _ s e m a n t i c _ c h e c k
 *
 * Check the fields of the input schema_entry for correctness.
 * If modify_parts is NULL, check every field (suitable for
 * rs_attr_schema_create_entry), otherwise, check only
 * those fields indicated by the modify_parts flags (suitable
 * for rs_attr_schema_update_entry).
 * See sec_attr_base.idl for the modifiable fields.
 *
 * Return true if the schema_entry passes all sematic checks.
 * Return false and a bad status if the schema_entry fails a check.
 *
 * This operation may take and release a database READ lock
 * if it's required to retrieve an existing schema entry for
 * comparison of the following fields: acl_mgr_set, scope,
 * and intercell_action flag.
 *
 * Since this schema_entry lookup uses rpc_ss_allocate, this
 * operation should only be called by a remote operation
 * manager routine.
 *
 * In Parameters:
 *	schema_name  - identifies the schema object on which
 *		to perform this operation.
 *      modify_parts - this flag set identifies the fields
 *		in the input schema_entry that should be checked.
 * 		If NULL, check all fields for validity.
 *		If not NULL, old_schema_entry must be passed
 *		in for comparison checking.
 * 		       
 *	schema_entry - a schema entry whose fields are being checked
 *		for semantic correctness before an update.
 * 		       
 */
PRIVATE boolean32 rs_attr_schema_semantic_check
(
    sec_attr_component_name_t       schema_name,	/* [in] */ 
    sec_attr_schema_entry_parts_t   modify_parts,	/* [in] */ 
    sec_attr_schema_entry_t         *schema_entry,	/* [in] */ 
    error_status_t                  *st_p		/* [out] */ 
)
{
    boolean32			check_all = false;
    sec_attr_schema_entry_t     old_schema_entry;
    short			scope_prefix_type;
    boolean32			binding_required = false;


    CLEAR_STATUS(st_p);

    if(!schema_entry) {
	SET_STATUS(st_p, sec_attr_bad_param);
	return false;
    }

    /* modify_parts == NULL indicates that every field should be
     * checked (for the schema_entry_create operation).
     */
    if(!modify_parts) {
	check_all = true;
    } else {
	/* When updating certain fields, noted below, data in the
	 * old schema entry must be checked for potential illegal 
	 * combinations or restricted modifications.
	 * Retrieve the old schema entry for comparison checking if
	 * any of the following fields are being updated:
	 * 	acl_mgr_set,
	 * 	scope,
	 * 	intercell_action == accept.
         */
        if( FLAG_SET(modify_parts, sec_attr_schema_part_acl_mgrs) || 
            FLAG_SET(modify_parts, sec_attr_schema_part_scope) ||
	    (FLAG_SET(modify_parts, sec_attr_schema_part_intercell) &&
            schema_entry->intercell_action == sec_attr_intercell_act_accept)) {
	    BEGIN_READ(lock_db, st_p) {
	        rsdb_attr_sch_lookup_by_uuid(schema_name, 
		    &(schema_entry->attr_id),
		    rpc_ss_allocate, &old_schema_entry, st_p);
	    } END_READ
	    if(BAD_STATUS(st_p)) {
	        return false;
	    }
	}
    }

    /* Check attr_name */
    if(check_all || FLAG_SET(modify_parts, sec_attr_schema_part_name) ) {
	if(!SA_SCH_ATTR_NAME_VALID(schema_entry->attr_name, 
	    RS_SCH_ATTR_NAME_MAX_LEN)) {
            SET_STATUS(st_p, sec_attr_bad_name);
	    return false;
	}
    }

    /* The following check_all fields can only be set on create_entry;
     * they can not be selected with modify_parts.
     */
    if(check_all) {

	/* Check attr_id */
	if(uuid_is_nil(&(schema_entry->attr_id), st_p)) {
	    do {
		uuid_create(&(schema_entry->attr_id), st_p);
	    } while (BAD_STATUS(st_p));
	}

	/* Check attr_encoding */
	if(!SA_SCH_ENCODING_TYPE_VALID(schema_entry->attr_encoding)) {
	    SET_STATUS(st_p, sec_attr_bad_encoding_type);
	    return false;
	}

	/* Check trig_types */
    	if( SA_SCH_TRIG_FLAG_IS_UPDATE(schema_entry) ) {
	    /* update trigger is not yet supported */
	    SET_STATUS(st_p, sec_attr_bad_trig_type);
	    return false;
	}

	/* Check special combinations 
	 */

	/* It's illegal to have the following three
	 * conditions in the same schema_entry:
	 * 	- unique flag is set
	 * 	- trig_type query flag is set
	 * 	- intercell_action is accept
 	 */
    	if( SA_SCH_FLAG_IS_SET_UNIQUE(schema_entry) &&
	    SA_SCH_TRIG_FLAG_IS_QUERY(schema_entry) &&
            schema_entry->intercell_action == sec_attr_intercell_act_accept ) {
	    SET_STATUS(st_p, sec_attr_bad_uniq_query_accept);
	    return false;
    	}

	/* If a trigger is indicated by the trig_type field or
	 * by intercell_action == evaluate, then require that
	 * trigger binding info is present.
	 */
	if( !SA_SCH_TRIG_FLAG_IS_NONE(schema_entry) ||
            schema_entry->intercell_action == 
	    sec_attr_intercell_act_evaluate ) {
	    binding_required = true;
	}
    }

    /* Check acl_mgr_set */
    if(check_all || FLAG_SET(modify_parts, sec_attr_schema_part_acl_mgrs) ) {
	if(!(rs_attr_schema_acl_mgr_check(schema_entry, &old_schema_entry,
	     !check_all, st_p))) {
	    return false;
	}
    } 

    /* Check unique flag */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_unique)) {
	/* unique flag can only be modified from set to unset */
	if(SA_SCH_FLAG_IS_SET_UNIQUE(schema_entry)) {
	    SET_STATUS(st_p, sec_attr_unique_no_update);
	    return false;
	}
    }

    /* Check multi-instanced flag */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_multi_inst)) {
        if(schema_entry->attr_encoding != sec_attr_enc_void) {
	    /* multi-instanced flag can only be modified from unset to set */
	    if(!(SA_SCH_FLAG_IS_SET_MULTI_INST(schema_entry))) {
	        SET_STATUS(st_p, sec_attr_multi_inst_no_update);
	        return false;
	    }
        } else {
	    SET_STATUS(st_p, sec_attr_multi_inst_disallowed);
	    return false;
        }
    }


    /* No checks required for reserved flag or apply_defaults flag */

    /* Check intercell_action */
    if(check_all || FLAG_SET(modify_parts, sec_attr_schema_part_intercell) ) {
	if(!(SA_SCH_INTERCELL_ACTION_VALID(schema_entry->intercell_action))) {
	    SET_STATUS(st_p, sec_attr_bad_intercell_action);
	    return false;
	}
    }

    /* Update of trig_types flag is not supported. */
    if(FLAG_SET(modify_parts, sec_attr_schema_part_trig_types)) {
	SET_STATUS(st_p, sec_attr_trig_types_no_update);
	return false;
    }

    /* Check trig_binding */
    if( binding_required || 
	FLAG_SET(modify_parts, sec_attr_schema_part_trig_bind) ) {
	if(!(rs_attr_schema_binding_check(schema_entry, st_p))) {
	    return false;
	}
    }

    /* Check scope */
    if(check_all || FLAG_SET(modify_parts, sec_attr_schema_part_scope) ) {
	if(!SA_SCH_STRING_VALID(schema_entry->scope, 
	    RS_SCH_SCOPE_MAX_LEN)) {
            SET_STATUS(st_p, sec_attr_bad_scope);
	    return false;
	}
	/* If scope is being modified, reject it if it's more
	 * restrictive than the existing scope.
	 */
	if(!check_all) {
	    /* Make sure the new scope is "equal to" or "a prefix of" the
	     * existing scope.
	     */
	    if( rsdb_name_util_is_prefix(
		*(sec_rgy_name_t *)(schema_entry->scope),
		u_strlen(schema_entry->scope), 
		*(sec_rgy_name_t *)(old_schema_entry.scope),
		u_strlen(old_schema_entry.scope), &scope_prefix_type) ) {
		if( scope_prefix_type == rsdb_over_qualified ) {
		    /* the new scope is more restrictive than the old scope */
		    SET_STATUS(st_p, sec_attr_bad_scope);
		    return false;
		}
	    } else {
		/* new scope is not a prefix of old scope */
		SET_STATUS(st_p, sec_attr_bad_scope);
                return false;
	    }
	}
    }

    /* Check comment */
    if(check_all || FLAG_SET(modify_parts, sec_attr_schema_part_comment) ) {
	if(!SA_SCH_STRING_VALID(schema_entry->comment, 
	    RS_SCH_COMMENT_MAX_LEN)) {
            SET_STATUS(st_p, sec_attr_bad_comment);
	    return false;
	}
    }

    /* Check special combinations outside of the "check_all" case 
     */
    /* If the intercell_action flag is being updated to "accept,"
     * make sure it won't create the following illegal combination:
     * 	- unique flag is set
     * 	- trig_type query flag is set
     * 	- intercell_action is accept
     */
    if( FLAG_SET(modify_parts, sec_attr_schema_part_intercell) &&
        schema_entry->intercell_action == sec_attr_intercell_act_accept ) {
	if( SA_SCH_FLAG_IS_SET_UNIQUE(&old_schema_entry) &&
            SA_SCH_TRIG_FLAG_IS_QUERY(&old_schema_entry) ) {
            SET_STATUS(st_p, sec_attr_bad_uniq_query_accept);
            return false;
	}
    }

    /* all checks passed */
    return true;
}



/* 
 * PUBLIC OPERATIONS
 */

/*
 * r s _ a t t r _ s c h e m a _ c r e a t e _ e n t r y
 *
 * Create a new schema entry.  Presumably you must be
 * authorized to add entries to the schema -
 * that is handled by the targetserver.
 * 
 * In Parameters:
 *      schema_name - identifies the schema object on which
 *              to perform this operation.
 *      schema_entry - a schema entry with a value in every
 *              field except perhaps the optional fields:
 *              trig_types, trig_bind, and comment.
 *
 */
PUBLIC void rs_attr_schema_create_entry (
    handle_t                     h,                /* [in] */
    sec_attr_component_name_t    schema_name,      /* [in] */
    sec_attr_schema_entry_t      *schema_entry,    /* [in] */
    rs_cache_data_t              *cache_info,      /* [out] */ 
    error_status_t               *st_p             /* [out] */
)
{
    char                        buf[sizeof(sec_rgy_name_t) + 512];
    sec_acl_permset_t           requested_perms;
    sec_acl_permset_t           parent_perms = (sec_acl_permset_t) NULL;
    boolean32                   check_quota = false;
    boolean32                   authorized = false;
    sec_rgy_foreign_id_t        null_client_ids;
    sec_attr_schema_entry_parts_t	modify_parts = NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_SchemaCreate, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        if (u_strlen(schema_name) == 0)
            ev_info.data.char_string = (unsigned char *)SEC_ATTR_SCHEMA_OBJ_NAME;
        else
            ev_info.data.char_string = schema_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = schema_entry->attr_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = schema_entry->attr_id;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* Perform semantic checks.
     * This operation won't take/release a READ lock since
     * modify_parts is NULL.
     */
    rs_attr_schema_semantic_check(schema_name, modify_parts,
	schema_entry, st_p);

    if (BAD_STATUS(st_p)) 
        rs_util_bad_client(h, "rs_attr_schema_create_entry", st_p);

    if (STATUS_OK(st_p)) {
        BEGIN_MASTER_RIW(lock_db, st_p) {
            sprintf(buf, "Attempt attribute schema creation (%s)", 
                    schema_entry->attr_name);
            rs_audit_trail_log(h, buf, (char *) schema_entry->attr_name);

            /* Only need insert permission for this operation */
            requested_perms = sec_acl_perm_insert; 

            /* Are we authorized to perform this operation? */
            if (rs_is_authorized(h, requested_perms, parent_perms, 
                                 *object_name_p, attr_schema_mgr_type, FALSE, 
                                 &null_client_ids, st_p)) {
                
                RIW_PROMOTE(lock_db);
				
                rsdb_attr_sch_add(schema_name, schema_entry, st_p);

                if (STATUS_OK(st_p)) {
                    sprintf(buf, "Attribute schema created (%s)", 
                            schema_entry->attr_name);
                    rs_audit_trail_log(h, buf, 
                                       (char *)schema_entry->attr_name);
                    rs_log_attr_schema_create(schema_name, schema_entry,
			NULL_PTR);
                } 
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(st_p)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                           1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*st_p)) {
                dce_aud_commit(sec_aud_trail, ard, 
                               aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, 
                               aud_c_evt_commit_sync_no_wait,
                               1, aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ a t t r _ s c h e m a _ d e l e t e _ e n t r y
 *
 * Delete a schema entry.  This is a radical operation
 * that will delete or invalidate any existing
 * attributes of this type on nodes dominated by
 * the schema.  Access to this operation should be
 * severely limited.
 *      In Parameters:
 *              schema_name - identifies the schema object on which
 *                      to perform this operation.
 *              attr_id - attribute type uuid of entry to be deleted.
 *
 */
PUBLIC void rs_attr_schema_delete_entry
(
    handle_t                    h,              /* [in] */ 
    sec_attr_component_name_t   schema_name,    /* [in] */ 
    uuid_t                      *attr_id,       /* [in] */ 
    rs_cache_data_t             *cache_info,    /* [out] */ 
    error_status_t              *st_p           /* [out] */ 
)
{
    char                     buf[sizeof(sec_rgy_name_t) + 512];
    sec_acl_permset_t        requested_perms;
    sec_acl_permset_t        parent_perms = (sec_acl_permset_t) NULL;
    unsigned_char_p_t	     attr_id_str = NULL;
    sec_rgy_foreign_id_t     null_client_ids;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t            ard;
    dce_aud_ev_info_t        ev_info;
    error_status_t           aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_SchemaDelete, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        if (u_strlen(schema_name) == 0)
            ev_info.data.char_string = (unsigned char *)SEC_ATTR_SCHEMA_OBJ_NAME;
        else
            ev_info.data.char_string = schema_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *attr_id;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */


    BEGIN_MASTER_RIW(lock_db, st_p) {
        uuid_to_string(attr_id, &attr_id_str, st_p);
        sprintf(buf, "Attempt delete of schema entry (%s)", attr_id_str);
        rs_audit_trail_log(h, buf, (char *) attr_id_str);

        /* Only need delete permission for this operation */
	requested_perms = sec_acl_perm_delete;

        /* Are we authorized to perform this operation? */
        if (rs_is_authorized(h, requested_perms, parent_perms,
                             *object_name_p, attr_schema_mgr_type, FALSE,
                             &null_client_ids, st_p)) {
            RIW_PROMOTE(lock_db);

            rsdb_attr_sch_delete(schema_name, attr_id, st_p);

            if (STATUS_OK(st_p)) {
/*                rs_util_update_cache_info(schema_name); */
                sprintf(buf, "Schema Entry delete (%s)", attr_id_str);

	        rs_log_attr_schema_delete(schema_name, attr_id, 
			NULL_PTR); 
	    }

	}
	
    } END_MASTER_RIW

    if(attr_id_str)
	free(attr_id_str);

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

    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);    
}


/*
 * r s _ a t t r _ s c h e m a _ u p d a t e _ e n t r y
 *
 * Update the modifiable fields of a schema entry
 * identified by the attr_id in the input schema_entry.
 * See sec_attr_base.idl for the modifiable fields.
 *
 * If a field is indicated by its flag in modify_parts, that
 * field from the input schema entry will completely replace
 * the current field of the existing schema entry.  All other
 * fields will remain untouched.  Note that fields which are
 * arrays of structures (like acl_mgr_set and trig_binding)
 * will be *completely* replaced by the new input array.
 * This operation will not simply add one more element to
 * the existing array.
 *
 * In Parameters:
 *	schema_name  - identifies the schema object on which
 *      	       to perform this operation.
 *      modify_parts - this flag set identifies the fields
 *		       in the input schema_entry that should be updated.
 * 		       Fields not indicated by modify_parts will retain
 * 		       their current values.
 *	schema_entry - a schema entry whow fields are null
 *      	       except for the fields indicated by modify_parts.
 *
 */
PUBLIC void rs_attr_schema_update_entry
(
    handle_t                        h,              /* [in] */ 
    sec_attr_component_name_t       schema_name,    /* [in] */ 
    sec_attr_schema_entry_parts_t   modify_parts,   /* [in] */ 
    sec_attr_schema_entry_t         *schema_entry,  /* [in] */ 
    rs_cache_data_t                 *cache_info,    /* [out] */ 
    error_status_t                  *st_p           /* [out] */ 
)
{
    char                        buf[sizeof(sec_rgy_name_t) + 512];
    sec_acl_permset_t           requested_perms;
    sec_acl_permset_t           parent_perms = (sec_acl_permset_t) NULL;
    boolean32                   check_quota = false;
    boolean32                   authorized = false;
    sec_rgy_foreign_id_t        null_client_ids;
    unsigned_char_p_t	        attr_id_str = NULL;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_SchemaUpdate, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        if (u_strlen(schema_name) == 0)
            ev_info.data.char_string = (unsigned char *)SEC_ATTR_SCHEMA_OBJ_NAME;
        else
            ev_info.data.char_string = schema_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = schema_entry->attr_id;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* Perform semantic checks.
     * This operation may take/release a READ lock, 
     * depending on the fields indicated in modify_parts.
     */
    rs_attr_schema_semantic_check(schema_name, modify_parts,
	schema_entry, st_p);

    if (BAD_STATUS(st_p)) 
        rs_util_bad_client(h, "rs_attr_schema_update_entry", st_p);

    if (STATUS_OK(st_p)) {
        BEGIN_MASTER_RIW(lock_db, st_p) {
            uuid_to_string(&(schema_entry->attr_id), &attr_id_str, st_p);
            sprintf(buf, "Attempt attribute schema update (%s)", attr_id_str);
            rs_audit_trail_log(h, buf, (char *) attr_id_str);

            /* Need management_info permission for this operation */
            requested_perms = sec_acl_perm_mgmt_info; 

            /* Are we authorized to perform this operation? */
            if (rs_is_authorized(h, requested_perms, parent_perms, 
                                 *object_name_p, attr_schema_mgr_type, FALSE, 
                                 &null_client_ids, st_p)) {
                
                RIW_PROMOTE(lock_db);
				
                rsdb_attr_sch_replace(schema_name, schema_entry, 
		    modify_parts, st_p);

                if (STATUS_OK(st_p)) {
                    sprintf(buf, "Attribute schema updated (%s)", attr_id_str);
                    rs_audit_trail_log(h, buf, (char *) attr_id_str);
                    rs_log_attr_schema_update(schema_name, schema_entry,
			modify_parts, NULL_PTR);
                } 
            }
        } END_MASTER_RIW
    }

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

    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ a t t r _ s c h e m a _ c u r s o r _ i n i t
 *
 * Initialize a scan cursor
 * In Parameters:
 *      schema_name - identifies the schema object on which
 *              to perform this operation.
 *
 * Out Parameters:
 *      cur_num_entries - the current total number of entries in 
 *              the schema at the time of this call.
 *      cursor - cursor initialized to the first in the
 *              list of entries in the named schema.
 *
 */
PUBLIC void rs_attr_schema_cursor_init
(
    handle_t                    h,                /* [in] */ 
    sec_attr_component_name_t   schema_name,      /* [in] */ 
    unsigned32                  *cur_num_entries, /* [out] */ 
    rs_attr_cursor_t            *cursor,          /* [out] */ 
    rs_cache_data_t             *cache_info,      /* [out] */ 
    error_status_t              *st_p             /* [out] */ 
)
{
    CLEAR_STATUS(st_p);

    BEGIN_READ(lock_db, st_p) {
        rsdb_attr_sch_cursor_init(schema_name, cur_num_entries, cursor, st_p);
    } END_READ

    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ a t t r _ s c h e m a _ s c a n
 *
 * Read num_to_read entries from the named schema -
 * useful for a browser.
 *      In Parameters:
 *              schema_name - identifies the schema object on which
 *                      to perform this operation.
 *              num_to_read - specifies the size of the
 *                      schema_entries array and the max number of
 *                      entries to be returned in this call.
 *
 *      In/Out Parameters:
 *              cursor - in: initialized cursor;
 *                      out: cursor advanced past entries returned
 *                      in this call.
 *
 *      Out Parameters:
 *              num_read - specifies the number of entries
 *                      returned in schema_entries.
 *              schema_entries - array containing num_read entries.
 */
PUBLIC void rs_attr_schema_scan
(
    handle_t                     h,                  /* [in] */ 
    sec_attr_component_name_t    schema_name,        /* [in] */ 
    rs_attr_cursor_t             *cursor,            /* [in, out] */ 
    unsigned32                   num_to_read,        /* [in] */ 
    unsigned32                   *num_read,          /* [out] */ 
    sec_attr_schema_entry_t      schema_entries[],   /* [out] */ 
    rs_cache_data_t              *cache_info,        /* [out] */ 
    error_status_t               *st_p               /* [out] */ 
)
{
    sec_acl_permset_t           requested_perms = sec_acl_perm_read;
    sec_acl_permset_t           parent_perms = (sec_acl_permset_t) NULL;
    sec_rgy_foreign_id_t        null_client_ids;
    unsigned32			num_sch_entries_left;

    CLEAR_STATUS(st_p);
    *num_read = 0;

    BEGIN_READ(lock_db, st_p) {
	/* make sure the cursor is valid, initializing it if necessary */
	rsdb_attr_sch_cursor_check(schema_name, cursor, st_p);
	
	/* Check if caller is authorized for queries on the schema */
        if (rs_is_authorized(h, requested_perms, parent_perms,
	    *object_name_p, attr_schema_mgr_type, FALSE,
	    &null_client_ids, st_p)) {

	    for((*num_read) = 0; (*num_read) < num_to_read; (*num_read)++) {
		rsdb_attr_sch_get_next(schema_name, rpc_ss_allocate,
		    cursor, &schema_entries[(*num_read)], 
		    &num_sch_entries_left, st_p);
		/* if lookup fails, break to avoid incrementing (*num_read) */
		if (BAD_STATUS(st_p)) {
			break;
		}
	    }
	}
    } END_READ

    if(BAD_STATUS(st_p)) {
	/* If the status from get_next is no_more_entries, but
	 * some schema entries are being returned, clear the status.
	 * If no schema entries are being returned, then report
	 * no_more_entries to the caller.
	 * In the case of any other bad status, don't return
	 * any schema entries.
	 */
	if(STATUS_EQUAL(st_p, sec_attr_no_more_entries) && 
	    (*num_read) != 0) {
	    CLEAR_STATUS(st_p);
	} else {
	    (*num_read) = 0;
	}
    }

    /* map non-attr status codes to sec_attr codes */
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
    return;
}


/*
 * r s _ a t t r _ s c h e m a _ l o o k u p _ b y _ n a m e
 *
 * Retrieve the schema entry identified by attr_name.
 * Useful for an interactive editor.
 * Caller must free allocated ptrs by calling:
 * sec_attr_util_sch_ent_free_ptrs(schema_entry) when done.
 *
 * INPUT
 *      schema_name - name of schema into which schema_entry
 *              should be added.  (Ignored since only one schema exists.)
 *      attr_name - attribute name identifying the schema entry
 *              to be retrieved.
 * OUTPUT:
 *      schema_entry - pointer to an allocated sec_attr_schema_entry_t.
 *              On output, internal ptrs allocated and data copied in.
 *
 * Status Codes:
 *      sec_attr_sch_entry_not_found    if schema entry not found
 *      error_status_ok         otherwise.
 */
PUBLIC void rs_attr_schema_lookup_by_name
(
    handle_t                    h,              /* [in] */ 
    sec_attr_component_name_t   schema_name,    /* [in] */ 
    unsigned_char_t             *attr_name,     /* [in] */ 
    sec_attr_schema_entry_t     *schema_entry,  /* [out] */ 
    rs_cache_data_t             *cache_info,    /* [out] */
    error_status_t              *st_p           /* [out] */
)
{
    sec_acl_permset_t           requested_perms;
    sec_acl_permset_t           parent_perms = (sec_acl_permset_t) NULL;
    sec_rgy_foreign_id_t        null_client_ids;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_SchemaLookupName, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        if (u_strlen(schema_name) == 0)
            ev_info.data.char_string = (unsigned char *)SEC_ATTR_SCHEMA_OBJ_NAME;
        else
            ev_info.data.char_string = schema_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = attr_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* On error (schema_entry not retrieved), avoid 
     * marshalling from bogus internal schema_entry pointers
     * by memset-ing schema_entry to zero.
     */
    memset(schema_entry, NULL, sizeof(*schema_entry));

    /* Determine if attr_name is legal */
    if(!SA_SCH_ATTR_NAME_VALID(attr_name, RS_SCH_ATTR_NAME_MAX_LEN)) {
        SET_STATUS(st_p, sec_attr_bad_name);
    } else if(!schema_entry) {
	SET_STATUS(st_p, sec_attr_bad_param);
    }
    if(BAD_STATUS(st_p)) {
        rs_util_bad_client(h, "rs_attr_schema_lookup_by_name", st_p);
    }

    if (STATUS_OK(st_p)) {
        BEGIN_READ(lock_db, st_p) {
            requested_perms = sec_acl_perm_read;
            /* Are we authorized to perform this operation? */
            if (rs_is_authorized(h, requested_perms, parent_perms, 
		*object_name_p, attr_schema_mgr_type, FALSE, 
		&null_client_ids, st_p)) {
                rsdb_attr_sch_lookup_by_name(schema_name, (char *)attr_name, 
		    rpc_ss_allocate, schema_entry, st_p);
            }
        } END_READ 
    }

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

    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ a t t r _ s c h e m a _ l o o k u p _ b y _ i d
 *
 * Retrieve the schema entry identified by attribute type uuid, attr_id.
 * Useful for programmatic access.
 * Caller must free allocated buffers by calling:
 * sec_attr_util_sch_ent_free_ptrs(schema_entry) when done.
 *
 * INPUT
 *      schema_name - name of schema into which schema_entry
 *              should be added.  (Ignored since only one schema exists.)
 *      attr_id - attribute type_uuid identifying the schema entry
 *              to be retrieved.
 * OUTPUT:
 *      schema_entry - pointer to an allocated sec_attr_schema_entry_t.
 *              On output, internal ptrs allocated and data copied in.
 *
 * Status Codes:
 *      sec_attr_sch_entry_not_found    if schema entry not found
 *      error_status_ok         otherwise.
 */
PUBLIC void rs_attr_schema_lookup_by_id
(
    handle_t                    h,              /* [in] */ 
    sec_attr_component_name_t   schema_name,    /* [in] */ 
    uuid_t                      *attr_id,       /* [in] */ 
    sec_attr_schema_entry_t     *schema_entry,  /* [out] */ 
    rs_cache_data_t             *cache_info,    /* [out] */
    error_status_t              *st_p           /* [out] */
)
{
    sec_acl_permset_t           requested_perms;
    sec_acl_permset_t           parent_perms = (sec_acl_permset_t) NULL;
    sec_rgy_foreign_id_t        null_client_ids;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t               ard;
    dce_aud_ev_info_t           ev_info;
    error_status_t              aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(st_p);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(ERA_SchemaLookupId, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_char_string;
        if (u_strlen(schema_name) == 0)
            ev_info.data.char_string = (unsigned char *)SEC_ATTR_SCHEMA_OBJ_NAME;
        else
            ev_info.data.char_string = schema_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_uuid;
        ev_info.data.uuid = *attr_id;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* On error (schema_entry not retrieved), avoid 
     * marshalling from bogus internal schema_entry pointers
     * by memset-ing schema_entry to zero.
     */
    memset(schema_entry, NULL, sizeof(*schema_entry));

    if(!schema_entry) {
	SET_STATUS(st_p, sec_attr_bad_param);
    }
    if(BAD_STATUS(st_p)) {
        rs_util_bad_client(h, "rs_attr_schema_lookup_by_id", st_p);
    }

    BEGIN_READ(lock_db, st_p) {
        requested_perms = sec_acl_perm_read;
        /* Are we authorized to perform this operation? */
	if (rs_is_authorized(h, requested_perms, parent_perms, *object_name_p,
	    attr_schema_mgr_type, FALSE, &null_client_ids, st_p)) {
            rsdb_attr_sch_lookup_by_uuid(schema_name, attr_id, 
		rpc_ss_allocate, schema_entry, st_p);
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

    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ a t t r _ s c h e m a _ g e t _ r e f e r r a l
 *
 * Obtain a referral to an schema update site.  This
 * function is used when the current schema site yields a
 * sec_schema_site_readonly error.  Some replication
 * managers will require all updates for a given object
 * to be directed to a given replica.
 * Clients of the generic schema interface may not know
 * they are dealing with an object that is replicated in
 * this way.  This function allows them to recover from
 * this problem and rebind to the proper update site.
 *    In Parameters:
 *        schema_name - identifies the schema object on which
 *            to perform this operation.
 *        attr_id - attribute type uuid identifying the entry.
 *
 *    Out Parameters:
 *        towers - tower set of a schema update server.
 */
PUBLIC void rs_attr_schema_get_referral
(
    handle_t                         h,               /* [in] */ 
    sec_attr_component_name_t        schema_name,     /* [in] */ 
    uuid_t                           *attr_id,        /* [in] */ 
    sec_attr_twr_set_p_t             *towers,         /* [out] */ 
    error_status_t                   *st_p            /* [out] */ 
)
{
    *towers = NULL;

    SET_STATUS(st_p, sec_attr_not_implemented);
    rs_attr_util_map_status(st_p);
}


/* r s _ a t t r _ s c h e m a _ g e t _ a c l _ m g r s 
 *
 * Retrieve the acl_mgr_types that protect the objects
 * dominated by the named schema.  The returned acl_mgr_types
 * are valid for use in the acl_mgr_set field of a schema_entry
 * for the named schema.
 *
 * If num_acl_mgr_types is greater than size_used, this operation
 * should be called again with a bigger acl_mgr_types buffer.
 *
 *	In Parameters:
 *		schema_name - identifies the schema object on which
 *			to perform this operation.
 *		size_avail - size of the acl_mgr_types array
 *
 *	Out Parameters:
 *		size_used - number of acl_mgr_types returned
 *		num_acl_mgr_types - total number of acl_mgr_types
 *			supported for this schema.  
 *		acl_mgr_types - array containing 'size_used' 
 *			acl_mgr_type UUIDs.
 *
 * Errors:
 *	sec_attr_unauthorized
 *	sec_attr_svr_unavailable
 *	sec_attr_no_memory
 */
PUBLIC void rs_attr_schema_get_acl_mgrs (
    handle_t			h,			/* [in] */
    sec_attr_component_name_t	schema_name,		/* [in] */
    unsigned32			size_avail,		/* [in] */
    unsigned32			*size_used,		/* [out] */
    unsigned32			*num_acl_mgr_types,	/* [out] */
    uuid_t			acl_mgr_types[],	/* [out] */
    rs_cache_data_t		*cache_info,		/* [out] */
    error_status_t		*st_p			/* [out] */
)
{
    *size_used = 0;
    *num_acl_mgr_types = 0;

    SET_STATUS(st_p, sec_attr_not_implemented);
    rs_util_get_cache_data(cache_info);
    rs_attr_util_map_status(st_p);
}


/* r s _ a t t r _ s c h e m a _ a c l m g r _ s t r i n g s
 *
 * Retrieve printable representations for each permission bit that the 
 * input acl_mgr_type will support.  There may be aliases for common
 * permission combinations - by convention simple entries should
 * appear at the beginning of the array, and combinations should at the
 * appear at the end.  When false the tokenize flag indicates that
 * permission printstrings are unambiguous and therefore printstrings
 * for various permissions can be concatenated.  When true, however,
 * this property does not hold and the strings should be tokenized
 * before input or output. 
 *
 * The acl_mgr_info string provides a name and
 * help info for the manager type as well as the complete set of
 * supported permission bits. total_num_printstrings is the total
 * number of acl printstrings supported by this acl manager type - if
 * it is greater than the size avail, this function should be invoked
 * again with a buffer of the appropriate size.
 *
 * If acl_mgr_type supports more than 32 permission
 * bits, multiple manager types can be used - one for each 32 bit wide
 * slice of permissions.  When this is the case the acl_mgr_type_chain
 * parameter is set to the uuid of the next manager type in the set.  The
 * final result for the chain returns uuid_nil in the manager_type_chain
 * parameter.
 *
 *	In Parameters:
 *		schema_name - identifies the schema object on which
 *			to perform this operation.
 *		acl_mgr_type - the UUID of the acl_mgr_type for which
 *			the printstrings are to be returned.
 *		size_avail - size of the printstrings array
 *
 *	Out Parameters:
 *		acl_mgr_type_chain - if not uuid_nil, identifies the
 *			next acl_mgr_type UUID in a chain supporting
 *			acl managers with more than 32 permission bits.
 *		acl_mgr_info - printstrings containing the name, help info,
 *			and complete set of supported permission bits
 *			for this acl_mgr.
 *		tokenize - if true, permission bit strings should be
 *			tokenized.
 *		total_num_permstrings -  the total number of permission 
 *			printstrings supported by this acl_mgr_type.
 *		size_used - number of perm printstrings returned
 *		permstrings - array containing the printstrings for
 *			each permission supported by this acl_mgr_type.
 *
 * Errors:
 *	sec_attr_unauthorized
 *	sec_attr_svr_unavailable
 *	sec_attr_no_memory
 *
 */
PUBLIC void rs_attr_schema_aclmgr_strings (
    handle_t			h,			/* [in] */
    sec_attr_component_name_t	schema_name,		/* [in] */
    uuid_t			*acl_mgr_type, 		/* [in] */
    unsigned32			size_avail,		/* [in] */
    uuid_t			*acl_mgr_type_chain,	/* [out] */
    sec_acl_printstring_t	*acl_mgr_info,		/* [out] */
    boolean32			*tokenize,		/* [out] */
    unsigned32			*total_num_printstrings, /* [out] */
    unsigned32			*size_used,		/* [out] */
    sec_acl_printstring_t	permstrings[],		/* [out] */
    rs_cache_data_t		*cache_info,		/* [out] */
    error_status_t		*st_p			/* [out] */
)
{
    error_status_t st;

    /* Initialize size_used to 0 so that, in case of error,
     * the stub doesn't try to marshall bogus data in permstrings
     */
    uuid_create_nil(acl_mgr_type_chain, &st);
    acl_mgr_info->printstring[0] = '\0';
    acl_mgr_info->helpstring[0] = '\0';
    acl_mgr_info->permissions = 0;
    *tokenize = FALSE;
    *total_num_printstrings = 0;
    *size_used = 0;

    SET_STATUS(st_p, sec_attr_not_implemented);
    rs_attr_util_map_status(st_p);
    rs_util_get_cache_data(cache_info);
}


