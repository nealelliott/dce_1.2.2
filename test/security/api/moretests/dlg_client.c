/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dlg_client.c,v $
 * Revision 1.1.8.2  1996/02/17  23:25:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:28  marty]
 *
 * Revision 1.1.8.1  1995/12/11  21:48:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:11  root]
 * 
 * Revision 1.1.3.9  1994/08/17  19:28:25  bhaim
 * 	fix for 11717
 * 	[1994/08/17  19:27:54  bhaim]
 * 
 * Revision 1.1.3.8  1994/08/12  17:37:54  mdf
 * 	Hewlett Packard Security Drop
 * 	[1994/08/12  14:16:02  mdf]
 * 
 * 	Add session_id and passing server delegation type from client
 * 
 * 	Add server delegation type to dlg_test_access()'s parameter list, so test can be driven by the client.
 * 
 * 	Add test to rock.
 * 
 * 	merge impersonation support from .../greg_dlg_imp
 * 
 * 	add impersonation test.
 * 
 * 	Add name_based check on sec_cred call.
 * 
 * 	Merge from mb_fix
 * 
 * 	Make attribute test tougher
 * 
 * 	Merge from dlg_bl2 again
 * 
 * 	More bl2 work
 * 
 * 	merge from dlg_bl2
 * 
 * 	More tests.
 * 
 * 	Merge from dlg_bl2
 * 
 * 	Remove obsolete params from sec_login_set_extended_attrs()
 * 
 * 	Add attribute
 * 
 * 	Merge from dlg_bl2
 * 
 * 	Add more tests
 * 
 * 	Merge from ODESSA_1
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993
** Unpublished work. All Rights Reserved.
*/

#include <stdio.h>
#include <locale.h>
#include <dlg_test.h>

#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/rgybase.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>

/* Hardwired test attribute type id */
static uuid_t test_attr_id = { /* 6c9d0ec8-dd2d-11cc-abdd-080009353559 */
        0x6c9d0ec8,
        0xdd2d,
        0x11cc,
        0xab,
        0xdd,
        {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
      };
#define TEST_ATTR_INT 92065

/* common for dce_error_inq_text */
int                     err_st;
dce_error_string_t      error_buf;



static void usage
  (
    char *name
  )
{
    fprintf(stderr,
     "Usage: %s [-la | -ra | -c | -nbi| -dis | -name | -rock | -sc | -id | -dbg ][-sdt n|t|i][-cdt n|t|i][-tr a|n][-cm n|i|c ][-p principal_name [-pw passwd]]\n",
     name);
}


static void bind_to_svr(handle_t *handlep, error_status_t *stp, boolean32 debug)
{
    rpc_ns_inq_handle_t         inquiry_context;
    rpc_ns_import_handle_t      import_context;
    uuid_t                      obj_uuid;
    unsigned char		*string_binding;

    /* Get the uuid for the object
     * We assume that these calls will yield a status code
     * telling us if we could read the entry...
     */    /*
     * rpc_ns_mgmt_set_exp_age with "0" forces ALL subsequent
     * RPC NSI next operations to cause a request to the name 
     * service database (rather than use anything locally cached).
     * This is normally NOT a good thing for applications to
     * do but in this case, it allows us to rerun the dlg
     * test frequently while moving the location of the 
     * intermediate and target servers ie. it prevents cached 
     * bindings from previous test runs from being used.
     * 
     */
    rpc_ns_mgmt_set_exp_age(0, stp);
    if (BAD_STATUS(stp)) {
	dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	fprintf(stderr, "Error on rpc_ns_mgmt_set_exp_age: %s\n", error_buf);
	exit(*stp);
    }



    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce,
        (unsigned_char_p_t) "/.:/dlg_test", &inquiry_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_begin: %s\n", error_buf);
      exit(*stp);
    }

    rpc_ns_entry_object_inq_next(inquiry_context, &obj_uuid, stp);
    if (STATUS_EQUAL(stp, rpc_s_no_more_members)) {
        uuid_create_nil(&obj_uuid, stp);
    } else if (BAD_STATUS(stp)) {
      dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_next: %s\n", error_buf
);
      exit(*stp);
    }
    rpc_ns_entry_object_inq_done(&inquiry_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_done: %s\n", error_buf);
      exit(*stp);
    }

    /* Get a binding to that entry */
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
                                (unsigned_char_p_t) "/.:/dlg_test",
                                (rpc_if_handle_t) NULL, &obj_uuid,
                                &import_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
      fprintf(stderr, "Error on rpc_ns_binding_import_begin: %s\n", error_buf);
      exit(*stp);
    }
    rpc_ns_binding_import_next(import_context, handlep, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
      fprintf(stderr, "Error on rpc_ns_binding_import_next: %s\n", error_buf);
      exit(*stp);
    }
    rpc_ns_binding_import_done(&import_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
      fprintf(stderr, "Error on rpc_ns_binding_import_done: %s\n", error_buf);
      exit(*stp);
    }

   if (debug) {
   rpc_binding_to_string_binding(
	*handlep,
	&string_binding,
	stp);
    if (BAD_STATUS(stp)) {
	dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
	fprintf(stderr, "Error on convert binding to string: %s\n",
            error_buf);
	exit(*stp);
    }
    printf("Binding handle is %s\n", string_binding);
    }
}


