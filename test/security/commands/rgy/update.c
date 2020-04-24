/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: update.c,v $
 * Revision 1.1.12.2  1996/02/17  23:26:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:13  marty]
 *
 * Revision 1.1.12.1  1995/12/11  21:52:27  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  22:07 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/02  15:29 UTC  gordon_l
 * 	merge changes
 * 
 * 	HP revision /main/HPDCE01/gordon_branch/1  1994/04/29  15:42 UTC  gordon_l
 * 	Fixes for change_master / become_master system test.
 * 	[1995/12/11  20:54:08  root]
 * 
 * Revision 1.1.3.3  1993/08/03  15:39:08  sekhar
 * 	Purge the login context to avoid filling up the creds
 * 	directory with credential files each time update is
 * 	run (this can be run repeatedly by the replication system test).
 * 	[1993/08/03  15:38:51  sekhar]
 * 
 * Revision 1.1.3.2  1993/08/02  18:23:22  sekhar
 * 	Changes for replication system test (which uses this):
 * 	1. return proper error codes on program exit
 * 	2. when an account is created set the sec_rgy_acct_admin_client
 * 	   in the admin flag to allow the newly created principal to
 * 	   log into the regstry (this is used by the rgylogin test).
 * 	[1993/08/02  18:18:06  sekhar]
 * 
 * Revision 1.1.1.10  1993/05/04  22:17:52  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.5.5  1993/04/20  20:30:19  cuti
 * 	Change site_open() to site_open_update().
 * 	[1993/04/20  19:35:29  cuti]
 * 
 * Revision 1.1.5.4  1992/12/31  17:36:10  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:55  htf]
 * 
 * Revision 1.1.5.3  1992/12/04  20:14:37  marty
 * 	OSC1.1.1 upgrade.
 * 	[1992/12/04  19:27:05  marty]
 * 
 * Revision 1.1.8.2  1992/11/24  20:23:28  sekhar
 * 	changed basename to prefix_nm because this clashes with the
 * 	libc function basename() which has a prototype definiton
 * 	in the header file /usr/include/string.h on OSF/1.1.1 .
 * 	Also changed basename_len to prefix_len to be consistent.
 * 	[1992/11/24  20:22:26  sekhar]
 * 
 * Revision 1.1.5.2  1992/11/04  20:25:39  sekhar
 * 	[OT 4848]   Added cmd line options -a , -r, -p and -pw for easier testing.
 * 	[OT 4889]
 * 
 * Revision 1.1  1992/01/19  04:08:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  update.c V=16 10/21/91 //littl/prgy/src/client/tests_dir
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * Update Test:  (Simple passwd etc test)
 *     Update test can be used to either add or delete principals, groups
 *     and organizations to the registry database.
 *  
 * Current Status:
 *  We only check PGO and ACCT objects - not yet checking policy and
 *  properties.
 *
 * Usage : update -a | -r [ -p <principal> -pw <password>] 
 *                num_accts site [ person_prefix [ group_prefix [ org_prefix ] ] ]
 * 
 * N.B.
 *         -a .... Adds entries to the registry
 *         -r .... Removes entries from the registry
 */ 


/* History of changes

      1. Don't retry on errors sec_rgy_rep_not_master and sec_rgy_rep_bad_state
      2. print the actual site bound to not argv[2] - misleading.
      3. if argv[2] is null, then bind to the update site else use sec_rgy_site_open
         to bind to the specified site.
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



struct TIMINGinfo *acct_timer;

static char *last_op = "";
static int last_line = 0;

typedef enum update_op_t {
    update_none,      /* neither -a nor -r set on the cmd line */
    update_add,       /* -a was specified on the cmd line      */
    update_remove     /* -r was specified on the cmd line      */
} update_op_t;

typedef struct cmdline_opts_t {
    char        *login_princname;
    char        *login_passwd;  
    update_op_t optype;
} cmdline_opts_t ;

cmdline_opts_t  cmdline_opts = { NULL, NULL, update_none} ;

cmdline_opts_t  *options = &cmdline_opts;

int             unix_nums[sec_rgy_domain_last]      = { 1, 1, 1 };
sec_rgy_name_t  prefix_nm[sec_rgy_domain_last]   = { "up.da.te._.te.st/per", "upd_test/grp", "upd_test/org" };
int             prefix_len[sec_rgy_domain_last]   = {20, 12, 12};
char            *domain_text[sec_rgy_domain_last]   = { "person", "group", "org" };

