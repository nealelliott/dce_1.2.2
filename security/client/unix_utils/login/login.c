/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: login.c,v $
 * Revision 1.1.7.2  1996/02/18  00:07:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:07  marty]
 *
 * Revision 1.1.7.1  1995/12/13  16:24:16  root
 * 	Submit
 * 	[1995/12/11  15:14:04  root]
 * 
 * Revision 1.1.5.4  1993/01/08  22:17:29  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:13  tom]
 * 
 * Revision 1.1.5.3  1992/12/29  13:05:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:54  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:17:11  devsrc
 * 	Revisited the merged code and changed to use sigset() instead of signal()
 * 	[1992/09/16  14:50:11  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:49  sekhar]
 * 
 * Revision 1.1.2.2  1992/06/02  17:02:01  burati
 * 	CR3727 Must use sec_passwd_rec_t for sec_login_valid_and_cert_ident
 * 	[1992/06/02  15:22:54  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  login.c V=4 11/11/91 //littl/prgy/src/client/unix_tools/login
** 
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
**      OSF DCE Security component
** 
**
** This is the bsd4.4 version of login modified to work with the OSF DCE Security
** component.  We expect that vendors will make the corresponding changes to the
** native version of login that ships with their OS rather than using this
** directly and that this code will simply serve as a model.
**
** To simplify the use of this code as a model, we have only made the minimum
** changes necessary for integrating the OSF DCE Security software.  We have not
** changed the basic bsd4.4 code to be POSIX compliant.
*/

/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/*-
 * Copyright (c) 1980, 1987, 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1980, 1987, 1988 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)login.c	5.63 (Berkeley) 6/29/90";
#endif /* not lint */

#if !defined(lint) && defined(OSF_DCE_SECURITY)
/*
 * OSF DCE Version info
 */
static char *VersionID = "@(#)login.c	4 - 11/11/91";
#endif

/*
 * login [ name ]
 * login -h hostname	(for telnetd, etc.)
 * login -f name	(for pre-authenticated login: datakit, xterm, etc.)
 */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <sgtty.h>

#include <utmp.h>
#include <signal.h>
#include <errno.h>
#include <ttyent.h>
#include <syslog.h>
#include <grp.h>
#include <pwd.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <tzfile.h>
#include "pathnames.h"

#define	TTYGRPNAME	"tty"		/* name of group to own ttys */

#ifdef OSF_DCE_SECURITY
#include <locale.h>

/*
 * Pre-bsd4.4 compatibility - whatever it takes to massage this code to
 * run on a bsd4.3 environment without requiring major surgery throughout.
 */
#include <lastlog.h> 

/*
 * bsd4.3 doesn't define sig_t; give it the definition of a signal handler
 * function pointer.  (This definition is currently Domain/OS specific)
 */
#define sig_t __sig_handler_t *

#endif /* OSF_DCE_SECURITY */

#ifdef OSF_DCE_SECURITY
/* 
 * Pick up the DCE sec_login interface
 */

#include <dce/sec/login.h>

/*
 * Principal (user) names can be as long as a pathname
 */
#ifndef UT_NAMESIZE
#   define UT_NAMESIZE MAXPATHLEN
#endif
#ifndef _PATH_LASTLOG
#   define _PATH_LASTLOG "/usr/adm/lastlog"
#endif

/*
 * We fork the parent process and wait for the child to die
 * so that we can destroy the kerberos ticket cache.
 */
#include <sys/wait.h>

/*
 * Define error status manipulation macros.
 */
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)


#endif /* OSF_DCE_SECURITY */

/*
 * This bounds the time given to login.  Not a define so it can
 * be patched on machines where it's too small.
 */
int	timeout = 300;
#ifdef KERBEROS
int	notickets = 1;
#endif

struct	passwd *pwd;
int	failures;
char	term[64], *envinit[1], *hostname, *username, *tty;

main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind;
	extern char *optarg, **environ;
	struct timeval tp;
	struct group *gr;
	register int ch;
	register char *p;
	int ask, fflag, hflag, pflag, cnt, uid;
	int quietlog, rval;
	char *domain, *salt, *ttyn;
#ifdef OSF_DCE_SECURITY
	char tbuf[UT_NAMESIZE + MAXPATHLEN + 2], tname[sizeof(_PATH_TTY) + 10];
