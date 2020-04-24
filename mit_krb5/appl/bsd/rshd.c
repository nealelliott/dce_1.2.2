/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rshd.c,v $
 * Revision 1.1.2.1  1996/06/04  21:48:24  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:36 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:43 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/14]
 *
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:10 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/13]
 *
 * 	Copy remote kerberos principal name (from ticket) to kremuser.
 * 	[1996/05/03  20:03 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/12]
 *
 * 	Only exec k5dcelogin if creds are forwarded.
 * 	[1996/04/29  14:35 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/11]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/04/22  19:02 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/10]
 *
 * 	Add OSF/HP copyrights.
 * 	Preserve BSD4.4-lite source base (not finished)
 * 	[1996/04/22  19:02 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/10]
 *
 * $EndLog$
 */
/*-
 * Copyright (c) 1988, 1989, 1992, 1993, 1994
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
"@(#) Copyright (c) 1988, 1989, 1992, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)rshd.c	8.2 (Berkeley) 4/6/94";
#endif /* not lint */

/*
 * remote shell server:
 *	[port]\0
 *	remuser\0
 *	locuser\0
 *	command\0
 *	data
 */
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <errno.h>
#include <fcntl.h>
#ifndef OSF_DCE
#include <paths.h>
#endif
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef OSF_DCE
#include <ndbm.h>
int debug = 0;
#include "pathnames.h"
#endif

int	keepalive = 1;
int	check_all;
int	log_success;		/* If TRUE, log all successful accesses */
int	sent_null;

#ifdef OSF_DCE
void	 doit(int, struct sockaddr_in *);
void	 error(const char *, ...);
void	 getstr(char *, int, char *);
int	 local_domain(char *);
char	 *topdomain(char *);
void	 usage(void);
#else
void     doit __P((struct sockaddr_in *));
void     error __P((const char *, ...));
void     getstr __P((char *, int, char *));
int      local_domain __P((char *));
char    *topdomain __P((char *));
void     usage __P((void));
#endif

#ifdef	KERBEROS
#ifdef KRBV
#include "krb5.h"
#include "com_err.h"
#include <kerberosIV/krb.h>

#define RSHD_BUFSIZ 5120
#define NMAX   16

char des_inbuf[2*RSHD_BUFSIZ];    /* needs to be > largest read size */
krb5_encrypt_block eblock;        /* eblock for encrypt/decrypt */
char des_outbuf[2*RSHD_BUFSIZ];   /* needs to be > largest write size */
krb5_data desinbuf,desoutbuf;
krb5_context bsd_context;
char *srvtab = NULL;
char *kremuser;
krb5_principal client;
int auth_sys = 0;       /* Which version of Kerberos used to authenticate */
char locuser[NMAX+1];
char remuser[NMAX+1];
char cmdbuf[NCARGS+1];
char lusername[NMAX+1];
char rusername[NMAX+1];
krb5_keytab keytab = NULL;
krb5_ccache ccache = NULL;
char *krusername = 0;
char term[64];
krb5_ticket     *ticket = 0;
char ttyn[12];          /* Line string for wtmp entries */
int pid;

AUTH_DAT        *v4_kdata;
Key_schedule v4_schedule;

int doencrypt = 0, use_kerberos, vacuous;
#else /* !KRBV */
#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>
#endif

#define	VERSION_SIZE	9
#define SECURE_MESSAGE  "This rsh session is using DES encryption for all transmissions.\r\n"
#ifdef OSF_DCE
#define	OPTIONS		"ankvxL"
#else
#define	OPTIONS		"alnkvxL"
#endif
#else /* !KERBEROS */
#ifdef OSF_DCE
#define	OPTIONS	"anL"
#else
#define	OPTIONS	"alnL"
#endif
#endif /* KERBEROS */

