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
 * $Log: server.c,v $
 * Revision 1.1.4.2  1996/02/18  00:32:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:18:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:49:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:40  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:00  root]
 * 
 * Revision 1.1.2.3  1994/07/28  18:30:35  mori_m
 * 	CR 11294  Changed attr_type to a uuid_t* for codesets attribute ops.
 * 	[1994/07/28  18:26:28  mori_m]
 * 
 * Revision 1.1.2.2  1994/06/10  20:43:28  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:35  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:57  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:28:47  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <dce/rpc.h>
#include <dce/nsattrid.h>
#include <locale.h>
#include <pthread.h>

#include "codesets.h"
#include "check_status2.h"
#include "get_env2.h"
#include "cs_test.h"


pthread_mutex_t	mutex;

boolean32 i18n_client_authorization
(
	rpc_binding_handle_t	client_binding,
	unsigned32		requested_mgmt_operation,
	error_status_t		*status
)
{
	*status = rpc_s_ok;
	return(TRUE);
}

int
main(int argc, char *argv[])
{
	error_status_t		status;
	int			i;
	rpc_ns_handle_t		inq_contxt;
	rpc_binding_vector_t	*binding_vector;
	rpc_codeset_mgmt_p_t 	arr;
	pthread_t		this_thread = pthread_self();
	sigset_t		sigset;
	char			*nsi_entry_name;
	char			*server_locale_name;
	error_status_t		expected = rpc_s_ok;
	int			server_pid;

	GET_SERVER_ENTRY2(nsi_entry_name);

	(void)pthread_mutex_init(&mutex, pthread_mutexattr_default);

	GET_SERVER_LOCALE2(server_locale_name);

	setlocale(LC_ALL, server_locale_name);

	/*
	 * Get supported code sets.
	 */
	rpc_rgy_get_codesets (
		&arr,
		&status );

	CHECK_STATUS2(TRUE, "rpc_rgy_get_codesets", status, expected);


	rpc_server_register_if (
		cs_test_v1_0_s_ifspec,
		NULL,
		NULL,
		&status );

	CHECK_STATUS2(TRUE, "rpc_server_register_if", status, expected);


	rpc_server_use_all_protseqs (
		rpc_c_protseq_max_reqs_default,
		&status );

	CHECK_STATUS2(TRUE, "rpc_server_use_all_protseqs", status, expected);

	
	rpc_server_inq_bindings (
		&binding_vector,
		&status );

	CHECK_STATUS2(TRUE, "rpc_server_inq_bindings", status, expected);


	rpc_ns_binding_export (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		cs_test_v1_0_s_ifspec,
		binding_vector,
		NULL,
		&status );

	CHECK_STATUS2(TRUE, "rpc_ns_binding_export", status, expected);


	rpc_ep_register (
		cs_test_v1_0_s_ifspec,
		binding_vector,
		NULL,
		NULL,
		&status );

	CHECK_STATUS2(TRUE, "rpc_ep_register", status, expected);


	 rpc_ns_mgmt_set_attribute (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		rpc_c_attr_codesets,
		(void *)arr,
		&status );

	CHECK_STATUS2(TRUE, "rpc_ns_mgmt_set_attribute", status, expected);


	rpc_ns_mgmt_free_codesets (&arr, &status);

	CHECK_STATUS2(TRUE, "rpc_ns_mgmt_free_codeset", status, expected);

	rpc_mgmt_set_authorization_fn((rpc_mgmt_authorization_fn_t)i18n_client_authorization, &status);

	CHECK_STATUS2(TRUE, "rpc_mgmt_set_authorization_fn", status, expected);

	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);

	if (pthread_signal_to_cancel_np(&sigset, &this_thread) != 0)
	{
		printf("pthread_signal_to_cancel_np failed\n");
		exit(1);
	}
	TRY
	{
		server_pid = getpid();

		printf("Listening for remote procedure calls...\n");

		rpc_server_listen (
			rpc_c_listen_max_calls_default,
			&status );

		CHECK_STATUS2(TRUE, "rpc_server_listen", status, expected);

		/* After the server returned, clear up the name space */

		rpc_ns_mgmt_remove_attribute (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		rpc_c_attr_codesets,
		&status );

		CHECK_STATUS2(TRUE, "rpc_ns_mgmt_remove_attribute", status, expected);

		rpc_ns_binding_unexport (
			rpc_c_ns_syntax_default,
			(unsigned_char_p_t)nsi_entry_name,
			cs_test_v1_0_s_ifspec,
			(uuid_vector_p_t)NULL,
			&status );

		CHECK_STATUS2(TRUE, "rpc_ns_binding_unexport", status, expected);

		rpc_ep_unregister (
			cs_test_v1_0_s_ifspec,
			binding_vector,
			NULL,
			&status );

		CHECK_STATUS2(TRUE, "rpc_ep_unregister", status, expected);
		
		rpc_binding_vector_free (
			&binding_vector,
			&status );

		CHECK_STATUS2(TRUE, "rpc_binding_vector_free", status, expected);

		
		rpc_server_unregister_if (
			cs_test_v1_0_s_ifspec,
			NULL,
			&status );

		CHECK_STATUS2(TRUE, "rpc_server_unregister_if", status, expected);

		(void)pthread_mutex_destroy(&mutex);
	}
	CATCH_ALL
	{
		/* When there is a signal, clear up the name space */

		rpc_ns_mgmt_remove_attribute (
		rpc_c_ns_syntax_default,
		(unsigned_char_p_t)nsi_entry_name,
		rpc_c_attr_codesets,
		&status );

		CHECK_STATUS2(TRUE, "rpc_ns_mgmt_remove_attribute", status, expected);

		rpc_ns_binding_unexport (
			rpc_c_ns_syntax_default,
			(unsigned_char_p_t)nsi_entry_name,
			cs_test_v1_0_s_ifspec,
			(uuid_vector_p_t)NULL,
			&status );

		CHECK_STATUS2(TRUE, "rpc_ns_binding_unexport", status, expected);

		rpc_ep_unregister (
			cs_test_v1_0_s_ifspec,
			binding_vector,
			NULL,
			&status );

		CHECK_STATUS2(TRUE, "rpc_ep_unregister", status, expected);
		
		rpc_binding_vector_free (
			&binding_vector,
			&status );

		CHECK_STATUS2(TRUE, "rpc_binding_vector_free", status, expected);

		
		rpc_server_unregister_if (
			cs_test_v1_0_s_ifspec,
			NULL,
			&status );

		CHECK_STATUS2(TRUE, "rpc_server_unregister_if", status, expected);

		(void)pthread_mutex_destroy(&mutex);
	}
	ENDTRY;
}