#else
	char tbuf[MAXPATHLEN + 2], tname[sizeof(_PATH_TTY) + 10];
#endif
	char localhost[MAXHOSTNAMELEN];
	char *ctime(), *ttyname(), *stypeof(), *crypt(), *getpass();
	time_t time();
	off_t lseek();
	void timedout();

#ifdef OSF_DCE_SECURITY
        /*
         * OSF DCE info...
         */
        sec_passwd_rec_t    pwd_rec;
        sec_login_handle_t  sec_login_handle;
        boolean32           reset_passwd;
        sec_login_auth_src_t auth_src;
        unsigned32          num_groups;
        signed32            *group_set;
        error_status_t      st;
        int                 pid;
#ifdef SNI_SVR4_POSIX
        int                 status;
#else
        union wait          status;
#endif /* SNI_SVR4_POSIX */
#endif /* OSF_DCE_SECURITY */

#ifdef OSF_DCE_SECURITY
	setlocale(LC_ALL, "");
#endif /* OSF_DCE_SECURITY */

#if defined(SNI_SVR4_POSIX)
        /*
        *       FIX-ME:
        *               It would be desirable to use sigaction(2), but the
        *       current implementation of DCE pthreads does not allow
        *       asynchronous signals to be used with sigaction(2).
        *
        *   sigset(2) is used here because, unlike signal(2),
        *   it blocks the signal within the interrupt handler
        *   and does not set its disposition to SIG_DFL.
        *   It also restores the signal mask that was in effect
        *   after exiting the signal handler.
        */
        (void)sigset(SIGALRM, timedout);
#else
	(void)signal(SIGALRM, timedout);
#endif /* SNI_SVR4_POSIX */

	(void)alarm((u_int)timeout);

#if defined(SNI_SVR4_POSIX)
        /*
        *       FIX-ME:
        *               It would be desirable to use sigaction(2), but the
        *       current implementation of DCE pthreads does not allow
        *       asynchronous signals to be used with sigaction(2).
        *               There is no benefit to using sigset(2) over signal
        *       here.
        */
        (void)sigset(SIGQUIT, SIG_IGN);
        (void)sigset(SIGINT, SIG_IGN);
#else
	(void)signal(SIGQUIT, SIG_IGN);
	(void)signal(SIGINT, SIG_IGN);
#endif
	(void)setpriority(PRIO_PROCESS, 0, 0);

	openlog("login", LOG_ODELAY, LOG_AUTH);

	/*
	 * -p is used by getty to tell login not to destroy the environment
 	 * -f is used to skip a second login authentication 
	 * -h is used by other servers to pass the name of the remote
	 *    host to login so that it may be placed in utmp and wtmp
	 */
	domain = NULL;
	if (gethostname(localhost, sizeof(localhost)) < 0)
		syslog(LOG_ERR, "couldn't get local hostname: %m");
	else
		domain = index(localhost, '.');

	fflag = hflag = pflag = 0;
	uid = getuid();
	while ((ch = getopt(argc, argv, "fh:p")) != EOF)
		switch (ch) {
		case 'f':
			fflag = 1;
			break;
		case 'h':
			if (uid) {
				(void)fprintf(stderr,
				    "login: -h for super-user only.\n");
				exit(1);
			}
			hflag = 1;
			if (domain && (p = index(optarg, '.')) &&
			    strcasecmp(p, domain) == 0)
				*p = 0;
			hostname = optarg;
			break;
		case 'p':
			pflag = 1;
			break;
		case '?':
		default:
			if (!uid)
				syslog(LOG_ERR, "invalid flag %c", ch);
			(void)fprintf(stderr,
			    "usage: login [-fp] [username]\n");
			exit(1);
		}
	argc -= optind;
	argv += optind;
	if (*argv) {
		username = *argv;
		if (strlen(username) > UT_NAMESIZE)
			username[UT_NAMESIZE] = '\0';
		ask = 0;
	} else
		ask = 1;

#ifdef OSF_DCE_SECURITY
        /*
         * Don't close all file descriptors at this point - we will need
         * to close them later when exec'ing the child process.  We do this
         * at that point to make sure we catch any file descriptors opened
         * by the rpc runtime (which will be opened as a side effect of the
         * rpc calls invoked through the sec_login interface)
         */
