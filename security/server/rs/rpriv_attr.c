/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpriv_attr.c,v $
 * Revision 1.1.10.3  1996/02/18  00:17:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:10  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:50:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/11  14:31 UTC  greg
 * 	Fix memory leaks per OSF CR # 12774
 * 	[1995/12/08  17:19:59  root]
 * 
 * Revision 1.1.6.1  1994/10/17  17:54:06  cuti
 * 	Fix a couple of minor bugs that might core secd if era triggers used.
 * 	[1994/10/14  16:16:03  cuti]
 * 
 * Revision 1.1.2.9  1994/09/28  20:50:20  greg
 * 	CR11910: pass delegation chain in priv_attr_trig_query calls.
 * 	[1994/09/28  18:01:50  greg]
 * 
 * Revision 1.1.2.8  1994/09/09  17:29:34  mdf
 * 	Remove developers comments
 * 	[1994/09/09  15:18:57  mdf]
 * 
 * 	Updates for CR 12099
 * 	[1994/09/09  14:52:13  mdf]
 * 
 * Revision 1.1.2.7  1994/08/25  16:19:41  mdf
 * 	Tweak version.
 * 	[1994/08/25  14:39:41  mdf]
 * 
 * 	Ensure the  DCE 1.1 version of priv_attr_trig.idl
 * 	defines the protocol the way we expect it to work for a while,
 * 	minimally.
 * 	[1994/08/24  18:45:23  mdf]
 * 
 * Revision 1.1.2.6  1994/08/12  20:49:15  burati
 * 	Fix attribute ptr usage in rpriv_attr_get_effective trigger check
 * 	(crashed secd if more than 1 attribute retrieved).  OT CR 11678.  Also
 * 	moved END_READ up to end of rsdb usage so read lock wouldn't be held
 * 	during trigger callouts.
 * 	[1994/08/12  20:22:45  burati]
 * 
 * Revision 1.1.2.5  1994/08/04  16:13:19  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:10:48  mdf]
 * 
 * Revision 1.1.2.4  1994/07/15  15:01:09  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:18:51  mdf]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:44  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:15:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/02  21:27:12  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:42  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  16:55:19  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:10  mdf]
 * 
 * $EndLog$
 */
#include <dce/sec_attr_tools.h>
#include <dce/sec_bind_util.h>

#include <rs_attr.h>
#include <rs_ch_lock.h>
#include <rs_auth.h>
#include <rs_util.h>
#include <rs_attr_pvt.h>
#include <priv_attr_trig.h>
/* #include <sec_attr_util.h> */

#include <rpriv_attr.h>

#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_create.h>
#include <rsdb_attr.h>
#include <rsdb_attr_schema.h>
#include <rsdb_name_util.h>
#include <rsdb_pgo.h>
#include <rsdb_name_util.h>

#define TRIGGERS	1	/* ONLY DOING ONE TRIGGER AT A TIME */


/*
 * Given a non-zero length array of epac data, allocate an
 * array of sec_id_foreign_t and copy cell and principal
 * uuids from the epac array.  Names are all set to NULL.
 */
static void priv_attr_trig_alloc_delegate_ids 
(
    unsigned32          num_upstream_delegates,  /* [in] */
    sec_id_epac_data_t  upstream_delegates[],    /* [in] */
    sec_id_foreign_t    **trig_delegates,        /* [out] */ 
    error_status_t      *st_p                    /* [out] */
) 
{
    unsigned32          i;
    sec_id_foreign_t    *fids = NULL;

    CLEAR_STATUS(st_p);

    if (num_upstream_delegates == 0) {
	*trig_delegates = NULL;
	return;
    }

    fids = (sec_id_foreign_t *) rpc_ss_allocate(num_upstream_delegates 
						* sizeof(*fids));

    if (fids == NULL) {
	SET_STATUS(st_p, sec_priv_s_no_mem);
	return;
    }

    for (i = 0; i < num_upstream_delegates; i++) {
	fids[i].realm.uuid = upstream_delegates[i].pa.realm.uuid;
	fids[i].realm.name = NULL;
	fids[i].id.uuid = upstream_delegates[i].pa.principal.uuid;
	fids[i].id.name = NULL;
    }
 
    *trig_delegates = fids;
}

