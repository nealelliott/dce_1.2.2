/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktfile.h,v $
 * Revision 1.1.10.1  1996/10/02  20:59:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:36  damon]
 *
 * Revision 1.1.5.1  1994/06/09  14:19:51  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:37  annie]
 * 
 * Revision 1.1.3.2  1993/01/21  15:19:51  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:58:26  cjd]
 * 
 * Revision 1.1  1992/01/19  02:52:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ktfile.h V=3 11/19/90 //littl/prgy/krb5/lib/keytab/file
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/file/security/dfsauth/ktfile.h,v $
 * $Author: damon $
 * $Id: ktfile.h,v 1.1.10.1 1996/10/02 20:59:18 damon Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * This header file contains information needed by internal routines
 * of the file-based ticket cache implementation.
 */

#include <krb5/copyright.h>

#ifndef KRB5_KTFILE__
#define KRB5_KTFILE__

#include <stdio.h>

/*
 * Constants
 */
#define IGNORE_VNO 0


/* 
 * Types
 */
typedef struct _krb5_ktfile_data {
    char *name;			/* Name of the file */
    FILE *openf;		/* open file, if any. */
} krb5_ktfile_data;

/*
 * Macros
 */
#define KTPRIVATE(id) ((krb5_ktfile_data *)(id)->data)
#define KTFILENAME(id) (((krb5_ktfile_data *)(id)->data)->name)
#define KTFILEP(id) (((krb5_ktfile_data *)(id)->data)->openf)

extern struct _krb5_kt_ops krb5_ktf_ops;
extern struct _krb5_kt_ops krb5_ktf_writable_ops;

/* widen prototypes, if needed */
#include <krb5/widen.h>

krb5_error_code krb5_ktfile_resolve PROTOTYPE((char *,
					       krb5_keytab *));
krb5_error_code krb5_ktfile_wresolve PROTOTYPE((char *,
					       krb5_keytab *));
krb5_error_code krb5_ktfile_get_name PROTOTYPE((krb5_keytab,
						char *,
						int));
krb5_error_code krb5_ktfile_close PROTOTYPE((krb5_keytab));
krb5_error_code krb5_ktfile_get_entry PROTOTYPE((krb5_keytab,
						 krb5_principal,
						 krb5_kvno,
						 krb5_keytab_entry *));
krb5_error_code krb5_ktfile_start_seq_get PROTOTYPE((krb5_keytab,
						     krb5_kt_cursor *));
krb5_error_code krb5_ktfile_get_next PROTOTYPE((krb5_keytab,
						krb5_keytab_entry *,
						krb5_kt_cursor *));
krb5_error_code krb5_ktfile_end_get PROTOTYPE((krb5_keytab,
					       krb5_kt_cursor *));
/* routines to be included on extended version (write routines) */
krb5_error_code krb5_ktfile_add PROTOTYPE((krb5_keytab,
					   krb5_keytab_entry *));
krb5_error_code krb5_ktfile_remove PROTOTYPE((krb5_keytab,
					      krb5_keytab_entry *));

krb5_error_code krb5_ktfileint_openr PROTOTYPE((krb5_keytab));
krb5_error_code krb5_ktfileint_openw PROTOTYPE((krb5_keytab));
krb5_error_code krb5_ktfileint_close PROTOTYPE((krb5_keytab));
krb5_error_code krb5_ktfileint_read_entry PROTOTYPE((krb5_keytab,
						     krb5_keytab_entry **));
krb5_error_code krb5_ktfileint_write_entry PROTOTYPE((krb5_keytab,
						      krb5_keytab_entry *));
/* and back to normal... */
#include <krb5/narrow.h>

#endif /* KRB5_KTFILE__ */
