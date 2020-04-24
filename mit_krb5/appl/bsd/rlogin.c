/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rlogin.c,v $
 * Revision 1.1.2.3  1996/10/03  14:39:10  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/07/09  19:47 UTC  mullan_s  /main/DCE_1.2.2/2]
 *
 * 	CHFts19907: Fix flow control.
 * 	[1996/08/28  20:36 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/6]
 *
 * 	Fix rlogin -x hang on hpux.
 * 	[1996/08/27  19:18 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/5]
 *
 * 	remove unnecessary ifdefs - makes code harder to read.
 * 	[1996/08/26  18:24 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/3]
 *
 * 	Fix usage synopis; add extra debugging statements.
 * 	[1996/06/18  14:42 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
 *
 * 	Fix ~^Z escapechar bug.
 * 	Fix window resizing problems.
 * 	[1996/06/14  14:58 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 *
 * 	Fix rlogin TERM vulnerability, use strncpy/strncat to avoid buffer overflow.
 * 	[1996/05/30  21:16 UTC  mullan_s  /main/DCE_1.2.2/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:35 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 *
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:08 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/8]
 *
 * 	-l option not passing local username to kcmd() correctly.
 * 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/7]
 *
 * 	Initialize dosigwinch.
 * 	[1996/04/29  14:28 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:28 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 *
 * Revision 1.1.2.2  1996/08/09  12:02:14  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/06/18  14:42 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
 * 
 * 	Fix ~^Z escapechar bug.
 * 	Fix window resizing problems.
 * 	[1996/06/14  14:58 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 * 
 * 	Fix rlogin TERM vulnerability, use strncpy/strncat to avoid buffer overflow.
 * 	[1996/05/30  21:16 UTC  mullan_s  /main/DCE_1.2.2/1]
 * 
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:35 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 * 
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:08 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/8]
 * 
 * 	-l option not passing local username to kcmd() correctly.
 * 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/7]
 * 
 * 	Initialize dosigwinch.
 * 	[1996/04/29  14:28 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 * 
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:28 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 * 
 * Revision 1.1.2.1  1996/06/04  21:47:43  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:35 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 * 
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:08 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/8]
 * 
 * 	-l option not passing local username to kcmd() correctly.
 * 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/7]
 * 
 * 	Initialize dosigwinch.
 * 	[1996/04/29  14:28 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 * 
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:28 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/6]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1983, 1990, 1993
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
"@(#) Copyright (c) 1983, 1990, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)rlogin.c	8.1 (Berkeley) 6/6/93";
#endif /* not lint */

/*
 * rlogin - remote login
 */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pwd.h>
#include <setjmp.h>
#ifdef POSIX_TERMIOS
#include <termios.h>
#ifndef CNUL
#define CNUL (char) 0
#endif
#else /* POSIX_TERMIOS */
#include <sgtty.h>
#endif /* POSIX_TERMIOS */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#ifdef DEBUG
#define DEEDEBUG(A) fprintf(stderr,A)
#define DEEDEBUG2(A,B) fprintf(stderr,A,B)
#else
#define DEEDEBUG(A)
#define DEEDEBUG2(A,B)
#endif

#ifdef KERBEROS
#ifdef KRBV
#include "krb5.h"
#include "com_err.h"

#define OPTS_FORWARD_CREDS           0x00000002
#define OPTS_FORWARDABLE_CREDS       0x00000001
#define RLOGIN_BUFSIZ 4096

char des_inbuf[2*RLOGIN_BUFSIZ];       /* needs to be > largest read size */
char des_outbuf[2*RLOGIN_BUFSIZ];      /* needs to be > largest write size */
krb5_data desinbuf,desoutbuf;
krb5_encrypt_block eblock;      /* eblock for encrypt/decrypt */
char *krb_realm = (char *)0;
int fflag = 0, Fflag = 0;
krb5_creds *cred;
struct sockaddr_in local, foreign;
krb5_context bsd_context;
#else
#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>

#include "krb.h"

CREDENTIALS cred;
Key_schedule schedule;
char dst_realm_buf[REALM_SZ], *dest_realm = NULL;
#endif
int use_kerberos = 1, doencrypt = 0;
#endif

#ifndef TIOCPKT_WINDOW
#define	TIOCPKT_WINDOW	0x80
#endif

/* concession to Sun */
#ifndef SIGUSR1
#define	SIGUSR1	30
#endif

#ifdef POSIX_TERMIOS
struct termios deftty;
#endif

int eight, litout, rem;

int noescape;
u_char escapechar = '~';

#if defined(__hpux) || defined(hpux)
char *speeds[] = {
	"0", "50", "75", "110", "134", "150", "200", "300", "600", "900",
	"1200", "1800", "2400", "3600", "4800", "7200", "9600", "19200", 
	"38400", "EXTA", "EXTB"
};
#else
char *speeds[] = {
	"0", "50", "75", "110", "134", "150", "200", "300", "600", "1200",
	"1800", "2400", "4800", "9600", "19200", "38400"
};
#endif

#ifndef POSIX_SIGNALS
#ifndef sigmask
#define sigmask(m)	(1 << ((m)-1))
#endif
#endif /* POSIX_SIGNALS */

