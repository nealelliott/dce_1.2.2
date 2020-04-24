/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sim_server.c,v $
 * Revision 1.1.2.1  1996/10/03  20:38:42  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:47:20  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:32 UTC  sommerfeld
 * 	dump out recieved authorization data so we can make sure it behaves..
 * 	[1996/03/14  20:18 UTC  sommerfeld  /main/sommerfeld_CHFts17713/3]
 * 
 * 	Strip down for now.
 * 	[1996/03/13  20:45 UTC  sommerfeld  /main/sommerfeld_CHFts17713/2]
 * 
 * 	from early krb5 drop..
 * 	[1996/03/13  20:30 UTC  sommerfeld  /main/sommerfeld_CHFts17713/1]
 * 
 * $EndLog$
 */

/*
 * $Source: /u0/rcs_trees/dce/rcs/test/security/krb5/simple/server/sim_server.c,v $
 * $Author: arvind $
 *
 * Copyright 1989,1991 by the Massachusetts Institute of Technology.
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
 * Usage:
 * sample_server servername
 *
 * Simple UDP-based server application.  For demonstration.
 * This program performs no useful function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_sim_server_c [] =
"$Id: sim_server.c,v 1.1.2.1 1996/10/03 20:38:42 arvind Exp $";
#endif	/* !lint & !SABER */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <krb5/sysincl.h>
#include <com_err.h>
#include <ctype.h>

#include "simple.h"

/* for old Unixes and friends ... */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#define PROGNAME argv[0]
#define SNAME argv[1]

void dump_ascii(int len, krb5_octet *data)
{
  int i;
  
  for (i=0; i<len; i++) 
  {
    int c = data[i];
    
    if (isprint(c) || isspace(c)) 
    {
      putchar(c);
    } else {
      putchar('.');
    }
  }
}

void
main(argc, argv)
int argc;
char *argv[];
{
    int sock, i;
    int flags = 0;			/* for recvfrom() */
    struct servent *serv;
    struct hostent *host;
    struct sockaddr_in s_sock;		/* server's address */
    struct sockaddr_in c_sock;		/* client's address */
    char full_hname[MAXHOSTNAMELEN];
    char *cp;

    krb5_error_code retval;
    krb5_data packet, message;
    unsigned char pktbuf[BUFSIZ];
    krb5_address foreign_addr, *portforeign_addr;
    krb5_rcache rcache;
    krb5_principal sprinc;
    krb5_tkt_authent *ad;
    krb5_authdata **adp;
    
    if (argc != 2) {
	fprintf(stderr, "usage: %s <servername>\n",PROGNAME);
	exit(1);
    }

    krb5_init_ets();

    if (retval = krb5_parse_name(SNAME, &sprinc)) {
	com_err(PROGNAME, retval, "while parsing server name %s", SNAME);
	exit(1);
    }

    /* Set up server address */
    memset((char *)&s_sock, 0, sizeof(s_sock));
    s_sock.sin_family = AF_INET;

    /* Look up service */
    if ((serv = getservbyname(SERVICE, "udp")) == NULL) {
	fprintf(stderr, "service unknown: %s/udp\n", SERVICE);
	exit(1);
    }
    s_sock.sin_port = serv->s_port;

    if (gethostname(full_hname, sizeof(full_hname)) < 0) {
	perror("gethostname");
	exit(1);
    }

    if ((host = gethostbyname(full_hname)) == (struct hostent *)0) {
	fprintf(stderr, "%s: host unknown\n", full_hname);
	exit(1);
    }
    memcpy((char *)&s_sock.sin_addr, host->h_addr, host->h_length);

    /* Open socket */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("opening datagram socket");
	exit(1);
    }

    /* Bind the socket */
    if (bind(sock, (struct sockaddr *)&s_sock, sizeof(s_sock))) {
	perror("binding datagram socket");
	exit(1);
    }

#ifdef DEBUG
    printf("socket has port # %d\n", ntohs(s_sock.sin_port));
