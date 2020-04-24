/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: port2ip.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:37  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  port2ip.c V=1 06/06/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/port2ip.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Take an ADDRPORT address and split into IP addr & port.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_port2ip_c[] =
"$Id: port2ip.c,v 1.1.4.2 1996/02/18 00:15:41 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/osconf.h>

#ifdef KRB5_USE_INET

#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include "os-proto.h"
#include <netinet/in.h>

krb5_error_code
krb5_unpack_full_ipaddr(inaddr, adr, port)
const krb5_address *inaddr;
krb5_int32 *adr;
krb5_int16 *port;
{
    unsigned long smushaddr;
    unsigned short smushport;
    register krb5_octet *marshal;
    krb5_addrtype temptype;
    krb5_int32 templength;

    if (inaddr->addrtype != ADDRTYPE_ADDRPORT)
	return KRB5_PROG_ATYPE_NOSUPP;

    if (inaddr->length != sizeof(smushaddr)+ sizeof(smushport) +
	2*sizeof(temptype) + 2*sizeof(templength))
	return KRB5_PROG_ATYPE_NOSUPP;

    marshal = inaddr->contents;

    (void) memcpy((char *)&temptype, (char *)marshal, sizeof(temptype));
    marshal += sizeof(temptype);
    if (temptype != htons(ADDRTYPE_INET))
	return KRB5_PROG_ATYPE_NOSUPP;

    (void) memcpy((char *)&templength, (char *)marshal, sizeof(templength));
    marshal += sizeof(templength);
    if (templength != htonl(sizeof(smushaddr)))
	return KRB5_PROG_ATYPE_NOSUPP;

    (void) memcpy((char *)&smushaddr, (char *)marshal, sizeof(smushaddr));
    /* leave in net order */
    marshal += sizeof(smushaddr);

    (void) memcpy((char *)&temptype, (char *)marshal, sizeof(temptype));
    marshal += sizeof(temptype);
    if (temptype != htons(ADDRTYPE_IPPORT))
	return KRB5_PROG_ATYPE_NOSUPP;

    (void) memcpy((char *)&templength, (char *)marshal, sizeof(templength));
    marshal += sizeof(templength);
    if (templength != htonl(sizeof(smushport)))
	return KRB5_PROG_ATYPE_NOSUPP;

    (void) memcpy((char *)&smushport, (char *)marshal, sizeof(smushport));
    /* leave in net order */

    *adr = (krb5_int32) smushaddr;
    *port = (krb5_int16) smushport;
    return 0;
}
#endif
