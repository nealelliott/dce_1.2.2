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
 * $Log: dcdrel002_client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:29  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:40  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:23:21  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:38  annie]
 * 
 * Revision 1.1.2.1  1994/06/27  17:15:06  baum
 * 	Initial checkin
 * 	[1994/06/27  17:14:30  baum]
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
#include <dcdrel002.h>

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

  error_status_t		status;
  char			errbuf[1024];
  int			done;

  status = error_status_ok;
  
  if (argc != 3) 
  {
    fprintf(stderr, "Usage: %s [ping_op|stop_clean_op|stop_bad_op] ns-entry-name\n", argv[0] );
    exit(1);
  }
  
  
  /*
   * Import the binding info from namespace
   */
  rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
			      (unsigned_char_t *) argv[2],
			      dcdrel002_v1_0_c_ifspec,
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
      sprintf ( errbuf, "rpc_ns_binding_import_next(%s)", argv[2] );
      print_error( errbuf, status);
      break;
    }

    done = 1;
    TRY 
    {
      if ( !strcmp ( argv[1], "ping_op" ) )
	(*dcdrel002_v1_0_c_epv.ping_op) ( bind_handle, &status);
      else
      if ( !strcmp ( argv[1], "stop_bad_op" ) )
	(*dcdrel002_v1_0_c_epv.stop_bad_op) ( bind_handle, &status);
      else
      if ( !strcmp ( argv[1], "stop_clean_op" ) )
	(*dcdrel002_v1_0_c_epv.stop_clean_op) ( bind_handle, &status);
      else
      {
        fprintf ( stderr, "Invalid operation requested\n" );
	status = error_status_ok;
        done = 0;
      }

      if (status != error_status_ok) 
      {
        sprintf ( errbuf, "%s", argv[1] );
        print_error( errbuf, status);
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
    fprintf (stderr, "dcdrel002_client failed\n" );
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
        fprintf(stderr,"dcdrel002_client: %s: %s\n", caller, msg);
	free(msg);
}