#else
#ifdef SNI_SVR4_POSIX
	for (cnt = sysconf(_SC_OPEN_MAX); cnt > 2; cnt--)
		close(cnt);
#else
	for (cnt = getdtablesize(); cnt > 2; cnt--)
		close(cnt);
#endif /* SNI_SVR4_POSIX */
#endif /* OSF_DCE_SECURITY */

	ttyn = ttyname(0);
	if (ttyn == NULL || *ttyn == '\0') {
		(void)sprintf(tname, "%s??", _PATH_TTY);
		ttyn = tname;
	}
	if (tty = rindex(ttyn, '/'))
		++tty;
	else
		tty = ttyn;

	for (cnt = 0;; ask = 1) {
		if (ask) {
			fflag = 0;
			getloginname();
		}
		/*
		 * Note if trying multiple user names; log failures for
		 * previous user name, but don't bother logging one failure
		 * for nonexistent name (mistyped username).
		 */
		if (failures && strcmp(tbuf, username)) {
			if (failures > (pwd ? 0 : 1))
				badlogin(tbuf);
			failures = 0;
		}
		(void)strcpy(tbuf, username);

#ifdef OSF_DCE_SECURITY
                /*
                 * Get the user's information via the sec_login interface
                 * rather than direct calls to getpwnam.  This interface
                 * will also establish the user's Kerberos Principal
                 * data.
                 */
 	        pwd = NULL;
                salt = "xx";
                sec_login_setup_identity(username,
                                            sec_login_no_flags, 
                                            &sec_login_handle,
                                            &st);
                if (GOOD_STATUS(&st)) {
                    /*
                     * As the login context has not been certified, the 
                     * sec_login imlementation will try local 
                     * passwd data (which is trustworthy) before 
                     * going to the network.  If there is no
                     * local data, then the user isn't
                     * "root".  This allows us to perform the
                     * secure tty check below.  Once the
                     * context is certified, we'll get the
                     * passwd data again.
                     */
                    sec_login_get_pwent(sec_login_handle, &pwd, &st);
                }
#else
		if (pwd = getpwnam(username))
			salt = pwd->pw_passwd;
		else
			salt = "xx";
#endif /* OSF_DCE_SECURITY */

		/*
		 * if we have a valid account name, and it doesn't have a
		 * password, or the -f option was specified and the caller
		 * is root or the caller isn't changing their uid, don't
		 * authenticate.
		 */
		if (pwd && (*pwd->pw_passwd == '\0' ||
		    fflag && (uid == 0 || uid == pwd->pw_uid)))
			break;
		fflag = 0;

		/*
		 * If trying to log in as root, but with insecure terminal,
		 * refuse the login attempt.
		 */
		if (pwd && pwd->pw_uid == 0 && !rootterm(tty)) {
			(void)fprintf(stderr,
			    "%s login refused on this terminal.\n",
			    pwd->pw_name);
			if (hostname)
				syslog(LOG_NOTICE,
				    "LOGIN %s REFUSED FROM %s ON TTY %s",
				    pwd->pw_name, hostname, tty);
			else
				syslog(LOG_NOTICE,
				    "LOGIN %s REFUSED ON TTY %s",
				     pwd->pw_name, tty);
			continue;
		}

		(void)setpriority(PRIO_PROCESS, 0, -4);

		p = getpass("Password:");

		if (pwd) {
#ifdef OSF_DCE_SECURITY
                        /* Set up password record */
                        pwd_rec.key.key_type = sec_passwd_plain;
                        pwd_rec.key.tagged_union.plain = (unsigned char *) p;
                        pwd_rec.pepper = NULL;
                        pwd_rec.version_numer = sec_passwd_c_version_none;

			/* 
			 * validate the user, and ceritfy the security server
			 */
                        rval = sec_login_valid_and_cert_ident(sec_login_handle, 
                                    &pwd_rec, &reset_passwd, &auth_src, &st);
                        /* BAD STATUS - give feedback to user? XXX */
#else
#ifdef KERBEROS
			rval = klogin(pwd, localhost, p);
			if (rval == 1)
				rval = strcmp(crypt(p, salt), pwd->pw_passwd);
#else
			rval = strcmp(crypt(p, salt), pwd->pw_passwd);
#endif /* KERBEROS */
#endif /* OSF_DCE_SECURITY */
		}
		bzero(p, strlen(p));

		(void)setpriority(PRIO_PROCESS, 0, 0);

		if (pwd && !rval)
			break;

		(void)printf("Login incorrect\n");
		failures++;
		/* we allow 10 tries, but after 3 we start backing off */
		if (++cnt > 3) {
			if (cnt >= 10) {
				badlogin(username);
				sleepexit(1);
			}
			sleep((u_int)((cnt - 3) * 5));
		}
	}

	/* committed to login -- turn off timeout */
	(void)alarm((u_int)0);

