/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_pgo_util.h,v $
 * Revision 1.1.6.1  1996/05/10  13:17:37  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/02  19:45 UTC  hanfei
 * 	multi-cell group work: add additional parameter object_domain
 * 	to rsdb_pgo_util_member_list_add().
 * 	[1996/05/09  20:51:07  arvind]
 *
 * Revision 1.1.2.2  1992/12/29  13:26:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_pgo_util.h V=5 04/29/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - database pgo utility operations
 * 
 */

#ifndef rsdb_pgo_util__included
#define rsdb_pgo_util__included


#include <dce/rgynbase.h>

#include <rsdb_pvt.h>
                    
/*
*   RSDB_PGO_UTIL_GET_BY_NAME: Use input pathname to retrieve the associated 
*   named_item in the given domain.  Check its type flag to make
*   sure it contains a pgo_item.
*/
PUBLIC boolean32 rsdb_pgo_util_get_by_name(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      name,
    long                name_len,
    rsdb_named_item_t   *item
#endif
);
        
/*
*   RSDB_PGO_UTIL_GET_BY_ID: Use given domain and ID to retrieve item.
*   Check item to make sure it is of type PGO.
*/
PUBLIC boolean32 rsdb_pgo_util_get_by_id(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    rsdb_named_item_t   *pgo   
#endif
);

/*
*   RSDB_PGO_UTIL_GET_BY_MARKER: Use given domain and marker to retrieve item.
*   Check item to make sure it is of type PGO.
*/
PUBLIC boolean32 rsdb_pgo_util_get_by_marker(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    rsdb_named_item_t   *pgo   
#endif
);

/*
*   RSDB_PGO_UTIL_GET_PGO_AND_NAME_BY_ID
*/
PUBLIC boolean32 rsdb_pgo_util_get_pgo_and_name_by_id(
#ifdef __STDC__
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *pgo_item,
    sec_rgy_name_t     name,
    long               *name_len
#endif
);

/*
*   RSDB_PGO_UTIL_PURGE_PGO
*/
PUBLIC void rsdb_pgo_util_purge_pgo ( 
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *pgo
#endif
);

/*
*   RSDB_PGO_UTIL_GET_NEXT_ALIAS
*/
PUBLIC boolean32 rsdb_pgo_util_get_next_alias(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *old_pgo,
    rsdb_named_item_t   *new_pgo
#endif
);

/*
*   RSDB_PGO_UTIL_GET_AN_ALIAS
*/
PUBLIC void rsdb_pgo_util_get_an_alias ( 
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_credentials_t  *credentials,
    boolean32           allow_aliases,
    sec_rgy_cursor_t    *(cursor),
    sec_rgy_name_t      name,
    sec_rgy_pgo_item_t  *(item),
    error_status_t      *(status)
#endif
);

/* 
*   RSDB_PGO_UTIL_DELETE_PERSON
*/
PUBLIC void rsdb_pgo_util_delete_person ( 
#ifdef __STDC__
    rsdb_named_item_t   *person
#endif
);
 

/*
*   RSDB_PGO_UTIL_DELETE_MEMBER 
*/
PUBLIC boolean32 rsdb_pgo_util_delete_member(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *aggregate,
    rsdb_named_item_t   *member
#endif
);

/*       
*   RSDB_PGO_UTIL_DELETE_MEMBER_LIST:
*  Used to when deleting a group or organization.  This will
*  delete all the accounts for members of the aggregate and will also delete
*  the membership list object.  (The aggregate object is not updated and will 
*  be inconsistent (have a dangling reference) if it is not deleted after this 
*  operation).
*/
PUBLIC void rsdb_pgo_util_delete_member_list(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *aggregate
#endif
);

/*
*   RSDB_PGO_UTIL_MEMBER_LIST_ADD
*/
PUBLIC void rsdb_pgo_util_member_list_add(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    sec_rgy_domain_t    object_domain,
    rsdb_named_item_t   *target,
    rsdb_named_item_t   *object
#endif
);

                                                    
/*
*   RSDB_PGO_UTIL_DELETE_MEMBER_ACCOUNTS
*/
PUBLIC void rsdb_pgo_util_delete_member_accounts(
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *aggregate,
    rsdb_named_item_t   *member
#endif
);


#endif      /* rsdb_pgo_util__included */
