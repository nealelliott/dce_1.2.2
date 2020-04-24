
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
 * $Log: hostdata_write.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:40  root
 * 	Submit
 * 	[1995/12/11  14:33:13  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:56  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:35  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:53  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:33:01  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:37  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:51  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	hostdata_write.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_hostdata_write 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            host_entry;
dced_attr_list_t        host_data;

void dced_hostdata_write_1();          /* Test purposes 1 */

void (*tet_startup)() = NULL;   /* Load the start routine into TET */
void (*tet_cleanup)() = NULL;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_hostdata_write_1,  1 },
    { (void (*)())NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_hostdata_write_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_hostdata_write(). 
**
*/
void 
dced_hostdata_write_1()
{
	dced_attr_list_t        in_data;
        sec_attr_t              *out_data;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_hostdata_write_1, Assertion 01(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_hostdata_write along with the hostdata entry id");
	tet_infoline(
	"and hostdata data, the status error_status_ok is returned.");

        fail_stat = false;

        test_hostdata_init( &binding_handle, &host_entry, &host_data, 1, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	test_hostattr_init(&in_data, 2, &st);
	dced_hostdata_write( binding_handle, &host_entry.id, &in_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_hostdata_write failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                fail_stat = true;
	}

	if (!fail_stat)
	{
		dced_hostdata_read( binding_handle, &host_entry.id, &(host_data.list->attr_id), &out_data, &st);
		if (st != error_status_ok)
        	{
#ifdef DEBUG
               		tet_infoline("dced_hostdata_read failed:");
			dced_util_printerr(st);
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
		}
		else
		{
        		if (!test_hostattr_verify( in_data.list,out_data))
        		{
#ifdef DEBUG
			    tet_infoline("test_hostattr_verify failed");
#endif
                	    tet_result(TET_FAIL);
                	    fail_stat = true;
        		}

        		dced_objects_release( binding_handle, 1, 
						(void *)out_data, &st);
			if (st != error_status_ok)
        		{
#ifdef DEBUG
                	    tet_infoline("dced_objects_release failed:");
                	    dced_util_printerr(st);
#endif
                	    if (!fail_stat)
                	    {
                        	tet_result(TET_UNRESOLVED);
                        	fail_stat = true;
                	    }
        		}
		}
	}


	test_hostattr_free(&in_data, &st);

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

