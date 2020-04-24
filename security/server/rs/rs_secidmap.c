/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_secidmap.c,v $
 * Revision 1.1.13.2  1996/02/18  00:20:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:17  marty]
 *
 * Revision 1.1.13.1  1995/12/08  17:56:41  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/09/29  14:28 UTC  jrr
 * 	Create bypass for sec_id_* calls.
 * 
 * 	HP revision /main/HPDCE02/sommerfeld_dfsperf/1  1995/09/15  16:00 UTC  sommerfeld
 * 	flush *BOGUS* audit point.
 * 	add loopback hook.
 * 
 * 	HP revision /main/HPDCE02/3  1994/12/08  17:37 UTC  mdf
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_0/2  1994/12/08  17:35 UTC  mdf
 * 	Backout previous fix.
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_0/1  1994/12/07  13:58 UTC  mdf
 * 	Fix for defect CHFts12974
 * 
 * 	In secd, rs_secidmap.c, rs_sec_id_parse_name(), it doesn't check the status
 * 	returned from rsdb_util_path_resolve before continuing on with the processing.
 * 	This can (and has) cause secd to dump core with a fatal error in
 * 	rsdb_pgo_uid_of_named_item()
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/05  19:49 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_bugfix/1  1994/11/29  16:17 UTC  mullan_s
 * 	Merge fix for CHFts12984 (secd crashes in sec_id_parse_name of '/.../%Network Trash Folder')
 * 	to Mothra.
 * 
 * 	HP revision /main/KK_FIX/1  1994/08/18  19:14 UTC  rmm
 * 	merge ahop's secd fix back for KK patch.
 * 
 * 	HP revision /main/rmm_secd/1  1994/08/11  15:09 UTC  rmm
 * 	return on bad status in parsename
 * 	[1995/12/08  17:21:40  root]
 * 
 * Revision 1.1.9.4  1994/09/30  20:52:20  kline_s
 * 	CR 12344 - Add secidmap audit points not previously submitted
 * 	due to CR 11166.
 * 	[1994/09/30  20:49:22  kline_s]
 * 
 * Revision 1.1.9.3  1994/06/06  16:02:03  rps
 * 	fix raw domain index into rsdb_acl_mgr_uuids
 * 	[1994/06/03  19:49:48  rps]
 * 
 * Revision 1.1.9.2  1994/06/02  21:20:37  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:12  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:12  mdf]
 * 
 * 	HP revision /main/ODESSA_2/1  1994/05/17  18:05 UTC  rps
 * 	merge osf1_1 back to odessa_2
 * 
 * 	HP revision /main/rps_b0/1  1994/05/06  19:08 UTC  rps
 * 	hcell b0 1.1->odessa2 merge
 * 
 * Revision 1.1.9.1  1994/04/22  19:14:01  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:15:03  rps]
 * 
 * Revision 1.1.7.2  1994/03/29  23:45:03  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.6.5  1993/02/09  16:55:29  sommerfeld
 * 	[OT7171] fix error in non-ansi function headers (oops).
 * 	[1993/02/09  16:18:57  sommerfeld]
 * 
 * Revision 1.1.6.4  1993/02/08  15:59:01  sommerfeld
 * 	Add new operations which do the same thing as the old ones plus
 * 	returning caching info.
 * 	[1993/02/04  22:35:37  sommerfeld]
 * 
 * Revision 1.1.6.3  1992/12/29  16:37:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:54  zeliff]
 * 
 * Revision 1.1.6.2  1992/10/07  20:37:23  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:40  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  17:28:43  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.3.2  1992/06/24  21:19:15  sommerfeld
 * 	Fix typo in previous change.
 * 	[1992/06/22  20:21:55  sommerfeld]
 * 
 * 	Prevent against information leakage by zeroing all output arguments
 * 	at the top of the file.
 * 	[1992/06/22  20:12:40  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:44:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry Server - PGO name management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_secidmap.c,v 1.1.13.2 1996/02/18 00:20:11 marty Exp $";
#endif

#include <rs_secidmap.h>
#include <un_strng.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <rsdb_policy.h>
#include <rsdb_pgo.h>
#include <rsdb_acl.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_auth.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */


PRIVATE void sec_id_convert_to_cell_name
  (
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
  )
{       
    rsdb_name_util_convert_to_cell(cell_namep, status);
    if (BAD_STATUS(status)) {
        SET_STATUS(status, sec_id_e_bad_cell_uuid);
    }
}

PRIVATE void sec_id_convert_from_global_name
  (
    sec_rgy_name_t  cell_namep,
    sec_rgy_name_t  unresolved_namep,
    boolean32       *islocal,
    error_status_t  *status
  )
{
    CLEAR_STATUS(status); 

    rsdb_name_util_convert_from_global(cell_namep, unresolved_namep,
                                       islocal, status);
    if (BAD_STATUS(status)) {
        SET_STATUS(status, sec_id_e_name_too_long);
    }
}

/*
 * r s e c _ g e t _ l o c a l _ p r i n c i p a l _ i n f o
 *
 * Get a local principal's name and uuid info.
 */

PRIVATE void rsec_get_local_principal_info
  (
    sec_rgy_domain_t            domain,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              unresolved_name,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
  )
{

    if (FLAG_SET(selector, rsec_id_output_select_puuid)) {
        rsdb_pgo_name_to_uid(domain, unresolved_name, princ_idp,
                                status);
    }

    if (STATUS_OK(status)) {
        if (FLAG_SET(selector, rsec_id_output_select_pname)) {
            strcpy((char *) princ_namep, (char *) unresolved_name);
        } else {
            princ_namep[0] = '\0';
        }
    }
}



/*
 * r s _ s e c _ i d _ p a r s e _ n a m e            AKA redbox-internals
 *
 * Parse a global principal name into a cell name and a cell relative
 * principal name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped (On the wire this is controlled by the 
 * selector parameter).
 */
 
PUBLIC void rs_sec_id_parse_name
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
  )
{
    long                    global_name_len;
    long                    resolved_len;
    sec_rgy_name_t          unresolved_name;
    rsdb_named_item_t       item;
    boolean32               islocal;
    sec_rgy_properties_t    cell_properties;
    error_status_t          lst;

    CLEAR_STATUS(status);

    sec_id_convert_from_global_name(global_namep, unresolved_name, &islocal,
                                    status);
    if(BAD_STATUS(status)) {
	return;
    }

    if (islocal) {
        /*
         * we are dealing with a cell relative name - so simply fill
         * in the cell portion, and go look for the principal info. 
         */
        rsdb_properties_get_info(&cell_properties, status);
        if (FLAG_SET(selector, rsec_id_output_select_cname)) {
            strcpy((char *) cell_namep, (char *) cell_properties.realm);
        }
        if (FLAG_SET(selector, rsec_id_output_select_cuuid)) {
            *cell_idp = cell_properties.realm_uuid;
        }

        rsec_get_local_principal_info(domain, selector, unresolved_name,
                                        princ_namep, princ_idp, status);

    } else {
        global_name_len = strlen((char *) unresolved_name);
        rsdb_util_path_resolve(sec_rgy_domain_person, unresolved_name,
                                &global_name_len, cell_namep,
                                &resolved_len, &item, status);
        if (resolved_len != 0 && RSDB_IS_PGO(&item)) {
            /*
             * We were able to find some info, so continue... 
             */

            /*
             * Delete the trailing "/" if it exists
             */
            if (cell_namep[resolved_len-1] == PATH_COMPONENT_SEPARATOR) {
                resolved_len--;
                cell_namep[resolved_len] = '\0';
            }

            rsdb_pgo_uid_of_named_item(sec_rgy_domain_person, &item,
                                        cell_idp);

            rsdb_properties_get_info(&cell_properties, &lst);
            islocal = uuid_equal(&cell_properties.realm_uuid, cell_idp,
                                    &lst);

            if (STATUS_OK(status)) {
                /* (We finally check the status argument from the path
                 *  resolution here)
                 * We've completely resolved the name - so we were
                 * simply naming a foreign cell with no principal
                 * info, we'll yield a null string for the principal
                 * info and UUID_NIL 
                 */
                uuid_create_nil(princ_idp, &lst);
                princ_namep[0] = '\0';
            } else {
                CLEAR_STATUS(status);
                /*
                 * Either we have a global name for the local cell -
                 * in which case we can resolve the principal
                 * information, or we have named a foreign cell, and
                 * we will have to get a referral to the foreign cell. 
                 */
                if (islocal) {
                    /*
                     * This global name named a local entry, get the
                     * principal info.
                     */
                    rsec_get_local_principal_info(domain, selector,
                                                    unresolved_name,
                                                    princ_namep,
                                                    princ_idp,
                                                    status);
                }
            }
            if (!islocal) {
                /*
                 * this may be a principal in a foreign cell...
                 * set the status code to force a referral.
                 */
                uuid_create_nil(princ_idp, &lst);
                if (FLAG_SET(selector, rsec_id_output_select_pname)) {
                    strcpy((char *) princ_namep, 
                                (char *) unresolved_name);
                }
                SET_STATUS(status, sec_id_e_foreign_cell_referral);
            }
            if (STATUS_EQUAL(status, sec_id_e_foreign_cell_referral) ||
                    FLAG_SET(selector, rsec_id_output_select_cname)) {
                /*
                 * Convert the cell name to global name syntax
                 */
                sec_id_convert_to_cell_name(cell_namep, &lst);
                if (!STATUS_OK(&lst)) {
                    *status = lst;
                }
            } else {
                cell_namep[0] = '\0';
            }
        } else {
            SET_STATUS(status, sec_rgy_object_not_found);
            cell_namep[0] = '\0';
        }
    }
}


