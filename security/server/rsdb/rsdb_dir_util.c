/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_dir_util.c,v $
 * Revision 1.1.10.2  1996/02/18  00:21:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  17:59:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:19  root]
 * 
 * Revision 1.1.8.2  1994/08/04  16:14:47  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:51  mdf]
 * 
 * Revision 1.1.8.1  1994/04/22  19:14:17  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:23:57  rps]
 * 
 * Revision 1.1.6.2  94/03/29  18:44:07  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.5.3  1992/12/29  16:38:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:27  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  20:14:05  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:40:18  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_dir_util.c V=7 06/26/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *      Registry Server - database directory utility operations
 */

#include <stdio.h>
#include <Pointer.h>
#include <bstring.h>
#include <un_strng.h> 

#include <dce/dce.h>
#include <rs_base.h>
#include <rs_util.h>

#include <rsdb_dir_util.h>   
#include <sec_svc.h>

/*
 *   RSDB_DIR_UTIL_GET_BY_NAME: Use input pathname to retrieve the associated 
 *   named_item in the given domain.  Check its type flag to make
 *   sure it contains a dir_item.
 */
PUBLIC boolean32 rsdb_dir_util_get_by_name
#ifndef __STDC__
    ( domain, name, name_len, item )
    sec_rgy_domain_t    domain;
    sec_rgy_name_t      name;
    long                name_len;
    rsdb_named_item_t   *item;
#else
  (
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      name,
    long                name_len,
    rsdb_named_item_t   *item
  )
#endif
{
    if(rsdb_util_get_by_name(domain, name, name_len, item)) {
        if (RSDB_IS_DIR(item))
            return(TRUE);
        else
            return(FALSE);
    }
    return(FALSE);

}   /* End rsdb_dir_util_get_by_name. */

        
/*
 *   RSDB_DIR_UTIL_GET_BY_ID: Use given domain and ID to retrieve dir.
 *   Check item to make sure it is of type DIR.
 */
PUBLIC boolean32 rsdb_dir_util_get_by_id
#ifndef __STDC__
    (domain, id, dir)
    sec_rgy_domain_t    domain;
    rsdb_pvt_id_t       id;
    rsdb_named_item_t   *dir;   
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    rsdb_named_item_t   *dir
  )
#endif
{                                     
    if(!rsdb_util_get_item_by_id(domain, id, dir))
        return(FALSE);
    else
        if (RSDB_IS_DIR(dir))
            return(TRUE);
        else
            return(FALSE);

}   /* End rsdb_dir_util_get_by_id. */


/*
 *   RSDB_DIR_UTIL_GET_KEY_OF_UID: Retrieve the name_key stored with
 *   the input directory uuid.
 */
PUBLIC boolean32 rsdb_dir_util_get_key_of_uid
#ifndef __STDC__
    ( domain, uid, key, keylen )
    sec_rgy_domain_t    domain;
    uuid_t              uid;
    Pointer             key;
    long                *keylen;
#else
  (
    sec_rgy_domain_t    domain,
    uuid_t              uid,
    Pointer             key,
    long                *keylen
  )
