/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hostaddr.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:06  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:59:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  hostaddr.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/hostaddr.h,v $
 * $Author: marty $
 * $Id: hostaddr.h,v 1.1.4.2 1996/02/18 23:02:32 marty Exp $
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
 * hostaddr definitions for Kerberos version 5.
 */

#ifndef KRB5_HOSTADDR__
#define KRB5_HOSTADDR__

/* structure for address */
typedef struct _krb5_address {
    krb5_addrtype addrtype;
    int length;
    krb5_octet *contents;
} krb5_address;

/* per Kerberos v5 protocol spec */
#define ADDRTYPE_UNSPECIFIED 0
#define	ADDRTYPE_INET	0x0002
#define	ADDRTYPE_CHAOS	0x0005
#define	ADDRTYPE_XNS	0x0006
#define	ADDRTYPE_ISO	0x0007
#define ADDRTYPE_DDP	0x0010
/* not yet in the spec... */
#define ADDRTYPE_ADDRPORT	0x0100
#define ADDRTYPE_IPPORT		0x0101

/* DCE addresses - represented as a string binding */
#define ADDRTYPE_DCE    0x8001

/* macros to determine if a type is a local type */
#define ADDRTYPE_IS_LOCAL(addrtype) (addrtype & 0x8000)

/* implementation-specific stuff: */
typedef struct _krb5_fulladdr {
    krb5_address *address;
    unsigned long port;			/* port, for some address types.
					   large enough for most protos? */
} krb5_fulladdr;

#endif /* KRB5_HOSTADDR__ */


