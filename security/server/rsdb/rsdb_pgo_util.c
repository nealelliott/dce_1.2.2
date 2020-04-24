/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_pgo_util.c,v $
 * Revision 1.1.10.2  1996/08/09  12:11:43  arvind
 * 	Fix rsdb_pgo_util_delete_member_list() to lookup the foreign principal
 * 	in the correct domain.
 * 	[1996/07/19  19:09 UTC  jrr  /main/DCE_1.2.2/jrr_122_3/1]
 *
 * 	multi-cell group work: modify rsdb_pgo_util_member_list_add() and
 * 	rsdb_pgo_util_delete_member_list().
 * 	[1996/02/02  19:36 UTC  hanfei  /main/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:36  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:28  root]
 *
 * Revision 1.1.10.1  1996/05/10  13:25:29  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  19:06 UTC  hanfei
 * 	fix functions that return no values
 * 	[1996/02/29  22:48 UTC  pdd  /main/HPDCE02/1]
 * 
 * 	provide return values
 * 	[1996/02/29  22:04 UTC  pdd  /main/pdd_branch_CHFts17478/1]
 * 
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/02  19:36 UTC  hanfei
 * 	multi-cell group work: modify rsdb_pgo_util_member_list_add() and
 * 	rsdb_pgo_util_delete_member_list().
 * 	[1996/05/09  20:56:31  arvind]
 * 
 * Revision 1.1.5.2  1994/08/04  16:14:52  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:14:02  mdf]
 * 
 * Revision 1.1.5.1  1994/04/22  19:14:21  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:27:33  rps]
 * 
 * Revision 1.1.3.2  94/03/29  18:45:27  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.2.2  1992/12/29  16:39:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:55  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_pgo_util.c V=8 11/19/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Registry Server - database pgo utility operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_pgo_util.c	8 - 11/19/91";
#endif

#include <stdio.h>
#include <Pointer.h>
#include <bstring.h>
#include <un_strng.h> 

#include <dce/dce.h>
#include <dce/uuid.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rsdb_util.h> 
#include <rsdb_pgo_util.h>
#include <rsdb_pgo.h>
#include <rsdb_acct.h>

#include <sec_svc.h>

 /*
  * r s d b _ p g o _ u t i l _ g e t _ b y _ n a m e 
  *
  * Use input pathname to retrieve the associated named_item in the given
  * domain.  Check its type flag to make sure it contains a pgo_item. 
  */
PUBLIC boolean32 rsdb_pgo_util_get_by_name
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
	if (RSDB_IS_PGO(item))
            return(TRUE);
        else
            return(FALSE);
    }
    return(FALSE);

}   /* End rsdb_pgo_util_get_by_name. */
        
 /*
  * r s d b _ p g o _ u t i l _ g e t _ b y _ i d 
  *
  * Use given domain and ID to retrieve item. Check item to make sure it is
  * of type PGO. 
  */
PUBLIC boolean32 rsdb_pgo_util_get_by_id
#ifndef __STDC__
    (domain, id, pgo)
    sec_rgy_domain_t    domain;
    rsdb_pvt_id_t       id;
    rsdb_named_item_t   *pgo;   
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    rsdb_named_item_t   *pgo
  )
#endif
{                                     
    if(!rsdb_util_get_item_by_id(domain, id, pgo))
        return(FALSE);
    else
	if (RSDB_IS_PGO(pgo))
            return(TRUE);
        else
            return(FALSE);

}   /* End rsdb_pgo_util_get_by_id. */
 

 /*
  * r s d b _ p g o _ u t i l _ g e t _ b y _ m a r k e r 
  *
  * Use given domain and marker to retrieve item. Check item to make sure it
  * is of type PGO. 
  */
PUBLIC boolean32 rsdb_pgo_util_get_by_marker
#ifndef __STDC__
    (domain, id, pgo)
    sec_rgy_domain_t    domain;
    rsdb_pvt_id_t       id;
    rsdb_named_item_t   *pgo;   
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_pvt_id_t       id,
    rsdb_named_item_t   *pgo
  )
#endif
{                                     
    if(!rsdb_util_get_item_by_marker(domain, id, pgo))
        return(FALSE);
    else
	if (RSDB_IS_PGO(pgo))
            return(TRUE);
        else
            return(FALSE);

}   /* End rsdb_pgo_util_get_by_marker. */


