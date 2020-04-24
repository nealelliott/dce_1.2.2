
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
 * $Log: server_disable_if.c,v $
 * Revision 1.1.4.2  1996/02/18  00:31:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:08  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:47  root
 * 	Submit
 * 	[1995/12/11  14:33:31  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:19  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:58  melman]
 * 
 * Revision 1.1.2.3  1994/09/28  17:34:34  baum
 * 	Bad use of local var. (OT 12381)
 * 	[1994/09/28  17:34:22  baum]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:49  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:09  annie]
 * 
 * Revision 1.1.2.1  1994/07/20  19:58:03  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:40:50  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	server_disable_if.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_server_disable_if 
**
*/

#include <dtapi_impl.h>
#include <dtapi_1_srvr.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   exec_handle;
server_t		server_entry;
uuid_t			srvr_proc_id;
boolean32		setup_fail;

void setup(void);
void cleanup(void);
void dced_server_disable_if_1(void);          /* Test purposes 1 */
void dced_server_disable_if_2(void);          /* Test purposes 2 */
void dced_server_disable_if_3(void);          /* Test purposes 3 */
void dced_server_disable_if_4(void);          /* Test purposes 4 */

void (*tet_startup)(void) = setup;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_server_disable_if_1,  1 },
    { dced_server_disable_if_2,  2 },
    { dced_server_disable_if_3,  3 },
    { dced_server_disable_if_4,  4 },
    { NULL, 0 }
};

void
setup()
{
	error_status_t		st;

	setup_fail=false;
        test_srvrexec_init( &exec_handle, &server_entry, &srvr_proc_id, &st);
        if (st != error_status_ok)
		setup_fail=true;
}

void
cleanup()
{
	error_status_t		st;

	if (!setup_fail)
		test_srvrexec_free(&exec_handle, &server_entry, 
					&srvr_proc_id, &st);
}

/*
** ROUTINE NAME:
**		dced_server_disable_if_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_server_disable_if(). 
**
*/
void 
dced_server_disable_if_1()
{
	service_t		*service_ptr;
	error_status_t		st;

	tet_infoline("dced_server_disable_if_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_disable_if along with the server object id,");
	tet_infoline(
	"a rpc interface id, a server interface will be disabled.");


	if (setup_fail)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	service_ptr=&(server_entry.services.list[0]);
       	dced_server_disable_if(	exec_handle, 
				&srvr_proc_id, 
				&service_ptr->ifspec, 
				&st);
	if (st != error_status_ok)
       	{
#ifdef DEBUG
		tet_infoline("dced_server_disable_if failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
       	}

	if (!test_api_10(error_status_ok/* ??? expected failure */))
	{
		tet_result(TET_FAIL);
		return;
        }

	tet_result(TET_PASS);
	return;

}
/*
** ROUTINE NAME:
**		dced_server_disable_if_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_server_disable_if(). 
**
*/
void 
dced_server_disable_if_2()
{
	dced_binding_handle_t   conf_handle;
	service_t		*service_ptr;
	error_status_t		st;

	tet_infoline("dced_server_disable_if_2, Assertion 02(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_disable_if, dced_s_bad_binding_status is returned.");

	if (setup_fail)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	dced_util_binding_create( SRVRCONF_SVC, &conf_handle, &st);
        service_ptr=&(server_entry.services.list[0]);
        dced_server_disable_if( conf_handle, 
			  &srvr_proc_id, 
			  &service_ptr->ifspec, 
			  &st);
	if (st != dced_s_bad_binding)
	{
#ifdef DEBUG
		tet_infoline("dced_server_disable_if failed:");
		tet_infoline("Expected dced_s_bad_binding_status, Received:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
	}
	else
		tet_result(TET_PASS);

	dced_binding_free(conf_handle, &st);
	return;

}


/*
** ROUTINE NAME:
**		dced_server_disable_if_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_server_disable_if(). 
**
*/
void 
dced_server_disable_if_3()
{
	uuid_t			srvr_id;
	service_t		*service_ptr;
	error_status_t		st;

	tet_infoline("dced_server_disable_if_3, Assertion 03(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_disable_if along with a non-existant server object id,");
	tet_infoline(
	"dced_s_not_found is returned.");

	if (setup_fail)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	uuid_create( &srvr_id, &st);
        service_ptr=&(server_entry.services.list[0]);
	dced_server_disable_if(	exec_handle, 
			   	&srvr_id, 
			  	&service_ptr->ifspec, 
				&st);
	if (st != dced_s_not_found)
	{
#ifdef DEBUG
		tet_infoline("dced_server_disable_if failed:");
		tet_infoline("Expected dced_s_not_found, Received:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
	}
	else
		tet_result(TET_PASS);

	return;

}


/*
** ROUTINE NAME:
**		dced_server_disable_if_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_server_disable_if(). 
**
*/
void 
dced_server_disable_if_4()
{
	rpc_if_id_t		rpc_id;
	error_status_t		st;

	tet_infoline("dced_server_disable_if_4, Assertion 04(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_disable_if along with a non-existant rpc interface,");
	tet_infoline(
	"dced_s_not_found is returned.");

	if (setup_fail)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	uuid_create( &rpc_id.uuid, &st);
	rpc_id.vers_major = 1;
	rpc_id.vers_minor = 0;
	dced_server_disable_if(	exec_handle, 
			   	&srvr_proc_id,
			  	&rpc_id,
				&st);
	if (st != dced_s_not_found)
	{
#ifdef DEBUG
		tet_infoline("dced_server_disable_if failed:");
		tet_infoline("Expected dced_s_not_found, Received:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
	}
	else
		tet_result(TET_PASS);

	return;

}



