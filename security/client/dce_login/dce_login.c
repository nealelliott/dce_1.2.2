/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_login.c,v $
 * Revision 1.1.18.3  1996/02/18  00:06:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:56  marty]
 *
 * Revision 1.1.18.2  1995/12/08  18:03:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/7  1995/07/10  18:23 UTC  aha
 * 	CHFts15668: do not purge existing creds if bad password entered while refreshing creds
 * 
 * 	HP revision /main/HPDCE02/aha_mothra7/1  1995/07/07  17:01 UTC  aha
 * 	CHFts15668: on refresh (-r) don't purge existing
 * 	 context if password is bad.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/19  16:54 UTC  jrr
 * 	Merge work for CHFts15204.
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts152204/1  1995/05/19  16:52 UTC  jrr
 * 	Stop dce_login -r from orphaning creds files (CHFts15204).
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/03  20:03 UTC  jrr
 * 	Merge -refresh work (CHFts14997).
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts14997/1  1995/04/26  13:45 UTC  jrr
 * 	Add -refresh to dce_login (CHFts14997).
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/21  18:53 UTC  mdf
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_6/3  1995/02/21  17:52 UTC  mdf
 * 	Cleanup.
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_6/2  1995/02/21  17:49 UTC  mdf
 * 	Change for CHFts14343, which reduces the amount of time the command line
 * 	arguments and be viewed via the ps() command.  The real fix is to not
 * 	pass passwords on the command line.
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_6/1  1995/02/21  14:13 UTC  mdf
 * 	Working on Defect Number CHFts14343
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/30  22:43 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/27  23:11 UTC  burati
 * 	Merge from mb_mothra
 * 
 * 	HP revision /main/HPDCE02/mb_mothra/2  1995/01/26  00:47 UTC  burati
 * 	Put back missing usage() stmt.
 * 
 * 	HP revision /main/HPDCE02/mb_mothra/1  1995/01/26  00:40 UTC  burati
 * 	Add -k option (for logging in from keytab)
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:31:01  root]
 * 
 * Revision 1.1.13.5  1994/07/15  14:58:38  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:37  mdf]
 * 	Revision 1.1.11.1  1993/10/05  22:30:11  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:05:47  mccann]
 * 
 * 	More mods to allow use of sec_login_valid_and_cert_ident
 * 	[1992/05/14  01:46:57  burati]
 * 
 * 	Add '-c' flag to (validate & certify) instead of just (validate)
 * 	[1992/05/11  19:10:23  burati]
 * 
 * 	        Minor fixup of previous change, to make sure types matched.
 * 	        [1992/04/25  23:43:00  burati]
 * 
 * 	        CR2817 Make sure it prints bad status on failures.
 * 	        CR2427 Zero out passwd after copying it if passed in argv
 * 	        [1992/04/25  23:33:29  burati]
 * 	[1992/04/27  14:38:21  burati]
 * 
 * Revision 1.1.1.11  1993/05/04  20:24:14  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.13.4  1994/06/02  21:57:24  mdf
 * 	Merged with changes from 1.1.13.3
 * 	[1994/06/02  21:54:42  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  19:33:50  mdf]
 * 
 * Revision 1.1.13.3  1994/06/02  20:20:47  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  19:33:50  mdf]
 * 
 * Revision 1.1.13.1  1994/03/25  17:46:09  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  17:41:55  sekhar]
 * 
 * Revision 1.1.5.7  1993/02/01  20:35:53  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:14:11  hinman]
 * 
 * Revision 1.1.5.6  1993/01/08  22:17:19  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:01  tom]
 * 	Revision 1.1.7.2  1993/01/11  16:11:38  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.8  93/01/06  09:53:34  meg
 * 	[meg] incorporating osf rev 1.1.5.5
 * 
 * Revision 9.5.3.2  93/01/06  09:52:18  meg
 * 	[meg] incorporating osf rev 1.1.5.5
 * 
 * Revision 9.6.1.1  92/12/30  09:56:45  root
 * 	Branch holding OSF rev
 * 	Revision 9.5.1.7  92/12/03  14:48:44  chen
 * 	moved an #undef sigaction for SVR4 platforms only pending re-entrant library
 * 	changes.  without this, the compilation fails. -- meg/rcc
 * 
 * Revision 9.6  92/12/30  09:46:13  root
 * 	Update checkin
 * 	Revision 9.5.1.6  92/11/05  17:32:24  hinman
 * 	Auto checkin of OSF rev 1.1.5.3
 * 
 * Revision 1.1.5.5  1992/12/29  12:42:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:37:07  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/28  23:34:31  sekhar
 * 	[OT 5911]  added -exec (or -e for short) option to dce_login for use by
 * 	           shell scripts.
 * 
 * 	[OT 6242]  Use execvp() instead of execv()
 * 	[1992/12/28  18:28:45  sekhar]
 * 
 * Revision 1.1.5.3  1992/10/27  20:17:01  sekhar
 * 	[OT 5590]  The warning message (when built with DES_NEUTER on) should
 * 	           only be issued when dce_login is used (not dce_login_neoxec).
 * 	[1992/10/27  15:42:46  sekhar]
 * 
 * Revision 1.1.5.2  1992/09/29  21:14:50  devsrc
 * 	Remove redefine of sigaction.
 * 	[1992/09/15  19:57:04  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:18:41  sekhar]
 * 
 * Revision 1.1.2.4  1992/05/14  18:54:26  burati
 * 	Revision 1.1  1992/01/19  14:47:59  devrcs
 * 	        Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
