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
 * $Log: dcdrel003_server.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:31  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:51  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:23:25  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:41  annie]
 * 
 * Revision 1.1.2.1  1994/06/27  17:12:42  baum
 * 	Changed names from 03 -> 003
 * 	[1994/06/27  17:09:41  baum]
 * 
 * $EndLog$
 */

/*
 * This module implements a test server used in the dcdrel003 System Tests.
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
#include <dcdrel003.h>
#include <dcdrel003_server.h>

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

/*
 * main() -- setup the test server
 */
void
main(int argc, char **argv)
{
  rpc_binding_vector_t		*bind_vector_p;
  uuid_t			obj_uuid;
  uuid_vector_t			obj_uuid_vec;
  error_status_t		status;
  char				errbuf[1024];
  
  if (argc != 2) 
  {
    fprintf(stderr, "Usage: %s ns-entry-name\n", argv[0]);
    exit(1);
  }

  /*
   * Delete CDS entry if it already exists to avoid multiple object UUIDs.
   */
  rpc_ns_mgmt_entry_delete ( rpc_c_ns_syntax_dce, 
			     (unsigned_char_t *) argv[1], 
			     &status );
  
  /* 
   * Generate object uuid 
   */
  uuid_create ( &obj_uuid, &status );
  if ( status != uuid_s_ok ) 
  {
    print_error("uuid_from_string()", status);
    exit(1);
  }
  obj_uuid_vec.count = 1;
  obj_uuid_vec.uuid[0] = &obj_uuid;
  
  /* 
   * Register interface with rpc runtime
   */
  rpc_server_register_if ( dcdrel003_v1_0_s_ifspec, NULL, NULL, &status );
  if ( status != rpc_s_ok ) 
  {
    print_error ( "rpc_server_register_if()", status );
    exit(1);
  }
  
  /* 
   * Tell rpc runtime we want to use all supported protocol sequences 
   */
  rpc_server_use_all_protseqs ( rpc_c_protseq_max_reqs_default, &status );
  if ( status != rpc_s_ok ) 
  {
    print_error ( "rpc_server_use_all_protseqs()", status );
    exit(1);
  }
  
  /* 
   * Ask the runtime which binding handle(s) it's going to let us use
   */
  rpc_server_inq_bindings(&bind_vector_p, &status);
  if (status != rpc_s_ok) 
  {
    print_error("rpc_server_inq_bindings()", status);
    exit(1);
  }
  
  /* 
   * Register binding info with endpoint map
   */
  rpc_ep_register(dcdrel003_v1_0_s_ifspec, 
		  bind_vector_p,
		  (uuid_vector_t *) &obj_uuid_vec,
		  (unsigned_char_t *) "Test Server for DCED System Tests",
		  &status);
  if (status != rpc_s_ok) 
  {
    print_error("rpc_ep_register()", status);
    exit(1);
  }
  
  /* 
   * Export binding info to the namespace 
   */
  rpc_ns_binding_export(rpc_c_ns_syntax_dce, 
			(unsigned_char_t *) argv[1],
			dcdrel003_v1_0_s_ifspec, 
			bind_vector_p,
			(uuid_vector_t *) &obj_uuid_vec, 
			&status);
  if (status != rpc_s_ok) 
  {
    fprintf ( errbuf, "rpc_ns_binding_export(%s)", argv[1] );
    print_error( errbuf, status);
    exit(1);
  }
  
  /* 
   * Listen for service requests 
   */
  rpc_server_listen(rpc_c_listen_max_calls_default, &status);
  if (status != rpc_s_ok) 
  {
    print_error("rpc_server_listen()", status);
    exit(1);
  }
  
  /* 
   * Returned from listen loop, it should be hanppened after
   * mgr routine calls rpc_mgmt_stop_server_listening
   */
  rpc_ep_unregister(dcdrel003_v1_0_s_ifspec, 
		    bind_vector_p,
		    (uuid_vector_t *) &obj_uuid_vec,
		    &status);
  if (status != rpc_s_ok) 
  {
    print_error("rpc_ep_unregister()", status);
    exit(1);
  }
  
  rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_dce, 
			   (unsigned_char_t *) argv[1], 
			   &status);
  if (status != rpc_s_ok) 
  {
    print_error("rpc_ns_mgmt_entry_delete()", status);
    exit(1);
  }
  
  fprintf(stderr, "%s: stopping at exit code: %d\n", argv[0], 
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
	fprintf(stderr, "dcdrel003_server: %s: %s\n", caller, msg);
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