PUBLIC void rsec_id_parse_name_cache
  (
    handle_t			h,
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    rs_cache_data_t		*cache_info,
    error_status_t              *status
  )
{
    rsec_id_parse_name (h, domain, global_namep, selector, 
	cell_namep, cell_idp,
	princ_namep, princ_idp,
	status);
    rs_util_get_cache_data(cache_info);
}



/*
 * r s e c _ i d _ p a r s e _ n a m e            AKA redbox
 *
 * Parse a global principal name into a cell name and a cell relative
 * principal name.  Return the IDs associated with each component.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped (On the wire this is controlled by the 
 * selector parameter).
 *
 *  If princ_idp is returned, perform an access check
 *  to verify that the client has any access to the principal
 *  with that uuid.
 */
 
PUBLIC void rsec_id_parse_name
  (
    handle_t                    h,
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
  )
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;                   
    error_status_t          tmp_status;
    uuid_t                  *mgr_type;

    CLEAR_STATUS(status);
    /*
     * Initialize output parameters
     */

    if (cell_idp) 
	uuid_create_nil (cell_idp, &tmp_status);
    if (princ_idp)
	uuid_create_nil (princ_idp, &tmp_status);
    memset (cell_namep, 0, sizeof(sec_rgy_name_t));
    memset (princ_namep, 0, sizeof(sec_rgy_name_t));

    if (!check_domain(domain)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(domain)]);

    BEGIN_READ(lock_db, status) {

        rs_sec_id_parse_name(domain, global_namep, selector,
                             cell_namep, cell_idp, princ_namep,
                             princ_idp, status);
        if (STATUS_OK(status) && 
           FLAG_SET(selector, rsec_id_output_select_puuid)) {
            /* Check client for any access to local principal 
             * Get local principal name if you don't already have it.
             */
            if (!FLAG_SET(selector, rsec_id_output_select_pname)) {
                rsdb_pgo_uid_to_name(domain, princ_idp, 
                                     princ_namep, &tmp_status);
            }
            requested_perms = NULL;
            if (!rs_auth_is_authorized(h, requested_perms, parent_perms,
                        domain, princ_namep, mgr_type,
                        false, &client_ids, &tmp_status)) {
                COPY_STATUS(&tmp_status, status);
                /* If not authorized, clear return values */
                uuid_create_nil(princ_idp, &tmp_status);
                if(FLAG_SET(selector, rsec_id_output_select_cname)) 
                    cell_namep[0] = '\0';
                if(FLAG_SET(selector, rsec_id_output_select_pname)) 
                    princ_namep[0] = '\0';
                if(FLAG_SET(selector, rsec_id_output_select_cuuid)) 
                    uuid_create_nil(cell_idp, &tmp_status);
            }
            if (!FLAG_SET(selector, rsec_id_output_select_pname)) {
                princ_namep[0] = '\0';
            }
        }

    } END_READ

}