sec_rgy_name_t  rgy_site_name ; 
         
int num_accts  = 500;  /* default number of accts to create - currently always overwritten by argv[1] */
int num_groups = 20;    /* number of new groups to spread num_accts over */
int num_orgs   = 5;    /* number of new orgs to spread num_accts over */

sec_rgy_handle_t  rgy_context;
#define CONTEXT (rgy_context)

long int error_count = 0;
long int comm_failures = 0;

#ifdef __STDC__
#   define CALL_RGY(op) last_op = #op ; last_line = __LINE__; op
#else
#   define CALL_RGY(op) last_line = __LINE__; op
#endif

sec_passwd_rec_t        admin_key;
sec_login_handle_t  lcontext;

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
    error_status_t      st;
    int                 retval;
    sec_passwd_rec_t    prec;
    

    if (user == NULL) {
        printf("Enter Admin Principal Name: ");
        fflush(stdout);
        user = fgets(buf, sizeof(buf), stdin);
        if (user == NULL) {
            exit(1);
        }
        len = strlen(user);
        if (user[len-1] == '\n') {
            user[len-1] = '\0';
        }
        user = (char *) malloc(len + 1);
        if (user == NULL) {
            exit(2);
        }
        strcpy(user, buf);
    }

    if (sec_login_setup_identity((unsigned_char_p_t) user,
                                    sec_login_no_flags, &lcontext, &st)) {

        if (passwd == NULL) {
            len = sizeof(buf) - 1;
            retval = krb5_read_password("Enter Password: ", NULL, buf,
                                            &len);
            if (retval) {
                exit(3);
            }
            passwd = (char *) malloc(1 + strlen(buf));
            strcpy(passwd, buf);
        }
        else {
            strcpy(buf, passwd);
	}

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
                exit(5);
            }
            sec_login_set_context(lcontext, &st);
            if (st) {
                printf("Unable to set context: %#X\n", st);
                exit(6);
            }
        } else {
            printf("Sorry.\nPassword Validation Failure.\n");
            exit(7);
        }
    } else {
        printf("Sorry.\nUser Identification Failure.\n");
        exit(8);
    }
}

do_logout()
{
    error_status_t st;

    sec_login_purge_context(&lcontext, &st);
    if (st != error_status_ok) {
        printf("Unable to purge context %#X\n", st);
    }
    return (st);
}



void print_status
#ifndef __STDC__
    ( info, s ) 
	char             *info;
        error_status_t   *s;
#else
  (
	char             *info,
        error_status_t   *s
  )
#endif
{
    printf("%s (status 0x%08.8x)\n", info, STATUS(s));
    printf("    Operation was: %s (line=%d)\n", last_op, last_line);
    if (*s == sec_rgy_server_unavailable) {
        comm_failures++;
    } else {
        error_count++;
    }
}


