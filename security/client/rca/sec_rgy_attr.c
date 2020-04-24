/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_attr.c,v $
 * Revision 1.1.10.3  1996/10/03  14:48:15  arvind
 * 	Silence compiler warnings.
 * 	[1996/09/16  23:37 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	CHFts19810: more robust checking of cell name length in
 * 	sec_rgy_triggers() and sec_rgy_attr_lookup_by_name()
 * 	[1996/08/23  16:04 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 *
 * 	CHFts19810: fix memory leak in sec_rgy_triggers() on cell_namep.
 * 	[1996/08/21  17:03 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	CHFts19500: fix memory leak in calls to dce_cf_get_cell_name
 * 	[1996/07/13  20:50 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/05/24  23:19 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	Submit
 * 	HP 	[1995/09/19  20:05 UTC  bartf  /main/HPDCE02/3]
 *
 * 	merge fix for odss sec_rgy_trigger() crash
 * 	HP 	[1995/09/15  13:55 UTC  bartf  /main/HPDCE02/bartf_sec_10/3]
 *
 * 	fix uuid checking
 * 	HP 	[1995/08/31  20:45 UTC  bartf  /main/HPDCE02/bartf_sec_10/2]
 *
 * 	uncomment
 * 	HP 	[1995/08/25  17:40 UTC  bartf  /main/HPDCE02/bartf_sec_10/1]
 *
 * 	fix bug CHFts13817 - sec_rgy_triggers() crashes
 * 	HP 	[1995/07/20  20:20 UTC  cuti  /main/HPDCE02/2]
 *
 * 	Initialize uninitized varaible unveil by  compiling with +O2
 * 	HP 	[1995/07/19  17:37 UTC  cuti  /main/HPDCE02/cuti_mothra10_fix/2]
 *
 * 	CHFts15455
 * 	HP 	[1995/07/19  13:42 UTC  cuti  /main/HPDCE02/cuti_mothra10_fix/1]
 *
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 	HP 	[1995/04/28  19:40 UTC  mdf  /main/HPDCE02/1]
 * 	[1996/10/03  13:32:20  arvind]
 *
 * Revision 1.1.10.2  1996/08/09  12:03:58  arvind
 * 	CHFts19500: fix memory leak in calls to dce_cf_get_cell_name
 * 	[1996/07/13  20:50 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 * 
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/05/24  23:19 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:39  marty]
 * 
 * 	Submit
 * 	HP 	[1995/09/19  20:05 UTC  bartf  /main/HPDCE02/3]
 * 
 * 	merge fix for odss sec_rgy_trigger() crash
 * 	HP 	[1995/09/15  13:55 UTC  bartf  /main/HPDCE02/bartf_sec_10/3]
 * 
 * 	fix uuid checking
 * 	HP 	[1995/08/31  20:45 UTC  bartf  /main/HPDCE02/bartf_sec_10/2]
 * 
 * 	uncomment
 * 	HP 	[1995/08/25  17:40 UTC  bartf  /main/HPDCE02/bartf_sec_10/1]
 * 
 * 	fix bug CHFts13817 - sec_rgy_triggers() crashes
 * 	HP 	[1995/07/20  20:20 UTC  cuti  /main/HPDCE02/2]
 * 
 * 	Initialize uninitized varaible unveil by  compiling with +O2
 * 	HP 	[1995/07/19  17:37 UTC  cuti  /main/HPDCE02/cuti_mothra10_fix/2]
 * 
 * 	CHFts15455
 * 	HP 	[1995/07/19  13:42 UTC  cuti  /main/HPDCE02/cuti_mothra10_fix/1]
 * 
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 	HP 	[1995/04/28  19:40 UTC  mdf  /main/HPDCE02/1]
 * 	[1996/08/08  20:08:09  arvind]
 * 
 * Revision 1.1.10.1  1996/05/10  13:14:28  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/12  20:35 UTC  pdd
 * 	fix functions that return no values
 * 	[1996/02/29  22:40 UTC  pdd  /main/HPDCE02/4]
 * 
 * 	provide return values
 * 	[1996/02/29  22:01 UTC  pdd  /main/HPDCE02/pdd_branch_CHFts17478/1]
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:28 UTC  psn
 * 	merge fix for odss sec_rgy_trigger() crash
 * 	[1995/09/19  20:05 UTC  bartf  /main/HPDCE02/3]
 * 
 * 	fix uuid checking
 * 	[1995/09/15  13:55 UTC  bartf  /main/HPDCE02/bartf_sec_10/3]
 * 
 * 	uncomment
 * 	[1995/08/31  20:45 UTC  bartf  /main/HPDCE02/bartf_sec_10/2]
 * 
 * 	fix bug CHFts13817 - sec_rgy_triggers() crashes
 * 	[1995/08/25  17:40 UTC  bartf  /main/HPDCE02/bartf_sec_10/1]
 * 
 * 	Initialize uninitized varaible unveil by  compiling with +O2
 * 	[1995/07/20  20:20 UTC  cuti  /main/HPDCE02/2]
 * 
 * 	CHFts15455
 * 	[1995/07/19  17:37 UTC  cuti  /main/HPDCE02/cuti_mothra10_fix/2]
 * 
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 	[1995/07/19  13:42 UTC  cuti  /main/HPDCE02/cuti_mothra10_fix/1]
 * 
 * 	Changes based on review.
 * 	[1995/04/14  19:58 UTC  mdf  /main/mdf_mothra_defects_3/2]
 * 
 * 	Resolve defect CHFts14561
 * 	[1995/04/13  21:24 UTC  mdf  /main/mdf_mothra_defects_3/1]
 * 
 * Revision 1.1.2.4  1994/09/09  17:29:32  mdf
 * 	Updates for CR 12099
 * 	[1994/09/09  15:16:13  mdf]
 * 
 * 	Updates for CR 12099
 * 	[1994/09/08  12:45:45  mdf]
 * 
 * Revision 1.1.2.3  1994/08/04  16:12:40  mdf
 * 	Fix include file
 * 	[1994/07/28  19:18:13  mdf]
 * 
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:16  mdf]
 * 
 * Revision 1.1.2.2  1994/06/17  18:41:47  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:23  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/02  17:54:41  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:40  hanfei]
 * 
 * 	Inital ERA drop
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993, 1994
** Unpublished work. All Rights Reserved.
**
**      Attributes Manipulation Interface
**
**
*/

#include <rca_pvt.h>
#include <rgymacro.h>

#include <dce/sec_attr_tools.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_bind_util.h>
#include <dce/dce_cf.h>
#include <rs_attr.h>
#include <sec_attr_trig.h>

#include <sysdep.h>
#include <bstring.h>
#include <u_str.h>

#define Crs_attr_cursor_init(CH,b,c,d,CI,e)\
    (*rs_attr_v0_0_c_epv.rs_attr_cursor_init) (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_lookup_by_id(CH,b,c,d,e,f,g,h,i,CI,j)\
    (*rs_attr_v0_0_c_epv.rs_attr_lookup_by_id) (HANDLE(CH),b,c,d,e,f,g,h,i,CI,j)

#define Crs_attr_lookup_no_expand(CH,b,c,d,e,f,g,h,i,CI,j)\
    (*rs_attr_v0_0_c_epv.rs_attr_lookup_no_expand)\
    (HANDLE(CH),b,c,d,e,f,g,h,i,CI,j)

#define Crs_attr_lookup_by_name(CH,b,c,d,CI,e)\
    (*rs_attr_v0_0_c_epv.rs_attr_lookup_by_name)\
    (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_update(CH,b,c,d,e,CI,f)\
    (*rs_attr_v0_0_c_epv.rs_attr_update) (HANDLE(CH),b,c,d,e,CI,f)

#define Crs_attr_test_and_update(CH,b,c,d,e,f,g,CI,h)\
    (*rs_attr_v0_0_c_epv.rs_attr_test_and_update) (HANDLE(CH),b,c,d,e,f,g,CI,h)

#define Crs_attr_delete(CH,b,c,d,e,CI,f)\
    (*rs_attr_v0_0_c_epv.rs_attr_delete) (HANDLE(CH),b,c,d,e,CI,f)

#define Crs_attr_get_referral(CH,b,c,d,CI,e)\
    (*rs_attr_v0_0_c_epv.rs_attr_get_referral)\
    (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_get_effective(CH,b,c,d,e,CI,f)\
    (*rs_attr_v0_0_c_epv.rs_attr_get_effective) (HANDLE(CH),b,c,d,e,CI,f)



/* Private Routines */

/* This routine sec_rgy_name_complete should be wire-ified
 * and moved to a more general location
 */
/* s e c _ r g y _ n a m e _ c o m p l e t e
 *
 * Use input domain to get the name of the domain.
 * If domain does not correspond to any of the three domains, return error.
 * Concatenate object_name to the domain name in complete_name.
 * 
 * Status codes:
 *     sec_rgy_bad_domain - input domain invalid.
 *     sec_rgy_bad_name - input object_name too long.
 *     error_status_ok.
 */
PRIVATE boolean32 sec_rgy_name_complete (
    sec_rgy_domain_t    domain,                 
    sec_rgy_name_t      object_name,
    sec_rgy_name_t      complete_name,
    error_status_t      *st_p
)
{
    long            object_name_len;
                                     
    CLEAR_STATUS(st_p);               
    object_name_len = u_strlen(object_name);

    if( object_name_len > 
        (sizeof(sec_rgy_name_t) - 1 - sizeof("principal/")) ) {
	SET_STATUS(st_p, sec_rgy_bad_name);
	return false;
    }

    /* Retrieve the name_key record of the given domain directory.
     */                              
    if(!check_domain(domain)) {
        SET_STATUS(st_p, sec_rgy_bad_domain);
        return false;
    }
    switch(domain) {
    case sec_rgy_domain_person:
        u_strcpy(complete_name, "principal");
	break;
    case sec_rgy_domain_group:
        u_strcpy(complete_name, "group");
	break;
    case sec_rgy_domain_org:
        u_strcpy(complete_name, "org");
	break;
    }

    if(object_name_len > 0) {
        u_strcat(complete_name, "/");
        u_strcat(complete_name, object_name);
    }
    return true;
}



PUBLIC void sec_rgy_attr_cursor_init
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [out] */ unsigned32 *cur_num_attrs,
	/* [out] */ sec_attr_cursor_t *cursor,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    CLEAR_STATUS(st_p);

    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    /* allocate a cursor */
    if(!(*cursor = (sec_attr_cursor_t)malloc(sizeof(rs_attr_cursor_t)))) {
	SET_STATUS(st_p, sec_attr_no_memory);
	return;
    }

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_cursor_init(context, 
	    (sec_attr_component_name_t)component_name, 
	    cur_num_attrs, (rs_attr_cursor_t *)(*cursor),
	    &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_attr_cursor_alloc
(
	/* [out] */ sec_attr_cursor_t *cursor,
	/* [out] */ error_status_t *st_p
)
{
    CLEAR_STATUS(st_p);

    /* allocate a cursor */
    if(!(*cursor = (sec_attr_cursor_t)malloc(sizeof(rs_attr_cursor_t)))) {
	SET_STATUS(st_p, sec_attr_no_memory);
	return;
    }
    memset(*cursor, 0, sizeof(rs_attr_cursor_t));
}


PUBLIC void sec_rgy_attr_cursor_reset
(
	/* [in, out] */ sec_attr_cursor_t *cursor,
	/* [out] */ error_status_t *st_p
)
{
    CLEAR_STATUS(st_p);

    if(!(*cursor)) {
	SET_STATUS(st_p, sec_attr_bad_cursor);
	return;
    }
    memset(*cursor, 0, sizeof(rs_attr_cursor_t));
}


PUBLIC void sec_rgy_attr_cursor_release
(
	/* [in, out] */ sec_attr_cursor_t *cursor,
	/* [out] */ error_status_t *st_p
)
{
    CLEAR_STATUS(st_p);

    if(*cursor) {
	free(*cursor);
	*cursor = NULL;
    }
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
PRIVATE boolean32 sec_rgy_try_trigger
(
    handle_t            h,			/* [in] */
    sec_rgy_name_t	component_name,		/* [in] */
    sec_rgy_name_t 	cell_name,		/* [in] */
    sec_attr_t		*attr_keys_p,		/* [in] */
    sec_attr_t		*sec_attr_p,		/* [in/out] */
    error_status_t 	*st_p			/* [out] */
)
{
    unsigned32			num_left, num_trig_attrs;
    sec_attr_t			tmp_sec_attr_p;
    sec_attr_trig_cursor_t	cursor;
    sec_rgy_name_t              full_princ_name; 
    sec_attr_trig_timeval_sec_t time_to_live;

    CLEAR_STATUS(st_p);
    /*
     * The NULL represents the default security login context
     * for the current address space (i.e. the user's login
     * context).
     */
    rpc_binding_set_auth_info( h, SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p),
	SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p),
	SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p), NULL,
	SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p), st_p);

    /* If set_auth_info fails do another import_next. */
    if (BAD_STATUS(st_p)) {
	return (FALSE);			/* continue new binding handle needed */
    }

    /*
     * Hardwired to handle a set number of attributes at a time,
     * in this case 1.
     */

    sec_attr_trig_query( h, cell_name, component_name, &cursor, 1, 1,
	attr_keys_p, &num_trig_attrs, &tmp_sec_attr_p, &time_to_live, &num_left,
	st_p);

    if(GOOD_STATUS(st_p)) {
        sec_attr_util_inst_free_ptrs(sec_attr_p);
        sec_attr_util_alloc_copy(NULL, tmp_sec_attr_p, sec_attr_p, st_p);
	/* 
         * Not checking the above status, because if it fails then 
	 * we don't want to try the trigger again.
	 */
        return (TRUE);		/* with good status */
    } else {
        /*
         * If status indicates the kind of comm failure that
         * may be remidied by a new binding handle, then do
         * another import_next retry, else sec_attr_trig_query
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

PRIVATE boolean32 sec_rgy_bind_and_invoke_trigger
(
    sec_rgy_name_t	component_name,			/* [in] */
    sec_rgy_name_t 	cell_name,			/* [in] */
    sec_attr_t		*attr_keys_p,			/* [in] */
    sec_attr_t		*sec_attr_p,			/* [in/out] */
    error_status_t 	*st_p				/* [out] */
)
{
    handle_t           	h;
    rpc_ns_handle_t	context;
    error_status_t	tmp_status;
    unsigned32		k, done = FALSE;

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
             * The sec_rgy_try_trigger has 3 possible return values:
             *
             *   If the status is good and it returns true then
             *   we found the trigger succesfuly.
             *
             *   If the status is bad, but the return value is true
             *   then we have hit a problem, that means don't try
             *   another binding.
             *
             *   If bad status, and return is false then its ok to
             *   try another binding.
             */

	    done = sec_rgy_try_trigger(h, component_name, cell_name, 
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
                sec_attr_trig_v0_0_c_ifspec, NULL, 
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

	    		done = sec_rgy_try_trigger(h, component_name, cell_name,
				attr_keys_p, sec_attr_p, st_p);

			/*
                         * Don't want to loose the status from
                         * the sec_rgy_try_trigger() call above, so
                         * use tmp_status here.
                         */

		        rpc_binding_free (&h, &tmp_status);

                } /* end if sec_attr_bind_auth_dce */
            } /* end while (!done) */
            /* Perform import_done after a successful call
             * to import_begin and after all import_next
             * calls have been completed.
             */
	    rpc_ns_binding_import_done(&context, &tmp_status);
	    break;
	default:
	    break;
        } /* end of switch on bind type */
    } /* end for each trigger binding */
    /* 
     * If not done, return an ERA whose:
     *     - attribute type is that of the q-trig ERA in question,
     *     - encoding type is "sec_attr_enc_type_status,"
     *     - attr_value is the error_status_t returned by sec_attr_trig_query
     */
    if (!done) {
        sec_attr_util_inst_free_ptrs(sec_attr_p);	
	SA_ATTR_ENCODING(sec_attr_p) = sec_attr_enc_type_status;
	SA_ATTR_STATUS(sec_attr_p) =  *st_p;
        CLEAR_STATUS(st_p);
    }
    return (done);		
}

/*
 *   IT IS ASSUMED THAT EACH ATTRIBUTE TRIGGER WILL RETURN AN EXACT
 *   NUMBER OF ATTRIBUTES BACK BASED ON THE NUMBER OF ATTRIBUTES 
 *   PASSED IN.  IN THIS CASE ONLY ONE ATTRIBUTE WILL BE PASSED IN
 *   AND ONLY ONE TRIGGER ATTRIBUTE WILL COME BACK FROM THE TRIGGER.
 */
PRIVATE void sec_rgy_triggers
(
    sec_rgy_name_t	component_name,		/* [in] */
    unsigned32		num_attr_keys,		/* [in] */
    sec_attr_t 		attr_keys[],		/* [in] */
    unsigned32 		*num_attrs,		/* [in] */
    sec_attr_t 		attrs[],		/* [in/out] */
    error_status_t 	*st_p			/* [in] */
)
{
    sec_attr_t			*sec_attr_p, *attr_keys_p = NULL;
    unsigned32			i, j, result = FALSE;
    unsigned32			*processed = NULL;
    sec_rgy_name_t              cell_name;
    char                        *cell_namep=NULL;
    sec_attr_trig_timeval_sec_t time_to_live;


    dce_cf_get_cell_name(&cell_namep, st_p);
    if (*st_p != dce_cf_st_ok)
	return;
    if (strlen(cell_namep) > sec_rgy_name_max_len) {
        *st_p=sec_attr_bad_name;
        free(cell_namep);
        return;
    }
    u_strncpy(cell_name, cell_namep, sec_rgy_name_max_len);
    free(cell_namep);
    cell_namep=NULL;

    if ( num_attr_keys != 0 ) {
        if(!(processed = malloc(num_attr_keys * sizeof(unsigned32)))) {
	    SET_STATUS(st_p, sec_attr_no_memory);
	    return;
        }
    	memset(processed, FALSE, num_attr_keys * sizeof(unsigned32));
    } else {
	if(!(attr_keys_p = (sec_attr_t *) malloc(sizeof(sec_attr_t)))) {
	    SET_STATUS(st_p, sec_attr_no_memory);
	    return;
	}
    }

    /* 
     * Walk down the list of attributes
     */
    for (i=0; i < *num_attrs; i++) {
	sec_attr_p = &attrs[i];
	if (SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_trig_binding) {
	    if ( num_attr_keys == 0 ) {
		memcpy(attr_keys_p, sec_attr_p, sizeof(sec_attr_t));
		SA_ATTR_ENCODING(attr_keys_p) = sec_attr_enc_void;	
		sec_rgy_bind_and_invoke_trigger(component_name,
			    cell_name, attr_keys_p, sec_attr_p, st_p);
		
	    } else {
	        /* 
	         * We don't want to reprocess attribute keys that's
	         * why we check to see if its been marked.
	         */
	        for (j = 0; j < num_attr_keys && processed[j] == FALSE; j++) {
		    attr_keys_p = &attr_keys[j];
		    result = uuid_equal(&(SA_ATTR_ID(sec_attr_p)),
		                &(SA_ATTR_ID(attr_keys_p)), st_p);
    		    if ( result == TRUE && GOOD_STATUS(st_p) ) { 
		        processed[j] = TRUE;
		        sec_rgy_bind_and_invoke_trigger(component_name,
			    cell_name, attr_keys_p, sec_attr_p, st_p);
    	            } 
		}
    	    }
        }
    }
    if ( num_attr_keys != 0 ) {
	free (processed);
    } else {
	free(attr_keys_p);
    }

}

PUBLIC void sec_rgy_attr_lookup_by_id
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in, out] */ sec_attr_cursor_t *cursor,
	/* [in] */ unsigned32 num_attr_keys,
	/* [in] */ unsigned32 space_avail,
	/* [in] */ sec_attr_t attr_keys[],
	/* [out] */ unsigned32 *num_returned,
	/* [out] */ sec_attr_t attrs[],
	/* [out] */ unsigned32 *num_left,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_lookup_by_id(context,
	    (sec_attr_component_name_t)component_name, 
		(rs_attr_cursor_t *)(*cursor),
		num_attr_keys, space_avail, attr_keys, num_returned, 
		attrs, num_left, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
	    sec_rgy_triggers(component_name,
		num_attr_keys, attr_keys, num_returned, attrs, st_p);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_lookup_no_expand
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in, out] */ sec_attr_cursor_t *cursor,
	/* [in] */ unsigned32 num_attr_keys,
	/* [in] */ unsigned32 space_avail,
	/* [in] */ sec_attr_t attr_keys[],
	/* [out] */ unsigned32 *num_returned,
	/* [out] */ sec_attr_t attr_sets[],
	/* [out] */ unsigned32 *num_left,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;
    sec_attr_t          *sec_attr_p, *attr_keys_p;
    unsigned32		i, j;
    unsigned32		*skip;
    boolean32		tagged;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_lookup_no_expand(context, 
	    (sec_attr_component_name_t)component_name, 
		(rs_attr_cursor_t *)(*cursor),
		num_attr_keys, space_avail, attr_keys, num_returned, 
		attr_sets, num_left, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
	    sec_rgy_triggers(component_name, num_attr_keys, attr_keys,
		num_returned, attr_sets, st_p);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_lookup_by_name
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in] */ unsigned_char_t *attr_name,
	/* [out] */ sec_attr_t *attr,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    sec_attr_t			*fake_attr_p;
    char                        *cell_namep;
    sec_rgy_name_t              cell_name;

    dce_cf_get_cell_name(&cell_namep, st_p);
    if (*st_p != dce_cf_st_ok)
	return;
    if (strlen(cell_namep) > sec_rgy_name_max_len) {
        *st_p=sec_attr_bad_name;
        free(cell_namep);
        return;
    }
    u_strncpy(cell_name, cell_namep, sec_rgy_name_max_len);
    free(cell_namep);
    cell_namep=NULL;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_lookup_by_name(context,
	    (sec_attr_component_name_t)component_name, 
	    attr_name, attr, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
	    if (SA_ATTR_ENCODING(attr) == sec_attr_enc_trig_binding) {
		if(!(fake_attr_p = (sec_attr_t *) malloc(sizeof(sec_attr_t)))) {
		    SET_STATUS(st_p, sec_attr_no_memory);
		} else {
    		    memcpy(fake_attr_p, attr, sizeof(sec_attr_t));
		    SA_ATTR_ENCODING(fake_attr_p) = sec_attr_enc_void;
		    sec_rgy_bind_and_invoke_trigger(component_name, cell_name,
			attr, fake_attr_p, st_p);
		}
	    }
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_update
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in] */ unsigned32 num_to_write,
	/* [in] */ unsigned32 space_avail,
	/* [in] */ sec_attr_t attrs[],
	/* [out] */ unsigned32 *num_returned,
	/* [out] */ sec_attr_t out_attrs[],
	/* [out] */ unsigned32 *num_left,
	/* [out] */ signed32 *failure_index,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_write, st_p) {
	Crs_attr_update(context,
	    (sec_attr_component_name_t)component_name, 
	    num_to_write, attrs, failure_index, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_test_and_update
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in] */ unsigned32 num_to_test,
	/* [in] */ sec_attr_t test_attrs[],
	/* [in] */ unsigned32 num_to_write,
	/* [in] */ sec_attr_t update_attrs[],
	/* [out] */ signed32 *failure_index,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_write, st_p) {
	Crs_attr_test_and_update(context,
	    (sec_attr_component_name_t)component_name, 
	    num_to_test, test_attrs, num_to_write, update_attrs, 
	    failure_index, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_delete
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in] */ unsigned32 num_to_delete,
	/* [in] */ sec_attr_t attrs[],
	/* [out] */ signed32 *failure_index,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_write, st_p) {
	Crs_attr_delete(context,
	    (sec_attr_component_name_t)component_name, 
	    num_to_delete, attrs, failure_index, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_attr_get_effective
(
	/* [in] */ sec_rgy_handle_t context,
	/* [in] */ sec_rgy_domain_t name_domain,
	/* [in] */ sec_rgy_name_t name,
	/* [in] */ unsigned32 num_attr_keys,
	/* [in] */ sec_attr_t attr_keys[],
	/* [out] */ sec_attr_vec_t *attr_list,
	/* [out] */ error_status_t *st_p
)
{
    rs_cache_data_t	cache_info;
    sec_rgy_name_t	component_name;

    CLEAR_STATUS(st_p);

    /* Need to convert domain/name to component name */
    if(!(sec_rgy_name_complete(name_domain, name, component_name, st_p))) {
	return;
    }

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_get_effective(context,
	    (sec_attr_component_name_t)component_name, 
		num_attr_keys, attr_keys, attr_list,
		&cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

