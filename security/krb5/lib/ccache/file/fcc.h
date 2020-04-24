/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc.h,v $
 * Revision 1.1.8.1  1996/06/04  22:01:13  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1995/12/08  17:46:09  root  1.1.6.1]
 *
 * Revision 1.1.6.2  1996/02/18  23:03:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:51  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:46:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:48  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:20  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:51:27  sommerfeld]
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
 * HP CHANGES:
 * add mutex pointer for thread-safeness
 * add FCC_OPEN_FOR_ERASE
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc.h,v $
 * $Author: arvind $
 * $Id: fcc.h,v 1.1.8.1 1996/06/04 22:01:13 arvind Exp $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * This file contains constant and function declarations used in the
 * file-based credential cache routines.
 */


#ifndef __KRB5_FILE_CCACHE__
#define __KRB5_FILE_CCACHE__

#include <krb5/krb5.h>
#include <krb5/osconf.h>
#include <krb5/ext-proto.h>
#include "fcc-proto.h"
#include <krb5/sysincl.h>
#include <stdio.h>

#define KRB5_OK 0

#define KRB5_FCC_MAXLEN 100

/*
 * FCC version 2 contains type information for principals.  FCC
 * version 1 does not.
 *  
 * FCC version 3 contains keyblock encryption type information, and is
 * architecture independent.  Previous versions are not.
 *
 * The code will accept version 1, 2, and 3 ccaches, and depending 
 * what KRB5_FCC_DEFAULT_FVNO is set to, it will create version 1, 2,
 * or 3 FCC caches.
 * 
 * KRB5_FCC_DEFAULT_FVNO should be set to version 3, unless there is
 * some overriding compatibility reasons not to do so.
 */

#define KRB5_FCC_FVNO_1 0x0501		/* krb5 v5, fcc v1 */
#define KRB5_FCC_FVNO_2 0x0502		/* krb5 v5, fcc v2 */
#define KRB5_FCC_FVNO_3 0x0503		/* krb5 v5, fcc v3 */

#define KRB5_FCC_DEFAULT_FVNO KRB5_FCC_FVNO_3

#define	FCC_OPEN_AND_ERASE	1
#define	FCC_OPEN_RDWR		2
#define	FCC_OPEN_RDONLY		3
#define FCC_OPEN_FOR_ERASE      4

#ifndef TKT_ROOT
#define TKT_ROOT "/tmp/tkt"
#endif

/* macros to make checking flags easier */
#define OPENCLOSE(id) (((krb5_fcc_data *)id->data)->flags & KRB5_TC_OPENCLOSE)

typedef struct _krb5_fcc_data {
     char *filename;
     int fd;
     krb5_flags flags;
     int mode;				/* needed for locking code */
     int version;	      		/* version number of the file */
#ifdef OSF_DCE
     void *mutex;                       /* Needed for multi-thread */
#endif
} krb5_fcc_data;

/* An off_t can be arbitrarily complex */
typedef struct _krb5_fcc_cursor {
     off_t pos;
} krb5_fcc_cursor;

#define MAYBE_OPEN(ID, MODE) \
{									\
    if (OPENCLOSE (ID)) {						\
	krb5_error_code maybe_open_ret = krb5_fcc_open_file (ID,MODE);	\
	if (maybe_open_ret) return maybe_open_ret; } }

#define MAYBE_CLOSE(ID, RET) \
{									\
    if (OPENCLOSE (ID)) {						\
	krb5_error_code maybe_close_ret = krb5_fcc_close_file (ID);	\
	if (!(RET)) RET = maybe_close_ret; } }

#define MAYBE_CLOSE_IGNORE(ID) \
{									\
    if (OPENCLOSE (ID)) {						\
	(void) krb5_fcc_close_file (ID); } }

/* DO NOT ADD ANYTHING AFTER THIS #endif */
#endif /* __KRB5_FILE_CCACHE__ */
