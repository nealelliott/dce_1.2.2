
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
 * $Log: hostdata_create.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:33  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:33  root
 * 	Submit
 * 	[1995/12/11  14:33:11  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:53  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:32  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:50  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:32:48  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:32  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:43  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	hostdata_create.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_hostdata_create 
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
void dced_hostdata_create_1();          /* Test purposes 1 */

void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_hostdata_create_1,  1 },
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
**		dced_hostdata_create_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_hostdata_create(). 
**
*/
void 
dced_hostdata_create_1()
{
        error_status_t          st, tmp_st;

	tet_infoline("dced_hostdata_create_1, Assertion 01(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_hostdata_create along with the hostdata entry");
	tet_infoline(
	"and hostdata data, the status error_status_ok is returned.");

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
		tet_result(TET_FAIL);
		return;
	}

        dced_hostdata_delete( binding_handle, &host_entry.id, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_delete failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_UNRESOLVED);
		return;
        }

	tet_result(TET_PASS);
	return;

}

