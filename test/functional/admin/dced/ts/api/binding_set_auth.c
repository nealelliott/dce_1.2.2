
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
 * $Log: binding_set_auth.c,v $
 * Revision 1.1.6.2  1996/11/14  16:10:32  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  14:26:22  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/10/18  17:08 UTC  bartf
 * 	fix dced_hostdata_create err status
 * 
 * Revision /main/DCE_1.2.2/1  1996/08/15  22:22 UTC  mullan_s
 * 	Submit fix for CHFts19700
 * 
 * Revision /main/mullan_dce122_bugs_1/1  1996/08/14  19:45 UTC  mullan_s
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:22  marty]
 * 	 *
 * 
 * 	Submit
 * 	[1995/12/11  14:33:02  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:14  root]
 * 
 * Revision /main/HPDCE02/1  1996/01/15  23:10 UTC  jss
 * 	Merge in all changes to dced tests for new ACLs.
 * 
 * Revision /main/jss_dced_acl/1  1996/01/15  22:55 UTC  jss
 * 	Remove unecessary acl_edit's on hostdata object. hostdata object default acls are
 * 	sufficient.
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:39  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:19  melman]
 * 
 * Revision 1.1.2.5  1994/09/14  15:42:21  ganni
 * 	delete the hostdata entry, if created with an unauthenticated handle
 * 	[1994/09/14  15:41:36  ganni]
 * 
 * Revision 1.1.2.4  1994/07/20  19:57:40  ganni
 * 	Permissions to hostdata service need to be modified.
 * 	[1994/07/20  19:36:24  ganni]
 * 
 * Revision 1.1.2.3  1994/06/17  18:44:45  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:32:23  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/07  19:52:06  ganni
 * 	use the protect level _connect while authenticating
 * 	[1994/06/07  19:39:20  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:19  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:21  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	binding_set_auth.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_binding_set_auth_info 
**
*/

#include <dtapi_impl.h>


/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            host_entry;
dced_attr_list_t        host_data;
char			*service_name;

void setup(void);
void cleanup(void);
void dced_binding_set_auth_info_1(void);		/* Test purposes 1 */

void (*tet_startup)(void) = setup;   /* Load the start routine into TET */
void (*tet_cleanup)(void) = cleanup;   /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_binding_set_auth_info_1,  1 },
    { NULL, 0 }
};

void
setup()
{
        unsigned32	status;
        char		acl_mod_cmd[250];

        test_format_3_name( HOST_SVC, &service_name);

        sprintf( acl_mod_cmd,
                 "dce_login %s %s -exec ./hacl_setup.sh %s",
                 CELL_ADMIN, CELL_ADMIN_PWD, service_name);
/*         status = system ( acl_mod_cmd ); */
        tet_infoline ( "acl_setup_host_svc: Done" );
}


void
cleanup()
{
        unsigned32	status;
        char		acl_mod_cmd[250];

        sprintf( acl_mod_cmd,
                 "dce_login %s %s -exec ./hacl_reset.sh %s",
                 CELL_ADMIN, CELL_ADMIN_PWD, service_name);
/*         status = system ( acl_mod_cmd ); */
        tet_infoline ( "acl_reset_host_svc: Done" );
}


/*
** ROUTINE NAME:
**		dced_binding_set_auth_info_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_binding_set_auth_info(). 
**
*/
void 
dced_binding_set_auth_info_1()
{
        error_status_t          st, tmp_st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_set_auth_info_1, Assertion 01(A):");
	tet_infoline(
	"When hostdata dced binding handle is supplied");
	tet_infoline(
	"to dced_binding_set_auth_info along with ");
	tet_infoline(
	"\tprotect_level = rpc_c_protect_level_connect,");
	tet_infoline(
	"\tauthn_svc = rpc_c_authn_dce_private,");
	tet_infoline(
	"\tauthn_identity = NULL,");
	tet_infoline(
	"\tauthz_svc = rpc_c_authz_dce");
	tet_infoline(
	"the binding handle is set to make authenticated calls.");

        dced_binding_create( HOST_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
		tet_result(TET_UNRESOLVED);
                return;
        }

        test_hostentry_init(&host_entry, &st);
        test_hostattr_init(&host_data, 2, &st);

	/*
	** Try to create a hostdata entry with an unauthenticated
	** binding handle. Should return with sec_acl_not_authorized.
	*/
        dced_hostdata_create( binding_handle, &host_entry, &host_data, &st);
        if (st != sec_acl_not_authorized)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_create (unauthenticated) failed:");
		tet_infoline("status should be:");
                dced_util_printerr(sec_acl_not_authorized);
		tet_infoline("but received:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		if (st == error_status_ok)
        	    dced_hostdata_delete( binding_handle, &host_entry.id, &st);

        	dced_binding_free( binding_handle, &st);
                return;
        }

        dced_binding_set_auth_info(binding_handle,
                                  rpc_c_protect_level_connect,
                                  rpc_c_authn_dce_private,
                                  NULL,
                                  rpc_c_authz_dce,
                                  &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_set_auth_info failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
        	dced_binding_free( binding_handle, &st);
                return;
        }

	/*
	** Now, create a hostdata entry with an authenticated
	** binding handle. Should succeed.
	*/
        dced_hostdata_create( binding_handle, &host_entry, &host_data, &st);
        test_hostattr_free(&host_data, &tmp_st);

        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_hostdata_create failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
        	dced_binding_free( binding_handle, &st);
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


