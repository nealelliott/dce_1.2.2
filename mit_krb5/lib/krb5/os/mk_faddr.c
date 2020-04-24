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
 * $Log: mk_faddr.c,v $
 * Revision 1.1.2.1  1996/06/05  21:18:11  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:08:29  arvind]
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
 * $Log: mk_faddr.c,v $
 * Revision 1.1.2.1  1996/06/05  21:18:11  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:08:29  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:08:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:19 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:44 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2  1996/05/17  14:18 UTC  mullan_s
 * 	Remove #ifdef OSF_DCE - just a bug fix, not an enhancement.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:07 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/os/full_ipadr.c
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
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
 * Take an IP addr & port and generate a full IP address.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#define NEED_SOCKETS
#include "k5-int.h"

#ifdef KRB5_USE_INET
   
#include "os-proto.h"
#ifndef _WINSOCKAPI_
#include <netinet/in.h>
#endif

krb5_error_code
krb5_make_fulladdr(context, kaddr, kport, raddr)
    krb5_context 	  context;
    krb5_address	* kaddr;
    krb5_address	* kport;
    krb5_address        * raddr;
{
    register krb5_octet * marshal;
    krb5_int32 tmp32;
    krb5_int16 tmp16;

    if ((kport == NULL) || (kport == NULL)) 
	return EINVAL;

    raddr->length = kaddr->length + kport->length + (4 * sizeof(krb5_int32));
    if (!(raddr->contents = (krb5_octet *)malloc(raddr->length)))
	return ENOMEM;

    raddr->addrtype = ADDRTYPE_ADDRPORT;
    marshal = raddr->contents;

    tmp16 = kaddr->addrtype;
    *marshal++ = 0x00;
    *marshal++ = 0x00;
    *marshal++ = (krb5_octet) (tmp16 & 0xff);
    *marshal++ = (krb5_octet) ((tmp16 >> 8) & 0xff);

    tmp32 = kaddr->length;
    *marshal++ = (krb5_octet) (tmp32 & 0xff);
    *marshal++ = (krb5_octet) ((tmp32 >> 8) & 0xff);
    *marshal++ = (krb5_octet) ((tmp32 >> 16) & 0xff);
    *marshal++ = (krb5_octet) ((tmp32 >> 24) & 0xff);

    (void) memcpy((char *)marshal, (char *)(kaddr->contents), kaddr->length);
    marshal += kaddr->length;

    tmp16 = kport->addrtype;
    *marshal++ = 0x00;
    *marshal++ = 0x00;
    *marshal++ = (krb5_octet) (tmp16 & 0xff);
    *marshal++ = (krb5_octet) ((tmp16 >> 8) & 0xff);

    tmp32 = kport->length;
    *marshal++ = (krb5_octet) (tmp32 & 0xff);
    *marshal++ = (krb5_octet) ((tmp32 >> 8) & 0xff);
    *marshal++ = (krb5_octet) ((tmp32 >> 16) & 0xff);
    *marshal++ = (krb5_octet) ((tmp32 >> 24) & 0xff);

    (void) memcpy((char *)marshal, (char *)(kport->contents), kport->length);
    marshal += kport->length;
    return 0;
}
#endif