/*
 * r s _ s e c _ i d _ g e n _ n a m e              AKA bluebox-internals
 *
 * Generate a global name from the cell and principal uuids.  Also returns
 * the name of the cell uuid and the principal uuid as independent units.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped (On the wire this is controlled by the 
 * selector parameter).
 */
PUBLIC void rs_sec_id_gen_name
  (
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    error_status_t              *status
  )
{
    sec_rgy_properties_t    cell_properties;

    princ_namep[0] = '\0';
    cell_namep[0] = '\0';

    rsdb_pgo_uid_to_name(sec_rgy_domain_person, cell_idp, cell_namep, status);
    if (STATUS_OK(status)) {
        sec_id_convert_to_cell_name(cell_namep, status);

        /*
         * If the cell uuid was for a foreign cell don't bother
         * looking for the principal information locally, generate
         * a referral to the foreign cell.
         */
        rsdb_properties_get_info(&cell_properties, status);
        if ((!uuid_equal(&cell_properties.realm_uuid, cell_idp, status)) &&
            (FLAG_SET(selector, rsec_id_output_select_pname) ||
             FLAG_SET(selector, rsec_id_output_select_gname)) ) {

            SET_STATUS(status, sec_id_e_foreign_cell_referral);
        }
    } else {
        SET_STATUS(status, sec_id_e_bad_cell_uuid);
        cell_namep[0] = '\0';
    }

    /*
     * Pick up the principal name if it is requested (or if we need to
     * construct a global name.
     */
    if (STATUS_OK(status) &&
        (FLAG_SET(selector, rsec_id_output_select_pname) ||
         FLAG_SET(selector, rsec_id_output_select_gname) ) ) {
        rsdb_pgo_uid_to_name(domain, princ_idp, princ_namep, status);
    }

    /*
     * Construct the global name if requested...
     */
    if (STATUS_OK(status) && FLAG_SET(selector, rsec_id_output_select_gname)) {
        int cname_len;
        int pname_len;
        int gname_len;

        /*
         * Global name is <cell name>/<prin name> plus trailing null.  Make
         * sure we will have room for this string.
         */
        cname_len = strlen((char *) cell_namep);
        pname_len = strlen((char *) princ_namep);
        gname_len = cname_len + pname_len + 2;

        if (gname_len > sizeof(sec_rgy_name_t)) {
            SET_STATUS(status, sec_id_e_name_too_long);
        } else {
            strcpy((char *) global_namep, (char *) cell_namep);
            global_namep[cname_len] = PATH_COMPONENT_SEPARATOR;
            strcpy((char *) &global_namep[cname_len+1], (char *) princ_namep);
        }

        /*
         * Null out cell or princ names if they were'nt requested.
         */
        if (!FLAG_SET(selector, rsec_id_output_select_cname)) {
            cell_namep[0] = '\0';
        }
        if (!FLAG_SET(selector, rsec_id_output_select_pname)) {
            princ_namep[0] = '\0';
        }
    }            
}