/*
 *  r s d b _ p g o _ u t i l _ g e t _ p g o _ a n d _ n a m e _ b y _ i d
 */
PUBLIC boolean32 rsdb_pgo_util_get_pgo_and_name_by_id
#ifndef __STDC__
    (domain, id, pgo_item, name, name_len)
    sec_rgy_domain_t   domain;
    rsdb_pvt_id_t      id;
    rsdb_named_item_t  *pgo_item;
    sec_rgy_name_t     name;
    long               *name_len;
#else
  (
    sec_rgy_domain_t   domain,
    rsdb_pvt_id_t      id,
    rsdb_named_item_t  *pgo_item,
    sec_rgy_name_t     name,
    long               *name_len
  )
#endif
{
    error_status_t     status;                  
                                                          
    if(!rsdb_pgo_util_get_by_id(domain, id, pgo_item))
        return(FALSE);
    rsdb_util_path_construct(domain, id, name, name_len, &status);
        if(BAD_STATUS(&status))
            return(FALSE);

    return(TRUE);

}   /* End rsdb_pgo_util_get_pgo_and_name_by_id. */



/*
 *  r s d b _ p g o _ u t i l _ p u r g e _ p g o
 */
PUBLIC void rsdb_pgo_util_purge_pgo
#ifndef __STDC__
    ( domain, pgo ) 
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *pgo;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *pgo
  )
#endif
{
    rsdb_name_key_t    pgo_key;
    long               key_len = sizeof(pgo_key);
    error_status_t     st;

    rsdb_util_get_key_of_id(domain, pgo->body.threads.id,
                            (Pointer) &pgo_key, &key_len);             

    rsdb_delete(domain_info[domain].db, (Pointer) &pgo_key, key_len, &st);
    rsdb_purge_sequential_key(domain_info[domain].db, pgo->body.threads.id);   

}   /* End rsdb_pgo_util_purge_pgo. */

                                  
/*
 *  r s d b _ p g o _ u t i l _ g e t _ n e x t _ a l i a s
 */
PUBLIC boolean32 rsdb_pgo_util_get_next_alias
#ifndef __STDC__
    (domain, old_pgo, new_pgo)
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *old_pgo;
    rsdb_named_item_t   *new_pgo;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *old_pgo,
    rsdb_named_item_t   *new_pgo
  )
#endif
{
    if (old_pgo->body.threads.alias == domain_info[domain].last.body.threads.id)
        return false;
    return rsdb_pgo_util_get_by_id(domain, old_pgo->body.threads.alias,
                                    new_pgo);     

}   /* End rsdb_pgo_util_get_next_alias. */
                                                   
/*
 *  r s d b _ p g o _ u t i l _ g e t _ a n _ a l i a s
 */
PUBLIC void rsdb_pgo_util_get_an_alias
#ifndef __STDC__
    ( domain, credentials, allow_aliases, cursor, name, item, status)  
    sec_rgy_domain_t    domain;
    rsdb_credentials_t  *credentials;
    boolean32           allow_aliases;
    sec_rgy_cursor_t    *(cursor);
    sec_rgy_name_t      name;
    sec_rgy_pgo_item_t  *(item);
    error_status_t      *(status);
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_credentials_t  *credentials,
    boolean32           allow_aliases,
    sec_rgy_cursor_t    *(cursor),
    sec_rgy_name_t      name,
    sec_rgy_pgo_item_t  *(item),
    error_status_t      *(status)
  )
