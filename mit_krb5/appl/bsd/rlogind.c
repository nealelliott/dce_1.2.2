/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rlogind.c,v $
 * Revision 1.1.2.2  1996/10/03  14:39:23  arvind
 * 	Submit code review fixes
 * 	[1996/08/28  21:04 UTC  mullan_s  /main/DCE_1.2.2/2]
 *
 * 	Remove -a option.
 * 	[1996/08/28  20:36 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
 *
 * 	remove unnecessary ifdefs - makes code harder to read.
 * 	[1996/08/26  19:22 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 *
 * 	Fix usage synopis; add extra debugging statements.
 * 	[1996/05/30  21:17 UTC  mullan_s  /main/DCE_1.2.2/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/12]
 *
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:11 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/11]
 *
 * 	Pass *fromp to recvauth instead of fromp.
 * 	Log fatal error messages to syslog.
 * 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/10]
 *
 * 	Only exec k5dcelogin if creds are forwarded.
 * 	[1996/04/29  14:35 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:35 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 *
 * Revision 1.1.2.1  1996/06/04  21:47:57  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/12]
 * 
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:11 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/11]
 * 
 * 	Pass *fromp to recvauth instead of fromp.
 * 	Log fatal error messages to syslog.
 * 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/10]
 * 
 * 	Only exec k5dcelogin if creds are forwarded.
 * 	[1996/04/29  14:35 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 * 
 * 	Add OSF/HP copyrights
 * 	[1996/04/29  14:35 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/9]
 * 
 * $EndLog$
 */
/*-
 * Copyright (c) 1983, 1988, 1989, 1993
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
"@(#) Copyright (c) 1983, 1988, 1989, 1993\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)rlogind.c	8.1 (Berkeley) 6/4/93";
#endif /* not lint */

/*
 * remote login server:
 *	\0
 *	remuser\0
 *	locuser\0
 *	terminal_type/speed\0
 *	data
 */

#define	FD_SETSIZE	16		/* don't need many bits for select */
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <termios.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <pwd.h>
#include <syslog.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "pathnames.h"

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#if defined(__hpux) || defined(hpux)
#include <sys/ptyio.h>
#endif

#ifdef DEBUG
#define DEEDEBUG(A) fprintf(stderr,A)
#define DEEDEBUG2(A,B) fprintf(stderr,A,B)
#else
#define DEEDEBUG(A)
#define DEEDEBUG2(A,B)
#endif

#ifndef TIOCPKT_WINDOW
#define TIOCPKT_WINDOW 0x80
#endif

#ifdef	KERBEROS
#define	SECURE_MESSAGE "This rlogin session is using DES encryption for all transmissions.\r\n"
int             doencrypt, retval, use_kerberos, vacuous;
#ifndef KRBV
#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>
AUTH_DAT        *kdata;
KTEXT           ticket;
u_char          auth_buf[sizeof(AUTH_DAT)];
u_char          tick_buf[sizeof(KTEXT_ST)];
Key_schedule    schedule;
#else
#include "krb5.h"
#include "com_err.h"
#include <kerberosIV/krb.h>
#include <libpty.h>

#define KRB5_RECVAUTH_V4        4
#define KRB5_RECVAUTH_V5        5
#define RLOGIND_BUFSIZ 5120

int auth_sys = 0;       /* Which version of Kerberos used to authenticate */
AUTH_DAT        *v4_kdata;
Key_schedule v4_schedule;
char des_inbuf[2*RLOGIND_BUFSIZ]; /* needs to be > largest read size */
char des_outbuf[2*RLOGIND_BUFSIZ];/* needs to be > largest write size */
krb5_data desinbuf,desoutbuf;
krb5_encrypt_block eblock;        /* eblock for encrypt/decrypt */
krb5_authenticator      *kdata;
krb5_ticket     *ticket = 0;
krb5_context bsd_context;
krb5_keytab keytab = NULL;
krb5_principal  client;
char *krusername = 0;
char cmdbuf[NCARGS+1];
krb5_ccache ccache = NULL;

void do_krb_login(char *, struct sockaddr_in *fromp);
#endif /* KRBV */
#ifdef OSF_DCE
#define		ARGSTR			"nkvx"
#else
#define		ARGSTR			"lnkvx"
#endif
#else
#ifdef OSF_DCE
#define		ARGSTR			"n"
#else
#define		ARGSTR			"ln"
#endif
#endif	/* KERBEROS */

char	*env[2];
#define	NMAX 30
char	lusername[NMAX+1], rusername[NMAX+1];
char term[64] = "TERM=";
#define	ENVSIZE	(sizeof("TERM=")-1)	/* skip null for concatenation */
int	keepalive = 1;

struct	passwd *pwd;

void	doit(int, struct sockaddr_in *);
int	control(int, char *, int);
void	protocol(int, int);
void	cleanup(int);
void	fatal(int, char *, int);
int	do_rlogin(char *);
void	getstr(char *, int, char *);
void	setup_term(int);
void	usage(void);
int	local_domain(char *);
char	*topdomain(char *);

int
main(argc, argv)
	int argc;
	char *argv[];
{
	struct sockaddr_in from;
	int ch, fromlen, on;

#ifdef DEBUG_MAIN
        int debug = 1;
        while (debug)
                ;
#endif

#if defined(KERBEROS) && defined(KRBV)
	pty_init();
        krb5_init_context(&bsd_context);
        krb5_init_ets(bsd_context);
#endif

	openlog("rlogind", LOG_PID | LOG_CONS, LOG_AUTH);

	opterr = 0;
	while ((ch = getopt(argc, argv, ARGSTR)) != EOF)
		switch (ch) {
		case 'n':
			keepalive = 0;
			break;
#ifdef KERBEROS
		case 'k':
			use_kerberos = 1;
			break;
		case 'v':
			vacuous = 1;
			break;
#endif
		case '?':
		default:
			usage();
			break;
		}
	argc -= optind;
	argv += optind;

#ifdef	KERBEROS
	if (use_kerberos && vacuous) {
		usage();
		fatal(STDERR_FILENO, "only one of -k and -v allowed", 0);
	}
#endif
	fromlen = sizeof (from);
	if (getpeername(0, (struct sockaddr *)&from, &fromlen) < 0) {
		syslog(LOG_ERR,"Can't get peer name of remote host: %m");
		fatal(STDERR_FILENO, "Can't get peer name of remote host", 1);
	}
	on = 1;
	if (keepalive &&
	    setsockopt(0, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof (on)) < 0)
		syslog(LOG_WARNING, "setsockopt (SO_KEEPALIVE): %m");
#ifdef IPTOS_LOWDELAY
	on = IPTOS_LOWDELAY;
	if (setsockopt(0, IPPROTO_IP, IP_TOS, (char *)&on, sizeof(int)) < 0)
		syslog(LOG_WARNING, "setsockopt (IP_TOS): %m");
#endif
	doit(0, &from);
}

int	child;
int	netf;
char	line[MAXPATHLEN];
int	confirmed;

#ifdef TIOCSWINSZ
struct winsize win = { 0, 0, 0, 0 };
#else /* ~TIOCWINSIZ */
/*
 * Window/terminal size structure.
 * This information is stored by the kernel
 * in order to provide a consistent interface,
 * but is not used by the kernel.
 *
 * Type must be "unsigned short" so that types.h not required.
 */
struct winsize {
        unsigned short  ws_row;                 /* rows, in characters */
        unsigned short  ws_col;                 /* columns, in characters */
        unsigned short  ws_xpixel;              /* horizontal size, pixels */
        unsigned short  ws_ypixel;              /* vertical size, pixels */
};
#endif /* ~TIOCWINSIZ */



void
doit(f, fromp)
	int f;
	struct sockaddr_in *fromp;
{
	int master, pid, on = 1;
	int authenticated = 0;
	register struct hostent *hp;
	char hostname[2 * MAXHOSTNAMELEN + 1];
	char c;
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif

	alarm(60);
	read(f, &c, 1);

	if (c != 0)
		exit(1);
#ifdef	KERBEROS
	if (vacuous)
		fatal(f, "Remote host requires Kerberos authentication", 0);
#endif

	alarm(0);

#ifdef POSIX_SIGNALS
	/* Initialize "sa" structure */
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
#endif

	fromp->sin_port = ntohs((u_short)fromp->sin_port);
	hp = gethostbyaddr((char *)&fromp->sin_addr, sizeof(struct in_addr),
	    fromp->sin_family);
	if (hp)
		(void)strcpy(hostname, hp->h_name);
	else
		(void)strcpy(hostname, inet_ntoa(fromp->sin_addr));

#ifdef	KERBEROS
	if (use_kerberos) {
#ifdef KRBV
        	/* setup des buffers */
	        desinbuf.data = des_inbuf;
        	desoutbuf.data = des_outbuf;    /* Set up des buffers */

		do_krb_login(hostname, fromp);
		authenticated++;
#else
                retval = do_krb_login(fromp);
                if (retval == 0)
                        authenticated++;
                else if (retval > 0)
                        fatal(f, krb_err_txt[retval], 0);
#endif
		write(f, &c, 1);
		confirmed = 1;		/* we sent the null! */
	} else
#endif
	{
		if (fromp->sin_family != AF_INET ||
		    fromp->sin_port >= IPPORT_RESERVED ||
		    fromp->sin_port < IPPORT_RESERVED/2) {
			syslog(LOG_NOTICE, "Connection from %s on illegal port",
				inet_ntoa(fromp->sin_addr));
			fatal(f, "Permission denied", 0);
		}
#ifdef IP_OPTIONS
		{
		u_char optbuf[BUFSIZ/3], *cp;
		char lbuf[BUFSIZ], *lp;
		int optsize = sizeof(optbuf), ipproto;
		struct protoent *ip;

		if ((ip = getprotobyname("ip")) != NULL)
			ipproto = ip->p_proto;
		else
			ipproto = IPPROTO_IP;
		if (getsockopt(0, ipproto, IP_OPTIONS, (char *)optbuf,
		    &optsize) == 0 && optsize != 0) {
			lp = lbuf;
			for (cp = optbuf; optsize > 0; cp++, optsize--, lp += 3)
				sprintf(lp, " %2.2x", *cp);
			syslog(LOG_NOTICE,
			    "Connection received using IP options (ignored):%s",
			    lbuf);
			if (setsockopt(0, ipproto, IP_OPTIONS,
			    (char *)NULL, optsize) != 0) {
				syslog(LOG_ERR,
				    "setsockopt IP_OPTIONS NULL: %m");
				exit(1);
			}
		}
		}
#endif
		if (do_rlogin(hostname) == 0)
			authenticated++;
	}
	if (confirmed == 0) {
		write(f, "", 1);
		confirmed = 1;		/* we sent the null! */
	}
#ifdef KERBEROS
#endif
	netf = f;

#ifdef POSIX_SIGNALS
	sa.sa_handler = cleanup;
	(void) sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
	signal(SIGCHLD, cleanup);
#endif
	pid = forkpty(&master, line, NULL, &win);
	if (pid < 0) {
		if (errno == ENOENT)
			fatal(f, "Out of ptys", 0);
		else
			fatal(f, "Forkpty", 1);
	}
	if (pid == 0) {
		if (f > 2)	/* f should always be 0, but... */
			(void) close(f);
		setup_term(0);
#if defined(KERBEROS) && defined(KRBV)
#ifndef NO_UT_PID
		pty_update_utmp(PTY_LOGIN_PROCESS, getpid(), "rlogin", line,
				""/*host*/, PTY_TTYSLOT_USABLE);
#endif
#endif

		if (authenticated) {
#ifdef	KERBEROS
#ifdef KRBV
        		/* Log access to account */
        		pwd = (struct passwd *) getpwnam(lusername);
			if (use_kerberos && pwd && (pwd->pw_uid == 0))
				syslog(LOG_INFO|LOG_AUTH,
                     		    "ROOT login by %s (%s@%s) as %s\n",
		                    krusername ? krusername : "", rusername,
		                    hostname, lusername);
#else
                        if (use_kerberos && (pwd->pw_uid == 0))
                                syslog(LOG_INFO|LOG_AUTH,
                                    "ROOT Kerberos login from %s.%s@%s on %s\n",
                                    kdata->pname, kdata->pinst, kdata->prealm,
                                    hostname);
#endif
#endif
#if defined(OSF_DCE) && !defined(DO_NOT_USE_K5DCELOGIN)
			/*
			 * Only exec k5dcelogin if creds are being forwarded.
			 * This is true if the ccache pointer is not NULL.
			 */
			if (use_kerberos && ccache)
				execl(K5DCELOGIN_PATH, "k5dcelogin", lusername, _PATH_LOGIN, 
				    "-p", "-h", hostname, "-f", lusername, (char *)NULL);
			else
#endif

			execl(_PATH_LOGIN, "login", "-p",
			    "-h", hostname, "-f", lusername, (char *)NULL);
		} else
			execl(_PATH_LOGIN, "login", "-p",
			    "-h", hostname, lusername, (char *)NULL);
		fatal(STDERR_FILENO, _PATH_LOGIN, 1);
		/*NOTREACHED*/
	}

		ioctl(f, FIONBIO, &on);
	ioctl(master, FIONBIO, &on);
#ifdef TIOCPKT
	ioctl(master, TIOCPKT, &on);
#endif

	protocol(f, master);
#ifdef POSIX_SIGNALS
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
	signal(SIGCHLD, SIG_IGN);
#endif
	cleanup(0);
}

char	magic[2] = { 0377, 0377 };
char	oobdata[] = {TIOCPKT_WINDOW};

/*
 * Handle a "control" request (signaled by magic being present)
 * in the data stream.  For now, we are only willing to handle
 * window size changes.
 */
int
control(pty, cp, n)
	int pty;
	char *cp;
	int n;
{
	struct winsize w;

	if (n < 4+sizeof (w) || cp[2] != 's' || cp[3] != 's')
		return (0);
	oobdata[0] &= ~TIOCPKT_WINDOW;	/* we know he heard */
	bcopy(cp+4, (char *)&w, sizeof(w));
	w.ws_row = ntohs(w.ws_row);
	w.ws_col = ntohs(w.ws_col);
	w.ws_xpixel = ntohs(w.ws_xpixel);
	w.ws_ypixel = ntohs(w.ws_ypixel);
	(void)ioctl(pty, TIOCSWINSZ, &w);
	return (4+sizeof (w));
}

/*
 * rlogin "protocol" machine.
 */
void
protocol(f, p)
	register int f, p;
{
	char pibuf[1024+1], fibuf[1024], *pbp, *fbp;
	register pcc = 0, fcc = 0;
	int cc, nfd, n;
	char cntl;
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif

	/*
	 * Must ignore SIGTTOU, otherwise we'll stop
	 * when we try and set slave pty's window shape
	 * (our controlling tty is the master pty).
	 */
#ifdef POSIX_SIGNALS
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGTTOU, &sa, (struct sigaction *)0);
#else
	(void) signal(SIGTTOU, SIG_IGN);
#endif
	send(f, oobdata, 1, MSG_OOB);	/* indicate new rlogin */
	if (f > p)
		nfd = f + 1;
	else
		nfd = p + 1;
	if (nfd > FD_SETSIZE) {
		syslog(LOG_ERR, "select mask too small, increase FD_SETSIZE");
		fatal(f, "internal error (select mask too small)", 0);
	}
	for (;;) {
		fd_set ibits, obits, ebits, *omask;

		FD_ZERO(&ebits);
		FD_ZERO(&ibits);
		FD_ZERO(&obits);
		omask = (fd_set *)NULL;
		if (fcc) {
			FD_SET(p, &obits);
			omask = &obits;
		} else
			FD_SET(f, &ibits);
		if (pcc >= 0)
			if (pcc) {
				FD_SET(f, &obits);
				omask = &obits;
			} else
				FD_SET(p, &ibits);
		FD_SET(p, &ebits);
		if ((n = select(nfd, &ibits, omask, &ebits, 0)) < 0) {
			if (errno == EINTR)
				continue;
			fatal(f, "select", 1);
		}
		if (n == 0) {
			/* shouldn't happen... */
			sleep(5);
			continue;
		}
#define	pkcontrol(c)	((c)&(TIOCPKT_FLUSHWRITE|TIOCPKT_NOSTOP|TIOCPKT_DOSTOP))
		if (FD_ISSET(p, &ebits)) {
			cc = read(p, &cntl, 1);
			if (cc == 1 && pkcontrol(cntl)) {
				if (cntl & TIOCPKT_FLUSHWRITE)
                                    DEEDEBUG("Received TIOCPKT_FLUSHWRITE flow control byte\n");
                                if (cntl & TIOCPKT_NOSTOP)
                                    DEEDEBUG("Received TIOCPKT_NOSTOP flow control byte\n");
                                if (cntl & TIOCPKT_DOSTOP)
                                    DEEDEBUG("Received TIOCPKT_DOSTOP flow control byte\n");
				cntl |= oobdata[0];
				send(f, &cntl, 1, MSG_OOB);
				if (cntl & TIOCPKT_FLUSHWRITE) {
					pcc = 0;
					FD_CLR(p, &ibits);
				}
			}
		}
		if (FD_ISSET(f, &ibits)) {
				fcc = read(f, fibuf, sizeof(fibuf));
			if (fcc < 0 && errno == EWOULDBLOCK)
				fcc = 0;
			else {
				register char *cp;
				int left, n;

				if (fcc <= 0)
					break;
				fbp = fibuf;

			top:
				for (cp = fibuf; cp < fibuf+fcc-1; cp++)
					if (cp[0] == magic[0] &&
					    cp[1] == magic[1]) {
						left = fcc - (cp-fibuf);
						n = control(p, cp, left);
						if (n) {
							left -= n;
							if (left > 0)
								bcopy(cp+n, cp, left);
							fcc -= n;
							goto top; /* n^2 */
						}
					}
				FD_SET(p, &obits);		/* try write */
			}
		}

		if (FD_ISSET(p, &obits) && fcc > 0) {
			cc = write(p, fbp, fcc);
			if (cc > 0) {
				fcc -= cc;
				fbp += cc;
			}
		}

		if (FD_ISSET(p, &ibits)) {
			pcc = read(p, pibuf, sizeof (pibuf));
			pbp = pibuf;
			if (pcc < 0 && errno == EWOULDBLOCK)
				pcc = 0;
			else if (pcc <= 0)
				break;
			else if (pibuf[0] == 0) {
				pbp++, pcc--;
					FD_SET(f, &obits);	/* try write */
			} else {
				if (pkcontrol(pibuf[0])) {
                                    if (pibuf[0] & TIOCPKT_FLUSHWRITE)
                   			DEEDEBUG("Received TIOCPKT_FLUSHWRITE flow control byte\n");
                                    if (pibuf[0] & TIOCPKT_NOSTOP)
		                        DEEDEBUG("Received TIOCPKT_NOSTOP flow control byte\n");
                                    if (pibuf[0] & TIOCPKT_DOSTOP)
		                        DEEDEBUG("Received TIOCPKT_DOSTOP flow control byte\n");
					pibuf[0] |= oobdata[0];
					send(f, &pibuf[0], 1, MSG_OOB);
				}
				pcc = 0;
			}
		}
		if ((FD_ISSET(f, &obits)) && pcc > 0) {
				cc = write(f, pbp, pcc);
			if (cc < 0 && errno == EWOULDBLOCK) {
				/*
				 * This happens when we try write after read
				 * from p, but some old kernels balk at large
				 * writes even when select returns true.
				 */
				if (!FD_ISSET(p, &ibits))
					sleep(5);
				continue;
			}
			if (cc > 0) {
				pcc -= cc;
				pbp += cc;
			}
		}
	}
}