#define NUM_ATTRS 1
void init_attrs(char *name, error_status_t *stp)
{
    sec_attr_t attrs[NUM_ATTRS], out_attrs[NUM_ATTRS];
    unsigned32 num_ret, num_left;
    signed32   failure_index;
    sec_rgy_handle_t        context;
    unsigned_char_p_t       site_name = NULL;

        /* Set up attributes to look for (just one for now) */
    attrs[0].attr_id = test_attr_id;
    attrs[0].attr_value.attr_encoding = sec_attr_enc_integer;
    attrs[0].attr_value.tagged_union.signed_int = TEST_ATTR_INT;

    sec_rgy_site_open_update (site_name, &context, stp);
    if (BAD_STATUS(stp)) {
	dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
        fprintf(stderr, "Error from sec_rgy_site_open_update: %s\n", error_buf);
        exit (stp);
        }

    sec_rgy_attr_update(context, sec_rgy_domain_person, 
        (unsigned_char_p_t) name, NUM_ATTRS, NUM_ATTRS, attrs, &num_ret,
        out_attrs, &num_left, &failure_index, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text((unsigned long)*stp, error_buf, &err_st);
        fprintf(stderr, "Error from sec_rgy_attr_update: %s\n", error_buf);
        fprintf(stderr, "Num_returned: %d, Num_left: %d, Failure_index: %d\n",
            num_ret, num_left, failure_index);
    }
}


