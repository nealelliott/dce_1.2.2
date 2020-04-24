/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: query.c,v $
 * Revision 1.1.9.2  1996/02/17  23:26:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:10  marty]
 *
 * Revision 1.1.9.1  1995/12/11  21:52:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:05  root]
 * 
 * Revision 1.1.7.2  1993/10/07  19:01:23  mccann
 * 	CR9064 64bits and make -show_key option work
 * 	[1993/10/07  19:01:07  mccann]
 * 
 * Revision 1.1.7.1  1993/09/14  20:34:57  desai
 * 	Fix for CR 8891. Fixed query core dump problem as well as -help option.
 * 	[1993/09/14  20:32:21  desai]
 * 
 * Revision 1.1.5.2  1993/08/04  19:55:10  cuti
 * 	 Add "-show_key" option to check salt and reset the property to hide the unix passwd
 * 	[1993/08/04  18:24:12  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:36:01  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:43  htf]
 * 
 * Revision 1.1  1992/01/19  04:08:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  query.c V=7 07/09/91 //littl/prgy/src/client/tests_dir
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * 
 * Query Test:  (Simple passwd etc test)
 *  Iterate through the registry database contents performing every query
 *  operation.  We check that the data returned for a particular object is 
 *  consistent when it can be returned via different query paths.
 *
 * Current Status:
 *  We only check PGO and ACCT objects - not yet checking policy and
 *  properties.
 */ 

#include <pthread.h>
#include <dce/exc_handling.h>

#include <dce/uuid.h>
#include <dce/binding.h>
#include <dce/acct.h>
#include <dce/pgo.h>
#include <dce/misc.h>

#include <stdio.h>
#include <un_strng.h>
#include <rgymacro.h>

sec_rgy_handle_t rgy_context;
#define CONTEXT rgy_context

static char *last_op = "";
static int last_line = 0;

long int error_count = 0;
short show_key = 0;

#ifdef __STDC__
#   define CALL_RGY(op) last_op = #op ; last_line = __LINE__; op
#else
#   define CALL_RGY(op) last_line = __LINE__; op
#endif

void print_status
#ifndef __STDC__
    ( info, s ) 
    char            *info;
    error_status_t  *s;
#else
  (
    char            *info,
    error_status_t  *s
  )
#endif
{
    printf("%s (status 0x%08.8x)\n", info, STATUS(s));
    printf("    Operation was: %s (line=%d)\n", last_op, last_line);
    error_count++;
}

void print_usage ( void )
{
    printf("usage: query [[-d] | [-d<rpc_debug_flags>] [ cellname ]\n");
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
    sec_rgy_pname_t site;
    int             i;
    int             j;
    int             arg_count;

    arg_count = argc;
    if(arg_count == 1){
       argv[1] = "1";
    }
    else{
    for (i = 1, j = 1; i < arg_count; i++) {
        argv[j] = argv[i];
        if (strncmp(argv[i], "-d", 2) == 0) {
            if (*(argv[i]+2) == '\0') {
                rpc__dbg_set_switches("0-4.3", &st);
            } else {
                rpc__dbg_set_switches(argv[i]+2, &st);
            }
            argc--;
        } else if (strncmp(argv[i], "-help", 2) == 0) {
            print_usage();
            exit(0);
        } else if (strncmp(argv[i], "-show_key", 2) == 0) {
            show_key = 1;
            argc--;
        } else {
            j++;
        }
    }
    }

    strcpy( (char *) site , argv[1]);
    len = strlen((char *) site);

    if (argc > 1) {
        CALL_RGY(sec_rgy_site_open)(site, &rgy_context, &st); 
        if (BAD_STATUS(&st)  && !STATUS_EQUAL(&st, sec_rgy_read_only)) {
            print_status("Unable to bind to requested site:", &st);
            exit(1);
        } else {
	    site[len] = '\0';
	    printf("Contacting registry at site %s\n", site);
	}
    }
}