int
main(argc, argv)
	int argc;
	char *argv[];
{
#ifndef OSF_DCE
	extern int __check_rhosts_file;
#endif
	struct linger linger;
	int ch, on = 1, fromlen;
	struct sockaddr_in from;
#ifdef DEBUG_MAIN
	debug = 1;
        while (debug)
		;
#endif

	openlog("rshd", LOG_PID | LOG_ODELAY, LOG_DAEMON);

#if defined(KERBEROS) && defined(KRBV)
        krb5_init_context(&bsd_context);
        krb5_init_ets(bsd_context);
#endif

	opterr = 0;
	while ((ch = getopt(argc, argv, OPTIONS)) != EOF)
		switch (ch) {
		case 'a':
			check_all = 1;
			break;
#ifndef OSF_DCE
		case 'l':
			__check_rhosts_file = 0;
			break;
#endif
		case 'n':
			keepalive = 0;
			break;
#ifdef	KERBEROS
		case 'k':
			use_kerberos = 1;
			break;

		case 'v':
			vacuous = 1;
			break;

#endif
		case 'L':
			log_success = 1;
			break;
		case '?':
		default:
			usage();
			break;
		}

	argc -= optind;
	argv += optind;

#ifdef	KERBEROS
	if (use_kerberos && vacuous) {
		syslog(LOG_ERR, "only one of -k and -v allowed");
		exit(2);
	}
#endif

	fromlen = sizeof (from);
	if (getpeername(0, (struct sockaddr *)&from, &fromlen) < 0) {
		syslog(LOG_ERR, "getpeername: %m");
		_exit(1);
	}
	if (keepalive &&
	    setsockopt(0, SOL_SOCKET, SO_KEEPALIVE, (char *)&on,
	    sizeof(on)) < 0)
		syslog(LOG_WARNING, "setsockopt (SO_KEEPALIVE): %m");
	linger.l_onoff = 1;
	linger.l_linger = 60;			/* XXX */
	if (setsockopt(0, SOL_SOCKET, SO_LINGER, (char *)&linger,
	    sizeof (linger)) < 0)
		syslog(LOG_WARNING, "setsockopt (SO_LINGER): %m");
#ifdef OSF_DCE
	doit(dup(0), &from);
#else
	doit(&from);
#endif
	/* NOTREACHED */
}

char	username[20] = "USER=";
char	homedir[64] = "HOME=";
char	shell[64] = "SHELL=";
char	path[100] = "PATH=";
#ifdef KERBEROS
char    term[64] = "TERM=network";
char    *envinit[] =
	    {homedir, shell, path, username, term, 0, 0, 0};
#else
char	*envinit[] =
	    {homedir, shell, path, username, 0};
#endif
extern char	**environ;

void
#ifdef OSF_DCE
doit(f, fromp)
	int f;
#else
doit(fromp)
#endif
	struct sockaddr_in *fromp;
{
#ifndef OSF_DCE
	extern char *__rcmd_errstr;	/* syslog hook from libc/net/rcmd.c. */
#endif
	struct hostent *hp;
	struct passwd *pwd;
	u_short port;
	fd_set ready, readfrom;
	int cc, nfd, pv[2], s;
	int one = 1;
	char *hostname, *errorstr, *errorhost;
	char *cp, sig, buf[BUFSIZ];
#ifndef KRBV
	char cmdbuf[NCARGS+1], locuser[16], remuser[16];
	int pid;
#endif
	char remotehost[2 * MAXHOSTNAMELEN + 1];
#ifdef POSIX_SIGNALS
	struct sigaction sa;
#endif

#ifdef	KERBEROS
#ifdef KRBV
        krb5_error_code status;
        struct sockaddr_in fromaddr = *fromp;
        krb5_sigtype     cleanup();
#else
        AUTH_DAT        *kdata = (AUTH_DAT *) NULL;
        KTEXT           ticket = (KTEXT) NULL;
        char            instance[INST_SZ], version[VERSION_SIZE];
        struct          sockaddr_in     fromaddr;
        int             rc;
        long            authopts;
#endif
        int pv1[2], pv2[2];
        fd_set wready, writeto;
#endif

#ifdef POSIX_SIGNALS
	(void) sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	(void) sigaction(SIGINT, &sa, (struct sigaction *)0);
	(void) sigaction(SIGQUIT, &sa, (struct sigaction *)0);
	(void) sigaction(SIGTERM, &sa, (struct sigaction *)0);
#else
	(void) signal(SIGINT, SIG_DFL);
	(void) signal(SIGQUIT, SIG_DFL);
	(void) signal(SIGTERM, SIG_DFL);
#endif
#ifdef DEBUG
	{ int t = open(_PATH_TTY, 2);
	  if (t >= 0) {
		ioctl(t, TIOCNOTTY, (char *)0);
		(void) close(t);
	  }
	}
#endif
	fromp->sin_port = ntohs((u_short)fromp->sin_port);
	if (fromp->sin_family != AF_INET) {
		syslog(LOG_ERR, "malformed \"from\" address (af %d)\n",
		    fromp->sin_family);
		exit(1);
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
	if (!getsockopt(0, ipproto, IP_OPTIONS, (char *)optbuf, &optsize) &&
	    optsize != 0) {
		lp = lbuf;
		for (cp = optbuf; optsize > 0; cp++, optsize--, lp += 3)
			sprintf(lp, " %2.2x", *cp);
		syslog(LOG_NOTICE,
		    "Connection received from %s using IP options (ignored):%s",
		    inet_ntoa(fromp->sin_addr), lbuf);
		if (setsockopt(0, ipproto, IP_OPTIONS,
		    (char *)NULL, optsize) != 0) {
			syslog(LOG_ERR, "setsockopt IP_OPTIONS NULL: %m");
			exit(1);
		}
	}
      }
#endif

#ifdef	KERBEROS
#ifdef KRBV
        desinbuf.data = des_inbuf;
        desoutbuf.data = des_outbuf;
#endif
	if (!use_kerberos)
#endif
		if (fromp->sin_port >= IPPORT_RESERVED ||
		    fromp->sin_port < IPPORT_RESERVED/2) {
			syslog(LOG_NOTICE|LOG_AUTH,
			    "Connection from %s on illegal port %u",
			    inet_ntoa(fromp->sin_addr),
			    fromp->sin_port);
			exit(1);
		}

