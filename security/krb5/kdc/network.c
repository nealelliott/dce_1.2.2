/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: network.c,v $
 * Revision 1.1.8.1  1996/10/03  15:00:48  arvind
 * 	merge up from beta6
 * 	[1996/09/16  20:59 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:08:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:17  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:45:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/08  20:53 UTC  burati
 * 	Merge fix for CHFts14321 from burati_chfts14321 branch
 * 
 * 	HP revision /main/mb_kk_fixes/1  1995/02/06  23:41 UTC  burati
 * 	Fix socket leak in setup_network()
 * 	[1995/12/08  16:57:41  root]
 * 
 * Revision 1.1.3.3  1992/12/29  14:18:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:55:05  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:19:29  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:33:38  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:54:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1995, 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * kdc/network.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Network code for Kerberos v5 KDC.
 */

#ifdef OSF_DCE
#define OSF_DCE_BUGFIX
#define OSF_DCE_ARCHAISM
#endif

#ifdef __hpux
#define HPUX_BRAINDEAD_SELECT_PROTOTYPE
#endif

#ifdef OSF_DCE_ARCHAISM
#include <krb5/copyright.h>
#include <krb5/osconf.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <krb5/kdb.h>
#else
#include "k5-int.h"
#endif
#include "com_err.h"
#include "kdc_util.h"
#include "extern.h"
#include "kdc5_err.h"

#include <ctype.h>
#ifdef KRB5_USE_INET
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#include <arpa/inet.h>
#ifdef OSF_DCE_ARCHAISM
#include <netdb.h>		/* for getservbyname & struct servent */
#endif


extern int errno;

static int *udp_port_fds = (int *) NULL;
static u_short *udp_port_nums = (u_short *) NULL;
static int n_udp_ports = 0;
static int max_udp_ports = 0;
static fd_set select_fds;
static int select_nfds;

#define safe_realloc(p,n) ((p)?(realloc(p,n)):(malloc(n)))

static krb5_error_code add_port(port)
     u_short port;
{
    int	i;
    int *new_fds;
    u_short *new_ports;
    int new_max;

    for (i=0; i < n_udp_ports; i++) {
	if (udp_port_nums[i] == port)
	    return 0;
    }
    
    if (n_udp_ports >= max_udp_ports) {
	new_max = max_udp_ports + 10;
	new_fds = safe_realloc(udp_port_fds, new_max * sizeof(int));
	if (new_fds == 0)
	    return ENOMEM;
	udp_port_fds = new_fds;

	new_ports = safe_realloc(udp_port_nums, new_max * sizeof(u_short));
	if (new_ports == 0)
	    return ENOMEM;
	udp_port_nums = new_ports;
#ifdef OSF_DCE_BUGFIX
	for (i=max_udp_ports; i<new_max; i++)
	    udp_port_nums[i] = -1;
#endif
	max_udp_ports = new_max;
    }
	
    udp_port_nums[n_udp_ports++] = port;
    return 0;
}
#undef safe_realloc

krb5_error_code
setup_network(prog)
const char *prog;
{
    struct sockaddr_in sin;
    krb5_error_code retval;
    u_short port;
    char *cp;
    int i;
#ifdef OSF_DCE_ARCHAISM
    extern char *krb5_kdc_udp_portname;
    struct servent *sp;
#endif

    FD_ZERO(&select_fds);
    select_nfds = 0;
    memset((char *)&sin, 0, sizeof(sin));

#ifdef OSF_DCE_ARCHAISM
    cp = krb5_kdc_udp_portname;
    sp = getservbyname(cp, "udp");
    if (!sp) {
	com_err(prog, 0, "%s/udp service unknown\n", cp);
	return KDC5_NOPORT;
    }
    port = ntohs(sp->s_port);
    add_port(port);
#else
    /* Handle each realm's ports */
    for (i=0; i<kdc_numrealms; i++) {
	cp = kdc_realmlist[i]->realm_ports;
	while (cp && *cp) {
	    if (*cp == ',' || isspace(*cp)) {
		cp++;
		continue;
	    }
	    port = strtol(cp, &cp, 10);
	    if (cp == 0)
		break;
	    retval = add_port(port);
	    if (retval)
		return retval;
	}
    }
#endif

    for (i=0; i<n_udp_ports; i++) {
	if ((udp_port_fds[i] = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
	    retval = errno;
	    com_err(prog, 0, "Cannot create server socket on port %d",
		    udp_port_nums[i]);
#ifdef OSF_DCE_BUGFIX
	    closedown_network(prog);
#endif
	    return(retval);
	}
	sin.sin_port = htons(udp_port_nums[i]);
	if (bind(udp_port_fds[i], (struct sockaddr *) &sin,
		 sizeof(sin)) == -1) {
	    retval = errno;
	    com_err(prog, 0, "Cannot bind server socket on port %d",
		    udp_port_nums[i]);
#ifdef OSF_DCE_BUGFIX	    
	    closedown_network(prog);
#endif
	    return(retval);
	}
	FD_SET(udp_port_fds[i], &select_fds);
	if (udp_port_fds[i]+1 > select_nfds)
	    select_nfds = udp_port_fds[i]+1;
    }

    return 0;
}

void process_packet(port_fd, prog, portnum)
    int	port_fd;
    const char	*prog;
    int		portnum;
{
    int cc, saddr_len;
    krb5_fulladdr faddr;
    krb5_error_code retval;
    struct sockaddr_in saddr;
    krb5_address addr;
    krb5_data request;
    krb5_data *response;
    char pktbuf[MAX_DGRAM_SIZE];

    if (port_fd < 0)
	return;
    
    saddr_len = sizeof(saddr);
    cc = recvfrom(port_fd, pktbuf, sizeof(pktbuf), 0,
		  (struct sockaddr *)&saddr, &saddr_len);
    if (cc == -1) {
	if (errno != EINTR)
	    com_err(prog, errno, "while receiving from network");
	return;
    }
    if (!cc)
	return;		/* zero-length packet? */

    request.length = cc;
    request.data = pktbuf;
    faddr.port = ntohs(saddr.sin_port);
    faddr.address = &addr;
    addr.addrtype = ADDRTYPE_INET;
    addr.length = 4;
    /* this address is in net order */
    addr.contents = (krb5_octet *) &saddr.sin_addr;
    if ((retval = dispatch(&request, &faddr, portnum, &response))) {
	com_err(prog, retval, "while dispatching");
	return;
    }
    cc = sendto(port_fd, response->data, response->length, 0,
		(struct sockaddr *)&saddr, saddr_len);
    if (cc == -1) {
        krb5_free_data(/*kdc_context, */response);
	com_err(prog, errno, "while sending reply to %s/%d",
		inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
	return;
    }
    if (cc != response->length) {
	krb5_free_data(/*kdc_context, */response);
	com_err(prog, 0, "short reply write %d vs %d\n",
		response->length, cc);
	return;
    }
    krb5_free_data(/*kdc_context, */response);
    return;
}

krb5_error_code
listen_and_process(prog)
const char *prog;
{
    int			nfound;
    fd_set		readfds;
    int			i;

    if (udp_port_fds == (int *) NULL)
	return KDC5_NONET;
    
    while (!signal_requests_exit) {
	readfds = select_fds;
#ifdef HPUX_BRAINDEAD_SELECT_PROTOTYPE
	nfound = select(select_nfds, (int *)&readfds, 0, 0, 0);
#else
	nfound = select(select_nfds, &readfds, 0, 0, 0);	
#endif
	if (nfound == -1) {
	    if (errno == EINTR)
		continue;
	    com_err(prog, errno, "while selecting for network input");
	    continue;
	}
	for (i=0; i<n_udp_ports; i++) {
	    if (FD_ISSET(udp_port_fds[i], &readfds)) {
		process_packet(udp_port_fds[i], prog, udp_port_nums[i]);
		nfound--;
		if (nfound == 0)
		    break;
	    }
	}
    }
    return 0;
}

krb5_error_code
closedown_network(prog)
const char *prog;
{
    int i;

    if (udp_port_fds == (int *) NULL)
	return KDC5_NONET;

    for (i=0; i<n_udp_ports; i++) {
	if (udp_port_fds[i] >= 0)
	    (void) close(udp_port_fds[i]);
    }
    free(udp_port_fds);
    free(udp_port_nums);

    return 0;
}

#endif /* INET */
