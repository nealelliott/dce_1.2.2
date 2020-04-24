/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kcmd.c,v $
 * Revision 1.1.2.2  1996/10/03  14:38:57  arvind
 * 	Submit code review fixes
 * 	[1996/08/28  20:36 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 *
 * 	remove unnecessary ifdefs - makes code harder to read.
 * 	[1996/05/30  21:17 UTC  mullan_s  /main/DCE_1.2.2/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:37 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/03  20:04 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 *
 * 	asd
 * 	[1996/04/29  21:46 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * 	Add OSF/HP copyrights.
 * 	[1996/04/29  21:46 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * Revision 1.1.2.1  1996/06/04  21:47:10  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:37 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/03  20:04 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 * 
 * 	asd
 * 	[1996/04/29  21:46 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 * 
 * 	Add OSF/HP copyrights.
 * 	[1996/04/29  21:46 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*
* Note on the ifdefs: KRBV implies Kerberos V support
* and was merged from the MIT krbv5 code for des read and write
* routines.
*/

#ifndef lint
static char Xsccsid[] = "derived from @(#)rcmd.c 5.17 (Berkeley) 6/27/88";
static char sccsid[] = "@(#)kcmd.c	8.2 (Berkeley) 8/19/93";
#endif /* not lint */

#include <sys/param.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef KRBV
#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>
#include <kerberosIV/kparse.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef KRBV
#include "krb5.h"
#define OPTS_FORWARD_CREDS           0x00000002
#define OPTS_FORWARDABLE_CREDS       0x00000001
char *default_service = "host";
extern krb5_context bsd_context;
#else
#include "krb.h"
#endif

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#ifndef POSIX_SIGNALS
#ifndef sigmask
#define sigmask(m)    (1 << ((m)-1))
#endif
#endif

#define	START_PORT	5120	 /* arbitrary */

int	getport (int *);

#ifdef KRBV
int
kcmd(sock, ahost, rport, locuser, remuser, cmd, fd2p, service, realm,
    cred, seqno, server_seqno, laddr, faddr, authopts)
#else
int
kcmd(sock, ahost, rport, locuser, remuser, cmd, fd2p, ticket, service, realm,
    cred, schedule, msg_data, laddr, faddr, authopts)
#endif
	int *sock;
	char **ahost;
	u_short rport;
	char *locuser, *remuser, *cmd;
	int *fd2p;
#ifndef KRBV
        KTEXT ticket;
#endif
	char *service;
	char *realm;
#ifdef KRBV
	krb5_creds **cred;
        krb5_int32 *seqno;
        krb5_int32 *server_seqno;
#else
        CREDENTIALS *cred;
        Key_schedule schedule;
        MSG_DAT *msg_data;
#endif
	struct sockaddr_in *laddr, *faddr;
#ifdef KRBV
	krb5_flags authopts;
#else
	long authopts;
#endif
{
	int s, timo = 1, pid;
#ifdef POSIX_SIGNALS
        sigset_t oldmask, urgmask;
#else
        long oldmask;
#endif
	struct sockaddr_in sin, from;
#ifdef KRBV
	struct sockaddr_in local_laddr;
        krb5_creds *get_cred, *ret_cred = 0;
#endif
	char c;
#ifdef ATHENA_COMPAT
	int lport = IPPORT_RESERVED - 1;
#else
	int lport = START_PORT;
#endif
	struct hostent *hp;
	int rc;
	char *host_save;
#ifdef KRBV
        krb5_error_code status;
        krb5_error *err_ret;
        krb5_ap_rep_enc_part *rep_ret;
        krb5_error  *error = 0;
        int sin_len;
        krb5_ccache cc;
        krb5_data outbuf;
        krb5_flags options = authopts;
        krb5_auth_context *auth_context = NULL;
	char *cksumbuf;
        krb5_data cksumdat;
#else
	int status;
#endif

#ifdef KRBV
        if ((cksumbuf = malloc(strlen(cmd)+strlen(remuser)+64)) == 0 ) {
        	fprintf(stderr, "Unable to allocate memory for checksum buffer.\n");
        	return(-1);
        }
        sprintf(cksumbuf, "%u:", ntohs(rport));
        strcat(cksumbuf, cmd);
        strcat(cksumbuf, remuser);
        cksumdat.data = cksumbuf;
        cksumdat.length = strlen(cksumbuf);
#endif

	pid = getpid();
	hp = gethostbyname(*ahost);
	if (hp == NULL) {
		/* fprintf(stderr, "%s: unknown host\n", *ahost); */
		return (-1);
	}

        if ((host_save = (char *) malloc(strlen(hp->h_name) + 1)) == NULL) {
        	fprintf(stderr,"kcmd: no memory\n");
        	return(-1);
        }
	strcpy(host_save, hp->h_name);
	*ahost = host_save;

#ifdef KRBV
        /* If no service is given set to the default service */
        if (!service) service = default_service;

        sin_len = strlen(host_save) + strlen(service)
        	+ (realm ? strlen(realm): 0) + 3;
        if ( sin_len < 20 ) sin_len = 20;
   
        if (!(get_cred = (krb5_creds *)calloc(1, sizeof(krb5_creds)))) {
        	fprintf(stderr,"kcmd: no memory\n");
        	return(-1);
        }
        status = krb5_sname_to_principal(bsd_context, host_save,service,
       					 KRB5_NT_SRV_HST, &get_cred->server);
        if (status) {
        	fprintf(stderr, "kcmd: krb5_sname_to_principal failed: %s\n",
                	error_message(status));
            	return(-1);
    	}

	if (realm && *realm) {
        	(void) krb5_xfree(krb5_princ_realm(bsd_context,get_cred->server)->data);
        	krb5_princ_set_realm_length(bsd_context,get_cred->server,strlen(realm));
        	krb5_princ_set_realm_data(bsd_context,get_cred->server,strdup(realm));
   	}
#else
        /* If realm is null, look up from table */
        if (realm == NULL || realm[0] == '\0')
                realm = krb_realmofhost(host_save);
#endif
#ifdef POSIX_SIGNALS
        sigemptyset(&urgmask);
        sigaddset(&urgmask, SIGURG);
        sigprocmask(SIG_BLOCK, &urgmask, &oldmask);
#else
	oldmask = sigblock(sigmask(SIGURG));
#endif
	for (;;) {
		s = getport(&lport);
		if (s < 0) {
			if (errno == EAGAIN)
				fprintf(stderr,
					"kcmd(socket): All ports in use\n");
			else
				perror("kcmd: socket");
#ifdef POSIX_SIGNALS
   		        sigprocmask(SIG_SETMASK, &oldmask, (sigset_t*)0);
#else
			sigsetmask(oldmask);
#endif
#ifdef KRBV
			krb5_free_creds(bsd_context, get_cred);
#endif
			return (-1);
		}

		sin.sin_family = hp->h_addrtype;
#if defined(ultrix) || defined(sun)
		bcopy(hp->h_addr, (caddr_t)&sin.sin_addr, hp->h_length);
#else
		bcopy(hp->h_addr_list[0], (caddr_t)&sin.sin_addr, hp->h_length);
#endif
		sin.sin_port = rport;
		if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) >= 0)
			break;
		(void) close(s);
		if (errno == EADDRINUSE) {
			lport--;
			continue;
		}
		/*
		 * don't wait very long for Kerberos rcmd.
		 */
		if (errno == ECONNREFUSED && timo <= 4) {
			/* sleep(timo); don't wait at all here */
			timo *= 2;
			continue;
		}
#if !(defined(ultrix) || defined(sun))
		if (hp->h_addr_list[1] != NULL) {
			int oerrno = errno;

			fprintf(stderr,
			    "kcmd: connect to address %s: ",
			    inet_ntoa(sin.sin_addr));
			errno = oerrno;
			perror(NULL);
			hp->h_addr_list++;
			bcopy(hp->h_addr_list[0], (caddr_t)&sin.sin_addr,
			    hp->h_length);
			fprintf(stderr, "Trying %s...\n",
				inet_ntoa(sin.sin_addr));
			continue;
		}
#endif /* !(defined(ultrix) || defined(sun)) */
		if (errno != ECONNREFUSED)
			perror(hp->h_name);
#ifdef POSIX_SIGNALS
	        sigprocmask(SIG_SETMASK, &oldmask, (sigset_t*)0);
#else
		sigsetmask(oldmask);
#endif
#ifdef KRBV
		krb5_free_creds(bsd_context, get_cred);
#endif
		return (-1);
	}
	lport--;
	if (fd2p == 0) {
		write(s, "", 1);
		lport = 0;
	} else {
		char num[8];
		int s2 = getport(&lport), s3;
		int len = sizeof(from);

		if (s2 < 0) {
			status = -1;
			goto bad;
		}
		listen(s2, 1);
		(void) sprintf(num, "%d", lport);
		if (write(s, num, strlen(num) + 1) != strlen(num) + 1) {
			perror("kcmd(write): setting up stderr");
			(void) close(s2);
			status = -1;
			goto bad;
		}
		s3 = accept(s2, (struct sockaddr *)&from, &len);
		(void) close(s2);
		if (s3 < 0) {
			perror("kcmd:accept");
			lport = 0;
			status = -1;
			goto bad;
		}
		*fd2p = s3;
		from.sin_port = ntohs((u_short)from.sin_port);
		if (from.sin_family != AF_INET ||
		    from.sin_port >= IPPORT_RESERVED) {
			fprintf(stderr,
			 "kcmd(socket): protocol failure in circuit setup.\n");
			status = -1;
			goto bad2;
		}
	}

#ifdef KRBV
        if (!laddr) laddr = &local_laddr;
        if (!faddr) faddr = &sin;
        else
      		memcpy(faddr,&sin,sizeof(sin));

    	sin_len = sizeof (struct sockaddr_in);
        if (getsockname(s, (struct sockaddr *)laddr, &sin_len) < 0) {
        	perror("getsockname");
        	status = -1;
        	goto bad2;
    	}

        if (status = krb5_cc_default(bsd_context, &cc))
        	goto bad2;

        if (status = krb5_cc_get_principal(bsd_context, cc, &get_cred->client)) {
        	(void) krb5_cc_close(bsd_context, cc);
        	goto bad2;
    	}

	{
        krb5_timestamp  currenttime = 0;
        (void) krb5_timeofday(bsd_context, &currenttime);
        get_cred->times.endtime = currenttime;
	}

	/* Get ticket from credentials cache or kdc */
        status = krb5_get_credentials(bsd_context, 0, cc, get_cred, &ret_cred);
        krb5_free_creds(bsd_context, get_cred);
        (void) krb5_cc_close(bsd_context, cc);
        if (status) goto bad2;

        /* Reset internal flags; these should not be sent. */
        authopts &= (~OPTS_FORWARD_CREDS);
        authopts &= (~OPTS_FORWARDABLE_CREDS);

        if (krb5_auth_con_init(bsd_context, &auth_context))
        	goto bad2;

        if (krb5_auth_con_setflags(bsd_context, auth_context,
                                   KRB5_AUTH_CONTEXT_RET_TIME))
        	goto bad2;

        /* Only need local address for mk_cred() to send to krlogind */
        if (status = krb5_auth_con_genaddrs(bsd_context, auth_context, s,
                            KRB5_AUTH_CONTEXT_GENERATE_LOCAL_FULL_ADDR))
        	goto bad2;

        /* call Kerberos library routine to obtain an authenticator,
           pass it over the socket to the server, and obtain mutual
           authentication. */
        status = krb5_sendauth(bsd_context, &auth_context, (krb5_pointer) &s,
                               "KCMDV0.1", ret_cred->client, ret_cred->server,
                               authopts, &cksumdat, ret_cred, 0, &error, &rep_ret, NULL);
	krb5_xfree(cksumdat.data);
        if (status) {
        	printf("Couldn't authenticate to server: %s\n", error_message(status));
        	if (error) {
            		printf("Server returned error code %d (%s)\n", error->error,
                               error_message(ERROR_TABLE_BASE_krb5 + error->error));
            		if (error->text.length) {
                		fprintf(stderr, "Error text sent from server: %s\n",
                        		error->text.data);
            		}
	            	krb5_free_error(bsd_context, error);
       	     		error = 0;
        	}
    	}
        if (status) goto bad2;
        if (rep_ret && server_seqno) {
        	*server_seqno = rep_ret->seq_number;
        	krb5_free_ap_rep_enc_part(bsd_context, rep_ret);
        }

        (void) write(s, remuser, strlen(remuser)+1);
        (void) write(s, cmd, strlen(cmd)+1);
        (void) write(s, locuser, strlen(locuser)+1);

        if (options & OPTS_FORWARD_CREDS) {   /* Forward credentials */
        	if (status = get_for_creds(bsd_context, auth_context,
                                           host_save,
                                           ret_cred->client,
                                           /* Forwardable TGT? */
                                           options & OPTS_FORWARDABLE_CREDS,
                                           &outbuf)) {
                	fprintf(stderr, "kcmd: Error getting forwarded creds\n");
                	goto bad2;
        	}

        	/* Send forwarded credentials */
        	if (status = krb5_write_message(bsd_context, (krb5_pointer)&s, &outbuf))
          		goto bad2;
        }
        else { /* Dummy write to signal no forwarding */
        	outbuf.length = 0;
        	if (status = krb5_write_message(bsd_context, (krb5_pointer)&s, &outbuf))
          		goto bad2;
    	}
#else
        /*
         * Kerberos-authenticated service.  Don't have to send locuser,
         * since its already in the ticket, and we'll extract it on
         * the other side.
         */
        /* (void) write(s, locuser, strlen(locuser)+1); */

        /* set up the needed stuff for mutual auth, but only if necessary */
        if (authopts & KOPT_DO_MUTUAL) {
                int sin_len;
                *faddr = sin;

                sin_len = sizeof(struct sockaddr_in);
                if (getsockname(s, (struct sockaddr *)laddr, &sin_len) < 0) {
                        perror("kcmd(getsockname)");
                        status = -1;
                        goto bad2;
                }
        }
#ifdef KERBEROS
        if ((status = krb_sendauth(authopts, s, ticket, service, *ahost,
                               realm, (unsigned long) getpid(), msg_data,
                               cred, schedule,
                               laddr,
                               faddr,
                               "KCMDV0.1")) != KSUCCESS)
                goto bad2;
#endif /* KERBEROS */

        (void) write(s, remuser, strlen(remuser)+1);
        (void) write(s, cmd, strlen(cmd)+1);
#endif

	if ((rc = read(s, &c, 1)) != 1) {
		if (rc == -1)
			perror(*ahost);
		else
			fprintf(stderr,"kcmd: bad connection with remote host\n");
		status = -1;
		goto bad2;
	}
	if (c != '\0') {
		while (read(s, &c, 1) == 1) {
			(void) write(2, &c, 1);
			if (c == '\n')
				break;
		}
		status = -1;
		goto bad2;
	}
#ifdef POSIX_SIGNALS
        sigprocmask(SIG_SETMASK, &oldmask, (sigset_t*)0);
#else
	sigsetmask(oldmask);
#endif
	*sock = s;

#ifdef KRBV
        /* pass back credentials if wanted */
        if (cred) krb5_copy_creds(bsd_context, ret_cred, cred);
        krb5_free_creds(bsd_context, ret_cred);

	return (0);
#else
        return (KSUCCESS);
#endif
bad2:
	if (lport)
		(void) close(*fd2p);
bad:
	(void) close(s);
#ifdef POSIX_SIGNALS
        sigprocmask(SIG_SETMASK, &oldmask, (sigset_t*)0);
#else
	sigsetmask(oldmask);
#endif
#ifdef KRBV
        if (ret_cred)
        	krb5_free_creds(bsd_context, ret_cred);
#endif
	return (status);
}

int
getport(alport)
	int *alport;
{
	struct sockaddr_in sin;
	int s;

	memset((char *) &sin, 0,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
		return (-1);
	for (;;) {
		sin.sin_port = htons((u_short)*alport);
		if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) >= 0)
			return (s);
		if (errno != EADDRINUSE) {
			(void) close(s);
			return (-1);
		}
		(*alport)--;
#ifdef ATHENA_COMPAT
		if (*alport == IPPORT_RESERVED/2) {
#else
		if (*alport == IPPORT_RESERVED) {
#endif
			(void) close(s);
			errno = EAGAIN;		/* close */
			return (-1);
		}
	}
}