void print_usage (
#ifdef __STDC__
    void
#endif
)
{
    printf("usage: update -a | -r [-d<rpc_debug_flags>] [-p <principal> -pw <passwd>]\n");
    printf("              num_accts site [ person_prefix [ group_prefix [ org_prefix ] ] ]\n");
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
        if (strncmp(argv[i], "-d", 2) == 0) {
            if (*(argv[i]+2) == '\0') {
                rpc__dbg_set_switches("0-4.3", &st);
            } else {
                rpc__dbg_set_switches(argv[i]+2, &st);
            }
            argc--;
        } else if (strcmp(argv[i], "-help") == 0) {
            print_usage();
            argc--;

	} else if (strcmp(argv[i], "-p") == 0) {  /* next arg is a principal name  unless */
            if (i == arg_count - 1 ) {            /* -p is the last arg   on the cmd line */
                print_usage();
                exit(1);
	    }
            options->login_princname = (char *) malloc(1 + strlen(argv[i+1]));
            strcpy(options->login_princname, argv[i+1]);
            i++;  /* skip the next argument */
            argc = argc - 2;

        } else if (strcmp(argv[i], "-pw") == 0) { /* next arg is the passwd unless   */
            if (i == arg_count - 1 ) {            /* -pw is the last arg on cmd line */
                print_usage();
                exit(1);
	    }
            options->login_passwd = (char *) malloc(1 + strlen(argv[i+1]));
            strcpy(options->login_passwd, argv[i+1]);
            i++;   /* skip the next argument */
            argc = argc - 2;
	}
        else if (strncmp(argv[i], "-a", 2) == 0) {
            options->optype = update_add;
            argc--;
	}
        else if (strncmp(argv[i], "-r", 2) == 0) {
            options->optype = update_remove;
            argc--;
	}
	else {
            j++;
        }
        i++;
    }

    /*
     *  Print useage and exit if any of the following are true :
     *
     *  1.  login principle name specified but no password was specified or vice versa .
     *  2.  neither -a nor -r was specified on the command line
     *
     */

    if (   (    ((options->login_princname != NULL) && (options->login_passwd == NULL))
             || ((options->login_princname == NULL) && (options->login_passwd != NULL)) )
        || (options->optype == update_none))
    {
        print_usage();
        exit(1);
    }


    strcpy( (char *) rgy_site_name, argv[2]);
       
    if (argc < 3 || argc > 6) {
        print_usage();
        exit(1);
    }

    num_accts =  atoi(argv[1]);

    if (argc > 3) {
	strcpy((char *) prefix_nm[sec_rgy_domain_person], argv[3]);
        prefix_len[sec_rgy_domain_person] = 
                strlen((char *) prefix_nm[sec_rgy_domain_person]);
    }

    if (argc > 4) {
	strcpy((char *) prefix_nm[sec_rgy_domain_group], argv[4]);
        prefix_len[sec_rgy_domain_group] = 
                strlen((char *) prefix_nm[sec_rgy_domain_group]);
    }

    if (argc > 5) {
	strcpy((char *) prefix_nm[sec_rgy_domain_org], argv[5]);
        prefix_len[sec_rgy_domain_org] = 
                strlen((char *) prefix_nm[sec_rgy_domain_org]);
    }
}


int assign_unix_num
#ifndef __STDC__
    ( domain ) 
    sec_rgy_domain_t domain;
#else
  (
    sec_rgy_domain_t domain
  )
#endif
{
    int  cur_unix_num;

    cur_unix_num = unix_nums[domain];
    unix_nums[domain] = unix_nums[domain] + 1;
    return  cur_unix_num;
}



gen_passwd
#ifndef __STDC__
    ( name, passwd ) 
    unsigned_char_p_t name;
    sec_passwd_rec_t *passwd;
#else
  (
    unsigned_char_p_t name,
    sec_passwd_rec_t  *passwd
  )
#endif
{
    PASSWD_TYPE(passwd) = sec_passwd_plain;
    /* server will assign one */
    passwd->pepper = NULL;
    /* server will use next available     */
    passwd->version_number = sec_passwd_c_version_none;
    strncpy((char *) PLAIN_PASSWD(passwd), (char *) name,
            sizeof(sec_passwd_str_t));
    /*
     * Guarantee null termination
     */
    PLAIN_PASSWD(passwd)[sizeof(sec_passwd_str_t) - 1] = '\0';
}


do_account
#ifndef __STDC__
    ( name, g_name, o_name ) 
    unsigned_char_p_t name;
    unsigned_char_p_t g_name;
    unsigned_char_p_t o_name;
#else
  (
    unsigned_char_p_t name,
    unsigned_char_p_t g_name,
    unsigned_char_p_t o_name
  )
