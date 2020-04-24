/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: hello_svc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:26:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:54:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:06  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:58:41  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:53  weir]
 * 
 * $EndLog$
 */
/*
**	hello_svc.c-- simple version.
**
**	
*/

#include <dce/dce.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <dce/utctypes.h>
#include <pthread.h>

#include <stdarg.h>
#include <dce/svcfilter.h>
#include <dce/assert.h>

#include "hel_svc.h"
#include <dce/dcesvcmsg.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include "dcehelsvc.h"
#include "dcehelmsg.h"
#include "dcehelmac.h"


/* Following is a routing specifier, passed as an argument to dce_svc_routing() */
/*   below. Its format is spelled out in the svcroute.5 manpage. Note that the  */
/*   "where" specifier is left out here (usually it would be a filename), since */
/*   it need not be specified for STDOUT, STDERR, and DISCARD...                */
/*                                                                              */
unsigned_char_t *my_route = (unsigned_char_t *)"NOTICE:STDERR:";

dce_svc_handle_t hel_svc_handle;


/*
 *	main()
 *
 *	
 */

int
main(
	int				argc,
	char				*argv[])
{

	unsigned32			status = svc_s_ok;

	/* This call sets the contents of a substring area at the beginning of                */
	/*  each message, identifying the program that is writing the message.                */
	/* If you leave it out, the message will contain a substring identifying the program  */
	/*  by process ID.                                                                    */
	/*                                                                                    */
	dce_svc_set_progname(argv[0], &status);
	if (status != svc_s_ok)
	{
		printf("dce_svc_set_progname failed\n");
		exit(1);
	}

	/* The following call is supposed to set up routing of serviceability messages. Note  */
	/*  that it must be called before dce_svc_register()...                               */
	/*                                                                                    */
	dce_svc_routing(my_route, &status);
	if (status != svc_s_ok)
	{
		printf("dce_svc_routing failed\n");
		exit(1);
	}


	/* This call registers the message table. It apparently sets up various service-      */
	/*  ability data structures. You use the handle that is returned by this call in      */
	/*  all subsequent calls to print out serviceability messages, etc...                 */
	/*                                                                                    */
	hel_svc_handle = dce_svc_register(hel_svc_table, (idl_char*)"hel", &status);
	if (status != svc_s_ok)
	{
		printf("dce_svc_register failed\n");
		exit(1);
	}

	/* This call can be used to add a message table to the in-core table. Note that in    */
	/*  order for it to work properly, you have to #include <dce/dce_msg.h>. You also     */
	/*  have to link in dcehelmsg.o and dcehelsvc.o, which contain the code for the mes-  */
	/*  sages and the table, respectively. If you do this, and if you make the following  */
	/*  call, you won't be dependent on having the message catalog installed correctly,   */
	/*  nor (it follows) on having the locale or NLSPATH or whatever correctly set.       */
	/*                                                                                    */
	/* Note the different table name, taken from dcehelmsg.c...                           */
	/*                                                                                    */
	dce_msg_define_msg_table(hel_msg_table, sizeof(hel_msg_table) / sizeof(hel_msg_table[0]), &status);
	if (status != svc_s_ok)
	{
		printf("dce_svc_define_msg_table failed\n");
		exit(1);
	}

	/* The following call prints out a serviceability message. Note the structure of the  */
	/*  call:                                                                             */
	/*                                                                                    */
	/*       1. Nested call to DCE_SVC macro, with arguments:                             */
	/*                     table handle                                                   */
	/*                     format string                                                  */
	/*                                                                                    */
	/*       2. Table index (hel_s_main)                                                  */
	/*                                                                                    */
	/*       3. Message attributes (these are or'ed in together)                          */
	/*                                                                                    */
	/*       4. Message id (hel_s_hello)                                                  */
	/*                                                                                    */
	/*      [5. Any necessary arguments demanded by the format string given in the        */
	/*          DCE_SVC macro call above.]                                                */
	/*                                                                                    */
	/* Note that if the message has been defined (in the .sams file) with certain fields  */
	/*  (i.e., "attributes" and "sub-component"), then a convenience macro will be gener- */
	/*  ated by sams that will automatically take care of arguments 1-4.                  */
	/*                                                                                    */
	/* full version...                                                                    */
	/*                                                                                    */
/*	dce_svc_printf(DCE_SVC(hel_svc_handle, ""), hel_s_main, svc_c_sev_notice, hel_s_hello);
	if (status != svc_s_ok)
	{
		printf("dce_svc_printf failed\n");
		exit(1);
	}
*/
	/* Using the convenience macro to print the same message...                           */
	/*                                                                                    */
	dce_svc_printf(HEL_S_HELLO_MSG);
	if (status != svc_s_ok)
	{
		printf("dce_svc_printf (with convenience macro) failed\n");
		exit(1);
	}

	dce_svc_unregister(hel_svc_handle, &status);
	if (status != svc_s_ok)
	{
		printf("dce_svc_unregister failed\n");
		exit(1);
	}


}


