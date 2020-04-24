/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gen_port.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:33  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:01:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  gen_port.c V=1 06/06/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/gen_port.c,v $
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
 * Take an IP addr & port and generate a full IP address.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gen_port_c[] =
"$Id: gen_port.c,v 1.1.4.2 1996/02/18 00:15:18 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/osconf.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include "os-proto.h"

krb5_error_code
krb5_gen_portaddr(addr, ptr, outaddr)
const krb5_address *addr;
krb5_const_pointer ptr;
krb5_address **outaddr;
{
#ifdef KRB5_USE_INET
    krb5_int32 adr;
    krb5_int16 port;

    if (addr->addrtype != ADDRTYPE_INET)
	return KRB5_PROG_ATYPE_NOSUPP;
    port = *(krb5_int16 *)ptr;
    
    memcpy((char *)&adr, (char *)addr->contents, sizeof(adr));
    return krb5_make_full_ipaddr(adr, port, outaddr);
#else
    return KRB5_PROG_ATYPE_NOSUPP;
#endif
}
