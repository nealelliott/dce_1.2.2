
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
 * $Log: server_create.c,v $
 * Revision 1.1.4.2  1996/02/18  00:31:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:05  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:42  root
 * 	Submit
 * 	[1995/12/11  14:33:29  root]
 * 
 * Revision 1.1.2.2  1994/09/30  22:55:46  baum
 * 	Fixed a number of test things (OT 12393)
 * 	[1994/09/30  22:54:32  baum]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:17  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:55  melman]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:48  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:07  annie]
 * 
 * Revision 1.1.2.1  1994/07/20  19:59:08  ganni
 * 	update the tests for more validation
 * 	[1994/07/20  19:45:48  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	server_create.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_server_create 
**
*/

#include <dtapi_impl.h>

char txt_msg[DCED_TEST_MSG_LEN];

/*
 *  Global Declarations.
 */
dced_binding_handle_t   conf_handle;
server_t		server_entry;
boolean32		setup_fail;

void setup(void);
void cleanup(void);
void dced_server_create_1(void);          /* Test purposes 1 */
void dced_server_create_2(void);          /* Test purposes 2 */

void (*tet_startup)(void) = setup;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_server_create_1,  1 },
    { dced_server_create_2,  2 },
    { NULL, 0 }
};

void
setup()
{
	error_status_t		st;

	setup_fail=false;
        dced_util_binding_create( SRVRCONF_SVC, &conf_handle, &st);
        if (st != error_status_ok)
		setup_fail=true;
}

void
cleanup()
{
	error_status_t		st;

	if (!setup_fail)
        	dced_binding_free( conf_handle, &st);
}

/*
** ROUTINE NAME:
**		dced_server_create_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_server_create(). 
**
*/
void 
dced_server_create_1()
{
        server_t		*out_data;
        unsigned32              fail_stat;
	error_status_t		st;

	tet_infoline("dced_server_create_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_create along with the server object,");
	tet_infoline(
	"its entry will be created and the status is set to error_status_ok.");

	fail_stat = false;

	if (setup_fail)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

        /*
         * Initialize the server object information
         */
        test_srvr_entry_init( &server_entry, &st);
	sprintf(txt_msg,"enter: dced_server_create");
	tet_infoline(txt_msg);
	dced_server_create( conf_handle, &server_entry, &st);
	sprintf(txt_msg,"leave: dced_server_create %d", st);
	tet_infoline(txt_msg);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_server_create failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
        	dced_object_read( conf_handle, &server_entry.id,
					(void **)&out_data, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_object_read (srvrconf) failed:");
		    dced_util_printerr(st);
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else
		{
		    print_server_t( (server_t *) out_data);

                    if (!verify_server_t( &server_entry, (server_t *)out_data))
                    {
#ifdef DEBUG
			tet_infoline("verify_server_t failed");
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
		    }
		}

        	dced_server_delete( conf_handle, &server_entry.id, &st);
		if (st != error_status_ok)
        	{
#ifdef DEBUG
                    tet_infoline("dced_server_delete failed:");
                    dced_util_printerr(st);
#endif
		    tet_result(TET_UNRESOLVED);
		    fail_stat = true;
        	}
        }

        test_srvr_entry_free( &server_entry, &st);
	if (fail_stat)
		return;
	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_server_create_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_server_create(). 
**
*/
void 
dced_server_create_2()
{
	dced_binding_handle_t   exec_handle;
	error_status_t		st;

	tet_infoline("dced_server_create_2, Assertion 02(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_create, dced_s_bad_binding_status is returned.");

	dced_util_binding_create( SRVREXEC_SVC, &exec_handle, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	dced_server_create( exec_handle, &server_entry, &st);
	if (st != dced_s_bad_binding)
	{
#ifdef DEBUG
		tet_infoline("dced_server_create failed:");
		tet_infoline("Expected dced_s_bad_binding_status, Received:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
	}
	else
		tet_result(TET_PASS);

	dced_binding_free (exec_handle, &st);
	return;

}


