/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_util.c,v $
 * Revision 1.1.17.1  1996/05/10  13:25:41  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/29  16:43 UTC  hanfei
 * 	use sec_rgy_domain_last in rsdb_util_get_domain_by_name()
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/21  15:42 UTC  hanfei
 * 	replace rsdb_membership_list_key_t by rsdb_pvt_id_key_t
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  15:42 UTC  hanfei
 * 	multi-cell group work: modify rsdb_util_get_domain_by_name() for handling
 * 	domain foreign_person and add function rsdb_util_get_uuid_by_name().
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	Merge acct copy fix (CHFts15845) from mb_mothra5
 * 	[1995/07/28  19:20 UTC  burati  /main/HPDCE02/3]
 *
 * 	Copy only the amount of data necessary for an rsdb_acct_item_t
 * 	[1995/07/24  19:51 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	WP merge
 * 	[1995/05/17  23:58 UTC  rps  /main/HPDCE02/2]
 *
 * 	[OT12669] cell renaming
 * 	[1994/12/15  00:40 UTC  rps  /main/rps_mothra/1]
 *
 * Revision 1.1.10.5  1994/08/04  16:14:54  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:14:09  mdf]
 * 
 * Revision 1.1.10.2  1994/04/22  19:14:25  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:29:05  rps]
 * 
 * 	Moved location of num_attrs and attr_list_id
 * 	[1994/03/30  16:44:57  rps]
 * 
 * Revision 1.1.8.2  1994/03/29  23:49:09  rps
 * 	Support rgy polymorphism
 * 
 * Revision 1.1.10.1  1994/03/02  17:56:12  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:52:51  hanfei]
 * 
 * 	HP revision /main/ODESSA_2/2  1994/02/10  04:51  ahop
 * 	 merge ODESSA_ERA4 up to ODESSA_2
 * 
 * 	HP revision /main/ODESSA_2/ODESSA_ERA4/1  1994/02/07  19:44  mdf
 * 	merge from ODESSA_ERA3 to ODESSA_ERA4.
 * 
 * 	HP revision /main/ODESSA_2/ODESSA_ERA3/1  1994/02/07  01:48  ahop
 * 	 merge it up
 * 
 * 	HP revision /main/ODESSA_2/ahop_odss_era_bl8/1  1994/02/05  20:57  ahop
 * 	set attr_list_id in  rsdb_util_get_obj_info_by_name
 * 
 * 	HP revision /main/ODESSA_ERA2/2  1994/01/31  14:29  ahop
 * 	 merge forward
 * 
 * 	HP revision /main/ODESSA_ERA1/ahop_odss_era_bl4/1  1994/01/29  21:26  ahop
 * 	fix get_obj_info_by_name for ATTR_SCHEMA and for use by rsdb_acl_get_ids_from_name
 * 
 * 	HP revision /main/ODESSA_ERA1/1  1994/01/26  18:09  ahop
 * 	 merge forward
 * 
 * 	HP revision /main/ahop_odss_era_bl1/1  1994/01/25  21:31  ahop
 * 	add rsdb_util_get_obj_info_by_name
 * 
 * Revision 1.1.6.5  1993/01/26  23:13:39  emartin
 * 	bl6d: fix get_next_by_name_key to ensure next key is type NAME_TYPE
 * 	[1993/01/07  18:40:41  emartin]
 * 
 * Revision 1.1.6.4  1992/12/29  16:39:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:29:20  zeliff]
 * 
 * Revision 1.1.6.3  1992/11/18  17:40:20  sekhar
 * 	[OT 5503]  Drop the following fix from Mike Burati
 * 	[1992/11/18  12:31:54  sekhar]
 * 
 * Revision 1.1.7.2  1992/10/15  17:58:02  burati
 * 	CR5503 Don't try to resolve 0 length path
 * 
 * Revision 1.1.6.2  1992/09/29  20:14:29  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:41:20  sekhar]
 * 
 * Revision 1.1.2.3  1992/03/19  22:10:34  ahop
 * 	 fix CR 2050: don't use overlapping strings in a strncpy
 * 	[1992/03/19  16:07:41  ahop]
 * 
 * Revision 1.1.2.2  1992/03/18  22:13:20  ahop
 * 	fix CR 2158: zero keys in get_next_by_name_key and store_new_named_item
 * 	[1992/03/17  20:05:01  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *      Registry Server - database interlude - pgo, dir data structure utilities
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_util.c	26 - 11/11/91";
#endif

#include <stdio.h>
#include <Pointer.h>
#include <bstring.h>
#include <un_strng.h> 

#include <dce/dce.h>
#include <sec_svc.h>
#include <dce/uuid.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rsdb_util.h>
#include <rsdb_pgo_util.h> 
#include <rsdb_name_util.h> 
#include <rsdb_pgo.h>
#include <rsdb_dir.h>
#include <rsdb_acct.h>
#include <rsdb_policy.h>

#define ACCT_SIZE(acct) \
    (acct)->user_part.homedir_len + (acct)->user_part.shell_len \
    + (acct)->user_part.gecos_len + (acct)->key_pepper_len      \
    + (acct)->key_len + sizeof(*(acct)) - sizeof((acct)->string_buf)


/* 
 * Private functions
 */

 /*
  * g e t _ k e y _ o f _ m a r k e r 
  *
  * Use input domain and ID to retrieve the name key.  Return TRUE if name key is
  * found. 
  */
PRIVATE boolean32 get_key_of_marker
#ifndef __STDC__
    ( domain, id, key, keylen ) 
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      id;
    Pointer            key;
    long               *keylen;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    Pointer            key,
    long               *keylen
  )
#endif
{
    char                   *name_key;
    int                    name_key_len;
    rsdb_sequential_key_t  sequential_key;
    error_status_t         st;

    bzero( (Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_fetch(domain_info[domain].db, (Pointer) &sequential_key,
	       sizeof(sequential_key),
                                &name_key, &name_key_len, &st);
    if (BAD_STATUS(&st)) {
        return false;
    }
    if (name_key_len <= *keylen) {
        bcopy(name_key, key, name_key_len);
        *keylen = name_key_len;
    } else {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_buf_too_small);
        /* NOTREACHED */
    }
    return true;
}

 /*
  * s e t _ k e y _ o f _ i d 
  *
  * Replace the key data associated with a given sequential ID in the given
  * domain. 
  */
PRIVATE void set_key_of_id
#ifndef __STDC__
    (domain, id, key, key_len)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      id;
    Pointer            key;
    long               key_len;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    Pointer            key,
    long               key_len
  )
