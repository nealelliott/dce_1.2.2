/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: fcc.h,v $
 * Revision 1.1.2.1  1996/06/05  20:37:39  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:40:29  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc.h,v $
 * Revision 1.1.2.1  1996/06/05  20:37:39  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:40:29  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  19:40:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:18 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:40 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2  1996/05/24  20:25 UTC  mullan_s
 * 	Set default ccache vno to 3 because DCE 1.2.2 now supports it.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:06 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/ccache/file/fcc.h
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
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#ifndef __KRB5_FILE_CCACHE__
#define __KRB5_FILE_CCACHE__

#define NEED_LOWLEVEL_IO
#include "k5-int.h"
#include "fcc-proto.h"
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

/*
 * NOTE: DCE 1.2.2 supports ccache versions 1-3. DCE 1.1 only
 * supports version 1-2. In order to be compatible with DCE 1.1,
 * this needs to be changed to KRB5_FCC_FVNO_2.
 */
#define KRB5_FCC_DEFAULT_FVNO KRB5_FCC_FVNO_3

#define	FCC_OPEN_AND_ERASE	1
#define	FCC_OPEN_RDWR		2
#define	FCC_OPEN_RDONLY		3

#ifndef TKT_ROOT
#ifdef MSDOS_FILESYSTEM
#define TKT_ROOT "\\tkt"
#else
#define TKT_ROOT "/tmp/tkt"
#endif
#endif

/* macros to make checking flags easier */
#define OPENCLOSE(id) (((krb5_fcc_data *)id->data)->flags & KRB5_TC_OPENCLOSE)

typedef struct _krb5_fcc_data {
     char *filename;
     int fd;
     krb5_flags flags;
     int mode;				/* needed for locking code */
     int version;	      		/* version number of the file */
} krb5_fcc_data;

/* An off_t can be arbitrarily complex */
typedef struct _krb5_fcc_cursor {
     off_t pos;
} krb5_fcc_cursor;

#define MAYBE_OPEN(CONTEXT, ID, MODE) \
{									\
    if (OPENCLOSE (ID)) {						\
	krb5_error_code maybe_open_ret = krb5_fcc_open_file (CONTEXT,ID,MODE);	\
	if (maybe_open_ret) return maybe_open_ret; } }

#define MAYBE_CLOSE(CONTEXT, ID, RET) \
{									\
    if (OPENCLOSE (ID)) {						\
	krb5_error_code maybe_close_ret = krb5_fcc_close_file (CONTEXT,ID);	\
	if (!(RET)) RET = maybe_close_ret; } }

#define MAYBE_CLOSE_IGNORE(CONTEXT, ID) \
{									\
    if (OPENCLOSE (ID)) {						\
	(void) krb5_fcc_close_file (CONTEXT,ID); } }

/* DO NOT ADD ANYTHING AFTER THIS #endif */
#endif /* __KRB5_FILE_CCACHE__ */