#endif
{                                   
    rsdb_named_item_t  pgo;
    boolean32          eq;

    CLEAR_STATUS(status);

    if (cursor->valid == true) {
        if (cursor->handle == END_OF_LIST) {
            cursor->valid = false;
            SET_STATUS(status, sec_rgy_no_more_entries);
            return;
        }
        if (!rsdb_pgo_util_get_by_marker(domain, cursor->handle, &pgo)) {
            /*
             * Somehow we lost the node that the marker referred to - probably
             * a deletion snuck in.  Mark this call sequence as dead by 
             * retyrning a no more entries status.
             */
            cursor->valid = false;
            SET_STATUS(status, sec_rgy_no_more_entries);
            return;
        }
        if (RSDB_PGO(&pgo).unix_id != credentials->unix_id) {
            /*
             * The previous object is no longer part of this alias chain - 
             * ignore it and start with the head of the chain afresh.
             */
            if (!rsdb_pgo_util_get_by_id(domain, credentials->id, &pgo)) {
                dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
            }            
        }
    } else {
        if (!rsdb_pgo_util_get_by_id(domain, credentials->id, &pgo)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
        }
    }
    export_pgo(domain, name, item, &RSDB_PGO(&pgo));
    cursor->valid = true;
    cursor->handle = pgo.body.threads.alias;
    if (!allow_aliases && RSDB_IS_AN_ALIAS(&RSDB_PGO(&pgo)))
       SET_STATUS(status, sec_rgy_is_an_alias);

}   /* End rsdb_pgo_util_get_an_alias. */
         
                                                  
/* 
 *  r s d b _ p g o _ u t i l _ d e l e t e _ p e r s o n
 */
PUBLIC void rsdb_pgo_util_delete_person
#ifndef __STDC__
    ( person ) 
    rsdb_named_item_t  *person;
#else
  (
    rsdb_named_item_t  *person
  )
#endif
{
    rsdb_named_item_t  aggregate;
    rsdb_pvt_id_t      acct_id;
    rsdb_acct_item_t   acct;

    /* Delete this person from the groups they belong to */
    rsdb_pgo_util_delete_member_list(sec_rgy_domain_person, person);

    /* Delete this person from the organizations they belong to */
    aggregate.body.threads.next = 
                      domain_info[sec_rgy_domain_org].first.body.threads.next;

    while (aggregate.body.threads.next != END_OF_LIST) {
        rsdb_pgo_util_get_by_id(sec_rgy_domain_org, aggregate.body.threads.next,
                                 &aggregate);
        rsdb_pgo_util_delete_member(sec_rgy_domain_org, &aggregate, person);
    }

    /* Delete this person's accounts */
    for (acct_id = RSDB_PGO(person).info; acct_id != END_OF_LIST; 
                                 acct_id = acct.threads.alias) {
        rsdb_get_acct_by_id(acct_id, &acct);
        rsdb_purge_account(false, person, &acct);
    }   
}   /* End rsdb_pgo_util_delete_person. */

/*
 *  r s d b _ p g o _ u t i l _ d e l e t e _ m e m b e r 
 */
PUBLIC boolean32 rsdb_pgo_util_delete_member
#ifndef __STDC__
    (domain, aggregate, member)
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *aggregate;
    rsdb_named_item_t   *member;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *aggregate,
    rsdb_named_item_t   *member
  )
#endif
{
    long                        i;
    rsdb_max_membership_list_t list;
    rsdb_pvt_id_t              list_id;
    rsdb_pvt_id_t              mem_id;

    /*
     * Don't need to do anything if the object is a directory instead
     * of a real PGO object
     */
    if (aggregate->type == rsdb_dir_type) {
        return false;
    }

    list_id = RSDB_PGO(aggregate).membership;
    if (list_id == END_OF_LIST &&
		RSDB_PGO(aggregate).foreign_membership == END_OF_LIST ) {
        return false;
    }

    mem_id = member->body.threads.id;

    while (list_id != END_OF_LIST) {
        rsdb_util_get_member_list(domain, list_id, &list);
        for (i = 0; i < list.total_count; i++) {
            if (!list.member[i].deleted && list.member[i].id == mem_id) {
                list.member[i].deleted = 1;
                list.count--;
                rsdb_util_store_member_list(domain, list_id, &list);
                RSDB_PGO(aggregate).num_members--;          
                rsdb_util_store_item_by_id(domain, aggregate->body.threads.id,
                                           aggregate);
                return true;
            }
        }
        if (list.next == END_OF_LIST)
            break;
        list_id = list.next;
    }
    
    list_id = RSDB_PGO(aggregate).foreign_membership;

    while (list_id != END_OF_LIST) {
        rsdb_util_get_member_list(domain, list_id, &list);
        for (i = 0; i < list.total_count; i++) {
            if (!list.member[i].deleted && list.member[i].id == mem_id) {
                list.member[i].deleted = 1;
                list.count--;
                rsdb_util_store_member_list(domain, list_id, &list);
                RSDB_PGO(aggregate).num_foreign_members--;          
                rsdb_util_store_item_by_id(domain, aggregate->body.threads.id,
                                           aggregate);
                return true;
            }
        }
        if (list.next == END_OF_LIST)
            break;
        list_id = list.next;
    }

    return false;

}   /* End rsdb_pgo_util_delete_member. */                         

                                          
 /*
  * r s d b _ p g o _ u t i l _ d e l e t e _ m e m b e r _ l i s t 
  *
  * Used to when deleting a group or organization.  This will delete all the
  * accounts for members of the aggregate and will also delete the
  * membership list object.  (The aggregate object is not updated and will
  * be inconsistent (have a dangling reference) if it is not deleted after
  * this operation). 
  */
