
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
 * $Log: hostdata_delete.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:36  root
 * 	Submit
 * 	[1995/12/11  14:33:12  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:54  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:33  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:51  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:32:52  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:33  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:46  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	hostdata_delete.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_hostdata_delete 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            host_entry;
dced_attr_list_t        host_data;

void setup();
void cleanup();
void dced_hostdata_delete_1();          /* Test purposes 1 */

void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_hostdata_delete_1,  1 },
    { (void (*)())NULL, 0 }
};

/*
** ROUTINE NAME:
**                    setup()
**
** DESCRIPTION:
**                    This routine performs necessary setup for all tests.
**
*/
void setup()
{
        error_status_t          st;

        dced_util_binding_create( HOST_SVC, &binding_handle, &st);
}

/*
** ROUTINE NAME:
**                    cleanup()
**
** DESCRIPTION:
**                    This routine performs necessary cleanup for all tests.
**
*/
void cleanup()
{
        error_status_t          st;

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
                return;
        }

}


/*
** ROUTINE NAME:
**		dced_hostdata_delete_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_hostdata_delete(). 
**
*/
void 
dced_hostdata_delete_1()
{
        error_status_t          st, tmp_st;
/*
        dced_attr_list_t        out_data;
*/
	tet_infoline("dced_hostdata_delete_1, Assertion 01(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_hostdata_delete along with the hostdata entry id");
	tet_infoline(
	"the hostdata entry is deleted and error_status_ok is returned.");

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

        dced_hostdata_delete( binding_handle, &host_entry.id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_delete failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
        }

/*
	try to read the entry and verify the status
	if st is success, delete did not succeed - assertion failed.
	at present dced_hostdata_read does not properly work on 
	non-existant entries.
	???
        dced_hostdata_read( binding_handle, &host_entry.id, &out_data, &st);
        if (st != xxx)
        {
#ifdef DEBUG
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                return;
        }
*/
	tet_result(TET_PASS);
	return;

}