int main
  (
    int     argc,
    char    *argv[]
  )
{
    handle_t                h;
    error_status_t          st, xst;
    sec_login_handle_t      lc, ilc = NULL, tlc = NULL;
    sec_cred_cursor_t       cursor;
    sec_cred_pa_handle_t    pa_h;
    sec_id_pa_t             *pa_dataP;
    sec_passwd_str_t        pbuf, pbuf2;
    sec_acl_permset_t       perms = sec_acl_perm_read;
    char                    *pname = "dlg_test";
    unsigned_char_p_t       princ_name = NULL;
    int                     i;
    boolean32               retval, negative_test, debug, test_client_cred, lattr, rattr, one,
                            become_initiator = true,reset_passwd, test_refresh,
                            disable_dlg = false, test_server_cred, name_based, rock_test;
    boolean32               is_anonymous, session_id_on;
    unsigned32              target_choice = 0; /* 1 for n (no any_other), 2 for a (all, with any_other) */
    uuid_t                  session_id;
    sec_timeval_sec_t       expiration;
    unsigned32      	    num_attrs = NUM_ATTRS;
    sec_attr_t              attrs[NUM_ATTRS], attr;
    sec_cred_attr_cursor_t  acursor;
    sec_passwd_rec_t        prec;
    sec_login_auth_src_t    auth_src;
    sec_id_delegation_type_t  c_deleg_type = sec_id_deleg_type_traced;  /* by default */
    sec_id_delegation_type_t  s_deleg_type = sec_id_deleg_type_traced;  /* by default */
    sec_id_compatibility_mode_t  compat_mode = sec_id_compat_mode_initiator;
    sec_id_restriction_set_t target_rstr;

    bind_to_svr(&h, &st, debug);
    if (BAD_STATUS(&st)) {
        exit(st);
    }

    /* get server name */
    rpc_mgmt_inq_server_princ_name(h,
        rpc_c_authn_dce_secret,
        &princ_name, &st);
    if (BAD_STATUS(&st)) {
	dce_error_inq_text((unsigned long)st, error_buf, &err_st);
	printf("Error: rpc_mgmt_inq_server_princ_name failed: %s\n", error_buf);
        exit(st);
    }

	/* Set up attributes to look for (just one for now) */
    attrs[0].attr_id = test_attr_id;
    attrs[0].attr_value.attr_encoding = sec_attr_enc_void;

    /* Assume that the passwd is the same as the principal name. */
    strcpy((char *) pbuf, pname);
    strcpy((char *) pbuf2, pname);

    lattr = rattr = test_refresh = one = test_client_cred = debug = negative_test = false;
    test_server_cred = name_based = rock_test = session_id_on = false;

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

        } else if (strncmp(argv[i], "-cdt", 4) == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -cdt requires delegation type argument [nti]\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
	    switch (*(argv[i])) {
	    case 'n':
		c_deleg_type = sec_id_deleg_type_none;
		break;

	    case 't':
		c_deleg_type = sec_id_deleg_type_traced;
		break;
	    
	    case 'i':
		c_deleg_type = sec_id_deleg_type_impersonation;
		break;

	    case 'z':
		c_deleg_type = sec_id_deleg_type_impersonation;
		negative_test = true;
		break;

	    default:
		fprintf(stderr,"invalid delegation type argument\n");
                usage(argv[0]);
                exit(1);	
	    }
	
        } else if (strncmp(argv[i], "-sdt", 4) == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -sdt requires delegation type argument [nti]\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
	    switch (*(argv[i])) {
	    case 'n':
		s_deleg_type = sec_id_deleg_type_none;
		break;

	    case 't':
		s_deleg_type = sec_id_deleg_type_traced;
		break;
	    
	    case 'i':
		s_deleg_type = sec_id_deleg_type_impersonation;
		break;
	    default:
		fprintf(stderr,"invalid delegation type argument\n");
                usage(argv[0]);
                exit(1);	
	    }

        } else if (strncmp(argv[i], "-tr", 3) == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -tr requires target restriction choice[an]\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
	    switch (*(argv[i])) {
	    case 'n':
		target_choice = 1;
		break;

	    case 'a':
		target_choice = 2;
		break;
	    
	    default:
		fprintf(stderr,"invalid target restriction choice\n");
                usage(argv[0]);
                exit(1);	
	    }
	

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
	
	} else if (strncmp(argv[i], "-dbg", 4) == 0) {

            debug = true;

	} else if (strncmp(argv[i], "-neg", 4) == 0) {

	    /* negative dlg_client test                    */
	    negative_test = true;
        } else if (strncmp(argv[i], "-c", 2) == 0) {

            if (one)
                usage(argv[0]);
            else {
                test_client_cred = one = true;
            }

        } else if (strncmp(argv[i], "-nbi", 4) == 0) {

            if (one)
                usage(argv[0]);
            else {
                one = true;
                become_initiator = false;
            }

        } else if (strncmp(argv[i], "-dis", 4) == 0) {

            if (one)
                usage(argv[0]);
            else {
                one = true;
                disable_dlg = true;
            }

        } else if (strncmp(argv[i], "-la", 3) == 0) {

            if (one)
                usage(argv[0]);
            else {
                lattr = one = true;
                become_initiator = false;
            }

        } else if (strncmp(argv[i], "-ra", 3) == 0) {

            if (one)
                usage(argv[0]);
            else {
                rattr = one = true;
                become_initiator = false;
            }

        } else if (strncmp(argv[i], "-re", 3) == 0) {

            test_refresh = true;

        } else if (strncmp(argv[i], "-id", 3) == 0) {

            session_id_on = true;

        } else if (strncmp(argv[i], "-name", 5) == 0) {

            if (one)
                usage(argv[0]);
            else {
                name_based = one = true;
            }

        } else if (strncmp(argv[i], "-rock", 5) == 0) {

            if (one)
                usage(argv[0]);
            else {
                rock_test = one = true;
            }

        } else if (strncmp(argv[i], "-sc", 3) == 0) {

             if (one)
                usage(argv[0]);
            else {
                test_server_cred = one = true;
            }

        } else if (strncmp(argv[i], "-h", 2) == 0) {

            usage(argv[0]);
	    exit(0);
               
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
	if (target_choice != 0) {

	    if (target_choice == 1) {/* target restriction is non_null and no any_other */
		target_rstr.num_restrictions = 1;
		target_rstr.restrictions = (sec_id_restriction_t *)malloc(sizeof(sec_id_restriction_t));
		memset(target_rstr.restrictions, 0, sizeof(sec_id_restriction_t));
		target_rstr.restrictions[0].entry_info.entry_type = sec_rstr_e_type_user;
		target_rstr.restrictions[0].entry_info.tagged_union.id.uuid =test_attr_id; /* any uuid will do */  
	    }
	    else {
		target_rstr.num_restrictions = 2; /* target restriction is non_null, anonym expected */
		target_rstr.restrictions = (sec_id_restriction_t *)malloc(sizeof(sec_id_restriction_t) * target_rstr.num_restrictions);
		memset(target_rstr.restrictions, 0, sizeof(sec_id_restriction_t) * target_rstr.num_restrictions);
		target_rstr.restrictions[0].entry_info.entry_type = sec_rstr_e_type_user;
		target_rstr.restrictions[0].entry_info.tagged_union.id.uuid =test_attr_id; /* any uuid will do */ 
		target_rstr.restrictions[1].entry_info.entry_type = sec_rstr_e_type_any_other;
	    }
	

	    ilc = sec_login_become_initiator(lc, c_deleg_type, NULL,
                                            &target_rstr, NULL, NULL,
                                            compat_mode, &st);
	    if (BAD_STATUS(&st)) {
		dce_error_inq_text((unsigned long)st, error_buf, &err_st);
		fprintf(stderr, "Error from sec_login_become_initiator: %s\n", error_buf);
		goto purge_login_context;
	    }
	}
	else {
	    ilc = sec_login_become_initiator(lc, c_deleg_type, NULL,
                                            NULL, NULL, NULL,
                                            compat_mode, &st);
	    if (BAD_STATUS(&st)) {
		dce_error_inq_text((unsigned long)st, error_buf, &err_st);
		fprintf(stderr, "Error from sec_login_become_initiator: %s\n", error_buf);
		goto purge_login_context;
	    }
	}

	pa_h = sec_login_cred_get_initiator(ilc, &st);
	if (BAD_STATUS(&st)) {
	    if (debug) {
		dce_error_inq_text((unsigned long)st, error_buf, &err_st);
		fprintf(stderr, "DEBUG: Error from sec_login__cred_get_initiator: %s\n",error_buf);
	    }
	    goto purge_deleg_context;
	}

	pa_dataP = sec_cred_get_pa_data (pa_h, &st);
	if (BAD_STATUS(&st)) {
	    if (debug) {
		dce_error_inq_text((unsigned long)st, error_buf, &err_st);
		fprintf(stderr, "DEBUG: Error from sec_cred_get_pa_data: %s\n",error_buf);
	    }
	    goto purge_deleg_context;
	}

	is_anonymous = sec_id_is_anonymous_pa(pa_dataP, &st);

	if (debug) {
	    printf("is anonymous? %s\n", (is_anonymous) ? "y" : "n");
	}

	if (target_choice == 1 && !is_anonymous) {
	    fprintf(stderr, 
                "FAILED: target restriction (n) with non-any_other restriction only should return anonymous pa data.\n");
	    st = 1;
	     goto purge_deleg_context;
	}
	else if (target_choice == 1 && is_anonymous){
	    fprintf(stderr, 
                "PASSED: target restriction (n) with non-any_other restriction only returned anonymous pa data.\n");
	}
	else if (target_choice == 2 && !is_anonymous){
	    fprintf(stderr, 
                "PASSED: target restriction (a) with any_other restriction returned not-anonymous pa data.\n");
	}		
	else if (target_choice == 2 && is_anonymous){
	    fprintf(stderr, 
                "FAILED: target restriction (a) with any_other restriction should return not-anonymous pa data.\n");
	    st = 1;
	     goto purge_deleg_context;
	}		

        if (test_refresh) {
	    sec_login_refresh_identity(ilc, &st);
            if (BAD_STATUS(&st)) {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr, "Error from sec_login_refresh: %s\n", error_buf);
                goto purge_login_context;
            }
            prec.key.key_type = sec_passwd_plain;
            prec.key.tagged_union.plain = pbuf2;
            prec.pepper = NULL;
            prec.version_number = sec_passwd_c_version_none;
            if (! sec_login_validate_identity(ilc, &prec, &reset_passwd,
                &auth_src, &st)) {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,"Error on refresh validate_identity: %s\n", error_buf);
            } else {
                sec_login_set_context(ilc, &st);
            }
	  } /* test_fresh */
    } else { /* not become_initaitor */
        ilc = lc;
    }  /* end of become_initiator */

    /* Is this a test of sec_login_disable_delegation() ? */
    if (disable_dlg) {
        tlc = sec_login_disable_delegation(ilc, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr,"Error from sec_login_disable_delegation: %s\n",
                error_buf);
            goto purge_deleg_context;
        }
        sec_login_purge_context(&ilc, &xst);
	ilc = tlc;
    }

     /* If -c specified, then only test the client side sec_login_cred_ calls */
    if (test_client_cred) {
	sec_id_delegation_type_t dlg_type_ret;
        pa_h = sec_login_cred_get_initiator(ilc, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr,"Error from sec_login_cred_get_initiator: %s\n", error_buf);
            goto purge_deleg_context;
	  }
        pa_dataP = sec_cred_get_pa_data(pa_h, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "Error from sec_cred_get_pa_data: %s\n", error_buf);
            goto purge_deleg_context;
        }
        dlg_type_ret = sec_cred_get_delegation_type(pa_h, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, 
               "FAILED: sec_cred_get_delegation_type status: %s\n", error_buf);
	     goto purge_deleg_context;
        } else if (dlg_type_ret == sec_id_deleg_type_traced) {
            fprintf(stderr,
               "PASSED: sec_cred_get_delegation_type returned correct type\n");
        } else {
            fprintf(stderr, 
               "FAILED: sec_cred_get_delegation_type returned: ");
            switch(dlg_type_ret) {
            case sec_id_deleg_type_none:
                fprintf(stderr, "sec_id_deleg_type_none\n");
                break;
             case sec_id_deleg_type_traced:
                fprintf(stderr, "sec_id_deleg_type_traced\n");
                break;
            case sec_id_deleg_type_impersonation:
                fprintf(stderr, "sec_id_deleg_type_impersonation\n");
                break;
            default:
                fprintf(stderr, "UNKNOWN TYPE: %d\n", dlg_type_ret);
            }
	    st = 1;
	    goto purge_deleg_context;
	  }
        sec_login_cred_init_cursor(&cursor, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "Error from sec_login_cred_init_cursor: %s\n", error_buf);
            goto purge_deleg_context;
        }
        pa_h = sec_login_cred_get_delegate(ilc, &cursor, &st);
        if (BAD_STATUS(&st)) {
            if (STATUS_EQUAL(&st, sec_cred_s_no_more_entries)) {
                CLEAR_STATUS(&st);
                fprintf(stderr,
                    "PASSED: Client only sec_login_cred_get_delegate test\n");
            } else {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
                    "FAILED: Error from sec_login_cred_get_delegate: %s\n", error_buf);
                goto purge_deleg_context;
            }
        } else {
            fprintf(stderr, 
                    "PASSED: Client only sec_login_cred_get_delegate test\n");
        } /* end of test_client_cred */  

    /* Attribute Test */
    } else if (lattr || rattr) {
        init_attrs(pname, &st);
        if (BAD_STATUS(&st)) {
            goto purge_login_context;
        }

        ilc = sec_login_set_extended_attrs(lc, num_attrs, attrs, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr,"FAILED: Error from sec_login_set_extended_attrs: %s\n", error_buf);
            goto purge_login_context;
        }
        if (lattr) {
            pa_h = sec_login_cred_get_initiator(ilc, &st);
            if (BAD_STATUS(&st)) {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr, "FAILED: Error from sec_login_cred_get_initiator: %s\n", error_buf);
                goto purge_deleg_context;
            }
            sec_cred_initialize_attr_cursor(&acursor, &st);
            if (BAD_STATUS(&st)) {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
		    "FAILED: Error from sec_cred_initialize_attr_cursor: %s\n",
error_buf);
                goto purge_deleg_context;
            }
            sec_cred_get_extended_attrs(pa_h, &acursor, &attr, &st);
            if (BAD_STATUS(&st)) {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
		    "FAILED: Error from sec_cred_get_extended_attrs: %s\n", error_buf);
                goto purge_deleg_context;
            } else {
		if (attr.attr_value.attr_encoding != sec_attr_enc_integer) {
                    fprintf(stderr,
			"FAILED: Returned attr_encoding type incorrect.\n");
		    st = 1;
                    goto purge_deleg_context;
		} else
		    if (attr.attr_value.tagged_union.signed_int !=
			TEST_ATTR_INT) {
                    fprintf(stderr,
			"FAILED: Returned attr_value incorrect: %d\n",
			attr.attr_value.tagged_union.signed_int);
		    st = 1;
                    goto purge_deleg_context;
		} else {
                    fprintf(stderr,
			"PASSED: Local set/get extended attr test.\n");
		}
	      }
        } else { /* not lattr */
            rpc_binding_set_auth_info(h, princ_name, rpc_c_authn_level_default,
                                        rpc_c_authn_dce_secret,
                                        (rpc_auth_identity_handle_t) ilc,
                                        rpc_c_authz_dce, &st);
            if (BAD_STATUS(&st)) {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr, "FAILED: Remote attr test; Error from rpc_binding_set_auth_info: %s\n", error_buf);
                goto purge_deleg_context;
            }
            dlg_test_attrs(h, &st);
	    if (GOOD_STATUS(&st)) {
                fprintf(stderr, "PASSED: Remote attribute test.\n");
            } else {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
                  "FAILED: Remote attr test; dlg_test_attrs returned %s\n", error_buf);
           } /* end of test attribute */
	  } /* end of not lattr */
      } else if (name_based) {
        rpc_binding_set_auth_info(h, princ_name, rpc_c_authn_level_default,
                                    rpc_c_authn_dce_secret,
                                    (rpc_auth_identity_handle_t) ilc,
                                    rpc_c_authz_name, &st);

        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "FAILED: authz_name test; Error from rpc_binding_set_auth_info: %s\n", error_buf);
            goto purge_deleg_context;
        }

        retval = dlg_test_authz_name(h, &st);
        if (debug) {
            fprintf(stderr, "dlg_test_authz_name retured %s\n", ((retval) ? 
                    "true" : "false"));
        }		  
            
        if (GOOD_STATUS(&st)) {
            if (retval) 
                fprintf(stderr, "PASSED: Authz_name test.\n");
            else {
                fprintf(stderr, "FAILED: Returned service data is wrong.\n");
		st = 1;
		goto purge_deleg_context;
	    }
        } else {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
                  "FAILED: Authz_name test:  returned %s\n", error_buf);
		goto purge_deleg_context;
	      }

      } else if (rock_test) {
        rpc_binding_set_auth_info(h, princ_name, rpc_c_authn_level_default,
                                    rpc_c_authn_dce_secret,
                                    (rpc_auth_identity_handle_t) ilc,
                                    rpc_c_authz_dce, &st);

        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "FAILED: rock test; Error from rpc_binding_set_auth_info: %s\n", error_buf);
            goto purge_deleg_context;
        }

        retval = dlg_test_rock (h, &st);
        if (debug) {
            fprintf(stderr, "dlg_test_rock retured %s\n", ((retval) ? 
                    "true" : "false"));
        }	
	  
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "FAILED: rock test; Error from dlg_test_rock: %s\n", error_buf);
            goto purge_deleg_context;
        }
            
        if (retval) {
	    fprintf(stderr, "PASSED: Rock test.\n");
	}
	else {
	    if (GOOD_STATUS(&st)) {
                fprintf(stderr, "FAILED: Data got is different from set.\n");
		st = 1;
		goto purge_deleg_context;		

	    }
	    else {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
                  "FAILED: Rock test:  returned %s\n", error_buf);
		goto purge_deleg_context;
	      }
	}

      } else if (session_id_on) {
        rpc_binding_set_auth_info(h, princ_name, rpc_c_authn_level_default,
                                    rpc_c_authn_dce_secret,
                                    (rpc_auth_identity_handle_t) ilc,
                                    rpc_c_authz_dce, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "FAILED: Delegation test; Error from rpc_binding_set_auth_info: %s\n", error_buf);
            goto purge_deleg_context;
        }
	retval = test_session_info(h, &session_id, &expiration, &st);

        if (debug) {
            fprintf(stderr, "test_session_info retured %s\n", ((retval) ? 
                    "true" : "false"));
        }		  
            
        if (retval) {
	    fprintf(stderr, "PASSED: sec_cred_get_authz_session_token() test.\n");
	}
	else {
	    if (GOOD_STATUS(&st)) {
                fprintf(stderr, "FAILED: Data gotton from the first call is different from the second call (test_session_info test).\n");
		st = 1;
		goto purge_deleg_context;
	    }
	    else {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
                fprintf(stderr,
                  "FAILED: sec_cred_get_authz_session_token() test:  returned %s\n", error_buf);
		goto purge_deleg_context;
	      }
	}

        if (debug)
            fprintf(stderr, 
       "test_session_info returned expired (%d), uuid(%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x)\n", expiration,
         session_id.time_low, session_id.time_mid, session_id.time_hi_and_version, 
	 session_id.clock_seq_hi_and_reserved, session_id.clock_seq_low, 
         session_id.node[0], session_id.node[1], session_id.node[2], 
         session_id.node[3], session_id.node[4], session_id.node[5]); 

    /* Delegation Test */
    } else {
        rpc_binding_set_auth_info(h, princ_name, rpc_c_authn_level_default,
                                    rpc_c_authn_dce_secret,
                                    (rpc_auth_identity_handle_t) ilc,
                                    rpc_c_authz_dce, &st);
        if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
            fprintf(stderr, "FAILED: Delegation test; Error from rpc_binding_set_auth_info: %s\n", error_buf);
            goto purge_deleg_context;
        }
        retval = dlg_test_access(h, s_deleg_type, (unsigned_char_p_t)"", &perms, &st);
        if (debug)
            fprintf(stderr, "dlg_test_access returned %s\n",
                    ((retval) ? "true" : "false"));
        if (!become_initiator || disable_dlg) {
            if (STATUS_EQUAL(&st, sec_login_s_deleg_not_enabled)) {
	        CLEAR_STATUS(&st);
		if (!become_initiator)
                    fprintf(stderr, "PASSED: did not become initiator test\n");
		else
                 fprintf(stderr, "PASSED: Delegation disabled test\n");
            } else {
                dce_error_inq_text((unsigned long)st, error_buf, &err_st);
		if (!become_initiator)
                	fprintf(stderr,
                  	"FAILED: did not become intiator test; dlg_test_access error: %s\n",
                  error_buf);
		else
                    fprintf(stderr,
                        "FAILED: Delegation disabled test; dlg_test_access error: %s\n",
                  error_buf);
		if (st == error_status_ok) {
		    st = 1;
		}
            }
        } else if (BAD_STATUS(&st)) {
            dce_error_inq_text((unsigned long)st, error_buf, &err_st);
	    if (debug) {
		fprintf(stderr, "dlg_test_access returnd: %s\n", error_buf);
	    }
	    if (c_deleg_type == sec_id_deleg_type_impersonation)
	    {
		if (negative_test == false)
            		fprintf(stderr,
		 "FAILED: Impersonation test; Error from dlg_test_access: %s\n",			 error_buf);
		else
            		fprintf(stderr,
			 "PASSED: Negative Impersonation test\n", error_buf);
	    }
	    else
		if (negative_test == false)
            		fprintf(stderr,
                "FAILED: Delegation test; Error from dlg_test_access: %s\n",
                	error_buf);
		else
            		fprintf(stderr,
                "PASSED: Negative delegation test\n",
                	error_buf);

	    if (c_deleg_type == s_deleg_type && c_deleg_type != sec_id_deleg_type_none) {
		fprintf(stderr, "FAILED: Delegation test; Error from dlg_test_access: %s\n", error_buf);
	    }
	    else {/* different delegation type, supposed to faile */
		if (st == sec_login_s_deleg_not_enabled) {
		    fprintf(stderr, "PASSED: Delegation test\n");
		    CLEAR_STATUS(&st);
		}
		else {
		    fprintf(stderr, "FAILED: Delegation test; Error from dlg_test_access: %s\n", error_buf);
		}
	    }
        } else {
	    if (c_deleg_type == sec_id_deleg_type_impersonation)
	    {
		if (negative_test == false)
            		fprintf(stderr, "PASSED: Impersonation test\n");
		else
                        if (retval) {
                        fprintf(stderr, "FAILED: Negative Impersonation test\n")
;
                        } else {
                        fprintf(stderr, "PASSED: Negative Impersonation test\n")
;
                        }
	    }
	    else
	    {
		if (negative_test == false)
            		fprintf(stderr, "PASSED: Delegation test\n");
		else
                        if (retval) {
                        fprintf(stderr, "FAILED: Negative Delegation test\n");
                        } else {
                        fprintf(stderr, "PASSED: Negative Delegation test\n");
                        }
            }
          }

      } /* delegation */

purge_deleg_context:
    if ((ilc != NULL) && (ilc != lc)) {
        sec_login_purge_context(&ilc, &xst);
        if (BAD_STATUS(&xst)) {
            dce_error_inq_text((unsigned long)xst, error_buf, &err_st);
            fprintf(stderr, "Error purging initiator context: 0x%s\n", error_buf);
        }
      }
	
purge_login_context:
    sec_login_purge_context(&lc, &xst);
    if (BAD_STATUS(&xst)) {
        dce_error_inq_text((unsigned long)xst, error_buf, &err_st);
        fprintf(stderr, "Error purging principal login context: 0x%s\n", error_buf);
      }
    
    exit(st);
  }
