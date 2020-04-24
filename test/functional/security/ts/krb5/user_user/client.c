/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.2.1  1996/08/28  20:58:06  arvind
 * 	DCE 1.2.2 drop 3.5 - (test) files left out
 * 	[1996/08/28  20:48:19  arvind]
 *
 * Revision /main/DCE_1.2.2/4  1996/06/25  20:48 UTC  psn
 * 	Fix for AIX.
 * 
 * Revision /main/DCE_1.2.2/3  1996/06/12  14:04 UTC  psn
 * 	Fix KRB5CCNAME env setting clash.
 * 
 * Revision /main/DCE_1.2.2/2  1996/06/11  20:22 UTC  psn
 * 	Merge path fixes for executable and scripts.
 * 
 * Revision /main/DCE_1.2.2/psn_krb5/1  1996/06/11  18:27 UTC  psn
 * 	Fix path to scripts and executable.
 * 
 * Revision /main/DCE_1.2.2/1  1996/05/30  20:59 UTC  psn
 * 	Merge to DCE 1.2.2.
 * 
 * Revision /main/psn_k5_test/2  1996/05/08  18:59 UTC  psn
 * 	Set KRB5CCNAME before forking off the server.
 * 
 * Revision /main/psn_k5_test/1  1996/04/17  00:00 UTC  psn
 * 	TETize KRB5 tests.
 * 
 * $EndLog$
 */
/*
 * appl/user_user/client.c
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
 * Other end of user-user client/server pair.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <krb5.h>
#include <com_err.h>
#include <tet_api.h>

#define TIME_TO_FINISH  300     /* in seconds as supplied to alarm() */

void uu_setup(void);
void uu_cleanup(void);
void uu_test(void);
void uu_client_test(void);
void uu_server_test(void);

void (*tet_startup) (void) = uu_setup;
void (*tet_cleanup) (void) = uu_cleanup;

struct tet_testlist tet_testlist[] = {
        { uu_test, 1},
        { NULL, 0}
};

#define TET_SETUP_FAIL \
     { \
        int test_no; \
 \
        for (test_no=0; tet_testlist[test_no++].testfunc != NULL;) \
             tet_delete(test_no, "Setup failed."); \
        return; \
     }

#define DEFAULT_PORT        "2001"
#define DEFAULT_PW          "-dce-"
#define UU_SERVICE          "uu"

#define ENV_UU_SERVER_HOST  "KRB5_UU_SERVER_HOST"
#define ENV_UU_PORT         "KRB5_UU_PORT"
#define ENV_UU_PW           "KRB5_UU_PW"
#define ENV_UU_SERVICE      "KRB5_UU_SERVICE"

#define UUBUFSZ 256
char *uu_syncfile = NULL;
char uu_host[UUBUFSZ], uu_port[UUBUFSZ], uu_krb5ccname[UUBUFSZ];

void
uu_setup(void)
{
    char *senv;

    senv = tet_getvar(ENV_UU_SERVER_HOST);
    if (!senv) {
        fmtet_infoline(stderr, "%s not set in tetexec.cfg\n",
	               ENV_UU_SERVER_HOST);
        TET_SETUP_FAIL;
    } else {
	char *denv;
	size_t bufsz;

	memset(uu_host, 0, sizeof(uu_host));
	strcpy(uu_host, senv);
	bufsz = strlen(ENV_UU_SERVER_HOST) + strlen(uu_host) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_UU_SERVER_HOST, uu_host);
	denv[bufsz-1] = '\0';
	putenv(denv);

	memset(uu_port, 0, sizeof(uu_port));
	senv = tet_getvar(ENV_UU_PORT);
	strcpy(uu_port, senv ? senv : DEFAULT_PORT);
	bufsz = strlen(ENV_UU_PORT) + strlen(uu_port) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_UU_PORT, uu_port);
	denv[bufsz-1] = '\0';
	putenv(denv);

	bufsz = strlen("/tmp/uu") + strlen(uu_port) + 2;
	uu_syncfile = (char *)malloc(bufsz);
	sprintf(uu_syncfile, "/tmp/uu.%s", uu_port);
	uu_syncfile[bufsz-1] = '\0';
	unlink(uu_syncfile);
	if (open(uu_syncfile, O_CREAT, 0777) < 0) {
	    TET_SETUP_FAIL;
	}

	senv = tet_getvar(ENV_UU_PW);
	if (!senv)
	    senv = DEFAULT_PW;
	bufsz = strlen(ENV_UU_PW) + strlen(senv) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_UU_PW, senv);
	denv[bufsz-1] = '\0';
	putenv(denv);

	senv = tet_getvar(ENV_UU_SERVICE);
	if (!senv)
	    senv = UU_SERVICE;
	bufsz = strlen(ENV_UU_SERVICE) + strlen(senv) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_UU_SERVICE, senv);
	denv[bufsz-1] = '\0';
	putenv(denv);

	if (system("./uu_setup.ksh") != 0) {
	    mtet_infoline("FAILED: system(\"./uu_setup.ksh\")\n");
	    TET_SETUP_FAIL;
	}
    }
}

