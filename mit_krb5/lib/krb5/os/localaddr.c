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
 * $Log: localaddr.c,v $
 * Revision 1.1.2.1  1996/06/05  21:17:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:07:37  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/localaddr.c
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
 * Return the protocol addresses supported by this host.
 *
 * XNS support is untested, but "Should just work".
 */


#define NEED_SOCKETS
#include "k5-int.h"

#if !defined(HAVE_MACSOCK_H) && !defined(_MSDOS)

/* needed for solaris, harmless elsewhere... */
#define BSD_COMP
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>

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

krb5_error_code INTERFACE
krb5_os_localaddr(addr)
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
	closesocket (s);
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
			krb5_xfree(address);
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
			krb5_xfree(address);
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
	    krb5_xfree(addr_temp[i]);
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

#else /* Windows/Mac version */

/* No ioctls in winsock so we just assume there is only one networking 
 * card per machine, so gethostent is good enough. 
 */

krb5_error_code INTERFACE
krb5_os_localaddr (krb5_address ***addr) {
    char host[64];                              /* Name of local machine */
    struct hostent *hostrec;
    int err;

    *addr = calloc (2, sizeof (krb5_address *));
    if (*addr == NULL)
        return ENOMEM;

#ifdef HAVE_MACSOCK_H
    hostrec = getmyipaddr();
#else /* HAVE_MACSOCK_H */
    if (gethostname (host, sizeof(host))) {
        err = WSAGetLastError();
        return err;
    }

    hostrec = gethostbyname (host);
    if (hostrec == NULL) {
        err = WSAGetLastError();
        return err;
    }
#endif /* HAVE_MACSOCK_H */

    (*addr)[0] = calloc (1, sizeof(krb5_address));
    if ((*addr)[0] == NULL) {
        free (*addr);
        return ENOMEM;
    }
    (*addr)[0]->addrtype = hostrec->h_addrtype;
    (*addr)[0]->length = hostrec->h_length;
    (*addr)[0]->contents = (unsigned char *)malloc((*addr)[0]->length);
    if (!(*addr)[0]->contents) {
        free((*addr)[0]);
        free(*addr);
        return ENOMEM;
    } else {
        memcpy ((*addr)[0]->contents,
                hostrec->h_addr,
                (*addr)[0]->length);
    }
	/* FIXME, deal with the case where gethostent returns multiple addrs */

    return(0);
}
#endif