#endif
{
    rsdb_sequential_key_t  sequential_key;
    error_status_t          st;

    bzero( (Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_replace(domain_info[domain].db, (Pointer) &sequential_key,
		 sizeof(sequential_key),
		 key, key_len, &st);
}

/*
 * s e t _ k e y _ o f _ d i r _ u i d
 */
PRIVATE void set_key_of_dir_uid
#ifndef __STDC__
    (domain, uid, key, key_len)
    sec_rgy_domain_t   domain;
    uuid_t             uid;
    Pointer            key;
    long               key_len;
#else
  (
    sec_rgy_domain_t   domain,
    uuid_t             uid,
    Pointer            key,
    long               key_len
  )
#endif
{
    rsdb_uid_key_t     uid_key;
    error_status_t     status;

    bzero( (Pointer)&uid_key, sizeof(uid_key));

    uid_key.record_type = DIRECTORY_UUID_TYPE;
    uid_key.uid         = uid;

    rsdb_replace(domain_info[domain].db, (Pointer)&uid_key,
		 sizeof(uid_key),
		 key, key_len, &status);
}

/*
 * END of private functions 
 */

 /*
  * r s d b _ u t i l _ q u o t a _ d e c r e m e n t 
  *
  * If the principal is unauthenticated/anonymous (princ_uuid is nil), decrement
  * the properties.unauthenticated_quota field.  Else, if the realm is local,
  * decrement the principal's quota (in the principal's PGO).  Else, if the
  *  realm is not local, decrement the quota for the principal
  * associated with the foreign realm.  In any case, if the quota is
  * sec_rgy_pgo_quota_unlimited (-1), do not decrement; return
  * sec_rgy_status_ok.  If the quota is zero, do not decrement; return
  * sec_rgy_quota_exhausted or foreign_quota_exhausted. 
  */
PUBLIC void rsdb_util_quota_decrement
#ifndef __STDC__
    ( princ_uuid, realm_uuid, status )
    uuid_t              *princ_uuid;            /* [in] principal UUID      */
    uuid_t              *realm_uuid;            /* [in] realm of principal  */
    error_status_t      *status;                /* [out]                    */
#else
  (
    uuid_t              *princ_uuid,    
    uuid_t              *realm_uuid,         
    error_status_t      *status
  )
#endif
{
    rsdb_credentials_t      cred;
    rsdb_named_item_t       pgo_item;
    char                    *dummy_name = "";
    uuid_t                  key_uuid;
    sec_rgy_properties_t    properties;

    CLEAR_STATUS(status);

    if(uuid_is_nil(princ_uuid, status)) { 
        /* Check quota for unauthenticated users */
        rsdb_properties_get_info(&properties, status);
        if (BAD_STATUS(status)) {
            return;
        }                              
        if (properties.unauthenticated_quota == sec_rgy_quota_unlimited) {
            /* return success without decrementing */
            return;
        }
        if (properties.unauthenticated_quota > 0) {
            /* decrement, replace properties */
            properties.unauthenticated_quota--;
            rsdb_properties_set_info(&properties, status);
            return;
        }
        /* We are here if quota <= 0 */
        SET_STATUS(status, sec_rgy_quota_exhausted);
        return;
    }
    /* Check quota for authenticated users. */
    if (rsdb_util_is_realm_local(dummy_name, realm_uuid, status)) {
        /* retrieve PGO for principal, decrement its quota */
        key_uuid = *princ_uuid;
    } else {
        /* retrieve PGO for foreign realm, decrement its quota */
        key_uuid = *realm_uuid;
    }
    if (BAD_STATUS(status)) {
        return;
    }
    if (!rsdb_get_credentials_by_uid(rsdb_person_db, &key_uuid, &cred)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    if (!rsdb_pgo_util_get_by_id(sec_rgy_domain_person, 
                                 cred.id, &pgo_item)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    if (RSDB_PGO(&pgo_item).quota == sec_rgy_quota_unlimited) {
        /* return success without decrementing */
        return;
    }
    if (RSDB_PGO(&pgo_item).quota > 0) {
        /* decrement, replace PGO */
        RSDB_PGO(&pgo_item).quota--;
        rsdb_pgo_replace_internal(sec_rgy_domain_person, &pgo_item);
        return;
    }
    /* We are here if quota <= 0 */
    SET_STATUS(status, sec_rgy_quota_exhausted);
    return;
}                               /* End rsdb_util_quota_decrement */


 /*
  * r s d b _ u t i l _ q u o t a _ a v a i l 
  *
  * If the principal is unauthenticated/anonymous (princ_uuid is nil), 
  * check the properties.unauthenticated_quota field.  
  * If the realm is local, check the principal's quota (in the principal's
  * PGO). If the realm is not local, check the quota for the principal
  * associated with the foreign realm.  In either case, if the quota is
  * sec_rgy_pgo_quota_unlimited (-1) or greater than 0, return true
  * with sec_rgy_status_ok.  If the quota is zero, return false with
  * sec_rgy_quota_exhausted or foreign_quota_exhausted. 
  */
PUBLIC boolean32 rsdb_util_quota_avail
#ifndef __STDC__
    ( princ_uuid, realm_uuid, status )
    uuid_t              *princ_uuid;            /* [in] principal UUID      */
    uuid_t              *realm_uuid;            /* [in] realm of principal  */
    error_status_t      *status;                /* [out]                    */
#else
  (
    uuid_t              *princ_uuid,    
    uuid_t              *realm_uuid,         
    error_status_t      *status
  )
#endif
{
    rsdb_credentials_t      cred;
    rsdb_named_item_t       pgo_item;
    char                    *dummy_name = "";
    uuid_t                  key_uuid;
    sec_rgy_properties_t    properties;

    CLEAR_STATUS(status);

    if(uuid_is_nil(princ_uuid, status)) { 
        /* Check quota for unauthenticated users */
        rsdb_properties_get_info(&properties, status);
        if (BAD_STATUS(status)) {
            return false;
        }                              
        if (properties.unauthenticated_quota == sec_rgy_quota_unlimited) {
            return true;
        }
        if (properties.unauthenticated_quota > 0) {
            return true;
        }
        /* We are here if quota <= 0 */
        SET_STATUS(status, sec_rgy_quota_exhausted);
        return false;
    }

    CLEAR_STATUS(status);
    if (rsdb_util_is_realm_local(dummy_name, realm_uuid, status)) {
        /* retrieve PGO for principal */
        key_uuid = *princ_uuid;
    } else {
        /* retrieve PGO for foreign realm  */
        key_uuid = *realm_uuid;
    }
    if (BAD_STATUS(status)) {
        return false;
    }
    if (!rsdb_get_credentials_by_uid(rsdb_person_db, &key_uuid, &cred)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return false;
    }
    if (!rsdb_pgo_util_get_by_id(sec_rgy_domain_person, 
                                 cred.id, &pgo_item)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return false;
    }
    if( (RSDB_PGO(&pgo_item).quota == sec_rgy_quota_unlimited) ||
        (RSDB_PGO(&pgo_item).quota > 0) ) {
        /* return success */
        return true;
    }
    /* We are here if quota <= 0 */
    SET_STATUS(status, sec_rgy_quota_exhausted);
    return false;
}                               /* End rsdb_util_quota_avail */


 /*
  * r s d b _ u t i l _ i s _ r e a l m _ l o c a l 
  *
  * Compare the input realm name and/or realm uuid with the realm info
  * stored in the properties record.  If the realm name has length 
  * zero, don't use it for comparison.  If the realm uuid is nil,
  * don't use it for comparison.  Otherwise, both must match.
  * Return TRUE if they match, FALSE otherwise.  If status
  * is not sec_rgy_status_ok, don't trust boolean result.  Input realm string
  * must be null-terminated. 
  */
PUBLIC boolean32 rsdb_util_is_realm_local
#ifndef __STDC__
    ( realm_name, realm_uuid, status )
    char                *realm_name;            /* [in] realm name          */
    uuid_t              *realm_uuid;            /* [in] realm uuid          */
    error_status_t      *status;                /* [out]                    */
#else
  (
    char                *realm_name,             
    uuid_t              *realm_uuid,             
    error_status_t      *status
  )
#endif
{
    char	*cell_name;
    char	**aliases, **alias;
    sec_rgy_properties_t properties;

    CLEAR_STATUS(status);
    rsdb_properties_get_info(&properties, status);
    if (BAD_STATUS(status)) {
        return false;
    }                              
    if (realm_uuid && !(uuid_is_nil(realm_uuid, status))) {
        if(!(uuid_equal(realm_uuid, &properties.realm_uuid, status))) {
            return false;
        }
    }
    if (ustrlen(realm_name) == (unsigned int) 0) {
	return true;
    }

    dce_cf_get_cell_name(&cell_name, status);
    if (BAD_STATUS(status)) {
	return false;
    }
    if ( !strcmp(cell_name, realm_name)) {
	free(cell_name);
	return true;
    }
    free(cell_name);

    dce_cf_get_cell_aliases(&aliases, status);
    if (BAD_STATUS(status))
	return false;

    if (aliases) {
	for (alias = aliases; *alias; alias++) {
	    if ( !strcmp(*alias, realm_name)) {
		dce_cf_free_cell_aliases(aliases, status);
		return true;
	    }
	}
    }

    dce_cf_free_cell_aliases(aliases, status);
    return false;
}

void rsdb_util_check_cell_name (error_status_t *status)
{
    sec_rgy_properties_t properties;

    CLEAR_STATUS(status);
    rsdb_properties_get_info(&properties, status);
    if (BAD_STATUS(status)) {
	dce_svc_printf(RSDB_CHECK_CELL_FAILURE_MSG);
    }
    if (!rsdb_util_is_realm_local((char *)properties.realm,
				  NULL, status)) {
	dce_svc_printf(RSDB_CELL_MISMATCH_MSG, properties.realm);
    }
}


 /*
  * r s d b _ u t i l _ p a t h _ r e s o l v e 
  *
  * The pathname to resolve is input in the residual parameter.  Within the given
  * domain, the item identified by this pathname is returned.  In the
  * successful case (item is found), the output residual pathname is empty and
  * the full pathname is returned in the resolved parameter.  In the
  * unsuccessful case (a portion of the pathname could not be found) the
  * pathname components naming existing directories are returned in the
  * resolved parameter and the rest is returned in residual.  
  *
  * The pathname must be relative to the domain and must not begin with a slash
  * ('/'). 
  *
  * Status: error_status_ok, sec_rgy_object_not_found 
  */

PUBLIC void rsdb_util_path_resolve
#ifndef __STDC__
    ( domain, residual, residual_len, resolved, resolved_len, item, status )
    sec_rgy_domain_t    domain;         /* [in]                     */
    sec_rgy_name_t      residual;       /* [in, out] unresolved     */ 
    long                *residual_len;  /* [in, out]                */
    sec_rgy_name_t      resolved;       /* [out] resolved part      */ 
    long                *resolved_len;  /* [out]                    */
    rsdb_named_item_t   *item;          /* [out] item               */ 
    error_status_t      *status;        /* [out]                    */
#else
  (
    sec_rgy_domain_t    domain,         /* [in]                     */
    sec_rgy_name_t      residual,       /* [in, out] unresolved     */ 
    long                *residual_len,  /* [in, out]                */
    sec_rgy_name_t      resolved,       /* [out] resolved part      */ 
    long                *resolved_len,  /* [out]                    */
    rsdb_named_item_t   *item,          /* [out] item               */ 
    error_status_t      *status
  )
#endif
{
    char           *resid;      /* advancing ptr into residual  */
    sec_rgy_name_t *name;
    long            name_len;
    rsdb_pvt_id_t   parent_id;
    sec_rgy_name_t  tmp_buf;

    /*
     * For each component in residual (starting with the leftmost),
     * concatenate the component to the pathname in resolved and use the
     * component name and parent ID to lookup the ID of the item identified by
     * the component.  The first component processed should use START_OF_LIST
     * for parent ID, indicating the root directory of the given domain. 
     *
     * Inside get_left_comp, the left-most component pointed to by resid is
     * copied to the end of the pathname in resolved. The same component is
     * returned in the name parameter. The resid pointer is advanced inside
     * get_left_comp to exclude the left component of residual; however, the
     * residual string itself is not altered.   
     */

    CLEAR_STATUS(status);
    item->body.threads.id = START_OF_LIST;
    resid = (char *) residual;
    resolved[0] = '\0';
    *resolved_len = 0;

    if (*residual_len <= 0) {
        SET_STATUS(status, sec_rgy_object_not_found);
    }

    while (*residual_len > 0) {
        rsdb_name_util_get_left_comp(&resid, residual_len,
                      resolved, resolved_len,
                      (char **) &name, &name_len);
        /*
         * ID of item from previous loop is parent for this loop. 
         */
        parent_id = item->body.threads.id;
        if (!rsdb_util_get_by_name_key(domain, parent_id, *name,
                                       name_len, item)) {
            /*
             * This component not found.  Remove it from resolved and make
             * residual include it again. 
             */
            *resolved_len -= name_len;
            resolved[*resolved_len] = '\0';
            resid -= name_len;
            *residual_len += name_len;
            /* Move string pointed to by resid to the beginning of residual.
             * Since not all systems support strcpy-ing between overlapping
             * strings, copy to a temporary buffer first.
             */
            u_strncpy(tmp_buf, resid, *residual_len);
            u_strncpy(residual, tmp_buf, *residual_len);
            residual[*residual_len] = '\0';
            SET_STATUS(status, sec_rgy_object_not_found);
            return;
        }
    }
    residual[0] = '\0';

}                               /* End rsdb_util_path_resolve */

 /*
  * r s d b _ u t i l _ p a t h _ c o n s t r u c t 
  *
  * Given item_id and domain, construct the full pathname of the item and output
  * it in the null-terminated name parameter. If there is no item stored for
  * the given item_id, return sec_rgy_object_not_found.  If the item for the
  * given item_id is found, but the registry database does not contain enough
  * information to construct the entire pathname, the database is inconsistent
  * and a fatal error results. 
  *
  * Status: error_status_ok, sec_rgy_object_not_found, 
  */

PUBLIC void rsdb_util_path_construct
#ifndef __STDC__
    (domain, item_id, name, name_len, status)
    sec_rgy_domain_t    domain;                 /* [in]                     */
    rsdb_pvt_id_t       item_id;                /* [in]                     */
    sec_rgy_name_t      name;                   /* [out] directory name     */
    long                *name_len;              /* [out]                    */
    error_status_t      *status;                /* [out]                    */
#else
  (
    sec_rgy_domain_t    domain,                 /* [in]                     */
    rsdb_pvt_id_t       item_id,                /* [in]                     */
    sec_rgy_name_t      name,                   /* [out] directory name     */
    long                *name_len,              /* [out]                    */
    error_status_t      *status
  )
#endif
{
    sec_rgy_name_t  tmp_name;
    long            tmp_name_len = sizeof(tmp_name);
    char           *name_ptr;
    rsdb_name_key_t name_key;
    long            key_size, key_len = sizeof(name_key);
    boolean32       first = true;

    CLEAR_STATUS(status);
    /*
     * The pathname is constructed in tmp_name from right to left. 
     */
    tmp_name[tmp_name_len - 1] = '\0';
    name_ptr = (char *) &tmp_name[tmp_name_len - 1];

    key_size = key_len;
    while (item_id != START_OF_LIST) {
        /*
         * Get name_key associated with given item_id. Prepend item name to
         * tmp_name. (If this is not the first component in tmp_name, use a
         * '/' separator.) Set item_id equal to parent_id from name_key for
         * next iteration. Set key_len to full size of name_key_t for next
         * iteration. 
         */
        if (!get_key_of_marker(domain, item_id, (Pointer) & name_key, &key_len)) {
            if (first) {
                SET_STATUS(status, sec_rgy_object_not_found);
                return;
            } else {
                dce_svc_printf(SEC_RSDB_DB_INCONSISTENT_MSG);
            }
        }
        if (first)
            first = false;
        else
            ustrncpy(--name_ptr, "/", 1);

        name_ptr -= name_key.name_len;
        ustrncpy(name_ptr, name_key.name, name_key.name_len);
        item_id = name_key.parent_id;
        key_len = key_size;
    }
    *name_len = strlen(name_ptr);
    ustrcpy(name, name_ptr);

}                               /* End rsdb_util_path_construct */


/*
 * r s d b _ u t i l _ b u i l d _ d i r s
 * 
 * Utility routine to build directory tree from pathname
 */
PUBLIC void rsdb_util_build_dirs
#ifndef __STDC__
    (domain, pname, pname_len, status)
    sec_rgy_domain_t    domain;
    sec_rgy_name_t      pname;
    long                pname_len;
    error_status_t      *status;
#else
  (
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      pname,
    long                pname_len,
    error_status_t      *status
  )
#endif
{                                                            
    sec_rgy_name_t      residual;
    long                residual_len;
    sec_rgy_name_t      resolved;
    unsigned_char_p_t   remaining, left_comp;
    long                resolved_len;
    rsdb_named_item_t   item;
                                     
    ustrncpy(residual, pname, pname_len);
    residual[pname_len] = '\0';
    residual_len = pname_len;
    rsdb_util_path_resolve(domain, residual, &residual_len, resolved, &resolved_len,
                            &item, status);
    /* Continue when an empty directory is found along the way */
    if (STATUS_EQUAL(status, sec_rgy_object_not_found))
        CLEAR_STATUS(status);

    /* change trailing pgo to aggregate */
    if (GOOD_STATUS(status) && *resolved) {
	sec_rgy_name_t	pgo;
	long		pgo_len = resolved_len;

	ustrncpy(pgo, resolved, pgo_len);
	if (pgo[pgo_len-1] == '/')
	    pgo_len--;
	pgo[pgo_len] = '\0';

	if (!rsdb_dir_util_get_by_name(domain, pgo, pgo_len, &item))
	    rsdb_dir_create(domain, pgo, status);
    }
    if (BAD_STATUS(status))
	    return;

    left_comp = (unsigned_char_p_t) &(residual[0]);
    while ((left_comp != NULL) && (*left_comp != '\0') && STATUS_OK(status)) {
        remaining = (unsigned_char_p_t) strchr((char *) left_comp, '/');

        /* Terminate the left component of the residual */
        if (remaining != NULL) {
            *remaining = '\0';
            remaining++;
        }

        /* Append the left component to the existing dir */
        strcat((char *) resolved, (char *) left_comp);

        /* Attempt to create the directory */
        rsdb_dir_create(domain, resolved, status);
        strcat((char *) resolved, "/");
        left_comp = remaining;
    }

}



/* 
 * r s d b _ u t i l _ d e l e t e _ d i r s _ i f _ e m p t y
 *
 * Database write lock required around this operation.
 *
 * If dir_name names an existing directory that contains 
 * no subdirectories or pgos, delete it.  Check to see if
 * its parent(s) become empty as a result and delete them.
 * Continue up the hierarchical directory path named by dir_name
 * deleting all empty directories.
 *
 */
PUBLIC void rsdb_util_delete_dirs_if_empty
#ifndef __STDC__
    ( domain, dir_name, status ) 
    sec_rgy_domain_t    domain;     /* [in] */    
    sec_rgy_name_t      dir_name;   /* [in] */
    error_status_t      *(status);  /* [out] */
#else
  (
    sec_rgy_domain_t    domain,     /* [in] */    
    sec_rgy_name_t      dir_name,   /* [in] */
    error_status_t      *(status)
  )
#endif
{                                       
    sec_rgy_name_t      pathname;
    sec_rgy_name_t      leafname;
    long                pathname_len;
    long                leafname_len;
    rsdb_named_item_t	item, newitem;
 
    CLEAR_STATUS(status);
    ustrcpy(pathname, dir_name);
    pathname_len = ustrlen(pathname);

    while( pathname_len > 0 && rsdb_dir_is_empty(domain, pathname, status) ) {
        rsdb_dir_delete(domain, pathname, status);
        rsdb_name_util_strip_leaf(pathname, &pathname_len, 
                                  leafname, &leafname_len);
    }
}


 /*
  * r s d b _ u t i l _ g e t _ b y _ n a m e 
  *
  * Retrieve the named_item identified by the input pathname. Return TRUE if
  * the named_item was found, FALSE otherwise. 
  */
PUBLIC boolean32 rsdb_util_get_by_name
#ifndef __STDC__
    ( domain, name, name_len, item )
    sec_rgy_domain_t    domain;         /* [in]                 */
    sec_rgy_name_t      name;           /* [in]                 */
    long                name_len;       /* [in]                 */
    rsdb_named_item_t   *item;          /* [out]                */                          
#else
  (
    sec_rgy_domain_t    domain,         /* [in]                 */
    sec_rgy_name_t      name,           /* [in]                 */
    long                name_len,       /* [in]                 */
    rsdb_named_item_t   *item
  )
#endif
{
    sec_rgy_name_t  residual;
    long            residual_len;
    sec_rgy_name_t  resolved;
    long            resolved_len;
    error_status_t  status;

    strncpy((char *) residual, (char *) name, name_len);
    residual_len = name_len;
    rsdb_util_path_resolve(domain, residual, &residual_len, resolved,
                           &resolved_len, item, &status);
    if (BAD_STATUS(&status)) {
        return (FALSE);
    } else {
        return (TRUE);
    }

}                               /* End of rsdb_util_get_by_name. */

                          
 /*
  * r s d b _ u t i l _ g e t _ b y _ n a m e _ k e y 
  *
  * Retrieve the named_item (dir/pgo union) identified by the input parent_id
  * and name.  Return TRUE if a named_item was found, FALSE otherwise. 
  *
  */
PUBLIC boolean32 rsdb_util_get_by_name_key
#ifndef __STDC__
    (domain, parent_id, name, namelen, item)
    sec_rgy_domain_t    domain;                 /* [in]                     */
    rsdb_pvt_id_t       parent_id;              /* [in]                     */
    sec_rgy_name_t      name;                   /* [in] name of pgo/dir item*/
    long                namelen;                /* [in]                     */
    rsdb_named_item_t   *item;                  /* [out]                    */
#else
  (
    sec_rgy_domain_t    domain,                 /* [in]                     */
    rsdb_pvt_id_t       parent_id,              /* [in]                     */
    sec_rgy_name_t      name,                   /* [in] name of pgo/dir item*/
    long                namelen,                /* [in]                     */
    rsdb_named_item_t   *item
  )
#endif
{
    rsdb_name_key_t name_key;
    long            name_key_len;

    bzero((Pointer) & name_key, sizeof(name_key));

    name_key.record_type = NAME_TYPE;
    name_key.parent_id = parent_id;
    name_key.name_len = namelen;
    strncpy((char *) name_key.name, (char *) name, namelen);
    name_key_len = NAME_LEN(&name_key);

    return rsdb_util_get_item(domain, (Pointer) & name_key,
                              name_key_len, item);

}                               /* End of rsdb_util_get_by_name_key */

                          
 /*
  * r s d b _ u t i l _ g e t _ d o m a i n _ b y _ n a m e
  *
  * Identify and retrieve the top-level domain directory item (either person,
  * group, or org domain) by its name.  Also return the domain.
  * If name does not correspond to any of the three domains, return error.
  * 
  * Status codes:
  *     sec_rgy_bad_domain - input name doesn't identify any domain.
  *     error_status_ok.
  */
PUBLIC boolean32 rsdb_util_get_domain_by_name
#ifndef __STDC__
    (name, namelen, item, domain, st)
    char                *name;                  /* [in] domain name         */
    long                namelen;                /* [in]                     */
    rsdb_named_item_t   *item;                  /* [out] domain dir item    */
    sec_rgy_domain_t    *domain;                /* [out]                    */
    error_status_t      *st;                    /* [out]                    */
#else
  (
    char                *name,                   
    long                namelen,                
    rsdb_named_item_t   *item,
    sec_rgy_domain_t    *domain,                 
    error_status_t      *st
  )
#endif
{
    rsdb_name_key_t name_key;
    long            name_key_len = sizeof(name_key);
                                     
    CLEAR_STATUS(st);                  
    /* Retrieve the name_key record of each of the three domain directories
     * in sequence, comparing the retrieved name with the input name.
     * When a match occurs, retrieve the domain directory item, set the
     * domain_t and return.  If no match occurs, return status "bad_domain."
     */ 
    for(*domain = sec_rgy_domain_person; *domain < sec_rgy_domain_last;
        (*domain)++) { 
	name_key_len = sizeof(name_key);
        rsdb_util_get_key_of_id(*domain, START_OF_LIST,
                                (Pointer)&name_key, &name_key_len);
        if( namelen == name_key.name_len &&
            ustrncmp(name, name_key.name, namelen) == 0 ) {

            if(rsdb_util_get_item(*domain, (Pointer)&name_key,
                                  name_key_len, item)) {
                return true;
            } else { 
                SET_STATUS(st, sec_rgy_object_not_found);
                return false;
            }
        }
    }
    SET_STATUS(st, sec_rgy_bad_domain);
    return false;
}                               /* End of rsdb_util_get_domain_by_name */


/* r s d b _ u t i l _ g e t _ d o m a i n _ o b j _ b y _ n a m e
 *                            
 * Use the input pathname to determine the domain and
 * to retrieve the object identified by the name.
 * The input name should have one of the following forms:
 *      <domain_dir>                i.e., person OR group OR org
 *      <domain_dir>/<pgo>          i.e., person/ahop
 *      <domain_dir>/<dir>/<pgo>    i.e., person/ccd/ahop
 *      etc.
 * The name must not begin with a slash '/'.
 *
 * Status Codes: 
 *      sec_rgy_bad_name - bad domain component in name.
 *      sec_rgy_object_not_found - name didn't identify any object.
 */                                                       
PUBLIC void rsdb_util_get_domain_obj_by_name
#ifndef __STDC__
    ( name, domain, object, st )
    unsigned_char_t         *name;      /* [in] name of object */
    sec_rgy_domain_t        *domain;    /* [out] domain of object */
    rsdb_named_item_t       *object;    /* [out] object */
    error_status_t          *st;        /* [out] */
#else
    (
    unsigned_char_t         *name,
    sec_rgy_domain_t        *domain,
    rsdb_named_item_t       *object,
    error_status_t          *st
    )
#endif
{      
    char                *i_ptr;              
    sec_rgy_name_t      obj_name; 
    long                obj_name_len;
    int                 domain_name_len;  
    sec_rgy_name_t      resolved;       
    long                resolved_len;
                          
    CLEAR_STATUS(st);

    if( i_ptr = strchr((char *)name, '/') ) {
        domain_name_len = i_ptr - (char *)name;  
        ustrcpy(obj_name, i_ptr + 1);
        obj_name_len = ustrlen(obj_name);
    } else {
        domain_name_len = ustrlen(name);
        obj_name[0] = '\0';              
        obj_name_len = 0;
    }
    if(!rsdb_util_get_domain_by_name((char *)name, domain_name_len,
                                     object, domain, st)) {
        SET_STATUS(st, sec_rgy_bad_name);
        return;
    }
    if(obj_name_len > 0) {  
        /* name identified an object within the domain. */
        rsdb_util_path_resolve(*domain, obj_name, &obj_name_len, resolved,
                               &resolved_len, object, st);
        if(BAD_STATUS(st)) {
            SET_STATUS(st, sec_rgy_object_not_found); 
            return;
        }         
    }                                        
}


/* 
 * r s d b _ u t i l _ g e t _ o b j _ i n f o _ b y _ n a m e
 *
 * Use the input security-relative object name to retrieve the
 * internal ids and other info used by the rsdb layer to identify 
 * and operate on the object.
 * Normally rgy_obj_info.acl_id is set to the id of the object_acl
 * on the object.  For object types (i.e., directories) that suport
 * more than one acl_type, the input acl_type is used to select
 * the proper acl_id (acl_type is otherwise ignored).
 * If the object in not in a legal domain, the domain field of
 * rgy_obj_info is set to sec_rgy_domain_last to avoid confusion.
 * If the object is in a pgo domain, rgy_obj_info.obj_type 
 * is set to pgo or dir.
 * For objects in the pgo domains, the object uuid is returned.
 *
 * Since the name-to-info mapping is costly, the rs layer uses
 * this routine once and passes the rgy_obj_info, rather than
 * a name, to the rsdb layer in subsequent calls.
 * Used by the attribute modules.
 *
 * The input name should have one of the following forms:
 *	policy
 *	replist
 *	attr_schema
 *      <domain_dir>                i.e., person OR group OR org
 *      <domain_dir>/<pgo>          i.e., person/ahop
 *      <domain_dir>/<dir>/<pgo>    i.e., person/ccd/ahop
 *      etc.
 * The name must not begin with a slash '/'.
 */
PUBLIC void rsdb_util_get_obj_info_by_name (
    unsigned_char_t		*name,		/* [in] name of object */
    named_item_type_t		obj_type,	/* [in] pgo_type vs dir_type */
    sec_acl_type_t              acl_type,	/* [in] acl_type of interest */
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,	/* [in,out] */
    uuid_t			*obj_uuid,	/* [out] uuid of named obj */
    error_status_t		*st_p
)
{
    rsdb_named_item_t   obj;
    sec_rgy_domain_t    domain;
    rsdb_credentials_t  credentials;

    CLEAR_STATUS(st_p);
    uuid_create_nil(obj_uuid, st_p);


    /* Initial '/' is illegal */
    if(*name == '/') {
        SET_STATUS(st_p, sec_rgy_object_not_found);
        return;
    }

    /* Handle the special case of SEC_POLICY_OBJ_NAME. */
    if(u_strcmp(name, SEC_POLICY_OBJ_NAME) == 0) {
        /* Get the acl_id of the POLICY node.
         * RSDB_POLICY_OBJ_ID identifies the POLICY node 
	 * (which does not possess an internal rsdb_pvt_id_t).
         */
	rgy_obj_info->obj_id = RSDB_POLICY_OBJ_ID;
        rgy_obj_info->acl_id = rsdb_get_rgy_acl_id();
        rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_rgy;
	rgy_obj_info->domain = sec_rgy_domain_last;
	rsdb_policy_get_attr_list_id(rgy_obj_info->obj_id, 
	    &(rgy_obj_info->attr_list_id));
        return;
    }

    /* Handle the special case of SEC_REPLIST_OBJ_NAME. */
    if(u_strcmp(name, SEC_REPLIST_OBJ_NAME) == 0) {
        /* Get the acl_id of the REPLIST node.
         * RSDB_REPLIST_OBJ_ID identifies the REPLIST node
         */
        rgy_obj_info->obj_id = RSDB_REPLIST_OBJ_ID;
        rgy_obj_info->acl_id = rsdb_get_replist_acl_id();
        rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_replist;
	rgy_obj_info->domain = sec_rgy_domain_last;
	rsdb_policy_get_attr_list_id(rgy_obj_info->obj_id, 
	    &(rgy_obj_info->attr_list_id));
        return;
    }

    /* Handle the special case of SEC_ATTR_SCHEMA_OBJ_NAME. */
    if(u_strcmp(name, SEC_ATTR_SCHEMA_OBJ_NAME) == 0) {
        /* Get the acl_id of the ATTR_SCHEMA node.
         * RSDB_ATTR_SCHEMA_OBJ_ID identifies the ATTR_SCHEMA node
         */
        rgy_obj_info->obj_id = RSDB_ATTR_SCHEMA_OBJ_ID;
        rgy_obj_info->acl_id = rsdb_get_attr_schema_acl_id();
        rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_attr_schema;
	rgy_obj_info->domain = sec_rgy_domain_last;
	rsdb_policy_get_attr_list_id(rgy_obj_info->obj_id, 
	    &(rgy_obj_info->attr_list_id));
        return;
    }

    /* Handle case where name identifies PRINCIPAL, GROUP, ORG or DIR node */
    rsdb_util_get_domain_obj_by_name(name, &domain, &obj, st_p);
    if(BAD_STATUS(st_p)) {
        SET_STATUS(st_p, sec_rgy_object_not_found);
        return;
    }
    rgy_obj_info->domain	= domain;
    rgy_obj_info->obj_id	= obj.body.threads.id;

    /* Select from polymorphs depending on input type requested */
    if (!RSDB_IS_DIR(&obj) ||
	(RSDB_IS_PGO(&obj) && (obj_type == rsdb_pgo_type)))
    {
        if(!rsdb_get_credentials_by_unix_id(domain_info[domain].db,
                                 RSDB_PGO(&obj).unix_id, &credentials)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_PGO_CREDS_MSG);
        }
	rgy_obj_info->obj_type = rsdb_pgo_type;
        rgy_obj_info->acl_id = credentials.acl_id;
	rgy_obj_info->attr_list_id = RSDB_PGO(&obj).attr_list_id;
        *obj_uuid = credentials.uid;
	switch(rgy_obj_info->domain) {
	case sec_rgy_domain_person:
            rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_person;
	    break;
	case sec_rgy_domain_group:
            rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_group;
	    break;
	case sec_rgy_domain_org:
            rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_org;
	    break;
	}
    } else {
	rgy_obj_info->obj_type = rsdb_dir_type;
        rgy_obj_info->acl_id = GET_DIR_ACL_ID(&obj, acl_type);
        rgy_obj_info->acl_mgr_type = rsdb_acl_mgr_type_dir;
	rgy_obj_info->attr_list_id = RSDB_DIR(&obj).attr_list_id;
        *obj_uuid = RSDB_DIR(&obj).uid;
    }
}

                                     
 /*
  * r s d b _ u t i l _ c h a n g e _ n a m e _ k e y 
  *
  * Store the item under a new name key created with parent_id and name. 
  * Replace the records that map ID to name key and (for directories only)
  * UID to name key with records containing the new name key. If the
  * parent directory identified by parent_id does not exist, return
  * sec_rgy_dir_not_found. 
  */
PUBLIC void rsdb_util_change_name_key
#ifndef __STDC__
    ( domain, item, parent_id, name, name_len, status )
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *item;          
    rsdb_pvt_id_t       parent_id;
    sec_rgy_name_t      name;
    long                name_len;
    error_status_t      *status;  
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *item,          
    rsdb_pvt_id_t       parent_id,
    sec_rgy_name_t      name,
    long                name_len,
    error_status_t      *status
  )
#endif
{
    long            item_len;
    rsdb_name_key_t name_key;
    long            name_key_len;
    rsdb_name_key_t old_item_key;
    long            old_item_key_len = sizeof(old_item_key);
    rsdb_named_item_t parent;

    CLEAR_STATUS(status);
    bzero((Pointer) & name_key, sizeof(name_key));

    if (!rsdb_util_get_item_by_id(domain, parent_id, &parent)) {
        SET_STATUS(status, sec_rgy_dir_not_found);
        return;
    }
    name_key.record_type = NAME_TYPE;
    name_key.parent_id = parent_id;
    name_key.name_len = name_len;
    strncpy((char *) name_key.name, (char *) name, name_len);
    name_key_len = NAME_LEN(&name_key);

    item_len = NAMED_ITEM_LEN(item);
    rsdb_store(domain_info[domain].db, (Pointer) & name_key, name_key_len,
               (Pointer) item, item_len, status);

    /* Delete the old name to data mapping */
    rsdb_util_get_key_of_id(domain, item->body.threads.id,
                            (Pointer) & old_item_key, &old_item_key_len);

    rsdb_delete(domain_info[domain].db, (Pointer) & old_item_key,
                old_item_key_len, status);

    /* Reset the id, uid to key mapping */
    set_key_of_id(domain, item->body.threads.id,
                  (Pointer) & name_key, name_key_len);

    if (RSDB_IS_DIR(item)) {
        set_key_of_dir_uid(domain, RSDB_DIR(item).uid,
                           (Pointer) & name_key, name_key_len);
    }
}                               /* End rsdb_util_change_name_key. */


                                                                   

 /*
  * r s d b _ u t i l _ s t o r e _ n e w _ n a m e d _ i t e m 
  *
  * This routine creates a name_key, completes the information in the
  * named_item, threads the new item  on the sequential list, and stores
  * it in the domain database. 
  */
PUBLIC void rsdb_util_store_new_named_item
#ifndef __STDC__
    (domain, parent_id, name, namelen, item)
    sec_rgy_domain_t    domain;         /* [in]                     */     
    rsdb_pvt_id_t       parent_id;      /* [in]                     */          
    sec_rgy_name_t      name;           /* [in]                     */     
    long                namelen;        /* [in]                     */     
    rsdb_named_item_t   *item;          /* [in, out]                */     
#else
  (
    sec_rgy_domain_t    domain,         /* [in]                     */     
    rsdb_pvt_id_t       parent_id,      /* [in]                     */          
    sec_rgy_name_t      name,           /* [in]                     */     
    long                namelen,        /* [in]                     */     
    rsdb_named_item_t   *item
  )
#endif
{
    rsdb_name_key_t name_key;
    long            name_key_len;
    long            item_len;
    error_status_t  status;

    bzero((Pointer) & name_key, sizeof(name_key));

    name_key.record_type = NAME_TYPE;
    name_key.parent_id = parent_id;
    name_key.name_len = namelen;
    strncpy((char *) name_key.name, (char *) name, namelen);
    name_key_len = NAME_LEN(&name_key);

    /*
     * Create new sequential ID.  Store name_key with it. Return the new
     * ID. 
     */
    item->body.threads.id = rsdb_new_primary_key(domain_info[domain].db,
                                                 (Pointer) & name_key,
                                                 name_key_len);
    /*
     * If item is a directory, store key = Dir UID, data = Name Key. 
     */
    if (RSDB_IS_DIR(item)) {
        rsdb_uid_key_t  uid_key;

        bzero((Pointer) & uid_key, sizeof(uid_key));

        uid_key.record_type = DIRECTORY_UUID_TYPE;
        uid_key.uid = RSDB_DIR(item).uid;
        rsdb_store(domain_info[domain].db, (Pointer) & uid_key,
           sizeof(uid_key), (Pointer) & name_key, name_key_len, &status);
        if (BAD_STATUS(&status)) {
            dce_svc_printf(SEC_RSDB_CANT_STORE_NEW_ITEM_MSG);
        }
    }
    /*
     * Thread the new item on the sequential list 
     */
    item->body.threads.next = domain_info[domain].last.body.threads.id;
    item->body.threads.prev = domain_info[domain].last.body.threads.prev;

    item_len = NAMED_ITEM_LEN(item);

    rsdb_store(domain_info[domain].db, (Pointer) & name_key, name_key_len,
               (Pointer) item, item_len, &status);

    if (BAD_STATUS(&status)) {
        dce_svc_printf(SEC_RSDB_CANT_STORE_NEW_ITEM_MSG);
    }
    /*
     * Now update the previous item and the last item 
     */
    rsdb_util_reset_next_item(domain,
                              domain_info[domain].last.body.threads.prev,
                              item->body.threads.id);
    rsdb_util_reset_previous_item(domain,
                                domain_info[domain].last.body.threads.id,
                                  item->body.threads.id);

}                               /* End rsdb_util_store_new_named_item. */

            
/*
 * r s d b _ u t i l _ r e s e t _ n e x t _ i t e m 
 */    
PUBLIC void rsdb_util_reset_next_item
#ifndef __STDC__
    (domain, item_id, new_next)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      item_id;
    rsdb_pvt_id_t      new_next;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      item_id,
    rsdb_pvt_id_t      new_next
  )
#endif
{
    rsdb_named_item_t  item;

    if (item_id == domain_info[domain].first.body.threads.id) {
        domain_info[domain].first.body.threads.next = new_next;
    }

    rsdb_util_get_item_by_id(domain, item_id, &item);
    item.body.threads.next = new_next;
    rsdb_util_store_item_by_id(domain, item_id, &item);
}
        
/*
 * r s d b _ u t i l _ r e s e t _ p r e v i o u s _ i t e m
 */
PUBLIC void rsdb_util_reset_previous_item
#ifndef __STDC__
    (domain, item_id, new_prev)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      item_id;
    rsdb_pvt_id_t      new_prev;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      item_id,
    rsdb_pvt_id_t      new_prev
  )
