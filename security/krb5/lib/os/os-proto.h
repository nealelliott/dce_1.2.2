/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: os-proto.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:36  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  os-proto.h V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/os-proto.h,v $
 * $Author: marty $
 * $Id: os-proto.h,v 1.1.4.2 1996/02/18 23:03:31 marty Exp $
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
 * LIBOS internal function prototypes.
 */

#ifndef KRB5_LIBOS_INT_PROTO__
#define KRB5_LIBOS_INT_PROTO__

#ifdef SOCK_DGRAM			/* XXX hack... */
krb5_error_code krb5_locate_kdc
    PROTOTYPE((const krb5_data *,
	       struct sockaddr **,
	       int *));
#endif

#ifdef KRB5_USE_INET
krb5_error_code krb5_unpack_full_ipaddr
    PROTOTYPE((const krb5_address *,
	       krb5_int32 *,
	       krb5_int16 *));

#ifdef NARROW_PROTOTYPES
krb5_error_code krb5_make_full_ipaddr
    PROTOTYPE((krb5_int32,
	       krb5_int16,
	       krb5_address **));
#else
krb5_error_code krb5_make_full_ipaddr
    PROTOTYPE((krb5_int32,
	       int,			/* unsigned short promotes to signed
					   int */
	       krb5_address **));
#endif /* NARROW_PROTOTYPES */
#endif /* KRB5_USE_INET */

#endif /* KRB5_LIBOS_INT_PROTO__ */