void
cleanup(signo)
	int signo;
{
	char *p;

	p = line + sizeof(_PATH_DEV) - 1;
	if (logout(p))
		pty_logwtmp(p, "", "");

	(void)chmod(line, 0666);
	(void)chown(line, 0, 0);
	*p = 'p';
	(void)chmod(line, 0666);
	(void)chown(line, 0, 0);
	shutdown(netf, 2);
	exit(1);
}

void
fatal(f, msg, syserr)
	int f;
	char *msg;
	int syserr;
{
	int len;
	char buf[BUFSIZ], *bp = buf;

	/*
	 * Prepend binary one to message if we haven't sent
	 * the magic null as confirmation.
	 */
	if (!confirmed)
		*bp++ = '\01';		/* error indicator */
	if (syserr)
		len = sprintf(bp, "rlogind: %s: %s.\r\n",
		    msg, strerror(errno));
	else
		len = sprintf(bp, "rlogind: %s.\r\n", msg);
	(void) write(f, buf, bp + len - buf);
	syslog(LOG_ERR,"%s\n",msg);
	exit(1);
}

int
do_rlogin(dest)
	char *dest;
{
	getstr(rusername, sizeof(rusername), "remuser too long");
	getstr(lusername, sizeof(lusername), "locuser too long");
	getstr(term+ENVSIZE, sizeof(term)-ENVSIZE, "Terminal type too long");

	pwd = getpwnam(lusername);
	if (pwd == NULL)
		return (-1);
	if (pwd->pw_uid == 0)
		return (-1);
	/* XXX why don't we syslog() failure? */
	return (ruserok(dest, 0, rusername, lusername));
}