	(void) alarm(60);
	port = 0;
	for (;;) {
		char c;
		if ((cc = read(STDIN_FILENO, &c, 1)) != 1) {
			if (cc < 0)
				syslog(LOG_NOTICE, "read: %m");
			shutdown(0, 1+1);
			exit(1);
		}
		if (c== 0)
			break;
		port = port * 10 + c - '0';
	}

	(void) alarm(0);
	if (port != 0) {
		int lport = IPPORT_RESERVED - 1;
		s = rresvport(&lport);
		if (s < 0) {
			syslog(LOG_ERR, "can't get stderr port: %m");
			exit(1);
		}
#ifdef	KERBEROS
		if (!use_kerberos)
#endif
			if (port >= IPPORT_RESERVED) {
				syslog(LOG_ERR, "2nd port not reserved\n");
				exit(1);
			}
		fromp->sin_port = htons(port);
		if (connect(s, (struct sockaddr *)fromp, sizeof (*fromp)) < 0) {
			syslog(LOG_INFO, "connect second port %d: %m", port);
			exit(1);
		}
	}

#ifdef	KERBEROS
	if (vacuous) {
		error("rshd: remote host requires Kerberos authentication\n");
		exit(1);
	}
#endif
	/* from inetd, socket is already on 0, 1, 2 */
	dup2(f, 0);
	dup2(f, 1);
	dup2(f, 2);
	errorstr = NULL;
	hp = gethostbyaddr((char *)&fromp->sin_addr, sizeof (struct in_addr),
		fromp->sin_family);
	if (hp) {
		/*
		 * If name returned by gethostbyaddr is in our domain,
		 * attempt to verify that we haven't been fooled by someone
		 * in a remote net; look up the name and check that this
		 * address corresponds to the name.
		 */
		hostname = hp->h_name;
#ifdef	KERBEROS
		if (!use_kerberos)
#endif
		if (check_all || local_domain(hp->h_name)) {
			strncpy(remotehost, hp->h_name, sizeof(remotehost) - 1);
			remotehost[sizeof(remotehost) - 1] = 0;
			errorhost = remotehost;
			hp = gethostbyname(remotehost);
			if (hp == NULL) {
				syslog(LOG_INFO,
				    "Couldn't look up address for %s",
				    remotehost);
				errorstr =
				"Couldn't look up address for your host (%s)\n";
				hostname = inet_ntoa(fromp->sin_addr);
			} else for (; ; hp->h_addr_list++) {
				if (hp->h_addr_list[0] == NULL) {
					syslog(LOG_NOTICE,
					  "Host addr %s not listed for host %s",
					    inet_ntoa(fromp->sin_addr),
					    hp->h_name);
					errorstr =
					    "Host address mismatch for %s\n";
					hostname = inet_ntoa(fromp->sin_addr);
					break;
				}
				if (!bcmp(hp->h_addr_list[0],
				    (caddr_t)&fromp->sin_addr,
				    sizeof(fromp->sin_addr))) {
					hostname = hp->h_name;
					break;
				}
			}
		}
	} else
		errorhost = hostname = inet_ntoa(fromp->sin_addr);

#ifdef	KERBEROS
	if (use_kerberos) {
#ifdef KRBV
        	if (status = recvauth(f, fromaddr, 1)) {
        		error("Authentication failed: %s\n", error_message(status));
        		exit(1);
    		}
		(void) strcpy(locuser, lusername);
		(void) strcpy(remuser, rusername);
		kremuser = krusername;
#else
	/* XXX - See BSD4.4lite code */
#endif
	} else
#endif
        {
		getstr(remuser, sizeof(remuser), "remuser");
	  	getstr(locuser, sizeof(locuser), "locuser");
	  	getstr(cmdbuf, sizeof(cmdbuf), "command");
	}
	setpwent();
	pwd = getpwnam(locuser);
	if (pwd == NULL) {
		syslog(LOG_INFO|LOG_AUTH,
		    "%s@%s as %s: unknown login. cmd='%.80s'",
		    remuser, hostname, locuser, cmdbuf);
		if (errorstr == NULL)
			errorstr = "Login incorrect.\n";
		goto fail;
	}
#if defined(KERBEROS) && defined(KRBV)
    /* Setup wtmp entry : we do it here so that if this is a CRAY
       the Process Id is correct and we have not lost our trusted
       privileges. */
    if (port) {
        /* Place entry into wtmp */
        sprintf(ttyn,"krsh%1d",getpid());
        pty_logwtmp(ttyn,locuser,hostname);
    }
    /*      We are simply execing a program over rshd : log entry into wtmp,
            as kexe(pid), then finish out the session right after that.
            Syslog should have the information as to what was exec'd */
    else {
        pty_logwtmp(ttyn,locuser,hostname);
    }
#endif
	if (chdir(pwd->pw_dir) < 0) {
		(void) chdir("/");
#ifdef notdef
		syslog(LOG_INFO|LOG_AUTH,
		    "%s@%s as %s: no home directory. cmd='%.80s'",
		    remuser, hostname, locuser, cmdbuf);
		error("No remote directory.\n");
#if defined(KERBEROS) && defined(KRBV)
		goto signout_please;
#else
		exit(1);
#endif
#endif
	}

#ifdef	KERBEROS
	if (use_kerberos) {
#if defined(KRB5_KRB4_COMPAT) && !defined(ALWAYS_V5_KUSEROK)
        	if (auth_sys == KRB5_RECVAUTH_V4) {
            		/* kuserok returns 0 if OK */
            		if (kuserok(v4_kdata, locuser)){
                		syslog(LOG_ERR ,
                       		       "Principal %s (%s@%s) for local user %s failed kuserok.\n",
                       		       kremuser, remuser, hostname, locuser);
				error("Permission denied.\n");
#ifdef KRBV
				goto signout_please;
#else
				exit(1);
#endif
			}
		}
	} else {
#endif
                /* krb5_kuserok returns 1 if OK */
                if (!krb5_kuserok(bsd_context, client, locuser)){
                	syslog(LOG_ERR ,
                       	       "Principal %s (%s@%s) for local user %s failed krb5_kuserok.\n",
                               kremuser, remuser, hostname, locuser);
                        error("Permission denied.\n");
#ifdef KRBV
                        goto signout_please;
#else
			exit(1);
#endif
                }
        } else
#endif
		if (errorstr ||
		    pwd->pw_passwd != 0 && *pwd->pw_passwd != '\0' &&
		    ruserok(hostname, pwd->pw_uid == 0,
		    remuser, locuser) < 0) {
#ifndef OSF_DCE
			if (__rcmd_errstr)
				syslog(LOG_INFO|LOG_AUTH,
			    "%s@%s as %s: permission denied (%s). cmd='%.80s'",
				    remuser, hostname, locuser, __rcmd_errstr,
				    cmdbuf);
			else
#endif
				syslog(LOG_INFO|LOG_AUTH,
			    "%s@%s as %s: permission denied. cmd='%.80s'",
				    remuser, hostname, locuser, cmdbuf);
fail:
			if (errorstr == NULL)
				errorstr = "Permission denied.\n";
			error(errorstr, errorhost);
			exit(1);
		}

