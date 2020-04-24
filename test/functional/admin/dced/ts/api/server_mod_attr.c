
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
 * $Log: server_mod_attr.c,v $
 * Revision 1.1.4.2  1996/02/18  00:31:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:10  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:52  root
 * 	Submit
 * 	[1995/12/11  14:33:33  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:22  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:16:00  melman]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:51  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:12  annie]
 * 
 * Revision 1.1.2.1  1994/07/20  19:58:08  ganni
 * 	initial version
 * 	[1994/07/20  19:41:29  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	server_mod_attr.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_server_modify_attributes 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   conf_handle;
server_t		server_entry;
boolean32               setup_fail;

void setup(void);
void cleanup(void);
void dced_server_modify_attributes_1(void);		/* Test purposes 1 */
void dced_server_modify_attributes_2(void);		/* Test purposes 2 */
void dced_server_modify_attributes_3(void);		/* Test purposes 3 */

void (*tet_startup)(void) = setup;      /* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;    /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_server_modify_attributes_1,  1 },
    { dced_server_modify_attributes_2,  2 },
    { dced_server_modify_attributes_3,  3 },
    { NULL, 0 }
};

void
setup()
{
        error_status_t          st;

        setup_fail=false;
        test_srvrconf_init( &conf_handle, &server_entry, &st);
        if (st != error_status_ok)
                setup_fail=true;
}

void
cleanup()
{
        error_status_t          st;

        if (!setup_fail)
        	test_srvrconf_free( &conf_handle, &server_entry, &st);
}

/*
** ROUTINE NAME:
**		dced_server_modify_attributes_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_server_modify_attributes(). 
**
*/
void 
dced_server_modify_attributes_1()
{
        server_t		*out_data;
        dced_attr_list_t        in_data;
        unsigned32              fail_stat;
	error_status_t		st;

	tet_infoline("dced_server_modify_attributes_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_server_modify_attributes along with the server object,");
	tet_infoline(
	"its entry will be created and the status is set to error_status_ok.");

	fail_stat = false;

        if (setup_fail)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        test_attr_init(&in_data, 1, &st);
	dced_server_modify_attributes(	conf_handle, &server_entry.id, 
					&in_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_server_modify_attributes failed:");
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
		    if (!test_attrlist_verify(	&in_data,
						&out_data->attributes))
                    {
#ifdef DEBUG
			tet_infoline("test_attrlist_verify failed");
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
		    }
		}
        }

	if (fail_stat)
		return;
	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**              dced_server_modify_attributes_2()
**
** DESCRIPTION:
**              This routine verifies Assertion 02(A) for the function
**              dced_server_modify_attributes().
**
*/
void
dced_server_modify_attributes_2()
{
        dced_binding_handle_t   exec_handle;
        dced_attr_list_t        in_data;
        error_status_t          st;

        tet_infoline("dced_server_modify_attributes_2, Assertion 02(A):");
        tet_infoline(
        "When a srvrexec dced binding handle is supplied to ");
        tet_infoline(
        "dced_server_modify_attributes, dced_s_bad_binding_status is returned.");

        dced_util_binding_create( SRVREXEC_SVC, &exec_handle, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        test_attr_init(&in_data, 1, &st);
	dced_server_modify_attributes(	exec_handle, &server_entry.id, 
					&in_data, &st);
        if (st != dced_s_bad_binding)
        {
#ifdef DEBUG
                tet_infoline("dced_server_modify_attributes failed:");
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

/*
** ROUTINE NAME:
**              dced_server_modify_attributes_3()
**
** DESCRIPTION:
**              This routine verifies Assertion 02(A) for the function
**              dced_server_modify_attributes().
**
*/
void
dced_server_modify_attributes_3()
{
        uuid_t                  srvr_id;
        dced_attr_list_t        in_data;
        error_status_t          st;

        tet_infoline("dced_server_modify_attributes_3, Assertion 02(A):");
        tet_infoline(
        "When a srvrconf dced binding handle is supplied to ");
        tet_infoline(
        "dced_server_modify_attributes along with a non-existant server object id,");
        tet_infoline(
        "dced_s_not_found is returned.");

        if (setup_fail)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        uuid_create(&srvr_id, &st);
        test_attr_init(&in_data, 1, &st);
        dced_server_modify_attributes( conf_handle, &srvr_id, &in_data, &st);
        if (st != dced_s_not_found)
        {
#ifdef DEBUG
                tet_infoline("dced_server_modify_attributes failed:");
                tet_infoline("Expected dced_s_bad_binding_status, Received:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
        }
        else
                tet_result(TET_PASS);

        return;

}

