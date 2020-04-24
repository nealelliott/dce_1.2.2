/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: su.c,v $
 * Revision 1.1.5.2  1996/02/18  00:08:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:12  marty]
 *
 * Revision 1.1.5.1  1995/12/13  16:24:20  root
 * 	Submit
 * 	[1995/12/11  15:14:07  root]
 * 
 * Revision 1.1.3.4  1993/01/08  22:17:35  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:19  tom]
 * 
 * Revision 1.1.3.3  1992/12/29  13:05:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:59  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:17:21  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:20:58  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:47:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  su.c V=2 11/11/91 //littl/prgy/src/client/unix_tools/su
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*  su.c V=2 11/11/91 //littl/prgy/src/client/unix_tools/su
** 
** Copyright (c) Hewlett-Packard Company 1991
**  Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
**      OSF DCE Security component
** 
**
** This is the bsd4.4 version of su modified to work with the OSF DCE Security
** component.  We expect that vendors will make the corresponding changes to
** native version of su that ships with their OS rather than using this
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
/*
 * Copyright (c) 1988 The Regents of the University of California.
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
"@(#) Copyright (c) 1988 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)su.c	5.21 (Berkeley) 6/20/90";
#endif /* not lint */

#if !defined(lint) && defined(OSF_DCE_SECURITY)
/*
 * OSF DCE Version info
 */
static char *VersionID = "@(#)su.c	2 - 11/11/91";
#endif


#include <sys/param.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <syslog.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <unistd.h>
#include "pathnames.h"

#ifdef OSF_DCE_SECURITY
#include <locale.h>

/* 
 * Pick up the DCE sec_login interface
 */

#include <dce/sec/login.h>

/*
 * Define error status manipulation macros.
 */
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)

#endif /* OSF_DCE_SECURITY */

#ifdef KERBEROS
#include <kerberosIV/des.h>
#include <kerberosIV/krb.h>
#include <netdb.h>

#define	ARGSTR	"-Kflm"

int use_kerberos = 1;
#else
#define	ARGSTR	"-flm"
#endif

