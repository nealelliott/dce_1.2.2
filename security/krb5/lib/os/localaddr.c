/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: localaddr.c,v $
 * Revision 1.1.9.2  1996/02/18  00:15:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:49  marty]
 *
 * Revision 1.1.9.1  1995/12/08  18:04:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:38  root]
 * 
 * Revision 1.1.7.2  1993/06/24  20:19:57  hinman
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:26:56  hinman]
 * 
 * Revision 1.1.3.3  1992/12/29  16:30:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:02  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:52  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:34:42  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  localaddr.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/localaddr.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Copyright Hewlett-Packard Company 1990, 1991
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 *
 * Return the protocol addresses supported by this host.
 *
 * XNS support is untested, but "Should just work".
 */
#ifdef SNI_SVR4_TAINTED
#endif /* SNI_SVR4_TAINTED */

#if !defined(lint) && !defined(SABER)
static char *VersionID = "@(#)localaddr.c	3 - 10/24/91";
static char rcsid_getaddr_c[] =
"$Id: localaddr.c,v 1.1.9.2 1996/02/18 00:15:29 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/osconf.h>

#include <krb5/ext-proto.h>

#include <sys/ioctl.h>
/*
 * OSF_DCE - cma....h will have already included socket.h put a guard here
 */
#ifndef SOCK_STREAM
#include <sys/socket.h>
#endif
#include <net/if.h>
#include <sys/errno.h>

/*
 * The SIOCGIF* ioctls require a socket.
 * It doesn't matter *what* kind of socket they use, but it has to be
 * a socket.
 *
 * Of course, you can't just ask the kernel for a socket of arbitrary
 * type; you have to ask for one with a valid type.
 *
 */
#ifdef KRB5_USE_INET

#include <netinet/in.h>

#ifndef USE_AF
#define USE_AF AF_INET
#define USE_TYPE SOCK_DGRAM
#define USE_PROTO 0
#endif

#endif

#ifdef KRB5_USE_NS

#include <netns/ns.h>

#ifndef USE_AF
#define USE_AF AF_NS
#define USE_TYPE SOCK_DGRAM
#define USE_PROTO 0		/* guess */
#endif

#endif
/*
 * Add more address families here.
 */

extern int errno;

#ifdef SNI_SVR4_TAINTED
#else /* SNI_SVR4_TAINTED */

/*
 * Return all the protocol addresses of this host.
 *
 * We could kludge up something to return all addresses, assuming that
 * they're valid kerberos protocol addresses, but we wouldn't know the
 * real size of the sockaddr or know which part of it was actually the
 * host part.
 *
 * This uses the SIOCGIFCONF, SIOCGIFFLAGS, and SIOCGIFADDR ioctl's.
 */

krb5_error_code krb5_os_localaddr(addr)
    krb5_address ***addr;
{
    struct ifreq *ifr;
    struct ifconf ifc;
    int s, code, n, i;
    char buf[1024];
    krb5_address *addr_temp [ 1024/sizeof(struct ifreq) ];
    int n_found;
    int mem_err = 0;
    
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;

    s = socket (USE_AF, USE_TYPE, USE_PROTO);
    if (s < 0)
	return errno;

    code = ioctl (s, SIOCGIFCONF, (char *)&ifc);
    if (code < 0) {
	int retval = errno;
	close(s);
	return retval;
    }
    n = ifc.ifc_len / sizeof (struct ifreq);
    
    for (n_found=0, i=0; i<n && ! mem_err; i++) {
	krb5_address *address;
	ifr = &ifc.ifc_req[i];
	
	if (ioctl (s, SIOCGIFFLAGS, (char *)ifr) < 0)
	    continue;

#ifdef IFF_LOOPBACK
	if (ifr->ifr_flags & IFF_LOOPBACK) 
	    continue;
#endif

	if (!(ifr->ifr_flags & IFF_UP)) 
	    /* interface is down; skip */
	    continue;

	if (ioctl (s, SIOCGIFADDR, (char *)ifr) < 0)
	    /* can't get address */
	    continue;

	/* ifr->ifr_addr has what we want! */
	switch (ifr->ifr_addr.sa_family) {
#ifdef KRB5_USE_INET
	case AF_INET:
	    {
		struct sockaddr_in *in =
		    (struct sockaddr_in *)&ifr->ifr_addr;
		
		address = (krb5_address *)
		    malloc (sizeof(krb5_address));
		if (address) {
		    address->addrtype = ADDRTYPE_INET;
		    address->length = sizeof(struct in_addr);
		    address->contents = (unsigned char *)malloc(address->length);
		    if (!address->contents) {
			free((char *)address);
			address = 0;
			mem_err++;
		    } else {
			memcpy ((char *)address->contents,
				(char *)&in->sin_addr, 
				address->length);
			break;
		    }
		} else mem_err++;
	    }
#endif
#ifdef KRB5_USE_NS
	    case AF_XNS:
	    {  
		struct sockaddr_ns *ns =
		    (struct sockaddr_ns *)&ifr->ifr_addr;		    
		address = (krb5_address *)
		    malloc (sizeof (krb5_address) + sizeof (struct ns_addr));
		if (address) {
		    address->addrtype = ADDRTYPE_XNS; 

		    /* XXX should we perhaps use ns_host instead? */

		    address->length = sizeof(struct ns_addr);
		    address->contents = (unsigned char *)malloc(address->length);
		    if (!address->contents) {
			free((char *)address);
			address = 0;
			mem_err++;
		    } else {
			memcpy ((char *)address->contents,
				(char *)&ns->sns_addr,
				address->length);
			break;
		    }
		} else mem_err++;
		break;
	    }
#endif
	/*
	 * Add more address families here..
	 */
	default:
	    continue;
	}
	if (address)
	    addr_temp[n_found++] = address;
	address = 0;
    }
    close(s);

    *addr = (krb5_address **)malloc (sizeof (krb5_address *) * (n_found+1));
    if (*addr == 0)
	mem_err++;
    
    if (mem_err) {
	for (i=0; i<n_found; i++) {
	    free((char *)addr_temp[i]);
	    addr_temp[i] = 0;
	}
	return ENOMEM;
    }
    
    for (i=0; i<n_found; i++) {
	(*addr)[i] = addr_temp[i];
    }
    (*addr)[n_found] = 0;
    return 0;
}

#endif /* SNI_SVR4_TAINTED */
