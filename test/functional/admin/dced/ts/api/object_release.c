
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
 * $Log: object_release.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:48  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:03  root
 * 	Submit
 * 	[1995/12/11  14:33:20  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:05  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:44  melman]
 * 
 * Revision 1.1.2.3  1994/07/20  19:58:02  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:40:39  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.1  1994/05/18  21:21:49  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:11  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	object_release.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_object_release 
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
server_t                server_entry;

void dced_object_release_1(void);          /* Test purposes 1 */
void dced_object_release_2(void);          /* Test purposes 2 */
void dced_object_release_3(void);          /* Test purposes 3 */
void dced_object_release_4(void);          /* Test purposes 4 */

void (*tet_startup)(void) = NULL;   /* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_object_release_1,  1 },
    { dced_object_release_2,  2 },
    { dced_object_release_3,  3 },
    { dced_object_release_4,  4 },
    { NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_object_release_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_object_release(). 
**
*/
void 
dced_object_release_1()
{
        server_t		*out_data;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_release_1, Assertion 01(A):");
	tet_infoline(
	"When srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_release along with the list of the associated");
	tet_infoline(
	"objects (server_t) obtained from dced_object_read or");
	tet_infoline(
	"dced_object_read_all and the count of the objects,");
	tet_infoline(
	"the memory and the resources allocated are freed.");	

        fail_stat = false;

        test_srvrconf_init( &binding_handle, &server_entry, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read( binding_handle, &server_entry.id,
			(void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read (srvrconf) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		fail_stat = true;
	}
	else
	{

		dced_objects_release(binding_handle, 1, (void *)out_data, &st);
        	if (st != error_status_ok)
        	{
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
**		dced_object_release_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_object_release(). 
**
*/
void 
dced_object_release_2()
{

        server_t		*out_data;
	uuid_t			srvr_id;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_release_2, Assertion 02(A):");
	tet_infoline(
	"When srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_release along with the list of the associated");
	tet_infoline(
	"objects (server_t) obtained from dced_object_read or");
	tet_infoline(
	"dced_object_read_all and the count of the objects,");
	tet_infoline(
	"the memory and the resources allocated are freed.");	

        fail_stat = false;

        test_srvrexec_init( &binding_handle, &server_entry, &srvr_id, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read( binding_handle, &srvr_id, (void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read (srvrexec) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		fail_stat = true;
	}
	else
	{

		dced_objects_release(binding_handle, 1, (void *)out_data, &st);
        	if (st != error_status_ok)
        	{
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
**		dced_object_release_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_object_release(). 
**
*/
void 
dced_object_release_3()
{
        sec_attr_t              *out_data;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_release_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_release along with the list of the associated");
	tet_infoline(
	"objects (sec_attr_t) obtained from dced_object_read or");
	tet_infoline(
	"dced_object_read_all and the count of the objects,");
	tet_infoline(
	"the memory and the resources allocated are freed.");	

        fail_stat = false;

        test_hostdata_init( &binding_handle, &host_entry, &host_data, 2, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read( binding_handle, &host_entry.id,
			(void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read (hostdata) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		fail_stat = true;
	}
	else
	{

		dced_objects_release(binding_handle, 1, (void *)out_data, &st);
        	if (st != error_status_ok)
        	{
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

/*
** ROUTINE NAME:
**		dced_object_release_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_object_release(). 
**
*/
void 
dced_object_release_4()
{
        dced_key_list_t         *out_data;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_release_4, Assertion 04(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_release along with the list of the associated");
	tet_infoline(
	"objects (dced_key_list_t) obtained from dced_object_read or");
	tet_infoline(
	"dced_object_read_all and the count of the objects,");
	tet_infoline(
	"the memory and the resources allocated are freed.");	

        fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 3, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read( binding_handle, &key_entry.id, 
			(void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read (key) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		fail_stat = true;
	}
	else
	{
		dced_objects_release(binding_handle, 1, (void *)out_data, &st);
        	if (st != error_status_ok)
                {
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

