/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dts_intermediary.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:25  root]
 * 
 * Revision 1.1.2.2  1994/10/18  18:51:31  agd
 * 	expand copyright
 * 	[1994/10/18  17:39:49  agd]
 * 
 * Revision 1.1.2.1  1994/09/29  21:02:50  bhaim
 * 	submit dlgcfg002 test
 * 	[1994/09/29  21:01:27  bhaim]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <locale.h>
#include <dts_test.h>

#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/passwd.h>
#include <dce/daclif.h>
#include "dts_macros.h"


#define EXIT_ON_ERROR(st) \
    if (BAD_STATUS(&st)) { \
        exit(st); \
    }

static sec_login_handle_t   lc;
static unsigned_char_t      entry[256];
static unsigned_char_t      hostname[64];
boolean32                   debug = false;
sec_acl_permset_t           perms;
sec_id_compatibility_mode_t  compat_mode;

boolean32 dts_test_access(
    handle_t                 h,
    sec_acl_component_name_t component_name,
    sec_acl_permset_t        *permset,
    error_status_t           *stp
)
{
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           pname;
    boolean32                   test;
    unsigned32                  plvl, an, az, su, nt;
    error_status_t              tmp_st;
    uuid_t                      mgr_type;
    sec_acl_handle_t            aclh;
    sec_acl_bind_auth_info_t    auth_info;
    sec_login_handle_t          dlh;

    CLEAR_STATUS(stp);
    test = false;
    if (debug) {
        fprintf(stderr, "DEBUG: Entered dts_test_access\n");
    }

    rpc_binding_inq_auth_caller(h, &caller, &pname, &plvl, &an, &az, stp);
    if (BAD_STATUS(stp)) {
        if (debug)
          fprintf(stderr, "Error from rpc_binding_inq_auth_caller: %x\n",*stp);
        return test;
    }
    
    dlh = sec_login_become_delegate(caller, lc, sec_id_deleg_type_traced,
                                    NULL, NULL, NULL, NULL,
                                    compat_mode, stp);
    if (BAD_STATUS(stp)) {
        if (debug)
            fprintf(stderr, "Error from sec_login_become_delegate: %x\n",*stp);
        return test;
    }

    /* Get a binding to the sample DCE ACL server with the delegated context */
    auth_info.info_type = sec_acl_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_lvl = rpc_c_authn_level_default;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = dlh;
    sec_acl_bind_auth(entry, false, &auth_info, &aclh, stp);
    if (BAD_STATUS(stp)) {
        if (debug)
            fprintf(stderr, "Error from sec_acl_bind: %x\n", *stp);
        goto test_access_return;
    }
    sec_acl_get_manager_types(aclh, sec_acl_type_object, 1, &su, &nt,
                                &mgr_type, stp);
    if (BAD_STATUS(stp)) {
        if (debug)
            fprintf(stderr, "Error from sec_acl_get_mgr_types: %x\n", *stp);
    } else {
        test = sec_acl_test_access(aclh, &mgr_type, perms, stp);
        if (!test) {            /* if sec_acl_access is false */
                if (!(BAD_STATUS(stp))) /* but status is good */
                        *stp = sec_acl_not_authorized; /* set status to bad*/
        }
        if (debug) {
            if (BAD_STATUS(stp)) {
                fprintf(stderr, "Error from sec_acl_test_access: %x\n", *stp);
            } else {
                fprintf(stderr, "DEBUG: test_access returned %s\n",
                        ((test) ? "true" : "false"));
            }
        }
    }
    sec_acl_release_handle(&aclh, &tmp_st);

test_access_return:
    sec_login_purge_context(&dlh, &tmp_st);
    if (debug && BAD_STATUS(&tmp_st)) {
        fprintf(stderr, "Error purging delegation context: %x\n", *stp);
    }
    return test;
}


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
"Usage: %s [-p server_princ_name][-f key_table_name][-cm n|i|c ][-e name][-h hostname][-d][-all][-read][-write][-control]\n",
        name);
}


