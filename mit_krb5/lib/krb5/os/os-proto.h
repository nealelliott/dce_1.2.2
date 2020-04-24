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
 * $Log: os-proto.h,v $
 * Revision 1.1.2.1  1996/06/05  21:18:43  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:09:00  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/os-proto.h
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
 * LIBOS internal function prototypes.
 */

#ifndef KRB5_LIBOS_INT_PROTO__
#define KRB5_LIBOS_INT_PROTO__

#ifdef SOCK_DGRAM			/* XXX hack... */
krb5_error_code krb5_locate_kdc
    PROTOTYPE((krb5_context,
	       const krb5_data *,
	       struct sockaddr **,
	       int *));
#endif

#ifdef KRB5_USE_INET
krb5_error_code krb5_unpack_full_ipaddr
    PROTOTYPE((krb5_context,
	       const krb5_address *,
	       krb5_int32 *,
	       krb5_int16 *));

krb5_error_code krb5_make_full_ipaddr
    PROTOTYPE((krb5_context,
	       krb5_int32,
	       int,			/* unsigned short promotes to signed
					   int */
	       krb5_address **));

krb5_error_code krb5_make_fulladdr
    PROTOTYPE((krb5_context,
	       krb5_address *,
	       krb5_address *,
	       krb5_address *));

#endif /* KRB5_USE_INET */

#endif /* KRB5_LIBOS_INT_PROTO__ */
