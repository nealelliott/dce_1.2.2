
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
 * $Log: entry_add.c,v $
 * Revision 1.1.6.1  1996/10/03  18:39:20  arvind
 * 	Submit fix for CHFts19700
 * 	[1996/08/14  19:45 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
 *
 * 	Merge in all changes to dced tests for new ACLs.
 * 	[1996/01/15  22:56 UTC  jss  /main/jss_dced_acl/1]
 *
 * 	Fix test.
 * 	[1994/09/28  21:47:48  melman  1.1.2.1]
 *
 * Revision 1.1.4.2  1996/02/18  00:30:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:29  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  15:18:21  root
 * 	Submit
 * 	[1995/12/11  14:33:08  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:22  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:48  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:27  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:48  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:32:36  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:25  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:27  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	entry_add.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_entry_add 
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

void dced_entry_add_1();          /* Test purposes 1 */
void dced_entry_add_2();          /* Test purposes 2 */

void (*tet_startup)() = NULL;   /* Load the start routine into TET */
void (*tet_cleanup)() = NULL;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_entry_add_1,  1 },
    { dced_entry_add_2,  2 },
    { (void (*)())NULL, 0 }
};


/*
** ROUTINE NAME:
**		dced_entry_add_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_entry_add(). 
**
*/
void 
dced_entry_add_1()
{
        sec_attr_t              *out_data;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_entry_add_1, Assertion 01(A):");
	tet_infoline(
	"When a hostdata dced binding handle is supplied to ");
	tet_infoline(
	"dced_entry_add along with the hostdata entry");
	tet_infoline(
	"the status error_status_ok is returned.");

        fail_stat = false;

        test_hostdata_init( &binding_handle, &host_entry, &host_data, 2, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

	dced_entry_remove( binding_handle, &host_entry.id, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
		fail_stat = true;
	}

	if (!fail_stat)
	{
		test_hostentry_init(&host_entry, &st);
		dced_entry_add( binding_handle, &host_entry, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_entry_add failed:");
       		    dced_util_printerr(st);
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else
		{
                    dced_object_read( binding_handle, &host_entry.id,
                                        (void **)&out_data, &st);
                    if (st != error_status_ok)
                    {
#ifdef DEBUG
                        tet_infoline("dced_object_read (key) failed:");
                        dced_util_printerr(st);
#endif
                        tet_result(TET_FAIL);
                        fail_stat = true;
                    }
		    else
		    {
		    	if (!test_hostattr_verify( host_data.list, out_data))
		    	{
#ifdef DEBUG
			    tet_infoline("test_hostattr_verify failed");
#endif
                            tet_result(TET_FAIL);
                            fail_stat = true;
		    	}
		    	dced_objects_release(	binding_handle, 1,
						(void *)out_data, &st);
		    }
		}
	}

        test_hostdata_entry_free( &binding_handle, &host_entry, &host_data, &st);
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
**		dced_entry_add_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_entry_add(). 
**
*/
void 
dced_entry_add_2()
{
        dced_key_list_t         *out_data;
        unsigned32              fail_stat;
        error_status_t          st;

	tet_infoline("dced_entry_add_2, Assertion 02(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_entry_add along with the keytab entry");
	tet_infoline(
	"the status error_status_ok is returned.");

        fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 3, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_entry_remove( binding_handle, &key_entry.id, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                fail_stat = true;;
        }       

        if (!fail_stat)
        {
		test_keyentry_init(&key_entry, &st);
		dced_entry_add( binding_handle, &key_entry, &st);
		if (st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_entry_add failed:");
       		    dced_util_printerr(st);
#endif
		    tet_result(TET_FAIL);
		    fail_stat = true;
		}
		else
		{
		    /*
		     * we should read successfully - otherwise 
		     * problem somewhere.
		     */
		    dced_object_read( binding_handle, &key_entry.id, 
					(void **)&out_data, &st);
		    if (st != error_status_ok)
		    {
#ifdef DEBUG
			tet_infoline("dced_object_read (key) failed:");
			dced_util_printerr(st);
#endif
			tet_result(TET_FAIL);
			fail_stat = true;
		    }
		    else
		    {
		    	if (!test_keylist_verify(&key_list, out_data))
		    	{
#ifdef DEBUG
			    tet_infoline("test_keylist_verify failed");
#endif
			    tet_result(TET_FAIL);
			    fail_stat = true;
			}
		    	dced_objects_release( 	binding_handle, 1,
						(void *)out_data, &st);
		    }

		}
	}

        test_keytab_entry_free( &binding_handle, &key_entry, &key_list, &st);
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