#endif
{
    char                    msg[512];
    sec_rgy_login_name_t    login_name;
    sec_rgy_acct_key_t      acct_key;
    sec_rgy_acct_user_t     user;
    sec_rgy_acct_admin_t    admin;
    sec_passwd_rec_t        passwd;   
    sec_passwd_type_t       new_key_type = sec_passwd_des;  /* tell server to generate des keys */
    sec_passwd_version_t    passwd_version_number; 
    unsigned_char_t         plain_buff[sec_passwd_str_t_size];
    error_status_t          st;

    CALL_RGY(sec_rgy_pgo_add_member)(CONTEXT, sec_rgy_domain_group, g_name,
				 name, &st);
    if (BAD_STATUS(&st)) {
        sprintf(msg, "Unable to add member '%s' to group '%s'", name, g_name);
        print_status(msg, &st);
        return;
    }
    CALL_RGY(sec_rgy_pgo_add_member)(CONTEXT, sec_rgy_domain_org, o_name,
				 name, &st);
    if (BAD_STATUS(&st)) {
        sprintf(msg, "Unable to add member '%s' to org '%s'", name, o_name);
        print_status(msg, &st);
        return;
    }

    strcpy( (char *) login_name.pname, (char *) name);
    strcpy( (char *) login_name.gname, (char *) g_name);
    strcpy( (char *) login_name.oname, (char *) o_name);

    acct_key = sec_rgy_acct_key_person;

    strcpy((char *) user.gecos, "office,work_phone,home_phone");
    sprintf((char *) user.homedir, "/home/%s", name);
    strcpy((char *) user.shell, "/bin/csh");
    *user.passwd = '\0';
    user.passwd_version_number = 0;
    user.passwd_dtm = 0;
    
    PLAIN_PASSWD(&passwd) = &(plain_buff[0]);
    gen_passwd(name, &passwd);
    user.flags = sec_rgy_acct_user_passwd_valid;

    bzero(&admin, sizeof(admin));
    admin.flags = sec_rgy_acct_admin_valid | sec_rgy_acct_admin_client ;

    CALL_RGY(sec_rgy_acct_add)(CONTEXT, &login_name, &acct_key, &user,
			   &admin, &admin_key, &passwd, new_key_type, &passwd_version_number, &st);
    if (BAD_STATUS(&st)) {
        sprintf(msg, "Unable to add acct '%s'", name);
        print_status(msg, &st);
        return;
    }
}

  
add_pgo
#ifndef __STDC__
    ( domain, pgo_name, pgo, stp ) 
    sec_rgy_domain_t domain;
    sec_rgy_name_t pgo_name;
    sec_rgy_pgo_item_t *pgo;
    error_status_t *stp;
#else
  (
    sec_rgy_domain_t domain,
    sec_rgy_name_t pgo_name,
    sec_rgy_pgo_item_t *pgo,
    error_status_t *stp
  )
#endif
{
    char  msg[512];

    uuid_create_nil(&pgo->id, stp);
retry:
    CALL_RGY(sec_rgy_pgo_add)(CONTEXT, domain, pgo_name, pgo, stp);
    if (BAD_STATUS(stp)) {
	sprintf(msg, "Unable to add %s '%s'", domain_text[domain], pgo_name);
        print_status(msg, stp);
        switch (STATUS(stp)) {
            case sec_rgy_object_exists:
            case sec_rgy_name_exists:
            case sec_rgy_is_an_alias:
            case sec_rgy_unix_id_changed:
            case sec_rgy_not_authorized:
            case sec_rgy_bad_alias_owner:
            case sec_rgy_read_only:
            case sec_rgy_bad_name:
            case sec_rgy_quota_exhausted:
            case sec_rgy_no_more_unix_ids:
            case sec_rgy_foreign_quota_exhausted:
            case sec_rgy_not_implemented:
            case sec_rgy_rep_not_master:
            case sec_rgy_rep_bad_state:
                break;

            default:
                sleep(2);
                goto retry;
        }
    }
}

delete_pgo
#ifndef __STDC__
    ( domain, pgo_name, stp ) 
    sec_rgy_domain_t domain;
    sec_rgy_name_t pgo_name;
    error_status_t *stp;
#else
  (
    sec_rgy_domain_t domain,
    sec_rgy_name_t pgo_name,
    error_status_t *stp
  )
#endif
{
    char  msg[512];

    CALL_RGY(sec_rgy_pgo_delete)(CONTEXT, domain, pgo_name, stp);
    if (BAD_STATUS(stp)) {
	sprintf(msg, "Unable to delete %s '%s'", domain_text[domain], pgo_name);
        print_status(msg, stp);
    }
}


void init_pgo
#ifndef __STDC__
    ( domain, pgo_name, pgo ) 
    sec_rgy_domain_t domain;
    sec_rgy_name_t pgo_name;
    sec_rgy_pgo_item_t * pgo;
#else
  (
    sec_rgy_domain_t domain,
    sec_rgy_name_t pgo_name,
    sec_rgy_pgo_item_t * pgo
  )