#ifdef OLDSUN
struct winsize {
	unsigned short ws_row, ws_col;
	unsigned short ws_xpixel, ws_ypixel;
};
#else
#define	get_window_size(fd, wp)	ioctl(fd, TIOCGWINSZ, wp)
#endif
struct	winsize winsize;

void		catch_child(int);
void		copytochild(int);
#ifdef POSIX_SIGNALS
void		doit(sigset_t *);
#else
void		doit(long);
#endif
void		done(int);
void		echo(char);
u_int		getescape(char *);
void		lostpeer(int);
void		mode(int);
void		msg(char *);
void		oob(int);
#ifdef POSIX_SIGNALS
int		reader(sigset_t *);
#else
int		reader(int);
#endif
void		sendwindow(void);
void		setsignal(int);
void		sigwinch(int);
void		stop(char);
void		usage(void);
void		writer(void);
void		writeroob(int);

#ifdef	KERBEROS
void		warning (const char *, ...);
#endif
#ifdef OLDSUN
int		get_window_size (int, struct winsize *);
#endif

#ifdef POSIX_TERMIOS
/* Globals for terminal modes and flow control */
struct termios defmodes;
struct termios ixon_state;
#endif

int
main(argc, argv)
	int argc;
	char *argv[];
{
	extern char *optarg;
	extern int optind;
	struct passwd *pw;
	struct servent *sp;
#ifdef POSIX_TERMIOS
	struct termios ttyb;
#else
	struct sgttyb ttyb;
#endif
#ifdef POSIX_SIGNALS
	struct sigaction sa;
	sigset_t oldmask, *omask, urgmask;
#else
	long omask;
#endif
	int argoff, ch, dflag, one, uid;
	char *host, *p, *user, term[1024];
#if defined(KERBEROS) && defined(KRBV)
        int sock;
        krb5_flags authopts;
        krb5_error_code status;
#endif

	argoff = dflag = 0;
	one = 1;
	host = user = NULL;

	if (p = rindex(argv[0], '/'))
		++p;
	else
		p = argv[0];

	if (strcmp(p, "rlogin"))
		host = p;

	/* handle "rlogin host flags" */
	if (!host && argc > 2 && argv[1][0] != '-') {
		host = argv[1];
		argoff = 1;
	}

#ifdef KERBEROS
#define	OPTIONS	"8EFKLde:fk:l:"
#else
#define	OPTIONS	"8EKLde:l:"
#endif
	while ((ch = getopt(argc - argoff, argv + argoff, OPTIONS)) != EOF)
		switch(ch) {
		case '8':
			eight = 1;
			break;
		case 'E':
			noescape = 1;
			break;
		case 'K':
#ifdef KERBEROS
			use_kerberos = 0;
#endif
			break;
		case 'L':
			litout = 1;
			break;
		case 'd':
			dflag = 1;
			break;
		case 'e':
			noescape = 0;
			escapechar = getescape(optarg);
			break;
#ifdef KERBEROS
		case 'k':
#ifndef KRBV
                        dest_realm = dst_realm_buf;
                        (void)strncpy(dest_realm, optarg, REALM_SZ);
#else
		        if(!(krb_realm = (char *)malloc(strlen(optarg) + 1))){
	            		fprintf(stderr, "rlogin: Cannot malloc.\n");
		        	exit(1);
        		}
        		strcpy(krb_realm, optarg);
			break;
		case 'f':
		        if (Fflag) {
	        		fprintf(stderr, "rlogin: Only one of -f and -F allowed\n");
		        	usage();
        		}
		        fflag++;
			break;
		case 'F':
		        if (fflag) {
        			fprintf(stderr, "rlogin: Only one of -f and -F allowed\n");
        			usage();
        		}
        		Fflag++;
#endif
			break;
#endif
		case 'l':
			user = optarg;
			break;
		case '?':
		default:
			usage();
		}
	optind += argoff;
	argc -= optind;
	argv += optind;

	/* if haven't gotten a host yet, do so */
	if (!host && !(host = *argv++))
		usage();

	if (*argv)
		usage();

	if (!(pw = getpwuid(uid = getuid()))) {
		(void)fprintf(stderr, "rlogin: unknown user id.\n");
		exit(1);
	}
	if (!user)
		user = pw->pw_name;

	sp = NULL;
#ifdef KERBEROS
	if (use_kerberos) {
#ifdef KRBV
	        krb5_init_context(&bsd_context);
        	krb5_init_ets(bsd_context);
        	desinbuf.data = des_inbuf;
                desoutbuf.data = des_outbuf;        /* Set up des buffers */
#endif
		sp = getservbyname((doencrypt ? "eklogin" : "klogin"), "tcp");
		if (sp == NULL) {
			use_kerberos = 0;
			warning("can't get entry for %s/tcp service",
			    doencrypt ? "eklogin" : "klogin");
		}
	}
#endif
	if (sp == NULL)
		sp = getservbyname("login", "tcp");
	if (sp == NULL) {
		(void)fprintf(stderr, "rlogin: login/tcp: unknown service.\n");
		exit(1);
	}

	(void)strncpy(term, (p = getenv("TERM")) ? p : "network", sizeof(term) - 1);
	term[sizeof(term) - 1] = '\0';
#ifdef POSIX_TERMIOS
	if (tcgetattr(0, &ttyb) == 0) {
		int ospeed = cfgetospeed(&ttyb);

		(void) strncat(term, "/", sizeof(term) - strlen(term));
	        term[sizeof(term) - 1] = '\0';
#if defined(__hpux) || defined(hpux)
                (void)strncat(term, speeds[ttyb.c_cflag & CBAUD], sizeof(term) - strlen(term));
	        term[sizeof(term) - 1] = '\0';
#else
		(void) strncat(term, speeds[ospeed], sizeof(term) - strlen(term));
                term[sizeof(term) - 1] = '\0';
#endif
	}
#else
	if (ioctl(0, TIOCGETP, &ttyb) == 0) {
		(void)strncat(term, "/", sizeof(term) - strlen(term));
                term[sizeof(term) - 1] = '\0';
		(void)strncat(term, speeds[(int)ttyb.sg_ospeed], sizeof(term) - strlen(term));
                term[sizeof(term) - 1] = '\0';
	}
#endif

	(void)get_window_size(0, &winsize);

#ifdef POSIX_TERMIOS
	tcgetattr(0, &defmodes);
	tcgetattr(0, &ixon_state);
#endif

#ifdef POSIX_SIGNALS
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = lostpeer;
	(void) sigaction(SIGPIPE, &sa, (struct sigaction *)0);

	(void) sigemptyset(&urgmask);
	(void) sigaddset(&urgmask, SIGURG);
	(void) sigaddset(&urgmask, SIGUSR1);
	omask = &oldmask;
	(void) sigprocmask(SIG_BLOCK, &urgmask, omask);
#else
	(void)signal(SIGPIPE, lostpeer);
	/* will use SIGUSR1 for window size hack, so hold it off */
	omask = sigblock(sigmask(SIGURG) | sigmask(SIGUSR1));
	/*
	 * We set SIGURG and SIGUSR1 below so that an
	 * incoming signal will be held pending rather than being
	 * discarded. Note that these routines will be ready to get
	 * a signal by the time that they are unblocked below.
	 */
	(void)signal(SIGURG, copytochild);
	(void)signal(SIGUSR1, writeroob);
#endif

#ifdef KERBEROS
try_connect:
        if (use_kerberos) {
#ifdef KRBV
        	authopts = AP_OPTS_MUTUAL_REQUIRED;

	        /* Piggy-back forwarding flags on top of authopts; */
       		/* they will be reset in kcmd */
 	        if (fflag || Fflag)
        		authopts |= OPTS_FORWARD_CREDS;
	        if (Fflag)
       	 		authopts |= OPTS_FORWARDABLE_CREDS;

	        status = kcmd(&sock, &host, sp->s_port,
       	              	      pw->pw_name,
                              user, term,
                              0, "host", krb_realm,
                              &cred,
                              0,            /* No need for sequence number */
                              0,            /* No need for server seq # */
                              &local, &foreign,
                              authopts);
                if (status) {
        		/* should check for KDC_PR_UNKNOWN, NO_TKT_FILE here -- XXX */
        		fprintf(stderr,
                		"rlogin: kcmd to host %s failed - %s\n", host,
                		error_message(status));
#else
                struct hostent *hp;

                /* Fully qualify hostname (needed for krb_realmofhost). */
                hp = gethostbyname(host);
                if (hp != NULL && !(host = strdup(hp->h_name))) {
                        (void)fprintf(stderr, "rlogin: %s\n",
                            strerror(ENOMEM));
                        exit(1);
                }

                rem = KSUCCESS;
                errno = 0;
                if (dest_realm == NULL)
                        dest_realm = krb_realmofhost(host);

                        rem = krcmd(&host, sp->s_port, user, term, 0,
                            dest_realm);
                if (rem < 0) {
#endif /* KRBV */
                        use_kerberos = 0;
                        sp = getservbyname("login", "tcp");
                        if (sp == NULL) {
                                (void)fprintf(stderr,
                                    "rlogin: unknown service login/tcp.\n");
                                exit(1);
                        }
                        if (errno == ECONNREFUSED)
                                warning("remote host doesn't support Kerberos");
                        if (errno == ENOENT)
                                warning("can't provide Kerberos auth data");
                        goto try_connect;
		}
#ifdef KRBV
		rem = sock;

		/* setup eblock for des_read and write */
		krb5_use_keytype(bsd_context, &eblock,cred->keyblock.keytype);
		if ( status = krb5_process_key(bsd_context, &eblock,&cred->keyblock)) {
			fprintf(stderr,
                		"rlogin: Cannot process session key : %s.\n",
		                error_message(status));
			exit(1);
    		}
#endif
	} else {
		rem = rcmd(&host, sp->s_port, pw->pw_name, user, term, 0);
	}
#else
	rem = rcmd(&host, sp->s_port, pw->pw_name, user, term, 0);
#endif /* KERBEROS */

	if (rem < 0)
		exit(1);

    /* we need to do the SETOWN here so that we get the SIGURG
       registered if the URG data come in early, before the reader() gets
       to do this for real (otherwise, the signal is never generated
       by the kernel).  We block it above, so when it gets unblocked
       it will get processed by the reader().
       There is a possibility that the signal will get delivered to both
       writer and reader, but that is harmless, since the writer reflects
       it to the reader, and the oob() processing code in the reader will
       work properly even if it is called when no oob() data is present.
       */
#if defined(__hpux) || defined(hpux)
        /* hpux invention */
        {
          int pid = getpid();
          ioctl(rem, FIOSSAIOSTAT, &pid); /* trick: pid is non-zero */
          ioctl(rem, FIOSSAIOOWN, &pid);
        }
#else
#ifdef HAVE_SETOWN
    	(void) fcntl(rem, F_SETOWN, getpid());
#else
	{
		int pid = getpid();
    		ioctl(rem, SIOCSPGRP, &pid);
	}
#endif /* HAVE_SETOWN */
#endif /* hpux */

	if (dflag &&
	    setsockopt(rem, SOL_SOCKET, SO_DEBUG, &one, sizeof(one)) < 0)
		(void)fprintf(stderr, "rlogin: setsockopt: %s.\n",
		    strerror(errno));

	(void)setuid(uid);
	doit(omask);
	/*NOTREACHED*/
}

int child, defflags, deflflags, tabflag;
char deferase, defkill;

#ifdef TIOCGLTC
/*
 * POSIX 1003.1-1988 does not define a 'suspend' character.
 * POSIX 1003.1-1990 does define an optional VSUSP but not a VDSUSP character.
 * Some termio implementations (A/UX, Ultrix 4.2) include both.
 *
 * However, since this is all derived from the BSD ioctl() and ltchars
 * concept, all these implementations generally also allow for the BSD-style
 * ioctl().  So we'll simplify the problem by only testing for the ioctl().
 */
struct  ltchars defltc;
struct  ltchars noltc = { -1, -1, -1, -1, -1, -1 };
#endif

#ifndef POSIX_TERMIOS
struct tchars deftc;
struct ltchars defltc;
#ifndef TIOCGLTC
struct tchars notc = { -1, -1, -1, -1, -1, -1 };
struct ltchars noltc = { -1, -1, -1, -1, -1, -1 };
#endif
#endif

void
doit(omask)
#ifdef POSIX_SIGNALS
	sigset_t *omask;
#else
	long omask;
#endif
{
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif

#ifdef POSIX_TERMIOS
	(void) tcgetattr(0, &deftty);
#ifdef VLNEXT
	deftty.c_cc[VLNEXT] = 0;
#endif
#else
	struct sgttyb sb;

	(void)ioctl(0, TIOCGETP, (char *)&sb);
	defflags = sb.sg_flags;
	tabflag = defflags & TBDELAY;
	defflags &= ECHO | CRMOD;
	deferase = sb.sg_erase;
	defkill = sb.sg_kill;
	(void)ioctl(0, TIOCLGET, &deflflags);
	(void)ioctl(0, TIOCGETC, &deftc);
	notc.t_startc = deftc.t_startc;
	notc.t_stopc = deftc.t_stopc;
	(void)ioctl(0, TIOCGLTC, &defltc);
#endif
#ifdef POSIX_SIGNALS
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGINT, &sa, (struct sigaction *)0);
#else
	(void)signal(SIGINT, SIG_IGN);
#endif

	setsignal(SIGHUP);
	setsignal(SIGQUIT);
	child = fork();
	if (child == -1) {
		(void)fprintf(stderr, "rlogin: fork: %s.\n", strerror(errno));
		done(1);
	}

	mode(1);

	if (child == 0) {
#ifdef DEBUG_CHILD
		int debug=1;
		while (debug)
			;
#endif
		if (reader(omask) == 0) {
			msg("connection closed.");
			exit(0);
		}
		sleep(1);
		msg("\007connection closed.");
		exit(1);
	}

	/*
	 * We may still own the socket, and may have a pending SIGURG (or might
	 * receive one soon) that we really want to send to the reader.  When
	 * one of these comes in, the trap copytochild simply copies such
	 * signals to the child. We can now unblock SIGURG and SIGUSR1
	 * that were set above.
	 */
#ifdef POSIX_SIGNALS
	sa.sa_handler = copytochild;
	(void) sigaction(SIGURG, &sa, (struct sigaction *)0);
	
	sa.sa_handler = writeroob;
	(void) sigaction(SIGUSR1, &sa, (struct sigaction *)0);

	sigprocmask(SIG_SETMASK, omask, (sigset_t*)0);

	sa.sa_handler = catch_child;
	(void) sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
	(void)sigsetmask(omask);
	(void)signal(SIGCHLD, catch_child);
#endif 
	writer();
	msg("closed connection.");
	done(0);
}

