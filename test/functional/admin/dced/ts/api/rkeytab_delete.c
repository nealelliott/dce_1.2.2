
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
 * $Log: rkeytab_delete.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:13  root
 * 	Submit
 * 	[1995/12/11  14:33:23  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:10  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:48  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:45:07  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:33:50  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:55  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:23  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	rkeytab_delete.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_keytab_delete 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            key_entry;
dced_key_list_t         key_list;

void setup();
void cleanup();
void dced_keytab_delete_1();          /* Test purposes 1 */

void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_keytab_delete_1,  1 },
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

        dced_util_binding_create( KEYTAB_SVC, &binding_handle, &st);

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
**		dced_keytab_delete_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_keytab_delete(). 
**
*/
void 
dced_keytab_delete_1()
{
        unsigned32              fail_stat;
	error_status_t		st;

	tet_infoline("dced_keytab_delete_1, Assertion 01(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_keytab_delete along with the key_table id");
	tet_infoline(
	"the status error_status_ok is returned.");

        fail_stat = false;

        /*
         * Initialize the key table information and
         * a key list with information for one key.
         */
        test_keyentry_init( &key_entry, &st);
	test_keylist_init( &key_list, 1, &st);

	dced_keytab_create( binding_handle, &key_entry, &key_list, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_keytab_create failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
                fail_stat = true;
	}
	else
	{
        	dced_keytab_delete( binding_handle, &key_entry.id, &st);
        	if (st != error_status_ok)
        	{
#ifdef DEBUG
                	tet_infoline("dced_keytab_delete failed:");
                	dced_util_printerr(st);
#endif
                	tet_result(TET_FAIL);
                	fail_stat = true;
		}
        }

	test_keylist_free( &key_list, &st);

	if (fail_stat)
		return;

	tet_result(TET_PASS);
	return;

}