void
getstr(buf, cnt, errmsg)
	char *buf;
	int cnt;
	char *errmsg;
{
	char c;

	do {
		if (read(0, &c, 1) != 1)
			exit(1);
		if (--cnt < 0)
			fatal(STDOUT_FILENO, errmsg, 0);
		*buf++ = c;
	} while (c != 0);
}

extern	char **environ;

void
setup_term(fd)
	int fd;
{
	register char *cp = index(term+ENVSIZE, '/');
	char *speed;
	struct termios tt;

#ifdef notyet
	tcgetattr(fd, &tt);
	if (cp) {
		*cp++ = '\0';
		speed = cp;
		cp = index(speed, '/');
		if (cp)
			*cp++ = '\0';
		cfsetspeed(&tt, atoi(speed));
	}

	tt.c_iflag = TTYDEF_IFLAG;
	tt.c_oflag = TTYDEF_OFLAG;
	tt.c_lflag = TTYDEF_LFLAG;
	tcsetattr(fd, TCSAFLUSH, &tt);
#else
	if (cp) {
		*cp++ = '\0';
		speed = cp;
		cp = index(speed, '/');
		if (cp)
			*cp++ = '\0';
		tcgetattr(fd, &tt);
		cfsetispeed(&tt, atoi(speed));
		cfsetospeed(&tt, atoi(speed));
		tcsetattr(fd, TCSAFLUSH, &tt);
	}
#endif

	putenv(term);
}

