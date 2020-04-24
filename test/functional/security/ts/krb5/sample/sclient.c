/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sclient.c,v $
 * Revision 1.1.2.1  1996/08/15  18:43:15  arvind
 * 	OSF DCE 1.2.2 drop3 missing files
 * 	[1996/08/15  18:42:52  arvind]
 *
 * Revision /main/DCE_1.2.2/4  1996/06/25  20:49 UTC  psn
 * 	Fix for AIX.
 * 
 * Revision /main/DCE_1.2.2/3  1996/06/11  20:28 UTC  psn
 * 	Merge the fixes for path to executable and scripts.
 * 
 * Revision /main/DCE_1.2.2/psn_krb5/1  1996/06/11  17:55 UTC  psn
 * 	Fix path to scripts and executable.
 * 
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:51 UTC  psn
 * 	Merge to DCE 1.2.2 main line.
 * 
 * Revision /main/DCE_1.2.2/psn_fix122/1  1996/05/30  21:50 UTC  psn
 * 	Merge from DCE_1.2
 * 	[1996/05/24  23:28 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * Revision /main/DCE_1.2/psn_k5_test/2  1996/04/14  21:35 UTC  psn
 * 	Do required setup.
 * 
 * Revision /main/DCE_1.2/psn_k5_test/1  1996/04/14  17:56 UTC  psn
 * 	Create child for server process.
 * 
 * Revision /main/DCE_1.2/1  1996/04/05  22:02 UTC  psn
 * 	TETize the KRB5 test for DCE 1.2.2
 * 
 * $EndLog$
 */
/*
 * appl/sample/sclient/sclient.c
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
 * Sample Kerberos v5 client.
 *
 * Usage: sample_client hostname
 */

#include <krb5.h>
#include <com_err.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sample.h>
#include <tet_api.h>

#define TIME_TO_FINISH	300	/* in seconds as supplied to alarm() */

void sample_setup(void);
void sample_cleanup(void);
void sample_test(void);
void sclient_test(void);
void sserver_test(void);

void (*tet_startup) (void) = sample_setup;
void (*tet_cleanup) (void) = sample_cleanup;


struct tet_testlist tet_testlist[] = {
	{ sample_test, 1},
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

#define DEFAULT_PORT 		"2001"
#define DEFAULT_PW 		"-dce-"

#define ENV_SAMPLE_SERVER_HOST	"KRB5_SAMPLE_SERVER_HOST"
#define ENV_SAMPLE_PORT		"KRB5_SAMPLE_PORT"
#define ENV_SAMPLE_PW		"KRB5_SAMPLE_PW"
#define ENV_SAMPLE_SERVICE	"KRB5_SAMPLE_SERVICE"


#define SBUFFSZ 256

char *sample_host, *sample_syncfile = NULL;
char sample_port[SBUFFSZ], sample_krb5ccname[SBUFFSZ];

void
sample_setup(void)
{
    sample_host = tet_getvar(ENV_SAMPLE_SERVER_HOST);

    if (!sample_host) {
        fmtet_infoline(stderr, "%s not set in tetexec.cfg\n",
	               ENV_SAMPLE_SERVER_HOST);
        TET_SETUP_FAIL;
    } else {
	char *senv, *denv;
	size_t bufsz;

	bufsz = strlen(ENV_SAMPLE_SERVER_HOST) + strlen(sample_host) + 2;
	denv = (char *)malloc(bufsz);
        sprintf(denv, "%s=%s", ENV_SAMPLE_SERVER_HOST, sample_host);
	denv[bufsz-1] = '\0';
	putenv(denv);

	memset(sample_port, 0, sizeof(sample_port));
	senv = tet_getvar(ENV_SAMPLE_PORT);
	strcpy(sample_port, senv ? senv : DEFAULT_PORT);
	bufsz = strlen(ENV_SAMPLE_PORT) + strlen(sample_port) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_SAMPLE_PORT, sample_port);
	denv[bufsz-1] = '\0';
        putenv(denv);

	bufsz = strlen("/tmp/sample") + strlen(sample_port) + 2;
	sample_syncfile = (char *)malloc(bufsz);
        sprintf(sample_syncfile, "/tmp/sample.%s", sample_port);
	sample_syncfile[bufsz-1] = '\0';
        unlink(sample_syncfile);
        if (open(sample_syncfile, O_CREAT, 0777) < 0) {
            TET_SETUP_FAIL; 
        }

        senv = tet_getvar(ENV_SAMPLE_PW);
	if (!senv)
	    senv = DEFAULT_PW;
	bufsz = strlen(ENV_SAMPLE_PW) + strlen(senv) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_SAMPLE_PW, senv);
	denv[bufsz-1] = '\0';
        putenv(denv);

        senv = tet_getvar(ENV_SAMPLE_SERVICE);
	if (!senv)
	    senv = SAMPLE_SERVICE;
	bufsz = strlen(ENV_SAMPLE_SERVICE) + strlen(senv) + 2;
	denv = (char *)malloc(bufsz);
	sprintf(denv, "%s=%s", ENV_SAMPLE_SERVICE, senv);
	denv[bufsz-1] = '\0';
        putenv(denv);

        if (system("./sample_setup.ksh") != 0) {
	    mtet_infoline("FAILED: system(\"./sample_setup.sh\")\n");
            TET_SETUP_FAIL;
        }
    }
}

