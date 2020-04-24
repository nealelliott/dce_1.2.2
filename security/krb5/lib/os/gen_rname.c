/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gen_rname.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:33  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  gen_rname.c V=1 06/06/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/gen_rname.c,v $
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
 * take a port-style address and unique string, and return
 * a replay cache tag string.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gen_rname_c[] =
"$Id: gen_rname.c,v 1.1.4.2 1996/02/18 00:15:19 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/osconf.h>

#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include "os-proto.h"
#ifdef KRB5_USE_INET
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

krb5_error_code
krb5_gen_replay_name(address, uniq, string)
const krb5_address *address;
const char *uniq;
char **string;
{
#ifdef KRB5_USE_INET
    krb5_int16 port;
    krb5_int32 addr;
    register krb5_error_code retval;
    register char *tmp, *tmp2;
    struct in_addr inaddr;

    if (retval = krb5_unpack_full_ipaddr(address, &addr, &port))
	return retval;
    inaddr.s_addr = addr;

    tmp = inet_ntoa(inaddr);
    tmp2 = malloc(strlen(uniq)+strlen(tmp)+1+1+5); /* 1 for NUL,
						      1 for ,,
						      5 for digits (65535 is max) */
    if (!tmp2)
	return ENOMEM;
    (void) sprintf(tmp2, "%s%s,%u",uniq,tmp,ntohs(port));
    *string = tmp2;
    return 0;
#else
    return KRB5_PROG_ATYPE_NOSUPP;
#endif
}