#ifdef	KERBEROS
#define	VERSION_SIZE	9

/*
 * Do the remote kerberos login to the named host with the
 * given inet address
 *
 * Return 0 on valid authorization
 * Return -1 on valid authentication, no authorization
 * Return >0 for error conditions
 *
 * DCE comment: See BSD4.4-Lite source for original routine.
 * This routine has changed drastically enough that preserving
 * the history via ifdef's isn't worth it.
 */
void
do_krb_login(host, fromp)
	char *host;
        struct sockaddr_in *fromp;
{
        krb5_error_code status;
        char *msg_fail;
	int passed_krb = 0;

        if (getuid()) {
            exit(1);
        }

        /* Check authentication. This can be either Kerberos V5, */
        /* Kerberos V4, or host-based. */
        if (status = recvauth(STDIN_FILENO, *fromp, 0)) {
        	if (status != 255)
          		syslog(LOG_ERR,
                 	       "Authentication failed from %s: %s\n",
                               host,error_message(status));
        	fatal(netf, "Kerberos authentication failed", 0);
        	return;
        }

        /* OK we have authenticated this user - now check authorization. */
        /* The Kerberos authenticated programs must use krb5_kuserok or kuserok*/

#ifndef KRB5_KRB4_COMPAT
        if (auth_sys == KRB5_RECVAUTH_V4) {
        	fatal(netf, "This server does not support Kerberos V4", 0);
        }
#endif

#if (defined(ALWAYS_V5_KUSEROK) || !defined(KRB5_KRB4_COMPAT))
        /* krb5_kuserok returns 1 if OK */
        if (client && krb5_kuserok(bsd_context, client, lusername))
        	passed_krb++;
#else
        if (auth_sys == KRB5_RECVAUTH_V4) {
        	/* kuserok returns 0 if OK */
	        if (!kuserok(v4_kdata, lusername))
                	passed_krb++;
        } else {
        	/* krb5_kuserok returns 1 if OK */
        	if (client && krb5_kuserok(bsd_context, client, lusername))
            		passed_krb++;
        }
#endif

