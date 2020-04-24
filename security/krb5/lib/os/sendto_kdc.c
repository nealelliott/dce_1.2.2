/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sendto_kdc.c,v $
 * Revision 1.1.8.1  1996/10/15  20:53:05  arvind
 * 	In krb5_sendto_kdc(), if sec_krb5rpc_sendto_kdc() returns an error
 * 	and the btm_flag is set, don't continue looking for another KDC.
 * 	[1996/03/28  16:03 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:15:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:05  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:44:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/09  21:37 UTC  sommerfeld
 * 	fix one last unresolved symbol.
 * 	[1995/09/18  15:25 UTC  sommerfeld  /main/sommerfeld_dfsperf/2]
 * 
 * 	CHFts16269: Merge this source file with krb5b5 (sort of).
 * 	[1995/09/08  22:36 UTC  sommerfeld  /main/sommerfeld_dfsperf/1]
 * 
 * Revision 1.1.3.3  1992/12/29  16:31:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:52  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:43:07  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:35:28  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sendto_kdc.c V=4 10/24/91 //littl/prgy/krb5/lib/os
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/sendto_kdc.c,v $
 * $Author: arvind $
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
 * Send packet to KDC for realm; wait for response, retransmitting
 * as necessary.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_sendto_kdc_c[] =
"$Id: sendto_kdc.c,v 1.1.8.1 1996/10/15 20:53:05 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <krb5/los-proto.h>
#include "os-proto.h"

#define INVALID_SOCKET -1
#define SOCKET int
#define SOCKET_ERROR -1
#define SOCKET_ERRNO errno
#define SOCKET_EINTR EINTR
#define SOCKET_NFDS(fd) (fd+1)

/*
 * send the formatted request 'message' to a KDC for realm 'realm' and
 * return the response (if any) in 'reply'.
 *
 * If the message is sent and a response is received, 0 is returned,
 * otherwise an error code is returned.
 *
 * The storage for 'reply' is allocated and should be freed by the caller
 * when finished.
 *
 */


extern int krb5_max_dgram_size;
extern int krb5_max_skdc_timeout;
extern int krb5_skdc_timeout_shift;
extern int krb5_skdc_timeout_1;

#ifdef OSF_DCE
/* Global bind_to_master flag set in sec_login_pvt.c */
extern unsigned int btm_flag;
#endif

krb5_error_code
krb5_sendto_kdc (DECLARG(const krb5_data *, message),
		 DECLARG(const krb5_data *, realm),
		 DECLARG(krb5_data *, reply))
OLDDECLARG(const krb5_data *, message)
OLDDECLARG(const krb5_data *, realm)
OLDDECLARG(krb5_data *, reply)
{
    register int timeout, host, i;
    struct sockaddr *addr = NULL;
    int naddr;
    int sent, nready;
    krb5_error_code retval;
    SOCKET *socklist = NULL;		/* one for each, if necessary! */
    fd_set readable;
    struct timeval waitlen;
    struct sockaddr fromaddr;
    int fromlen, cc;

    if (!(reply->data = malloc(krb5_max_dgram_size))) {
	return ENOMEM;
    }
    reply->length = krb5_max_dgram_size;

    sec_krb5rpc_sendto_kdc(realm, message->length, message->data, 
                            reply->length, &reply->length, reply->data, 
                            &retval);
#ifdef OSF_DCE
    /*
     * If the previous call succeeded, return, or if the previous call 
     * failed, and we need to bind to the master, return.
     */
    if ((retval == 0) || ((retval != 0) && (btm_flag > 0))) {
        return retval;
    }
#else
    if (retval == 0) {
        return retval;
    }
#endif

    reply->length = krb5_max_dgram_size;

    /*
     * find KDC location(s) for realm
     */

    if (retval = krb5_locate_kdc (realm, &addr, &naddr)) 
    {
	addr = NULL;
	goto out;
    }
    if (naddr == 0) {
	retval = KRB5_REALM_UNKNOWN;
	goto out;
    }
    
    socklist = (SOCKET *)malloc(naddr * sizeof(SOCKET));
    if (socklist == NULL) {
	retval = ENOMEM;
	goto out;
    }
    for (i = 0; i < naddr; i++)
	socklist[i] = INVALID_SOCKET;

    /*
     * do exponential backoff.
     */

    for (timeout = krb5_skdc_timeout_1; timeout < krb5_max_skdc_timeout;
	 timeout <<= krb5_skdc_timeout_shift) {
	sent = 0;
	for (host = 0; host < naddr; host++) {
	    /* send to the host, wait timeout seconds for a response,
	       then move on. */
	    /* cache some sockets for each host */
	    if (socklist[host] == INVALID_SOCKET) {
		/* XXX 4.2/4.3BSD has PF_xxx = AF_xxx, so the socket
		   creation here will work properly... */
		/*
		 * From socket(2):
		 *
		 * The protocol specifies a particular protocol to be
		 * used with the socket.  Normally only a single
		 * protocol exists to support a particular socket type
		 * within a given protocol family.
		 */
		socklist[host] = socket(addr[host].sa_family, SOCK_DGRAM, 0);
		if (socklist[host] == INVALID_SOCKET)
		    continue;		/* try other hosts */
		/* have a socket to send/recv from */
		/* On BSD systems, a connected UDP socket will get connection
		   refused and net unreachable errors while an unconnected
		   socket will time out, so use connect, send, recv instead of
		   sendto, recvfrom.  The connect here may return an error if
		   the destination host is known to be unreachable. */
		if (connect(socklist[host],
			    &addr[host], sizeof(addr[host])) == SOCKET_ERROR)
		  continue;
	    }
	    if (send(socklist[host],
		       message->data, message->length, 0) != message->length)
	      continue;
	retry:
	    waitlen.tv_usec = 0;
	    waitlen.tv_sec = timeout;
	    FD_ZERO(&readable);
	    FD_SET(socklist[host], &readable);
	    if (nready = select(SOCKET_NFDS(socklist[host]),
				(int *)&readable,
				0,
				0,
				&waitlen)) {
		if (nready == SOCKET_ERROR) {
		    if (SOCKET_ERRNO == SOCKET_EINTR)
			goto retry;
		    retval = SOCKET_ERRNO;
		    goto out;
		}
		if ((cc = recv(socklist[host],
			       reply->data, reply->length, 0)) == SOCKET_ERROR)
		  {
		    /* man page says error could be:
		       EBADF: won't happen
		       ENOTSOCK: it's a socket.
		       EWOULDBLOCK: not marked non-blocking, and we selected.
		       EINTR: could happen
		       EFAULT: we allocated the reply packet.

		       In addition, net related errors like ECONNREFUSED
		       are possble (but undocumented).  Assume anything
		       other than EINTR is a permanent error for the
		       server (i.e. don't set sent = 1).
		       */

		    if (SOCKET_ERRNO == SOCKET_EINTR)
		      sent = 1;
		    continue;
		  }

		/* We might consider here verifying that the reply
		   came from one of the KDC's listed for that address type,
		   but that check can be fouled by some implementations of
		   some network types which might show a loopback return
		   address, for example, if the KDC is on the same host
		   as the client. */

		reply->length = cc;
		retval = 0;
		goto out;
	    } else if (nready == 0) {
		/* timeout */
	        sent = 1;
	    }
	    /* not ready, go on to next server */
	}
	if (!sent) {
	    /* never were able to send to any servers; give up */
	    retval = KRB5_KDC_UNREACH;
	    break;
	}
    }
    retval = KRB5_KDC_UNREACH;
 out:
    if (socklist) 
    {
	for (i = 0; i < naddr; i++)
	    if (socklist[i] != INVALID_SOCKET)
		(void) close (socklist[i]);
	krb5_xfree(socklist);
    }
    if (addr)
	krb5_xfree(addr);
	
    if (retval) {
	free(reply->data);
	reply->data = 0;
	reply->length = 0;
    }
    return retval;
}