main(argc, argv)
	int argc;
	char **argv;
{
	extern char **environ;
	extern int errno, optind;
#ifdef OSF_DCE_SECURITY
        /*
         * We can't take the address of a register variable, and this pointer
         * must be written to by the sec_login code.
         */
	struct passwd *pwd;
#else
	register struct passwd *pwd;
#endif
	register char *p, **g;
	struct group *gr;
	uid_t ruid, getuid();
	int asme, ch, asthem, fastlogin, prio;
	enum { UNSET, YES, NO } iscsh = UNSET;
	char *user, *shell, *username, *cleanenv[2], *nargv[4], **np;
	char shellbuf[MAXPATHLEN];
	char *crypt(), *getpass(), *getenv(), *getlogin(), *mytty();
#ifdef OSF_DCE_SECURITY
        /*
         * OSF DCE info...
         */
        sec_login_handle_t  sec_login_handle;
        boolean32           reset_passwd;
        sec_login_auth_src_t auth_src;
        unsigned32          num_groups;
        signed32            *group_set;
        error_status_t      st;
        int                 cnt;
#endif /* OSF_DCE_SECURITY */

#ifdef OSF_DCE_SECURITY
	setlocale(LC_ALL, "");
#endif /* OSF_DCE_SECURITY */

	np = &nargv[3];
	*np-- = NULL;
	asme = asthem = fastlogin = 0;
	while ((ch = getopt(argc, argv, ARGSTR)) != EOF)
		switch((char)ch) {
#ifdef KERBEROS
		case 'K':
			use_kerberos = 0;
			break;
#endif
		case 'f':
			fastlogin = 1;
			break;
		case '-':
		case 'l':
			asme = 0;
			asthem = 1;
			break;
		case 'm':
			asme = 1;
			asthem = 0;
			break;
		case '?':
		default:
			(void)fprintf(stderr, "usage: su [%s] [login]\n",
			    ARGSTR);
			exit(1);
		}
	argv += optind;

	errno = 0;
	prio = getpriority(PRIO_PROCESS, 0);
	if (errno)
		prio = 0;
	(void)setpriority(PRIO_PROCESS, 0, -2);

	/* get current login name and shell */
	if ((pwd = getpwuid(ruid = getuid())) == NULL) {
		fprintf(stderr, "su: who are you?\n");
		exit(1);
	}
	username = strdup(pwd->pw_name);
	if (asme)
		if (pwd->pw_shell && *pwd->pw_shell)
			shell = strcpy(shellbuf,  pwd->pw_shell);
		else {
			shell = _PATH_BSHELL;
			iscsh = NO;
		}

	/* get target login information, default to root */
	user = *argv ? *argv : "root";

#ifdef OSF_DCE_SECURITY
        /*
         * Get the user's information via the sec_login interface
         * rather than direct calls to getpwnam.  This interface
         * will also establish the user's Kerberos Principal
         * data. 
         *
         * A network "su" requires a password, regardless of
         * whether or not the user is "root". 
         *        
         */
        pwd = NULL;
        sec_login_setup_identity(username, sec_login_no_flags, &sec_login_handle,
                                 &st);  
        if (BAD_STATUS&&st)) {
		fprintf(stderr, "su: unknown login %s\n", user);
		exit(1);
	}

	p = getpass("Password:");
        if (!sec_login_valid_and_cert_ident(sec_login_handle,
                            p, &reset_passwd, &auth_src, &st)) {
		fprintf(stderr, "Sorry\n");
			syslog(LOG_AUTH|LOG_CRIT,
				"BAD SU %s on %s to %s", username,
				mytty(), user);
			exit(1);
                }

        sec_login_get_pwent(sec_login_handle, &pwd, &st);
        if (BAD_STATUS(&st)) {

#else
	if ((pwd = getpwnam(user)) == NULL) {

#endif /* OSF_DCE_SECURITY */

		fprintf(stderr, "su: unknown login %s\n", user);
		exit(1);
	}

	/* only allow those in group zero to su to root. */
	if (pwd->pw_uid == 0 && (gr = getgrgid((gid_t)0)))
		for (g = gr->gr_mem;; ++g) {
			if (!*g) {
				(void)fprintf(stderr,
				    "su: you are not in the correct group to su %s.\n", user);
				exit(1);
			}
			if (!strcmp(username, *g))
				break;
		}
	openlog("su", LOG_CONS, 0);

#ifndef OSF_DCE_SECURITY
	if (ruid) {
#ifdef KERBEROS
		if (!use_kerberos || kerberos(username, user, pwd->pw_uid))
#endif
		/* if target requires a password, verify it */
		if (*pwd->pw_passwd) {
			p = getpass("Password:");
			if (strcmp(pwd->pw_passwd, crypt(p, pwd->pw_passwd))) {
				fprintf(stderr, "Sorry\n");
				syslog(LOG_AUTH|LOG_CRIT,
					"BAD SU %s on %s to %s", username,
					mytty(), user);
				exit(1);
			}
		}
#endif /* OSF_DCE_SECURITY */
	}

	if (asme) {
		/* if asme and non-standard target shell, must be root */
		if (!chshell(pwd->pw_shell) && ruid) {
			(void)fprintf(stderr,
				"su: permission denied (shell).\n");
			exit(1);
		}
	} else if (pwd->pw_shell && *pwd->pw_shell) {
		shell = pwd->pw_shell;
		iscsh = UNSET;
	} else {
		shell = _PATH_BSHELL;
		iscsh = NO;
	}

	/* if we're forking a csh, we want to slightly muck the args */
	if (iscsh == UNSET) {
		if (p = rindex(shell, '/'))
			++p;
		else
			p = shell;
		iscsh = strcmp(p, "csh") ? NO : YES;
	}

	/* set permissions */
	if (setgid(pwd->pw_gid) < 0) {
		perror("su: setgid");
		exit(1);
	}

#ifdef OSF_DCE_SECURITY
        
        /*
         * Establish the user's concurrent group set.
         * At DCE 1.0, a locally authenticated context contains no
         * groupset info, so use the initgroups() to establish 
         * group list using /etc/grps
         */
	if (auth_src == sec_login_auth_src_network) {
	        sec_login_get_groups(sec_login_handle, &num_groups,
	                                &group_set, &st);
	        if (num_groups > NGROUPS) {
	            num_groups = NGROUPS;
	        }
	        if (setgroups(num_groups, group_set)) {
			(void)fprintf(stderr, "su: initgroups failed.\n");
			exit(1);
		}
        } else {
		if (initgroups(user, pwd->pw_gid)) {
			(void)fprintf(stderr, "su: initgroups failed.\n");
			exit(1);
		}
	}

        /*
         * Establish network credentials
         */
        sec_login_set_context(sec_login_handle, &st);

        /*
         * Close any open file descriptors (This action was delayed
         * to this point to make sure that we would catch any opened
         * by the RPC runtime)
         */
#ifdef SNI_SVR4_POSIX
	for (cnt = sysconf(_SC_OPEN_MAX); cnt > 2; cnt--)
		close(cnt);
#else
	for (cnt = getdtablesize(); cnt > 2; cnt--) {
		close(cnt);
        }
#endif /* SNI_SVR4_POSIX */

#else
	if (initgroups(user, pwd->pw_gid)) {
		(void)fprintf(stderr, "su: initgroups failed.\n");
		exit(1);
	}
#endif /* OSF_DCE_SECURITY */


	if (setuid(pwd->pw_uid) < 0) {
		perror("su: setuid");
		exit(1);
	}

	if (!asme) {
		if (asthem) {
			p = getenv("TERM");
			cleanenv[0] = _PATH_SEARCHPATH;
			cleanenv[1] = NULL;
			environ = cleanenv;
			(void)setenv("TERM", p, 1);
			if (chdir(pwd->pw_dir) < 0) {
				fprintf(stderr, "su: no directory\n");
				exit(1);
			}
		}
		if (asthem || pwd->pw_uid)
			(void)setenv("USER", pwd->pw_name, 1);
		(void)setenv("HOME", pwd->pw_dir, 1);
		(void)setenv("SHELL", shell, 1);
	}

	if (iscsh == YES) {
		if (fastlogin)
			*np-- = "-f";
		if (asme)
			*np-- = "-m";
	}

	/* csh strips the first character... */
	*np = asthem ? "-su" : iscsh == YES ? "_su" : "su";

	syslog(LOG_NOTICE|LOG_AUTH, "%s on %s to %s", username, mytty(), user);

	(void)setpriority(PRIO_PROCESS, 0, prio);

	execv(shell, np);
	(void)fprintf(stderr, "su: %s not found.\n", shell);
	exit(1);
}