void
uu_test(void)
{
  FILE *fenv;

    if (!(fenv = fopen("/tmp/uu.env", "r"))) {
        mtet_infoline("uu-client: %s - error while opening /tmp/uu.env\n",
                      strerror(errno));
        tet_result(TET_FAIL);
        return;
    } else {
        memset(uu_krb5ccname, 0, sizeof(uu_krb5ccname));
        fscanf(fenv, "%s", uu_krb5ccname);
        putenv(uu_krb5ccname);
    }

    tet_infoline("Fork uu server processe. The parent will play client\n");
    tet_fork(uu_server_test, uu_client_test, TIME_TO_FINISH, 0);
}

void
uu_server_test(void)
{
    static char *server_args[] = { "./uu-server",
                                   uu_host,
                                   uu_port,
                                   NULL };

    mtet_infoline("Starting up uu server: %s\n", server_args[0]);
    (void) tet_exec(server_args[0], server_args, environ);
    mtet_infoline("tet_exec(\"%s\", args, env) failed with error - %s.\n",
                  server_args[0], strerror(errno));
    tet_result(TET_UNRESOLVED);
}

void
uu_client_test(void)
{
  int s;
  int retval, i;
  char *hname;		/* full name of server */
  char **srealms;	/* realm(s) of server */
  char *princ;		/* principal in credentials cache */
  struct servent *serv;
  struct hostent *host;
  struct sockaddr_in serv_net_addr, cli_net_addr;
  krb5_ccache cc;
  krb5_creds creds, *new_creds;
  krb5_data reply, msg, princ_data;
  krb5_auth_context * auth_context = NULL;
  krb5_ticket * ticket = NULL;
  krb5_context context;
  unsigned short port;
  struct stat statbuf;
  char *uu_service = NULL;

  while (!stat(uu_syncfile, &statbuf))
      sleep(10);

  krb5_init_context(&context);
  krb5_init_ets(context);

  port = htons(atoi(uu_port));

  if ((host = gethostbyname (uu_host)) == NULL)
    {
      extern int h_errno;

      if (h_errno == HOST_NOT_FOUND)
	fmtet_infoline (stderr, "uu-client: unknown host \"%s\".\n", uu_host);
      else
	fmtet_infoline (stderr,
	                "uu-client: can't get address of host \"%s\".\n",
	                uu_host);
      goto test_failed;
    }

  if (host->h_addrtype != AF_INET)
    {
      fmtet_infoline (stderr, "uu-client: bad address type %d for \"%s\".\n",
	       host->h_addrtype, uu_host);
      goto test_failed;
    }

  hname = strdup (host->h_name);

#ifndef USE_STDOUT
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      mtet_infoline ("uu-client: %s - error in socket()\n", strerror(errno));
      goto test_failed;
    } else {
      cli_net_addr.sin_family = AF_INET;
      cli_net_addr.sin_port = 0;
      cli_net_addr.sin_addr.s_addr = 0;
      if (bind (s, (struct sockaddr *)&cli_net_addr, sizeof (cli_net_addr)) < 0)
	{
	  mtet_infoline ("uu-client: %s - error in bind()\n", strerror(errno));
	  goto test_failed;
	}
    }

  serv_net_addr.sin_family = AF_INET;
  serv_net_addr.sin_port = port;

  i = 0;
  while (1)
    {
      if (host->h_addr_list[i] == 0)
	{
	  fmtet_infoline (stderr, "uu-client: unable to connect to \"%s\"\n",
	                  hname);
	  goto test_failed;
	}
      memcpy ((char *)&serv_net_addr.sin_addr, host->h_addr_list[i++], host->h_length);
      if (connect(s, (struct sockaddr *)&serv_net_addr, sizeof (serv_net_addr)) == 0)
	break;
      mtet_infoline ("uu-client: %s - error in connect()-ing to \"%s\" (%s).\n",
	       strerror(errno), hname, inet_ntoa(serv_net_addr.sin_addr));
    }
#else
  s = 1;
