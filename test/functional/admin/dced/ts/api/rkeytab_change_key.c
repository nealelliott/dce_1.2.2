
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
 * $Log: rkeytab_change_key.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:08  root
 * 	Submit
 * 	[1995/12/11  14:33:22  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:07  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:46  melman]
 * 
 * Revision 1.1.2.4  1994/06/17  18:45:04  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:33:42  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/03  14:05:36  ganni
 * 	create the principal needed for the tests through scripts
 * 	[1994/06/03  14:03:18  ganni]
 * 
 * Revision 1.1.2.2  1994/05/25  17:13:07  ganni
 * 	reset the key after the first change_key
 * 	[1994/05/25  17:12:01  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:52  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:17  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	rkeytab_change_key.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_keytab_change_key 
**
*/

#include <dtapi_impl.h>
#include <dce/keymgmt.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            key_entry;
dced_key_list_t         key_list;

void setup();
void cleanup();
void dced_keytab_change_key_1();          /* Test purposes 1 */

void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_keytab_change_key_1,  1 },
    { (void (*)())NULL, 0 }
};

void 
setup()
{
	unsigned32    status;
	char  add_princ_cmd[250];

	sprintf( add_princ_cmd, 
		 "dce_login %s %s -exec ./key_setup.sh %s", 
		 CELL_ADMIN, CELL_ADMIN_PWD, CELL_ADMIN_PWD);
	status = system ( add_princ_cmd );
	tet_infoline ( "create_principal_account_key: Done" );
}


void
cleanup()
{
}

/*
** ROUTINE NAME:
**		dced_keytab_change_key_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_keytab_change_key(). 
**
*/
void 
dced_keytab_change_key_1()
{
	dced_key_t		key_info, nkey_info;
        unsigned32              fail_stat;
	error_status_t		st;
	unsigned char		*raw_pw;
	unsigned32		len, curr_kv, next_kv;
	sec_passwd_type_t	key_type;

	tet_infoline("dced_keytab_change_key_1, Assertion 01(A):");
	tet_infoline(
	"When a keytab dced binding handle is supplied to ");
	tet_infoline(
	"dced_keytab_change_key along with the key information to be");
	tet_infoline(
	"updated, the modifications are updated to the specified key table");
	tet_infoline(
	"and the status error_status_ok is returned.");

        fail_stat = false;

        test_keytab_init( &binding_handle, &key_entry, &key_list, 0, &st);
        if (st != error_status_ok)
        {
                tet_result(TET_UNRESOLVED);
                return;
        }

        key_info.passwd	= 
		(sec_passwd_rec_t *)malloc( sizeof(sec_passwd_rec_t));
	test_key_init( 1, &key_info );

	/* 
	 * get the vesion number of the key, so that the 
	 * correct key info can be added in the local file.
	 */
	key_type = sec_passwd_plain;
	sec_key_mgmt_get_next_kvno( rpc_c_authn_dce_secret, NULL, 
				key_info.principal,
				&key_type, &curr_kv, &next_kv, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("sec_key_mgmt_get_next_kvno failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_UNRESOLVED);
                fail_stat = true;
        }

	if (!fail_stat)
	{
        	key_info.version = (key_info.passwd)->version_number  = curr_kv;
        	dced_keytab_add_key(	binding_handle, &key_entry.id, 
					&key_info, &st);
		if (st != error_status_ok)
        	{
#ifdef DEBUG
                    tet_infoline("dced_keytab_add_key failed:");
                    dced_util_printerr(st);
#endif
                    tet_result(TET_UNRESOLVED);
                    fail_stat = true;

		}

		/*
		 * change the registry with the new key and the next version
		 */
        	nkey_info.passwd	= 
			(sec_passwd_rec_t *)malloc( sizeof(sec_passwd_rec_t));
		test_key_init( 1, &nkey_info );
        	nkey_info.version = (nkey_info.passwd)->version_number  = 0;
        	len = strlen((char *)DCED_KEY_PASSWD_2)+1;
        	(nkey_info.passwd)->key.tagged_union.plain = 
						raw_pw = malloc(len);
		memcpy (raw_pw, DCED_KEY_PASSWD_2, len);
        	raw_pw[len]='\0';

        	dced_keytab_change_key( binding_handle, &key_entry.id, 
					&nkey_info, &st);
		if (st != error_status_ok)
        	{
#ifdef DEBUG
                    tet_infoline("dced_keytab_change_key failed:");
                    dced_util_printerr(st);
#endif
                    tet_result(TET_FAIL);
                    fail_stat = true;
        	}
        	free( nkey_info.passwd );

		/*
		 * reset the key info to the old key with the next version
		 */
        	key_info.version = (key_info.passwd)->version_number  = 0;
        	dced_keytab_change_key( binding_handle, &key_entry.id, 
					&key_info, &st);
        	if (st != error_status_ok)
        	{
#ifdef DEBUG
                    tet_infoline("dced_keytab_change_key (reset) failed:");
                    dced_util_printerr(st);
#endif
                    tet_result(TET_FAIL);
                    fail_stat = true;
        	}
	}

        free( key_info.passwd );
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