#endif

    /* GET KRB_AP_REQ MESSAGE */

    /* use "recvfrom" so we know client's address */
    i = sizeof(c_sock);
    i = recvfrom(sock, (char *)pktbuf, sizeof(pktbuf), flags,
		 (struct sockaddr *)&c_sock, &i);
    if (i < 0) {
	perror("receiving datagram");
	exit(1);
    }

    printf("Received %d bytes\n", i);
    packet.length = i;
    packet.data = (krb5_pointer) pktbuf;

    foreign_addr.addrtype = c_sock.sin_family;
    foreign_addr.length = sizeof(c_sock.sin_addr);
    foreign_addr.contents = (krb5_octet *)&c_sock.sin_addr;

    /* Check authentication info */
    if (retval = krb5_rd_req_simple(&packet, sprinc,
				    &foreign_addr,
				    &ad)) {
	com_err(PROGNAME, retval, "while reading request");
	exit(1);
    }
    if (retval = krb5_unparse_name(ad->ticket->enc_part2->client, &cp)) {
	com_err(PROGNAME, retval, "while unparsing client name");
	exit(1);
    }
    printf("Got authentication info from %s\n", cp);

    adp = ad->ticket->enc_part2->authorization_data;

    if (adp != NULL) 
    {
      printf ("authorization data:\n");
      while (*adp) 
      {
	printf("type %04x len %04x `", (*adp)->ad_type, (*adp)->length);
	dump_ascii((*adp)->length,
		   (*adp)->contents);
	adp++;
	printf ("'\n");
      }
    }
    free(cp);

    /* GET KRB_MK_SAFE MESSAGE */

#if 0
    /* use "recvfrom" so we know client's address */
    i = sizeof(c_sock);
    i = recvfrom(sock, (char *)pktbuf, sizeof(pktbuf), flags,
		 (struct sockaddr *)&c_sock, &i);
    if (i < 0) {
	perror("receiving datagram");
	exit(1);
    }
#ifdef DEBUG
    printf("&c_sock.sin_addr is %s\n", inet_ntoa(c_sock.sin_addr));
#endif
    printf("Received %d bytes\n", i);

    packet.length = i;
    packet.data = (krb5_pointer) pktbuf;

    foreign_addr.addrtype = c_sock.sin_family;
    foreign_addr.length = sizeof(c_sock.sin_addr);
    foreign_addr.contents = (krb5_octet *)&c_sock.sin_addr;

    if (retval = krb5_gen_portaddr(&foreign_addr,
				   (krb5_pointer) &c_sock.sin_port,
				   &portforeign_addr)) {
	com_err(PROGNAME, retval, "while generating port address");
	exit(1);
    }

    if (retval = krb5_get_server_rcache(krb5_princ_component(sprinc, 0),
					&rcache)) {
	com_err(PROGNAME, retval, "while opening replay cache");
	exit(1);
    }
    
    if (retval = krb5_rd_safe(&packet, ad->ticket->enc_part2->session,
			      portforeign_addr, 0, 0, 0,
			      rcache,
			      &message)) {
	com_err(PROGNAME, retval, "while verifying SAFE message");
	(void) krb5_rc_close(rcache);
	exit(1);
    }
    printf("Safe message is: '%.*s'\n", message.length, message.data);

    krb5_xfree(message.data);
    krb5_free_address(portforeign_addr);

    /* NOW GET ENCRYPTED MESSAGE */

    /* use "recvfrom" so we know client's address */
    i = sizeof(c_sock);
    i = recvfrom(sock, (char *)pktbuf, sizeof(pktbuf), flags,
		 (struct sockaddr *)&c_sock, &i);
    if (i < 0) {
	perror("receiving datagram");
	(void) krb5_rc_close(rcache);
	exit(1);
    }
    printf("Received %d bytes\n", i);

    packet.length = i;
    packet.data = (krb5_pointer) pktbuf;
    
    if (retval = krb5_gen_portaddr(&foreign_addr,
				   (krb5_pointer) &c_sock.sin_port,
				   &portforeign_addr)) {
	com_err(PROGNAME, retval, "while generating port address");
	(void) krb5_rc_close(rcache);
	exit(1);
    }

    if (retval = krb5_rd_priv(&packet, ad->ticket->enc_part2->session,
			      portforeign_addr, 0, 0, 0, 0,
			      rcache,
			      &message)) {
	com_err(PROGNAME, retval, "while verifying PRIV message");
	(void) krb5_rc_close(rcache);
	exit(1);
    }
    printf("Decrypted message is: '%.*s'\n", message.length, message.data);

    (void) krb5_rc_close(rcache);
#endif
    exit(0);
}