	if (pwd->pw_uid && !access(_PATH_NOLOGIN, F_OK)) {
		error("Logins currently disabled.\n");
#if defined(KERBEROS) && defined(KRBV)
		goto signout_please;
#else
		exit(1);
#endif
	}

	(void) write(STDERR_FILENO, "\0", 1);
	sent_null = 1;

/*
 * DCE Comment: the rest of this routine is very different from
 * the BSD4.4Lite version. Since it is too difficult to retain
 * the history via ifdefs, please refer to the BSD source for
 * the differences.
 */
	if (port) {
		if (pipe(pv) < 0) {
			error("Can't make pipe.\n");
#if defined(KERBEROS) && defined(KRBV)
			goto signout_please;
#else
			exit(1);
#endif
		}
		if (pipe(pv1) < 0) {
			error("Can't make 2nd pipe.\n");
#if defined(KERBEROS) && defined(KRBV)
			goto signout_please;
#else
			exit(1);
#endif
		}
		if (pipe(pv2) < 0) {
			error("Can't make 3rd pipe.\n");
#if defined(KERBEROS) && defined(KRBV)
			goto signout_please;
#else
			exit(1);
#endif
		}
		pid = fork();
		if (pid == -1)  {
			error("Can't fork; try again.\n");
#if defined(KERBEROS) && defined(KRBV)
			goto signout_please;
#else
			exit(1);
#endif
		}
		if (pid) {

#ifdef POSIX_SIGNALS
                	sa.sa_handler = cleanup;
    	                (void)sigaction(SIGINT, &sa, (struct sigaction *)0);
               	        (void)sigaction(SIGQUIT, &sa, (struct sigaction *)0);
    	                (void)sigaction(SIGTERM, &sa, (struct sigaction *)0);
           	        (void)sigaction(SIGPIPE, &sa, (struct sigaction *)0);
    	                (void)sigaction(SIGHUP, &sa, (struct sigaction *)0);

            		sa.sa_handler = SIG_IGN;
            		(void)sigaction(SIGCHLD, &sa, (struct sigaction *)0);
#else
            		signal(SIGINT, cleanup);
            		signal(SIGQUIT, cleanup);
            		signal(SIGTERM, cleanup);
            		signal(SIGPIPE, cleanup);
            		signal(SIGHUP, cleanup);
            		signal(SIGCHLD,SIG_IGN);
#endif

		        (void) close(0); (void) close(1); (void) close(2);
            		(void) close(pv[1]);
            		(void) close(pv1[1]);
            		(void) close(pv2[0]);

            		ioctl(pv[0], FIONBIO, (char *)&one);
            		ioctl(pv1[0], FIONBIO, (char *)&one);
            		/* should set s nbio! */

#ifdef KERBEROS
#endif

			FD_ZERO(&readfrom);
			FD_SET(f, &readfrom);
			FD_SET(s, &readfrom);
			FD_SET(pv[0], &readfrom);
			FD_SET(pv1[0], &readfrom);

			do {
				ready = readfrom;
				if (select(8*sizeof(ready), &ready, (fd_set *)0,
					  (fd_set *)0, (struct timeval *)0) < 0)
						break;
				if (FD_ISSET(s, &ready)) {
					int	ret;
						ret = read(s, &sig, 1);
					if (ret <= 0)
						FD_CLR(s, &readfrom);
					else {
#ifdef POSIX_SIGNALS
                        			sa.sa_handler = cleanup;
			                        (void)sigaction(sig, &sa, (struct sigaction *)0);
						kill(-pid, sig);
#else
			                        signal(sig, cleanup);
						killpg(pid, sig);
#endif
					}
				}
                		if (FD_ISSET(f, &ready)) {
                    			errno = 0;
						cc = read(f, buf, sizeof(buf));
                    			if (cc <= 0) {
                        			(void) close(pv2[1]);
                        			FD_CLR(f, &readfrom);
                    			} else
                        			(void) write(pv2[1], buf, cc);
                		}
				if (FD_ISSET(pv[0], &ready)) {
					errno = 0;
					cc = read(pv[0], buf, sizeof(buf));
					if (cc <= 0) {
						shutdown(s, 1+1);
						FD_CLR(pv[0], &readfrom);
					} else {
							(void)
							  write(s, buf, cc);
					}
				}
				if (FD_ISSET(pv1[0], &ready)) {
					errno = 0;
					cc = read(pv1[0], buf, sizeof(buf));
					if (cc <= 0) {
						shutdown(f, 1+1);
						FD_CLR(pv1[0], &readfrom);
					} else {
							(void) write(f,
								buf, cc);
					}
				}
			} while (FD_ISSET(s, &readfrom) ||
			    FD_ISSET(f, &readfrom) ||
			    FD_ISSET(pv1[0], &readfrom) ||
			    FD_ISSET(pv[0], &readfrom));
#if defined(KERBEROS) && defined(KRBV)
            		syslog(LOG_INFO ,
		               "Shell process completed.");
		        /* Finish session in wmtp */
		        pty_logwtmp(ttyn,"","");
			if (ccache)
				krb5_cc_destroy(bsd_context, ccache);
			ccache = NULL;
#endif
			exit(0);
		}
#ifdef DEBUG_CHILD
       		debug = 1;
	        while (debug)
               		;
#endif
#ifdef SETPGRP_TWOARG
		setpgrp(0, getpid());
#else
		setpgrp();
#endif
		(void) close(s);
		(void) close(f);
		(void) close(pv[0]);
		close(pv1[0]); close(pv2[0]);
		dup2(pv1[1], 1);
		dup2(pv2[0], 0);
		dup2(pv[1], 2);
		close(pv1[1]);
		close(pv2[0]);
		close(pv[1]);
	}
#if defined(KERBEROS) && defined(KRBV)
        /*      We are simply execing a program over rshd : log entry into wtmp,
                as kexe(pid), then finish out the session right after that.
                Syslog should have the information as to what was exec'd */
        else {
        	pty_logwtmp(ttyn,"","");
        }
#endif
	if (*pwd->pw_shell == '\0')
		pwd->pw_shell = _PATH_BSHELL;
#if	BSD > 43
	if (setlogin(pwd->pw_name) < 0)
		syslog(LOG_ERR, "setlogin() failed: %m");
#endif
#ifdef OSF_DCE
	(void) close(f);
#endif
	(void) setgid((gid_t)pwd->pw_gid);
	initgroups(pwd->pw_name, pwd->pw_gid);
	(void) setuid((uid_t)pwd->pw_uid);
	strncat(homedir, pwd->pw_dir, sizeof(homedir)-6);
	strcat(path, _PATH_DEFPATH);
	strncat(shell, pwd->pw_shell, sizeof(shell)-7);
	strncat(username, pwd->pw_name, sizeof(username)-6);
	cp = strrchr(pwd->pw_shell, '/');
	if (cp)
		cp++;
	else
		cp = pwd->pw_shell;
#if defined(KERBEROS) && defined(KRBV)
	/* If we have KRB5CCNAME set, then copy into the
	 * child's environment.  This can't really have
	 * a fixed position because tz may or may not be set.
	 */
	if (getenv("KRB5CCNAME")) {
		int i;
		char *buf = (char *)malloc(strlen(getenv("KRB5CCNAME"))
		                +strlen("KRB5CCNAME=")+1);
                if (buf) {
			sprintf(buf, "KRB5CCNAME=%s",getenv("KRB5CCNAME"));
			for (i = 0; envinit[i]; i++);
			envinit[i] =buf;
			
		}
		/* If we do anything else, make sure there is space in the array.
		 */
	}
#endif
	environ = envinit;
	endpwent();
	if (log_success || pwd->pw_uid == 0) {
#ifdef	KERBEROS
		if (use_kerberos)
#ifdef KRBV
          		syslog(LOG_NOTICE, "Executing %s for principal %s (%s@%s) as local user %s",
                 	       cmdbuf, kremuser, remuser, hostname, locuser);
#else
                    syslog(LOG_INFO|LOG_AUTH,
                        "Kerberos shell from %s.%s@%s on %s as %s, cmd='%.80s'",
                        kdata->pname, kdata->pinst, kdata->prealm,
                        hostname, locuser, cmdbuf);
#endif
		else
#endif
		    syslog(LOG_INFO|LOG_AUTH, "%s@%s as %s: cmd='%.80s'",
			remuser, hostname, locuser, cmdbuf);
	}
#if defined(OSF_DCE) && !defined(DO_NOT_USE_K5DCELOGIN)
	/*
	 * Only exec k5dcelogin if creds are being forwarded.
	 * This is true if the ccache pointer is not NULL.
	 */
	if (use_kerberos && ccache)
		execl(K5DCELOGIN_PATH, "k5dcelogin", locuser, pwd->pw_shell,
		    "-c", cmdbuf, 0);
	else
#endif
	execl(pwd->pw_shell, cp, "-c", cmdbuf, 0);
	perror(pwd->pw_shell);
	exit(1);

#if defined(KERBEROS) && defined(KRBV)
  signout_please:
	if (ccache)
		krb5_cc_destroy(bsd_context, ccache);
	ccache = NULL;
	pty_logwtmp(ttyn,"","");
    	exit(1);
#endif
}

