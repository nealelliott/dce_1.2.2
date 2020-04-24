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
 * $Log: ns_codesets1.c,v $
 * Revision 1.1.4.2  1996/02/18  00:34:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:19:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:51:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:35  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:59  root]
 * 
 * Revision 1.1.2.3  1994/07/28  18:30:36  mori_m
 * 	CR 11294  Changed attr_type to a uuid_t* for codesets attribute ops.
 * 	[1994/07/28  18:25:54  mori_m]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:25  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:15  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:54:51  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:17:51  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <dce/rpc.h>
#include <dce/nsattrid.h>
#include <locale.h>
#include <tet_api.h>

#include "codesets.h"
#include "dummy_if.h"
#include "check_status.h"
#include "get_env.h"



static void tp1();

/*
 * Initialize TCM data structures
 */

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
struct tet_testlist tet_testlist[] = {
	{tp1, 1},
	{NULL, 0}
};



static void
tp1()
{
	rpc_ns_handle_t		inq_contxt;
	rpc_binding_vector_t	*binding_vector;
	rpc_codeset_mgmt_p_t 	arr;
	rpc_codeset_mgmt_p_t	out_arr;
	int			i;
	int			failed = FALSE;
	char			*nsi_entry_name;
	char			*locale_name;
	error_status_t		st;

	tet_infoline("test case (ns_mgmt_1)");

	GET_SERVER_ENTRY(nsi_entry_name);
	GET_CLIENT_LOCALE(locale_name);

	setlocale(LC_ALL, locale_name);

	/*
	 * Get supported code sets.
	 */
	rpc_rgy_get_codesets (
		&arr,
		&st );

	CHECK_STATUS(TRUE, "rpc_rgy_get_codesets", st, rpc_s_ok);

	rpc_server_register_if (
		dummy_if_v1_0_s_ifspec,
		NULL,
		NULL,
		&st );

	CHECK_STATUS(TRUE, "rpc_server_register_if", st, rpc_s_ok);

	rpc_server_use_all_protseqs (
		rpc_c_protseq_max_reqs_default,
		&st );

	CHECK_STATUS(TRUE, "rpc_server_use_all_protseqs", st, rpc_s_ok);
	
	rpc_server_inq_bindings (
		&binding_vector,
		&st );

	CHECK_STATUS(TRUE, "rpc_server_inq_bindings", st, rpc_s_ok);

	rpc_ns_binding_export (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		dummy_if_v1_0_s_ifspec,
		binding_vector,
		NULL,
		&st );

	CHECK_STATUS(TRUE, "rpc_ns_binding_export", st, rpc_s_ok);

	rpc_ep_register (
		dummy_if_v1_0_s_ifspec,
		binding_vector,
		NULL,
		NULL,
		&st );

	CHECK_STATUS(TRUE, "rpc_ep_register", st, rpc_s_ok);

	 rpc_ns_mgmt_set_attribute (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		rpc_c_attr_codesets,
		(void *)arr,
		&st );

	CHECK_STATUS(TRUE, "rpc_ns_mgmt_set_attribute", st, rpc_s_ok);

	 rpc_ns_mgmt_read_codesets (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		&out_arr,
		&st );

	CHECK_STATUS(TRUE, "rpc_ns_mgmt_read_codeset", st, rpc_s_ok);

	rpc_ns_mgmt_remove_attribute (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		rpc_c_attr_codesets,
		&st );

	CHECK_STATUS(TRUE, "rpc_ns_mgmt_remove_attribute", st, rpc_s_ok);

	rpc_ns_binding_unexport (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		dummy_if_v1_0_s_ifspec,
		(uuid_vector_p_t)NULL,
		&st );

	CHECK_STATUS(TRUE, "rpc_ns_binding_unexport", st, rpc_s_ok);

	rpc_ep_unregister (
		dummy_if_v1_0_s_ifspec,
		binding_vector,
		NULL,
		&st );

	CHECK_STATUS(TRUE, "rpc_ep_unregister", st, rpc_s_ok);

	rpc_binding_vector_free (
		&binding_vector,
		&st );

	CHECK_STATUS(TRUE, "rpc_binding_vector_free", st, rpc_s_ok);

	rpc_server_unregister_if (
		dummy_if_v1_0_s_ifspec,
		NULL,
		&st );

	CHECK_STATUS(TRUE, "rpc_server_unregister_if", st, rpc_s_ok);

		
	/* check the input and output are the same */
	if (out_arr->version != arr->version)
	{
		mtet_infoline("out_arr->version - found %d - expecting %d\n",
			i, out_arr->version, arr->version);
		failed = TRUE;
	}

	if (out_arr->count != arr->count)
	{
		mtet_infoline("out_arr->count - found %d - expecting %d\n",
			out_arr->count, arr->count);
		failed = TRUE;
	}

	for (i = 0; i < out_arr->count; i++)
	{
		if (out_arr->codesets[i].c_set != arr->codesets[i].c_set)
		{
			mtet_infoline("out_arr->codesets[%d].c_set - found %d - expecting %d\n",
				i, out_arr->codesets[i].c_set, arr->codesets[i].c_set);
			failed = TRUE;
		}

		if (out_arr->codesets[i].c_max_bytes != arr->codesets[i].c_max_bytes)
		{
			mtet_infoline("out_arr->codesets[%d].c_max_bytes - found %d - expecting %d\n",
				i, out_arr->codesets[i].c_max_bytes, arr->codesets[i].c_max_bytes);
			failed = TRUE;
		}

	}

	rpc_ns_mgmt_free_attr_data ((idl_byte **)&arr, &st);
	CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_attr_data", st, rpc_s_ok);

	rpc_ns_mgmt_free_attr_data ((idl_byte **)&out_arr, &st);
	CHECK_STATUS(TRUE, "rpc_ns_mgmt_free_attr_data", st, rpc_s_ok);

	if (failed)
		tet_result(TET_FAIL);
	else
		tet_result(TET_PASS);

	return;
}