**      DCE Network Login - does not manipulate local OS context.
**
** This is a sample application for DCE Security.  It is not a "deliverable"
** portion of the DCE Security component.
*/

#define _INCLUDE_POSIX_SOURCE_
#ifdef SNI_SVR4
#include <pthread.h>
#include <sys/signal.h>
#else
#include <sys/signal.h>
#include <pthread.h>
#endif /* SNI_SVR4 */
#include <stdlib.h>
#include <locale.h>

#include <un_maloc.h>
#include <stdio.h>
#include <dce/dce.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <string.h>

#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

#if defined(CMA_INCLUDE) && (_CMA_KTHREADS_ == _CMA__NONE) && !defined(execvp) || defined(SNI_SVR4)
/*
 * CMA doesn't wrap exec - and the standard implementation of exec will
 * inherit the virtual timer interrupt settings from the parent.  We clone
 * the cma wrapper for fork here to turn off the nasty inherited behavior.
 */
int cma_execvp (
#if defined(__STDC__) || defined(_K_R_PORT_CHECK_)
    char    *argp,
    char    *argv[]
#endif
  );

#   define execvp(P,A) cma_execvp(P,A)
#   define EXPAND_CMA_EXECVP
#endif

static void usage(char *argv0)
{
    fprintf(stderr,
        "Usage: %s [username] [password] [-r] [-k <file> | -c] [-e <cmd_string>]\n",
        argv0);
}

#ifdef HPUX
#include <sys/pstat.h>

/* Clear command line */
void ccl ()
{
    char buf[BUFSIZ];
    union pstun pbuf;
    sprintf(buf, "dce_login");
    pbuf.pst_command = buf;
    (void)pstat((int)PSTAT_SETCMD, pbuf, 0,0,0);
}
#endif


