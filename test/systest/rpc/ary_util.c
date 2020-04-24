/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ary_util.c,v $
 * Revision 1.1.11.2  1996/03/09  20:50:52  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:52  marty]
 *
 * Revision 1.1.11.1  1995/12/13  21:55:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:09  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:23:46  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:23:19  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:25:07  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:53:41  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:11  mcfarland]
 * 
 * 	Remove common util routines
 * 	[1992/04/15  15:56:26  rps]
 * 
 * 	Fix ifdef
 * 	[1992/03/18  00:12:09  rps]
 * 
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:31:23  melman]
 * 
 * Revision 1.1.1.2  1992/05/28  15:11:54  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * $EndLog$
 */
/*
 *   	FILE_NAME: ary_util.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: make_header
 *		   check_status
 *		   check_exit_code
 *		   string_binding_to_name
 *		   name_from_addr
 *		   addr_from_name
 *		   compare_text
 *		   print_exception
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST: dcerpary
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains functions used by ary_server and ary_client.
 *
 *      	Function make_header constructs a header to be logged with 
 *		all messages.  The header has the form of hostname, program 
 *		name and process id number.  This header aids in determining 
 *		which program logged which message.
 *
 *      	Function check_status receives a status code, the expected 
 *		value of the status, the message to log if the status code 
 *		does not match the expected value, an opcode, a boolean value 
 *		and whether the error originated from an RPC call or not.  If 
 *		the values do not satisfy the opcode, an error message is 
 *		constructed by appending the message passed to the header.  If
 *		the status code resulted from an RPC call (as specified by the
 *		mod parameter), dce_error_inq_text is called to convert the 
 *		status code into an error string.  If the status did not origi-
 *		nate from an RPC call, the status value is appended to the 
 *		error message.   If the boolean value is true, the workstation 
 *		will log the message via xx_severe and exit.  If the boolean 
 *		value is false, the workstation will log the message via 
 *		xx_error and continue.
 *
 *		Function check_exit_code simply calls xx_severe or xx_error
 *		with the error message passed to it, depending on the boolean
 *		exit status.  If the status is true, the workstation exits.
 *		If false, the testcase continues.
 *
 *		Function string_binding_to_name parses the host address and 
 *		port number out of an rpc string.  The address is then con-
 *		verted to its string name counterpart via a call to 
 *		name_from_addr().
 *
 * 		Function name_from_addr calls gethostbyaddr() to return the 
 *		host string name of the address it was passed.
 *
 *		Function addr_from_name calls gethostbyname() to obtain the
 *		network address of the machine name it was passed.
 *
 * 		Function compare_text performs a strncmp on the each array of 
 *		arrays and array of pointers to verify that each of them con-
 *		tain identical characters.  If they do, compare_text() returns
 *		true.  If not, false is returned.
 *
 *              Function print_exception takes an exception and turns it into
 *              a readable form for the tester.
 *
 *   	HISTORY:
 *      	09/16/91        Gerald Cantor           Initial design/code.
 *      	10/18/91        Gerald Cantor           ANSIfied functions.
 *      	10/19/91        Gerald Cantor           Updated prologs.
 *		12/20/91	Gerald Cantor		Added print_exception().
 *              01/08/92        Gerald Cantor           Changed error_message and
 *                                                      dce_text_message to
 *                                                      arrays from char *.
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <dce/cma.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/dce_error.h>
#include <ary.h>
#include <ary_util.h>

/*
 * compare_text performs a strncmp on the array of arrays and array of
 * pointers to verify that each of them contain identical characters.
 */
boolean32 compare_text(array_groups_t *group)
{
	int i;

	for (i = 0; i < MAX_ARRAY_ELEMENTS; i++)
        {
		if (strncmp(group->ary_of_ary[i], 
			   group->ary_of_ptr[i]->the_strg, MAX_LINE_LEN))
			return(false);
        }
        return(true);
}
