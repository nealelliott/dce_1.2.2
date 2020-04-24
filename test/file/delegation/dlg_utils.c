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
 * $Log: dlg_utils.c,v $
 * Revision 1.1.9.1  1996/10/17  18:18:58  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:21  damon]
 *
 * Revision 1.1.4.2  1994/08/10  19:23:08  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:25  annie]
 * 
 * Revision 1.1.4.1  1994/07/13  22:33:09  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:17:47  devsrc]
 * 
 * 	Delegation Functional Tests - Initial Submit
 * 	[1994/06/08  18:30:28  delgado]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <locale.h>

#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/rpcpvt.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/passwd.h>
#include <dfsdlg_test.h>

static sec_login_handle_t   lc;
static unsigned_char_t      entry[256];
boolean32                   debug = false;

/*
 * Adapting HP's delegation test program for DFS use
 */

void register_server(nsentry_name, server_princ_name, register_server_auth,
                     server_if, server_epv, server_obj_uuid, server_annotation,
                     keytable, stp)

  char              *nsentry_name;
  unsigned_char_p_t server_princ_name;
  boolean32         register_server_auth;
  rpc_if_handle_t   *server_if;
  rpc_mgr_epv_t     *server_epv;
  unsigned_char_p_t server_obj_uuid;
  char              *server_annotation;
  char              *keytable;
  error_status_t    *stp;

{
    rpc_binding_vector_p_t  bvec;
    error_status_t          tmp_st;
    ndr_char                *string_binding;
    int                     i;
    unsigned_char_t         annotation[64];
    dce_error_string_t      dce_error;
    uuid_t                  uuid_nil, obj;
    uuid_vector_t           test_server_obj_uuids;

    uuid_create_nil(&uuid_nil, &tmp_st);
    uuid_from_string(server_obj_uuid,  &obj, &tmp_st);
    rpc_server_use_all_protseqs(5, stp);
    if (BAD_STATUS(stp)) {        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(
            stderr, "Cannot use protocol sequence - %s\n",
            dce_error
        );
        exit(1);
    }
    rpc_server_register_if(server_if, &uuid_nil, server_epv, stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        printf("Cannot register dlg_test interface - %s\n", dce_error);
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
    strcpy((char *) annotation, server_annotation);
    test_server_obj_uuids.count = 1;
    test_server_obj_uuids.uuid[0] = &obj;
    rpc_ep_register(server_if,
		    bvec,
		    &test_server_obj_uuids,
		    annotation,
		    stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error registering endpoint: %s\n", dce_error);
    }

    if (register_server_auth){
        rpc_server_register_auth_info(server_princ_name, rpc_c_authn_dce_secret,
                                  (rpc_auth_key_retrieval_fn_t) NULL,
                                  (void *) keytable, stp);
        if (BAD_STATUS(stp)) {
            dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
            fprintf(stderr, "Error: %s\n", dce_error);
            fprintf(stderr, "Unable to register authorization info.\n");
        }
    }
    /* Establish a name-service database entry with binding handles */
    rpc_ns_binding_export(rpc_c_ns_syntax_default,
                             nsentry_name,
                             server_if, bvec,
                            &test_server_obj_uuids,
                            stp);
    if (BAD_STATUS(stp)) {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error exporting interface to namespace: %s\n",
                dce_error);
    }

    fprintf(stderr,"Bindings:\n");
    for (i = 0; i < bvec->count; i++) {
        rpc_binding_to_string_binding(
            bvec->binding_h[i], &string_binding, stp
        );
        if (BAD_STATUS(stp)) {
            dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
            printf("Cannot get string binding - %s\n", dce_error);
            exit(1);
        }
        fprintf(stderr,"%s\n", (char *)string_binding);

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

/***********************************/


void bind_to_svr(handlep, nsentry, stp)
    handle_t *handlep;
    unsigned_char_p_t nsentry;
    error_status_t *stp;

{
    rpc_ns_inq_handle_t         inquiry_context;
    rpc_ns_import_handle_t      import_context;
    uuid_t                      obj_uuid;
    error_status_t              tmp_st;
    dce_error_string_t          dce_error;

    /* Get the uuid for the object
     * We assume that these calls will yield a status code
     * telling us if we could read the entry...
     */
    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce,
                                    nsentry,
                                    &inquiry_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_begin: %s\n", dce_error);
      exit(*stp);
    }
    rpc_ns_entry_object_inq_next(inquiry_context, &obj_uuid, stp);
    if (STATUS_EQUAL(stp, rpc_s_no_more_members)) {
        fprintf(stderr,"No objects for %s, creating nill obj uuid \n",nsentry);
        uuid_create_nil(&obj_uuid, stp);
    } else if (BAD_STATUS(stp)) {
      dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_next: %s\n", dce_error);
      exit(*stp);
    }

    rpc_ns_entry_object_inq_done(&inquiry_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
      fprintf(stderr, "Error on rpc_ns_entry_object_inq_done: %s\n", dce_error);
      exit(*stp);
    }

    /* Get a binding to that entry */
    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
                                nsentry,
                                (rpc_if_handle_t) NULL, &obj_uuid,
                                &import_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
      fprintf(stderr, "Error on rpc_ns_binding_import_begin: %s\n", dce_error);
      exit(*stp);
    }
    rpc_ns_binding_import_next(import_context, handlep, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
      fprintf(stderr, "Error on rpc_ns_binding_import_next: %s\n", dce_error);
      exit(*stp);
    }
    rpc_ns_binding_import_done(&import_context, stp);
    if (BAD_STATUS(stp)) {
      dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
      fprintf(stderr, "Error on rpc_ns_binding_import_done: %s\n", dce_error);
      exit(*stp);
    }
}


void log_in(
    sec_login_handle_t *lcp,
    unsigned_char_p_t  user,
    unsigned_char_p_t  pbuf,
    error_status_t     *stp
)
{
    sec_passwd_rec_t     prec;
    sec_login_auth_src_t auth_src;
    boolean32            reset_passwd;
    error_status_t              tmp_st;
    dce_error_string_t          dce_error;

    CLEAR_STATUS(stp);

    if (sec_login_setup_identity(user, sec_login_no_flags, lcp, stp)) {
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = pbuf;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        if (! sec_login_validate_identity(*lcp, &prec, &reset_passwd,
            &auth_src, stp)) {
            dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
            fprintf(stderr, "Error from validate_identity for user %s: %s\n", user,dce_error);
	} else {
	    sec_login_set_context(*lcp, stp);
	}
    } else {
        dce_error_inq_text(*stp, dce_error, (int *) &tmp_st);
        fprintf(stderr, "Error from setup_identity for user %s: %s\n", user, dce_error);
    }
}