/*
 * This is the routine that actually makes the attempt to make
 * the trigger callout.  It uses a temporary sec_attr_t incase
 * the trigger fails, (don't want to clobber data before all possible
 * binding types are handled).
 *
 *  STATUS CODE		RETURN VALUE		RESULT
 *  ---------------------------------------------------------------------------
 *  good		true		successful completion of trigger.
 *  bad 		true		give up don't try trigger again.
 *  bad 		false		continue, new binding handle needed.
 *
 */
PRIVATE boolean32 rpriv_try_trigger
(
    handle_t            	h,			/* [in] */
    sec_id_foreign_t 		principal,		/* [in] */
    rpc_auth_identity_handle_t 	(*get_auth_ident)(),	/* [in]  */
    unsigned32                  num_upstream_delegates, /* [in] */
    sec_id_foreign_t            *upstream_delegate_ids, /* [in] */
    sec_attr_t			*attr_keys_p,		/* [in] */
    sec_attr_t			*sec_attr_p,		/* [in/out] */
    error_status_t 		*st_p			/* [out] */
)
{
    unsigned32			num_left, num_trig_attrs;
    sec_attr_t			tmp_sec_attr[TRIGGERS];
    priv_attr_trig_cursor_t	cursor;
    sec_rgy_name_t              full_princ_name; 
    priv_attr_trig_timeval_sec_t time_to_live;

    CLEAR_STATUS(st_p);

    /* Use get_auth_ident when available */

    rpc_binding_set_auth_info( h, SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p),
	SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p),
	SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p), NULL,
/*	(rpc_auth_identity_handle_t) *(get_auth_ident)(), */
	SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p), st_p);

    /* If set_auth_info fails do another import_next. */
    if (BAD_STATUS(st_p)) {
	return (FALSE);			/* continue new binding handle needed */
    }

    /*
     * Hardwired to handle a set number of attributes at a time, 
     * in this case 1.
     */
    priv_attr_trig_query( h, principal, num_upstream_delegates,
	upstream_delegate_ids,&cursor, TRIGGERS, TRIGGERS, attr_keys_p, 
	&num_trig_attrs, tmp_sec_attr, &time_to_live, &num_left, st_p);

    if(GOOD_STATUS(st_p)) {
        sec_attr_util_inst_free_ptrs(sec_attr_p);
        sec_attr_util_alloc_copy(NULL, &tmp_sec_attr[0], sec_attr_p, st_p);
	/* 
         * Not checking the above status, because if it fails then 
	 * we don't want to try the trigger again.
	 */
        return (TRUE);		/* with good status */
    } else {
        /*
         * If status indicates the kind of comm failure that
         * may be remidied by a new binding handle, then do
         * another import_next retry, else  priv_attr_trig_query
         * failure was not due to a comm failure so no sense
         * retrying with a new binding.
         */
        if(sec_bind_retry_status(st_p)) {
            return (FALSE);		/* continue new binding handle needed */
        } else {
	    return (TRUE);		/* don't try trigger again */
        }   
    }
}

/* 
 * Determine trigger binding type, create binding handle,
 * and then try the trigger until either done or 
 * all of the number of trigger bindings has been processed.
 */

