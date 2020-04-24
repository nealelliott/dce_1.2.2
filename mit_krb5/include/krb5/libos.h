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
 * $Log: libos.h,v $
 * Revision 1.1.2.1  1996/06/05  20:27:58  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:27:30  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5/libos.h
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
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
 * Definitions for this implementation of the libos layer.
 */


#ifndef KRB5_LIBOS__
#define KRB5_LIBOS__

typedef struct _krb5_os_context {
	krb5_magic	magic;
} *krb5_os_context;

/* lock mode flags */
#define	KRB5_LOCKMODE_SHARED	0x0001
#define	KRB5_LOCKMODE_EXCLUSIVE	0x0002
#define	KRB5_LOCKMODE_DONTBLOCK	0x0004
#define	KRB5_LOCKMODE_UNLOCK	0x0008

#endif /* KRB5_LIBOS__ */
