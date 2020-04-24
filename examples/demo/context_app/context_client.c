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
 * $Log: context_client.c,v $
 * Revision 1.1.7.3  1996/02/18  18:25:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:09  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:30  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:06:23  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:38:46  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:09:15  weir
 * 	Submit sample sources (CR 12635)
 * 	[1994/10/21  19:08:08  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [27.VI.94]                                                               */
/*                                                                          */
/* context_client.c -- a minimal client for the simple test interface.      */
/*                                                                          */
/*   An obvious enhancement would be to make the client multi-threaded,     */
/*    and have it spawn several threads opening different stores, and       */
/*    operating on them all at the same time.                               */
/*                                                                          */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include "context.h"

/* Amount of data to read from server store...                              */
#define REPLY_SIZE 256


void print_error(char *_1,
		error_status_t _2);



/******
*
* main -- 
*
*
******/
int
main(
	int argc,
	char *argv[]
)
{
	handle_t binding_h;                     /* Our binding handle.      */
	store_handle_t sthandle;                /* Server store context     */
						/*  handle.                 */
	idl_char reply[REPLY_SIZE], *reply_ptr;  /* For reads from store.   */
	unsigned32 count;                     /* To hold data read/written  */
						/*  value from calls.       */
	error_status_t status;

	unsigned_char_t *string_binding; /* For string binding conversions. */
	rpc_ns_handle_t import_context;  /* Context for importing bindings. */


	/* Check the command line...                                        */
	if (argc < 2)
	{
		fprintf(stderr, "Usage: context_client <message>\n");
		exit(1);
	}

	/* Start importing servers. Note that the contents of the environ-  */
	/*  ment variable RPC_DEFAULT_ENTRY are used to determine the entry */
	/*  to import from...                                               */
	fprintf(stdout, "context_client: Calling rpc_ns_binding_import_begin()...\n");
	rpc_ns_binding_import_begin(
		rpc_c_ns_syntax_default,
		NULL,                         /* Use the RPC_DEFAULT_ENTRY. */
		store_v1_0_c_ifspec,
		NULL,
		&import_context,
		&status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_import_begin()", status);
		exit(1);
	}

	/* Import the first server (we could iterate here, but we'll just   */
	/*  take the first one)...                                          */
	fprintf(stdout, "context_client: Calling rpc_ns_binding_import_next()...\n");
	rpc_ns_binding_import_next(import_context, &binding_h, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_import_next()", status);
		exit(1);
	}

	/* Free the import context...                                       */
	fprintf(stdout, "context_client: Calling rpc_ns_binding_import_done()...\n");
	rpc_ns_binding_import_done(&import_context, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_import_done()", status);
		exit(1);
	}


	/* Convert the binding to a readable string...                      */
	fprintf(stdout, "context_client: Calling rpc_binding_to_string_binding()...\n");
	rpc_binding_to_string_binding(binding_h, &string_binding, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_binding_to_string_binding()", status);
		exit(1);
	}

	/* Print it...                                                      */
	fprintf(stdout, "context_client: Imported resolved binding == %s\n", string_binding);

	/* Free the string binding space...                                 */
	fprintf(stdout, "context_client: Calling rpc_string_free()...\n");
	rpc_string_free(&string_binding, &status);
	if (status != rpc_s_ok)
	{	
		print_error("rpc_string_free()", status);
		exit(1);
	}

	/* Open a store at the server...                                    */
	store_open(
		binding_h,
		256,
		&sthandle,
		&status);

	/* Now write the message we were passed on the command line to that */
	/*  store...                                                        */
	store_write(
		sthandle,
		strlen(argv[1]),
		(idl_byte *)argv[1],
		&count,
		&status);

	/* Write the same message a second time...                          */
	store_write(
		sthandle,
		strlen(argv[1]),
		(idl_byte *)argv[1],
		&count,
		&status);

	/* Move the store read/write pointer to "2"...                      */
	store_set_ptr(sthandle, 2, &status);

	/* Now read from the current read/write position in the store...    */
	store_read(
		sthandle,
		256,
		reply,
		&count,
		&status);

	/* Print what was read...                                           */
	printf("Simple Context Client: Read: %s\n", reply); 

	/* Reset the store read/write pointer to the beginning of the       */
	/*  store...                                                        */
	store_set_ptr(sthandle, 0, &status);

	/* Now read from the current read/write position in the store...    */
	store_read(
		sthandle,
		256,
		reply,
		&count,
		&status);

	/* Print what was read...                                           */
	printf("Simple Context Client: Read: %s\n",reply); 

	/* Close the store...                                               */
	store_close(&sthandle, &status);

}



/******
 *
 * print_error-- Client version. Prints text associated with bad status code.
 *
 *
 *
 ******/

void
print_error(char *caller, /* Routine that received the error.               */
error_status_t status)    /* The status we want to print the message for.   */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stderr," Client: %s: %s\n", caller, error_string);
}

