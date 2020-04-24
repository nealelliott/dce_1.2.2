/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ccache.h,v $
 * Revision 1.1.6.2  1996/11/13  18:03:33  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/21  16:50 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Remove psm handle from krb5_ccache, no longer needed with
 * 	new preauth specific data storage mechanism.
 * 	[1996/09/26  23:17 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.6.1  1996/10/03  14:55:46  arvind
 * 	Add hidey-hole for psm handle in ccache.
 * 	(arguably the wrong place for it..)
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.4.2  1996/02/18  23:02:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:36  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:41:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:54  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:58:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ccache.h V=5 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/ccache.h,v $
 * $Author: arvind $
 * $Id: ccache.h,v 1.1.6.2 1996/11/13 18:03:33 arvind Exp $
 *
 * Copyright 1989,1991 by the Massachusetts Institute of Technology.
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
 * Credentials cache definitions.
 */


#ifndef KRB5_CCACHE__
#define KRB5_CCACHE__

typedef	krb5_pointer	krb5_cc_cursor;	/* cursor for sequential lookup */

typedef struct _krb5_ccache {
    struct _krb5_cc_ops *ops;
    krb5_pointer data;
} *krb5_ccache;

typedef struct _krb5_cc_ops {
	char *prefix;
	char *(*get_name) PROTOTYPE((krb5_ccache));
	krb5_error_code (*resolve) PROTOTYPE((krb5_ccache *, char *));
	krb5_error_code (*gen_new) PROTOTYPE((krb5_ccache *));
	krb5_error_code (*init) PROTOTYPE((krb5_ccache, krb5_principal));
	krb5_error_code (*destroy) PROTOTYPE((krb5_ccache));
	krb5_error_code (*close) PROTOTYPE((krb5_ccache));
	krb5_error_code (*store) PROTOTYPE((krb5_ccache, krb5_creds *));
	krb5_error_code (*retrieve) PROTOTYPE((krb5_ccache, krb5_flags,
				   krb5_creds *, krb5_creds *));
	krb5_error_code (*get_princ) PROTOTYPE((krb5_ccache,
						krb5_principal *));
	krb5_error_code (*get_first) PROTOTYPE((krb5_ccache,
						krb5_cc_cursor *));
	krb5_error_code (*get_next) PROTOTYPE((krb5_ccache, krb5_cc_cursor *,
				   krb5_creds *));
	krb5_error_code (*end_get) PROTOTYPE((krb5_ccache, krb5_cc_cursor *));
	krb5_error_code (*remove_cred) PROTOTYPE((krb5_ccache, krb5_flags,
				      krb5_creds *));
	krb5_error_code (*set_flags) PROTOTYPE((krb5_ccache, krb5_flags));
} krb5_cc_ops;

/* for retrieve_cred */
#define	KRB5_TC_MATCH_TIMES		0x00000001
#define	KRB5_TC_MATCH_IS_SKEY		0x00000002
#define	KRB5_TC_MATCH_FLAGS		0x00000004
#define	KRB5_TC_MATCH_TIMES_EXACT	0x00000008
#define	KRB5_TC_MATCH_FLAGS_EXACT	0x00000010
#define	KRB5_TC_MATCH_AUTHDATA		0x00000020
#define	KRB5_TC_MATCH_SRV_NAMEONLY	0x00000040
#define	KRB5_TC_MATCH_2ND_TKT		0x00000080

/* for set_flags and other functions */
#define KRB5_TC_OPENCLOSE		0x00000001

#define krb5_cc_initialize(cache, principal) (*(cache)->ops->init)(cache, principal)
#define krb5_cc_destroy(cache) (*(cache)->ops->destroy)(cache)
#define krb5_cc_close(cache) (*(cache)->ops->close)(cache)
#define krb5_cc_store_cred(cache, creds) (*(cache)->ops->store)(cache, creds)
#define krb5_cc_retrieve_cred(cache, flags, mcreds, creds) (*(cache)->ops->retrieve)(cache, flags, mcreds, creds)
#define krb5_cc_get_principal(cache, principal) (*(cache)->ops->get_princ)(cache, principal)
#define krb5_cc_start_seq_get(cache, cursor) (*(cache)->ops->get_first)(cache, cursor)
#define krb5_cc_next_cred(cache, cursor, creds) (*(cache)->ops->get_next)(cache, cursor, creds)
#define krb5_cc_end_seq_get(cache, cursor) (*(cache)->ops->end_get)(cache, cursor)
#define krb5_cc_remove_cred(cache, flags, creds) (*(cache)->ops->remove_cred)(cache,flags, creds)
#define krb5_cc_set_flags(cache, flags) (*(cache)->ops->set_flags)(cache, flags)
#define krb5_cc_get_name(cache) (*(cache)->ops->get_name)(cache)

extern krb5_cc_ops *krb5_cc_dfl_ops;

#endif /* KRB5_CCACHE__ */