int main (
    int     argc,
    char    *argv[]
)
{
    char                *shell, *user = NULL, *keyfile = NULL;
    boolean32           setup = false;
    boolean32           kflag = false, pflag = false;
    boolean32           cflag = false, rflag = false;
    sec_passwd_str_t    pbuf;
    boolean32           reset_passwd, valid;
    sec_login_auth_src_t auth_src;
    char                buf[2048];
    int                 i, len, retval, lst;
    sec_login_handle_t  lcontext;
    error_status_t      st;
    sec_passwd_rec_t    prec;
    dce_error_string_t  err_string;
    char                **cmd_to_exec = NULL;  /* (opt) cmd string to exec */
    unsigned32          kvno, asvc = rpc_c_authn_dce_secret;
#ifdef HPUX
    ccl();		/* Clear Command Line */
#endif

    setlocale(LC_ALL, "");

    dce_svc_set_progname("dce_login", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);

    /*
     *  Process arguments (the order is important). The options -c and -exec
     *  must be checked first in the loop before checking the user and password
     */

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            rflag = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            if (kflag) {
                fprintf(stderr, "Incompatible options: -k -c\n");
		usage(argv[0]);
                exit(1);
	    }
            cflag = true;
        } else if (strcmp(argv[i], "-k") == 0) {
            if (cflag) {
                fprintf(stderr, "Incompatible options: -c -k\n");
		usage(argv[0]);
                exit(1);
	    }
            kflag = pflag = true;

	    /* Next argument *must* be the name of the keytable */
	    if (++i >= argc) {
                fprintf(stderr, "Missing keytable filename for -k option.\n");
                usage(argv[0]);
		exit(1);
	    }
            keyfile = argv[i];

        } else if ( (!strcmp(argv[i], "-exec")) || (!strcmp(argv[i], "-e"))) {
            if (i + 1 == argc) {   /* check for null cmd string */
                fprintf(stderr, "Command must be specified after -exec\n");
                fprintf(stderr, "Usage: %s [username] [password] [-c] [-exec | -e <cmd_string> ]\n",
                       argv[0]);
                exit(1);
            }
            cmd_to_exec = &argv[i+1] ;
            break;                /* rest of the line will be execed */
        } else if (user == (char *)NULL) {  /* Username must be 1st */
            user = argv[i];
        } else if (!pflag) {                /* then passwd if not set */
            strncpy((char *)pbuf, argv[i], sec_passwd_str_max_len);
            pbuf[sec_passwd_str_max_len] = '\0';
            memset(argv[i], 0, strlen((char *)argv[i]));
            pflag = true;
        } else {
            fprintf(stderr, "Illegal argument %s\n", argv[i]);
	    usage(argv[0]);
            exit(1);
        }
    }

    if (!rflag) {
        if (user == NULL) {
            printf("Enter Principal Name: ");
            fflush(stdout);
            user = fgets(buf, sizeof(buf), stdin);
            if (user == NULL) {
                exit(1);
            }
            len = strlen(user);
            if (user[len-1] == '\n') {
                user[len-1] = '\0';
            }
            user = malloc(len + 1);
            if (user == NULL) {
                exit(2);
            }
            strcpy(user, buf);
        }
    } /* if (!rflag) */

    if (rflag) {
        sec_login_get_current_context(&lcontext, &st);
        if (st != error_status_ok) {
            dce_error_inq_text(st, err_string, &lst);
            printf("Unable to acquire current context for refresh: %s\n", 
                   err_string);
            exit(3);
        }
        
        if (sec_login_refresh_identity(lcontext, &st)) {
            if (st != error_status_ok) {
                dce_error_inq_text(st, err_string, &lst);
                printf("Unable to refresh current context: %s\n", 
                       err_string);
                exit(3);
            }
        }
        setup = true;
    }
    else {
        setup = sec_login_setup_identity((unsigned_char_p_t) user,
                                     sec_login_no_flags, &lcontext, &st);
    } /* if (rflag) */

    if (setup) {
        /* If keytable specified, then log in from there */
        if (kflag) {
            if (getuid() != geteuid()) {
                fprintf(stderr, "Effective UID not equal to real UID, cannot access keytable.\n");
                exit(3);
            }
            sec_login_valid_from_keytable(lcontext, asvc, keyfile, 0, &kvno,
                                          &reset_passwd, &auth_src, &st);
            valid = (st == error_status_ok);
        } else {
            if (!pflag) {
                len = sizeof(pbuf) - 1;
                retval = krb5_read_password("Enter Password: ", NULL,
                                            (char *)pbuf, &len);
                if (retval) {
                    exit(3);
                }
            }
            
            prec.key.key_type = sec_passwd_plain;
            prec.key.tagged_union.plain = (unsigned char *) pbuf;
            prec.pepper = NULL;
            prec.version_number = sec_passwd_c_version_none;
            
            if (cflag) {
                valid = sec_login_valid_and_cert_ident(lcontext, &prec,
                                                       &reset_passwd, &auth_src, &st);
            } else {
                valid = sec_login_validate_identity(lcontext, &prec,
                                                    &reset_passwd, &auth_src, &st);
            }
        }
        
        if (valid) {
            
            if (st) {
                dce_error_inq_text(st, err_string, &lst);
                printf("Validation error: %s\n", err_string);
            }

            if (reset_passwd) {
                fprintf(stderr, "Password must be changed!\n");
            }
            if (auth_src == sec_login_auth_src_local) {
                fprintf(stderr, "Credentials obtained from local registry.\n");
            }
            if (auth_src == sec_login_auth_src_overridden) {
                fprintf(stderr, "Validated from local override entry, no network credentials obtained.\n");
            } else {
                /* Don't do a set_context on refresh */
                if (!rflag) {
                    sec_login_set_context(lcontext, &st);
                }
                if (st) {
                    dce_error_inq_text(st, err_string, &lst);
                    printf("Unable to set context: %s\n", err_string);
                    exit(6);
                }
            }
        } else {
            printf("Sorry.\nPassword Validation Failure.");
            if (st) {
               dce_error_inq_text(st, err_string, &lst);
               printf(" - %s\n", err_string);
            } else {
               printf("\n");
            }
            if (!rflag) {
                sec_login_purge_context(&lcontext, &st);
            }
            exit(7);
        }
    } else {
        printf("Sorry.\nUser Identification Failure.");
        if (st) {
           dce_error_inq_text(st, err_string, &lst);
           printf(" - %s\n", err_string);
        } else {
           printf("\n");
        }
        exit(8);
    }

    if (!strcmp(argv[0], "dce_login_noexec")) {
        /*
         * Special support for DCE initial configuration script - don't
         * exec a sub shell and emit the name of the credential file.
         * This is NOT a general mechanism.  It is intimately tied to the
         * current implementation and is not a recommended practice.
         */
        printf("%s\n", getenv("KRB5CCNAME"));
        exit(0);
    } else if (rflag) {
        /* If we are doing a refresh, we're done. */
        exit (0);
    } else {
        /*
         * The following warning message should be printed only when dce_login
         * is invoked. Printing the message when dce_login_noexec is used
         * would cause the credential cache name i.e. KRB5CCNAME to be set
         * incorrectly.
         */
#ifdef DES_NEUTER
        printf("Warning: System configured without encryption code.\n");
        printf("         No authentication or password validation available.\n");
#endif
        if (cmd_to_exec != NULL) {
            execvp(cmd_to_exec[0], cmd_to_exec);
            printf("Unable to execute %s\n", cmd_to_exec[0]);
        }
        else {
            shell = getenv("SHELL");
	    if ((shell == NULL) || (*shell == '\0')) {
#if __hpux1000p
                shell = "/usr/bin/sh";
#else
                shell = "/bin/sh";
#endif
            }

            argv[0] = shell;
            argv[1] = NULL;

            execvp(shell, argv);

            printf("Unable to execute %s\n", shell);
        }
    }
    exit(9);
}


