
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
 * $Log: inq_name.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:39  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:44  root
 * 	Submit
 * 	[1995/12/11  14:33:15  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:58  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:37  melman]
 * 
 * Revision 1.1.2.3  1994/07/20  19:57:57  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:39:34  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.1  1994/05/18  21:21:40  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:58  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	inq_name.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_inq_name 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            host_entry;
dced_attr_list_t        host_data;
dced_entry_t            key_entry;
dced_key_list_t         key_list;
server_t		server_entry;

void dced_inq_name_1(void);		/* Test purposes 1 */
void dced_inq_name_2(void);		/* Test purposes 2 */
void dced_inq_name_3(void);		/* Test purposes 3 */
void dced_inq_name_4(void);		/* Test purposes 4 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_inq_name_1,  1 },
    { dced_inq_name_2,  2 },
    { dced_inq_name_3,  3 },
    { dced_inq_name_4,  4 },
    { NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_inq_name_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_inq_name(). 
**
*/
void 
dced_inq_name_1()
{
        error_status_t          st;
	dced_string_t           out_name;
	boolean32		fail_stat;

	tet_infoline("dced_inq_name_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_name along with the entry id,");
	tet_infoline(
	"the entry name and the status error_status_ok is returned.");

	fail_stat = false;

        test_srvrconf_init( &binding_handle, &server_entry, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_inq_name( binding_handle, &server_entry.id, &out_name, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_name failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (strcmp( (char *)out_name, (char *)server_entry.name))
		{
#ifdef DEBUG
			tet_infoline("out entryname is wrong");
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
		free( out_name );
	}

        test_srvrconf_free( &binding_handle, &server_entry, &st);
        if ((st != error_status_ok) && (!fail_stat))
        {
                tet_result(TET_UNRESOLVED);
                fail_stat = true;
        }

	if (fail_stat)
		return;

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_inq_name_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_inq_name(). 
**
*/
void 
dced_inq_name_2()
{

        error_status_t          st;
	dced_string_t           out_name;
        uuid_t                  srvr_id;
	boolean32		fail_stat;

	tet_infoline("dced_inq_name_2, Assertion 02(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_name along with the entry id,");
	tet_infoline(
	"the entry name and the status error_status_ok is returned.");

	fail_stat = false;

        test_srvrexec_init( &binding_handle, &server_entry, &srvr_id, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_inq_name( binding_handle, &srvr_id, &out_name, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_name failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (strcmp( (char *)out_name, (char *)server_entry.name))
		{
#ifdef DEBUG
			tet_infoline("out entryname is wrong");
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
		free( out_name );
	}

        test_srvrexec_free( &binding_handle, &server_entry,  &srvr_id, &st);
        if ((st != error_status_ok) && (!fail_stat))
        {
                tet_result(TET_UNRESOLVED);
                fail_stat = true;
        }

	if (fail_stat)
		return;

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_inq_name_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_inq_name(). 
**
*/
void 
dced_inq_name_3()
{
        error_status_t          st;
	dced_string_t           out_name;
	boolean32		fail_stat;

	tet_infoline("dced_inq_name_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_name along with the hostdata entry id,");
	tet_infoline(
	"the entry name and the status error_status_ok is returned.");

	fail_stat = false;

        test_hostdata_init( &binding_handle, &host_entry, &host_data, 2, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_inq_name( binding_handle, &host_entry.id, &out_name, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_name failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (strcmp( (char *)out_name, (char *)host_entry.name))
		{
#ifdef DEBUG
			tet_infoline("out entryname is wrong");
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
		free( out_name );
	}

        test_hostdata_free( &binding_handle, &host_entry, &host_data, &st);
        if ((st != error_status_ok) && (!fail_stat))
        {
                tet_result(TET_UNRESOLVED);
                fail_stat = true;
        }

	if (fail_stat)
		return;

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_inq_name_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_inq_name(). 
**
*/
void 
dced_inq_name_4()
{
        error_status_t          st;
	dced_string_t           out_name;
	boolean32		fail_stat;

	tet_infoline("dced_inq_name_4, Assertion 04(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_name along with the keytab entry id,");
	tet_infoline(
	"the entry name and the status error_status_ok is returned.");

	fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 1, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_inq_name( binding_handle, &key_entry.id, &out_name, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_name failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (strcmp( (char *)out_name, (char *)key_entry.name))
		{
#ifdef DEBUG
			tet_infoline("out entryname is wrong");
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
		free( out_name );
	}

        test_keytab_free( &binding_handle, &key_entry, &key_list, &st);
        if ((st != error_status_ok) && (!fail_stat))
        {
                tet_result(TET_UNRESOLVED);
                fail_stat = true;
        }

	if (fail_stat)
		return;

	tet_result(TET_PASS);
	return;

}


