/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_util.h,v $
 * Revision 1.1.10.1  1996/05/10  13:18:00  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/21  15:40 UTC  hanfei
 * 	take out rsdb_util_get_uuid_by_name header
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  15:48 UTC  hanfei
 * 	mutli-cell group work: add header for rsdb_util_get_uuid_by_name()
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Add cell name sanity check..
 * 	[1995/05/11  00:32 UTC  sommerfeld  /main/sommerfeld_wp_idmap/1]
 *
 * Revision 1.1.4.2  1994/04/22  19:13:55  rps
 * 	Add named_item_type differentiator to _get_obj_info_by_name
 * 	[1994/04/21  00:08:01  rps]
 * 
 * Revision 1.1.4.1  1994/03/02  17:55:08  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:09:00  hanfei]
 * 
 * Revision 1.1.2.2  1992/12/29  13:26:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:13  devrcs
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
 * 
 *      Registry Server - data utilities
 */

#ifndef rsdb_util__included
#define rsdb_util__included

#include <dce/rgynbase.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>

/* TYPEDEFS */
typedef struct rsdb_util_rgy_obj_info_t {
    sec_rgy_domain_t        domain;
    named_item_type_t	    obj_type; /* rsdb_pgo_type or rsdb_dir_type */
    rsdb_pvt_id_t           obj_id;
    rsdb_pvt_id_t           acl_id;
    rsdb_acl_mgr_type_t     acl_mgr_type;
    rsdb_pvt_id_t	    attr_list_id;
} rsdb_util_rgy_obj_info_t;


 /*
  * r s d b _ u t i l _ q u o t a _ d e c r e m e n t 
  *
  * If the realm is local, decrement the principal's quota (in the principal's
  * PGO). If the realm is not local, decrement the quota for the principal
  * associated with the foreign realm.  In either case, if the quota is
  * sec_rgy_pgo_quota_unlimited (-1), do not decrement; return
  * sec_rgy_status_ok.  If the quota is zero, do not decrement; return
  * sec_rgy_quota_exhausted or foreign_quota_exhausted. 
  */
PUBLIC void rsdb_util_quota_decrement(
    uuid_t              *princ_uuid,            /* [in] principal UUID      */
    uuid_t              *realm_uuid,            /* [in] realm of principal  */
    error_status_t      *status                 /* [out]                    */
);

 /*
  * r s d b _ u t i l _ q u o t a _ a v a i l
  *
  * If the realm is local, check the principal's quota (in the principal's
  * PGO). If the realm is not local, check the quota for the principal
  * associated with the foreign realm.  In either case, if the quota is
  * sec_rgy_pgo_quota_unlimited (-1) or greater than 0, return true with
  * sec_rgy_status_ok.  If the quota is zero, return false with 
  * sec_rgy_quota_exhausted or foreign_quota_exhausted. 
  */
PUBLIC boolean32 rsdb_util_quota_avail(
    uuid_t              *princ_uuid,            /* [in] principal UUID      */
    uuid_t              *realm_uuid,            /* [in] realm of principal  */
    error_status_t      *status                 /* [out]                    */
);




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
PUBLIC boolean32 rsdb_util_is_realm_local(
    char                *realm_name,            /* [in] realm name          */
    uuid_t              *realm_uuid,            /* [in] realm uuid          */
    error_status_t      *status                 /* [out]                    */
);

/*
 * r s d b _ u t i l _ c h e c k _ c e l l _ n a m e
 *
 * Sanity check cell name of DB vs cell name of host at server boot time
 */
PUBLIC void rsdb_util_check_cell_name(
    error_status_t      *status                 /* [out]                    */
);

 /*
  * RSDB_UTIL_PATH_RESOLVE: The pathname to resolve is input in the
  * residual parameter.  Within the given domain, the item identified by
  * this name is returned.  In the successful case (item is found), the
  * output residual pathname is empty and the full pathname is returned in
  * the resolved parameter.  In the unsuccessful case (a portion of the
  * pathname could not be found) the pathname components that were found
  * are returned in the resolved parameter and the rest is returned in
  * residual. 
  *
  * Status: error_status_ok, rgy_item_not_found 
  */
void rsdb_util_path_resolve(
    sec_rgy_domain_t    domain,                 /* [in]                     */
    sec_rgy_name_t      residual,               /* [in, out] unprocessed    */ 
    long                *residual_len,          /* [in, out]                */
    sec_rgy_name_t      resolved,               /* [out] resolved part      */ 
    long                *resolved_len,          /* [out]                    */
    rsdb_named_item_t   *item,                  /* [out] item               */
    error_status_t      *status                 /* [out]                    */
);                                                


 /*
  * RSDB_UTIL_PATH_CONSTRUCT: Given item_id and domain, construct the full
  * pathname of the item. If there is no item stored for the given
  * item_id, return rgy_item_not_found.  If the item for the given item_id
  * is found, but the registry database does not contain enough
  * information to construct the entire pathname, the database is
  * inconsistent and a fatal error results. 
  *
  * Status: error_status_ok, rgy_item_not_found, 
  */