void
sample_test(void)
{
    tet_infoline("Fork sample server processe. The parent will play client\n");
    tet_fork(sserver_test, sclient_test, TIME_TO_FINISH, 0);
}

void
sserver_test(void)
{
    static char *server_args[] = { "./sserver",
                                   sample_port,
                                   NULL };

    mtet_infoline("Starting up sample server: %s\n", server_args[0]);
    (void) tet_exec(server_args[0], server_args, environ);
    mtet_infoline("tet_exec(\"%s\", args, env) failed with error - %s\n",
                  server_args[0], strerror(errno));
    tet_result(TET_UNRESOLVED);
}

void
sclient_test(void)
{
    struct servent *sp;
    struct hostent *hp;
    struct sockaddr_in sin, lsin;
    int sock, namelen;
    krb5_context context;
    krb5_data recv_data;
    krb5_data cksum_data;
    krb5_error_code retval;
    krb5_ccache ccdef;
    krb5_principal client, server;
    krb5_error *err_ret;
    krb5_ap_rep_enc_part *rep_ret;
    krb5_auth_context * auth_context = 0;
    short xmitlen;
    struct stat statbuf;
    char *sample_service = NULL;
    FILE *fenv;

    while (!stat(sample_syncfile, &statbuf))
        sleep(10);
 
    if (!(fenv = fopen("/tmp/sample.env", "r"))) {
	mtet_infoline("sclient: %s - error while opening /tmp/sample.env\n",
                      strerror(errno));
        goto test_failed;
    } else {
        memset(sample_krb5ccname, 0, sizeof(sample_krb5ccname));
        fscanf(fenv, "%s", sample_krb5ccname);
        putenv(sample_krb5ccname);
        unlink("/tmp/sample.env");
    }

    krb5_init_context(&context);
    krb5_init_ets(context);

    if (!valid_cksumtype(CKSUMTYPE_CRC32)) {
	mtet_infoline("sclient: %s - error while using CRC-32\n",
                      error_message(KRB5_PROG_SUMTYPE_NOSUPP));
        goto test_failed;
    }

    /* clear out the structure first */
    (void) memset((char *)&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(sample_port));

    /* look up the server host */
    hp = gethostbyname(sample_host);
    if (!hp) {
	fmtet_infoline(stderr, "unknown host %s\n", sample_host);
	goto test_failed;
    }

    sample_service = tet_getvar(ENV_SAMPLE_SERVICE);
    if (retval = krb5_sname_to_principal(context, sample_host, sample_service ?
					 sample_service : SAMPLE_SERVICE,
					 KRB5_NT_SRV_HST, &server)) {
	mtet_infoline("sclient: %s - error while creating server name for %s\n",
		      error_message(retval), sample_host);
	goto test_failed;
    }

    /* set up the address of the foreign socket for connect() */
    sin.sin_family = hp->h_addrtype;
    (void) memcpy((char *)&sin.sin_addr,
		  (char *)hp->h_addr,
		  sizeof(hp->h_addr));

    /* open a TCP socket */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	mtet_infoline("sclient: %s - error in socket()\n", strerror(errno));
	goto test_failed;
    }

    /* connect to the server */
    if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	mtet_infoline("sclient: %s - error in connect()\n", strerror(errno));
	close(sock);
	goto test_failed;
    }

    /* find out who I am, now that we are connected and therefore bound */
    namelen = sizeof(lsin);
    if (getsockname(sock, (struct sockaddr *) &lsin, &namelen) < 0) {
	mtet_infoline("sclient: %s - error in getsockname()\n", strerror(errno));
	close(sock);
	goto test_failed;
    }

    cksum_data.data = sample_host;
    cksum_data.length = strlen(sample_host);

    if (retval = krb5_cc_default(context, &ccdef)) {
	mtet_infoline("sclient: %s - error while getting default ccache\n",
                      error_message(retval));
	goto test_failed;
    }

    if (retval = krb5_cc_get_principal(context, ccdef, &client)) {
	mtet_infoline("sclient: %s - error while getting client principal name\n",
                      error_message(retval));
	goto test_failed;
    }

    retval = krb5_sendauth(context, &auth_context, (krb5_pointer) &sock,
			   SAMPLE_VERSION, client, server,
			   AP_OPTS_MUTUAL_REQUIRED,
			   &cksum_data,
			   0,		/* no creds, use ccache instead */
			   ccdef, &err_ret, &rep_ret, NULL);

    krb5_free_principal(context, server);	/* finished using it */

    if (retval && retval != KRB5_SENDAUTH_REJECTED) {
	mtet_infoline("sclient: %s - error while using sendauth\n",
                      error_message(retval));
	goto test_failed;
    }
    if (retval == KRB5_SENDAUTH_REJECTED) {
	/* got an error */
	mtet_infoline(
               "sclient: sendauth rejected, error reply is:\n\t\"%*s\"\n",
	       err_ret->text.length, err_ret->text.data);
        goto test_failed;
    } else if (rep_ret) {
	/* got a reply */
	mtet_infoline("sclient: sendauth succeeded, reply is:\n");
	if ((retval = krb5_net_read(context, sock, (char *)&xmitlen,
				    sizeof(xmitlen))) <= 0) {
	    if (retval == 0)
		errno = ECONNABORTED;
	    mtet_infoline("sclient: %s - error while reading data from server\n",
                          strerror(errno));
	    goto test_failed;
	}
	recv_data.length = ntohs(xmitlen);
	if (!(recv_data.data = (char *)malloc(recv_data.length + 1))) {
	    mtet_infoline("sclient: %s - error while allocating buffer to read from server\n",
                          strerror(ENOMEM));
	    goto test_failed;
	}
	if ((retval = krb5_net_read(context, sock, (char *)recv_data.data,
				    recv_data.length)) <= 0) {
	    if (retval == 0)
		errno = ECONNABORTED;
	    mtet_infoline("sclient: %s - error while reading data from server\n",
                          strerror(errno));
	    goto test_failed;
	}
	recv_data.data[recv_data.length] = '\0';
	mtet_infoline("sclient: reply len %d, contents:\n%s\n",
	              recv_data.length,recv_data.data);
    } else {
	mtet_infoline("sclient: Error - no error or reply from sendauth!\n");
	goto test_failed;
    }

    tet_result(TET_PASS);
    return;

test_failed:
    tet_result(TET_FAIL);
    return;
}

void
sample_cleanup(void)
{
    if (!sample_syncfile)
	free(sample_syncfile);

    system("./sample_cleanup.ksh");
}