boolean32 compare_pgo
#ifndef __STDC__
    ( pname1, p1, pname2, p2, print_diagnostic ) 
    sec_rgy_name_t      pname1;
    sec_rgy_pgo_item_t  *p1;
    sec_rgy_name_t      pname2;
    sec_rgy_pgo_item_t  *p2;
    boolean32           print_diagnostic;
#else
  (
    sec_rgy_name_t      pname1,
    sec_rgy_pgo_item_t  *p1,
    sec_rgy_name_t      pname2,
    sec_rgy_pgo_item_t  *p2,
    boolean32           print_diagnostic
  )
#endif
{
    boolean32       retcode = true;
    error_status_t  st;

    if (strcmp((char *) pname1, (char *) pname2)) {
        if (print_diagnostic) {
            printf("%s : %s - names differ\n", pname1, pname2);
        }
        retcode = false;
    }

    CLEAR_STATUS(&st);
    if (!uuid_equal(&p1->id, &p2->id, &st)) {
        if (print_diagnostic) {
            printf("%s - uuids differ\n", pname1);
        }
        retcode = false;
    }
    if (BAD_STATUS(&st)) {
	printf("uuid_equal failed with status %x", STATUS(&st));
    }

    if (p1->unix_num != p2->unix_num) {
        if (print_diagnostic) {
            printf("%s unix ids differ\n", pname1);
        }
        retcode = false;
    }

    if (strcmp( (char *) p1->fullname, (char *) p2->fullname) != 0) {
        if (print_diagnostic) {
            printf("%s fullnames differ\n", pname1);
        }
        retcode = false;
    }

    if (p1->flags != p2->flags) {
        if (print_diagnostic) {
            printf("%s flags differ\n", pname1);
        }
        retcode = false;
    }

    return retcode;
}


void check_pgo
#ifndef __STDC__
    ( domain, pgo_name, pgo ) 
    sec_rgy_domain_t    domain;
    sec_rgy_name_t      pgo_name;
    sec_rgy_pgo_item_t  *pgo;
#else
  (
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      pgo_name,
    sec_rgy_pgo_item_t  *pgo
  )
#endif
{
    sec_rgy_pgo_item_t  item;
    sec_rgy_name_t      scope;
    sec_rgy_name_t      item_name;
    sec_rgy_cursor_t    cursor;
    error_status_t      st;

    CALL_RGY(sec_rgy_cursor_reset)(&cursor);
    *scope = '\0';

    CALL_RGY(sec_rgy_pgo_get_by_name)(CONTEXT, domain, pgo_name, &cursor, 
                                        &item, &st);
    if (BAD_STATUS(&st)) {
        print_status("check_pgo 1", &st);
    }

    if (!compare_pgo(pgo_name, pgo, pgo_name, &item, true)) {
        printf("PGO item: %s query by name failed\n", pgo_name);
    }

    CALL_RGY(sec_rgy_cursor_reset)(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_id)(CONTEXT, domain, scope, &pgo->id, true, 
                                    &cursor, &item, item_name, &st);
    if (BAD_STATUS(&st)) {
        print_status("check_pgo 2", &st);
    }

    if (!compare_pgo(pgo_name, pgo, item_name, &item, true) && 
               !(pgo->flags & sec_rgy_pgo_is_an_alias)) {
        printf("PGO item: %.*s query by UID failed\n", pgo_name);
    }

    CALL_RGY(sec_rgy_cursor_reset)(&cursor);
    CALL_RGY(sec_rgy_pgo_get_by_unix_num)(CONTEXT, domain, scope, pgo->unix_num, 
                                            true, &cursor, &item, item_name, &st);
    if (BAD_STATUS(&st)) {
        print_status("check_pgo 3", &st);
    }

    if (!compare_pgo(pgo_name, pgo, item_name, &item, true) && 
            !(pgo->flags & sec_rgy_pgo_is_an_alias)) {
        printf("PGO item: %s query by UNIX NUM failed\n", pgo_name);
    }
}


char *char_tbl = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

/* This needs to be writable */
char tbl_mark[65];

