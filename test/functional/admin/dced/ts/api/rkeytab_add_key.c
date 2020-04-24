
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
 * $Log: rkeytab_add_key.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:06  root
 * 	Submit
 * 	[1995/12/11  14:33:21  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:06  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:45  melman]
 * 
 * Revision 1.1.2.2  1994/06/17  18:45:02  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:33:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:50  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:14  ganni]
 * 
 * $EndLog$
 */

/*
** Module: keytab_add_key.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the tests for dced api 
**	     dced_keytab_add_key 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            key_entry;
dced_key_list_t         key_list;

void dced_keytab_add_key_1();          /* Test purposes 1 */

void (*tet_startup)() = NULL;   /* Load the start routine into TET */
void (*tet_cleanup)() = NULL;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_keytab_add_key_1,  1 },
    { (void (*)())NULL, 0 }
};


/*
** ROUTINE NAME:
**		dced_keytab_add_key_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_keytab_add_key(). 
**
*/
void 
dced_keytab_add_key_1()
{
	dced_key_t		key_info;
        unsigned32              fail_stat;
	error_status_t		st;

	tet_infoline("dced_keytab_add_key_1, Assertion 01(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_keytab_add_key along with the key_table information");
	tet_infoline(
	"and a list of keys, the status error_status_ok is returned.");

        fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 0, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        key_info.passwd	= (sec_passwd_rec_t *)malloc( sizeof(sec_passwd_rec_t));
	test_key_init( 1, &key_info );
        dced_keytab_add_key( binding_handle, &key_entry.id, &key_info, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_add_key failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }

	/*
	 * try to add the same again - should return with
	 * the status sec_key_mgmt_e_key_version_ex
	 */
	test_key_init( 1, &key_info );
        dced_keytab_add_key( binding_handle, &key_entry.id, &key_info, &st);
        if (st != sec_key_mgmt_e_key_version_ex)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_add_key failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }

	free ( key_info.passwd );
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

