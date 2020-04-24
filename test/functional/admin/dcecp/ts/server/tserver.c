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
 * $Log: tserver.c,v $
 * Revision 1.1.4.2  1996/02/18  00:29:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:17:13  root
 * 	Submit
 * 	[1995/12/11  14:32:43  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:17:49  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:03  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:14:54  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:35  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:31:37  devsrc]
 * 
 * Revision 1.1.2.1  1994/06/10  21:40:33  pwang
 * 	Initial implementation
 * 	[1994/06/10  21:35:51  pwang]
 * 
 * $EndLog$
 */

/*
 * This module implements a test server used in dcecp server
 * object functional test
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/dce_error.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <tserver.h>
#include <test_server.h>

void main(
	int 	/* argc */, 
	char** 	/* argv */
);
void print_error(
	char*		/* caller */, 
	error_status_t	/* status */
);

void server_stop(
	rpc_binding_handle_t	/* handle */,
	unsigned32		/* quit_code */,
	error_status_t*		/* st */
);

static unsigned32 exit_code;

boolean32    our_auth_fn(
              rpc_binding_handle_t client_binding,           /* in  */
              unsigned32           requested_mgmt_operation, /* in  */
              unsigned32           *status                   /* out */
    ) {
	*status = rpc_s_ok;
	return( TRUE );         /* allow anyone to call any mgmt function */
}

/*
 * main() -- setup the test server
 */
void
main(int argc, char **argv)
{
	rpc_binding_vector_t		*bind_vector_p;
	uuid_t				obj_uuid;
	uuid_vector_t			obj_uuid_vec;
	error_status_t			status;


	if (argc != 2) {
		fprintf(stderr, "Usage: %s ns-entry-name\n", argv[0]);
		exit(1);
	}

	/* 
	 * Initialize object uuid 
	 */
	uuid_from_string(OBJ_UUID, &obj_uuid, &status);
	if (status != uuid_s_ok) {
		print_error("uuid_from_string()", status);
		exit(1);
	}
	obj_uuid_vec.count = 1;
	obj_uuid_vec.uuid[0] = &obj_uuid;

	/* 
	 * Register interface with rpc runtime
	 */
	rpc_server_register_if(tserver_v1_0_s_ifspec, NULL, NULL, &status);
	if (status != rpc_s_ok) {
		print_error("rpc_server_register_if()", status);
		exit(1);
	}

	/* 
	 * Tell rpc runtime we want to use all supported protocol sequences 
	 */
	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, &status);
	if (status != rpc_s_ok) {
		print_error("rpc_server_use_all_protseqs()", status);
		exit(1);
	}

	/* 
	 * Ask the runtime which binding handle(s) it's going to let us use
	 */
	rpc_server_inq_bindings(&bind_vector_p, &status);
	if (status != rpc_s_ok) {
		print_error("rpc_server_inq_bindings()", status);
		exit(1);
	}

	/* 
	 * Register authentication info with rpc runtime
	 */
	rpc_server_register_auth_info(SERVER_PRINC_NAME,
	    			      rpc_c_authn_dce_secret, 
				      (rpc_auth_key_retrieval_fn_t) NULL,
	    			      (void *) NULL, 
				      &status);
	if (status != rpc_s_ok) {
		print_error("rpc_server_register_auth_info()", status);
		exit(1);
	}

	/* 
	 * Register binding info with endpoint map
	 */
	rpc_ep_register(tserver_v1_0_s_ifspec, 
			bind_vector_p,
            		(uuid_vector_t *) &obj_uuid_vec,
	    		(unsigned_char_t *) "test  server of dcecp server obj",
			&status);
	if (status != rpc_s_ok) {
		print_error("rpc_ep_register()", status);
		exit(1);
	}

	/* 
	 * Export binding info to the namespace 
	 */
	rpc_ns_binding_export(rpc_c_ns_syntax_dce, 
			      (unsigned_char_t *) argv[1],
	    		      tserver_v1_0_s_ifspec, 
			      bind_vector_p,
	    		      (uuid_vector_t *) &obj_uuid_vec, 
			      &status);
	if (status != rpc_s_ok) {
		print_error("rpc_ns_binding_export()", status);
		exit(1);
	}
	/*
	 * Set up authorizing function so that dced may use
	 * rpc_mgmt_stop_server_listening() to stop this server
	 * (see rpc_mgmt_set_authorization_fn(3rpc) and
	 * rpc_mgmt_stop_server_listening(3rpc) -- note that
	 * a server MUST call rpc_mgmt_set_authorization_fn()
	 * to get behaviour other than the default, and that the
	 * default behaviour is to disallow rpc_mgmt_stop_server_listen()
	 * from ANY client).
	 */
	rpc_mgmt_set_authorization_fn(our_auth_fn,
				      &status);
	if (status != rpc_s_ok) {
	    print_error("rpc_mgmt_set_authorization_fn()", status);
		exit(1);
	}

	/* 
	 * Listen for service requests 
	 */
	rpc_server_listen(rpc_c_listen_max_calls_default, &status);
	if (status != rpc_s_ok) {
		print_error("rpc_server_listen()", status);
		exit(1);
	}

	/* 
	 * Returned from listen loop, it should be hanppened after
	 * mgr routine calls rpc_mgmt_stop_server_listening
	 */
	rpc_ep_unregister(tserver_v1_0_s_ifspec, 
			bind_vector_p,
       			(uuid_vector_t *) &obj_uuid_vec,
			&status);
	if (status != rpc_s_ok) {
		print_error("rpc_ep_unregister()", status);
		exit(1);
	}

	rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_dce, 
				 (unsigned_char_t *) argv[1], 
				 &status);
	if (status != rpc_s_ok) {
		print_error("rpc_ns_mgmt_entry_delete()", status);
		exit(1);
	}

	fprintf(stderr, "tserver: stopping at exit code: %d\n", 
		exit_code);
	exit(exit_code);
}


/*
 *
 * print_error -- Prints text associated with bad status code.
 *
 */
void
print_error(char *caller, error_status_t status)
{
	char	*msg;

	msg = (char *) dce_sprintf(status);
	fprintf(stderr, "tserver: %s: %s\n", caller, msg);
	free(msg);

	return;
}

/*
 * server_stop -- Instruct server quits as a certain quit code
 */
void
server_stop(rpc_binding_handle_t	handle,
	    unsigned32			quit_code,
	    error_status_t		*st)

{
	*st = error_status_ok;

	/*
	 * Just follows order to exit
	 */
	exit_code = quit_code;
	rpc_mgmt_stop_server_listening(NULL, st);
}

