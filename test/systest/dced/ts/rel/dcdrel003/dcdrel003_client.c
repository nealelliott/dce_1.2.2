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
 * $Log: dcdrel003_client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:47  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:23:24  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:41  annie]
 * 
 * Revision 1.1.2.1  1994/06/27  17:12:38  baum
 * 	Changed names from 03 -> 003
 * 	[1994/06/27  17:09:37  baum]
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
#include <dcdrel003.h>
#include <dcdrel003_server.h>

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
  rpc_binding_handle_t	bind_handle;
  unsigned32		exit_code;

  error_status_t		status;
  char			errbuf[1024];
  int			done;

  status = error_status_ok;
  
  if (argc != 3) 
  {
    fprintf(stderr, "Usage: %s ns-entry-name exit-code\n", argv[0] );
    exit(1);
  }
  
  
  exit_code = atoi(argv[2]);
  
  /*
   * Import the binding info from namespace
   */
  rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
			      (unsigned_char_t *) argv[1],
			      dcdrel003_v1_0_c_ifspec,
			      NULL, 
			      &import_context, 
			      &status);				     
  if (status != rpc_s_ok) 
  {
    print_error("rpc_ns_binding_import_begin()", status);
    exit(1);
  }
 
  done = 0;
  while ( done == 0 )
  {
    rpc_ns_binding_import_next(import_context,
			       &bind_handle,
			       &status);
    if (status != rpc_s_ok) 
    {
      sprintf ( errbuf, "rpc_ns_binding_import_next(%s)", argv[1] );
      print_error( errbuf, status);
      break;
    }

    done = 1;
    TRY 
    {
      (*dcdrel003_v1_0_c_epv.server_stop)(bind_handle,
					exit_code, 
					&status);
      if (status != error_status_ok) 
      {
	print_error("dcdrel003_v1_0_c_epv.server_stop", status);
      }
    }
    CATCH_ALL 
    {
      done = 0;
    }
    ENDTRY;
    /* Should binding free but don;t care in this program */
  }

  if ( done == 0 )
  {
    fprintf (stderr, "dcdrel003_client failed\n" );
    exit ( 1 );
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
        fprintf(stderr,"dcdrel003_client: %s: %s\n", caller, msg);
	free(msg);
}
