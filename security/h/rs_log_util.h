/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_util.h,v $
 * Revision 1.1.6.3  1996/02/18  22:58:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:41  marty]
 *
 * Revision 1.1.6.2  1995/12/13  16:25:22  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Move some acl logging types/routines from rs_log_acl.c into
 * 	rs_log_util.c since they are now common to both the
 * 	acl and software version logging modules.
 * 	[1995/04/03  18:40 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.2.2  1992/12/29  13:08:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_log_util.h V=6 10/14/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - commomn log data manipulation
 * 
 */

#ifndef rs_log_util__included
#define rs_log_util__included
 
#include <dce/rgynbase.h>
#include <rsdb_pvt.h>

#define ACCT_NAME_BUFSIZE (3 * (sizeof(sec_rgy_name_t) -1))

typedef char rs_log_acct_name_buf_t[ACCT_NAME_BUFSIZE];

/*
 * Although the rsdb_acl_t was designed for database
 * storage purposes (e.g., it includes the rsdb_thread_hdr_t
 * which is unnecessary for logging), it is used for logging
 * in order to leverage the acl import/export and allocation code.
 */
typedef rsdb_acl_fixed_t rs_log_acl_item_t;

extern void rs_log_import_acct_name (
    sec_rgy_login_name_t    *login_name,  /* [in] */
    unsigned32              *pname_len,   /* [out] */
    unsigned32              *gname_len,   /* [out] */
    unsigned32              *oname_len,   /* [out] */
    rs_log_acct_name_buf_t  buf,          /* [out] */
    long                    *size         /* [out] */
);

extern void rs_log_export_acct_name (
    sec_rgy_login_name_t    *login_name,  /* [out] */
    unsigned32              pname_len,    /* [in] */
    unsigned32              gname_len,    /* [in] */
    unsigned32              oname_len,    /* [in] */
    rs_log_acct_name_buf_t  buf           /* [in] */
);

extern void rs_log_util_import_acl_item (
    sec_acl_list_t          *sec_acl_list,
    rs_log_acl_item_t       *item,
    error_status_t          *st
);

extern void rs_log_util_export_acl_item (
    rs_log_acl_item_t       *item,
    sec_acl_list_t          **sec_acl_list,
    error_status_t          *st
);

extern void rs_log_util_alloc_acl_item_log_rec (
    sec_acl_t                   *acl,      /* [in] */
    long                        *size,     /* [in, out] */
    long                        *acl_len,  /* [out] */
    void                        **log_rec, /* [out] */
    error_status_t              *st
);

#endif