chshell(sh)
	char *sh;
{
	register char *cp;
	char *getusershell();

	while ((cp = getusershell()) != NULL)
		if (!strcmp(cp, sh))
			return(1);
	return(0);
}

char *
mytty()
{
	char *p, *ttyname();

	return((p = ttyname(STDERR_FILENO)) ? p : "UNKNOWN TTY");
}

#ifdef KERBEROS
kerberos(username, user, uid)
	char *username, *user;
	int uid;
{
	extern char *krb_err_txt[];
	KTEXT_ST ticket;
	AUTH_DAT authdata;
	struct hostent *hp;
	register char *p;
	int kerno;
	u_long faddr;
	char lrealm[REALM_SZ], krbtkfile[MAXPATHLEN];
	char hostname[MAXHOSTNAMELEN], savehost[MAXHOSTNAMELEN];
	char *mytty();

	if (krb_get_lrealm(lrealm, 1) != KSUCCESS) {
		(void)fprintf(stderr, "su: couldn't get local realm.\n");
		return(1);
	}
	if (koktologin(username, lrealm, user) && !uid) {
		(void)fprintf(stderr, "kerberos su: not in %s's ACL.\n", user);
		return(1);
	}
	(void)sprintf(krbtkfile, "%s_%s_%d", TKT_ROOT, user, getuid());

	(void)setenv("KRBTKFILE", krbtkfile, 1);
	if (setuid(0) < 0) {
		perror("su: setuid");
		return(1);
	}
	(void)unlink(krbtkfile);

	/*
	 * Little trick here -- if we are su'ing to root,
	 * we need to get a ticket for "xxx.root", where xxx represents
	 * the name of the person su'ing.  Otherwise (non-root case),
	 * we need to get a ticket for "yyy.", where yyy represents
	 * the name of the person being su'd to, and the instance is null
	 *
	 * Also: POLICY: short ticket lifetime for root
	 */
	kerno = krb_get_pw_in_tkt((uid == 0 ? username : user),
		(uid == 0 ? "root" : ""), lrealm,
	    	"krbtgt", lrealm, (uid == 0 ? 2 : DEFAULT_TKT_LIFE), 0);

	if (kerno != KSUCCESS) {
		if (kerno == KDC_PR_UNKNOWN) {
			fprintf(stderr, "principal unknown: %s.%s@%s\n",
				(uid == 0 ? username : user),
				(uid == 0 ? "root" : ""), lrealm);
			return(1);
		}
		(void)printf("su: unable to su: %s\n", krb_err_txt[kerno]);
		syslog(LOG_NOTICE|LOG_AUTH,
		    "su: BAD Kerberos SU: %s on %s to %s: %s",
		    username, mytty(), user, krb_err_txt[kerno]);
		return(1);
	}

	if (chown(krbtkfile, uid, -1) < 0) {
		perror("su: chown:");
		(void)unlink(krbtkfile);
		return(1);
	}

	(void)setpriority(PRIO_PROCESS, 0, -2);

	if (gethostname(hostname, sizeof(hostname)) == -1) {
		perror("su: hostname");
		dest_tkt();
		return(1);
	}

	(void)strncpy(savehost, krb_get_phost(hostname), sizeof(savehost));
	savehost[sizeof(savehost) - 1] = '\0';

	kerno = krb_mk_req(&ticket, "rcmd", savehost, lrealm, 33);

	if (kerno == KDC_PR_UNKNOWN) {
		(void)printf("Warning: tgt not verified.\n");
		syslog(LOG_NOTICE|LOG_AUTH,
			"su: %s on %s to %s, TGT not verified",
		    	username, mytty(), user);
	} else if (kerno != KSUCCESS) {
		(void)printf("Unable to use TGT: %s\n", krb_err_txt[kerno]);
		syslog(LOG_NOTICE|LOG_AUTH, "su: failed su: %s on %s to %s: %s",
		    username, mytty(), user, krb_err_txt[kerno]);
		dest_tkt();
		return(1);
	} else {
		if (!(hp = gethostbyname(hostname))) {
			(void)printf("su: can't get addr of %s\n", hostname);
			dest_tkt();
			return(1);
		}
		(void)bcopy((char *)hp->h_addr, (char *)&faddr, sizeof(faddr));

		if ((kerno = krb_rd_req(&ticket, "rcmd", savehost, faddr,
		    &authdata, "")) != KSUCCESS) {
			(void)printf("su: unable to verify rcmd ticket: %s\n",
			    krb_err_txt[kerno]);
			syslog(LOG_NOTICE|LOG_AUTH,
			    "su: failed su: %s on %s to %s: %s", username,
			    mytty(), user, krb_err_txt[kerno]);
			dest_tkt();
			return(1);
		}
	}
	return(0);
}

koktologin(name, realm, toname)
	char *name, *realm, *toname;
{
	register AUTH_DAT *kdata;
	AUTH_DAT kdata_st;

	kdata = &kdata_st;
	bzero((caddr_t) kdata, sizeof(*kdata));
	(void)strcpy(kdata->pname, name);
	(void)strcpy(kdata->pinst,
	    ((strcmp(toname, "root") == 0) ? "root" : ""));
	(void)strcpy(kdata->prealm, realm);
	return(kuserok(kdata, toname));
}
#endif
