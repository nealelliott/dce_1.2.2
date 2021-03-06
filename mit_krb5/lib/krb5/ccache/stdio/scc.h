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
 * $Log: scc.h,v $
 * Revision 1.1.2.1  1996/06/05  20:42:36  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:43:52  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/ccache/stdio/scc.h
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

#include "k5-int.h"
#include "scc-proto.h"
#include <stdio.h>

#define KRB5_OK 0

#define KRB5_SCC_MAXLEN 100

/*
 * SCC version 2 contains type information for principals.  SCC
 * version 1 does not.  The code will accept either, and depending on
 * what KRB5_SCC_DEFAULT_FVNO is set to, it will create version 1 or
 * version 2 SCC caches.
 *
 * KRB5_SCC_DEFAULT_FVNO should be set to version 2, unless there is
 * some overriding compatibility reasons not to do so.
 */

#define KRB5_SCC_FVNO_1 0x0501		/* krb v5, scc v1 */
#define KRB5_SCC_FVNO_2   0x0502	/* krb v5, scc v2 */
#define KRB5_SCC_FVNO_3   0x0503	/* krb v5, scc v2 */

#define KRB5_SCC_DEFAULT_FVNO KRB5_SCC_FVNO_3

#define	SCC_OPEN_AND_ERASE	1
#define	SCC_OPEN_RDWR		2
#define	SCC_OPEN_RDONLY		3

#ifndef TKT_ROOT
#define TKT_ROOT "/tmp/tkt"
#endif

/* macros to make checking flags easier */
#define OPENCLOSE(id) (((krb5_scc_data *)id->data)->flags & KRB5_TC_OPENCLOSE)

typedef struct _krb5_scc_data {
     char *filename;
     FILE *file;
     krb5_flags flags;
     char stdio_buffer[BUFSIZ];
     int version;
} krb5_scc_data;

/* An off_t can be arbitrarily complex */
typedef struct _krb5_scc_cursor {
    long pos;
} krb5_scc_cursor;

#define MAYBE_OPEN(context, ID, MODE) \
{									\
    if (OPENCLOSE (ID)) {						\
	krb5_error_code maybe_open_ret = krb5_scc_open_file (context, ID,MODE);	\
	if (maybe_open_ret) return maybe_open_ret; } }

#define MAYBE_CLOSE(context, ID, RET) \
{									\
    if (OPENCLOSE (ID)) {						\
	krb5_error_code maybe_close_ret = krb5_scc_close_file (context, ID);	\
	if (!(RET)) RET = maybe_close_ret; } }

/* DO NOT ADD ANYTHING AFTER THIS #endif */
#endif /* __KRB5_FILE_CCACHE__ */
