/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsh.c,v $
 * Revision 1.1.2.2  1996/10/03  14:39:35  arvind
 * 	Submit code review fixes
 * 	[1996/08/28  20:36 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
 *
 * 	remove unnecessary ifdefs - makes code harder to read.
 * 	[1996/08/23  19:25 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 *
 * 	Fix usage() synopsis.
 * 	[1996/05/30  21:16 UTC  mullan_s  /main/DCE_1.2.2/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 *
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/04/29  14:29 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:29 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 *
 * Revision 1.1.2.1  1996/06/04  21:48:08  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 * 
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/04/29  14:29 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 * 
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:29 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 * 
 * $EndLog$
 */
/*-
 * Copyright (c) 1983, 1990, 1993, 1994
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
*/

#ifndef lint
static char copyright[] =
"@(#) Copyright (c) 1983, 1990, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)rsh.c	8.3 (Berkeley) 4/6/94";
#endif /* not lint */

/*
 * $Source: /u0/rcs_trees/dce/rcs/mit_krb5/appl/bsd/rsh.c,v $
 * $Header: /u0/rcs_trees/dce/rcs/mit_krb5/appl/bsd/rsh.c,v 1.1.2.2 1996/10/03 14:39:35 arvind Exp $
 */

#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/file.h>

#include <netinet/in.h>
#include <netdb.h>

#include <err.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include "pathnames.h"

#ifdef KERBEROS
#ifdef KRBV
#include "krb5.h"
#include "com_err.h"
#define OPTS_FORWARD_CREDS           0x00000002
#define OPTS_FORWARDABLE_CREDS       0x00000001
#define RSH_BUFSIZ 4096
int use_kerberos = 1;
char des_inbuf[2*RSH_BUFSIZ];       /* needs to be > largest read size */
char des_outbuf[2*RSH_BUFSIZ];      /* needs to be > largest write size */
krb5_data desinbuf,desoutbuf;
krb5_encrypt_block eblock;      /* eblock for encrypt/decrypt */
krb5_context bsd_context;
krb5_creds *cred;
int     doencrypt = 0;
char    *krb_realm = (char *)0;
#else
#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>
CREDENTIALS cred;
Key_schedule schedule;
int use_kerberos = 1, doencrypt;
char dst_realm_buf[REALM_SZ], *dest_realm;
extern char *krb_realmofhost();
#endif
#endif

/*
 * rsh - remote shell
 */
int	rfd2;

char    *copyargs(char **);
void	sendsig(int);
#ifdef POSIX_SIGNALS
void	talk(int, sigset_t, pid_t, int);
#else
void	talk(int, long, pid_t, int);
#endif
void	usage(void);
#ifdef HAVE_STDARG_H
void warning(char *, ...);
#else
void	warning();
#endif
char *__progname;	/* for error reporting routines */

#ifndef _PATH_RLOGIN
#ifdef KERBEROS
#define _PATH_RLOGIN KRB5_PATH_RLOGIN
#else
#ifndef UCB_RLOGIN 
#define UCB_RLOGIN "/usr/ucb/rlogin"
#endif
#define _PATH_RLOGIN UCB_RLOGIN
#endif
#endif

int
main(argc, argv)
	int argc;
	char **argv;
{
	struct passwd *pw;
	struct servent *sp;
#ifdef POSIX_SIGNALS
	sigset_t omask, igmask;
	struct sigaction sa, osa;
#else
	long omask;
#endif
	int argoff, asrsh, ch, dflag, nflag, one, rem;
	pid_t pid;
	uid_t uid;
	char *args, *host, *p, *user;

#if defined(KERBEROS) && defined(KRBV)
        krb5_flags authopts;
        krb5_error_code status;
        int fflag = 0, Fflag = 0;
#endif  /* KRBV */
	argoff = asrsh = dflag = nflag = 0;
	one = 1;
	host = user = NULL;

	/* if called as something other than "rsh", use it as the host name */
	if (p = strrchr(argv[0], '/'))
		++p;
	else
		p = argv[0];
	if (strcmp(p, "rsh"))
		host = p;
	else
		asrsh = 1;

	__progname = p;

	/* handle "rsh host flags" */
	if (!host && argc > 2 && argv[1][0] != '-') {
		host = argv[1];
		argoff = 1;
	}

#ifdef KERBEROS
#define	OPTIONS	"8FKLdefk:l:nw"
#else
#define	OPTIONS	"8KLdel:nw"
#endif
	while ((ch = getopt(argc - argoff, argv + argoff, OPTIONS)) != EOF)
		switch(ch) {
		case 'K':
#ifdef KERBEROS
			use_kerberos = 0;
#endif
			break;
		case 'L':	/* -8Lew are ignored to allow rlogin aliases */
		case 'e':
		case 'w':
		case '8':
			break;
		case 'd':
			dflag = 1;
			break;
		case 'l':
			user = optarg;
			break;
#ifdef KERBEROS
		case 'k':
#ifndef KRBV
                        dest_realm = dst_realm_buf;
                        strncpy(dest_realm, optarg, REALM_SZ);
#else
		        if(!(krb_realm = (char *)malloc(strlen(optarg) + 1)))
		        	errx(1, "Cannot malloc.");
	        	strcpy(krb_realm, optarg);
			break;
		case 'f':
		        if (Fflag) {
		        	warnx("rsh: Only one of -f and -F allowed");
            			usage();
        		}
        		fflag++;
			break;
		case 'F':
		        if (fflag) {
            			warnx("rsh: Only one of -f and -F allowed");
            			usage();
        		}
        		Fflag++;
#endif
			break;
#endif
		case 'n':
			nflag = 1;
			break;
#ifdef KERBEROS
#endif
		case '?':
		default:
			usage();
		}
	optind += argoff;

	/* if haven't gotten a host yet, do so */
	if (!host && !(host = argv[optind++]))
		usage();

	/* if no further arguments, must have been called as rlogin. */
	if (!argv[optind]) {
		if (asrsh)
			*argv = "rlogin";
		execv(_PATH_RLOGIN, argv);
		err(1, "can't exec %s", _PATH_RLOGIN);
	}

	argc -= optind;
	argv += optind;

	if (!(pw = getpwuid(uid = getuid())))
		errx(1, "unknown user id");
	if (!user)
		user = pw->pw_name;

#ifdef KERBEROS
#endif

	args = copyargs(argv);

	sp = NULL;
#ifdef KERBEROS
	if (use_kerberos) {
		sp = getservbyname((doencrypt ? "ekshell" : "kshell"), "tcp");
		if (sp == NULL) {
			use_kerberos = 0;
			warning("can't get entry for %s/tcp service",
			    doencrypt ? "ekshell" : "kshell");
		}
	}
#endif
	if (sp == NULL)
		sp = getservbyname("shell", "tcp");
	if (sp == NULL)
		errx(1, "shell/tcp: unknown service");

#ifdef KERBEROS
#ifdef KRBV
	if (use_kerberos) {
            krb5_init_context(&bsd_context);
            krb5_init_ets(bsd_context);
            authopts = AP_OPTS_MUTUAL_REQUIRED;

            /* Piggy-back forwarding flags on top of authopts; */
            /* they will be reset in kcmd */
            if (fflag || Fflag)
                    authopts |= OPTS_FORWARD_CREDS;
            if (Fflag)
      		    authopts |= OPTS_FORWARDABLE_CREDS;
	}
#endif
try_connect:
	if (use_kerberos) {
#ifdef KRBV
    		status = kcmd(&rem, &host, sp->s_port,
                  	      pw->pw_name,
                  	      user,
                  	      args, &rfd2, "host", krb_realm,
                  	      &cred,
                  	      0,           /* No need for sequence number */
                  	      0,           /* No need for server seq # */
                  	      (struct sockaddr_in *) 0,
                  	      (struct sockaddr_in *) 0,
                  	      authopts);
		if (status) {
        		/* check NO_TKT_FILE or equivalent... */
        		fprintf(stderr,
                		"rsh: kcmd to host %s failed - %s\n",host,
                		error_message(status));
#else
                struct hostent *hp;

                /* fully qualify hostname (needed for krb_realmofhost) */
                hp = gethostbyname(host);
                if (hp != NULL && !(host = strdup(hp->h_name)))
                        err(1, NULL);

                rem = KSUCCESS;
                errno = 0;
                if (dest_realm == NULL)
                        dest_realm = krb_realmofhost(host);

                        rem = krcmd(&host, sp->s_port, user, args, &rfd2,
                            dest_realm);
                if (rem < 0) {
#endif
			use_kerberos = 0;
			sp = getservbyname("shell", "tcp");
			if (sp == NULL)
				errx(1, "shell/tcp: unknown service");
			if (errno == ECONNREFUSED)
				warning("remote host doesn't support Kerberos");
			if (errno == ENOENT)
				warning("can't provide Kerberos auth data");
			goto try_connect;
		}

#ifdef KRBV
    		/* Setup for des_read and write */
	        desinbuf.data = des_inbuf;
    		desoutbuf.data = des_outbuf;
    		krb5_use_keytype(bsd_context, &eblock,cred->keyblock.keytype);
    		if (status = krb5_process_key(bsd_context, &eblock,&cred->keyblock)) {
        		fprintf(stderr, "rsh: Cannot process session key : %s.\n",
                		error_message(status));
        		exit(1);
    		}
#endif
	} else {
		warnx("trying normal rsh");
		if (doencrypt)
			errx(1, "the -x flag requires Kerberos authentication");
		rem = rcmd(&host, sp->s_port, pw->pw_name, user, args, &rfd2);
	}
#else
	rem = rcmd(&host, sp->s_port, pw->pw_name, user, args, &rfd2);
#endif

	if (rem < 0)
		exit(1);

	if (rfd2 < 0)
		errx(1, "can't establish stderr");
	if (dflag) {
		if (setsockopt(rem, SOL_SOCKET, SO_DEBUG, &one,
		    sizeof(one)) < 0)
			warn("setsockopt");
		if (setsockopt(rfd2, SOL_SOCKET, SO_DEBUG, &one,
		    sizeof(one)) < 0)
			warn("setsockopt");
	}

	(void)setuid(uid);
#ifdef POSIX_SIGNALS
	sigemptyset(&igmask);
	sigaddset(&igmask, SIGINT);
	sigaddset(&igmask, SIGQUIT);
	sigaddset(&igmask, SIGTERM);
	sigprocmask(SIG_BLOCK, &igmask, &omask);

	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sendsig;

	(void) sigaction(SIGINT, (struct sigaction *)0, &osa);
	if (osa.sa_handler != SIG_IGN)
		(void) sigaction(SIGINT, &sa, (struct sigaction *)0);

	(void) sigaction(SIGQUIT, (struct sigaction *)0, &osa);
	if (osa.sa_handler != SIG_IGN)
		(void) sigaction(SIGQUIT, &sa, (struct sigaction *)0);

	(void) sigaction(SIGTERM, (struct sigaction *)0, &osa);
	if (osa.sa_handler != SIG_IGN)
		(void) sigaction(SIGTERM, &sa, (struct sigaction *)0);
#else	
	omask = sigblock(sigmask(SIGINT)|sigmask(SIGQUIT)|sigmask(SIGTERM));
	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
		(void)signal(SIGINT, sendsig);
	if (signal(SIGQUIT, SIG_IGN) != SIG_IGN)
		(void)signal(SIGQUIT, sendsig);
	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
		(void)signal(SIGTERM, sendsig);
#endif
	if (!nflag) {
		pid = fork();
		if (pid < 0)
			err(1, "fork");
	}
#ifdef DEBUG_CHILD
	{
        	int debug = 1;
	        while (debug)
       	        	;
	}
#endif

#ifdef KERBEROS
#endif
	{
		(void)ioctl(rfd2, FIONBIO, &one);
		(void)ioctl(rem, FIONBIO, &one);
	}

	talk(nflag, omask, pid, rem);

	if (!nflag)
		(void)kill(pid, SIGKILL);
	exit(0);
}

void
talk(nflag, omask, pid, rem)
	int nflag;
#ifdef POSIX_SIGNALS
	sigset_t omask;
#else
	long omask;
#endif
	pid_t pid;
	int rem;
{
	int cc, wc;
	fd_set readfrom, ready, rembits;
	char *bp, buf[BUFSIZ];

	if (!nflag && pid == 0) {
		(void)close(rfd2);

reread:		errno = 0;
		if ((cc = read(0, buf, sizeof buf)) <= 0)
			goto done;
		bp = buf;

rewrite:	
		FD_ZERO(&rembits);
		FD_SET(rem, &rembits);
		if (select(16, 0, &rembits, 0, 0) < 0) {
			if (errno != EINTR)
				err(1, "select");
			goto rewrite;
		}
		if (!FD_ISSET(rem, &rembits))
			goto rewrite;
#ifdef KERBEROS
#endif
			wc = write(rem, bp, cc);
		if (wc < 0) {
			if (errno == EWOULDBLOCK)
				goto rewrite;
			goto done;
		}
		bp += wc;
		cc -= wc;
		if (cc == 0)
			goto reread;
		goto rewrite;
done:
		(void)shutdown(rem, 1);
		exit(0);
	}

#ifdef POSIX_SIGNALS
	sigprocmask(SIG_SETMASK, &omask, (sigset_t *)0);
#else
	(void)sigsetmask(omask);
#endif
	FD_ZERO(&readfrom);
	FD_SET(rfd2, &readfrom);
	FD_SET(rem, &readfrom);
	do {
		ready = readfrom;
		if (select(16, &ready, 0, 0, 0) < 0) {
			if (errno != EINTR)
				err(1, "select");
			continue;
		}
		if (FD_ISSET(rfd2, &ready)) {
			errno = 0;
#ifdef KERBEROS
#endif
				cc = read(rfd2, buf, sizeof buf);
			if (cc <= 0) {
				if (errno != EWOULDBLOCK)
					FD_CLR(rfd2, &readfrom);
			} else
				(void)write(2, buf, cc);
		}
		if (FD_ISSET(rem, &ready)) {
			errno = 0;
#ifdef KERBEROS
#endif
				cc = read(rem, buf, sizeof buf);
			if (cc <= 0) {
				if (errno != EWOULDBLOCK)
					FD_CLR(rem, &readfrom);
			} else
				(void)write(1, buf, cc);
		}
	} while (FD_ISSET(rfd2, &readfrom) || FD_ISSET(rem, &readfrom));
}

void
sendsig(sig)
	int sig;
{
	char signo;

	signo = sig;
#ifdef KERBEROS
#endif
		(void)write(rfd2, &signo, 1);
}

#ifdef KERBEROS
/* VARARGS */
#ifdef HAVE_STDARG_H
void warning(char *fmt, ...)
#else
void
warning(va_alist)
va_dcl
#endif
{
	va_list ap;
#ifndef HAVE_STDARG_H
	char *fmt;
#endif

	(void)fprintf(stderr, "rsh: warning, using standard rsh: ");
#ifdef HAVE_STDARG_H
	va_start(ap, fmt);
#else
	va_start(ap);
	fmt = va_arg(ap, char *);
#endif
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, ".\n");
}
#endif

char *
copyargs(argv)
	char **argv;
{
	int cc;
	char **ap, *args, *p;

	cc = 0;
	for (ap = argv; *ap; ++ap)
		cc += strlen(*ap) + 1;
	if (!(args = malloc((u_int)cc)))
		err(1, NULL);
	for (p = args, ap = argv; *ap; ++ap) {
		(void)strcpy(p, *ap);
		for (p = strcpy(p, *ap); *p; ++p);
		if (ap[1])
			*p++ = ' ';
	}
	return (args);
}

void
usage()
{

	(void)fprintf(stderr,
	    "usage: rsh [-nd%s] [-l login] host [command]\n",
#ifdef KERBEROS
	    "fFK");
#else
	    "");
#endif
	exit(1);
}
