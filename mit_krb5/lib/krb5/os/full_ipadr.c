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
 * $Log: full_ipadr.c,v $
 * Revision 1.1.2.1  1996/06/05  21:09:28  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:05:34  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/full_ipadr.c
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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

#define NEED_SOCKETS
#include "k5-int.h"

#ifdef KRB5_USE_INET

#include "os-proto.h"

krb5_error_code
krb5_make_full_ipaddr(context, adr, port, outaddr)
    krb5_context context;
    krb5_int32 adr;
    krb5_int16 port;
    krb5_address ** outaddr;
{
    unsigned long smushaddr = (unsigned long) adr; /* already in net order */
    unsigned short smushport = (unsigned short) port; /* ditto */
    register krb5_address *retaddr;
    register krb5_octet *marshal;
    krb5_addrtype temptype;
    krb5_int32 templength;

    if (!(retaddr = (krb5_address *)malloc(sizeof(*retaddr)))) {
	return ENOMEM;
    }
    retaddr->addrtype = ADDRTYPE_ADDRPORT;
    retaddr->length = sizeof(smushaddr)+ sizeof(smushport) +
	2*sizeof(temptype) + 2*sizeof(templength);

    if (!(retaddr->contents = (krb5_octet *)malloc(retaddr->length))) {
	krb5_xfree(retaddr);
	return ENOMEM;
    }
    marshal = retaddr->contents;

    temptype = htons(ADDRTYPE_INET);
    (void) memcpy((char *)marshal, (char *)&temptype, sizeof(temptype));
    marshal += sizeof(temptype);

    templength = htonl(sizeof(smushaddr));
    (void) memcpy((char *)marshal, (char *)&templength, sizeof(templength));
    marshal += sizeof(templength);

    (void) memcpy((char *)marshal, (char *)&smushaddr, sizeof(smushaddr));
    marshal += sizeof(smushaddr);

    temptype = htons(ADDRTYPE_IPPORT);
    (void) memcpy((char *)marshal, (char *)&temptype, sizeof(temptype));
    marshal += sizeof(temptype);

    templength = htonl(sizeof(smushport));
    (void) memcpy((char *)marshal, (char *)&templength, sizeof(templength));
    marshal += sizeof(templength);

    (void) memcpy((char *)marshal, (char *)&smushport, sizeof(smushport));
    marshal += sizeof(smushport);

    *outaddr = retaddr;
    return 0;
}
#endif