/*
 * Report error to client.  Note: can't be used until second socket has
 * connected to client, or older clients will hang waiting for that
 * connection first.
 */
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

void
#if __STDC__
error(const char *fmt, ...)
#else
error(fmt, va_alist)
	char *fmt;
        va_dcl
#endif
{
	va_list ap;
	int len;
	char *bp, buf[BUFSIZ];
#if __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	bp = buf;
	if (sent_null == 0) {
		*bp++ = 1;
		len = 1;
	} else
		len = 0;
#ifdef OSF_DCE
	/* XXX - not sure if this will work */
	(void)vsprintf(bp, fmt, ap);
#else
	(void)vsnprintf(bp, sizeof(buf) - 1, fmt, ap);
#endif
	(void)write(STDERR_FILENO, buf, len + strlen(bp));
}

void
getstr(buf, cnt, err)
	char *buf, *err;
	int cnt;
{
	char c;

	do {
		if (read(STDIN_FILENO, &c, 1) != 1)
			exit(1);
		*buf++ = c;
		if (--cnt == 0) {
			error("%s too long\n", err);
			exit(1);
		}
	} while (c != 0);
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
	char *p, *maybe = NULL;
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

void
usage()
{

	syslog(LOG_ERR, "usage: rshd [-%s]", OPTIONS);
	exit(2);
}

#ifdef OSF_DCE
void    fatal(int, char *, int);
/*
 * Wrapper routine which calls error(). This is
 * needed so rlogind & rshd can share common code for
 * recvauth()
 */
void
fatal(f, msg, syserr)
        int f;
        char *msg;
        int syserr;
{
    error(msg);
    exit(1);
}
#endif

#if defined(KERBEROS) && defined(KRBV)
krb5_sigtype
  cleanup()
{
#ifdef POSIX_SIGNALS
    struct sigaction sa;

    (void)sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_IGN;
    (void)sigaction(SIGINT, &sa, (struct sigaction *)0);
    (void)sigaction(SIGQUIT, &sa, (struct sigaction *)0);
    (void)sigaction(SIGTERM, &sa, (struct sigaction *)0);
    (void)sigaction(SIGPIPE, &sa, (struct sigaction *)0);
    (void)sigaction(SIGHUP, &sa, (struct sigaction *)0);

    (void)kill(-pid, SIGTERM);
#else
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    killpg(pid, SIGTERM);
#endif
    wait(0);

    pty_logwtmp(ttyn,"","");
    syslog(LOG_INFO ,"Shell process completed.");
    if (ccache)
        krb5_cc_destroy(bsd_context, ccache);
    exit(0);
}
#endif
