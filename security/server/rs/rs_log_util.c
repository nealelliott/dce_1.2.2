/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_util.c,v $
 * Revision 1.1.6.3  1996/02/18  00:18:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:03  marty]
 *
 * Revision 1.1.6.2  1995/12/08  17:52:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:07 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:58 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Move some acl logging routines from rs_log_acl.c into
 * 	rs_log_util.c since they are now common to both the
 * 	acl and software version logging modules.
 * 	[1995/04/03  18:41 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.2.2  1992/12/29  16:35:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_log_util.c V=7 10/14/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 *     Registry Server - Common Log operations
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_log_util.c	7 - 10/14/91";
#endif

#include <rgymacro.h>
#include <un_strng.h>
#include <bstring.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rs_log_util.h>

void rs_log_import_acct_name (
    sec_rgy_login_name_t    *login_name,
    unsigned32              *pname_len,   /* [out] */
    unsigned32              *gname_len,   /* [out] */
    unsigned32              *oname_len,   /* [out] */
    rs_log_acct_name_buf_t  buf,          /* [out] */
    long                    *size
)
{
    char                    *p;

    p = buf;

    *size = 0;

    /* Fixed portion */
    *pname_len   = strlen( (char *) login_name->pname);
    *gname_len   = strlen( (char *) login_name->gname);
    *oname_len   = strlen( (char *) login_name->oname);

    /* Variable portion */
    bcopy( (Pointer) login_name->pname, p, *pname_len);
    p       += *pname_len;
    *size   += *pname_len;

    bcopy( (Pointer) login_name->gname, p, *gname_len);
    p       += *gname_len;
    *size   += *gname_len;

    bcopy( (Pointer) login_name->oname, p, *oname_len);
    p       += *oname_len;
    *size   += *oname_len;
}


void rs_log_export_acct_name (
    sec_rgy_login_name_t    *login_name,  /* [out] */
    unsigned32              pname_len,    /* [in] */
    unsigned32              gname_len,    /* [in] */
    unsigned32              oname_len,    /* [in] */
    rs_log_acct_name_buf_t  buf
)
{
    char                    *p = buf;

    bcopy(p, (Pointer) login_name->pname, pname_len);
    login_name->pname[pname_len] = NULL;
    p += pname_len;

    bcopy(p, (Pointer) login_name->gname, gname_len);
    login_name->gname[gname_len] = NULL;
    p += gname_len;

    bcopy(p, (Pointer) login_name->oname, oname_len);
    login_name->oname[oname_len] = NULL;
    p += oname_len;
}

void rs_log_util_import_acl_item (
    sec_acl_list_t          *sec_acl_list,
    rs_log_acl_item_t       *item,
    error_status_t          *st
)
{
    rsdb_acl_t              *db_acl;
    sec_acl_t               *sec_acl;

    CLEAR_STATUS(st);
    /* this code only handles a "list" containing one sec_acl */
    sec_acl = sec_acl_list->sec_acls[0];

    /* Pack the acl into item. */
    db_acl = (rsdb_acl_t *)item;
    rsdb_acl_import(db_acl, sec_acl, st);
    return;
}

void rs_log_util_export_acl_item (
    rs_log_acl_item_t       *item,
    sec_acl_list_t          **sec_acl_list,
    error_status_t          *st
)
{
    rsdb_acl_t              *db_acl;
    sec_acl_t               *sec_acl;
    unsigned32              size, acl_len;

    CLEAR_STATUS(st);
    db_acl = (rsdb_acl_t *)item;

    size = sizeof(sec_acl_list_t);
    /* Allocate the sec_acl_list header.
     */
    if(!(*sec_acl_list = (sec_acl_list_t *)malloc(size))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        return;
    }
    /* Allocate buffer for output acl and export acl info.
     * sec_acl allocated in rsdb_acl_alloc_sec_acl
     * must be freed by caller.
     */
    rsdb_acl_alloc_sec_acl(db_acl, &sec_acl, &acl_len, st);
    if(BAD_STATUS(st)) {
        free((char *)(*sec_acl_list));
        return;
    }
    rsdb_acl_export(sec_acl, db_acl, st);
    if(BAD_STATUS(st)) {
        free((char *)sec_acl);
        free((char *)(*sec_acl_list));
        return;
    }
    (*sec_acl_list)->num_acls = 1;
    (*sec_acl_list)->sec_acls[0] = sec_acl;

    return;
}

/*  r s _ l o g _ u t i l _ a l l o c _ a c l _ i t e m _ l o g _ r e c
 *
 * Malloc a buffer large enough to hold the input "size" value
 * and the variable length parts of the input sec_acl_t.
 * If sec_acl has no entries, keep one rsdb_acl_entry for access to
 * variable length data in the log rec containing the acl.
 * Set acl_len parameter provided by the caller. Buffer allocated must be 
 * freed by caller.
 *
 * Status Codes:
 *      sec_acl_invalid_entry_type      if bad entry in input acl.
 *      sec_acl_cant_allocate_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
void rs_log_util_alloc_acl_item_log_rec (
    sec_acl_t                   *acl,      /* [in] */
    long                        *size,     /* [in, out] */
    long                        *acl_len,  /* [out] */
    void                        **log_rec, /* [out] */
    error_status_t              *st
)
{
    int                 i;
    unsigned_char_p_t   new_buf;

    CLEAR_STATUS(st);
    *log_rec = NULL;
                                      
    *acl_len = sizeof(rs_log_acl_item_t);

    /* Since the rs_log_acl_item_t already includes one
     * rsdb_acl_entry_t, account for num_entries - 1 more
     */
    for(i = 0; i < acl->num_entries - 1; i++ ) {
        *acl_len += DB_ACL_ENTRY_LEN(&(acl->sec_acl_entries[i]));
    }

    *size += *acl_len;
    if(!(new_buf = (unsigned_char_p_t) malloc(*size))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        return;
    }
    *log_rec = new_buf;
}
