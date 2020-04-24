/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: passwd_import_subs.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:25  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:26  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:57:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:03  root]
 * 
 * Revision 1.1.6.1  1994/03/25  17:29:35  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  17:28:40  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/29  12:38:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:07  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/23  14:14:44  burati
 * 	Initial version
 * 	[1992/04/23  01:31:03  burati]
 * 
 * $EndLog$
 */

/*  passwd_import_subs.c, greg, 07/27/89
**   dynamic cache subroutines for passwd_import
**
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
**
** Changes: 
**  04/15/92    burati  More DCE porting changes/minor rewrite.
**  07/13/91    burati  Changes for NCS2.0 & DCE Security,
**  07/27/89    greg    port from original import_passwd.pas
*/

#include <stdio.h>
#include <string.h>
#include <dce/dce.h>
#include <dce/lbase.h>
#include <dce/dce_error.h>
#include <stdlib.h>
#include <macros.h>
#include <dce/dcesadmsg.h>
#include <balanced_trees.h>
#include <passwd_import_subs.h>

static tree_handle         p_num_tree;
static tree_handle         p_name_tree;
static tree_handle         g_num_tree;
static tree_handle         g_name_tree;

/* imp_init
 *
 * MUST be called before using the imp package 
 */
PUBLIC void imp_init(
#ifdef __STDC__
    void
#endif
)
{
    p_num_tree = p_name_tree = g_num_tree = g_name_tree = (tree_handle)NULL;
}


/*
 * imp_add - adds an entry to the cache.
 *
 *  Packed Entry:   <orig_num><new_num>
 *                  <orig_name_len><orig_name>
 *                  <new_name_len><new_name>
 */
PUBLIC void imp_add
#ifndef __STDC__
    ( domain, entry, is_alias, status ) 
    sec_rgy_domain_t    domain;
    imp_tbl_ent_t       *entry;
    boolean32           *is_alias; 
    error_status_t      *status;
#else
  (
    sec_rgy_domain_t    domain,
    imp_tbl_ent_t       *entry,
    boolean32           *is_alias, 
    error_status_t      *status
  )
#endif
{ 
    boolean32       alias_ok;
    int             len;
    imp_tbl_ent_t   e;
    char            *p;
    b_datum         num_key, name_key, new_data;
    tree_handle     *num_tree, *name_tree;

    alias_ok = ((domain == sec_rgy_domain_person) ||
                (domain == sec_rgy_domain_group));
    *is_alias = false;

    switch(domain) {
        case sec_rgy_domain_person:
            num_tree  = &p_num_tree;
            name_tree = &p_name_tree;
            break;
        case sec_rgy_domain_group:
            num_tree  = &g_num_tree;
            name_tree = &g_name_tree;
            break;
        default:
            SET_STATUS(status, pwd_imp_err_internal_error);
            return;
            break;
    }

    /* First see if the name is already in the cache.
     * If it is, it"s an error.
     */
    imp_lookup_name(domain, entry->orig_name,
                    NULL, status);
    if (GOOD_STATUS(status)) {
        SET_STATUS(status, pwd_imp_err_dup_name);
        return;
    }

    /* see if the num is already in the cache.  If so,
     * and aliases are not allowed, its an error.
     */
    imp_lookup_num(domain, entry->orig_num, &e, status);
    *is_alias = (GOOD_STATUS(status)); 
    if (*is_alias && !alias_ok) {
        SET_STATUS(status, pwd_imp_err_dup_num);
        return;
    }

    /* ok so far, get a new entry */
    len=((4*sizeof(signed32)) +
         ((strlen(entry->orig_name)+strlen(entry->new_name)) * sizeof(char)) );
    p = new_data.dptr = (char *) malloc(len);
    if (p == NULL) { 
        SET_STATUS(status, pwd_imp_err_no_memory);
        return;
    }                       
    new_data.dsize = len;

    /* Pack the data */
        /* Pack the orig_num in data and num_key */
    memcpy(p, (char *)&entry->orig_num, sizeof(signed32));
    p += sizeof(signed32);
    num_key.dsize = sizeof(signed32);
    num_key.dptr = (char *)malloc(sizeof(signed32));
    if (num_key.dptr == NULL) { 
        SET_STATUS(status, pwd_imp_err_no_memory);
        return;
    }                       
    memcpy(num_key.dptr, (char *)&entry->orig_num, sizeof(signed32));

    memcpy(p, (char *)&entry->new_num, sizeof(signed32));
    p += sizeof(signed32);

        /* Pack orig_name in data, and name_key */
    len = strlen(entry->orig_name);
    memcpy(p, (char *)&len, sizeof(signed32));
    p += sizeof(signed32);
    memcpy(p, entry->orig_name, (size_t)len);
    p += len;
    name_key.dsize = len;
    name_key.dptr = (char *)malloc(len * sizeof(char));
    if (name_key.dptr == NULL) { 
        SET_STATUS(status, pwd_imp_err_no_memory);
        return;
    }                       
    memcpy(name_key.dptr, entry->orig_name, (size_t)len);

    len = strlen(entry->new_name);
    memcpy(p, (char *)&len, sizeof(signed32));
    p += sizeof(signed32);
    memcpy(p, entry->new_name, (size_t)len);

    if (! insert_data(false, name_tree, &name_key, &new_data)) {
        SET_STATUS(status, pwd_imp_err_internal_error);
        return;
    }
    if (! *is_alias) {
        if (! insert_data(false, num_tree, &num_key, &new_data)) {
            SET_STATUS(status, pwd_imp_err_internal_error);
            return;
        }
    }

    CLEAR_STATUS(status); 
    return;
} /* imp_add */ 