#endif
{
    rsdb_named_item_t  item;

    if (item_id == domain_info[domain].last.body.threads.id) {
        domain_info[domain].last.body.threads.prev = new_prev;
    }

    rsdb_util_get_item_by_id(domain, item_id, &item);
    item.body.threads.prev = new_prev;
    rsdb_util_store_item_by_id(domain, item_id, &item);
}
          
        
/* 
 * r s d b _ u t i l _ s t o r e _ i t e m _ b y _ i d   
 */                          
PUBLIC void rsdb_util_store_item_by_id
#ifndef __STDC__
    (domain, id, item)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      id;
    rsdb_named_item_t  *item;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *item
  )
#endif
{
    rsdb_name_key_t    name_key;
    long               key_len = sizeof(name_key);
    long               item_len;
    error_status_t     st;

    rsdb_util_get_key_of_id(domain, id, (Pointer) &name_key, &key_len);
    item_len = NAMED_ITEM_LEN(item);
    rsdb_replace(domain_info[domain].db, (Pointer) &name_key, key_len,
		 (Pointer) item, item_len, &st); 

}   /* End of rsdb_util_store_item_by_id. */

        
/*
 * r s d b _ u t i l _ g e t _ i t e m _ b y _ i d
 */
PUBLIC boolean32 rsdb_util_get_item_by_id
#ifndef __STDC__
    (domain, id, item)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      id;
    rsdb_named_item_t  *item;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *item
  )