#endif

  if (retval = krb5_cc_default(context, &cc))
    {
      mtet_infoline("uu-client: %s - error while getting credentials cache\n",
                    error_message(retval));
      goto test_failed;
    }

  memset ((char*)&creds, 0, sizeof(creds));
  if (retval = krb5_cc_get_principal(context, cc, &creds.client))
    {
      mtet_infoline("uu-client: %s - error while getting principal name\n",
                    error_message(retval));
      goto test_failed;
    }

  if (retval = krb5_unparse_name(context, creds.client, &princ))
    mtet_infoline("uu-client: %s - error while printing principal name\n",
                  error_message(retval));
  else
    fmtet_infoline(stderr, "uu-client: client principal is \"%s\".\n", princ);

  if (retval = krb5_get_host_realm(context, hname, &srealms))
    {
      mtet_infoline("uu-client: %s - error while getting realms for \"%s\"\n",
                    error_message(retval), hname);
      goto test_failed;
    }

  if (retval = krb5_build_principal_ext(context, &creds.server,
				krb5_princ_realm(context, creds.client)->length,
				krb5_princ_realm(context, creds.client)->data,
				6, "krbtgt",
				krb5_princ_realm(context, creds.client)->length,
				krb5_princ_realm(context, creds.client)->data,
					0))
    {
      mtet_infoline("uu-client: %s - error while setting up tgt server name\n",
                    error_message(retval));
      goto test_failed;
    }

  /* Get TGT from credentials cache */
  if (retval = krb5_get_credentials(context, KRB5_GC_CACHED, cc, 
				    &creds, &new_creds))
    {
      mtet_infoline("uu-client: %s - error while getting TGT\n",
                    error_message(retval));
      goto test_failed;
    }

  i = strlen(princ) + 1;

  fmtet_infoline(stderr, "uu-client: sending %d bytes\n",
                 new_creds->ticket.length + i);
  princ_data.data = princ;
  princ_data.length = i;		/* include null terminator for
					   server's convenience */
  retval = krb5_write_message(context, (krb5_pointer) &s, &princ_data);
  if (retval)
    {
      mtet_infoline(
	"uu-client: %s - error while sending principal name to server\n",
                    error_message(retval));
      goto test_failed;
    }
  free(princ);
  retval = krb5_write_message(context, (krb5_pointer) &s, &new_creds->ticket);
  if (retval)
    {
      mtet_infoline("uu-client: %s - error while sending ticket to server\n",
                    error_message(retval));
      goto test_failed;
    }

  retval = krb5_read_message(context, (krb5_pointer) &s, &reply);
  if (retval)
    {
      mtet_infoline("uu-client: %s - error while reading reply from server\n",
                    error_message(retval));
      goto test_failed;
    }

    if (retval = krb5_auth_con_init(context, &auth_context)) {
      	mtet_infoline(
	    "uu-client: %s - error while initializing the auth_context\n",
                      error_message(retval));
      	goto test_failed;
    }

    if (retval = krb5_auth_con_genaddrs(context, auth_context, s,
			KRB5_AUTH_CONTEXT_GENERATE_LOCAL_FULL_ADDR |
			KRB5_AUTH_CONTEXT_GENERATE_REMOTE_FULL_ADDR)) {
      	mtet_infoline(
	    "uu-client: %s - error while generating addrs for auth_context\n",
                      error_message(retval));
      	goto test_failed;
    }

    if (retval = krb5_auth_con_setflags(context, auth_context,
					KRB5_AUTH_CONTEXT_DO_SEQUENCE)) {
	mtet_infoline(
	    "uu-client: %s - error while initializing the auth_context flags\n",
                      error_message(retval));
	goto test_failed;
    }

    if (retval = krb5_auth_con_setuseruserkey(context, auth_context, 
					      &new_creds->keyblock)) {
	mtet_infoline(
	    "uu-client: %s - error while setting useruserkey for authcontext\n",
                      error_message(retval));
	goto test_failed;
    }

#if 1
    /* read the ap_req to get the session key */
    retval = krb5_rd_req(context, &auth_context, &reply,
			 NULL, NULL, NULL, &ticket);
    free(reply.data);
#else
    retval = krb5_recvauth(context, &auth_context, (krb5_pointer)&s, "???",
			 0, /* server */, 0, NULL, &ticket);
#endif

  if (retval) {
      mtet_infoline("uu-client: %s - error while reading AP_REQ from server\n",
                    error_message(retval));
      goto test_failed;
  }
  if (retval = krb5_unparse_name(context, ticket->enc_part2->client, &princ))
      mtet_infoline("uu-client: %s - error while unparsing client name\n",
                    error_message(retval));
  else {
      fmtet_infoline(stdout, "server is named \"%s\"\n", princ);
      free(princ);
  }
  retval = krb5_read_message(context, (krb5_pointer) &s, &reply);
  if (retval)
    {
      mtet_infoline("uu-client: %s error while reading reply from server\n",
                    error_message(retval));
      goto test_failed;
    }

    if (retval = krb5_rd_safe(context, auth_context, &reply, &msg, NULL)) {
    	mtet_infoline("uu-client: %s error while decoding reply from server\n",
                      error_message(retval));
      	goto test_failed;
    }
    fmtet_infoline(stdout, "uu-client: server says \"%s\".\n", msg.data);
    tet_result(TET_PASS);
    return;

test_failed:
    tet_result(TET_FAIL);
    return;
}

void
uu_cleanup(void)
{
    if (!uu_syncfile)
	free(uu_syncfile);

    system("./uu_cleanup.ksh");
}