void rsec_id_gen_name_cache
  (
    handle_t                    h,
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    rs_cache_data_t             *cache_info,
    error_status_t              *status
  )
{
    rsec_id_gen_name (h, domain, cell_idp, princ_idp, selector, global_namep,
	cell_namep, princ_namep, status);
    rs_util_get_cache_data(cache_info);
}

/*
 * r s e c _ i d _ g e n _ n a m e              AKA bluebox
 *
 * Generate a global name from the cell and principal uuids.  Also returns
 * the name of the cell uuid and the principal uuid as independent units.
 * A NULL pointer for an output value will cause that portion of the
 * translation to be skipped (On the wire this is controlled by the 
 * selector parameter).
 *
 *  If global_namep or princ_namep are returned, perform an access check
 *  to verify that the client has any access to the named local principal.
 */
 
PUBLIC void rsec_id_gen_name
  (
    handle_t                    h,
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    error_status_t              *status
  )
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = NULL;
    sec_rgy_foreign_id_t    client_ids;                   
    error_status_t          tmp_status;
    uuid_t                  *mgr_type;

    CLEAR_STATUS(status);
    memset (global_namep, 0, sizeof(sec_rgy_name_t));
    memset (cell_namep, 0, sizeof(sec_rgy_name_t));
    memset (princ_namep, 0, sizeof(sec_rgy_name_t));

    if (!check_domain(domain)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }


    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(domain)]);

    BEGIN_READ(lock_db, status) {
        rs_sec_id_gen_name(domain, cell_idp, princ_idp, selector, global_namep,
                            cell_namep, princ_namep, status);
        if (STATUS_OK(status) && 
           (FLAG_SET(selector, rsec_id_output_select_gname) ||
            FLAG_SET(selector, rsec_id_output_select_pname))) {
            /* Check client for any access to local principal 
             * Get local principal name if you don't already have it.
             */
            if(!FLAG_SET(selector, rsec_id_output_select_pname)) {
                rsdb_pgo_uid_to_name(domain, princ_idp, 
                                     princ_namep, &tmp_status);
            }
            requested_perms = NULL;
            if (!rs_auth_is_authorized(h, requested_perms, parent_perms,
                        domain, princ_namep, mgr_type,
                        false, &client_ids, &tmp_status)) {
                COPY_STATUS(&tmp_status, status);
                /* If not authorized, clear return values */
                if(FLAG_SET(selector, rsec_id_output_select_gname)) 
                    global_namep[0] = '\0';
                if(FLAG_SET(selector, rsec_id_output_select_cname)) 
                    cell_namep[0] = '\0';
                if(FLAG_SET(selector, rsec_id_output_select_pname)) 
                    princ_namep[0] = '\0';
            }
            if (!FLAG_SET(selector, rsec_id_output_select_pname)) {
                princ_namep[0] = '\0';
            }
        }

    } END_READ

}


