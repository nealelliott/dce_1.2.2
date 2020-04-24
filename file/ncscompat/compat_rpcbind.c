/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_rpcbind.c,v $
 * Revision 1.1.89.1  1996/10/02  17:54:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:20  damon]
 *
 * $EndLog$
*/
#ifndef __unix
#define __unix 1	/* for CMA */
#endif /* __unix */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <compat.h>


long rpcx_ipaddr_from_sockaddr(asockaddrp, aipaddrp)
struct sockaddr_in *asockaddrp;
char *aipaddrp; {
    bcopy((char *)&asockaddrp->sin_addr, aipaddrp, 4);
    return 0;
}

long rpcx_sockaddr_from_ipaddr(aipaddrp, asockaddrp)
struct sockaddr_in *asockaddrp;
char *aipaddrp; {
    bzero((char *)asockaddrp, sizeof(*asockaddrp));
    bcopy(aipaddrp, (char *)&asockaddrp->sin_addr, 4);
    asockaddrp->sin_family = AF_INET;
    return 0;
}

long rpcx_binding_to_sockaddr(ahandlep, asockaddrp)
struct sockaddr_in *asockaddrp;
rpc_binding_handle_t ahandlep; {
    unsigned char *prefix;
    char *tp;
    u_long ipAddr;
    u_short portAddr;
    unsigned_char_t *obj_uuid;
    unsigned_char_t *protseq;
    unsigned_char_t *network_addr;
    unsigned_char_t *endpoint;
    register long code = 0;
    error_status_t st;
    error_status_t dontcare;

    rpc_binding_to_string_binding(ahandlep, &prefix, (unsigned32 *)&st);
    if (st != error_status_ok) return (long) st;

    /* break apart the string binding */
    rpc_string_binding_parse(prefix, &obj_uuid, &protseq, &network_addr, 
			     &endpoint, NULL, &st);
    rpc_string_free(&prefix, &dontcare);
    if ( st != rpc_s_ok )
    {
	code = (long) st; goto exit;
    }

    /* check the protseq, the only ones supported */
    if ( strcmp("ncadg_ip_udp", (char *)protseq) != 0
	&& strcmp("ncacn_ip_tcp", (char *)protseq) != 0 )
    {
	code = -1; goto exit;
    }

    /* now for the ip addr and port */
    ipAddr = inet_addr(network_addr);

    if ( strcmp((char *)endpoint, "") != 0 )
    {
	int temp;

	temp = atoi((char *)endpoint);
	portAddr = htons(temp);
    }
    else
	portAddr = 0;

    bzero((char *)asockaddrp, sizeof(*asockaddrp));
    asockaddrp->sin_family = AF_INET;
    /* finally, copy in the results of the parsed string binding into
     * the sockaddr_in structure.
     */
    asockaddrp->sin_addr.s_addr = ipAddr;
    asockaddrp->sin_port = portAddr;

exit:
    rpc_string_free(&protseq, &st);
    rpc_string_free(&obj_uuid, &st);
    rpc_string_free(&network_addr, &st);
    rpc_string_free(&endpoint, &st);
    
    return (code);
}

long rpcx_binding_from_sockaddr(asockaddrp, ahandlep)
struct sockaddr *asockaddrp;
rpc_binding_handle_t *ahandlep;
{
    unsigned char *prefix;	/* buffer into which to put string binding */
    error_status_t st;
    unsigned char ipAddr[20];
    unsigned char taddr[4];
    unsigned char portString[20];
    register struct sockaddr_in *tap;

    /* this is *so* strange, having to convert a perfectly good socket addr
     * struct into a character string so that we can create a connection
     * from it. 
     */
    tap = (struct sockaddr_in *)asockaddrp;
    bcopy((char *)&tap->sin_addr, (char *)taddr, sizeof(taddr));
    sprintf((char *)ipAddr, "%d.%d.%d.%d", taddr[0], taddr[1], taddr[2], taddr[3]);
    sprintf((char *)portString, "%d", ntohs(tap->sin_port));
    rpc_string_binding_compose(/* uuid */ (unsigned char *)"",
				 (unsigned char *)"ncadg_ip_udp", ipAddr, portString,
			       /* options */ (unsigned char *)"", &prefix, &st);
    if (st != error_status_ok) {
	return st;
    }
    
    /* get the real RPC binding */
    rpc_binding_from_string_binding(prefix, ahandlep, (unsigned32 *)&st);
    if(st !=  error_status_ok)
	return st;

    rpc_string_free(&prefix, &st);

    /* all done */
    return 0;
}


