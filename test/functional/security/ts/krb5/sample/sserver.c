/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sserver.c,v $
 * Revision 1.1.2.1  1996/08/15  18:44:42  arvind
 * 	OSF DCE 1.2.2 drop3 missing files
 * 	[1996/08/15  18:44:16  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:57 UTC  psn
 * 	Merge to DCE 1.2.2 main line.
 * 
 * Revision /main/DCE_1.2.2/psn_fix122/1  1996/05/30  21:53 UTC  psn
 * 	Merge from DCE_1.2
 * 	[1996/05/24  23:28 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * Revision /main/DCE_1.2/psn_k5_test/1  1996/04/14  17:56 UTC  psn
 * 	Create child for server process.
 * 
 * Revision /main/DCE_1.2/2  1996/04/09  14:26 UTC  psn
 * 	TETize KRB5 tests.
 * 
 * Revision /main/DCE_1.2/1  1996/04/05  22:02 UTC  psn
 * 	TETize the KRB5 test for DCE 1.2.2
 * 
 * $EndLog$
 */
/*
 * appl/sample/sserver/sserver.c
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
 * Sample Kerberos v5 server.
 *
 * sample_server:
 * A sample Kerberos server, which reads an AP_REQ from a TCP socket,
 * decodes it, and writes back the results (in ASCII) to the client.
 *
 * Usage:
 * sample_server servername
 *
 * file descriptor 0 (zero) should be a socket connected to the requesting
 * client (this will be correct if this server is started by inetd).
 */

#include <krb5.h>
#include <com_err.h>

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <syslog.h>

#include <sample.h>
#include <tet_api.h>

extern krb5_deltat krb5_clockskew;

#define DEBUG

int
tet_main(argc, argv)
    int argc;
    char *argv[];
{
    krb5_context context;
    krb5_auth_context * auth_context = NULL;
    krb5_ticket * ticket;
    krb5_address peeraddr;
    struct sockaddr_in peername;
    int namelen = sizeof(peername);
    int sock = -1;			/* incoming connection fd */
    krb5_data recv_data;
    short xmitlen;
    krb5_error_code retval;
    krb5_principal server, client;
    char repbuf[BUFSIZ];
    char *cname;
    int acc;
    struct sockaddr_in sin;
    char sync_file[256];

    sprintf(sync_file, "/tmp/sample.%s", argv[1]);
    krb5_init_context(&context);
    krb5_init_ets(context);

    if (retval = krb5_sname_to_principal(context, NULL, SAMPLE_SERVICE, 
					 KRB5_NT_SRV_HST, &server)) {
	mtet_infoline("%s: error while generating service name (%s): %s",
                      argv[0], SAMPLE_SERVICE, error_message(retval));
	goto test_failed;
    }

    /*
     * If user specified a port, then listen on that port; otherwise,
     * assume we've been started out of inetd.
     */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	mtet_infoline("%s: %s - error in socket()", argv[0], strerror(errno));
        goto test_failed;
    }

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(atoi(argv[1]));
    if (bind(sock, &sin, sizeof(sin))) {
	mtet_infoline("%s: %s - error in bind()", argv[0], strerror(errno));
        goto test_failed;
    }
    if (listen(sock, 1) == -1) {
	mtet_infoline("%s: %s - error in listen()", argv[0], strerror(errno));
	goto test_failed;
    }

    unlink(sync_file);

    if ((acc = accept(sock, (struct sockaddr *)&peername, &namelen)) == -1){
	mtet_infoline("%s: %s - error in accept()", argv[0], strerror(errno));
	goto test_failed;
    }
    dup2(acc, 0);
    close(sock);
    sock = 0;

    peeraddr.addrtype = peername.sin_family;
    peeraddr.length = sizeof(peername.sin_addr);
    peeraddr.contents = (krb5_octet *)&peername.sin_addr;

    if (retval = krb5_recvauth(context, &auth_context, (krb5_pointer)&sock,
			       SAMPLE_VERSION, server, 
			       0,	/* no flags */
			       NULL,	/* default keytab */
			       &ticket)) {
	mtet_infoline("%s: recvauth failed--%s", argv[0],
                      error_message(retval));
	goto test_failed;
    }

    /* Get client name */
    if (retval = krb5_unparse_name(context, ticket->enc_part2->client, &cname)){
	mtet_infoline("%s: unparse failed: %s", argv[0], error_message(retval));
        sprintf(repbuf, "You are <unparse error>\n");
    } else {
        sprintf(repbuf, "You are %s\n", cname);
	free(cname);
    }
    xmitlen = htons(strlen(repbuf));
    recv_data.length = strlen(repbuf);
    recv_data.data = repbuf;
    if ((retval = krb5_net_write(context, 0, (char *)&xmitlen,
				 sizeof(xmitlen))) < 0) {
	mtet_infoline("%s: %s - error while writing len to client (1)",
                      argv[0], error_message(retval));
	goto test_failed;
    }
    if ((retval = krb5_net_write(context, 0, (char *)recv_data.data,
				 recv_data.length)) < 0) {
	mtet_infoline("%s: %s - error while writing data to client (2)",
                      argv[0], error_message(retval));
	goto test_failed;
    }

    tet_result(TET_PASS);
    return(0);

test_failed:
    tet_result(TET_PASS);
    return(0);
}
