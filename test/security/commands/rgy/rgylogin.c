/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rgylogin.c,v $
 * Revision 1.1.2.2  1996/03/09  20:50:31  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:37  marty]
 *
 * Revision 1.1.2.1  1995/12/11  21:52:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  22:07 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/11  20:54:06  root]
 * 
 * Revision 1.1.2.2  1993/08/02  18:23:14  sekhar
 * 	Initial Version . Created for use by security
 * 	replication system test.
 * 	[1993/08/02  18:20:17  sekhar]
 * 
 * $EndLog$
 */
/* 
 * Login Test:
 *     This will attempt to login as each principal created by
 *     the Update test. This is primarily used for testing 
 *     replication.
 *  
 * Usage : rgylogin [-p person_prefix] num_accts
 * 
 */ 


#include <pthread.h>
#include <dce/exc_handling.h>

#include <dce/uuid.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/acct.h>
#include <dce/misc.h>

#include <dce/lbase.h>

#include <Pointer.h>
#include <bstring.h>
#include <un_strng.h>
#include <timing.h>
#include <rgymacro.h>

#include <stdio.h>
#include <math.h>



static char *last_op = "";
static int last_line = 0;

sec_rgy_name_t  prefix_nm = "up.da.te._.te.st/per";
unsigned long   prefix_len = 20;

char            *login_princ = NULL;

unsigned long   num_accts  = 500;  /* default number of accts to create */
                                   /* currently always overwritten by argv[1] */

unsigned long error_count = 0;
unsigned long comm_failures = 0;

#ifdef __STDC__
#   define CALL_RGY(op) last_op = #op ; last_line = __LINE__; op
#else
#   define CALL_RGY(op) last_line = __LINE__; op
#endif

sec_passwd_rec_t        admin_key;

do_login
#ifndef __STDC__
    (user, passwd)
       char *user;
       char *passwd;
#else
  (
   char *user,
   char *passwd
  )
#endif
{
    boolean32           reset_passwd;
    sec_login_auth_src_t auth_src;
    char                buf[2048];
    int                 len;
    sec_login_handle_t  lcontext;
    error_status_t      st;
    int                 retval;
    sec_passwd_rec_t    prec;
    

    if (sec_login_setup_identity((unsigned_char_p_t) user,
                                    sec_login_no_flags, &lcontext, &st)) {
        strcpy(buf, passwd);

        prec.key.key_type = sec_passwd_plain;  
        prec.key.tagged_union.plain = (unsigned char *) buf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        admin_key = prec;
        /*
         * Use copy of key when setting passwords later in do_acct.
         * The validate_identity sequence will destroy the key it
         * processes.
         */
        admin_key.key.tagged_union.plain = (unsigned char *) passwd;

        if (sec_login_validate_identity(lcontext, &prec, &reset_passwd,
                                        &auth_src, &st)) {
            if (reset_passwd) {
                printf("Password must be changed!\n");
            }

            if (auth_src != sec_login_auth_src_network) {
                printf("Unable to obtain network credentials!\n");
                return(5);
            }
            sec_login_set_context(lcontext, &st);
            if (st) {
                printf("Unable to set context: %#X\n", st);
                return(6);
            }
            sec_login_purge_context (&lcontext, &st);
            if (st) {
                printf("sec_login_purge_context %#X\n", st);
                return (st);
            }
            else
                return (error_status_ok) ;
        } else {
            printf("Sorry.\nPassword Validation Failure.\n");
            return(7);
        }
    } else {
        printf("Sorry.\nUser Identification Failure.\n");
        return(8);
    }
}

void print_usage (
#ifdef __STDC__
    void
#endif
)
{
    printf("usage: rgylogin [-p person_prefix] num_accts \n");

}

void process_args
#ifndef __STDC__
    ( argc, argv, envp ) 
        int     argc;
        char    *argv[];
        char    *envp[];
#else
  (
        int     argc,
        char    *argv[],
        char    *envp[]
  )
#endif
{
    error_status_t  st;
    long            len;
    int             i;
    int             j;
    int             arg_count;

    arg_count = argc;
    i = 1; 
    j = 1;
    while (i < arg_count) {
        argv[j] = argv[i];
        if (strcmp(argv[i], "-help") == 0) {
            print_usage();
            argc--;

	} 
        else if (strcmp(argv[i], "-p") == 0) {  

            /* next arg is a principal name  unless */
            if (i == arg_count - 1 ) { /* -p is the last arg */
                print_usage();         /* on the cmd line    */
                exit(1);               /* so exit            */
	    }
            
            prefix_len = strlen((char *) argv[i+1]);
            login_princ = malloc(1 + prefix_len);
            strcpy(login_princ, argv[i+1]);
            i++;  /* skip the next argument */
            argc = argc - 2;
        } 
	else {
            j++;
        }
        i++;
    }

    if (login_princ == NULL) {
        login_princ = (char *) prefix_nm ;
    }

    if (argc != 2) {
        print_usage();
        exit(1);
    }
    num_accts = atoi(argv[1]);
}

int main
#ifndef __STDC__
    ( argc, argv, envp ) 
        int     argc;
        char    *argv[];
        char    *envp[];
#else
  (
        int     argc,
        char    *argv[],
        char    *envp[]
  )
#endif
{
    error_status_t  st;
    unsigned long   i ;
    sec_rgy_name_t  princ_nm;
    int             len;

    TRY {
        /*
         * Allow the test sequence to be interrupted.
         */
        pthread_testcancel();

        process_args(argc, argv, envp);
        strcpy((char *)princ_nm, login_princ);
        printf(".... Login into %d accounts ; basename = %s\n",num_accts, princ_nm);
        for (i = 1 ; i <= num_accts ; i++){
            sprintf((char *) &(princ_nm[prefix_len]), "/X%05.5d", i);
            printf(".... Attempting to login as %s \n",princ_nm);
            if (do_login((char *) princ_nm, (char *) princ_nm)) 
                error_count++ ;
	}

    } CATCH_ALL {
        printf("Exception Raised\n");
    } ENDTRY;

    if (error_count) {
        printf("%d error%s during rgylogin test\n", error_count, error_count > 1 ? "s" : "");
    } else {
        printf("No errors during rgylogin test\n");
    }
    if (comm_failures) {
        printf("%d communication failure%s during rgylogin test\n", comm_failures, comm_failures > 1 ? "s" : "");
    }

    if (error_count) 
        exit(1);
    else
        exit(0);
}