#ifdef OSF_DCE_SECURITY
	/*
	 * Now that we're sure that the security server is legit, 
         * get the passwd info.  If this call fails, just bail out
         * 'cause we can't trust the uid/gid
         */
	sec_login_get_pwent(sec_login_handle, &pwd, &st);
#else
	/* paranoia... */
	endpwent();
#endif /* OSF_DCE_SECURITY */
	/* if user not super-user, check for disabled logins */
	if (pwd->pw_uid)
		checknologin();

	if (chdir(pwd->pw_dir) < 0) {
		(void)printf("No directory %s!\n", pwd->pw_dir);
		if (chdir("/"))
			exit(0);
		pwd->pw_dir = "/";
		(void)printf("Logging in with home = \"/\".\n");
	}

	quietlog = access(_PATH_HUSHLOGIN, F_OK) == 0;

	if (pwd->pw_change || pwd->pw_expire)
		(void)gettimeofday(&tp, (struct timezone *)NULL);
	if (pwd->pw_change)
		if (tp.tv_sec >= pwd->pw_change) {
			(void)printf("Sorry -- your password has expired.\n");
			sleepexit(1);
		}
		else if (pwd->pw_change - tp.tv_sec <
		    2 * DAYSPERWEEK * SECSPERDAY && !quietlog)
			(void)printf("Warning: your password expires on %s",
			    ctime(&pwd->pw_expire));
	if (pwd->pw_expire)
		if (tp.tv_sec >= pwd->pw_expire) {
			(void)printf("Sorry -- your account has expired.\n");
			sleepexit(1);
		}
		else if (pwd->pw_expire - tp.tv_sec <
		    2 * DAYSPERWEEK * SECSPERDAY && !quietlog)
			(void)printf("Warning: your account expires on %s",
			    ctime(&pwd->pw_expire));

	/* nothing else left to fail -- really log in */
	{
		struct utmp utmp;

		bzero((void *)&utmp, sizeof(utmp));
		(void)time(&utmp.ut_time);
		strncpy(utmp.ut_name, username, sizeof(utmp.ut_name));
		if (hostname)
			strncpy(utmp.ut_host, hostname, sizeof(utmp.ut_host));
		strncpy(utmp.ut_line, tty, sizeof(utmp.ut_line));
		login(&utmp);
	}

	dolastlog(quietlog);

	(void)chown(ttyn, pwd->pw_uid,
	    (gr = getgrnam(TTYGRPNAME)) ? gr->gr_gid : pwd->pw_gid);
	(void)chmod(ttyn, 0620);
	(void)setgid(pwd->pw_gid);

#ifdef OSF_DCE_SECURITY
        /*
         * At DCE 1.0, a locally authenticated context contains no
         * groupset info, so use the initgroups() to establish 
         * group list using /etc/grps
         */
	if (auth_src == sec_login_auth_src_network) {
	        /*
	         * Establish the user's concurrent group set.
	         */
	        sec_login_get_groups(sec_login_handle, &num_groups,
	                                &group_set, &st);
	        if (num_groups > NGROUPS) {
	            num_groups = NGROUPS;
	        }
	        setgroups(num_groups, group_set); 
	} else {
		initgroups(username, pwd->pw_gid);
        }
#else
	initgroups(username, pwd->pw_gid);