PUBLIC void rsdb_pgo_util_delete_member_list
#ifndef __STDC__
    (domain, aggregate)
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *aggregate;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *aggregate
  )
#endif
{
    long                        i;
    rsdb_max_membership_list_t  list;
    rsdb_pvt_id_t               list_id;
    rsdb_named_item_t           member;
    sec_rgy_domain_t            member_domain;

    /*
     * Don't need to do anything if the object is a directory instead
     * of a real PGO object
     */
    if (aggregate->type == rsdb_dir_type) {
        return;
    }

    list_id = RSDB_PGO(aggregate).membership;
    if (list_id == END_OF_LIST && 
		RSDB_PGO(aggregate).foreign_membership == END_OF_LIST ) {
        return;
    }

    if (domain == sec_rgy_domain_person || 
		domain == sec_rgy_domain_foreign_person) {
        member_domain = sec_rgy_domain_group;
    } else {
        member_domain = sec_rgy_domain_person;
    }

    while (list_id != END_OF_LIST) {
        rsdb_util_get_member_list(domain, list_id, &list);
        for (i = 0; i < list.total_count; i++) {
            if (list.member[i].deleted == 0) {   
                if(!rsdb_pgo_util_get_by_id(member_domain, list.member[i].id,
                                             &member)) {
                    dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
                }
                if (domain == sec_rgy_domain_person) {
                    rsdb_pgo_util_delete_member(member_domain,
                                                &member, aggregate);
                } else {
                    if (domain == sec_rgy_domain_group) {
                        rsdb_pgo_util_delete_member(member_domain,
                                                    &member, aggregate);
                    }
                    rsdb_pgo_util_delete_member_accounts(domain, aggregate,
                                                         &member);
                                         
                }
            }
        }

        purge_member_list(domain, list_id);

        if (list.next == END_OF_LIST)
            break;
        list_id = list.next;
    }                     

    list_id = RSDB_PGO(aggregate).foreign_membership;

    if (domain == sec_rgy_domain_person || 
		domain == sec_rgy_domain_foreign_person) {
        member_domain = sec_rgy_domain_group;
    } else {
        member_domain = sec_rgy_domain_foreign_person;
    }

    while ( list_id != END_OF_LIST ) {
        rsdb_util_get_member_list(domain, list_id, &list);
        for (i = 0; i < list.total_count; i++) {
            if (list.member[i].deleted == 0) {   
                if(!rsdb_pgo_util_get_by_id(member_domain, list.member[i].id,
                                            &member)) {
                    dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
                }
                if (domain == sec_rgy_domain_foreign_person) {
                    rsdb_pgo_util_delete_member(member_domain,
                                                &member, aggregate);
                } 
            }
        }

        purge_member_list(domain, list_id);

        if (list.next == END_OF_LIST)
            break;
        list_id = list.next;
    }                     

}   /* End rsdb_pgo_util_delete_member_list. */


/*
 *  r s d b _ p g o _ u t i l _ m e m b e r _ l i s t _ a d d
 */
PUBLIC void rsdb_pgo_util_member_list_add
#ifndef __STDC__
    (domain, object_domain, target, object)
    sec_rgy_domain_t    domain;
    sec_rgy_domain_t	object_domain;
    rsdb_named_item_t   *target;
    rsdb_named_item_t   *object;
#else
  (
    sec_rgy_domain_t    domain,
    sec_rgy_domain_t	object_domain,
    rsdb_named_item_t   *target,
    rsdb_named_item_t   *object
  )
