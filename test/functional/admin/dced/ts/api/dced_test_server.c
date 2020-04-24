
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
 * $Log: dced_test_server.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:04  root
 * 	Submit
 * 	[1995/12/11  14:33:02  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:15  root]
 * 
 * Revision 1.1.2.2  1994/09/30  22:55:33  baum
 * 	Fixed a number of test things (OT 12393)
 * 	[1994/09/30  22:54:21  baum]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:41  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:20  melman]
 * 
 * Revision 1.1.2.3  1994/09/16  20:10:47  ganni
 * 	enhancements and cleanup of srvrexec tests
 * 	[1994/09/16  20:09:51  ganni]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:44  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:04  annie]
 * 
 * Revision 1.1.2.1  1994/07/20  19:57:41  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:36:44  ganni]
 * 
 * $EndLog$
 */

/*
** Module:      test_server.c
**
** FACILITY:    DCE Host Daemon API
**
** ABSTRACT:    This module implements the server interface for
**              testing dced api services (srvrconf, srvrexec).
**
*/

#include	<dtapi_impl.h>
#include	<dtapi_1_srvr.h>

int
main(
        int     /* argc */,
        char**  /* argv */
);

/*
**  ROUTINE NAME:
**		main()
**
**  SCOPE:
**		Public
**
**  DESCRIPTION:
*/

int
main(int argc, char **argv)
{
	error_status_t    	stat;
	char			*protseq;
	char			*server_name;
	rpc_binding_vector_p_t  bvec;
	uuid_t			obj_uuid;
	uuid_vector_t		obj_uuid_vec;

	if (argc != 3) {
                fprintf(stderr, "Usage: %s ns-entry-name protseq\n", argv[0]);
                exit(1);
        }

	protseq =   argv[2];
	server_name = dce_strdup ( argv[1] );

	rpc_server_use_protseq( (unsigned_char_t *)protseq, 
				rpc_c_protseq_max_reqs_default, &stat );
	if ( stat != error_status_ok )
	{
		printf( "Can't use protocol sequence %s - %d \n", protseq, stat );
		return( -1 );
	}
  
	rpc_server_register_if( dtapi_1_srvr_v1_0_s_ifspec, 
			  NULL,
			  NULL, 
			  &stat                  );
	if ( stat != error_status_ok )
	{
		printf( "Can't register interface - %d\n", stat );
		return( -1 );
	}
  
	rpc_server_inq_bindings( &bvec, &stat  );
	if ( stat != error_status_ok )
	{
		printf( "Can't inquire bindings - %d\n", stat );
		return( -1 );
	}
  
        uuid_from_string(DCED_SRVR_OBJ1, &obj_uuid, &stat);
        if (stat != error_status_ok)
	{
		printf( "uuid_from_string - %d\n", stat );
		return( -1 );
        }
        obj_uuid_vec.count = 1;
        obj_uuid_vec.uuid[0] = &obj_uuid;

	rpc_ep_register( dtapi_1_srvr_v1_0_s_ifspec, bvec, 
			&obj_uuid_vec,
			(unsigned_char_t *)server_name, &stat );
	if ( stat != error_status_ok )
	{
		printf( "Can't ep register - %d\n", stat );
		return( -1 );
	}

	rpc_ns_binding_export(	rpc_c_ns_syntax_dce, 
				(unsigned_char_t *)server_name,
				dtapi_1_srvr_v1_0_s_ifspec, 
				bvec, &obj_uuid_vec, &stat );
	if ( stat != error_status_ok )
	{
		printf( "Can't export binding - %d\n", stat );
		return( -1 );
	}

#ifdef DEBUG
	printf("dced test server listening\n");
#endif

	rpc_server_listen( rpc_c_listen_max_calls_default, &stat );
	if ( stat != error_status_ok )
 	{
	      printf( "Error on server listen - %d.\n", stat );
 	}

	rpc_ep_unregister(	dtapi_1_srvr_v1_0_s_ifspec, bvec, 
				&obj_uuid_vec, &stat );
	if ( stat != error_status_ok )
	{
		printf( "Can't ep unregister - %d\n", stat );
		return( -1 );
	}

        rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_dce,
                                 (unsigned_char_t *) server_name,
                                 &stat);
        if (stat != error_status_ok)
	{
		printf( "Can't delete ns entry - %d\n", stat );
		return( -1 );
        }

#ifdef DEBUG
printf( "server completing.\n" );
#endif

	return 0;
}



/*
**  ROUTINE NAME:
**		dced_api_t_0()
**
**  DESCRIPTION:
**
*/
error_status_t
dtapi_10(  handle_t handle, idl_ulong_int x, idl_ulong_int *y )
{

	printf("entering dced_api_t_0 %d\n", x);
	*y=RESULT_1;
	printf("leaving dced_api_t_0 %d\n", *y);
	return(error_status_ok);
}

/*
**  ROUTINE NAME:
**		stop_server()
**
**  DESCRIPTION:
**
*/
void
stop_server( handle_t handle, error_status_t *st )
{

	 rpc_mgmt_stop_server_listening(NULL, st);
}
