
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
 * $Log: entry_get_next.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:18:24  root
 * 	Submit
 * 	[1995/12/11  14:33:08  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:24  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:49  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:28  melman]
 * 
 * Revision 1.1.2.5  1994/08/23  20:42:46  ganni
 * 	validate the output correctly in assertion #4
 * 	[1994/08/23  20:40:33  ganni]
 * 
 * Revision 1.1.2.4  1994/07/20  19:57:51  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:38:32  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.2  1994/06/03  16:03:08  ganni
 * 	args to dced_release_cursor changed
 * 	[1994/06/03  16:01:15  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:26  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:31  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	entry_get_next.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_entry_get_next 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;

void dced_entry_get_next_1(void);		/* Test purposes 1 */
void dced_entry_get_next_2(void);		/* Test purposes 2 */
void dced_entry_get_next_3(void);		/* Test purposes 3 */
void dced_entry_get_next_4(void);		/* Test purposes 4 */
void dced_entry_get_next_5(void);		/* Test purposes 5 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_entry_get_next_1,  1 },
    { dced_entry_get_next_2,  2 },
    { dced_entry_get_next_3,  3 },
    { dced_entry_get_next_4,  4 },
    { dced_entry_get_next_5,  5 },
    { NULL, 0 }
};


/*
** ROUTINE NAME:
**		dced_entry_get_next_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_entry_get_next(). 
**
*/
void 
dced_entry_get_next_1()
{
	server_t		server_entry;
	dced_entry_t		*out_entry_ptr;
	dced_cursor_t		cursor;
	unsigned32		fail_stat;
	boolean32		found;
        error_status_t          st;

	tet_infoline("dced_entry_get_next_1, Assertion 01(A):");
	tet_infoline(
	"When a srvrconf dced binding handle is supplied to ");
	tet_infoline(
	"dced_entry_get_next, srvrconf entry list is returned.");

	fail_stat = false;

        test_srvrconf_init( &binding_handle, &server_entry, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_initialize_cursor(
                                binding_handle,
                                &cursor,
                                &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_initialize_cursor failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }
	else
	{
	    found=false;
	    while ( !(found) )
	    {
		dced_entry_get_next( cursor, &out_entry_ptr, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    if (st != dced_s_no_more_entries)
		    {
		    	tet_infoline("dced_entry_get_next failed:");
		    	dced_util_printerr(st);
		    }
		    else
		    	tet_infoline("input server entry does not exist");
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		    break;
		}
		if (!uuid_compare(&(out_entry_ptr->id), &(server_entry.id), &st))
		    found=true;
	    }

	    if ((found) && 
		!test_cfgentry_verify( (dced_entry_t *)&server_entry, 
					out_entry_ptr))
	    {
#ifdef DEBUG
		tet_infoline("test_cfgentry_verify failed");
		tet_infoline("server entry info from dced_entry_get_next:");
		print_cfgentry( out_entry_ptr );
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	    }

	    dced_release_cursor( &cursor, &st);
	    if (st != error_status_ok)
	    {
#ifdef DEBUG
		tet_infoline("dced_release_cursor failed:");
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
**		dced_entry_get_next_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_entry_get_next(). 
**
*/
void 
dced_entry_get_next_2()
{
	server_t		server_entry;
	uuid_t			srvr_id;
	dced_entry_t		*out_entry_ptr;
	dced_cursor_t		cursor;
	unsigned32		fail_stat;
	boolean32		found;
        error_status_t          st;

	tet_infoline("dced_entry_get_next_2, Assertion 02(A):");
	tet_infoline(
	"When a srvrexec dced binding handle is supplied to ");
	tet_infoline(
	"dced_entry_get_next, srvrexec entry list is returned.");

	fail_stat = false;

        test_srvrexec_init( &binding_handle, &server_entry, &srvr_id, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_initialize_cursor(
                                binding_handle,
                                &cursor,
                                &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_initialize_cursor failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }
	else
	{
	    found=false;
	    while ( !(found) )
	    {
		dced_entry_get_next( cursor, &out_entry_ptr, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    if (st != dced_s_no_more_entries)
		    {
		    	tet_infoline("dced_entry_get_next failed:");
		    	dced_util_printerr(st);
		    }
		    else
		    	tet_infoline("input server entry does not exist");
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		    break;
		}
		if (!uuid_compare(&(out_entry_ptr->id), &srvr_id, &st))
		    found=true;
	    }

	    if ((found) && 
		!test_cfgentry_verify( (dced_entry_t *)&server_entry, 
					out_entry_ptr))
	    {
#ifdef DEBUG
		tet_infoline("test_cfgentry_verify failed");
		tet_infoline("server entry info from dced_entry_get_next:");
		print_cfgentry( out_entry_ptr );
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	    }

	    dced_release_cursor( &cursor, &st);
	    if (st != error_status_ok)
	    {
#ifdef DEBUG
		tet_infoline("dced_release_cursor failed:");
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
**		dced_entry_get_next_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_entry_get_next(). 
**
*/
void 
dced_entry_get_next_3()
{
	dced_entry_t            host_entry;
	dced_attr_list_t        host_data;
	dced_entry_t		*out_entry_ptr;
	dced_cursor_t		cursor;
	unsigned32		fail_stat;
	boolean32		found;
        error_status_t          st;

	tet_infoline("dced_entry_get_next_3, Assertion 03(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_entry_get_next, host configuration entry list is returned.");

	fail_stat = false;

	test_hostdata_init( &binding_handle, &host_entry, &host_data, 2, &st);
	if (st != error_status_ok)
	{
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_initialize_cursor(
                                binding_handle,
                                &cursor,
                                &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_initialize_cursor failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }
	else
	{
	    found=false;
	    while ( !(found) )
	    {
		dced_entry_get_next( cursor, &out_entry_ptr, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    if (st != dced_s_no_more_entries)
		    {
		    	tet_infoline("dced_entry_get_next failed:");
		    	dced_util_printerr(st);
		    }
		    else
		    	tet_infoline("input host entry does not exist");
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		    break;
		}
		if (!uuid_compare(&(out_entry_ptr->id), &(host_entry.id), &st))
		    found=true;
	    }

	    if ((found) && !test_cfgentry_verify( &host_entry, out_entry_ptr))
	    {
#ifdef DEBUG
		tet_infoline("test_cfgentry_verify failed");
		tet_infoline("host entry info from dced_entry_get_next:");
		print_cfgentry( out_entry_ptr );
#endif
		tet_result(TET_FAIL);
		fail_stat = true;
	    }

	    dced_release_cursor( &cursor, &st);
	    if (st != error_status_ok)
	    {
#ifdef DEBUG
		tet_infoline("dced_release_cursor failed:");
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

/*
** ROUTINE NAME:
**		dced_entry_get_next_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_entry_get_next(). 
**
*/
void 
dced_entry_get_next_4()
{
        dced_entry_t            key_entry;
        dced_key_list_t         key_list;
	dced_entry_t		*out_entry_ptr;
	dced_cursor_t		cursor;
	unsigned32		fail_stat;
	int			i;
	boolean32		found;
        error_status_t          st;

	tet_infoline("dced_entry_get_next_4, Assertion 04(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_entry_get_next, keytab configuration entry list is returned.");

	fail_stat = false;

	test_keytab_init( &binding_handle, &key_entry, &key_list, 1, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_initialize_cursor(
                                binding_handle,
                                &cursor,
                                &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_initialize_cursor failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }
	else
	{
	    found=false;
	    while ( !(found) )
	    {
		dced_entry_get_next( cursor, &out_entry_ptr, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    if (st != dced_s_no_more_entries)
		    {
		    	tet_infoline("dced_entry_get_next failed:");
		    	dced_util_printerr(st);
		    }
		    else
		    	tet_infoline("input key entry does not exist");
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		    break;
		}
		if (!uuid_compare(&(out_entry_ptr->id), &(key_entry.id), &st))
		    found=true;

	    }
	    if ((found) && !test_cfgentry_verify( &key_entry, out_entry_ptr))
            {
#ifdef DEBUG
		tet_infoline("test_cfgentry_verify failed");
                tet_infoline("key entry info from dced_entry_get_next:");
                print_cfgentry( out_entry_ptr );
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
            }

            dced_release_cursor( &cursor, &st);
            if (st != error_status_ok)
            {
#ifdef DEBUG
                tet_infoline("dced_release_cursor failed:");
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

/*
** ROUTINE NAME:
**		dced_entry_get_next_5()
**
** DESCRIPTION:
**		This routine verifies Assertion 05(A) for the function
**		dced_entry_get_next(). 
**
*/
void 
dced_entry_get_next_5()
{
	dced_cursor_t		cursor;
        error_status_t          st;

	tet_infoline("dced_entry_get_next_5, Assertion 05(A):");
	tet_infoline(
	"When a secval dced binding handle is supplied to ");
	tet_infoline(
        "dced_initialize_cursor, the status dced_s_not_support is returned.");

        dced_util_binding_create( SECVAL_SVC, &binding_handle, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_initialize_cursor(
                                binding_handle,
                                &cursor,
                                &st);
        if (st != dced_s_no_support)
        {
#ifdef DEBUG
                tet_infoline("dced_initialize_cursor failed:");
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