/* trap a signal, unless it is being ignored. */
void
setsignal(sig)
	int sig;
{
#ifdef POSIX_SIGNALS
	sigset_t omask, igmask;
	struct sigaction sa;

	sigemptyset(&igmask);
	sigaddset(&igmask, sig);
	sigprocmask(SIG_BLOCK, &igmask, &omask);
#else
	int omask = sigblock(sigmask(sig));
#endif

#ifdef POSIX_SIGNALS
	(void) sigaction(sig, (struct sigaction *)0, &sa);
	if (sa.sa_handler != SIG_IGN) {
		(void) sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = exit;
		(void) sigaction(sig, &sa, (struct sigaction *)0);
	}
	sigprocmask(SIG_SETMASK, &omask, (sigset_t*)0);
#else
	if (signal(sig, exit) == SIG_IGN)
		(void)signal(sig, SIG_IGN);
	(void)sigsetmask(omask);
#endif
}

void
done(status)
	int status;
{
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif
	int w, wstatus;

	mode(0);
	if (child > 0) {
		/* make sure catch_child does not snap it up */
#ifdef POSIX_SIGNALS
		(void) sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		(void) sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
		(void)signal(SIGCHLD, SIG_DFL);
#endif
		if (kill(child, SIGKILL) >= 0)
#ifdef HAVE_WAITPID
			(void) waitpid(child, 0, 0);
#else
			while ((w = wait(&wstatus)) > 0 && w != child);
#endif
	}
	exit(status);
}