PRIVATE boolean32 rpriv_bind_and_invoke_trigger
(
    sec_id_foreign_t 		principal,
    rpc_auth_identity_handle_t 	(*get_auth_ident)(),	/* [in]  */
    unsigned32                  num_upstream_delegates, /* [in] */
    sec_id_foreign_t            *upstream_delegate_ids, /* [in] */
    sec_attr_t			*sec_attr_p,
    sec_attr_t			*attr_keys_p,	
    error_status_t 		*st_p
)
{
    handle_t           	h;
    rpc_ns_handle_t	context;
    error_status_t	tmp_status;
    unsigned32		k, result = FALSE, done = FALSE;

    /*
     * Walk down the list of bindings until were either "done"
     * or no more exist.
     */
    for(k = 0; !done && k < SA_ATTR_BND_NUM(sec_attr_p); k++) {
        switch(SA_ATTR_BND_TYPE(sec_attr_p, k)) {
        case sec_attr_bind_type_string:
	    rpc_binding_from_string_binding(SA_ATTR_BND_STRING_P(sec_attr_p, k),
		&h, st_p);
	    /*
	     * If we couldn't get a binding, (that's ok) just try again.
             * Status is of no interest.
	     */
	    if (BAD_STATUS(st_p)) 
		break;

	    /*
	     * The rpriv_try_trigger has 3 possible return values:
	     *
	     *   If the status is good and it returns true then 
	     * 	 we found the trigger succesfuly.
	     *
	     *   If the status is bad, but the return value is true
	     *   then we have hit a problem, that means don't try
	     *   another binding.
	     *
	     *   If bad status, and return is false then its ok to 
	     *   try another binding.
	     */
	    done = rpriv_try_trigger(h, principal, get_auth_ident,
			num_upstream_delegates, upstream_delegate_ids,
			attr_keys_p, sec_attr_p, st_p);

	    /*
	     * Don't loose the status from the rpriv_try_trigger call
	     * so use a temporary status in the rpc_binding_free()
	     * call.
	     */
	    rpc_binding_free(&h, &tmp_status);
	    break;

        case sec_attr_bind_type_twrs:
            break;			/* Not implemented at this time */
        case sec_attr_bind_type_svrname:
            rpc_ns_binding_import_begin(
                SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p, k),
                SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, k),
                 priv_attr_trig_v1_0_c_ifspec, NULL, 	
    		&context, st_p);

            if (BAD_STATUS(st_p)) {
    		break;			/* from the case statement. */
    	    }
            while (!done) {
                rpc_ns_binding_import_next(context, &h, st_p);
                if (BAD_STATUS(st_p)) {
                    /* if import_next fails, no use trying 
    		     * another import_next, so break from
    		     * while(!done) loop
    		     */
                     break;
                }
                if (SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p) == 
    			sec_attr_bind_auth_dce) {

	    		done = rpriv_try_trigger(h, principal, get_auth_ident,
				 num_upstream_delegates, upstream_delegate_ids,
				 attr_keys_p, sec_attr_p, st_p);

			/*
			 * Don't want to loose the status from
	 		 * the rpriv_try_trigger() call above, so
			 * use tmp_status here. 
			 */
		        rpc_binding_free (&h, &tmp_status);

                } /* end if sec_attr_bind_auth_dce */
            } /* end while (!done) */
            /* Perform import_done after a successful call
             * to import_begin and after all import_next
             * calls have been completed.
             */
	    rpc_ns_binding_import_done(&context, st_p);
	    break;
	default:
	    break;
        } /* end of switch on bind type */
    } /* end for each trigger binding */
    return (done);
}

/*
 *   IT IS ASSUMED THAT EACH ATTRIBUTE TRIGGER WILL RETURN AN EXACT
 *   NUMBER OF ATTRIBUTES BACK BASED ON THE NUMBER OF ATTRIBUTES 
 *   PASSED IN.  IN THIS CASE ONLY ONE ATTRIBUTE WILL BE PASSED IN
 *   AND ONLY ONE TRIGGER ATTRIBUTE WILL COME BACK FROM THE TRIGGER.
 */
PRIVATE void rpriv_triggers
(
    sec_id_foreign_t 		principal,
    rpc_auth_identity_handle_t 	(*get_auth_ident)(),	/* [in]  */
    unsigned32                  num_upstream_delegates, /* [in] */
    sec_id_epac_data_t          upstream_delegates[],   /* [in] */
    unsigned32			num_attr_keys,
    sec_attr_t 			attr_keys[],
    unsigned32 			*num_attrs,
    sec_attr_t 			**attrs,
    error_status_t 		*st_p
)
{
    handle_t                    h;
    rpc_ns_handle_t		context;
    sec_attr_t			*sec_attr_p, *attr_keys_p;
    unsigned32			i, j, k, result = FALSE, done, first_trigger;
    unsigned32			num_left, num_trig_attrs;
    priv_attr_trig_cursor_t	cursor;
    unsigned32			*processed;
    sec_rgy_name_t              full_princ_name;
    priv_attr_trig_timeval_sec_t time_to_live;
    sec_id_foreign_t 		*upstream_delegate_ids = NULL;


    processed = rpc_ss_allocate(num_attr_keys * sizeof(unsigned32));
    if (processed == NULL) {
	SET_STATUS(st_p, sec_priv_s_no_mem);
	return;
    }
    memset(processed, FALSE, num_attr_keys * sizeof(unsigned32));
    CLEAR_STATUS(st_p);

    /*
     * If necessary, construct the upstream delegate
     * array.
     */
    first_trigger = 0;
    done = FALSE;
    if (num_upstream_delegates > 0) {
	done = TRUE;
	for(i=first_trigger; i < *num_attrs; i++) {
	    sec_attr_p = &(*attrs)[i];
	    if (SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_trig_binding) {
		priv_attr_trig_alloc_delegate_ids(num_upstream_delegates,
						  upstream_delegates,
						  &upstream_delegate_ids,
						  st_p);
		first_trigger = i;
		done = FALSE;
		break;
	    }
	}
    }

    if (BAD_STATUS(st_p) || done) {
	return;
    }

    /* 
     * Walk down the list of attributes
     */
    for(i=first_trigger; i < *num_attrs; i++) {
        sec_attr_p = &(*attrs)[i];
	/* 
	 * Check to see if there are any trigger attributes.
	 */
	if (SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_trig_binding) {
	    /* 
	     * We don't want to reprocess attribute keys that's
	     * why we check to see if its been marked.
	     */
	    for (j = 0; j < num_attr_keys && processed[j] == FALSE; j++) {
		attr_keys_p = &attr_keys[j];
		result = uuid_equal(&(SA_ATTR_ID(sec_attr_p)),
		                &(SA_ATTR_ID(attr_keys_p)), st_p);
    		if (result && GOOD_STATUS(st_p)) {
		    processed[j] = TRUE;
		    if (!rpriv_bind_and_invoke_trigger(principal, get_auth_ident,
			    num_upstream_delegates, upstream_delegate_ids,
			    sec_attr_p, attr_keys_p, st_p)) {
			/*
			 * Since the trigger was not found then 
			 * either remove the attribute from the middle
			 * of the attribute list of null out the one
			 * at the end.
			 */
			if ( i != --(*num_attrs)) {
                            memcpy(sec_attr_p, &attrs[*num_attrs],
                                sizeof (sec_attr_t));
			    i--;
                        } else {
                            memset(sec_attr_p, NULL, sizeof(sec_attr_t));
                        }
		    break;
		    } 
	        } 
	    }
        }
    }
}