#endif
{
    char                *name_key;
    int                 name_key_len;
    rsdb_uid_key_t      uid_key;
    error_status_t      status;

    bzero( (Pointer)&uid_key, sizeof(uid_key));

    uid_key.record_type = DIRECTORY_UUID_TYPE;
    uid_key.uid         = uid;

    rsdb_fetch(domain_info[domain].db, (Pointer)&uid_key,
	       sizeof(uid_key), &name_key, &name_key_len, &status);
                               
    if (BAD_STATUS(&status)) {
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

}   /* End rsdb_dir_util_get_key_of_uid. */

/*
 *   RSDB_DIR_UTIL_GET_BY_UID: Retrieve the directory
 *   identified by the input directory uuid.
 *   Return TRUE if the directory was found, FALSE otherwise.
 *
 */       
PUBLIC boolean32 rsdb_dir_util_get_by_uid
#ifndef __STDC__
    ( domain, uid, dir ) 
    sec_rgy_domain_t    domain;             /* [in]                     */
    uuid_t              uid;                /* [in]                     */
    rsdb_named_item_t   *dir;
#else
  (
    sec_rgy_domain_t    domain,             /* [in]                     */
    uuid_t              uid,                /* [in]                     */
    rsdb_named_item_t   *dir
  )
#endif
{                                             
    rsdb_name_key_t     name_key;  
    long                name_key_len = sizeof(name_key);

    if(!rsdb_dir_util_get_key_of_uid(domain, uid, (Pointer)&name_key,
                                 &name_key_len)) {
        return false;
    }                          
    if(!rsdb_util_get_item(domain, (Pointer)&name_key, 
                           name_key_len, dir)) { 
        return false;
    }                                               
    
    return true;

}   /* End rsdb_dir_util_get_by_uid. */
     
/*
 *   RSDB_DIR_UTIL_PURGE_DIR: Delete all records associated with the input 
 *   directory.
 */
PUBLIC void rsdb_dir_util_purge_dir
#ifndef __STDC__
    ( domain, dir ) 
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *dir;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *dir
  )
#endif
{
    rsdb_name_key_t    name_key;
    long               key_len = sizeof(name_key);
    error_status_t     st;

    rsdb_util_get_key_of_id(domain, dir->body.threads.id,
                            (Pointer)&name_key, &key_len);             
                                          
    /*
    *   Delete three records:
    *       Key                 Data
    *       ---                 ----
    *       name key            dir item	[only if not leaving a pgo]
    *       id                  name key	[only if not leaving a pgo]
    *       uid                 name_key
    */
    if (!RSDB_IS_PGO(dir)) {
	rsdb_delete(domain_info[domain].db, (Pointer) &name_key, key_len, &st);
	rsdb_purge_sequential_key(domain_info[domain].db, dir->body.threads.id);   
    }
    rsdb_dir_util_purge_uid_key(domain_info[domain].db, RSDB_DIR(dir).uid);

}   /* End rsdb_dir_util_purge_dir. */                         


/*
 *   RSDB_DIR_UTIL_PURGE_UID_KEY
 */
PRIVATE void rsdb_dir_util_purge_uid_key
#ifndef __STDC__
    ( db, uid ) 
    db_handle       db;
    uuid_t          uid;
#else
  (
    db_handle       db,
    uuid_t          uid
  )
#endif
{
    rsdb_uid_key_t  uid_key;
    error_status_t  status;

    bzero((Pointer) &uid_key, sizeof(uid_key));
                                        
    uid_key.record_type = DIRECTORY_UUID_TYPE;
    uid_key.uid         = uid;

    rsdb_delete(db, (Pointer)&uid_key, sizeof(uid_key), &status);

}   /* End rsdb_dir_util_purge_uid_key. */


/*
 *   RSDB_DIR_UTIL_IS_EMPTY: If the directory, identified by dir_id, in the 
 *   given domain is empty (contains no child directory or pgo items), 
 *   return TRUE.  Otherwise, return FALSE.
 */
                           
PUBLIC boolean32 rsdb_dir_util_is_empty
#ifndef __STDC__
    (domain, dir_id)
    sec_rgy_domain_t    domain;                     /* [in]                   */
    rsdb_pvt_id_t       dir_id;                     /* [in]                   */
#else
  (
    sec_rgy_domain_t    domain,                     /* [in]                   */
    rsdb_pvt_id_t       dir_id
  )
#endif
{                                                
    sec_rgy_name_t      name;
    long                name_len = 0;
    rsdb_name_key_t     next;
    rsdb_named_item_t   item;  
    error_status_t      status;

    /*
    * Try to get at least one PGO or DIR item in this directory
    * by searching for an item whose name_key contains this
    * dir_id as the parent_id.  Name_len is 0 so this key
    * is lexicographically less than any other key with the
    * same parent_id.
    */               
    rsdb_util_get_next_by_name_key(domain, dir_id, name, name_len,
                                   &next, &item, &status); 
    if(STATUS_OK(&status))   
        if(next.parent_id == dir_id) 
            return false;

    return true;

}   /* End rsdb_dir_util_is_empty. */

                 