int dosigwinch = 0;

/*
 * This is called when the reader process gets the out-of-band (urgent)
 * request to turn on the window-changing protocol.
 */
void
writeroob(signo)
	int signo;
{
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif

	if (dosigwinch == 0) {
		sendwindow();
#ifdef POSIX_SIGNALS
		(void) sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = sigwinch;
		(void) sigaction(SIGWINCH, &sa, (struct sigaction *)0);
#else
		(void)signal(SIGWINCH, sigwinch);
#endif
	}
	dosigwinch = 1;
}

void
catch_child(signo)
	int signo;
{
#ifdef WAIT_USES_INT
	int status;
#else
	union wait status;
#endif
	int pid;

	for (;;) {
#ifdef HAVE_WAITPID
		pid = waitpid(-1, &status, WNOHANG|WUNTRACED);
#else
		pid = wait3((int *)&status, WNOHANG|WUNTRACED, NULL);
#endif
		if (pid == 0)
			return;
		/* if the child (reader) dies, just quit */
#ifdef WAIT_USES_INT
		if (pid < 0 || (pid == child && !WIFSTOPPED(status)))
			done(status);
#else
#if defined(hpux) || defined(__hpux)
		if (pid < 0 || (pid == child && (!WIFSTOPPED(status.w_stopval))))
#else
		if (pid < 0 || (pid == child && !WIFSTOPPED(status)))
#endif
			done((int)(status.w_termsig | status.w_retcode));
#endif
	}
	/* NOTREACHED */
}

