/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bnk_util.c,v $
 * Revision 1.1.17.2  1996/02/17  23:27:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:45  marty]
 *
 * Revision 1.1.17.1  1995/12/11  22:00:47  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/09  17:14 UTC  bissen
 * 	fix merge: include dce.h
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:27 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/27  16:14  bissen
 * 	merge kk and hpdce01
 * 	[1995/12/11  20:59:48  root]
 * 
 * Revision 1.1.8.2  1993/07/09  12:28:59  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:28:32  root]
 * 	Revision 1.1.13.2  1993/10/14  17:37:08  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:23  cmckeen]
 * 
 * Revision 1.1.6.2  1993/04/23  14:42:17  giza
 * 	dce1.0.2 merge up
 * 	[1993/04/23  13:43:02  giza]
 * 
 * Revision 1.1.3.2  1992/06/11  19:54:17  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:40  mcfarland]
 * 
 * Revision 1.1.13.1  1993/09/22  19:17:50  pellis
 * 	Fixed problem with execution on a i486 machine.
 * 	[1993/09/22  19:17:20  pellis]
 * 
 * Revision 1.1.11.2  1993/07/28  19:32:52  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/28  14:24:19  cmckeen]
 * 
 * Revision 1.1.7.3  1993/05/27  18:49:08  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.7.2  1993/05/27  13:51:56  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  15:44:03  eheller]
 * 
 * 	dce1.0.2 merge up
 * 	[1993/04/23  13:43:02  giza]
 * 
 * Revision 1.1.3.2  1992/06/11  19:54:17  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:40  mcfarland]
 * 
 * 	Remove common util routines
 * 	[1992/04/15  15:57:17  rps]
 * 
 * Revision 1.1.2.3  1992/03/18  01:20:04  rps
 * 	Fix ifdef
 * 	[1992/03/18  00:12:33  rps]
 * 
 * Revision 1.1.2.2  1992/01/22  23:05:59  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:31:29  melman]
 * 
 * $EndLog$
 */
/*
 *   	FILE_NAME: bnk_util.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: make_header
 *		   check_status
 *		   check_exit_code
 *		   string_binding_to_name
 *		   name_from_addr
 *		   addr_from_name
 *		   BankNameToID
 *		   get_uuids
 *		   set_objects
 *		   build_object_vector
 *		   SAlloc
 *		   get_handle_obj
 *		   print_exception
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST(s): dcethrpc dcerprec dcerpbnk dcerpacf
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains functions used by clients and servers.
 *
 *      	Function make_header constructs a header to be logged with 
 *		all messages.  The header has the form of hostname, program 
 *		name and process id number.  This header aids in determining 
 *		which program logged which message.
 *
 *              Function check_status receives a status code, the expected
 *              value of the status, the message to log if the status code
 *              does not match the expected value, an opcode, a boolean value
 *              and whether the error originated from an RPC call or not.  If
 *              the values do not satisfy the opcode, an error message is
 *              constructed by appending the message passed to the header.  If
 *              the status code resulted from an RPC call (as specified by the
 *              mod parameter), dce_error_inq_text is called to convert the
 *              status code into an error string.  If the status did not origi-
 *              nate from an RPC call, the status value is appended to the
 *              error message.   If the boolean value is true, the workstation
 *              will log the message via xx_severe and exit.  If the boolean
 *              value is false, the workstation will log the message via
 *              xx_error and continue.
 *
 *              Function check_exit_code simply calls xx_severe or xx_error
 *              with the error message passed to it, depending on the boolean
 *              exit status.  If the status is true, the workstation exits.
 *              If false, the testcase continues.
 *
 *              Function string_binding_to_name parses the host address and
 *              port number out of an rpc string.  The address is then con-
 *              verted to its string name counterpart via a call to
 *              name_from_addr().
 *
 *              Function name_from_addr calls gethostbyaddr() to return the
 *              host string name of the address it was passed.
 *
 *              Function addr_from_name calls gethostbyname() to obtain the
 *              network address of the machine name it was passed.
 *
 *		Function BankNameToID returns creates a uuid for the bank
 *		name passed in by the tester.
 *		
 *              Function get_uuids returns the binary form of all account
 *              type and transaction type uuids.
 *
 *              Function set_objects sets the account type objects to have
 *              the type passed.  This function fills in entries in the
 *              object registry table.
 *
 *              Function build_object_vector assign values to the malloc'd
 *              object_vector_t.  This vectory is used for registering and
 *              exporting interfaces.
 *
 *              Function SAlloc allocates storage for strings.
 *
 *              Function get_handle_obj returns the object uuid set in the
 *              handle passed.
 *
 *              Function print_exception takes an exception and turns it into
 *              a readable form for the tester.
 *
 *   	HISTORY:
 *      	11/10/90        Gerald Cantor           Initial design/code
 *		12/18/90	Gerald Cantor		Convert check_status 
 *							to use boolean 
 *							parameter.
 *		03/20/91	Gerald Cantor 		Incorperated function
 *							string_binding_to_name
 *		10/20/91	Gerald Cantor		Merged function from
 *							util.c into this file.
 *							ANSIfied functions and
 *							updated prologs and 
 *							comments.
 *		12/17/91	Gerald Cantor		Added print_exception().
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
#include <dce/dce.h>
#include <systest.h>
#include <bnk_util.h>
#include <util.h>

char header[512], buf[128];
uuid_t BankID;

/*
 * BankNameToID obtains a uuid for the bank entered by the tester. 
 */
