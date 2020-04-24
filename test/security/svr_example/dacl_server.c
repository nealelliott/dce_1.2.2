/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dacl_server.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:16  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:52:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:12  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:23:16  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:32  annie]
 * 
 * Revision 1.1.2.1  1994/08/02  20:11:41  bhaim
 * 	move acl_server to test area
 * 	[1994/08/02  20:11:05  bhaim]
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or disribute the same at your own risk.
 */
/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
** Unpublished work. All Rights Reserved.
*/
/*
 * SEC_ACL dummy server mainline, for testing/debugging purposes only.
 * Do not use as basis for SEC_ACL manager, as it does not utilize
 * the sec_acl_mgr_ interface, nor was this intended to be an efficient use
 * of ACL storage.  It's sole purpose it to test the client agent and clients.
 *
 * General skeleton from Template/server.c in nidl2 examples
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
"$Id: dacl_server.c,v 1.1.4.2 1996/02/17 23:26:26 marty Exp $";
#endif

#include <stdio.h>
#include <locale.h>
#include <rdaclif.h>
#include <rdaclifv0.h>

#include <string.h>
#include <un_io.h>
#include <macros.h>
#include <dce/dce_error.h>

PUBLIC boolean32 debug_acl_svr = false;

/* EXTERNAL MGR UUID (for test version only) */
extern uuid_t   mgr_uuid;

/* INTERNAL PROTOTYPE (for test version only) */
PUBLIC void rdacl__get_manager_type(
#ifdef __STDC__
void
#endif
);

globalref rdaclif_v0_0_epv_t rdaclif_v0_0_manager_epv;
globalref rdaclif_v1_0_epv_t rdaclif_v1_0_manager_epv;
globalref uuid_t uuid_nil;

/* Server principal name for authenticated rpc */
unsigned_char_p_t   server_princ_name = (unsigned_char_p_t) "aclmgr";

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
            "Usage: %s [-d][-e][-p server_princ_name][-f key_table_name]\n",
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
    rpc_binding_vector_p_t  bvec;
    error_status_t          st, tmp_st;
    ndr_boolean             validfamily;
    ndr_char                *string_binding;
    int                     i;
    unsigned_char_t         annotation[64];
    dce_error_string_t      dce_error;
    uuid_vector_t           acl_server_obj_uuids;
    char                    keytabname[BUFSIZ];
    char                    *keytable;
    unsigned_char_p_t       acl_mgr_uuid;
    boolean32               export_ns_bindings;

    setlocale(LC_ALL, "");

    export_ns_bindings = false;
    keytable = NULL;
    CLEAR_STATUS(&st);

      /* argument 1 must be the protocol family, everything else is checked */
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
        } else if (strncmp(argv[i], "-d", 2) == 0) {

            debug_acl_svr = true;

        } else if (strncmp(argv[i], "-e", 2) == 0) {

            export_ns_bindings = true;

        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    rpc_server_use_all_protseqs((unsigned32)5, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(
            stderr, "Cannot use all protocol sequences - %s\n",
            dce_error
        );
        exit(1);
    }

    /*  TEST VERSION USES SINGLE SEC_ACL MANAGER TYPE, SO INITIALIZE IT NOW */
    rdacl__get_manager_type();

    rpc_server_register_if(
        rdaclif_v0_0_s_ifspec, &uuid_nil,
            (rpc_mgr_epv_t)&rdaclif_v0_0_manager_epv, &st
    );
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        printf("Cannot register rdaclif v0 interface - %s\n", dce_error);
        exit(1);
    }
    rpc_server_register_if(
        rdaclif_v1_0_s_ifspec, &uuid_nil,
            (rpc_mgr_epv_t)&rdaclif_v1_0_manager_epv, &st
    );
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        printf("Cannot register rdaclif v1 interface - %s\n", dce_error);
        exit(1);
    }

    rpc_server_inq_bindings(&bvec, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        printf("Cannot inquire bindings - %s\n", dce_error);
        exit(1);
    }

    /* LOCATE  register all interfaces with the local endpoint
     *         map so all interfaces are reachable with
     *         bound-to-host handles.
     */
    strcpy((char *) annotation, "DCE Sec_Acl Editor, Test Server");
    acl_server_obj_uuids.count = 1;
    acl_server_obj_uuids.uuid[0] = &mgr_uuid;
    rpc_ep_register(rdaclif_v0_0_s_ifspec,
		    bvec,
		    &acl_server_obj_uuids,
		    annotation,
		    &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error registering v0 endpoint: %s\n", dce_error);
    }
    rpc_ep_register(rdaclif_v1_0_s_ifspec,
                    bvec,
                    &acl_server_obj_uuids,
                    annotation,
                    &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error registering v1 endpoint: %s\n", dce_error);
    }

    rpc_server_register_auth_info(server_princ_name, rpc_c_authn_dce_secret,
                                  (rpc_auth_key_retrieval_fn_t) NULL,
                                  (void *) keytable, &st);
    if (BAD_STATUS(&st)) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error: %s\n", dce_error);
        fprintf(stderr, "Unable to register authorization info.\n");
    }

    if (export_ns_bindings) {
        /* Establish a name-service database entry with binding handles */
        rpc_ns_binding_export(rpc_c_ns_syntax_default,
                                (unsigned_char_p_t) "/.:/myfilesystem",
                                rdaclif_v0_0_s_ifspec, bvec, 
                                &acl_server_obj_uuids,
                                &st);
        if (st != error_status_ok) {
            dce_error_inq_text(st, dce_error, (int *) &tmp_st);
            fprintf(stderr, "Error exporting v0 interface to namespace: %s\n", dce_error);
        }
        rpc_ns_binding_export(rpc_c_ns_syntax_default,
                                (unsigned_char_p_t) "/.:/myfilesystem",
                                rdaclif_v1_0_s_ifspec, bvec, 
                                &acl_server_obj_uuids,
                                &st);
        if (st != error_status_ok) {
            dce_error_inq_text(st, dce_error, (int *) &tmp_st);
            fprintf(stderr, "Error exporting v1 interface to namespace: %s\n", dce_error);
        }
    }

#if DEBUG
    rpc__dbg_set_switches("0-3.10", &st);
#endif

    printf("Bindings:\n");
    uuid_to_string(&mgr_uuid, &acl_mgr_uuid, &st);
    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr,
                "Cannot get acl_mgr uuid string representation %s\n",
                dce_error
                );
        exit(1);
    }
    for (i = 0; i < bvec->count; i++) {
        rpc_binding_to_string_binding(
            bvec->binding_h[i], &string_binding, &st
        );
        if (st != error_status_ok) {
            dce_error_inq_text(st, dce_error, (int *) &tmp_st);
            printf("Cannot get string binding - %s\n", dce_error);
            exit(1);
        }
        printf("%s@%s\n", (char *)acl_mgr_uuid, (char *)string_binding);

        rpc_string_free(&string_binding, &st);
        if (st != error_status_ok) {
            dce_error_inq_text(st, dce_error, (int *) &tmp_st);
            fprintf(
                stderr, "Cannot free string binding %s - %s\n",
                string_binding, dce_error
            );
            exit(1);
        }
    }
    rpc_string_free(&acl_mgr_uuid, &st);

    printf("Listening...\n");

    rpc_server_listen((unsigned32)5, &st);

    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error: %s\n", dce_error);
    }
}
