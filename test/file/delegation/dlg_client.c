/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dlg_client.c,v $
 * Revision 1.1.9.1  1996/10/17  18:18:53  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:18  damon]
 *
 * Revision 1.1.4.2  1994/08/10  19:23:05  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:21  annie]
 * 
 * Revision 1.1.4.1  1994/07/13  22:33:05  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:17:44  devsrc]
 * 
 * 	Delegation Functional Tests - Initial Submit
 * 	[1994/06/08  18:30:24  delgado]
 * 
 * $EndLog$
 */

/* Adapting HP's delegation test for DFS */

/* This is the client program to the dfsdlg admin list test
 * server.
 *
 * We send a check access request for the server to check our
 * admin list acces and the program will return 0 (success) if the access
 * check passed.
 */

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dfsdlg_test.h>
#include <dfsdlg_int.h>
#include <dfsdlg_target.h>

extern boolean32 debug;

static void usage
#ifndef __STDC__
    (name)
    char *name;
#else
  (
    char *name
  )
#endif
{
    fprintf(stderr,
     "Usage: %s [-d][-p principal_name [-pw passwd]]\n",
     name);
}


int main
#ifndef __STDC__
    (argc, argv) 
    int     argc;
    char    *argv[];
#else
  (
    int     argc,
    char    *argv[]
  )
#endif
{
    handle_t                h;
    error_status_t          st, xst;
    sec_login_handle_t      lc, ilc = NULL;
    sec_cred_cursor_t       cursor;
    sec_cred_pa_handle_t    pa_h;
    sec_id_pa_t             *pa_dataP;
    sec_passwd_str_t        pbuf, pbuf2;
    sec_acl_permset_t       perms = sec_acl_perm_read;
    char           *pname = CLIENT_PRINCIPAL_NAME;
    int                     i, expect, expect_set;
    boolean32               retval, lattr, rattr, one,
                            become_initiator = true,reset_passwd, test_refresh;
    sec_passwd_rec_t        prec;
    sec_login_auth_src_t    auth_src;
    error_status_t              tmp_st;
    dce_error_string_t          dce_error;

   


    /* Assume that the passwd is the same as the principal name. */
    strcpy((char *) pbuf, pname);
    strcpy((char *) pbuf2, pname);
    expect_set=0;
    lattr = rattr = one = debug = false;
    /* Argument parsing */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -p requires principal name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            pname = argv[i];

        } else if (strcmp(argv[i], "-pw") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -pw requires password argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            strcpy((char *) pbuf, argv[i]);
            strcpy((char *) pbuf2, argv[i]);

        } else if (strncmp(argv[i], "-d", 2) == 0) {

            debug = true;


        } else if (strncmp(argv[i], "-ndlg", 4) == 0) {
            /* without delegation */
            if (one)
                usage(argv[0]);
            else {
                one = true;
                become_initiator = false;
            }

	}else if (strncmp(argv[i], "-expect",7) == 0){
           /* the invoker tells us whether we should expect
            * the test to pass (0) or fail (non-0)
            */
           expect_set=1;
           expect = atoi(argv[i+1]);
           i++;

        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            exit(1);
	  }
      }
    if (expect_set == 0){
        printf("-exepect <value> not specified\n");
        exit(1);
    }
    /* Log in and set the context current */
    log_in(&lc, (unsigned_char_p_t)pname, pbuf, &st);
    if (BAD_STATUS(&st)) {
        exit(st);
    }
    /* Enable delegation unless we're explicitly testing with it disabled */
    if (become_initiator) {
        bind_to_svr(&h, (unsigned_char_p_t)INTEMEDIARY_NSENTRY_NAME, &st);
        if (BAD_STATUS(&st)) {
            exit(st);
        }
        ilc = sec_login_become_initiator(lc, sec_id_deleg_type_traced, NULL,
                                            NULL, NULL, NULL,
                                            sec_id_compat_mode_initiator, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text(st, dce_error, (int *) &tmp_st);
    	    fprintf(stderr, "Error from sec_login_become_initiator: %s\n", dce_error);
            goto purge_login_context;
        }

    } else {
        ilc = lc;
        bind_to_svr(&h, (unsigned_char_p_t)TARGET_NSENTRY_NAME, &st);
        if (BAD_STATUS(&st)) {
            exit(st);
        }
      }


    /* Delegation Test */
        rpc_binding_set_auth_info(h, NULL , rpc_c_authn_level_default,
                                    rpc_c_authn_dce_secret,
                                    (rpc_auth_identity_handle_t) ilc,
                                    rpc_c_authz_dce, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text(st, dce_error, (int *) &tmp_st);
            fprintf(stderr, "FAILED: Delegation test; Error from rpc_binding_set_auth_info: %s\n", dce_error);
            goto purge_deleg_context;
        }
     if (become_initiator){
        retval = dfsdlgint_has_admin_access(h, &st);
        if (debug)
            fprintf(stderr, "dfsdlgint_has_admin_acces returned %s\n",
                    ((retval) ? "true" : "false"));

      }else{ /* skip the intermediary and call directly to the target */

       retval = dfsdlg_target_admin_access(h, &st);
        if (debug)
            fprintf(stderr, "dfsd_target_admin_acces returned %s\n",
                    ((retval) ? "true" : "false"));
      }
purge_deleg_context:
    if ((ilc != NULL) && (ilc != lc)) {
        sec_login_purge_context(&ilc, &xst);
        if (BAD_STATUS(&xst)) {
            dce_error_inq_text(xst, dce_error, (int *) &tmp_st);
            fprintf(stderr, "Error purging initiator context: %s\n", dce_error);
        }
      }
	
purge_login_context:
    sec_login_purge_context(&lc, &xst);
    if (BAD_STATUS(&xst)) {
            dce_error_inq_text(xst, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error purging principal login context: %s\n", dce_error);
    }
    if (st != error_status_ok)    {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr,"Remote call failed %s\n", dce_error);
        exit(st);
      }
    if (debug)
        fprintf(stderr,"client: expected %ld,  got %ld\n", expect, retval);
    if (retval == expect)
       exit(0);
    else
       exit(1);
}