#endif
{
    rsdb_name_key_t    name_key;
    long               key_len = sizeof(name_key);

    rsdb_util_get_key_of_id(domain, id, (Pointer) &name_key, &key_len);
    if (!rsdb_util_get_item(domain,  (Pointer) &name_key, key_len, item)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_SEQID_MSG);
    }
    return true;
}

/*
 * rsdb_util_get_item_by_marker
 */ 
PUBLIC boolean32 rsdb_util_get_item_by_marker
#ifndef __STDC__
    ( domain, id, item ) 
    sec_rgy_domain_t    domain;
    rsdb_pvt_id_t       id;
    rsdb_named_item_t   *item;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    rsdb_named_item_t   *item
  )
#endif
{
    rsdb_name_key_t     name_key;
    long                key_len = sizeof(name_key);
    boolean32           retcode = false;

    if (get_key_of_marker(domain, id, (Pointer) &name_key, &key_len)) {
        if (!rsdb_util_get_item(domain, (Pointer) &name_key, key_len, item)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_SEQID_MSG);
        } else {
            retcode = true;
        }
    }
    return retcode;
}


  
 /*
  * rsdb_util_get_item 
  *
  * Use the input key to retrieve the associated item (pgo/dir) in the given
  * domain. 
  */
PUBLIC boolean32 rsdb_util_get_item
#ifndef __STDC__
    (domain, key, key_len, item)
    sec_rgy_domain_t   domain;          /* in       */
    Pointer            key;             /* in       */
    long               key_len;         /* in       */
    rsdb_named_item_t  *item;           /* out      */