void check_salt
#ifdef __STDC__
    (passwd)
     sec_rgy_unix_passwd_buf_t passwd;
#else
    (
     sec_rgy_unix_passwd_buf_t passwd
    )
#endif
{
    short i, j;
    short find = 0;

    if ((passwd[0] != '\0') && strcmp(passwd, "CIPHER")) {
    for (i = 0; i < 2; i++) {
        find = 0;
	for (j = 0; j < 64; j++) { 
	    if (passwd[i] == *(char_tbl + j)) {
		*(tbl_mark + j) = '1';
		find = 1;
		break;
	    }
	}
        if (find == 0) {
                *(tbl_mark + 64) = '1';
  		printf("salt: %x (%o) %x (%o)\n", passwd[0], passwd[0], passwd[1], passwd[1]);
        }
    }
    }

}

void check_salt_final
#ifdef __STDC__
    ()
#else
    (
    void
    )
#endif
{
    short i;
    short error = 0;

   if (*(tbl_mark + 64) == '1') {
        error = 1;
        printf("Error: Extra characters appear in salt.\n"); 
    }

   for (i = 0; i < 64; i++)
    if (*(tbl_mark + i) == '0') {
        error = 1;
        printf("Warning:  Character %c does not show up in salt.\n", char_tbl[i]);
    }

    if (error == 0)
        printf(" No error in salt checking.\n");

}
    

void check_accounts
#ifndef __STDC__
    ( person_name ) 
    sec_rgy_name_t  person_name;
#else
  (
    sec_rgy_name_t  person_name
  )
#endif
{
#define NUM 64
    error_status_t          status;
    error_status_t          lst;
    sec_rgy_login_name_t    name,
                            name_result;
    sec_rgy_plcy_t          policy;
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;
    sec_rgy_cursor_t        cursor;
    sec_rgy_cursor_t        projlist_cursor;
/*UNIX
    sec_rgy_cursor_t        unix_cursor;
    sec_rgy_unix_passwd_t   unix_rec;
*/
    uuid_t                  proj_uid[NUM];
    int                     proj_gid[NUM];
    signed32                num_supplied,
                            num_projects;
    sec_rgy_name_t          cell_name;
    uuid_t                  cell_uuid;


    CALL_RGY(sec_rgy_cursor_reset)(&cursor);

/*UNIX
    CALL_RGY(sec_rgy_cursor_reset)(&unix_cursor);
*/

    strcpy((char *) name.pname, (char *) person_name);
    MAKE_WILD(name.gname);
    MAKE_WILD(name.oname);

    do {
        /*
         * Allow the test sequence to be interrupted.
         */
        pthread_testcancel();

        CALL_RGY(sec_rgy_acct_lookup)(CONTEXT, &name, &cursor, &name_result, 
                                        &sid, &unix_sid, &key_parts,
                                        &user_part, &admin_part, &lst);

        if (BAD_STATUS(&lst) && !STATUS_EQUAL(&lst, sec_rgy_no_more_entries)
                && !STATUS_EQUAL(&lst, sec_rgy_object_not_found)) {
            print_status("check_accounts 1", &lst);
            return;
        }
        if (!BAD_STATUS(&lst) && show_key){
        check_salt(user_part.passwd);
}

        CALL_RGY(sec_rgy_login_get_info)(CONTEXT, &name_result, &key_parts, 
                                        &sid, &unix_sid, &user_part, 
                                        &admin_part, &policy, 
                                        0, &num_supplied, proj_uid,
                                        proj_gid, &num_projects, cell_name,
                                        &cell_uuid, &status);

        if (BAD_STATUS(&status) && !STATUS_EQUAL(&status, sec_rgy_object_not_found)) {
            print_status("check_accounts 2", &status);
            return;
        }
                      
	CALL_RGY(sec_rgy_cursor_reset)(&projlist_cursor);
        CALL_RGY(sec_rgy_acct_get_projlist)(CONTEXT, &name, &projlist_cursor, 
                                            NUM, &num_supplied, proj_uid, 
			                    proj_gid, &num_projects, &status);
        if (BAD_STATUS(&status) && !STATUS_EQUAL(&status, sec_rgy_object_not_found)) {
            print_status("check_accounts 3", &status);
            return;
        }

/*UNIX
        rgy_unix_getpwnam(name, name_len, &unix_cursor, &unix_rec, &status);
        if (BAD_STATUS(&status) && !STATUS_EQUAL(&status, sec_rgy_object_not_found)) {
            fprintf(stderr, "    %.*s ", name_len, name);
            print_status("check_accounts 4", &status);
            return;
        }
*/

    } while (STATUS_OK(&lst));

}

