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
 * $Log: tclient.c,v $
 * Revision 1.1.4.2  1996/02/18  00:29:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:17:09  root
 * 	Submit
 * 	[1995/12/11  14:32:42  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:17:47  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:00  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:14:52  melman]
 * 
 * Revision 1.1.2.3  1994/07/05  16:14:41  pwang
 * 	Merged with changes from 1.1.2.2
 * 	[1994/07/05  16:14:20  pwang]
 * 
 * 	Removed while loop on binding import.
 * 	[1994/07/05  16:07:56  pwang]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:33  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:31:29  devsrc]
 * 
 * Revision 1.1.2.1  1994/06/10  21:40:32  pwang
 * 	Initial implementation
 * 	[1994/06/10  21:35:14  pwang]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/exc_handling.h>
#include <tserver.h>
#include <test_server.h>

void 
main(
	int	/* argc */, 
	char**	/* argv */
);

void
print_error(
	char*		/* caller */, 
	error_status_t	/* status */
);

/*
 * main() -- Instruct server to quit 
 */
void 
main(int argc, char **argv)
{
	rpc_ns_handle_t		import_context;
	uuid_t			obj_uuid;
	rpc_binding_handle_t	bind_handle;
	unsigned32		exit_code;
	error_status_t		status, status1;

	status = status1 = error_status_ok;

	if (argc != 3) {
		fprintf(stderr, 
		  "Usage: tclient ns-entry-name exit-code\n");
		exit(1);
	}


	exit_code = atoi(argv[2]);

	/*
	 * Get object uuid
	 */
	uuid_from_string(OBJ_UUID, &obj_uuid, &status);
	if (status != uuid_s_ok) {
		print_error("uuid_from_string", status);
		exit(1);
	}

	/*
	 * Import the binding info from namespace
	 */
	 rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
				     (unsigned_char_t *) argv[1],
				     tserver_v1_0_c_ifspec,
				     &obj_uuid, 
				     &import_context, 
				     &status);				     
	if (status != rpc_s_ok) {
		print_error("rpc_ns_binding_import_begin()", status);
		exit(1);
	}

	/* 
	 * Import bindings one at a time 
	 */
	rpc_ns_binding_import_next(import_context, &bind_handle, &status);

	/* 
	 * Release the binding import context 
	 */
	rpc_ns_binding_import_done(&import_context, 
				   &status1);

	if (status != rpc_s_ok) {
		print_error("rpc_ns_binding_import_next", status);
		exit(1);
	}
	
	/* 
	 * Annotate binding handles for security 
	rpc_binding_set_auth_info(bind_handle,
				  SERVER_PRINC_NAME,
				  rpc_c_protect_level_pkt_integ,
				  rpc_c_authn_dce_secret,
				  NULL,
				  rpc_c_authz_name,
				  &status);
	if (status != rpc_s_ok) {
		print_error("rpc_binding_set_auth_info", status);
		exit(1);
	}
	 */

	TRY {
		(*tserver_v1_0_c_epv.server_stop)(bind_handle,
						  exit_code, 
						  &status);
	}
	CATCH_ALL {
		fprintf(stderr, 
	                "tclient: tserver_v1_0_c_epv.server_stop failed\n");
		exit(1);
	}

	ENDTRY;

	if (status != error_status_ok) {
		print_error("tserver_v1_0_c_epv.server_stop", status);
		exit(1);
	}
	exit(0);
}

/*
 * print_error -- Prints text associated with bad status code.
 */
void
print_error(char *caller, error_status_t status)
{
	char *msg;

	msg = (char *) dce_sprintf(status);
        fprintf(stderr,"tclient: %s: %s\n", caller, msg);
	free(msg);
}