#else
  (
    sec_rgy_domain_t   domain,          /* in       */
    Pointer            key,             /* in       */
    long               key_len,         /* in       */
    rsdb_named_item_t  *item
  )
#endif
{
    Pointer             data;
    int                 data_len;
    error_status_t      st;

    rsdb_fetch(domain_info[domain].db, key, key_len, &data, &data_len, &st);
    if (BAD_STATUS(&st)) {
        return false;
    } else {
        if (data_len > sizeof(rsdb_named_item_t)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_buf_too_small);
        } else {
            bcopy(data, (Pointer) item, data_len);
        }
    }
    return true;
}


/*
 * r s d b _ u t i l _ g e t _ k e y _ o f _ i d 
 */
PUBLIC void rsdb_util_get_key_of_id
#ifndef __STDC__
    (domain, id, key, keylen)
    sec_rgy_domain_t    domain;
    rsdb_pvt_id_t       id;
    Pointer             key;
    long                *keylen;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    Pointer             key,
    long                *keylen
  )
#endif
{
    if (!get_key_of_marker(domain, id, key, keylen)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_KEY_MSG);
    }
}
  
/*
 * r s d b _ u t i l _ g e t _ n e x t _ b y _ i d
 */
PUBLIC void rsdb_util_get_next_by_id
#ifndef __STDC__
    ( domain, cursor, item, id, status )
    sec_rgy_domain_t    domain;     /* [in] */
    sec_rgy_cursor_t    *(cursor);  /* [in, out] */
    rsdb_named_item_t   *(item);    /* [out] */
    rsdb_pvt_id_t       *id;        /* [out] */
    error_status_t      *(status);  /* [out] */