/*
 * imp_lookup_name - search for a matching orig_name entry in the
 *                   specified domain.
 */ 
void imp_lookup_name
#ifndef __STDC__
  ( domain, orig_name, entry, status )
    sec_rgy_domain_t    domain;
    char                *orig_name;
    imp_tbl_ent_t       *entry;
    error_status_t      *status;
#else
  (
    sec_rgy_domain_t    domain,
    char                *orig_name,
    imp_tbl_ent_t       *entry,
    error_status_t      *status
  )
#endif
{
    tree_handle name_tree;
    b_datum     *data, search_key;
    signed32    len;
    char        *p;

    SET_STATUS(status, pwd_imp_err_cache_e_not_found);  /* be a pessimist */
    switch(domain) {
        case sec_rgy_domain_person:
            name_tree = p_name_tree;
            break;
        case sec_rgy_domain_group:
            name_tree = g_name_tree;
            break;
        default:
            SET_STATUS(status, pwd_imp_err_internal_error);
            return;
            break;
    }
    search_key.dsize = strlen((char *)orig_name);
    search_key.dptr = (char *)orig_name;
    data = fetch_data(name_tree, &search_key);
    if (data) {
        CLEAR_STATUS(status);
        if (entry) {        /* Only pass back data if requested */
            p = data->dptr;
            memcpy((char *)&(entry->orig_num), p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)&(entry->new_num), p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)&len, p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)entry->orig_name, p, (size_t)len);
            entry->orig_name[len] = '\0';
            p += len;
            memcpy((char *)&len, p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)entry->new_name, p, (size_t)len);
            entry->new_name[len] = '\0';
        }
    }

    return;
} /* imp_lookup_name */


/*
 * imp_lookup_num - search for a matching orig_num entry in the
 *                  specified domain.
 */
void imp_lookup_num
#ifndef __STDC__
  ( domain, orig_num, entry, status ) 
    sec_rgy_domain_t    domain;
    signed32            orig_num;
    imp_tbl_ent_t       *entry;
    error_status_t      *status;
#else
  (
    sec_rgy_domain_t    domain,
    signed32            orig_num,
    imp_tbl_ent_t       *entry,
    error_status_t      *status
  )
#endif
{
    tree_handle num_tree;
    b_datum     *data, search_key;
    int         len;
    char        *p;

    SET_STATUS(status, pwd_imp_err_cache_e_not_found);  /* be a pessimist */
    switch(domain) {
        case sec_rgy_domain_person:
            num_tree = p_num_tree;
            break;
        case sec_rgy_domain_group:
            num_tree = g_num_tree;
            break;
        default:
            SET_STATUS(status, pwd_imp_err_internal_error);
            return;
            break;
    }
    search_key.dsize = sizeof(signed32);
    search_key.dptr = (char *)&orig_num;
    data = fetch_data(num_tree, &search_key);
    if (data) {
        CLEAR_STATUS(status);
        if (entry) {        /* Only pass back data if requested */
            p = data->dptr;
            memcpy((char *)&(entry->orig_num), p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)&(entry->new_num), p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)&len, p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)entry->orig_name, p, (size_t)len);
            entry->orig_name[len] = '\0';
            p += len;
            memcpy((char *)&len, p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)entry->new_name, p, (size_t)len);
            entry->new_name[len] = '\0';
        }
    }
    
    return;
} /* imp_lookup_num */ 


/*
 * imp_get_next - get next sequential entry from specified domain.
 *              To get first entry, pass in NULL for imp_ptr.
 */
void imp_get_next
#ifndef __STDC__
  ( domain, imp_cursor, entry, status ) 
    sec_rgy_domain_t    domain;
    void                **imp_cursor;
    imp_tbl_ent_t       *entry;
    error_status_t      *status;
#else
  (
    sec_rgy_domain_t    domain,
    void                **imp_cursor,
    imp_tbl_ent_t       *entry,
    error_status_t      *status
  )
#endif
{
    b_datum     dummy, *data, *next;
    tree_handle tree;
    char        *p, ch = '\0';
    int         len;

    SET_STATUS(status, pwd_imp_err_cache_e_not_found);  /* be a pessimist */
    switch(domain) {
        case sec_rgy_domain_person:
            tree = p_name_tree;
            break;
        case sec_rgy_domain_group:
            tree = g_name_tree;
            break;
        default:
            SET_STATUS(status, pwd_imp_err_internal_error);
            return;
            break;
    }
    
        /* Caller treats cursor as abstract ptr (void),
         * handle cursor reset (null ptr) here.
         */
    if (*imp_cursor == NULL) {
        *imp_cursor = (void *)&dummy;
        dummy.dsize = sizeof(char);
        dummy.dptr  = (char *)&ch;
    }

    data = fetch_next(tree, (b_datum *)*imp_cursor, &next);
    if (data) {
        CLEAR_STATUS(status);
        *imp_cursor = (void *)next;
        if (entry) {        /* Only pass back data if requested */
            p = data->dptr;
            memcpy((char *)&(entry->orig_num), p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)&(entry->new_num), p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)&len, p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)entry->orig_name, p, (size_t)len);
            entry->orig_name[len] = '\0';
            p += len;
            memcpy((char *)&len, p, sizeof(signed32));
            p += sizeof(signed32);
            memcpy((char *)entry->new_name, p, (size_t)len);
            entry->new_name[len] = '\0';
        }

    }
    return;
} /* imp_get_next */


/*
 * imp_cleanup - release allocated memory
 */
void imp_cleanup(
#ifdef __STDC__
void
#endif
) 
{
    clear_database(&p_num_tree);
    clear_database(&p_name_tree);
    clear_database(&g_num_tree);
    clear_database(&g_name_tree);
    return;
} /* imp_cleanup */