	if (passed_krb)
	        return;

        msg_fail =  (char *) malloc( strlen(krusername) + strlen(lusername) + 80 );
        if (!msg_fail)
        	fatal(netf, "User is not authorized to login to specified account", 0);
        sprintf(msg_fail, "User %s is not authorized to login to account %s",
            	krusername, lusername);
        fatal(netf, msg_fail, 0);
        /* NOTREACHED */
}
#endif /* KERBEROS */

void
usage()
{
#ifdef KERBEROS
#ifdef OSF_DCE
	syslog(LOG_ERR, "usage: rlogind [-n] [-k | -v]");
#else
	syslog(LOG_ERR, "usage: rlogind [-ln] [-k | -v]");
#endif
#else
#ifdef OSF_DCE
	syslog(LOG_ERR, "usage: rlogind [-n]");
#else
	syslog(LOG_ERR, "usage: rlogind [-ln]");
#endif
#endif
}

/*
 * Check whether host h is in our local domain,
 * defined as sharing the last two components of the domain part,
 * or the entire domain part if the local domain has only one component.
 * If either name is unqualified (contains no '.'),
 * assume that the host is local, as it will be
 * interpreted as such.
 */
int
local_domain(h)
	char *h;
{
	char localhost[MAXHOSTNAMELEN];
	char *p1, *p2;

	localhost[0] = 0;
	(void) gethostname(localhost, sizeof(localhost));
	p1 = topdomain(localhost);
	p2 = topdomain(h);
	if (p1 == NULL || p2 == NULL || !strcasecmp(p1, p2))
		return (1);
	return (0);
}

char *
topdomain(h)
	char *h;
{
	register char *p;
	char *maybe = NULL;
	int dots = 0;

	for (p = h + strlen(h); p >= h; p--) {
		if (*p == '.') {
			if (++dots == 2)
				return (p);
			maybe = p;
		}
	}
	return (maybe);
}
