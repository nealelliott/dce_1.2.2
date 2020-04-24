/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdb_dbm.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:08  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:59:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  kdb_dbm.h V=7 11/11/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/kdb_dbm.h,v $
 * $Author: marty $
 * $Id: kdb_dbm.h,v 1.1.4.2 1996/02/18 23:02:36 marty Exp $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * KDC Database interface definitions.
 */


#ifndef KRB5_KDB5_DBM__
#define KRB5_KDB5_DBM__

#define	DEFAULT_DBM_FILE		"/krb5/principal"

/* exclusive or shared lock flags */
#define	KRB5_DBM_SHARED		0
#define	KRB5_DBM_EXCLUSIVE	1

#define KRB5_DB_SHARED 		KRB5_DBM_SHARED
#define KRB5_DB_EXCLUSIVE	KRB5_DBM_EXCLUSIVE

/* #define these to avoid an indirection function; for future implementations,
   these may be redirected from a dispatch table/routine */
#define krb5_dbm_db_set_name krb5_db_set_name
#define krb5_dbm_db_set_nonblocking krb5_db_set_nonblocking
#define krb5_dbm_db_init krb5_db_init
#define krb5_dbm_db_fini krb5_db_fini
#define krb5_dbm_db_get_age krb5_db_get_age
#define krb5_dbm_db_create krb5_db_create
/* #define krb5_dbm_db_destroy kdb5_db_destroy */
#define krb5_dbm_db_rename krb5_db_rename
#define krb5_dbm_db_get_principal krb5_db_get_principal
#define krb5_dbm_db_free_principal krb5_db_free_principal
#define krb5_dbm_db_put_principal krb5_db_put_principal
#define krb5_dbm_db_delete_principal krb5_db_delete_principal
#define krb5_dbm_db_iterate krb5_db_iterate
#define krb5_dbm_db_lock krb5_db_lock
#define krb5_dbm_db_unlock krb5_db_unlock
#define krb5_dbm_db_set_lockmode krb5_db_set_lockmode
#define krb5_dbm_db_get_policy krb5_db_get_policy

/* libkdb.spec */
krb5_error_code krb5_dbm_db_set_name PROTOTYPE(());
krb5_error_code krb5_dbm_db_init PROTOTYPE(());
krb5_error_code krb5_dbm_db_fini PROTOTYPE(());
krb5_error_code krb5_dbm_db_get_age PROTOTYPE(());
krb5_error_code krb5_dbm_db_create PROTOTYPE(());
/* krb5_error_code krb5_dbm_db_destroy PROTOTYPE(()); */
krb5_error_code krb5_dbm_db_rename PROTOTYPE(());
void krb5_dbm_db_free_principal PROTOTYPE((krb5_db_entry *, int ));
krb5_error_code krb5_dbm_db_put_principal 
    PROTOTYPE((krb5_db_entry *ignored, int * ignored1));
krb5_error_code krb5_dbm_db_get_policy PROTOTYPE((krb5_db_policy *));
krb5_error_code krb5_dbm_db_iterate
    PROTOTYPE((krb5_error_code (*) PROTOTYPE((krb5_pointer,
					      krb5_db_entry *)),
	       krb5_pointer ));

/* need to play games here, since we take a pointer and the real thing,
   and it might be narrow. */
#ifdef NARROW_PROTOTYPES
krb5_error_code krb5_dbm_db_get_principal PROTOTYPE((krb5_principal,
						     krb5_keytype,
						     krb5_kvno,
						     krb5_db_entry *,
						     int *,
						     krb5_boolean * ));

krb5_error_code krb5_dbm_db_set_nonblocking PROTOTYPE((krb5_boolean,
						       krb5_boolean * ));
krb5_boolean krb5_dbm_db_set_lockmode
	PROTOTYPE((krb5_boolean ));
#else
krb5_error_code krb5_dbm_db_get_principal PROTOTYPE((krb5_principal,
						     int,  /* krb5_keytype */
						     int,  /* krb5_kvno */
						     krb5_db_entry *,
						     int *,
						     krb5_boolean * ));

krb5_error_code krb5_dbm_db_set_nonblocking PROTOTYPE((int, /* krb5_boolean */
						       krb5_boolean * ));
krb5_boolean krb5_dbm_db_set_lockmode
	PROTOTYPE((int /* krb5_boolean */ ));
#endif /* NARROW_PROTOTYPES */

#endif /* KRB5_KDB5_DBM__ */