#if defined(EXPAND_CMA_EXECVP)
/*
 * CMA doesn't wrap exec - and the standard implementation of exec will
 * inherit the virtual timer interrupt settings from the parent.  We clone
 * the cma wrapper for fork here to turn off the nasty inherited behavior.
 */
#undef execvp
#ifdef fprintf
#   undef fprintf
#endif
#ifndef SNI_SVR4
#ifdef sigaction
#   undef sigaction
#endif
#endif

#define cma__c_timer                    ITIMER_VIRTUAL

#include <sys/time.h>
#include <signal.h>

int cma_execvp (
    char    *argp,
    char    *argv[]
)
{
    struct itimerval    unix_timer, unix_oldtime;
    struct sigaction    act;
    int                 i;

    timerclear(&unix_timer.it_interval);
    timerclear(&unix_timer.it_value);

    if (setitimer(cma__c_timer, &unix_timer, &unix_oldtime) == -1) {
        fprintf(stderr, "setitimer failed in exec with %d", errno);
    }

    /*
     * Set all signal actions to the default
     */
    if (sigemptyset (&act.sa_mask) == -1) {
        fprintf (stderr, "sigemptyset failed in exec with %d", errno);
    }

    act.sa_flags = 0;
    act.sa_handler = SIG_DFL;

    for (i = 1; i < NSIG; i++) {

        switch (i) {
            case SIGKILL :
            case SIGSTOP : {    /* Can't be caught */
                break;
            }
            default : {
#ifdef SNI_SVR4
#   undef sigaction
#endif
                (void)sigaction (i, &act, NULL);
                break;
            }
        }
    }

    return execvp(argp, argv);
}

#endif /* EXPAND_CMA_EXECV */