#else
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    rsdb_named_item_t   *(item),    /* [out] */
    rsdb_pvt_id_t       *id,        /* [out] */
    error_status_t      *(status)
  )
#endif
{
    CLEAR_STATUS(status);

    if (!cursor->valid) {
        cursor->handle = domain_info[domain].first.body.threads.next;
    }
    if (!rsdb_util_get_item_by_marker(domain, cursor->handle, item)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    if (item->body.threads.id == domain_info[domain].last.body.threads.id) {
        SET_STATUS(status, sec_rgy_no_more_entries);
    } else {
        *id = item->body.threads.id;
        cursor->valid = true;
        cursor->handle = item->body.threads.next;
    }
}                               /* End rsdb_util_get_next_id. */
                            

 /*
  * r s d b _ u t i l _ g e t _ n e x t _ b y _ n a m e _ k e y 
  *
  * Use domain, parent_id and item name to retrieve the item stored with the
  * next (lexicographically greater) name key.  Because the name key
  * contains the parent directory ID before the item name, this operation
  * can be used to retrieve all items in the parent directory as long as
  * the caller checks for matching parent ids.  
  *
  * NOTE: The input parent_id and name do not have to comprise a name_key
  * stored in the database.  They are simply used as the lexicographical
  * starting point for the "next" search.   The name_key returned in 'next'
  * corresponds to the 'item' returned.
  */
PUBLIC void rsdb_util_get_next_by_name_key
#ifndef __STDC__
    (domain, parent_id, name, name_len, next, item, status)
    sec_rgy_domain_t    domain;         /* [in]     */
    rsdb_pvt_id_t       parent_id;      /* [in]     */
    sec_rgy_name_t      name;           /* [in]     */
    long                name_len;       /* [in]     */
    rsdb_name_key_t     *next;          /* [out]    */
    rsdb_named_item_t   *item;          /* [out]    */  
    error_status_t      *status;        /* [out]    */  
#else
  (
    sec_rgy_domain_t    domain,         /* [in]     */
    rsdb_pvt_id_t       parent_id,      /* [in]     */
    sec_rgy_name_t      name,           /* [in]     */
    long                name_len,       /* [in]     */
    rsdb_name_key_t     *next,          /* [out]    */
    rsdb_named_item_t   *item,          /* [out]    */  
    error_status_t      *status
  )
#endif
{
    rsdb_name_key_t name_key;
    int             name_key_len;
    char           *next_key;
    int             next_key_len;
    Pointer         data;
    int             data_len;

    CLEAR_STATUS(status);
    bzero((Pointer) &name_key, sizeof(name_key));

    name_key.record_type = NAME_TYPE;
    name_key.parent_id = parent_id;
    name_key.name_len = name_len;
    strncpy((char *) name_key.name, (char *) name, name_len);
    name_key_len = NAME_LEN(&name_key);

    rsdb_fetch_next(domain_info[domain].db, (Pointer) & name_key, name_key_len,
                    &next_key, &next_key_len,
                    &data, &data_len, status);
    if (BAD_STATUS(status)) {
        return;
    } else {
        if (data_len > sizeof(rsdb_named_item_t)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_buf_too_small);
        } else {
            bcopy(next_key, (Pointer) next, next_key_len);
            if(next->record_type != NAME_TYPE) {
                SET_STATUS(status, sec_rgy_no_more_entries);
                return;
            }
            bcopy(data, (Pointer) item, data_len);
        }
    }
}                               /* End rsdb_util_get_next_by_name_key. */
                          

/*
 * r s d b _ u t i l _ g e t _ u n i x _ n u m _ o f _ i d
 */
PUBLIC void rsdb_util_get_unix_num_of_id
#ifndef __STDC__
    ( domain, id, unix_num ) 
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      *id;
    long               *unix_num;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      *id,
    long               *unix_num
  )