/*
 * writer: write to remote: 0 -> line.
 * ~.				terminate
 * ~^Z				suspend rlogin process.
 * ~<delayed-suspend char>	suspend rlogin process, but leave reader alone.
 */
void
writer()
{
	register int bol, local, n;
	char c;

	bol = 1;			/* beginning of line */
	local = 0;
	for (;;) {
		n = read(STDIN_FILENO, &c, 1);
		if (n <= 0) {
			if (n < 0 && errno == EINTR)
				continue;
			break;
		}
		/*
		 * If we're at the beginning of the line and recognize a
		 * command character, then we echo locally.  Otherwise,
		 * characters are echo'd remotely.  If the command character
		 * is doubled, this acts as a force and local echo is
		 * suppressed.
		 */
		if (bol) {
			bol = 0;
			if (!noescape && c == escapechar) {
				local = 1;
				continue;
			}
		} else if (local) {
			local = 0;
#ifdef POSIX_TERMIOS
			if (c == '.' || c == deftty.c_cc[VEOF]) {
#else
			if (c == '.' || c == deftc.t_eofc) {
#endif
				echo(c);
				break;
			}
#ifdef POSIX_TERMIOS
			if (c == deftty.c_cc[VSUSP]
#ifdef VDSUSP
			      || (c == deftty.c_cc[VDSUSP])
#endif
			      ) {
				bol = 1;
				echo(c);
				stop(c);
				continue;
			}
#else
#ifdef TIOCGLTC
			if (c == defltc.t_suspc || c == defltc.t_dsuspc) {
				bol = 1;
				echo(c);
				stop(c);
				continue;
			}
#endif
#endif
			if (c != escapechar)
					(void)write(rem, &escapechar, 1);
		}

			if (write(rem, &c, 1) == 0) {
				msg("line gone");
				break;
			}
#ifdef POSIX_TERMIOS
		bol = (c == deftty.c_cc[VKILL] ||
		       c == deftty.c_cc[VINTR] ||
		       c == '\r' || c == '\n');
#else
		bol = c == defkill || c == deftc.t_eofc ||
		    c == deftc.t_intrc || c == defltc.t_suspc ||
		    c == '\r' || c == '\n';
#endif
	}
}

void
#if __STDC__
echo(register char c)
#else
echo(c)
	register char c;
#endif
{
	register char *p;
	char buf[8];

	p = buf;
	c &= 0177;
	*p++ = escapechar;
	if (c < ' ') {
		*p++ = '^';
		*p++ = c + '@';
	} else if (c == 0177) {
		*p++ = '^';
		*p++ = '?';
	} else
		*p++ = c;
	*p++ = '\r';
	*p++ = '\n';
	(void)write(STDOUT_FILENO, buf, p - buf);
}

void
#if __STDC__
stop(char cmdc)
#else
stop(cmdc)
	char cmdc;
#endif
{
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif

	DEEDEBUG("rlogin: stop command received\n");
	mode(0);
#ifdef POSIX_SIGNALS
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
	(void)signal(SIGCHLD, SIG_IGN);
#endif

#ifdef POSIX_TERMIOS
	(void)kill(cmdc == deftty.c_cc[VSUSP] ? 0 : getpid(), SIGTSTP);
#else
#ifdef TIOCGLTC
	(void)kill(cmdc == defltc.t_suspc ? 0 : getpid(), SIGTSTP);
#endif
#endif

#ifdef POSIX_SIGNALS
	sa.sa_handler = catch_child;
	(void) sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
	(void)signal(SIGCHLD, catch_child);
#endif
	mode(1);
	sigwinch(0);			/* check for size changes */
}

void
sigwinch(signo)
	int signo;
{
	struct winsize ws;

	if (dosigwinch && get_window_size(0, &ws) == 0 &&
	    bcmp(&ws, &winsize, sizeof(ws))) {
		winsize = ws;
		sendwindow();
	}
}

/*
 * Send the window size to the server via the magic escape
 */
void
sendwindow()
{
	struct winsize *wp;
	char obuf[4 + sizeof (struct winsize)];

	wp = (struct winsize *)(obuf+4);
	obuf[0] = 0377;
	obuf[1] = 0377;
	obuf[2] = 's';
	obuf[3] = 's';
	wp->ws_row = htons(winsize.ws_row);
	wp->ws_col = htons(winsize.ws_col);
	wp->ws_xpixel = htons(winsize.ws_xpixel);
	wp->ws_ypixel = htons(winsize.ws_ypixel);

		(void)write(rem, obuf, sizeof(obuf));
}

/*
 * reader: read from remote: line -> 1
 */
#define	READING	1
#define	WRITING	2

#ifdef POSIX_SETJMP
sigjmp_buf rcvtop;
#else
jmp_buf rcvtop;
#endif
int ppid, rcvcnt, rcvstate;
char rcvbuf[8 * 1024];

void
oob(signo)
	int signo;
{
#ifdef POSIX_TERMIOS
	struct termios tty;
#else
	struct sgttyb sb;
#endif
	int atmark, n, out, rcvd;
	char waste[BUFSIZ], mark;

	DEEDEBUG("rlogin(oob): SIGURG signal received\n");
	out = O_RDWR;
	rcvd = 0;
	while (recv(rem, &mark, 1, MSG_OOB) < 0) {
		switch (errno) {
		case EWOULDBLOCK:
			/*
			 * Urgent data not here yet.  It may not be possible
			 * to send it yet if we are blocked for output and
			 * our input buffer is full.
			 */
			if (rcvcnt < sizeof(rcvbuf)) {
				n = read(rem, rcvbuf + rcvcnt,
				    sizeof(rcvbuf) - rcvcnt);
				if (n <= 0)
					return;
				rcvd += n;
			} else {
				n = read(rem, waste, sizeof(waste));
				if (n <= 0)
					return;
			}
			continue;
		default:
			return;
		}
	}
	if (mark & TIOCPKT_WINDOW) {
		/* Let server know about window size changes */
		(void)kill(ppid, SIGUSR1);
	}
#ifdef POSIX_TERMIOS
	if (!eight && (mark & TIOCPKT_NOSTOP)) {
		DEEDEBUG("rlogin: TIOCPKT_NOSTOP oob message received\n");
		(void) tcgetattr(0, &tty);
		tty.c_iflag &= ~IXON;
		(void) tcsetattr(0, TCSADRAIN, &tty);
	}
	if (!eight && (mark & TIOCPKT_DOSTOP)) {
		DEEDEBUG("rlogin: TIOCPKT_DOSTOP oob message received\n");
		(void) tcgetattr(0, &tty);
		tty.c_iflag |= IXON;
		(void) tcsetattr(0, TCSADRAIN, &tty);
	}
#else
	if (!eight && (mark & TIOCPKT_NOSTOP)) {
		(void)ioctl(0, TIOCGETP, (char *)&sb);
		sb.sg_flags &= ~CBREAK;
		sb.sg_flags |= RAW;
		(void)ioctl(0, TIOCSETN, (char *)&sb);
		notc.t_stopc = -1;
		notc.t_startc = -1;
		(void)ioctl(0, TIOCSETC, (char *)&notc);
	}
	if (!eight && (mark & TIOCPKT_DOSTOP)) {
		(void)ioctl(0, TIOCGETP, (char *)&sb);
		sb.sg_flags &= ~RAW;
		sb.sg_flags |= CBREAK;
		(void)ioctl(0, TIOCSETN, (char *)&sb);
		notc.t_stopc = deftc.t_stopc;
		notc.t_startc = deftc.t_startc;
		(void)ioctl(0, TIOCSETC, (char *)&notc);
	}
#endif
	if (mark & TIOCPKT_FLUSHWRITE) {
#ifdef POSIX_TERMIOS
		(void) tcflush(1, TCOFLUSH);
#else
		(void)ioctl(1, TIOCFLUSH, (char *)&out);
#endif
		for (;;) {
			if (ioctl(rem, SIOCATMARK, &atmark) < 0) {
				(void)fprintf(stderr, "rlogin: ioctl: %s.\n",
				    strerror(errno));
				break;
			}
			if (atmark)
				break;
			n = read(rem, waste, sizeof (waste));
			if (n <= 0)
				break;
		}
		/*
		 * Don't want any pending data to be output, so clear the recv
		 * buffer.  If we were hanging on a write when interrupted,
		 * don't want it to restart.  If we were reading, restart
		 * anyway.
		 */
		rcvcnt = 0;
#ifdef POSIX_SETJMP
		siglongjmp(rcvtop, 1);
#else
		longjmp(rcvtop, 1);
#endif
	}

	/* oob does not do FLUSHREAD (alas!) */

	/*
	 * If we filled the receive buffer while a read was pending, longjmp
	 * to the top to restart appropriately.  Don't abort a pending write,
	 * however, or we won't know how much was written.
	 */
#ifdef POSIX_SETJMP
	if (rcvd && rcvstate == READING)
		siglongjmp(rcvtop, 1);
#else
	if (rcvd && rcvstate == READING)
		longjmp(rcvtop, 1);
#endif
}

/* reader: read from remote: line -> 1 */
int
reader(omask)
#ifdef POSIX_SIGNALS
	sigset_t *omask;
#else
	int omask;
#endif
{
	int pid, n, remaining;
	char *bufp;

	pid = getpid();
	if (pid < 0)
		pid = -pid;	/* old broken systems use negatives */

#ifdef POSIX_SIGNALS
{
	struct sigaction sa;

	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGTTOU, &sa, (struct sigaction *)0);
#ifdef SA_RESTART
	/* Because SIGURG will be coming in during a read,
	 * we want to restart the syscall afterwards. */
	sa.sa_flags |= SA_RESTART;
#endif
	sa.sa_handler = oob;
	(void) sigaction(SIGURG, &sa, (struct sigaction *)0);
}
#else
	(void)signal(SIGTTOU, SIG_IGN);
	(void)signal(SIGURG, oob);
#endif

	ppid = getppid();
#ifdef HAVE_SETOWN
	(void)fcntl(rem, F_SETOWN, pid);
#else
            ioctl(rem, SIOCSPGRP, &pid);
#endif

#ifdef POSIX_SETJMP
	(void) sigsetjmp(rcvtop, 1);
#else
	(void)setjmp(rcvtop);
#endif
#ifdef POSIX_SIGNALS
	sigprocmask(SIG_SETMASK, omask, (sigset_t*)0);
#else
	(void)sigsetmask(omask);
#endif
	bufp = rcvbuf;
	for (;;) {
		while ((remaining = rcvcnt - (bufp - rcvbuf)) > 0) {
			rcvstate = WRITING;
			n = write(STDOUT_FILENO, bufp, remaining);
			if (n < 0) {
				if (errno != EINTR)
					return (-1);
				continue;
			}
			bufp += n;
		}
		bufp = rcvbuf;
		rcvcnt = 0;
		rcvstate = READING;

			rcvcnt = read(rem, rcvbuf, sizeof (rcvbuf));
		if (rcvcnt == 0)
			return (0);
		if (rcvcnt < 0) {
			if (errno == EINTR)
				continue;
			(void)fprintf(stderr, "rlogin: read: %s.\n",
			    strerror(errno));
			return (-1);
		}
	}
}

