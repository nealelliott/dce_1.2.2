/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktfile.h,v $
 * Revision 1.1.6.2  1996/02/18  23:03:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:56:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:25  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:02:21  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:58:18  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/keytab/file/ktfile.h,v $
 * $Author: marty $
 * $Id: ktfile.h,v 1.1.6.2 1996/02/18 23:03:28 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * This header file contains information needed by internal routines
 * of the file-based ticket cache implementation.
 */


#ifndef KRB5_KTFILE__
#define KRB5_KTFILE__

#include <stdio.h>

/*
 * Constants
 */
#define IGNORE_VNO 0

#define KRB5_KT_VNO_1	0x0501	/* krb v5, keytab version 1 (DCE compat) */
#define KRB5_KT_VNO	0x0502	/* krb v5, keytab version 2 (standard)  */

#define KRB5_KT_DEFAULT_VNO KRB5_KT_VNO

/* 
 * Types
 */
typedef struct _krb5_ktfile_data {
    char *name;			/* Name of the file */
    FILE *openf;		/* open file, if any. */
    int	version;		/* Version number of keytab */
} krb5_ktfile_data;

/*
 * Macros
 */
#define KTPRIVATE(id) ((krb5_ktfile_data *)(id)->data)
#define KTFILENAME(id) (((krb5_ktfile_data *)(id)->data)->name)
#define KTFILEP(id) (((krb5_ktfile_data *)(id)->data)->openf)
#define KTVERSION(id) (((krb5_ktfile_data *)(id)->data)->version)

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
krb5_error_code krb5_ktfileint_delete_entry PROTOTYPE((krb5_keytab,
                                                      krb5_int32));
krb5_error_code krb5_ktfileint_internal_read_entry PROTOTYPE((krb5_keytab,
						     krb5_keytab_entry **,
                                                     krb5_int32 *));
krb5_error_code krb5_ktfileint_size_entry PROTOTYPE((krb5_keytab_entry *,
                                                      krb5_int32 *));
krb5_error_code krb5_ktfileint_find_slot PROTOTYPE((krb5_keytab,
                                                      krb5_int32 *,
                                                      krb5_int32 *));
/* and back to normal... */
#include <krb5/narrow.h>

#endif /* KRB5_KTFILE__ */
