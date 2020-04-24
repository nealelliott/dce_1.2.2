/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_dir_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:28:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:49  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:26:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_dir_util.h V=7 05/24/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - database directory utility operations
 * 
 */

#ifndef rsdb_dir_util_h__included  
#define rsdb_dir_util_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <rsdb_util.h>  


/*
*   RSDB_DIR_UTIL_GET_BY_NAME: Use input pathname to retrieve the associated 
*   named_item in the given domain.  Check its type flag to make
*   sure it contains a dir_item.
*/
PUBLIC boolean32 rsdb_dir_util_get_by_name(
#ifdef __STDC__  
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      name,
    long                name_len,
    rsdb_named_item_t   *item
#endif
);


     

/*
*   RSDB_DIR_UTIL_GET_BY_ID: Use given domain and ID to retrieve dir.
*   Check item to make sure it is of type DIR.
*/
PUBLIC boolean32 rsdb_dir_util_get_by_id(
#ifdef __STDC__  
    sec_rgy_domain_t    domain,                 /* [in]                     */
    rsdb_pvt_id_t       id,                     /* [in]                     */
    rsdb_named_item_t   *dir                    /* [out]                    */
#endif
);   
   

/*
*   RSDB_DIR_UTIL_GET_KEY_OF_UID: Retrieve the name_key stored with
*   the input directory uuid.
*/
PUBLIC boolean32 rsdb_dir_util_get_key_of_uid( 
#ifdef __STDC__  
    sec_rgy_domain_t    domain,
    uuid_t              uid,
    Pointer             key,
    long                *keylen
#endif
);


/*
*   RSDB_DIR_UTIL_GET_BY_UID: Retrieve the directory
*   identified by the input directory uuid.
*   Return TRUE if the directory was found, FALSE otherwise.
*
*/       
PUBLIC boolean32 rsdb_dir_util_get_by_uid(
#ifdef __STDC__  
    sec_rgy_domain_t    domain,             /* [in]                     */
    uuid_t              uid,                /* [in]                     */
    rsdb_named_item_t   *dir                /* [out]                    */
#endif
);


/*
*   RSDB_DIR_UTIL_PURGE_DIR: Delete all records associated with the input 
*   directory.
*/
PUBLIC void rsdb_dir_util_purge_dir( 
#ifdef __STDC__  
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *dir
#endif
);


/*
*   RSDB_DIR_UTIL_PURGE_UID_KEY
*/
PRIVATE void rsdb_dir_util_purge_uid_key( 
#ifdef __STDC__  
    db_handle       db,
    uuid_t          uid
#endif
);



/*
*   RSDB_DIR_UTIL_IS_EMPTY: If the directory, identified by dir_id, in the 
*   given domain is empty (contains no child directory or pgo items), 
*   return TRUE.  Otherwise, return FALSE.
*/
PUBLIC boolean32 rsdb_dir_util_is_empty(
#ifdef __STDC__  
    sec_rgy_domain_t domain,                     /* [in]                     */
    rsdb_pvt_id_t    dir_id                      /* [in]                     */
#endif
);     

#endif /* rsdb_dir_util_h__included */