#endif
{
    rsdb_named_item_t  pgo_item;
    sec_rgy_unix_sid_t unix_sid;
    rsdb_pgo_item_t    *pgo_ptrs[sec_rgy_domain_last];
    sec_rgy_domain_t   i;

    if (rsdb_pgo_util_get_by_id(domain, *id, &pgo_item)) {
        for (i = 0; i < sec_rgy_domain_last; i++) {
            pgo_ptrs[i] = NULL;
        }
        pgo_ptrs[domain] = &RSDB_PGO(&pgo_item);

        rsdb_util_get_sid(pgo_ptrs[sec_rgy_domain_person],
                               pgo_ptrs[sec_rgy_domain_group],
                               pgo_ptrs[sec_rgy_domain_org],
                               &unix_sid, NULL);
        if (domain == sec_rgy_domain_person) {
            *unix_num = unix_sid.person;
        } else if (domain == sec_rgy_domain_group) {
            *unix_num = unix_sid.group;
        } else {
            *unix_num = unix_sid.org;
        }
    } else {
        *id         = 0;
        *unix_num   = 0;
    }
}   /* End rsdb_util_get_unix_num_of_id. */


/*
 * r s d b _ u t i l _ g e t _ s i d
 */
PUBLIC void rsdb_util_get_sid
#ifndef __STDC__
    (person, group, org, unix_sid, sid) 
    rsdb_pgo_item_t    *person;
    rsdb_pgo_item_t    *group;
    rsdb_pgo_item_t    *org;
    sec_rgy_unix_sid_t *unix_sid;
    sec_rgy_sid_t      *sid;
#else
  (
    rsdb_pgo_item_t    *person,
    rsdb_pgo_item_t    *group,
    rsdb_pgo_item_t    *org,
    sec_rgy_unix_sid_t *unix_sid,
    sec_rgy_sid_t      *sid
  )
#endif
{
    rsdb_credentials_t credentials;
    sec_rgy_sid_t       dummy_sid;
    sec_rgy_unix_sid_t  dummy_unix_sid;

    if (!unix_sid) {
        unix_sid = &dummy_unix_sid;
    }

    if (!sid) {
        sid = &dummy_sid;
    }

    if (person != NULL) {
        if (!rsdb_get_credentials_by_unix_id(domain_info[sec_rgy_domain_person].db, 
                                            person->unix_id, &credentials)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_PERSON_CREDS_MSG);
        } else {
            unix_sid->person    = credentials.unix_id;
            sid->person         = credentials.uid;
        }
    } else {
        unix_sid->person = -1;
    }

    if (group != NULL) {
        if (!rsdb_get_credentials_by_unix_id(domain_info[sec_rgy_domain_group].db, 
                                            group->unix_id, &credentials)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_GROUP_CREDS_MSG);
        } else {
            unix_sid->group = credentials.unix_id;
            sid->group      = credentials.uid;
        }
    } else {
        unix_sid->group = -1;
    }

    if (org != NULL) {
        if (!rsdb_get_credentials_by_unix_id(domain_info[sec_rgy_domain_org].db, 
                                            org->unix_id, &credentials)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_ORG_CREDS_MSG);
         } else {
            unix_sid->org   = credentials.unix_id;
            sid->org        = credentials.uid;
         }
    } else {
        unix_sid->org = -1;
    }
}   /* End rsdb_util_get_sid */


/*
 * r s d b _ u t i l _ g e t _ m e m b e r _ l i s t
 */
PUBLIC void rsdb_util_get_member_list
#ifndef __STDC__
    (domain, list_id, list)
    sec_rgy_domain_t            domain;
    rsdb_pvt_id_t               list_id;
    rsdb_max_membership_list_t  *list;
#else
  (
    sec_rgy_domain_t            domain,
    rsdb_pvt_id_t               list_id,
    rsdb_max_membership_list_t  *list
  )
#endif
{
    char                        *data;
    int                         data_len;
    rsdb_membership_list_key_t  mem_key;
    error_status_t              st;

    if (list_id == END_OF_LIST) {
        list->total_count = 0;
        list->count = 0;
        list->next = END_OF_LIST;
        return;
    }

    bzero((Pointer) &mem_key, sizeof(mem_key));

    mem_key.id          = list_id;
    mem_key.record_type = MEMBER_LIST_TYPE;

    rsdb_fetch(domain_info[domain].db, (Pointer) &mem_key,
	       sizeof(mem_key), &data, &data_len, &st);
    if (BAD_STATUS(&st)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_MEMBER_DATA_MSG);
    }
    if (data_len > sizeof(rsdb_max_membership_list_t)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_buf_too_small);
    }
    bcopy(data, (Pointer) list, data_len);

}   /* End rsdb_util_get_member_list. */

/*
 *  r s d b _ u t i l _ s t o r e _ m e m b e r _ l i s t
 */
PUBLIC void rsdb_util_store_member_list
#ifndef __STDC__
    (domain, list_id, list)
    sec_rgy_domain_t            domain;
    rsdb_pvt_id_t               list_id;
    rsdb_max_membership_list_t  *list;
#else
  (
    sec_rgy_domain_t            domain,
    rsdb_pvt_id_t               list_id,
    rsdb_max_membership_list_t  *list
  )
#endif
{
    long            	data_len;
    rsdb_membership_list_key_t 	mem_key;
    error_status_t  	st;

    bzero((Pointer) & mem_key, sizeof(mem_key));

    mem_key.id = list_id;
    mem_key.record_type = MEMBER_LIST_TYPE;

    /*
     * Size needs to subtract the extra member that is already accounted
     * for by the rsdb_membership_list_t structure. 
     */
    data_len = sizeof(rsdb_membership_list_t) +
        (sizeof(list->member[0]) * (list->total_count - 1));

    rsdb_replace(domain_info[domain].db, (Pointer) & mem_key, sizeof(mem_key),
                 (Pointer) list, data_len, &st);

}                               /* End rsdb_util_store_member_list. */
        
/*
 *  r s d b _ u t i l _ i s _ m e m b e r
 */
PUBLIC boolean32 rsdb_util_is_member
#ifndef __STDC__
    (domain, list_id, member, free_list_id)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      list_id;
    rsdb_pvt_id_t      member;
    rsdb_pvt_id_t      *free_list_id;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      list_id,
    rsdb_pvt_id_t      member,
    rsdb_pvt_id_t      *free_list_id
  )
#endif
{
    long                        i;
    rsdb_max_membership_list_t list;
    rsdb_pvt_id_t              space_id = END_OF_LIST;

    if (list_id == END_OF_LIST) {
        if (free_list_id)
            *free_list_id = list_id;
        return false;
    }

    while (list_id != END_OF_LIST) {
        rsdb_util_get_member_list(domain, list_id, &list);
        for (i = 0; i < list.total_count; i++) {
            if (!list.member[i].deleted && list.member[i].id == member) {
                return true;
            }
        }
        if (list.count < MAX_MEMBER_LIST_SIZE && space_id == END_OF_LIST) {
            space_id = list_id;
        }
        if (list.next == END_OF_LIST)
            break;
        list_id = list.next;
    }
    if (free_list_id) {
        *free_list_id = (space_id != END_OF_LIST) ? space_id : list_id;
    }
    return false;        

}   /* End rsdb_util_is_member. */


                          
/*
 * r s d b _ u t i l _ m e m b e r _ h a s _ a c c o u n t
 */
PUBLIC void rsdb_util_member_has_account
#ifndef __STDC__
    (domain, target, object, state) 
    sec_rgy_domain_t   domain;
    rsdb_pgo_item_t    *target;
    rsdb_pgo_item_t    *object;
    boolean32          state;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pgo_item_t    *target,
    rsdb_pgo_item_t    *object,
    boolean32          state
  )