#endif /* OSF_DCE_SECURITY */

	if (*pwd->pw_shell == '\0')
		pwd->pw_shell = _PATH_BSHELL;

	/* destroy environment unless user has requested preservation */
	if (!pflag)
		environ = envinit;
	(void)setenv("HOME", pwd->pw_dir, 1);
	(void)setenv("SHELL", pwd->pw_shell, 1);
	if (term[0] == '\0')
		strncpy(term, stypeof(tty), sizeof(term));
	(void)setenv("TERM", term, 0);
	(void)setenv("USER", pwd->pw_name, 1);
	(void)setenv("PATH", _PATH_DEFPATH, 0);

	if (tty[sizeof("tty")-1] == 'd')
		syslog(LOG_INFO, "DIALUP %s, %s", tty, pwd->pw_name);
	/* if fflag is on, assume caller/authenticator has logged root login */
	if (pwd->pw_uid == 0 && fflag == 0)
		if (hostname)
			syslog(LOG_NOTICE, "ROOT LOGIN ON %s FROM %s",
			    tty, hostname);
		else
			syslog(LOG_NOTICE, "ROOT LOGIN ON %s", tty);

#ifdef KERBEROS
	if (!quietlog && notickets == 1)
		(void)printf("Warning: no Kerberos tickets issued.\n");
#endif

	if (!quietlog) {
		struct stat st;

		motd();
		(void)sprintf(tbuf, "%s/%s", _PATH_MAILDIR, pwd->pw_name);
		if (stat(tbuf, &st) == 0 && st.st_size != 0)
			(void)printf("You have %smail.\n",
			    (st.st_mtime > st.st_atime) ? "new " : "");
	}

#if defined(SNI_SVR4_POSIX)
        /*
        *       FIX-ME:
        *               It would be desirable to use sigaction(2), but the
        *       current implementation of DCE pthreads does not allow
        *       asynchronous signals to be used with sigaction(2).
        *               There is no benefit to using sigset(2) over signal
        *       here.
        */

	(void)sigset(SIGALRM, SIG_DFL);
	(void)sigset(SIGQUIT, SIG_DFL);
	(void)sigset(SIGINT, SIG_DFL);
	(void)sigset(SIGTSTP, SIG_IGN);
#else
	(void)signal(SIGALRM, SIG_DFL);
	(void)signal(SIGQUIT, SIG_DFL);
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTSTP, SIG_IGN);
#endif
	tbuf[0] = '-';
	strcpy(tbuf + 1, (p = rindex(pwd->pw_shell, '/')) ?
	    p + 1 : pwd->pw_shell);

	if (setlogin(pwd->pw_name) < 0)
		syslog(LOG_ERR, "setlogin() failure: %m");

#ifdef OSF_DCE_SECURITY
        /*
         * Establish network credentials
         */
        sec_login_set_context(sec_login_handle, &st);
#endif /* OSF_DCE_SECURITY */

	/* discard permissions last so can't get killed and drop core */
	(void)setuid(pwd->pw_uid);

	execlp(pwd->pw_shell, tbuf, 0);
	(void)fprintf(stderr, "login: no shell: %s.\n", strerror(errno));
	exit(0);
}

getloginname()
{
	register int ch;
	register char *p;
	static char nbuf[UT_NAMESIZE + 1];

	for (;;) {
		(void)printf("login: ");
		for (p = nbuf; (ch = getchar()) != '\n'; ) {
			if (ch == EOF) {
				badlogin(username);
				exit(0);
			}
			if (p < nbuf + UT_NAMESIZE)
				*p++ = ch;
		}
		if (p > nbuf)
			if (nbuf[0] == '-')
				(void)fprintf(stderr,
				    "login names may not start with '-'.\n");
			else {
				*p = '\0';
				username = nbuf;
				break;
			}
	}
}

void
timedout()
{
	(void)fprintf(stderr, "Login timed out after %d seconds\n", timeout);
	exit(0);
}

rootterm(ttyn)
	char *ttyn;
{
	struct ttyent *t;

	return((t = getttynam(ttyn)) && t->ty_status&TTY_SECURE);
}

#if defined(SNI_SVR4_POSIX)
sigjmp_buf motdinterrupt;
#else
jmp_buf motdinterrupt;
#endif /* SNI_SVR4_POSIX */

