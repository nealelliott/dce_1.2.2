
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
 * $Log: object_read_all.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:47  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:00  root
 * 	Submit
 * 	[1995/12/11  14:33:20  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:04  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:43  melman]
 * 
 * Revision 1.1.2.5  1994/09/16  20:10:52  ganni
 * 	enhancements and cleanup of srvrexec tests
 * 	[1994/09/16  20:09:56  ganni]
 * 
 * Revision 1.1.2.4  1994/08/23  20:42:48  ganni
 * 	Validate the output correctly in assertion #4
 * 	[1994/08/23  20:41:44  ganni]
 * 
 * Revision 1.1.2.3  1994/07/20  19:58:01  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:40:28  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.1  1994/05/18  21:21:47  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:08  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	object_read_all.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_object_read_all 
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

void dced_object_read_all_1(void);          /* Test purposes 1 */
void dced_object_read_all_2(void);          /* Test purposes 2 */
void dced_object_read_all_3(void);          /* Test purposes 3 */
void dced_object_read_all_4(void);          /* Test purposes 4 */

void (*tet_startup)(void) = NULL;   /* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_object_read_all_1,  1 },
    { dced_object_read_all_2,  2 },
    { dced_object_read_all_3,  3 },
    { dced_object_read_all_4,  4 },
    { NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_object_read_all_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_object_read_all(). 
**
*/
void 
dced_object_read_all_1()
{
        server_t		*out_data;
        unsigned32              count,i;
        boolean32               found;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_read_all_1, Assertion 01(A):");
	tet_infoline(
	"When srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_read_all, a list of objects (server_t)");
	tet_infoline(
	"is returned along with a count of the objects.");

        fail_stat = false;

        test_srvrconf_init( &binding_handle, &server_entry, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read_all( binding_handle, &count, (void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read_all (srvrconf) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
	    found=false;

	    for (i=0; ((!found)&&(i<count));i++)
	    {
                if (!uuid_compare( &(out_data[i].id), 
					&(server_entry.id), &st))
                    found=true;

	    }

            if (!found)
            {
#ifdef DEBUG
                tet_infoline("input server entry does not exist.");
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
	    }

	    dced_objects_release(binding_handle, count, (void *)out_data, &st);
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
**		dced_object_read_all_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_object_read_all(). 
**
*/
void 
dced_object_read_all_2()
{
        server_t		*out_data;
        uuid_t			srvr_id;
        unsigned32              count,i;
        boolean32               found;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_read_all_2, Assertion 02(A):");
	tet_infoline(
	"When srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_read_all, a list of objects (server_t)");
	tet_infoline(
	"is returned along with a count of the objects.");

        fail_stat = false;

        test_srvrexec_init( &binding_handle, &server_entry, &srvr_id, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read_all( binding_handle, &count, (void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read_all (srvrexec) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
	    found=false;

	    for (i=0; ((!found)&&(i<count));i++)
	    {

                if (!uuid_compare( &(out_data[i].id), &srvr_id, &st))
                    found=true;
	    }

            if (!found)
            {
#ifdef DEBUG
                tet_infoline("input server entry does not exist.");
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
	    }

	    dced_objects_release(binding_handle, count, (void *)out_data, &st);
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
**		dced_object_read_all_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_object_read_all(). 
**
*/
void 
dced_object_read_all_3()
{
        sec_attr_t              *out_data;
        unsigned32              count,i;
        boolean32               found;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_read_all_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_read_all, a list of objects (sec_attr_t)");
	tet_infoline(
	"is returned along with a count of the objects.");

        fail_stat = false;

/* ??? need to enter with two different object id's */

        test_hostdata_init( &binding_handle, &host_entry, &host_data, 2, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read_all( binding_handle, &count, (void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read_all (hostdata) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
	    found=false;
	    for (i=0; ((!found)&&(i<count));i++)
	    {
		if (test_hostattr_verify( host_data.list, &out_data[i]))
		    found=true;
	    }
            if (!found)
            {
#ifdef DEBUG
                tet_infoline("input host attributes does not exist.");
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
	    }

	    dced_objects_release(binding_handle, count, (void *)out_data, &st);
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
**		dced_object_read_all_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_object_read_all(). 
**
*/
void 
dced_object_read_all_4()
{
        dced_key_list_t         *out_data;
        unsigned32              count,i;
        boolean32               found;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_object_read_all_4, Assertion 04(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_object_read_all, a list of objects (dced_key_list_t)");
	tet_infoline(
	"is returned along with a count of the objects.");

        fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 3, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_object_read_all( binding_handle, &count, (void **)&out_data, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_object_read_all (key) failed:");
		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{

	    found=false;
	    for (i=0; ((!found)&&(i<count));i++)
	    {
		if (test_keylist_verify( &key_list, &out_data[i]))
		    found=true;
		
	    }
            if (!found)
            {
#ifdef DEBUG
                tet_infoline("input key entries do not exist.");
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
	    }

	    dced_objects_release( binding_handle, count, 
				  (void *)out_data, &st);
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