void
mode(f)
	int f;
{
	struct ltchars *ltc;
#ifdef POSIX_TERMIOS
	struct termios newtty;
#if defined(__hpux) || defined(hpux)
	static struct termios ixon_state;
	static int first = 1;

        /* store initial state of IXON bit */
        if (first) {
            (void) tcgetattr(0, &ixon_state);
            first = 0;
        }
#endif

	switch(f) {
		case 0:
#ifdef TIOCGLTC
			(void) ioctl(0, TIOCSLTC, (char *)&defltc);
#endif
			(void) tcsetattr(0, TCSADRAIN, &deftty);
#if defined(__hpux) || defined(hpux)
        	        /*
               	 	 * remember whether IXON was set, so it can be restored
                 	 * when mode(1) is next done
                 	 */
	                (void) tcgetattr(0, &ixon_state);
#endif
			break;
		case 1:
			(void) tcgetattr(0, &newtty);
			newtty.c_lflag &= ~(ICANON|ISIG|ECHO);
/*			if (!flow) {
				newtty.c_lflag &= ~(ICANON|ISIG|ECHO);
				newtty.c_iflag &= ~(BRKINT|INLCR|ICRNL|ISTRIP);
				/* newtty.c_iflag |= ~(IXON|IXANY); *
				newtty.c_iflag &= ~(IXON|IXANY);
				newtty.c_oflag &= ~(OPOST);
			} else {*/
				newtty.c_lflag &= ~(ICANON|ISIG|ECHO);
#if defined(__hpux) || defined(hpux)
				newtty.c_iflag &= ~(INLCR|ICRNL|BRKINT);
				newtty.c_oflag &= ~(ONLCR|OCRNL);
				if (eight)
					newtty.c_iflag &= ~(ISTRIP);
				/* restore current flow control state */
				if (ixon_state.c_iflag & IXON) {
					newtty.c_iflag |= IXON;
				} else {
					newtty.c_iflag &= ~IXON;
				}
#else
				newtty.c_iflag &= ~(INLCR|ICRNL);
				/* newtty.c_iflag |= ~(BRKINT|ISTRIP|IXON|IXANY); */
				newtty.c_iflag &= ~(IXON|IXANY);
				newtty.c_iflag |= (BRKINT|ISTRIP);
				newtty.c_oflag &= ~(ONLCR|ONOCR);
				newtty.c_oflag |= (OPOST);
#endif

#ifdef TABDLY
	                /* preserve tab delays, but turn off XTABS */
	                if ((newtty.c_oflag & TABDLY) == TAB3)
	                        newtty.c_oflag &= ~TABDLY;
#endif
		
			if (litout)
				newtty.c_oflag &= ~OPOST;		

			newtty.c_cc[VMIN] = 1;
#if defined(__hpux) || defined(hpux)
			newtty.c_cc[VTIME] = 1;
			(void) tcsetattr(0, TCSANOW, &newtty);
#else
			newtty.c_cc[VTIME] = 0;
			(void) tcsetattr(0, TCSADRAIN, &newtty);
#endif
#ifdef TIOCGLTC
	                /* Do this after the tcsetattr() in case this version
       	          	 * of termio supports the VSUSP or VDSUSP characters */
#ifndef solaris20
                	/* this forces ICRNL under Solaris... */
                	(void) ioctl(0, TIOCSLTC, (char *)&noltc);
#endif
#endif
			break;
		default:
			return;
			/* NOTREACHED */
		}
#else
	struct sgttyb sb;
	struct tchars *tc;
	int lflags;

	(void)ioctl(0, TIOCGETP, (char *)&sb);
	(void)ioctl(0, TIOCLGET, (char *)&lflags);
	switch(f) {
	case 0:
		sb.sg_flags &= ~(CBREAK|RAW|TBDELAY);
		sb.sg_flags |= defflags|tabflag;
		tc = &deftc;
		ltc = &defltc;
		sb.sg_kill = defkill;
		sb.sg_erase = deferase;
		lflags = deflflags;
		break;
	case 1:
		sb.sg_flags |= (eight ? RAW : CBREAK);
		sb.sg_flags &= ~defflags;
		/* preserve tab delays, but turn off XTABS */
		if ((sb.sg_flags & TBDELAY) == XTABS)
			sb.sg_flags &= ~TBDELAY;
		tc = &notc;
		ltc = &noltc;
		sb.sg_kill = sb.sg_erase = -1;
		if (litout)
			lflags |= LLITOUT;
		break;
	default:
		return;
	}
	(void)ioctl(0, TIOCSLTC, (char *)ltc);
	(void)ioctl(0, TIOCSETC, (char *)tc);
	(void)ioctl(0, TIOCSETN, (char *)&sb);
	(void)ioctl(0, TIOCLSET, (char *)&lflags);
#endif
}

