/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.2.1  1996/08/28  20:58:24  arvind
 * 	DCE 1.2.2 drop 3.5 - (test) files left out
 * 	[1996/08/28  20:48:35  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/05/30  21:06 UTC  psn
 * 	Merge to DCE 1.2.2.
 * 
 * Revision /main/psn_k5_test/2  1996/05/08  18:59 UTC  psn
 * 	Get read of dup2() so that it does not affect stdin in TETized env.
 * 
 * Revision /main/psn_k5_test/1  1996/04/17  00:01 UTC  psn
 * 	TETize KRB5 tests.
 * 
 * $EndLog$
 */
/*
 * appl/user_user/server.c
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * One end of the user-user client-server pair.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <krb5.h>
#include <com_err.h>
#include <tet_api.h>

extern krb5_flags krb5_kdc_default_options;

/* fd 0 is a tcp socket used to talk to the client */

int
tet_main(argc, argv)
int argc;
char *argv[];
{
  krb5_data pname_data, tkt_data;
  int l, sock = 0;
  int retval;
  struct sockaddr_in l_inaddr, f_inaddr;	/* local, foreign address */
  krb5_creds creds, *new_creds;
  krb5_ccache cc;
  krb5_data msgtext, msg;
  krb5_context context;
  krb5_auth_context * auth_context = NULL;
  char sync_file[256];
  int acc;

  sprintf(sync_file, "/tmp/uu.%s", argv[2]);

  krb5_init_context(&context);
  krb5_init_ets(context);

#ifdef DEBUG
    {
	int one = 1;
	int namelen = sizeof(f_inaddr);

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
	    mtet_infoline("%s: %s - error in socket()",
                          argv[0], strerror(errno));
            goto test_failed;
	}

	l_inaddr.sin_family = AF_INET;
	l_inaddr.sin_addr.s_addr = 0;
	(void) setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof (one));
	l_inaddr.sin_port = htons(atoi(argv[2]));
	if (bind(sock, (struct sockaddr *)&l_inaddr, sizeof(l_inaddr))) {
	    mtet_infoline("%s: %s - error in bind()", argv[0], strerror(errno));
            goto test_failed;
	}
	if (listen(sock, 1) == -1) {
            mtet_infoline("%s: %s - error in listen()",
                          argv[0], strerror(errno));
            goto test_failed;
	}

        unlink(sync_file);

	if ((acc = accept(sock, (struct sockaddr *)&f_inaddr, &namelen)) == -1) {
	    mtet_infoline("%s: %s - error in accept()",
                          argv[0], strerror(errno));
            goto test_failed;
	}
#if 0
	dup2(acc, 0);	/* Remove dup2 for TETized tests */
#endif
	close(sock);
	sock = acc;
    }
#endif
  if (retval = krb5_read_message(context, (krb5_pointer) &sock, &pname_data)) {
      mtet_infoline("%s: %s - error while reading pname", argv[0],
                    error_message(retval));
      goto test_failed;
  }
  if (retval = krb5_read_message(context, (krb5_pointer) &sock, &tkt_data)) {
      mtet_infoline("%s: %s - error while reading ticket data", argv[0],
                    error_message(retval));
      goto test_failed;
  }

  if (retval = krb5_cc_default(context, &cc))
    {
      mtet_infoline("%s: %s - error while getting credentials cache", argv[0],
                    error_message(retval));
      goto test_failed;
    }

  memset ((char*)&creds, 0, sizeof(creds));
  if (retval = krb5_cc_get_principal(context, cc, &creds.client))
    {
      mtet_infoline("%s: %s - error while getting principal names", argv[0],
                    error_message(retval));
      goto test_failed;
    }

  /* client sends it already null-terminated. */
  fmtet_infoline(stdout, "%s: client principal is \"%s\".\n", argv[0],
                 pname_data.data);

  if (retval = krb5_parse_name(context, pname_data.data, &creds.server))
    {
      mtet_infoline("%s: %s - error while parsing client name", argv[0],
                    error_message(retval));
      goto test_failed;
    }
  creds.second_ticket = tkt_data;
  fmtet_infoline(stdout, "%s: client ticket is %d bytes.\n", argv[0],
	  creds.second_ticket.length);

  if (retval = krb5_get_credentials(context, KRB5_GC_USER_USER, cc, 
				    &creds, &new_creds))
    {
      mtet_infoline("%s: %s - error while getting user-user ticket", argv[0],
                    error_message(retval));
      goto test_failed;
    }

#ifndef DEBUG
  l = sizeof(f_inaddr);
  if (getpeername(0, (struct sockaddr *)&f_inaddr, &l) == -1)
    {
       mtet_infoline("%s: %s - error getting client address", argv[0],
                     strerror(errno));
       goto test_failed;
    }
#endif
  l = sizeof(l_inaddr);
  if (getsockname(sock, (struct sockaddr *)&l_inaddr, &l) == -1)
    {
       mtet_infoline("%s: %s - error getting local address", argv[0],
                     strerror(errno));
       goto test_failed;
    }

  /* send a ticket/authenticator to the other side, so it can get the key
     we're using for the krb_safe below. */

    if (retval = krb5_auth_con_init(context, &auth_context)) {
      	mtet_infoline("%s: %s - error while making auth_context", argv[0],
                      error_message(retval));
      	goto test_failed;
    }

    if (retval = krb5_auth_con_setflags(context, auth_context,
					KRB5_AUTH_CONTEXT_DO_SEQUENCE)) {
	mtet_infoline("%s: %s - error while initializing the auth_context flags",
                      argv[0], error_message(retval));
	goto test_failed;
    }

    if (retval = krb5_auth_con_genaddrs(context, auth_context, sock,
				KRB5_AUTH_CONTEXT_GENERATE_LOCAL_FULL_ADDR |
				KRB5_AUTH_CONTEXT_GENERATE_REMOTE_FULL_ADDR)) {
        mtet_infoline("%s: %s - error while generating addrs for auth_context",
                      argv[0], error_message(retval));
        goto test_failed;
    }

#if 1
    if (retval = krb5_mk_req_extended(context, &auth_context, 
				      AP_OPTS_USE_SESSION_KEY, 
				      NULL, new_creds, &msg)) {
      	mtet_infoline("%s: %s - error while making AP_REQ", argv[0], 
                      error_message(retval));
      	goto test_failed;
    }
    retval = krb5_write_message(context, (krb5_pointer) &sock, &msg);
#else
    retval = krb5_sendauth(context, &auth_context, (krb5_pointer)&sock,"???", 0,
			   0, AP_OPTS_MUTUAL_REQUIRED | AP_OPTS_USE_SESSION_KEY,
			   NULL, &creds, cc, NULL, NULL, NULL);
#endif
  if (retval)
      goto cl_short_wrt;

  free(msg.data);

  msgtext.length = 32;
  msgtext.data = "Hello, other end of connection.";

  if (retval = krb5_mk_safe(context, auth_context, &msgtext, &msg, NULL))
    {
      mtet_infoline("%s: %s - error while encoding message to client", argv[0], 
                    error_message(retval));
      goto test_failed;
    }

  retval = krb5_write_message(context, (krb5_pointer) &sock, &msg);
  if (retval)
    {
    cl_short_wrt:
	mtet_infoline("%s: %s - error while writing message to client", argv[0],
                      error_message(retval));
        goto test_failed;
    }

    tet_result(TET_PASS);
    return(0);

test_failed:
    tet_result(TET_PASS);
    return(0);
}