void rsdb_util_path_construct(
    sec_rgy_domain_t    domain,                 /* [in]                     */
    rsdb_pvt_id_t       item_id,                /* [in]                     */
    sec_rgy_name_t      name,                   /* [out] directory name     */
    long                *name_len,              /* [out]                    */
    error_status_t      *status                 /* [out]                    */
);                                                


void rsdb_util_build_dirs (
    sec_rgy_domain_t    domain,                 /* [in] */
    sec_rgy_name_t      parent_name,            /* [in] */
    long                pname_len,              /* [in] */
    error_status_t      *status                 /* [out] */
);


void rsdb_util_delete_dirs_if_empty (
    sec_rgy_domain_t    domain,     /* [in] */    
    sec_rgy_name_t      dir_name,   /* [in] */
    error_status_t      *(status)
); 

                       
 /*
  * RSDB_UTIL_GET_BY_NAME: Retrieve the named_item identified by the input
  * pathname. Return TRUE if the named_item was found, FALSE otherwise. 
  */
boolean32 rsdb_util_get_by_name(
    sec_rgy_domain_t    domain,                 /* [in]                     */
    sec_rgy_name_t      name,                   /* [in] name of pgo/dir item*/
    long                name_len,               /* [in]                     */
    rsdb_named_item_t   *item                   /* [out]                    */
);


 /*
  * RSDB_UTIL_GET_BY_NAME_KEY: Retrieve the named_item (dir/pgo union)
  * identified by the input parent_id and name.  Return TRUE if a
  * named_item was found, FALSE otherwise. 
  *
  */
boolean32 rsdb_util_get_by_name_key(
    sec_rgy_domain_t    domain,                 /* [in]                     */
    rsdb_pvt_id_t       parent_id,              /* [in]                     */
    sec_rgy_name_t      name,                   /* [in] name of pgo/dir item*/
    long                namelen,                /* [in]                     */
    rsdb_named_item_t   *item                   /* [out]                    */
);                          
                           
 /*
  * RSDB_UTIL_GET_DOMAIN_BY_NAME
  * Identify and retrieve the top-level domain directory item (either person,
  * group, or org domain) by its name.  Also return the domain.
  * If name does not correspond to any of the three domains, return error.
  *
  * Status codes:
  *     sec_rgy_bad_domain - input name doesn't identify any domain.
  *     error_status_ok.
  */
PUBLIC boolean32 rsdb_util_get_domain_by_name (
    char                *name,                  /* [in] domain name         */
    long                namelen,                /* [in]                     */
    rsdb_named_item_t   *item,                  /* [out] domain dir item    */
    sec_rgy_domain_t    *domain,                /* [out]                    */   
    error_status_t      *st                     /* [out]                    */
);

PUBLIC void rsdb_util_get_domain_obj_by_name (
    unsigned_char_t         *name,
    sec_rgy_domain_t        *domain,
    rsdb_named_item_t       *object,
    error_status_t          *st
);

PUBLIC void rsdb_util_get_obj_info_by_name (
    unsigned_char_t             *name,		/* [in] name of object */
    named_item_type_t		obj_type,	/* [in] pgo vs dir */
    sec_acl_type_t              acl_type,       /* [in] acl_type of interest */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,	/* [in,out] */
    uuid_t                      *obj_uuid,      /* [out] uuid of named obj */
    error_status_t              *st_p
);

 /*
  * RSDB_UTIL_CHANGE_NAME_KEY: Store the item under a new name key created
  * with parent_id and name.  Replace the records that map ID to name key
  * and (for directories only) UID to name key with records containing the
  * new name key. If the parent directory identified by parent_id does not
  * exist, return sec_rgy_dir_not_found. 
  */
void rsdb_util_change_name_key(
    sec_rgy_domain_t        domain,
    rsdb_named_item_t   *item,          
    rsdb_pvt_id_t       parent_id,
    sec_rgy_name_t          name,
    long                name_len,
    error_status_t      *status
);  

                          
 /*
  * RSDB_UTIL_STORE_NEW_NAMED_ITEM:  This routine creates a name_key,
  * completes the information in the named_item, threads the new item  on
  * the sequential list, and stores it in the domain database. A
  * sequential id is created for the item in the process. 
  *
  * This routine is called by rsdb_dir_util_store_new_item and
  * rsdb_pgo_util_store_new_item. 
  */
void rsdb_util_store_new_named_item(
    sec_rgy_domain_t    domain,             /* [in]                     */     
    rsdb_pvt_id_t       parent_id,          /* [in]                     */     
    sec_rgy_name_t      name,               /* [in]                     */     
    long                namelen,            /* [in]                     */     
    rsdb_named_item_t   *item               /* [in, out]                */     
);

/*
 * RSDB_UTIL_RESET_NEXT_ITEM 
 */    
void rsdb_util_reset_next_item(
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      item_id,
    rsdb_pvt_id_t      new_next
);
        
/*
 * RSDB_UTIL_RESET_PREVIOUS_ITEM
 */