/* r p r i v _ a t t r _ g e t _ e f f e c t i v e
 *
 * Allocate an array for and return all attributes 
 * of the requested types for the named principal object.
 * If a requested attribute doesn't exist for the
 * object, attempt to retrieve an appropriate default.
 * The calling identity, represented by client_epac, is checked for
 * access to the requested attributes.  If the caller does
 * not have authorization to view one or more of the
 * requested attributes, those attributes are not returned.
 *
 * This operation never returns a bad status code.
 * If a requested attribute is not found or is
 * not retrievable for any reason, it will simply
 * be excluded from the returned attrs array and
 * a successful status will be returned.
 *
 * Attribute Sets:
 * Reads of an attr_set return all instances of members 
 * of that set.  If an instance doesn't exist for a certain
 * member, attempt to retrieve a default.
 * The attr_set instance itself will not be returned.
 *
 * Triggers:
 * If the requested attribute type is associated with a query
 * trigger AND the invoke_trigger input boolean is FALSE, 
 * the value returned for the attribute will be the
 * binding (as set in the schema entry) of the trigger server.
 * The caller must bind to the trigger server and pass the
 * original input query attribute to the priv_attr_trig_query call
 * in order to retrieve the attribute value.
 * If the requested attribute type is associated with a query
 * trigger AND the invoke_trigger input boolean is TRUE, 
 * this routine will attempt to bind to the trigger server to
 * retrieve the requested attributes.
 *
 *    In Parameters:
 *	principal_name - identifies the registry principal object on which
 *            to perform this operation.  Name is relative to 
 *		to /.:/sec/principal (e.g., "root").
 *	client_epac - identifies the caller; used for authorization
 *		check.
 *      num_upstream_delegates - if the principal is being added
 *            to an existing delegation chain, the number of delegates
 *            in that existing ("upstream") delegation chain.
 *      upstream_delegates - the upstream delegation chain, if any.
 *	num_attr_keys - specifies the number of elements in the
 *            input attr_keys array.
 *	attr_keys - in the attr_id field of each element, contains
 *            the attribute type id of the attribute instance(s)
 *            requested by this lookup. If the requested attribute
 *            type is associated with a query trigger, the attr_val
 *            field may be used to pass in optional information 
 *            required by the trigger query. If no information is
 *            to be passed in the attr_val field (whether the type
 *            indicates a trigger query or not), the attr_val
 *            encoding type should be set to
 *            sec_attr_enc_void.
 *	invoke_trigger - if TRUE, and an attr_key is associated with
 *		a query trigger, this routine will perform the
 *		remote priv_attr_trig_query call and return the
 *		attribute(s) from that call.
 *		if FALSE, this routine will not perform any 
 *		priv_attr_trig_query calls and will return
 *		the attribute's local representation (including
 *		trigger server binding info).
 *	get_auth_ident - function used by this routine, in
 *		cases where a query trigger must be made, to
 *		generate the auth_identity of the caller for
 *		use in setting the auth info on the binding
 *		handle used to contact the trigger server.
 *            
 *    Out Parameters:
 *	num_returned - specifies the number of attribute instances
 *            returned in the attrs array.
 *	attrs - rpc_ss_allocated array of every attribute
 *		satisfying the search criteria in attr_keys.
 *
 */