void
lostpeer(signo)
	int signo;
{
#ifdef POSIX_SIGNALS
	struct sigaction sa;
	
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGPIPE, &sa, (struct sigaction *)0);
#else
	(void)signal(SIGPIPE, SIG_IGN);
#endif

	msg("\007connection closed.");
	done(1);
}

/* copy SIGURGs to the child process. */
void
copytochild(signo)
	int signo;
{
	(void)kill(child, SIGURG);
}

void
msg(str)
	char *str;
{
	(void)fprintf(stderr, "rlogin: %s\r\n", str);
}

#ifdef KERBEROS
/* VARARGS */
void
#if __STDC__
warning(const char *fmt, ...)
#else
warning(fmt, va_alist)
	char *fmt;
	va_dcl
#endif
{
	va_list ap;

	(void)fprintf(stderr, "rlogin: warning, using standard rlogin: ");
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	(void)fprintf(stderr, ".\n");
}
#endif

void
usage()
{
	(void)fprintf(stderr,
	    "usage: rlogin [ -%s] [-e char] [ -l username ] host\n",
#ifdef KERBEROS
	    "8EFKLdf");
#else
	    "8ELd");
#endif
	exit(1);
}

/*
 * The following routine provides compatibility (such as it is) between older
 * Suns and others.  Suns have only a `ttysize', so we convert it to a winsize.
 */
#ifdef OLDSUN
int
get_window_size(fd, wp)
	int fd;
	struct winsize *wp;
{
	struct ttysize ts;
	int error;

	if ((error = ioctl(0, TIOCGSIZE, &ts)) != 0)
		return (error);
	wp->ws_row = ts.ts_lines;
	wp->ws_col = ts.ts_cols;
	wp->ws_xpixel = 0;
	wp->ws_ypixel = 0;
	return (0);
}
#endif

u_int
getescape(p)
	register char *p;
{
	long val;
	int len;

	if ((len = strlen(p)) == 1)	/* use any single char, including '\' */
		return ((u_int)*p);
					/* otherwise, \nnn */
	if (*p == '\\' && len >= 2 && len <= 4) {
		val = strtol(++p, NULL, 8);
		for (;;) {
			if (!*++p)
				return ((u_int)val);
			if (*p < '0' || *p > '8')
				break;
		}
	}
	msg("illegal option value -- e");
	usage();
	/* NOTREACHED */
}
