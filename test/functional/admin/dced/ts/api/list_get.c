
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
 * $Log: list_get.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:41  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:50  root
 * 	Submit
 * 	[1995/12/11  14:33:17  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:00  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:40  melman]
 * 
 * Revision 1.1.2.4  1994/08/23  20:42:47  ganni
 * 	validate the output correctly in assertion #4
 * 	[1994/08/23  20:41:12  ganni]
 * 
 * Revision 1.1.2.3  1994/07/20  19:57:58  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:39:47  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.1  1994/05/18  21:21:42  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:01  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	list_get.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_list_get 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;

void dced_list_get_1(void);		/* Test purposes 1 */
void dced_list_get_2(void);		/* Test purposes 2 */
void dced_list_get_3(void);		/* Test purposes 3 */
void dced_list_get_4(void);		/* Test purposes 4 */
void dced_list_get_5(void);		/* Test purposes 5 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_list_get_1,  1 },
    { dced_list_get_2,  2 },
    { dced_list_get_3,  3 },
    { dced_list_get_4,  4 },
    { dced_list_get_5,  5 },
    { NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_list_get_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_list_get(). 
**
*/
void 
dced_list_get_1()
{
        server_t                server_entry;
	dced_entry_list_t	out_entry;
	boolean32		found;
	unsigned32		fail_stat, i;
        error_status_t          st;

	tet_infoline("dced_list_get_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_list_get, srvrconf entry list is returned.");

	fail_stat = false;

        test_srvrconf_init( &binding_handle, &server_entry, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	dced_list_get( binding_handle, &out_entry, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_list_get failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
		found=false;
		for (i=0; ((!found)&&(i<out_entry.count));i++)
		{
		    if (!uuid_compare(&(out_entry.list[i].id),
				&(server_entry.id), &st))
			found=true;
		}

		if (!found) 
		{
#ifdef DEBUG
		    tet_infoline("input server entry does not exist.");
		    tet_infoline("server entry info from dced_list_get:");
		    print_cfgentry_list( &out_entry );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else if (!test_cfgentry_verify( (dced_entry_t *)&server_entry, 
						&out_entry.list[i-1]))
		{
#ifdef DEBUG
		    tet_infoline("test_cfgentry_verify failed");
		    tet_infoline("server entry info from dced_list_get:");
		    print_cfgentry( &out_entry.list[i-1] );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}

		dced_list_release( binding_handle, &out_entry, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_list_release failed:");
       		    dced_util_printerr(st);
#endif
		    if (!fail_stat)
		    {
                	tet_result(TET_UNRESOLVED);
			fail_stat = true;
		    }
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
**		dced_list_get_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_list_get(). 
**
*/
void 
dced_list_get_2()
{
        server_t                server_entry;
	dced_entry_list_t	out_entry;
	uuid_t			srvr_id;
	boolean32		found;
	unsigned32		fail_stat, i;
        error_status_t          st;

	tet_infoline("dced_list_get_2, Assertion 02(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_list_get, srvrexec entry list is returned.");

	fail_stat = false;

        test_srvrexec_init( &binding_handle, &server_entry, &srvr_id, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	dced_list_get( binding_handle, &out_entry, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_list_get failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
		found=false;
		for (i=0; ((!found)&&(i<out_entry.count));i++)
		{
		    if (!uuid_compare(&(out_entry.list[i].id), &srvr_id, &st))
			found=true;
		}

		if (!found) 
		{
#ifdef DEBUG
		    tet_infoline("input server entry does not exist.");
		    tet_infoline("server entry info from dced_list_get:");
		    print_cfgentry_list( &out_entry );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else if (!test_cfgentry_verify( (dced_entry_t *)&server_entry, 
						&out_entry.list[i-1]))
		{
#ifdef DEBUG
		    tet_infoline("test_cfgentry_verify failed");
		    tet_infoline("server entry info from dced_list_get:");
		    print_cfgentry( &out_entry.list[i-1] );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}

		dced_list_release( binding_handle, &out_entry, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_list_release failed:");
       		    dced_util_printerr(st);
#endif
		    if (!fail_stat)
		    {
                	tet_result(TET_UNRESOLVED);
			fail_stat = true;
		    }
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
**		dced_list_get_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_list_get(). 
**
*/
void 
dced_list_get_3()
{
	dced_entry_t            host_entry;
	dced_attr_list_t        host_data;
	dced_entry_list_t	out_entry;
	boolean32		found;
	unsigned32		fail_stat, i;
        error_status_t          st;

	tet_infoline("dced_list_get_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_list_get, host configuration entry list is returned.");

	fail_stat = false;

	test_hostdata_init(&binding_handle, &host_entry, &host_data, 2, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

	dced_list_get( binding_handle, &out_entry, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_list_get failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
		found=false;
		for (i=0; ((!found)&&(i<out_entry.count));i++)
		{
		    if (!uuid_compare(&(out_entry.list[i].id),
				&(host_entry.id), &st))
			found=true;
		}

		if (!found) 
		{
#ifdef DEBUG
		    tet_infoline("input host entry does not exist.");
		    tet_infoline("host entry info from dced_list_get:");
		    print_cfgentry_list( &out_entry );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else if (!test_cfgentry_verify( &host_entry, 
						&out_entry.list[i-1]))
		{
#ifdef DEBUG
		    tet_infoline("test_cfgentry_verify failed");
		    tet_infoline("host entry info from dced_list_get:");
		    print_cfgentry_list( &out_entry );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}

		dced_list_release( binding_handle, &out_entry, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_list_release failed:");
       		    dced_util_printerr(st);
#endif
		    if (!fail_stat)
		    {
                	tet_result(TET_UNRESOLVED);
			fail_stat = true;
		    }
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
**		dced_list_get_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_list_get(). 
**
*/
void 
dced_list_get_4()
{
	dced_entry_t            key_entry;
	dced_key_list_t         key_list;
	dced_entry_list_t	out_entry;
	unsigned32		fail_stat, i;
	boolean32		found;
        error_status_t          st;

	tet_infoline("dced_list_get_4, Assertion 04(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_list_get, key configuration entry list is returned.");

	fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 1, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_list_get( binding_handle, &out_entry, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_list_get failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	}
	else
	{
		found=false;
		for (i=0; ((!found)&&(i<out_entry.count));i++)
		{
		    if (!uuid_compare(&(out_entry.list[i].id),
				&(key_entry.id), &st))
			found=true;
		}

		if (!found) 
		{
#ifdef DEBUG
		    tet_infoline("input key entry does not exist.");
		    tet_infoline("key entry info from dced_list_get:");
		    print_cfgentry_list( &out_entry );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else if (!test_cfgentry_verify( &key_entry, 
						&out_entry.list[i-1]))
		{
#ifdef DEBUG
		    tet_infoline("test_cfgentry_verify failed");
		    tet_infoline("key entry info from dced_list_get:");
		    print_cfgentry_list( &out_entry );
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}

		dced_list_release( binding_handle, &out_entry, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_list_release failed:");
       		    dced_util_printerr(st);
#endif
		    if (!fail_stat)
		    {
                	tet_result(TET_UNRESOLVED);
			fail_stat = true;
		    }
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

/*
** ROUTINE NAME:
**		dced_list_get_5()
**
** DESCRIPTION:
**		This routine verifies Assertion 05(A) for the function
**		dced_list_get(). 
**
*/
void 
dced_list_get_5()
{
	dced_entry_list_t	out_entry;
        error_status_t          st;

	tet_infoline("dced_list_get_5, Assertion 05(A):");
	tet_infoline(
	"When a secval dced binding handle is supplied to ");
	tet_infoline(
	"dced_list_get, the status dced_s_not_support is returned.");

        dced_util_binding_create( SECVAL_SVC, &binding_handle, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_list_get( binding_handle, &out_entry, &st);
	if (st != dced_s_no_support)
	{
#ifdef DEBUG
		tet_infoline("dced_list_get failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
        	dced_binding_free( binding_handle, &st);
		return;
	}

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
               	tet_result(TET_UNRESOLVED);
		return;
        }

	tet_result(TET_PASS);
	return;

}
