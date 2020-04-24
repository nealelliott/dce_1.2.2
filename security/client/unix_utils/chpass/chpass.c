/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: chpass.c,v $
 * Revision 1.1.6.2  1996/02/18  00:07:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:59  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:07  root
 * 	Submit
 * 	[1995/12/11  15:13:57  root]
 * 
 * Revision 1.1.4.4  1993/03/24  19:23:02  burati
 * 	case 'n' should check for lflag already set, not nflag
 * 	[1993/03/24  18:24:36  burati]
 * 
 * Revision 1.1.4.3  1993/01/08  22:17:24  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:03:38  tom]
 * 
 * Revision 1.1.4.2  1992/12/29  13:04:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:42:36  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/17  19:00:13  burati
 * 	Integration with DCE
 * 	[1992/07/17  18:44:27  burati]
 * 
 * 	        Initial version from BSD4.4
 * 	[1992/07/13  18:53:06  burati]
 * 
 * $EndLog$
 */

/*-
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

/* CHPASS - Reference implementation.
 *     This source is provided as an example of the integration
 * necessary to provide chpass functionality in a DCE environment.
 * This generic Berkeley chpass utility has been modified to make
 * the necessary DCE calls, to provide most of the functionality
 * available in the UNIX(tm) version.  It is not provided in
 * buildable form, and will need porting and completion work on
 * each and every platform.
 *
 *  UID changes are not allowed in this version because of a DCE
 *  restriction, that UIDs corresponding to a particular name may
 *  not be changed.  In order to implement this functionality,
 *  you would need to delete the account and principal/uid, then
 *  recreate the principal with the new uid and recreate the acct
 *  (saving all previous account information that wasn't changed).
 *
 *  GID changes are not implemented in this version.  To change
 *  a group id, chpass would have to get the name of this new
 *  group (making sure it exists), then perform a sec_rgy_acct_rename
 *
 *  Because password lifespan and expiration dates are part of the
 *  DCE registry policy, as opposed to being attached to particular
 *  accounts, any change to the password expiration date via this
 *  interface will be ignored.  It is there for display purposes
 *  only.
 *
 *  Future versions of this chpass reference implementation will
 *  allow the utility to manipulate the password_override file,
 *  but that functionality is not available in this 1.0.1 version.
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1988 The Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)chpass.c        5.15 (Berkeley) 6/29/90";
#endif /* not lint */

#include <sys/param.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pwd.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <pathnames.h>
#include <chpass.h>
#include <locale.h>

char e1[] = ": ";
char e2[] = ":,";

struct entry list[] = {
        { "Login",                p_login,  1,   5, e1,   },
        { "Password",                p_passwd, 1,   8, e1,   },
        { "Uid",                p_uid,    1,   3, e1,   },
        { "Gid",                p_gid,    1,   3, e1,   },
        { "Class",                p_class,  1,   5, e1,   },
        { "Change",                p_change, 1,   6, NULL, },
        { "Expire",                p_expire, 1,   6, NULL, },
#define        E_NAME                7
        { "Full Name",                p_gecos,  0,   9, e2,   },
#define        E_BPHONE        8
        { "Office Phone",        p_gecos,  0,  12, e2,   },
#define        E_HPHONE        9
        { "Home Phone",                p_gecos,  0,  10, e2,   },
#define        E_LOCATE        10
        { "Location",                p_gecos,  0,   8, e2,   },
        { "Home directory",        p_hdir,   1,  14, e1,   },
#define        E_SHELL                12
        { "Shell",                p_shell,  0,   5, e1,   },
        { NULL, 0, },
};

uid_t uid;

void error_msg
#ifndef __STDC__
    (message, status_p)
    char            *message;
    error_status_t  *status_p;
#else
  (
    char            *message,
    error_status_t  *status_p
  )
#endif
{
    int                 tmp_stat;
    dce_error_string_t  buf;
    dce_error_inq_text(*status_p, buf, &tmp_stat);
    fprintf(stderr, "chpass: %s - %s\n", message, buf);
}


static int edit
#ifndef __STDC__
    (file)
        char *file;
#else
  (
        char *file
  )
#endif
{
        int status, pid, w;
        char *p, *editor, *getenv();

        if (editor = getenv("EDITOR")) {
                if (p = rindex(editor, '/'))
                        ++p;
                else
                        p = editor;
        }
        else
                p = editor = "vi";
        if (!(pid = vfork())) {
                (void)setgid(getgid());
                (void)setuid(getuid());
                execlp(editor, p, file, NULL);
                (void)fprintf(stderr, "chpass: can't find \"%s\".\n", editor);
                _exit(127);
        }
        while ((w = wait(&status)) != pid && w != -1);
        return(w == -1 || status);
}


static void loadpw
#ifndef __STDC__
    (arg, pw)
        char *arg;
        register struct passwd *pw;
#else
  (
        char *arg,
        register struct passwd *pw
  )
