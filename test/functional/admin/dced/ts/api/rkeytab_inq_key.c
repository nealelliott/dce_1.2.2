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
 * $Log: rkeytab_inq_key.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:16  root
 * 	Submit
 * 	[1995/12/11  14:33:24  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:11  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:49  melman]
 * 
 * Revision 1.1.2.3  1994/06/17  18:45:09  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:33:55  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/03  16:03:09  ganni
 * 	args to dced_keytab_release_cursor changed
 * 	[1994/06/03  16:00:59  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:57  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:27  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	rkeytab_inq_key.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced apis
**		dced_keytab_initialize_cursor, dced_keytab_get_next_key,
**		dced_keytab_release_cursor.
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            key_entry;
dced_key_list_t         key_list;

void dced_keytab_inq_key_1();          /* Test purposes 1 */

void (*tet_startup)() = NULL;   /* Load the start routine into TET */
void (*tet_cleanup)() = NULL;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_keytab_inq_key_1,  1 },
    { (void (*)())NULL, 0 }
};


/*
** ROUTINE NAME:
**		dced_keytab_inq_key_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_keytab_change_key(). 
**
*/
void 
dced_keytab_inq_key_1()
{
	dced_key_t		key_info;
	dced_key_t		*key_out_ptr;
	dced_keytab_cursor_t    cursor;
        unsigned32              fail_stat;
	int			i;
	error_status_t		st;

	tet_infoline("dced_keytab_inq_key_1, Assertion 01(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_keytab_change_key along with the new key information");
	tet_infoline(
	"it is added to the specified key table and the status ");
	tet_infoline(
	"error_status_ok is returned.");

        fail_stat = false;
        key_info.passwd = 
		(sec_passwd_rec_t *)malloc( sizeof(sec_passwd_rec_t)) ;

        test_keytab_init(&binding_handle,
			&key_entry,
			&key_list,
			MAX_API_KEYS,
			&st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        dced_keytab_initialize_cursor( 
				binding_handle, 
				&key_entry.id, 
				&cursor, 
				&st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_initialize_cursor failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
                fail_stat = true;
        }

	/*
	 * do the next calls in a loop until all expected values are returned.
	 */
	i = 0;
	while (TRUE)
	{
		dced_keytab_get_next_key( 
					cursor, 
					&key_out_ptr, 
					&st);

	        if (st != error_status_ok)
        	{
		    if (i == MAX_API_KEYS)
		    {
			if (st == dced_s_no_more_entries)
			{
				tet_result(TET_PASS);
				break;
			}
		    }
#ifdef DEBUG
		    tet_infoline("dced_keytab_get_next_key failed");
		    tet_infoline("Expected dced_s_no_more_entry, but received:");
                    dced_util_printerr(st);
#endif
		    tet_result(TET_FAIL);
               	    fail_stat = true;
		    break;
		}

	        if (i < MAX_API_KEYS)
		{
                    test_key_init( i+1, &key_info );
		    if (!test_key_verify(&key_info, key_out_ptr))
		    {
#ifdef DEBUG
			tet_infoline("test_key_verify failed");
#endif

			tet_result(TET_FAIL);
               		fail_stat = true;
			break;
		    }
		}
		else
		{
		    tet_result(TET_FAIL);
               	    fail_stat = true;
		    break;
		}
		++i;
	}

        dced_keytab_release_cursor( &cursor, &st );
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_keytab_release_cursor failed:");
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