void BankNameToID(char *name, uuid_t *id)
{
    	error_status_t st;

	uuid_create(id, &st);
    	sprintf(buf, "uuid_create for bank %s failed\n", name);
    	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * get_uuids return the binary forms of all account object type and 
 * transaction type uuids.
 */
void get_uuids(uuid_t *ck_obj, uuid_t *sv_obj, uuid_t *cd_obj, uuid_t *ira_obj,
	       uuid_t *rest_type, uuid_t *unrest_type)
{
/*	
	ck object uuid - cdc79674-f5ad-11c9-912a-02608c2ea88e
	sv object uuid - fa1ecda0-229e-11ca-8698-02608c2c688e 
	cd object uuid - d71a9d20-f5ad-11c9-bceb-02608c2ea88e
	ira object uuid - d83f9aac-f5ad-11c9-aa6d-02608c2ea88e
	restricted type uuid - d9803f8e-f5ad-11c9-851b-02608c2ea88e
	unrestricted type uuid - daff505c-f5ad-11c9-893a-02608c2ea88e
*/
	error_status_t st;

	uuid_from_string(CK_UUID, ck_obj, &st);
	sprintf(buf, "uuid_from_string for ck failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	uuid_from_string(SV_UUID, sv_obj, &st);
	sprintf(buf, "uuid_from_string for sv failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	uuid_from_string(CD_UUID, cd_obj, &st);
	sprintf(buf, "uuid_from_string for cd failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	uuid_from_string(IRA_UUID, ira_obj, &st);
	sprintf(buf, "uuid_from_string for ira failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	uuid_from_string(RES_UUID, rest_type, &st);
	sprintf(buf, "uuid_from_string for rest. failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	uuid_from_string(UNRES_UUID, unrest_type, &st);
	sprintf(buf, "uuid_from_string for unrest. failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * set_objects associate proper type uuids with account object uuid.
 */
void set_objects(uuid_t *ck_obj, uuid_t *sv_obj, uuid_t *cd_obj, 
		 uuid_t *ira_obj, uuid_t *rest_type, uuid_t *unrest_type)
{
	error_status_t st;

	rpc_object_set_type(ck_obj, unrest_type, &st);
	sprintf(buf, "rpc_oject_set_type for ck failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_object_set_type(sv_obj, unrest_type, &st);
	sprintf(buf, "rpc_oject_set_type for sv failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_object_set_type(cd_obj, rest_type, &st);
	sprintf(buf, "rpc_oject_set_type for cd failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);

	rpc_object_set_type(ira_obj, rest_type, &st);
	sprintf(buf, "rpc_oject_set_type for ira failed\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
}

/*
 * build_object_vector builds object uuid vector for use in rpc_ep_register 
 * and rpc_ns_binding_export (used in admin interface only).
 */
void build_object_vector(uuid_vector_p_t object_vec, uuid_p_t id)
{

        object_vec->count = 1;
        object_vec->uuid[0] = id;
}

/*
 * SAlloc allocates storage for a string.
 */

char *SAlloc(char *s)
{
    char *p;

    p = (char *)malloc(strlen(s)+1);
    strcpy(p, s);
    return(p);
}

/*
 * get_handle_obj returns the object uuid of handle rh.
 */
uuid_t *get_handle_obj(handle_t rh)
{
	uuid_t *handle_obj = malloc(sizeof(uuid_t));
	error_status_t st;

	rpc_binding_inq_object(rh, handle_obj, &st);
	sprintf(buf, "Cannot inquire handle's object\n");
	check_status(st, rpc_s_ok, NOT_EQUAL, buf, TRUE, RPC);
	return(handle_obj);
}
