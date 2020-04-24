
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
 * $Log: binding_fm_rpc.c,v $
 * Revision 1.1.4.2  1996/02/18  00:29:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:17:57  root
 * 	Submit
 * 	[1995/12/11  14:33:00  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:11  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:34  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:16  melman]
 * 
 * Revision 1.1.2.6  1994/09/09  19:06:48  ganni
 * 	verify if secval is activated
 * 	[1994/09/09  19:06:15  ganni]
 * 
 * Revision 1.1.2.5  1994/07/20  19:57:38  ganni
 * 	For keytab testing, the binding need to be authneticated
 * 	with pkt_privacy protection level.
 * 	[1994/07/20  19:34:38  ganni]
 * 
 * Revision 1.1.2.4  1994/06/17  18:44:43  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:32:14  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/07  19:52:04  ganni
 * 	use the protect level _connect while authenticating
 * 	[1994/06/07  19:39:18  ganni]
 * 
 * Revision 1.1.2.2  1994/06/03  16:03:06  ganni
 * 	remove the temp files
 * 	[1994/06/03  16:02:25  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:15  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:15  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	binding_fm_rpc.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_binding_from_rpc_binding 
**
*/

#include <dtapi_impl.h>
#include <unistd.h>


/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
dced_entry_t            host_entry;
dced_attr_list_t        host_data;
dced_entry_t            key_entry;
dced_key_list_t         key_list;


void dced_binding_from_rpc_binding_1(void);		/* Test purposes 1 */
void dced_binding_from_rpc_binding_2(void);		/* Test purposes 2 */
void dced_binding_from_rpc_binding_3(void);		/* Test purposes 3 */
void dced_binding_from_rpc_binding_4(void);		/* Test purposes 4 */
void dced_binding_from_rpc_binding_5(void);		/* Test purposes 5 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_binding_from_rpc_binding_1,  1 },
    { dced_binding_from_rpc_binding_2,  2 },
    { dced_binding_from_rpc_binding_3,  3 },
    { dced_binding_from_rpc_binding_4,  4 },
    { dced_binding_from_rpc_binding_5,  5 },
    { NULL, 0 }
};


/*
** ROUTINE NAME:
**		dced_binding_from_rpc_binding_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_binding_from_rpc_binding(). 
**
*/
void 
dced_binding_from_rpc_binding_1()
{
	unsigned_char_t		*string_binding;
        rpc_binding_handle_t	rpc_binding;
	rpc_protseq_vector_t	*protseq_vector;
        error_status_t          st, tmp_st;

	tet_infoline("dced_binding_from_rpc_binding_1, Assertion 01(A):");
	tet_infoline(
	"When srvrconf serivce name and rpc binding handle are ");
	tet_infoline(
	"supplied to dced_binding_from_rpc_binding, a dced binding handle");
	tet_infoline(
	"for dced srvrconf service in the local host is returned.");

	rpc_network_inq_protseqs (&protseq_vector,&st);
	if ((protseq_vector == NULL) || (st != rpc_s_ok))
	{
#ifdef DEBUG
		tet_infoline("rpc_network_inq_protseqs failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_string_binding_compose(NULL, 
				   (unsigned_char_p_t)protseq_vector->protseq[0], 
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"", 
				   &string_binding,
                                   &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_string_binding_compose failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_binding_from_string_binding(string_binding, &rpc_binding, &st);
	rpc_string_free(&string_binding, &tmp_st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_binding_from_string_binding failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_binding_from_rpc_binding(	dced_e_service_type_srvrconf,
					rpc_binding,
					&binding_handle, 
					&st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_from_rpc_binding failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

/* ??? we have to use this binding handle - code is not yet ready */

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

/*
** ROUTINE NAME:
**		dced_binding_from_rpc_binding_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_binding_from_rpc_binding(). 
**
*/
void 
dced_binding_from_rpc_binding_2()
{
	unsigned_char_t		*string_binding;
        rpc_binding_handle_t	rpc_binding;
        rpc_protseq_vector_t    *protseq_vector;
        error_status_t          st, tmp_st;

	tet_infoline("dced_binding_from_rpc_binding_2, Assertion 02(A):");
	tet_infoline(
	"When srvrexec serivce name and rpc binding handle are ");
	tet_infoline(
	"supplied to dced_binding_from_rpc_binding, a dced binding handle");
	tet_infoline(
	"for dced srvrexec service in the local host is returned.");

	rpc_network_inq_protseqs (&protseq_vector,&st);
	if ((protseq_vector == NULL) || (st != rpc_s_ok))
	{
#ifdef DEBUG
		tet_infoline("rpc_network_inq_protseqs failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}
	rpc_string_binding_compose(NULL, 
				   (unsigned_char_p_t)protseq_vector->protseq[0], 
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"", 
				   &string_binding,
                                   &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_string_binding_compose failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_binding_from_string_binding(string_binding, &rpc_binding, &st);
	rpc_string_free(&string_binding, &tmp_st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_binding_from_string_binding failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_binding_from_rpc_binding(	dced_e_service_type_srvrexec,
					rpc_binding,
					&binding_handle, 
					&st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_from_rpc_binding failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

/* ??? we have to use this binding handle - code is not yet ready */

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

/*
** ROUTINE NAME:
**		dced_binding_from_rpc_binding_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_binding_from_rpc_binding(). 
**
*/
void 
dced_binding_from_rpc_binding_3()
{
	unsigned_char_t		*string_binding;
        rpc_binding_handle_t	rpc_binding;
        rpc_protseq_vector_t    *protseq_vector;
        error_status_t          st, tmp_st;

	tet_infoline("dced_binding_from_rpc_binding_3, Assertion 03(A):");
	tet_infoline(
	"When hostdata serivce name and rpc binding handle are ");
	tet_infoline(
	"supplied to dced_binding_from_rpc_binding, a dced binding handle");
	tet_infoline(
	"for dced hostdata service in the local host is returned.");

	/* remove the temporart host file if exists */
        unlink((char * )DCED_HOST_FILE);

	rpc_network_inq_protseqs (&protseq_vector,&st);
	if ((protseq_vector == NULL) || (st != rpc_s_ok))
	{
#ifdef DEBUG
		tet_infoline("rpc_network_inq_protseqs failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}
	rpc_string_binding_compose(NULL, 
				   (unsigned_char_p_t)protseq_vector->protseq[0], 
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"", 
				   &string_binding,
                                   &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_string_binding_compose failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_binding_from_string_binding(string_binding, &rpc_binding, &st);
	rpc_string_free(&string_binding, &tmp_st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_binding_from_string_binding failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_binding_from_rpc_binding(	dced_e_service_type_hostdata,
					rpc_binding,
					&binding_handle, 
					&st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_from_rpc_binding failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        test_hostentry_init(&host_entry, &st);
        test_hostattr_init(&host_data, 2, &st);

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


/*
** ROUTINE NAME:
**		dced_binding_from_rpc_binding_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_binding_from_rpc_binding(). 
**
*/
void 
dced_binding_from_rpc_binding_4()
{
	unsigned_char_t		*string_binding;
        rpc_binding_handle_t	rpc_binding;
        rpc_protseq_vector_t    *protseq_vector;
        error_status_t          st, tmp_st;
        unsigned32              fail_stat;

	tet_infoline("dced_binding_from_rpc_binding_4, Assertion 04(A):");
	tet_infoline(
	"When keytab serivce name and rpc binding handle are ");
	tet_infoline(
	"supplied to dced_binding_from_rpc_binding, a dced binding handle");
	tet_infoline(
	"for dced keytab service in the local host is returned.");

        fail_stat = false;
	/* remove the temporart key file if exists */
        unlink((char * )DCED_KEY_FILE);

	rpc_network_inq_protseqs (&protseq_vector,&st);
	if ((protseq_vector == NULL) || (st != rpc_s_ok))
	{
#ifdef DEBUG
		tet_infoline("rpc_network_inq_protseqs failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}
	rpc_string_binding_compose(NULL, 
				   (unsigned_char_p_t)protseq_vector->protseq[0], 
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"", 
				   &string_binding,
                                   &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_string_binding_compose failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_binding_from_string_binding(string_binding, &rpc_binding, &st);
	rpc_string_free(&string_binding, &tmp_st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_binding_from_string_binding failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_binding_from_rpc_binding(	dced_e_service_type_keytab,
					rpc_binding,
					&binding_handle, 
					&st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_from_rpc_binding failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_set_auth_info(binding_handle,
                                  rpc_c_protect_level_pkt_privacy,
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
	** Now, create a keytab entry with an authenticated
	** binding handle. Should succeed.
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
                tet_result(TET_FAIL);
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
                    tet_result(TET_UNRESOLVED);
		    fail_stat = true;
        	}
	}

        test_keylist_free( &key_list, &st);
        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
                if (!fail_stat)
                {
                        tet_result(TET_UNRESOLVED);
                        fail_stat = true;
                }
        }

        if (fail_stat)
                return;

	tet_result(TET_PASS);
	return;

}


/*
** ROUTINE NAME:
**		dced_binding_from_rpc_binding_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 05(A) for the function
**		dced_binding_from_rpc_binding(). 
**
*/
void 
dced_binding_from_rpc_binding_5()
{
	unsigned_char_t		*string_binding;
        rpc_binding_handle_t	rpc_binding;
        rpc_protseq_vector_t    *protseq_vector;
        error_status_t          st, tmp_st;
        boolean32               sv_active= FALSE;

	tet_infoline("dced_binding_from_rpc_binding_5, Assertion 05(A):");
	tet_infoline(
	"When secval serivce name and rpc binding handle are ");
	tet_infoline(
	"supplied to dced_binding_from_rpc_binding, a dced binding handle");
	tet_infoline(
	"for dced secval service in the local host is returned.");

	rpc_network_inq_protseqs (&protseq_vector,&st);
	if ((protseq_vector == NULL) || (st != rpc_s_ok))
	{
#ifdef DEBUG
		tet_infoline("rpc_network_inq_protseqs failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_string_binding_compose(NULL, 
				   (unsigned_char_p_t)protseq_vector->protseq[0], 
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"",
				   (unsigned_char_p_t)"", 
				   &string_binding,
                                   &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_string_binding_compose failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	rpc_binding_from_string_binding(string_binding, &rpc_binding, &st);
	rpc_string_free(&string_binding, &tmp_st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("rpc_binding_from_string_binding failed");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

        dced_binding_from_rpc_binding(	dced_e_service_type_secval,
					rpc_binding,
					&binding_handle, 
					&st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_from_rpc_binding failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
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

        dced_secval_status( binding_handle, &sv_active, &st);
        if (sv_active)
                dced_secval_stop( binding_handle, &st);

        dced_secval_start( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_secval_start failed:");
                dced_util_printerr(st);
#endif
                tet_result(TET_FAIL);
        	dced_binding_free( binding_handle, &st);
		return;
        }
        else if (!sv_active)
        {
                dced_secval_stop( binding_handle, &st);
                if (st != error_status_ok)
                {
#ifdef DEBUG
                        tet_infoline("dced_secval_stop failed:");
                        dced_util_printerr(st);
#endif
                        tet_result(TET_UNRESOLVED);
        		dced_binding_free( binding_handle, &st);
			return;
                }
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