#endif
{

    bzero((Pointer) pgo, sizeof(sec_rgy_pgo_item_t));
    strcpy((char *) pgo_name, (char *) prefix_nm[domain]);
    if (domain == sec_rgy_domain_group) {
	pgo->flags =  sec_rgy_pgo_projlist_ok;
    }

}
 

void modulate_pgo
#ifndef __STDC__
    ( domain, pgo_name, pgo ) 
    sec_rgy_domain_t domain;
    sec_rgy_name_t pgo_name;
    sec_rgy_pgo_item_t * pgo;
#else
  (
    sec_rgy_domain_t domain,
    sec_rgy_name_t pgo_name,
    sec_rgy_pgo_item_t * pgo
  )
#endif
{
    error_status_t  st;
    char           *dp;
    int             num;

    num = assign_unix_num(domain);
    pgo->unix_num = sec_rgy_uxid_unknown;
    sprintf((char *) &(pgo_name[prefix_len[domain]]), "/X%05.5d", num);
    sprintf((char *) pgo->fullname, "%s %05.05d", domain_text[domain], num);
    if (options->optype == update_add)
        add_pgo(domain, pgo_name, pgo, &st);
    else if (options->optype == update_remove)
        delete_pgo(domain, pgo_name, &st);
}
        

do_pgo (
#ifdef __STDC__
    void
#endif
)
{
    sec_rgy_pgo_item_t ppgo, gpgo, opgo;
    sec_rgy_name_t     pname, gname, oname;
    sec_rgy_domain_t   domain;
    error_status_t  st;
    int             i,
                    accts_per_group,
                    accts_per_org;
   
    if (num_groups < num_accts) {
	accts_per_group = num_accts/num_groups;
    } else {
	accts_per_group = num_accts;
    }

    if (num_orgs < num_accts) {
	accts_per_org = num_accts/num_orgs;
    } else {
	accts_per_org = num_accts;
    }

    init_pgo(sec_rgy_domain_person, pname, &ppgo);

    init_pgo(sec_rgy_domain_group, gname, &gpgo);

    init_pgo(sec_rgy_domain_org, oname, &opgo);

    for (i = 0; i < num_accts; i++) {
        /*
         * Allow the test sequence to be interrupted.
         */
        pthread_testcancel();

        TIMINGstart(acct_timer);
	modulate_pgo(sec_rgy_domain_person, pname, &ppgo);

	if ( (i % accts_per_group) == 0 ) {
	    modulate_pgo(sec_rgy_domain_group, gname, &gpgo);
	}

	if ( (i % accts_per_org) == 0 ) {
	    modulate_pgo(sec_rgy_domain_org, oname, &opgo);
	}

        /*
         *  do_account should only be called when adding accounts i.e.
         *  -a is specified on the cmd line. Since accounts are removed
         *  when a principal name is removed, there is no need to explicitly
         *  delete the accounts when -r is specified.
         */

           
        if (options->optype == update_add)
            do_account(pname, gname, oname);

        TIMINGstop(acct_timer);
        if (i%20 == 19) {
            TIMINGprint(acct_timer);
        }
    }
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

    TRY {
        /*
         * Allow the test sequence to be interrupted.
         */
        pthread_testcancel();

        process_args(argc, argv, envp);
        do_login(options->login_princname, options->login_passwd);

        CALL_RGY(sec_rgy_site_open_update)(rgy_site_name, &rgy_context, &st);
/*         if (BAD_STATUS(&st) && !STATUS_EQUAL(&st, sec_rgy_read_only)) { */
        if (BAD_STATUS(&st)) {
            print_status("Unable to bind to requested site:", &st);
            exit(1);
        } else {
            printf("Opening registry at site %s\n", rgy_site_name);
        }

        acct_timer = TIMINGregister("Account add");
        do_pgo();
        st=do_logout();
        if (st != error_status_ok)
            error_count=1 ;

    } CATCH_ALL {
        printf("Exception Raised\n");
    } ENDTRY;

    if (error_count) {
        printf("%d error%s during update test\n", error_count, error_count > 1 ? "s" : "");
    } else {
        printf("No errors during update test\n");
    }
    if (comm_failures) {
        printf("%d communication failure%s during update test\n", comm_failures, comm_failures > 1 ? "s" : "");
    }
    if (error_count) 
        exit(1);
    else
        exit(0);
}






