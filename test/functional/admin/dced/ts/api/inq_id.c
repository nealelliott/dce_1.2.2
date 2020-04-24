
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
 * $Log: inq_id.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:38  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:42  root
 * 	Submit
 * 	[1995/12/11  14:33:14  root]
 * 
 * Revision 1.1.2.2  1994/09/30  22:55:43  baum
 * 	Fixed a number of test things (OT 12393)
 * 	[1994/09/30  22:54:29  baum]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:57  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:37  melman]
 * 
 * Revision 1.1.2.3  1994/07/20  19:57:56  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:39:19  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.1  1994/05/18  21:21:38  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:54  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	inq_id.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_inq_id 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t	binding_handle;
dced_entry_t		host_entry;
dced_attr_list_t	host_data;
dced_entry_t		key_entry;
dced_key_list_t		key_list;
server_t		server_entry;

void dced_inq_id_1(void);		/* Test purposes 1 */
void dced_inq_id_2(void);		/* Test purposes 2 */
void dced_inq_id_3(void);		/* Test purposes 3 */
void dced_inq_id_4(void);		/* Test purposes 4 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_inq_id_1,  1 },
    { dced_inq_id_2,  2 },
    { dced_inq_id_3,  3 },
    { dced_inq_id_4,  4 },
    { NULL, 0 }
};



/*
** ROUTINE NAME:
**		dced_inq_id_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_inq_id(). 
**
*/
void 
dced_inq_id_1()
{
        error_status_t          st;
	uuid_t			out_id;
	boolean32		fail_stat;

	tet_infoline("dced_inq_id_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_id along with the srvrconf entry name,");
	tet_infoline(
	"the entry id and the status error_status_ok is returned.");

	fail_stat = false;

        test_srvrconf_init( &binding_handle, &server_entry,  &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_inq_id( binding_handle, DCED_TEST_SRVR, &out_id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_id failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (uuid_compare(&server_entry.id, &out_id, &st))
		{
#ifdef DEBUG
			tet_infoline("uuid_compare failed");
			print_uuid((char *)"input uuid", &server_entry.id);
			print_uuid((char *)"output uuid",&out_id);
			dced_util_printerr(st);
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
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
**		dced_inq_id_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_inq_id(). 
**
*/
void 
dced_inq_id_2()
{

        error_status_t          st;
	uuid_t			out_id, srvr_id;
	boolean32		fail_stat;

	tet_infoline("dced_inq_id_2, Assertion 02(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_id along with the srvrexec entry name,");
	tet_infoline(
	"the entry id and the status error_status_ok is returned.");

	fail_stat = false;

        test_srvrexec_init( &binding_handle, &server_entry, &srvr_id,  &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_inq_id( binding_handle, DCED_TEST_SRVR, &out_id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_id failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (uuid_compare(&srvr_id, &out_id, &st))
		{
#ifdef DEBUG
			tet_infoline("uuid_compare failed");
			print_uuid((char *)"input uuid", &srvr_id);
			print_uuid((char *)"output uuid",&out_id);
			dced_util_printerr(st);
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
	}

        test_srvrexec_free( &binding_handle, &server_entry, &srvr_id, &st);
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
**		dced_inq_id_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_inq_id(). 
**
*/
void 
dced_inq_id_3()
{
        error_status_t          st;
	uuid_t			out_id;
	boolean32		fail_stat;

	tet_infoline("dced_inq_id_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_id along with the hostdata entry name,");
	tet_infoline(
	"the entry id and the status error_status_ok is returned.");

	fail_stat = false;

        test_hostdata_init( &binding_handle, &host_entry, &host_data, 2, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_inq_id( binding_handle, DCED_TEST_HOST, &out_id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_id failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (uuid_compare(&host_entry.id, &out_id, &st))
		{
#ifdef DEBUG
			tet_infoline("uuid_compare failed");
			print_uuid((char *)"input uuid", &host_entry.id);
			print_uuid((char *)"output uuid",&out_id);
			dced_util_printerr(st);
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
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

#ifdef TEST_CASE_CORE
/* the following test core dumps dced */
void 
dced_inq_id_3()
{
	dced_binding_handle_t   hd_handle;
        error_status_t          st;
	uuid_t			out_id;
	unsigned char		raw_data[]="dced_test_host";

	tet_infoline("dced_inq_id_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_id along with the hostdata entry name,");
	tet_infoline(
	"the entry id and the status error_status_ok is returned.");

        dced_util_binding_create( HOST_SVC, &binding_handle, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	test_hostentry_init(&host_entry, &st);
	test_hostattr_init(&host_data, 2, &st);
	dced_hostdata_create( binding_handle, &host_entry, &host_data, &st);
	test_hostattr_free(&host_data, &tmp_st);

	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_hostdata_create failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free1 failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
                return;
        }

	tet_infoline("create the binding handle again");
        dced_util_binding_create( HOST_SVC, &hd_handle, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }
/*
        dced_binding_set_auth_info(hd_handle,
                                  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_dce_private,
                                  NULL,
                                  rpc_c_authz_dce,
                                  &st);
*/
	tet_infoline("do   inq id");

/*
	dced_inq_id( hd_handle, raw_data, &out_id, &st);
	dced_inq_id( hd_handle, "dced_test_host", &out_id, &st);
	dced_inq_id( hd_handle, DCED_TEST_HOST, &out_id, &st);
*/
	dced_inq_id( hd_handle, "hostdata", &out_id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_id failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
		dced_hostdata_delete( hd_handle, &host_entry.id, &st);
		dced_binding_free( hd_handle, &st);
		return;
        }

        dced_hostdata_delete( hd_handle, &host_entry.id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_delete failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_UNRESOLVED);
		return;
        }

        dced_binding_free( hd_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free2 failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
                return;
        }

	tet_result(TET_PASS);
	return;

}
#endif

/*
** ROUTINE NAME:
**		dced_inq_id_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_inq_id(). 
**
*/
void 
dced_inq_id_4()
{
        error_status_t          st;
	uuid_t			out_id;
	boolean32		fail_stat;

	tet_infoline("dced_inq_id_4, Assertion 04(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_inq_id along with the keytab entry name,");
	tet_infoline(
	"the entry id and the status error_status_ok is returned.");

	fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 1, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_inq_id( binding_handle, DCED_KEY_USER, &out_id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_inq_id failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
        }

        if (!fail_stat)
	{
		if (uuid_compare(&key_entry.id, &out_id, &st))
		{
#ifdef DEBUG
			tet_infoline("uuid_compare failed");
			print_uuid((char *)"input uuid", &key_entry.id);
			print_uuid((char *)"output uuid",&out_id);
			dced_util_printerr(st);
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
        	}
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