#endif
{
        register char *cp;
        char *bp = arg;
        long atol();
        char *strsep();

        pw->pw_name = strsep(&bp, ":");
        pw->pw_passwd = strsep(&bp, ":");
        if (!(cp = strsep(&bp, ":")))
                goto bad;
        pw->pw_uid = atoi(cp);
        if (!(cp = strsep(&bp, ":")))
                goto bad;
        pw->pw_gid = atoi(cp);
        pw->pw_class = strsep(&bp, ":");
        if (!(cp = strsep(&bp, ":")))
                goto bad;
        pw->pw_change = atol(cp);
        if (!(cp = strsep(&bp, ":")))
                goto bad;
        pw->pw_expire = atol(cp);
        pw->pw_gecos = strsep(&bp, ":");
        pw->pw_dir = strsep(&bp, ":");
        pw->pw_shell = strsep(&bp, ":");
        if (!pw->pw_shell || strsep(&bp, ":")) {
bad:                (void)fprintf(stderr, "chpass: bad password list.\n");
                exit(1);
        }
}

static int prompt(
#ifdef __STDC__
void
#endif
)
{
        register int c;

        for (;;) {
                (void)printf("re-edit the password entry? [y]: ");
                (void)fflush(stdout);
                c = getchar();
                if (c != EOF && c != (int)'\n')
                        while (getchar() != (int)'\n');
                return(c == (int)'n');
        }
        /* NOTREACHED */
}


static void baduser(
#ifdef __STDC__
void
#endif
)
{
        (void)fprintf(stderr, "chpass: %s\n", strerror(EACCES));
        exit(1);
}


static void usage_exit(
#ifdef __STDC__
void
#endif
)
{
        (void)fprintf(stderr, "usage: chpass [-a list] [-s shell] [user] [-l | -n]\n");
        exit(1);
}


static int check
#ifndef __STDC__
    (fp, pw)
        FILE *fp;
        struct passwd *pw;
#else
  (
        FILE *fp,
        struct passwd *pw
  )
#endif
{
        register struct entry *ep;
        register char *p;
        static char buf[1024];

        while (fgets(buf, sizeof(buf), fp)) {
                if (!buf[0] || buf[0] == '#')
                        continue;
                if (!(p = index(buf, '\n'))) {
                        (void)fprintf(stderr, "chpass: line too long.\n");
                        return(0);
                }
                *p = '\0';
                for (ep = list;; ++ep) {
                        if (!ep->prompt) {
                                (void)fprintf(stderr,
                                    "chpass: unrecognized field.\n");
                                return(0);
                        }
                        if (!strncasecmp(buf, ep->prompt, ep->len)) {
                                if (ep->restricted && uid) {
                                        (void)fprintf(stderr,
                                            "chpass: you may not change the %s field.\n",
                                            ep->prompt);
                                        return(0);
                                }
                                if (!(p = index(buf, ':'))) {
                                        (void)fprintf(stderr,
                                            "chpass: line corrupted.\n");
                                        return(0);
                                }
                                while (isspace(*++p));
                                if (ep->except && strpbrk(p, ep->except)) {
                                        (void)fprintf(stderr,
                                            "chpass: illegal character in the \"%s\" field.\n",
                                            ep->prompt);
                                        return(0);
                                }
                                if ((ep->func)(p, pw, ep))
                                        return(0);
                                break;
                        }
                }
        }
        /*
         * special checks...
         *
         * there has to be a limit on the size of the gecos fields,
         * otherwise getpwent(3) can choke.
         */
        if (strlen(list[E_NAME].save) + strlen(list[E_BPHONE].save) +
            strlen(list[E_HPHONE].save) + strlen(list[E_LOCATE].save)
            > 512) {
                (void)fprintf(stderr, "chpass: gecos field too large.\n");
                exit(1);
        }
        (void)sprintf(pw->pw_gecos = buf, "%s,%s,%s,%s",
            list[E_NAME].save, list[E_LOCATE].save, list[E_BPHONE].save,
            list[E_HPHONE].save);
        return(1);
}


static int info
#ifndef __STDC__
    (pw)
        struct passwd *pw;
#else
  (
        struct passwd *pw
  )
#endif
{
        struct stat begin, end;
        FILE *fp;
        int fd, rval;
        char *tfile;

        tfile = _PATH_TMP;
        if ((fd = mkstemp(tfile)) == -1 || !(fp = fdopen(fd, "w+"))) {
                (void)fprintf(stderr, "chpass: no temporary file");
                return(0);
        }

        /*
         * if print doesn't print out a shell field, make it restricted.
         * Not particularly pretty, but print is the routine that checks
         * to see if the user can change their shell.
         */
        if (!print(fp, pw))
                list[E_SHELL].restricted = 1;
        (void)fflush(fp);

        /*
         * give the file to the real user; setuid permissions
         * are discarded in edit()
         */
        (void)fchown(fd, getuid(), getgid());

        for (rval = 0;;) {
                (void)fstat(fd, &begin);
                if (edit(tfile)) {
                        (void)fprintf(stderr, "chpass: edit failed; ");
                        break;
                }
                (void)fstat(fd, &end);
                if (begin.st_mtime == end.st_mtime) {
                        (void)fprintf(stderr, "chpass: no changes made; ");
                        break;
                }
                (void)rewind(fp);
                if (check(fp, pw)) {
                        rval = 1;
                        break;
                }
                (void)fflush(stderr);
                if (prompt())
                        break;
        }
        (void)fclose(fp);
        (void)unlink(tfile);
        return(rval);
}