void rsdb_util_reset_previous_item(
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      item_id,
    rsdb_pvt_id_t      new_prev
);
        
/* 
 * RSDB_UTIL_STORE_ITEM_BY_ID   
 */                          
void rsdb_util_store_item_by_id(
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *item
);
        
/*
 * RSDB_UTIL_GET_ITEM_BY_ID
 */
boolean32 rsdb_util_get_item_by_id(
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *item
);

/*
 * RSDB_UTIL_GET_ITEM_BY_MARKER
 */ 
boolean32 rsdb_util_get_item_by_marker( 
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *item
);
  
 /*
  * RSDB_UTIL_GET_ITEM: Use the input key to retrieve the associated item
  * (pgo/dir) in the given domain. 
  */
boolean32 rsdb_util_get_item(
    sec_rgy_domain_t    domain,
    Pointer             key,
    long                key_len,
    rsdb_named_item_t   *item
);

/*
 * RSDB_UTIL_GET_KEY_OF_ID 
 */
void rsdb_util_get_key_of_id(
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    Pointer            key,
    long               *keylen
);


/*
 *   RSDB_UTIL_GET_NEXT_BY_ID
 */
void rsdb_util_get_next_by_id(
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    rsdb_named_item_t   *(item),    /* [out] */
    rsdb_pvt_id_t       *id,        /* [out] */
    error_status_t      *(status)   /* [out] */
);


 /*
  * RSDB_UTIL_GET_NEXT_BY_NAME_KEY: Use domain, parent_id and item name to
  * retrieve the item stored with the next (lexicographically greater)
  * name key.  Because the name key contains the parent directory ID
  * before the item name, this operation can be used to retrieve all items
  * in the parent directory.  
  *
  * NOTE: The input parent_id and name do not have to comprise a name_key
  * stored in the database.  They are simply used as the lexicographical
  * starting point for the "next" search. 
  */
void rsdb_util_get_next_by_name_key(
    sec_rgy_domain_t    domain,         /* [in]     */
    rsdb_pvt_id_t       parent_id,      /* [in]     */
    sec_rgy_name_t      name,           /* [in]     */
    long                name_len,       /* [in]     */
    rsdb_name_key_t     *next,          /* [out]    */
    rsdb_named_item_t   *item,          /* [out]    */  
    error_status_t      *status         /* [out]    */  
);
 

/*
 *   RSDB_UTIL_GET_UNIX_NUM_OF_ID
 */
void rsdb_util_get_unix_num_of_id( 
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       *id,
    long                *unix_num
);


/*
 *   RSDB_UTIL_GET_UNIX_SID
 */
void rsdb_util_get_sid (
    rsdb_pgo_item_t     *person,
    rsdb_pgo_item_t     *group,
    rsdb_pgo_item_t     *org,
    sec_rgy_unix_sid_t  *unix_sid,
    sec_rgy_sid_t       *sid
);


/*
 *   RSDB_UTIL_GET_MEMBER_LIST
 */
void rsdb_util_get_member_list(
    sec_rgy_domain_t            domain,
    rsdb_pvt_id_t               list_id,
    rsdb_max_membership_list_t  *list
);


/*
 *   RSDB_UTIL_STORE_MEMBER_LIST
 */
void rsdb_util_store_member_list(
    sec_rgy_domain_t            domain,
    rsdb_pvt_id_t               list_id,
    rsdb_max_membership_list_t  *list
);


/*
 *   RSDB_UTIL_IS_MEMBER
 */
boolean32 rsdb_util_is_member(
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      list_id,
    rsdb_pvt_id_t      member,
    rsdb_pvt_id_t      *free_list_id
);

                           
/*
 *   RSDB_UTIL_MEMBER_HAS_ACCOUNT
 */
void rsdb_util_member_has_account(
    sec_rgy_domain_t   domain,
    rsdb_pgo_item_t    *target,
    rsdb_pgo_item_t    *object,
    boolean32          state
);

void purge_member_list (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      list_id
);      

void store_acct_by_id (
    rsdb_pvt_id_t      id,
    rsdb_acct_item_t   *acct
);
 
boolean32 account_exists (
    rsdb_named_item_t  *person,
    rsdb_named_item_t  *group,
    rsdb_named_item_t  *org,
    sec_rgy_acct_key_t key_parts,
    rsdb_pvt_id_t      *last_account
);

boolean32 find_account (
    boolean32          exact,
    sec_rgy_cursor_t   *cursor,
    rsdb_pgo_item_t    *person,
    rsdb_pgo_item_t    *group,
    rsdb_pgo_item_t    *org,
    sec_rgy_acct_key_t key_parts,
    rsdb_acct_item_t   *account
);
   
boolean32 find_login_account ( 
    sec_rgy_cursor_t   *cursor,
    rsdb_pgo_item_t    *person,
    rsdb_pgo_item_t    *group,
    rsdb_pgo_item_t    *org,
    sec_rgy_acct_key_t key_parts,
    rsdb_acct_item_t   *account
);

void rsdb_print_key (
    char    *key,
    char    *trailer
);                       
   
#endif  /* rsdb_util__included */