motd()
{
	register int fd, nchars;
	sig_t oldint;
	int sigint();
	char tbuf[8192];

	if ((fd = open(_PATH_MOTDFILE, O_RDONLY, 0)) < 0)
		return;
#if defined(SNI_SVR4_POSIX)
        /*
        *   FIX-ME:
        *       It would be desirable to use sigaction(2) here but
        *   the current implementation of DCE pthreads does not
        *   allow sigaction to be used with asynchronous signals.
        *
        *   sigset(2) is used here because, unlike signal(2),
        *   it ignores the signal within the interrupt handler
        *   and does not set its disposition to SIG_DFL.
        *   It also restores the signal mask that was in effect
        *   before the signal handler was invoked, unlike signal(2).
        *
        */
        oldint = sigset(SIGINT, sigint);
#else
	oldint = signal(SIGINT, sigint);
#endif /* SNI_SVR4_POSIX */
#if defined(SNI_SVR4_POSIX)
	if (sigsetjmp(motdinterrupt,1) == 0)
#else
	if (setjmp(motdinterrupt) == 0)
#endif /* SNI_SVR4_POSIX */
		while ((nchars = read(fd, tbuf, sizeof(tbuf))) > 0)
			(void)write(fileno(stdout), tbuf, nchars);
#if defined(SNI_SVR4_POSIX)
        /*
        *   FIX-ME:
        *       It would be desirable to use sigaction(2) here but
        *   the current implementation of DCE pthreads does not
        *   allow sigaction to be used with asynchronous signals.
        *
        *   sigset(2) is used here because, unlike signal(2),
        *   it ignores the signal within the interrupt handler
        *   and does not set its disposition to SIG_DFL.
        *   It also restores the signal mask that was in effect
        *   before the signal handler was invoked, unlike signal(2).
        *
        */
        (void)sigset(SIGINT, oldint);
#else
	(void)signal(SIGINT, oldint);
#endif /* SNI_SVR4_POSIX */
	(void)close(fd);
}

sigint()
{
#if defined(SNI_SVR4_POSIX)
	siglongjmp(motdinterrupt, 1);
#else
	longjmp(motdinterrupt, 1);
#endif /* SNI_SVR4_POSIX */
}

checknologin()
{
	register int fd, nchars;
	char tbuf[8192];

	if ((fd = open(_PATH_NOLOGIN, O_RDONLY, 0)) >= 0) {
		while ((nchars = read(fd, tbuf, sizeof(tbuf))) > 0)
			(void)write(fileno(stdout), tbuf, nchars);
		sleepexit(0);
	}
}

dolastlog(quiet)
	int quiet;
{
	struct lastlog ll;
	int fd;
	char *ctime();

	if ((fd = open(_PATH_LASTLOG, O_RDWR, 0)) >= 0) {
		(void)lseek(fd, (off_t)pwd->pw_uid * sizeof(ll), L_SET);
		if (!quiet) {
			if (read(fd, (char *)&ll, sizeof(ll)) == sizeof(ll) &&
			    ll.ll_time != 0) {
				(void)printf("Last login: %.*s ",
				    24-5, (char *)ctime(&ll.ll_time));
				if (*ll.ll_host != '\0')
					(void)printf("from %.*s\n",
					    sizeof(ll.ll_host), ll.ll_host);
				else
					(void)printf("on %.*s\n",
					    sizeof(ll.ll_line), ll.ll_line);
			}
			(void)lseek(fd, (off_t)pwd->pw_uid * sizeof(ll), L_SET);
		}
		bzero((void *)&ll, sizeof(ll));
		(void)time(&ll.ll_time);
		strncpy(ll.ll_line, tty, sizeof(ll.ll_line));
		if (hostname)
			strncpy(ll.ll_host, hostname, sizeof(ll.ll_host));
		(void)write(fd, (char *)&ll, sizeof(ll));
		(void)close(fd);
	}
}

badlogin(name)
	char *name;
{
	if (failures == 0)
		return;
	if (hostname)
		syslog(LOG_NOTICE, "%d LOGIN FAILURE%s FROM %s, %s",
		    failures, failures > 1 ? "S" : "", hostname, name);
	else
		syslog(LOG_NOTICE, "%d LOGIN FAILURE%s ON %s, %s",
		    failures, failures > 1 ? "S" : "", tty, name);
}

#undef	UNKNOWN
#define	UNKNOWN	"su"

char *
stypeof(ttyid)
	char *ttyid;
{
	struct ttyent *t;

	return(ttyid && (t = getttynam(ttyid)) ? t->ty_type : UNKNOWN);
}

sleepexit(eval)
	int eval;
{
	sleep((u_int)5);
	exit(eval);
}
