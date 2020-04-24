/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: k5dcelogin.c,v $
 * Revision 1.1.2.3  1996/10/03  14:43:45  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/07/09  19:48 UTC  mullan_s  /main/DCE_1.2.2/2]
 *
 * 	Call seteuid instead of setreuid on AIX to set effective uid.
 * 	[1996/08/15  22:26 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/6]
 *
 * 	Use setresuid/setreuid/setuid depending on platform.
 * 	[1996/08/09  15:39 UTC  jrr  /main/DCE_1.2.2/mullan_dce122_bugs_1/5]
 *
 * 	Replace calls to setresuid() with the more portable setuid() call.
 * 	[1996/08/08  19:36 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/4]
 *
 * 	Set real/effective userids where necessary; add wait() error handling;
 * 	improve utmp file parsing.
 * 	[1996/07/29  14:02 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/3]
 *
 * 	The principal arg to sec_login_setup_identity should be the principal owner
 * 	of the creds file, not the user logging in.
 * 	[1996/06/24  21:41 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
 *
 * 	No need for cleanup signal handler.
 * 	[1996/06/24  21:24 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 *
 * 	Purge DCE credentials when remote session is done.
 * 	[1996/05/30  21:17 UTC  mullan_s  /main/DCE_1.2.2/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:39 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/15  14:27 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 *
 * 	Purge DCE credentials on error. Improve error formatting.
 * 	[1996/05/14  18:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * 	Don't destroy original krb5 credentials if there are errors
 * 	trying to create the DCE credentials. Continue on error.
 * 	[1996/05/14  18:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * Revision 1.1.2.2  1996/08/09  21:20:01  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/06/24  21:41 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/2]
 * 
 * 	No need for cleanup signal handler.
 * 	[1996/06/24  21:24 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce122_bugs_1/1]
 * 
 * 	Purge DCE credentials when remote session is done.
 * 	[1996/05/30  21:17 UTC  mullan_s  /main/DCE_1.2.2/1]
 * 
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:39 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/15  14:27 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 * 
 * 	Purge DCE credentials on error. Improve error formatting.
 * 	[1996/05/14  18:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 * 
 * 	Don't destroy original krb5 credentials if there are errors
 * 	trying to create the DCE credentials. Continue on error.
 * 	[1996/05/14  18:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 * 
 * Revision 1.1.2.1  1996/06/05  14:17:56  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:39 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/15  14:27 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 * 
 * 	Purge DCE credentials on error. Improve error formatting.
 * 	[1996/05/14  18:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 * 
 * 	Don't destroy original krb5 credentials if there are errors
 * 	trying to create the DCE credentials. Continue on error.
 * 	[1996/05/14  18:54 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * k5dcelogin - frontend program for login which will
 * convert a K5 forward ticket if any to a DCE context,
 * then call the specified command.
 * This seperates out the K5 code and libs from any DCE libs
 * since they are not compatable. This program is based on  
 * k5dceauth from HP.
 *
 * NOTE: k5dcelogin is intended to be invoked by the rlogind
 * process, which runs as root. It is not intended to be useful
 * as a standalone tool. The permissions on k5dcelogin should be
 * set to 700, and the owner should be root.
 */

#ifndef lint
static char rcsid[] = "$Revision: 1.1.2.3 $";
#endif

#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <pwd.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <utmp.h>
#include <sys/wait.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <com_err.h>

#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>

/*#define DEBUG*/

#ifdef DEBUG
#define DEEDEBUG(A) fprintf(stderr,A)
#define DEEDEBUG2(A,B) fprintf(stderr,A,B)
#else
#define DEEDEBUG(A)
#define DEEDEBUG2(A,B)
#endif

char *progname;
char *username;
char *defrealm;
sec_login_handle_t lcontext = NULL;
boolean32 execing_login = false;
void replace_utmp_info(void);

#if defined(hpux) || defined(__hpux) || defined(AIX32)
static char krb5ccname[_POSIX_PATH_MAX+16] = { "KRB5CCNAME=" };
#endif

#ifndef HAVE_SETREUID
#ifdef HAVE_SETRESUID
#define setreuid(r,e) setresuid(r,e,-1)
#else
#define setreuid(r,e) setuid(r)
#endif
#endif

usage(void)
{
    fprintf(stderr, "Usage: %s username cmd [parms to be passed to cmd]\n", progname);
    exit(1);
}


main(int argc, char *argv[])
{
    char *sname, *realm, *tgtname;
    int code, i, j;
    krb5_ccache cache = NULL;
    krb5_cc_cursor cur;
    krb5_creds creds, *krbtgt = NULL;
    char *cache_name = (char *)NULL;
    krb5_principal princ, tgt_princ;
    krb5_flags flags;
    error_status_t st;
    sec_login_auth_src_t auth_src;
    boolean32 reset_passwd;
    int lst;
    dce_error_string_t  err_string;
    struct passwd *pw;
    char *ccname;
    char **newargv;
    char *cp;
    pid_t pid;
    int wstat;

#ifdef DEBUG_MAIN
    int debug = 1;
    while (debug)
	;
#endif

    if (progname = strrchr(argv[0], '/'))
	progname++;
    else
	progname = argv[0];

    setlocale(LC_ALL, "");
    krb5_init_ets();

    if (argc < 3)
	usage();

    if ((pw = getpwnam(argv[1])) == NULL) {
        DEEDEBUG("Who are you?\n");
        goto done;
    }
   
    DEEDEBUG2("User=%s\n", argv[1]);
    DEEDEBUG2("uid=%d\n",getuid());
    DEEDEBUG2("euid=%d\n",geteuid());
    if (setreuid(pw->pw_uid, -1) < 0) {
	com_err(progname, errno, "while setting real user id");
	exit(1);
    }
    DEEDEBUG2("uid=%d\n",getuid());
    DEEDEBUG2("euid=%d\n",geteuid());
#ifdef DEBUG
    /* If there is no cache to be converted, i.e. no forwarded
     * cred, then just call the login program
     */

    if ((ccname = getenv("KRB5CCNAME")) == NULL)
	goto done;
    DEEDEBUG2("KRB5CCNAME = %s\n",ccname);
#endif

    if (cache == NULL) {
        if (code = krb5_cc_default(&cache)) {
            com_err(progname, code, "while getting default ccache");
	    goto done;
        }
    }

    flags = 0;                          /* turns off OPENCLOSE mode */
    if (code = krb5_cc_set_flags(cache, flags)) {
        if (code == ENOENT) {
            com_err(progname, code, "(ticket cache %s)",
                    krb5_cc_get_name(cache));
        } else
            com_err(progname, code,
                    "while setting cache flags (ticket cache %s)",
                    krb5_cc_get_name(cache));
	goto done;
    }
    if (code = krb5_cc_get_principal(cache, &princ)) {
        com_err(progname, code, "while retrieving principal name");
        exit(1);
    }
    if (code = krb5_unparse_name(princ, &username)) {
        com_err(progname, code, "while unparsing principal name");
        exit(1);
    }
    DEEDEBUG2("username=%s\n",username);
    if (code = krb5_get_default_realm(&defrealm)) {
        com_err(progname, code, "while getting default realm");
        exit(1);
    }

    if (code = krb5_build_principal(&tgt_princ, strlen(defrealm), defrealm,
				    "krbtgt", defrealm, NULL)) {
        com_err(progname, code, "while building default tgt");
        exit(1);
    }
    if (code = krb5_unparse_name(tgt_princ, &tgtname)) {
        com_err(progname, code, "while unparsing tgt principal name");
        exit(1);
    }

    /*
     * Look for the DCE ptgt, if we find it exit - this is already
     * a valid DCE ccache.
     *
     * If we don't find it, save the krbtgt for the principal's realm
     */
    if (code = krb5_cc_start_seq_get(cache, &cur)) {
        com_err(progname, code, "while starting to retrieve tickets");
        exit(1);
    }
    while (!(code = krb5_cc_next_cred(cache, &cur, &creds))) {
	krb5_creds *cred = &creds;

	if (code = krb5_unparse_name(cred->server, &sname)) {
	    com_err(progname, code, "while unparsing server name");
	    continue;
	}

	if (strncmp(sname, "dce-ptgt", 8) == 0) {
	    goto done;
	}

	else if (strcmp(sname, tgtname) == 0) {
	    if (code = krb5_copy_creds(&creds, &krbtgt)) {
		com_err(progname, code, "while copying TGT");
		exit(1);
	    }
	}
	free(sname);
    }

    if (code == KRB5_CC_END) {
        if (code = krb5_cc_end_seq_get(cache, &cur)) {
            com_err(progname, code, "while finishing ticket retrieval");
            exit(1);
        }
        flags = KRB5_TC_OPENCLOSE;      /* turns on OPENCLOSE mode */
        if (code = krb5_cc_set_flags(cache, flags)) {
            com_err(progname, code, "while closing ccache");
            exit(1);
        }
    } else {
        com_err(progname, code, "while retrieving a ticket");
        exit(1);
    }

    if (krbtgt == NULL) {
	fprintf(stderr, "%s: Did not find TGT\n", progname);
	goto done;
    }

    DEEDEBUG2("flags=%x\n",krbtgt->ticket_flags);
    if (!(krbtgt->ticket_flags & TKT_FLG_FORWARDED)){
        fprintf(stderr, "%s: Ticket not forwarded\n", progname);
        goto done;
    }


    /*
     * Setup a DCE login context. If there are any errors, just
     * print out the error but preserve the original kerberos
     * credentials.
     *
     * Why? If the TGT is marked proxiable, tickets to DCE services
     * cannot be acquired unless they are marked as allowing proxiable
     * tickets. By default, they will not be, and the DCE login
     * context APIs will fail because they cannot get the required
     * tickets.
     */
#ifdef AIX32
    if (seteuid(pw->pw_uid) < 0) {
#else
    if (setreuid(-1, pw->pw_uid) < 0) {
#endif
        com_err(progname, errno, "while setting effective uid");
        exit(1);
    }
    DEEDEBUG2("uid=%d\n",getuid());
    DEEDEBUG2("euid=%d\n",geteuid());
    if (sec_login_setup_identity((unsigned_char_p_t)username,
				 (sec_login_external_tgt|sec_login_proxy_cred),
				 &lcontext, &st)) {
	/*
	 * Add our TGT.
	 */
	DEEDEBUG("Adding our new TGT\n");
	sec_login_krb5_add_cred(lcontext, krbtgt, &st);
	if (st) {
	    dce_error_inq_text(st, err_string, &lst);
	    fprintf(stderr,
		    "%s: Error while adding credentials for %s because %s\n",
		    progname, username, err_string);
            sec_login_purge_context(&lcontext, &st);
	    goto done;
	}
	DEEDEBUG("validating and certifying\n");
	/*
	 * Now "validate" and certify the identity,
	 *  usually we would pass a password here, but...
	 * sec_login_valid_and_cert_ident
	 * sec_login_validate_identity
	 */
	if (sec_login_validate_identity(lcontext, 0, &reset_passwd,
					&auth_src, &st)) {
	    DEEDEBUG2("validate_identity st=%d\n",st);
	    if (st) {
		dce_error_inq_text(st, err_string, &lst);
		fprintf(stderr, "%s: DCE Validation error for %s because %s\n",
			progname, username, err_string);
	        sec_login_purge_context(&lcontext, &st);
		goto done;
	    }
	    if (!sec_login_certify_identity(lcontext,&st)) {
		dce_error_inq_text(st, err_string, &lst);
		fprintf(stderr,
			"%s: DCE Credentials not certified because %s\n",
			progname, err_string);
	    }
	    if (reset_passwd) {
		fprintf(stderr, "%s: Password must be changed for %s\n", 
			progname, username);
	    }
	    if (auth_src == sec_login_auth_src_local) {
		fprintf(stderr,
			"%s: DCE Credentials obtained from local registry for %s\n",
			progname, username);
	    }
	    if (auth_src == sec_login_auth_src_overridden) {
		fprintf(stderr, "%s: Validated %s from local override entry, no DCE network credentials obtained\n", 
			progname, username);
	        sec_login_purge_context(&lcontext, &st);
		goto done;
	    }
	    /*
	     * Actually create the cred files.
	     */
	    DEEDEBUG("Creating new cred files.\n");
	    sec_login_set_context(lcontext, &st);
	    if (st) {
		dce_error_inq_text(st, err_string, &lst);
		fprintf(stderr, "%s: Unable to set DCE context for %s because %s\n", 
			progname, username, err_string);
	        sec_login_purge_context(&lcontext, &st);
		goto done;
	    }
	}
	else {
	    DEEDEBUG2("validate failed %d\n",st);
	    dce_error_inq_text(st, err_string, &lst);
	    fprintf(stderr, "%s: Unable to validate DCE identity for %s because %s\n", 
		    progname, username, err_string);
	    sec_login_purge_context(&lcontext, &st);
	    goto done;
	}
    }
    else {
	dce_error_inq_text(st, err_string, &lst);
	fprintf(stderr, "%s: Unable to setup DCE login entry for %s because %s\n", 
		progname, username, err_string);
	goto done;
    }

    /*
     * Destroy the Kerberos5 cred cache file
     */
    DEEDEBUG("Destroying the old cache\n");
    if (code = krb5_cc_destroy(cache)) {
        com_err(progname, code, "while destroying Kerberos5 ccache");
        exit(1);
    }

    DEEDEBUG2("KRB5CCNAME=%s\n", getenv("KRB5CCNAME"));

done:

    DEEDEBUG2("calling the %s program\n", argv[2]);

    /*
     * Recreate new argument list without the first 2
     * arguments.
     */
    newargv = (char **) malloc(sizeof(char *) * argc);
    if (!newargv) {
	fprintf(stderr, "%s: Out of memory\n", progname);
        sec_login_purge_context(&lcontext, &st);
	exit(1);
    }
    memset(newargv, NULL, argc * sizeof(char *));
    /* 
     * Formulate argument list 
     */
    newargv[0] = argv[2];
    for (i = 3, j = 1; i < argc; i++, j++)
	newargv[j] = argv[i];
    newargv[j] = NULL;

    cp = strrchr(newargv[0], '/');
    if (cp)
	cp++;
    else
	cp = newargv[0];
    if (strcmp(cp, "login") == 0) {
	execing_login = true;
#if defined(hpux) || defined(__hpux) || defined(AIX32)
        /*
         * Add KRB5CCNAME to end of command line otherwise
         * HP-UX login will erase it from the environment,
         * EVEN if the -p option is specified!
         * NOTE: We should have at least 1 entry left in newargv 
         * because we removed 2 args (argv[0,1]).
         */
	{
  	    char *cp;
	    cp = getenv("KRB5CCNAME");
	    if (cp) {
                (void) strncat(krb5ccname, cp, sizeof(krb5ccname) - strlen(krb5ccname));
                newargv[j] = krb5ccname;
                newargv[++j] = NULL;
	    } else {
		fprintf(stderr, "%s: KRB5CCNAME not found\n", progname);
		sec_login_purge_context(&lcontext, &st);
                exit(1);
	    }
	}
#endif
    }

    fflush(stdout); fflush(stderr);

    /*
     * Fork new process and execute specified command
     */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "%s: Fork failed: %s\n", progname,
            strerror(errno));
    }
    if (pid == 0) {	/* child */
	if (execing_login) {
	    /*
	     * Need to replace utmp information with child pid
	     * so that login process won't exit with utmp errors.
	     * Effective uid must be root for utmp handling and
	     * exec of "login" to succeed.
	     */
#ifdef AIX32
	    if (seteuid(0) < 0) {
#else
	    if (setreuid(-1, 0) < 0) {
#endif
	        com_err(progname, errno, "while setting effective uid");
                sec_login_purge_context(&lcontext, &st);
	        exit(1);
	    }
    	    DEEDEBUG2("uid=%d\n",getuid());
            DEEDEBUG2("euid=%d\n",geteuid());
	    replace_utmp_info();
	}
        execv(newargv[0], newargv);
        /* only reachable if execv fails */
        fprintf(stderr, "%s: Exec of %s failed: %s\n", progname,
            newargv[0], strerror(errno));
    }
    /* parent */
    (void) waitpid(pid, &wstat, 0);
    if (WIFEXITED(wstat))
        DEEDEBUG2("Normal termination, exit status = %d\n", WEXITSTATUS(wstat));
    else if (WIFSIGNALED(wstat))
	fprintf(stderr, "%s: Abnormal termination, signal number = %d\n", 
		progname, WTERMSIG(wstat));
    else if (WIFSTOPPED(wstat))
	fprintf(stderr, "%s: Child stopped, signal number %d\n", progname,
		WSTOPSIG(wstat));

    fflush(stdout); fflush(stderr);
    sec_login_purge_context(&lcontext, &st);
    exit(0);
}

/*
 * Replace utmp information for the current remote
 * login session. Only replace the process id with the
 * current process id, so that login programs will
 * not complain about not finding a utmp entry from the parent
 * process.
 */
void replace_utmp_info(void)
{
    struct utmp *out_ut = NULL, in_ut;
    char *ttyn;
    pid_t cpid, ppid;

    setutent();
    in_ut.ut_type = LOGIN_PROCESS;
    ttyn = ttyname(0);
    if (ttyn) {
	DEEDEBUG2("Ttyname = %s\n", ttyn);
        strncpy(in_ut.ut_line, ttyn+sizeof("/dev/")-1, sizeof(in_ut.ut_line));
    }
#ifdef AIX32
    (void) getutline(&in_ut, &out_ut);
#else
    out_ut = getutline(&in_ut);
#endif
    cpid = getpid();
    ppid = getppid();
    if (out_ut && (out_ut->ut_pid == ppid)) {
	DEEDEBUG2("UT pid = %d\n", out_ut->ut_pid);
	DEEDEBUG2("UT user = %s\n", out_ut->ut_user);
	out_ut->ut_pid = cpid;
	pututline(out_ut);
    } else {
	setutent();
	while ((out_ut = getutent()) != NULL) {
	    if ((out_ut->ut_type == LOGIN_PROCESS) && 
		(out_ut->ut_pid == ppid)) {
		out_ut->ut_pid = cpid;
		pututline(out_ut);
		break;
	    }
	}
    }
    endutent();
}