static void register_server(
  char              *keytable,
  unsigned_char_p_t server_princ_name,
  error_status_t    *stp
)
{
    rpc_binding_vector_p_t  bvec;
    error_status_t          tmp_st;
    ndr_char                *string_binding;
    int                     i;
    unsigned_char_t         annotation[64];
    dce_error_string_t      dce_error;
    uuid_t                  uuid_nil, obj;
    uuid_vector_t           test_server_obj_uuids;
    dts_test_v0_0_epv_t     dts_test_v0_0_manager_epv =
      { dts_test_access };

    uuid_create_nil(&uuid_nil, &tmp_st);
    uuid_from_string((unsigned_char_p_t)"c71e3b8d-9421-11cd-8338-0000c083e054",
      &obj, &tmp_st);
    rpc_server_use_all_protseqs(5, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(
            stderr, "Cannot use protocol sequence - %s\n",
            dce_error
        );
        exit(1);
    }
    rpc_server_register_if(dts_test_v0_0_s_ifspec, &uuid_nil,
                             (rpc_mgr_epv_t)&dts_test_v0_0_manager_epv, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        printf("Cannot register dts_test interface - %s\n", dce_error);
        exit(1);
    }

    rpc_server_inq_bindings(&bvec, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        printf("Cannot inquire bindings - %s\n", dce_error);
        exit(1);
    }

    /* LOCATE  register all interfaces with the local endpoint
     *         map so all interfaces are reachable with
     *         bound-to-host handles.
     */
    strcpy((char *) annotation, "DCE DTS Delegation Test Server");
    test_server_obj_uuids.count = 1;
    test_server_obj_uuids.uuid[0] = &obj;
    rpc_ep_register(dts_test_v0_0_s_ifspec,
		    bvec,
		    &test_server_obj_uuids,
		    annotation,
		    stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error registering endpoint: %s\n", dce_error);
    }

    rpc_server_register_auth_info(server_princ_name, rpc_c_authn_dce_secret,
                                  (rpc_auth_key_retrieval_fn_t) NULL,
                                  (void *) keytable, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error: %s\n", dce_error);
        fprintf(stderr, "Unable to register authorization info.\n");
    }

    /* Establish a name-service database entry with binding handles */
    rpc_ns_binding_export(rpc_c_ns_syntax_default,
                            (unsigned_char_p_t) "/.:/dts_test",
                            dts_test_v0_0_s_ifspec, bvec,
                            &test_server_obj_uuids,
                            stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error exporting interface to namespace: %s\n",
                dce_error);
    }

    printf("Bindings:\n");
    for (i = 0; i < bvec->count; i++) {
        rpc_binding_to_string_binding(
            bvec->binding_h[i], &string_binding, stp
        );
        if (BAD_STATUS(stp)) {
            dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
            printf("Cannot get string binding - %s\n", dce_error);
            exit(1);
        }
        printf("%s\n", (char *)string_binding);

        rpc_string_free(&string_binding, stp);
        if (BAD_STATUS(stp)) {
            dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
            fprintf(
                stderr, "Cannot free string binding %s - %s\n",
                string_binding, dce_error
            );
            exit(1);
        }
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
    error_status_t          st, tmp_st;
    dce_error_string_t      dce_error;
    char                    *keytable;
    char                    keytabname[BUFSIZ];
    unsigned_char_p_t       server_princ_name = (unsigned_char_p_t) "dts_svr";
    sec_passwd_str_t        pbuf;
    int                     i;

    setlocale(LC_ALL, "");

    keytable = NULL;
    CLEAR_STATUS(&st);
    compat_mode = sec_id_compat_mode_initiator;
    bzero(entry,sizeof(entry));
    bzero(hostname,sizeof(hostname));
    strcpy((char *) entry, "/.:/hosts/");
    gethostname(hostname,sizeof(hostname));
    strcat((char *) entry,(char *)hostname);
    strcat((char *) entry,"/dts-entity");
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -p requires principal name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            server_princ_name = (unsigned_char_p_t) argv[i];

        } else if (strcmp(argv[i], "-f") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -f requires keytable name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            strcpy(keytabname, argv[i]);
            keytable = &keytabname[0];
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



        } else if (strcmp(argv[i], "-e") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -e requires entry name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            bzero(entry,sizeof(entry));
            strcpy((char *)entry, argv[i]);
        } else if (strcmp(argv[i], "-h") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -h requires hostname argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
    	    bzero(entry,sizeof(entry));
            bzero(hostname,sizeof(hostname));
            strcpy((char *)hostname, argv[i]);
            strcpy((char *) entry, "/.:/hosts/");
            strcat((char *) entry,(char *)hostname);
            strcat((char *) entry,"/dts-entity");
        } else if (strncmp(argv[i], "-d", 2) == 0) {

            debug = true;

        } else if (strcmp(argv[i], "-all") == 0) {

    	    perms = (sec_acl_perm_read|sec_acl_perm_write|sec_acl_perm_control);

        } else if (strcmp(argv[i], "-read") == 0) {

    	    perms = (sec_acl_perm_read);

        } else if (strcmp(argv[i], "-write") == 0) {

    	    perms = (sec_acl_perm_write);

        } else if (strcmp(argv[i], "-control") == 0) {

    	    perms = (sec_acl_perm_control);


        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    printf("ACL entry  = %s\n",entry);

    strcpy((char *) pbuf, "dts_svr");
    register_server(keytable, server_princ_name, &st);
    EXIT_ON_ERROR(st);
    log_in(&lc, "dts_svr", pbuf, &st);
    EXIT_ON_ERROR(st);
    
    printf("Listening...\n");

    rpc_server_listen((unsigned32)5, &st);

    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error: %s\n", dce_error);
    }
    sec_login_purge_context(&lc, &tmp_st);
}