void do_pgo (
#ifdef __STDC__
    void
#endif
)
{
    sec_rgy_cursor_t    cursor;
    sec_rgy_name_t      person_name;
    sec_rgy_name_t      scope;
    sec_rgy_pgo_item_t  person;
    error_status_t      st;
    int                 num = 0;
    int                 blurb_count = 0;
    sec_rgy_domain_t    domain;
    sec_rgy_properties_t    property;

    *scope = '\0';

    if (show_key) { /* set flag to show unix passwd */
        sec_rgy_properties_get_info(CONTEXT, &property, &st);
        if (BAD_STATUS(&st)) {
            print_status("property_get_info :", &st);
        }
        property.flags &= ~sec_rgy_prop_shadow_passwd;
        sec_rgy_properties_set_info(CONTEXT, &property, &st);
        if (BAD_STATUS(&st)) {
            print_status("property_set_info :", &st);
        }
    }

    for (domain = sec_rgy_domain_person; domain < sec_rgy_domain_last; domain++) {
        if (domain == sec_rgy_domain_person) {
            printf("Processing People...\n");
        } else if (domain == sec_rgy_domain_group) {
            printf("Processing Groups...\n");
        } else if (domain == sec_rgy_domain_org) {
            printf("Processing Orgs...\n");
        } else {
            printf("Unknown domain...\n");
        }

        CALL_RGY(sec_rgy_cursor_reset)(&cursor);

        num = 0;
        blurb_count = 0;

        do {
            /*
             * Allow the test sequence to be interrupted.
             */
            pthread_testcancel();

            CALL_RGY(sec_rgy_pgo_get_next)(CONTEXT, domain, scope, &cursor, 
			                &person, person_name, &st);
            if (BAD_STATUS(&st) && !STATUS_EQUAL(&st, sec_rgy_no_more_entries)) {
                print_status("do_pgo", &st);
                break;
            }

            if (STATUS_OK(&st)) {
                check_pgo(domain, person_name, &person);
                if (domain == sec_rgy_domain_person) {
                    check_accounts(person_name);
                }
                if ((++num % 10) == 0) {
                    printf("%4d ", num);
                    fflush(stdout);
                    if (++blurb_count == 10) {
                        printf("\n");
                        blurb_count = 0;
                    }
                }
            }
        } while (STATUS_OK(&st));

        if (blurb_count > 0) {
            printf("\n");
        }

    }
    if (show_key) {
        check_salt_final();
        /* reset to not show unix passwd */
        property.flags |= sec_rgy_prop_shadow_passwd;
        sec_rgy_properties_set_info(CONTEXT, &property, &st);
        if (BAD_STATUS(&st)) {
            print_status("property_set_info :", &st);
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
    int i;

    TRY {
        /*
         * Allow the test sequence to be interrupted.
         */
        pthread_testcancel();

	/* Initialize tbl_mark */
	for (i=0; i<sizeof(tbl_mark); ++i)
	    tbl_mark[i] = '0';

        process_args(argc, argv, envp);
        do_pgo();

    } CATCH_ALL {
        printf("Exception Raised\n");
    } ENDTRY;

    if (error_count) {
        printf("%d error%s during query test\n", error_count, error_count > 1 ? "s" : "");
	exit(1);
    } else {
        printf("No errors during query test\n");
	exit(0);
    }
}


