/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: keytab.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:09  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:59:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  keytab.h V=4 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/keytab.h,v $
 * $Author: marty $
 * $Id: keytab.h,v 1.1.4.2 1996/02/18 23:02:38 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Keytab definitions.
 */


#ifndef KRB5_KEYTAB__
#define KRB5_KEYTAB__


/* XXX */
#define MAX_KEYTAB_NAME_LEN 1100 /* Long enough for MAXPATHLEN + some extra */

typedef krb5_pointer krb5_kt_cursor;	/* XXX */

typedef struct krb5_keytab_entry_st {
    krb5_principal principal;	/* principal of this key */
    krb5_timestamp timestamp;   /* time entry written to keytable */
    krb5_kvno vno;		/* key version number */
    krb5_keyblock key;		/* the secret key */
} krb5_keytab_entry;


typedef struct _krb5_kt {
	struct _krb5_kt_ops *ops;
	krb5_pointer data;
} *krb5_keytab;


/* widen prototypes, if needed */
#include <krb5/widen.h>

typedef struct _krb5_kt_ops {
	char *prefix;
        /* routines always present */
	krb5_error_code (*resolve) PROTOTYPE((char *,
					      krb5_keytab *));
	krb5_error_code (*get_name) PROTOTYPE((krb5_keytab,
					       char *,
					       int));
	krb5_error_code (*close) PROTOTYPE((krb5_keytab));
	krb5_error_code (*get) PROTOTYPE((krb5_keytab,
					  krb5_principal,
					  krb5_kvno,
					  krb5_keytab_entry *));
	krb5_error_code (*start_seq_get) PROTOTYPE((krb5_keytab,
						    krb5_kt_cursor *));	
	krb5_error_code (*get_next) PROTOTYPE((krb5_keytab,
					       krb5_keytab_entry *,
					       krb5_kt_cursor *));
	krb5_error_code (*end_get) PROTOTYPE((krb5_keytab,
					      krb5_kt_cursor *));
	/* routines to be included on extended version (write routines) */
	krb5_error_code (*add) PROTOTYPE((krb5_keytab,
					  krb5_keytab_entry *));
	krb5_error_code (*remove) PROTOTYPE((krb5_keytab,
					     krb5_keytab_entry *));
} krb5_kt_ops;

/* and back to narrow */
#include <krb5/narrow.h>

#define krb5_kt_get_name(keytab, name, namelen) (*(keytab)->ops->get_name)(keytab,name,namelen)
#define krb5_kt_close(keytab) (*(keytab)->ops->close)(keytab)
#define krb5_kt_get_entry(keytab, principal, vno, entry) (*(keytab)->ops->get)(keytab, principal, vno, entry)
#define krb5_kt_start_seq_get(keytab, cursor) (*(keytab)->ops->start_seq_get)(keytab, cursor)
#define krb5_kt_next_entry(keytab, entry, cursor) (*(keytab)->ops->get_next)(keytab, entry, cursor)
#define krb5_kt_end_seq_get(keytab, cursor) (*(keytab)->ops->end_get)(keytab, cursor)
/* remove and add are functions, so that they can return NOWRITE
   if not a writable keytab */


extern krb5_kt_ops krb5_kt_dfl_ops;

#endif /* KRB5_KEYTAB__ */