main
#ifndef __STDC__
    (argc, argv)
        int argc;
        char **argv;
#else
  (
        int argc,
        char **argv
  )
#endif
{
        extern int errno, optind;
        extern char *optarg;
        register char *p;
        struct passwd lpw, *pw;
        struct rlimit rlim;
        FILE *temp_fp;
        int aflag, lflag, nflag, ch, fd;
        char *fend, *newsh, *passwd, *temp, *tend;
        char from[MAXPATHLEN], to[MAXPATHLEN];
        char *getusershell();
        sec_login_handle_t  lc;
        error_status_t      st;
        boolean32           verbose = false;

	setlocale(LC_ALL, "");

        uid = getuid();
        aflag = lflag = nflag = 0;
        newsh = NULL;
        while ((ch = getopt(argc, argv, "a:s:lnv")) != EOF)
                switch(ch) {
                case 'a':
                        if (uid)
                                baduser();
                        loadpw(optarg, pw = &lpw);
                        aflag = 1;
                        break;
                case 's':
                        newsh = optarg;
                        /* protect p_field -- it thinks NULL is /bin/sh */
                        if (!*newsh)
                                usage_exit();
                        break;
                case 'l':
                        if (nflag) {
                            fprintf(stderr, "-n -l are incompatible options\n");
                            usage_exit();
                        } else {
                            lflag = 1;
                        }
                        /* REMOVE THIS WHEN CHPASS HAS BEEN UPDATED TO
                         * PROVIDE OVERRIDE FILE MANIPULATION FUNCTIONALITY
                         */
                        printf("Override manipulation not implemented\n");
                        exit(1);
                        break;
                case 'n':
                        if (lflag) {
                            fprintf(stderr, "-l -n are incompatible options\n");
                            usage_exit();
                        } else {
                            nflag = 1;
                        }
                        break;
                case 'v':
                        verbose = true;
                        break;
                case '?':
                default:
                        usage_exit();
                }
        argc -= optind;
        argv += optind;

        if (!aflag) {
                switch(argc) {
                case 0:
                        sec_login_get_current_context(&lc, &st);
                        if (GOOD_STATUS(&st)) {
                            sec_login_get_pwent(lc, &pw, &st);
                            if (STATUS_EQUAL(&st, sec_login_s_not_certified)) {
                                CLEAR_STATUS(&st);
                            } else if (BAD_STATUS(&st)) {
                                error_msg("Cannot obtain current password entry", &st);
                                exit(1);
                            }
                            get_account_data((unsigned_char_p_t)pw->pw_name,
                                                true, pw, &st);
                        } else {
                            error_msg("Cannot obtain current context", &st);
                            exit(1);
                        }
                        break;
                case 1:
                        pw = &lpw; pw->pw_name = *argv;
                        get_account_data((unsigned_char_p_t)*argv,
                                            true, pw, &st);
                        if (BAD_STATUS(&st)) {
                            error_msg("Cannot change specified account", &st);
                            exit(1);
                        }
                        break;
                default:
                        usage_exit();
                }
        }
        (void)signal(SIGHUP, SIG_IGN);
        (void)signal(SIGINT, SIG_IGN);
        (void)signal(SIGQUIT, SIG_IGN);
        (void)signal(SIGTSTP, SIG_IGN);

        rlim.rlim_cur = rlim.rlim_max = RLIM_INFINITY;
        (void)setrlimit(RLIMIT_CPU, &rlim);
        (void)setrlimit(RLIMIT_FSIZE, &rlim);

        if (newsh) {
                if (p_shell(newsh, pw, (struct entry *)NULL))
                        goto bad;
        }
        else if (!aflag && !info(pw))
                goto bad;

        /* root should have a 0 uid and a reasonable shell */
        if (!strcmp(pw->pw_name, "root")) {
                if (pw->pw_uid) {
                        (void)fprintf(stderr, "chpass: root uid should be 0.");
                        exit(1);
                }
                setusershell();
                for (;;)
                        if (!(p = getusershell())) {
                                (void)fprintf(stderr,
                                    "chpass: warning, unknown root shell.");
                                break;
                        }
                        else if (!strcmp(pw->pw_shell, p))
                                break;
        }

        /* Save any changes */
        if (verbose)
            printf("chpass: Attempting to save entry\n");
        save_entry(pw, &st);
        if (GOOD_STATUS(&st)) {
            exit(0);
        } else {
            error_msg("Error saving entry", &st);
bad:        fprintf(stderr, "Not changed.\n");
            exit(1);
        }
}