/*
 * this is a spacer to avoid a bug in certain versions of
 * connection-oriented RPC.
 */

void rsec_id_avoid_cn_bug(h)
    handle_t h;
{
    /* do nothing */
}    
    
/*
 * parse bypass. !!! cut/paste shared with secidmap.c for now..
 */
typedef void (*sec_id_parse_bypass_fn_t)(
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
);

typedef void (*sec_id_gen_bypass_fn_t)(
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    error_status_t              *status
);


void sec_id_bypass (unsigned char *cell_name,
    sec_id_parse_bypass_fn_t parse,
    sec_id_gen_bypass_fn_t gen);

void rs_sec_id_bypass_parse_name(
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              global_namep,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              cell_namep,
    uuid_t                      *cell_idp,
    sec_rgy_name_t              princ_namep,
    uuid_t                      *princ_idp,
    error_status_t              *status
)
{
    BEGIN_READ(lock_db, status) {
        rs_sec_id_parse_name(domain, global_namep, selector,
                             cell_namep, cell_idp, princ_namep,
                             princ_idp, status);
    } END_READ
}

void rs_sec_id_bypass_gen_name(
    sec_rgy_domain_t            domain,
    uuid_t                      *cell_idp,
    uuid_t                      *princ_idp,
    rsec_id_output_selector_t   selector,
    sec_rgy_name_t              global_namep,
    sec_rgy_name_t              cell_namep,
    sec_rgy_name_t              princ_namep,
    error_status_t              *status
)
{
    BEGIN_READ(lock_db, status) {
        rs_sec_id_gen_name(domain, cell_idp, princ_idp, selector, global_namep,
                            cell_namep, princ_namep, status);
    } END_READ
}



void rs_sec_id_install_bypass(status)
    error_status_t *status;
{
    sec_rgy_properties_t    cell_properties;
    rsdb_properties_get_info(&cell_properties, status);
    if (BAD_STATUS(status))
	return;
    sec_id_bypass(cell_properties.realm,
		  rs_sec_id_bypass_parse_name, rs_sec_id_bypass_gen_name);
}
