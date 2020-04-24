/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_server.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:31  root]
 * 
 * Revision 1.1.2.1  1994/06/14  20:00:10  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:16:52  weir]
 * 
 * $EndLog$
 */
/*
 * OLD HISTORY:
 * 
 * Revision 1.2.5.5  1993/10/28  21:07:30  weir
 * 	String handling fixes for OT 9215
 * 	[1993/10/28  21:06:49  weir]
 *
 * Revision 1.2.5.4  1993/10/28  20:37:33  weir
 * 	String handling fixes for OT 9215
 * 	[1993/10/28  20:36:37  weir]
 * 
 * Revision 1.2.5.3  1993/10/21  19:44:49  weir
 * 	timop for DCE 1.0.3, with following fixes:
 * 	CR 4804: use DCE status codes
 * 	CR 4805: use relative principal names
 * 	CR 7460: initialize remote status parameter in server manager
 * 	CR 9215: port to HP platform.
 * 	[1993/10/21  19:38:29  weir]
 * 
 * Revision 1.2.2.3  1993/02/04  20:32:36  cjd
 * 	Embedded copyright notice
 * 	[1993/02/04  20:30:04  cjd]
 * 
 * Revision 1.2.2.2  1992/12/18  18:18:49  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:33  marcyw]
 * 
 * Revision 1.2  1992/01/19  22:14:31  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * 
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or distribute the same at your own risk.
 */
/*
 * Copyright (c) 1990, 1991  
 * Open Software Foundation, Inc. 
 *  
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty. 
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
**	timop_server.c
**
**	Server program for timop interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <dce/dce_error.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include "timop.h"
#include "timop_aux.h"
#include "timop_server.h"

/* Declare manager EPV.  This EPV could be bulk-initialized here,
   but we prefer to do it one operation at a time in main(). */
timop_v1_0_epv_t		manager_epv;


/*
 *	main()
 *
 *	Get started -- set up server the way we want it, and call listen loop.
 */

int
main(
	int				argc,
	char				*argv[])
{

	unsigned_char_t			*server_name;
	rpc_binding_vector_t		*bind_vector_p;
	unsigned32			status;
	int				i;
	uuid_t				type_uuid, obj_uuid;
	uuid_vector_t			obj_uuid_vec;
	uuid_t				objects[NUM_OBJS];


	/* Check usage and initialize. */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s namespace_server_name\n", argv[0]);
		exit(1);
	}
	server_name = (unsigned_char_t *)argv[1];
	obj_uuid_vec.count = NUM_OBJS;
	obj_uuid_vec.uuid[0] = &obj_uuid;

	/* Initialize manager EPV (just one entry point in this example). */
	manager_epv.timop_getspan = getspan_ep;

	/* Initialize object uuid (just one in this example). */
	uuid_from_string(OBJ_UUID, /* &objects[0] */ &obj_uuid, &status);

	if (status != uuid_s_ok)
	{
		print_error("uuid_from_string()", status);
		exit(1);
	}

	/* Initialize type uuid (just one in this example). */
	uuid_create(&type_uuid, &status);
	if (status != uuid_s_ok)
	{
		print_error("uuid_create()", status);
		exit(1);
	}

	/* Register object/type uuid associations with rpc runtime. */
	rpc_object_set_type(/* &objects[0] */ &obj_uuid, &type_uuid, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_object_set_type()", status);
		exit(1);
	}

	/* Register interface/type_uuid/epv associations with rpc runtime. */
	rpc_server_register_if(timop_v1_0_s_ifspec, &type_uuid,
	    (rpc_mgr_epv_t)&manager_epv, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_server_register_if()", status);
		exit(1);
	}

	/* Tell rpc runtime we want to use all supported protocol sequences. */
	rpc_server_use_all_protseqs(MAX_CONC_CALLS_PROTSEQ, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_server_use_all_protseqs()", status);
		exit(1);
	}

	/* Ask the runtime which binding handle(s) it's going to let us use. */
	rpc_server_inq_bindings(&bind_vector_p, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_server_inq_bindings()", status);
		exit(1);
	}


	/* Register authentication info with rpc runtime. */
	rpc_server_register_auth_info(SERVER_PRINC_NAME,
	    rpc_c_authn_dce_secret, NULL /*default key retrieval function*/,
	    KEYTAB /*server key table for this example*/, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_server_register_auth_info()", status);
		exit(1);
	}

	/* Establish server's login context(s), if necessary.
	   In this example we just use the default login context,
	   so we do NOTHING here. */

	/* Decide what to do upon server termination.  It would be prudent
	   to handle signals and decide what to do if the listen loop returns
	   (e.g., clean exported info out of endpoint map and namespace,
	   something that is not usually done for a persistent server),
	   but since this is just an example we don't do those things here. */

	/* Register binding info with endpoint map. */
	rpc_ep_register(timop_v1_0_s_ifspec, bind_vector_p,
            (uuid_vector_t *)&obj_uuid_vec,
	    (unsigned_char_t *)"timop server, version 1.0", &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ep_register()", status);
		exit(1);
	}

	/* Export binding info to the namespace. */
	rpc_ns_binding_export(rpc_c_ns_syntax_dce, server_name,
	    timop_v1_0_s_ifspec, bind_vector_p,
	    (uuid_vector_t *)&obj_uuid_vec, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_export()", status);
		exit(1);
	}

	/* Listen for service requests (semi-infinite loop). */
	fprintf(stdout, "Server %s ready.\n", server_name);
	rpc_server_listen(MAX_CONC_CALLS_TOTAL, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_server_listen()", status);
		exit(1);
	}

	/* Returned from listen loop.  We haven't arranged for this. */
	fprintf(stderr, " Server: Unexpectedly returned from server_listen()\n");
	exit(1);
}


/*****
*
* print_error-- Server version. Prints text associated with bad status code.
*
*****/
void
print_error(caller, status)
char *caller;		/* a string identifying the routine that received the error  */
error_status_t status;	/* the status we want to print the message for               */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stderr," Server: %s: %s\n", caller, error_string);
}

