/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dts_client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:24  root]
 * 
 * Revision 1.1.2.2  1994/10/18  18:51:30  agd
 * 	expand copyright
 * 	[1994/10/18  17:39:43  agd]
 * 
 * Revision 1.1.2.1  1994/09/29  21:02:49  bhaim
 * 	submit dlgcfg002 test
 * 	[1994/09/29  21:01:26  bhaim]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <locale.h>
#include <dts_test.h>

#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include "dts_macros.h"

/* common for dce_error_inq_text */
int                     err_st;
dce_error_string_t      error_buf;



/* Hardwired test attribute type id */
static uuid_t test_attr_id = { /* 6c9d0ec8-dd2d-11cc-abdd-080009353559 */
        0x6c9d0ec8,
        0xdd2d,
        0x11cc,
        0xab,
        0xdd,
        {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
      };

#define NUM_ATTRS 1


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
     "Usage: %s [-cm n|i|c ][-d][-p principal_name [-pw passwd]]\n",
     name);
}


static void bind_to_svr(handle_t *handlep, error_status_t *stp)
{
    rpc_ns_inq_handle_t         inquiry_context;
    rpc_ns_import_handle_t      import_context;
    uuid_t                      obj_uuid;
    /* Get the uuid for the object
     * We assume that these calls will yield a status code
     * telling us if we could read the entry...
     */
    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce,
                                    (unsigned_char_p_t) "/.:/dts_test",
                                    &inquiry_context, stp);
    if (BAD_STATUS(stp)) {
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_begin: %x\n", *stp);
      exit(*stp);
    }
    rpc_ns_entry_object_inq_next(inquiry_context, &obj_uuid, stp);
    if (STATUS_EQUAL(stp, rpc_s_no_more_members)) {
        uuid_create_nil(&obj_uuid, stp);
    } else if (BAD_STATUS(stp)) {
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_next: %x\n", *stp);
      exit(*stp);
    }
    rpc_ns_entry_object_inq_done(&inquiry_context, stp);
    if (BAD_STATUS(stp)) {
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_done: %x\n", *stp);
      exit(*stp);
    }

    /* Get a binding to that entry */
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
                                (unsigned_char_p_t) "/.:/dts_test",
                                (rpc_if_handle_t) NULL, &obj_uuid,
                                &import_context, stp);
    if (BAD_STATUS(stp)) {
      fprintf(stderr, "Error on rpc_ns_binding_import_begin: %x\n", *stp);
      exit(*stp);
    }
    rpc_ns_binding_import_next(import_context, handlep, stp);
    if (BAD_STATUS(stp)) {
      fprintf(stderr, "Error on rpc_ns_binding_import_next: %x\n", *stp);
      exit(*stp);
    }
    rpc_ns_binding_import_done(&import_context, stp);
    if (BAD_STATUS(stp)) {
      fprintf(stderr, "Error on rpc_ns_binding_import_done: %x\n", *stp);
      exit(*stp);
    }
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
    char                    *pname = "dts_test";
    int                     i;
    boolean32               retval, debug, test_client_cred, lattr, rattr, one,
                            become_initiator = true,reset_passwd, test_refresh;
    unsigned32      	    num_attrs = NUM_ATTRS;
    sec_attr_t              attrs[NUM_ATTRS], attr;
    sec_cred_attr_cursor_t  acursor;
    sec_passwd_rec_t        prec;
    sec_login_auth_src_t    auth_src;
    sec_id_compatibility_mode_t  compat_mode = sec_id_compat_mode_initiator;


    bind_to_svr(&h, &st);
    if (BAD_STATUS(&st)) {
        exit(st);
    }

#if 0
	/* Set up attributes to look for (just one for now) */
    attrs[0].attr_id = test_attr_id;
    attrs[0].attr_value.attr_encoding = sec_attr_enc_void;
#endif

    /* Assume that the passwd is the same as the principal name. */
    strcpy((char *) pbuf, pname);
    strcpy((char *) pbuf2, pname);

    lattr = rattr = test_refresh = one = test_client_cred = debug = false;
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

        } else if (strncmp(argv[i], "-cm", 3) == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -cm requires compat mode argument [nic]\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            switch (*(argv[i])) {
            case 'n':
                compat_mode = sec_id_compat_mode_none;
                break;

            case 'i':
                compat_mode = sec_id_compat_mode_initiator;
                break;

            case 'c':
                compat_mode =  sec_id_compat_mode_caller;
                break;
            default:
                fprintf(stderr,"invalid compat mode argument\n");
                usage(argv[0]);
                exit(1);
            }



        } else if (strncmp(argv[i], "-d", 2) == 0) {

            debug = true;

        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    /* Log in and set the context current */
    log_in(&lc, pname, pbuf, &st);
    if (BAD_STATUS(&st)) {
        exit(st);
    }

    /* Enable delegation unless we're explicitly testing with it disabled */
    if (become_initiator) {
        ilc = sec_login_become_initiator(lc, sec_id_deleg_type_traced, NULL,
                                            NULL, NULL, NULL,
                                            compat_mode, &st);
        if (BAD_STATUS(&st)) {
    	    fprintf(stderr, "Error from sec_login_become_initiator: %x\n", st);
            goto purge_login_context;
        }
    } else {
        ilc = lc;
    }

    /* Delegation Test */
    {
        rpc_binding_set_auth_info(h, NULL, rpc_c_authn_level_default,
                                    rpc_c_authn_dce_secret,
                                    (rpc_auth_identity_handle_t) ilc,
                                    rpc_c_authz_dce, &st);
        if (BAD_STATUS(&st)) {
            fprintf(stderr, "FAILED: Delegation test; Error from rpc_binding_set_auth_info: %x\n", st);
            goto purge_deleg_context;
        }
        retval = dts_test_access(h, (unsigned_char_p_t)"", &perms, &st);
        if (debug)
            fprintf(stderr, "dts_test_access returned %s\n",
                    ((retval) ? "true" : "false"));
        if (!become_initiator) {
            if (STATUS_EQUAL(&st, sec_login_s_deleg_not_enabled)) {
	        CLEAR_STATUS(&st);
                fprintf(stderr,
                    "PASSED: Delegation disabled test\n");
            } else {
	        dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
                  "FAILED: Delegation disabled test; dts_test_access error: %s\n",
                  error_buf);
            }
        } else if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "FAILED: Delegation test; Error from dts_test_access: %s\n", error_buf);
        } else {
            fprintf(stderr, "PASSED: Delegation test\n");
        }
    }

purge_deleg_context:
    if ((ilc != NULL) && (ilc != lc)) {
        sec_login_purge_context(&ilc, &xst);
        if (BAD_STATUS(&xst)) {
            fprintf(stderr, "Error purging initiator context: 0x%x\n", xst);
        }
    }
	
purge_login_context:
    sec_login_purge_context(&lc, &xst);
    if (BAD_STATUS(&xst)) {
        fprintf(stderr, "Error purging principal login context: 0x%x\n", xst);
    }
    
    exit(st);
}