#endif
{
    long                       i;
    rsdb_pvt_id_t              id;
    rsdb_pvt_id_t              list_id;
    boolean32                  found = false;
    rsdb_max_membership_list_t list;

    id = object->threads.id;

    for (list_id = target->membership; list_id != END_OF_LIST; 
                                       list_id = list.next) {
        rsdb_util_get_member_list(domain, list_id, &list);
        for (i = 0; i < list.total_count; i++) {
            if (list.member[i].deleted == 1)
                continue;

            if (list.member[i].id == id) {
                list.member[i].has_acct = state ? 1 : 0;
                rsdb_util_store_member_list(domain, list_id, &list);
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
}   /* End rsdb_util_member_has_account. */


PUBLIC void store_acct_by_id
#ifndef __STDC__
    (id, acct)
    rsdb_pvt_id_t      id;
    rsdb_acct_item_t   *acct;
#else
  (
    rsdb_pvt_id_t      id,
    rsdb_acct_item_t   *acct
  )
#endif
{
    rsdb_sequential_key_t  sequential_key;
    int                    acct_size;
    error_status_t         st;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;
    acct_size = ACCT_SIZE(acct);
    rsdb_replace(rsdb_acct_db, (Pointer) &sequential_key,
                 sizeof(sequential_key), (Pointer) acct, acct_size, &st);
}


/*
 * account_exists:
 *
 *              p       p.g     p.g.o
 *      p       E       =p.g.o  =p.g.o
 *      p.g     =p.g.o  E       =p.g.o
 *      p.g.o   =p.g.o  =p.g.o  E
 *
 */

PUBLIC boolean32 account_exists
#ifndef __STDC__
    (person, group, org, key_parts, last_account)
    rsdb_named_item_t  *person;
    rsdb_named_item_t  *group;
    rsdb_named_item_t  *org;
    sec_rgy_acct_key_t key_parts;
    rsdb_pvt_id_t      *last_account;
#else
  (
    rsdb_named_item_t  *person,
    rsdb_named_item_t  *group,
    rsdb_named_item_t  *org,
    sec_rgy_acct_key_t key_parts,
    rsdb_pvt_id_t      *last_account
  )
#endif
{
    rsdb_acct_item_t   account;

    *last_account = RSDB_PGO(person).info;
    if (RSDB_PGO(person).info == END_OF_LIST) {
        return false;
    }

    while (rsdb_get_acct_by_id(*last_account, &account)) {
        if (account.key_parts == key_parts) {
            if (account.person_id == person->body.threads.id) {
                if (key_parts == sec_rgy_acct_key_person) {
                    return true;
                }
                if (account.group_id == group->body.threads.id) {
                    if (key_parts == sec_rgy_acct_key_group) {
                        return true;
                    }
                    if (account.org_id == org->body.threads.id) {
                        return true;
                    }
                }
            }
        } else {
            if (account.person_id == person->body.threads.id
                && account.group_id == group->body.threads.id
                && account.org_id == org->body.threads.id) {
                return true;
            }
        }
        if (account.threads.alias == END_OF_LIST) {
            break;
        } else {
            *last_account = account.threads.alias;
        }
    }

    return false;
}

PUBLIC boolean32 find_account
#ifndef __STDC__
    ( exact, cursor, person, group, org, key_parts, account )
    boolean32          exact;
    sec_rgy_cursor_t   *cursor;
    rsdb_pgo_item_t    *person;
    rsdb_pgo_item_t    *group;
    rsdb_pgo_item_t    *org;
    sec_rgy_acct_key_t key_parts;
    rsdb_acct_item_t   *account;
#else
  (
    boolean32          exact,
    sec_rgy_cursor_t   *cursor,
    rsdb_pgo_item_t    *person,
    rsdb_pgo_item_t    *group,
    rsdb_pgo_item_t    *org,
    sec_rgy_acct_key_t key_parts,
    rsdb_acct_item_t   *account
  )
#endif
{
    rsdb_pvt_id_t      account_id;
    boolean32          retval = false;

    if (cursor->valid) {
        if (rsdb_get_acct_by_id(cursor->handle, account)) {
            if (account->person_id != person->threads.id) {
                return false;
            } else {
                account_id = cursor->handle;
            }
        } else {
            return false;
        }
    } else {
        account_id = person->info;
    }

    if (account_id == END_OF_LIST) {
        return false;
    }

    while (rsdb_get_acct_by_id(account_id, account)) {
        if (account->key_parts <= key_parts) {
            if (account->person_id == person->threads.id) {
                if (key_parts == sec_rgy_acct_key_person) {
                    retval = true;
                    break;
                }
                if (account->group_id == group->threads.id) {
                    if (key_parts == sec_rgy_acct_key_group) {
                        retval = true;
                        break;
                    }
                    if (account->org_id == org->threads.id) {
                        retval = true;
                        break;
                    }
                }
            }
        } 
        if (!exact) {
            if (account->person_id == person->threads.id) {
                if (key_parts < sec_rgy_acct_key_group) {
                    retval = true;
                    break;
                } else if (account->group_id == group->threads.id) {
                    if (key_parts < sec_rgy_acct_key_org) {
                        retval = true;
                        break;
                    } else if (account->org_id == org->threads.id) {
                        retval = true;
                        break;
                    }
                } else if (group->threads.id == END_OF_LIST) {
                    if (account->org_id == org->threads.id) {
                        retval = true;
                        break;
                    }
                }
            }
        }
        if (account->threads.alias == END_OF_LIST) {
            break;
        } else {
            account_id = account->threads.alias;
        }
    }

    if (retval) {
        cursor->valid = true;
        cursor->handle = account->threads.alias;
    } else {
        cursor->valid = false;
    }

    return retval;
}

PUBLIC boolean32 find_login_account
#ifndef __STDC__
    ( cursor, person, group, org, key_parts, account )
    sec_rgy_cursor_t   *cursor;
    rsdb_pgo_item_t    *person;
    rsdb_pgo_item_t    *group;
    rsdb_pgo_item_t    *org;
    sec_rgy_acct_key_t key_parts;
    rsdb_acct_item_t   *account;
#else
  (
    sec_rgy_cursor_t   *cursor,
    rsdb_pgo_item_t    *person,
    rsdb_pgo_item_t    *group,
    rsdb_pgo_item_t    *org,
    sec_rgy_acct_key_t key_parts,
    rsdb_acct_item_t   *account
  )
#endif
{
    rsdb_pvt_id_t      account_id;
    boolean32          possible = false;
    rsdb_acct_item_t   possible_acct;
    boolean32          retval = false;

    if (cursor->valid) {
        if (rsdb_get_acct_by_id(cursor->handle, account)) {
            if (account->person_id != person->threads.id) {
                return false;
            } else {
                account_id = cursor->handle;
            }
        } else {
            return false;
        }
    } else {
        account_id = person->info;
    }

    if (account_id == END_OF_LIST) {
        return false;
    }

    while (rsdb_get_acct_by_id(account_id, account)) {
        if (account->key_parts == key_parts) {
            if ((account->person_id == person->threads.id 
                        || person->threads.id == END_OF_LIST)
                    && (account->group_id == group->threads.id
                        || group->threads.id == END_OF_LIST)
                    && (account->org_id == org->threads.id
                        || org->threads.id == END_OF_LIST)) {
                possible = false;
                retval = true;
                break;
            }
        } else if (account->key_parts < key_parts) {
            if ((account->person_id == person->threads.id 
                        || person->threads.id == END_OF_LIST)
                    && (account->group_id == group->threads.id
                        || group->threads.id == END_OF_LIST)
                    && (account->org_id == org->threads.id
                        || org->threads.id == END_OF_LIST)) {
                if (!possible) {
                    possible = true;
                    possible_acct = *account;
                }
            }
        }

        if (!possible) {
            if (account->person_id == person->threads.id) {
                if (key_parts < sec_rgy_acct_key_group) {
                    if (!possible) {
                        possible = true;
                        possible_acct = *account;
                    }
                } else if (account->group_id == group->threads.id) {
                    if (key_parts < sec_rgy_acct_key_org) {
                        if (!possible) {
                            possible = true;
                            possible_acct = *account;
                        }
                    } else if (account->org_id == org->threads.id) {
                        if (!possible) {
                            possible = true;
                            possible_acct = *account;
                        }
                    }
                } else if (group->threads.id == END_OF_LIST) {
                    if (account->org_id == org->threads.id) {
                        if (!possible) {
                            possible = true;
                            possible_acct = *account;
                        }
                    }
                }
            }
        }
        if (account->threads.alias == END_OF_LIST) {
            break;
        } else {
            account_id = account->threads.alias;
        }
    }

    if (possible) {
        memcpy(account, &possible_acct, ACCT_SIZE(&possible_acct));
        retval = true;
    }

    if (retval) {
        cursor->valid = true;
        cursor->handle = account->threads.alias;
    } else {
        cursor->valid = false;
    }

    return retval;
}


PUBLIC void rsdb_print_key
#ifndef __STDC__
    ( key, trailer ) 
    char    *key;
    char    *trailer;
#else
  (
    char    *key,
    char    *trailer
  )
#endif
{
    union   {
        char                        *any;
        rsdb_sequential_key_t      *seq;
        rsdb_membership_list_key_t *mem;
        rsdb_policy_key_t          *pol;
        rsdb_name_key_t            *nam;
        rsdb_uid_key_t             *uid;
        rsdb_unix_id_key_t         *unid;
    } k;

    if (trailer == NULL)
        trailer = "\n";

    k.any = key;
    switch (k.seq->record_type) {
        case SEQUENTIAL_TYPE:
            printf("'S %d'%s", k.seq->id, trailer);
            break;

        case DUMMY_TYPE: 
            printf("'D %*s'%s", k.nam->name_len, k.nam->name, trailer);
            break;

        case NAME_TYPE: 
            printf("'N %d %*s'%s", k.nam->parent_id, k.nam->name_len,
                     k.nam->name, trailer);
            break;


        case UUID_TYPE:
	    {
		unsigned_char_p_t  uuid_p;
		error_status_t     lst;
		uuid_to_string(&(k.uid->uid), &uuid_p, &lst);
		printf("'U %s'%s", uuid_p, trailer);
		if (uuid_p != NULL) {
		    rpc_string_free(&uuid_p, &lst);
		}
	    }
            break;

        case UNIX_ID_TYPE:
            printf("'# %d'%s", k.unid->unix_id, trailer);
            break;

        case MEMBER_LIST_TYPE:
            printf("'M %d'%s", k.mem->id, trailer);
            break;

        case POLICY_TYPE:
            printf("'P %d'%s", k.pol->id, trailer);
            break;

        default:
            printf("**unknown**%s", trailer);
            break;
    }
}

PUBLIC void purge_member_list
#ifndef __STDC__
    (domain, list_id)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t  list_id;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t  list_id
  )
#endif
{
    rsdb_membership_list_key_t 	mem_key;
    error_status_t              st;

    bzero((Pointer) &mem_key, sizeof(mem_key));

    mem_key.id          = list_id;
    mem_key.record_type = MEMBER_LIST_TYPE;

    rsdb_delete(domain_info[domain].db, (Pointer) &mem_key, sizeof(mem_key), &st);
} 