PRIVATE uuid_t  *per_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);

void rpriv_attr_get_effective
(
    sec_attr_component_name_t	principal_name,		/* [in]  */
    sec_id_epac_data_t          *client_epac,		/* [in]  */
    unsigned32                  num_upstream_delegates, /* [in] */
    sec_id_epac_data_t          upstream_delegates[],   /* [in] */
    unsigned32                  num_attr_keys,		/* [in]  */
    sec_attr_t                  attr_keys[],		/* [in]  */
    boolean32			invoke_trigger, 	/* [in]  */
    rpc_auth_identity_handle_t 	(*get_auth_ident)(),	/* [in]  */
    unsigned32                  *num_returned,  	/* [out] */
    sec_attr_t                  **attrs,        	/* [out] */
    error_status_t              *st_p			/* [out] */
)
{
    uuid_t                      obj_uuid;
    signed32                    i, failure_index, *xref=NULL;
    handle_t			h;
    boolean32			result=FALSE;
    sec_rgy_name_t		full_princ_name, cell_name;
    sec_id_foreign_t 		principal;
    sec_acl_permset_t           requested_perms;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    sec_rgy_foreign_id_t	client_ids;
    sec_attr_schema_entry_t     *sch_entry_p;
    rsdb_util_rgy_obj_info_t    rgy_obj_info;

    
    CLEAR_STATUS(st_p);
    *num_returned = 0;
    *attrs = NULL;

    BEGIN_READ(lock_db, st_p) {
	/* Prepend principal directory name to "principal_name"
 	 * since support routines expect names relative to /.:/sec
 	 */
	rsdb_name_util_complete_name(sec_rgy_domain_person, 
	    principal_name, full_princ_name, st_p);

        /* translate the object name into rgy_obj_info for use
         * in following rsdb_attr calls
         */
        rsdb_util_get_obj_info_by_name(full_princ_name, rsdb_pgo_type,
		sec_acl_type_object, &rgy_obj_info, &obj_uuid, st_p);

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
                result = rs_is_authorized_by_epac_data(
			    client_epac, requested_perms, NULL, 
			    full_princ_name, per_mgr_type, &client_ids, st_p);
                if(!result || BAD_STATUS(st_p)) {
                    /* caller not authorized to query attributes
		     * of this type, so delete the entry from the list
		     */
		    CLEAR_STATUS(st_p);
		    sch_list->entries[i].flags.deleted = 1;
		}
	    }
        }
        if(STATUS_OK(st_p)) {
            rsdb_attr_get_effective(&rgy_obj_info, sch_list,
		rpc_ss_allocate, num_returned, attrs, st_p);
	}

        principal.id.name = principal_name;
        principal.id.uuid = client_epac->pa.principal.uuid;
        principal.realm.uuid = client_epac->pa.realm.uuid;
                    
        /* Get cell name from cell uuid */
        rsdb_pgo_uid_to_name(sec_rgy_domain_person,
		&principal.realm.uuid, cell_name, st_p);
        if (GOOD_STATUS(st_p)) {
	    rsdb_name_util_convert_to_cell(cell_name, st_p);
	    if (GOOD_STATUS(st_p)) {
		principal.realm.name = cell_name;
	    }
	}
    } END_READ

    if (invoke_trigger == TRUE)
        rpriv_triggers(principal, get_auth_ident, num_upstream_delegates,
	        upstream_delegates, num_attr_keys, attr_keys, num_returned,
		attrs, st_p);

    if(sch_list) {
        /* free internal ptrs of each schema_entry allocated */
        for(i = 0; i < sch_list->count; i++) {
            sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
        }
        free(sch_list);
    }
    if(xref)
        free(xref);

    if(STATUS_EQUAL(st_p, sec_attr_no_more_entries)) {
	CLEAR_STATUS(st_p);
    }

    return;
}