#endif
{
    long                        i;
    long                        slot;
    rsdb_pvt_id_t               id;
    rsdb_pvt_id_t               list_id;
    rsdb_max_membership_list_t  list;
    rsdb_pvt_id_t	        *member_id;
    long			*num_members;

    if ( domain == sec_rgy_domain_foreign_person ||
		object_domain == sec_rgy_domain_foreign_person ) {
	if ( object_domain == sec_rgy_domain_group ) {
	    /* add group as member to this foreign person */
	    member_id = &(RSDB_PGO(target).membership);
	    num_members = &(RSDB_PGO(target).num_members);
	} else {
	    /* add this foreign person as member to the group */
	    member_id = &(RSDB_PGO(target).foreign_membership);
	    num_members = &(RSDB_PGO(target).num_foreign_members);
	}
    } else {
	member_id = &(RSDB_PGO(target).membership);
	num_members = &(RSDB_PGO(target).num_members);
    }
    id = object->body.threads.id;
    list_id = *member_id;

    if (rsdb_util_is_member(domain, list_id, id, &list_id))
        return;

    /*
     * If there is no membership list create one
     */
    if (*member_id == END_OF_LIST) {
        *member_id = rsdb_new_sequential_id();
        list_id = *member_id;
        list.next = END_OF_LIST;
        list.total_count = 0;
        list.count = 0;
    } else {
        rsdb_util_get_member_list(domain, list_id, &list);
        if (list.count == MAX_MEMBER_LIST_SIZE) {
            /*
	     * element is full, add another list element
	     */
            if (list.next != END_OF_LIST) {
                dce_svc_printf(SEC_RSDB_LIST_NOT_TERMINATED_MSG);
            }
            list.next = rsdb_new_sequential_id();
            rsdb_util_store_member_list(domain, list_id, &list);
	    
            list.total_count = 0;
            list.count = 0;
            list_id = list.next;
            list.next = END_OF_LIST;
        }
    }
    /* 
     * locate the next available slot in the membership list
     */
    if (list.count == list.total_count) {
	/* 
	 * the contains no deleted elements (possibly because we
	 * just created it)
	 */
        slot = list.total_count;
        list.total_count++;
    } else {
	/*
	 * the list contains deleted elements
	 */
        slot = -1;
        for (i = 0; i < list.total_count; i++) {
            if (list.member[i].deleted == 1) {
                slot = i;
                break;
            }
        }
        if (slot == -1)
            dce_svc_printf(SEC_RSDB_NO_OPEN_SLOT_MSG);
    } 

    list.member[slot].id = id;
    list.member[slot].deleted = 0;
    list.member[slot].has_acct = 0;
    list.count++;
    rsdb_util_store_member_list(domain, list_id, &list);
    (*num_members)++;
    rsdb_util_store_item_by_id(domain, target->body.threads.id, target);

}   /*  End rsdb_pgo_util_member_list_add. */


                                                    
/*
 *  r s d b _ p g o _ u t i l _ d e l e t e _ m e m b e r _ a c c o u n t s
 */
PUBLIC void rsdb_pgo_util_delete_member_accounts
#ifndef __STDC__
    (domain, aggregate, member)
    sec_rgy_domain_t    domain;
    rsdb_named_item_t   *aggregate;
    rsdb_named_item_t   *member;
#else
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *aggregate,
    rsdb_named_item_t   *member
  )
#endif
{
    rsdb_acct_item_t    account;
    rsdb_pgo_item_t     dummy;
    rsdb_pgo_item_t     *group;
    rsdb_pgo_item_t     *org;
    sec_rgy_acct_key_t  key_parts;
    sec_rgy_cursor_t    cursor;

    dummy.threads.id = END_OF_LIST;
    cursor.valid = false;

    if (domain == sec_rgy_domain_group) {
        group = &RSDB_PGO(aggregate);
        org = &dummy;
        key_parts = sec_rgy_acct_key_group;
    } else {
        group = &dummy;
        org = &RSDB_PGO(aggregate);
        key_parts = sec_rgy_acct_key_org;
    }

    while (find_account(false, &cursor, &RSDB_PGO(member),
                        group, org, key_parts, &account)) {
		
        rsdb_purge_account(true, member, &account);
    }
}   /* End rsdb_pgo_util_delete_member_accounts. */

