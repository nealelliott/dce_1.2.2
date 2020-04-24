/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: locate_kdc.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:04:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:39  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  locate_kdc.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/locate_kdc.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Copyright Hewlett-Packard Company 1990, 1991
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 *
 * get socket addresses for KDC.
 */

#if !defined(lint) && !defined(SABER)
static char *VersionID = "@(#)locate_kdc.c	3 - 10/24/91";
static char rcsid_locate_kdc_c[] =
"$Id: locate_kdc.c,v 1.1.4.2 1996/02/18 00:15:30 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/osconf.h>

#include <krb5/ext-proto.h>
#include <stdio.h>
#include <krb5/los-proto.h>

#include <sys/types.h>
/*
 * OSF_DCE - cma....h will have already included socket.h put a guard here
 */
#ifndef SOCK_STREAM
#include <sys/socket.h>
#endif
#ifdef KRB5_USE_INET
#include <netinet/in.h>
#endif
#include <netdb.h>
#include "os-proto.h"

#ifdef KRB5_USE_INET
extern char *krb5_kdc_udp_portname;
#endif

/*
 * returns count of number of addresses found
 */

krb5_error_code
krb5_locate_kdc(realm, addr_pp, naddrs)
    const krb5_data *realm;
    struct sockaddr **addr_pp;
    int *naddrs;
{
    char **hostlist;
    int code;
    int i, j, out, count;
    struct sockaddr *addr_p;
    struct sockaddr_in *sin_p;
    struct hostent *hp;
    struct servent *sp;
#ifdef KRB5_USE_INET
    u_short udpport = 0;		/* 0 is an invalid UDP port #. */
#endif

    hostlist = 0;
    
    if (code = krb5_get_krbhst (realm, &hostlist))
	return(code);

#ifdef KRB5_USE_INET
    if (sp = getservbyname(krb5_kdc_udp_portname, "udp"))
	udpport = sp->s_port;
#endif

    for (i=0; hostlist[i]; i++)
	;
    count = i;
    
    if (count == 0) {
	*naddrs = 0;
	return 0;
    }

    addr_p = (struct sockaddr *)malloc (sizeof (struct sockaddr) * count);

    for (i=0, out=0; hostlist[i]; i++) {
	hp = gethostbyname(hostlist[i]);
	if (hp != 0) {
	    switch (hp->h_addrtype) {
#ifdef KRB5_USE_INET
	    case AF_INET:
		if (udpport)		/* must have gotten a port # */
		for (j=0; hp->h_addr_list[j]; j++) {
		    sin_p = (struct sockaddr_in *) &addr_p[out++];
		    memset ((char *)sin_p, 0, sizeof(struct sockaddr));
		    sin_p->sin_family = hp->h_addrtype;
		    sin_p->sin_port = udpport;
		    memcpy((char *)&sin_p->sin_addr,
			   (char *)hp->h_addr_list[j],
			   sizeof(struct in_addr));
		    if (out >= count) {
			count *= 2;
			addr_p = (struct sockaddr *)
			    realloc ((char *)addr_p,
				     sizeof(struct sockaddr) * count);
		    }
		}
		break;
#endif
	    default:
		break;
	    }
	}
	free(hostlist[i]);
	hostlist[i] = 0;
    }
    free ((char *)hostlist);
				/*
				 * XXX need to distinguish between
				 * "can't resolve KDC name" and
				 * "can't find any KDC names"
				 */
    *addr_pp = addr_p;
    *naddrs = out;
    return 0;
}
